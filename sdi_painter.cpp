#include "sdi_painter.h"

SDI_Painter::SDI_Painter() : QPainter()
{

}

SDI_Painter::SDI_Painter(QPaintDevice *device) : QPainter(device)
{

}

void SDI_Painter::drawOxy(int width, int height, QPoint &Origin)
{
    int factor{10};
    QPoint leftMostOx{0, height/2};
    QPoint rightMostOx{width, height/2};
    drawLine(leftMostOx, rightMostOx);
    drawText(rightMostOx + QPoint(-20, 20), "x");

    QPoint topMostOy{width/2, 0};
    QPoint bottomMostOy{width/2, height};
    drawLine(topMostOy, bottomMostOy);
    drawText(topMostOy + QPoint(20, 20), "y");

    drawText(Origin + QPoint(10, -10), "O");

    int yUP{Origin.y() + factor/2};
    int yDOWN{Origin.y() - factor/2};
    for (int xleft{Origin.x()}, xright{Origin.x()}; xleft < width; xleft += factor, xright -= factor)
    {
        drawLine(QPoint(xleft, yUP), QPoint(xleft, yDOWN));
        drawLine(QPoint(xright, yUP), QPoint(xright, yDOWN));
    }
    int xLEFT{Origin.x() + factor/2};
    int xRIGHT{Origin.x() - factor/2};
    for (int yup{Origin.y() - factor}, ydown{Origin.y() + factor}; yup > 0; yup -= factor, ydown += factor)
    {
        drawLine(QPoint(xLEFT, yup), QPoint(xRIGHT, yup));
        drawLine(QPoint(xLEFT, ydown), QPoint(xRIGHT, ydown));
    }

}

void SDI_Painter::drawLine(const QPoint &p1,const QPoint &p2)
{
     //QPainter::drawLine(p1, p2);
    if (std::abs(p1.y() - p2.y()) >= std::abs(p1.x() - p2.x()))
    {
        if (p1.x() < p2.x())
            midPointYLine(p2, p1);
        else
            midPointYLine(p1, p2 );
    }
    else
    {
        if (p1.y() <= p2.y())
            midPointXLine(p1, p2);
        else
            midPointXLine(p2, p1);
    }
}

void SDI_Painter::drawRect(const QPoint &topLeft, const QPoint &bottomRight)
{
    QPoint topRight{bottomRight.x(), topLeft.y()};
    QPoint bottomLeft{topLeft.x(), bottomRight.y()};
    drawLine(topLeft, topRight);
    drawLine(topLeft, bottomLeft);
    drawLine(topRight, bottomRight);
    drawLine(bottomLeft, bottomRight);
}

void SDI_Painter::midPointYLine(const QPoint &p1, const QPoint &p2)
{
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    QPoint firstPoint{(p1.y() <= p2.y() ? p1 : p2)};
    QPoint laststPoint{(p1.y() <= p2.y() ? p2 : p1)};
    double x{double(firstPoint.x())};
    //drawPoint(firstPoint);
    for (int y{firstPoint.y()}; y < laststPoint.y(); y++)
    {
        double xNext(x >= laststPoint.x() ? x - 1/2 : x + 1/2);
        if (aFactor*xNext + bFactor*y + cFactor >= 0 && x >= laststPoint.x()) x--;
        else if (aFactor*xNext + bFactor*y + cFactor >= 0 && x < laststPoint.x()) x++;
        drawPoint(x, y);
    }
    //drawPoint(laststPoint);
}

void SDI_Painter::midPointXLine(const QPoint &p1, const QPoint &p2)
{
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    QPoint firstPoint{(p1.x() <= p2.x() ? p1 : p2)};
    QPoint laststPoint{(p1.x() <= p2.x() ? p2 : p1)};
    double y{double(firstPoint.y())};
    //drawPoint(firstPoint);
    for (int x{firstPoint.x()}; x < laststPoint.x(); x++)
    {
        double yNext(y >= laststPoint.y() ? y - 1/2 : y + 1/2);
        if (aFactor*x + bFactor*yNext + cFactor >= 0 && y >= laststPoint.y()) y--;
        else if (aFactor*x + bFactor*yNext + cFactor >= 0 && y < laststPoint.y()) y++;
        drawPoint(x, y);
    }
}
