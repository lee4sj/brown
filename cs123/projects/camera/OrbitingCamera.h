/*!
   @file   OrbitingCamera.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Contains definitions for an orbiting camera class based on OpenGL.
*/

#ifndef ORBITINGCAMERA_H
#define ORBITINGCAMERA_H

#include "Camera.h"


/**
 @class OrbitingCamera

 @brief A simple orbiting camera

 You shouldn't need to work on this class. It's there for your convenience, really,
 and the way this camera is implemented is NOT the way you should be implementing
 your camera in the assignments. We hide the real implementation by using OpenGL to
 perform the camera calculations.

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/1/2010

*/
class OrbitingCamera : public Camera
{
public:
    OrbitingCamera();

    virtual void setAspectRatio(float aspectRatio);

    virtual Matrix4x4 getProjectionMatrix() const;
    virtual Matrix4x4 getModelviewMatrix() const;
    virtual Matrix4x4 getViewMatrix() const;

    virtual Vector4 getPosition() const;

    virtual void mouseDown(int x, int y);
    virtual void mouseDragged(int x, int y);
    virtual void mouseScrolled(int delta);

    void updateMatrices();

private:
    void updateProjectionMatrix();
    void updateModelviewMatrix();

    Matrix4x4 m_modelviewMatrix;
    Matrix4x4 m_projectionMatrix;
    float m_aspectRatio, m_angleX, m_angleY, m_zoomZ;
    int m_oldX, m_oldY;
};

#endif // ORBITINGCAMERA_H
