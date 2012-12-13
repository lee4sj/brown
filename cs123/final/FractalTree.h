#ifndef FRACTALTREE_H
#define FRACTALTREE_H

#include "math/CS123Algebra.h"

class FractalTree
{
public:
    FractalTree();
    void generateTree();

private:
    void generateBranches(const Vector3 newPosition,
                          float length,
                          float rotation,
                          const int depth);
    void drawLine(float length);
};

#endif // FRACTALTREE_H
