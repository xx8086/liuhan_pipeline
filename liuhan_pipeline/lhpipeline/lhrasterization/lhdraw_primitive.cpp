#include "lhdraw_primitive.h"
#include "lhtools.inl"
#include <cmath>
#include <memory.h>

namespace lh_pipeline {
#define fixp16_shift 16
#define fixp28_shift 28
#define use_1_z 1

	//static unsigned int texture[512][512] = {0};
	//void init_texture() {
	//    int i, j;
	//    for (j = 0; j < 512; j++) {
	//        for (i = 0; i < 512; i++) {
	//            int x = i / 32, y = j / 32;
	//            texture[j][i] = ((x + y) & 1) ? 0x00ff00 : 0xff0000;
	//        }
	//    }

	//}


	LhDrawPrimitive::LhDrawPrimitive()
	{
		//init_texture();
	}

	LhDrawPrimitive::~LhDrawPrimitive()
	{
		if (nullptr != _frame_deep_buffers) {
			delete[] _frame_deep_buffers;
			_frame_deep_buffers = nullptr;
		}
	}
	void LhDrawPrimitive::enablelight() {
		if (_light.is_visible()) {
			_light.invisible();
		}
		else {
			_light.visible();
		}
	}
	void LhDrawPrimitive::set_buffer(int w, int h, void* pbits) {
		if (_width*_height < w) {
			delete[] _frame_deep_buffers;
		}
		_frame_deep_buffers = new float[w*h];

		_width = w;
		_height = h;
		_z_far_clip = 1.0f;
		_z_near_clip = -1.0f;
		_frame_buffers = static_cast<unsigned char*>(pbits);
		set_clip_window(0.0f, 0.0f, (float)w, (float)h);

		_light.set_type(LIGHT_TYPE_POINT, 1);
		_light.set_light_color(lh_color(0.0f, 0.0f, 255.0f), LhVertexFloat3(0.0f, 0.0f, 1.0f), 0.05f, 0.3f, 0.1f);
		_light.set_point(LhVertexFloat3(1.0f, 1.0f, -3.0f), 1.0f, 0.09f, 0.032f);
		_clip.set_clip_region(CLIP_ALL_FACE, 0, 0, w, h, _z_near_clip, _z_far_clip);
	}

	void LhDrawPrimitive::set_view(LhVertexFloat3* view) {
		_view = view;
		_clip.set_z_view(view->get_z());
	}


	void LhDrawPrimitive::clear_deep() {
		if (nullptr != _frame_deep_buffers) {
			memset(_frame_deep_buffers, 0, sizeof(float) * _width * _height);
		}
	}

	bool LhDrawPrimitive::deeptest(int x, int y, float w) {
		if (!_deep_test) {
			return true;
		}

		int deep_pos = y * _width + x + 1;
		if (/*z < _z_far_clip &&
			_z_near_clip < z &&*/
			deep_pos >= 0 &&
			deep_pos < _width * _height &&
			_frame_deep_buffers[deep_pos] < w
			) {
			_frame_deep_buffers[deep_pos] = w;
		}
		else {
			return false;
		}

		return true;
	}

	void LhDrawPrimitive::setpixel(float x, float y, lh_color color) {
		setpixel((int)x, (int)y, color);
	}

	void LhDrawPrimitive::setpixel(int x, int y, lh_color color) {
		if (y >= _y_max_clip ||
			y < _y_min_clip ||
			x >= _x_max_clip ||
			x < _x_min_clip) {
			return;
		}

		int pos = (y * _width + x + 1) * 4;
		_frame_buffers[pos] = (unsigned char)color.blue;//b
		_frame_buffers[pos + 1] = (unsigned char)color.green;//g
		_frame_buffers[pos + 2] = (unsigned char)color.red;//r
		_frame_buffers[pos + 3] = (unsigned char)color.alph;//a
	}
	//void setpixtel(int x, int y, unsigned int);
	void LhDrawPrimitive::setpixtel(int x, int y, unsigned int color) {
		if (y >= _y_max_clip ||
			y < _y_min_clip ||
			x >= _x_max_clip ||
			x < _x_min_clip) {
			return;
		}
		//unsigned int* texture = (unsigned int*)_current_uv_texture_datas;
		unsigned int* des = (unsigned int*)_frame_buffers;
		des[y * _width + x] = color;// texture[v * _current_uv_size + u];
	}
	/*
	dx = (x(i+1) - x(i)) / (t(i+1) - t(i))
	dy = (y(i+1) - y(i)) / (t(i+1) - t(i))
	dm = max(|dx|, |dy|)
	∴
	x(i+1) = x(i) + dx / dm
	y(i+1) = y(i) + dy / dm

	|k| >= 1 (dy > dx, dm = dy)
	.P2
	/|
	/ |
	/  |
	/   |
	/    |
	/     | 单位长度(1) dy / dm
	/      |
	/       |
	/        |
	.P1--------
	dx / dm

	|k| < 1 (dy < dx, dm = dx)
	.P2
	/  |
	/      |
	/          |
	/              |
	/                   | dy / dm
	/                        |
	/                             |
	/                                  |
	.P1-------------------------------------
	单位长度(1) dx / dm
	*/
	void LhDrawPrimitive::line_dda(int x1, int y1, int x2, int y2, lh_color color) {
		int dm = 0;
		int tx = abs(x2 - x1);
		int ty = abs(y2 - y1);
		dm = (tx >= ty ? tx : ty);

		float x = x1 + 0.5f;
		float y = y1 + 0.5f;
		float dx = (float)tx / dm;
		float dy = (float)ty / dm;

		for (int i = 0; i < dm; i++) {
			setpixel(x, y, color);
			x += dx;
			y += dy;
		}
	}

	/*
	-------T----/--
	|      |  /   |
	|      P      |
	|   /  |      |
	|/     |      |
	O------S-------
	(xi, yi)

	y = kx + b
	直线与单位网格边线段TS相交与P(x, y)点，P点下第一个网格点S，P点上第一个点T，
	s = |PS|, t = |PT|
	O点坐标(xi, yi)
	S点坐标(xi + 1, yi)
	T点坐标(xi + 1, yi + 1)
	P = s > t ? T : P// T.S谁距离P点进选谁作为P点

	(yp) = k*(xp) + b  = k*(xi + 1) + b
	s - t = (yp - yi) - (yi + 1 - yp) = 2*yp - 2*yi - 1
	= 2*k(xi + 1) + 2*b - 2*yi -1
	dx = x2 - x1
	dy = y2 - y1
	k = dy / dx

	∴ dx*(s-t) = 2*dy*(xi + 1) + dx*(2*b - 2*yi -1)
	= 2*dy*xi - 2*dx*yi + 2*dy + dx(2*b - 1)
	令常数 C = 2*dy + dx(2*b - 1)
	令di = dx*(s-t)//当dx>0时di与s-t同符号
	则di = 2*dy*xi - 2*dx*yi + C
	d(i+1) =  2*dy*x(i+1) - 2*dx*y(i+1) + C
	d(i+1) - di = 2*dy*(x(i+1) - xi) - 2*dx(y(i+1) - yi)
	= 2*dy - 2*dx(y(i+1) - yi)
	// x(i+1) - xi == |OS| == 1; y(i+1) - yi == |PS| <= 1
	∴ d(i+1) = di + 2*dy - 2*dx(y(i+1) - yi)
	当di>=0时s-t>=0,P = T,所以y(i+1) - yi == 1
	即d(i+1) = di + 2*(dy - dx)
	当di<0时s-t<0,P = S,所以y(i+1) - yi == 0
	即d(i+1) = di + 2*dy
	*/
	void LhDrawPrimitive::line_bresenham(float x1, float y1, float x2, float y2, lh_color color) {
		setpixel(x1, y1, color);
		int dx = (int)abs(x2 - x1);
		int dy = (int)abs(y2 - y1);
		if (dx == 0 && dy == 0) {
			return;
		}

		bool b_coorde_transf = false;
		if (dx < dy) {//斜率k > 1
			b_coorde_transf = true;
			swap_vaue(x1, y1);//交换x,y坐标，使0 <= |k| <= 1
			swap_vaue(x2, y2);
			swap_vaue(dx, dy);
		}

		int tx = (x2 - x1) > 0 ? 1 : -1;
		int ty = (y2 - y1) > 0 ? 1 : -1;
		int curx = (int)x1;
		int cury = (int)y1;
		int dS = 2 * dy;
		int dT = 2 * (dy - dx);
		int d = dS - dx;
		while (curx <= x2) {
			if (d < 0) {
				d += dS;
			}
			else {
				cury += ty;
				d += dT;
			}

			b_coorde_transf ? setpixel(cury, curx, color) : setpixel(curx, cury, color);
			curx += tx;
		}
	}

	// 绘制线段
	void LhDrawPrimitive::line(int x1, int y1, int x2, int y2, lh_color c) {
		int x, y, rem = 0;
		if (x1 == x2 && y1 == y2) {
			setpixel(x1, y1, c);
		}
		else if (x1 == x2) {
			int inc = (y1 <= y2) ? 1 : -1;
			for (y = y1; y != y2; y += inc) setpixel(x1, y, c);
			setpixel(x2, y2, c);
		}
		else if (y1 == y2) {
			int inc = (x1 <= x2) ? 1 : -1;
			for (x = x1; x != x2; x += inc) setpixel(x, y1, c);
			setpixel(x2, y2, c);
		}
		else {
			int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
			int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
			if (dx >= dy) {
				if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
				for (x = x1, y = y1; x <= x2; x++) {
					setpixel(x, y, c);
					rem += dy;
					if (rem >= dx) {
						rem -= dx;
						y += (y2 >= y1) ? 1 : -1;
						setpixel(x, y, c);
					}
				}
				setpixel(x2, y2, c);
			}
			else {
				if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
				for (x = x1, y = y1; y <= y2; y++) {
					setpixel(x, y, c);
					rem += dx;
					if (rem >= dy) {
						rem -= dy;
						x += (x2 >= x1) ? 1 : -1;
						setpixel(x, y, c);
					}
				}
				setpixel(x2, y2, c);
			}
		}
	}


	void LhDrawPrimitive::draw_line(int x1, int y1, int x2, int y2, lh_color c) {
		line(x1, y1, x2, y2, c);
	}

	void LhDrawPrimitive::set_clip_window(float x_min, float y_min, float x_max, float y_max) {
		_x_min_clip = x_min;
		_y_min_clip = y_min;
		_x_max_clip = x_max;
		_y_max_clip = y_max;
	}

	void LhDrawPrimitive::draw_triangle_line(int x1, int y1, int x2, int y2, int x3, int y3, lh_color color) {
		if (!_draw_trangle_line) {
			return;
		}
		draw_line(x1, y1, x2, y2, color);
		draw_line(x1, y1, x2, y2, color);
		draw_line(x2, y2, x3, y3, color);
		draw_line(x1, y1, x3, y3, color);
	}

	bool LhDrawPrimitive::clip(VertexColor& v1, VertexColor& v2, VertexColor& v3) {
		if (v2.postion.get_y() < v1.postion.get_y()) {
			swap_vaue(v1, v2);
		}
		if (v3.postion.get_y() < v1.postion.get_y()) {
			swap_vaue(v1, v3);
		}
		if (v3.postion.get_y() < v2.postion.get_y()) {
			swap_vaue(v2, v3);
		}//v3.y > v2.y > v1.y

		if (/*v1.postion.get_y() >= _y_max_clip || v3.postion.get_y() <= _y_min_clip ||*/
			(v1.postion.get_x() < _x_min_clip && v2.postion.get_x() < _x_min_clip && v3.postion.get_x() < _x_min_clip) ||
			(v1.postion.get_x() > _x_max_clip && v2.postion.get_x() > _x_max_clip && v3.postion.get_x() > _x_max_clip)) {
			return false;
		}

		return true;
	}

	void LhDrawPrimitive::clip_triangle(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		_clip.triangle_clip(triangles, v1, v2, v3);
	}

	bool LhDrawPrimitive::backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir) {
		return _clip.backface_culling(normal, dir);
	}

	void LhDrawPrimitive::draw_triangle(VertexColor& v1, VertexColor& v2, VertexColor& v3, bool use_uv) {
		if (!clip(v1, v2, v3)) {
			return;
		}

		LhVertexFloat3 normal = get_normal(v1.postion - v2.postion, v1.postion - v3.postion);
		normalize(normal);
		if (v2.postion.get_y() == v1.postion.get_y()) {//平顶
			top_triangle(v1, v2, v3,
				normal,
				use_uv);
			draw_triangle_line((int)v1.postion.get_x(), (int)v1.postion.get_y(),
				(int)v2.postion.get_x(), (int)v2.postion.get_y(),
				(int)v3.postion.get_x(), (int)v3.postion.get_y(),
				lh_color(255, 0, 0));
		}
		else if (v3.postion.get_y() == v2.postion.get_y()) {
			bottom_triangle(v1, v2, v3,
				normal,
				use_uv);
			draw_triangle_line((int)v1.postion.get_x(), (int)v1.postion.get_y(),
				(int)v2.postion.get_x(), (int)v2.postion.get_y(),
				(int)v3.postion.get_x(), (int)v3.postion.get_y(),
				lh_color(255, 0, 0));
		}
		else {
			assert(v2.postion.get_y() > v1.postion.get_y());
			float dy = /*v3.postion.get_y() == v1.postion.get_y() ? 0.0f : */1.0f / (v3.postion.get_y() - v1.postion.get_y());
			float t = (v2.postion.get_y() - v1.postion.get_y()) / (v3.postion.get_y() - v1.postion.get_y());
			VertexColor interp_v = lerp(v1, v3, t);
			bottom_triangle(v1, interp_v, v2,
				normal,
				use_uv);
			top_triangle(interp_v, v2, v3,
				normal,
				use_uv);

			draw_triangle_line((int)v1.postion.get_x(), (int)v1.postion.get_y(),
				(int)interp_v.postion.get_x(), (int)interp_v.postion.get_y(),
				(int)v2.postion.get_x(), (int)v2.postion.get_y(),
				lh_color(0, 255, 0));
			draw_triangle_line(
				(int)interp_v.postion.get_x(), (int)interp_v.postion.get_y(),
				(int)v2.postion.get_x(), (int)v2.postion.get_y(),
				(int)v3.postion.get_x(), (int)v3.postion.get_y(),
				lh_color(0, 0, 255));
		}
	}

	void LhDrawPrimitive::top_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool use_uv) {
		if ((int)v1.postion.get_y() == (int)v3.postion.get_y()) {
			return;
		}

		if (v2.postion.get_x() < v1.postion.get_x()) {
			swap_vaue(v1, v2);
		}
		float dy = /*v3.postion.get_y() == v1.postion.get_y() ? 0.0f : */1.0f / (v3.postion.get_y() - v1.postion.get_y());
		float fy1 = v1.postion.get_y();
		float fy3 = v3.postion.get_y();
		int iy1 = FloatToInt(fy1);
		int iy3 = FloatToInt(fy3);
		VertexColor v_left_1 = v1;
		VertexColor v_right_1 = v2;
		VertexColor v_left = v3;
		VertexColor v_right = v3;
		if (iy1 < _y_min_clip) {
			iy1 = _y_min_clip;
			float t = (_y_min_clip - fy1) * dy;
			v_left_1 = lerp(v1, v3, t);
			v_right_1 = lerp(v2, v3, t);
			dy = 1.0f / (v3.postion.get_y() - v_left_1.postion.get_y());
		}
		if (iy3 > _y_max_clip) {
			iy3 = _y_max_clip;
			float t = (_y_max_clip - fy1) * dy;
			v_left = lerp(v1, v3, t);
			v_right = lerp(v2, v3, t);
			dy = 1.0f / (v_left.postion.get_y() - v1.postion.get_y());
		}
		
		for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
			float t = ((float)y_loop - iy1) * dy;
			VertexColor left = lerp(v_left_1, v_left, t);
			VertexColor right = lerp(v_right_1, v_right, t);
			draw_interp_texture_scanline(left, right, normal, use_uv);
		}
	}


	VertexColor LhDrawPrimitive::insert_lerp(VertexColor& v1, VertexColor& v2, float t) {
		if (v1.postion.get_x() > v2.postion.get_x()) {
			swap_vaue(v1, v2);
		}
		return lerp(v1, v2, t);
	}
	void LhDrawPrimitive::bottom_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool use_uv) {
		if ((int)v1.postion.get_y() == (int)v2.postion.get_y()) {
			return;
		}

		if (v3.postion.get_x() < v2.postion.get_x()) {
			swap_vaue(v2, v3);
		}
		float dy = /*v2.postion.get_y() == v1.postion.get_y() ? 0.0f : */1.0f / (v2.postion.get_y() - v1.postion.get_y());
		float fy1 = v1.postion.get_y();// ceil(v1.postion.get_y());
		float fy3 = v3.postion.get_y();// ceil(v3.postion.get_y()) - 1;
		int iy1 = FloatToInt(fy1);
		int iy3 = FloatToInt(fy3);
		VertexColor v_left_1 = v1;
		VertexColor v_right_1 = v1;
		VertexColor v_left = v2;
		VertexColor v_right = v3;

		if (iy1 < _y_min_clip) {
			float t = (_y_min_clip - iy1) * dy;
			v_left_1 = lerp(v1, v2, t);
			v_right_1 = lerp(v1, v3, t);
			dy = 1.0f / (v2.postion.get_y() - v_left_1.postion.get_y());
			iy1 = _y_min_clip;
		}
		if (iy3 > _y_max_clip) {
			float t = (_y_max_clip - iy3) * dy;
			v_left = lerp(v1, v2, t);
			v_right = lerp(v1, v3, t);
			dy = 1.0f / (v_left.postion.get_y() - v1.postion.get_y());
			iy3 = _y_max_clip;
		}

		for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
			float t = ((float)y_loop - iy1) * dy;
			VertexColor left = lerp(v_left_1, v_left, t);
			VertexColor right = lerp(v_right_1, v_right, t);
			draw_interp_texture_scanline(left, right, normal, use_uv);
		}
	}

	VertexColor LhDrawPrimitive::interp_step(const VertexColor& left, const VertexColor& right) {
		if (right.postion.get_x() == left.postion.get_x()) {
			return VertexColor();
		}
		float inv =  1.0f / (right.postion.get_x() - left.postion.get_x());
		return   (right - left) * inv;
	}


	void LhDrawPrimitive::draw_interp_texture_scanline(VertexColor left, VertexColor right, LhVertexFloat3 normal, bool use_uv) {
		int y = FloatToInt(left.postion.get_y());
		if (y < _y_min_clip || _y_max_clip < y) {
			return;
		}

		float x_begin = left.postion.get_x();
		float x_end = right.postion.get_x();
		float z_begin = left.postion.get_z();
		float z_end = right.postion.get_z();
		float dx = x_end == x_begin ? 0.0f : 1.0f / float(x_end - x_begin);
		if (z_begin <= z_end) {
			z_begin = z_begin < _z_near_clip ? _z_near_clip : z_begin;
			z_end = z_end > _z_far_clip ? _z_far_clip : z_end;
		}
		else {
			z_begin = z_begin > _z_far_clip ? _z_far_clip : z_begin;
			z_end = z_end < _z_near_clip ? _z_near_clip : z_end;
		}

		int x_clip_begin = FloatToInt(x_begin);// x_begin > _x_min_clip ? x_begin : _x_min_clip;
		int x_clip_end = FloatToInt(x_end);// x_end < _x_max_clip ? x_end : _x_max_clip;//裁剪
		VertexColor clip_left = left;
		if (x_begin < _x_min_clip) {
			x_clip_begin = _x_min_clip;
			clip_left = lerp(left, right, (_x_min_clip - x_begin) / (x_end - x_begin));
		}
		if (x_end > _x_max_clip) {
			x_clip_end = _x_max_clip;
			//clip_right = lerp(left, right, (_x_max_clip - x_begin) / (x_end - x_begin));
		}

		//VertexColor step = interp_step(left, right);//用step均值插值，每个单位步长uv会错位！！！！！！
		for (int i = x_clip_begin; i <= x_clip_end; i++) {
			float t = (i - x_begin) * dx;
			clip_left = lerp(left, right, t);
			float z = clip_left._rhw;
			float w = 1.0f / z;
			if (deeptest(i, y, z)) {
				unsigned int color = 0.0f;
				if (use_uv) {
					TextureUV pixtel = clip_left.uv * w;
					pixtel = pixtel * _max_uv_size;
					int iu = (int)(pixtel.u + 0.5f);
					int iv = (int)(pixtel.v + 0.5f);
					iu = lh_min(iu, 0, _max_uv_size);
					iv = lh_min(iv, 0, _max_uv_size);
					unsigned int* texture = (unsigned int*)_current_uv_texture_datas;
					color = texture[iv * _current_uv_size + iu];
					//clip_left.uv = clip_left.uv + step.uv;
				}
				else {
					color = (clip_left.color * w).get_t<unsigned int>();
					//clip_left.color = clip_left.color + step.color;
					//left.postion.set_z(left.postion.get_z() + step.postion.get_z());
				}
				//clip_left._rhw = clip_left._rhw + step._rhw;
				if (_light.is_visible()) {
					lh_color lightcolor = _light.get_pointlight(normal, *_view,
						LhVertexFloat3(window_to_view(float(i), float(_width)),
							window_to_view(float(y), float(_height)), z));
					lh_color c(color);
					lh_color h = c + lightcolor;
					h.check();
					color = h.get_t<unsigned int>();
				}
				setpixtel(i, y, color);
			}
		}
	}

	void LhDrawPrimitive::set_current_uv(unsigned char* uv, int  uv_size) {
		_current_uv_size = uv_size;
		_max_uv_size = uv_size - 1;
		_current_uv_texture_datas = (unsigned int*)(uv);
	}

	float LhDrawPrimitive::window_to_view(float pos, float length) {
		return 2 * (pos - length / 2) / length;
	}

	int LhDrawPrimitive::lh_min(int x, int min, int max) {
		return (x < min) ? min : ((x > max) ? max : x);
	}

	LhVertexFloat3 LhDrawPrimitive::get_normal(LhVertexFloat3 a, LhVertexFloat3 b) {
		LhVertexFloat3 n(0.0f, 0.0f, 0.0f);
		n = cross(a, b);
		return n;
	}

	void LhDrawPrimitive::draw_3dline(LhVertexFloat3& left, LhVertexFloat3& right, lh_color c) {
		;
	}
	void LhDrawPrimitive::set_draw_triangle_line() {
		_draw_trangle_line = !_draw_trangle_line;
	}
}