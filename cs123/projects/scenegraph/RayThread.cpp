#include "RayThread.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"
#include "shapes/Sphere.h"

//Definitions for global constants
#define M_EPSILON 0.05 //shadow
#define M_EPSILON2 0.00001 //texture
#define M_MAX_DEPTH 7 //define the max depth to be 7. (0 is always the top layer)


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
    map<string, QImage *>::iterator it;
    for (it = m_texturesAll.begin(); it != m_texturesAll.end(); it++)
        delete (*it).second;

    m_texturesAll.clear();
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
    Vector3 rContrib;
    trace(m_eyePos, m_eyeRays, m_start, m_end, 0, rContrib);
}

void RayThread::trace(Vector4 &pWorld, QList<Vector4> *dWorlds, int start, int end, int depth, Vector3 &rContrib)
{
    if (depth == M_MAX_DEPTH)
        return;

    Vector4 ray, normalObj, normalWorld, intersectionWorld, intersectionObj;
    SceneListNode closestObj;
    REAL t;
    int index;
    /* for each ray*/
    for (int i = start; i < end; i++) {
        ray = dWorlds->at(i);

        /* 1) find the closest intersecting object if any */
        /* 2) find the object normal of the intersecting object point. */
        if ( (index = findIntersectingObj(pWorld, ray, t, normalObj)) < 0 )
            continue;

        closestObj = m_sceneList->at(index);

        /* convert normal to world space */
        normalObj.w = 0;
        normalObj.normalize();
        //normalObj.w = 1;
        normalWorld = closestObj.mat.getInverse().getTranspose() * normalObj;
        normalWorld.w = 0;
        normalWorld.normalize();

        intersectionWorld = pWorld + ray * t;

        Vector4 pObj = closestObj.mat.getInverse() * pWorld;
        Vector4 dObj = closestObj.mat.getInverse() * ray;
        pObj.w = 0;
        dObj.w = 0;
        intersectionObj = pObj + dObj * t;

        /* 3) check for the opague object along the line between the intersecting object point and the light sources.
           4) Determine the color using illumination model */
        Vector3 curRContrib = Vector3(0.0, 0.0, 0.0);
        if (settings.useReflection) {
            QList<Vector4> recursiveDWorlds;

            //get secondary rays.
            Vector4 sRay, reflectedRay;
            reflectedRay = normalWorld * 2 * ( normalWorld.dot(-ray)) + ray;  //reflected view

            recursiveDWorlds.append(reflectedRay);

            Vector4 curPWorld = intersectionWorld + reflectedRay*M_EPSILON;
            trace(curPWorld, &recursiveDWorlds, 0, 1, depth+1, curRContrib);
        }

        computeLightContribution(pWorld, i, closestObj, normalWorld, intersectionWorld, intersectionObj, depth, curRContrib, rContrib);
    }
}

void RayThread::getTexture(Vector4 &intersectionObj, SceneListNode &obj, Vector3 &texture) {
    QImage *textureMap;

    if (m_texturesAll.count(obj.textureMap.filename) == 0) {
        textureMap = new QImage(obj.textureMap.filename.c_str());
        if (textureMap->width() == 0) {
            printf("failed to load %s, %u\n", obj.textureMap.filename.c_str(),
                   obj.textureMap.isUsed);
            fflush(stdout);
            delete textureMap;
            texture = Vector3(0,0,0);
            return;
        }
        m_texturesAll[obj.textureMap.filename] = textureMap;
    } else {
        textureMap = m_texturesAll[obj.textureMap.filename];
    }

    switch (obj.primitive.type) {
    case PRIMITIVE_CUBE:
        shapes[SHAPE_CUBE]->getTexture(textureMap, obj, intersectionObj, M_EPSILON2, texture);
        break;
    case PRIMITIVE_CONE:
        shapes[SHAPE_CONE]->getTexture(textureMap, obj, intersectionObj, M_EPSILON2, texture);
        break;
    case PRIMITIVE_CYLINDER:
        shapes[SHAPE_CYLINDER]->getTexture(textureMap, obj, intersectionObj, M_EPSILON2, texture);
        break;
    case PRIMITIVE_SPHERE:
        shapes[SHAPE_SPHERE]->getTexture(textureMap, obj, intersectionObj, M_EPSILON2, texture);
        break;
    default:
        break;
    }
}

void RayThread::computeLightContribution(Vector4 &pWorld, int index, SceneListNode &obj, Vector4 &normalWorld, Vector4 &intersectionWorld, Vector4 &intersectionObj, int depth, Vector3 &curRContrib, Vector3 &rContrib)
{
    REAL r, g, b;
    CS123SceneLightData light;
    REAL lightMagnitude, fatt;
    Vector4 lightDirection;

    Vector4 rVec, vVec;

    CS123SceneColor specularContribution, pointContribution;

    r = m_global.ka * obj.primitive.material.cAmbient.r;
    g = m_global.ka * obj.primitive.material.cAmbient.g;
    b = m_global.ka * obj.primitive.material.cAmbient.b;

    //get texture map if using
    Vector3 texture;
    if (settings.useTextureMapping && obj.textureMap.isUsed) {
        getTexture(intersectionObj, obj, texture);
    }

    for (int i = 0; i < m_lights->size(); i++) {
        light = m_lights->at(i);

        //calculate light direction and magnitude

        if (light.type == LIGHT_DIRECTIONAL && settings.useDirectionalLights) {
            lightDirection = -light.dir;
            lightDirection.normalize();
            lightMagnitude = -1;
        } else if (light.type == LIGHT_POINT && settings.usePointLights) {
            lightDirection = light.pos - intersectionWorld;
            lightDirection.w = 0;
            lightMagnitude = lightDirection.getMagnitude();
            lightDirection.normalize();
        } else
            continue;

        //Check for shadowing
        if (settings.useShadows) {
            int shadowIndex;
            REAL shadowT;
            Vector4 shadowNormal, pworld;

            pworld = intersectionWorld + lightDirection * M_EPSILON;
            shadowIndex = findIntersectingObj(pworld, lightDirection, shadowT, shadowNormal);

            // If the first object the light is hitting is NOT this object (i.e the new t <= the lightMagnitude), then something is obstructing. So skip this light.
            if (light.type == LIGHT_DIRECTIONAL && shadowIndex >= 0)
                continue;
            else if (light.type == LIGHT_POINT && (shadowT >= 0 && shadowT < lightMagnitude))
                continue;
        }

        //calculate specular light
        rVec = normalWorld * 2 * (normalWorld.dot(lightDirection)) - lightDirection;
        vVec = pWorld - intersectionWorld;

        rVec.w = 0;
        vVec.w = 0;
        rVec.normalize();
        vVec.normalize();
        REAL specCoeff = pow(max(0.0, rVec.dot(vVec)), obj.primitive.material.shininess);
        specularContribution.r = m_global.ks * obj.primitive.material.cSpecular.r * specCoeff;
        specularContribution.g = m_global.ks * obj.primitive.material.cSpecular.g * specCoeff;
        specularContribution.b = m_global.ks * obj.primitive.material.cSpecular.b * specCoeff;

        //calculate attenuation, fatt
        if (settings.useDirectionalLights && light.type == LIGHT_DIRECTIONAL)
            fatt = 1.0;
        else if (settings.usePointLights && light.type == LIGHT_POINT) {
            fatt = 1 / (light.function.x + light.function.y * lightMagnitude + light.function.z * lightMagnitude * lightMagnitude);
            fatt = min(1.0, max(0.0, fatt));
        } else
            fatt = 0;

        //caculate point light contribution
        REAL pointCoeff = max(0.0, normalWorld.dot(lightDirection));


        if (settings.useTextureMapping && obj.textureMap.isUsed) {
            REAL blend = obj.primitive.material.blend;
            pointContribution.r = m_global.kd * obj.primitive.material.cDiffuse.r * pointCoeff * (1.0 - blend)
                                                 + texture.x * pointCoeff * blend;
            pointContribution.g = m_global.kd * obj.primitive.material.cDiffuse.g * pointCoeff * (1.0 - blend)
                                                 + texture.y * pointCoeff * blend;
            pointContribution.b = m_global.kd * obj.primitive.material.cDiffuse.b * pointCoeff * (1.0 - blend)
                                                 + texture.z * pointCoeff * blend;
        } else {
            pointContribution.r = m_global.kd * obj.primitive.material.cDiffuse.r * pointCoeff;
            pointContribution.g = m_global.kd * obj.primitive.material.cDiffuse.g * pointCoeff;
            pointContribution.b = m_global.kd * obj.primitive.material.cDiffuse.b * pointCoeff;
        }

        r += fatt * light.color.r * (pointContribution.r + specularContribution.r);
        g += fatt * light.color.g * (pointContribution.g + specularContribution.g);
        b += fatt * light.color.b * (pointContribution.b + specularContribution.b);
    }

    if (settings.useReflection) {
        r += m_global.ks * obj.primitive.material.cReflective.r * curRContrib.x;
        g += m_global.ks * obj.primitive.material.cReflective.g * curRContrib.y;
        b += m_global.ks * obj.primitive.material.cReflective.b * curRContrib.z;
    }

    if (depth == 0) {
        m_data[index].r = min( 255.0, max( 0.0, r * 255.0 ));
        m_data[index].g = min( 255.0, max( 0.0, g * 255.0 ));
        m_data[index].b = min( 255.0, max( 0.0, b * 255.0 ));
    } else {
        rContrib.x += r;
        rContrib.y += g;
        rContrib.z += b;
    }
}


/* returns the index of the closest intersecting object
           -1 if there's no intersecting object */
int RayThread::findIntersectingObj(Vector4 &pworld, Vector4 &ray, REAL &t, Vector4 &normal)
{
    Vector4 nTemp;
    REAL tTemp;
    t = -1; // t = -1 notes that t is not set yet. Usually t >= 0.
    SceneListNode obj;
    int index = -1;
    for(int i = 0; i < m_sceneList->size(); i++) {
        obj = m_sceneList->at(i);

        if (isIntersecting(pworld, ray, obj, tTemp, nTemp) &&
            (tTemp < t || t < 0)) {
            index = i;
            assert(tTemp >= 0);
            t = tTemp;
            normal = nTemp;
        }
    }

    return index;
}

bool RayThread::isIntersecting(Vector4 &pworld, Vector4 &ray, SceneListNode &obj, REAL &t, Vector4 &normal)
{
    switch (obj.primitive.type) {
    case PRIMITIVE_CUBE:
        return shapes[SHAPE_CUBE]->isIntersect(pworld, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_CONE:
        return shapes[SHAPE_CONE]->isIntersect(pworld, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_CYLINDER:
        return shapes[SHAPE_CYLINDER]->isIntersect(pworld, ray, obj.mat.getInverse(), t, normal);
        break;
    case PRIMITIVE_SPHERE:
        return shapes[SHAPE_SPHERE]->isIntersect(pworld, ray, obj.mat.getInverse(), t, normal);
        break;
    default:
        return false;
        break;
    }
}
