#include "lhdevice.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h >
#include <chrono>
#include <windows.h>

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
    void LhDevice::set_front_begin(float xpos, float ypos) {
        _last_x = xpos;
        _last_y = ypos;
    }
    void LhDevice::set_front(float xpos, float ypos) {
		return;
        float xoffset = xpos - _last_x;
        float yoffset = _last_y - ypos; // reversed since y-coordinates go from bottom to top
        _last_x = xpos;
        _last_y = ypos;

        _piple.set_front(-xoffset, -yoffset);
    }

    void LhDevice::keyboard(/*char vk, */char key) {
        float add = 0.1f;
        bool change = true;
		float roateangle = 20 * _draw_cost_time;
		float speed = 5 * _draw_cost_time;
        switch (key) {
        case 'W':
        case 0x26://VK_UP
            //_piple.set_view_ward(VIEW_FORWARD, _draw_cost_time);
            //z_mip();
			_m_z += speed;
            break;
        case 'S':
        case 0x28://VK_DOWN
            //_piple.set_view_ward(VIEW_BACKWARD, _draw_cost_time);
            //z_mip();
			_m_z -= speed;
			break;
        case 'A':
        case 0x25://VK_LEFT
            //_piple.set_view_ward(VIEW_LEFT, _draw_cost_time);
            //z_mip();
			_m_x -= speed;
			break;
        case 'D':
        case 0x27://VK_RIGHT
            //_piple.set_view_ward(VIEW_RIGHT, _draw_cost_time);
            //z_mip();
			_m_x += speed;
			break;
        case 0x20:
            enablelight();
            break;
        case 'I':
            set_render_state(LH_TEST);
            break;
        case 'O':
            set_render_state(LH_TRIANGLES_FILL);
            break;
        case 'P':
            set_render_state(LH_TRIANGLES_TEXTURE_FILL);
            break;
		case 'X':
			_r_x += roateangle;
			break;
		case 'Y':
			_r_y += roateangle;
			break;
		case 'Z':
			_r_z += roateangle;
			break;
        default:
            change = false;
            break;
        }

		char msgbuf[512];
		LhVertexFloat3 view = _piple.get_view_pos();
		LhVertexFloat3 dir = _piple.get_view_dir();
		sprintf_s(msgbuf, "z = %f, speed = %f, view(%f, %f, %f), centor(%f, %f, %f)\n", 
			_m_z, speed, 
			view.get_x(), view.get_y(), view.get_z(),
			dir.get_x(), dir.get_y(), dir.get_z());
		///_liu = true;
		OutputDebugString(msgbuf);
    }

    void LhDevice::z_mip() {
        LhVertexFloat3 viewpos = _piple.get_view_pos();
        /*float z = (viewpos.get_x() * viewpos.get_x()  + 
            viewpos.get_y() * viewpos.get_y() * + viewpos.get_z() * viewpos.get_z());
        if (z >= 1.2f) {
            set_current_texture_uv(TEXTURE_LEVEL_128);
        }
        else if (1.2f > z && z >= 0.4f) {
            set_current_texture_uv(TEXTURE_LEVEL_256);
        }
        else if (0.4f > z) {
            set_current_texture_uv(TEXTURE_LEVEL_512);
        }*/
		set_current_texture_uv(TEXTURE_LEVEL_256);
        set_current_uv(ger_current_texutre_uv_buffers(), get_current_texture_uv_size());
    }

    void LhDevice::bind_vertex(const float* vertex, const unsigned int* vertex_color, const float* vertex_uv, const int vertex_size) {
        assert(nullptr != vertex);
        assert(vertex_size > 0);
        update_vertex(vertex, vertex_color, vertex_uv, vertex_size);
        z_mip();
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_rotate(0.0f, 0.0f, 0.0f);
        _piple.set_worldpos(0.0f, 0.0f, 0.0f);
        _piple.set_camera_pos(LhVertexFloat3(0.0f, 0.0f, -2.0f));
        PersProjInfo per(60.0f, static_cast<float>(get_width()), static_cast<float>(get_height()), 1.0f, 100.0f);
        _piple.set_perspective_proj(per);
        set_view(&_piple.get_view_pos());
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
        static float srotate = 0.0;
        srotate += 0.5f;
        auto start = std::chrono::system_clock::now();
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
			_piple.set_rotate(_r_x, _r_y, _r_z);
			_piple.set_worldpos(_m_x, _m_y, _m_z);
            draw_trangles_texture_fill();
            break;
        case LH_TEST:
            draw_floor();
            break;
        }

        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        _draw_cost_time = float(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

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

                lh_color c1((float)colors[i], (float)colors[i + 1], (float)colors[i + 2]);
                lh_color c2((float)colors[i + 3], (float)colors[i + 4], (float)colors[i +5]);
                lh_color c3((float)colors[i + 6], (float)colors[i + 7], (float)colors[i + 8]);
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

    float floor_v[] = {
        -1.5f, -1.5f, -0.5f,//b
        1.5f, -1.5f, -0.5f,//c
        -1.5f, 1.5f, -0.5f,//a

        1.5f, -1.5f, -0.5f,//c
        1.5f, 1.5f, -0.5f,//d
        -1.5f, 1.5f, -0.5f,//a
    };

    unsigned int  floor_colors[] = {
        0, 255, 0,
        0, 0, 255,
        255, 0, 0,

        0, 0, 255,
        0, 255, 0,
        255, 0, 0
    };

    float floor_uv[] = {
        0, 1,
        1, 1,
        0, 0,

        1, 1,
        1, 0,
        0, 0
    };
    void LhDevice::draw_floor() {
        _piple.set_rotate(-80.0f, 0, 0.0f);
        _piple.set_sale(1.5f, 1.0f, 1.5f);
        _piple.set_worldpos(0, 2.3f, 1.0f);
        _piple.get_wvp();

        const int counts = 18;
        for (int i = 0; i < counts; i += 9) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            LhVertexFloat4 p3;
            if (get_pos(p1, LhVertexFloat3(floor_v[i], floor_v[i + 1], floor_v[i + 2])) &&
                get_pos(p2, LhVertexFloat3(floor_v[i + 3], floor_v[i + 4], floor_v[i + 5])) &&
                get_pos(p3, LhVertexFloat3(floor_v[i + 6], floor_v[i + 7], floor_v[i + 8]))) {

                lh_color c1((float)floor_colors[i], (float)floor_colors[i + 1], (float)floor_colors[i + 2]);
                lh_color c2((float)floor_colors[i + 3], (float)floor_colors[i + 4], (float)floor_colors[i + 5]);
                lh_color c3((float)floor_colors[i + 6], (float)floor_colors[i + 7], (float)floor_colors[i + 8]);

                draw_triangle(VertexColor(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), c1),
                    VertexColor(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), c2),
                    VertexColor(LhVertexFloat3(p3.get_x(), p3.get_y(), p3.get_z()), c3));

            }
        }
    }

    void LhDevice::draw_grid() {
        if (!_grid) {
            return;
        }

        float value = 1.0f;
        float z = -0.5;
        float x_begin = -value;
        float x_end = value;
        float y_begin = -value;
        float y_end = value;
        lh_color lc(255, 0, 0);
        int color = lc.get_t<int>();
        for (float i = x_begin; i <= x_end; i += 0.15) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            get_pos(p1, LhVertexFloat3(i, -value, z));
            get_pos(p2, LhVertexFloat3(i, value, z));
            draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
        }
        for (float j = y_begin; j <= y_end; j += 0.15) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            get_pos(p1, LhVertexFloat3(-value, j, z));
            get_pos(p2, LhVertexFloat3(value, j, z));
            draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
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

    void LhDevice::update_texture(unsigned char* texture_datas, int texture_size) {
        set_texture(texture_datas, texture_size);
    }

    void LhDevice::draw_trangles_texture_fill() {
		if (!_piple.can_draw()) {
			return;
		}
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

				if (_liu) {
					char msgbuf[1024] = { 0 };
					sprintf_s(msgbuf, "v1(%f, %f, %f),v2(%f, %f, %f),v3(%f, %f, %f)\n", 
						p1.get_x(), p1.get_y(), p1.get_z(),
						p2.get_x(), p2.get_y(), p2.get_z(),
						p3.get_x(), p3.get_y(), p3.get_z());
					_liu = false;
					OutputDebugString(msgbuf);
				}

                draw_triangle(VertexColor(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), TextureUV(uv[uv_count], uv[uv_count + 1])),
                    VertexColor(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), TextureUV(uv[uv_count + 2], uv[uv_count + 3])),
                    VertexColor(LhVertexFloat3(p3.get_x(), p3.get_y(), p3.get_z()), TextureUV(uv[uv_count + 4], uv[uv_count + 5])),
                    true);
            }
        }
    }

}
