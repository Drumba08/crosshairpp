
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include <QAction>
#include <QCheckBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QStyle>
#include <QSystemTrayIcon>

#include "crosshair.h"
#include "mainwindow.h"
#include "render.h"

// main window constructor. important here is to init QMainWindow and the
// CrosshairRenderer cr
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), cr(m_opt)
{
    ui.setupUi(this);

    setupTray();
    setupTrayConnections();

    loadConfig();
    m_opt.clamp();

    showConfig();

    if (m_opt.firstTime)
    {
        QMessageBox::information(nullptr, "Welcome", "hi");

        m_opt.firstTime = false;
        saveConfig();
    }

    this->show();

    setupConnections();
    updateUi();
}

// hooks main window closeEvent to prevent the app
// from shutting down and hides the window instead
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

// restores the default config in memory.
// DOES NOT write the changes to disk on its own
void MainWindow::resetConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    Crosshair::Options defaultOptions;

    m_opt.enabled = defaultOptions.enabled;
    m_opt.color = defaultOptions.color;
    m_opt.length = defaultOptions.length;
    m_opt.thickness = defaultOptions.thickness;
    m_opt.gap = defaultOptions.gap;
    m_opt.dot = defaultOptions.dot;
    m_opt.dotSize = defaultOptions.dotSize;
    m_opt.shadow = defaultOptions.shadow;
    m_opt.shadowBlurRadius = defaultOptions.shadowBlurRadius;
    m_opt.shadowColor = defaultOptions.shadowColor;

    m_opt.currentScreenIndex = defaultOptions.currentScreenIndex;
}

// reads the saved config on program startup
// from config file / Win Registry
void MainWindow::loadConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    Crosshair::Options defaultOptions;

    int alpha = settings.value("crosshair/shadowAlpha", defaultOptions.shadowColor.alpha()).toInt();

    m_opt.firstTime = settings.value("crosshair/firstTime", defaultOptions.firstTime).toBool();

    m_opt.enabled = settings.value("crosshair/enabled", defaultOptions.enabled).toBool();
    m_opt.color = settings.value("crosshair/color", defaultOptions.color).value<QColor>();
    m_opt.length = settings.value("crosshair/length", defaultOptions.length).toInt();
    m_opt.thickness = settings.value("crosshair/thickness", defaultOptions.thickness).toInt();
    m_opt.gap = settings.value("crosshair/gap", defaultOptions.gap).toInt();
    m_opt.dot = settings.value("crosshair/dotEnabled", defaultOptions.dot).toBool();
    m_opt.dotSize = settings.value("crosshair/dotSize", defaultOptions.dotSize).toInt();
    m_opt.shadow = settings.value("crosshair/shadowEnabled", defaultOptions.shadow).toBool();
    m_opt.shadowBlurRadius = settings.value("crosshair/shadowRadius", defaultOptions.shadowBlurRadius).toInt();
    m_opt.shadowColor = QColor(0, 0, 0, alpha);

    m_opt.currentScreenIndex =
        settings.value("crosshair/currentScreenIndex", defaultOptions.currentScreenIndex).toBool();
}

// loads the config from memory into all
// the widgets to display
void MainWindow::showConfig()
{
    ui.i_enableCrosshair->setChecked(m_opt.enabled);

    ui.i_length->setValue(m_opt.length);
    ui.i_thickness->setValue(m_opt.thickness);
    ui.i_gap->setValue(m_opt.gap);
    ui.i_dotEnabled->setChecked(m_opt.dot);
    ui.i_dotSize->setValue(m_opt.dotSize);
    ui.i_shadow->setChecked(m_opt.shadow);
    ui.i_shadowradius->setValue(m_opt.shadowBlurRadius);
    ui.i_shadowalpha->setValue(m_opt.shadowColor.alpha());

    ui.i_length_2->setValue(m_opt.length);
    ui.i_thickness_2->setValue(m_opt.thickness);
    ui.i_gap_2->setValue(m_opt.gap);
    ui.i_dotEnabled->setChecked(m_opt.dot);
    ui.i_dotSize_2->setValue(m_opt.dotSize);
    ui.i_shadow->setChecked(m_opt.shadow);
    ui.i_shadowradius_2->setValue(m_opt.shadowBlurRadius);
    ui.i_shadowalpha_2->setValue(m_opt.shadowColor.alpha());

    if (m_opt.enabled)
    {
        cr.show();
    }
}

// save current config to disk / Win Registry
// should be called on every settings change
void MainWindow::saveConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    m_opt.clamp();

    settings.setValue("crosshair/firstTime", m_opt.firstTime);

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

    settings.setValue("crosshair/currentScreenIndex", m_opt.currentScreenIndex);
}

// connect the tray actions to program logic
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

// helper that renders and displays
// the crosshair in preview and on screen
void MainWindow::render()
{
    // render the preview and main element
    ui.crossPreview->setPixmap(Crosshair::render(m_opt));
    cr.label->setPixmap(Crosshair::render(m_opt));

    // show only if enabled
    if (m_opt.enabled)
    {
        cr.show();
    }
    else
    {
        cr.hide();
    }
}

// this function updates the ui after an user input
// that changed any settings / configuration
void MainWindow::updateUi()
{
    QString crosshairCode = updateCode();
    ui.i_crosshairCode->setText(crosshairCode);
    render();
}

// this function generates a crosshair code
// encoding user changable settings
QString MainWindow::updateCode()
{
    QString code;

    code += QString("%1;").arg(m_opt.enabled ? 1 : 0);
    code += QString("%1;").arg(m_opt.color.red());
    code += QString("%1;").arg(m_opt.color.green());
    code += QString("%1;").arg(m_opt.color.blue());
    code += QString("%1;").arg(m_opt.length);
    code += QString("%1;").arg(m_opt.gap);
    code += QString("%1;").arg(m_opt.thickness);
    code += QString("%1;").arg(m_opt.dot ? 1 : 0);
    code += QString("%1;").arg(m_opt.dotSize);
    code += QString("%1;").arg(m_opt.shadow ? 1 : 0);
    code += QString("%1;").arg(m_opt.shadowBlurRadius);
    code += QString("%1").arg(m_opt.shadowColor.alpha());

    return code;
}

// takes a crosshair code and applys it
// to settings if its valid
void MainWindow::applyCode(QString &code)
{
    QStringList parts = code.split(';');

    if (parts.size() != 12)
    {
        qWarning() << "invalid code: " << code;
        return;
    }

    bool ok;

    // enabled
    int tempInt = parts[0].toInt(&ok);
    if (ok)
        m_opt.enabled = (tempInt != 0);

    // color
    int r = parts[1].toInt(&ok);
    if (!ok)
        r = m_opt.color.red();

    int g = parts[2].toInt(&ok);
    if (!ok)
        g = m_opt.color.green();

    int b = parts[3].toInt(&ok);
    if (!ok)
        b = m_opt.color.blue();

    m_opt.color.setRgb(r, g, b);

    // length
    tempInt = parts[4].toInt(&ok);
    if (ok)
        m_opt.length = tempInt;

    // gap
    tempInt = parts[5].toInt(&ok);
    if (ok)
        m_opt.gap = tempInt;

    // thickness
    tempInt = parts[6].toInt(&ok);
    if (ok)
        m_opt.thickness = tempInt;

    // dot
    tempInt = parts[7].toInt(&ok);
    if (ok)
        m_opt.dot = (tempInt != 0);

    // dotSize
    tempInt = parts[8].toInt(&ok);
    if (ok)
        m_opt.dotSize = tempInt;

    // shadow
    tempInt = parts[9].toInt(&ok);
    if (ok)
        m_opt.shadow = (tempInt != 0);

    // shadowBlurRadius
    tempInt = parts[10].toInt(&ok);
    if (ok)
        m_opt.shadowBlurRadius = tempInt;

    // shadowColor alpha
    tempInt = parts[11].toInt(&ok);
    if (ok)
    {
        QColor c = m_opt.shadowColor;
        c.setAlpha(tempInt);
        m_opt.shadowColor = c;
    }
}

// logic for all the buttons. the changes on the crosshair options get written to settings,
// and a save is triggered. also links sliders to their matching QSpinBox to ensure sync.
void MainWindow::setupConnections()
{
    // crosshair code LineEdit change handler
    connect(ui.i_crosshairCode, &QLineEdit::textEdited, this, [this](QString value) {
        applyCode(value);
        m_opt.clamp();
        saveConfig();
        render();
    });

    // screen cycle button
    connect(ui.i_cycleScreen, &QPushButton::clicked, &cr, &CrosshairRenderer::cycleScreen);

    // reset config button
    connect(ui.i_resetConf, &QPushButton::clicked, this, [this]() {
        resetConfig();

        updateUi();
        saveConfig();
        showConfig();

        render();
        cr.update();
    });

    // color button
    connect(ui.i_changeColor, &QPushButton::clicked, this, [this]() {
        m_opt.color = QColorDialog::getColor(m_opt.color, this, "Select Color");
        updateUi();
        saveConfig();
    });

    // enable checkmark
    connect(ui.i_enableCrosshair, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.enabled = value;
        render();
        saveConfig();
    });

    // crosshair length
    connect(ui.i_length, &QSlider::valueChanged, this, [this](int value) {
        m_opt.length = value;
        ui.i_length_2->setValue(value);
        updateUi();
        saveConfig();
    });

    // crosshair thickness
    connect(ui.i_thickness, &QSlider::valueChanged, this, [this](int value) {
        m_opt.thickness = value;
        ui.i_thickness_2->setValue(value);
        updateUi();
        saveConfig();
    });

    // crosshair gap
    connect(ui.i_gap, &QSlider::valueChanged, this, [this](int value) {
        m_opt.gap = value;
        ui.i_gap_2->setValue(value);
        updateUi();
        saveConfig();
    });

    // crosshair dot enabled
    connect(ui.i_dotEnabled, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.dot = value;
        updateUi();
        saveConfig();
    });

    // crosshair dot size
    connect(ui.i_dotSize, &QSlider::valueChanged, this, [this](int value) {
        m_opt.dotSize = value;
        ui.i_dotSize_2->setValue(value);
        updateUi();
        saveConfig();
    });

    // crosshair shadow enabled
    connect(ui.i_shadow, &QCheckBox::toggled, this, [this](bool value) {
        m_opt.shadow = value;
        updateUi();
        saveConfig();
    });

    // crosshair shadow radius
    connect(ui.i_shadowradius, &QSlider::valueChanged, this, [this](int value) {
        m_opt.shadowBlurRadius = value;
        ui.i_shadowradius_2->setValue(value);
        updateUi();
        saveConfig();
    });

    // crosshair shadow alpha
    connect(ui.i_shadowalpha, &QSlider::valueChanged, this, [this](int value) {
        m_opt.shadowColor = QColor(0, 0, 0, value);
        ;
        ui.i_shadowalpha_2->setValue(value);
        updateUi();
        saveConfig();
    });

    connect(ui.i_length_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_length, &QSlider::setValue);

    connect(ui.i_thickness_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_thickness, &QSlider::setValue);

    connect(ui.i_gap_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_gap, &QSlider::setValue);

    connect(ui.i_dotSize_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_dotSize, &QSlider::setValue);

    connect(ui.i_shadowradius_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowradius, &QSlider::setValue);

    connect(ui.i_shadowalpha_2, QOverload<int>::of(&QSpinBox::valueChanged), ui.i_shadowalpha, &QSlider::setValue);
}
