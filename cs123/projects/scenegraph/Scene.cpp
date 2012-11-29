#include "Scene.h"
#include "Camera.h"
#include "CS123SceneData.h"
#include "CS123ISceneParser.h"
#include <QtGlobal>
#include <QQueue>

Scene::Scene() //: m_camera(NULL)
{
    sceneList = new QList<SceneListNode>;
    m_lights = new QList<CS123SceneLightData>;
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
    delete sceneList;
    delete m_lights;
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser)
{
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and finishParsing()

    /* Use two stack, one for the transformation and the other for the node.
       Then using the two stack, perform breadth first traversal to form the list. */

    /* Deal with global */
    CS123SceneGlobalData global;
    if (parser->getGlobalData(global))
        sceneToFill->setGlobal(global);

    /* Deal with lights */
    CS123SceneLightData light;
    for (int i = 0; i < parser->getNumLights(); i++) {
        if (parser->getLightData(i, light))
            sceneToFill->addLight(light);
    }

    /* Deal with Objects */
    QQueue<CS123SceneNode*> nodeQ;
    QQueue<Matrix4x4> matQ;

    Matrix4x4 rootMat = Matrix4x4::identity();

    nodeQ.enqueue(parser->getRootNode());
    matQ.enqueue(rootMat);

    CS123SceneNode *curNode;
    Matrix4x4 curMat;
    Vector4 vtemp = Vector4(0, 0, 0, 1);

    while(!nodeQ.isEmpty()) {
        //First update current transformation
        curNode = nodeQ.dequeue();
        curMat = matQ.dequeue();

        Matrix4x4 mat = Matrix4x4::identity();

        for (unsigned int i = 0; i < curNode->transformations.size(); i++) {
            CS123SceneTransformation *trans = curNode->transformations.at(i);
            switch (trans->type) {
            case TRANSFORMATION_TRANSLATE:
                curMat *= getTransMat(trans->translate);
                break;
            case TRANSFORMATION_SCALE:
                curMat *= getScaleMat(trans->scale);
                break;
            case TRANSFORMATION_ROTATE:
                curMat *= getRotMat(vtemp, trans->rotate, trans->angle);
                break;
            case TRANSFORMATION_MATRIX:
                curMat *= trans->matrix;
                break;
            }
        }

        //Deal with the primitives
        for (unsigned int i = 0; i < curNode->primitives.size(); i++) {
            CS123ScenePrimitive *primitive = curNode->primitives.at(i);
            sceneToFill->addPrimitive(*primitive, curMat);
        }

        //Deal with children
        for (unsigned int i = 0; i < curNode->children.size(); i++) {
            CS123SceneNode *child = curNode->children.at(i);
            nodeQ.enqueue(child);
            matQ.enqueue(curMat);
        }
    }
}

//void Scene::setCamera(Camera *newCamera)
//{
//    // Do not delete the old m_camera, it is owned by SupportCanvas3D
//    m_camera = newCamera;
//}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const Matrix4x4 &matrix)
{
    SceneListNode listNode = {scenePrimitive, matrix, (*scenePrimitive.material.textureMap)};
//    listNode.textureMap.isUsed = scenePrimitive.material.textureMap->isUsed;
//    listNode.textureMap.filename = scenePrimitive.material.textureMap->filename;
//    listNode.textureMap.repeatU = scenePrimitive.material.textureMap->repeatU;
//    listNode.textureMap.repeatV = scenePrimitive.material.textureMap->repeatV;
    sceneList->append(listNode);
}

void Scene::addLight(const CS123SceneLightData &sceneLight)
{
    m_lights->append(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global)
{
    m_global = global;
}

