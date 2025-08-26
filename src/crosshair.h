
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtGui/QColor>
#include <QtGui/QPainterPath>
#include <QtGui/QPixmap>

namespace Crosshair
{

struct Options
{
    // Crosshair lines
    int length = 16;
    int gap = 8;
    int thickness = 2;
    QColor color = QColor(255, 255, 255);

    // Center dot
    bool dot = true;
    int dotSize = 4;

    // Shadow
    bool shadow = true;
    QColor shadowColor = QColor(0, 0, 0, 255);
    int shadowBlurRadius = 6;
    QPoint shadowOffset = QPoint(0, 0);

    // Padding / DPI
    int padding = 12;
    qreal devicePixelRatio = 1.0;
    qreal supersample = 1.0;
};

QPainterPath buildPath(const Options &opt, const QSize &canvas);

QPixmap render(const Options &opt);

}