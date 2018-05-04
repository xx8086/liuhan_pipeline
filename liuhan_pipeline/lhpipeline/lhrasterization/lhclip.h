#pragma once

#include <vector>
#include "..\lhmath\lhmath_common.h"
#include "lhlinesegment.h"

typedef enum ClipOpticVertebraFace {
	CLIP_NEAR_FAR_FACE = 1,
	CLIP_LEFT_RIGHT_FACE = 2,
	CLIP_TOP_BOTTOM_FACE = 4,
	CLIP_ALL_FACE = (1 | 2 | 4),
}CLIPOPTICVERTEBRAFACE;

namespace lh_pipeline {
	class LhClip
	{
	public:
		LhClip();
		~LhClip();

	public:
        void triangle_clip_single_plane(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		void triangle_clip_plane(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		void triangle_clip(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir);
	private:
        void clip_left(std::vector<VertexColor>&, std::vector<VertexColor>&);
        void clip_right(std::vector<VertexColor>&, std::vector<VertexColor>&);
        void clip_top(std::vector<VertexColor>&, std::vector<VertexColor>&);
        void clip_bottom(std::vector<VertexColor>&, std::vector<VertexColor>&);
        void clip_near(std::vector<VertexColor>&, std::vector<VertexColor>&);
        void clip_far(std::vector<VertexColor>&, std::vector<VertexColor>&);

		bool on_x_space(std::vector<VertexColor>& triangles, 
			VertexColor* v1, VertexColor* v2, VertexColor* v3,
			LhVertexInt3 sign, int nums);
		void on_y_space(std::vector<VertexColor>& triangles,
			VertexColor* v1, VertexColor* v2, VertexColor* v3);
        void on_z_space(std::vector<VertexColor>& triangles,
            VertexColor* v1, VertexColor* v2, VertexColor* v3);
	private:
		int outside(LhVertexInt3& sign, float a, float b, float c);
		bool split_triangle_x(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool split_triangle_y(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool split_triangle_z(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);

		int one_triangle_x(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_x(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int one_triangle_y(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_y(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int one_triangle_z(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_z(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int signcheck(float v);
        int signcheck_than(float v, float plane);
        int signcheck_less(float v, float plane);
	private:
		float _z_tan_x = 0.0f;
		float _z_tan_y = 0.0f;
        LhLineSegment _lineoperator;
		CLIPOPTICVERTEBRAFACE _clip_face = CLIP_ALL_FACE;
	};
}