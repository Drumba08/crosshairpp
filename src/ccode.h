
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#pragma once

#include "config.h"
#include <QDebug>

namespace ccode
{

QString generateCode(Config::Options &m_opt);

void applyCode(QString &code, Config::Options &m_opt);

} // namespace ccode