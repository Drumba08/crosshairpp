
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "render.h"
#include "QScreen"
#include "qguiapplication.h"
#include "qlabel.h"
#include "qlist.h"
#include "qnamespace.h"
#include "qpolygon.h"
#include "qscreen.h"

CrosshairRenderer::CrosshairRenderer()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(100, 100);

    label = new QLabel(this);
    label->setGeometry(0, 0, width(), height());
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QList<QScreen *> screens = QGuiApplication::screens();

    if (!screens.isEmpty())
    {
        QRect screenGeometry = screens[currentScreenIndex]->geometry();
        int cx = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
        int cy = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

        move(cx, cy);
    }
}

void CrosshairRenderer::cycleScreen()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.isEmpty())
        return;

    currentScreenIndex = (currentScreenIndex + 1) % screens.size();

    QRect screenGeometry = screens[currentScreenIndex]->geometry();
    int cx = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
    int cy = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

    move(cx, cy);
}