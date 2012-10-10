#ifndef FILTER_H
#define FILTER_H

#include "CS123Common.h"
#include "Canvas2D.h"

class Filter
{
public:
    Filter();
    virtual ~Filter();
    virtual void applyFilter(Canvas2D *canvas) {}

protected:
    virtual bool checkBoundary(QPoint &start, QPoint &end, int width, int height);
};

#endif // FILTER_H
