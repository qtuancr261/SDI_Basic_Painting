#ifndef SDI_GEOMETRIC3DSHAPE_H
#define SDI_GEOMETRIC3DSHAPE_H
#include "sdi_point.h"
#include "sdi_painter.h"
#include <QVector>
enum class geometric3DShape
{
    selectShape = 0, parallelepiped, pyramid
};

class SDI_Geometric3DShape
{
private:
    QVector<SDI_Point>setOfPoint;
    geometric3DShape shapeID;
    SDI_Point OriginPos;
    SDI_Point centralPoint;
public:
    SDI_Geometric3DShape() = default;
    SDI_Geometric3DShape(geometric3DShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3, const SDI_Point& Origin);
    void init3DShapeData();
};

#endif // SDI_GEOMETRIC3DSHAPE_H
