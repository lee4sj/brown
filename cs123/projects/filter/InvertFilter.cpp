#include "InvertFilter.h"

InvertFilter::InvertFilter() {}

InvertFilter::~InvertFilter() {}

void InvertFilter::applyFilter(Canvas2D *canvas)
{
    BGRA *image = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();

    if (!checkBoundary(start, end, width, height))
        return;

    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y(); y < end.y(); y++) {
            // invert rgb channels, but keep alpha channel as it is.
            image[x + width*y].r = (unsigned char)(-1) - image[x + width*y].r;
            image[x + width*y].g = (unsigned char)(-1) - image[x + width*y].g;
            image[x + width*y].b = (unsigned char)(-1) - image[x + width*y].b;
        }
    }
}

