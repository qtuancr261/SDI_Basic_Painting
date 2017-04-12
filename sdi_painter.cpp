#include "sdi_painter.h"

SDI_Painter::SDI_Painter() : QPainter()
{

}

SDI_Painter::SDI_Painter(QPaintDevice *device) : QPainter(device)
{

}

void SDI_Painter::drawOxy(int width, int height, SDI_Point &Origin)
{
    int factor{10};
    SDI_Point leftMostOx{0, height/2};
    SDI_Point rightMostOx{width, height/2};
    drawLine(leftMostOx, rightMostOx);
    drawText(rightMostOx + SDI_Point(-20, 20), "x");

    SDI_Point topMostOy{width/2, 0};
    SDI_Point bottomMostOy{width/2, height};
    drawLine(topMostOy, bottomMostOy);
    drawText(topMostOy + SDI_Point(20, 20), "y");

    drawText(Origin + SDI_Point(10, -10), "O");

    int yUP{Origin.y() + factor/2};
    int yDOWN{Origin.y() - factor/2};
    for (int xleft{Origin.x()}, xright{Origin.x()}; xleft < width; xleft += factor, xright -= factor)
    {
        drawLine(SDI_Point(xleft, yUP), SDI_Point(xleft, yDOWN));
        drawLine(SDI_Point(xright, yUP), SDI_Point(xright, yDOWN));
    }
    int xLEFT{Origin.x() + factor/2};
    int xRIGHT{Origin.x() - factor/2};
    for (int yup{Origin.y() - factor}, ydown{Origin.y() + factor}; yup > 0; yup -= factor, ydown += factor)
    {
        drawLine(SDI_Point(xLEFT, yup), SDI_Point(xRIGHT, yup));
        drawLine(SDI_Point(xLEFT, ydown), SDI_Point(xRIGHT, ydown));
    }

}

void SDI_Painter::drawPoint(const SDI_Point &p1)
{
    QPainter::drawPoint(p1);
}

void SDI_Painter::drawPoint(int x, int y)
{
    QPainter::drawPoint(x,y);
}

void SDI_Painter::drawLine(const SDI_Point &p1,const SDI_Point &p2)
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

void SDI_Painter::drawRect(const SDI_Point &topLeft, const SDI_Point &bottomRight)
{
    SDI_Point topRight{bottomRight.x(), topLeft.y()};
    SDI_Point bottomLeft{topLeft.x(), bottomRight.y()};
    drawLine(topLeft, topRight);
    drawLine(topLeft, bottomLeft);
    drawLine(topRight, bottomRight);
    drawLine(bottomLeft, bottomRight);
}

void SDI_Painter::drawSquare(const SDI_Point &firstPoint, SDI_Point &lastPoint)
{
    int rectWidth{std::abs(lastPoint.x() - firstPoint.x())};
    int rectHeight{std::abs(lastPoint.y() - firstPoint.y())};
    if (rectWidth > rectHeight)
    {
        if (lastPoint.x() > firstPoint.x())
            lastPoint.rx() -= (rectWidth - rectHeight);
        else
            lastPoint.rx() += (rectWidth - rectHeight);
    }
    else if (rectWidth < rectHeight)
    {
        if (lastPoint.y() > firstPoint.y())
            lastPoint.ry() -= (rectHeight - rectWidth);
        else
            lastPoint.ry() += (rectHeight - rectWidth);
    }
    SDI_Point topRight{lastPoint.x(), firstPoint.y()};
    SDI_Point bottomLeft{firstPoint.x(), lastPoint.y()};
    drawLine(firstPoint, topRight);
    drawLine(firstPoint, bottomLeft);
    drawLine(topRight, lastPoint);
    drawLine(bottomLeft, lastPoint);
}

void SDI_Painter::drawCircle(const SDI_Point &centralPoint, const SDI_Point &pointOnCircle)
{
    double radius {SDI_Point::distance(centralPoint, pointOnCircle)};
    SDI_Point firstCirclePoint(centralPoint.translate(0, -radius));
}

void SDI_Painter::midPointYLine(const SDI_Point &p1, const SDI_Point &p2)
{
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    SDI_Point firstPoint{(p1.y() <= p2.y() ? p1 : p2)};
    SDI_Point laststPoint{(p1.y() <= p2.y() ? p2 : p1)};
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

void SDI_Painter::midPointXLine(const SDI_Point &p1, const SDI_Point &p2)
{
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    SDI_Point firstPoint{(p1.x() <= p2.x() ? p1 : p2)};
    SDI_Point laststPoint{(p1.x() <= p2.x() ? p2 : p1)};
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
