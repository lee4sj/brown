#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include "vector.h"

/**
    An orbiting perspective camera specified by a center, two angles, and a zoom factor

    @author: Justin Ardini (jardini)
**/
struct OrbitCamera
{
    Vector3 center, up;
    float theta, phi;
    float fovy;
    float zoom;

    void mouseMove(const Vector2 &delta);
    void mouseWheel(float delta);
};

#endif // CAMERA_H
