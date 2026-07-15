#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QScrollArea>
#include <QStackedWidget>
#include <QComboBox>
#include <QFrame>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPropertyAnimation>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QProgressBar>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QDesktopServices>
#include <QUrl>
#include <functional>

#include "api/github_api.h"
#include "i18n/translations.h"

namespace GitHubColors {
    constexpr const char *HEADER_BG = "#0d1117";
    constexpr const char *HEADER_TEXT = "#e6edf3";
    constexpr const char *HEADER_TEXT_MUTED = "#8b949e";
    constexpr const char *SIDEBAR_BG = "#0d1117";
    constexpr const char *MAIN_BG = "#0d1117";
    constexpr const char *CARD_BG = "#161b22";
    constexpr const char *BORDER = "#30363d";
    constexpr const char *BUTTON_PRIMARY = "#238636";
    constexpr const char *BUTTON_PRIMARY_HOVER = "#2ea043";
    constexpr const char *LINK = "#58a6ff";
    constexpr const char *TEXT_PRIMARY = "#e6edf3";
    constexpr const char *TEXT_SECONDARY = "#8b949e";
    constexpr const char *SUCCESS = "#3fb950";
    constexpr const char *DANGER = "#f85149";
    constexpr const char *WARNING = "#d29922";
    constexpr const char *PURPLE = "#a371f7";
}

class GitHubWebPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit GitHubWebPage(QObject *parent = nullptr) : QWebEnginePage(parent) {}
signals:
    void signInDetected();
    void signUpDetected();
protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override {
        if (type != NavigationTypeLinkClicked) return true;
        QString host = url.host();
        if (host == "github.com" || host == "www.github.com") {
            if (url.path().startsWith("/login")) { emit signInDetected(); return false; }
            if (url.path().startsWith("/signup")) { emit signUpDetected(); return false; }
            load(url);
            return false;
        }
        QDesktopServices::openUrl(url);
        return false;
    }
};

class SpinnerWidget : public QWidget {
    Q_OBJECT
    Q_PROPERTY(int angle READ angle WRITE setAngle)
public:
    explicit SpinnerWidget(QWidget *parent = nullptr, int size = 40);
    void start();
    void stop();
    int angle() const { return m_angle; }
    void setAngle(int a) { m_angle = a; update(); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int m_angle;
    int m_size;
    QTimer *m_timer;
};

class LoadingLabel : public QWidget {
    Q_OBJECT
public:
    explicit LoadingLabel(const QString &text = "", QWidget *parent = nullptr);
    void setLoadingText(const QString &text);

private:
    SpinnerWidget *m_spinner;
    QLabel *m_label;
};

class ClickableFrame : public QFrame {
    Q_OBJECT
public:
    explicit ClickableFrame(QWidget *parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *) override { emit clicked(); }
};

class NavButton : public QPushButton {
    Q_OBJECT
public:
    explicit NavButton(const QString &text, QWidget *parent = nullptr);
};

class HomePage : public QWidget {
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
signals:
    void signInClicked();
    void signUpClicked();
    void searchClicked();

private:
    void setupUi();
};

class SignInPage;

class BrowserCookieDialog : public QDialog {
    Q_OBJECT
public:
    explicit BrowserCookieDialog(QWidget *parent = nullptr);
    QMap<QString, QString> selectedCookies() const { return m_selectedCookies; }
private:
    QLabel *m_statusLabel;
    QMap<QString, QString> m_selectedCookies;
    void onBrowserSelected(const QString &browser);
};

class SignInPage : public QWidget {
    Q_OBJECT
public:
    explicit SignInPage(QWidget *parent = nullptr);
    QString getToken() const;
    QString getCookie() const;
    QString getLanguage() const;
    void setStatus(const QString &msg, bool error = false);

signals:
    void signInRequested();
    void backRequested();

private:
    QLineEdit *m_tokenInput;
    QLineEdit *m_cookieInput;
    QComboBox *m_langCombo;
    QLabel *m_statusLabel;

    void setupUi();
    void importFromBrowser();
};

class RepoCard : public ClickableFrame {
    Q_OBJECT
public:
    explicit RepoCard(const QJsonObject &repoData, QWidget *parent = nullptr);
signals:
    void clicked(const QJsonObject &repo);
private:
    QJsonObject m_repoData;
    void setupUi();
};

class RepoFilesView : public QWidget {
    Q_OBJECT
public:
    explicit RepoFilesView(GitHubAPI *api, const QString &owner, const QString &repo,
                           QWidget *parent = nullptr);
private:
    GitHubAPI *m_api;
    QString m_owner, m_repo, m_currentPath, m_branch;
    QLabel *m_pathLabel;
    QTreeWidget *m_fileTree;
    LoadingLabel *m_loading;

    void setupUi();
    void loadFiles();
    void onFilesLoaded(const QJsonArray &contents);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void showFileContent(const QJsonObject &fileData);
    void onError(const QString &error);
    static QString formatSize(int size);
};

class RepoIssuesView : public QWidget {
    Q_OBJECT
public:
    explicit RepoIssuesView(GitHubAPI *api, const QString &owner, const QString &repo,
                            QWidget *parent = nullptr);
private:
    GitHubAPI *m_api;
    QString m_owner, m_repo;
    QStackedWidget *m_stack;
    LoadingLabel *m_loading;
    QTableWidget *m_table;
    void setupUi();
    void loadIssues();
    void onIssuesLoaded(const QJsonArray &issues);
    void onError(const QString &error);
};

class RepoPRsView : public QWidget {
    Q_OBJECT
public:
    explicit RepoPRsView(GitHubAPI *api, const QString &owner, const QString &repo,
                         QWidget *parent = nullptr);
private:
    GitHubAPI *m_api;
    QString m_owner, m_repo;
    QStackedWidget *m_stack;
    LoadingLabel *m_loading;
    QTableWidget *m_table;
    void setupUi();
    void loadPRs();
    void onPRsLoaded(const QJsonArray &prs);
    void onError(const QString &error);
};

class RepositoryDetailView : public QWidget {
    Q_OBJECT
public:
    explicit RepositoryDetailView(GitHubAPI *api, const QJsonObject &repoData,
                                   QWidget *parent = nullptr);
signals:
    void backRequested();
private:
    GitHubAPI *m_api;
    QJsonObject m_repoData;
    void setupUi();
};

class RepositoriesView : public QWidget {
    Q_OBJECT
public:
    explicit RepositoriesView(GitHubAPI *api, QWidget *parent = nullptr);
signals:
    void repoClicked(const QJsonObject &repo);
private:
    GitHubAPI *m_api;
    QStackedWidget *m_stack;
    LoadingLabel *m_loading;
    QScrollArea *m_scroll;
    QVBoxLayout *m_cardsLayout;
    void setupUi();
    void loadRepositories();
    void onReposLoaded(const QJsonArray &repos);
    void onError(const QString &error);
};

class ProfileView : public QWidget {
    Q_OBJECT
public:
    explicit ProfileView(GitHubAPI *api, const QJsonObject &userData, QWidget *parent = nullptr);
private:
    GitHubAPI *m_api;
    QJsonObject m_userData;
    void setupUi();
};

class DashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit DashboardPage(GitHubAPI *api, const QJsonObject &user, QWidget *parent = nullptr);
    void openRepositoryDetail(const QJsonObject &repo);
signals:
    void repoClicked(const QJsonObject &repo);
    void logoutRequested();
private:
    GitHubAPI *m_api;
    QJsonObject m_user;
    QStackedWidget *m_contentStack;
    QVector<QWidget *> m_repoStack;
    void setupUi();
    void goBack();
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    GitHubAPI *m_api;
    QJsonObject m_user;
    QStackedWidget *m_pageStack;
    HomePage *m_homePage;
    SignInPage *m_signInPage;
    DashboardPage *m_dashboardPage;
    QWidget *m_headerLoggedOut;
    QWidget *m_headerLoggedIn;
    QLabel *m_headerUserLabel;

    void setupUi();
    void goToHome();
    void goToSignIn();
    void goToDashboard();
    void doLogin();
    void doLogout();
    void openRepoDetail(const QJsonObject &repo);
    QWidget* createHeaderLoggedOut();
    QWidget* createHeaderLoggedIn();
};
