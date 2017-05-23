#ifndef SDI_NAMESPACE_H
#define SDI_NAMESPACE_H
enum class GraphicsMode
{
    GM_NoMode= 0,
    GM_2D = 2,
    GM_3D = 3
};

enum class ClearImageMode
{
    CIM_All = 0,
    CIM_ForNewSession
};

enum class DrawLineDelegateMode
{
    DLDM_None = 0,
    DLDM_Triangle,
    DLDM_Parrallelogram
};

enum class GeometricShape
{
    GS_SelectShape = 0,
    GS_Point,
    GS_Line,
    GS_Rect,
    GS_Square,
    GS_Parallelogram,
    GS_Circle,
    GS_Triangle
};

enum class Geometric3DShape
{
    G3DS_SelectShape = 0,
    G3DS_Parallelepiped,
    G3DS_Pyramid
};

enum class DisplayCoordinateState
{
    DCS_Hide = 0,
    DCS_Show
};

enum class StateOfShape
{
    SOS_NewTempShape,
    SOS_NewPermanentShape,
    SOS_AllExistentShapes
};












#endif // SDI_NAMESPACE_H
