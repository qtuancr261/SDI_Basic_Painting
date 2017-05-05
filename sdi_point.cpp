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

SDI_Point SDI_Point::translate(int xtrans, int ytrans) const
{
    QVector<QVector<int>> transMatrix; // 2D translate matrix
    transMatrix = matrix2DLibs::createTranslateMatrix(xtrans, ytrans);
    QVector<int> currentPoint{x(), y(), h};
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{0}; j < 3; j++)
            newPoint[i] += currentPoint[j]*transMatrix[j][i];
    }
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}

SDI_Point SDI_Point::scale(int Sx, int Sy) const
{
    QVector<QVector<int>> ScaMatrix; // 2D scale matrix
    ScaMatrix = matrix2DLibs::createScaleMatrix(Sx, Sy);
    QVector<int> currentPoint{ x(), y(), h };
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{ 0 }; j < 3; j++)
            newPoint[i] += currentPoint[j] * ScaMatrix[j][i];
    }
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}

SDI_Point SDI_Point::rotate(double radian) const
{
    QVector<QVector<double>> RotMatrix; // 2D rotate matrix
    RotMatrix = matrix2DLibs::createRotateMatrix(radian);
    //matrix2DLibs::ouputMatrix(RotMatrix);
    QVector<double> currentPoint{ static_cast<double>(x()), static_cast<double>(y()), static_cast<double>(h) };
    QVector<double> newPoint(3, 0.0);
    for (int i{}; i < 3; i++)
    {
        for (int j{}; j < 3; j++)
        {
          //cout << i << " Loop " <<newPoint[i] << " " << currentPoint[j] << " " << RotMatrix[j][i];
          newPoint[i] += (currentPoint[j] *RotMatrix[j][i]);
          //cout << "-> " << newPoint[i] << endl;
        }
    }
    cout << "New " << newPoint[0] << newPoint[1] << newPoint[2] << endl;
    return SDI_Point(float(newPoint[0]), float(newPoint[1]), newPoint[2]);
}

SDI_Point SDI_Point::centralSymmetry(const QPoint &centralPoint) const
{
    QVector<QVector<int>> centralSymMatrix; // 2D reflect matrix
    centralSymMatrix = matrix2DLibs::createCentralSymetryMatrix(centralPoint);
    QVector<int> currentPoint{ x(), y(), h };
    QVector<int> newPoint(3, 0);
    for (int i{}; i < 3; i++)
    {
        for (int j{ 0 }; j < 3; j++)
            newPoint[i] += currentPoint[j] * centralSymMatrix[j][i];
    }
    return SDI_Point(newPoint[0], newPoint[1], newPoint[2]);
}

double SDI_Point::distance(const QPoint &p1, const QPoint &p2) // static function
{
    return qSqrt(qPow(p1.x() - p2.x(), 2.0) + qPow(p1.y() - p2.y(), 2.0));
}

SDI_Point SDI_Point::convertToUserSystem(const SDI_Point &cvtPoint, const SDI_Point &Origin)
{
    return SDI_Point(cvtPoint.x() - Origin.x(), Origin.y() - cvtPoint.y());
}
