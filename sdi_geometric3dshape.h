#ifndef SDI_GEOMETRIC3DSHAPE_H
#define SDI_GEOMETRIC3DSHAPE_H

#include <QVector>
#include "sdi_namespace.h"
#include "sdi_point.h"
#include "sdi_painter.h"
class SDI_Geometric3DShape
{
private:
    QVector<SDI_Point>setOfPoints;
    Geometric3DShape shapeID;
    SDI_Point OriginPos;
    SDI_Point centralPoint;
public:
    SDI_Geometric3DShape() = default;
    SDI_Geometric3DShape(Geometric3DShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3, const SDI_Point& Origin);
    QVector<SDI_Point>& getSetOfPoints();
    Geometric3DShape getShapeID() const;
};

#endif // SDI_GEOMETRIC3DSHAPE_H
