/*!
   @file   Camera.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Contains definitions for an abstract virtual camera class.
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <CS123Algebra.h>

/**

 @class Camera

 @brief An abstract class representing a virtual camera and an optional mouse interaction.

 Part of the CS123 support code.

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/1/2010

*/
class Camera
{
public:
    // Will be called when the window size changes
    virtual void setAspectRatio(float) = 0;

    // C++ Note: The following are pure virtual functions - they must be overridden in subclasses.

    //! Return the projection matrix for the current camera settings.
    virtual Matrix4x4 getProjectionMatrix() const = 0;

    //! Return the modelview matrix for the current camera settings.
    virtual Matrix4x4 getModelviewMatrix() const = 0;

    //
    // The following functions will be called for mouse events on the 3D canvas tab.
    //
    // C++ Note: Why are the following methods not pure virtual? If they are not overridden in
    // subclasses, then the empty implementation here ({}) will take over.
    //

    //! Called when the mouse button is pressed down.
    virtual void mouseDown(int x, int y)    {}

    //! Called when the mouse button is dragged.
    virtual void mouseDragged(int x, int y) {}

    //! Called when the mouse button is released.
    virtual void mouseUp(int x, int y)      {}

    //! Called when the scroll wheel changes position.
    virtual void mouseScrolled(int delta)   {}


};

#endif // CAMERA_H
