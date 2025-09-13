
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "config.h"

#include "ui_preset.h"
#include <QSettings>

namespace Config
{

// restores the default config in memory.
// DOES NOT write the changes to disk on its own
void resetConfig(Options &m_opt)
{
    Options defaultOptions;

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
// from config file / Windows Registry
void loadConfig(Options &m_opt)
{
    QSettings settings("Crosshair++", "crosshair");

    Options defaultOptions;

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
    m_opt.currentScreenIndex =
        settings.value("crosshair/currentScreenIndex", defaultOptions.currentScreenIndex).toBool();

    int alpha = settings.value("crosshair/shadowAlpha", defaultOptions.shadowColor.alpha()).toInt();
    m_opt.shadowColor = QColor(0, 0, 0, alpha);
}

// updates all the setting widgets to match the config in memory
void showConfig(Options &m_opt, Ui::MainWindow &ui)
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
}

// save current config to disk / Win Registry
// should be called on every settings change
void saveConfig(Options &m_opt)
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
} // namespace Config