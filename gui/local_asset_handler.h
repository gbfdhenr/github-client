#pragma once
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>
#include <QFile>
#include <QBuffer>
#include <QDebug>

class LocalAssetHandler : public QWebEngineUrlSchemeHandler {
    Q_OBJECT
public:
    explicit LocalAssetHandler(QObject *parent = nullptr)
        : QWebEngineUrlSchemeHandler(parent) {}

    void requestStarted(QWebEngineUrlRequestJob *job) override {
        QString path = job->requestUrl().path();

        // Map local://assets/xxx → qrc:/github_files/xxx
        // Map local://fonts/xxx  → qrc:/fonts/xxx
        QString filePath;
        if (path.startsWith("/assets/")) {
            filePath = ":/github_files/" + path.mid(8);
        } else if (path.startsWith("/fonts/")) {
            filePath = ":/fonts/" + path.mid(7);
        } else {
            job->fail(QWebEngineUrlRequestJob::UrlNotFound);
            return;
        }

        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly)) {
            qWarning() << "LocalAssetHandler: not found" << filePath;
            job->fail(QWebEngineUrlRequestJob::UrlNotFound);
            return;
        }

        QByteArray data = f.readAll();
        f.close();

        QString mime = "application/octet-stream";
        if (path.endsWith(".js"))           mime = "application/javascript; charset=utf-8";
        else if (path.endsWith(".mjs"))     mime = "application/javascript; charset=utf-8";
        else if (path.endsWith(".css"))     mime = "text/css; charset=utf-8";
        else if (path.endsWith(".html"))    mime = "text/html; charset=utf-8";
        else if (path.endsWith(".svg"))     mime = "image/svg+xml";
        else if (path.endsWith(".png"))     mime = "image/png";
        else if (path.endsWith(".jpg"))     mime = "image/jpeg";
        else if (path.endsWith(".jpeg"))    mime = "image/jpeg";
        else if (path.endsWith(".webp"))    mime = "image/webp";
        else if (path.endsWith(".gif"))     mime = "image/gif";
        else if (path.endsWith(".ico"))     mime = "image/x-icon";
        else if (path.endsWith(".woff2"))   mime = "font/woff2";
        else if (path.endsWith(".woff"))    mime = "font/woff";
        else if (path.endsWith(".ttf"))     mime = "font/ttf";
        else if (path.endsWith(".json"))    mime = "application/json; charset=utf-8";
        else if (path.endsWith(".map"))     mime = "application/json; charset=utf-8";

        QBuffer *buf = new QBuffer();
        buf->open(QIODevice::WriteOnly);
        buf->write(data);
        buf->close();
        connect(job, &QObject::destroyed, buf, &QObject::deleteLater);
        job->reply(mime.toUtf8(), buf);
    }
};
