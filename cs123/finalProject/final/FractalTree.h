#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include "vector.h"
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

    void generateTree(QHash<QString, QGLShaderProgram *> &m_shaderPrograms,
                      QHash<QString, GLuint> &m_textures);
    void renderNormal();

private:
    void generateBranches(float length,
                          float rotx,
                          float rotz,
                          const int depth,
                          double rStart);
    void drawLine(float length, double, double);
    void drawLeaf(double length, double width);
    void generateLeaf(float length,
                      float rotx,
                      float rotz);
    void applyMaterial(const CS123SceneMaterial &material);

    BranchCylinder *cyl;
    unsigned int randSeed;

    GLuint m_skybox;

    //bump map
    QHash<QString, QGLShaderProgram *> *m_shaderPrograms;
    QHash<QString, GLuint> *m_textures;
};

#endif // FRACTALTREE_H
