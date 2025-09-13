
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "render.h"

#include <QGuiApplication>
#include <QLabel>
#include <QList>
#include <QPolygon>
#include <QScreen>
#include <QWidget>

// this constructor creates the widget where the crosshair is rendered on screen.
// its initially centered on the first system screen. it pulls the m_opt ref
// from the MainWindow Constructor to access the settings currentScreenIndex
CrosshairRenderer::CrosshairRenderer(Config::Options &opt) : QWidget(), m_opt(opt)
{
    // make sure the window is transparent and click thru
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool | Qt::WindowTransparentForInput |
                   Qt::BypassWindowManagerHint);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);

    setFixedSize(200, 200);

    // the QPixmap is rendered in this QLabel
    label = new QLabel(this);
    label->setGeometry(0, 0, width(), height());
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QList<QScreen *> screens = QGuiApplication::screens();

    // this part of code gets the prefered screen from settings and centeres
    // the CrosshairRenderer QWidget on it.
    if (!screens.isEmpty())
    {
        // make sure currentScreenIndex < screen amount to
        // prevent runtime errors. also prevent <0 values.
        if (m_opt.currentScreenIndex >= screens.size())
            m_opt.currentScreenIndex = screens.size() - 1;
        else if (m_opt.currentScreenIndex < 0)
            m_opt.currentScreenIndex = 0;

        QRect screenGeometry = screens[m_opt.currentScreenIndex]->geometry();
        int cx = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
        int cy = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

        move(cx, cy);
    }
}

// this function loops thru the available monitors and centers the
// crosshair on them until the user found the prefered monitor
void CrosshairRenderer::cycleScreen()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.isEmpty())
        return;

    m_opt.currentScreenIndex = (m_opt.currentScreenIndex + 1) % screens.size();

    QRect screenGeometry = screens[m_opt.currentScreenIndex]->geometry();
    int cx = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
    int cy = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

    move(cx, cy);
}

// updates the position of the crosshair to match the
// position from the settings
void CrosshairRenderer::update()
{
    QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.isEmpty())
        return;

    QRect screenGeometry = screens[m_opt.currentScreenIndex]->geometry();
    int cx = screenGeometry.x() + (screenGeometry.width() - width()) / 2;
    int cy = screenGeometry.y() + (screenGeometry.height() - height()) / 2;

    move(cx, cy);
}