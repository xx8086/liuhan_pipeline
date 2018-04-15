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

void LhDevice::draw(int x1, int x2, int y1, int y2, int color) {
    line_dda(x1, x2, y1, y2, color);
}
