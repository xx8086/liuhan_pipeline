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
        void specialkeyboard(unsigned int uchar, unsigned int utype);
        void specialkeydown(unsigned int uchar, unsigned int utype);
        void specialkeyup(unsigned int uchar, unsigned int utype);
        void destroy(HWND hwnd);
        void release();
    private:
        template<typename T>
        void insert_point(std::vector<T>& v, T a[3]);
        template<typename T>
        void insert_quadrilateral(std::vector<T>& v, T a[3], T b[3], T c[3], T d[3]);
        void update_vertex();
        bool load_dib_texture(TCHAR* img);
        bool load_level_texture(TCHAR* img, int size);
		bool load_floor_texture(TCHAR* img, int size);
        void* load_bmp(TCHAR* img, int &w, int& h, int& bitcounts);
		void profile(HDC& hdc);
    private:
        HDC _frame_dc = nullptr;
        HBITMAP _dib = nullptr;
        HBITMAP _old_bitmap = nullptr;
		HFONT _font;
		HGDIOBJ  _old_font = nullptr;
		RECT _rc;
    };
}
