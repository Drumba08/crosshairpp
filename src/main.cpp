
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "QMessageBox"
#include "QSharedMemory"
#include "mainwindow.h"
#include "qapplication.h"
#include "signal.h"

QSharedMemory *gShared = nullptr;

void handleSignal(int)
{
    if (gShared)
        gShared->detach();
    exit(0);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // shared memory to check for other running process
    QSharedMemory shared("crosshairpp.sharedmem");
    gShared = &shared;
    if (!shared.create(1))
    {
        QMessageBox::warning(nullptr, "Already running", "Crosshair++ is already running. Check your system tray to open the settings.");
        return 1;
    }

    // close sharedmem on sigint/sigterm to cleanup
    signal(SIGINT, handleSignal);
    signal(SIGTERM, handleSignal);

    // create instance of MainWindow (the settings)
    MainWindow window;

    app.exec();
    return 0;
}