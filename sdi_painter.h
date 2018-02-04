// Algorithms
#ifndef SDI_PAINTER_H
#define SDI_PAINTER_H

#include <QPainter>
#include <QPaintDevice>
#include <QVector>
#include <point.h>
#include "sdi_geometricshape.h"
#include "geometric3dshape.h"
class SDI_GeometricShape; // forward declare
class Geometric3DShape; // forward declare
class SDI_Painter : public QPainter
{
public:
    SDI_Painter();
    SDI_Painter(QPaintDevice* device);
    ~SDI_Painter() = default;
    void drawOxy(int width, int height, Point& Origin); // draw user's coordinate
    void drawOxyz(int width, int height, Point& Origin);
    //--------------------------------------------------------------------------------
    virtual void drawPoint(const Point& p1);
    virtual void drawPoint(int x, int y);
    //--------------------------------------------------------------------------------
    virtual void drawTetragon(QVector<Point>& points);
    virtual void drawTetragon(QVector<Point>& points, const QPen& tetragonPen);
    //------------------------------------------------------------------------------
    virtual void drawLine(const Point &p1, const Point &p2);
    virtual void drawLine(const Point &p1, const Point &p2, const QPen& linePen); // draw with custom pencil
    static void updateLineData(SDI_GeometricShape& shape); // update after changing its properties
    //------------------------------------------------------------------------------
    virtual void drawRect(const Point& topLeft, const Point& bottomRight);
    static void updateRectData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawSquare(const Point &firstPoint, const Point &lastPoint);
    static void updateSquareData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawCircle(const Point& centralPoint, const Point& pointOnCircle);
    virtual void drawCircle(const Point& centralPoint, const Point& pointOnCircle, const QPen& circlePen);
    static void updateCircleData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawTriangle(const Point& point1, const Point& point2, const Point& point3);
    virtual void drawTriangle(const Point& point1, const Point& point2, const Point& point3, const QPen& trianglePen);
    static void updateTriangleData(SDI_GeometricShape& shape); // update after changing its properties

    virtual void drawIsoscelesRightTriangle(const Point& cpoint, const Point& epoint);
    static void updateIRTriangleData(SDI_GeometricShape& shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawParallelogram(const Point& pointA, const Point& pointB,const Point& pointC);
    static void updateParallelogramData(SDI_GeometricShape& shape);
    //--------------------------------------------------------------------------------
    static void updateTetragonData(SDI_GeometricShape& shape); // update after changing its properties

    void midPointYLine(const Point& p1, const Point& p2); // drawLine using midpoint algorithm if yValue > xValue
    void midPointXLine(const Point& p1, const Point& p2); // drawLine using midpoint algorithm if xValue > yValue
    void midPointXYCircle(const Point& topCirclePoint, const Point& centralCirclePoint, double radius); // draw circle using midpoint algorithm
    // x : central.x() < central.x()+R*sqrt(2.0)/2

    virtual void drawParallelePiped(const Point& point1, const Point& point3, const Point& pointHSize, const QPen& shapePen);
    virtual void drawPyramid(const Point& point1, const Point& point3, const Point& pointHSize, const QPen& shapePen);

    static void calculateBasePlaneABCD(Point& pointA, Point& pointB, Point& pointC, Point& pointD);
};

#endif // SDI_PAINTER_H
