#include "final/FractalTree.h"
#include "math.h"
#include <qgl.h>

#include "BranchCylinder.h"
#include "CS123SceneData.h"
//#include "shapes/Cylinder.h"

#define MAX_DEPTH m_depth
#define BRANCH_DECREASING_FACTOR 0.8
#define RANDRANGE 240
#define RANDMIN 120
#define INITIAL_HEIGHT 0.2
#define LEAF_RADIUS 0.003
#define LEAF_LENGTH 0.008
#define PER_LEAFGEN_COUNT m_leafCount
#define RESIZE_FACTOR 8.0

FractalTree::FractalTree()
{
    cyl = new BranchCylinder();
    randSeed = 0;
    m_leafOn = true;
    m_depth = 7;
    m_textureOn = true;
    m_bumpIntensity = 1.0;
    m_leafCount = 15;
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

void FractalTree::setRandSeed(unsigned int r)
{
    randSeed = r;
}

void FractalTree::setDepth(int r)
{
    if (r >= 0)
        m_depth = r;
}

int FractalTree::getDepth()
{
    return m_depth;
}

void FractalTree::toggleLeaf()
{
    m_leafOn = !m_leafOn;
}

void FractalTree::toggleTexture()
{
    m_textureOn = !m_textureOn;
}

void FractalTree::toggleBump()
{
    if (m_bumpIntensity != 0.0f)
        m_bumpIntensity = 0.0f;
    else
        m_bumpIntensity = 1.0f;
}


void FractalTree::generateTree(QHash<QString, QGLShaderProgram *> &shaderPrograms,
                               QHash<QString, GLuint> &textures)
{
    m_shaderPrograms = &shaderPrograms;
    m_textures = &textures;

    //setup shaders
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures["normalMap"]);

    if (m_textureOn == true) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures["treeTexture"]);
    } else {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures["brown"]);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures["leafTexture"]);

    shaderPrograms["bump"]->bind();
    shaderPrograms["bump"]->setUniformValue("normalMap", 0);
    shaderPrograms["bump"]->setUniformValue("treeTexture", 1);
    shaderPrograms["bump"]->setUniformValue("bumpIntensity", m_bumpIntensity);

    shaderPrograms["leaf"]->setUniformValue("leafTexture", 0);


    //draw the body
    glPushMatrix();
    glEnable(GL_NORMALIZE);


    CS123SceneMaterial material;
    bzero(&material, sizeof(CS123SceneMaterial));
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


    //release shaders
    shaderPrograms["bump"]->release();



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

void FractalTree::drawLeaf(double length, double width)
{
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0, 0);
    glVertex3f(length, 0.0f, 0.0f);
    glTexCoord2f(1, 1);
    glVertex3f(length, 0.0f, width);
    glTexCoord2f(0, 1);
    glVertex3f(0.0f, 0.0f, width);

    glTexCoord2f(0, 0);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(0, 1);
    glVertex3f(0.0f, 0.0f, width);
    glTexCoord2f(1, 1);
    glVertex3f(length, 0.0f, width);
    glTexCoord2f(1, 0);
    glVertex3f(length, 0.0f, 0.0f);
    glEnd();
}


void FractalTree::generateLeaf(float length,
                               float rotx,
                               float rotz)
{
    if (m_leafOn == false)
        return;

    //Shader Setup
    (*m_shaderPrograms)["bump"]->release();
    (*m_shaderPrograms)["leaf"]->bind();

//    (*m_shaderPrograms)["bump"]->setUniformValue("treeTexture", 2);


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

    drawLeaf(length, LEAF_RADIUS);

    glPopMatrix();
    glPopAttrib();

//    (*m_shaderPrograms)["bump"]->setUniformValue("treeTexture", 1);


    //reverse the shader Setup
    (*m_shaderPrograms)["leaf"]->release();
    (*m_shaderPrograms)["bump"]->bind();
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
        int numLeaves = round(rand()/((float)RAND_MAX) * PER_LEAFGEN_COUNT);

        for (int i = 0; i < numLeaves; i++) {
            rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
            rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
            generateLeaf(LEAF_LENGTH, rot1Rand, rot2Rand);
        }

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

    int numLeaves = round(rand()/((float)RAND_MAX) * PER_LEAFGEN_COUNT);

    for (int i = 0; i < numLeaves; i++) {
        rot1Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        rot2Rand = (float)rand()/((float)RAND_MAX) * RANDRANGE - RANDMIN;
        generateLeaf(LEAF_LENGTH, rot1Rand, rot2Rand);
    }

    glPopMatrix();
}

void FractalTree::renderNormal()
{
//    cyl->renderNormals(INITIAL_HEIGHT, INITIAL_HEIGHT/40, INITIAL_HEIGHT/50);
}












