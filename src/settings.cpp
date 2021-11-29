/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#include "settings.hpp"

#include <QProcess>
#include <QFileInfo>
#include <QSettings>
#include <QTemporaryDir>

#include <QDebug>

// TODO: ...or no hardcoded defaults and just load configuration file(s)?

namespace Default {
const bool Relogin = false;
const char* HaltCommand = "/usr/bin/systemctl poweroff";
const char* NumLock = "none";
const char* RebootCommand = "/usr/bin/systemctl reboot";
const short DisableAvatarsThreshold = 7;
const bool EnableAvatars = true;
const char* FacesDir = "/usr/share/sddm/faces";
const char* ThemeDir = "/usr/share/sddm/themes";
const char* Path = "/usr/local/sbin:/usr/local/bin:/usr/bin";
// https://systemd.io/UIDS-GIDS/#special-systemd-uid-ranges
const int MaximumUid = 60513;
const int MinimumUid = 1000;
const bool RememberLastSession = true;
const bool RememberLastUser = true;
const bool ReuseSession = true;
namespace Wayland {
    const bool EnableHiDPI = false;
    const char* SessionCommand = "/usr/share/sddm/scripts/wayland-session";
    const char* SessionDir = "/usr/share/wayland-sessions";
    const char* SessionLogFile = ".local/share/sddm/wayland-session.log";
} // namespace Wayland
namespace X11 {
    const bool EnableHiDPI = false;
    const char* DisplayCommand = "/usr/share/sddm/scripts/Xsetup";
    const char* DisplayStopCommand = "/usr/share/sddm/scripts/Xstop";
    const short MinimumVT = 1;
    const char* ServerArguments = "-nolisten tcp";
    const char* ServerPath = "/usr/bin/X";
    const char* SessionCommand = "/usr/share/sddm/scripts/Xsession";
    const char* SessionDir = "/usr/share/xsessions";
    const char* SessionLogFile = ".local/share/sddm/xorg-session.log";
    const char* UserAuthFile = ".Xauthority";
    const char* XauthPath = "/usr/bin/xauth";
    const char* XephyrPath = "/usr/bin/Xephyr";
} // namespace X11
} // namespace Default

Settings::Settings()
    : haltCommand_(Default::HaltCommand)
    , numlock_(Default::NumLock)
    , rebootCommand_(Default::RebootCommand)
    , facesDir_(Default::FacesDir)
    , themeDir_(Default::ThemeDir)
    , defaultPath_(Default::Path)
    , wSessionCommand_(Default::Wayland::SessionCommand)
    , wSessionDir_(Default::Wayland::SessionDir)
    , wSessionLogFile_(Default::Wayland::SessionLogFile)
    , xDisplayCommand_(Default::X11::DisplayCommand)
    , xDisplayStopCommand_(Default::X11::DisplayStopCommand)
    , xServerArguments_(Default::X11::ServerArguments)
    , xServerPath_(Default::X11::ServerPath)
    , xSessionCommand_(Default::X11::SessionCommand)
    , xSessionDir_(Default::X11::SessionDir)
    , xSessionLogFile_(Default::X11::SessionLogFile)
    , xUserAuthFile_(Default::X11::UserAuthFile)
    , xAuthPath_(Default::X11::XauthPath)
    , xephyrPath_(Default::X11::XephyrPath)
    , relogin_(Default::Relogin)
    , enableAvatars_(Default::EnableAvatars)
    , rememberLastSession_(Default::RememberLastSession)
    , rememberLastUser_(Default::RememberLastUser)
    , reuseSession_(Default::ReuseSession)
    , wEnableHiDPI_(Default::Wayland::EnableHiDPI)
    , xEnableHiDPI_(Default::X11::EnableHiDPI)
    , avatarsThreshold_(Default::DisableAvatarsThreshold)
    , xMinimumVT_(Default::X11::MinimumVT)
    , uidMin_(Default::MinimumUid)
    , uidMax_(Default::MaximumUid)
{
}

void Settings::load()
{
    QSettings settings(path_, QSettings::IniFormat);

    settings.beginGroup("Autologin");
    relogin_ = settings.value(QStringLiteral("Relogin"), Default::Relogin).toBool();
    session_ = settings.value(QStringLiteral("Session")).toString();
    user_ = settings.value(QStringLiteral("User")).toString();
    settings.endGroup();

    // settings.beginGroup("General");
    haltCommand_ = settings.value(QStringLiteral("HaltCommand"), Default::HaltCommand).toString();
    inputMethod_ = settings.value(QStringLiteral("InputMethod")).toString();
    namespaces_ = settings.value(QStringLiteral("Namespaces")).toString();
    numlock_ = settings.value(QStringLiteral("Numlock"), Default::NumLock).toString();
    rebootCommand_ = settings.value(QStringLiteral("RebootCommand"), Default::RebootCommand).toString();
    // settings.endGroup();

    settings.beginGroup("Theme");
    currentTheme_ = settings.value(QStringLiteral("Current")).toString();
    cursorTheme_ = settings.value(QStringLiteral("CursorTheme")).toString();
    avatarsThreshold_ = settings.value(QStringLiteral("DisableAvatarsThreshold"), Default::DisableAvatarsThreshold).toInt();
    enableAvatars_ = settings.value(QStringLiteral("EnableAvatars")).toBool();
    facesDir_ = settings.value(QStringLiteral("FacesDir"), Default::FacesDir).toString();
    font_ = settings.value(QStringLiteral("Font")).toString();
    themeDir_ = settings.value(QStringLiteral("ThemeDir"), Default::ThemeDir).toString();
    settings.endGroup();

    settings.beginGroup("Users");
    defaultPath_ = settings.value(QStringLiteral("DefaultPath"), Default::Path).toString();
    hideShells_ = settings.value(QStringLiteral("HideShells")).toString();
    hideUsers_ = settings.value(QStringLiteral("HideUsers")).toString();
    uidMax_ = settings.value(QStringLiteral("MaximumUid"), Default::MaximumUid).toInt();
    uidMin_ = settings.value(QStringLiteral("MinimumUid"), Default::MinimumUid).toInt();
    rememberLastSession_ = settings.value(QStringLiteral("RememberLastSession"), Default::RememberLastSession).toBool();
    rememberLastUser_ = settings.value(QStringLiteral("RememberLastUser"), Default::RememberLastUser).toBool();
    reuseSession_ = settings.value(QStringLiteral("ReuseSession"), Default::ReuseSession).toBool();
    settings.endGroup();

    settings.beginGroup("Wayland");
    wEnableHiDPI_ = settings.value(QStringLiteral("EnableHiDPI"), Default::Wayland::EnableHiDPI).toBool();
    wSessionCommand_ = settings.value(QStringLiteral("SessionCommand"), Default::Wayland::SessionCommand).toString();
    wSessionDir_ = settings.value(QStringLiteral("SessionDir"), Default::Wayland::SessionDir).toString();
    wSessionLogFile_ = settings.value(QStringLiteral("SessionLogFile"), Default::Wayland::SessionLogFile).toString();
    settings.endGroup();

    settings.beginGroup("X11");
    xDisplayCommand_ = settings.value(QStringLiteral("DisplayCommand"), Default::X11::DisplayCommand).toString();
    xDisplayStopCommand_ = settings.value(QStringLiteral("DisplayStopCommand"), Default::X11::DisplayStopCommand).toString();
    xEnableHiDPI_ = settings.value(QStringLiteral("EnableHiDPI"), Default::X11::EnableHiDPI).toBool();
    xMinimumVT_ = settings.value(QStringLiteral("MinimumVT"), Default::X11::MinimumVT).toInt();
    xServerArguments_ = settings.value(QStringLiteral("ServerArguments"), Default::X11::ServerArguments).toString();
    xServerPath_ = settings.value(QStringLiteral("ServerPath"), Default::X11::ServerPath).toString();
    xSessionCommand_ = settings.value(QStringLiteral("SessionCommand"), Default::X11::SessionCommand).toString();
    xSessionDir_ = settings.value(QStringLiteral("SessionDir"), Default::X11::SessionDir).toString();
    xSessionLogFile_ = settings.value(QStringLiteral("SessionLogFile"), Default::X11::SessionLogFile).toString();
    xUserAuthFile_ = settings.value(QStringLiteral("UserAuthFile"), Default::X11::UserAuthFile).toString();
    xAuthPath_ = settings.value(QStringLiteral("XauthPath"), Default::X11::XauthPath).toString();
    xephyrPath_ = settings.value(QStringLiteral("XephyrPath"), Default::X11::XephyrPath).toString();
    settings.endGroup();
}

void Settings::save()
{
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        qDebug() << "Error while creating the temporary directory.";
        return;
    }
    QString tempFilePath = tempDir.path() + "/sddm.conf";
    QSettings settings(tempFilePath, QSettings::IniFormat);

    settings.beginGroup("Autologin");
    settings.setValue(QStringLiteral("Relogin"), relogin_);
    settings.setValue(QStringLiteral("Session"), session_);
    settings.setValue(QStringLiteral("User"), user_);
    settings.endGroup();

    //  settings.beginGroup("General");
    settings.setValue(QStringLiteral("HaltCommand"), haltCommand_);
    settings.setValue(QStringLiteral("InputMethod"), inputMethod_);
    settings.setValue(QStringLiteral("Namespaces"), namespaces_);
    settings.setValue(QStringLiteral("Numlock"), numlock_);
    settings.setValue(QStringLiteral("RebootCommand"), rebootCommand_);
    //  settings.endGroup();

    settings.beginGroup("Theme");
    settings.setValue(QStringLiteral("Current"), currentTheme_);
    settings.setValue(QStringLiteral("CursorTheme"), cursorTheme_);
    settings.setValue(QStringLiteral("DisableAvatarsThreshold"), avatarsThreshold_);
    settings.setValue(QStringLiteral("EnableAvatars"), enableAvatars_);
    settings.setValue(QStringLiteral("FacesDir"), facesDir_);
    settings.setValue(QStringLiteral("Font"), font_);
    settings.setValue(QStringLiteral("ThemeDir"), themeDir_);
    settings.endGroup();

    settings.beginGroup("Users");
    settings.setValue(QStringLiteral("DefaultPath"), defaultPath_);
    settings.setValue(QStringLiteral("HideShells"), hideShells_);
    settings.setValue(QStringLiteral("HideUsers"), hideUsers_);
    settings.setValue(QStringLiteral("MaximumUid"), uidMax_);
    settings.setValue(QStringLiteral("MinimumUid"), uidMin_);
    settings.setValue(QStringLiteral("RememberLastSession"), rememberLastSession_);
    settings.setValue(QStringLiteral("RememberLastUser"), rememberLastUser_);
    settings.setValue(QStringLiteral("ReuseSession"), reuseSession_);
    settings.endGroup();

    settings.beginGroup("Wayland");
    settings.setValue(QStringLiteral("EnableHiDPI"), wEnableHiDPI_);
    settings.setValue(QStringLiteral("SessionCommand"), wSessionCommand_);
    settings.setValue(QStringLiteral("SessionDir"), wSessionDir_);
    settings.setValue(QStringLiteral("SessionLogFile"), wSessionLogFile_);
    settings.endGroup();

    settings.beginGroup("X11");
    settings.setValue(QStringLiteral("DisplayCommand"), xDisplayCommand_);
    settings.setValue(QStringLiteral("DisplayStopCommand"), xDisplayStopCommand_);
    settings.setValue(QStringLiteral("EnableHiDPI"), xEnableHiDPI_);
    settings.setValue(QStringLiteral("MinimumVT"), xMinimumVT_);
    settings.setValue(QStringLiteral("ServerArguments"), xServerArguments_);
    settings.setValue(QStringLiteral("ServerPath"), xServerPath_);
    settings.setValue(QStringLiteral("SessionCommand"), xSessionCommand_);
    settings.setValue(QStringLiteral("SessionDir"), xSessionDir_);
    settings.setValue(QStringLiteral("SessionLogFile"), xSessionLogFile_);
    settings.setValue(QStringLiteral("UserAuthFile"), xUserAuthFile_);
    settings.setValue(QStringLiteral("XauthPath"), xAuthPath_);
    settings.setValue(QStringLiteral("XephyrPath"), xephyrPath_);
    settings.endGroup();

    settings.sync();

    QFile settingsFile(tempFilePath);
    if (!settingsFile.setPermissions(
            QFile::ReadOwner | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOther)) {
        qDebug() << "Error while setting configuration file permissions.";
        return;
    }
    QProcess process;
    process.start("pkexec", QStringList() << "cp" << tempFilePath << path_);
    process.waitForFinished(-1);
}
