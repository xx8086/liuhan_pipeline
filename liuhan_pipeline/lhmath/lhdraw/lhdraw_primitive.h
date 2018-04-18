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
    /*template <typename T>
    lh_color(T rgba) {
        alph = rgba;
        blue = rgba >> 8;
        green = rgba >> 16;
        red = rgba >> 24;
    }*/

    lh_color(int rgba) {
        alph = rgba;
        blue = rgba >> 8;
        green = rgba >> 16;
        red = rgba >> 24;
    }

    lh_color& operator+=(const lh_color &v){
        alph += v.alph;
        red += v.red;
        green += v.green;
        blue += v.blue;
        return *this;
    }

    lh_color operator- (const lh_color &v) {
        return lh_color(red - v.red,
            green - v.green,
            blue - v.blue,
            alph - v.alph);
    }

    lh_color operator* (float v) {
        return lh_color(red * v,
            green * v,
            blue * v,
            alph * v);
    }

    lh_color interp(const lh_color &c, float factor){
        float na = alph + (c.alph - alph)*factor;
        float nr = red + (c.red - red)*factor;
        float ng = green + (c.green - green)*factor;
        float nb = blue + (c.blue - blue)*factor;
        return lh_color(nr, ng, nb, na);
    }

    template <typename T>
    T get_t() {
        return T( alph | blue << 8 | green << 16 | red << 24);
    }

};

struct VertexColor {
    LhVertex<float, 3> postion;
    lh_color color;
    VertexColor(LhVertex<float, 3> v, lh_color c):
        postion(v), color(c){};
};

class LhDrawPrimitive
{
public:
    LhDrawPrimitive();
    ~LhDrawPrimitive();
public:
    //std::function<void(int, int, int)> setpixel;
    void set_buffer(int w, int h, void* pbits);
    void line_dda(int x1, int y1, int x2, int y2, lh_color color);
    void line_bresenham(int x1, int y1, int x2, int y2, lh_color color);
    void line(int x1, int y1, int x2, int y2, lh_color c);
public:
    void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
private:
    template <typename T>
    void swap_vaue(T& a, T& b);
    void setpixel(int x, int y, lh_color color);//trapezoid
    
    VertexColor interp(const VertexColor& from, const VertexColor& to, float factor);

private:
    void left_top_fill();
    void top_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
    void bottom_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color);
    
private:
    void scanline(float left, float right, float y, lh_color left_color,  lh_color right_color);
private:
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
};

