#include "Cube.h"
#include "CS123Algebra.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <qgl.h>

Cube::Cube()
{
    shapeType = SHAPE_CUBE;
}

Cube::~Cube()
{

}

/*
  Render Cube.
  No points are saved since looping through all the saved points are the same as this.
  In the for loops, the bound is (0.5 - step/2) instead of (0.5 - step) to compensate for
  the rounding error of step.
 */
void Cube::renderGeometry(int param1, int param2, float param3)
{
    if (param1 == 0)
        param1 = 1;

    double step = 1/(double)param1;

    glBegin(GL_TRIANGLES);

    for (double x = -0.5; x < 0.5 - step/2; x += step) {
        for (double y = -0.5; y < 0.5 - step/2; y += step) {
            glNormal3f(0, 0, 1);
            glVertex3f(x, y ,0.5);
            glVertex3f(x + step, y, 0.5);
            glVertex3f(x + step, y + step, 0.5);
            glVertex3f(x, y , 0.5);
            glVertex3f(x + step, y + step, 0.5);
            glVertex3f(x, y + step, 0.5);

            glNormal3f(0, 0, -1);
            glVertex3f(x + step, y + step, -0.5);
            glVertex3f(x + step, y, -0.5);
            glVertex3f(x, y , -0.5);
            glVertex3f(x, y + step, -0.5);
            glVertex3f(x + step, y + step, -0.5);
            glVertex3f(x, y , -0.5);

            glNormal3f(0, 1, 0);
            glVertex3f(x + step, 0.5, y + step);
            glVertex3f(x + step, 0.5, y);
            glVertex3f(x, 0.5, y);
            glVertex3f(x, 0.5, y + step);
            glVertex3f(x + step, 0.5, y + step);
            glVertex3f(x, 0.5, y);

            glNormal3f(0, -1, 0);
            glVertex3f(x, -0.5, y);
            glVertex3f(x + step, -0.5, y);
            glVertex3f(x + step, -0.5, y + step);
            glVertex3f(x, -0.5, y );
            glVertex3f(x + step, -0.5, y + step);
            glVertex3f(x, -0.5, y + step);

            glNormal3f(1, 0, 0);
            glVertex3f(0.5, x, y);
            glVertex3f(0.5, x + step, y);
            glVertex3f(0.5, x + step, y + step);
            glVertex3f(0.5, x, y);
            glVertex3f(0.5, x + step,  y + step);
            glVertex3f(0.5, x, y + step);

            glNormal3f(-1, 0, 0);
            glVertex3f(-0.5, x + step, y + step);
            glVertex3f(-0.5, x + step, y);
            glVertex3f(-0.5, x, y);
            glVertex3f(-0.5, x, y + step);
            glVertex3f(-0.5, x + step, y + step);
            glVertex3f(-0.5, x, y);
        }
    }

    glEnd();
}

bool Cube::isIntersect(Vector4 &eyePos, Vector4 &ray, Matrix4x4 invTrans, REAL &t, Vector4 &normal)
{
    Vector4 pObj = invTrans * eyePos;
    Vector4 dObj = invTrans * ray;

    pObj.w = 0;
    dObj.w = 0;

    Vector4 N[6];
    N[0] = Vector4(-1.0, 0.0, 0.0, 0.0); //Left
    N[1] = Vector4(1.0, 0.0, 0.0, 0.0); //Right
    N[2] = Vector4(0.0, -1.0, 0.0, 0.0); //Bottom
    N[3] = Vector4(0.0, 1.0, 0.0, 0.0); //Top
    N[4] = Vector4(0.0, 0.0, -1.0, 0.0); //Back
    N[5] = Vector4(0.0, 0.0, 1.0, 0.0); //Front

    Vector4 fPoint[6];
    for (int i = 0; i < 6; i++) {
        fPoint[i] = 0.5 * N[i];
    }

    REAL tTemp;
    t = -1;
    Vector4 intersect;
    for (int i = 0; i < 6; i++) {
        tTemp = (N[i].dot(fPoint[i]) - N[i].dot(pObj)) / N[i].dot(dObj);
        intersect = pObj + dObj * tTemp;

        if (((-0.5 <= intersect.x && intersect.x <= 0.5) || N[i].x != 0.0) &&
            ((-0.5 <= intersect.y && intersect.y <= 0.5) || N[i].y != 0.0) &&
            ((-0.5 <= intersect.z && intersect.z <= 0.5) || N[i].z != 0.0) &&
            ( tTemp >= 0 && (tTemp < t || t < 0))) {
            t = tTemp;
            normal = N[i];
        }
    }

    if (t >= 0)
        return true;

    return false;
}


void Cube::getTexture(QImage *textureMap, SceneListNode &obj, Vector4 &intersectionObj, REAL epsilon, Vector3 &texture) {
    REAL a, b, u, v;
    int s, t;

    u = obj.textureMap.repeatU;
    v = obj.textureMap.repeatV;

    if (ABS(intersectionObj.x - 0.5) <= epsilon)
    {
        a = -intersectionObj.z + 0.5;
        b = -intersectionObj.y + 0.5;
    }
    if (ABS(intersectionObj.x + 0.5) <= epsilon)
    {
        a = intersectionObj.z + 0.5;
        b = -intersectionObj.y + 0.5;
    }
    if (ABS(intersectionObj.y - 0.5) <= epsilon)
    {
        a = intersectionObj.x + 0.5;
        b = intersectionObj.z + 0.5;
    }
    if (ABS(intersectionObj.y + 0.5) <= epsilon)
    {
        a = intersectionObj.x + 0.5;
        b = -intersectionObj.z + 0.5;
    }
    if (ABS(intersectionObj.z - 0.5) <= epsilon)
    {
        a = intersectionObj.x + 0.5;
        b = -intersectionObj.y + 0.5;
    }
    if (ABS(intersectionObj.z + 0.5) <= epsilon)
    {
        a = -intersectionObj.x + 0.5;
        b = -intersectionObj.y + 0.5;
    }

//    if (ABS(intersectionObj.x - 0.5) <= epsilon || ABS(intersectionObj.x + 0.5) <= epsilon) {
//        a = intersectionObj.z + 0.5;
//        b = intersectionObj.y + 0.5;
//    } else if (ABS(intersectionObj.y - 0.5) <= epsilon || ABS(intersectionObj.y + 0.5) <= epsilon) {
//        a = intersectionObj.x + 0.5;
//        b = intersectionObj.z + 0.5;
//    } else {
//        a = intersectionObj.x + 0.5;
//        b = intersectionObj.y + 0.5;
//    }

    s = ((int)(a * u * textureMap->width())) % textureMap->width();
    t = ((int)(b * v * textureMap->height())) % textureMap->height();

    QColor color = QColor(textureMap->pixel(s, t));
    texture.x = color.red() / 255.0;
    texture.y = color.green() / 255.0;
    texture.z = color.blue() / 255.0;
}
