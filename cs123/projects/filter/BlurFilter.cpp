#include "BlurFilter.h"

BlurFilter::BlurFilter() {}

BlurFilter::~BlurFilter() {}

void BlurFilter::getGaussian(double *kernel, int r)
{
    double sigma = r / 3.0;
    double total = 1;

    for (int i = 1; i <= r; i++) {
        kernel[r+i] = 1 / exp((i*i) / (2*sigma*sigma));
        total += 2 * kernel[r+i];
    }

    kernel[r] = 1/total;
    for (int i = 1; i <= r; i++) {
        kernel[r+i] /= total;
        kernel[r-i] = kernel[r+i];
    }
}

void BlurFilter::applyHorizontalKernel(BGRADouble *convIm, BGRA *image, int width,
                                       double *kernel, QPoint &start, QPoint &end, int radius)
{
    int convWidth = end.x() - start.x();
    int convHeight = end.y() - start.y();

    int yIm = start.y();
    int xIm;
    for (int y = 0; y < convHeight; y++) {
        xIm = start.x() + radius;
        //inner loop
        for (int x = radius; x < convWidth - radius; x++) {
            for (int i = -radius; i <= radius; i++) {
                convIm[x + convWidth * y].r += kernel[i + radius] * image[(xIm + i) + width * yIm].r / 255.0;
                convIm[x + convWidth * y].g += kernel[i + radius] * image[(xIm + i) + width * yIm].g / 255.0;
                convIm[x + convWidth * y].b += kernel[i + radius] * image[(xIm + i) + width * yIm].b / 255.0;
            }
            xIm++;
        }

        //edge cases: use mirror
        xIm = start.x();
        for (int x = 0; x < radius; x++) {
            for (int i = -x; i <= radius; i++) { //i = -radius + (radius - x)
                convIm[x + convWidth * y].r += kernel[i + radius] * image[(xIm + i) + width * yIm].r / 255.0;
                convIm[x + convWidth * y].g += kernel[i + radius] * image[(xIm + i) + width * yIm].g / 255.0;
                convIm[x + convWidth * y].b += kernel[i + radius] * image[(xIm + i) + width * yIm].b / 255.0;
            }

            for (int i = -radius; i < -x; i++) {
                convIm[x + convWidth * y].r += kernel[i + radius] * image[(xIm - x - i) + width * yIm].r / 255.0;
                convIm[x + convWidth * y].g += kernel[i + radius] * image[(xIm - x - i) + width * yIm].g / 255.0;
                convIm[x + convWidth * y].b += kernel[i + radius] * image[(xIm - x - i) + width * yIm].b / 255.0;
            }
            xIm++;
        }
        xIm = end.x() - radius;
        for (int x = convWidth - radius; x < convWidth; x++) {
            for (int i = -radius; i < (convWidth - x); i++) { //i = -radius + (radius - x)
                convIm[x + convWidth * y].r += kernel[i + radius] * image[(xIm + i) + width * yIm].r / 255.0;
                convIm[x + convWidth * y].g += kernel[i + radius] * image[(xIm + i) + width * yIm].g / 255.0;
                convIm[x + convWidth * y].b += kernel[i + radius] * image[(xIm + i) + width * yIm].b / 255.0;
            }

            for (int i = (convWidth - x); i <= radius; i++) {
                int index = radius - i + 1;
                convIm[x + convWidth * y].r += kernel[i + radius] * image[(start.x() + convWidth - 1 - index) + width * yIm].r / 255.0;
                convIm[x + convWidth * y].g += kernel[i + radius] * image[(start.x() + convWidth - 1 - index) + width * yIm].g / 255.0;
                convIm[x + convWidth * y].b += kernel[i + radius] * image[(start.x() + convWidth - 1 - index) + width * yIm].b / 255.0;
            }
            xIm++;
        }
        yIm++;
    }
}

void BlurFilter::applyVerticalKernel(BGRA *image, BGRADouble *convIm, int width,
                                     double *kernel, QPoint &start, QPoint &end, int radius)
{
    int convWidth = end.x() - start.x();
    int convHeight = end.y() - start.y();

    int yIm;
    int xIm = start.x();
    double r, g, b;
    for (int x = 0; x < convWidth; x++) {
        yIm = start.y() + radius;
        for (int y = radius; y < convHeight - radius; y++) {
            r = 0, g = 0, b = 0;
            for (int i = -radius; i <= radius; i++) {
                r += kernel[i + radius] * convIm[x + convWidth * (y + i)].r;
                g += kernel[i + radius] * convIm[x + convWidth * (y + i)].g;
                b += kernel[i + radius] * convIm[x + convWidth * (y + i)].b;
            }
            image[xIm + width * yIm].r = fmin(1, fmax(r, 0)) * 255;
            image[xIm + width * yIm].g = fmin(1, fmax(g, 0)) * 255;
            image[xIm + width * yIm].b = fmin(1, fmax(b, 0)) * 255;
            yIm++;
        }

        //edge cases: use mirror
        yIm = start.y();
        for (int y = 0; y < radius; y++) {
            r = 0, g = 0, b = 0;
            for (int i = -y; i <= radius; i++) { //i = -radius + (radius - x)
                r += kernel[i + radius] * convIm[x + convWidth * (y + i)].r;
                g += kernel[i + radius] * convIm[x + convWidth * (y + i)].g;
                b += kernel[i + radius] * convIm[x + convWidth * (y + i)].b;
            }


            for (int i = -radius; i < -y; i++) {
                r += kernel[i + radius] * convIm[x + convWidth * (-y - i)].r;
                g += kernel[i + radius] * convIm[x + convWidth * (-y - i)].g;
                b += kernel[i + radius] * convIm[x + convWidth * (-y - i)].b;
            }

            image[xIm + width * yIm].r = fmin(1, fmax(r, 0)) * 255;
            image[xIm + width * yIm].g = fmin(1, fmax(g, 0)) * 255;
            image[xIm + width * yIm].b = fmin(1, fmax(b, 0)) * 255;
            yIm++;
        }
        yIm = end.y() - radius;
        for (int y = convHeight - radius; y < convHeight; y++) {
            r = 0, g = 0, b = 0;
            for (int i = -radius; i < (convHeight - y); i++) { //i = -radius + (radius - x)
                r += kernel[i + radius] * convIm[x + convWidth * (y + i)].r;
                g += kernel[i + radius] * convIm[x + convWidth * (y + i)].g;
                b += kernel[i + radius] * convIm[x + convWidth * (y + i)].b;
            }

            for (int i = (convHeight - y); i <= radius; i++) {
                int index = radius - i + 1;
                r += kernel[i + radius] * convIm[x + convWidth * (convHeight - index)].r;
                g += kernel[i + radius] * convIm[x + convWidth * (convHeight - index)].g;
                b += kernel[i + radius] * convIm[x + convWidth * (convHeight - index)].b;
            }

            image[xIm + width * yIm].r = fmin(1, fmax(r, 0)) * 255;
            image[xIm + width * yIm].g = fmin(1, fmax(g, 0)) * 255;
            image[xIm + width * yIm].b = fmin(1, fmax(b, 0)) * 255;
            yIm++;
        }

        xIm++;
    }
}

void BlurFilter::applyFilter(Canvas2D *canvas, int radius)
{
    BGRA *image = canvas->data();
    QPoint start = canvas->marqueeStart();
    QPoint end = canvas->marqueeStop();
    int width = canvas->width();
    int height = canvas->height();

    if (!checkBoundary(start, end, width, height))
        return;

    int xSize = end.x() - start.x();
    int ySize = end.y() - start.y();

    BGRADouble *convIm = new BGRADouble[xSize * ySize];
    bzero(convIm, xSize * ySize * sizeof(BGRADouble));

    //horizontal: check radius and get kernel and apply
    if ((2*radius + 1) > xSize)
        radius = (xSize - 1)/2;

    double *kernel = new double[(2*radius + 1)];
    getGaussian(kernel, radius);

    applyHorizontalKernel(convIm, image, width, kernel, start, end, radius);

    //vertical: check radius and get kernel and apply
    if ((2*radius + 1) > ySize) {
        //if width radius != height radius
        if (radius != (height - 1) / 2) {
            delete[] kernel;
            kernel = new double[height];
            getGaussian(kernel, (height - 1) / 2);
        }
        radius = (ySize - 1) / 2;
    }

    for (int y = start.y(); y < end.y(); y++)
        memset(&image[start.x() + width*y], 0, xSize * sizeof(BGRA));

    applyVerticalKernel(image, convIm, width, kernel, start, end, radius);

    delete[] kernel;
    delete[] convIm;
}
