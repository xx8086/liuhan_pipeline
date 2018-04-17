#pragma once
#include "..\lhpipeline\lhdevice.h"

namespace lh_pipeline {
    class LhDib : protected LhDevice
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
        void insert_point(std::vector<float>& v, float a[3]);
        void insert_quadrilateral(std::vector<float>& v, float a[3], float b[3], float c[3], float d[3]);
        void update_vertex();
        bool load_dib_texture(TCHAR* img);
    private:
        HDC _frame_dc = nullptr;
        HBITMAP _dib = nullptr;
        HBITMAP _old_bitmap = nullptr;
    };
}
