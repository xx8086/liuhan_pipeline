#include "lhframe_buffer.h"
#include <cmath>
#include <assert.h>
#include <memory.h>

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
    assert(pbits);
    assert(w > 0);
    assert(h > 0);
    _width = w;
    _height = h;
    _frame_buffers = static_cast<unsigned char*>(pbits);
}
void LhFrameBuffer::update() {
}

void LhFrameBuffer::update_triangle_vertex(float* vertex_buffer, /*float* color_buffer,*/ int triangle_counts) {
    assert(vertex_buffer);
    //assert(color_buffer);
    assert(triangle_counts >= 0);

    if (_triangle_counts < triangle_counts) {
        if (nullptr != _vertex_buffers) {
            delete[] _vertex_buffers;
            _vertex_buffers = nullptr;
        }
        /*if (nullptr != _vertex_color_buffers) {
            delete[] _vertex_color_buffers;
            _vertex_color_buffers = nullptr;
        }*/
    }

    if (nullptr == _vertex_buffers) {
        _vertex_buffers = new float[9 * triangle_counts];//三个float确定一个顶点，三个顶点确定一个三角形
    }
    /*if (nullptr == _vertex_color_buffers) {
        _vertex_color_buffers = new float[3 * triangle_counts];
    }*/

    memcpy(_vertex_buffers, vertex_buffer, 9 * triangle_counts * sizeof(float));
    //memcpy(_vertex_color_buffers, color_buffer, 3 * triangle_counts);
    _triangle_counts = triangle_counts;
}