#include "Canvas3D.h"
#include <Settings.h>
#include <typeinfo>
#include <Camera.h>
#include <OrbitingCamera.h>
#include <OpenGLScene.h>

Canvas3D::Canvas3D(QWidget *parent) : SupportCanvas3D(parent)
{


}

Canvas3D::~Canvas3D()
{


}


void Canvas3D::initializeGL()
{
    // Track the camera settings so we can generate deltas
    m_oldPosX = settings.cameraPosX;
    m_oldPosY = settings.cameraPosY;
    m_oldPosZ = settings.cameraPosZ;
    m_oldRotU = settings.cameraRotU;
    m_oldRotV = settings.cameraRotV;
    m_oldRotN = settings.cameraRotN;

    // Use a depth buffer and only draw when objects have a depth less than or
    // equal to the depth present in the buffer.  This will make sure the final
    // buffer will only contain the closest object at each pixel (the depth
    // buffer starts off filled with the furthest possible distance).
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Move the polygons back a bit so lines are still drawn even though they are
    // coplanar with the polygons they came from, which will be drawn before them
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);

    // Specular reflections will be off without this, since OpenGL calculates
    // specular highlights using an infinitely far away camera by default, not
    // the actual location of the camera
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

    // TODO: might want to do this for textured scenes
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    // Hide backfaces so you can tell when triangles are backwards (counter-clockwise
    // triangles will be visible)
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    // Show all ambient light for the entire scene (not one by default)
    static float one[] = { 1, 1, 1, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

    // Calculate the orbiting camera matrices now that the OpenGL context is created.
    getOrbitingCamera()->updateMatrices();
}


void Canvas3D::paintGL()
{
    OpenGLScene *glScene = this->getScene();

    if (glScene != NULL)
    {

        // Check for errors from the last frame
        int err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            fprintf(stderr, "GL is in an error state before painting.\n");
            fprintf(stderr, "(GL error code %d)\n", err);
            //assert(0);
        }

        // Update the scene camera
        glViewport(0, 0, width(), height());
        getCamera()->setAspectRatio((float)width() / (float)height());

        glScene->render(this);

    }
    else
    {

        // Display a placeholder message explaining that no OpenGLScene is
        // currently loaded.

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1, 1, 1);
        renderText(width() / 2 - 80, height() / 2 + 5, "No OpenGLScene loaded");

    }
}


void Canvas3D::settingsChanged() {

    // TODO: Process changes to the application settings.


    // Call superclass (this repaints the scene for you)
    SupportCanvas3D::settingsChanged();
}
