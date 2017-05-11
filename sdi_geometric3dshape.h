#ifndef SDI_GEOMETRIC3DSHAPE_H
#define SDI_GEOMETRIC3DSHAPE_H
#include "sdi_point.h"
#include "sdi_painter.h"
#include <QVector>
enum class geometric3DShape
{
    selectShape = 0, parallelepiped, pyramid
};

class SDI_Geometric3DShape
{
public:
    SDI_Geometric3DShape();
};

#endif // SDI_GEOMETRIC3DSHAPE_H
