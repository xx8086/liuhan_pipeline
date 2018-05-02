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

#define TRIANGLE_OUTER_PROJECT(point_nums) \
if (0 == point_nums || 3 == point_nums) {\
	return point_nums;\
}


#define ORGANIZATION_POINTS\
    VertexColor* p0;\
	VertexColor* p1;\
	VertexColor* p2;\
	if (CLIP_CODE_I == sign.get_x()) {\
		p0 = v1;\
		p1 = v2;\
		p2 = v3;\
	}\
	else if (CLIP_CODE_I == sign.get_y()) {\
		p0 = v2;\
		p1 = v3;\
		p2 = v1;\
	}\
	else {\
		p0 = v3;\
		p1 = v1;\
		p2 = v2;\
	}

	LhClip::LhClip()
	{
	}


	LhClip::~LhClip()
	{
	}

	bool LhClip::backface_culling(LhVertexFloat3& normal, LhVertexFloat3& dir) {
		return dot(normal, dir) > 0 ? false : true;//左手,大于零同向,不消除
	}

	int LhClip::split_triangle_x(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		LhVertexInt3 sign;
		int point_nums = outside(sign, v1->postion.get_x(), v2->postion.get_x(), v3->postion.get_x());
		TRIANGLE_OUTER_PROJECT(point_nums)
		ORGANIZATION_POINTS
		if (1 == point_nums) {
			one_triangle_x(triangles, p0, p1, p2);
		}
		else if (2 == point_nums) {
			two_triangle_x(triangles, p0, p1, p2);
		}
		return point_nums;
	}

	int LhClip::split_triangle_y(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		LhVertexInt3 sign;
		int point_nums = outside(sign, v1->postion.get_y(), v2->postion.get_y(), v3->postion.get_y());
		TRIANGLE_OUTER_PROJECT(point_nums)
			ORGANIZATION_POINTS

			if (1 == point_nums) {
				one_triangle_y(triangles, p0, p1, p2);
			}
			else if (2 == point_nums) {
				two_triangle_y(triangles, p0, p1, p2);
			}
			return point_nums;
	}

	int LhClip::split_triangle_z(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		LhVertexInt3 sign;
		int point_nums = outside(sign, v1->postion.get_z(), v2->postion.get_z(), v3->postion.get_z());
		TRIANGLE_OUTER_PROJECT(point_nums)
		ORGANIZATION_POINTS

		if (1 == point_nums) {
			one_triangle_z(triangles, p0, p1, p2);
		}
		else if (2 == point_nums) {
			two_triangle_z(triangles, p0, p1, p2);
		}
		return point_nums;
	}

	void LhClip::triangle_clip(std::vector<VertexColor>&triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		v1->rhw();
		v2->rhw();
		v3->rhw();

		if (3 == split_triangle_x(triangles, v1, v2, v3) &&//三个点在左右截面内
			3 == split_triangle_y(triangles, v1, v2, v3) &&//三个点在上下截面内
			3 == split_triangle_z(triangles, v1, v2, v3)) {//三个点在前后截面内
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v2);
			triangles.emplace_back(*v3);
		}
	}

	int LhClip::two_triangle_z(std::vector<VertexColor>& triangles, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		if (p1->postion.get_z() < p2->postion.get_z()) {
			swap_vaue<VertexColor* >(p1, p2);
		}
		float t;
		t = (-1.0f - p2->postion.get_z()) / (p0->postion.get_z() - p2->postion.get_z());
		VertexColor near_p0 = lerp(*p2, *p0, t);

		t = (-1.0f - p2->postion.get_z()) / (p1->postion.get_z() - p2->postion.get_z());
		VertexColor near_p1 = lerp(*p2, *p1, t);

		triangles.emplace_back(*p0);
		triangles.emplace_back(near_p0);
		triangles.emplace_back(*p1);

		triangles.emplace_back(near_p0);
		triangles.emplace_back(near_p1);
		triangles.emplace_back(*p1);
		return 2;
	}

	int LhClip::one_triangle_z(std::vector<VertexColor>& triangles, VertexColor* p0, VertexColor* p1, VertexColor* p2) {
		//对每条边裁剪
		float t;
		VertexColor v;
		//对p0->p1边裁剪
		t = (-1.0f - p1->postion.get_z()) / (p0->postion.get_z()  - p1->postion.get_z());
		*p1 = lerp(*p1 , *p0, t);
		//p1->postion.set_z(_near_z);

		//对p0->p2边裁剪
		t = (-1.0f - p2->postion.get_z()) / (p0->postion.get_z() - p2->postion.get_z());
		*p2 = lerp(*p2, *p0, t);

		triangles.emplace_back(*p0);
		triangles.emplace_back(*p1);
		triangles.emplace_back(*p2);
		return 1;
	}

	int LhClip::outside(LhVertexInt3& sign, float a, float b, float c) {
		int s1 = signcheck(a);
		int s2 = signcheck(b);
		int s3 = signcheck(c);
		OUT_SIDE_SIGNED(s1, s2, s3);

		int point_on_view_nums = 0;
		if (CLIP_CODE_I == s1)point_on_view_nums++;
		if (CLIP_CODE_I == s2)point_on_view_nums++;
		if (CLIP_CODE_I == s3)point_on_view_nums++;
		sign.set_x(s1);
		sign.set_y(s2);
		sign.set_z(s3);
		return point_on_view_nums;
	}

	int LhClip::one_triangle_x(std::vector<VertexColor>& triangles,  VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		if (v2->postion.get_x() < v3->postion.get_x()) {
			swap_vaue<VertexColor* >(v2, v3);
		}
		if (v2->postion.get_x() < -1.0f &&
			v3->postion.get_x() <  -1.0f) {//left
			float t;
			VertexColor v;
			t = (-1 - v2->postion.get_x()) / (v1->postion.get_x() - v2->postion.get_x());
			*v2 = lerp(*v2, *v1, t);
			t = (-1 - v3->postion.get_x()) / (v1->postion.get_x() - v3->postion.get_x());
			*v3 = lerp(*v3, *v1, t);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v2);
			triangles.emplace_back(*v3);
		}
		else if (1.0f < v2->postion.get_x() &&
			1.0f < v3->postion.get_x()) {//right
			float t;
			VertexColor v;
			t = (1 - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
			*v2 = lerp(*v1, *v2, t);
			t = (1 - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
			*v3 = lerp(*v1, *v3, t);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(*v2);
		}
		/*else if (v3->postion.get_x() < -1.0f &&
			1.0f < v2->postion.get_x()) {}*/
		else {
			assert(false);
		}
		return 1;
	}
	int LhClip::two_triangle_x(std::vector<VertexColor>& triangles,  VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		if (v2->postion.get_x() < v3->postion.get_x()) {
			swap_vaue<VertexColor* >(v2, v3);
		}
		if (v3->postion.get_x() < -1.0f) {//left
			float t = (-1.0f - v3->postion.get_x()) / (v1->postion.get_x() - v3->postion.get_x());
			VertexColor left_p1 = lerp(*v3, *v1, t);

			t = (-1.0f - v3->postion.get_x()) / (v2->postion.get_x() - v3->postion.get_x());
			VertexColor left_p2 = lerp(*v3, *v2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(left_p1);
			triangles.emplace_back(*v2);

			triangles.emplace_back(left_p1);
			triangles.emplace_back(left_p2);
			triangles.emplace_back(*v2);
		}
		else if (v2->postion.get_x() > 1.0f) {//right
			float t = (1.0f - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
			VertexColor rigfht_p1 = lerp(*v1, *v2, t);

			t = (1.0f - v3->postion.get_x()) / (v2->postion.get_x() - v3->postion.get_x());
			VertexColor rigfht_p2 = lerp(*v3, *v2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(rigfht_p1);

			triangles.emplace_back(rigfht_p1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(rigfht_p2);
		}
		else {
			assert(false);
		}

		return 2;
	}

	int LhClip::one_triangle_y(std::vector<VertexColor>& triangles,  VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		if (v2->postion.get_y() < v3->postion.get_y()) {
			swap_vaue<VertexColor* >(v2, v3);
		}
		if (v2->postion.get_y() < -1.0f &&
			v3->postion.get_y() < -1.0f) {//bottom
			float t;
			VertexColor v;
			t = (-1 - v2->postion.get_y()) / (v1->postion.get_y() - v2->postion.get_y());
			*v2 = lerp(*v2, *v1, t);
			t = (-1 - v3->postion.get_y()) / (v1->postion.get_y() - v3->postion.get_y());
			*v3 = lerp(*v3, *v1, t);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v2);
			triangles.emplace_back(*v3);
		}
		else if (v1->postion.get_y() < v2->postion.get_y() &&
			v1->postion.get_y() < v3->postion.get_y()) {//right
			float t;
			VertexColor v;
			t = (1 - v1->postion.get_y()) / (v2->postion.get_y() - v1->postion.get_y());
			*v2 = lerp(*v1, *v2, t);
			t = (1 - v1->postion.get_y()) / (v3->postion.get_y() - v1->postion.get_y());
			*v3 = lerp(*v1, *v3, t);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(*v2);
		}
		/*else if(v2->postion.get_y() > 1.0f &&
			v2->postion.get_y() < -1.0f){ }*/
		else {
			assert(false);
		}
		return 1;
	}
	int LhClip::two_triangle_y(std::vector<VertexColor>& triangles,  VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		if (v2->postion.get_y() < v3->postion.get_y()) {
			swap_vaue<VertexColor* >(v2, v3);
		}
		if (v3->postion.get_y() < -1.0f) {//bottom
			float t = (-1 - v3->postion.get_y()) / (v1->postion.get_y() - v3->postion.get_y());
			VertexColor p1 = lerp(*v3, *v1, t);

			t = (-1 - v3->postion.get_y()) / (v2->postion.get_y() - v3->postion.get_y());
			VertexColor p2 = lerp(*v3, *v2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(p1);
			triangles.emplace_back(p2);

			triangles.emplace_back(*v1);
			triangles.emplace_back(p2);
			triangles.emplace_back(*v2);
		}
		else if (v2->postion.get_y() > 1.0f) {//top
			float t = (1 - v1->postion.get_y()) / (v2->postion.get_y() - v1->postion.get_y());
			VertexColor p1 = lerp(*v1, *v2, t);

			t = (1 - v3->postion.get_y()) / (v2->postion.get_y() - v3->postion.get_y());
			VertexColor p2 = lerp(*v3, *v2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(p2);

			triangles.emplace_back(p1);
			triangles.emplace_back(*v1);
			triangles.emplace_back(p2);
		}
		else {
			assert(false);
		}

		return 2;
	}

	int LhClip::signcheck(float v) {
		int sign = 0;
        if (v < -1.0f) {//cvv
            sign = CLIP_CODE_G;
        }
        else if (v > 1.0f) {
            sign = CLIP_CODE_L;
        }
        else {
            sign = CLIP_CODE_I;
        }

		return sign;
	}
}