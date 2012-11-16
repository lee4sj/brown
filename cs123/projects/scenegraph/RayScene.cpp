#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"
#include "Camera.h"
#include "RayThread.h"
#include "CamtransCamera.h"

#include <QThreadPool>

RayScene::RayScene()
{
}

RayScene::~RayScene()
{
}

/* Basic Algorithm */
/*
 1. Find the ray from the eye point to the film plane. and convert the rays to the world space.
 2. For each ray,
    1) find the closest intersecting object if any, (otherwise don't do anything).
    2) find the object normal of the intersecting object point.
    3) check for the opague object along the line between the intersecting object point and the light sources.
    4) Determine the color using illumination model
*/

void RayScene::trace(BGRA *data, Camera *camera, int width, int height)
{
    /* 1. find the ray and convert them to the world space */
    QList<Vector4> eyeRays = QList<Vector4>();
    findEyeRays(eyeRays, width, height, camera);

    assert(eyeRays.size() == width*height);

    Vector4 eyePos = camera->getPosition();

    /* 2. For each ray, compute */
    RayThread *rayThread = new RayThread(sceneList, m_lights, m_global, eyePos, &eyeRays, data);
    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(rayThread);
    threadPool->waitForDone();
}

void RayScene::findEyeRays(QList<Vector4> &eyeRays, int width, int height, Camera *camera)
{
    Vector4 pFilm, pWorld;
    pFilm.z = -1.0;
    pFilm.w = 1.0;

    Matrix4x4 viewMat = camera->getViewMatrix();
    Vector4 eyePos = camera->getPosition();

    Vector4 ray;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            //1. find the rays
            pFilm.x = 2.0 * x / width - 1.0;
            pFilm.y = 1.0 - 2.0 * y / height;
            pWorld = viewMat * pFilm;

            //2. convert the rays into the world space
            ray = (pWorld - eyePos);
            ray.w = 0.0;
            ray.normalize();

            eyeRays.append(ray);
        }
    }
}
