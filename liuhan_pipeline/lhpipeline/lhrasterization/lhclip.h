#pragma once

#include <vector>
#include "..\lhmath\lhmath_common.h"

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
		void triangle_clip(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir);
	private:
		int outside(LhVertexInt3& sign, float a, float b, float c);
		
		int split_triangle_x(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int split_triangle_y(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int split_triangle_z(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);

		int one_triangle_x(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_x(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int one_triangle_y(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_y(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int one_triangle_z(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_z(std::vector<VertexColor>&,  VertexColor* v1, VertexColor* v2, VertexColor* v3);
		
		int signcheck(float v);
	private:
		float _z_tan_x = 0.0f;
		float _z_tan_y = 0.0f;
		CLIPOPTICVERTEBRAFACE _clip_face = CLIP_ALL_FACE;
	};
}