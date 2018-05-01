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

	void LhClip::set_z_view(float z_view) {
		_z_view = z_view;
		_z_tan_x = 0.5f * (_max_x - _min_x) / (_near_z - z_view);
		_z_tan_y = 0.5f * (_max_y - _min_y) / (_near_z - z_view);
	}

	void LhClip::set_clip_region(CLIPOPTICVERTEBRAFACE clipface, float min_x, float min_y, float max_x, float max_y, float near_z, float far_z) {
		_clip_face = clipface;
		_min_x = min_x;
		_max_x = max_x;
		_min_y = min_y;
		_max_y = max_y;
		_near_z = near_z;
		_far_z = far_z;
	}

	bool LhClip::backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir) {
		return dot(normal, dir) > 0 ? false : true;//左手,大于零同向,不消除
	}

	void LhClip::triangle_clip(std::vector<VertexColor>&triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		v1->rhw();
		v2->rhw();
		v3->rhw();

		LhVertexInt3 sign;
		int points = outside(sign, v1->postion, v2->postion, v3->postion);
		if (0 == points) {//在视锥体内点的数量
			return;
		}

		if (3 == points) {
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

			//1.找出位于内侧的顶点
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

			if (1 == points) {
				one_triangle_z(triangles, sign, p0, p1, p2);
			}
			else if (2 == points) {
				two_triangle_z(triangles, sign, p0, p1, p2);
			}
		}
	}
	/*
	p0					p1
--------------------------
		p2	
	*/
	int LhClip::two_triangle_z(std::vector<VertexColor>& triangles, LhVertexInt3& sign, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		if (p1->postion.get_z() < p2->postion.get_z()) {
			swap_vaue<VertexColor* >(p1, p2);
		}
		float t;
		t = (_near_z - p2->postion.get_z()) / (p0->postion.get_z() - p2->postion.get_z());
		VertexColor near_p0 = lerp(*p2, *p0, t);

		t = (_near_z - p2->postion.get_z()) / (p1->postion.get_z() - p2->postion.get_z());
		VertexColor near_p1 = lerp(*p2, *p1, t);

		triangles.emplace_back(*p0);
		triangles.emplace_back(near_p0);
		triangles.emplace_back(*p1);

		triangles.emplace_back(near_p0);
		triangles.emplace_back(near_p1);
		triangles.emplace_back(*p1);
		return 2;
	}
	/*
		p0			
	--------------------------
	p1			P2
	*/
	int LhClip::one_triangle_z(std::vector<VertexColor>& triangles, LhVertexInt3& sign, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		//对每条边裁剪
		float t;
		VertexColor v;
		//对p0->p1边裁剪
		t = (_near_z - p1->postion.get_z()) / (p0->postion.get_z()  - p1->postion.get_z());
		*p1 = lerp(*p1 , *p0, t);
		//p1->postion.set_z(_near_z);

		//对p0->p2边裁剪
		t = (_near_z - p2->postion.get_z()) / (p0->postion.get_z() - p2->postion.get_z());
		*p2 = lerp(*p2, *p0, t);

		triangles.emplace_back(*p0);
		triangles.emplace_back(*p1);
		triangles.emplace_back(*p2);
		return 1;
	}

	int LhClip::outside(LhVertexInt3& sign, LhVertexFloat3& v1, LhVertexFloat3& v2, LhVertexFloat3& v3) {
		if (CLIP_LEFT_RIGHT_FACE & _clip_face) {
			int s1 = signcheck_x(v1.get_x(), v1.get_z());
			int s2 = signcheck_x(v2.get_x(), v2.get_z());
			int s3 = signcheck_x(v3.get_x(), v3.get_z());
			OUT_SIDE_SIGNED(s1, s2, s3)
		}
		if (CLIP_TOP_BOTTOM_FACE & _clip_face) {
			int s1 = signcheck_y(v1.get_y(), v1.get_z());
			int s2 = signcheck_y(v2.get_y(), v2.get_z());
			int s3 = signcheck_y(v3.get_y(), v3.get_z());
			OUT_SIDE_SIGNED(s1, s2, s3)
		}

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
			sign.set_y(s2);
			sign.set_z(s3);
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

	/*
	--------------z--------------(max_x)
	\					 |				     /
	   \                 |                /
	      \---------|---------/ (_max_x - _min_x)/2
             \	         |           /
	            \        |        /
				  \      |     /
					 \   |  /tan
					   view
	*/
	int LhClip::signcheck_x(float v, float z) {
		if (z < _near_z) {
			return CLIP_CODE_L;
		}
		int sign = 0;
		float half_x = (z - _z_view) * _z_tan_x;
		float x = (v - half_x * 0.5) * 2;
		if (x > half_x) {
			sign = CLIP_CODE_G;
		}
		else if (x < -half_x) {
			sign = CLIP_CODE_L;
		}
		else
		{
			sign = CLIP_CODE_I;
		}
		return sign;
	}

	int LhClip::signcheck_y(float v, float z) {
		if (z < _near_z) {
			return CLIP_CODE_L;
		}
		int sign = 0;
		float half_y = (z - _z_view) * _z_tan_y;
		float x = (v - half_y * 0.5) * 2;
		if (x > half_y) {
			sign = CLIP_CODE_G;
		}
		else if (x < -half_y) {
			sign = CLIP_CODE_L;
		}
		else
		{
			sign = CLIP_CODE_I;
		}
		return sign;
	}
}