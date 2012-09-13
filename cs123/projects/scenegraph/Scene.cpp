#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>


Scene::Scene() //: m_camera(NULL)
{

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D

}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and finishParsing()
}

//void Scene::setCamera(Camera *newCamera)
//{
//    // Do not delete the old m_camera, it is owned by SupportCanvas3D
//    m_camera = newCamera;
//}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const Matrix4x4 &matrix)
{
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
}

