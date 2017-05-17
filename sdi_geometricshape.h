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
    SDI_Point OriginPos;
    SDI_Point centralPoint;
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
