#include "Filter.h"

Filter::Filter() {}
Filter::~Filter() {}

bool Filter::checkBoundary(QPoint &start, QPoint &end, int width, int height)
{
    if (start == end) {
        start.setX(0);
        start.setY(0);
        end.setX(width);
        end.setY(height);
        return true;
    }

    if (start.x() < 0) start.setX(0);
    if (start.x() > width) return false; //out of boundary : start.setX(width);
    if (start.y() < 0) start.setY(0);
    if (start.y() > height) return false; //out of boundary : start.setY(height);

    if (end.x() < 0) return false; //out of boundary : end.setX(-1);
    if (end.x() > width) end.setX(width);
    if (end.y() < 0) return false; //out of boundary : end.setY(-1);
    if (end.y() > height) end.setY(height);

    return true;
}
