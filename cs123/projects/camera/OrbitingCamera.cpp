/*!

   @file   OrbitingCamera.cpp
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     See the header file, really. You don't need to be poking around
   in this file unless you're interested in how an orbiting camera
   works.

   The way we have implemented this class is NOT how you should be
   implementing your Camtrans. This camera is a DIFFERENT TYPE of
   camera which we're providing so you can easily view your Shapes
   and to make sure your scene graph is working if your camera isn't.

   In the Camtrans assignment, you'll be implementing your own
   perspective camera from scratch! This one uses OpenGL.

*/

#include "OrbitingCamera.h"
#include "Settings.h"

#include <CS123Algebra.h>
#include <math.h>
#include <qgl.h>

OrbitingCamera::OrbitingCamera()
{
    m_aspectRatio = 1;
    m_angleX = m_angleY = 0;
    m_zoomZ = -5;
}

void OrbitingCamera::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;

    updateProjectionMatrix();
}

Matrix4x4 OrbitingCamera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

Matrix4x4 OrbitingCamera::getModelviewMatrix() const
{
    return m_modelviewMatrix;
}

Matrix4x4 OrbitingCamera::getViewMatrix() const
{
    return Matrix4x4::identity();
}

Vector4 OrbitingCamera::getPosition() const
{
    return Vector4(0, 0, 0, 0);
}

void OrbitingCamera::mouseDown(int x, int y)
{
    m_oldX = x;
    m_oldY = y;
}

void OrbitingCamera::mouseDragged(int x, int y)
{
    m_angleY += x - m_oldX;
    m_angleX += y - m_oldY;
    m_oldX = x;
    m_oldY = y;
    if (m_angleX < -90) m_angleX = -90;
    if (m_angleX > 90) m_angleX = 90;

    updateModelviewMatrix();
}

void OrbitingCamera::mouseScrolled(int delta)
{
    // Use an exponential factor so the zoom increments are small when we are
    // close to the object and large when we are far away from the object
    m_zoomZ *= powf(0.999f, delta);

    updateModelviewMatrix();
}

void OrbitingCamera::updateMatrices()
{
    updateProjectionMatrix();
    updateModelviewMatrix();
}

void OrbitingCamera::updateProjectionMatrix()
{
    double matrix[16];
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(settings.cameraFov, m_aspectRatio, settings.cameraNear, settings.cameraFar);
    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    m_projectionMatrix = Matrix4x4(matrix).getTranspose();
}

void OrbitingCamera::updateModelviewMatrix()
{
    double matrix[16];
    glPushMatrix();
    glLoadIdentity();

    // Move the object forward by m_zoomZ units before we rotate, so it will rotate about a point in front of us
    glTranslatef(0, 0, m_zoomZ);

    // Now rotate the object, pivoting it about the new origin in front of us
    glRotatef(m_angleX, 1, 0, 0);
    glRotatef(m_angleY, 0, 1, 0);

    glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
    glPopMatrix();
    m_modelviewMatrix = Matrix4x4(matrix).getTranspose();
}
