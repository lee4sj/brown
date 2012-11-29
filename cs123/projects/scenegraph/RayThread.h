#ifndef RAYTHREAD_H
#define RAYTHREAD_H

#include <QRunnable>
#include <QList>
#include "CS123SceneData.h"
#include "Settings.h"
#include <map>
#include <QImage>

class Shapes;

class RayThread : public QRunnable
{
public:
    RayThread(QList<SceneListNode> *sceneList,
              QList<CS123SceneLightData> *lights,
              CS123SceneGlobalData global,
              Vector4 eyePos,
              QList<Vector4> *eyeRays,
              BGRA *data,
              int start,
              int end);
    virtual ~RayThread();

    void run();

private:
    QList<SceneListNode> *m_sceneList;
    QList<CS123SceneLightData> *m_lights;
    CS123SceneGlobalData m_global;
    Vector4 m_eyePos;
    QList<Vector4> *m_eyeRays;
    BGRA *m_data;
    int m_start, m_end;
    std::map<string, QImage *> m_texturesAll; //This is to only load the texture once.

    Shapes *shapes[NUM_SHAPE_TYPES];

    void trace(Vector4 &pWorld, QList<Vector4> *dWorlds, int start, int end, int depth, Vector3 &rContrib);
    void computeLightContribution(Vector4 &pWorld, int index, SceneListNode &obj, Vector4 &normalWorld, Vector4 &intersectionWorld, Vector4 &intersectionObj, int depth, Vector3 &curRContrib, Vector3 &rContrib);
    int findIntersectingObj(Vector4 &pworld, Vector4 &ray, REAL &t, Vector4 &normal);
    bool isIntersecting(Vector4 &pworld, Vector4 &ray, SceneListNode &obj, REAL &t, Vector4 &normal);
    void getTexture(Vector4 &intersectionObj, SceneListNode &obj, Vector3 &texture);

};

#endif // RAYTHREAD_H
