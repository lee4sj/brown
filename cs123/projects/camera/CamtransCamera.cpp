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
    setClip(1, 30);
    setHeightAngle(60);
    setAspectRatio(1);
    Vector4 eye = Vector4(2, 2, 2, 1);
    Vector4 look = Vector4(-1, -1, -1, 0);
    Vector4 up = Vector4(0, 1, 0, 0);
    orientLook(eye, look, up);
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
}

Matrix4x4 CamtransCamera::getProjectionMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...

    //get unhinging transformation
    REAL c = -m_near/m_far;
    Matrix4x4 M1 = Matrix4x4(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, -1/(c+1), c/(c+1),
                             0, 0, -1, 0);


    //get M2
    REAL tHeight = tan(m_heightAngle * M_PI / 180 / 2);
    REAL tWidth = m_aspectRatio * tHeight;
    Matrix4x4 M2 = Matrix4x4(1/(m_far * tWidth), 0, 0, 0,
                             0, 1/(m_far * tHeight), 0, 0,
                             0, 0, 1/m_far, 0,
                             0, 0, 0, 1);

    return M1 * M2;
}

Matrix4x4 CamtransCamera::getModelviewMatrix() const
{
    // @TODO: [CAMTRANS] Fill this in...

    //get M3
    Matrix4x4 M3 = Matrix4x4(u.x, u.y, u.z, 0,
                             v.x, v.y, v.z, 0,
                             w.x, w.y, w.z, 0,
                             0, 0, 0, 1);

    Matrix4x4 M4 = Matrix4x4(1, 0, 0, -pos.x,
                             0, 1, 0, -pos.y,
                             0, 0, 1, -pos.z,
                             0, 0, 0, 1);

    return M3*M4;
}

Matrix4x4 CamtransCamera::getViewMatrix() const
{
    REAL tHeight = tan(m_heightAngle * M_PI / 180 / 2);
    REAL tWidth = m_aspectRatio * tHeight;
    Matrix4x4 M2 = Matrix4x4(1/(m_far * tWidth), 0, 0, 0,
                             0, 1/(m_far * tHeight), 0, 0,
                             0, 0, 1/m_far, 0,
                             0, 0, 0, 1);

    Matrix4x4 M3 = Matrix4x4(u.x, u.y, u.z, 0,
                             v.x, v.y, v.z, 0,
                             w.x, w.y, w.z, 0,
                             0, 0, 0, 1);

    Matrix4x4 M4 = Matrix4x4(1, 0, 0, -pos.x,
                             0, 1, 0, -pos.y,
                             0, 0, 1, -pos.z,
                             0, 0, 0, 1);

    return (M4.getInverse() * M3.getInverse() * M2.getInverse());
}

Vector4 CamtransCamera::getPosition() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return pos;
}

Vector4 CamtransCamera::getLook() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return -w;
}

Vector4 CamtransCamera::getUp() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return v;
}

float CamtransCamera::getAspectRatio() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const
{
    // @TODO: [CAMTRANS] Fill this in...
    return m_heightAngle;
}

void CamtransCamera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up)
{
    // @TODO: [CAMTRANS] Fill this in...
    pos = eye;

    w = -look.getNormalized();
    v = up - (up.dot(w) * w);
    v = v.getNormalized();
    u = v.cross(w);
}

void CamtransCamera::setHeightAngle(float h)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_heightAngle = h;
}

void CamtransCamera::translate(const Vector4 &v)
{
    // @TODO: [CAMTRANS] Fill this in...
    pos = getTransMat(v) * pos;
}

void CamtransCamera::rotateU(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    v = getRotMat(pos, -u, degrees * M_PI / 180) * v;
    w = getRotMat(pos, -u, degrees * M_PI / 180) * w;
}

void CamtransCamera::rotateV(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    u = getRotMat(pos, v, degrees * M_PI / 180) * u;
    w = getRotMat(pos, v, degrees * M_PI / 180) * w;
}

void CamtransCamera::rotateN(float degrees)
{
    // @TODO: [CAMTRANS] Fill this in...
    u = getRotMat(pos, w, degrees * M_PI / 180) * u;
    v = getRotMat(pos, w, degrees * M_PI / 180) * v;
}

void CamtransCamera::setClip(float nearPlane, float farPlane)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
}

