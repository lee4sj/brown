#ifndef SHAPES_H
#define SHAPES_H
#include "CS123Algebra.h"

#include <QImage>
#include "CS123SceneData.h"

class Shapes
{
public:
    Shapes() {}
    virtual ~Shapes() {}
    int shapeType;

    /*! render the geometry (i.e. tesslate the right shape */
    virtual void renderGeometry(int param1, int param2, float param3) {}

    /*! Override this and put all renderNormal() calls in here (rendering normals are optional) */
    virtual void renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye) {}
    virtual void renderNormal(const Vector4 &vertex, const Vector4 &direction);

    /* Intersect Project */
    virtual bool isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal);
    virtual void getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture) {}

    Vector4 m_cameraEye;
};


#endif // SHAPES_H
