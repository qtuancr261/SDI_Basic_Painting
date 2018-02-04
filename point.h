// object
#ifndef POINT_H
#define POINT_H
#include <QPoint>
#include <QString>
#include <QDebug>
#include "matrixlibs.h"
class Point : public QPoint
{
private:
    int h;
public:
    Point(); //default constructor - no parameters
    Point(int xpos, int ypos, int hValue = 1);
    Point(const QPoint& srcPoint); // copy constructor
    Point(const Point& srcPoint) = default;
    Point& operator =(const Point &srcPoint);
     ~Point() = default; // default destructor

    // Transforming function
    void translate(int xtrans, int ytrans);
    void scale(double Sx, double Sy, const QPoint& centralPoint);
    void rotate(double degree,int xtrans, int ytrans);
    void centralSymmetry(const QPoint& centralPoint);
    void OxSymmetry(int yValue); // line y = value
    void OySymmetry(int xValue); // line x = value
    bool operator <(const Point& srcPoint) const;
    static double distance(const Point& p1, const Point& p2); // calculate the distance between two point in the system coordinate machine
    static double distanceFromPointToLine(const Point& point, const Point& lineP1, const Point& lineP2);
    // static functions - create new point
    static Point convertToUserSystem(const Point& cvtPoint, const Point& Origin);
    static Point translate(const Point& cvtPoint, int xtrans, int ytrans);
    static Point scale(const Point& cvtPoint, int Sx, int Sy);
    static Point rotate(const Point& cvtPoint, double radian);
    static Point centralSymmetry(const Point& cvtPoint, const Point& centralPoint);
};

#endif // POINT_H
