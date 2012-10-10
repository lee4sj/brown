#include "GreyscaleFilter.h"

GreyscaleFilter::GreyscaleFilter() {}

GreyscaleFilter::~GreyscaleFilter() {}

void GreyscaleFilter::applyFilter(Canvas2D *canvas)
{
    BGRA *image = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();
    unsigned char val = 0;

    if (!checkBoundary(start, end, width, height))
        return;

    for (int x = start.x(); x < end.x(); x++) {
        for (int y = start.y(); y < end.y(); y++) {
            // invert rgb channels, but keep alpha channel as it is.
            val = (unsigned char)(0.299 * image[x + width*y].r +
                                  0.587 * image[x + width*y].g +
                                  0.114 * image[x + width*y].b);

            image[x + width*y].r = val;
            image[x + width*y].g = val;
            image[x + width*y].b = val;
        }
    }
}


