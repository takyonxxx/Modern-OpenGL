#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Initialize application
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    // Start event loop
    return a.exec();
}
