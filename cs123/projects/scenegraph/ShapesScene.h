/*!
   @file   ShapesScene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef SHAPESSCENE_H
#define SHAPESSCENE_H

#include "OpenGLScene.h"


/**

 @class ShapesScene

 @brief A scene that is used to render a single shape.

 This scene has no notion of the scene graph, so it will not be useful to you in
 assignments requiring the display of multiple shapes. Luckily, the first time you
 will need that kind of functionality is in the Sceneview assignment... and we've
 left that task up to you in the SceneviewScene class.

 By default, the ShapesScene displays only a single triangle. You'll need to do
 a little work here to render your shapes. You could render the shapes directly
 in this class, or you could pass the work on to one or more subclasses. Think
 carefully about your design here - you'll be reusing your shapes multiple times
 during this course!

 @author  Evan Wallace (edwallac)
 @author  Ben Herila (bherila)
 @date    9/1/2010

**/
class ShapesScene : public OpenGLScene
{
public:
    ShapesScene();
    virtual ~ShapesScene();

protected:

    /*! Apply the camera matrices and enable or disable lights as necessary.
        These two steps cannot be split up because for Shapes the light moves
        with the camera. In Sceneview, the camera moves with the scene. */
    virtual void setLights(const Camera *follow);

    /*! Render geometry for Shapes and Sceneview. We don't apply materials
        when useMaterials is false because we are drawing the outlines and
        want them colored black. */
    virtual void renderGeometry(bool useMaterials);

    /*! Override this and put all renderNormal() calls in here (rendering normals are optional) */
    virtual void renderNormals();

private:
    // Storage for private copies of the scene's light and material data
    // Note that these don't need to be freed because they are VALUE types (not pointers)
    // and the memory for them is freed when the class itself is freed.

    CS123SceneLightData  m_light;
    CS123SceneMaterial   m_material;

};

#endif // SHAPESSCENE_H
