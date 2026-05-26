#include "sessionsmanager.h"

void SessionsManager::saveSession(const char *token)
{
    QSettings settings(ORG_NAME, APP_NAME);

    settings.setValue("user/token", token);
    settings.sync();
}

void SessionsManager::getSession(char *token)
{
    QSettings settings(ORG_NAME, APP_NAME);

    std::string token_str = settings.value("user/token").toString().toStdString();

    const char *user_token = token_str.c_str();

    memcpy(token, user_token, strlen(user_token));
}

void SessionsManager::deleteSession() {
    QSettings settings(ORG_NAME, APP_NAME);

    settings.clear();
    settings.sync();
}

bool SessionsManager::containsSession() {
    QSettings settings(ORG_NAME, APP_NAME);

    return settings.contains("user/token");
}

void SessionsManager::getToken(char *token) {
    QSettings settings(ORG_NAME, APP_NAME);

    std::string token_str = settings.value("user/token").toString().toStdString();

    const char *user_token = token_str.c_str();

    memcpy(token, user_token, strlen(user_token));
}
#include "utils.h"
void SessionsManager::getLanguage(QString& lang)
{
    QSettings settings(ORG_NAME, APP_NAME);

    lang = settings.value("language").toString();

    log_str("lang: %s.\n", lang.toStdString().c_str());
}

void SessionsManager::setLanguage(const QString& lang)
{
    QSettings settings(ORG_NAME, APP_NAME);

    log_str("> language: %s.\n", lang.toStdString().c_str());

    settings.setValue("language", lang);
    settings.sync();
}

void SessionsManager::setLanguage(const char *lang)
{
    QSettings settings(ORG_NAME, APP_NAME);

    log_str("> language: %s.\n", lang);

    settings.setValue("language", lang);
    settings.sync();
}

//QSettings* Sessions::Manager::settings = new QSettings("bikunovmm", "converter");
