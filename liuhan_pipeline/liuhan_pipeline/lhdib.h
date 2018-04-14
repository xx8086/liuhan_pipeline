#pragma once
#include "..\lhpipeline\lhframe_buffer\lhframe_buffer.h"

class LhDib : protected LhFrameBuffer
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
    bool load_dib_texture(TCHAR* img);
private:
    HDC _frame_dc = nullptr;
    HBITMAP _dib = nullptr;
    HBITMAP _old_bitmap = nullptr;
};

