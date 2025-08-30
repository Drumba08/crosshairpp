
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include <QWidget>
#include <QSystemTrayIcon>

#include "crosshair.h"
#include "render.h"
#include "ui_preset.h"

class MainWindow : public QWidget
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

    void setupTray();

    void render();

    QString updateCode();

    void applyCode(QString &code);

    void updateUi();

    void resetConfig();
    
    void showConfig();

    void loadConfig();

    void saveConfig();

    void setupTrayConnections();

    void setupConnections();

  private:
    Ui::MainWindow ui;
    Crosshair::Options m_opt;

    CrosshairRenderer cr;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *restoreAction;
    QAction *quitAction;

    QPointF dragPosition;
    bool mouseDown = false;

    void closeEvent(QCloseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
};