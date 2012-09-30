/*!
   @file   ConstantBrush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a brush with a constant mask distribution.
   You should fill this file in while completing the Brush assignment.

*/

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(BGRA color, int flow, int radius) : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.
    if (m_mask != NULL)
        delete[] m_mask;
}

void ConstantBrush::makeMask()
{
    //allocate appropriate memory
    m_mask = new float[(m_radius * 2 + 1) * (m_radius * 2 + 1)];
    int x_size = m_radius * 2 + 1;
    int radius_sq = m_radius * m_radius;

    //initialize
    for (int x = -m_radius; x <= m_radius; x++) {
        for (int y = -m_radius; y <= m_radius; y++) {
            //change coordinate relative to the center
            if (x*x + y*y <= radius_sq)
                m_mask[(x + m_radius) + x_size * (y + m_radius)] = 1;
            else
                m_mask[(x + m_radius) + x_size * (y + m_radius)] = 0;
        }
    }
}


