
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include <QMainWindow>
#include <QSystemTrayIcon>

#include "crosshair.h"
#include "ui_preset.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

    void setupTray();

    void loadConfig();

    void saveConfig();

    void setupTrayConnections();

    void setupConnections();

  private:
    Ui::MainWindow ui;
    Crosshair::Options m_opt;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *restoreAction;
    QAction *quitAction;

    void closeEvent(QCloseEvent *event);
};