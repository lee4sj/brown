#ifndef BRANCHCYLINDER_H
#define BRANCHCYLINDER_H

#include "shapes/Shapes.h"

class BranchCylinder : public Shapes
{
public:
    BranchCylinder();
    virtual ~BranchCylinder();

    virtual void renderGeometry(int param1, int param2, float param3);
    virtual void renderGeometry(float length, double rStart, double rEnd);
    virtual void renderNormals(float length, double rStart, double rEnd);
};

#endif // BRANCHCYLINDER_H