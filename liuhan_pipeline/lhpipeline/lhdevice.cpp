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

    void LhDevice::bind_vertex(const float* vertex, const unsigned int* vertex_color, const float* vertex_uv, const int vertex_size) {
        assert(nullptr != vertex);
        assert(vertex_size > 0);
        update_vertex(vertex, vertex_color, vertex_uv, vertex_size);
        set_current_uv(ger_current_texutre_uv_buffers(), get_current_texture_uv_size());
        _piple.set_rotate(0.0f, 0.0f, 0.0f);
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_worldpos(0, 0, 0);
        _piple.set_camera(LhVertexFloat3(0.0f, 0.0f, -3.0f), LhVertexFloat3(0.0f, 0.0f, 1.0f), LhVertexFloat3(0.0f, 1.0f, 0.0f));
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
        if (LH_OFF_DRAW == _render_state) {
            return;
        }
        static int count = 0;
        if (count < 200) {
            count++;
            return;
        }
        count = 0;

        clear_buffer();
        clear_deep();
        _piple.get_wvp();
        switch (_render_state) {
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
        case LH_TRIANGLES_TEXTURE_FILL:
            draw_trangles_texture_fill();
            break;
        case LH_TEST:
            static float roat_x = 0;
            static float roat_y = 0;
            static float roat_z = 0;
            
            
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
            //draw_triangles();
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
        const unsigned int* colors = get_vertex_color_buffers();
        const int counts = 3 * get_vertex_buffers_size();

        for (int i = 0; i < counts; i += 9) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            LhVertexFloat4 p3;
            if (get_pos(p1, LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5])) &&
                get_pos(p3, LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]))) {

                lh_color c1(colors[i], colors[i + 1], colors[i + 2]);
                lh_color c2(colors[i + 3], colors[i + 4], colors[i +5]);
                lh_color c3(colors[i + 6], colors[i + 7], colors[i + 8]);
#if 0
                draw_triangle(p1.get_x(), p1.get_y(), 
                    p2.get_x(), p2.get_y(), 
                    p3.get_x(), p3.get_y(),
                    lh_color(0.0f, 255.0f, 0.0f));
#else
                draw_triangle(VertexColor(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), c1),
                    VertexColor(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), c2),
                    VertexColor(LhVertexFloat3(p3.get_x(), p3.get_y(), p3.get_z()), c3));
#endif
            }
        }

        //int texture_size = get_current_texture_uv_size();
        //unsigned int* texture = (unsigned int*)ger_current_texutre_uv_buffers();
        //unsigned int* des = (unsigned int*)get_frame_buffers();
        //for (int i = 0; i < texture_size; i++) {
        //    for (int j = 0; j < texture_size; j++) {
        //        int pos = (j * texture_size + i) ;
        //        //setpixel(i, j, lh_color(texture[pos]));
        //        des[j*get_width() + i] = texture[pos];
        //    }
        //}
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

    void LhDevice::update_texture(unsigned char* texture_datas, int texture_size) {
        set_texture(texture_datas, texture_size);
    }

    void LhDevice::draw_trangles_texture_fill() {
        const float* v = get_vertex_buffers();
        const unsigned int* colors = get_vertex_color_buffers();
        const int counts = 3 * get_vertex_buffers_size();
        const float* uv = ger_current_uv();
        int uv_count = 0;//2 * get_vertex_buffers_size();
        for (int i = 0; i < counts; i += 9, uv_count += 6) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            LhVertexFloat4 p3;
            if (get_pos(p1, LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5])) &&
                get_pos(p3, LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]))) {

                draw_triangle(VertexColor(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), TextureUV(uv[uv_count], uv[uv_count + 1])),
                    VertexColor(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), TextureUV(uv[uv_count + 2], uv[uv_count + 3])),
                    VertexColor(LhVertexFloat3(p3.get_x(), p3.get_y(), p3.get_z()), TextureUV(uv[uv_count + 4], uv[uv_count + 5])),
                    true);
            }
        }
    }

}
