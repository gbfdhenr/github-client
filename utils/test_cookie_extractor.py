#!/usr/bin/env python3
"""
测试 Cookie 提取器是否正常工作
在 Debian/Linux 系统上测试 Edge Chrome Firefox
"""

import sys
import os

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from utils.browser_cookies import BrowserCookieExtractor, list_available_browsers


def main():
    print("=" * 60)
    print("Cookie 提取器测试工具")
    print("=" * 60)
    
    extractor = BrowserCookieExtractor()
    
    print(f"\n检测到的平台：{extractor.platform}")
    print(f"Debian/Ubuntu 系：{extractor._is_debian_based()}")
    
    # 列出可用浏览器
    print("\n可用的浏览器:")
    available = list_available_browsers()
    for browser in available:
        print(f"  ✓ {browser}")
    
    if not available:
        print("  未检测到任何浏览器 Cookie 文件")
        print("\n请确保：")
        print("  1. 至少安装了一个浏览器 (Chrome/Edge/Firefox)")
        print("  2. 浏览器已登录过 github.com")
        print("  3. 浏览器已完全关闭（不是最小化）")
    
    # 测试各浏览器
    print("\n" + "=" * 60)
    print("测试从各浏览器提取 Cookie:")
    print("=" * 60)
    
    browsers_to_test = ['edge', 'chrome', 'firefox']
    
    for browser in browsers_to_test:
        print(f"\n正在测试 {browser}...")
        
        try:
            if browser == 'edge':
                cookies = extractor.get_github_cookie_from_edge()
            elif browser == 'chrome':
                cookies = extractor.get_github_cookie_from_chrome()
            elif browser == 'firefox':
                cookies = extractor.get_github_cookie_from_firefox()
            else:
                cookies = None
            
            if cookies:
                cookie_names = list(cookies.keys())
                print(f"  ✅ 成功！找到 {len(cookie_names)} 个 Cookie")
                print(f"     Cookie 名称：{', '.join(cookie_names)}")
            else:
                print(f"  ⚠️  未找到 Cookie")
                print(f"     可能原因:")
                print(f"     - 浏览器未安装")
                print(f"     - 浏览器未完全关闭")
                print(f"     - 未登录 github.com")
        except Exception as e:
            print(f"  ❌ 失败：{e}")
    
    print("\n" + "=" * 60)
    print("测试完成")
    print("=" * 60)


if __name__ == '__main__':
    main()
