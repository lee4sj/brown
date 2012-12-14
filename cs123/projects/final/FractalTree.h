#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include "math/CS123Algebra.h"
#include <QHash>
#include <QGLShaderProgram>

class SupportCanvas3D;
class BranchCylinder;
class CS123SceneMaterial;

class FractalTree
{
public:
    FractalTree();
    virtual ~FractalTree();

    void generateTree();
    void renderNormal();

private:
    void generateBranches(float length,
                          float rotx,
                          float rotz,
                          const int depth,
                          double rStart);
    void drawLine(float length, double, double);
    void generateLeaf(float length,
                      float rotx,
                      float rotz);
    void applyMaterial(const CS123SceneMaterial &material);

    BranchCylinder *cyl;
    unsigned int randSeed;

    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    GLuint m_skybox;
};

#endif // FRACTALTREE_H
