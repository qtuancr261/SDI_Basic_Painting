// Object
#ifndef SDI_GEOMETRICSHAPE_H
#define SDI_GEOMETRICSHAPE_H


#include "sdi_point.h"
#include "sdi_painter.h"
#include <QVector>
#include <QRect>
enum class geometricShape
{
    selectShape = 0, point, line, rect, square, parallelogram, circle, triangle
};

class SDI_GeometricShape
{
private:
    QVector<SDI_Point> setOfPoints;
    geometricShape shapeID;
    QString shapeData;
    QRect shapeBoundingRect;
    QString shapeName;
    QPoint OriginPos;
    void setShapeName();

    //---------static memmbert---------------
    static QVector<QString> shapeNames;
public:
    SDI_GeometricShape() = default;
    SDI_GeometricShape(geometricShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& Origin);
    SDI_GeometricShape(geometricShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3, const SDI_Point& Origin);
    SDI_GeometricShape(const SDI_GeometricShape& srcShape);
    ~SDI_GeometricShape() = default;

    geometricShape getShapeId() const;
    QVector<SDI_Point>& getSetOfPoints();
    QString getShapeName() const;

    QString getShapeData() const;
    void setShapeData(QString newData);

    const QRect& getShapeBoundinRect() const;
    void setShapeBoundinRect(const QPoint& topLeft, const QSize& rectSize);

    void initShapeData(const SDI_Point& Origin);
    void updateShapeData(const SDI_Point& Origin);
public slots:
    void translate(int xtrans, int ytrans);
};

#endif // SDI_GEOMETRICSHAPE_H
