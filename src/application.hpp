/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#pragma once

#include "settings.hpp"

#include <QApplication>

class Application : public QApplication {
    Q_OBJECT

public:
    Application(int&, char**);

    Settings& settings() { return settings_; }

private:
    Settings settings_;
};
