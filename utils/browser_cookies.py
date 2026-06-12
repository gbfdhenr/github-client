"""
从主流浏览器获取 GitHub Cookie 的工具模块
支持：Chrome、Edge、Firefox
平台：Windows、Linux (Debian/Ubuntu)、macOS
"""

import os
import sys
import sqlite3
import json
import hashlib
from typing import Optional, Dict, List
from pathlib import Path


class BrowserCookieExtractor:
    """浏览器 Cookie 提取器"""
    
    # 各浏览器在不同平台的 Cookie 路径
    BROWSER_PATHS = {
        'chrome': {
            'win': r'%LOCALAPPDATA%\Google\Chrome\User Data\Default\Cookies',
            'linux': '~/.config/google-chrome/Default/Cookies',
            'debian': '~/.config/google-chrome/Default/Cookies',
            'mac': '~/Library/Application Support/Google/Chrome/Default/Cookies'
        },
        'edge': {
            'win': r'%LOCALAPPDATA%\Microsoft\Edge\User Data\Default\Cookies',
            'linux': '~/.config/microsoft-edge/Default/Cookies',
            'debian': '~/.config/microsoft-edge/Default/Cookies',
            'mac': '~/Library/Application Support/Microsoft Edge/Default/Cookies'
        },
        'firefox': {
            'win': r'%APPDATA%\Mozilla\Firefox\Profiles\\',
            'linux': '~/.mozilla/firefox/',
            'debian': '~/.mozilla/firefox/',
            'mac': '~/Library/Application Support/Firefox/Profiles/'
        }
    }
    
    # Linux 下检测 Debian/Ubuntu 的特征文件
    DEBIAN_MARKER = '/etc/debian_version'
    UBUNTU_MARKER = '/etc/lsb-release'
    
    def __init__(self):
        self.platform = self._detect_platform()
        self.is_debian = self._is_debian_based()
    
    def _detect_platform(self) -> str:
        """检测操作系统平台"""
        if os.name == 'nt':
            return 'win'
        elif os.name == 'posix':
            if sys.platform == 'darwin':
                return 'mac'
            return 'linux'
        return 'linux'
    
    def _is_debian_based(self) -> bool:
        """检测是否为 Debian/Ubuntu 系系统"""
        return os.path.exists(self.DEBIAN_MARKER) or os.path.exists(self.UBUNTU_MARKER)
    
    def _get_browser_path(self, browser: str) -> Optional[str]:
        """获取浏览器 Cookie 文件路径"""
        paths = self.BROWSER_PATHS.get(browser, {})
        
        # 优先尝试平台特定路径
        path_key = self.platform
        path = paths.get(path_key)
        
        if path:
            path = os.path.expandvars(os.path.expanduser(path))
            if os.path.exists(path):
                return path
        
        # Linux 下尝试 Debian 特定路径
        if self.platform == 'linux':
            debian_path = paths.get('debian')
            if debian_path:
                debian_path = os.path.expandvars(os.path.expanduser(debian_path))
                if os.path.exists(debian_path):
                    return debian_path
        
        # 兜底：尝试 linux 路径
        linux_path = paths.get('linux')
        if linux_path:
            linux_path = os.path.expandvars(os.path.expanduser(linux_path))
            if os.path.exists(linux_path):
                return linux_path
        
        return None
    
    def get_github_cookie_from_chrome(self) -> Optional[Dict]:
        """从 Chrome 获取 GitHub Cookie"""
        return self._extract_chromium_cookie('chrome')
    
    def get_github_cookie_from_edge(self) -> Optional[Dict]:
        """从 Edge 获取 GitHub Cookie（支持 Debian/Ubuntu）"""
        return self._extract_chromium_cookie('edge')
    
    def get_github_cookie_from_firefox(self) -> Optional[Dict]:
        """从 Firefox 获取 GitHub Cookie"""
        return self._extract_firefox_cookie()
    
    def _extract_chromium_cookie(self, browser: str, domain: str = 'github.com') -> Optional[Dict]:
        """
        从 Chromium 系浏览器（Chrome/Edge）提取 Cookie
        支持 Windows、Linux (Debian/Ubuntu)、macOS
        """
        cookie_path = self._get_browser_path(browser)
        
        if not cookie_path:
            return None
        
        if not os.path.exists(cookie_path):
            return None
        
        # 检查文件是否可读
        if not os.access(cookie_path, os.R_OK):
            return None
        
        cookies = {}
        conn = None
        
        try:
            # 复制数据库到临时位置（避免锁定问题）
            import tempfile
            import shutil
            
            temp_db = tempfile.NamedTemporaryFile(delete=False, suffix='.db')
            temp_path = temp_db.name
            temp_db.close()
            
            # 复制 Cookie 数据库
            shutil.copy2(cookie_path, temp_path)
            
            # 连接临时数据库
            conn = sqlite3.connect(temp_path)
            cursor = conn.cursor()
            
            # 查询 GitHub 相关 Cookie
            cursor.execute(
                'SELECT name, encrypted_value FROM cookies WHERE host_key LIKE ?',
                (f'%{domain}%',)
            )
            
            for name, encrypted_value in cursor.fetchall():
                if encrypted_value:
                    try:
                        decrypted = self._decrypt_chromium_value(encrypted_value, browser)
                        if decrypted:
                            cookies[name] = decrypted
                    except Exception:
                        # 解密失败，尝试直接使用原始值
                        try:
                            cookies[name] = encrypted_value.decode('utf-8', errors='ignore')
                        except Exception:
                            continue
            
            if conn:
                conn.close()
            conn = None
            
            # 清理临时文件
            os.unlink(temp_path)
            
        except sqlite3.Error:
            if conn:
                try:
                    conn.close()
                except Exception:
                    pass
            return None
        except Exception:
            return None
        finally:
            if conn:
                try:
                    conn.close()
                except Exception:
                    pass
        
        return cookies if cookies else None
    
    def _decrypt_chromium_value(self, encrypted_value: bytes, browser: str) -> Optional[str]:
        """
        解密 Chromium Cookie 值
        Windows: 使用 DPAPI
        Linux/macOS: 使用 AES 加密
        """
        try:
            # v10/v11 开头表示 AES 加密
            if isinstance(encrypted_value, bytes) and (encrypted_value.startswith(b'v10') or encrypted_value.startswith(b'v11')):
                return self._decrypt_aes_cookie(encrypted_value, browser)
            
            # 已解密的字符串
            if isinstance(encrypted_value, str):
                return encrypted_value
            
            # 尝试直接解码
            return encrypted_value.decode('utf-8', errors='ignore')
        except Exception:
            return None
    
    def _decrypt_aes_cookie(self, encrypted_value: bytes, browser: str) -> Optional[str]:
        """
        使用 AES 解密 Cookie
        Linux/macOS: 从密钥环获取密码
        """
        try:
            # 提取加密数据部分
            nonce = encrypted_value[3:15]  # 12 字节
            ciphertext = encrypted_value[15:-16]  # 密文
            tag = encrypted_value[-16:]  # 16 字节认证标签
            
            # 获取密钥
            key = self._get_linux_key(browser)
            if not key:
                return None
            
            # AES GCM 解密
            from Crypto.Cipher import AES
            cipher = AES.new(key, AES.MODE_GCM, nonce=nonce)
            decrypted = cipher.decrypt_and_verify(ciphertext, tag)
            
            return decrypted.decode('utf-8')
        except Exception:
            return None
    
    def _get_linux_key(self, browser: str) -> Optional[bytes]:
        """
        获取 Linux 下的解密密钥
        从密钥环或配置文件读取
        """
        key = None
        
        # 尝试从密钥环获取
        try:
            import keyring
            service_name = f"{browser.capitalize()} Safe Storage"
            key = keyring.get_password(service_name, service_name)
            
            if key:
                return self._derive_key(key)
        except Exception:
            pass
        
        # 备用：尝试从本地状态文件读取
        local_state_path = self._get_local_state_path(browser)
        if local_state_path and os.path.exists(local_state_path):
            try:
                with open(local_state_path, 'r', encoding='utf-8') as f:
                    local_state = json.load(f)
                
                key = local_state.get('os_crypt', {}).get('encrypted_key')
                if key:
                    import base64
                    key = base64.b64decode(key)
                    if key.startswith(b'DPAPI'):
                        key = key[5:]
                    return None
            except Exception:
                pass
        
        # 兜底：使用默认密钥
        return self._derive_key(browser + ' Safe Storage')
    
    def _derive_key(self, password: str) -> bytes:
        """派生 AES 密钥"""
        return hashlib.pbkdf2_hmac('sha1', password.encode('utf-8'), b'saltysalt', 1, 16)
    
    def _get_local_state_path(self, browser: str) -> Optional[str]:
        """获取浏览器的 Local State 文件路径"""
        browser_base_paths = {
            'chrome': '~/.config/google-chrome/',
            'edge': '~/.config/microsoft-edge/',
            'chromium': '~/.config/chromium/'
        }
        
        base_path = browser_base_paths.get(browser)
        if base_path:
            return os.path.expanduser(os.path.join(base_path, 'Local State'))
        
        return None
    
    def _extract_firefox_cookie(self, domain: str = 'github.com') -> Optional[Dict]:
        """
        从 Firefox 提取 Cookie
        Firefox Cookie 未加密，直接读取 SQLite
        """
        profiles_path = self._get_browser_path('firefox')
        
        if not profiles_path or not os.path.exists(profiles_path):
            return None
        
        # 查找 profiles.ini
        profiles_ini = os.path.join(os.path.dirname(profiles_path.rstrip('/')), 'profiles.ini')
        profiles = []
        
        if os.path.exists(profiles_ini):
            profiles = self._parse_firefox_profiles(profiles_ini, profiles_path)
        else:
            # 直接扫描目录
            try:
                for item in os.listdir(profiles_path):
                    if item.endswith('.default') or item.endswith('.default-release'):
                        profile_path = os.path.join(profiles_path, item)
                        if os.path.isdir(profile_path):
                            profiles.append(profile_path)
            except Exception:
                pass
        
        # 尝试每个 profile 的 Cookie 数据库
        for profile in profiles:
            cookies_db = os.path.join(profile, 'cookies.sqlite')
            if os.path.exists(cookies_db):
                cookies = self._read_firefox_cookies(cookies_db, domain)
                if cookies:
                    return cookies
        
        return None
    
    def _parse_firefox_profiles(self, profiles_ini: str, base_path: str) -> List[str]:
        """解析 Firefox profiles.ini 文件"""
        profiles = []
        
        try:
            import configparser
            config = configparser.ConfigParser()
            config.read(profiles_ini, encoding='utf-8')
            
            for section in config.sections():
                if section.startswith('Profile'):
                    path = config.get(section, 'Path', fallback=None)
                    if path:
                        is_relative = config.getboolean(section, 'IsRelative', fallback=False)
                        if is_relative:
                            full_path = os.path.join(base_path, path)
                        else:
                            full_path = path
                        
                        if os.path.isdir(full_path):
                            profiles.append(full_path)
        except Exception:
            pass
        
        return profiles
    
    def _read_firefox_cookies(self, db_path: str, domain: str) -> Optional[Dict]:
        """读取 Firefox Cookie 数据库"""
        cookies = {}
        conn = None
        
        try:
            import tempfile
            import shutil
            
            temp_db = tempfile.NamedTemporaryFile(delete=False, suffix='.sqlite')
            temp_path = temp_db.name
            temp_db.close()
            
            shutil.copy2(db_path, temp_path)
            
            conn = sqlite3.connect(temp_path)
            cursor = conn.cursor()
            
            cursor.execute(
                'SELECT name, value FROM moz_cookies WHERE host LIKE ?',
                (f'%{domain}%',)
            )
            
            for name, value in cursor.fetchall():
                cookies[name] = value
            
            if conn:
                conn.close()
            conn = None
            os.unlink(temp_path)
            
        except Exception:
            if conn:
                try:
                    conn.close()
                except Exception:
                    pass
            return None
        
        return cookies if cookies else None
    
    def get_all_github_cookies(self) -> Dict[str, Optional[Dict]]:
        """获取所有浏览器的 GitHub Cookie"""
        results = {}
        
        chrome = self.get_github_cookie_from_chrome()
        if chrome:
            results['chrome'] = chrome
        
        edge = self.get_github_cookie_from_edge()
        if edge:
            results['edge'] = edge
        
        firefox = self.get_github_cookie_from_firefox()
        if firefox:
            results['firefox'] = firefox
        
        return results
    
    def list_available_browsers(self) -> List[str]:
        """列出当前系统上可用的浏览器"""
        available = []
        
        for browser in ['chrome', 'edge', 'firefox']:
            path = self._get_browser_path(browser)
            if path and os.path.exists(path):
                available.append(browser)
        
        return available
    
    def format_cookie_string(self, cookies: Dict[str, str]) -> str:
        """将 Cookie 字典格式化为字符串"""
        return '; '.join(f'{k}={v}' for k, v in cookies.items())


# 便捷函数

def get_github_cookie_from_browser(browser: str = 'auto') -> Optional[Dict]:
    """从指定浏览器获取 GitHub Cookie"""
    extractor = BrowserCookieExtractor()
    
    if browser == 'auto':
        all_cookies = extractor.get_all_github_cookies()
        if all_cookies:
            return list(all_cookies.values())[0]
    else:
        method_map = {
            'chrome': extractor.get_github_cookie_from_chrome,
            'edge': extractor.get_github_cookie_from_edge,
            'firefox': extractor.get_github_cookie_from_firefox
        }
        
        if browser in method_map:
            return method_map[browser]()
    
    return None


def get_github_cookie_string(browser: str = 'auto') -> Optional[str]:
    """获取 GitHub Cookie 字符串"""
    extractor = BrowserCookieExtractor()
    cookies = get_github_cookie_from_browser(browser)
    
    if cookies:
        return extractor.format_cookie_string(cookies)
    
    return None


def list_available_browsers() -> List[str]:
    """返回当前系统上可用的浏览器列表"""
    extractor = BrowserCookieExtractor()
    return extractor.list_available_browsers()
