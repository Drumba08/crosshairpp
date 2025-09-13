
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include "ui_preset.h"
#include <QColor>

class Config
{
  public:
    bool enabled = true;
    bool firstTime = true;
    QColor color = QColor(255, 255, 255);
    int length = 8;
    int gap = 32;
    int thickness = 2;
    bool dot = true;
    int dotSize = 4;
    bool shadow = true;
    int shadowBlurRadius = 3;
    QColor shadowColor = QColor(0, 0, 0, 255);
    int currentScreenIndex = 0;
    qreal devicePixelRatio = 1.0;
    qreal supersample = 1.0;

    void resetConfig();

    void loadConfig();

    void showConfig(Ui::MainWindow &ui);

    void saveConfig();

    void clamp();
};