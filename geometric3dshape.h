#ifndef SDI_GEOMETRIC3DSHAPE_H
#define SDI_GEOMETRIC3DSHAPE_H

#include <QVector>
#include "sdi_namespace.h"
#include "point.h"
#include "sdi_painter.h"
class Geometric3DShape
{
private:
    QVector<Point>setOfPoints;
    G3DShape shapeID;
    Point OriginPos;
    Point centralPoint;
    QPen shapePen;

public:
    Geometric3DShape() = default;
    Geometric3DShape(G3DShape id, const Point& point1, const Point& point2, const Point& point3, const Point& Origin, const QPen& currentPen);
    QVector<Point>& getSetOfPoints();
    G3DShape getShapeID() const;
    QPen getShapePen() const;
};

#endif // SDI_GEOMETRIC3DSHAPE_H
