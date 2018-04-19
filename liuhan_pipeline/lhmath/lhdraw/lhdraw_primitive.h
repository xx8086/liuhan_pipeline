#pragma once
//#include <functional>
#include "../lhdetail/lhvertex.h"

struct lh_color {
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    unsigned char alph = 0;
    lh_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255):
    red(r), green(g), blue(b){}

    template <typename T>
    lh_color(T rgba) {
        alph = rgba;
        blue = rgba >> 8;
        green = rgba >> 16;
        red = rgba >> 24;
    }

    template <typename T>
    T get_t() {
        return T(alph | blue << 8 | green << 16 | red << 24);
    }

    lh_color& operator+=(const lh_color &v){
        alph += v.alph;
        red += v.red;
        green += v.green;
        blue += v.blue;
        return *this;
    }

    lh_color operator+ (const lh_color &v) {
        return lh_color(red + v.red,
            green + v.green,
            blue + v.blue,
            alph + v.alph);
    }

    lh_color operator- (const lh_color &v) {
        return lh_color(red - v.red,
            green - v.green,
            blue - v.blue,
            alph - v.alph);
    }

    lh_color operator+ (float v) {
        return lh_color(red + v,
            green + v,
            blue + v,
            alph + v);
    }

    lh_color operator* (float v) {
        return lh_color(red * v,
            green * v,
            blue * v,
            alph * v);
    }

    lh_color operator/ (float v) {
        return lh_color(red / v,
            green / v,
            blue / v,
            alph / v);
    }
};

struct VertexColor {
    LhVertex<float, 3> postion;
    lh_color color;
    VertexColor operator= (const VertexColor &v) {
        postion = v.postion;
        color = v.color;
        return *this;
    }
    VertexColor():color(0){}
    VertexColor(LhVertex<float, 3> v, lh_color c):
        postion(v), color(c){};

    VertexColor& operator+=(const VertexColor &v) {
        postion = postion + v.postion;
        color += v.color;
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
    void draw_line(int x1, int y1, int x2, int y2, lh_color c);
    void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
    void draw_triangle(VertexColor v1, VertexColor v2, VertexColor v3);
private:
    template <typename T> 
    void swap_vaue(T& a, T& b); 
    void setpixel(float x, float y, lh_color color);
    void setpixel(int x, int y, lh_color color);//trapezoid
    void draw_interp_scanline(VertexColor left, VertexColor right);
    void set_clip_window(float x_min, float y_min, float x_max, float y_max);
private:
    void line_dda(int x1, int y1, int x2, int y2, lh_color color);
    void line_bresenham(float x1, float y1, float x2, float y2, lh_color color);
    void line(int x1, int y1, int x2, int y2, lh_color c);
    void top_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
    void bottom_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
    void top_triangle(VertexColor v1, VertexColor v2, VertexColor v3);
    void bottom_triangle(VertexColor v1, VertexColor v2, VertexColor v3);

private:
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
    float _x_min_clip = 0;
    float _y_min_clip = 0;
    float _x_max_clip = 0;
    float _y_max_clip = 0;
};

