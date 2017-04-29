// GUI
#ifndef DRAW2DWIDGET_H
#define DRAW2DWIDGET_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QSignalMapper>
#include "sdi_painter.h"
#include "sdi_point.h"
#include "sdi_geometricshape.h"
enum class graphicsMode
{
   graphic2D = 2, graphic3D
};
enum class clearImageMode
{
   clearAll, clearForNewSession
};
enum class drawLineDelegateMode
{
    none, triangle, parrallelogram
};

class draw2DWidget : public QWidget // responsible for drawing 2D Objects, images
{
    Q_OBJECT
public:
    explicit draw2DWidget(QWidget *parent = nullptr); //  default constructor
    ~draw2DWidget();
    bool openImage(const QString& fileName);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void setPenColor(const QColor& newColor); // set painter's color
    void setPenWidth(int newWidth);  // set painter width

    bool isModified() const {return modified;} // check if image has been modified
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}

protected: // handle events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawObject(const SDI_Point &endPoint, int idMode); // draw 2D Objects
    void drawExistentObject(SDI_Painter *painter, int idMode);
    void resizeImage(QImage* image, const QSize& newSize);

    bool modified;
    bool drawPausing;
    QImage image;
    QImage tempImage;
    QSize originalSize; // original size of loaded image

    SDI_Point origin; // where the axes of the system intersect
    SDI_Point lastPoint;
    SDI_Point lastPoint_2;

    graphicsMode graphicMode;
    geometricShape draw2DObjectMode; // hold 2D Object ID
    drawLineDelegateMode delegateMode;
    int triangleTypeID; // hold triangle ID
    int myPenWidth;
    QColor myPenColor;

    QVector<SDI_GeometricShape*> setOfShapes;

signals:
    void mouseMoveTo(QString currentPos);
public slots:
    void setDraw2DObjectMode(int newId);
    void setGraphicsMode(int newId);
    void setTriangleTypeID(int newID);
    void clearImage(clearImageMode clearID = clearImageMode::clearAll);
    void print();
};

#endif // DRAW2DWIDGET_H
