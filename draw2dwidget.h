#ifndef DRAW2DWIDGET_H
#define DRAW2DWIDGET_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <QImage>
#include <QMouseEvent>
#include <QSignalMapper>
#include "sdi_painter.h"
enum class draw2DMode
{
    normal, point, line, rect, square, parallelogram, circle, triangle
};

class draw2DWidget : public QWidget
{
    Q_OBJECT
public:
    explicit draw2DWidget(QWidget *parent = nullptr);

    bool openImage(const QString& fileName);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void setPenColor(const QColor& newColor);
    void setPenWidth(int newWidth);

    bool isModified() const {return modified;}
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}
    void showGUI();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawObject(const QPoint &endPoint);
    void resizeImage(QImage* image, const QSize& newSize);

    bool modified;
    QPoint origin;
    draw2DMode drawMode;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
    QSize originalSize;
signals:

public slots:
    void setDraw2DObjectMode(int id);
    void clearImage();
    void print();
};

#endif // DRAW2DWIDGET_H
