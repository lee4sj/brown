//#include "terrain.h"
//#ifdef __APPLE__
//#include <OpenGL/glext.h>
//#else
//#include <GL/glext.h>
//#endif

////#include <qgl.h>
//#include <iostream>

//#include <QMouseEvent>
//#include <QTimer>
//#include <QFile>

//#include <cmath>

//#include <stdio.h>

////#include "CS123Algebra.h"

////#define Vector3 vec3<REAL>

//Terrain::Terrain()
//{
//}

//Terrain::~Terrain(){

//}

//void Terrain::drawTerrain(){
//    glClearColor(0, 0, 0, 0);   // Always reset the screen to black before drawing anything
//    glEnable(GL_DEPTH_TEST);    // When drawing a triangle, only keep pixels closer to the camera than what's already been drawn

//    // Make things pretty
//    glEnable(GL_MULTISAMPLE);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

//    // Bind the ambient and diffuse color of each vertex to the current glColor() value
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//    // Cull triangles that are facing away from the camera
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);

//    // Set up a single light
//    glEnable(GL_LIGHTING);
//    GLfloat ambientColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//    GLfloat diffuseColor[] = { 1.0f, 1.0f, 1.0, 1.0f };
//    GLfloat specularColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//    GLfloat lightPosition[] = { 0.f, 0.f, 10.f, 1.0f };
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
//    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
//    glEnable(GL_LIGHT0);

//    // Enable texturing - feel free to substitue with your own image
//    glEnable(GL_TEXTURE_2D);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    if (loadTexture("/course/cs123/data/image/BoneHead.jpg") == -1)
//        //cout << "Texture does not exist" << endl;

//    srand(0);
//    populateTerrain();
//    computeNormals();
//    paintGL();
//}

//void Terrain::subdivideSquare(GridIndex topleft, GridIndex botright, GLint curDepth)
//{
//    // corner coordinates (in the grid space [x,y])
//    GridIndex TL = GridIndex(topleft.x, topleft.y);
//    GridIndex TR = GridIndex(botright.x, topleft.y);
//    GridIndex BL = GridIndex(topleft.x, botright.y);
//    GridIndex BR = GridIndex(botright.x, botright.y);

//    // corner vertices on the terrain (in the grid space [x,y,z])
//    WorldPoint &vTL = m_terrain[getIndex(TL)];
//    WorldPoint &vTR = m_terrain[getIndex(TR)];
//    WorldPoint &vBL = m_terrain[getIndex(BL)];
//    WorldPoint &vBR = m_terrain[getIndex(BR)];

//    GridIndex TM = GridIndex((TL+TR)/2);
//    GridIndex ML = GridIndex((TL+BL)/2);
//    GridIndex MR = GridIndex((TR+BR)/2);
//    GridIndex BM = GridIndex((BL+BR)/2);
//    GridIndex MM = GridIndex((ML+MR)/2);

//    WorldPoint vTM; //= vTL.x + vTR.x;
//    WorldPoint vML; //= vTL + vBL;
//    WorldPoint vMR; //= vTR + vBR;
//    WorldPoint vBM; //= vBL + vBR;
//    WorldPoint vMM;

//    vTM = (vTL+vTR)/2;
//    vML = (vTL+vBL)/2;
//    vMR = (vTR+vBR)/2;
//    vBM = (vBL+vBR)/2;
//    vMM = (vTL+vTR+vBL+vBR)/4;

//    vMM.y = (vTL.y + vTR.y + vBL.y + vBR.y)*0.25 + getPerturb(curDepth);

//    m_terrain[getIndex(TL)] = vTL;
//    m_terrain[getIndex(TM)] = vTM;
//    m_terrain[getIndex(TR)] = vTR;

//    m_terrain[getIndex(ML)] = vML;
//    m_terrain[getIndex(MM)] = vMM;
//    m_terrain[getIndex(MR)] = vMR;

//    m_terrain[getIndex(BL)] = vBL;
//    m_terrain[getIndex(BM)] = vBM;
//    m_terrain[getIndex(BR)] = vBR;

//    if(curDepth > 0){
//        subdivideSquare(TL,MM,curDepth-1);
//        subdivideSquare(TM,MR,curDepth-1);
//        subdivideSquare(ML,BM,curDepth-1);
//        subdivideSquare(MM,BR,curDepth-1);
//    }
//}

//void Terrain::populateTerrain()
//{
//    WorldPoint tl(-10, 0, -10);
//    WorldPoint tr(10, 0, -10);
//    WorldPoint bl(-10, 0, 10);
//    WorldPoint br(10, 0, 10);
//    GridIndex tlg(0,0);
//    GridIndex trg(0,m_gridLength-1);
//    GridIndex blg(m_gridLength-1, 0);
//    GridIndex brg(m_gridLength-1, m_gridLength-1);
//    m_terrain[getIndex(tlg)] = tl;
//    m_terrain[getIndex(trg)] = tr;
//    m_terrain[getIndex(blg)] = bl;
//    m_terrain[getIndex(brg)] = br;
//    subdivideSquare(tlg, brg, m_depth);
//}

//void Terrain::computeNormals()
//{
//    // For each vertex in the 2D grid...
//    for (int row = 0; row < m_gridLength; row++)
//    {
//        for (int column = 0; column < m_gridLength; column++)
//        {
//            const GridIndex gridPosition(row, column);                // 2D coordinate of the vertex on the terrain grid
//            const int terrainIndex = getIndex(gridPosition);          // Index into the 1D position and normal arrays
//            const WorldPoint& vertexPosition  = m_terrain[terrainIndex]; // Position of the vertex

//            // Get the neighbors of the vertex at (a,b)
//            const QList<WorldPoint*>& neighbors = getSurroundingVertices(gridPosition);
//            int numNeighbors = neighbors.size();

//            // @TODO: [Lab 5] Compute a list of vectors from vertexPosition to each neighbor in neighbors
//            Vector3 *offsets = new Vector3[numNeighbors];
//            for (int i = 0; i < numNeighbors; ++i)
//                offsets[i] = *neighbors[i] - vertexPosition;

//            // @TODO: [Lab 5] Compute cross products for each neighbor
//            Vector3 *normals = new Vector3[numNeighbors];
//            for (int i = 0; i < numNeighbors; ++i)
//                //normals[i] = Vector3::zero(); // TODO
//                normals[i] = offsets[i].cross(offsets[(i+1)%numNeighbors]);

//            // Average the normals and store the final value in the normal map
//            Vector3 sum = Vector3(0,0,0);
//            for (int i = 0; i < numNeighbors; ++i)
//                sum += normals[i];
//            m_normalMap[terrainIndex] = sum.getNormalized();

//            delete[] offsets;
//            delete[] normals;
//        }
//    }
//}

//void Terrain::paintGL()
//{
//    // Clear the color and depth buffers to the current glClearColor
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    // Push a new matrix onto the stack for modelling transformations
//    glPushMatrix();

//    glColor3f(0.5,0.5,0.5);
//    for(int i=0; i<m_gridLength-1; i++){//row
//        glBegin(GL_TRIANGLE_STRIP);
//        for(int j=0; j<m_gridLength; j++){//col

//            glTexCoord2f(j/(float)m_gridLength,i/(float)m_gridLength);
//            glNormal3dv(m_normalMap[getIndex(i,j)].data);
//            glVertex3dv(m_terrain[getIndex(i,j)].data);

//            glTexCoord2f(j/(float)m_gridLength,(i+1)/(float)m_gridLength);
//            glNormal3dv(m_normalMap[getIndex(i+1,j)].data);
//            glVertex3dv(m_terrain[getIndex(i+1,j)].data);


//        }
//        glEnd();

//    }


//    //draw normals
//    ///////////////
//    //drawNormals(); -->causes crazy line issues

//    // Discard the modelling transformations (leaving only camera settings)
//    glPopMatrix();
//    // Force OpenGL to perform all pending operations -- usually a good idea to call this
//    glFlush();
//    // Swap the buffers to show what we have just drawn onto the screen
//    //swapBuffers();
//}

//double Terrain::getPerturb(int cur_depth)
//{
//    return m_roughness
//            * pow((double)cur_depth / m_depth, m_decay)
//            * ((rand() % 200-100) / 100.0);
//}

//inline int Terrain::getIndex(const GridIndex &c)
//{
//    return getIndex(c.x, c.y);
//}

//inline int Terrain::getIndex(int row, int col)
//{
//    if (row < 0 || row >= m_gridLength || col < 0 || col >= m_gridLength)
//        return -1;

//    return row * m_gridLength + col;
//}

//// All of the following methods are to be used in finding the normals

//QList<WorldPoint*> Terrain::getSurroundingVertices(const GridIndex &coordinate)
//{
//    GridIndex coords[8];
//    coords[0] = GridIndex(coordinate.x,     coordinate.y - 1);
//    coords[1] = GridIndex(coordinate.x + 1, coordinate.y - 1);
//    coords[2] = GridIndex(coordinate.x + 1, coordinate.y);
//    coords[3] = GridIndex(coordinate.x + 1, coordinate.y + 1);
//    coords[4] = GridIndex(coordinate.x,     coordinate.y + 1);
//    coords[5] = GridIndex(coordinate.x - 1, coordinate.y + 1);
//    coords[6] = GridIndex(coordinate.x - 1, coordinate.y);
//    coords[7] = GridIndex(coordinate.x - 1, coordinate.y - 1);

//    int index;
//    QList<WorldPoint*> vecs;

//    for (int i = 0; i < 8; i++)
//    {
//        index = getIndex(coords[i]);
//        if (index != -1)
//            vecs.push_back(& m_terrain[index]);
//    }

//    return vecs;
//}

//void Terrain::drawNormals()
//{
//    if (m_renderNormals)
//    {
//        glColor3f(1,0,0);

//        for (int row = 0; row < m_gridLength; row++)
//        {
//            for (int column = 0; column < m_gridLength; column++)
//            {
//                glBegin(GL_LINES);

//                WorldPoint curVert = m_terrain[getIndex(row, column)];
//                Vector3 curNorm = m_normalMap[getIndex(row, column)];

//                glNormal3dv(curNorm.data);
//                glVertex3f(curVert.x, curVert.y, curVert.z);
//                glVertex3f(curVert.x +curNorm.x,
//                           curVert.y + curNorm.y,
//                           curVert.z + curNorm.z);

//                glEnd();
//            }
//        }
//    }
//}

//GLuint Terrain::loadTexture(const QString &filename)
//{
//    // Make sure the image file exists
//    QFile file(filename);
//    if (!file.exists())
//        return -1;

//    // Load the file into memory
//    QImage image;
//    image.load(file.fileName());
//    image = image.mirrored(false, true);
//    QImage texture = QGLWidget::convertToGLFormat(image);

//    // Generate a new OpenGL texture ID to put our image into
//    GLuint id = 0;
//    glGenTextures(1, &id);

//    // Make the texture we just created the new active texture
//    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);

//    // Copy the image data into the OpenGL texture
//    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());

//    // Set filtering options
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    // Set coordinate wrapping options
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

//    return 0;
//}
