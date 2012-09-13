#include "SelectionRecorder.h"
#include "CS123Common.h"
#include <qgl.h>
#include <iostream>

using namespace std;

SelectionRecorder::SelectionRecorder()
{
    selectionBuffer = NULL;
}

SelectionRecorder::~SelectionRecorder()
{
    delete [] selectionBuffer;
}

void SelectionRecorder::enterSelectionMode(int x, int y, int numObjects)
{
    if(selectionBuffer != NULL)
    {
        cout << "error: enterSelectionMode() called twice without calling exitSelectionMode() in between" << endl;
        assert(false);
        return;
    }

    // Pre-multiply the projection matrix with a pick matrix, which expands
    // the viewport to fill the 1x1 square under the mouse pointer at (x, y).
    int viewport[4];
    double projectionMatrix[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glGetDoublev(GL_PROJECTION, projectionMatrix);
    glLoadIdentity();
    gluPickMatrix((double)x, (double)y, 1, 1, viewport);
    glMultMatrixd(projectionMatrix);
    glMatrixMode(GL_MODELVIEW);

    // Make sure the members of HitRecord are packed tightly.
    COMPILE_TIME_ASSERT(sizeof(HitRecord) == sizeof(unsigned int) * 4);

    // Enter selection mode with enough room for numObjects hit records.
    selectionBuffer = new HitRecord[numObjects < 1 ? 1 : numObjects];
    glSelectBuffer(numObjects * 4, (unsigned int *)selectionBuffer);
    glRenderMode(GL_SELECT);
    glInitNames();
    glLoadName(-1);
}

void SelectionRecorder::setObjectIndex(int index)
{
    // Replace the current name on top of the name stack with index.
    // We only use a name stack of depth one, so it's like a variable.
    glLoadName(index);
}

int SelectionRecorder::exitSelectionMode()
{
    // Default to -1, which means no selection.
    int selectedObject = -1;

    // Leave selection mode and parse the hits it generated.
    int hits = glRenderMode(GL_RENDER);
    unsigned int minDepth = ~0;
    for (int i = 0; i < hits; i++)
    {
        const HitRecord &hit = selectionBuffer[i];
        if (hit.minDepth < minDepth)
        {
            selectedObject = hit.name;
            minDepth = hit.minDepth;
        }
    }

    // We're done with the selection buffer.
    delete [] selectionBuffer;
    selectionBuffer = NULL;

    return selectedObject;
}
