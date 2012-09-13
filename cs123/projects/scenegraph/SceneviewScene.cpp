#include <qgl.h>

#include "SceneviewScene.h"
#include "CS123SceneData.h"
#include "Camera.h"


SceneviewScene::SceneviewScene()
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...

}

SceneviewScene::~SceneviewScene()
{
    // TODO: [SCENEVIEW] Don't leak memory!

}

void SceneviewScene::setLights(const Camera *follow)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Use OpenGL to set up the lighting for your scene. The lighting information
    // will most likely be stored in CS123SceneLightData structures.
    //
}

void SceneviewScene::renderGeometry(bool useMaterials)
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //
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
