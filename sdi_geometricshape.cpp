#include "sdi_geometricshape.h"

void SDI_GeometricShape::setShapeName()
{
    if (shapeID == geometricShape::triangle && setOfPoints.size() == 2)
        shapeName = shapeNames.at(static_cast<int>(shapeID) + 1);
    else
        shapeName = shapeNames.at(static_cast<int>(shapeID));
}

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &Origin)
    : shapeID(id), OriginPos(Origin)
{
    switch (shapeID)
    {
    case geometricShape::selectShape:
    case geometricShape::line:
    case geometricShape::rect:
    case geometricShape::square:
    case geometricShape::circle:
    case geometricShape::triangle: // isosceless Triangle
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setShapeName();
        initShapeData();
        break;
    default:
        break;
    }
}

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const SDI_Point &Origin)
    : shapeID(id), OriginPos(Origin)
{
    switch (shapeID)
    {
    case geometricShape::triangle:
    case geometricShape::parallelogram:
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
}

geometricShape SDI_GeometricShape::getShapeId() const
{
    return shapeID;
}

QVector<SDI_Point> &SDI_GeometricShape::getSetOfPoints()
{
    return setOfPoints;
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
    case geometricShape::line:
        SDI_Painter::updateLineData(*this);
        break;
    case geometricShape::rect:
        SDI_Painter::updateRectData(*this);
        break;
    case geometricShape::square:
        SDI_Painter::updateSquareData(*this);
        break;
    case geometricShape::circle:
        SDI_Painter::updateCircleData(*this);
        break;
    case geometricShape::triangle:
        if (this->getShapeName() == "Tam giác")
            SDI_Painter::updateTriangleData(*this);
        else
            SDI_Painter::updateIRTriangleData(*this);
        break;
    case geometricShape::parallelogram:
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
    case geometricShape::line:
        SDI_Painter::updateLineData(*this);
        break;
    case geometricShape::rect:
    case geometricShape::square:
    case geometricShape::parallelogram:
        SDI_Painter::updateTetragonData(*this);
        break;
    case geometricShape::circle:
        SDI_Painter::updateCircleData(*this);
        break;
    case geometricShape::triangle:
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
        point.rx() += xtrans;
        point.ry() -= ytrans;
    }
    updateShapeData();
}

void SDI_GeometricShape::scale(double xscale, double yscale)
{
    for (SDI_Point& point : setOfPoints)
    {
        int xtransValue{point.x() - centralPoint.x()};
        int ytransValue{point.y() - centralPoint.y()};
        point.rx() = centralPoint.x() + xscale*xtransValue;
        point.ry() = centralPoint.y() + yscale*ytransValue;
    }
    updateShapeData();
}

void SDI_GeometricShape::rotate(double degree)
{
    if (static_cast<int>(degree) % 180 != 0)
    {
        int xtrans{-centralPoint.x()};
        int ytrans{-centralPoint.y()};
        for (SDI_Point& point : setOfPoints)
        {
            point.rx() = point.x() + xtrans;
            point.ry() = point.y() + ytrans;
            double a = point.x()*qCos(qDegreesToRadians(degree)) - point.y()*qSin(qDegreesToRadians(degree));
            double b = point.x()*qSin(qDegreesToRadians(degree)) + point.y()*qCos(qDegreesToRadians(degree));
            point = QPointF(a, b).toPoint();
            point.rx() = point.x() - xtrans;
            point.ry() = point.y() - ytrans;
        }
        updateShapeData();
    }
    else
        return;
}



