#include "stdafx.h"
#include "lhdib.h"


LhDib::LhDib()
{
}


LhDib::~LhDib()
{
}
void LhDib::setpixel(int y, int x, unsigned int rgba) {
            _frame_buffers[(x*_width + y + 1) * 4 + 0] = 0;//b
            _frame_buffers[(x*_width + y + 1) * 4 + 1] = 0;//g
            _frame_buffers[(x*_width + y + 1) * 4 + 2] = 255;//r
            _frame_buffers[(x*_width + y + 1) * 4 + 3] = 255;//a
}

// 绘制线段
void LhDib::device_draw_line(int x1, int y1, int x2, int y2, unsigned int c) {
    int x, y, rem = 0;
    if (x1 == x2 && y1 == y2) {
        setpixel(x1, y1, c);
    }
    else if (x1 == x2) {
        int inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc) setpixel( x1, y, c);
        setpixel( x2, y2, c);
    }
    else if (y1 == y2) {
        int inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc) setpixel( x, y1, c);
        setpixel( x2, y2, c);
    }
    else {
        int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
        if (dx >= dy) {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; x <= x2; x++) {
                setpixel( x, y, c);
                rem += dy;
                if (rem >= dx) {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    setpixel( x, y, c);
                }
            }
            setpixel( x2, y2, c);
        }
        else {
            if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; y <= y2; y++) {
                setpixel( x, y, c);
                rem += dx;
                if (rem >= dy) {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    setpixel( x, y, c);
                }
            }
            setpixel( x2, y2, c);
        }
    }
}
void LhDib::init(HWND hwnd, int w, int h) {
    _width = w;
    _height = h;
    HDC hdc = GetDC(hwnd);

    _frame_dc = CreateCompatibleDC(hdc);
#if 0
    load_dib_texture("D:\\liuhan.dib");
#else
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
        w * h * 4, 0, 0, 0, 0 } };
    void* ptr;
    _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (_dib == nullptr) return;
    _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);
    _frame_buffers = (unsigned char*)ptr;

    device_draw_line(0, 0, _width - 1, _height - 1, 1);
#endif
    ReleaseDC(hwnd, hdc);
}

void LhDib::bitblt(HWND hwnd) {
    HDC hdc = GetDC(hwnd);
    ::BitBlt(hdc, 0, 0, _width, _height, _frame_dc, 0, 0, SRCCOPY);
    ReleaseDC(hwnd, hdc);
}

void LhDib::destroy() {
    if (nullptr != _frame_dc) {
        if (nullptr != _old_bitmap) {
            SelectObject(_frame_dc, _old_bitmap);
            _old_bitmap = nullptr;
        }

        DeleteDC(_frame_dc);
        _frame_dc = nullptr;
    }

    if (nullptr != _dib) {
        DeleteObject(_dib);
        _dib = nullptr;
    }
}

void LhDib::release() {
    ;
}

void LhDib::load_dib_texture(TCHAR* img) {
    void* ptr;
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), _width, _height, 1, 24, BI_RGB,
        _width * _height * 3, 0, 0, 0, 0 } };
    _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (_dib == nullptr) return;
    _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);
    _frame_buffers = (unsigned char*)ptr;

/*
    ------------------------
    | BITMAPFILEHEADER |
    ------------------------   <-----sizeof(BITMAPFILEHEADER)
    | BITMAPINFO            |  <== BITMAPINFOHEADER + RGBQUAD
    ------------------------   <-----BITMAPFILEHEADER::bfOffBits
    | buffer_datas           |
    ------------------------   <-----BITMAPFILEHEADER::bfSize
*/
    BITMAPFILEHEADER            bmfh;
    BITMAPINFO                      *pbmi;
    BOOL                                    bSuccess;
    DWORD                               dwInfoSize;
    DWORD                               dwBytesRead;
    HANDLE                              hFile;

    hFile = CreateFile(img, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (INVALID_HANDLE_VALUE == hFile){
        return ;
    }

    //读入DIB文件头  BITMAPFILEHEADER
    bSuccess = ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
    if (!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER)) || (bmfh.bfType != *(WORD *)"BM")){
        CloseHandle(hFile);
        return ;
    }

    //为DIB BITMAPINFO分配内存,并读入DIB数据  
    dwInfoSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
    pbmi = (BITMAPINFO*)malloc(dwInfoSize);

    if (NULL == pbmi){
        CloseHandle(hFile);
        return ;
    }

    //读取BITMAPINFO
    bSuccess = ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, NULL);
    if (!bSuccess || (dwBytesRead != dwInfoSize)){
        free(pbmi);
        CloseHandle(hFile);
        return ;
    }

    //读取图片数据
    bSuccess = ReadFile(hFile, _frame_buffers, bmfh.bfSize - bmfh.bfOffBits, &dwBytesRead, NULL);
    CloseHandle(hFile);

    for (int i = 0; i < _width / 2; i++) {
        for (int j = 0; j < _height / 2; j++) {
            _frame_buffers[(i*_width + j + 1) * 3] = 255;//b
            _frame_buffers[(i*_width + j + 1) * 3 + 1] = 0;//g
            _frame_buffers[(i*_width + j + 1) * 3 + 2] = 0;//r
        }
    }
}