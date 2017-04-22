#include "sdi_point3d.h"

SDI_Point3D::SDI_Point3D() : QPoint()
{

}

SDI_Point3D::SDI_Point3D(int xpos, int ypos, int zpos, int hValue)
    : QPoint(xpos, ypos), z(zpos), h(hValue)
{

}

SDI_Point3D SDI_Point3D::translate(int xtrans, int ytrans, int ztrans) const
{

}

SDI_Point3D SDI_Point3D::centralSymmetry(const SDI_Point3D &centralPoint) const
{

}

