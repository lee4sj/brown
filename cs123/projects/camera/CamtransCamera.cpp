/*!
   @file   CamtransCamera.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  This is the perspective camera class you will need to fill in for the
   Camtrans assignment.  See the assignment handout for more details.

   For extra credit, you can also create an Orthographic camera. Create another
   class if you want to do that.

*/

#include "CamtransCamera.h"
#include <Settings.h>
#include <qgl.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...

}

Matrix4x4 CamtransCamera::getProjectionMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();
}

Matrix4x4 CamtransCamera::getModelviewMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Matrix4x4::identity();
}

Vector4 CamtransCamera::getPosition() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

Vector4 CamtransCamera::getLook() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

Vector4 CamtransCamera::getUp() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return Vector4::zero();
}

float CamtransCamera::getAspectRatio() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return 0;
}

float CamtransCamera::getHeightAngle() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return 0;
}

void CamtransCamera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::setHeightAngle(float h)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::translate(const Vector4 &v)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateU(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateV(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::rotateN(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...

}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    // @TODO: [CAMTRANS] Fill this in...

}

