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
    drawMode = draw2DMode::pencil;
    myPenWidth = 2;
    myPenColor = Qt::blue;
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
    paintOxy(painter);
}

void draw2DWidget::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    SDI_Painter painter(&image);
    paintOxy(painter);
    modified = true;
    update();
}

void draw2DWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawMode == draw2DMode::pencil)
    {
        lastPoint = event->pos();
        //scribbling = true;
    }
}

void draw2DWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton  && drawMode == draw2DMode::pencil )
        drawLineTo(event->pos());
}

void draw2DWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton  && drawMode == draw2DMode::pencil)
    {
        drawLineTo(event->pos());
        //scribbling = false;
    }
}

void draw2DWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
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

void draw2DWidget::drawLineTo(const QPoint &endPoint)
{
    SDI_Painter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
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

void draw2DWidget::paintOxy(QPainter &painter)
{
    QPoint OPoint(width()/2, height()/2);
    QPoint OyTop(width()/2, 0);
    QPoint OyBot(width()/2, height());
    QPoint OxLeft(0, height()/2);
    QPoint OxRight(width(), height()/2);
    painter.setPen(QPen(Qt::gray, 2, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(OyBot,OyTop);
    painter.drawLine(OxLeft, OxRight);
    //--- Draw Origin Point
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(OPoint);
    painter.drawText(OPoint + QPoint(-20, 20), "O" );
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

