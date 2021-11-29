/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#include "maindialog.hpp"
#include "ui_maindialog.h"
#include "application.hpp"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QProcess>
#include <QTextStream>

MainDialog::MainDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
    , previewProcess_(new QProcess(this))
    , previewButton_(new QPushButton(QIcon::fromTheme("window-close"), QString(), nullptr))
{
    ui->setupUi(this);
    setWindowTitle(tr("SDDM Configuration Editor"));

    loadSettings();
    loadFile();

    previewButton_->setWindowFlags(
        Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint
        | Qt::BypassWindowManagerHint);
    previewButton_->move(0, 0);
    previewButton_->hide();

    Settings* settings = &static_cast<Application*>(qApp)->settings();
    QPushButton* reset = ui->buttonBox->button(QDialogButtonBox::Reset);
    QPushButton* save = ui->buttonBox->button(QDialogButtonBox::Save);

    reset->setEnabled(false);
    save->setEnabled(false);

    connect(qApp, &QCoreApplication::aboutToQuit, previewButton_, &QObject::deleteLater);
    connect(qApp, &QCoreApplication::aboutToQuit, this, [=] {
        if (previewProcess_->state() == QProcess::Running)
            previewProcess_->terminate();
        previewProcess_->deleteLater();
    });
    connect(reset, &QPushButton::clicked, this, [=] {
        settings->load();
        loadSettings();
        reset->setEnabled(false);
        save->setEnabled(false);
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [=] {
        settings->save();
        loadFile();
        reset->setEnabled(false);
        save->setEnabled(false);
    });
    connect(ui->pbnAbout, &QPushButton::clicked, this, [=] {
        QMessageBox::about(this, tr("SDDM Conf v" SDDM_CONF_VERSION),
                           tr("Copyright (C) 2022\n"
                              "Andrea Zanellato <redtid3@gmail.com>"));
    });
    connect(ui->pbnThemePreview, &QPushButton::clicked, this, [=] {
        QString currentTheme = settings->currentTheme();
        QString path = QStringLiteral("%1/%2/").arg(settings->themeDir(), currentTheme);
        QStringList args;
        args << "--test-mode"
             << "--theme" << path;

        previewButton_->setText(tr("Close %1 preview").arg(currentTheme));
        previewButton_->show();
        previewProcess_->start("sddm-greeter", args);
        hide();
    });
    connect(previewButton_, &QPushButton::clicked, this, [=] {
        if (previewProcess_->state() == QProcess::Running)
            previewProcess_->terminate();
        previewButton_->hide();
        show();
    });

#define CONNECT_FILEOPEN(NAME)                                 \
    connect(ui->tbn##NAME, &QToolButton::clicked, this, [=] {  \
        QString fileName = QFileDialog::getOpenFileName(       \
            this, tr("Choose a file"), ui->txt##NAME->text()); \
        if (!fileName.isEmpty())                               \
            ui->txt##NAME->setText(fileName);                  \
    });
#define CONNECT_DIROPEN(NAME)                                              \
    connect(ui->tbn##NAME, &QToolButton::clicked, this, [=] {              \
        QString dirName = QFileDialog::getExistingDirectory(               \
            this, tr("Choose a directory"), ui->txt##NAME->text(),         \
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks); \
        if (!dirName.isEmpty())                                            \
            ui->txt##NAME->setText(dirName);                               \
    });
#define RESET_BUTTONS        \
    reset->setEnabled(true); \
    save->setEnabled(true);

#define CONNECT_CHECKBOX(NAME)                              \
    connect(ui->chk##NAME, &QCheckBox::toggled, this, [=] { \
        settings->set##NAME(ui->chk##NAME->isChecked());    \
        RESET_BUTTONS;                                      \
    });
#define CONNECT_COMBOBOX(NAME)                                         \
    connect(ui->cbx##NAME, &QComboBox::currentTextChanged, this, [=] { \
        settings->set##NAME(ui->cbx##NAME->currentText());             \
        RESET_BUTTONS;                                                 \
    });
#define CONNECT_LINEEDIT(NAME)                                  \
    connect(ui->txt##NAME, &QLineEdit::textChanged, this, [=] { \
        settings->set##NAME(ui->txt##NAME->text());             \
        RESET_BUTTONS;                                          \
    });
#define CONNECT_SPINBOX(NAME)                                                       \
    connect(ui->sbx##NAME, QOverload<int>::of(&QSpinBox::valueChanged), this, [=] { \
        settings->set##NAME(ui->sbx##NAME->value());                                \
        RESET_BUTTONS;                                                              \
    });

    CONNECT_DIROPEN(FacesDir);
    CONNECT_DIROPEN(ThemeDir);
    CONNECT_DIROPEN(WaylandSessionDir);
    CONNECT_DIROPEN(X11SessionDir);

    CONNECT_FILEOPEN(WaylandSessionCommand);
    CONNECT_FILEOPEN(WaylandSessionLogFile);
    CONNECT_FILEOPEN(X11DisplayCommand);
    CONNECT_FILEOPEN(X11DisplayStopCommand);
    CONNECT_FILEOPEN(X11SessionCommand);
    CONNECT_FILEOPEN(X11ServerPath);
    CONNECT_FILEOPEN(XauthPath);
    CONNECT_FILEOPEN(XephyrPath);
    CONNECT_FILEOPEN(X11UserAuthFile);
    CONNECT_FILEOPEN(X11SessionLogFile);

    CONNECT_CHECKBOX(EnableAvatars);
    CONNECT_CHECKBOX(Relogin);
    CONNECT_CHECKBOX(RememberLastSession);
    CONNECT_CHECKBOX(RememberLastUser);
    CONNECT_CHECKBOX(WaylandEnableHiDPI);
    CONNECT_CHECKBOX(X11EnableHiDPI);

    CONNECT_COMBOBOX(CurrentTheme);
    CONNECT_COMBOBOX(Numlock);
    CONNECT_COMBOBOX(SessionFile);

    CONNECT_SPINBOX(AvatarsThreshold);
    CONNECT_SPINBOX(UidMin);
    CONNECT_SPINBOX(UidMax);
    CONNECT_SPINBOX(X11MinimumVT);

    CONNECT_LINEEDIT(CursorTheme);
    CONNECT_LINEEDIT(DefaultPath);
    CONNECT_LINEEDIT(FacesDir);
    CONNECT_LINEEDIT(HaltCommand);
    CONNECT_LINEEDIT(HideShells);
    CONNECT_LINEEDIT(HideUsers);
    CONNECT_LINEEDIT(InputMethod);
    CONNECT_LINEEDIT(RebootCommand);
    CONNECT_LINEEDIT(ThemeDir);
    CONNECT_LINEEDIT(Username);
    CONNECT_LINEEDIT(WaylandSessionCommand);
    CONNECT_LINEEDIT(WaylandSessionDir);
    CONNECT_LINEEDIT(WaylandSessionLogFile);
    CONNECT_LINEEDIT(XauthPath);
    CONNECT_LINEEDIT(XephyrPath);
    CONNECT_LINEEDIT(X11DisplayCommand);
    CONNECT_LINEEDIT(X11DisplayStopCommand);
    CONNECT_LINEEDIT(X11ServerArguments);
    CONNECT_LINEEDIT(X11ServerPath);
    CONNECT_LINEEDIT(X11SessionCommand);
    CONNECT_LINEEDIT(X11SessionDir);
    CONNECT_LINEEDIT(X11SessionLogFile);
    CONNECT_LINEEDIT(X11UserAuthFile);

#undef CONNECT_DIROPEN
#undef CONNECT_FILEOPEN
#undef CONNECT_SPINBOX
#undef CONNECT_LINEEDIT
#undef CONNECT_COMBOBOX
#undef CONNECT_CHECKBOX
#undef RESET_BUTTONS
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::loadSettings()
{
    Settings& settings = static_cast<Application*>(qApp)->settings();
    //===========
    // Autologin
    //===========
    ui->chkRelogin->setChecked(settings.relogin());

    QFileInfo fileInfo;
    QDir sessionDir(settings.waylandSessionDir());
    QStringList sessions = sessionDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (int i = 0; i < sessions.count(); ++i) {
        fileInfo.setFile(sessions.at(i));
        sessions[i] = fileInfo.baseName();
    }
    ui->cbxSessionFile->addItem(QString());
    ui->cbxSessionFile->addItems(sessions);

    sessionDir.setPath(settings.x11SessionDir());
    if (sessionDir.exists()) {
        sessions = sessionDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

        for (int i = 0; i < sessions.count(); ++i) {
            fileInfo.setFile(sessions.at(i));
            sessions[i] = fileInfo.baseName();
        }
        ui->cbxSessionFile->addItems(sessions);
    }
    int index = ui->cbxSessionFile->findText(settings.sessionFile());
    if (index != -1)
        ui->cbxSessionFile->setCurrentIndex(index);

    ui->txtUsername->setText(settings.username());
    //=========
    // General
    //=========
    ui->txtHaltCommand->setText(settings.haltCommand());
    ui->txtRebootCommand->setText(settings.rebootCommand());
    ui->txtInputMethod->setText(settings.inputMethod());

    index = ui->cbxNumlock->findText(settings.numlock());
    if (index != -1)
        ui->cbxNumlock->setCurrentIndex(index);
    //=======
    // Theme
    //=======
    QDir themeDir(settings.themeDir());
    QStringList themes = themeDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->cbxCurrentTheme->addItem(QString());
    ui->cbxCurrentTheme->addItems(themes);

    index = ui->cbxCurrentTheme->findText(settings.currentTheme());
    if (index != -1)
        ui->cbxCurrentTheme->setCurrentIndex(index);

    ui->txtCursorTheme->setText(settings.cursorTheme());
    ui->sbxAvatarsThreshold->setValue(settings.avatarsThreshold());
    ui->chkEnableAvatars->setChecked(settings.enableAvatars());
    ui->txtFacesDir->setText(settings.facesDir());
    ui->txtThemeDir->setText(settings.themeDir());
    //=======
    // Users
    //=======
    ui->txtDefaultPath->setText(settings.defaultPath());
    ui->txtHideShells->setText(settings.hideShells());
    ui->txtHideUsers->setText(settings.hideUsers());
    ui->sbxUidMin->setValue(settings.uidMin());
    ui->sbxUidMax->setValue(settings.uidMax());
    ui->chkRememberLastSession->setChecked(settings.rememberLastSession());
    ui->chkRememberLastUser->setChecked(settings.rememberLastUser());
    //=========
    // Wayland
    //=========
    ui->chkWaylandEnableHiDPI->setChecked(settings.waylandEnableHiDPI());
    ui->txtWaylandSessionCommand->setText(settings.waylandSessionCommand());
    ui->txtWaylandSessionLogFile->setText(settings.waylandSessionLogFile());
    ui->txtWaylandSessionDir->setText(settings.waylandSessionDir());
    //=====
    // X11
    //=====
    ui->chkX11EnableHiDPI->setChecked(settings.x11EnableHiDPI());
    ui->sbxX11MinimumVT->setValue(settings.x11MinimumVT());
    ui->txtX11ServerArguments->setText(settings.x11ServerArguments());
    ui->txtX11SessionDir->setText(settings.x11SessionDir());
    ui->txtX11DisplayCommand->setText(settings.x11DisplayCommand());
    ui->txtX11DisplayStopCommand->setText(settings.x11DisplayStopCommand());
    ui->txtX11SessionCommand->setText(settings.x11SessionCommand());
    ui->txtX11ServerPath->setText(settings.x11ServerPath());
    ui->txtXauthPath->setText(settings.xAuthPath());
    ui->txtXephyrPath->setText(settings.xephyrPath());
    ui->txtX11UserAuthFile->setText(settings.x11UserAuthFile());
    ui->txtX11SessionLogFile->setText(settings.x11SessionLogFile());
}

void MainDialog::loadFile()
{
    Settings& settings = static_cast<Application*>(qApp)->settings();
    QFile f(settings.path());
    f.open(QFile::ReadOnly | QFile::Text);

    QTextStream in(&f);
    QString text = in.readAll();
    f.close();

    ui->txtFile->setPlainText(text);
}
