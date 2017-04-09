#include "sdi_point.h"

SDI_Point::SDI_Point() : QPoint()
{
    // default constructor
}

SDI_Point::SDI_Point(int xpos, int ypos, int hValue) : QPoint(xpos, ypos), h(hValue)
{

}

SDI_Point SDI_Point::translate(int xtrans, int ytrans)
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
