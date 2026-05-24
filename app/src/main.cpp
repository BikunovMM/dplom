#include "mainwindow.h"
#include "appdata.h"

#include <QApplication>
#include <QFontDatabase>
#include <QTranslator>
#include <QLibraryInfo>
#include "sessionsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName(ORG_NAME);
    QApplication::setOrganizationDomain(ORG_DOMAIN);
    QApplication::setApplicationName(APP_NAME);

    QFontDatabase::addApplicationFont(":/fonts/segoeui.ttf");
    QApplication::setFont(QFont("Segoe UI", 10, 500));

    QTranslator trans;
    QString lang;
    SessionsManager::getLanguage(lang);
    if (!lang.isEmpty()) {
        if (lang == "russian") {
            if (trans.load(":/translations/ru.qm")) {
                app.installTranslator(&trans);
            }
        }
    }
    else { /* auto - russian */
        if (trans.load(":/translations/ru.qm")) {
            app.installTranslator(&trans);
        }

        SessionsManager::setLanguage("russian");
    }

    QFile style_fd(":/styles/style.qss");
    if (style_fd.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(style_fd.readAll());
        app.setStyleSheet(stylesheet);
        style_fd.close();
    }
    else {
        log_err("[!] [main] Failed to open "
                "style.css from resources.\n");
        return 1;
    }

    MainWindow win(&trans, lang);
    win.show();

    return app.exec();
}

/*
 * TODO:
 * 1.  LoginWindow
 * 2.  MetadataWindow
 * 3.  ParametersWindow
 * 4.  ProfileWindow
 * 5.  SettingsWindow
 * 6.  Set .rc Window icon
 * 7.  Connect lib-fe (postgres)
 * 8.  Re-write server
 * 8.1 add openssl
 * 8.2 add lobsodium in registration on client
 *      and on server when comparing password with it hash
 * 9.  Make banner
 * 10. Change PagesBtns to huge icon + tiny text under it,
 *      like in QtCreatoe
 * 11. Chose font
 * 12. Deal with convertation
 */
