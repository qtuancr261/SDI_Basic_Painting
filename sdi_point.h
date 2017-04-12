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
    SDI_Point(const QPoint& srcPoint);
     ~SDI_Point() = default;

    SDI_Point translate(int xtrans, int ytrans) const;
    SDI_Point scale(int Sx, int Sy) const;
    SDI_Point rotate(double radian) const;
    SDI_Point centralSymmetry(const QPoint& centralPoint) const;

    static double distance(const QPoint& p1, const QPoint& p2);
};

#endif // SDI_POINT_H