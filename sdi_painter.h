#ifndef SDI_PAINTER_H
#define SDI_PAINTER_H

#include <QPainter>
#include <QPaintDevice>
#include <QVector>
#include <cmath>
class SDI_Painter : public QPainter
{
public:
    SDI_Painter();
    SDI_Painter(QPaintDevice* device);
    ~SDI_Painter() = default;
    void drawOxy(int width, int height, QPoint& Origin);
    virtual void drawLine(const QPoint &p1, const QPoint &p2);
    virtual void drawRect(const QPoint&topLeft, const QPoint& bottomRight);
    void midPointYLine(const QPoint& p1, const QPoint& p2);
    void midPointXLine(const QPoint& p1, const QPoint& p2);

private:
    QVector<QPoint> line;
    QVector<QPoint> setPoints;
};

#endif // SDI_PAINTER_H
