#ifndef BRANCHCYLINDER_H
#define BRANCHCYLINDER_H

class BranchCylinder
{
public:
    BranchCylinder();
    virtual ~BranchCylinder();

    virtual void renderGeometry(float length, double rStart, double rEnd);
};

#endif // BRANCHCYLINDER_H
