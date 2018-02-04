#include "point.h"

Point::Point() : QPoint()
{
    // default constructor
}

Point::Point(int xpos, int ypos, int hValue) : QPoint(xpos, ypos), h(hValue)
{

}

Point::Point(const QPoint &srcPoint) : Point(srcPoint.x(), srcPoint.y())
{

}

Point &Point::operator =(const Point &srcPoint)
{
    rx() = srcPoint.x();
    ry() = srcPoint.y();
    h = srcPoint.h;
    return *this;
}

void Point::translate(int xtrans, int ytrans)
{
    rx() += xtrans;
    ry() -= ytrans;
}

void Point::scale(double Sx, double Sy, const QPoint &centralPoint)
{
    int xtrans{x() - centralPoint.x()};
    int ytrans{y() - centralPoint.y()};
    rx() = centralPoint.x() + Sx*xtrans;
    ry() = centralPoint.y() + Sy*ytrans;
}

void Point::rotate(double degree, int xtrans, int ytrans)
{
    if (degree == 0.0 || degree == 180.0)
        return;
    else
    {
        rx() = x() + xtrans;
        ry() = y() + ytrans;
        double a{x()*qCos(qDegreesToRadians(degree)) - y()*qSin(qDegreesToRadians(degree))};
        double b{x()*qSin(qDegreesToRadians(degree)) + y()*qCos(qDegreesToRadians(degree))};
        rx() = QPointF(a,b).toPoint().x() - xtrans;
        ry() = QPointF(a,b).toPoint().y() - ytrans;
    }
}

void Point::centralSymmetry(const QPoint &centralPoint)
{
    rx() = centralPoint.x()*2 - x();
    ry() = centralPoint.y()*2 - y();
}

void Point::OxSymmetry(int yValue)
{
    centralSymmetry(Point(x(), yValue));
}

void Point::OySymmetry(int xValue)
{
    centralSymmetry(Point(xValue, y()));
}

bool Point::operator <(const Point &srcPoint) const
{
    return this->x() < srcPoint.x();
}

double Point::distance(const Point &p1, const Point &p2) // static function
{
    return qSqrt(qPow(p1.x() - p2.x(), 2.0) + qPow(p1.y() - p2.y(), 2.0));
}

double Point::distanceFromPointToLine(const Point &point, const Point &lineP1, const Point &lineP2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{lineP2.y() - lineP1.y()};
    int bFactor{-(lineP2.x() - lineP1.x())};
    int cFactor{lineP2.x()*lineP1.y() - lineP1.x()*lineP2.y()};
    //qDebug() << qAbs(aFactor*point.x() + bFactor*point.y() + cFactor)/qSqrt(qPow(aFactor, 2.0) + qPow(bFactor, 2.0));
    return qAbs(aFactor*point.x() + bFactor*point.y() + cFactor)/qSqrt(qPow(aFactor, 2.0) + qPow(bFactor, 2.0));
}

Point Point::convertToUserSystem(const Point &cvtPoint, const Point &Origin)
{
    return Point(cvtPoint.x() - Origin.x(), Origin.y() - cvtPoint.y());
}

Point Point::translate(const Point &cvtPoint, int xtrans, int ytrans)
{
    QVector<QVector<int>> transMatrix; // 2D scale matrix
    transMatrix = matrix2DLibs::createTranslateMatrix(xtrans, ytrans);
    QVector<int> currentPoint{ cvtPoint.x(), cvtPoint.y(), cvtPoint.h };
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{ 0 }; j < 3; j++)
            newPoint[i] += currentPoint[j] * transMatrix[j][i];
    }
    return Point(newPoint[0], newPoint[1], newPoint[2]);
}

Point Point::scale(const Point &cvtPoint, int Sx, int Sy)
{
    QVector<QVector<int>> scaMatrix; // 2D scale matrix
    scaMatrix = matrix2DLibs::createScaleMatrix(Sx, Sy);
    QVector<int> currentPoint{ cvtPoint.x(), cvtPoint.y(), cvtPoint.h };
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{ 0 }; j < 3; j++)
            newPoint[i] += currentPoint[j] * scaMatrix[j][i];
    }
    return Point(newPoint[0], newPoint[1], newPoint[2]);
}

Point Point::rotate(const Point &cvtPoint, double radian)
{
    QVector<QVector<double>> RotMatrix; // 2D rotate matrix
    RotMatrix = matrix2DLibs::createRotateMatrix(radian);
    QVector<double> currentPoint{ static_cast<double>(cvtPoint.x()), static_cast<double>(cvtPoint.y()), static_cast<double>(cvtPoint.h) };
    QVector<double> newPoint(3, 0.0);
    for (int i{}; i < 3; i++)
    {
        for (int j{}; j < 3; j++)
        {
          newPoint[i] += (currentPoint[j] *RotMatrix[j][i]);
        }
    }
    return Point(float(newPoint[0]), float(newPoint[1]), newPoint[2]);
}

Point Point::centralSymmetry(const Point &cvtPoint, const Point &centralPoint)
{
    QVector<QVector<int>> centralSymMatrix; // 2D reflect matrix
    centralSymMatrix = matrix2DLibs::createCentralSymetryMatrix(centralPoint);
    QVector<int> currentPoint{ cvtPoint.x(), cvtPoint.y(), cvtPoint.h };
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{ 0 }; j < 3; j++)
            newPoint[i] += currentPoint[j] * centralSymMatrix[j][i];
    }
    return Point(newPoint[0], newPoint[1], newPoint[2]);
}
