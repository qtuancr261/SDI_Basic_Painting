// GUI
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QSignalMapper>
#include <QSharedPointer>
#include <QWeakPointer>
#include "sdi_namespace.h"
#include "sdi_painter.h"
#include "point.h"
#include "sdi_geometricshape.h"
#include "geometric3dshape.h"

class PaintWidget : public QWidget // responsible for drawing 2D Objects, images
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = nullptr); //  default constructor
    ~PaintWidget();
    bool openImage(const QString& fileName);
    bool saveImage(const QString& fileName, const char* fileFormat);
    void setPenColor(const QColor& newColor); // set painter's color
    void setPenWidth(int newWidth);  // set painter width

    bool isModified() const {return modified;} // check if image has been modified
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}
    void locateSelectedShape(const Point &selectPos);
    void drawObject(const Point &endPoint, StateOfShape drawState);
    void draw2DShape(SDI_Painter *painter, const Point& endPoint, StateOfShape drawState);
    void draw3DShape(SDI_Painter* painter, const Point& endPoint, StateOfShape drawState);

protected: // handle events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawExistentObject(SDI_Painter *painter);
    void resizeImage(QImage* backgroundImage, const QSize& newSize);

    bool modified;
    QImage defaultBackgroundImage;
    QImage backgroundImage;
    QImage transparentImg;
    QSize originalSize; // original size of loaded image

    Point origin; // where the axes of the system intersect
    Point lastPoint;
    Point lastPoint_2;

    GraphicsMode graphicMode;
    DisplayCoordinateState displayCoordinateMode;
    GeometricShape draw2DObjectMode; // hold 2D Object ID
    G3DShape draw3DObjectMode;
    DrawLineDelegateMode delegateMode;
    int triangleTypeID; // hold triangle ID
    int myPenWidth;
    QColor myPenColor;
    QPen currentPen;

    QVector<QSharedPointer<SDI_GeometricShape>> setOfShapes;
    QVector<QSharedPointer<Geometric3DShape>> setOf3DShapes;

signals:
    void mouseMoveTo(QString currentPos);
    void selectedShape(QWeakPointer<SDI_GeometricShape> shape);
    void modificationChanged(bool state);
    void graphicModeChanged(QString newMode);

public slots:
    void setDraw2DObjectMode(int newId);
    void setDraw3DObjectMode(int newId);
    void setGraphicsMode(GraphicsMode newMode);
    void setDisplayCoordinateMode(DisplayCoordinateState newState);
    void setTriangleTypeID(int newID);
    void clearImage(ClearImageMode clearID = ClearImageMode::CIM_All);
    void resetBackground();
    void print();
};

#endif // PAINTWIDGET_H
