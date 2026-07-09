#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <functional>

class GitHubAPI : public QObject {
    Q_OBJECT

public:
    explicit GitHubAPI(const QString &token, const QString &cookie = "", QObject *parent = nullptr);

    using JsonCallback = std::function<void(const QJsonDocument &)>;
    using JsonArrayCallback = std::function<void(const QJsonArray &)>;
    using JsonObjectCallback = std::function<void(const QJsonObject &)>;
    using ErrorCallback = std::function<void(const QString &)>;

    void getUser(JsonObjectCallback onSuccess, ErrorCallback onError = nullptr);
    void getRepositories(const QString &username = "", const QString &sort = "updated",
                         JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getRepositoryDetail(const QString &owner, const QString &repo,
                             JsonObjectCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getIssues(const QString &owner, const QString &repo, const QString &state = "open",
                   JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getPullRequests(const QString &owner, const QString &repo, const QString &state = "open",
                         JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getReadme(const QString &owner, const QString &repo, const QString &branch = "main",
                   JsonObjectCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getRepositoryContents(const QString &owner, const QString &repo,
                               const QString &path = "", const QString &ref = "",
                               JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getFileContent(const QString &owner, const QString &repo,
                        const QString &path, const QString &branch = "main",
                        JsonObjectCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getBranches(const QString &owner, const QString &repo,
                     JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getCommits(const QString &owner, const QString &repo,
                    const QString &path = "", int perPage = 30,
                    JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getIssueDetail(const QString &owner, const QString &repo, int issueNumber,
                        JsonObjectCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getIssueComments(const QString &owner, const QString &repo, int issueNumber,
                          JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getStarredRepos(const QString &username = "",
                         JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getGists(const QString &username = "",
                  JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getFollowers(const QString &username,
                      JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void getFollowing(const QString &username,
                      JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void searchRepositories(const QString &query, const QString &sort = "stars",
                            JsonArrayCallback onSuccess = nullptr, ErrorCallback onError = nullptr);
    void validateToken(JsonObjectCallback onSuccess, ErrorCallback onError = nullptr);
    void getRateLimit(JsonObjectCallback onSuccess = nullptr, ErrorCallback onError = nullptr);

    QString token() const { return m_token; }
    QString cookie() const { return m_cookie; }

private:
    QNetworkAccessManager *m_manager;
    QString m_token;
    QString m_cookie;
    QString m_baseUrl;

    void get(const QString &path, const QMap<QString, QString> &params,
             JsonCallback onSuccess, ErrorCallback onError);
};
