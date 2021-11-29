/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#include "application.hpp"
#include "src/maindialog.hpp"

#include <QFile>
#include <QLocale>
#include <QStandardPaths>
#include <QTranslator>

Application::Application(int& argc, char** argv)
    : QApplication(argc, argv)
{
    // NOTE: the example on following link seems not working; manually locating (12/2021)
    // https://doc.qt.io/qt-5/internationalization.html#example-basic-qt-modules
    QTranslator qtTranslator;
    const QString qtDataPath = QStandardPaths::locate(
        QStandardPaths::GenericDataLocation, "qt/translations",
        QStandardPaths::LocateDirectory);

    if (qtTranslator.load(
            QLocale(), QLatin1String("qtbase"), QLatin1String("_"), qtDataPath))
        installTranslator(&qtTranslator);

    QTranslator translator;
    const QString appDataPath = QStandardPaths::locate(
        QStandardPaths::GenericDataLocation, "sddm-conf/translations",
        QStandardPaths::LocateDirectory);

    if (translator.load(
            QLocale(), QLatin1String("sddm-conf"), QLatin1String("_"), appDataPath))
        installTranslator(&translator);

    // FIXME: The configuration file management is more complex,
    // this is just a temporary solution, see sddm.conf manual.
    QString path = QStringLiteral("/etc/sddm.conf");
    if (!QFile::exists(path))
        path = QStringLiteral("/etc/sddm.conf.d/sddm.conf");

    settings_.setPath(path);
    settings_.load();
}

int main(int argc, char* argv[])
{
    Application app(argc, argv);
    MainDialog w;
    w.show();

    return app.exec();
}
