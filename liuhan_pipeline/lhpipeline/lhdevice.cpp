#include "lhdevice.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h >

LhDevice::LhDevice()
{
}


LhDevice::~LhDevice()
{
}

void LhDevice::update_buffer(int w, int h, void* pbits) {
    LhDrawPrimitive::set_buffer(w, h, pbits);
    LhFrameBuffer::set_buffer(w, h, pbits);
}

void LhDevice::draw(int x1, int y1, int x2, int y2, int color) {
    line_bresenham(x1, y1, x2, y2, lh_color(255,0, 0));
    line_dda(x1 + 2, y1, x2, y2 - 2, lh_color(0, 0, 255));
}
