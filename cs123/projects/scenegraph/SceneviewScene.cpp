#include <qgl.h>

#include "SceneviewScene.h"
#include "CS123SceneData.h"
#include "Camera.h"

#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"

SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...

    memset(shapes, 0, sizeof(Shapes *) * NUM_SHAPE_TYPES);

    shapes[SHAPE_CUBE] = new Cube();
    shapes[SHAPE_CONE] = new Cone();
    shapes[SHAPE_SPHERE] = new Sphere();
    shapes[SHAPE_CYLINDER] = new Cylinder();
}

SceneviewScene::~SceneviewScene()
{
    // TODO: [SCENEVIEW] Don't leak memory!
    for (int i = 0; i < NUM_SHAPE_TYPES; i++) {
        if (shapes[i])
            delete shapes[i];
    }
}

void SceneviewScene::setLights(const Camera *follow)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Use OpenGL to set up the lighting for your scene. The lighting information
    // will most likely be stored in CS123SceneLightData structures.
    //

    for (int i = 0; i < m_lights->size(); i++) {
        CS123SceneLightData light = m_lights->at(i);
        setLight(light);
    }
}

void SceneviewScene::renderGeometry(bool useMaterials)
{
    // TODO: [SCENEVIEW] Fill this in...

    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    for (int i = 0; i < sceneList->size(); i++) {
        SceneListNode shape = sceneList->at(i);
        CS123ScenePrimitive prim = shape.primitive;
        Matrix4x4 mat = shape.mat;

        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_NORMALIZE);
        glPushMatrix();

        if (useMaterials) {
            CS123SceneMaterial m = prim.material;
            m.cAmbient.r *= m_global.ka;
            m.cAmbient.g *= m_global.ka;
            m.cAmbient.b *= m_global.ka;
            m.cDiffuse.r *= m_global.kd;
            m.cDiffuse.g *= m_global.kd;
            m.cDiffuse.b *= m_global.kd;

            OpenGLScene::applyMaterial(m);
        }

        switch(prim.type) {
        case PRIMITIVE_CUBE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CUBE]->renderGeometry(10, 10, 10);
            break;
        case PRIMITIVE_CONE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CONE]->renderGeometry(30, 30, 10);
            break;
        case PRIMITIVE_CYLINDER:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CYLINDER]->renderGeometry(30, 30, 10);
            break;
        case PRIMITIVE_TORUS:
            break;
        case PRIMITIVE_SPHERE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_SPHERE]->renderGeometry(30, 30, 10);
            break;
        case PRIMITIVE_MESH:

            break;
        }
        glPopMatrix();
    }
}

void SceneviewScene::renderNormals()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Rendering normals is an **optional** part of Sceneview (sorry, not extra credit)
    // You can put all of your renderNormal() calls here if you want to render normals.
    //
    // HINT: Rendering normals can be a helpful debugging tool if things don't look
    // quite right. Your Shapes might be incorrect, and if that's the case, you'll
    // need to go back and fix your buggy shapes code before turning in Sceneview.
    //

    for (int i = 0; i < sceneList->size(); i++) {
        SceneListNode shape = sceneList->at(i);
        CS123ScenePrimitive prim = shape.primitive;
        Matrix4x4 mat = shape.mat;

        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_NORMALIZE);
        glPushMatrix();

        switch(prim.type) {
        case PRIMITIVE_CUBE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CUBE]->renderNormals(30, 30, 10, m_cameraEye);
            break;
        case PRIMITIVE_CONE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CONE]->renderNormals(30, 30, 10, m_cameraEye);
            break;
        case PRIMITIVE_CYLINDER:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_CYLINDER]->renderNormals(30, 30, 10, m_cameraEye);
            break;
        case PRIMITIVE_TORUS:
            break;
        case PRIMITIVE_SPHERE:
            glMultMatrixd(mat.getTranspose().data);
            shapes[SHAPE_SPHERE]->renderNormals(30, 30, 10, m_cameraEye);
            break;
        case PRIMITIVE_MESH:

            break;
        }
        glPopMatrix();
    }
}


void SceneviewScene::setSelection(int x, int y)
{
    // TODO: [MODELER LAB] Fill this in...
    //
    // Using m_selectionRecorder, set m_selectionIndex to the index in your
    // flattened parse tree of the object under the mouse pointer.  The
    // selection recorder will pick the object under the mouse pointer with
    // some help from you, all you have to do is:

    // 1) Set this to the number of objects you will be drawing.
    int numObjects = 0;

    // 2) Start the selection process
    m_selectionRecorder.enterSelectionMode(x, y, numObjects);

    // 3) Draw your objects, calling m_selectionRecorder.setObjectIndex() before drawing each one.

    // 4) Find out which object you selected, if any (-1 means no selection).
    m_selectionIndex = m_selectionRecorder.exitSelectionMode();
}
