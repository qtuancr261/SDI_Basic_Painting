// GUI
#ifndef DRAW2DWIDGET_H
#define DRAW2DWIDGET_H

#include <QWidget>
#include <QColor>
#include <QImage>
#include <QMouseEvent>
#include <QSignalMapper>
#include <QSharedPointer>
#include <QWeakPointer>
#include "sdi_namespace.h"
#include "sdi_painter.h"
#include "sdi_point.h"
#include "sdi_geometricshape.h"
#include "sdi_geometric3dshape.h"

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
    void locateSelectedShape(const SDI_Point &selectPos);
    void drawObject(const SDI_Point &endPoint, StateOfShape drawState);
    void draw2DShape(SDI_Painter *painter, const SDI_Point& endPoint, StateOfShape drawState);
    void draw3DShape(SDI_Painter* painter, const SDI_Point& endPoint, StateOfShape drawState);

protected: // handle events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void drawExistentObject(SDI_Painter *painter);
    void resizeImage(QImage* image, const QSize& newSize);

    bool modified;
    QImage image;
    QImage transparentImg;
    QSize originalSize; // original size of loaded image

    SDI_Point origin; // where the axes of the system intersect
    SDI_Point lastPoint;
    SDI_Point lastPoint_2;

    GraphicsMode graphicMode;
    DisplayCoordinateState displayCoordinateMode;
    GeometricShape draw2DObjectMode; // hold 2D Object ID
    Geometric3DShape draw3DObjectMode;
    DrawLineDelegateMode delegateMode;
    int triangleTypeID; // hold triangle ID
    int myPenWidth;
    QColor myPenColor;
    QPen currentPen;

    QVector<QSharedPointer<SDI_GeometricShape>> setOfShapes;
    QVector<QSharedPointer<SDI_Geometric3DShape>> setOf3DShapes;

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
    void print();
};

#endif // DRAW2DWIDGET_H
