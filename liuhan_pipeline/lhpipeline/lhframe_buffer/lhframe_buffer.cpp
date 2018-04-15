#include "lhframe_buffer.h"
#include <cmath>

LhFrameBuffer::LhFrameBuffer()
{
}

LhFrameBuffer::~LhFrameBuffer()
{
}

int LhFrameBuffer::get_width() const {
    return _width;
}
int LhFrameBuffer::get_height() const {
    return _height;
}
unsigned char * LhFrameBuffer::get_buffers() {
    return _frame_buffers;
}
void LhFrameBuffer::set_buffer(int w, int h,  void* pbits) {
    _width = w;
    _height = h;
    _frame_buffers = static_cast<unsigned char*>(pbits);
}
void LhFrameBuffer::update() {
     
}
