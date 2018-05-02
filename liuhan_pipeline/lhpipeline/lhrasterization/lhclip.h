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
		void set_clip_region(CLIPOPTICVERTEBRAFACE, float, float, float, float, float, float);
		void set_z_view(float z_view);
		void triangle_clip(std::vector<VertexColor>&, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		bool backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir);
	private:
		int outside(LhVertexInt3& sign, LhVertexFloat3& v1, LhVertexFloat3& v2, LhVertexFloat3& v3);//判断三角形是否整体在视椎体之外
		int one_triangle_z(std::vector<VertexColor>&, LhVertexInt3& sign, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int two_triangle_z(std::vector<VertexColor>&, LhVertexInt3& sign, VertexColor* v1, VertexColor* v2, VertexColor* v3);
		int signcheck_x(float v, float z);
		int signcheck_y(float v, float z);
		int signcheck_z(float z);
	private:
		float _min_x;
		float _min_y;
		float _max_x;
		float _max_y;
		float _near_z;
		float _far_z;
		float _z_tan_x = 0.0f;
		float _z_tan_y = 0.0f;
		float _z_view = 0.0f;
		CLIPOPTICVERTEBRAFACE _clip_face;
	};
}