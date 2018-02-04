#include "geometric3dshape.h"

Geometric3DShape::Geometric3DShape(G3DShape id, const Point &point1, const Point &point2, const Point &point3, const Point &Origin, const QPen &currentPen)
    : shapeID(id), OriginPos(Origin), shapePen(currentPen)
{
    switch (shapeID)
    {
    case G3DShape::G3DS_Parallelepiped:
    case G3DShape::G3DS_Pyramid:
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setOfPoints.push_back(point3);
        break;
    default:
        break;
    }
}

QVector<Point> &Geometric3DShape::getSetOfPoints()
{
    return setOfPoints;
}

G3DShape Geometric3DShape::getShapeID() const
{
    return shapeID;
}

QPen Geometric3DShape::getShapePen() const
{
    return shapePen;
}
