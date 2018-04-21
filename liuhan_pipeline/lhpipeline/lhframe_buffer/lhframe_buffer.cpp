#include "lhframe_buffer.h"
#include <cmath>
#include <assert.h>
#include <memory.h>

LhFrameBuffer::LhFrameBuffer()
{
}

LhFrameBuffer::~LhFrameBuffer()
{
    release();
}
void LhFrameBuffer::release() {
    _vertex_buffers_size = 0;
    if (nullptr != _vertex_color_buffers) {
        delete[] _vertex_color_buffers;
        _vertex_color_buffers = nullptr;
    }
    if (nullptr != _vertex_buffers) {
        delete[] _vertex_buffers;
        _vertex_buffers = nullptr;
    }
}
int LhFrameBuffer::get_width() const {
    return _width;
}
int LhFrameBuffer::get_height() const {
    return _height;
}
unsigned char * LhFrameBuffer::get_frame_buffers() {
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

void LhFrameBuffer::clear_buffer() {
    if (nullptr != _frame_buffers) {
        memset(_frame_buffers, 0, _width * _height * sizeof(float));
    }
}

void LhFrameBuffer::update_vertex(const float* vertex_buffer, const unsigned int* vertex_buffer_color, const int counts) {
    assert(nullptr != vertex_buffer);
    assert(counts >= 0);

    if (_vertex_buffers_size < counts) {
        if (nullptr != _vertex_buffers) {
            delete[] _vertex_buffers;
            _vertex_buffers = nullptr;
        }

        if (nullptr != _vertex_color_buffers) {
            delete[] _vertex_color_buffers;
            _vertex_color_buffers = nullptr;
        }
    }

    if (nullptr == _vertex_buffers) {
        _vertex_buffers = new float[counts];
    }
    if (nullptr == _vertex_color_buffers) {
        _vertex_color_buffers = new unsigned int[counts];
    }

    memcpy(_vertex_buffers, vertex_buffer, counts * sizeof(float));
    memcpy(_vertex_color_buffers, vertex_buffer_color, counts * sizeof(unsigned int));
    _vertex_buffers_size = counts;
}