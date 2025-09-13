
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "crosshair.h"

#include "config.h"
#include <QGraphicsDropShadowEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QPointF>

namespace Crosshair
{

// creates the QPainterPath for the main crosshair lines,
// so it can be used in the render function
QPainterPath buildPath(const Config::Options &opt, const QSize &canvasSize)
{
    QPainterPath path;

    // canvas center
    qreal cx = canvasSize.width() / 2.0;
    qreal cy = canvasSize.height() / 2.0;

    // If thickness is odd, shift center by 0.5 to align with screen pixels
    // that prevents sub pixel anti aliasing mess or uneven lengths
    qreal shift = (opt.thickness % 2) ? 0.5 : 0.0;
    cx += shift;
    cy += shift;

    // canvas center c, gap g, line length l
    const QPointF c(cx, cy);
    const qreal g = opt.gap;
    const qreal L = opt.length;

    // helper lambda to add a line segment to the QPainterPath
    // path from QPointF a to QPointF b
    auto addSegment = [&](const QPointF &a, const QPointF &b) {
        path.moveTo(a);
        path.lineTo(b);
    };

    // calculate each lines position and length using the given center, length and gap properties
    addSegment(QPointF(c.x(), c.y() - g - L), QPointF(c.x(), c.y() - g));
    addSegment(QPointF(c.x(), c.y() + g), QPointF(c.x(), c.y() + g + L));
    addSegment(QPointF(c.x() - g - L, c.y()), QPointF(c.x() - g, c.y()));
    addSegment(QPointF(c.x() + g, c.y()), QPointF(c.x() + g + L, c.y()));

    return path;
}

// renders the crosshair lines with thicknes color
// shadow and the centerdot to a QPixmap
QPixmap render(const Config::Options &opt)
{
    // calculate canvas size
    const int size = (opt.length + opt.gap) * 2 + 100;
    const QSize canvasSize(size, size);

    QImage base(canvasSize, QImage::Format_ARGB32_Premultiplied);
    base.setDevicePixelRatio(opt.devicePixelRatio);
    base.fill(Qt::transparent);

    // Paint crosshair lines and center dot.
    {
        QPainter painter(&base);

        // enable anti aliasing to smooth the crosshair and center dot
        // and remove flickering on thickness change due to the subpixel
        // prevention in buildPath() func
        painter.setRenderHint(QPainter::Antialiasing, true);

        QPen pen(opt.color);
        pen.setWidth(opt.thickness);
        pen.setCapStyle(Qt::FlatCap);
        pen.setJoinStyle(Qt::MiterJoin);

        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        // generate the painter path using previous func
        QPainterPath path = buildPath(opt, canvasSize);
        painter.drawPath(path);

        // Draw center dot if enabled
        if (opt.dot && opt.dotSize > 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(opt.color);

            const QPointF c(base.width() / 2.0, base.height() / 2.0);
            const qreal r = opt.dotSize / 2.0;

            painter.drawEllipse(QRectF(c.x() - r, c.y() - r, 2 * r, 2 * r));
        }
    }

    // If shadow is disabled, we can return
    // the finished QPixmap here
    if (!opt.shadow)
    {
        return QPixmap::fromImage(base);
    }

    // else, we have to generate the shadow aswell
    QPixmap out = renderShadow(base, opt);
    return out;
}

// this function takes the rendered crosshair/dot and applies
// a QGraphicsDropShadowEffect using a QGraphicsScene
QPixmap renderShadow(const QImage &base, const Config::Options &opt)
{
    QGraphicsScene scene;

    // add the previous generated QImage to the scene
    QGraphicsPixmapItem *item = scene.addPixmap(QPixmap::fromImage(base));

    // create, configure and apply the QGraphicsDropShadowEffect
    auto *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(opt.shadowBlurRadius);
    effect->setOffset(QPointF(0.0, 0.0));
    effect->setColor(opt.shadowColor);
    item->setGraphicsEffect(effect);

    // add padding to avoid cutting off the shadow
    const qreal shadowPadding = opt.shadowBlurRadius + 2;
    QRectF bounds = item->boundingRect().adjusted(-shadowPadding, -shadowPadding, shadowPadding, shadowPadding);
    scene.setSceneRect(bounds);

    QImage out((bounds.size()).toSize(), QImage::Format_ARGB32_Premultiplied);
    out.setDevicePixelRatio(opt.devicePixelRatio);
    out.fill(Qt::transparent);

    QPainter p(&out);

    // anti aliasing for the shadow
    p.setRenderHint(QPainter::Antialiasing, true);

    scene.render(&p, QRectF(QPointF(0, 0), bounds.size()), bounds);
    p.end();

    return QPixmap::fromImage(out);
}

} // namespace Crosshair