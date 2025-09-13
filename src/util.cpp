
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "util.h"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>

namespace util
{

// this function loads all the required
// fonts from the resources into the db,
// and sets the default font
void loadFonts(QApplication &app)
{
    int font = QFontDatabase::addApplicationFont(":/fonts/SourceCodePro-Regular.ttf");
    if (font == -1)
    {
        qWarning() << "Failed to load font.";
        return;
    }

    QString fontFamily = QFontDatabase::applicationFontFamilies(font).at(0);
    QFont fontRegular = QFont(fontFamily, 10);
    app.setFont(fontRegular);
}

// this shows a nice welcome dialogue
void welcomeDialogue()
{
    QString message = "Crosshair++ offers the following features:\n"
                      "- Configure a custom crosshair\n"
                      "- Change its color, size, thickness, and gap\n"
                      "- Optionally add a central dot\n"
                      "- Enable a shadow behind the crosshair and adjust its blur radius and transparency\n"
                      "- Export your settings as a code\n\n"

                      "When you close the settings window, Crosshair++ will continue running in the system tray, "
                      "where you can reopen the settings or stop the program completely. If you encounter any "
                      "issues, please report them at https://github.com/Drumba08/crosshairpp/issues.";

    QMessageBox::information(nullptr, "Getting started", message);
}

} // namespace util