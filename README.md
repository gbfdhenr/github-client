# GitHub Client

GitHub 桌面客户端应用，C++17/Qt5 原生实现。使用 QWebEngineView 渲染真实 GitHub 首页，所有静态资源本地化嵌入。

## 功能特性

- QWebEngineView 渲染 GitHub 真实首页，视觉像素级还原
- 110 个静态资源（CSS/JS/图片/字体）全部本地嵌入，离线可用
- 自定义 `local://` URL scheme，静态资源走本地，API 请求走网络
- 导航拦截：登录/注册 → 应用内处理，外部链接 → 系统浏览器
- GitHub API 集成（Token + Cookie 双重认证）
- 从浏览器自动导入 Cookie（Chrome、Edge、Firefox）
- 中英文界面切换
- 仓库列表、文件浏览、Issues/PR 查看、个人资料
- Windows MSI + Linux DEB 安装包 CI 构建

## 系统要求

- C++17 编译器（GCC 8+ / Clang 10+ / MSVC 2019+）
- CMake >= 3.16
- Qt 5.15+（Widgets / Network / Sql / **WebEngineWidgets**）
- OpenSSL 开发库
- SQLite3 开发库

## 安装依赖

### Debian/Ubuntu

```bash
apt-get install -y qtbase5-dev qtwebengine5-dev libqt5sql5-sqlite libqt5webenginewidgets5 libssl-dev libsqlite3-dev cmake build-essential
```

### Fedora/RHEL

```bash
dnf install -y qt5-qtbase-devel qt5-qtwebengine-devel openssl-devel sqlite-devel cmake gcc-c++
```

### Windows

安装 [Qt 5.15](https://www.qt.io/download) 和 [CMake](https://cmake.org/download/)，确保勾选 WebEngine 模块。

## 构建

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## 运行方式

```bash
./build/GitHubClient
```

## 测试 Cookie 提取

```bash
./build/test_cookie_extractor
```

## 使用说明

### 登录认证

1. 访问 https://github.com/settings/tokens
2. 生成 Token（推荐权限：`repo`, `user`, `read:org`）
3. 在应用中粘贴 Token 并登录
4. 可选：点击"从浏览器导入"自动提取 Cookie（Chrome/Edge/Firefox）

## 项目结构

```
GitHubClient/
├── CMakeLists.txt              # CMake 构建配置 (Qt5 + WebEngine + OpenSSL + SQLite3)
├── main.cpp                    # 主入口，注册 local:// scheme
├── gui/
│   ├── github_ui.h             # GUI 头文件 (GitHubWebPage, 首页/登录页等)
│   ├── github_ui.cpp           # QWebEngineView 首页渲染
│   └── local_asset_handler.h   # local:// 自定义 URL scheme 处理器
├── api/
│   ├── github_api.h            # GitHub API 异步 HTTP 客户端
│   └── github_api.cpp
├── i18n/
│   ├── translations.h          # 国际化
│   └── translations.cpp
├── utils/
│   ├── browser_cookies.h       # 浏览器 Cookie 提取
│   ├── browser_cookies.cpp     # SQLite3 + OpenSSL AES-GCM 解密
│   └── test_cookie_extractor.cpp
├── resources/
│   ├── homepage.html           # GitHub 首页，资源引用改写为 local://
│   ├── resources.qrc           # Qt 资源清单（110 个文件）
│   ├── github_files/           # 105 个 CSS/JS/图片
│   ├── fonts/                  # 3 个 woff2 字体（Mona Sans, Hubot Sans, Mona Sans Mono）
│   └── favicons/               # 2 个图标（PNG + SVG）
└── packaging/
    └── GitHubClient.wxs        # Windows WiX v3 安装包定义
```

## 浏览器 Cookie 支持

| 浏览器 | Windows | Linux (Debian/Ubuntu) | macOS |
|--------|---------|----------------------|-------|
| Google Chrome | ✅ | ✅ | ✅ |
| Microsoft Edge | ✅ | ✅ | ✅ |
| Mozilla Firefox | ✅ | ✅ | ✅ |

### 实现细节

- **Chrome/Edge**：SQLite3 读取加密 Cookie，OpenSSL `EVP_aes_128_gcm` 解密
- **Firefox**：直接读取 `cookies.sqlite`（未加密）
- **安全性**：Cookie 仅在当前会话使用，关闭应用即清除

## 静态资源离线化

110 个文件（10.8 MB）通过 Qt Resource System 编译进二进制，启动时注册 `local://` URL scheme 映射：

| 前缀 | 映射路径 |
|------|----------|
| `local://assets/` | `qrc:/github_files/` |
| `local://fonts/` | `qrc:/fonts/` |
| `local://favicons/` | `qrc:/favicons/` |

首页 HTML 以 `https://github.com/` 为 base URL 加载，API fetch/XHR 请求直接发往网络，静态资源走本地嵌入。

## CI/CD

GitHub Actions，每次 push 触发，支持异步取消：

- **Windows**：cmake + nmake + WiX v3 (heat/candle/light) → MSI 安装包
- **Linux**：cmake + make + dpkg-deb + `ldd` 依赖提取 → DEB 安装包

## 技术栈

| 模块 | 方案 |
|------|------|
| GUI | Qt5 Widgets + QWebEngineView |
| 资源嵌入 | Qt Resource System + 自定义 URL scheme |
| HTTP | QNetworkAccessManager (异步) |
| 数据库 | SQLite3 (C API) |
| 加密 | OpenSSL (AES-GCM, PBKDF2) |
| 构建 | CMake |
| 打包 | WiX v3 (Windows) / dpkg-deb (Linux) |

## 故障排查

### 无法从浏览器导入 Cookie

- 确保浏览器进程已完全退出
- Linux 下确认有读取 Cookie 文件权限
- 尝试手动输入 Token

### QWebEngineView 页面空白

- 确认 `qtwebengine5-dev` 已安装
- 检查 `QWebEngineUrlScheme::registerScheme` 在 QApplication 之前调用
- 查看终端输出的资源加载日志

### Token 无效

- 检查 Token 是否过期 / 权限正确
- 重新生成 Token

### 编译问题

- Qt5 开发包完整安装（含 `qtwebengine5-dev`）
- CMake >= 3.16
- OpenSSL 版本兼容

## 注意事项

1. Token 和 Cookie 仅在本地使用，不会上传到第三方
2. 关闭应用后认证信息自动清除
3. 静态资源嵌入二进制，无需网络即可渲染首页
4. API 数据实时从 github.com 获取

## 许可证

MIT 许可证。详见 [LICENSE](LICENSE)。
