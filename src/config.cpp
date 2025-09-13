
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "config.h"

#include "ui_preset.h"
#include <QSettings>

// restores the default config in memory.
// DOES NOT write the changes to disk on its own
void Config::resetConfig()
{
    Config defaultOptions;

    enabled = defaultOptions.enabled;
    color = defaultOptions.color;
    length = defaultOptions.length;
    thickness = defaultOptions.thickness;
    gap = defaultOptions.gap;
    dot = defaultOptions.dot;
    dotSize = defaultOptions.dotSize;
    shadow = defaultOptions.shadow;
    shadowBlurRadius = defaultOptions.shadowBlurRadius;
    shadowColor = defaultOptions.shadowColor;
    currentScreenIndex = defaultOptions.currentScreenIndex;
}

// reads the saved config on program startup
// from config file / Windows Registry
void Config::loadConfig()
{
    QSettings settings("Crosshair++", "config");

    Config defaultOptions;

    firstTime = settings.value("crosshair/firstTime", defaultOptions.firstTime).toBool();
    enabled = settings.value("crosshair/enabled", defaultOptions.enabled).toBool();
    color = settings.value("crosshair/color", defaultOptions.color).value<QColor>();
    length = settings.value("crosshair/length", defaultOptions.length).toInt();
    thickness = settings.value("crosshair/thickness", defaultOptions.thickness).toInt();
    gap = settings.value("crosshair/gap", defaultOptions.gap).toInt();
    dot = settings.value("crosshair/dotEnabled", defaultOptions.dot).toBool();
    dotSize = settings.value("crosshair/dotSize", defaultOptions.dotSize).toInt();
    shadow = settings.value("crosshair/shadowEnabled", defaultOptions.shadow).toBool();
    shadowBlurRadius = settings.value("crosshair/shadowRadius", defaultOptions.shadowBlurRadius).toInt();
    currentScreenIndex = settings.value("crosshair/currentScreenIndex", defaultOptions.currentScreenIndex).toBool();

    int alpha = settings.value("crosshair/shadowAlpha", defaultOptions.shadowColor.alpha()).toInt();
    shadowColor = QColor(0, 0, 0, alpha);
}

// updates all the setting widgets to match the config in memory
void Config::showConfig(Ui::MainWindow &ui)
{
    ui.i_enableCrosshair->setChecked(enabled);

    ui.i_length->setValue(length);
    ui.i_thickness->setValue(thickness);
    ui.i_gap->setValue(gap);
    ui.i_dotEnabled->setChecked(dot);
    ui.i_dotSize->setValue(dotSize);
    ui.i_shadow->setChecked(shadow);
    ui.i_shadowradius->setValue(shadowBlurRadius);
    ui.i_shadowalpha->setValue(shadowColor.alpha());

    ui.i_length_2->setValue(length);
    ui.i_thickness_2->setValue(thickness);
    ui.i_gap_2->setValue(gap);
    ui.i_dotEnabled->setChecked(dot);
    ui.i_dotSize_2->setValue(dotSize);
    ui.i_shadow->setChecked(shadow);
    ui.i_shadowradius_2->setValue(shadowBlurRadius);
    ui.i_shadowalpha_2->setValue(shadowColor.alpha());
}

// save current config to disk / Win Registry
// should be called on every settings change
void Config::saveConfig()
{
    QSettings settings("Crosshair++", "crosshair");

    clamp();

    settings.setValue("crosshair/firstTime", firstTime);

    settings.setValue("crosshair/enabled", enabled);
    settings.setValue("crosshair/color", color);
    settings.setValue("crosshair/length", length);
    settings.setValue("crosshair/thickness", thickness);
    settings.setValue("crosshair/gap", gap);
    settings.setValue("crosshair/dotEnabled", dot);
    settings.setValue("crosshair/dotSize", dotSize);
    settings.setValue("crosshair/shadowEnabled", shadow);
    settings.setValue("crosshair/shadowRadius", shadowBlurRadius);
    settings.setValue("crosshair/shadowAlpha", shadowColor.alpha());

    settings.setValue("crosshair/currentScreenIndex", currentScreenIndex);
}

void Config::clamp()
{
    length = std::clamp(length, 1, 50);
    gap = std::clamp(gap, 0, 50);
    thickness = std::clamp(thickness, 1, 50);
    dotSize = std::clamp(dotSize, 0, 100);
    shadowBlurRadius = std::clamp(shadowBlurRadius, 0, 24);
    shadowColor.setAlpha(std::clamp(shadowColor.alpha(), 0, 255));
}