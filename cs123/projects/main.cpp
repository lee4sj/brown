#include <CS123Algebra.h>
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <cmath>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
