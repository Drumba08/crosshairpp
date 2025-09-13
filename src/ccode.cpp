
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "ccode.h"

#include "config.h"
#include <QColor>
#include <QDebug>

namespace ccode
{

// this function generates a crosshair code encoding user changable settings, in the format:
// enabled;r;g;b;length;gap;thickness;dotenabled;dotsize;shadowenabled;shadowblur;shadowalpha
QString generateCode(Config &m_opt)
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

// takes a crosshair code, if it has the right amount of
// values its parsed and passed into the settings
void applyCode(QString &code, Config &m_opt)
{
    Config defaultOptions;
    QStringList parts = code.split(';');

    if (parts.size() != 12)
    {
        return;
    }

    // helper to convert QString into int
    auto toInt = [](const QString &str, int fallback) {
        bool ok;
        int val = str.toInt(&ok);
        return ok ? val : fallback;
    };

    // crosshair enable/disable
    m_opt.enabled = toInt(parts[0], defaultOptions.enabled ? 1 : 0) != 0;

    // crosshair color
    int r = toInt(parts[1], m_opt.color.red());
    int g = toInt(parts[2], m_opt.color.green());
    int b = toInt(parts[3], m_opt.color.blue());
    m_opt.color = QColor(r, g, b);

    // line properties
    m_opt.length = toInt(parts[4], defaultOptions.length);
    m_opt.gap = toInt(parts[5], defaultOptions.gap);
    m_opt.thickness = toInt(parts[6], defaultOptions.thickness);

    // center dot
    m_opt.dot = toInt(parts[7], defaultOptions.dot ? 1 : 0) != 0;
    m_opt.dotSize = toInt(parts[8], defaultOptions.dotSize);

    // shadow
    m_opt.shadow = toInt(parts[9], defaultOptions.shadow ? 1 : 0) != 0;
    m_opt.shadowBlurRadius = toInt(parts[10], defaultOptions.shadowBlurRadius);
    int a = toInt(parts[11], defaultOptions.shadowColor.alpha());
    m_opt.shadowColor.setAlpha(a);
}

} // namespace ccode