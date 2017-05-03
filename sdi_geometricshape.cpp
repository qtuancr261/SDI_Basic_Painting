#include "sdi_geometricshape.h"

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &Origin)
    : shapeID(id)
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
        setShapeData(Origin);
        break;
    default:
        break;
    }
}

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const SDI_Point &Origin)
    : shapeID(id)
{
    switch (shapeID)
    {
    case geometricShape::triangle:
    case geometricShape::parallelogram:
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setOfPoints.push_back(point3);
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
    if (shapeID == geometricShape::triangle && setOfPoints.size() == 2)
        return shapeName.at(static_cast<int>(shapeID) + 1);
    else
        return shapeName.at(static_cast<int>(shapeID));
}

QString SDI_GeometricShape::getShapeData() const
{
    return shapeData;
}

const QRect &SDI_GeometricShape::getShapeBoundinRect() const
{
    return shapeBoundingRect;
}

void SDI_GeometricShape::setShapeData(const SDI_Point &Origin)
{
    switch (shapeID) {
    case geometricShape::line:
        shapeData = SDI_Painter::getLineData(setOfPoints.at(0), setOfPoints.at(1), Origin, shapeBoundingRect);
        break;
    case geometricShape::rect:
        shapeData = SDI_Painter::getRectData(setOfPoints.at(0), setOfPoints.at(1), Origin, shapeBoundingRect);
        break;
    case geometricShape::square:
        shapeData = SDI_Painter::getSquareData(setOfPoints.at(0), setOfPoints.at(1), Origin);
        break;
    case geometricShape::circle:
        shapeData = SDI_Painter::getCircleData(setOfPoints.at(0), setOfPoints.at(1), Origin);
        break;
    default:
        break;
    }
}



