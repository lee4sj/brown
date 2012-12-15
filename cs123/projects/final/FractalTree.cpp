#include "final/FractalTree.h"
#include "math.h"
#include <qgl.h>

#include "BranchCylinder.h"
#include "shapes/Cylinder.h"

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

void FractalTree::applyMaterial(const CS123SceneMaterial &material)
{
    // Make sure the members of CS123SceneColor are packed tightly
    COMPILE_TIME_ASSERT(sizeof(CS123SceneColor) == sizeof(float) * 4);

    // Use materials when lighting is enabled
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material.cAmbient.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material.cDiffuse.r);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material.cSpecular.r);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &material.cEmissive.r);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
}

#define RESIZE_FACTOR 3.0
void FractalTree::generateTree()
{
    //draw the body
    glPushMatrix();
    glEnable(GL_NORMALIZE);


    CS123SceneMaterial material;
    bzero(&material, sizeof(CS123SceneMaterial));
//    material.cAmbient.r = 166.0/255.0;
//    material.cAmbient.g = 45.0/255.0;
//    material.cAmbient.b = 45.0/255.0;
    material.cDiffuse.r = 120.0/255.0;
    material.cDiffuse.g = 45.0/255.0;
    material.cDiffuse.b = 45.0/255.0;
    applyMaterial(material);

    glScalef(RESIZE_FACTOR, RESIZE_FACTOR, RESIZE_FACTOR);

    drawLine(INITIAL_HEIGHT, INITIAL_HEIGHT/40.0, INITIAL_HEIGHT/50.0);
    glTranslatef(0.0, INITIAL_HEIGHT, 0.0);

    srand(randSeed);
    float rot1Rand = 45;
    float rot2Rand = 45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50.0);

    rot1Rand = -45;
    rot2Rand = 45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50.0);

    rot1Rand = 45;
    rot2Rand = -45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50.0);

    rot1Rand = -45;
    rot2Rand = -45;
    generateBranches(0.05f, rot1Rand, rot2Rand, MAX_DEPTH, INITIAL_HEIGHT/50);

    glDisable(GL_NORMALIZE);
    glPopMatrix();




//    Cylinder cyl;
//    cyl.renderGeometry(50, 50, 10);
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
    glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT);
    CS123SceneMaterial material;
    bzero(&material, sizeof(CS123SceneMaterial));
    material.cDiffuse.r = 45.0/255.0;
    material.cDiffuse.g = 190.0/255.0;
    material.cDiffuse.b = 45.0/255.0;
    applyMaterial(material);

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

void FractalTree::renderNormal()
{
    cyl->renderNormals(INITIAL_HEIGHT, INITIAL_HEIGHT/40, INITIAL_HEIGHT/50);
}












