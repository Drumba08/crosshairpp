
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include "config.h"
#include <QColor>
#include <QImage>
#include <QPainterPath>
#include <QPixmap>
#include <QPoint>
#include <QSize>

namespace Crosshair
{

QPainterPath buildPath(const Config &opt, const QSize &canvas);

QPixmap render(const Config &opt);

QPixmap renderShadow(const QImage &base, const Config &opt);

} // namespace Crosshair