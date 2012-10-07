#ifndef FILTER_H
#define FILTER_H

#include "CS123Common.h"

class Filter
{
public:
    Filter();
    virtual ~Filter();
    virtual void applyFilter(BGRA *image, int x_max, int y_max);
};

#endif // FILTER_H
