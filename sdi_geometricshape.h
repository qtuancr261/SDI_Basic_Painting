#ifndef SDI_GEOMETRICSHAPE_H
#define SDI_GEOMETRICSHAPE_H


#include "sdi_point.h"
#include <QVector>
enum class geometricShape
{
    selectShape = 0, point, line, rect, square, parallelogram, circle, triangle
};

class SDI_GeometricShape
{
private:
    QVector<SDI_Point> setOfPoints;
    geometricShape shapeID;
public:
    SDI_GeometricShape() = default;
    SDI_GeometricShape(geometricShape id, const SDI_Point& point1, const SDI_Point& point2);
    SDI_GeometricShape(geometricShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3);
    SDI_GeometricShape(const SDI_GeometricShape& srcShape);
    ~SDI_GeometricShape() = default;
    geometricShape getShapeId() const;
    QVector<SDI_Point>& getSetOfPoints();
};

#endif // SDI_GEOMETRICSHAPE_H
