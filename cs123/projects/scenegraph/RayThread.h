#ifndef RAYTHREAD_H
#define RAYTHREAD_H

#include <QRunnable>
#include <QList>
#include "CS123SceneData.h"
#include "Settings.h"

class Shapes;

class RayThread : public QRunnable
{
public:
    RayThread(QList<SceneListNode> *sceneList,
              QList<CS123SceneLightData> *lights,
              CS123SceneGlobalData global,
              Vector4 eyePos,
              QList<Vector4> *eyeRays,
              BGRA *data);
    virtual ~RayThread();

    void run();

private:
    QList<SceneListNode> *m_sceneList;
    QList<CS123SceneLightData> *m_lights;
    CS123SceneGlobalData m_global;
    Vector4 m_eyePos;
    QList<Vector4> *m_eyeRays;
    BGRA *m_data;

    Shapes *shapes[NUM_SHAPE_TYPES];


    void computeLightContribution(int index, SceneListNode obj, Vector4 &normalWorld, Vector4 &intersectionWorld);
    int findIntersectingObj(Vector4 &ray, REAL &t, Vector4 &normal);
    bool isIntersecting(Vector4 &ray, SceneListNode &obj, REAL &t, Vector4 &normal);
};

#endif // RAYTHREAD_H
