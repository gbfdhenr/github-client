#include <QApplication>
#include <QWebEngineUrlScheme>
#include <QWebEngineProfile>
#include "gui/github_ui.h"
#include "gui/local_asset_handler.h"

int main(int argc, char *argv[])
{
    QWebEngineUrlScheme localScheme("local");
    localScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);
    localScheme.setDefaultPort(QWebEngineUrlScheme::PortUnspecified);
    localScheme.setFlags(QWebEngineUrlScheme::SecureScheme
                       | QWebEngineUrlScheme::LocalScheme
                       | QWebEngineUrlScheme::LocalAccessAllowed
                       | QWebEngineUrlScheme::ContentSecurityPolicyIgnored);
    QWebEngineUrlScheme::registerScheme(localScheme);

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    app.setOrganizationName("GitHubClient");
    app.setApplicationName("GitHub Client");
    app.setApplicationVersion("0.0.3");

    auto *handler = new LocalAssetHandler(&app);
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler("local", handler);

    MainWindow window;
    window.show();

    return app.exec();
}
