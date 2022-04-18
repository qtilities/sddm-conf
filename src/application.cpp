/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#include "application.hpp"
#include "src/maindialog.hpp"

#include <QFile>
#include <QDir>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{
    // FIXME: The configuration file management is more complex,
    // this is just a temporary solution, see sddm.conf manual.
    QString path = QStringLiteral("/etc/sddm.conf");
    if (!QFile::exists(path)) {
        QDir dir(QStringLiteral("/etc/sddm.conf.d"));
        if(dir.exists())
            path = QStringLiteral("/etc/sddm.conf.d/sddm.conf");
    }

    settings_.setPath(path);
    settings_.load();
}

int main(int argc, char* argv[])
{
    Application app(argc, argv);

    // Qt translations
    QTranslator qtTranslator;
    if (qtTranslator.load(QStringLiteral("qt_") + QLocale::system().name(),
                          QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(&qtTranslator);
    }
    // app translations
    QTranslator translator;
    if (translator.load(QStringLiteral("sddm-conf_") + QLocale::system().name(),
                        QStringLiteral(SDDM_DATA_DIR) + QStringLiteral("/translations"))) {
        app.installTranslator(&translator);
    }

    MainDialog w;
    w.show();

    return app.exec();
}
