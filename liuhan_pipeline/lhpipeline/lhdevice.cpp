#include "lhdevice.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h >
#include <chrono>
#include <vector>

namespace lh_pipeline {
    LhDevice::LhDevice()
    {
        _timer.set_count_time(1000);
    }

    LhDevice::~LhDevice()
    {
    }

    void LhDevice::update_size(int w, int h) {
        ;
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
        _piple.set_front_begin(xpos, ypos);
    }
    void LhDevice::set_front(float xpos, float ypos) {
        _piple.set_front(xpos, ypos);
    }

    void LhDevice::keyboard_down(char vk, char key) {
        _key[vk] = 1;
        if (_key['I']) set_render_state(LH_TRIANGLES);
        if (_key['O']) set_render_state(LH_TRIANGLES_FILL);
        if (_key['P']) set_render_state(LH_TRIANGLES_TEXTURE_FILL);
        if (_key['F']) _front = !_front;
        if (_key['V']) _floor = !_floor;
        if (_key['R']) _rotate = !_rotate;
        if (_key['L']) set_draw_triangle_line();
        if (_key[32]) enablelight();
    }
//#include <windows.h>
    void LhDevice::keyboard_up(char vk, char key) {
        _key[vk] = 0;
        /*char  buff[56] = {0};
        sprintf_s(buff, "up key=%c, %d\n", vk, _key[vk]);
        OutputDebugString(buff);*/
    }

    void LhDevice::keyboard_update() {
        float speed = 0.009;
        float roateangle = 2.0f;
        if (_key['A']) { _piple.set_view_ward(VIEW_RIGHT, 1.0f); z_mip(); }
        if (_key['S']) { _piple.set_view_ward(VIEW_BACKWARD, 1.0f); z_mip(); }
        if (_key['D']) { _piple.set_view_ward(VIEW_LEFT, 1.0f); z_mip(); }
        if (_key['W']) { _piple.set_view_ward(VIEW_FORWARD, 1.0f); z_mip(); }
        if (_key['X']) _r_x += roateangle;
        if (_key['Y']) _r_y += roateangle;
        if (_key['Z']) _r_z += roateangle;
        if (_key[0x25]) _m_x -= speed;
		if (_key[0x26]) { _m_z += speed; z_mip();}
        if (_key[0x27]) _m_x += speed;
		if (_key[0x28]) { _m_z -= speed; z_mip(); }
    }

    void LhDevice::z_mip() {
        float z = length_float3((_piple.get_view_pos() - LhVertexFloat3(_m_x, _m_y, _m_z)));
        if (z >= 7.0f) {
            set_current_texture_uv(TEXTURE_LEVEL_128);
        }
        else if (7.0f > z && z >= 4.0f) {
            set_current_texture_uv(TEXTURE_LEVEL_256);
        }
        else if (4.0f > z) {
            set_current_texture_uv(TEXTURE_LEVEL_512);
        }

        set_current_uv(get_current_texutre_uv_buffers(), get_current_texture_uv_size());
        set_view(&_piple.get_view_pos());
    }

    void LhDevice::bind_vertex(const float* vertex, const unsigned int* vertex_color, const float* vertex_uv, const int vertex_size) {
        assert(nullptr != vertex);
        assert(vertex_size > 0);
        
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_rotate(0.0f, 0.0f, 0.0f);
        _piple.set_worldpos(0.0f, 0.0f, 0.0f);
        _piple.set_camera_pos(LhVertexFloat3(0.0f, 0.0f, -4.0f),
            LhVertexFloat3(0, 0, 1),
            LhVertexFloat3(0, 1, 0));
        PersProjInfo per(60.0f, static_cast<float>(get_width()), static_cast<float>(get_height()), 1.0f, 100.0f);
        _piple.set_perspective_proj(per);
        set_view(&_piple.get_view_pos());

		update_vertex(vertex, vertex_color, vertex_uv, vertex_size);
		z_mip();
    }

    bool LhDevice::get_pos(LhVertexFloat4& f4, LhVertexFloat3 f3) {
		LhVertexFloat4 model = _piple.transformation_in_mvp(f3);
        /*if (!_piple.transformation_cvv(model)) {
            f4 = model;
            return false;
        }*/

        f4 = _piple.transformation_homogeneous(model);
		f4.set_w(model.get_w());
        return true;
    }

	void LhDevice::draw_croe() {
		if (_rotate) {
			_r_x += 0.5f;
			_r_y += 0.5f;
			_r_z += 0.5f;
		}
		_piple.set_rotate(_r_x, _r_y, _r_z);
		_piple.set_worldpos(_m_x, _m_y, _m_z);
		_piple.get_wvp();
		switch (_render_state) {
		case LH_LINES:
			draw_line();
			break;
		case LH_LINES_LOOP:
			draw_line(true);
			break;
		case LH_TRIANGLES:
			draw_triangles(lh_color(255, 0, 0));
			break;
		case LH_TRIANGLES_FILL:
			draw_triangles_fill();
			break;
		case LH_TRIANGLES_TEXTURE_FILL:
			draw_trangles_texture_fill();
			break;
		case LH_TEST:
			break;
		}
	}

    void LhDevice::draw() {
        if (LH_OFF_DRAW == _render_state) {
            return;
        }

		clear_buffer();
		clear_deep();

        keyboard_update();
		draw_floor();
		draw_croe();

        if (_timer.count_time()) {
            _fps = _draw_counts;
            _draw_cost_time = 1.0f / _fps;
            _draw_counts = 0.0f;
        }
        _draw_counts++;
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
		const float* v = get_vertex_buffers();
		const unsigned int* colors = get_vertex_color_buffers();
		const int counts = 3 * get_vertex_buffers_size();
        draw_triangles_color(v, colors, counts);
	}

    float floor_v[] = {
        -1.0f, -1.0f, -0.5f,//b
        1.0f, -1.0f, -0.5f,//c
        -1.0f, 1.0f, -0.5f,//a

        1.0f, -1.0f, -0.5f,//c
        1.0f, 1.0f, -0.5f,//d
        -1.0f, 1.0f, -0.5f,//a
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
		if (!_floor) {
			return;
		}
        /*if (get_eyes_pos().get_z() < 1.6f && -1.5f < get_eyes_pos().get_z()) {
            return;
        }*/

        _piple.set_rotate(45.0f, 0, 0.0f);
        _piple.set_sale(1.0f, 1.0f, 1.0f);
        _piple.set_worldpos(0, -1.0f, 1.0f);
        _piple.get_wvp();
        const float* v = floor_v;
        const float* uv = floor_uv;
        int counts = 18;
		set_current_uv(get_floor(), 512);
        
        int uv_count = 0;
        std::vector<VertexColor> triangles;
        for (int i = 0; i < counts; i += 9, uv_count += 6) {
            LhVertexFloat4 p1 = _piple.transformation_in_mvp(LhVertexFloat3(v[i], v[i + 1], v[i + 2]));
            LhVertexFloat4 p2 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5]));
            LhVertexFloat4 p3 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]));

            LhVertexFloat3 v1(p1.get_x(), p1.get_y(), p1.get_z());
            LhVertexFloat3 v2(p2.get_x(), p2.get_y(), p2.get_z());
            LhVertexFloat3 v3(p3.get_x(), p3.get_y(), p3.get_z());
            VertexColor vc1(v1, TextureUV(uv[uv_count], uv[uv_count + 1]), 1.0f / p1.get_w());
            VertexColor vc2(v2, TextureUV(uv[uv_count + 2], uv[uv_count + 3]), 1.0f / p2.get_w());
            VertexColor vc3(v3, TextureUV(uv[uv_count + 4], uv[uv_count + 5]), 1.0f / p3.get_w());
            clip_triangle(triangles, &vc1, &vc2, &vc3);

        }

        for (std::vector<VertexColor>::iterator iter = triangles.begin();
            iter != triangles.end();
            iter += 3) {
            draw_triangle(*iter, *(iter + 1), *(iter + 2), true);
        }

		set_current_uv(get_current_texutre_uv_buffers(), get_current_texture_uv_size());
    }

    void LhDevice::draw_grid() {
        if (!_grid) {
            return;
        }

		_piple.set_rotate(0, 0, 0);
		_piple.set_worldpos(0, 0, 0);
		_piple.get_wvp();

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
            //draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);

			VertexColor v1(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), TextureUV(), 1.0f / p1.get_w());
			VertexColor v2(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), TextureUV(), 1.0f / p2.get_w());
			//draw_3dline(v1, v2);
        }
        for (float j = y_begin; j <= y_end; j += 0.15) {
            LhVertexFloat4 p1;
            LhVertexFloat4 p2;
            get_pos(p1, LhVertexFloat3(-value, j, z));
            get_pos(p2, LhVertexFloat3(value, j, z));
			VertexColor v1(LhVertexFloat3(p1.get_x(), p1.get_y(), p1.get_z()), TextureUV(), 1.0f / p1.get_w());
			VertexColor v2(LhVertexFloat3(p2.get_x(), p2.get_y(), p2.get_z()), TextureUV(), 1.0f / p2.get_w());
			//draw_3dline(v1, v2);
        }
    }
    void LhDevice::draw_triangles(lh_color lc) {
        const float* v = get_vertex_buffers();
        const int counts = 3 * get_vertex_buffers_size();
        std::vector<VertexColor> triangles;
        for (int i = 0; i < counts; i += 9) {
            LhVertexFloat4 p1 = _piple.transformation_in_mvp(LhVertexFloat3(v[i], v[i + 1], v[i + 2]));
            LhVertexFloat4 p2 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5]));
            LhVertexFloat4 p3 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]));
            LhVertexFloat3 v1(p1.get_x(), p1.get_y(), p1.get_z());
            LhVertexFloat3 v2(p2.get_x(), p2.get_y(), p2.get_z());
            LhVertexFloat3 v3(p3.get_x(), p3.get_y(), p3.get_z());
            clip_triangle(triangles,
                &VertexColor(v1, lc, TextureUV(), 1.0f / p1.get_w()),
                &VertexColor(v2, lc, TextureUV(), 1.0f / p2.get_w()),
                &VertexColor(v3, lc, TextureUV(), 1.0f / p3.get_w()));
            /*
            draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p2.get_x(), (int)p2.get_y(), color);
            draw_line((int)p2.get_x(), (int)p2.get_y(), (int)p3.get_x(), (int)p3.get_y(), color);
            draw_line((int)p1.get_x(), (int)p1.get_y(), (int)p3.get_x(), (int)p3.get_y(), color);*/
        }

        int color = lc.get_t<int>();
        for (std::vector<VertexColor>::iterator iter = triangles.begin();
            iter != triangles.end();
            iter += 3) {
            VertexColor& v1 = *iter;
            VertexColor& v2 = *(iter + 1);
            VertexColor& v3 = *(iter + 2);
            if (!toscreen(v1, v2, v3)) {
                continue;
            }
            draw_line(v1.postion.get_x(), v1.postion.get_y(), v2.postion.get_x(), v2.postion.get_y(), color);
            draw_line(v2.postion.get_x(), v2.postion.get_y(), v3.postion.get_x(), v3.postion.get_y(), color);
            draw_line(v1.postion.get_x(), v1.postion.get_y(), v3.postion.get_x(), v3.postion.get_y(), color);
        }
    }

    void LhDevice::set_render_state(LHRENDER_STATE state) {
        _render_state = state;
    }

    void LhDevice::update_texture(unsigned char* texture_datas, int texture_size) {
        set_texture(texture_datas, texture_size);
    }

	bool LhDevice::front(LhVertexFloat3& v1, LhVertexFloat3& v2, LhVertexFloat3& v3, LhVertexFloat3& look) {
		//return true;
		LhVertexFloat3 n1 = v2 - v1;
		LhVertexFloat3 n2 = v3 - v2;
		LhVertexFloat3 normal = cross(n1, n2);
		normalize(normal);
		if (_front) {
			if (!backface_culling(normal, look)) {
				return false;
			}
		}
		else {
			if (backface_culling(normal, look)) {
				return false;
			}
		}
		return true;
	}

	void LhDevice::draw_trangles_texture_fill() {
		if (!_piple.can_draw()) {
			return;
		}
		const float* v = get_vertex_buffers();
		const unsigned int* colors = get_vertex_color_buffers();
		const int counts = 3 * get_vertex_buffers_size();
		const float* uv = ger_current_uv();
        draw_triangles_uv(v, uv, counts);
	}

    void LhDevice::draw_triangles_color(const float* v, const unsigned int* colors, const int counts) {
        int uv_count = 0;
        LhVertexFloat3 look = _piple.get_view_dir();
        std::vector<VertexColor> triangles;
        for (int i = 0; i < counts; i += 9, uv_count += 6) {
            LhVertexFloat4 p1 = _piple.transformation_in_mvp(LhVertexFloat3(v[i], v[i + 1], v[i + 2]));
            LhVertexFloat4 p2 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5]));
            LhVertexFloat4 p3 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]));

            LhVertexFloat3 v1(p1.get_x(), p1.get_y(), p1.get_z());
            LhVertexFloat3 v2(p2.get_x(), p2.get_y(), p2.get_z());
            LhVertexFloat3 v3(p3.get_x(), p3.get_y(), p3.get_z());
            if (front(v1, v2, v3, look)) {
                lh_color c1((float)colors[i], (float)colors[i + 1], (float)colors[i + 2]);
                lh_color c2((float)colors[i + 3], (float)colors[i + 4], (float)colors[i + 5]);
                lh_color c3((float)colors[i + 6], (float)colors[i + 7], (float)colors[i + 8]);
                clip_triangle(triangles,
                    &VertexColor(v1, c1, TextureUV(), 1.0f / p1.get_w()),
                    &VertexColor(v2, c2, TextureUV(), 1.0f / p2.get_w()),
                    &VertexColor(v3, c3, TextureUV(), 1.0f / p3.get_w()));
            }

        }

        for (std::vector<VertexColor>::iterator iter = triangles.begin();
            iter != triangles.end();
            iter += 3) {
            draw_triangle(*iter, *(iter + 1), *(iter + 2));
        }
    }

    void LhDevice::draw_triangles_uv(const float* v, const float* uv, const int counts) {
        int uv_count = 0;
        LhVertexFloat3 look (0.0f, 0.0f, 1.0f);// = _piple.get_view_dir();
        std::vector<VertexColor> triangles;
        for (int i = 0; i < counts; i += 9, uv_count += 6) {
            LhVertexFloat4 p1 = _piple.transformation_in_mvp(LhVertexFloat3(v[i], v[i + 1], v[i + 2]));
            LhVertexFloat4 p2 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5]));
            LhVertexFloat4 p3 = _piple.transformation_in_mvp(LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]));

            LhVertexFloat3 v1(p1.get_x(), p1.get_y(), p1.get_z());
            LhVertexFloat3 v2(p2.get_x(), p2.get_y(), p2.get_z());
            LhVertexFloat3 v3(p3.get_x(), p3.get_y(), p3.get_z());
            if (front(v1, v2, v3, look)) {
                VertexColor vc1(v1, TextureUV(uv[uv_count], uv[uv_count + 1]), 1.0f / p1.get_w());
                VertexColor vc2(v2, TextureUV(uv[uv_count + 2], uv[uv_count + 3]), 1.0f / p2.get_w());
                VertexColor vc3(v3, TextureUV(uv[uv_count + 4], uv[uv_count + 5]), 1.0f / p3.get_w());
                clip_triangle(triangles, &vc1, &vc2, &vc3);
            }

        }

        for (std::vector<VertexColor>::iterator  iter = triangles.begin();
            iter != triangles.end();
            iter += 3) {
            draw_triangle(*iter, *(iter + 1), *(iter + 2), true);
        }
    }

}
