// main.cpp — entry point of the Mad Libs application.
//
// Creates the QApplication (required before any other Qt object), sets
// the application metadata (used by the window title bar, taskbar and
// desktop integration), shows the game window, and runs the event loop.

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    // The QApplication must be constructed before any widget is created,
    // since widgets depend on it (clipboard, event dispatching, styles…).
    QApplication app(argc, argv);

    // Application metadata: "display name" is what desktop environments
    // append to the window title, the org name is used for persistence
    // paths / global menu bar grouping, etc.
    app.setApplicationName(QStringLiteral("Mad Libs"));
    app.setApplicationDisplayName(QStringLiteral("Mad Libs"));
    app.setOrganizationName(QStringLiteral("Mad Libs"));

    // The main window is a stack object; its child widgets are cleaned up
    // with it when the app quits.
    madlibs::MainWindow w;
    w.show();

    // Blocks here, processing events, until the last window is closed
    // (i.e. until the user quits through the UI).
    return app.exec();
}
