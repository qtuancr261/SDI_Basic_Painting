// Algorithms
#ifndef SDI_PAINTER_H
#define SDI_PAINTER_H

#include <QPainter>
#include <QPaintDevice>
#include <QVector>
#include <sdi_point.h>
#include "sdi_geometricshape.h"
#include "sdi_geometric3dshape.h"
class SDI_GeometricShape; // forward declare
class SDI_Geometric3DShape; // forward declare
class SDI_Painter : public QPainter
{
public:
    SDI_Painter();
    SDI_Painter(QPaintDevice* device);
    ~SDI_Painter() = default;
    void drawOxy(int width, int height, SDI_Point& Origin); // draw user's coordinate
    void drawOxyz(int width, int height, SDI_Point& Origin);
    //--------------------------------------------------------------------------------
    virtual void drawPoint(const SDI_Point& p1);
    virtual void drawPoint(int x, int y);
    //--------------------------------------------------------------------------------
    virtual void drawTetragon(QVector<SDI_Point>& points);
    virtual void drawTetragon(QVector<SDI_Point>& points, const QPen& tetragonPen);
    //------------------------------------------------------------------------------
    virtual void drawLine(const SDI_Point &p1, const SDI_Point &p2);
    virtual void drawLine(const SDI_Point &p1, const SDI_Point &p2, const QPen& linePen); // draw with custom pencil
    static void updateLineData(SDI_GeometricShape& shape); // update after changing its properties
    //------------------------------------------------------------------------------
    virtual void drawRect(const SDI_Point& topLeft, const SDI_Point& bottomRight);
    static void updateRectData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawSquare(const SDI_Point &firstPoint, const SDI_Point &lastPoint);
    static void updateSquareData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawCircle(const SDI_Point& centralPoint, const SDI_Point& pointOnCircle);
    virtual void drawCircle(const SDI_Point& centralPoint, const SDI_Point& pointOnCircle, const QPen& circlePen);
    static void updateCircleData(SDI_GeometricShape &shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawTriangle(const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3);
    virtual void drawTriangle(const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3, const QPen& trianglePen);
    static void updateTriangleData(SDI_GeometricShape& shape); // update after changing its properties

    virtual void drawIsoscelesRightTriangle(const SDI_Point& cpoint, const SDI_Point& epoint);
    static void updateIRTriangleData(SDI_GeometricShape& shape); // update after changing its properties
    //--------------------------------------------------------------------------------
    virtual void drawParallelogram(const SDI_Point& pointA, const SDI_Point& pointB,const SDI_Point& pointC);
    static void updateParallelogramData(SDI_GeometricShape& shape);
    //--------------------------------------------------------------------------------
    static void updateTetragonData(SDI_GeometricShape& shape); // update after changing its properties

    void midPointYLine(const SDI_Point& p1, const SDI_Point& p2); // drawLine using midpoint algorithm if yValue > xValue
    void midPointXLine(const SDI_Point& p1, const SDI_Point& p2); // drawLine using midpoint algorithm if xValue > yValue
    void midPointXYCircle(const SDI_Point& topCirclePoint, const SDI_Point& centralCirclePoint, double radius); // draw circle using midpoint algorithm
    // x : central.x() < central.x()+R*sqrt(2.0)/2

    virtual void drawParallelePiped(const SDI_Point& point1, const SDI_Point& point3, const SDI_Point& pointHSize, const QPen& shapePen);
    virtual void drawPyramid(const SDI_Point& point1, const SDI_Point& point3, const SDI_Point& pointHSize, const QPen& shapePen);

    static void calculateBasePlaneABCD(SDI_Point& pointA, SDI_Point& pointB, SDI_Point& pointC, SDI_Point& pointD);
};

#endif // SDI_PAINTER_H
