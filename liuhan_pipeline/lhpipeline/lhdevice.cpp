#include "lhdevice.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h >

namespace lh_pipeline {
    LhDevice::LhDevice()
    {
    }


    LhDevice::~LhDevice()
    {
    }

    void LhDevice::update_buffer(int w, int h, void* pbits) {
        LhDrawPrimitive::set_buffer(w, h, pbits);
        LhFrameBuffer::set_buffer(w, h, pbits);
        _piple.set_windows_size(w, h);
    }

    void LhDevice::draw_line(int x1, int y1, int x2, int y2, int color) {
        line_bresenham(x1, y1, x2, y2, lh_color(color));
        //line(x1, y1, x2, y2, lh_color(color));
    }

    void LhDevice::bind_vertex(float* vertex, int triangle_counts) {
        assert(nullptr != vertex);
        assert(triangle_counts > 0);
        update_triangle_vertex(vertex, triangle_counts);

        _piple.set_rotate(60.0f, 30.0f, 0.0f);
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_worldpos(1, 0, 0);
        _piple.set_camera(LhVertexFloat3(0.0f, 0.0f, -4.0f), LhVertexFloat3(0.0f, 0.0f, 1.0f), LhVertexFloat3(0.0f, 1.0f, 0.0f));
        PersProjInfo per(60.0f, get_width(), get_height(), 1.0f, 100.0f);
        _piple.set_perspective_proj(per);

    }

    bool LhDevice::get_pos(LhVertexFloat4& f4, LhVertexFloat3 f3) {
        f4 = _piple.transformation_in_mvp(f3);
        /*if (0 == _piple.transformation_cut(f4)) {
            return false;
        }*/
        f4 = _piple.transformation_normalization(f4);
        return true;
    }

    //#include <windows.h> 
    void LhDevice::draw() {
        const float* v = get_vertex_buffers();
        const int counts = get_triangle_counts() * 9;
        _piple.get_wvp();
        lh_color lc(255, 0, 0);
        int color = lc.get_t<int>();
        //TCHAR info[1024] = { 0 };
        for (int i = 0; i < counts; i += 9) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            LhVertexFloat4 p3;
            if (get_pos(p1, LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5])) &&
                get_pos(p3, LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]))) {
                draw_line(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y(), color);
                draw_line(p2.get_x(), p2.get_y(), p3.get_x(), p3.get_y(), color);
                draw_line(p1.get_x(), p1.get_y(), p3.get_x(), p3.get_y(), color);

                /*snprintf(info, 1024 * sizeof(TCHAR),
                    "p1:%.3f, %.3f, %.3f, p2:%.3f, %.3f, %.3f, p3:%.3f, %.3f, %.3f\n",
                    p1.get_x(), p1.get_y(), p1.get_z(),
                    p2.get_x(), p2.get_y(), p2.get_z(),
                    p3.get_x(), p3.get_y(), p3.get_z());
                OutputDebugString(info);*/
            }
        }
    }
}
