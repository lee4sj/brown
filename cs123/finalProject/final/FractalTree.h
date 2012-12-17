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

    void setRandSeed(unsigned int r);
    unsigned int getRandSeed() { return randSeed; }

    void toggleLeaf();
    void toggleTexture();
    void toggleBump();

    void setDepth(int r);
    int getDepth();

    void setBumpIntensity(float b) { if (b >= 0.0) m_bumpIntensity = b; }
    float getBumpIntensity() { return m_bumpIntensity; }

    void setLeafCount(int l) { if (l >= 0) m_leafCount = l; }
    int getLeafCount() { return m_leafCount; }

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

//    GLuint m_skybox;

    bool m_leafOn;
    int m_depth;
    bool m_textureOn;
    float m_bumpIntensity;
    int m_leafCount;

    //bump map
    QHash<QString, QGLShaderProgram *> *m_shaderPrograms;
    QHash<QString, GLuint> *m_textures;
};

#endif // FRACTALTREE_H
