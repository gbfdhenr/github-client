#include "utils/browser_cookies.h"
#include "api/github_api.h"

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>

GitHubAPI::GitHubAPI(const QString &token, const QString &cookie, QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_token(token)
    , m_cookie(cookie)
    , m_baseUrl("https://api.github.com")
{
}

void GitHubAPI::get(const QString &path, const QMap<QString, QString> &params,
                     JsonCallback onSuccess, ErrorCallback onError)
{
    QUrl url(m_baseUrl + path);
    QUrlQuery query;
    for (auto it = params.begin(); it != params.end(); ++it) {
        query.addQueryItem(it.key(), it.value());
    }
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("token " + m_token).toUtf8());
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    if (!m_cookie.isEmpty()) {
        request.setRawHeader("Cookie", m_cookie.toUtf8());
    }

    QNetworkReply *reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            if (onError) onError(reply->errorString());
            return;
        }
        QByteArray data = reply->readAll();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error != QJsonParseError::NoError) {
            if (onError) onError("JSON parse error: " + err.errorString());
            return;
        }
        if (onSuccess) onSuccess(doc);
    });
}

void GitHubAPI::getUser(JsonObjectCallback onSuccess, ErrorCallback onError)
{
    get("/user", {}, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object());
    }, onError);
}

void GitHubAPI::getRepositories(const QString &username, const QString &sort,
                                 JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QString path = username.isEmpty() ? "/user/repos" : ("/users/" + username + "/repos");
    QMap<QString, QString> params;
    params["per_page"] = "100";
    params["sort"] = sort;
    get(path, params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getRepositoryDetail(const QString &owner, const QString &repo,
                                     JsonObjectCallback onSuccess, ErrorCallback onError)
{
    get("/repos/" + owner + "/" + repo, {}, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object());
    }, onError);
}

void GitHubAPI::getIssues(const QString &owner, const QString &repo, const QString &state,
                           JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["state"] = state;
    params["per_page"] = "100";
    get("/repos/" + owner + "/" + repo + "/issues", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getPullRequests(const QString &owner, const QString &repo, const QString &state,
                                 JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["state"] = state;
    params["per_page"] = "100";
    get("/repos/" + owner + "/" + repo + "/pulls", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getReadme(const QString &owner, const QString &repo, const QString &branch,
                           JsonObjectCallback onSuccess, ErrorCallback onError)
{
    get("/repos/" + owner + "/" + repo + "/readme", {}, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object());
    }, onError);
}

void GitHubAPI::getRepositoryContents(const QString &owner, const QString &repo,
                                       const QString &path, const QString &ref,
                                       JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QString apiPath = "/repos/" + owner + "/" + repo + "/contents";
    if (!path.isEmpty()) apiPath += "/" + path;
    QMap<QString, QString> params;
    if (!ref.isEmpty()) params["ref"] = ref;
    get(apiPath, params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getFileContent(const QString &owner, const QString &repo,
                                const QString &path, const QString &branch,
                                JsonObjectCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["ref"] = branch;
    get("/repos/" + owner + "/" + repo + "/contents/" + path, params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object());
    }, onError);
}

void GitHubAPI::getBranches(const QString &owner, const QString &repo,
                             JsonArrayCallback onSuccess, ErrorCallback onError)
{
    get("/repos/" + owner + "/" + repo + "/branches", {}, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getCommits(const QString &owner, const QString &repo,
                            const QString &path, int perPage,
                            JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["per_page"] = QString::number(perPage);
    if (!path.isEmpty()) params["path"] = path;
    get("/repos/" + owner + "/" + repo + "/commits", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getIssueDetail(const QString &owner, const QString &repo, int issueNumber,
                                JsonObjectCallback onSuccess, ErrorCallback onError)
{
    get("/repos/" + owner + "/" + repo + "/issues/" + QString::number(issueNumber), {},
        [onSuccess](const QJsonDocument &doc) {
            if (onSuccess) onSuccess(doc.object());
        }, onError);
}

void GitHubAPI::getIssueComments(const QString &owner, const QString &repo, int issueNumber,
                                  JsonArrayCallback onSuccess, ErrorCallback onError)
{
    get("/repos/" + owner + "/" + repo + "/issues/" + QString::number(issueNumber) + "/comments", {},
        [onSuccess](const QJsonDocument &doc) {
            if (onSuccess) onSuccess(doc.array());
        }, onError);
}

void GitHubAPI::getStarredRepos(const QString &username,
                                 JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["per_page"] = "100";
    QString path = username.isEmpty() ? "/user/starred" : ("/users/" + username + "/starred");
    get(path, params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getGists(const QString &username,
                          JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["per_page"] = "100";
    QString path = username.isEmpty() ? "/gists" : ("/users/" + username + "/gists");
    get(path, params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getFollowers(const QString &username,
                              JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["per_page"] = "100";
    get("/users/" + username + "/followers", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::getFollowing(const QString &username,
                              JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["per_page"] = "100";
    get("/users/" + username + "/following", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.array());
    }, onError);
}

void GitHubAPI::searchRepositories(const QString &query, const QString &sort,
                                    JsonArrayCallback onSuccess, ErrorCallback onError)
{
    QMap<QString, QString> params;
    params["q"] = query;
    params["sort"] = sort;
    params["per_page"] = "100";
    get("/search/repositories", params, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object()["items"].toArray());
    }, onError);
}

void GitHubAPI::validateToken(JsonObjectCallback onSuccess, ErrorCallback onError)
{
    getUser(onSuccess, onError);
}

void GitHubAPI::getRateLimit(JsonObjectCallback onSuccess, ErrorCallback onError)
{
    get("/rate_limit", {}, [onSuccess](const QJsonDocument &doc) {
        if (onSuccess) onSuccess(doc.object());
    }, onError);
}
