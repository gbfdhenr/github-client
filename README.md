# GitHub Client

GitHub 桌面客户端应用，原生 PyQt5 实现，模拟 GitHub 网站界面风格。

## 功能特性

- ✅ GitHub API 完整集成（所有数据来自 github.com）
- ✅ 支持 Token + Cookie 双重认证
- ✅ **从浏览器自动导入 Cookie**（Chrome、Edge、Firefox）
- ✅ **Debian/Ubuntu 系统完整支持**
- ✅ 中英文界面切换
- ✅ 仓库列表和详情查看
- ✅ 文件浏览器（支持查看文件内容）
- ✅ Issues 和 Pull Requests 查看
- ✅ 个人资料展示
- ✅ 原生 GUI，无需 WebView
- ✅ Windows + Linux 双平台支持

## 系统要求

- Python 3.8+
- Windows 10/11 或 Linux（Debian/Ubuntu/RHEL 等）
- 浏览器（Chrome/Edge/Firefox，用于 Cookie 导入）

## 安装依赖

### Debian/Ubuntu

```bash
cd GithubClient
pip3 install -r requirements.txt
```

### 如果权限不足

```bash
pip3 install --break-system-packages -r requirements.txt
```

### 手动安装

```bash
pip3 install PyQt5 requests markdown pycryptodomex keyring
```

## 运行方式

```bash
python main.py
```

## 使用说明

### 方式一：使用 Token 登录

1. 访问 https://github.com/settings/tokens
2. 点击 "Generate new token (classic)"
3. 选择权限（建议：`repo`, `user`, `read:org`）
4. 生成后复制 Token
5. 在应用中粘贴 Token 并点击登录

### 方式二：Token + 浏览器 Cookie

1. 点击 "从浏览器导入" 按钮
2. 选择你的浏览器（Chrome/Edge/Firefox）
3. 应用会自动提取 GitHub Cookie
4. 配合 Token 使用，可访问更多数据

### 方式三：纯 Cookie（部分功能可用）

某些情况下，仅需 Cookie 也可访问部分公开数据

## 项目结构

```
GithubClient/
├── main.py                 # 主入口
├── requirements.txt        # Python 依赖
├── README.md              # 说明文档
├── LICENSE                # MIT 许可证
├── gui/
│   └── github_ui.py       # PyQt5 GUI 实现
├── api/
│   └── github_api.py      # GitHub API 封装
├── i18n/
│   └── translations.py    # 中英文翻译
└── utils/
    └── browser_cookies.py # 浏览器 Cookie 提取工具
    └── test_cookie_extractor.py  # Cookie 提取测试工具
```

## 浏览器 Cookie 支持

### 支持的浏览器和平台

| 浏览器 | Windows | Linux (Debian/Ubuntu) | macOS |
|--------|---------|----------------------|-------|
| Google Chrome | ✅ | ✅ | ✅ |
| Microsoft Edge | ✅ | ✅ | ✅ |
| Mozilla Firefox | ✅ | ✅ | ✅ |

### Cookie 存储位置

**Windows:**
- Chrome: `%LOCALAPPDATA%\Google\Chrome\User Data\Default\Cookies`
- Edge: `%LOCALAPPDATA%\Microsoft\Edge\User Data\Default\Cookies`
- Firefox: `%APPDATA%\Mozilla\Firefox\Profiles\<profile>\cookies.sqlite`

**Linux (Debian/Ubuntu):**
- Chrome: `~/.config/google-chrome/Default/Cookies`
- Edge: `~/.config/microsoft-edge/Default/Cookies`
- Firefox: `~/.mozilla/firefox/<profile>/cookies.sqlite`

**macOS:**
- Chrome: `~/Library/Application Support/Google/Chrome/Default/Cookies`
- Edge: `~/Library/Application Support/Microsoft Edge/Default/Cookies`
- Firefox: `~/Library/Application Support/Firefox/Profiles/cookies.sqlite`

### Cookie 提取说明

- **Chrome/Edge**: 使用 SQLite 数据库，Cookie 可能需要 AES 解密
- **Firefox**: 直接读取 cookies.sqlite 数据库（未加密）
- **权限要求**: 需要关闭浏览器才能读取 Cookie 文件
- **安全性**: Cookie 仅在当前会话使用，关闭应用即清除
- **Debian 适配**: 自动检测 `/etc/debian_version` 和 `/etc/lsb-release`

### 测试 Cookie 提取

在 Debian 系统上测试 Cookie 提取是否正常工作：

```bash
python3 utils/test_cookie_extractor.py
```

## 界面预览

- **深色头部导航栏**：GitHub 经典配色
- **仓库卡片**：显示星标、Forks、Issues 数量
- **文件浏览器**：树形结构，双击查看文件
- **Issues/PR**：表格视图，实时数据
- **个人资料**：头像、简介、统计信息

## 故障排查

### 无法从浏览器导入 Cookie

- **确保浏览器已关闭**：浏览器进程必须完全退出
- **确认浏览器配置目录存在**：检查 Cookie 文件路径是否正确
- **检查文件读取权限**（Linux）：确保有读取 Cookie 文件的权限
- **尝试手动输入 Cookie**：使用浏览器开发者工具查看

### 测试 Cookie 提取

在 Debian/Ubuntu 系统上：

```bash
python3 utils/test_cookie_extractor.py
```

查看输出结果：
- ✅ 成功：显示找到的 Cookie 名称
- ⚠️ 未找到：检查是否安装浏览器并登录 GitHub
- ❌ 失败：查看错误信息

### Token 无效

- 检查 Token 是否已过期
- 确认 Token 权限是否正确
- 重新生成新的 Token

### 界面无响应

- 数据加载可能需要几秒，请等待
- 大量仓库时加载时间较长
- 检查网络连接

## 注意事项

1. Token 和 Cookie 都直接从本地浏览器获取，不会上传到任何第三方
2. 关闭应用后 Token 和 Cookie 会自动清除
3. 纯 Cookie 模式功能受限，建议配合 Token 使用
4. 部分浏览器 Cookie 需要关闭浏览器才能成功读取
5. Linux 系统需要确保有读取浏览器配置文件的权限
6. Debian/Ubuntu 系统已特别适配，自动检测系统特征文件

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。
