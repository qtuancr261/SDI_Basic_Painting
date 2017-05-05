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
        calculateShape(Origin);
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
        calculateShape(Origin);
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

void SDI_GeometricShape::setShapeBoundinRect(const QPoint &topLeft, const QSize &rectSize)
{
    shapeBoundingRect.setTopLeft(topLeft);
    shapeBoundingRect.setSize(rectSize);
}

void SDI_GeometricShape::calculateShape(const SDI_Point &Origin)
{
    switch (shapeID)
    {
    case geometricShape::line:
        SDI_Painter::getLineData(*this, Origin);
        break;
    case geometricShape::rect:
        SDI_Painter::getRectData(*this, Origin);
        break;
    case geometricShape::square:
        SDI_Painter::getSquareData(*this, Origin);
        break;
    case geometricShape::circle:
        SDI_Painter::getCircleData(*this, Origin);
        break;
    case geometricShape::triangle:
        if (this->getShapeName() == "Tam giác")
            SDI_Painter::getTriangleData(*this, Origin);
        else
            SDI_Painter::getIRTriangleData(*this, Origin);
        break;
    case geometricShape::parallelogram:
        SDI_Painter::getParallelogramData(*this, Origin);
        break;
    default:
        break;
    }
}

void SDI_GeometricShape::translate(int xtrans, int ytrans)
{
    for (SDI_Point& point : setOfPoints)
        point = point.translate(xtrans, ytrans);
    //calculateShape(OriginPos);
}



