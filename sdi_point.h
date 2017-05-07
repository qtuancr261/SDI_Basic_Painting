// object
#ifndef SDI_POINT_H
#define SDI_POINT_H
#include <QPoint>
#include <QString>
#include "matrixlibs.h"
class SDI_Point : public QPoint
{
private:
    int h;
public:
    SDI_Point(); //default constructor - no parameters
    SDI_Point(int xpos, int ypos, int hValue = 1);
    SDI_Point(const QPoint& srcPoint); // copy constructor
    SDI_Point(const SDI_Point& srcPoint) = default;
    SDI_Point& operator =(const SDI_Point &srcPoint);
     ~SDI_Point() = default; // default destructor
    // Transforming function
    SDI_Point translate(int xtrans, int ytrans) const;
    SDI_Point scale(int Sx, int Sy) const;
    SDI_Point rotate(double radian) const;
    SDI_Point centralSymmetry(const QPoint& centralPoint) const;

    static double distance(const QPoint& p1, const QPoint& p2); // calculate the distance between two point in the system coordinate machine
    static SDI_Point convertToUserSystem(const SDI_Point& cvtPoint, const SDI_Point& Origin);
    static SDI_Point translate(const SDI_Point& cvtPoint, int xtrans, int ytrans);
    static SDI_Point scale(const SDI_Point& cvtPoint, int Sx, int Sy);
    static SDI_Point rotate(const SDI_Point& cvtPoint, double radian);
    static SDI_Point centralSymmetry(const SDI_Point& cvtPoint, const SDI_Point& centralPoint);
};

#endif // SDI_POINT_H
