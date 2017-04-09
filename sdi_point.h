#ifndef SDI_POINT_H
#define SDI_POINT_H
#include <QPoint>
#include <QString>
#include "matrixlibs.h"
class SDI_Point : public QPoint
{
private:
    int h;
    QString label;
public:
    SDI_Point();
    SDI_Point(int xpos, int ypos, int hValue = 1);

    SDI_Point translate(int xtrans, int ytrans);
    ~SDI_Point() = default;


};

#endif // SDI_POINT_H
