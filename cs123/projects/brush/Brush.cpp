/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of common functionality of bitmap brushes.
   You should fill this file in while completing the Brush assignment.

*/

#include "Brush.h"
#include "Canvas2D.h"

#include <iostream>

using namespace std;

unsigned char lerp(unsigned char a, unsigned char b, float percent)
{
    float fa = (float)a / 255.0f;
    float fb = (float)b / 255.0f;
    return (unsigned char)((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(BGRA color, int flow, int radius) {

    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //        need to store the mask in memory. This might (or might not) be a good place to
    //        allocate that memory.

    m_color = color;
    m_flow = flow;
    m_radius = radius;
    m_mask = NULL;
}


Brush::~Brush()
{
    // @TODO: [BRUSH] Don't forget to delete any memory you allocate. Use delete[] to delete
    //        a whole array. Otherwise you'll just delete the first element!
    //
    //        i.e. delete[] m_mask;
    //
}


void Brush::setGreen(int green)
{
    m_color.g = green;
}


void Brush::setRed(int red)
{
    m_color.r = red;
}


void Brush::setBlue(int blue)
{
    m_color.b = blue;
}


void Brush::setFlow(int flow)
{
    m_flow = flow;
    makeMask();
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
    makeMask();
}


void Brush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    float alpha = m_flow / (float)255;
    BGRA* pix = canvas->data();
    int x_size = (m_radius * 2 + 1);
    int width = canvas->width();
    int height = canvas->height();

    for (int x = -m_radius; x <= m_radius; x++) {
        for (int y = -m_radius; y <= m_radius; y++) {
            int im_x = mouse_x + x;
            int im_y = mouse_y + y;
            if ((0 <= im_x && im_x < width) && (0 <= im_y && im_y < height) &&
                    m_mask[(x + m_radius) + x_size * (y + m_radius)] != 0) {
                pix[im_x + width * im_y].r = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                        alpha, m_color.r / (float)255, pix[im_x + width * im_y].r/ (float)255);
                pix[im_x + width * im_y].g = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                        alpha, m_color.g / (float)255, pix[im_x + width * im_y].g / (float)255);
                pix[im_x + width * im_y].b = calcBlend(m_mask[(x + m_radius) + x_size * (y + m_radius)],
                                                        alpha, m_color.b / (float)255, pix[im_x + width * im_y].b / (float)255);
                pix[im_x + width * im_y].a = 255;
            }
        }
    }
}

unsigned char Brush::calcBlend(float mask, float alpha, float brush, float current)
{
    return round(255 * ((mask * alpha * brush) + ((1 - mask * alpha) * current)));
}


