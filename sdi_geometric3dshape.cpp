#include "sdi_geometric3dshape.h"

SDI_Geometric3DShape::SDI_Geometric3DShape(Geometric3DShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const SDI_Point &Origin, const QPen &currentPen)
    : shapeID(id), OriginPos(Origin), shapePen(currentPen)
{
    switch (shapeID)
    {
    case Geometric3DShape::G3DS_Parallelepiped:
    case Geometric3DShape::G3DS_Pyramid:
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setOfPoints.push_back(point3);
        break;
    default:
        break;
    }
}

QVector<SDI_Point> &SDI_Geometric3DShape::getSetOfPoints()
{
    return setOfPoints;
}

Geometric3DShape SDI_Geometric3DShape::getShapeID() const
{
    return shapeID;
}

QPen SDI_Geometric3DShape::getShapePen() const
{
    return shapePen;
}
