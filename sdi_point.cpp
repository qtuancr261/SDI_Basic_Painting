#include "sdi_point.h"

SDI_Point::SDI_Point() : QPoint()
{
    // default constructor
}

SDI_Point::SDI_Point(int xpos, int ypos, int hValue) : QPoint(xpos, ypos), h(hValue)
{

}

SDI_Point::SDI_Point(const QPoint &srcPoint) : SDI_Point(srcPoint.x(), srcPoint.y())
{

}

SDI_Point &SDI_Point::operator =(const SDI_Point &srcPoint)
{
    rx() = srcPoint.x();
    ry() = srcPoint.y();
    h = srcPoint.h;
    return *this;
}

void SDI_Point::translate(int xtrans, int ytrans)
{
    rx() += xtrans;
    ry() -= ytrans;
}

void SDI_Point::scale(double Sx, double Sy, const QPoint &centralPoint)
{
    int xtrans{x() - centralPoint.x()};
    int ytrans{y() - centralPoint.y()};
    rx() = centralPoint.x() + Sx*xtrans;
    ry() = centralPoint.y() + Sy*ytrans;
}

void SDI_Point::rotate(double degree, int xtrans, int ytrans)
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

void SDI_Point::centralSymmetry(const QPoint &centralPoint)
{
    rx() = centralPoint.x()*2 - x();
    ry() = centralPoint.y()*2 - y();
}

void SDI_Point::OxSymmetry(int yValue)
{
    centralSymmetry(SDI_Point(x(), yValue));
}

void SDI_Point::OySymmetry(int xValue)
{
    centralSymmetry(SDI_Point(xValue, y()));
}

bool SDI_Point::operator <(const SDI_Point &srcPoint) const
{
    return this->x() < srcPoint.x();
}

double SDI_Point::distance(const SDI_Point &p1, const SDI_Point &p2) // static function
{
    return qSqrt(qPow(p1.x() - p2.x(), 2.0) + qPow(p1.y() - p2.y(), 2.0));
}

double SDI_Point::distanceFromPointToLine(const SDI_Point &point, const SDI_Point &lineP1, const SDI_Point &lineP2)
{
    // aFact*x + bFact*y + c = 0
    int aFactor{lineP2.y() - lineP1.y()};
    int bFactor{-(lineP2.x() - lineP1.x())};
    int cFactor{lineP2.x()*lineP1.y() - lineP1.x()*lineP2.y()};
    //qDebug() << qAbs(aFactor*point.x() + bFactor*point.y() + cFactor)/qSqrt(qPow(aFactor, 2.0) + qPow(bFactor, 2.0));
    return qAbs(aFactor*point.x() + bFactor*point.y() + cFactor)/qSqrt(qPow(aFactor, 2.0) + qPow(bFactor, 2.0));
}

SDI_Point SDI_Point::convertToUserSystem(const SDI_Point &cvtPoint, const SDI_Point &Origin)
{
    return SDI_Point(cvtPoint.x() - Origin.x(), Origin.y() - cvtPoint.y());
}

SDI_Point SDI_Point::translate(const SDI_Point &cvtPoint, int xtrans, int ytrans)
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
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}

SDI_Point SDI_Point::scale(const SDI_Point &cvtPoint, int Sx, int Sy)
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
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}

SDI_Point SDI_Point::rotate(const SDI_Point &cvtPoint, double radian)
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
    return SDI_Point(float(newPoint[0]), float(newPoint[1]), newPoint[2]);
}

SDI_Point SDI_Point::centralSymmetry(const SDI_Point &cvtPoint, const SDI_Point &centralPoint)
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
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}
