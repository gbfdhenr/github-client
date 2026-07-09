#pragma once

#include <QString>
#include <QMap>
#include <QStringList>
#include <QVector>

struct DecryptedCookie {
    QMap<QString, QString> cookies;
    QString browserName;
};

class BrowserCookieExtractor {
public:
    BrowserCookieExtractor();

    QMap<QString, QString> getGithubCookieFromChrome();
    QMap<QString, QString> getGithubCookieFromEdge();
    QMap<QString, QString> getGithubCookieFromFirefox();
    QMap<QString, QMap<QString, QString>> getAllGithubCookies();
    QStringList listAvailableBrowsers();
    static QString formatCookieString(const QMap<QString, QString> &cookies);

    QString platform() const { return m_platform; }
    bool isDebianBased() const { return m_isDebian; }

private:
    QString m_platform;
    bool m_isDebian;

    QString detectPlatform();
    bool detectIsDebian();
    QString getBrowserPath(const QString &browser);

    QMap<QString, QString> extractChromiumCookie(const QString &browser, const QString &domain = "github.com");
    QMap<QString, QString> extractFirefoxCookie(const QString &domain = "github.com");
    QString decryptChromiumValue(const QByteArray &encrypted, const QString &browser);
    QString decryptAesCookie(const QByteArray &encrypted, const QString &browser);
    QByteArray getLinuxKey(const QString &browser);
    QByteArray deriveKey(const QString &password);
    QString getLocalStatePath(const QString &browser);

    QStringList parseFirefoxProfiles(const QString &profilesIni, const QString &basePath);
    QMap<QString, QString> readFirefoxCookies(const QString &dbPath, const QString &domain);
};

QMap<QString, QString> getGithubCookieFromBrowser(const QString &browser = "auto");
QString getGithubCookieString(const QString &browser = "auto");
QStringList listAvailableBrowsers();
