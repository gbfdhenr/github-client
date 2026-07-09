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
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <functional>

#include "api/github_api.h"
#include "i18n/translations.h"

namespace GitHubColors {
    constexpr const char *HEADER_BG = "#24292f";
    constexpr const char *HEADER_TEXT = "#ffffff";
    constexpr const char *SIDEBAR_BG = "#f6f8fa";
    constexpr const char *MAIN_BG = "#ffffff";
    constexpr const char *BORDER = "#d0d7de";
    constexpr const char *BUTTON_PRIMARY = "#2da44e";
    constexpr const char *BUTTON_PRIMARY_HOVER = "#2c974b";
    constexpr const char *BUTTON_SECONDARY = "#f3f4f6";
    constexpr const char *LINK = "#0969da";
    constexpr const char *TEXT_PRIMARY = "#24292f";
    constexpr const char *TEXT_SECONDARY = "#57606a";
    constexpr const char *SUCCESS = "#1a7f37";
    constexpr const char *DANGER = "#cf222e";
    constexpr const char *WARNING = "#9a6700";
}

class AvatarLabel : public QLabel {
    Q_OBJECT
public:
    explicit AvatarLabel(QWidget *parent = nullptr);
    void loadImage(const QString &url);

private:
    QNetworkAccessManager *m_networkManager;

private slots:
    void onImageLoaded(QNetworkReply *reply);
};

class BrowserCookieDialog : public QDialog {
    Q_OBJECT
public:
    explicit BrowserCookieDialog(QWidget *parent = nullptr);
    QMap<QString, QString> selectedCookies() const { return m_selectedCookies; }

private:
    QLabel *m_statusLabel;
    QMap<QString, QString> m_selectedCookies;

    void setupUi();
    void onBrowserSelected(const QString &browser);
};

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getToken() const;
    QString getCookie() const;
    QString getLanguage() const;

private:
    QLineEdit *m_tokenInput;
    QLineEdit *m_cookieInput;
    QComboBox *m_langCombo;
    QLabel *m_statusLabel;

    void setupUi();
    void importFromBrowser();
};

class RepoCard : public QFrame {
    Q_OBJECT
public:
    explicit RepoCard(const QJsonObject &repoData, std::function<void(const QJsonObject &)> onClick = nullptr);

private:
    QJsonObject m_repoData;
    std::function<void(const QJsonObject &)> m_onClick;

    void setupUi();
};

class RepoFilesView : public QWidget {
    Q_OBJECT
public:
    explicit RepoFilesView(GitHubAPI *api, const QString &owner, const QString &repo,
                           QWidget *parent = nullptr);

private:
    GitHubAPI *m_api;
    QString m_owner;
    QString m_repo;
    QString m_currentPath;
    QString m_branch;
    QLabel *m_pathLabel;
    QTreeWidget *m_fileTree;

    void setupUi();
    void loadFiles();
    void onFilesLoaded(const QJsonArray &contents);
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void showFileContent(const QJsonObject &fileData);
    void onError(const QString &error);

    static QString formatSize(int size);
};

class RepoIssuesView : public QTableWidget {
    Q_OBJECT
public:
    explicit RepoIssuesView(GitHubAPI *api, const QString &owner, const QString &repo,
                            QWidget *parent = nullptr);

private:
    GitHubAPI *m_api;
    QString m_owner;
    QString m_repo;

    void setupUi();
    void loadIssues();
    void onIssuesLoaded(const QJsonArray &issues);
    void onError(const QString &error);
};

class RepoPRsView : public QTableWidget {
    Q_OBJECT
public:
    explicit RepoPRsView(GitHubAPI *api, const QString &owner, const QString &repo,
                         QWidget *parent = nullptr);

private:
    GitHubAPI *m_api;
    QString m_owner;
    QString m_repo;

    void setupUi();
    void loadPRs();
    void onPRsLoaded(const QJsonArray &prs);
    void onError(const QString &error);
};

class RepositoryDetailView : public QWidget {
    Q_OBJECT
public:
    explicit RepositoryDetailView(GitHubAPI *api, const QJsonObject &repoData,
                                   std::function<void()> onBack = nullptr,
                                   QWidget *parent = nullptr);

private:
    GitHubAPI *m_api;
    QJsonObject m_repoData;
    std::function<void()> m_onBack;

    void setupUi();
    void goBack();
};

class RepositoriesView : public QWidget {
    Q_OBJECT
public:
    explicit RepositoriesView(GitHubAPI *api, QWidget *parent = nullptr);

signals:
    void repoClicked(const QJsonObject &repo);

private:
    GitHubAPI *m_api;
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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showLoginDialog();

private:
    GitHubAPI *m_api;
    QJsonObject m_user;
    QStackedWidget *m_contentStack;
    QComboBox *m_langCombo;
    QLabel *m_userLabel;
    QPushButton *m_logoutButton;
    QVector<QWidget *> m_repoStack;

    void setupUi();
    void setupHeader(QVBoxLayout *layout);
    bool login(const QString &token, const QString &cookie, const QString &lang);
    void logout();
    void openRepositoryDetail(const QJsonObject &repo);
    void goBack();
};
