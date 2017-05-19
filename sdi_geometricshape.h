// Object
#ifndef SDI_GEOMETRICSHAPE_H
#define SDI_GEOMETRICSHAPE_H

#include <QVector>
#include <QRect>
#include "sdi_namespace.h"
#include "sdi_point.h"
#include "sdi_painter.h"

class SDI_GeometricShape
{
private:
    QVector<SDI_Point> setOfPoints;
    GeometricShape shapeID;
    QString shapeData;
    QRect shapeBoundingRect;
    QPen shapePen;
    QString shapeName;
    SDI_Point OriginPos;
    SDI_Point centralPoint;
    void setShapeName();

    //---------static memmbert---------------
    static QVector<QString> shapeNames;
public:
    SDI_GeometricShape() = default;
    SDI_GeometricShape(GeometricShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& Origin, const QPen& pen);
    SDI_GeometricShape(GeometricShape id, const SDI_Point& point1, const SDI_Point& point2, const SDI_Point& point3, const SDI_Point& Origin, const QPen& pen);
    SDI_GeometricShape(const SDI_GeometricShape& srcShape);
    ~SDI_GeometricShape() = default;

    GeometricShape getShapeId() const;
    QVector<SDI_Point>& getSetOfPoints();
    QPen getShapePen() const;
    QString getShapeName() const;

    QString getShapeData() const;
    void setShapeData(QString newData);

    const QRect& getShapeBoundinRect() const;
    void setShapeBoundinRect(const SDI_Point& topLeft, const QSize& rectSize);

    const SDI_Point& getOriginPos() const;

    void setCentralPoint(const SDI_Point&src);

    void initShapeData();
    void updateShapeData();
public slots:
    void translate(int xtrans, int ytrans);
    void scale(double xscale, double yscale);
    void rotate(double degree, int centralPointID);
    void originPosSymmetry();
    void OxSymmetry();
    void OySymmetry();
};

#endif // SDI_GEOMETRICSHAPE_H
