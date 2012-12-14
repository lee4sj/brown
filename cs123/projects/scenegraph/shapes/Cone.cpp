#include "Cone.h"
#include "CS123Algebra.h"
#include "Settings.h"

#include <qgl.h>

Cone::Cone()
{
    shapeType = SHAPE_CONE;
}

Cone::~Cone()
{

}

/*
  Render Cone.
  No points are saved since looping through all the saved points are the same as this.
  In the for loops, the bound is (0.5 - step/2) instead of (0.5 - step) to compensate for
  the rounding error of step.
 */
void Cone::renderGeometry(int param1, int param2, float param3)
{
    if (param1 == 0)
        param1 = 1;
    if (param2 == 0)
        param2 = 1;

    double rStep = 0.5/(double)param1;
    double yStep = 1/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);

    double x;
//    double y = -0.5;
    double z;
    double xNext;
    double zNext;
    double xRNext;
    double zRNext;
    double xNextRNext;
    double zNextRNext;
    double normX;
    double normY = 1.0/(2 * sqrt(5.0/4));
    double normZ;
    double normXNext;
    double normZNext;

    double r = 0.5;

    glBegin(GL_TRIANGLES);
    for (double y = -0.5; y <= 0.5 - yStep/2; y += yStep) {
        for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
            x = r * cos(theta);
            z = r * sin(theta);
            xNext = r * cos(theta - thetaStep);
            zNext = r * sin(theta - thetaStep);

            xRNext = (r - rStep) * cos(theta);
            zRNext = (r - rStep) * sin(theta);
            xNextRNext = (r - rStep) * cos(theta - thetaStep);
            zNextRNext = (r - rStep) * sin(theta - thetaStep);

            normX = cos(theta)/sqrt(5.0/4);
            normZ = sin(theta)/sqrt(5.0/4);
            normXNext = cos(theta - thetaStep)/sqrt(5.0/4);
            normZNext = sin(theta - thetaStep)/sqrt(5.0/4);

            glNormal3f(normX, normY, normZ);
            glVertex3f(x, y, z);
            glNormal3f(normXNext, normY, normZNext);
            glVertex3f(xNext, y, zNext);
            glNormal3f(normXNext, normY, normZNext);
            glVertex3f(xNextRNext, y + yStep, zNextRNext);

            glNormal3f(normX, normY, normZ);
            glVertex3f(x, y, z);
            glNormal3f(normXNext, normY, normZNext);
            glVertex3f(xNextRNext, y + yStep, zNextRNext);
            glNormal3f(normX, normY, normZ);
            glVertex3f(xRNext, y + yStep, zRNext);
        }
        r -= rStep;
    }

    double y = 0.5;

    //draw the tip
    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
        x = r * cos(theta);
        z = r * sin(theta);
        xNext = r * cos(theta - thetaStep);
        zNext = r * sin(theta - thetaStep);

        normX = cos(theta)/sqrt(5.0/4);
        normZ = sin(theta)/sqrt(5.0/4);
        normXNext = cos(theta - thetaStep)/sqrt(5.0/4);
        normZNext = sin(theta - thetaStep)/sqrt(5.0/4);

        glNormal3f(normX, normY, normZ);
        glVertex3f(x, y, z);
        glNormal3f(normXNext, normY, normZNext);
        glVertex3f(xNext, y, zNext);
        glNormal3f(normXNext, normY, normZNext);
        glVertex3f(0, y + yStep, 0);

        glNormal3f(normX, normY, normZ);
        glVertex3f(x, y, z);
        glNormal3f(normXNext, normY, normZNext);
        glVertex3f(0, y + yStep, 0);
        glNormal3f(normX, normY, normZ);
        glVertex3f(0, y + yStep, 0);
    }

    //draw the bottom
    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
        glNormal3f(0, -1, 0);
        glVertex3f(0, -0.5, 0);
        glVertex3f(0.5 * cos(theta), -0.5, 0.5 * sin(theta));
        glVertex3f(0.5 * cos(theta + thetaStep), -0.5, 0.5 * sin(theta + thetaStep));
    }
    glEnd();
}

void Cone::renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye)
{
    m_cameraEye = cameraEye;

    if (param1 == 0)
        param1 = 1;
    if (param2 == 0)
        param2 = 1;

    double rStep = 0.5/(double)param1;
    double yStep = 1/(double)param1;
    double thetaStep = 2*M_PI/((double)param2 + 2);

    double x;
    double y = -0.5;
    double z;
    double normX;
    double normY = 1.0/(2 * sqrt(5.0/4));
    double normZ;

    glBegin(GL_TRIANGLES);
    for (double r = 0.5; r > 0; r-=rStep) {
        for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
            x = r * cos(theta);
            z = r * sin(theta);
            normX = cos(theta) / sqrt(5.0/4);
            normZ = sin(theta) / sqrt(5.0/4);
            Vector4 normal = Vector4(normX, normY, normZ, 1);
            renderNormal(Vector4(x, y, z, 1), normal);
        }
        y += yStep;
    }

    //draw the bottom
    for (double theta = 2*M_PI; theta > 0; theta -= thetaStep) {
        glNormal3f(0, -1, 0);
        glVertex3f(0, -0.5, 0);
        glVertex3f(0.5 * cos(theta), -0.5, 0.5 * sin(theta));
        glVertex3f(0.5 * cos(theta + thetaStep), -0.5, 0.5 * sin(theta + thetaStep));
    }
    glEnd();
}


bool Cone::isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal)
{
    REAL a, b, c, d, root;

    t = -1;

    Vector4 pObj = invTrans * eyePos;
    Vector4 dObj = invTrans * ray;
    pObj.w = 0;
    dObj.w = 0;


    //cone body
    a = dObj.x * dObj.x + dObj.z * dObj.z - 0.25 * dObj.y * dObj.y;
    b = 2*pObj.x*dObj.x + 2*pObj.z*dObj.z - .5*pObj.y*dObj.y + .25*dObj.y;
    c = pObj.x*pObj.x + pObj.z*pObj.z - .25*pObj.y*pObj.y + .25*pObj.y - .0625;

    d = b*b - 4*a*c;

    if (d < 0)
        return false;

    root = sqrt(d);

    REAL q = b<0? (-b + root)/(2.0) : (-b - root)/(2.0);;
    REAL tTemp = q/a;

    Vector4 intersect = pObj + dObj * tTemp;

    if (-0.5 <= intersect.y && intersect.y <= 0.5 &&
        tTemp >= 0 && (tTemp < t || t < 0)) {
        t = tTemp;
        normal = Vector4(intersect.x, 0.0, intersect.z, 0.0);
        normal.normalize();
        normal.y = 0.5;
        normal.normalize();
    }

    tTemp = c/q;

    intersect = pObj + dObj * tTemp;

    if (-0.5 <= intersect.y && intersect.y <= 0.5 &&
        tTemp >= 0 && (tTemp < t || t < 0)) {
        t = tTemp;
        normal = Vector4(intersect.x, 0.0, intersect.z, 0.0);
        normal.normalize();
        normal.y = 0.5;
        normal.normalize();
    }

    //cone cap
    Vector4 N = Vector4(0.0, -1.0, 0.0, 0.0);
    Vector4 cPoint = Vector4(0.0, -0.5, 0.0, 0.0);

    tTemp = (N.dot(cPoint) - N.dot(pObj)) / N.dot(dObj);
    intersect = pObj + dObj * tTemp;

    if (intersect.x * intersect.x + intersect.z * intersect.z <= 0.5 * 0.5 &&
        tTemp >= 0 && (tTemp < t || t < 0)) {
        t = tTemp;
        normal = N;
    }

    if (t < 0) {
        t = -1;
        return false;
    }

    return true;
}

void Cone::getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture)
{
    REAL a, b, u, v, theta;
    int s, t;

    u = obj.textureMap.repeatU;
    v = obj.textureMap.repeatV;

    // if cone cap
    if (ABS(intersectionObj.y + 0.5) <= epsilon) {
        a = intersectionObj.x + 0.5;
        b = -intersectionObj.z + 0.5;
    }

    // if cone body
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


























