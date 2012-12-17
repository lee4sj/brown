#ifndef TERRAIN_H
#define TERRAIN_H

#include <QGLWidget>
#include <QMenuBar>

#include "vector.h"

#include <QFile>

typedef Vector2 GridIndex;
typedef Vector3 WorldPoint;

class Terrain
{
public:
    Terrain();
    ~Terrain();

    void drawTerrain();

    inline int getIndex(const GridIndex &c);
    inline int getIndex(int row, int column);
    double getPerturb(int cur_depth);
    QList<WorldPoint*> getSurroundingVertices(const GridIndex &coordinate);

    virtual void subdivideSquare(GridIndex tlg, GridIndex brg, GLint curDepth);
    virtual void computeNormals();
    virtual void paintGL();

    GLuint loadTexture(const QString &filename);
    //virtual void populateTerrain();
    virtual void drawNormals();
    void populateTerrain();

    //Member variables
    WorldPoint *m_terrain;
    Vector3 *m_normalMap;
    GLint m_gridLength;
    GLint m_depth;
    GLfloat m_roughness;
    GLint m_decay;
    bool m_renderNormals;
};

#endif // TERRAIN_H
