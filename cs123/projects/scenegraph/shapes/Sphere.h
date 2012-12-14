#ifndef SPHERE_H
#define SPHERE_H

#include "Shapes.h"

class Sphere : public Shapes
{
public:
    Sphere();
    virtual ~Sphere();

    virtual void renderGeometry(int param1, int param2, float param3);
    virtual void renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye);

    virtual bool isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal);
    virtual void getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture);

private:
    Vector3 getCoord(double r, double phi, double theta);
};

#endif // SPHERE_H
