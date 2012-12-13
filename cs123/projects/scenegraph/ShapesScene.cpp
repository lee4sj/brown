#include "ShapesScene.h"
#include "Settings.h"
#include "Camera.h"
#include <qgl.h>
#include <SupportCanvas3D.h>

#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"

#include "final/FractalTree.h"

Vector4 lightDirection = Vector4(1, -1, -1, 0).getNormalized();

ShapesScene::ShapesScene()
{
    // Use a shiny orange material
    memset(&m_material, 0, sizeof(m_material));
    m_material.cAmbient.r = 0.2f;
    m_material.cAmbient.g = 0.1f;
    m_material.cDiffuse.r = 1;
    m_material.cDiffuse.g = 0.5f;
    m_material.cSpecular.r = m_material.cSpecular.g = m_material.cSpecular.b = 1;
    m_material.shininess = 64;

    // Use a white directional light from the upper left corner
    memset(&m_light, 0, sizeof(m_light));
    m_light.type = LIGHT_DIRECTIONAL;
    m_light.dir = lightDirection;
    m_light.color.r = m_light.color.g = m_light.color.b = 1;
    m_light.id = 0;

    //TODO: [SHAPES] Allocate any additional memory you need...
    printf("created\n");

    currentShape = NULL;
    tree = new FractalTree();

}

ShapesScene::~ShapesScene()
{
    //TODO: [SHAPES] Don't leak memory!
    if (currentShape)
        delete currentShape;

    delete tree;
}

void ShapesScene::setLights(const Camera *follow)
{
    // YOU DON'T NEED TO TOUCH THIS METHOD,
    // unless you want to do fancy lighting...

    // Disable all OpenGL lights so the lights from the previous scene
    // don't appear in the next one.  There are 8 lights in OpenGL,
    // GL_LIGHT0 to GL_LIGHT7, and each GL_LIGHT* is one greater than
    // the previous one.  This means the nth light is GL_LIGHT0 + n.
    for (int id = 0; id < NUM_OPENGL_LIGHTS; ++id)
        glDisable(GL_LIGHT0 + id);

    m_light.dir = lightDirection * follow->getModelviewMatrix().getInverse().getTranspose();

    // Load one light in the upper left (subclasses will have to load lights
    // for sceneview anyway, so they will overwrite this light).
    setLight(m_light);
}

void ShapesScene::renderGeometry(bool useMaterials)
{
    // TODO: [SHAPES] Render a shape based on settings.shapeType.
    //       The sample code provided draws a single triangle to
    //       the screen using OpenGL. If you've chosen to implement
    //       subclasses for your shapes, this would be a good time
    //       to call upon the subclass to draw itself as well.
    //

    // Example shape:
    if (useMaterials)
        applyMaterial(m_material);

//    switch (settings.shapeType) {
//    case SHAPE_CUBE:
//        if (!currentShape)
//            currentShape = new Cube();
//        else if (currentShape->shapeType != SHAPE_CUBE) {
//            delete currentShape;
//            currentShape = new Cube();
//        }
//        break;
//    case SHAPE_CYLINDER:
//        if (!currentShape)
//            currentShape = new Cylinder();
//        else if (currentShape->shapeType != SHAPE_CYLINDER) {
//            delete currentShape;
//            currentShape = new Cylinder();
//        }
//        break;
//    case SHAPE_CONE:
//        if (!currentShape)
//            currentShape = new Cone();
//        else if (currentShape->shapeType != SHAPE_CONE) {
//            delete currentShape;
//            currentShape = new Cone();
//        }
//        break;
//    case SHAPE_SPHERE:
//        if (!currentShape)
//            currentShape = new Sphere();
//        else if (currentShape->shapeType != SHAPE_SPHERE) {
//            delete currentShape;
//            currentShape = new Sphere();
//        }
//        break;

//    case SHAPE_SPECIAL_1:
//        tree->generateTree();
//        break;

//    default:
//        if (currentShape)
//            delete currentShape;
//        currentShape = NULL;
//        break;
//    }

    tree->generateTree();



    if (!currentShape)
        return;

    currentShape->renderGeometry(settings.shapeParameter1,
                                 settings.shapeParameter2,
                                 settings.shapeParameter3);



}

void ShapesScene::renderNormals()
{
    //TODO: [SHAPES] Rendering normals is OPTIONAL in shapes, and it's not
    //      extra credit. However, we believe that rendering normals will
    //      greatly assist you with visual debugging - both in this and
    //      future assignments. Therefore, we highly recommend you fill
    //      this method in.
    //

    if (currentShape)
        currentShape->renderNormals(settings.shapeParameter1,
                                    settings.shapeParameter2,
                                    settings.shapeParameter3,
                                    m_cameraEye);
}

