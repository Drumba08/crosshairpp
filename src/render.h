
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include <QLabel>
#include "qwidget.h"

#include "crosshair.h"

class CrosshairRenderer : public QWidget
{
    Q_OBJECT

  public:
    CrosshairRenderer(Crosshair::Options &opt);

    void Paint();

    QLabel *label;
  public slots:
    void cycleScreen();

  private:
    Crosshair::Options &m_opt;
};