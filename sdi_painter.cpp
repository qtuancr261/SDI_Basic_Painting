#include "sdi_painter.h"

SDI_Painter::SDI_Painter() : QPainter()
{

}

SDI_Painter::SDI_Painter(QPaintDevice *device) : QPainter(device)
{

}

void SDI_Painter::drawOxy(int width, int height, Point &Origin)
{
    Point leftMostOx{0, height/2};
    Point rightMostOx{width, height/2};
    drawLine(leftMostOx, rightMostOx); // draw x-axis
    drawText(rightMostOx + Point(-20, 20), "x");

    Point topMostOy{width/2, 0};
    Point bottomMostOy{width/2, height};
    drawLine(topMostOy, bottomMostOy);
    drawText(topMostOy + Point(20, 20), "y"); // draw y-axis
    drawText(Origin + Point(10, -10), "O");
}

void SDI_Painter::drawOxyz(int width, int height, Point &Origin)
{
    drawText(Origin + Point(20, -20), "O");
    Point rightMostOy{width, height/2};
    drawLine(Origin, rightMostOy);
    drawText(rightMostOy + Point(-20, -20), "y");

    Point topMostOz{width/2, 0};
    drawLine(Origin, topMostOz);
    drawText(topMostOz + Point(20, 20), "z");

    Point bottomMostOx{Origin.x() - height/2, Origin.y() + height/2};
    drawLine(Origin, bottomMostOx);
    drawText(bottomMostOx + Point(-20, -20), "x");
}

void SDI_Painter::drawPoint(const Point &p1)
{
    QPainter::drawPoint(p1);
}

void SDI_Painter::drawPoint(int x, int y)
{
    QPainter::drawPoint(x,y);
}

void SDI_Painter::drawTetragon(QVector<Point> &points)
{
    drawLine(points.at(0), points.at(1));
    drawLine(points.at(1), points.at(2));
    drawLine(points.at(2), points.at(3));
    drawLine(points.at(3), points.at(0));
}

void SDI_Painter::drawTetragon(QVector<Point> &points, const QPen &tetragonPen)
{
    setPen(tetragonPen);
    drawTetragon(points);
}

void SDI_Painter::drawLine(const Point &p1,const Point &p2)
{
    /*if (qAbs(p1.y() - p2.y()) >= qAbs(p1.x() - p2.x()))
    {
        if (p1.x() < p2.x())
            midPointYLine(p2, p1); // draw from
        else
            midPointYLine(p1, p2);
    }
    else
    {
        if (p1.y() <= p2.y())
            midPointXLine(p1, p2); // draw from
        else
            midPointXLine(p2, p1);
    }*/
    // Just ignore the previous section
    QPainter::drawLine(p1, p2);
}

void SDI_Painter::drawLine(const Point &p1, const Point &p2, const QPen &linePen)
{
    setPen(linePen);
    //drawLine(p1, p2);
    QPainter::drawLine(p1, p2);
}

void SDI_Painter::updateLineData(SDI_GeometricShape &shape)
{
    Point p1(shape.getSetOfPoints().at(0));
    Point p2(shape.getSetOfPoints().at(1));
    Point userP1(Point::convertToUserSystem(p1, shape.getOriginPos()));
    Point userP2(Point::convertToUserSystem(p2, shape.getOriginPos()));
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
                                          .arg(QString::number(static_cast<int>(Point::distance(p1, p2))));
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
    shape.setShapeBoundinRect(Point(xLeft, yLeft), QSize(width > 30 ? width : 80, height > 30 ? height : 80));
    shape.setCentralPoint(Point((p1.x() + p2.x())/2, (p1.y() + p2.y())/2));
}

void SDI_Painter::drawRect(const Point &topLeft, const Point &bottomRight)
{
    Point topRight{bottomRight.x(), topLeft.y()};
    Point bottomLeft{topLeft.x(), bottomRight.y()};
    drawLine(topLeft, topRight);
    drawLine(topLeft, bottomLeft);
    drawLine(topRight, bottomRight);
    drawLine(bottomLeft, bottomRight);
}

void SDI_Painter::updateRectData(SDI_GeometricShape& shape)
{
    Point topLeft(shape.getSetOfPoints().at(0));
    Point bottomRight(shape.getSetOfPoints().at(1));
    Point topRight{bottomRight.x(), topLeft.y()};
    Point bottomLeft{topLeft.x(), bottomRight.y()};
    shape.getSetOfPoints().clear();
    shape.getSetOfPoints().push_back(topLeft);
    shape.getSetOfPoints().push_back(topRight);
    shape.getSetOfPoints().push_back(bottomRight);
    shape.getSetOfPoints().push_back(bottomLeft);
    updateTetragonData(shape);
}

void SDI_Painter::drawSquare(const Point &firstPoint,const Point &lastPoint)
{
    Point exactPoint{lastPoint};
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
    Point topRight{exactPoint.x(), firstPoint.y()};
    Point bottomLeft{firstPoint.x(), exactPoint.y()};
    drawLine(firstPoint, topRight);
    drawLine(firstPoint, bottomLeft);
    drawLine(topRight, exactPoint);
    drawLine(bottomLeft, exactPoint);
}

void SDI_Painter::updateSquareData(SDI_GeometricShape& shape)
{
    Point firstPoint(shape.getSetOfPoints().at(0));
    Point exactPoint{shape.getSetOfPoints().at(1)};
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
    Point topRight{exactPoint.x(), firstPoint.y()};
    Point bottomLeft{firstPoint.x(), exactPoint.y()};
    shape.getSetOfPoints().clear();
    shape.getSetOfPoints().push_back(firstPoint);
    shape.getSetOfPoints().push_back(topRight);
    shape.getSetOfPoints().push_back(exactPoint);
    shape.getSetOfPoints().push_back(bottomLeft);
    updateTetragonData(shape);
}

void SDI_Painter::drawCircle(const Point &centralPoint, const Point &pointOnCircle)
{
    double radius {Point::distance(centralPoint, pointOnCircle)};
    //SDI_Point topCirclePoint(SDI_Point::translate(centralPoint, 0, -radius));
    //drawPoint(centralPoint);
    //midPointXYCircle(topCirclePoint, centralPoint, radius);
    QPainter::drawEllipse(centralPoint, static_cast<int>(radius), static_cast<int>(radius));
}

void SDI_Painter::drawCircle(const Point &centralPoint, const Point &pointOnCircle, const QPen &circlePen)
{
    setPen(circlePen);
    drawCircle(centralPoint, pointOnCircle);
}

void SDI_Painter::updateCircleData(SDI_GeometricShape& shape)
{
    Point centralPoint{shape.getSetOfPoints().at(0)};
    Point pointOnCircle{shape.getSetOfPoints().at(1)};
    int radius {static_cast<int>(Point::distance(centralPoint, pointOnCircle))};
    Point userCentralPoint(Point::convertToUserSystem(centralPoint, shape.getOriginPos()));
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
    shape.setShapeBoundinRect(Point(xLeft, yLeft), QSize(radius*2, radius*2));
    shape.setShapeData(data);
    shape.setCentralPoint(centralPoint);
}

void SDI_Painter::drawTriangle(const Point &point1, const Point &point2, const Point &point3)
{
    drawPoint(point1);
    drawPoint(point2);
    drawPoint(point3);
    drawLine(point1, point2);
    drawLine(point2, point3);
    drawLine(point3, point1);
}

void SDI_Painter::drawTriangle(const Point &point1, const Point &point2, const Point &point3, const QPen &trianglePen)
{
    setPen(trianglePen);
    drawTriangle(point1, point2, point3);
}

void SDI_Painter::updateTriangleData(SDI_GeometricShape &shape)
{
    Point point1{shape.getSetOfPoints().at(0)};
    Point point2{shape.getSetOfPoints().at(1)};
    Point point3{shape.getSetOfPoints().at(2)};

    int xLeft{qMin(qMin(point1.x(), point2.x()), point3.x())};
    int xRight{qMax(qMax(point1.x(), point2.x()), point3.x())};
    int yLeft{qMin(qMin(point1.y(), point2.y()), point3.y())};
    int yRight{qMax(qMax(point1.y(), point2.y()), point3.y())};

    Point userP1{Point::convertToUserSystem(point1, shape.getOriginPos())};
    Point userP2{Point::convertToUserSystem(point2, shape.getOriginPos())};
    Point userP3{Point::convertToUserSystem(point3, shape.getOriginPos())};

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
    shape.setShapeBoundinRect(Point(xLeft, yLeft), QSize(xRight - xLeft, yRight - yLeft));
    shape.setShapeData(data);

    Point centralP1P2{(point1.x() + point2.x())/2, (point1.y() + point2.y())/2};
    Point centralP1P3{(point1.x() + point3.x())/2, (point1.y() + point3.y())/2};
    shape.setCentralPoint(Point((centralP1P2.x() + centralP1P3.x())/2, (centralP1P2.y() + centralP1P3.y())/2));
}

void SDI_Painter::drawIsoscelesRightTriangle(const Point &cpoint, const Point &epoint)
{
    double legLength{Point::distance(cpoint, epoint)};
    Point topLegPoint(Point::translate(cpoint, 0, cpoint.y() > epoint.y() ? -legLength : legLength));
    Point sideLegPoint(Point::translate(cpoint, (cpoint.x() < epoint.x() ? legLength : -legLength), 0));
    drawLine(cpoint, topLegPoint); // draw leg 1
    drawLine(cpoint, sideLegPoint); // draw leg 2
    drawLine(topLegPoint, sideLegPoint); // draw hypotenuse
}

void SDI_Painter::updateIRTriangleData(SDI_GeometricShape &shape)
{
    Point cpoint{shape.getSetOfPoints().at(0)};
    Point epoint{shape.getSetOfPoints().at(1)};
    double legLength{Point::distance(cpoint,epoint)};
    Point topLegPoint(Point::translate(cpoint, 0, cpoint.y() > epoint.y() ? -legLength : legLength));
    Point sideLegPoint(Point::translate(cpoint, (cpoint.x() < epoint.x() ? legLength : -legLength), 0));

    shape.getSetOfPoints().pop_back();
    shape.getSetOfPoints().push_back(topLegPoint);
    shape.getSetOfPoints().push_back(sideLegPoint);
    updateTriangleData(shape);
}

void SDI_Painter::drawParallelogram(const Point &pointA, const Point &pointB, const Point &pointC)
{
    Point pointD;
    pointD.setX(pointC.x() + pointA.x() - pointB.x());
    pointD.setY(pointA.y() - pointB.y() + pointC.y());
    drawLine(pointA, pointB);
    drawLine(pointB, pointC);
    drawLine(pointC, pointD);
    drawLine(pointD, pointA);
}

void SDI_Painter::updateParallelogramData(SDI_GeometricShape &shape)
{
    Point pointD;
    pointD.setX(shape.getSetOfPoints().at(2).x() + shape.getSetOfPoints().at(0).x() - shape.getSetOfPoints().at(1).x());
    pointD.setY(shape.getSetOfPoints().at(0).y() - shape.getSetOfPoints().at(1).y() + shape.getSetOfPoints().at(2).y());
    shape.getSetOfPoints().push_back(pointD);
    updateTetragonData(shape);

}

void SDI_Painter::updateTetragonData(SDI_GeometricShape &shape)
{
    Point pointA{shape.getSetOfPoints().at(0)};
    Point pointB{shape.getSetOfPoints().at(1)};
    Point pointC{shape.getSetOfPoints().at(2)};
    Point pointD{shape.getSetOfPoints().at(3)};

    Point userTopL(Point::convertToUserSystem(pointA, shape.getOriginPos()));
    Point userTopR(Point::convertToUserSystem(pointB, shape.getOriginPos()));
    Point userBotR(Point::convertToUserSystem(pointC, shape.getOriginPos()));
    Point userBotL(Point::convertToUserSystem(pointD, shape.getOriginPos()));

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

    shape.setShapeBoundinRect(Point(xLeft, yLeft), QSize(xRight - xLeft, yRight - yLeft));
    shape.setShapeData(data);
    shape.setCentralPoint(Point((pointA.x() + pointC.x())/2, (pointA.y() + pointC.y())/2));
}

void SDI_Painter::midPointYLine(const Point &p1, const Point &p2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    Point firstPoint{(p1.y() <= p2.y() ? p1 : p2)};
    Point laststPoint{(p1.y() <= p2.y() ? p2 : p1)};
    double x{double(firstPoint.x())};

    for (int y{firstPoint.y()}; y < laststPoint.y(); y++)
    {
        double xNext(x >= laststPoint.x() ? x - 1/2 : x + 1/2);
        if (aFactor*xNext + bFactor*y + cFactor >= 0 && x >= laststPoint.x()) x--;
        else if (aFactor*xNext + bFactor*y + cFactor >= 0 && x < laststPoint.x()) x++;
        drawPoint(x, y);
    }
}

void SDI_Painter::midPointXLine(const Point &p1, const Point &p2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{p2.y() - p1.y()};
    int bFactor{-(p2.x() - p1.x())};
    int cFactor{p2.x()*p1.y() - p1.x()*p2.y()};
    Point firstPoint{(p1.x() <= p2.x() ? p1 : p2)};
    Point laststPoint{(p1.x() <= p2.x() ? p2 : p1)};
    double y{double(firstPoint.y())};
    for (int x{firstPoint.x()}; x < laststPoint.x(); x++)
    {
        double yNext(y >= laststPoint.y() ? y - 1/2 : y + 1/2);
        if (aFactor*x + bFactor*yNext + cFactor >= 0 && y >= laststPoint.y()) y--;
        else if (aFactor*x + bFactor*yNext + cFactor >= 0 && y < laststPoint.y()) y++;
        drawPoint(x, y);
    }
}

void SDI_Painter::midPointXYCircle(const Point &topCirclePoint, const Point &centralCirclePoint, double radius)
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
        drawPoint(Point::centralSymmetry(Point(x,y), centralCirclePoint));
        drawPoint(Point::centralSymmetry(Point(x,y), Point(x, centralCirclePoint.y())));
        drawPoint(Point::centralSymmetry(Point(x,y), Point(centralCirclePoint.x(), y)));
    }

    double x{double(topCirclePoint.x() + xMax)};
    for (; y <= centralCirclePoint.y(); y++) // continue from the last point we drew
    {
        double xNext(x + 1/2); // midpoint
        if (qPow(xNext - centralCirclePoint.x(), 2.0) + qPow(y - centralCirclePoint.y(), 2.0) - qPow(radius, 2.0) < 0.0) x++;
        // if (xNext-a)^2 + (y-b)^2 - radius^2 < 0
        // x' = x + 1 -> machine coordinate system
        drawPoint(x, y);
        drawPoint(Point::centralSymmetry(Point(x,y), centralCirclePoint));
        drawPoint(Point::centralSymmetry(Point(x,y), Point(x, centralCirclePoint.y())));
        drawPoint(Point::centralSymmetry(Point(x,y), Point(centralCirclePoint.x(), y)));
    }

}

void SDI_Painter::drawParallelePiped(const Point &point1, const Point &point3, const Point &pointHSize, const QPen &shapePen)
{
    setPen(shapePen);
    QPen specialPen(shapePen.color(), shapePen.width()+1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    int xTrans{point3.y() - point1.y()};
    Point point2(point1.x() - xTrans, point3.y());
    Point point4(point3.x() + xTrans, point1.y());
    int height{qAbs(pointHSize.y() - point3.y())};

    Point pointA(point1), pointB(point2), pointC(point3), pointD(point4);
    calculateBasePlaneABCD(pointA, pointB, pointC, pointD);

    Point pointE(pointA.x(), pointA.y() - height);
    Point pointF(pointB.x(), pointB.y() - height);
    Point pointG{pointC.x(), pointC.y() - height};
    Point pointH{pointD.x(), pointD.y() - height};

    drawText(pointA + Point(-10, -10), "A");
    drawText(pointB + Point(-10, -10), "B");
    drawText(pointC + Point(10, 10), "C");
    drawText(pointD + Point(10, -10), "D");
    drawText(pointE + Point(-10, -10), "E");
    drawText(pointF + Point(-10, -10), "F");
    drawText(pointG + Point(-10, -10), "G");
    drawText(pointH + Point(-10, -10), "H");

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

void SDI_Painter::drawPyramid(const Point &point1, const Point &point3, const Point &pointHSize, const QPen &shapePen)
{
    setPen(shapePen);
    QPen specialPen(shapePen.color(), shapePen.width() + 1, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin);
    int xTrans{point3.y() - point1.y()};
    Point point2(point1.x() - xTrans, point3.y());
    Point point4(point3.x() + xTrans, point1.y());
    int height{qAbs(pointHSize.y() - point3.y())};

    Point pointA(point1), pointB(point2), pointC(point3), pointD(point4);
    calculateBasePlaneABCD(pointA, pointB, pointC, pointD);
    Point pointE((pointA.x() + pointC.x())/2, (pointA.y() + pointC.y())/2);
    Point pointF(pointE.x(), pointE.y() - height);

    Point midPointAB((pointA.x() + pointB.x())/2, (pointA.y() + pointB.y())/2);
    int disBetweenE_midAB(Point::distance(midPointAB, pointE));
    int disBetweenE_F(Point::distance(pointE, pointF));

    drawText(pointA + Point(-10, -10), "A");
    drawText(pointB + Point(-10, -10), "B");
    drawText(pointC + Point(10, 10), "C");
    drawText(pointD + Point(10, -10), "D");
    drawText(pointE + Point(-10, -10), "E");
    drawText(pointF + Point(-10, 0), "F");

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

void SDI_Painter::calculateBasePlaneABCD(Point &pointA, Point &pointB, Point &pointC, Point &pointD)
{
    QVector<Point> setPoint({pointA, pointB, pointC, pointD});
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
