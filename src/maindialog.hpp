/*
    Copyright (c) 2022-2023 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#pragma once

#include <QDialog>

class QProcess;
class QPushButton;

namespace Ui {
class MainDialog;
}
class MainDialog : public QDialog {
    Q_OBJECT

public:
    explicit MainDialog(QWidget* parent = nullptr);
    ~MainDialog();

private:
    void loadSettings();
    void loadFile();

    Ui::MainDialog* ui;
    QProcess* previewProcess_;
    QPushButton* previewButton_;
};
