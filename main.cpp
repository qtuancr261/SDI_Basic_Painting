#include "sdi_mainwindow.h"
#include <QApplication>

QVector<QString> SDI_GeometricShape::shapeName({"None","Điểm" ,"Đường thẳng", "Hình chữ nhật", "Hình vuông", "Hình bình hành", "Hình tròn", "Tam giác", "Tam giác vuông cân" });
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SDI_MainWindow w;
    w.showMaximized();
    //leftToolsWidget w;
    //w.show();
    return a.exec();
}
