#include "sdi_geometricshape.h"

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2)
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
        break;
    default:
        break;
    }
}

SDI_GeometricShape::SDI_GeometricShape(geometricShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3)
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

QString SDI_GeometricShape::getInfo() const
{
    return shapeName.at(static_cast<int>(shapeID));
}


