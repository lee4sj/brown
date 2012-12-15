#include "Cylinder.h"
#include "CS123Algebra.h"
#include "Settings.h"
#include "OpenGLScene.h"

#include <qgl.h>

Cylinder::Cylinder()
{
    shapeType = SHAPE_CYLINDER;
}

Cylinder::~Cylinder()
{

}

void Cylinder::renderGeometry(int param1, int param2, float param3)
{
    if (param1 == 0)
        param1 = 1;
    if (param2 == 0)
        param2 = 1;

    double yStep = 1/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);
    double r = 0.5;

    double x;
    double z;
    double xNext;
    double zNext;
    double normConst = 1/r;

    glBegin(GL_TRIANGLES);
    for (double theta = 0; theta < 2*M_PI; theta += thetaStep) {
        x = r * cos(theta);
        z = r * sin(theta);
        xNext = r * cos(theta + thetaStep);
        zNext = r * sin(theta + thetaStep);

        //render sides
        for (double y = -0.5; y <= 0.5 - yStep/2; y += yStep) {

            glNormal3f(normConst * x, 0, normConst * z);
            glTexCoord2f(theta / (2*M_PI), y + 0.5);
            glVertex3f(x, y, z);

            glNormal3f(normConst * xNext, 0, normConst * zNext);
            glTexCoord2f((theta+thetaStep) / (2*M_PI), y + yStep + 0.5);
            glVertex3f(xNext, y + yStep,zNext);

            glNormal3f(normConst * xNext, 0, normConst * zNext);
            glTexCoord2f((theta+thetaStep) / (2*M_PI), y + 0.5);
            glVertex3f(xNext, y, zNext);


            glNormal3f(normConst * x, 0, normConst * z);
            glTexCoord2f(theta / (2*M_PI), y + 0.5);
            glVertex3f(x, y, z);

            glNormal3f(normConst * x, 0, normConst * z);
            glTexCoord2f(theta / (2*M_PI), y  + yStep + 0.5);
            glVertex3f(x, y + yStep, z);

            glNormal3f(normConst * xNext, 0, normConst * zNext);
            glTexCoord2f((theta+thetaStep) / (2*M_PI), y + yStep + 0.5);
            glVertex3f(xNext, y + yStep, zNext);
        }

        //render the bottom and the top
        double xTex, zTex, xNextTex, zNextTex;

        xTex = x + r, zTex = z + r, xNextTex = xNext + r, zNextTex = zNext + r;

        glNormal3f(0, 1, 0);

        glTexCoord2f(0.5, 0.5);
        glVertex3f(0, 0.5, 0);

        glTexCoord2f(xNextTex, zNextTex);
        glVertex3f(xNext, 0.5, zNext);

        glTexCoord2f(xTex, zTex);
        glVertex3f(x, 0.5, z);



        glNormal3f(0, -1, 0);

        glTexCoord2f(0.5, 0.5);
        glVertex3f(0, -0.5, 0);

        glTexCoord2f(xTex, zTex);
        glVertex3f(x, -0.5, z);

        glTexCoord2f(xNextTex, zNextTex);
        glVertex3f(xNext, -0.5, zNext);
    }
    glEnd();
}

void Cylinder::renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye)
{
    m_cameraEye = cameraEye;
    if (param1 == 0)
        param1 = 1;
    if (param2 == 0)
        param2 = 1;

    double yStep = 1/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);
    double r = 0.5;

    double x;
    double z;
    double xNext;
    double zNext;

    glBegin(GL_TRIANGLES);
    for (double theta = 0; theta < 2*M_PI; theta += thetaStep) {
        x = r * cos(theta);
        z = r * sin(theta);
        xNext = r * cos(theta - thetaStep);
        zNext = r * sin(theta - thetaStep);

        //render sides
        for (double y = -0.5; y <= 0.5 - yStep/2; y += 2*yStep) {
            Vector4 normal = Vector4(x, 0, z, 1);

            renderNormal(Vector4(x, y, z, 1), normal);
            renderNormal(Vector4(x, y + yStep, z, 1), normal);
        }

        //render the bottom and the top
        Vector4 normal = Vector4(0, 1, 0, 1);
        renderNormal(Vector4(x, 0.5, z, 1), normal);
        renderNormal(Vector4(xNext, 0.5, zNext, 1), normal);

        normal = Vector4(0, -1, 0, 1);
        renderNormal(Vector4(xNext, -0.5, zNext, 1), normal);
        renderNormal(Vector4(x, -0.5, z, 1), normal);
    }
    glEnd();
}

bool Cylinder::isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal)
{
    Vector4 pObj = invTrans * eyePos;
    Vector4 dObj = invTrans * ray;
    pObj.w = 0;
    dObj.w = 0;

    t = -1;

    //Cylinder Cap Bottom
    Vector4 N = Vector4(0.0, -1.0, 0.0, 0.0);
    Vector4 cPoint = Vector4(0.0, -.5, 0.0, 0.0);

    float tTemp = (N.dot(cPoint) - N.dot(pObj)) / N.dot(dObj);
    Vector4 intersect = pObj + dObj*tTemp;

    if (intersect.x * intersect.x + intersect.z * intersect.z < .5*.5 && tTemp >= 0) {
        t = tTemp;
        normal = N;
    }

    //Cylinder Top Bottom
    N = Vector4(0.0, 1.0, 0.0, 0.0);
    cPoint = Vector4(0.0, .5, 0.0, 0.0);

    tTemp = (N.dot(cPoint) - N.dot(pObj)) / N.dot(dObj);
    intersect = pObj + dObj*tTemp;

    if (intersect.x * intersect.x + intersect.z * intersect.z < .5*.5 &&
        tTemp >= 0 && (tTemp < t || t < 0)) {
        t = tTemp;
        normal = N;
    }

    //Cylinder Body
    float a, b, c, d;

    a = dObj.x*dObj.x + dObj.z*dObj.z;
    b = 2*pObj.x*dObj.x + 2*pObj.z*dObj.z;
    c = pObj.x*pObj.x + pObj.z*pObj.z - .25;

    d = b*b - 4*a*c;

    if (d < 0)
        return false;

    REAL root = sqrt(d);

    REAL q = b<0 ? (-b + root)/(2.0) : (-b - root)/(2.0);
    tTemp = q/a;

    intersect = pObj + dObj*tTemp;

    if (tTemp >= 0 && (tTemp < t || t < 0) &&
        intersect.y <= 0.5 && intersect.y >= -0.5) {
        t = tTemp;
        normal = Vector4(intersect.x, 0.0, intersect.z, 0.0);
        normal.normalize();
    }

    tTemp = c/q;

    intersect = pObj + dObj*tTemp;

    if (tTemp >= 0 && (tTemp < t || t < 0) &&
        intersect.y <= 0.5 && intersect.y >= -0.5) {
        t = tTemp;
        normal = Vector4(intersect.x, 0.0, intersect.z, 0.0);
        normal.normalize();
    }

    if (t < 0) {
        t = -1;
        return false;
    }

    return true;
}

void Cylinder::getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture)
{
    REAL a, b, u, v, theta;
    int s, t;

    u = obj.textureMap.repeatU;
    v = obj.textureMap.repeatV;

    // if cap
    if (ABS(intersectionObj.y - 0.5) <= epsilon) {
        a = intersectionObj.x + 0.5;
        b = intersectionObj.z + 0.5;
    }

    else if (ABS(intersectionObj.y + 0.5) <= epsilon) {
        a = intersectionObj.x + 0.5;
        b = -intersectionObj.z + 0.5;
    }

    // if body
    else {
        theta = atan2(intersectionObj.z, intersectionObj.x);
        a = theta < 0 ? -theta / 2 / M_PI : 1.0 - theta/ 2 / M_PI;
        b = -intersectionObj.y + 0.5;
    }

    s = ((int)(a * u * textureMap->width())) % textureMap->width();
    t = ((int)(b * v * textureMap->height())) % textureMap->height();

    QColor color = QColor(textureMap->pixel(s, t));
    texture.x = color.red() / 255.0;
    texture.y = color.green() / 255.0;
    texture.z = color.blue() / 255.0;
}


















