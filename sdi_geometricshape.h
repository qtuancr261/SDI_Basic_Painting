// Object
#ifndef SDI_GEOMETRICSHAPE_H
#define SDI_GEOMETRICSHAPE_H

#include <QVector>
#include <QRect>
#include "sdi_namespace.h"
#include "point.h"
#include "sdi_painter.h"

class SDI_GeometricShape
{
private:
    QVector<Point> setOfPoints;
    GeometricShape shapeID;
    QString shapeData;
    QRect shapeBoundingRect;
    QPen shapePen;
    QString shapeName;
    Point OriginPos;
    Point centralPoint;
    void setShapeName();

    //---------static member---------------
    static QVector<QString> shapeNames;
public:
    SDI_GeometricShape() = default;
    SDI_GeometricShape(GeometricShape id, const Point& point1, const Point& point2, const Point& Origin, const QPen& pen);
    SDI_GeometricShape(GeometricShape id, const Point& point1, const Point& point2, const Point& point3, const Point& Origin, const QPen& pen);
    SDI_GeometricShape(const SDI_GeometricShape& srcShape);
    ~SDI_GeometricShape() = default;

    GeometricShape getShapeId() const;

    QVector<Point>& getSetOfPoints();

    QPen getShapePen() const;

    QString getShapeName() const;

    QString getShapeData() const;
    void setShapeData(QString newData);

    const QRect& getShapeBoundinRect() const;
    void setShapeBoundinRect(const Point& topLeft, const QSize& rectSize);

    const Point& getOriginPos() const;

    void setCentralPoint(const Point&src);

    void initShapeData();
    void updateShapeData();
    bool containsPoint(const Point& point);
public slots:
    void translate(int xtrans, int ytrans);
    void scale(double xscale, double yscale);
    void rotate(double degree, int centralPointID);
    void originPosSymmetry();
    void OxSymmetry();
    void OySymmetry();
};

#endif // SDI_GEOMETRICSHAPE_H
