/*
    Copyright (c) 2022 Andrea Zanellato
    SPDX-License-Identifier: MIT
*/
#pragma once

#include <QString>

class Settings {

public:
    Settings();

    void load();
    void save();

    QString username() const { return user_; }
    void setUsername(const QString& v) { user_ = v; }

    QString sessionFile() const { return session_; }
    void setSessionFile(const QString& v) { session_ = v; }

    QString haltCommand() const { return haltCommand_; }
    void setHaltCommand(const QString& v) { haltCommand_ = v; }

    QString inputMethod() const { return inputMethod_; }
    void setInputMethod(const QString& v) { inputMethod_ = v; }

    QString namespaces() const { return namespaces_; }
    void setNamespaces(const QString& v) { namespaces_ = v; }

    QString numlock() const { return numlock_; }
    void setNumlock(const QString& v) { numlock_ = v; }

    QString rebootCommand() const { return rebootCommand_; }
    void setRebootCommand(const QString& v) { rebootCommand_ = v; }

    QString currentTheme() const { return currentTheme_; }
    void setCurrentTheme(const QString& v) { currentTheme_ = v; }

    QString cursorTheme() const { return cursorTheme_; }
    void setCursorTheme(const QString& v) { cursorTheme_ = v; }

    QString facesDir() const { return facesDir_; }
    void setFacesDir(const QString& v) { facesDir_ = v; }

    QString font() const { return font_; }
    void setFont(const QString& v) { font_ = v; }

    QString themeDir() const { return themeDir_; }
    void setThemeDir(const QString& v) { themeDir_ = v; }

    QString defaultPath() const { return defaultPath_; }
    void setDefaultPath(const QString& v) { defaultPath_ = v; }

    QString hideShells() const { return hideShells_; }
    void setHideShells(const QString& v) { hideShells_ = v; }

    QString hideUsers() const { return hideUsers_; }
    void setHideUsers(const QString& v) { hideUsers_ = v; }

    QString waylandSessionCommand() const { return wSessionCommand_; }
    void setWaylandSessionCommand(const QString& v) { wSessionCommand_ = v; }

    QString waylandSessionDir() const { return wSessionDir_; }
    void setWaylandSessionDir(const QString& v) { wSessionDir_ = v; }

    QString waylandSessionLogFile() const { return wSessionLogFile_; }
    void setWaylandSessionLogFile(const QString& v) { wSessionLogFile_ = v; }

    QString x11DisplayCommand() const { return xDisplayCommand_; }
    void setX11DisplayCommand(const QString& v) { xDisplayCommand_ = v; }

    QString x11DisplayStopCommand() const { return xDisplayStopCommand_; }
    void setX11DisplayStopCommand(const QString& v) { xDisplayStopCommand_ = v; }

    QString x11ServerArguments() const { return xServerArguments_; }
    void setX11ServerArguments(const QString& v) { xServerArguments_ = v; }

    QString x11ServerPath() const { return xServerPath_; }
    void setX11ServerPath(const QString& v) { xServerPath_ = v; }

    QString x11SessionCommand() const { return xSessionCommand_; }
    void setX11SessionCommand(const QString& v) { xSessionCommand_ = v; }

    QString x11SessionDir() const { return xSessionDir_; }
    void setX11SessionDir(const QString& v) { xSessionDir_ = v; }

    QString x11SessionLogFile() const { return xSessionLogFile_; }
    void setX11SessionLogFile(const QString& v) { xSessionLogFile_ = v; }

    QString x11UserAuthFile() const { return xUserAuthFile_; }
    void setX11UserAuthFile(const QString& v) { xUserAuthFile_ = v; }

    QString xAuthPath() const { return xAuthPath_; }
    void setXauthPath(const QString& v) { xAuthPath_ = v; }

    QString xephyrPath() const { return xephyrPath_; }
    void setXephyrPath(const QString& v) { xephyrPath_ = v; }

    bool relogin() const { return relogin_; }
    void setRelogin(bool v) { relogin_ = v; }

    bool enableAvatars() const { return enableAvatars_; }
    void setEnableAvatars(bool v) { enableAvatars_ = v; }

    bool rememberLastSession() const { return rememberLastSession_; }
    void setRememberLastSession(bool v) { rememberLastSession_ = v; }

    bool rememberLastUser() const { return rememberLastUser_; }
    void setRememberLastUser(bool v) { rememberLastUser_ = v; }

    bool reuseSession() const { return reuseSession_; }
    void setReuseSession(bool v) { reuseSession_ = v; }

    bool waylandEnableHiDPI() const { return wEnableHiDPI_; }
    void setWaylandEnableHiDPI(bool v) { wEnableHiDPI_ = v; }

    bool x11EnableHiDPI() const { return xEnableHiDPI_; }
    void setX11EnableHiDPI(bool v) { xEnableHiDPI_ = v; }

    short avatarsThreshold() const { return avatarsThreshold_; }
    void setAvatarsThreshold(short v) { avatarsThreshold_ = v; }

    short x11MinimumVT() const { return xMinimumVT_; }
    void setX11MinimumVT(short v) { xMinimumVT_ = v; }

    int uidMin() const { return uidMin_; }
    void setUidMin(int v) { uidMin_ = v; }

    int uidMax() const { return uidMax_; }
    void setUidMax(int v) { uidMax_ = v; }

    QString path() const { return path_; }
    void setPath(const QString& path) { path_ = path; }

private:
    QString user_;
    QString session_;
    QString haltCommand_;
    QString inputMethod_;
    QString namespaces_;
    QString numlock_;
    QString rebootCommand_;
    QString currentTheme_;
    QString cursorTheme_;
    QString facesDir_;
    QString font_;
    QString themeDir_;
    QString defaultPath_;
    QString hideShells_;
    QString hideUsers_;
    QString wSessionCommand_;
    QString wSessionDir_;
    QString wSessionLogFile_;
    QString xDisplayCommand_;
    QString xDisplayStopCommand_;
    QString xServerArguments_;
    QString xServerPath_;
    QString xSessionCommand_;
    QString xSessionDir_;
    QString xSessionLogFile_;
    QString xUserAuthFile_;
    QString xAuthPath_;
    QString xephyrPath_;
    QString path_;
    bool relogin_;
    bool enableAvatars_;
    bool rememberLastSession_;
    bool rememberLastUser_;
    bool reuseSession_;
    bool wEnableHiDPI_;
    bool xEnableHiDPI_;
    short avatarsThreshold_;
    short xMinimumVT_;
    int uidMin_;
    int uidMax_;
};
