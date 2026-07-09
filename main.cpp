#include <QApplication>
#include "gui/github_ui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    app.setOrganizationName("GitHubClient");
    app.setApplicationName("GitHub Client");
    app.setApplicationVersion("0.0.3");

    MainWindow window;
    window.show();

    return app.exec();
}
