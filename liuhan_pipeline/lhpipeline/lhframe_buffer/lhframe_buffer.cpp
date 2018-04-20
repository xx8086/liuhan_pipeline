#include "lhframe_buffer.h"
#include <cmath>
#include <assert.h>
#include <memory.h>

LhFrameBuffer::LhFrameBuffer()
{
}

LhFrameBuffer::~LhFrameBuffer()
{
    if (nullptr != _frame_deep) {
        delete[] _frame_deep;
        _frame_deep = nullptr;
    }
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

    if (nullptr != _frame_deep) {
        delete[] _frame_deep;
        _frame_deep = nullptr;
    }

    _frame_deep = new float[w * h];
}

void LhFrameBuffer::clear_buffer() {
    if (nullptr != _frame_buffers) {
        memset(_frame_buffers, 0, _width * _height * sizeof(float));
    }
    
}

bool LhFrameBuffer::deep_test(int x, int y, float z) {
    bool r = false;
    if (*(_frame_deep + x * _width + y) < z) {
        *(_frame_deep + x * _width + y) = z;
        r = true;
    }
    return r;
}

void LhFrameBuffer::update_vertex(float* vertex_buffer, /*float* color_buffer,*/ int counts) {
    assert(vertex_buffer);
    //assert(color_buffer);
    assert(counts >= 0);

    if (_vertex_buffers_size < counts) {
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
        _vertex_buffers = new float[counts];
    }
    /*if (nullptr == _vertex_color_buffers) {
        _vertex_color_buffers = new float[counts];
    }*/

    memcpy(_vertex_buffers, vertex_buffer, counts * sizeof(float));
    //memcpy(_vertex_color_buffers, color_buffer, counts / 3);
    _vertex_buffers_size = counts;
}