#include "RayThread.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"


RayThread::RayThread(QList<SceneListNode> *sceneList,
                     QList<CS123SceneLightData> *lights,
                     CS123SceneGlobalData global,
                     Vector4 eyePos,
                     QList<Vector4> *eyeRays,
                     BGRA *data,
                     int start,
                     int end)
{
    m_sceneList = sceneList;
    m_lights = lights;
    m_global = global;
    m_eyePos = eyePos;
    m_eyeRays = eyeRays;
    m_data = data;
    m_start = start;
    m_end = end;

    memset(shapes, 0, sizeof(Shapes *) * NUM_SHAPE_TYPES);

    shapes[SHAPE_CUBE] = new Cube();
    shapes[SHAPE_CONE] = new Cone();
    shapes[SHAPE_SPHERE] = new Sphere();
    shapes[SHAPE_CYLINDER] = new Cylinder();
}

RayThread::~RayThread()
{
    for (int i = 0; i < NUM_SHAPE_TYPES; i++) {
        if (shapes[i])
            delete shapes[i];
    }
}


/*
 2. For each ray,
    1) find the closest intersecting object if any, (otherwise don't do anything).
    2) find the object normal of the intersecting object point.
    3) check for the opague object along the line between the intersecting object point and the light sources.
    4) Determine the color using illumination model
*/
void RayThread::run()
{
    Vector4 ray, normalObj, normalWorld, intersectionWorld;
    SceneListNode closestObj;
    REAL t;
    int index;
    /* for each ray*/
    for (int i = m_start; i < m_end; i++) {
        ray = m_eyeRays->at(i);

        /* 1) find the closest intersecting object if any */
        /* 2) find the object normal of the intersecting object point. */
        if ( (index = findIntersectingObj(ray, t, normalObj)) < 0 )
            continue;

        closestObj = m_sceneList->at(index);

        /* convert normal to world space */
        normalObj.w = 0;
        normalObj.normalize();
        //normalObj.w = 1;
        normalWorld = closestObj.mat.getInverse().getTranspose() * normalObj;
        normalWorld.w = 0;
        normalWorld.normalize();

        intersectionWorld = m_eyePos + ray * t;

        /* 3) check for the opague object along the line between the intersecting object point and the light sources.
           4) Determine the color using illumination model */
        computeLightContribution(i, closestObj, normalWorld, intersectionWorld);
    }
}


void RayThread::computeLightContribution(int index, SceneListNode obj, Vector4 &normalWorld, Vector4 &intersectionWorld)
{
    REAL r, g, b;
    CS123SceneLightData light;
    Vector4 lightDirection;

    r = m_global.ka * obj.primitive.material.cAmbient.r;
    g = m_global.ka * obj.primitive.material.cAmbient.g;
    b = m_global.ka * obj.primitive.material.cAmbient.b;

    for (int i = 0; i < m_lights->size(); i++) {
        light = m_lights->at(i);

        lightDirection = light.pos - intersectionWorld;
        lightDirection.w = 0;
        lightDirection.normalize();

        r += light.color.r * (m_global.kd * obj.primitive.material.cDiffuse.r *
                            max(0.0, normalWorld.dot(lightDirection)));
        g += light.color.g * (m_global.kd * obj.primitive.material.cDiffuse.g *
                            max(0.0, normalWorld.dot(lightDirection)));
        b += light.color.b * (m_global.kd * obj.primitive.material.cDiffuse.b *
                            max(0.0, normalWorld.dot(lightDirection)));
    }

    m_data[index].r = min( 255.0, max( 0.0, r * 255.0 ));
    m_data[index].g = min( 255.0, max( 0.0, g * 255.0 ));
    m_data[index].b = min( 255.0, max( 0.0, b * 255.0 ));
}


/* returns the index of the closest intersecting object
           -1 if there's no intersecting object */
int RayThread::findIntersectingObj(Vector4 &ray, REAL &t, Vector4 &normal)
{
    Vector4 nTemp;
    REAL tTemp;
    t = -1; // t = -1 notes that t is not set yet. Usually t >= 0.
    SceneListNode obj;
    int index = -1;
    for(int i = 0; i < m_sceneList->size(); i++) {
        obj = m_sceneList->at(i);

        if (isIntersecting(ray, obj, tTemp, nTemp) &&
            (tTemp < t || t < 0)) {
            index = i;
            assert(tTemp >= 0);
            t = tTemp;
            normal = nTemp;
        }
    }

    return index;
}

bool RayThread::isIntersecting(Vector4 &ray, SceneListNode &obj, REAL &t, Vector4 &normal)
{
    switch (obj.primitive.type) {
    case PRIMITIVE_CUBE:
        return shapes[SHAPE_CUBE]->isIntersect(m_eyePos, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_CONE:
        return shapes[SHAPE_CONE]->isIntersect(m_eyePos, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_CYLINDER:
        return shapes[SHAPE_CYLINDER]->isIntersect(m_eyePos, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_SPHERE:
        return shapes[SHAPE_SPHERE]->isIntersect(m_eyePos, ray, obj.mat.getInverse(), t, normal);
        break;
    default:
        return false;
        break;
    }
}
