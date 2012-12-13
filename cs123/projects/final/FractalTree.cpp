#include "final/FractalTree.h"
#include "math.h"
#include <qgl.h>

#include "BranchCylinder.h"

#define MAX_DEPTH 7
#define BRANCH_DECREASING_FACTOR 0.8
#define RANDRANGE 240
#define RANDMIN 120
#define INITIAL_HEIGHT 0.2
#define LEAF_RADIUS 0.001
#define LEAF_LENGTH 0.005

FractalTree::FractalTree()
{
    cyl = new BranchCylinder();
    unsigned int randSeed = rand();
}

FractalTree::~FractalTree()
{
    delete cyl;
}

void FractalTree::generateTree()
{
    //draw the body
    glPushMatrix();
    glColor3f(166.0/255.0, 42/255.0, 42/255.0);
    drawLine(INITIAL_HEIGHT, INITIAL_HEIGHT/40, INITIAL_HEIGHT/50);
    glTranslatef(0.0, INITIAL_HEIGHT, 0.0);

    srand(randSeed);
//    float rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
//    float rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
    float rot1Rand = 45;
    float rot2Rand = 45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50);

    rot1Rand = -45;
    rot2Rand = 45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50);

    rot1Rand = 45;
    rot2Rand = -45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50);

//    rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
//    rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;

    rot1Rand = -45;
    rot2Rand = -45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50);
    glPopMatrix();
}

void FractalTree::drawLine(float length, double rStart, double rEnd)
{
//    glBegin( GL_LINES );
//    glVertex3f( 0.0f, 0.0f, 0.0f );
//    glVertex3f( 0.0f, length, 0.0f );
//    glEnd();

    cyl->renderGeometry(length, rStart, rEnd);
}

void FractalTree::generateLeaf(float length,
                               float rotx,
                               float rotz)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(rotz, 0.0, 0.0, 1.0);
    drawLine(length, LEAF_RADIUS, LEAF_RADIUS);

    glPopMatrix();
    glPopAttrib();
}

void FractalTree::generateBranches(float length,
                                   float rotx,
                                   float rotz,
                                   const int depth,
                                   double rStart)
{
    if (depth == 0)
        return;

    float rot1Rand;
    float rot2Rand;
    if (depth == 1) {
        rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        generateLeaf(LEAF_LENGTH, rot1Rand, rot2Rand);
        return;
    }

    glPushMatrix();
    glRotatef(rotx, 1.0, 0.0, 0.0);
    glRotatef(rotz, 0.0, 0.0, 1.0);


    drawLine(length, rStart, rStart/2);

    glTranslatef(0.0, length, 0.0);

    float newLength = length * BRANCH_DECREASING_FACTOR;

    rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
    rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
    generateBranches(newLength, rot1Rand, rot2Rand, depth-1, rStart/2);

    rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
    rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
    generateBranches(newLength, rot1Rand, rot2Rand, depth-1, rStart/2);

    int numLeaves = round(rand()/((float)RAND_MAX) * 3);

    for (int i = 0; i < numLeaves; i++) {
        rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        generateLeaf(LEAF_LENGTH, rot1Rand, rot2Rand);
    }

    glPopMatrix();
}












