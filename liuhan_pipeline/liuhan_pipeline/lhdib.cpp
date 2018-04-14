#include "stdafx.h"
#include "lhdib.h"


LhDib::LhDib()
{
}


LhDib::~LhDib()
{
}

void LhDib::init(HWND hwnd, int w, int h) {
    HDC hdc = GetDC(hwnd);
    void* ptr;

    _frame_dc = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd, hdc);

#if 1
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, h, 1, 24, BI_RGB,
        w * h * 3, 0, 0, 0, 0 } };
#else
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB,
        w * h * 4, 0, 0, 0, 0 } };
#endif
    _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (_dib == nullptr) return;
    _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);
    _frame_buffers = (unsigned char*)ptr;
    load_dib_texture("D:\\liuhan.dib");
    _width = w;
    _height = h;
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
}