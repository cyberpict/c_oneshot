#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Mad Libs"));
    app.setApplicationDisplayName(QStringLiteral("Mad Libs"));
    app.setOrganizationName(QStringLiteral("Mad Libs"));

    madlibs::MainWindow w;
    w.show();

    return app.exec();
}
