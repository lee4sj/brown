#ifndef SHARPENFILTER_H
#define SHARPENFILTER_H

#include "filter/Filter.h"

class Canvas2D;

class SharpenFilter : public Filter
{
public:
    SharpenFilter();
    virtual ~SharpenFilter();
    virtual void applyFilter(Canvas2D *canvas);
};

#endif // SHARPENFILTER_H
