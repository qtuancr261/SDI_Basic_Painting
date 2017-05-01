#include "sdi_mainwindow.h"
#include <QApplication>

QVector<QString> SDI_GeometricShape::shapeName({"None","Diem" ,"Duong thang", "Hinh chu nhat"});
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SDI_MainWindow w;
    w.showMaximized();
    //leftToolsWidget w;
    //w.show();
    return a.exec();
}
