#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include "math/CS123Algebra.h"
class SupportCanvas3D;
class BranchCylinder;

class FractalTree
{
public:
    FractalTree();
    virtual ~FractalTree();

    void generateTree();

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

    BranchCylinder *cyl;
    unsigned int randSeed;
};

#endif // FRACTALTREE_H
