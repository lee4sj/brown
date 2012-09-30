/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     Implementation of a brush that smudges the image on the canvas as
   the mouse is dragged. You should fill this file in for the Brush
   assignment.

*/

#include <iostream>
#include <CS123Common.h>

#include "SmudgeBrush.h"
#include "Canvas2D.h"

using namespace std;


SmudgeBrush::SmudgeBrush(BGRA color, int flow, int radius) : QuadraticBrush(color, flow, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.


    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!
    if (temp_buffer)
        delete[] temp_buffer;
}

//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

    temp_buffer = new BGRA[(m_radius * 2 + 1) * (m_radius * 2 + 1)];
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int _x, int _y, Canvas2D* canvas)
{
    int width = canvas->width();
    int height = canvas->height();
    int x_size = m_radius * 2 + 1;
    BGRA *im = canvas->data();

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    for (int x = -m_radius; x <= m_radius; x++) {
        for (int y = -m_radius; y <= m_radius; y++) {
            int im_x = _x + x;
            int im_y = _y + y;
            if ((0 <= im_x && im_x < width) && (0 <= im_y && im_y < height)) {
                temp_buffer[(x + m_radius) + x_size * (y + m_radius)].r = im[im_x + height * im_y].r;
                temp_buffer[(x + m_radius) + x_size * (y + m_radius)].g = im[im_x + height * im_y].g;
                temp_buffer[(x + m_radius) + x_size * (y + m_radius)].b = im[im_x + height * im_y].b;
            }
        }
    }
}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    BGRA *im = canvas->data();
    int width = canvas->width();
    int height = canvas->height();
    int x_size = m_radius * 2 + 1;
    float alpha = (float)m_flow/255;

    for (int x = -m_radius; x <= m_radius; x++) {
        for (int y = -m_radius; y <= m_radius; y++) {
            int im_x = mouse_x + x;
            int im_y = mouse_y + y;
            if ((0 <= im_x && im_x < width) && (0 <= im_y && im_y < height) &&
                    m_mask[(x + m_radius) + x_size * (y + m_radius)] != 0) {
                im[im_x + width * im_y].r = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                      alpha,
                                                      temp_buffer[(x + m_radius) + x_size * (y + m_radius)].r / (float)255,
                                                      im[im_x + width * im_y].r / (float)255);
                im[im_x + width * im_y].g = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                      alpha,
                                                      temp_buffer[(x + m_radius) + x_size * (y + m_radius)].g / (float)255,
                                                      im[im_x + width * im_y].g / (float)255);
                im[im_x + width * im_y].b = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                      alpha,
                                                      temp_buffer[(x + m_radius) + x_size * (y + m_radius)].b / (float)255,
                                                      im[im_x + width * im_y].b / (float)255);
                im[im_x + width * im_y].a = 255;
            }
        }
    }

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);
}
