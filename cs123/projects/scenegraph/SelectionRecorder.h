#ifndef SELECTIONRECORDER_H
#define SELECTIONRECORDER_H

struct HitRecord
{
    unsigned int numNames;
    unsigned int minDepth;
    unsigned int maxDepth;
    unsigned int name;
};

/*!
  @class SelectionRecorder
  @brief Allows you to work out which object is under the mouse pointer.

  This uses the GL_SELECT mode under the hood, which associates an object identifier
  with each pixel fragment.  Instead of being drawn to the screen, the pixel
  fragments are written to a "selection buffer" that records the pixel fragment's
  object identifier and depth value.

  To get the object under the cursor, we temporarily restrict the viewport to the
  1x1 square under the cursor, render our scene into the selection buffer, and take
  the object identifier of the closest fragment (the one with the smallest depth value).
*/
class SelectionRecorder
{
public:
    SelectionRecorder();
    ~SelectionRecorder();

    /*! @brief Enters selection mode with the ability to record numObjects objects. */
    void enterSelectionMode(int x, int y, int numObjects);

    /*! @brief Records any object drawn after this call as belonging to the object at index.
        @note This cannot be called within a glBegin() ... glEnd() block. */
    void setObjectIndex(int index);

    /*! @brief Returns the index of the selected object, or -1 for no selection. */
    int exitSelectionMode();

private:
    HitRecord *selectionBuffer;
};

#endif // SELECTIONRECORDER_H
