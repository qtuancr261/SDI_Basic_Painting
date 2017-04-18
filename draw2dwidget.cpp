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
    myPenColor = Qt::blue;
    triangleTypeID = 0; // normal
    setMouseTracking(true);
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

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
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

void draw2DWidget::showGUI()
{
    SDI_Painter painter(&image);
    painter.drawOxy(this->width(), this->height(), origin);
}

void draw2DWidget::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    origin = QPoint(width()/2, height()/2);
    SDI_Painter painter(&image);
    painter.drawOxy(this->width(), this->height(), origin);
    modified = true;
    update();
}

void draw2DWidget::mousePressEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton)
    switch (drawMode)
    {
    case draw2DMode::point:
        drawObject(eventPos);
        break;
    case draw2DMode::line:
    case draw2DMode::rect:
    case draw2DMode::square:
    case draw2DMode::circle:
        if (lastPoint.isNull())
            lastPoint = eventPos;
        else
        {
            drawObject(eventPos);
            lastPoint = QPoint(0, 0); // set to null
        }
        break;
    case draw2DMode::triangle:
        switch (triangleTypeID)
        {
        case 0: // Triangle
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else if (lastPoint_2.isNull())
                lastPoint_2 = eventPos;
            else
            {
                drawObject(eventPos);
                lastPoint = lastPoint_2 = QPoint(0, 0); // reset to null
            }
            break;
        case 1: // Isosceles Right Triangle
            if (lastPoint.isNull())
                lastPoint = eventPos;
            else
            {
                drawObject(eventPos);
                lastPoint = QPoint(0, 0); // set to null
            }
            break;
        }
        break;
    case draw2DMode::parallelogram:
        if (lastPoint.isNull())
            lastPoint = eventPos;
        else if (lastPoint_2.isNull())
            lastPoint_2 = eventPos;
        else
        {
            drawObject(eventPos);
            lastPoint = lastPoint_2 = QPoint(0, 0);
        }
        break;
    default: // scribbling mode
        if (event->button() == Qt::LeftButton)
            lastPoint = eventPos;
        break;
    }
}

void draw2DWidget::mouseMoveEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->buttons() == Qt::LeftButton  && drawMode == draw2DMode::normal )
        drawObject(eventPos);
    //emit mouseMoveTo();
    emit mouseMoveTo(QString("(x = %1| y = %2)").arg(QString::number((event->pos().x() - origin.x())))
                                                .arg(QString::number((origin.y() - event->pos().y()))));

}

void draw2DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    SDI_Point eventPos(event->pos());
    if (event->button() == Qt::LeftButton  && drawMode == draw2DMode::normal)
    {
        drawObject(eventPos);
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

void draw2DWidget::drawObject(const SDI_Point &endPoint) // handle draw Object
{
    SDI_Painter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    int rad = (myPenWidth / 2) + 2;
    modified = true;
    //------------------------------ set up panter ------------------//


    //using painter to draw current 2D Object
    switch (drawMode)
    {
    case draw2DMode::point:
        painter.drawPoint(endPoint);
        update(QRect(endPoint, endPoint + QPoint(10, 10)).normalized().adjusted(-rad, -rad, +rad, +rad));
        break;
    case draw2DMode::normal:
    case draw2DMode::line:
        painter.drawLine(lastPoint, endPoint);
        update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        lastPoint = endPoint;
        break;
    case draw2DMode::rect:
        if (lastPoint.x() < endPoint.x())
            painter.drawRect(lastPoint, endPoint);
        else
            painter.drawRect(endPoint, lastPoint);
        update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
        break;
    case draw2DMode::square:
    {
        SDI_Point exactPoint(endPoint);
        painter.drawSquare(lastPoint, exactPoint);
        update(QRect(lastPoint, exactPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    }
        break;
    case draw2DMode::circle:
        painter.drawCircle(lastPoint, endPoint);
        update();
        break;
    case draw2DMode::triangle:
    {
        switch (triangleTypeID)
        {
        case 0: //triangle
            painter.drawTriangle(lastPoint, lastPoint_2, endPoint);
            update();
            break;
        case 1:// // Isosceles Right Triangle
            painter.drawIsoscelesRightTriangle(lastPoint, endPoint);
            update();
            break;
        }
        break;
    }
    case draw2DMode::parallelogram:
        painter.drawParallelogram(lastPoint, lastPoint_2, endPoint);
        update();
        break;
    }
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

void draw2DWidget::setDraw2DObjectMode(int id)
{
    switch (id) {
    case 1:
        drawMode = draw2DMode::point;
        break;
    case 2:
        drawMode = draw2DMode::line;
        break;
    case 3:
        drawMode = draw2DMode::rect;
        break;
    case 4:
        drawMode = draw2DMode::square;
        break;
    case 5:
        drawMode = draw2DMode::parallelogram;
        break;
    case 6:
        drawMode = draw2DMode::circle;
        break;
    case 7:
        drawMode = draw2DMode::triangle;
        break;
    default:
        drawMode = draw2DMode::normal;
        break;
    }
    lastPoint = lastPoint_2 =  QPoint(0, 0); // reset
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

