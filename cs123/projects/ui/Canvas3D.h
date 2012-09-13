/*!
   @file   Canvas3D.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef CANVAS3D_H
#define CANVAS3D_H

#include "SupportCanvas3D.h"

/*!
   @class Canvas3D

   @brief 3D canvas that uses OpenGL. Students will implement this canvas in
   the OpenGL labs and in the Shapes, Camtrans, and Sceneview assignments.

**/
class Canvas3D : public SupportCanvas3D
{
    Q_OBJECT
public:
    Canvas3D(QWidget *parent);
    virtual ~Canvas3D();

    virtual void initializeGL();

    virtual void paintGL();

    //! This will be called when the settings have changed
    virtual void settingsChanged();

};

#endif // CANVAS3D_H
