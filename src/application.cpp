/*
    MIT License

    Copyright (c) 2022-2023 Andrea Zanellato <redtid3@gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
#include "application.hpp"
#include "maindialog.hpp"
#include "dialogabout.hpp"

#include <QDir>
#include <QFile>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>

Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
{
    // UseHighDpiPixmaps is default from Qt6
#if QT_VERSION < 0x060000
    setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
    initLocale();
    initUi();
}

void Application::initLocale()
{
#if PROJECT_TRANSLATION_TEST_ENABLED
    QLocale locale(QLocale(PROJECT_TRANSLATION_TEST));
    QLocale::setDefault(locale);
#else
    QLocale locale = QLocale::system();
#endif
    // install the translations built-into Qt itself
    if (qtTranslator_.load(QStringLiteral("qt_") + locale.name(),
#if QT_VERSION < 0x060000
                           QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
#else
                           QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
#endif
        installTranslator(&qtTranslator_);

    // E.g. "<appname>_en"
    QString translationsFileName = QCoreApplication::applicationName().toLower() + '_' + locale.name();
    // Try first in the same binary directory, in case we are building,
    // otherwise read from system data
    QString translationsPath = QCoreApplication::applicationDirPath();

    bool isLoaded = translator_.load(translationsFileName, translationsPath);
    if (!isLoaded) {
        // "/usr/share/<appname>/translations
        isLoaded = translator_.load(translationsFileName,
                                    QStringLiteral(PROJECT_DATA_DIR) + QStringLiteral("/translations"));
    }
    if (isLoaded)
        installTranslator(&translator_);
}

void Application::initUi()
{
    // FIXME: The configuration file management is more complex,
    // this is just a temporary solution, see sddm.conf manual.
    QString path = QStringLiteral("/etc/sddm.conf");
    if (!QFile::exists(path)) {
        QDir dir(QStringLiteral("/etc/sddm.conf.d"));
        if (dir.exists())
            path = QStringLiteral("/etc/sddm.conf.d/sddm.conf");
    }
    settings_.setPath(path);
    settings_.load();

    mainDialog_ = new MainDialog;
    mainDialog_->setWindowIcon(QIcon::fromTheme("preferences-system", QIcon(":/preferences-system")));
    mainDialog_->show();
}

void Application::about()
{
    Qtilities::DialogAbout dlg(mainDialog_);
    dlg.exec();
}

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    return app.exec();
}
