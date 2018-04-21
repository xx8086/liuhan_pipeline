﻿#pragma once

class LhFrameBuffer
{
public:
    LhFrameBuffer();
    ~LhFrameBuffer();
protected:
    void set_buffer(int w, int h, void* pbits);
    void update_vertex(const float* vertex_buffer, const unsigned int* vertex_buffer_color, const float* vertex_uv, const int counts);
    void clear_buffer();
    const float* ger_current_uv();
    const float* get_vertex_buffers() { return _vertex_buffers; }
    const unsigned int*  get_vertex_color_buffers() { return _vertex_color_buffers; }
    unsigned char * get_frame_buffers();
    unsigned char * ger_current_texutre_uv_buffers();
    int get_width() const;
    int get_height() const;

    const int get_current_texture_uv() { return _current_texture_level; }
    const int get_current_texture_uv_size() { return _current_uv_size[_current_texture_level]; }
    const int get_vertex_buffers_size() { return _vertex_buffers_size; }
protected:
    typedef enum TextureLevel {
        TEXTURE_LEVEL_0 = 0,//56
        TEXTURE_LEVEL_1 = 1,//128
        TEXTURE_LEVEL_2 = 2,//256
        TEXTURE_LEVEL_3 = 3,
        TEXTURE_LEVEL_4 = 4,
        TEXTURE_LEVEL_MAX,
    }TEXTURE_LEVEL;
    void set_texture(unsigned char* texture_datas, int texture_size);
private:
    int texture_size_check(int texture_size);
    void release();
private:
    int _width = 0;
    int _height = 0;
    unsigned char* _frame_buffers = nullptr;
    unsigned int* _vertex_color_buffers = nullptr;
    float* _vertex_buffers = nullptr;
    int _vertex_buffers_size = 0;
    unsigned char* _texture[TEXTURE_LEVEL_MAX] = { nullptr };
    float* _uv[TEXTURE_LEVEL_MAX] = { nullptr };
    int _texture_size[TEXTURE_LEVEL_MAX] = { 0 };
    int _current_texture_level = TEXTURE_LEVEL_MAX;
    int _current_uv_size[TEXTURE_LEVEL_MAX] = { 56, 128, 256, 512, 1024 };
};

