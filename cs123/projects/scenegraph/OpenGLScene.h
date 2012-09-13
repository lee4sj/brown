/*!
   @file   OpenGLScene.h
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010
*/

#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include "Scene.h"

// OpenGL has 8 lights, GL_LIGHT0 to GL_LIGHT7
#define NUM_OPENGL_LIGHTS 8

class SupportCanvas3D;

/*!
  @class  OpenGLScene
  @author Evan Wallace

  @brief  Basic Scene implementation that supports OpenGL. Students will extend this class
          in ShapesScene and SceneviewScene.

  @see    ShapesScene
  @see    SceneviewScene
**/
class OpenGLScene : public Scene
{
public:
    OpenGLScene();
    virtual ~OpenGLScene();

    void render(SupportCanvas3D *context);

protected:
    /*! Apply the camera matrices and enable or disable lights as necessary.  These two steps cannot be split up
     because for Shapes the light moves with the camera while for Sceneview the camera moves with the scene. */
    virtual void setLights(const Camera *follow) = 0;

    /*! Render geometry for Shapes and Sceneview, don't apply materials when useMaterials is false because we
     are drawing the outlines and want them colored black. */
    virtual void renderGeometry(bool useMaterials) = 0;

    //! Override this and put all renderNormal() calls in here (rendering normals are optional)
    virtual void renderNormals() {}

    /*! Draw a normal from vertex along direction.  Vertex must be a point (vertex[3] == 1) and direction must be a
     vector (direction[3] == 0).  Do not call renderNormal() between glBegin() and glEnd(), since renderNormal()
     contains calls to glBegin() and glEnd() inside it. */
    void renderNormal(const Vector4 &vertex, const Vector4 &direction);

    //! Perform the necessary glMaterial*() calls to switch materials.
    void applyMaterial(const CS123SceneMaterial &material);

    //! Enable the light and perform the necessary glLight*() calls to switch light properties.
    void setLight(const CS123SceneLightData &light);

private:
    Vector4 m_cameraEye;

};

#endif // OPENGLSCENE_H
