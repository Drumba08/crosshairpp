
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include <QColor>
#include <QPainterPath>
#include <QPixmap>
#include <QPoint>
#include <QSize>

namespace Crosshair
{

struct Options
{
    bool enabled = true;

    bool firstTime = true;

    QColor color = QColor(255, 255, 255);

    int length = 8;
    int gap = 32;
    int thickness = 2;

    bool dot = true;
    int dotSize = 4;

    bool shadow = true;
    int shadowBlurRadius = 6;
    QColor shadowColor = QColor(0, 0, 0, 255);
    QPoint shadowOffset = QPoint(0, 0);

    int padding = 12;

    int currentScreenIndex = 0;

    qreal devicePixelRatio = 1.0;
    qreal supersample = 1.0;

    void clamp()
    {
        length = std::clamp(length, 1, 50);
        gap = std::clamp(gap, 0, 50);
        thickness = std::clamp(thickness, 1, 50);
        dotSize = std::clamp(dotSize, 0, 100);
        shadowBlurRadius = std::clamp(shadowBlurRadius, 0, 24);
        shadowColor.setAlpha(std::clamp(shadowColor.alpha(), 0, 255));
    }
};

// creates the QPainterPath for the main crosshair lines, so
// they can be used in the render function
QPainterPath buildPath(const Options &opt, const QSize &canvas);

// renders the crosshair lines with thicknes color
// shadow and the centerdot to a QPixmap
QPixmap render(const Options &opt);

} // namespace Crosshair