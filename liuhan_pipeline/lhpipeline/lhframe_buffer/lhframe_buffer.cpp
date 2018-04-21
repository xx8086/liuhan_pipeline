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

    for (int i = TEXTURE_LEVEL_0; i < TEXTURE_LEVEL_MAX; i++) {
        _texture_size[i] = 0;
        if (nullptr != _texture[i]) {
            delete[] _texture[i];
            _texture[i] = nullptr;
        }
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

void LhFrameBuffer::update_vertex(const float* vertex_buffer, const unsigned int* vertex_buffer_color, const float* vertex_uv, const int counts) {
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
        _vertex_buffers = new float[3 * counts];
    }
    if (nullptr == _vertex_color_buffers) {
        _vertex_color_buffers = new unsigned int[3 * counts];
    }

    memcpy(_vertex_buffers, vertex_buffer, 3 * counts * sizeof(float));
    memcpy(_vertex_color_buffers, vertex_buffer_color, 3 * counts * sizeof(unsigned int));
    _vertex_buffers_size = counts;

    if (_current_texture_level >= TEXTURE_LEVEL_0&&
        TEXTURE_LEVEL_MAX > _current_texture_level) {
        if (nullptr != _uv[_current_texture_level]) {
            delete[] _uv[_current_texture_level];
        }
        _uv[_current_texture_level] = new float[2 * counts];
        memcpy(_uv[_current_texture_level], vertex_uv, 2 * counts * sizeof(float));
    }
}

int LhFrameBuffer::texture_size_check(int texture_size) {
    int level = TEXTURE_LEVEL_MAX;
    switch (texture_size) {
    case 56:
        level = TEXTURE_LEVEL_0;
        break;
    case 128:
        level = TEXTURE_LEVEL_1;
        break;
    case 256:
        level = TEXTURE_LEVEL_2;
        break;
    case 512:
        level = TEXTURE_LEVEL_3;
        break;
    case 1024:
        level = TEXTURE_LEVEL_4;
        break;
    default:
        break;
    }
    return level;
}
void LhFrameBuffer::set_texture(unsigned char* texture_datas, int texture_size) {
    int level = texture_size_check(texture_size);
    assert(level >= TEXTURE_LEVEL_0 && TEXTURE_LEVEL_MAX > level);
    assert(nullptr != texture_datas);
    if (nullptr != _texture[level]) {
        delete[] _texture[level];
        _texture[level] = nullptr;
    }

    _texture[level] = texture_datas;
    _texture_size[level] = texture_size;
    _current_texture_level = level;
}

const float* LhFrameBuffer::ger_current_uv() {
    return _uv[_current_texture_level];
}

unsigned char * LhFrameBuffer::ger_current_texutre_uv_buffers() {
    return _texture[_current_texture_level];
}