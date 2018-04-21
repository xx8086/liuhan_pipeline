#pragma once
#include "..\lhpipeline\lhdevice.h"
#include <vector>

namespace lh_pipeline {
    class LhDib: public LhDevice
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
        template<typename T>
        void insert_point(std::vector<T>& v, T a[3]);
        template<typename T>
        void insert_quadrilateral(std::vector<T>& v, T a[3], T b[3], T c[3], T d[3]);
        void update_vertex();
        bool load_dib_texture(TCHAR* img);
    private:
        HDC _frame_dc = nullptr;
        HBITMAP _dib = nullptr;
        HBITMAP _old_bitmap = nullptr;
    };
}
