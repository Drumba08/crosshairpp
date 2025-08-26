
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "mainwindow.h"
#include "qaction.h"
#include "qcheckbox.h"
#include "qcolordialog.h"
#include "qsettings.h"
#include "qsystemtrayicon.h"
#include "render.h"
#include <QCloseEvent>
#include <QMenu>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    setupTray();
    setupTrayConnections();

    loadConfig();

    setupConnections();
    render();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void MainWindow::setupTray()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(window()->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("Crosshair++");

    trayMenu = new QMenu(this);
    restoreAction = new QAction("Open", this);
    quitAction = new QAction("Quit", this);

    trayMenu->addAction(restoreAction);
    trayMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::loadConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    int alpha = settings.value("crosshair/shadowAlpha", 255).toInt();

    m_opt.enabled = settings.value("crosshair/enabled", true).toBool();
    m_opt.color = settings.value("crosshair/color", QColor(255, 255, 255)).value<QColor>();
    m_opt.length = settings.value("crosshair/length", 16).toInt();
    m_opt.thickness = settings.value("crosshair/thickness", 2).toInt();
    m_opt.gap = settings.value("crosshair/gap", 8).toInt();
    m_opt.dot = settings.value("crosshair/dotEnabled", true).toBool();
    m_opt.dotSize = settings.value("crosshair/dotSize", 4).toInt();
    m_opt.shadow = settings.value("crosshair/shadowEnabled", true).toBool();
    m_opt.shadowBlurRadius = settings.value("crosshair/shadowRadius", 4).toInt();
    m_opt.shadowColor = QColor(0, 0, 0, alpha);

    ui.i_enableCrosshair->setChecked(m_opt.enabled);

    ui.i_length->setValue(m_opt.length);
    ui.i_thickness->setValue(m_opt.thickness);
    ui.i_gap->setValue(m_opt.gap);
    ui.i_dotEnabled->setChecked(m_opt.dot);
    ui.i_dotSize->setValue(m_opt.dotSize);
    ui.i_shadow->setChecked(m_opt.shadow);
    ui.i_shadowradius->setValue(m_opt.shadowBlurRadius);
    ui.i_shadowalpha->setValue(alpha);

    ui.i_length_2->setValue(m_opt.length);
    ui.i_thickness_2->setValue(m_opt.thickness);
    ui.i_gap_2->setValue(m_opt.gap);
    ui.i_dotEnabled->setChecked(m_opt.dot);
    ui.i_dotSize_2->setValue(m_opt.dotSize);
    ui.i_shadow->setChecked(m_opt.shadow);
    ui.i_shadowradius_2->setValue(m_opt.shadowBlurRadius);
    ui.i_shadowalpha_2->setValue(alpha);

    ui.crossPreview->setPixmap(Crosshair::render(m_opt));

    if (m_opt.enabled)
    {
        cr.show();
    }
}

void MainWindow::saveConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    settings.setValue("crosshair/enabled", m_opt.enabled);
    settings.setValue("crosshair/color", m_opt.color);
    settings.setValue("crosshair/length", m_opt.length);
    settings.setValue("crosshair/thickness", m_opt.thickness);
    settings.setValue("crosshair/gap", m_opt.gap);
    settings.setValue("crosshair/dotEnabled", m_opt.dot);
    settings.setValue("crosshair/dotSize", m_opt.dotSize);
    settings.setValue("crosshair/shadowEnabled", m_opt.shadow);
    settings.setValue("crosshair/shadowRadius", m_opt.shadowBlurRadius);
    settings.setValue("crosshair/shadowAlpha", m_opt.shadowColor.alpha());
}

void MainWindow::setupTrayConnections()
{
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    connect(trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
            if (isHidden())
            {
                show();
                raise();
            }
    });
}

void MainWindow::render()
{
    // render the preview and main element
    ui.crossPreview->setPixmap(Crosshair::render(m_opt));
    cr.label->setPixmap(Crosshair::render(m_opt));
}

void MainWindow::setupConnections()
{
    // screen cycle button
    connect(ui.i_cycleScreen, &QPushButton::clicked, &cr, &CrosshairRenderer::cycleScreen);

    // color button
    connect(ui.i_changeColor, &QPushButton::clicked, this, [this]() {
        m_opt.color = QColorDialog::getColor(m_opt.color, this, "Select Color");
        render();
        saveConfig();
    });

    // enable checkmark
    connect(ui.i_enableCrosshair, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.enabled = value;

        if (value)
        {
            cr.show();
        }
        else
        {
            cr.hide();
        }

        saveConfig();
    });

    // crosshair length
    connect(ui.i_length, &QSlider::valueChanged, this, [this](int value) {
        m_opt.length = value;
        ui.i_length_2->setValue(value);
        render();
        saveConfig();
    });

    // crosshair thickness
    connect(ui.i_thickness, &QSlider::valueChanged, this, [this](int value) {
        m_opt.thickness = value;
        ui.i_thickness_2->setValue(value);
        render();
        saveConfig();
    });

    // crosshair gap
    connect(ui.i_gap, &QSlider::valueChanged, this, [this](int value) {
        m_opt.gap = value;
        ui.i_gap_2->setValue(value);
        render();
        saveConfig();
    });

    // crosshair dot enabled
    connect(ui.i_dotEnabled, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.dot = value;
        render();
        saveConfig();
    });

    // crosshair dot size
    connect(ui.i_dotSize, &QSlider::valueChanged, this, [this](int value) {
        m_opt.dotSize = value;
        ui.i_dotSize_2->setValue(value);
        render();
        saveConfig();
    });

    // crosshair shadow enabled
    connect(ui.i_shadow, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.shadow = value;
        render();
        saveConfig();
    });

    // crosshair shadow radius
    connect(ui.i_shadowradius, &QSlider::valueChanged, this, [this](int value) {
        m_opt.shadowBlurRadius = value;
        ui.i_shadowradius_2->setValue(value);
        render();
        saveConfig();
    });

    // crosshair shadow alpha
    connect(ui.i_shadowalpha, &QSlider::valueChanged, this, [this](int value) {
        m_opt.shadowColor = QColor(0, 0, 0, value);
        ;
        ui.i_shadowalpha_2->setValue(value);
        render();
        saveConfig();
    });

    connect(ui.i_length_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_length, &QSlider::setValue);

    connect(ui.i_thickness_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_thickness, &QSlider::setValue);

    connect(ui.i_gap_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_gap, &QSlider::setValue);

    connect(ui.i_dotSize_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_dotSize, &QSlider::setValue);

    connect(ui.i_shadowradius_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowradius, &QSlider::setValue);

    connect(ui.i_shadowalpha_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowalpha, &QSlider::setValue);
}
