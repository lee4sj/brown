#ifndef GREYSCALEFILTER_H
#define GREYSCALEFILTER_H

#include "filter/Filter.h"

class GreyscaleFilter : public Filter
{
public:
    GreyscaleFilter();
    virtual ~GreyscaleFilter();
    virtual void applyFilter(Canvas2D *canvas);
};

#endif // GREYSCALEFILTER_H
