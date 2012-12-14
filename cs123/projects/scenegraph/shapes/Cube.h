/*!
   @file   ShapesScene.h
   @author SeungJi Lee (lee4sj@gmail.com)
   @date   Fall 2010
*/

#ifndef CUBE_H
#define CUBE_H

#include "Shapes.h"

class Cube : public Shapes
{
public:
    Cube();
    virtual ~Cube();

    virtual void renderGeometry(int param1, int param2, float param3);
    virtual bool isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal);
    virtual void getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture);

};

#endif // CUBE_H
