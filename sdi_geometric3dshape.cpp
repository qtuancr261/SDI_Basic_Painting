#include "sdi_geometric3dshape.h"

SDI_Geometric3DShape::SDI_Geometric3DShape(geometric3DShape id, const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const SDI_Point &Origin)
    : shapeID(id), OriginPos(Origin)
{
    switch (shapeID)
    {
    case geometric3DShape::parallelepiped:
    case geometric3DShape::pyramid:
        setOfPoints.push_back(point1);
        setOfPoints.push_back(point2);
        setOfPoints.push_back(point3);
        break;
    default:
        break;
    }
}

void SDI_Geometric3DShape::init3DShapeData()
{
    switch (shapeID)
    {
    case geometric3DShape::parallelepiped:
        break;
    default:
        break;
    }
}

QVector<SDI_Point> &SDI_Geometric3DShape::getSetOfPoints()
{
    return setOfPoints;
}

geometric3DShape SDI_Geometric3DShape::getShapeID() const
{
    return shapeID;
}
