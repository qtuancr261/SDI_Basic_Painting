#include "sdi_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SDI_MainWindow w;
    w.showMaximized();
    //leftToolsWidget w;
    //w.show();
    return a.exec();
}
