#include "stdafx.h"
#include "lhdib.h"


LhDib::LhDib(){
}


LhDib::~LhDib(){
    release();
}

void LhDib::init(HWND hwnd, int w, int h) {
    HDC hdc = GetDC(hwnd);
    _frame_dc = CreateCompatibleDC(hdc);
#if 0
    load_dib_texture("D:\\liuhan.dib");
#else
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
        DWORD(w * h * 4), 0, 0, 0, 0 } };
    void* ptr;
    _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (_dib == nullptr) return;
    _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);    

    update_buffer(w, h, ptr);
    
    //draw_line(0, 0, get_width() - 2, get_height() - 2, 1);
#endif
    ReleaseDC(hwnd, hdc);
}

void LhDib::bitblt(HWND hwnd) {
    draw();
    HDC hdc = GetDC(hwnd);
    ::BitBlt(hdc, 0, 0, get_width(), get_height(), _frame_dc, 0, 0, SRCCOPY);
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

bool LhDib::load_dib_texture(TCHAR* img) {
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
        return false;
    }

    //读入DIB文件头  BITMAPFILEHEADER
    bSuccess = ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
    if (!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER)) || (bmfh.bfType != *(WORD *)"BM")){
        CloseHandle(hFile);
        return false;
    }

    //为DIB BITMAPINFO分配内存,并读入DIB数据  
    dwInfoSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
    pbmi = (BITMAPINFO*)malloc(dwInfoSize);

    if (NULL == pbmi){
        CloseHandle(hFile);
        return false;
    }

    //读取BITMAPINFO
    bSuccess = ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, NULL);
    if (!bSuccess || (dwBytesRead != dwInfoSize)){
        free(pbmi);
        CloseHandle(hFile);
        return false;
    }

    //读取图片数据
    void* ptr = nullptr;
    int w = pbmi->bmiHeader.biWidth;
    int h = pbmi->bmiHeader.biHeight;
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, h, 1, 
        DWORD(pbmi->bmiHeader.biBitCount), 
        BI_RGB,
        DWORD(w * h * pbmi->bmiHeader.biBitCount / 8),
        0, 0, 0, 0 } };
    _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (_dib == nullptr) return false;
    _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);

    bSuccess = ReadFile(hFile, ptr, bmfh.bfSize - bmfh.bfOffBits, &dwBytesRead, NULL);
    CloseHandle(hFile);

    update_buffer(w, h, ptr);
    //for (int i = 0; i < _width / 2; i++) {
    //    for (int j = 0; j < _height / 2; j++) {
    //        _frame_buffers[(i*_width + j + 1) * 3] = 255;//b
    //        _frame_buffers[(i*_width + j + 1) * 3 + 1] = 0;//g
    //        _frame_buffers[(i*_width + j + 1) * 3 + 2] = 0;//r
    //    }
    //}
    return true;
}