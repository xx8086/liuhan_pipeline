#include "stdafx.h"
#include "lhdib.h"

namespace lh_pipeline {
    LhDib::LhDib() {
		_rc.left = 10;
		_rc.top = 10;
		_rc.right = 350;
		_rc.bottom = 400;

		_font =
			CreateFont(
				20, // nHeight
				0, // nWidth
				0, // nEscapement
				0, // nOrientation
				FW_NORMAL, // nWeight
				FALSE, // bItalic
				FALSE, // bUnderline
				0, // cStrikeOut
				ANSI_CHARSET, // nCharSet
				OUT_DEFAULT_PRECIS, // nOutPrecision
				CLIP_DEFAULT_PRECIS, // nClipPrecision
				DEFAULT_QUALITY, // nQuality
				DEFAULT_PITCH | FF_SWISS,
				_T("Arial") // nPitchAndFamily Arial
			);
    }


    LhDib::~LhDib() {
        release();
    }

    void LhDib::init(HWND hwnd, int w, int h) {
        HDC hdc = GetDC(hwnd);
        _frame_dc = CreateCompatibleDC(hdc);
#if 0
        load_dib_texture("../res/512.bmp");
        LhDevice::set_render_state(LH_OFF_DRAW);
#else
        BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
            DWORD(w * h * 4), 0, 0, 0, 0 } };
        void* ptr;
        _dib = CreateDIBSection(_frame_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
        if (_dib == nullptr) return;
        _old_bitmap = (HBITMAP)SelectObject(_frame_dc, _dib);

        LhDevice::update_buffer(w, h, ptr);
        LhDevice::set_render_state(LH_TRIANGLES_TEXTURE_FILL);
        
        load_level_texture((TCHAR*)"../res/128.bmp", 128);
        load_level_texture((TCHAR*)"../res/256.bmp", 256);
        load_level_texture((TCHAR*)"../res/512.bmp", 512);
		load_floor_texture((TCHAR*)"../res/512_f.bmp", 512);
        update_vertex();
#endif
        ReleaseDC(hwnd, hdc);
    }

    void LhDib::bitblt(HWND hwnd) {
        draw();
        HDC hdc = GetDC(hwnd);
		profile(_frame_dc);
        ::BitBlt(hdc, 0, 0, get_width(), get_height(), _frame_dc, 0, 0, SRCCOPY);
        ReleaseDC(hwnd, hdc);
    }

    void LhDib::specialkeydown(unsigned int uchar, unsigned int utype) {
        keyboard_down(uchar, utype);
    }
    void LhDib::specialkeyup(unsigned int uchar, unsigned int utype) {
        keyboard_up(uchar, utype);
    }
    void LhDib::specialkeyboard(unsigned int uchar, unsigned int utype){
        
    }

    void LhDib::release() {
        ;
    }

    void* LhDib::load_bmp(TCHAR* img, int &w, int& h, int& bitcounts) {
        BITMAPFILEHEADER            bmfh;
        BITMAPINFO                      *pbmi;
        BOOL                                    bSuccess;
        DWORD                               dwInfoSize;
        DWORD                               dwBytesRead;
        HANDLE                              hFile;
        hFile = CreateFile(img, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
        if (INVALID_HANDLE_VALUE == hFile) {
            return false;
        }

        bSuccess = ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, nullptr);
        if (!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER)) || (bmfh.bfType != *(WORD *)"BM")) {
            CloseHandle(hFile);
            return false;
        }

        dwInfoSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
        pbmi = (BITMAPINFO*)malloc(dwInfoSize);
        if (nullptr == pbmi) {
            CloseHandle(hFile);
            return false;
        }

        bSuccess = ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, nullptr);
        if (!bSuccess || (dwBytesRead != dwInfoSize)) {
            free(pbmi);
            CloseHandle(hFile);
            return false;
        }

        void* ptr = nullptr;
        w = pbmi->bmiHeader.biWidth;
        h = pbmi->bmiHeader.biHeight;
        bitcounts = pbmi->bmiHeader.biBitCount / 8;
        ptr = new unsigned char[bmfh.bfSize - bmfh.bfOffBits + 1];
        bSuccess = ReadFile(hFile, ptr, bmfh.bfSize - bmfh.bfOffBits, &dwBytesRead, NULL);
        CloseHandle(hFile);

        free(pbmi);
        return ptr;
    }
	bool LhDib::load_floor_texture(TCHAR* img, int size) {
		int texture_w = 0;
		int texture_h = 0;
		int texture_bit_counts = 4;

		void* texture_datas = load_bmp(img, texture_w, texture_h, texture_bit_counts);
		if (size != abs(texture_w) || size != abs(texture_h)) {
			delete[] texture_datas;
			texture_datas = nullptr;
			return false;
		}

		set_floor(static_cast<unsigned char*>(texture_datas), size);
		return true;
	}

    bool LhDib::load_level_texture(TCHAR* img, int size) {
        int texture_w = 0;
        int texture_h = 0;
        int texture_bit_counts = 4;

        void* texture_datas =load_bmp(img, texture_w, texture_h, texture_bit_counts);
        if (size != abs(texture_w) || size != abs(texture_h)) {
            delete[] texture_datas;
            texture_datas = nullptr;
            return false;
        }
        update_texture(static_cast<unsigned char*>(texture_datas), size);
		//texture_datas把指针地址赋值给framebuffer管理，最后结束的时候由framebuffer管理delete.
        return true;
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
        if (INVALID_HANDLE_VALUE == hFile) {
            return false;
        }

        //读入DIB文件头  BITMAPFILEHEADER
        bSuccess = ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
        if (!bSuccess || (dwBytesRead != sizeof(BITMAPFILEHEADER)) || (bmfh.bfType != *(WORD *)"BM")) {
            CloseHandle(hFile);
            return false;
        }

        //为DIB BITMAPINFO分配内存,并读入DIB数据  
        dwInfoSize = bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
        pbmi = (BITMAPINFO*)malloc(dwInfoSize);

        if (NULL == pbmi) {
            CloseHandle(hFile);
            return false;
        }

        //读取BITMAPINFO
        bSuccess = ReadFile(hFile, pbmi, dwInfoSize, &dwBytesRead, NULL);
        if (!bSuccess || (dwBytesRead != dwInfoSize)) {
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
        //        _frame_buffers[(i*_width + j + 1) * 3] = 0;//a
        //    }
        //}

        free(pbmi);
        return true;
    }


    template<typename T>
    void LhDib::insert_point(std::vector<T>& v, T a[3]) {
        v.push_back(a[0]);
        v.push_back(a[1]);
        v.push_back(a[2]);
    }

    template<typename T>
    void LhDib::insert_quadrilateral(std::vector<T>& v, T a[3], T b[3], T c[3], T d[3]) {
        //abc, cda
        insert_point(v, a);
        insert_point(v, b);
        insert_point(v, c);

        insert_point(v, c);
        insert_point(v, d);
        insert_point(v, a);
    }

    void LhDib::update_vertex() {
#if 1
        float vertex[8][3] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,//
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
        };
        unsigned int vertex_color[8][3] = {
            255, 0, 0,
            0, 255, 0,
            0, 0, 255,
            0, 255, 0,//
            0, 0, 255,
            255, 0, 0,
            0, 255, 0,
            255, 0, 0,
        };
        std::vector<float> vecs;
        std::vector<unsigned int> vecs_color;
        
        insert_quadrilateral<float>(vecs, vertex[0], vertex[2], vertex[3], vertex[1]);
        insert_quadrilateral<float>(vecs, vertex[0], vertex[4], vertex[6], vertex[2]);
        insert_quadrilateral<float>(vecs, vertex[0], vertex[1], vertex[5], vertex[4]);
        insert_quadrilateral<float>(vecs, vertex[4], vertex[5], vertex[7], vertex[6]);
        insert_quadrilateral<float>(vecs, vertex[1], vertex[3], vertex[7], vertex[5]);
        insert_quadrilateral<float>(vecs, vertex[2], vertex[6], vertex[7], vertex[3]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[0], vertex_color[2], vertex_color[3], vertex_color[1]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[0], vertex_color[4], vertex_color[6], vertex_color[2]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[0], vertex_color[1], vertex_color[5], vertex_color[4]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[4], vertex_color[5], vertex_color[7], vertex_color[6]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[1], vertex_color[3], vertex_color[7], vertex_color[5]);
        insert_quadrilateral<unsigned int>(vecs_color, vertex_color[2], vertex_color[6], vertex_color[7], vertex_color[3]);


		const float uvs[72] = {
			0.0f,  0.0f,
			0.0f,  1.0f,
			1.0f,  1.0f,
			1.0f,  1.0f,
			1.0f,  0.0f,
			0.0f,  0.0f,

			0.0f,  0.0f,
			1.0f,  0.0f,
			1.0f,  1.0f,
			1.0f,  1.0f,
			0.0f,  1.0f,
			0.0f,  0.0f,

			1.0f,  0.0f,
			1.0f,  1.0f,
			0.0f,  1.0f,
			0.0f,  1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f,

			1.0f,  0.0f,
			0.0f,  0.0f,
			0.0f,  1.0f,
			0.0f,  1.0f,
			1.0f,  1.0f,
			1.0f,  0.0f,

			0.0f,  1.0f,
			1.0f,  1.0f,
			1.0f,  0.0f,
			1.0f,  0.0f,
			0.0f,  0.0f,
			0.0f,  1.0f,

			0.0f,  1.0f,
			0.0f,  0.0f,
			1.0f,  0.0f,
			1.0f,  0.0f,
			1.0f,  1.0f,
			0.0f,  1.0f
	};
        LhDevice::bind_vertex(vecs.data(), vecs_color.data(), uvs, vecs.size()/3);
#else
/*    a-----|------d--------->u
        |    \  |         |
    ---|-----0-----|--------->x
        |        |   \    |
       b-----|-----c
       |v      \/ y
 */
        float v[] = { 
            -1.0f, -1.0f, -0.5f,//b
            1.0f, -1.0f, -0.5f,//c
            -1.0f, 1.0f, -0.5f,//a

            1.0f, -1.0f, -0.5f,//c
            1.0f, 1.0f, -0.5f,//d
            -1.0f, 1.0f, -0.5f,//a
        };

        unsigned int  color[] = {
            0, 255, 0,
            0, 0, 255,
            255, 0, 0,

            0, 0, 255,
            0, 255, 0,
            255, 0, 0
        };

        float uv[] = {
            0, 1,
            1, 1,
            0, 0,

            1, 1, 
            1, 0,
            0, 0
        };
        LhDevice::bind_vertex(v, color, uv, 6);
#endif
    }

	void LhDib::profile(HDC& hdc) {
		LhVertexFloat3 pos = get_world_pos();
        LhVertexFloat3 eyes = get_eyes_pos();
		char str_fps[2048] = { 0 };
		sprintf_s(str_fps,
			"方向键控制被观察对象的移动；\n"
            "ASDW键控制摄像机的移动；\n"
            "按住鼠标左键拖动修改镜头朝向；\n"
			"空格键: 开关灯光；\n"
			"V: 开关地板；\n"
			"R: 开关自动旋转；\n"
			"L: 开关画片元三角形；\n"
			"F: 切换正反面消除；\n"
			"I: 线框； O: Gouraud； P: uv贴图；\n"
			"XYZ: 绕xyz轴旋转；\n"
			"world_pos: (%.3f, %.3f, %.3f)；\n"
            "eyes_pos: (%.3f, %.3f, %.3f)；\n"
			"每一帧耗时：%.3fms;  fps: %.3f；\n",
			pos.get_x(), pos.get_y(), pos.get_z(),
            eyes.get_x(), eyes.get_y(), eyes.get_z(),
			1000 * get_draw_cost(), get_fps());
		_old_font = SelectObject(hdc, _font);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 0, 0));
		DrawText(hdc, str_fps, strlen(str_fps) * sizeof(char), &_rc, DT_LEFT);
		SelectObject(hdc, _old_font);
	}

	void LhDib::destroy(HWND hwd) {
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

		if (nullptr != _font) {
			HDC hdc = GetDC(hwd);
			DeleteObject(_font);
			ReleaseDC(hwd, hdc);
		}
	}
}