#include "sdi_geometricshape.h"

void SDI_GeometricShape::setShapeName()
{
    if (shapeID == GeometricShape::GS_Triangle && setOfPoints.size() == 2)
        shapeName = shapeNames.at(static_cast<int>(shapeID) + 1);
    else
        shapeName = shapeNames.at(static_cast<int>(shapeID));
}

SDI_GeometricShape::SDI_GeometricShape(GeometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &Origin, const QPen &pen)
    : shapeID(id), shapePen(pen),OriginPos(Origin)
{
    switch (shapeID)
    {
    case GeometricShape::GS_SelectShape:
    case GeometricShape::GS_Line:
    case GeometricShape::GS_Rect:
    case GeometricShape::GS_Square:
    case GeometricShape::GS_Circle:
    case GeometricShape::GS_Triangle: // isosceless Triangle
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setShapeName();
        initShapeData();
        break;
    default:
        break;
    }
}

SDI_GeometricShape::SDI_GeometricShape(GeometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const SDI_Point &Origin, const QPen &pen)
    : shapeID(id), shapePen(pen), OriginPos(Origin)
{
    switch (shapeID)
    {
    case GeometricShape::GS_Triangle:
    case GeometricShape::GS_Parallelogram:
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setOfPoints.push_back(point3);
        setShapeName();
        initShapeData();
        break;
    default:
        break;
    }
}

SDI_GeometricShape::SDI_GeometricShape(const SDI_GeometricShape &srcShape)
{
    setOfPoints = srcShape.setOfPoints;
    shapeID = srcShape.shapeID;
    shapeBoundingRect = srcShape.shapeBoundingRect;
    shapePen = srcShape.shapePen;
    shapeName = srcShape.shapeName;
    OriginPos = srcShape.OriginPos;
    centralPoint = srcShape.centralPoint;
}

GeometricShape SDI_GeometricShape::getShapeId() const
{
    return shapeID;
}

QVector<SDI_Point> &SDI_GeometricShape::getSetOfPoints()
{
    return setOfPoints;
}

QPen SDI_GeometricShape::getShapePen() const
{
    return shapePen;
}

QString SDI_GeometricShape::getShapeName() const
{
    return shapeName;
}

QString SDI_GeometricShape::getShapeData() const
{
    return shapeData;
}

void SDI_GeometricShape::setShapeData(QString newData)
{
    shapeData = newData;
}

const QRect &SDI_GeometricShape::getShapeBoundinRect() const
{
    return shapeBoundingRect;
}

void SDI_GeometricShape::setShapeBoundinRect(const SDI_Point &topLeft, const QSize &rectSize)
{
    shapeBoundingRect.setTopLeft(topLeft);
    shapeBoundingRect.setSize(rectSize);
}

const SDI_Point &SDI_GeometricShape::getOriginPos() const
{
    return OriginPos;
}

void SDI_GeometricShape::setCentralPoint(const SDI_Point &src)
{
    centralPoint = src;
}

void SDI_GeometricShape::initShapeData()
{
    switch (shapeID)
    {
    case GeometricShape::GS_Line:
        SDI_Painter::updateLineData(*this);
        break;
    case GeometricShape::GS_Rect:
        SDI_Painter::updateRectData(*this);
        break;
    case GeometricShape::GS_Square:
        SDI_Painter::updateSquareData(*this);
        break;
    case GeometricShape::GS_Circle:
        SDI_Painter::updateCircleData(*this);
        break;
    case GeometricShape::GS_Triangle:
        if (this->getShapeName() == "Tam giác")
            SDI_Painter::updateTriangleData(*this);
        else
            SDI_Painter::updateIRTriangleData(*this);
        break;
    case GeometricShape::GS_Parallelogram:
        SDI_Painter::updateParallelogramData(*this);
        break;
    default:
        break;
    }
}

void SDI_GeometricShape::updateShapeData()
{
    switch (shapeID)
    {
    case GeometricShape::GS_Line:
        SDI_Painter::updateLineData(*this);
        break;
    case GeometricShape::GS_Rect:
    case GeometricShape::GS_Square:
    case GeometricShape::GS_Parallelogram:
        SDI_Painter::updateTetragonData(*this);
        break;
    case GeometricShape::GS_Circle:
        SDI_Painter::updateCircleData(*this);
        break;
    case GeometricShape::GS_Triangle:
        SDI_Painter::updateTriangleData(*this);
        break;
    default:
        break;
    }
}

void SDI_GeometricShape::translate(int xtrans, int ytrans)
{
    for (SDI_Point& point : setOfPoints)
    {
        point.translate(xtrans, ytrans);
    }
    updateShapeData();
}

void SDI_GeometricShape::scale(double xscale, double yscale)
{
    for (SDI_Point& point : setOfPoints)
    {
        point.scale(xscale, yscale, centralPoint);
    }
    updateShapeData();
}

void SDI_GeometricShape::rotate(double degree, int centralPointID)
{
    if (static_cast<int>(degree) % 180 != 0)
    {
        int xtrans{centralPointID == 1 ? -centralPoint.x() : -OriginPos.x()};
        int ytrans{centralPointID == 1 ? -centralPoint.y() : -OriginPos.y()};
        for (SDI_Point& point : setOfPoints)
        {
            point.rotate(degree, xtrans, ytrans);
        }
        updateShapeData();
    }
    else
        return;
}

void SDI_GeometricShape::originPosSymmetry()
{
    for (SDI_Point& point : setOfPoints)
    {
        //point.rx() = OriginPos.x()*2 - point.x();
        //point.ry() = OriginPos.y()*2 - point.y();
        point.centralSymmetry(OriginPos);
    }
    updateShapeData();
}

void SDI_GeometricShape::OxSymmetry()
{
    for (SDI_Point& point : setOfPoints)
    {
        point.OxSymmetry(OriginPos.y());
    }
    updateShapeData();
}

void SDI_GeometricShape::OySymmetry()
{
    for (SDI_Point& point : setOfPoints)
    {
        point.OySymmetry(OriginPos.x());
    }
    updateShapeData();
}
