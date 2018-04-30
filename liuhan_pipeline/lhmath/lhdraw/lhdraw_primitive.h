#pragma once
//#include <functional>
#include "../lhmath_common.h"
#include "lhlights.h"
#include "lhclip.h"

namespace lh_pipeline {

	class LhDrawPrimitive
	{
	public:
		LhDrawPrimitive();
		~LhDrawPrimitive();
	public:
		void set_buffer(int w, int h, void* pbits);
		void set_view(LhVertexFloat3* view);
		void draw_line(int x1, int y1, int x2, int y2, lh_color c);
		void draw_triangle_line(int x1, int y1, int x2, int y2, int x3, int y3, lh_color color);
		void draw_triangle(VertexColor& v1, VertexColor& v2, VertexColor& v3, bool use_uv = false);
		void draw_3dline(LhVertexFloat3& left, LhVertexFloat3& right, lh_color c);
		void clip_triangle(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir);
	protected:
		void clear_deep();
		void set_current_uv(unsigned char* uv, int  uv_size);
		void setpixel(int x, int y, lh_color color);
		void enablelight();

	private:
		bool deeptest(int x, int y, float w);
		void setpixel(float x, float y, lh_color color);
		void setpixtel(int x, int y, unsigned int);
		void draw_interp_texture_scanline(VertexColor left, VertexColor right, LhVertexFloat3 normal, bool = false);
		void set_clip_window(float x_min, float y_min, float x_max, float y_max);
		void line_dda(int x1, int y1, int x2, int y2, lh_color color);
		void line_bresenham(float x1, float y1, float x2, float y2, lh_color color);
		void line(int x1, int y1, int x2, int y2, lh_color c);
		void top_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool = false);
		void bottom_triangle(VertexColor v1, VertexColor v2, VertexColor v3, LhVertexFloat3 normal, bool = false);
		bool clip(VertexColor& v1, VertexColor& v2, VertexColor& v3);
		float window_to_view(float pos, float length);
		int lh_min(int x, int min, int max);
		LhVertexFloat3 get_normal(LhVertexFloat3 a, LhVertexFloat3 b);
		VertexColor insert_lerp(VertexColor& v1, VertexColor& v2, float t);
		VertexColor interp_step(const VertexColor& left, const VertexColor& right);

	private:
		int _width = 0;
		int _height = 0;
		int _current_uv_size = 0;
		int _max_uv_size = 0;
		unsigned int *_current_uv_texture_datas = nullptr;
		unsigned char *_frame_buffers = nullptr;
		float* _frame_deep_buffers = nullptr;
		float _x_min_clip = 0;
		float _y_min_clip = 0;
		float _x_max_clip = 0;
		float _y_max_clip = 0;
		float _z_near_clip = 1.0;
		float _z_far_clip = 100.0;
		bool _deep_test = true;

		LhClip _clip;
		LhLights _light;
		LhVertexFloat3* _view;
	};
}
