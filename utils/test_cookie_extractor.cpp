#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include "utils/browser_cookies.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QTextStream out(stdout);

    out << "============================================================" << Qt::endl;
    out << "Cookie 提取器测试工具" << Qt::endl;
    out << "============================================================" << Qt::endl;

    BrowserCookieExtractor extractor;

    out << "\n检测到的平台：" << extractor.platform() << Qt::endl;
    out << "Debian/Ubuntu 系：" << (extractor.isDebianBased() ? "是" : "否") << Qt::endl;

    out << "\n可用的浏览器:" << Qt::endl;
    QStringList available = extractor.listAvailableBrowsers();
    for (const QString &browser : available) {
        out << "  v " << browser << Qt::endl;
    }

    if (available.isEmpty()) {
        out << "  未检测到任何浏览器 Cookie 文件" << Qt::endl;
        out << "\n请确保：" << Qt::endl;
        out << "  1. 至少安装了一个浏览器 (Chrome/Edge/Firefox)" << Qt::endl;
        out << "  2. 浏览器已登录过 github.com" << Qt::endl;
        out << "  3. 浏览器已完全关闭（不是最小化）" << Qt::endl;
    }

    out << "\n============================================================" << Qt::endl;
    out << "测试从各浏览器提取 Cookie:" << Qt::endl;
    out << "============================================================" << Qt::endl;

    QStringList browsersToTest = {"edge", "chrome", "firefox"};

    for (const QString &browser : browsersToTest) {
        out << "\n正在测试 " << browser << "..." << Qt::endl;

        QMap<QString, QString> cookies;
        if (browser == "edge")
            cookies = extractor.getGithubCookieFromEdge();
        else if (browser == "chrome")
            cookies = extractor.getGithubCookieFromChrome();
        else if (browser == "firefox")
            cookies = extractor.getGithubCookieFromFirefox();

        if (!cookies.isEmpty()) {
            QStringList names = cookies.keys();
            out << "  OK 成功！找到 " << names.size() << " 个 Cookie" << Qt::endl;
            out << "     Cookie 名称：" << names.join(", ") << Qt::endl;
        } else {
            out << "  WARNING 未找到 Cookie" << Qt::endl;
            out << "     可能原因:" << Qt::endl;
            out << "     - 浏览器未安装" << Qt::endl;
            out << "     - 浏览器未完全关闭" << Qt::endl;
            out << "     - 未登录 github.com" << Qt::endl;
        }
    }

    out << "\n============================================================" << Qt::endl;
    out << "测试完成" << Qt::endl;
    out << "============================================================" << Qt::endl;

    return 0;
}
