#include "matrixlibs.h"

QVector<QVector<int> > matrix2DLibs::createReflectXMatrix()
{
    return QVector<QVector<int>>{ { -1, 0, 0 },
                                  { 0,  1, 0 },
                                  { 0,  0, 1 }};
}

QVector<QVector<int> > matrix2DLibs::createReflectYMatrix()
{
    return QVector<QVector<int>>{ { 1,  0, 0 },
                                  { 0, -1, 0 },
                                  { 0,  0, 1 }};
}

QVector<QVector<int> > matrix2DLibs::createCentralSymetryMatrix(const QPoint &centralPoint)
{
    return QVector<QVector<int>>{ { -1, 0, 0 },
                                  { 0, -1, 0 },
                                  { 2*centralPoint.x(), 2*centralPoint.y(), 1 }};
}
