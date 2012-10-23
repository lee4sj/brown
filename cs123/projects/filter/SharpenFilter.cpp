
#include "EdgeDetectFilter.h"
#include "filter/SharpenFilter.h"

SharpenFilter::SharpenFilter() {}

SharpenFilter::~SharpenFilter() {}

void SharpenFilter::applyFilter(Canvas2D *canvas)
{
    BGRA *im = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();

    if (!checkBoundary(start, end, width, height))
        return;

    int xSize = end.x() - start.x();
    int ySize = end.y() - start.y();

    BGRA *convIm = new BGRA[xSize * ySize];
    int r, g, b;

    for (int y = 1; y < ySize - 1; y++) {
        r = 0, g = 0, b = 0;
        for (int x = 1; x < xSize - 1; x++) {
            r = -im[start.x() + (x - 1) + width * (start.y() + (y + 1))].r
                -im[start.x() + x + width * (start.y() + (y + 1))].r
                -im[start.x() + (x + 1) + width * (start.y() + (y + 1))].r
                -im[start.x() + (x - 1) + width * (start.y() + y)].r
                + 9 * im[start.x() + x + width * (start.y() + y)].r
                -im[start.x() + (x + 1) + width * (start.y() + y)].r
                -im[start.x() + (x - 1) + width * (start.y() + (y - 1))].r
                -im[start.x() + x + width * (start.y() + (y - 1))].r
                -im[start.x() + (x + 1) + width * (start.y() + (y - 1))].r;
            g = -im[start.x() + (x - 1) + width * (start.y() + (y + 1))].g
                -im[start.x() + x + width * (start.y() + (y + 1))].g
                -im[start.x() + (x + 1) + width * (start.y() + (y + 1))].g
                -im[start.x() + (x - 1) + width * (start.y() + y)].g
                + 9 * im[start.x() + x + width * (start.y() + y)].g
                -im[start.x() + (x + 1) + width * (start.y() + y)].g
                -im[start.x() + (x - 1) + width * (start.y() + (y - 1))].g
                -im[start.x() + x + width * (start.y() + (y - 1))].g
                -im[start.x() + (x + 1) + width * (start.y() + (y - 1))].g;
            b = -im[start.x() + (x - 1) + width * (start.y() + (y + 1))].b
                -im[start.x() + x + width * (start.y() + (y + 1))].b
                -im[start.x() + (x + 1) + width * (start.y() + (y + 1))].b
                -im[start.x() + (x - 1) + width * (start.y() + y)].b
                + 9 * im[start.x() + x + width * (start.y() + y)].b
                -im[start.x() + (x + 1) + width * (start.y() + y)].b
                -im[start.x() + (x - 1) + width * (start.y() + (y - 1))].b
                -im[start.x() + x + width * (start.y() + (y - 1))].b
                -im[start.x() + (x + 1) + width * (start.y() + (y - 1))].b;

            convIm[x + xSize * y].r = max(0, min(255, r));
            convIm[x + xSize * y].g = max(0, min(255, g));
            convIm[x + xSize * y].b = max(0, min(255, b));
        }
    }


    for (int y = 0; y < ySize; y++) {
        memcpy(&im[width * (start.y() + y)], &convIm[xSize * y], xSize * sizeof(BGRA));
    }

    delete[] convIm;
}
