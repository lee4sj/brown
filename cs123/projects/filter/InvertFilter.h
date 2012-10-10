#ifndef INVERTFILTER_H
#define INVERTFILTER_H

#include "filter/Filter.h"

class InvertFilter : public Filter
{
public:
    InvertFilter();
    virtual ~InvertFilter();
    virtual void applyFilter(Canvas2D *canvas);
};

#endif // INVERTFILTER_H
