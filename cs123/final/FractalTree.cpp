#include "FractalTree.h"
#include "math.h"
#include <qgl.h>
#include <GL/freeglut.h>

#define MAX_DEPTH 3
#define BRANCH_LENGTH_DECREASE_FACTOR 0.7f
#define CHILD_BRANCH_ANGLE 5.0f

FractalTree::FractalTree()
{
}

void FractalTree::generateTree()
{
    Vector3 startPoint({0.0f,0.0f,0.0f});
    Vector3 endPoint({1.0f,0.0f,0.0f});
    float rotation = 30.0f;
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

void FractalTree::generateBranches(Vector3 curPos,
                                   float length,
                                   float rotation,
                                   const int depth)
{
//    if(depth > MAX_DEPTH) return;
//    cout << "at depth = " << depth << endl;


//    if(depth == 0){
//        glColor3f(1.0f,1.0f,1.0f);
//    }else if(depth == 1){
//        glColor3f(1.0f,0.0f,0.0f);
//    }else{
//        glColor3f(0.0f,1.0f,0.0f);
//    }

//    glPushMatrix();
//    glTranslatef(newPosition.x,newPosition.y,newPosition.z);
//    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
//    drawLine(length);
//    glPopMatrix();


//    const float newLength = length * BRANCH_LENGTH_DECREASE_FACTOR;
//    int nextDepth = depth + 1;
//    Vector3 nextPosition(newPosition.x+length, newPosition.y, newPosition.z);

//    float leftRotation = rotation + CHILD_BRANCH_ANGLE * nextDepth;
//    generateBranches(nextPosition,newLength,leftRotation,nextDepth);

//    float rightRotation = rotation - CHILD_BRANCH_ANGLE * nextDepth;
//    generateBranches(nextPosition,newLength,rightRotation,nextDepth);


    if (depth > MAX_DEPTH)
        return;

    glPushMatrix();
    glTranslatef(curPos.x, curPos.y, curPos.z);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    drawLine(length);
    glTranslatef(0.0, length, 0.0);

    float newLength = length * BRANCH_LENGTH_DECREASE_FACTOR;

    float leftRot = (rotation + CHILD_BRANCH_ANGLE * (depth+1));
    generateBranches(curPos, length, leftRot, depth+1);

    float rightRot = (rotation - CHILD_BRANCH_ANGLE * (depth+1));
    generateBranches(curPos, length, rightRot, depth+1);

    glPopMatrix();

}
