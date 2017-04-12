#ifndef MATRIXLIBS_H
#define MATRIXLIBS_H

#include <QVector>
#include <QtMath>
#include <QPoint>
#include <iostream>
using std::cout;
using std::endl;
using std::fixed;
namespace matrix2DLibs
{
    template<typename T>
    QVector<QVector<T>> createTranslateMatrix(T xTrans, T yTrans)
    {
        return QVector<QVector<T>>{{1, 0, 0},
                                   {0, 1, 0},
                                   {xTrans, yTrans, 1}};
    }

    template<typename T>
    QVector<QVector<T>> createScaleMatrix(T Sx, T Sy)
    {
        return QVector<QVector<T>>{ { Sx, 0, 0 },
                                    { 0, Sy, 0 },
                                    { 0, 0, 1 }};
    }

    template<typename T>
    QVector<QVector<T>> createRotateMatrix(T Rx)
    {
        double a = Rx*M_PI / 180;
        return QVector<QVector<T>>{ { qCos(a),  qSin(a), 0 },
                                    { -qSin(a), qCos(a), 0 },
                                    { 0,            0, 1 }};
    }

    template<typename T>
    void ouputMatrix(QVector<QVector<T>>& matrix)
    {
        for (int i{}; i < 3; i++)
        {
            for (int j{}; j < 3; j++)
                cout << fixed << int(matrix[i][j]) << " ";
            cout << endl;
        }
    }

    QVector<QVector<int>> createCentralSymetryMatrix(const QPoint& centralPoint);

    QVector<QVector<int>> createReflectXMatrix();

    QVector<QVector<int>> createReflectYMatrix();

}
#endif // MATRIXLIBS_H
