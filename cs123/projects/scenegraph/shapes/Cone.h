#ifndef CONE_H
#define CONE_H

#include "Shapes.h"

class Cone : public Shapes
{
public:
    Cone();
    virtual ~Cone();

    virtual void renderGeometry(int param1, int param2, float param3);
    virtual void renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye);

    virtual bool isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal);
    virtual void getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture);
};


#endif // CONE_H
