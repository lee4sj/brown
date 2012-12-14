#include "final/FractalTree.h"
#include "math.h"
#include <qgl.h>
#include <GL/freeglut.h>

#include "SupportCanvas3D.h"
#include "Camera.h"

#define MAX_DEPTH 10
#define BRANCH_LENGTH_DECREASE_FACTOR 0.7f
#define CHILD_BRANCH_ANGLE 60.0f

FractalTree::FractalTree()
{
}

void FractalTree::generateTree(SupportCanvas3D *context)
{
    // Get the active camera
    Camera *camera = context->getCamera();
    assert(camera);

    // Apply the camera settings
    double matrix[16];
    camera->getProjectionMatrix().getTranspose().fillArray(matrix);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
    camera->getModelviewMatrix().getTranspose().fillArray(matrix);
    glLoadMatrixd(matrix);

//    glTranslatef(context->width()/2, context->height()/2, 0);

    Vector3 startPoint({0.0f,0.0f,0.0f});
    Vector3 endPoint({1.0f,0.0f,0.0f});
    float rotation = 90.0f;
//    glutWireSphere(0.05, 4, 4);
    generateBranches(startPoint,1.0f,rotation,0);
}

void FractalTree::drawLine(float length)
{
    glBegin( GL_LINES );
    glColor3f( 1.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, length, 0.0f );
    glEnd();
}

void FractalTree::generateBranches(const Vector3 curPos,
                                   float length,
                                   float rotation,
                                   const int depth)
{
    if (depth > MAX_DEPTH)
        return;

    glPushMatrix();
    glTranslatef(curPos.x, curPos.y, curPos.z);
    glRotatef(rotation, 0.0, 1.0, 0.0);
    drawLine(length);
    glTranslatef(0.0, length, 0.0);

    float newLength = length * BRANCH_LENGTH_DECREASE_FACTOR;

    float leftRot = (rotation + CHILD_BRANCH_ANGLE * (depth+1));
    generateBranches(curPos, length, leftRot, depth+1);

    float rightRot = (rotation - CHILD_BRANCH_ANGLE * (depth+1));
    generateBranches(curPos, length, rightRot, depth+1);

    glPopMatrix();

}
