#pragma once

#include <vector>
#include "../lhmath_common.h"

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
		void set_clip_region(CLIPOPTICVERTEBRAFACE, float, float, float, float, float, float);
		void triangle_clip(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
	private:
		int outside(LhVertexInt3& sign, LhVertexFloat3& v1, LhVertexFloat3& v2, LhVertexFloat3& v3);//判断三角形是否整体在视椎体之外
		int one_triangle_z(std::vector<VertexColor>&, LhVertexInt3& sign, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_z(std::vector<VertexColor>&, LhVertexInt3& sign, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int signcheck(float v, float z);
		int signcheck_z(float z);
	private:
		float _min_x;
		float _min_y;
		float _max_x;
		float _max_y;
		float _near_z;
		float _far_z;
		float _z_factor;
		CLIPOPTICVERTEBRAFACE _clip_face;
	};
}