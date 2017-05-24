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

void SDI_Painter::drawTetragon(QVector<SDI_Point> &points)
{
    drawLine(points.at(0), points.at(1));
    drawLine(points.at(1), points.at(2));
    drawLine(points.at(2), points.at(3));
    drawLine(points.at(3), points.at(0));
}

void SDI_Painter::drawTetragon(QVector<SDI_Point> &points, const QPen &tetragonPen)
{
    setPen(tetragonPen);
    drawTetragon(points);
}

void SDI_Painter::drawLine(const SDI_Point &p1,const SDI_Point &p2)
{
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

void SDI_Painter::drawLine(const SDI_Point &p1, const SDI_Point &p2, const QPen &linePen)
{
    setPen(linePen);
    drawLine(p1, p2);
}

void SDI_Painter::updateLineData(SDI_GeometricShape &shape)
{
    SDI_Point p1(shape.getSetOfPoints().at(0));
    SDI_Point p2(shape.getSetOfPoints().at(1));
    SDI_Point userP1(SDI_Point::convertToUserSystem(p1, shape.getOriginPos()));
    SDI_Point userP2(SDI_Point::convertToUserSystem(p2, shape.getOriginPos()));
    QString data;
    data = QString("<ul>"
                   "            <li>Hai điểm mút:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "            </ol>"
                   "            <li>Độ dài : %5 </li>"
                   "      </ul>").arg(QString::number(userP1.x())).arg(QString::number(userP1.y()))
                                          .arg(QString::number(userP2.x())).arg(QString::number(userP2.y()))
                                          .arg(QString::number(static_cast<int>(SDI_Point::distance(p1, p2))));
    //----------------Calculate bounding rect---------------------------------
    int xLeft{(p1.x() < p2.x() ? p1.x() : p2.x())};
    int yLeft{(p1.y() < p2.y() ? p1.y() : p2.y())};
    int height{qAbs(p1.y() - p2.y())};
    int width{qAbs(p1.x() - p2.x())};
    if (height <= 30 || width <= 30)
    {
        xLeft -= 20;
        yLeft -= 20;
    }
    shape.setShapeData(data);
    shape.setShapeBoundinRect(SDI_Point(xLeft, yLeft), QSize(width > 30 ? width : 80, height > 30 ? height : 80));
    shape.setCentralPoint(SDI_Point((p1.x() + p2.x())/2, (p1.y() + p2.y())/2));
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

void SDI_Painter::updateRectData(SDI_GeometricShape& shape)
{
    SDI_Point topLeft(shape.getSetOfPoints().at(0));
    SDI_Point bottomRight(shape.getSetOfPoints().at(1));
    SDI_Point topRight{bottomRight.x(), topLeft.y()};
    SDI_Point bottomLeft{topLeft.x(), bottomRight.y()};
    shape.getSetOfPoints().clear();
    shape.getSetOfPoints().push_back(topLeft);
    shape.getSetOfPoints().push_back(topRight);
    shape.getSetOfPoints().push_back(bottomRight);
    shape.getSetOfPoints().push_back(bottomLeft);
    updateTetragonData(shape);
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

void SDI_Painter::updateSquareData(SDI_GeometricShape& shape)
{
    SDI_Point firstPoint(shape.getSetOfPoints().at(0));
    SDI_Point exactPoint{shape.getSetOfPoints().at(1)};
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
    shape.getSetOfPoints().clear();
    shape.getSetOfPoints().push_back(firstPoint);
    shape.getSetOfPoints().push_back(topRight);
    shape.getSetOfPoints().push_back(exactPoint);
    shape.getSetOfPoints().push_back(bottomLeft);
    updateTetragonData(shape);
}

void SDI_Painter::drawCircle(const SDI_Point &centralPoint, const SDI_Point &pointOnCircle)
{
    double radius {SDI_Point::distance(centralPoint, pointOnCircle)};
    SDI_Point topCirclePoint(SDI_Point::translate(centralPoint, 0, -radius));
    drawPoint(centralPoint);
    midPointXYCircle(topCirclePoint, centralPoint, radius);
}

void SDI_Painter::drawCircle(const SDI_Point &centralPoint, const SDI_Point &pointOnCircle, const QPen &circlePen)
{
    setPen(circlePen);
    drawCircle(centralPoint, pointOnCircle);
}

void SDI_Painter::updateCircleData(SDI_GeometricShape& shape)
{
    SDI_Point centralPoint{shape.getSetOfPoints().at(0)};
    SDI_Point pointOnCircle{shape.getSetOfPoints().at(1)};
    int radius {static_cast<int>(SDI_Point::distance(centralPoint, pointOnCircle))};
    SDI_Point userCentralPoint(SDI_Point::convertToUserSystem(centralPoint, shape.getOriginPos()));
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
    shape.setShapeBoundinRect(SDI_Point(xLeft, yLeft), QSize(radius*2, radius*2));
    shape.setShapeData(data);
    shape.setCentralPoint(centralPoint);
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

void SDI_Painter::drawTriangle(const SDI_Point &point1, const SDI_Point &point2, const SDI_Point &point3, const QPen &trianglePen)
{
    setPen(trianglePen);
    drawTriangle(point1, point2, point3);
}

void SDI_Painter::updateTriangleData(SDI_GeometricShape &shape)
{
    SDI_Point point1{shape.getSetOfPoints().at(0)};
    SDI_Point point2{shape.getSetOfPoints().at(1)};
    SDI_Point point3{shape.getSetOfPoints().at(2)};

    int xLeft{qMin(qMin(point1.x(), point2.x()), point3.x())};
    int xRight{qMax(qMax(point1.x(), point2.x()), point3.x())};
    int yLeft{qMin(qMin(point1.y(), point2.y()), point3.y())};
    int yRight{qMax(qMax(point1.y(), point2.y()), point3.y())};

    SDI_Point userP1{SDI_Point::convertToUserSystem(point1, shape.getOriginPos())};
    SDI_Point userP2{SDI_Point::convertToUserSystem(point2, shape.getOriginPos())};
    SDI_Point userP3{SDI_Point::convertToUserSystem(point3, shape.getOriginPos())};

    QString data;
    data = QString("<ul>"
                   "            <li>Ba đỉnh của tam giác:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "                  <li>(%5; %6)</li>"
                   "            </ol>").arg(QString::number(userP1.x())).arg(QString::number(userP1.y()))
                                       .arg(QString::number(userP2.x())).arg(QString::number(userP2.y()))
                                       .arg(QString::number(userP3.x())).arg(QString::number(userP3.y()));
    shape.setShapeBoundinRect(SDI_Point(xLeft, yLeft), QSize(xRight - xLeft, yRight - yLeft));
    shape.setShapeData(data);

    SDI_Point centralP1P2{(point1.x() + point2.x())/2, (point1.y() + point2.y())/2};
    SDI_Point centralP1P3{(point1.x() + point3.x())/2, (point1.y() + point3.y())/2};
    shape.setCentralPoint(SDI_Point((centralP1P2.x() + centralP1P3.x())/2, (centralP1P2.y() + centralP1P3.y())/2));
}

void SDI_Painter::drawIsoscelesRightTriangle(const SDI_Point &cpoint, const SDI_Point &epoint)
{
    double legLength{SDI_Point::distance(cpoint, epoint)};
    SDI_Point topLegPoint(SDI_Point::translate(cpoint, 0, cpoint.y() > epoint.y() ? -legLength : legLength));
    SDI_Point sideLegPoint(SDI_Point::translate(cpoint, (cpoint.x() < epoint.x() ? legLength : -legLength), 0));
    drawLine(cpoint, topLegPoint); // draw leg 1
    drawLine(cpoint, sideLegPoint); // draw leg 2
    drawLine(topLegPoint, sideLegPoint); // draw hypotenuse
}

void SDI_Painter::updateIRTriangleData(SDI_GeometricShape &shape)
{
    SDI_Point cpoint{shape.getSetOfPoints().at(0)};
    SDI_Point epoint{shape.getSetOfPoints().at(1)};
    double legLength{SDI_Point::distance(cpoint,epoint)};
    SDI_Point topLegPoint(SDI_Point::translate(cpoint, 0, cpoint.y() > epoint.y() ? -legLength : legLength));
    SDI_Point sideLegPoint(SDI_Point::translate(cpoint, (cpoint.x() < epoint.x() ? legLength : -legLength), 0));

    shape.getSetOfPoints().pop_back();
    shape.getSetOfPoints().push_back(topLegPoint);
    shape.getSetOfPoints().push_back(sideLegPoint);
    updateTriangleData(shape);
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

void SDI_Painter::updateParallelogramData(SDI_GeometricShape &shape)
{
    SDI_Point pointD;
    pointD.setX(shape.getSetOfPoints().at(2).x() + shape.getSetOfPoints().at(0).x() - shape.getSetOfPoints().at(1).x());
    pointD.setY(shape.getSetOfPoints().at(0).y() - shape.getSetOfPoints().at(1).y() + shape.getSetOfPoints().at(2).y());
    shape.getSetOfPoints().push_back(pointD);
    updateTetragonData(shape);

}

void SDI_Painter::updateTetragonData(SDI_GeometricShape &shape)
{
    SDI_Point pointA{shape.getSetOfPoints().at(0)};
    SDI_Point pointB{shape.getSetOfPoints().at(1)};
    SDI_Point pointC{shape.getSetOfPoints().at(2)};
    SDI_Point pointD{shape.getSetOfPoints().at(3)};

    SDI_Point userTopL(SDI_Point::convertToUserSystem(pointA, shape.getOriginPos()));
    SDI_Point userTopR(SDI_Point::convertToUserSystem(pointB, shape.getOriginPos()));
    SDI_Point userBotR(SDI_Point::convertToUserSystem(pointC, shape.getOriginPos()));
    SDI_Point userBotL(SDI_Point::convertToUserSystem(pointD, shape.getOriginPos()));

    int xLeft{qMin(qMin(pointA.x(), pointB.x()), qMin(pointC.x(), pointD.x()))};
    int xRight{qMax(qMax(pointA.x(), pointB.x()), qMax(pointC.x(), pointD.x()))};
    int yLeft{qMin(qMin(pointA.y(), pointB.y()), qMin(pointC.y(), pointD.y()))};
    int yRight{qMax(qMax(pointA.y(), pointB.y()), qMax(pointC.y(), pointD.y()))};

    QString data;
    data = QString("<ul>"
                   "            <li>Bốn đỉnh của hình:</li>"
                   "            <ol>"
                   "                  <li>(%1; %2)</li>"
                   "                  <li>(%3; %4)</li>"
                   "                  <li>(%5; %6)</li>"
                   "                  <li>(%7; %8)</li>"
                   "            </ol>").arg(QString::number(userTopL.x())).arg(QString::number(userTopL.y()))
                                          .arg(QString::number(userTopR.x())).arg(QString::number(userTopR.y()))
                                          .arg(QString::number(userBotR.x())).arg(QString::number(userBotR.y()))
                                          .arg(QString::number(userBotL.x())).arg(QString::number(userBotL.y()));

    shape.setShapeBoundinRect(SDI_Point(xLeft, yLeft), QSize(xRight - xLeft, yRight - yLeft));
    shape.setShapeData(data);
    shape.setCentralPoint(SDI_Point((pointA.x() + pointC.x())/2, (pointA.y() + pointC.y())/2));
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
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), centralCirclePoint));
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), SDI_Point(x, centralCirclePoint.y())));
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), SDI_Point(centralCirclePoint.x(), y)));
    }

    double x{double(topCirclePoint.x() + xMax)};
    for (; y <= centralCirclePoint.y(); y++) // continue from the last point we drew
    {
        double xNext(x + 1/2); // midpoint
        if (qPow(xNext - centralCirclePoint.x(), 2.0) + qPow(y - centralCirclePoint.y(), 2.0) - qPow(radius, 2.0) < 0.0) x++;
        // if (xNext-a)^2 + (y-b)^2 - radius^2 < 0
        // x' = x + 1 -> machine coordinate system
        drawPoint(x, y);
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), centralCirclePoint));
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), SDI_Point(x, centralCirclePoint.y())));
        drawPoint(SDI_Point::centralSymmetry(SDI_Point(x,y), SDI_Point(centralCirclePoint.x(), y)));
    }

}

void SDI_Painter::drawParallelePiped(const SDI_Point &point1, const SDI_Point &point3, const SDI_Point &pointHSize, const QPen &shapePen)
{
    setPen(shapePen);
    QPen specialPen(shapePen.color(), shapePen.width()+1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    int xTrans{point3.y() - point1.y()};
    SDI_Point point2(point1.x() - xTrans, point3.y());
    SDI_Point point4(point3.x() + xTrans, point1.y());
    int height{qAbs(pointHSize.y() - point3.y())};

    SDI_Point pointA(point1), pointB(point2), pointC(point3), pointD(point4);
    calculateBasePlaneABCD(pointA, pointB, pointC, pointD);

    SDI_Point pointE(pointA.x(), pointA.y() - height);
    SDI_Point pointF(pointB.x(), pointB.y() - height);
    SDI_Point pointG{pointC.x(), pointC.y() - height};
    SDI_Point pointH{pointD.x(), pointD.y() - height};

    drawText(pointA + SDI_Point(-10, -10), "A");
    drawText(pointB + SDI_Point(-10, -10), "B");
    drawText(pointC + SDI_Point(10, 10), "C");
    drawText(pointD + SDI_Point(10, -10), "D");
    drawText(pointE + SDI_Point(-10, -10), "E");
    drawText(pointF + SDI_Point(-10, -10), "F");
    drawText(pointG + SDI_Point(-10, -10), "G");
    drawText(pointH + SDI_Point(-10, -10), "H");

    QPainter::drawLine(pointB, pointF);
    QPainter::drawLine(pointC, pointG);
    QPainter::drawLine(pointD, pointH);

    QPainter::drawLine(pointB, pointC);
    QPainter::drawLine(pointC, pointD);

    QPainter::drawLine(pointE, pointF);
    QPainter::drawLine(pointF, pointG);
    QPainter::drawLine(pointG, pointH);
    QPainter::drawLine(pointH, pointE);

    this->setPen(specialPen);
    QPainter::drawLine(pointA, pointE);
    QPainter::drawLine(pointA, pointB);
    QPainter::drawLine(pointD, pointA);

}

void SDI_Painter::drawPyramid(const SDI_Point &point1, const SDI_Point &point3, const SDI_Point &pointHSize, const QPen &shapePen)
{
    setPen(shapePen);
    QPen specialPen(shapePen.color(), shapePen.width() + 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    int xTrans{point3.y() - point1.y()};
    SDI_Point point2(point1.x() - xTrans, point3.y());
    SDI_Point point4(point3.x() + xTrans, point1.y());
    int height{qAbs(pointHSize.y() - point3.y())};

    SDI_Point pointA(point1), pointB(point2), pointC(point3), pointD(point4);
    calculateBasePlaneABCD(pointA, pointB, pointC, pointD);
    SDI_Point pointE((pointA.x() + pointC.x())/2, (pointA.y() + pointC.y())/2);
    SDI_Point pointF(pointE.x(), pointE.y() - height);

    SDI_Point midPointAB((pointA.x() + pointB.x())/2, (pointA.y() + pointB.y())/2);
    int disBetweenE_midAB(SDI_Point::distance(midPointAB, pointE));
    int disBetweenE_F(SDI_Point::distance(pointE, pointF));

    drawText(pointA + SDI_Point(-10, -10), "A");
    drawText(pointB + SDI_Point(-10, -10), "B");
    drawText(pointC + SDI_Point(10, 10), "C");
    drawText(pointD + SDI_Point(10, -10), "D");
    drawText(pointE + SDI_Point(-10, -10), "E");
    drawText(pointF + SDI_Point(-10, 0), "F");

    QPainter::drawLine(pointB, pointC);
    QPainter::drawLine(pointC, pointD);
    QPainter::drawLine(pointB, pointF);
    QPainter::drawLine(pointC, pointF);
    QPainter::drawLine(pointD, pointF);

    if (disBetweenE_F > disBetweenE_midAB)
        setPen(specialPen);
    QPainter::drawLine(pointA, pointB);

    if (pointF.y() < pointA.y())
        setPen(specialPen);
    else if (pointF.y() >= pointA.y())
        setPen(shapePen);
    QPainter::drawLine(pointD, pointA);

    if (disBetweenE_F <= disBetweenE_midAB)
        setPen(shapePen);
    QPainter::drawLine(pointA, pointF);

    setPen(specialPen);
    QPainter::drawLine(pointE, pointF);
    QPainter::drawLine(pointA, pointC);
    QPainter::drawLine(pointB, pointD);
}

void SDI_Painter::calculateBasePlaneABCD(SDI_Point &pointA, SDI_Point &pointB, SDI_Point &pointC, SDI_Point &pointD)
{
    QVector<SDI_Point> setPoint({pointA, pointB, pointC, pointD});
    std::sort(std::begin(setPoint), std::end(setPoint));

    //---------Calculate--------------------
    if (qAbs(setPoint.at(1).x() - setPoint.at(3).x()) > qAbs(setPoint.at(1).y() - setPoint.at(3).y()))
    {
        pointA = setPoint.at(1);
        pointC = setPoint.at(2);
    }
    else
    {
        pointA = setPoint.at(2);
        pointC = setPoint.at(1);
    }
    pointB = setPoint.at(0);
    pointD = setPoint.at(3);
}
