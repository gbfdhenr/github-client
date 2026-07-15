# GitHub Client

中文文档 / Chinese Documentation

GitHub Client 是一个用 C++17 和 Qt5 构建的 GitHub 桌面客户端。主页面通过 QWebEngineView 渲染真实的 GitHub 首页，静态资源全部嵌入本地，API 数据实时从网络获取。

## 核心架构

- **QWebEngineView 渲染首页**：加载经过处理的 `homepage.html`，base URL 设为 `https://github.com/`
- **自定义 `local://` URL Scheme**：所有 CSS/JS/图片/字体通过 `local://` 协议从 Qt 资源系统读取
- **静态资源全量嵌入**：110 个文件（CSS/JS/图片/字体/图标）编译进二进制，总计 10.8 MB
- **API 走网络**：fetch/XHR 请求正常发往 github.com，仅静态资源走本地

## 资源映射规则

| 页面引用 | local:// 映射 | Qt 资源路径 |
|----------|---------------|-------------|
| `local://assets/xxx.css` | `qrc:/github_files/xxx.css` | `resources/github_files/` |
| `local://fonts/xxx.woff2` | `qrc:/fonts/xxx.woff2` | `resources/fonts/` |
| `local://favicons/xxx.svg` | `qrc:/favicons/xxx.svg` | `resources/favicons/` |

## 系统要求

- **操作系统**：Windows 10/11 或 Linux（Debian/Ubuntu）
- **编译器**：GCC 8+ / Clang 10+ / MSVC 2019+，C++17
- **CMake**：>= 3.16
- **Qt 5.15+**：Widgets、Network、Sql、WebEngineWidgets 模块
- **其他依赖**：OpenSSL、SQLite3

## 快速开始

### 安装依赖（Debian/Ubuntu）

```shell
apt-get install -y qtbase5-dev qtwebengine5-dev libqt5sql5-sqlite libqt5webenginewidgets5 libssl-dev libsqlite3-dev cmake build-essential
```

### 编译

```shell
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 运行

```shell
./build/GitHubClient
```

## 项目结构

```
GitHubClient/
├── CMakeLists.txt              # 构建配置
├── main.cpp                    # 入口，QApplication 前注册 local scheme
├── gui/
│   ├── github_ui.h/cpp         # 主窗口，QWebEngineView 首页
│   └── local_asset_handler.h   # local:// scheme 处理器
├── api/
│   └── github_api.h/cpp        # GitHub REST API 异步客户端
├── i18n/
│   └── translations.h/cpp      # 中英文翻译
├── utils/
│   ├── browser_cookies.h/cpp   # 浏览器 Cookie 提取解密
│   └── test_cookie_extractor.cpp
├── resources/
│   ├── homepage.html           # 处理后的 GitHub 首页
│   ├── resources.qrc           # 资源清单
│   ├── github_files/           # 105 个 CSS/JS/图片
│   ├── fonts/                  # 3 个 woff2 字体
│   └── favicons/               # 2 个图标文件
└── packaging/
    └── GitHubClient.wxs        # WiX v3 Windows 安装包定义
```

## 浏览器 Cookie 导入

从本地浏览器自动提取 GitHub 登录态，支持 Chrome / Edge / Firefox。

| 浏览器 | Windows | Linux | 实现方式 |
|--------|---------|-------|----------|
| Chrome | ✅ | ✅ | SQLite3 + OpenSSL AES-GCM 解密 |
| Edge | ✅ | ✅ | 同上 |
| Firefox | ✅ | ✅ | 直接读取 SQLite |

安全机制：Cookie 仅在内存中使用，应用关闭后即释放。

## CI/CD 构建

GitHub Actions 自动化打包，每次 push 触发：

- **Windows**：cmake + nmake → WiX v3 heat/candle/light → `.msi`
- **Linux**：cmake + make → `ldd` 提取依赖 → dpkg-deb → `.deb`

## 注意事项

- QWebEngineView 需要 `qtwebengine5-dev`，这是额外包，不在 `qtbase5-dev` 中
- `QWebEngineUrlScheme::registerScheme` 必须在 `QApplication` 构造之前调用
- 编译产物约 5.8 MB，已包含所有静态资源
- Token/Cookie 不会上传至第三方

## 许可证

MIT License
