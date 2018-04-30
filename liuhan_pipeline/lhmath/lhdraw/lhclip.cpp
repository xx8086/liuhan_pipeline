#include "lhclip.h"
#include "lhtools.hpp"

namespace lh_pipeline {
#define CLIP_CODE_G   0x0001    // x > x_max
#define CLIP_CODE_L   0x0002    // x < x_min
#define CLIP_CODE_I   0x0004    // x_min < x < x_max

#define CLIP_CODE_NULL 0x0000

#define  OUT_SIDE_SIGNED(a, b, c) \
    if ((a == CLIP_CODE_G && \
         b == CLIP_CODE_G && \
         c == CLIP_CODE_G) || \
        (a == CLIP_CODE_L && \
         b == CLIP_CODE_L && \
         c == CLIP_CODE_L)) return 0; 


	LhClip::LhClip()
	{
	}


	LhClip::~LhClip()
	{
	}


	void LhClip::set_clip_region(CLIPOPTICVERTEBRAFACE clipface, float min_x, float min_y, float max_x, float max_y, float near_z, float far_z) {
		_clip_face = clipface;
		_min_x = min_x;
		_min_y = min_y;
		_max_x = max_x;
		_max_y = max_x;
		_near_z = near_z;
		_far_z = far_z;
		_z_factor = 0.5 * (max_x - min_x) * 1.0;
	}

	void LhClip::triangle_clip(std::vector<VertexColor>&triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		LhVertexInt3 sign;
		int triangle_nums = outside(sign, v1->postion, v2->postion, v3->postion);
		if (0 == triangle_nums) {
			return;
		}

		if (3 == triangle_nums) {
			triangles.push_back(*v1);
			triangles.push_back(*v2);
			triangles.push_back(*v3);
			return;
		}

		//检查是否有顶点在近截面之外
		if ((sign.get_x() | sign.get_y() | sign.get_z()) & CLIP_CODE_L) {
			VertexColor* p0;
			VertexColor* p1;
			VertexColor* p2;

			//1.找出位于内存的顶点
			if (CLIP_CODE_I == sign.get_x()) {
				p0 = v1;//1,2,3
				p1 = v2;
				p2 = v3;
			}
			else if (CLIP_CODE_I == sign.get_y()) {
				p0 = v2;//2,3,1
				p1 = v3;
				p2 = v1;
			}
			else {
				p0 = v3;//3,1,2
				p1 = v1;
				p2 = v2;
			}

			if (1 == triangle_nums) {
				one_triangle_z(triangles, sign, p0, p1, p2);
			}
			else if (2 == triangle_nums) {
				two_triangle_z(triangles, sign, p0, p1, p2);
			}
		}
	}

	int LhClip::two_triangle_z(std::vector<VertexColor>& triangles, LhVertexInt3& sign, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		float t;
		VertexColor v;
		v = *p1 - *p0;
		t = (_near_z - p0->postion.get_z()) / v.postion.get_z();
		VertexColor v0 = *p0 + v * t;
		v0.postion.set_z(_near_z);

		v = *p2 - *p0;
		t = (_near_z - p0->postion.get_z()) / v.postion.get_z();
		VertexColor temp_v0 = *p0 + v * t;
		//VertexColor temp_v1(v0);

		triangles.emplace_back(v0);
		triangles.emplace_back(*p2);
		triangles.emplace_back(*p1);

		triangles.emplace_back(temp_v0);
		triangles.emplace_back(*p2);
		triangles.emplace_back(v0);
		return 0;
	}

	int LhClip::one_triangle_z(std::vector<VertexColor>& triangles, LhVertexInt3& sign, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		//对每条边裁剪
		float t;
		VertexColor v;
		//对p0->p1边裁剪
		v = *p1 - *p0;
		t = (_near_z - p0->postion.get_z()) / v.postion.get_z();
		*p1 = *p0 + v * t;//使用交点覆盖原顶点
		p1->postion.set_z(_near_z);

		//对p0->p2边裁剪
		v = *p2 - *p0;
		t = (_near_z - p0->postion.get_z()) / v.postion.get_z();
		*p2 = *p0 + v * t;
		p2->postion.set_z(_near_z);

		//LhVertexFloat3 p01 = p1->postion - p0->postion;
		//LhVertexFloat3 p02 = p2->postion - p0->postion;
		//LhVertexFloat3 n = cross(p01, p02);

		triangles.emplace_back(*p0);
		triangles.emplace_back(*p1);
		triangles.emplace_back(*p2);
		return 1;
	}


	int LhClip::outside(LhVertexInt3& sign, LhVertexFloat3& v1, LhVertexFloat3& v2, LhVertexFloat3& v3) {
		/*if (CLIP_LEFT_RIGHT_FACE & _clip_face) {
			int s1 = signcheck(v1.get_x(), v1.get_z());
			int s2 = signcheck(v2.get_x(), v2.get_z());
			int s3 = signcheck(v3.get_x(), v3.get_z());
			OUT_SIDE_SIGNED(s1, s2, s3)
		}
		if (CLIP_TOP_BOTTOM_FACE & _clip_face) {
			int s1 = signcheck(v1.get_y(), v1.get_z());
			int s2 = signcheck(v2.get_y(), v2.get_z());
			int s3 = signcheck(v3.get_y(), v3.get_z());
			OUT_SIDE_SIGNED(s1, s2, s3)
		}*/

		int triangle_nums = 0;
		if (CLIP_NEAR_FAR_FACE & _clip_face) {
			int s1 = signcheck_z(v1.get_z());
			int s2 = signcheck_z(v2.get_z());
			int s3 = signcheck_z(v3.get_z());
			OUT_SIDE_SIGNED(s1, s2, s3);

			if (CLIP_CODE_I == s1)triangle_nums++;
			if (CLIP_CODE_I == s2)triangle_nums++;
			if (CLIP_CODE_I == s3)triangle_nums++;
			sign.set_x(s1);
			sign.set_x(s2);
			sign.set_x(s3);
		}

		return triangle_nums;
	}

	int LhClip::signcheck_z(float z) {
		int sign = 0;
		if (z > _far_z) {
			sign = CLIP_CODE_G;
		}
		else if (z < _near_z) {
			sign = CLIP_CODE_L;
		}
		else {
			sign = CLIP_CODE_I;
		}
		return sign;
	}

	int LhClip::signcheck(float v, float z) {
		int sign = 0;
		float z_test = z * _z_factor;
		if (v > z_test) {
			sign = CLIP_CODE_G;
		}
		else if (v < -z_test) {
			sign = CLIP_CODE_L;
		}
		else
		{
			sign = CLIP_CODE_I;
		}
		return sign;
	}


}