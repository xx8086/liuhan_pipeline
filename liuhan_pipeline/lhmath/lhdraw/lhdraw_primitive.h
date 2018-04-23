#pragma once
//#include <functional>
#include "../lhdetail/lhvertex.h"
#include "../lhmath_common.h"
#include "lhlights.h"

namespace lh_pipeline {

    struct TextureUV
    {
        TextureUV() {}
        TextureUV(float _u, float _v) :u(_u), v(_v) {}
        float u = 0;
        float v = 0;

        TextureUV operator*(const float& other) {
            return TextureUV(u * other, v * other);
        }
        TextureUV operator-(const TextureUV &otherv) {
            return TextureUV(u - otherv.u, v - otherv.v);
        }
        TextureUV operator+(const TextureUV &otherv) {
            return TextureUV(u + otherv.u, v + otherv.v);
        }
        /*                           1
    0.----------------.-----> +u
    |                         |
    |                         |
    |                         |
    |                         |
    1 |.----------------(texure_size, texure_size)
    \/
    +v   */
    };

    struct VertexColor {
        LhVertex<float, 3> postion;
        lh_color color;
        TextureUV uv;
        VertexColor operator= (const VertexColor &v) {
            postion = v.postion;
            color = v.color;
            uv = v.uv;
            return *this;
        }
        VertexColor() :color(0) {}
        VertexColor(LhVertex<float, 3> v, lh_color c, TextureUV texture = TextureUV()) :
            postion(v), color(c), uv(texture) {};
        VertexColor(LhVertex<float, 3> v, TextureUV texture = TextureUV()) :
            postion(v), uv(texture) {};

        VertexColor& operator+=(const VertexColor &v) {
            postion = postion + v.postion;
            color += v.color;
            uv.u += v.uv.u;
            uv.v += v.uv.v;
            return *this;
        }
    };

    class LhDrawPrimitive
    {
    public:
        LhDrawPrimitive();
        ~LhDrawPrimitive();
    public:
        void set_buffer(int w, int h, void* pbits);
        void set_view(LhVertexFloat3 view);
        void draw_line(int x1, int y1, int x2, int y2, lh_color c);
        void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
        void draw_triangle_line(int x1, int y1, int x2, int y2, int x3, int y3, lh_color color);
        void draw_triangle(VertexColor v1, VertexColor v2, VertexColor v3, bool use_uv = false);
    protected:
        void clear_deep();
        void set_current_uv(unsigned char* uv, int  uv_size);
        void setpixel(int x, int y, lh_color color);
        void enablelight();
    private:
        template <typename T>
        void swap_vaue(T& a, T& b);
        bool deeptest(int x, int y, float z);
        void setpixel(float x, float y, lh_color color);
        void setpixtel(int x, int y, unsigned int);
        void draw_interp_scanline(VertexColor left, VertexColor right, LhVertexFloat3 normal);
        void draw_interp_texture_scanline(VertexColor left, VertexColor right, LhVertexFloat3 normal);
        void set_clip_window(float x_min, float y_min, float x_max, float y_max);
    private:
        void line_dda(int x1, int y1, int x2, int y2, lh_color color);
        void line_bresenham(float x1, float y1, float x2, float y2, lh_color color);
        void line(int x1, int y1, int x2, int y2, lh_color c);
        void top_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
        void bottom_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
        void top_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool = false);
        void bottom_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool = false);
        float window_to_view(float pos, float length);
    private:
        int _width = 0;
        int _height = 0;
        int _current_uv_size = -1;
        unsigned int *_current_uv_texture_datas = nullptr;
        unsigned char *_frame_buffers = nullptr;
        float* _frame_deep_buffers = nullptr;
        float _x_min_clip = 0;
        float _y_min_clip = 0;
        float _x_max_clip = 0;
        float _y_max_clip = 0;
        float _z_near_clip = 100.0;
        float _z_far_clip = -1.0;
        bool _deep_test = true;
        LhLights _light;
        LhVertexFloat3 _view;
    };
}
