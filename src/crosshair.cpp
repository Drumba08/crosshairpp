
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include <QImage>
#include <QPainter>
#include <QPen>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsScene>

#include "crosshair.h"

namespace Crosshair
{

// builds the draw path from settings
QPainterPath buildPath(const Options &opt, const QSize &canvas)
{
    QPainterPath path;

    const QPointF c(canvas.width() / 2.0, canvas.height() / 2.0);
    const qreal g = opt.gap;
    const qreal L = opt.length;

    auto addSegment = [&](const QPointF &a, const QPointF &b) {
        path.moveTo(a);
        path.lineTo(b);
    };

    addSegment(QPointF(c.x(), c.y() - g - L), QPointF(c.x(), c.y() - g)); // Up
    addSegment(QPointF(c.x(), c.y() + g), QPointF(c.x(), c.y() + g + L)); // Down
    addSegment(QPointF(c.x() - g - L, c.y()), QPointF(c.x() - g, c.y())); // Left
    addSegment(QPointF(c.x() + g, c.y()), QPointF(c.x() + g + L, c.y())); // Right

    return path;
}

QPixmap render(const Options &opt)
{
    const int size = (opt.length + opt.gap) * 2 + opt.padding * 2;
    const QSize canvasSize(size, size);

    // supersampled QImage buffer
    QImage base(canvasSize * opt.supersample, QImage::Format_ARGB32_Premultiplied);
    base.setDevicePixelRatio(opt.devicePixelRatio);
    base.fill(Qt::transparent);

    // Paint crosshair lines and center dot
    {
        QPainter painter(&base);
        painter.setRenderHint(QPainter::Antialiasing, true);

        QPen pen(opt.color);
        pen.setWidth(opt.thickness * opt.supersample);
        pen.setCapStyle(Qt::FlatCap);
        pen.setJoinStyle(Qt::MiterJoin);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        QPainterPath path = buildPath(opt, canvasSize * opt.supersample);
        painter.drawPath(path);

        // Draw center dot if enabled
        if (opt.dot && opt.dotSize > 0)
        {
            painter.setPen(Qt::NoPen);
            painter.setBrush(opt.color);

            const QPointF c(base.width() / 2.0, base.height() / 2.0);
            const qreal r = (opt.dotSize / 2.0) * opt.supersample;

            painter.drawEllipse(QRectF(c.x() - r, c.y() - r, 2 * r, 2 * r));
        }
    }

    // If shadow is disabled, downscale and return
    if (!opt.shadow)
    {
        return QPixmap::fromImage(base);
    }

    // Shadow enabled: use QGraphicsScene
    QGraphicsScene scene;
    QGraphicsPixmapItem *item = scene.addPixmap(QPixmap::fromImage(base));

    auto *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(opt.shadowBlurRadius * opt.supersample);
    effect->setOffset(opt.shadowOffset * opt.supersample);
    effect->setColor(opt.shadowColor);
    item->setGraphicsEffect(effect);

    // Adjust bounds to include shadow
    const qreal pad = opt.shadowBlurRadius * opt.supersample +
                      std::max(std::abs(opt.shadowOffset.x()), std::abs(opt.shadowOffset.y())) * opt.supersample +
                      2 * opt.supersample;

    QRectF bounds = item->boundingRect().adjusted(-pad, -pad, pad, pad);
    scene.setSceneRect(bounds);

    // Render final image
    QImage out((bounds.size()).toSize(), QImage::Format_ARGB32_Premultiplied);
    out.setDevicePixelRatio(opt.devicePixelRatio);
    out.fill(Qt::transparent);

    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    scene.render(&p, QRectF(QPointF(0, 0), bounds.size()), bounds);
    p.end();

    return QPixmap::fromImage(out);
}

}