#pragma once
//#include <functional>

struct lh_color {
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;
    unsigned char alph = 0;
    lh_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255):
    red(r), green(g), blue(b){}
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
    void line_lh(int x1, int y1, int x2, int y2, lh_color c);
private:
    template <typename T>
    void swap_vaue(T& a, T& b);
    void setpixel(int x, int y, lh_color color);
private:
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
};

