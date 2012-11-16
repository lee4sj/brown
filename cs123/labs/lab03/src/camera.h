#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

/**
    A basic camera
*/
struct LabCamera {
    Vector3 eye, center, up;
    float fovy, near, far;
};

#endif // CAMERA_H
