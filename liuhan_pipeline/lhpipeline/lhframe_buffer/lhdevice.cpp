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

void LhDevice::init(int width, int height, void *fb) {
    int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
    char *ptr = (char*)malloc(need + 64);
    char *framebuf = nullptr ;
    char *zbuf = nullptr;
    int j = 0;
    assert(ptr);
    _framebuffer = (unsigned int**)ptr;
    _zbuffer = (float**)(ptr + sizeof(void*) * height);
    ptr += sizeof(void*) * height * 2;
    _texture = (unsigned int**)ptr;
    ptr += sizeof(void*) * 1024;
    framebuf = (char*)ptr;
    zbuf = (char*)ptr + width * height * 4;
    ptr += width * height * 8;
    if (fb != NULL) framebuf = (char*)fb;
    for (j = 0; j < height; j++) {
        _framebuffer[j] = (unsigned int*)(framebuf + width * 4 * j);
        _zbuffer[j] = (float*)(zbuf + width * 4 * j);
    }
    _texture[0] = (unsigned int*)ptr;
    _texture[1] = (unsigned int*)(ptr + 16);
    memset(_texture[0], 0, 64);
    _tex_width = 2;
    _tex_height = 2;
    _max_u = 1.0f;
    _max_v = 1.0f;
    _width = width;
    _height = height;
    _background = 0xc0c0c0;
    _foreground = 0;
    //transform_init(&transform, width, height);
    //_render_state = RENDER_STATE_WIREFRAME;
}

void LhDevice::release() {
    if (_framebuffer){
        free(_framebuffer);
    }
        
    _framebuffer = nullptr;
    _zbuffer = nullptr;
    _texture = nullptr;
}


// 设置当前纹理
void LhDevice::set_texture(void *bits, long pitch, int w, int h) {
    char *ptr = (char*)bits;
    int j;
    assert(w <= 1024 && h <= 1024);
    for (j = 0; j < h; j++) {// 重新计算每行纹理的指针
        _texture[j] = (unsigned int*)ptr;
        ptr += pitch;
    }
    _tex_width = w;
    _tex_height = h;
    _max_u = (float)(w - 1);
    _max_v = (float)(h - 1);
}
// 清空 framebuffer 和 zbuffer
void LhDevice::clear_frame_and_z_buffer(int mode) {
    int y = 0;
    int x = 0; 
    int height = _height;

    for (y = 0; y < _height; y++) {
        unsigned int *dst = _framebuffer[y];
        unsigned int cc = (height - 1 - y) * 230 / (height - 1);
        cc = (cc << 16) | (cc << 8) | cc;
        if (mode == 0) {
            cc = _background;
        }
        for (x = _width; x > 0;  x--) {
            dst[0] = cc;
            dst++;
        }
    }
    for (y = 0; y < _height; y++) {
        float *dst = _zbuffer[y];
        for (x = _width; x > 0; x--) {
            dst[0] = 0.0f;
            dst++;
        }
    }
}
// 画点
void LhDevice::draw_pixel( int x, int y, unsigned int color) {
    if (((unsigned int)x) < (unsigned int)_width && 
        ((unsigned int)y) < (unsigned int)_height) {
        _framebuffer[y][x] = color;
    }
}
// 绘制线段
void LhDevice::draw_line( int x1, int y1, int x2, int y2, unsigned int c) {
    int x, y, rem = 0;
    if (x1 == x2 && y1 == y2) {
        draw_pixel(x1, y1, c);
    }
    else if (x1 == x2) {
        int inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc) {
            draw_pixel(x1, y, c);
        }
        draw_pixel(x2, y2, c);
    }
    else if (y1 == y2) {
        int inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc) {
            draw_pixel(x, y1, c);
        }
        draw_pixel(x2, y2, c);
    }
    else {
        int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
        if (dx >= dy) {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; x <= x2; x++) {
                draw_pixel(x, y, c);
                rem += dy;
                if (rem >= dx) {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    draw_pixel(x, y, c);
                }
            }
            draw_pixel(x2, y2, c);
        }
        else {
            if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; y <= y2; y++) {
                draw_pixel(x, y, c);
                rem += dx;
                if (rem >= dy) {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    draw_pixel(x, y, c);
                }
            }
            draw_pixel(x2, y2, c);
        }
    }
}
// 根据坐标读取纹理