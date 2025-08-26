
/*
 * Copyright (c) 2025 @Drumba08 <drumba08@gmail.com>
 *
 * Licensed under the GNU General Public License v3.0 (GPLv3)
 * See the LICENSE file for full license text.
 */

#include "mainwindow.h"
#include "qapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    app.exec();
    return 0;
}