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
        LhDrawPrimitive::draw_line(x1, y1, x2, y2, lh_color(color));
    }

    void LhDevice::bind_vertex(float* vertex, int vertex_size) {
        assert(nullptr != vertex);
        assert(vertex_size > 0);
        update_vertex(vertex, vertex_size);

        _piple.set_rotate(30.0f, 30.0f, 0.0f);
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_worldpos(1, 0, 0);
        _piple.set_camera(LhVertexFloat3(0.0f, 0.0f, 6.0f), LhVertexFloat3(0.0f, 0.0f, 1.0f), LhVertexFloat3(0.0f, 1.0f, 0.0f));
        PersProjInfo per(60.0f, static_cast<float>(get_width()), static_cast<float>(get_height()), 1.0f, 100.0f);
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
        //clear_buffer();
        _piple.get_wvp();
        switch (LG_TEST) {
        case LH_LINES:
            draw_line();
            break;
        case LH_LINES_LOOP:
            draw_line(true);
            break;
        case LH_TRIANGLES:
            draw_triangles();
            break;
        case LH_TRIANGLES_FILL:
            draw_triangles_fill();
            break;
        case LG_TEST:
            static float roat_x = 0;
            static float roat_y = 0;
            static float roat_z = 0;
            static int count = 0;
            if (count < 200) {
                count++;
                break;
            }
            count = 0;
            clear_buffer();
            roat_x += 1;
            roat_y += 1;
            roat_z += 1;
            /*_piple.set_rotate(roat_x, roat_y, roat_z);
            _piple.set_worldpos(3, 0, 0);
            _piple.get_wvp();
            draw_triangles();*/

            _piple.set_rotate(roat_x, roat_y, roat_z);
            _piple.set_worldpos(0, 0, 0);
            _piple.get_wvp();
            draw_triangles_fill();
            draw_triangles();
            break;
        }

    }

    void LhDevice::draw_line(bool loop) {
        const float* v = get_vertex_buffers();
        const int counts = get_vertex_buffers_size() - 3;
        lh_color lc(255, 0, 0);
        int color = lc.get_t<int>();
        for (int i = 0; i < counts; i += 3) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            if (get_pos(p1, LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5]))) {
                draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
            }
        }

        if (loop) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            if (get_pos(p1, LhVertexFloat3(v[counts], v[counts + 1], v[counts + 2])) &&
                get_pos(p2, LhVertexFloat3(v[0], v[1], v[2]))) {
                draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
            }
        }
    }
    void LhDevice::draw_triangles_fill() {
        //triangle_fill;
        const float* v = get_vertex_buffers();
        const int counts = get_vertex_buffers_size();
        
        lh_color lc(0, 0, 255);
        int color = lc.get_t<int>();
        for (int i = 0; i < counts; i += 9) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            LhVertexFloat4 p3;
            if (get_pos(p1, LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5])) &&
                get_pos(p3, LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]))) {
                //
                draw_triangle(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y(), p3.get_x(), p3.get_y(),
                    //lh_color(float(rand()) / 255.0f, float(rand()) / 255.0f, float(rand()) / 255.0f));
                    lh_color(0.0f, 255.0f, 0.0f));
                /*triangle_fill(VertexColor(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), lh_color(0.0f, 255.0f, 0.0f)),
                    VertexColor(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), lh_color(0.0f, 255.0f, 0.0f)),
                    VertexColor(LhVertexFloat3(p3.get_x(), p3.get_y(), p3.get_z()), lh_color(0.0f, 255.0f, 0.0f)));*/
            }
        }
    }
    void LhDevice::draw_triangles() {
        const float* v = get_vertex_buffers();
        const int counts = get_vertex_buffers_size();
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
                draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
                draw_line((int)p2.get_x(), (int)p2.get_y(), (int)p3.get_x(), (int)p3.get_y(), color);
                draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p3.get_x(), (int)p3.get_y(), color);

                /*snprintf(info, 1024 * sizeof(TCHAR),
                "p1:%.3f, %.3f, %.3f, p2:%.3f, %.3f, %.3f, p3:%.3f, %.3f, %.3f\n",
                p1.get_x(), p1.get_y(), p1.get_z(),
                p2.get_x(), p2.get_y(), p2.get_z(),
                p3.get_x(), p3.get_y(), p3.get_z());
                OutputDebugString(info);*/
            }
        }
    }

    void LhDevice::set_render_state(LHRENDER_STATE state) {
        _render_state = state;
    }
}
