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
    modified = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
    currentPen = QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    graphicMode = GraphicsMode::GM_NoMode;
    displayCoordinateMode = DisplayCoordinateState::DCS_Show;
    triangleTypeID = 0; // normal triangle
    delegateMode = DrawLineDelegateMode::DLDM_None; // disable delegate on startup
    setAttribute(Qt::WA_StaticContents);
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
    QImage scaledImage = loadedImage.scaled(transparentImg.size(), Qt::IgnoreAspectRatio);;
    //QSize newSize = loadedImage.size().scaled(size().width(), size().height(), Qt::IgnoreAspectRatio);
    //resizeImage(&scaledImage, newSize);
    image = scaledImage;
    modified = false;
    emit modificationChanged(modified);
    update();
    return true;
}

bool draw2DWidget::saveImage(const QString &fileName, const char *fileFormat)
{
    if (image.isNull())
    {
        image = QImage(size(), QImage::QImage::Format_ARGB32);
        image.fill(qRgba(255,255,255,255));
    }
    QPainter paint(&image);
    paint.drawImage(0,0, transparentImg);
    QImage visibleImage = image.scaled(originalSize, Qt::IgnoreAspectRatio);

    if (visibleImage.save(fileName, fileFormat))
    {
        modified = false;
        emit modificationChanged(modified);
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
    currentPen.setColor(myPenColor);
}

void draw2DWidget::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
    currentPen.setWidth(myPenWidth);
}

void draw2DWidget::locateSelectedShape(const SDI_Point &selectPos)
{
    if (!setOfShapes.isEmpty() && graphicMode == GraphicsMode::GM_2D)
    {
        for (int i{setOfShapes.size() - 1}; i >=0 ; i--)
            if (setOfShapes.at(i)->containsPoint(selectPos))
            {
                emit selectedShape(setOfShapes.at(i).toWeakRef());
                return;
            }
    }
    emit selectedShape(QSharedPointer<SDI_GeometricShape>(nullptr));
}

void draw2DWidget::clearImage(ClearImageMode clearID)
{
    transparentImg.fill(qRgba(0,0,0,0));
    if (image.isNull())
        image.fill(qRgb(255,255,255));
    origin = QPoint(width()/2, height()/2);
    SDI_Painter painter(&transparentImg);
    if (graphicMode == GraphicsMode::GM_2D)
    {
        if (displayCoordinateMode == DisplayCoordinateState::DCS_Show)
            painter.drawOxy(this->width(), this->height(), origin);
        if (clearID == ClearImageMode::CIM_All)
        {
            setOfShapes.clear();
            modified = true;
            emit modificationChanged(modified);
        }
        else
        {
            drawExistentObject(&painter);
            modified = false;
            emit modificationChanged(modified);
        }
    }
    else
    {
        if (displayCoordinateMode == DisplayCoordinateState::DCS_Show)
            painter.drawOxyz(this->width(), this->height(), origin);
        if (clearID == ClearImageMode::CIM_All)
        {
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
    delegateMode = DrawLineDelegateMode::DLDM_None;
    update();
}

void draw2DWidget::mousePressEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    //--------------LEFT MOUSE HANDLER  FOR 2D Mode----------------------------
    if (event->button() == Qt::LeftButton && graphicMode == GraphicsMode::GM_2D)
        switch (draw2DObjectMode)
        {
        case GeometricShape::GS_Point:
            break;
        //-----------------------------------------------------------
        case GeometricShape::GS_Line:
        case GeometricShape::GS_Rect:
        case GeometricShape::GS_Square:
        case GeometricShape::GS_Circle:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else
            {
                drawObject(eventPos, StateOfShape::SOS_NewPermanentShape);
                lastPoint = SDI_Point(0, 0); // set to null
            }
            break;
       //-------------------------------------------------------------
        case GeometricShape::GS_Triangle:
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
                    drawObject(eventPos, StateOfShape::SOS_NewPermanentShape);
                    lastPoint = lastPoint_2 = SDI_Point(0, 0); // reset to null
                }
                break;
            case 1: // Isosceles Right Triangle
                if (lastPoint.isNull())
                    lastPoint = eventPos;
                else
                {
                    drawObject(eventPos, StateOfShape::SOS_NewPermanentShape);
                    lastPoint = SDI_Point(0, 0); // set to null
                }
                break;
            }
            break;
        //-------------------------------------------------------------
        case GeometricShape::GS_Parallelogram:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
                lastPoint_2 = eventPos;
            else
            {
                drawObject(eventPos, StateOfShape::SOS_NewPermanentShape);
                lastPoint = lastPoint_2 = SDI_Point(0, 0);
            }
            break;
        //-------------------------------------------------------------
        default: // select shape mode
            locateSelectedShape(eventPos);
            break;
        }
    //---------------------LEFT MOUSE HANDLER for 3D Mode
    else if (event->button() == Qt::LeftButton && graphicMode == GraphicsMode::GM_3D)
        switch (draw3DObjectMode)
        {
        case Geometric3DShape::G3DS_Parallelepiped:
        case Geometric3DShape::G3DS_Pyramid:
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
                lastPoint_2 = eventPos;
            else
            {
                drawObject(eventPos, StateOfShape::SOS_NewPermanentShape);
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
        delegateMode = DrawLineDelegateMode::DLDM_None;
        drawObject(SDI_Point(0,0), StateOfShape::SOS_AllExistentShapes); //
    }
}

void draw2DWidget::mouseMoveEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (graphicMode == GraphicsMode::GM_2D)
    {
        switch (draw2DObjectMode)
        {
        case GeometricShape::GS_SelectShape:
            break;
        case GeometricShape::GS_Line:
        case GeometricShape::GS_Rect:
        case GeometricShape::GS_Square:
        case GeometricShape::GS_Circle:
            if (!lastPoint.isNull())
                drawObject(eventPos, StateOfShape::SOS_NewTempShape); // 0 means draw a temporary shape
            break;
        case GeometricShape::GS_Triangle:
            switch (triangleTypeID)
            {
            case 0: // Triangle
                if (lastPoint.isNull())
                    break;
                else if (lastPoint_2.isNull())
                {
                    draw2DObjectMode = GeometricShape::GS_Line; // make a trick
                    delegateMode = DrawLineDelegateMode::DLDM_Triangle; // delegate drawing triangle sides to drawLine function
                    drawObject(eventPos, StateOfShape::SOS_NewTempShape);
                }
                else
                {
                    drawObject(eventPos, StateOfShape::SOS_NewTempShape);
                }
                break;
            case 1: // Isosceles Right Triangle
                if (lastPoint.isNull())
                    break;
                else
                {
                    drawObject(eventPos, StateOfShape::SOS_NewTempShape);
                }
                break;
            }
            break;
        case GeometricShape::GS_Parallelogram:
            if (lastPoint.isNull())
                break;
            else if (lastPoint_2.isNull())
            {
                draw2DObjectMode = GeometricShape::GS_Line;
                delegateMode = DrawLineDelegateMode::DLDM_Parrallelogram;
                drawObject(eventPos, StateOfShape::SOS_NewTempShape);
            }
            else
                drawObject(eventPos, StateOfShape::SOS_NewTempShape);
            break;
        default:
            break;
        }
        emit mouseMoveTo(QString("<b>x = %1 | y = %2 </b>").arg(QString::number((event->pos().x() - origin.x())))
                                                    .arg(QString::number((origin.y() - event->pos().y()))));
    }
    else if (graphicMode == GraphicsMode::GM_3D)
    {
        switch (draw3DObjectMode)
        {
        case Geometric3DShape::G3DS_Parallelepiped:
        case Geometric3DShape::G3DS_Pyramid:
            if (lastPoint.isNull())
                break;
            else if (lastPoint_2.isNull())
                drawObject(eventPos, StateOfShape::SOS_NewTempShape);
            else
                drawObject(eventPos, StateOfShape::SOS_NewTempShape);
            break;
        default:
            break;
        }
    }
}

void draw2DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton  && draw2DObjectMode == GeometricShape::GS_SelectShape)
    {
        drawObject(eventPos, StateOfShape::SOS_NewTempShape);
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

void draw2DWidget::drawObject(const SDI_Point &endPoint, StateOfShape drawState) // handle draw Object
{
    transparentImg.fill(qRgba(0,0,0,0));
    //Repaint the user's coordinate  system
    SDI_Painter painter(&transparentImg);
    if (graphicMode == GraphicsMode::GM_2D && displayCoordinateMode == DisplayCoordinateState::DCS_Show)
        painter.drawOxy(this->width(), this->height(), origin);
    else if (graphicMode == GraphicsMode::GM_3D && displayCoordinateMode == DisplayCoordinateState::DCS_Show)
         painter.drawOxyz(this->width(), this->height(), origin);
    //--------------------------finish---------------------------------

    //Repaint existent shapes
    drawExistentObject(&painter);
    //------------------------------ finish ------------------//

    //using painter to draw new shape
    painter.setPen(currentPen);
    if (drawState != StateOfShape::SOS_AllExistentShapes && graphicMode == GraphicsMode::GM_2D)
        draw2DShape(&painter, endPoint, drawState);
    else if (drawState != StateOfShape::SOS_AllExistentShapes && graphicMode == GraphicsMode::GM_3D)
        draw3DShape(&painter, endPoint, drawState);

}

void draw2DWidget::draw2DShape(SDI_Painter* painter, const SDI_Point &endPoint, StateOfShape drawState)
{
    int rad = (myPenWidth / 2) + 2;
    switch (draw2DObjectMode)
    {
    case GeometricShape::GS_SelectShape:
        break;
    case GeometricShape::GS_Line:
        switch (delegateMode)
        {
        case DrawLineDelegateMode::DLDM_Triangle: //
            if (lastPoint_2.isNull())
            {
                painter->drawLine(lastPoint, endPoint);
                draw2DObjectMode = GeometricShape::GS_Triangle; // restore draw2DObjectMode
            }
            break;
        case DrawLineDelegateMode::DLDM_Parrallelogram:
            if (lastPoint_2.isNull())
            {
                painter->drawLine(lastPoint, endPoint);
                draw2DObjectMode = GeometricShape::GS_Parallelogram;
            }
            break;
        default:
            painter->drawLine(lastPoint, endPoint);
            if (drawState == StateOfShape::SOS_NewPermanentShape)
            {
                setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin, currentPen)));
                update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
            }
            else
                update();
            break;
        }
        break;
    case GeometricShape::GS_Rect:
        if (lastPoint.x() < endPoint.x())
            painter->drawRect(lastPoint, endPoint);
        else
            painter->drawRect(endPoint, lastPoint);
        if (drawState == StateOfShape::SOS_NewPermanentShape)
        {
            setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin, currentPen)));
            update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    case GeometricShape::GS_Square:
    {
        painter->drawSquare(lastPoint, endPoint);
        if(drawState == StateOfShape::SOS_NewPermanentShape)
        {
            setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin, currentPen)));
            update(QRect(lastPoint,endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        }
        else
            update();
        break;
    }
    case GeometricShape::GS_Circle:
        painter->drawCircle(lastPoint, endPoint);
        if (drawState == StateOfShape::SOS_NewPermanentShape)
            setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint, origin, currentPen)));
        update();
        break;
    case GeometricShape::GS_Triangle:
    {
        if (drawState == StateOfShape::SOS_NewPermanentShape)
            switch (triangleTypeID)
            {
            case 0: //triangle
                painter->drawTriangle(lastPoint, lastPoint_2, endPoint);
                setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint, origin, currentPen)));
                update();
                break;
            case 1:// // Isosceles Right Triangle
                painter->drawIsoscelesRightTriangle(lastPoint, endPoint);
                setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, endPoint,origin, currentPen)));
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
    case GeometricShape::GS_Parallelogram:
        painter->drawParallelogram(lastPoint, lastPoint_2, endPoint);
        if (drawState == StateOfShape::SOS_NewPermanentShape)
            setOfShapes.push_back(QSharedPointer<SDI_GeometricShape>(new SDI_GeometricShape(draw2DObjectMode, lastPoint, lastPoint_2, endPoint, origin, currentPen)));
        update();
        break;
    default:
        break;
    }
    modified = true;
    emit modificationChanged(modified);
}

void draw2DWidget::draw3DShape(SDI_Painter *painter, const SDI_Point &endPoint, StateOfShape drawState)
{
    switch (draw3DObjectMode)
    {
    case Geometric3DShape::G3DS_Parallelepiped:
        if (!lastPoint.isNull() && lastPoint_2.isNull())
        {
           int xTrans{endPoint.y() - lastPoint.y()};
           SDI_Point tempPoint(lastPoint.x() - xTrans, endPoint.y());
           painter->drawParallelogram(lastPoint, tempPoint, endPoint);
        }
        else
            painter->drawParallelePiped(lastPoint, lastPoint_2, endPoint, currentPen);
        if (drawState == StateOfShape::SOS_NewPermanentShape)
        {
            setOf3DShapes.push_back(QSharedPointer<SDI_Geometric3DShape>(new SDI_Geometric3DShape(draw3DObjectMode, lastPoint, lastPoint_2, endPoint, origin, currentPen)));
        }
        update();
        break;
    case Geometric3DShape::G3DS_Pyramid:
        if (!lastPoint.isNull() && lastPoint_2.isNull())
        {
            int xTrans{endPoint.y() - lastPoint.y()};
            SDI_Point tempPoint(lastPoint.x() - xTrans, endPoint.y());
            painter->drawParallelogram(lastPoint, tempPoint, endPoint);
        }
        else
            painter->drawPyramid(lastPoint, lastPoint_2, endPoint, currentPen);
        if (drawState == StateOfShape::SOS_NewPermanentShape)
        {
            setOf3DShapes.push_back(QSharedPointer<SDI_Geometric3DShape>(new SDI_Geometric3DShape(draw3DObjectMode, lastPoint, lastPoint_2, endPoint, origin, currentPen)));
        }
        update();
    default:
        break;
    }
    modified = true;
    emit modificationChanged(modified);
}

void draw2DWidget::drawExistentObject(SDI_Painter *painter)
{
    painter->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if (!setOfShapes.isEmpty() && graphicMode == GraphicsMode::GM_2D)
        for (QSharedPointer<SDI_GeometricShape>& shape:setOfShapes)
        {
            QVector<SDI_Point> setOfPoints(shape->getSetOfPoints());
            GeometricShape shapeName{shape->getShapeId()};
            if (shapeName == GeometricShape::GS_Rect || shapeName == GeometricShape::GS_Square || shapeName == GeometricShape::GS_Parallelogram)
                painter->drawTetragon(setOfPoints, shape->getShapePen());
            else if (shapeName == GeometricShape::GS_Line)
                painter->drawLine(setOfPoints.at(0), setOfPoints.at(1), shape->getShapePen());
            else if (shapeName == GeometricShape::GS_Circle)
                painter->drawCircle(setOfPoints.at(0), setOfPoints.at(1), shape->getShapePen());
            else if (shapeName == GeometricShape::GS_Triangle)
                painter->drawTriangle(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2), shape->getShapePen());
        }
    else if (!setOf3DShapes.isEmpty() && graphicMode == GraphicsMode::GM_3D)
        for (QSharedPointer<SDI_Geometric3DShape> shape:setOf3DShapes)
        {
            QVector<SDI_Point> setOfPoints(shape->getSetOfPoints());
            Geometric3DShape shapeName{shape->getShapeID()};
            painter->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            if (shapeName == Geometric3DShape::G3DS_Parallelepiped)
                painter->drawParallelePiped(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2), shape->getShapePen());
            else if (shapeName == Geometric3DShape::G3DS_Pyramid)
                painter->drawPyramid(setOfPoints.at(0), setOfPoints.at(1), setOfPoints.at(2), shape->getShapePen());
        }
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
        draw2DObjectMode = GeometricShape::GS_Line;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 2:
        draw2DObjectMode = GeometricShape::GS_Rect;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 3:
        draw2DObjectMode = GeometricShape::GS_Square;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 4:
        draw2DObjectMode = GeometricShape::GS_Parallelogram;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 5:
        draw2DObjectMode = GeometricShape::GS_Circle;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 6:
        draw2DObjectMode = GeometricShape::GS_Triangle;
        setCursor(Qt::ClosedHandCursor);
        break;
    default:
        draw2DObjectMode = GeometricShape::GS_SelectShape;
        setCursor(Qt::PointingHandCursor);
        break;
    }
    lastPoint = lastPoint_2 =  SDI_Point(0, 0); // reset recorded position
    delegateMode = DrawLineDelegateMode::DLDM_None;
}

void draw2DWidget::setDraw3DObjectMode(int newId)
{
    switch (newId)
    {
    case 0:
        draw3DObjectMode = Geometric3DShape::G3DS_Parallelepiped;
        setCursor(Qt::ClosedHandCursor);
        break;
    case 1:
        draw3DObjectMode = Geometric3DShape::G3DS_Pyramid;
        setCursor(Qt::ClosedHandCursor);
        break;
    }
    lastPoint = lastPoint_2 =  SDI_Point(0, 0); // reset recorded position
    delegateMode = DrawLineDelegateMode::DLDM_None;
}

void draw2DWidget::setGraphicsMode(GraphicsMode newMode)
{
    // change graphics mode and clear for new session
    graphicMode = newMode;
    clearImage(ClearImageMode::CIM_ForNewSession);
    emit graphicModeChanged(newMode == GraphicsMode::GM_2D ? "Chế độ -> :2 :D" : "Chế độ -> :3 :D");
    emit selectedShape(QSharedPointer<SDI_GeometricShape>(nullptr));
    //-------------------------finish-------------------------------
}

void draw2DWidget::setDisplayCoordinateMode(DisplayCoordinateState newState)
{
    displayCoordinateMode = newState;
    drawObject(SDI_Point(0,0), StateOfShape::SOS_AllExistentShapes);
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

