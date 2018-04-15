#pragma once
//#include <functional>

class LhDrawPrimitive
{
public:
    LhDrawPrimitive();
    ~LhDrawPrimitive();
public:
    //std::function<void(int, int, int)> setpixel;
    void set_buffer(int w, int h, void* pbits);
    void line_dda(int x1, int y1, int x2, int y2, int color);
    void line_lh(int x1, int y1, int x2, int y2, unsigned int c);
private:
    void LhDrawPrimitive::setpixel(int x, int y, int color);
private:
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
};

