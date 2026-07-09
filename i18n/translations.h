#pragma once

#include <QMap>
#include <QString>

void setLanguage(const QString &lang);
QString getText(const QString &key);

extern QString g_currentLang;
