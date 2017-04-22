#include "sdi_painter.h"

SDI_Painter::SDI_Painter() : QPainter()
{

}

SDI_Painter::SDI_Painter(QPaintDevice *device) : QPainter(device)
{

}

void SDI_Painter::drawOxy(int width, int height, SDI_Point &Origin)
{
    SDI_Point leftMostOx{0, height/2};
    SDI_Point rightMostOx{width, height/2};
    drawLine(leftMostOx, rightMostOx); // draw x-axis
    drawText(rightMostOx + SDI_Point(-20, 20), "x");

    SDI_Point topMostOy{width/2, 0};
    SDI_Point bottomMostOy{width/2, height};
    drawLine(topMostOy, bottomMostOy);
    drawText(topMostOy + SDI_Point(20, 20), "y"); // draw y-axis
    drawText(Origin + SDI_Point(10, -10), "O");

    /*int yUP{Origin.y() + factor/2};
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
    }*/
}

void SDI_Painter::drawOxyz(int width, int height, SDI_Point &Origin)
{
    drawText(Origin + SDI_Point(20, -20), "O");
    SDI_Point rightMostOy{width, height/2};
    drawLine(Origin, rightMostOy);
    drawText(rightMostOy + SDI_Point(-20, -20), "y");

    SDI_Point topMostOz{width/2, 0};
    drawLine(Origin, topMostOz);
    drawText(topMostOz + SDI_Point(20, 20), "z");

    SDI_Point bottomMostOx{Origin.x() - height/2, Origin.y() + height/2};
    drawLine(Origin, bottomMostOx);
    drawText(bottomMostOx + SDI_Point(-20, -20), "x");
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
            midPointYLine(p2, p1); // draw from
        else
            midPointYLine(p1, p2 );
    }
    else
    {
        if (p1.y() <= p2.y())
            midPointXLine(p1, p2); // draw from
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
    SDI_Point topCirclePoint(centralPoint.translate(0, -radius));
    drawPoint(centralPoint);
    midPointXYCircle(topCirclePoint, centralPoint, radius);
}

void SDI_Painter::drawTriangle(const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3)
{
    drawPoint(point1);
    drawPoint(point2);
    drawPoint(point3);
    drawLine(point1, point2);
    drawLine(point2, point3);
    drawLine(point3, point1);
}

void SDI_Painter::drawIsoscelesRightTriangle(const SDI_Point &cpoint, const SDI_Point &epoint)
{
    double legLength{SDI_Point::distance(cpoint, epoint)};
    SDI_Point topLegPoint(cpoint.translate(0, cpoint.y() > epoint.y() ? -legLength : legLength));
    SDI_Point sideLegPoint(cpoint.translate((cpoint.x() < epoint.x() ? legLength : -legLength), 0));
    drawLine(cpoint, topLegPoint); // draw leg 1
    drawLine(cpoint, sideLegPoint); // draw leg 2
    drawLine(topLegPoint, sideLegPoint); // draw hypotenuse
}

void SDI_Painter::drawParallelogram(const SDI_Point &pointA, const SDI_Point &pointB, const SDI_Point &pointC)
{
    SDI_Point pointD;
    pointD.setX(pointC.x() + pointA.x() - pointB.x());
    pointD.setY(pointA.y() - pointB.y() + pointC.y());
    drawLine(pointA, pointB);
    drawLine(pointB, pointC);
    drawLine(pointC, pointD);
    drawLine(pointD, pointA);
}

void SDI_Painter::midPointYLine(const SDI_Point &p1, const SDI_Point &p2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    SDI_Point firstPoint{(p1.y() <= p2.y() ? p1 : p2)};
    SDI_Point laststPoint{(p1.y() <= p2.y() ? p2 : p1)};
    double x{double(firstPoint.x())};

    for (int y{firstPoint.y()}; y < laststPoint.y(); y++)
    {
        double xNext(x >= laststPoint.x() ? x - 1/2 : x + 1/2);
        if (aFactor*xNext + bFactor*y + cFactor >= 0 && x >= laststPoint.x()) x--;
        else if (aFactor*xNext + bFactor*y + cFactor >= 0 && x < laststPoint.x()) x++;
        drawPoint(x, y);
    }
}

void SDI_Painter::midPointXLine(const SDI_Point &p1, const SDI_Point &p2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    SDI_Point firstPoint{(p1.x() <= p2.x() ? p1 : p2)};
    SDI_Point laststPoint{(p1.x() <= p2.x() ? p2 : p1)};
    double y{double(firstPoint.y())};
    for (int x{firstPoint.x()}; x < laststPoint.x(); x++)
    {
        double yNext(y >= laststPoint.y() ? y - 1/2 : y + 1/2);
        if (aFactor*x + bFactor*yNext + cFactor >= 0 && y >= laststPoint.y()) y--;
        else if (aFactor*x + bFactor*yNext + cFactor >= 0 && y < laststPoint.y()) y++;
        drawPoint(x, y);
    }
}

void SDI_Painter::midPointXYCircle(const SDI_Point &topCirclePoint, const SDI_Point &centralCirclePoint, double radius)
{
    double y{double(topCirclePoint.y())};
    int xMax{static_cast<int>(radius*qSqrt(2.0)/2)};
    for (int x{topCirclePoint.x()}; x <= topCirclePoint.x() + xMax; x++) // from x = topPoint.x() to topPoint.x() + R*sqrt(2.0)/2
    {
        double yNext(y + 1/2); // midpoint
        if (qPow(x - centralCirclePoint.x(), 2.0) + qPow(yNext - centralCirclePoint.y(), 2.0) - qPow(radius, 2.0) >= 0.0) y++;
        // if (x-a)^2 + (yNext-b)^2 - radius^2 >= 0
        // y' = y + 1 -> machine coordinate system
        drawPoint(x, y); // draw next point (x, y')
        drawPoint(SDI_Point(x,y).centralSymmetry(centralCirclePoint));
        drawPoint(SDI_Point(x,y).centralSymmetry(QPoint(x, centralCirclePoint.y())));
        drawPoint(SDI_Point(x,y).centralSymmetry(QPoint(centralCirclePoint.x(), y)));
    }

    double x{double(topCirclePoint.x() + xMax)};
    for (; y <= centralCirclePoint.y(); y++) // continue from the last point we drew
    {
        double xNext(x + 1/2); // midpoint
        if (qPow(xNext - centralCirclePoint.x(), 2.0) + qPow(y - centralCirclePoint.y(), 2.0) - qPow(radius, 2.0) < 0.0) x++;
        // if (xNext-a)^2 + (y-b)^2 - radius^2 < 0
        // x' = x + 1 -> machine coordinate system
        drawPoint(x, y);
        drawPoint(SDI_Point(x,y).centralSymmetry(centralCirclePoint));
        drawPoint(SDI_Point(x,y).centralSymmetry(QPoint(x, centralCirclePoint.y())));
        drawPoint(SDI_Point(x,y).centralSymmetry(QPoint(centralCirclePoint.x(), y)));
    }

}
