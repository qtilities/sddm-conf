/*
    Copyright (c) 2022-2023 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#pragma once

#include "settings.hpp"

#include <QApplication>
#include <QTranslator>

class MainDialog;
class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char *argv[]);

    Settings &settings() { return settings_; }

    void initLocale();
    void initUi();
    void about();

private:
    MainDialog *mainDialog_;
    Settings settings_;
    QTranslator qtTranslator_, translator_;
};
