#ifndef SDI_PAINTER_H
#define SDI_PAINTER_H

#include <QPainter>
#include <QPaintDevice>
#include <QVector>
#include <sdi_point.h>
#include <cmath>
class SDI_Painter : public QPainter
{
public:
    SDI_Painter();
    SDI_Painter(QPaintDevice* device);
    ~SDI_Painter() = default;
    void drawOxy(int width, int height, SDI_Point& Origin);
    virtual void drawPoint(const SDI_Point& p1);
    virtual void drawPoint(int x, int y);
    virtual void drawLine(const SDI_Point &p1, const SDI_Point &p2);
    virtual void drawRect(const SDI_Point&topLeft, const SDI_Point& bottomRight);
    virtual void drawSquare(const SDI_Point &firstPoint, SDI_Point &lastPoint);
    virtual void drawCircle(const SDI_Point& centralPoint, const SDI_Point& pointOnCircle);
    void midPointYLine(const SDI_Point& p1, const SDI_Point& p2);
    void midPointXLine(const SDI_Point& p1, const SDI_Point& p2);
    void midPointXCircle(const SDI_Point& topCirclePoint, const SDI_Point& centralCirclePoint, const SDI_Point& rightCirclePoint, double radius);

private:
    QVector<SDI_Point> line;
    QVector<SDI_Point> setPoints;
};

#endif // SDI_PAINTER_H
