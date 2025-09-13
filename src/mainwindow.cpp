
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "mainwindow.h"

#include "ccode.h"
#include "config.h"
#include "crosshair.h"
#include "render.h"
#include "util.h"
#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPointF>
#include <QSettings>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QWidget>

// main window constructor
MainWindow::MainWindow(Config &cfg) : QWidget(), m_config(cfg), crosshairRenderer(m_config)
{
    // this loads the ui compiled by uic
    ui.setupUi(this);

    // required window flags
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    // generate code and
    // render crosshair
    updateUi();
}

void MainWindow::setup()
{
    // sets all the ui components to the settings
    m_config.showConfig(ui);
    crosshairRenderer.show();

    // if the program is started for the first time,
    // show a nice welcome dialogue
    if (m_config.firstTime)
    {
        util::welcomeDialogue();

        m_config.firstTime = false;
        m_config.saveConfig();
    }

    // add tray and the connections for quitting
    // and restoring the settings page
    setupTray();

    // connections for
    // buttons, tray etc...
    setupTrayConnections();
    setupConnections();
}

// hooks main window closeEvent to prevent the app
// from shutting down and hides the window instead
void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

// the following functions are very interesting, because
// they are making the windows header bar draggable by
// detecting when the user is holding the mouse on the
// header bar and is dragging it across the screen
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (ui.header->geometry().contains(event->pos()) && event->button() == Qt::LeftButton)
    {
        mouseDown = true;
        dragPosition = event->globalPosition() - frameGeometry().topLeft();
    }
    else
    {
        mouseDown = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseDown && (event->buttons() & Qt::LeftButton))
    {
        QPointF newPos = event->globalPosition() - dragPosition;
        move(newPos.toPoint());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    dragPosition = QPointF();
    mouseDown = false;
}

// here we setup a system tray
// where we can later open or
// quit the program
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

// connect the tray actions to program logic
void MainWindow::setupTrayConnections()
{
    connect(restoreAction, &QAction::triggered, this, [this]() {
        this->showNormal();
        this->raise();
        this->activateWindow();
    });

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

// this functions takes the crosshair settings
// and renders them into a crosshair on screen
// you want to call this after changing settings
void MainWindow::render()
{
    // render the crosshair
    crosshairRenderer.label->setPixmap(Crosshair::render(m_config));

    // show only if enabled
    if (m_config.enabled)
    {
        crosshairRenderer.show();
    }
    else
    {
        crosshairRenderer.hide();
    }
}

// this function refreshes the shown crossharCode
// and also renders and displays the crosshair again
void MainWindow::updateUi()
{
    QString crosshairCode = ccode::generateCode(m_config);
    ui.i_crosshairCode->setText(crosshairCode);
    render();
}

// logic for all the buttons. the changes on the crosshair options get written to settings,
// and a save is triggered. also links sliders to their matching QSpinBox to ensure sync.
void MainWindow::setupConnections()
{
    // crosshair code LineEdit change handler
    connect(ui.i_crosshairCode, &QLineEdit::textEdited, this, [this](QString value) {
        ccode::applyCode(value, m_config);
        m_config.clamp();

        m_config.saveConfig();
        m_config.showConfig(ui);
        crosshairRenderer.show();
        render();
    });

    // settings exit button
    connect(ui.i_exit, &QPushButton::clicked, this, [this]() { this->hide(); });

    // screen cycle button
    connect(ui.i_cycleScreen, &QPushButton::clicked, &crosshairRenderer, &CrosshairRenderer::cycleScreen);

    // reset config button
    connect(ui.i_resetConf, &QPushButton::clicked, this, [this]() {
        m_config.resetConfig();

        updateUi();
        m_config.saveConfig();
        m_config.showConfig(ui);
        crosshairRenderer.show();

        render();
        crosshairRenderer.update();
    });

    // color button
    connect(ui.i_changeColor, &QPushButton::clicked, this, [this]() {
        m_config.color = QColorDialog::getColor(m_config.color, this, "Select Color");
        updateUi();
        m_config.saveConfig();
    });

    // enable checkmark
    connect(ui.i_enableCrosshair, &QCheckBox::toggled, this, [this](bool value) {
        m_config.enabled = value;
        render();
        m_config.saveConfig();
    });

    // crosshair length
    connect(ui.i_length, &QSlider::valueChanged, this, [this](int value) {
        m_config.length = value;
        ui.i_length_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // crosshair thickness
    connect(ui.i_thickness, &QSlider::valueChanged, this, [this](int value) {
        m_config.thickness = value;
        ui.i_thickness_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // crosshair gap
    connect(ui.i_gap, &QSlider::valueChanged, this, [this](int value) {
        m_config.gap = value;
        ui.i_gap_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // crosshair dot enabled
    connect(ui.i_dotEnabled, &QCheckBox::toggled, this, [this](bool value) {
        m_config.dot = value;
        updateUi();
        m_config.saveConfig();
    });

    // crosshair dot size
    connect(ui.i_dotSize, &QSlider::valueChanged, this, [this](int value) {
        m_config.dotSize = value;
        ui.i_dotSize_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // crosshair shadow enabled
    connect(ui.i_shadow, &QCheckBox::toggled, this, [this](bool value) {
        m_config.shadow = value;
        updateUi();
        m_config.saveConfig();
    });

    // crosshair shadow radius
    connect(ui.i_shadowradius, &QSlider::valueChanged, this, [this](int value) {
        m_config.shadowBlurRadius = value;
        ui.i_shadowradius_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // crosshair shadow alpha
    connect(ui.i_shadowalpha, &QSlider::valueChanged, this, [this](int value) {
        m_config.shadowColor = QColor(0, 0, 0, value);
        ui.i_shadowalpha_2->setValue(value);
        updateUi();
        m_config.saveConfig();
    });

    // here we connect the QSpinBox widgets to the slider so if the spinbox changes it also applies to the slider
    connect(ui.i_length_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_length, &QSlider::setValue);

    connect(ui.i_thickness_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_thickness, &QSlider::setValue);

    connect(ui.i_gap_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_gap, &QSlider::setValue);

    connect(ui.i_dotSize_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_dotSize, &QSlider::setValue);

    connect(ui.i_shadowradius_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowradius, &QSlider::setValue);

    connect(ui.i_shadowalpha_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowalpha, &QSlider::setValue);
}
