
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include "config.h"
#include "crosshair.h"
#include <QLabel>
#include <QWidget>

class CrosshairRenderer : public QWidget
{
    Q_OBJECT

  public:
    CrosshairRenderer(Config::Options &opt);

    void update();

    QLabel *label;
  public slots:
    void cycleScreen();

  private:
    Config::Options &m_opt;
};