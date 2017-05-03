// Algorithms
#ifndef SDI_PAINTER_H
#define SDI_PAINTER_H

#include <QPainter>
#include <QPaintDevice>
#include <QVector>
#include <sdi_point.h>
#include <cmath>
class SDI_Painter : public QPainter
{
public:
    SDI_Painter();
    SDI_Painter(QPaintDevice* device);
    ~SDI_Painter() = default;
    void drawOxy(int width, int height, SDI_Point& Origin); // draw user's coordinate
    void drawOxyz(int width, int height, SDI_Point& Origin);
    virtual void drawPoint(const SDI_Point& p1);
    virtual void drawPoint(int x, int y);
    //------------------------------------------------------------------------------
    virtual void drawLine(const SDI_Point &p1, const SDI_Point &p2);
    static QString getLineData(const SDI_Point &p1, const SDI_Point &p2, const SDI_Point& Origin, QRect& boundinRect);
    virtual void drawRect(const SDI_Point& topLeft, const SDI_Point& bottomRight);
    static QString getRectData(const SDI_Point&topLeft, const SDI_Point& bottomRight, const SDI_Point& Origin, QRect& boundinRect);
    virtual void drawSquare(const SDI_Point &firstPoint, const SDI_Point &lastPoint);
    static QString getSquareData(const SDI_Point& firstPoint, const SDI_Point& lastPoint, const SDI_Point& Origin, QRect& boundinRect);
    virtual void drawCircle(const SDI_Point& centralPoint, const SDI_Point& pointOnCircle);
    static QString getCircleData(const SDI_Point& centralPoint, const SDI_Point& pointOnCircle, const SDI_Point& Origin, QRect& boundinRect);
    virtual void drawTriangle(const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3);
    virtual void drawIsoscelesRightTriangle(const SDI_Point& cpoint, const SDI_Point& epoint);
    virtual void drawParallelogram(const SDI_Point& pointA, const SDI_Point& pointB,const SDI_Point& pointC);
    void midPointYLine(const SDI_Point& p1, const SDI_Point& p2); // drawLine using midpoint algorithm if yValue > xValue
    void midPointXLine(const SDI_Point& p1, const SDI_Point& p2); // drawLine using midpoint algorithm if xValue > yValue
    void midPointXYCircle(const SDI_Point& topCirclePoint, const SDI_Point& centralCirclePoint, double radius); // draw circle using midpoint algorithm
    // x : central.x() < central.x()+R*sqrt(2.0)/2

private:
    QVector<SDI_Point> line;
    QVector<SDI_Point> setPoints;
};

#endif // SDI_PAINTER_H
