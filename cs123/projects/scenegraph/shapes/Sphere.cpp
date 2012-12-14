
#include "Sphere.h"
#include "CS123Algebra.h"
#include "Settings.h"

#include <qgl.h>

Sphere::Sphere()
{
    shapeType = SHAPE_SPHERE;
}

Sphere::~Sphere()
{

}

Vector3 Sphere::getCoord(double r, double phi, double theta)
{
    return Vector3(r * sin(phi) * cos(theta), r * cos(phi), r * sin(phi) * sin(theta));
}

/*
  Render Sphere.
  No points are saved since looping through all the saved points are the same as this.
  In the for loops, the bound is (0.5 - step/2) instead of (0.5 - step) to compensate for
  the rounding error of step.
 */
void Sphere::renderGeometry(int param1, int param2, float param3)
{
    if (param1 == 0)
        param1 = 1;
    if (param2 == 0)
        param2 = 1;

    double r = 0.5;

    double phiStep = M_PI/(param1 + 1); //xy plane
    double thetaStep = 2*M_PI/(param2 + 2); //xz plane

    Vector3 p1, p2, p3, p4;

    glBegin(GL_TRIANGLES);
    for (double theta = 0; theta < 2*M_PI; theta += thetaStep) {
        for(double phi = 0; phi < M_PI; phi += phiStep) {

            p1 = getCoord(r, phi, theta);
            p2 = getCoord(r, phi, theta + thetaStep);
            p3 = getCoord(r, phi + phiStep, theta + thetaStep);
            p4 = getCoord(r, phi + phiStep, theta);

            glNormal3f(p1.x / r, p1.y / r, p1.z / r);
            glVertex3f(p1.x, p1.y, p1.z);
            glNormal3f(p2.x / r, p2.y / r, p2.z / r);
            glVertex3f(p2.x, p2.y, p2.z);
            glNormal3f(p3.x / r, p3.y / r, p3.z / r);
            glVertex3f(p3.x, p3.y, p3.z);

            glNormal3f(p1.x / r, p1.y / r, p1.z / r);
            glVertex3f(p1.x, p1.y, p1.z);
            glNormal3f(p3.x / r, p3.y / r, p3.z / r);
            glVertex3f(p3.x, p3.y, p3.z);
            glNormal3f(p4.x / r, p4.y / r, p4.z / r);
            glVertex3f(p4.x, p4.y, p4.z);
        }
    }

    glEnd();
}

void Sphere::renderNormals(int  param1, int param2, float param3, Vector4 &cameraEye)
{
}

bool Sphere::isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal)
{
    Vector4 pObj = invTrans * eyePos;
    Vector4 dObj = invTrans * ray;
    pObj.w = 0;
    dObj.w = 0;

    t = -1;

    REAL a, b, c, d, q;

    a = dObj.dot(dObj);
    b = 2*(dObj.dot(pObj));
    c = pObj.dot(pObj) - .25;
    d = b*b - 4*a*c;

    if (d < 0)
        return false;

    REAL root = sqrt(d);

    q = b < 0? (-b + root)/(2.0) : (-b - root)/(2.0);
    t = q/a;

    if (c/q >= 0 && (c/q < t || t < 0))
        t = c/q;

    Vector4 intersect = pObj + dObj * t;

    normal = Vector4(intersect.x, intersect.y, intersect.z, 0.0);
    normal.normalize();

    if (t < 0)
        return false;

    return true;
}

void Sphere::getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture)
{
    REAL a, b, u, v, theta;
    int s, t;

    u = obj.textureMap.repeatU;
    v = obj.textureMap.repeatV;

    REAL phi = asin(-intersectionObj.y / 0.5);
    b = phi/M_PI + 0.5;

    if (b == 0 || b == 1)
        a = 0.5;
    else {
        theta = atan2(intersectionObj.z, intersectionObj.x);
        a = theta < 0 ? -theta / 2 / M_PI : 1.0 - theta/ 2 / M_PI;
    }

    s = ((int)(a * u * textureMap->width())) % textureMap->width();
    t = ((int)(b * v * textureMap->height())) % textureMap->height();

    QColor color = QColor(textureMap->pixel(s, t));
    texture.x = color.red() / 255.0;
    texture.y = color.green() / 255.0;
    texture.z = color.blue() / 255.0;
}











