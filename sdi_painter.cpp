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

QString SDI_Painter::getLineData(const SDI_Point &p1, const SDI_Point &p2, const SDI_Point &Origin, QRect &boundinRect)
{
    SDI_Point userP1(SDI_Point::convertToUserSystem(p1, Origin));
    SDI_Point userP2(SDI_Point::convertToUserSystem(p2, Origin));
    QString data;
    data = QString("<ul>"
                   "            <li>Hai điểm mút:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "            </ol>").arg(QString::number(userP1.x())).arg(QString::number(userP1.y()))
                                          .arg(QString::number(userP2.x())).arg(QString::number(userP2.y()));
    //----------------Calculate bounding rect---------------------------------
    int xLeft{(p1.x() < p2.x() ? p1.x() : p2.x())};
    int yLeft{(p1.y() < p2.y() ? p1.y() : p2.y())};
    int height{qAbs(p1.y() - p2.y())};
    int width{qAbs(p1.x() - p2.x())};
    boundinRect.setTopLeft(QPoint(xLeft, yLeft));
    boundinRect.setSize(QSize(width, height));

    return data;
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

QString SDI_Painter::getRectData(const SDI_Point &topLeft, const SDI_Point &bottomRight, const SDI_Point &Origin, QRect &boundinRect)
{
    SDI_Point topRight{bottomRight.x(), topLeft.y()};
    SDI_Point bottomLeft{topLeft.x(), bottomRight.y()};
    SDI_Point userTopL(SDI_Point::convertToUserSystem(topLeft, Origin));
    SDI_Point userTopR(SDI_Point::convertToUserSystem(topRight, Origin));
    SDI_Point userBotR(SDI_Point::convertToUserSystem(bottomRight, Origin));
    SDI_Point userBotL(SDI_Point::convertToUserSystem(bottomLeft, Origin));

    QString data;
    data = QString("<ul>"
                   "            <li>Bốn điểm mút:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "                  <li>(%5; %6)</li>"
                   "                  <li>(%7; %8)</li>"
                   "            </ol>").arg(QString::number(userTopL.x())).arg(QString::number(userTopL.y()))
                                          .arg(QString::number(userTopR.x())).arg(QString::number(userTopR.y()))
                                          .arg(QString::number(userBotR.x())).arg(QString::number(userBotR.y()))
                                          .arg(QString::number(userBotL.x())).arg(QString::number(userBotL.y()));

    //----------------Calculate bounding rect---------------------------------
    int xLeft{(topLeft.x() < bottomRight.x() ? topLeft.x() : bottomRight.x())};
    int yLeft{(topLeft.y() < bottomRight.y() ? topLeft.y() : bottomRight.y())};
    int height{qAbs(topLeft.y() - bottomRight.y())};
    int width{qAbs(topLeft.x() - bottomRight.x())};
    boundinRect.setTopLeft(QPoint(xLeft, yLeft));
    boundinRect.setSize(QSize(width, height));
    return data;
}

void SDI_Painter::drawSquare(const SDI_Point &firstPoint,const SDI_Point &lastPoint)
{
    SDI_Point exactPoint{lastPoint};
    int rectWidth{std::abs(exactPoint.x() - firstPoint.x())};
    int rectHeight{std::abs(exactPoint.y() - firstPoint.y())};
    if (rectWidth > rectHeight)
    {
        if (exactPoint.x() > firstPoint.x())
            exactPoint.rx() -= (rectWidth - rectHeight);
        else
            exactPoint.rx() += (rectWidth - rectHeight);
    }
    else if (rectWidth < rectHeight)
    {
        if (exactPoint.y() > firstPoint.y())
            exactPoint.ry() -= (rectHeight - rectWidth);
        else
            exactPoint.ry() += (rectHeight - rectWidth);
    }
    SDI_Point topRight{exactPoint.x(), firstPoint.y()};
    SDI_Point bottomLeft{firstPoint.x(), exactPoint.y()};
    drawLine(firstPoint, topRight);
    drawLine(firstPoint, bottomLeft);
    drawLine(topRight, exactPoint);
    drawLine(bottomLeft, exactPoint);
}

QString SDI_Painter::getSquareData(const SDI_Point &firstPoint, const SDI_Point &lastPoint, const SDI_Point &Origin, QRect &boundinRect)
{
    SDI_Point exactPoint{lastPoint};
    int rectWidth{qAbs(exactPoint.x() - firstPoint.x())};
    int rectHeight{qAbs(exactPoint.y() - firstPoint.y())};
    if (rectWidth > rectHeight)
    {
        if (exactPoint.x() > firstPoint.x())
            exactPoint.rx() -= (rectWidth - rectHeight);
        else
            exactPoint.rx() += (rectWidth - rectHeight);
    }
    else if (rectWidth < rectHeight)
    {
        if (exactPoint.y() > firstPoint.y())
            exactPoint.ry() -= (rectHeight - rectWidth);
        else
            exactPoint.ry() += (rectHeight - rectWidth);
    }
    SDI_Point topRight{exactPoint.x(), firstPoint.y()};
    SDI_Point bottomLeft{firstPoint.x(), exactPoint.y()};
    SDI_Point userTopL(SDI_Point::convertToUserSystem(firstPoint, Origin));
    SDI_Point userTopR(SDI_Point::convertToUserSystem(topRight, Origin));
    SDI_Point userBotR(SDI_Point::convertToUserSystem(exactPoint, Origin));
    SDI_Point userBotL(SDI_Point::convertToUserSystem(bottomLeft, Origin));

    QString data;
    data = QString("<ul>"
                   "            <li>Bốn điểm mút:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "                  <li>(%5; %6)</li>"
                   "                  <li>(%7; %8)</li>"
                   "            </ol>").arg(QString::number(userTopL.x())).arg(QString::number(userTopL.y()))
                                          .arg(QString::number(userTopR.x())).arg(QString::number(userTopR.y()))
                                          .arg(QString::number(userBotR.x())).arg(QString::number(userBotR.y()))
                                          .arg(QString::number(userBotL.x())).arg(QString::number(userBotL.y()));

    int xLeft{(topRight.x() < bottomLeft.x() ? topRight.x() : bottomLeft.x())};
    int yLeft{(topRight.y() < bottomLeft.y() ? topRight.y() : bottomLeft.y())};
    int height{qAbs(topRight.y() - bottomLeft.y())};
    boundinRect.setTopLeft(QPoint(xLeft, yLeft));
    boundinRect.setSize(QSize(height, height));

    return data;
}

void SDI_Painter::drawCircle(const SDI_Point &centralPoint, const SDI_Point &pointOnCircle)
{
    double radius {SDI_Point::distance(centralPoint, pointOnCircle)};
    SDI_Point topCirclePoint(centralPoint.translate(0, -radius));
    drawPoint(centralPoint);
    midPointXYCircle(topCirclePoint, centralPoint, radius);
}

QString SDI_Painter::getCircleData(const SDI_Point &centralPoint, const SDI_Point &pointOnCircle, const SDI_Point &Origin, QRect &boundinRect)
{
    int radius {static_cast<int>(SDI_Point::distance(centralPoint, pointOnCircle))};
    SDI_Point userCentralPoint(SDI_Point::convertToUserSystem(centralPoint, Origin));
    QString data;
    data = QString("<ul>"
                   "            <li>Thông số đường tròn:</li>"
                   "            <ol>"
                   "                  <li>Tâm: (%1; %2)</li>"
                   "                  <li>Bán kính: %3</li>"
                   "            </ol>").arg(QString::number(userCentralPoint.x())).arg(QString::number(userCentralPoint.y()))
                                          .arg(QString::number(radius));

    int xLeft{centralPoint.x() - radius};
    int yLeft{centralPoint.y() - radius};
    boundinRect.setTopLeft(QPoint(xLeft, yLeft));
    boundinRect.setSize(QSize(radius*2, radius*2));
    return data;

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
