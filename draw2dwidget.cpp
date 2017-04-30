#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "draw2dwidget.h"

draw2DWidget::draw2DWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    //scribbling = false;
    myPenWidth = 2;
    graphicMode = graphicsMode::graphic2D;
    myPenColor = Qt::blue;
    triangleTypeID = 0; // normal triangle
    delegateMode = drawLineDelegateMode::none; // disable delegate on startup
    drawPausing = false;
    setMouseTracking(true);
}

draw2DWidget::~draw2DWidget()
{
    setOfShapes.clear();
}

bool draw2DWidget::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    originalSize = loadedImage.size();
    QImage scale = loadedImage.scaled(size(), Qt::KeepAspectRatio);;
    QSize newSize = loadedImage.size().scaled(size().width(), size().height(), Qt::KeepAspectRatio);
    resizeImage(&scale, newSize);
    image = scale;
    modified = false;
    update();
    return true;
}

bool draw2DWidget::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image.scaled(originalSize, Qt::IgnoreAspectRatio);
    resizeImage(&visibleImage, originalSize);

    if (visibleImage.save(fileName, fileFormat))
    {
        modified = false;
        return true;
    }
    else
    {
        return false;
    }
}

void draw2DWidget::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void draw2DWidget::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void draw2DWidget::clearImage(clearImageMode clearID)
{
    image.fill(qRgb(255, 255, 255));
    origin = QPoint(width()/2, height()/2);
    SDI_Painter painter(&image);
    if (graphicMode == graphicsMode::graphic2D)
    {
        painter.drawOxy(this->width(), this->height(), origin);
        if (clearID == clearImageMode::clearAll)
        {
            setOfShapes.clear();
            modified = true;
        }
        else
            modified = false;
    }
    else
    {
        painter.drawOxyz(this->width(), this->height(), origin);
    }
    update();
}

void draw2DWidget::mousePressEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton)
    switch (draw2DObjectMode)
    {
    case geometricShape::point:
        drawObject(eventPos, 1);
        break;
    case geometricShape::line:
    case geometricShape::rect:
    case geometricShape::square:
    case geometricShape::circle:
        if (lastPoint.isNull())
            lastPoint = eventPos;
        else
        {
            drawObject(eventPos,1); // 1 means draw a permanent shape
            lastPoint = QPoint(0, 0); // set to null
        }
        break;
    case geometricShape::triangle:
        switch (triangleTypeID)
        {
        case 0: // Triangle
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
            {
                lastPoint_2 = eventPos;
            }
            else
            {
                drawObject(eventPos,1);
                lastPoint = lastPoint_2 = QPoint(0, 0); // reset to null
            }
            break;
        case 1: // Isosceles Right Triangle
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else
            {
                drawObject(eventPos,1);
                lastPoint = QPoint(0, 0); // set to null
            }
            break;
        }
        break;
    case geometricShape::parallelogram:
        if (lastPoint.isNull())
            lastPoint = eventPos;
        else if (lastPoint_2.isNull())
            lastPoint_2 = eventPos;
        else
        {
            drawObject(eventPos,1);
            lastPoint = lastPoint_2 = QPoint(0, 0);
        }
        break;
    default: // scribbling mode
        //if (event->button() == Qt::LeftButton)
        //    lastPoint = eventPos;
        break;
    }
}

void draw2DWidget::mouseMoveEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    switch (draw2DObjectMode)
    {
    case geometricShape::normal:
        //if (event->button() == Qt::LeftButton)
        //    drawObject(eventPos,1);
        break;
    case geometricShape::line:
    case geometricShape::rect:
    case geometricShape::square:
    case geometricShape::circle:
        if (!lastPoint.isNull())
            drawObject(eventPos, 0); // 0 means draw a temporary shape
        break;
    case geometricShape::triangle:
        switch (triangleTypeID)
        {
        case 0: // Triangle
            if (lastPoint.isNull())
                break;
            else if (lastPoint_2.isNull())
            {
                draw2DObjectMode = geometricShape::line; // make a trick
                delegateMode = drawLineDelegateMode::triangle; // delegate drawing triangle sides to drawLine function
                drawObject(eventPos, 0);
            }
            else
            {
                draw2DObjectMode = geometricShape::line;// next trick
                drawObject(eventPos,0);
            }
            break;
        case 1: // Isosceles Right Triangle
            if (lastPoint.isNull())
                break;
            else
            {
                drawObject(eventPos,0);
            }
            break;
        }
        break;
    default:
        break;
    }
    /*if (event->buttons() == Qt::LeftButton  && draw2DObjectMode == geometricShape::normal )
        drawObject(eventPos);
    //emit mouseMoveTo();*/
    emit mouseMoveTo(QString("(x = %1| y = %2)").arg(QString::number((event->pos().x() - origin.x())))
                                                .arg(QString::number((origin.y() - event->pos().y()))));

}

void draw2DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton  && draw2DObjectMode == geometricShape::normal)
    {
        drawObject(eventPos, 0);
        //scribbling = false;
    }
}

void draw2DWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    origin = QPoint(width()/2, height()/2);
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void draw2DWidget::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = width();
        int newHeight = height();
        originalSize = QSize(newWidth, newHeight);
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void draw2DWidget::drawObject(const SDI_Point &endPoint, int stateOfShape) // handle draw Object
{
    SDI_Painter painter(&image);
    int rad = (myPenWidth / 2) + 2;
    //Repaint the user's coordinate  system
    image.fill(qRgb(255, 255, 255));
    if (graphicMode == graphicsMode::graphic2D)
        painter.drawOxy(this->width(), this->height(), origin);
    else
         painter.drawOxyz(this->width(), this->height(), origin);
    //--------------------------finish---------------------------------

    //Repaint existent shapes
    drawExistentObject(&painter, stateOfShape);
    //------------------------------ finish ------------------//

    //using painter to draw new shape
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    switch (draw2DObjectMode)
    {
    case geometricShape::point:
        painter.drawPoint(endPoint);
        update(QRect(endPoint, endPoint + QPoint(10, 10)).normalized().adjusted(-rad, -rad, +rad, +rad));
        break;
    case geometricShape::normal:
        break;
    case geometricShape::line:
        switch (delegateMode)
        {
        case drawLineDelegateMode::triangle: //
            if (lastPoint_2.isNull())
            {
                painter.drawLine(lastPoint, endPoint);
                draw2DObjectMode = geometricShape::triangle; // restore draw2DObjectMode
            }
            else
            {
                painter.drawLine(lastPoint, lastPoint_2);
                painter.drawLine(lastPoint, endPoint);
                painter.drawLine(lastPoint_2, endPoint);
                draw2DObjectMode = geometricShape::triangle;
            }
            break;
        case drawLineDelegateMode::parrallelogram:
            break;
        default:
            painter.drawLine(lastPoint, endPoint);
            if (stateOfShape == 1)
            {
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint));
                update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
            }
            else
                update();
            break;
        }


        /*
        ;*/
        break;
    case geometricShape::rect:
        if (lastPoint.x() < endPoint.x())
            painter.drawRect(lastPoint, endPoint);
        else
            painter.drawRect(endPoint, lastPoint);
        if (stateOfShape == 1)
        {
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint));
            update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    case geometricShape::square:
    {
        painter.drawSquare(lastPoint, endPoint);
        if(stateOfShape == 1)
        {
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint));
            update(QRect(lastPoint,endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    }
    case geometricShape::circle:
        painter.drawCircle(lastPoint, endPoint);
        if (stateOfShape == 1)
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint));
        update();
        break;
    case geometricShape::triangle:
    {
        if (stateOfShape == 1)
            switch (triangleTypeID)
            {
            case 0: //triangle
                painter.drawTriangle(lastPoint, lastPoint_2, endPoint);
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint));
                update();
                break;
            case 1:// // Isosceles Right Triangle
                painter.drawIsoscelesRightTriangle(lastPoint, endPoint);
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint));
                update();
                break;
            }
        else // icoscless Triangle
        {
            painter.drawIsoscelesRightTriangle(lastPoint, endPoint);
        }
        break;
    }
    case geometricShape::parallelogram:
        painter.drawParallelogram(lastPoint, lastPoint_2, endPoint);
        if (stateOfShape == 1)
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint));
        update();
        break;
    }
}

void draw2DWidget::drawExistentObject(SDI_Painter *painter, int idMode)
{
    painter->setPen(QPen(myPenColor, idMode == 0 ? 1 : myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if (!setOfShapes.isEmpty())
        for (SDI_GeometricShape* shape:setOfShapes)
        {
            QVector<SDI_Point> setOfPoints(shape->getSetOfPoints());
            geometricShape currentShapeName{shape->getShapeId()};
            if (currentShapeName == geometricShape::rect)
                painter->drawRect(setOfPoints.at(0), setOfPoints.at(1));
            else if (currentShapeName == geometricShape::line)
                painter->drawLine(setOfPoints.at(0), setOfPoints.at(1));
            else if (currentShapeName == geometricShape::square)
                painter->drawSquare(setOfPoints.at(0), setOfPoints.at(1));
            else if (currentShapeName == geometricShape::circle)
                painter->drawCircle(setOfPoints.at(0), setOfPoints.at(1));
            else if (currentShapeName == geometricShape::triangle && setOfPoints.size() == 3)
                painter->drawTriangle(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2));
            else if (currentShapeName == geometricShape::triangle && setOfPoints.size() == 2)
                painter->drawIsoscelesRightTriangle(setOfPoints.at(0), setOfPoints.at(1));
            else
                painter->drawParallelogram(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2));
        }
    modified = true;
    update();
}

void draw2DWidget::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    static int callTime {0};
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    //-----------------------------------------
    // draw x-axis and y-axis
    SDI_Painter painter(&newImage);
    //paintOxy(painter);
    //line1.push_back(QPoint(250, 200));
   // if (callTime == 1) image->fill(qRgb(255, 255, 255));
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
    callTime++;
}

void draw2DWidget::setDraw2DObjectMode(int newId)
{
    switch (newId) {
    case 1:
        draw2DObjectMode = geometricShape::point;
        break;
    case 2:
        draw2DObjectMode = geometricShape::line;
        break;
    case 3:
        draw2DObjectMode = geometricShape::rect;
        break;
    case 4:
        draw2DObjectMode = geometricShape::square;
        break;
    case 5:
        draw2DObjectMode = geometricShape::parallelogram;
        break;
    case 6:
        draw2DObjectMode = geometricShape::circle;
        break;
    case 7:
        draw2DObjectMode = geometricShape::triangle;
        break;
    default:
        draw2DObjectMode = geometricShape::normal;
        break;
    }
    lastPoint = lastPoint_2 =  QPoint(0, 0); // reset
    delegateMode = drawLineDelegateMode::none;
}

void draw2DWidget::setGraphicsMode(int newId)
{
    // save the current session
    if (modified)
    {
        drawPausing = true;
        if (graphicMode == graphicsMode::graphic2D)
            saveImage(QDir::currentPath() + "/temp2D", "PNG");
        else
            saveImage(QDir::currentPath() + "/temp3D", "PNG");
    }
    //-------------------------finish-------------------------------

    // change graphics mode and clear for new session
    if (newId == 2)
        graphicMode = graphicsMode::graphic2D;
    else
        graphicMode = graphicsMode::graphic3D;
    clearImage(clearImageMode::clearForNewSession);
    //-------------------------finish-------------------------------

    // reload the previous session if the painter was paused
    if (drawPausing)
    {
        if (graphicMode == graphicsMode::graphic2D)
            openImage(QDir::currentPath() + "/temp2D");
        else
            openImage(QDir::currentPath() + "/temp3D");
        modified = true;
    }
    //-------------------------finish-------------------------------
}

void draw2DWidget::setTriangleTypeID(int newID)
{
    triangleTypeID =  newID;
    lastPoint = lastPoint_2 = QPoint(0,0);
}

void draw2DWidget::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        SDI_Painter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}

