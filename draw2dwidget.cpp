#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#include <QDebug>
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
    loadedLayerImage = false;
    delegateMode = drawLineDelegateMode::none; // disable delegate on startup
    setMouseTracking(true);
}

draw2DWidget::~draw2DWidget()
{
    setOfShapes.clear();
    setOf3DShapes.clear();
}

bool draw2DWidget::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;
    originalSize = loadedImage.size();
    QImage scaledImage = loadedImage.scaled(size(), Qt::KeepAspectRatio);;
    QSize newSize = loadedImage.size().scaled(size().width(), size().height(), Qt::KeepAspectRatio);
    resizeImage(&scaledImage, newSize);
    image = scaledImage;
    modified = false;
    update();
    return true;
}

bool draw2DWidget::saveImage(const QString &fileName, const char *fileFormat)
{
    if (image.isNull())
    {
        image = QImage(size(), QImage::Format_RGB32);
        image.fill(qRgb(255,255,255));
    }
    QPainter paint(&image);
    paint.drawImage(0,0, transparentImg);
    QImage visibleImage = image.scaled(originalSize, Qt::KeepAspectRatio);
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

void draw2DWidget::locateSelectedShape(const SDI_Point &selectPos)
{
    if (!setOfShapes.isEmpty() && graphicMode == graphicsMode::graphic2D)
    {
        for (int i{setOfShapes.size() - 1}; i >=0 ; i--)
            if (setOfShapes.at(i)->getShapeBoundinRect().contains(selectPos))
            {
                emit selectedShape(setOfShapes.at(i));
                return;
            }
    }
    emit selectedShape(nullptr);
}

void draw2DWidget::clearImage(clearImageMode clearID)
{
    transparentImg.fill(qRgba(0,0,0,0));
    image.fill(qRgb(255,255,255));
    origin = QPoint(width()/2, height()/2);
    SDI_Painter painter(&transparentImg);
    if (graphicMode == graphicsMode::graphic2D)
    {
        painter.drawOxy(this->width(), this->height(), origin);
        if (clearID == clearImageMode::clearAll)
        {
            setOfShapes.clear();
            modified = true;
        }
        else
        {
            drawExistentObject(&painter);
            modified = false;
        }
    }
    else
    {
        painter.drawOxyz(this->width(), this->height(), origin);
        if (clearID == clearImageMode::clearAll)
        {
            setOfShapes.clear();
            setOf3DShapes.clear();
            modified = true;
        }
        else
        {
            drawExistentObject(&painter);
            modified = false;
        }
    }
    lastPoint = lastPoint_2 =  SDI_Point(0, 0); // reset recorded position
    delegateMode = drawLineDelegateMode::none;
    update();
}

void draw2DWidget::mousePressEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    //--------------LEFT MOUSE HANDLER  FOR 2D Mode----------------------------
    if (event->button() == Qt::LeftButton && graphicMode == graphicsMode::graphic2D)
        switch (draw2DObjectMode)
        {
        case geometricShape::point:
            drawObject(eventPos, 1);
            break;
        //-----------------------------------------------------------
        case geometricShape::line:
        case geometricShape::rect:
        case geometricShape::square:
        case geometricShape::circle:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else
            {
                drawObject(eventPos,1); // 1 means draw a permanent shape
                lastPoint = SDI_Point(0, 0); // set to null
            }
            break;
       //-------------------------------------------------------------
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
                    lastPoint = lastPoint_2 = SDI_Point(0, 0); // reset to null
                }
                break;
            case 1: // Isosceles Right Triangle
                if (lastPoint.isNull())
                    lastPoint = eventPos;
                else
                {
                    drawObject(eventPos,1);
                    lastPoint = SDI_Point(0, 0); // set to null
                }
                break;
            }
            break;
        //-------------------------------------------------------------
        case geometricShape::parallelogram:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
                lastPoint_2 = eventPos;
            else
            {
                drawObject(eventPos,1);
                lastPoint = lastPoint_2 = SDI_Point(0, 0);
            }
            break;
        //-------------------------------------------------------------
        default: // select shape mode
            locateSelectedShape(eventPos);
            break;
        }
    //---------------------LEFT MOUSE HANDLER for 3D Mode
    else if (event->button() == Qt::LeftButton && graphicMode == graphicsMode::graphic3D)
        switch (draw3DObjectMode)
        {
        case geometric3DShape::parallelepiped:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
                lastPoint_2 = eventPos;
            else
            {
                drawObject(eventPos, 1);
                lastPoint = lastPoint_2 = SDI_Point(0,0);
            }
            break;
        default:
            break;
        }
    //--------------RIGHT MOUSE HANDLER---------------------------
    else if (event->button() == Qt::RightButton)
    {
        lastPoint = lastPoint_2 = SDI_Point(0, 0);
        delegateMode = drawLineDelegateMode::none;
        drawObject(SDI_Point(0,0), 2); //
    }
}

void draw2DWidget::mouseMoveEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (graphicMode == graphicsMode::graphic2D)
    {
        switch (draw2DObjectMode)
        {
        case geometricShape::selectShape:
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
        case geometricShape::parallelogram:
            if (lastPoint.isNull())
                break;
            else if (lastPoint_2.isNull())
            {
                draw2DObjectMode = geometricShape::line;
                delegateMode = drawLineDelegateMode::parrallelogram;
                drawObject(eventPos, 0);
            }
            else
                drawObject(eventPos, 0);
            break;
        default:
            break;
        }
        /*if (event->buttons() == Qt::LeftButton  && draw2DObjectMode == geometricShape::normal )
            drawObject(eventPos);
        //emit mouseMoveTo();*/
        emit mouseMoveTo(QString("<b>x = %1 | y = %2 </b>").arg(QString::number((event->pos().x() - origin.x())))
                                                    .arg(QString::number((origin.y() - event->pos().y()))));
    }
    else if (graphicMode == graphicsMode::graphic3D)
    {
        switch (draw3DObjectMode)
        {
        case geometric3DShape::parallelepiped:
            if (lastPoint.isNull())
                break;
            else if (lastPoint_2.isNull())
                drawObject(eventPos, 0);
            else
                drawObject(eventPos, 0);
            break;
        default:
            break;
        }
    }
}

void draw2DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton  && draw2DObjectMode == geometricShape::selectShape)
    {
        drawObject(eventPos, 0);
        //scribbling = false;
    }
}

void draw2DWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    origin = SDI_Point(width()/2, height()/2);
    if (!image.isNull())
        painter.drawImage(dirtyRect, image, dirtyRect); // if user loaded an image and painted over it
    painter.drawImage(dirtyRect, transparentImg, dirtyRect);
}

void draw2DWidget::resizeEvent(QResizeEvent *event)
{
    if (width() > transparentImg.width() || height() > transparentImg.height()) {
        int newWidth = width();
        int newHeight = height();
        originalSize = QSize(newWidth, newHeight);
        resizeImage(&transparentImg, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void draw2DWidget::drawObject(const SDI_Point &endPoint, int stateOfShape) // handle draw Object
{
    transparentImg.fill(qRgba(0,0,0,0));
    //Repaint the user's coordinate  system
    SDI_Painter painter(&transparentImg);
    if (graphicMode == graphicsMode::graphic2D)
        painter.drawOxy(this->width(), this->height(), origin);
    else
         painter.drawOxyz(this->width(), this->height(), origin);
    //--------------------------finish---------------------------------

    //Repaint existent shapes
    drawExistentObject(&painter);
    //------------------------------ finish ------------------//

    //using painter to draw new shape
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    if (stateOfShape != 2 && graphicMode == graphicsMode::graphic2D)
        draw2DShape(&painter, endPoint, stateOfShape);
    else if (stateOfShape != 2 && graphicMode == graphicsMode::graphic3D)
        draw3DShape(&painter, endPoint, stateOfShape);

}

void draw2DWidget::draw2DShape(SDI_Painter* painter, const SDI_Point &endPoint, int stateOfShape)
{
    int rad = (myPenWidth / 2) + 2;
    switch (draw2DObjectMode)
    {
    case geometricShape::selectShape:
        break;
    case geometricShape::line:
        switch (delegateMode)
        {
        case drawLineDelegateMode::triangle: //
            if (lastPoint_2.isNull())
            {
                painter->drawLine(lastPoint, endPoint);
                draw2DObjectMode = geometricShape::triangle; // restore draw2DObjectMode
            }
            break;
        case drawLineDelegateMode::parrallelogram:
            if (lastPoint_2.isNull())
            {
                painter->drawLine(lastPoint, endPoint);
                draw2DObjectMode = geometricShape::parallelogram;
            }
            break;
        default:
            painter->drawLine(lastPoint, endPoint);
            if (stateOfShape == 1)
            {
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin));
                update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
            }
            else
                update();
            break;
        }
        break;
    case geometricShape::rect:
        if (lastPoint.x() < endPoint.x())
            painter->drawRect(lastPoint, endPoint);
        else
            painter->drawRect(endPoint, lastPoint);
        if (stateOfShape == 1)
        {
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin));
            update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    case geometricShape::square:
    {
        painter->drawSquare(lastPoint, endPoint);
        if(stateOfShape == 1)
        {
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin));
            update(QRect(lastPoint,endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    }
    case geometricShape::circle:
        painter->drawCircle(lastPoint, endPoint);
        if (stateOfShape == 1)
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin));
        update();
        break;
    case geometricShape::triangle:
    {
        if (stateOfShape == 1)
            switch (triangleTypeID)
            {
            case 0: //triangle
                painter->drawTriangle(lastPoint, lastPoint_2, endPoint);
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint, origin));
                update();
                break;
            case 1:// // Isosceles Right Triangle
                painter->drawIsoscelesRightTriangle(lastPoint, endPoint);
                setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint,origin));
                update();
                break;
            }
        else // icoscless Triangle
        {
            if (triangleTypeID == 0)
                painter->drawTriangle(lastPoint, lastPoint_2, endPoint);
            else
                painter->drawIsoscelesRightTriangle(lastPoint, endPoint);
        }
        break;
    }
    case geometricShape::parallelogram:
        painter->drawParallelogram(lastPoint, lastPoint_2, endPoint);
        if (stateOfShape == 1)
            setOfShapes.push_back(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint, origin));
        update();
        break;
    }
}

void draw2DWidget::draw3DShape(SDI_Painter *painter, const SDI_Point &endPoint, int stateOfShape)
{
    switch (draw3DObjectMode)
    {
    case geometric3DShape::parallelepiped:
        if (!lastPoint.isNull() && lastPoint_2.isNull())
        {
           int xTrans{endPoint.y() - lastPoint.y()};
           SDI_Point tempPoint(lastPoint.x() - xTrans, endPoint.y());
           painter->drawParallelogram(lastPoint, tempPoint, endPoint);
        }
        else
            painter->drawParallelePiped(lastPoint, lastPoint_2, endPoint);
        if (stateOfShape == 1)
        {
            setOf3DShapes.push_back(new SDI_Geometric3DShape(draw3DObjectMode, lastPoint, lastPoint_2, endPoint, origin));
        }
        update();
        break;
    default:
        break;
    }
}

void draw2DWidget::drawExistentObject(SDI_Painter *painter)
{
    painter->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if (!setOfShapes.isEmpty() && graphicMode == graphicsMode::graphic2D)
        for (SDI_GeometricShape* shape:setOfShapes)
        {
            QVector<SDI_Point> setOfPoints(shape->getSetOfPoints());
            geometricShape shapeName{shape->getShapeId()};
            if (shapeName == geometricShape::rect || shapeName == geometricShape::square || shapeName == geometricShape::parallelogram)
                painter->drawTetragon(setOfPoints);
            else if (shapeName == geometricShape::line)
                painter->drawLine(setOfPoints.at(0), setOfPoints.at(1));
            else if (shapeName == geometricShape::circle)
                painter->drawCircle(setOfPoints.at(0), setOfPoints.at(1));
            else if (shapeName == geometricShape::triangle && setOfPoints.size() == 3)
                painter->drawTriangle(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2));
            else if (shapeName == geometricShape::triangle && setOfPoints.size() == 2)
                painter->drawIsoscelesRightTriangle(setOfPoints.at(0), setOfPoints.at(1));
        }
    else if (!setOf3DShapes.isEmpty() && graphicMode == graphicsMode::graphic3D)
        for (SDI_Geometric3DShape* shape:setOf3DShapes)
        {
            QVector<SDI_Point> setOfPoints(shape->getSetOfPoints());
            geometric3DShape shapeName{shape->getShapeID()};
            if (shapeName == geometric3DShape::parallelepiped)
            {
                painter->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                                    Qt::RoundJoin));
                painter->drawParallelePiped(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2));
            }
        }
    modified = true;
    update();
}

void draw2DWidget::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    //QImage newImage(newSize, QImage::Format_RGB32);
    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgba(0,0,0,0));
    //-----------------------------------------
    SDI_Painter painter(&newImage);
    painter.drawImage(SDI_Point(0, 0), *image);
    *image = newImage;
}

void draw2DWidget::setDraw2DObjectMode(int newId)
{
    switch (newId)
    {
    case 1:
        draw2DObjectMode = geometricShape::line;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 2:
        draw2DObjectMode = geometricShape::rect;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 3:
        draw2DObjectMode = geometricShape::square;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 4:
        draw2DObjectMode = geometricShape::parallelogram;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 5:
        draw2DObjectMode = geometricShape::circle;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 6:
        draw2DObjectMode = geometricShape::triangle;
        setCursor(Qt::ClosedHandCursor);
        break;
    default:
        draw2DObjectMode = geometricShape::selectShape;
        setCursor(Qt::PointingHandCursor);
        break;
    }
    lastPoint = lastPoint_2 =  SDI_Point(0, 0); // reset recorded position
    delegateMode = drawLineDelegateMode::none;
}

void draw2DWidget::setDraw3DObjectMode(int newId)
{
    switch (newId)
    {
    case 0:
        draw3DObjectMode = geometric3DShape::parallelepiped;
        setCursor(Qt::ClosedHandCursor);
        break;
    default:
        break;
    }
    lastPoint = lastPoint_2 =  SDI_Point(0, 0); // reset recorded position
    delegateMode = drawLineDelegateMode::none;
}

void draw2DWidget::setGraphicsMode(int newId)
{
    // change graphics mode and clear for new session
    if (newId == 2)
    {
        graphicMode = graphicsMode::graphic2D;
    }
    else
    {
        graphicMode = graphicsMode::graphic3D;
    }
    clearImage(clearImageMode::clearForNewSession);
    //-------------------------finish-------------------------------
}

void draw2DWidget::setTriangleTypeID(int newID)
{
    triangleTypeID =  newID;
    lastPoint = lastPoint_2 = SDI_Point(0,0);
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

