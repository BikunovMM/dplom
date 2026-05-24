#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H

#include <QSettings>
//#include <QtKeychain> /* TODO */
#include "appdata.h"

namespace SessionsManager
{
    void saveSession(const char *token);
    void getSession(char *token);
    void deleteSession();
    bool containsSession();
    void getToken(char *token);
    void getLanguage(QString& lang);
    void setLanguage(const QString& lang);
    void setLanguage(const char *lang);
}

#endif /* SESSIONSMANAGER_H */
