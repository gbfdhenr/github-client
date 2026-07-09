#include "utils/browser_cookies.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QStandardPaths>
#include <QTemporaryFile>

#include <sqlite3.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

BrowserCookieExtractor::BrowserCookieExtractor()
    : m_platform(detectPlatform())
    , m_isDebian(detectIsDebian())
{
}

QString BrowserCookieExtractor::detectPlatform()
{
#ifdef Q_OS_WIN
    return "win";
#elif defined(Q_OS_MACOS)
    return "mac";
#else
    return "linux";
#endif
}

bool BrowserCookieExtractor::detectIsDebian()
{
    return QFileInfo::exists("/etc/debian_version") || QFileInfo::exists("/etc/lsb-release");
}

QString BrowserCookieExtractor::getBrowserPath(const QString &browser)
{
    if (browser == "chrome") {
        if (m_platform == "win")
            return QDir::fromNativeSeparators(qgetenv("LOCALAPPDATA"))
                   + "/Google/Chrome/User Data/Default/Cookies";
        else if (m_platform == "mac")
            return QDir::homePath() + "/Library/Application Support/Google/Chrome/Default/Cookies";
        else
            return QDir::homePath() + "/.config/google-chrome/Default/Cookies";
    } else if (browser == "edge") {
        if (m_platform == "win")
            return QDir::fromNativeSeparators(qgetenv("LOCALAPPDATA"))
                   + "/Microsoft/Edge/User Data/Default/Cookies";
        else if (m_platform == "mac")
            return QDir::homePath() + "/Library/Application Support/Microsoft Edge/Default/Cookies";
        else
            return QDir::homePath() + "/.config/microsoft-edge/Default/Cookies";
    } else if (browser == "firefox") {
        if (m_platform == "win")
            return QDir::fromNativeSeparators(qgetenv("APPDATA")) + "/Mozilla/Firefox/Profiles/";
        else if (m_platform == "mac")
            return QDir::homePath() + "/Library/Application Support/Firefox/Profiles/";
        else
            return QDir::homePath() + "/.mozilla/firefox/";
    }
    return QString();
}

QMap<QString, QString> BrowserCookieExtractor::getGithubCookieFromChrome()
{
    return extractChromiumCookie("chrome");
}

QMap<QString, QString> BrowserCookieExtractor::getGithubCookieFromEdge()
{
    return extractChromiumCookie("edge");
}

QMap<QString, QString> BrowserCookieExtractor::getGithubCookieFromFirefox()
{
    return extractFirefoxCookie();
}

QMap<QString, QString> BrowserCookieExtractor::extractChromiumCookie(const QString &browser, const QString &domain)
{
    QString cookiePath = getBrowserPath(browser);
    if (cookiePath.isEmpty() || !QFileInfo::exists(cookiePath))
        return {};

    QFileInfo fi(cookiePath);
    if (!fi.isReadable())
        return {};

    QMap<QString, QString> cookies;

    QTemporaryFile tmpFile;
    if (!tmpFile.open())
        return {};
    QString tmpPath = tmpFile.fileName();
    tmpFile.close();

    if (!QFile::copy(cookiePath, tmpPath))
        return {};

    sqlite3 *db = nullptr;
    if (sqlite3_open(tmpPath.toUtf8().constData(), &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        QFile::remove(tmpPath);
        return {};
    }

    QString sql = "SELECT name, encrypted_value FROM cookies WHERE host_key LIKE ?";
    sqlite3_stmt *stmt = nullptr;
    QString pattern = "%" + domain + "%";

    if (sqlite3_prepare_v2(db, sql.toUtf8().constData(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, pattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            const void *encrypted = sqlite3_column_blob(stmt, 1);
            int encLen = sqlite3_column_bytes(stmt, 1);

            if (name && encrypted && encLen > 0) {
                QByteArray encData(static_cast<const char *>(encrypted), encLen);
                QString decrypted = decryptChromiumValue(encData, browser);
                if (!decrypted.isEmpty()) {
                    cookies[QString::fromUtf8(name)] = decrypted;
                }
            }
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    QFile::remove(tmpPath);

    return cookies.isEmpty() ? QMap<QString, QString>() : cookies;
}

QString BrowserCookieExtractor::decryptChromiumValue(const QByteArray &encrypted, const QString &browser)
{
    if (encrypted.startsWith("v10") || encrypted.startsWith("v11")) {
        return decryptAesCookie(encrypted, browser);
    }
    return QString::fromUtf8(encrypted);
}

QString BrowserCookieExtractor::decryptAesCookie(const QByteArray &encrypted, const QString &browser)
{
    if (encrypted.size() < 31)
        return QString();

    const unsigned char *data = reinterpret_cast<const unsigned char *>(encrypted.constData());
    const unsigned char *nonce = data + 3;
    const unsigned char *ciphertext = data + 15;
    int cipherLen = encrypted.size() - 31;
    const unsigned char *tag = data + 15 + cipherLen;

    QByteArray key = getLinuxKey(browser);
    if (key.isEmpty())
        return QString();

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
        return QString();

    QByteArray plain(cipherLen + 16, 0);
    int outLen = 0, finalLen = 0;

    bool ok = false;
    do {
        if (EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), nullptr, nullptr, nullptr) != 1) break;
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, nullptr) != 1) break;
        if (EVP_DecryptInit_ex(ctx, nullptr, nullptr,
                               reinterpret_cast<const unsigned char *>(key.constData()),
                               nonce) != 1) break;
        if (EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char *>(plain.data()), &outLen,
                              ciphertext, cipherLen) != 1) break;
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, const_cast<unsigned char *>(tag)) != 1) break;
        if (EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char *>(plain.data()) + outLen, &finalLen) != 1) break;
        ok = true;
    } while (false);

    EVP_CIPHER_CTX_free(ctx);

    if (!ok) return QString();
    return QString::fromUtf8(plain.left(outLen + finalLen));
}

QByteArray BrowserCookieExtractor::getLinuxKey(const QString &browser)
{
    QString localStatePath = getLocalStatePath(browser);
    if (!localStatePath.isEmpty() && QFileInfo::exists(localStatePath)) {
        QFile f(localStatePath);
        if (f.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
            f.close();
            QJsonObject obj = doc.object();
            QJsonObject osCrypt = obj["os_crypt"].toObject();
            QString encKey = osCrypt["encrypted_key"].toString();
            if (!encKey.isEmpty()) {
                QByteArray decoded = QByteArray::fromBase64(encKey.toUtf8());
                if (decoded.startsWith("DPAPI")) {
                    decoded = decoded.mid(5);
                }
            }
        }
    }

    return deriveKey(browser + " Safe Storage");
}

QByteArray BrowserCookieExtractor::deriveKey(const QString &password)
{
    QByteArray key(16, 0);
    QByteArray pw = password.toUtf8();
    QByteArray salt = QByteArray("saltysalt");

    PKCS5_PBKDF2_HMAC_SHA1(pw.constData(), pw.size(),
                            reinterpret_cast<const unsigned char *>(salt.constData()), salt.size(),
                            1, 16,
                            reinterpret_cast<unsigned char *>(key.data()));
    return key;
}

QString BrowserCookieExtractor::getLocalStatePath(const QString &browser)
{
    if (browser == "chrome")
        return QDir::homePath() + "/.config/google-chrome/Local State";
    else if (browser == "edge")
        return QDir::homePath() + "/.config/microsoft-edge/Local State";
    return QString();
}

QMap<QString, QString> BrowserCookieExtractor::extractFirefoxCookie(const QString &domain)
{
    QString profilesPath = getBrowserPath("firefox");
    if (profilesPath.isEmpty() || !QDir(profilesPath).exists())
        return {};

    QString basePath = QDir::cleanPath(profilesPath + "/..");
    QString profilesIni = basePath + "/profiles.ini";

    QStringList profiles;
    if (QFileInfo::exists(profilesIni)) {
        profiles = parseFirefoxProfiles(profilesIni, profilesPath);
    } else {
        QDir dir(profilesPath);
        for (const QString &entry : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            if (entry.endsWith(".default") || entry.endsWith(".default-release")) {
                profiles.append(profilesPath + "/" + entry);
            }
        }
    }

    for (const QString &profile : profiles) {
        QString cookiesDb = profile + "/cookies.sqlite";
        if (QFileInfo::exists(cookiesDb)) {
            QMap<QString, QString> cookies = readFirefoxCookies(cookiesDb, domain);
            if (!cookies.isEmpty())
                return cookies;
        }
    }

    return {};
}

QStringList BrowserCookieExtractor::parseFirefoxProfiles(const QString &profilesIni, const QString &basePath)
{
    QStringList profiles;
    QFile f(profilesIni);
    if (!f.open(QIODevice::ReadOnly))
        return profiles;

    QString currentSection;
    QString currentPath;
    bool currentIsRelative = false;
    bool inProfile = false;

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (line.startsWith("[") && line.endsWith("]")) {
            if (inProfile && !currentPath.isEmpty()) {
                QString fullPath = currentIsRelative ? basePath + "/" + currentPath : currentPath;
                if (QDir(fullPath).exists())
                    profiles.append(fullPath);
            }
            currentSection = line.mid(1, line.size() - 2);
            inProfile = currentSection.startsWith("Profile");
            currentPath.clear();
            currentIsRelative = false;
        } else if (inProfile) {
            int eq = line.indexOf('=');
            if (eq > 0) {
                QString key = line.left(eq).trimmed().toLower();
                QString val = line.mid(eq + 1).trimmed();
                if (key == "path") currentPath = val;
                else if (key == "isrelative") currentIsRelative = (val == "1");
            }
        }
    }

    if (inProfile && !currentPath.isEmpty()) {
        QString fullPath = currentIsRelative ? basePath + "/" + currentPath : currentPath;
        if (QDir(fullPath).exists())
            profiles.append(fullPath);
    }

    f.close();
    return profiles;
}

QMap<QString, QString> BrowserCookieExtractor::readFirefoxCookies(const QString &dbPath, const QString &domain)
{
    QMap<QString, QString> cookies;

    QTemporaryFile tmpFile;
    if (!tmpFile.open())
        return {};
    QString tmpPath = tmpFile.fileName();
    tmpFile.close();

    if (!QFile::copy(dbPath, tmpPath))
        return {};

    sqlite3 *db = nullptr;
    if (sqlite3_open(tmpPath.toUtf8().constData(), &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        QFile::remove(tmpPath);
        return {};
    }

    QString sql = "SELECT name, value FROM moz_cookies WHERE host LIKE ?";
    sqlite3_stmt *stmt = nullptr;
    QString pattern = "%" + domain + "%";

    if (sqlite3_prepare_v2(db, sql.toUtf8().constData(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, pattern.toUtf8().constData(), -1, SQLITE_TRANSIENT);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            const char *value = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
            if (name && value) {
                cookies[QString::fromUtf8(name)] = QString::fromUtf8(value);
            }
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    QFile::remove(tmpPath);

    return cookies.isEmpty() ? QMap<QString, QString>() : cookies;
}

QMap<QString, QMap<QString, QString>> BrowserCookieExtractor::getAllGithubCookies()
{
    QMap<QString, QMap<QString, QString>> results;

    auto chrome = getGithubCookieFromChrome();
    if (!chrome.isEmpty()) results["chrome"] = chrome;

    auto edge = getGithubCookieFromEdge();
    if (!edge.isEmpty()) results["edge"] = edge;

    auto firefox = getGithubCookieFromFirefox();
    if (!firefox.isEmpty()) results["firefox"] = firefox;

    return results;
}

QStringList BrowserCookieExtractor::listAvailableBrowsers()
{
    QStringList available;
    for (const QString &browser : {"chrome", "edge", "firefox"}) {
        QString path = getBrowserPath(browser);
        if (!path.isEmpty()) {
            if (browser == "firefox") {
                if (QDir(path).exists())
                    available.append(browser);
            } else {
                if (QFileInfo::exists(path))
                    available.append(browser);
            }
        }
    }
    return available;
}

QString BrowserCookieExtractor::formatCookieString(const QMap<QString, QString> &cookies)
{
    QStringList parts;
    for (auto it = cookies.begin(); it != cookies.end(); ++it) {
        parts.append(it.key() + "=" + it.value());
    }
    return parts.join("; ");
}

QMap<QString, QString> getGithubCookieFromBrowser(const QString &browser)
{
    BrowserCookieExtractor extractor;
    if (browser == "auto") {
        auto all = extractor.getAllGithubCookies();
        if (!all.isEmpty())
            return all.first();
    } else if (browser == "chrome") {
        return extractor.getGithubCookieFromChrome();
    } else if (browser == "edge") {
        return extractor.getGithubCookieFromEdge();
    } else if (browser == "firefox") {
        return extractor.getGithubCookieFromFirefox();
    }
    return {};
}

QString getGithubCookieString(const QString &browser)
{
    auto cookies = getGithubCookieFromBrowser(browser);
    if (!cookies.isEmpty())
        return BrowserCookieExtractor::formatCookieString(cookies);
    return QString();
}

QStringList listAvailableBrowsers()
{
    BrowserCookieExtractor extractor;
    return extractor.listAvailableBrowsers();
}
