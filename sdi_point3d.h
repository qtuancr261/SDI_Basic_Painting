// object
#ifndef SDI_POINT3D_H
#define SDI_POINT3D_H
#include <QPoint>
#include <QString>
#include "matrixlibs.h"
class SDI_Point3D : public QPoint
{
private:
    int z;
    int h;
    QString label;
public:
    SDI_Point3D(); //default constructor - no parameters
    SDI_Point3D(int xpos, int ypos, int zpos, int hValue = 1);
    //SDI_Point3D(const QPoint& srcPoint); // copy constructor
     ~SDI_Point3D() = default; // default destructor
    // Transforming function
    SDI_Point3D translate(int xtrans, int ytrans, int ztrans) const;
    SDI_Point3D centralSymmetry(const SDI_Point3D& centralPoint) const;

};

#endif // SDT_POINT3D_H
