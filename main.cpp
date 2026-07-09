#include <QApplication>
#include "gui/github_ui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    app.setOrganizationName("GitHubClient");
    app.setApplicationName("GitHub Client");
    app.setApplicationVersion("0.0.2");

    app.setStyleSheet(R"(
        QMainWindow {
            background-color: #ffffff;
        }
        QLabel {
            color: #24292f;
        }
        QTableWidget::item {
            padding: 8px;
        }
        QTreeWidget::item {
            padding: 6px;
        }
    )");

    MainWindow window;
    window.showLoginDialog();
    window.show();

    return app.exec();
}
