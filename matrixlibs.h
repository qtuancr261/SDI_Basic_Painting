#ifndef MATRIXLIBS_H
#define MATRIXLIBS_H

#endif // MATRIXLIBS_H
#include <QVector>

namespace matrix2DLibs
{
    template<typename T>
    QVector<QVector<T>> createTranslateMatrix(T xTrans, T yTrans)
    {
        return QVector<QVector<T>>{{1, 0, 0},
                                   {0, 1, 0},
                                   {xTrans, yTrans, 1}};
    }

}
