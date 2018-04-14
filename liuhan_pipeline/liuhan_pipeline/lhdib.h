#pragma once

class LhDib
{
public:
    LhDib();
    ~LhDib();
public:
    void init(HWND hwnd, int w, int h);
    void bitblt(HWND hwnd);
    void destroy();
    void release();
private:
    void setpixel(int, int, unsigned int);
    void device_draw_line(int x1, int y1, int x2, int y2, unsigned int c);
    void load_dib_texture(TCHAR* img);
private:
    HDC _frame_dc = nullptr;
    HBITMAP _dib = nullptr;
    HBITMAP _old_bitmap = nullptr;
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
    unsigned int* _frame_rgba = nullptr;
};

