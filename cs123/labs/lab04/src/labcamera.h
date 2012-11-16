#ifndef LABCAMERA_H
#define LABCAMERA_H

#include "vector.h"

struct LabCamera
{
    Vector3 eye, center, up;
    float angle, near, far;
};

#endif // LABCAMERA_H
