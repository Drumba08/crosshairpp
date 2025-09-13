
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "mainwindow.h"
#include "util.h"
#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
#include <QObject>
#include <QSharedMemory>
#include <signal.h>

QSharedMemory *gShared = nullptr;

// SIGINT/SIGTERM handler,
// it just cleans up and exits
void handleSignal(int)
{
    if (gShared)
    {
        gShared->detach();
    }
    exit(0);
}

void registerSignalHandlers()
{
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);
}

// this function creates a shared memory to probe for
// other instances of this app running
bool alreadyRunning()
{
    QSharedMemory shared("crosshairpp.sharedmem");
    gShared = &shared;
    if (!shared.create(1))
    {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    registerSignalHandlers();

    QApplication app(argc, argv);

    if (alreadyRunning())
    {
        QMessageBox::warning(nullptr, "Already running",
                             "Crosshair++ is already running. Check your system tray to open the settings.");
        return 1;
    }

    // load font
    util::loadFonts(app);

    // create mainwindow
    MainWindow window;
    window.setup();

    window.show();

    app.exec();
    return 0;
}