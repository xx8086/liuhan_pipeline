#include "lhclip.h"
#include "lhtools.hpp"

namespace lh_pipeline {
#define CLIP_CODE_G   0x0001    // x > x_max
#define CLIP_CODE_L   0x0002    // x < x_min
#define CLIP_CODE_I   0x0004    // x_min < x < x_max

#define CLIP_CODE_IN   0x0001
#define CLIP_CODE_OUT   0x0000

#define CLIP_CODE_NULL 0x0000

#define  OUT_SIDE_SIGNED(a, b, c) \
    if ((a == CLIP_CODE_G && \
         b == CLIP_CODE_G && \
         c == CLIP_CODE_G) || \
        (a == CLIP_CODE_L && \
         b == CLIP_CODE_L && \
         c == CLIP_CODE_L)) return 0; 

#define TRIANGLE_OUTER_PROJECT(point_nums) \
if (0 == point_nums) {\
	return true;\
}


#define ORGANIZATION_POINTS(sign)\
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

    bool LhClip::split_triangle_x(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign;
        int point_nums = outside(sign, v1->postion.get_x(), v2->postion.get_x(), v3->postion.get_x());
        if (0 == point_nums)return true;
        if (3 == point_nums) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
            return true;
        }
        ORGANIZATION_POINTS(sign)
            if (1 == point_nums) {
                one_triangle_x(triangles, p0, p1, p2);
            }
            else if (2 == point_nums) {
                two_triangle_x(triangles, p0, p1, p2);
            }
            return true;
    }

    bool LhClip::split_triangle_y(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign;
        int point_nums = outside(sign, v1->postion.get_y(), v2->postion.get_y(), v3->postion.get_y());
        if (0 == point_nums)return true;
        if (3 == point_nums) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
            return true;
        }
        ORGANIZATION_POINTS(sign)

            if (1 == point_nums) {
                one_triangle_y(triangles, p0, p1, p2);
            }
            else if (2 == point_nums) {
                two_triangle_y(triangles, p0, p1, p2);
            }
            return true;
    }

	bool LhClip::split_triangle_z(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
		LhVertexInt3 sign;
		int point_nums = outside(sign, v1->postion.get_z(), v2->postion.get_z(), v3->postion.get_z());
		TRIANGLE_OUTER_PROJECT(point_nums)
		ORGANIZATION_POINTS(sign)

		if (1 == point_nums) {
			one_triangle_z(triangles, p0, p1, p2);
		}
		else if (2 == point_nums) {
			two_triangle_z(triangles, p0, p1, p2);
		}
		return point_nums;
	}

	void LhClip::triangle_clip(std::vector<VertexColor>&triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign;
        int z_point_nums = outside(sign, v1->postion.get_z(), v2->postion.get_z(), v3->postion.get_z());
        if (3 == z_point_nums) {
            split_triangle_x(triangles, v1, v2, v3);
            split_triangle_y(triangles, v1, v2, v3);
        }
        else {
            split_triangle_z(triangles, v1, v2, v3);
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
			float t2 = (-1 - v2->postion.get_x()) / (v1->postion.get_x() - v2->postion.get_x());
            float t3 = (-1 - v3->postion.get_x()) / (v1->postion.get_x() - v3->postion.get_x());
			*v2 = lerp(*v2, *v1, t2);
			*v3 = lerp(*v3, *v1, t3);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v2);
			triangles.emplace_back(*v3);
		}
		else if (1.0f < v2->postion.get_x() &&
			1.0f < v3->postion.get_x()) {//right
			float t;
			t = (1 - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
			*v2 = lerp(*v1, *v2, t);
			t = (1 - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
			*v3 = lerp(*v1, *v3, t);
			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(*v2);
		}
		else if (v3->postion.get_x() < -1.0f &&
			1.0f < v2->postion.get_x()) {
            float t2 = (1 - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
            float t3 = (-1 - v3->postion.get_x()) / (v1->postion.get_x() - v3->postion.get_x());
            *v2 = lerp(*v1, *v2, t2);
            *v3 = lerp(*v3, *v1, t3);
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v3);
            triangles.emplace_back(*v2);
            if (v2->postion.get_y() < -1.0f || v3->postion.get_y() < -1.0f) {
                VertexColor p2 = *v2;
                VertexColor p3 = *v3;
                if (p2.postion.get_y() > p3.postion.get_y()) {
                    swap_vaue(p2, p3);
                }

                float t3 = (p2.postion.get_y() - p3.postion.get_y()) / (v1->postion.get_y() - p3.postion.get_y());
                VertexColor temp_v3 = lerp(p3, *v1, t3);
                t3 = (-1.0f - temp_v3.postion.get_x()) / (p2.postion.get_x() - temp_v3.postion.get_x());
                temp_v3 = lerp(temp_v3, p2, t3);

                triangles.emplace_back(p3);
                triangles.emplace_back(temp_v3);
                triangles.emplace_back(p2);
            }
        }
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
            VertexColor p3 = *v3;
            VertexColor p2 = *v2;
			float t = (-1.0f - p3.postion.get_x()) / (v1->postion.get_x() - p3.postion.get_x());
            *v3 = lerp(p3, *v1, t);

			t = (-1.0f - p3.postion.get_x()) / (p2.postion.get_x() - p3.postion.get_x());
			*v2 = lerp(p3, p2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(*v2);

			triangles.emplace_back(*v1);
			triangles.emplace_back(*v2);
			triangles.emplace_back(p2);
		}
		else if (v2->postion.get_x() > 1.0f) {//right
            VertexColor p3 = *v3;
            VertexColor p2 = *v2;
			float t = (1.0f - v1->postion.get_x()) / (p2.postion.get_x() - v1->postion.get_x());
			*v2 = lerp(*v1, p2, t);

			t = (1.0f - p3.postion.get_x()) / (p2.postion.get_x() - p3.postion.get_x());
            *v3 = lerp(p3, p2, t);

			triangles.emplace_back(*v1);
			triangles.emplace_back(*v3);
			triangles.emplace_back(*v2);

			triangles.emplace_back(*v1);
			triangles.emplace_back(p3);
			triangles.emplace_back(*v3);
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
		else if(v2->postion.get_y() > 1.0f &&
			v3->postion.get_y() < -1.0f){ 
            float t2 = (1 - v1->postion.get_y()) / (v2->postion.get_y() - v1->postion.get_y());
            float t3 = (-1 - v3->postion.get_y()) / (v1->postion.get_y() - v3->postion.get_y());
            *v2 = lerp(*v1, *v2, t2);
            *v3 = lerp(*v3, *v1, t3);
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v3);
            triangles.emplace_back(*v2);
        }
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

            //if (v2->postion.get_y() <= v1->postion.get_y() &&
            //    v2->postion.get_y() > -1.0f) {//右下角
            //    float t = (-1 - p2.postion.get_x()) / (p1.postion.get_x() - p2.postion.get_x());
            //    VertexColor v = lerp(p2, p1, t);
            //    triangles.emplace_back(*v2);
            //    triangles.emplace_back(v);
            //    triangles.emplace_back(p2);
            //}
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

    int signcheck(float v, float plane) {
        if (v > plane) {
            return CLIP_CODE_I;
        }
    }

	//////////////////////
	bool LhClip::on_x_space(std::vector<VertexColor>& triangles,
		VertexColor* v1, VertexColor* v2, VertexColor* v3,
		LhVertexInt3 sign, int nums) {
        if (0 == nums)return true;
        if (3 == nums) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
            return true;
        }
        ORGANIZATION_POINTS(sign)
            
			if (1 == nums) {
				one_triangle_x(triangles, p0, p1, p2);
			}
			else if (2 == nums) {
				two_triangle_x(triangles, p0, p1, p2);
			}
            return true;
	}

	void LhClip::on_y_space(std::vector<VertexColor>& triangles,
		VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign_updown;
        int nums_updown = outside(sign_updown, v1->postion.get_y(), v2->postion.get_y(), v3->postion.get_y());
        if (0 == nums_updown)return;
        if (3 == nums_updown) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
            return;
        }
		ORGANIZATION_POINTS(sign_updown)
			if (1 == nums_updown) {
				one_triangle_y(triangles, p0, p1, p2);
			}
			else if (2 == nums_updown) {
				two_triangle_y(triangles, p0, p1, p2);
			}
	}
    void LhClip::triangle_clip_plane(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign_both;
        LhVertexInt3 sign_updown;
        LhVertexInt3 sign_near;
        int nums_both = outside(sign_both, v1->postion.get_x(), v2->postion.get_x(), v3->postion.get_x());
        int nums_updown = outside(sign_updown, v1->postion.get_y(), v2->postion.get_y(), v3->postion.get_y());
        int nums_near = outside(sign_near, v1->postion.get_z(), v2->postion.get_z(), v3->postion.get_z());
        if (3 == nums_both && 3 == nums_updown && 3 == nums_near) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
        }
        if (0 == nums_both) {
            if (0 == nums_updown) {
                return;
            }
            if (v1->postion.get_x() > v2->postion.get_x()) {
                swap_vaue(v1, v2);
            }
            if (v2->postion.get_x() > v3->postion.get_x()) {
                swap_vaue(v2, v3);
            }
            if (v1->postion.get_x() > v3->postion.get_x()) {
                swap_vaue(v1, v3);
            }
            
            if (v1->postion.get_x() < -1.0f &&
                v3->postion.get_x() > 1.0f) {
                std::vector<VertexColor> y_triangles;
                if (v2->postion.get_x() < -1.0f) {
                    float t23 = (-1.0f - v2->postion.get_x()) / (v3->postion.get_x() - v2->postion.get_x());
                    VertexColor temp_v2 = lerp(*v2, *v3, t23);
                    t23 = (1.0f - v2->postion.get_x()) / (v3->postion.get_x() - v2->postion.get_x());
                    VertexColor temp_v3 = lerp(*v2, *v3, t23);

                    float t13 = (-1.0f - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_v1 = lerp(*v1, *v3, t13);
                    t13 = (1.0f - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_v4 = lerp(*v1, *v3, t13);
                    VertexColor tv1 = temp_v1;
                    VertexColor tv3 = temp_v3;
                    on_y_space(y_triangles, &temp_v1, &temp_v2, &temp_v3);
                    on_y_space(y_triangles, &tv1, &tv3, &temp_v4);
                }
                if (v2->postion.get_x() > 1.0f) {
                    float t12 = (-1.0f - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_1 = lerp(*v1, *v2, t12);
                    t12 = (1.0f - v1->postion.get_x()) / (v2->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_4 = lerp(*v1, *v2, t12);
                    float t13 = (-1.0f - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_2 = lerp(*v1, *v3, t13);
                    t13 = (1.0f - v1->postion.get_x()) / (v3->postion.get_x() - v1->postion.get_x());
                    VertexColor temp_3 = lerp(*v1, *v3, t13);
                    VertexColor tv1 = temp_1;
                    VertexColor tv3 = temp_3;
                    on_y_space(y_triangles, &temp_1, &temp_2, &temp_3);
                    on_y_space(y_triangles, &tv1, &tv3, &temp_4);
                }

                for (std::vector<VertexColor>::iterator iter = y_triangles.begin();
                    iter != y_triangles.end();
                    iter += 3) {
                    on_z_space(triangles, &(*(iter)), &(*(iter + 1)), &(*(iter + 2)));
                }
            }

            return;
        }


        if (1 == nums_both || 2 == nums_both){
            VertexColor temp_v1_x(*v1);
            VertexColor temp_v2_x(*v2);
            VertexColor temp_v3_x(*v3);

            std::vector<VertexColor> x_triangles_1;
            on_x_space(x_triangles_1, &temp_v1_x, &temp_v2_x, &temp_v3_x, sign_both, nums_both);
            if (temp_v2_x.postion.get_y() < temp_v3_x.postion.get_y()) {
                swap_vaue(temp_v2_x, temp_v3_x);
            }

            float t = 0.0f;
            VertexColor left;
            if (temp_v2_x.postion.get_y() < 1.0f &&
                -1.0f < temp_v2_x.postion.get_y() &&
                temp_v3_x.postion.get_y() < -1.0f) {//下角
                t = (-1.0f - temp_v3_x.postion.get_y()) / (temp_v2_x.postion.get_y() - temp_v3_x.postion.get_y());
            }
            else if (temp_v3_x.postion.get_y() < 1.0f &&
                -1.0f < temp_v3_x.postion.get_y() &&
                temp_v2_x.postion.get_y() > 1.0f) {//上角
                t = (1.0f - temp_v3_x.postion.get_y()) / (temp_v2_x.postion.get_y() - temp_v3_x.postion.get_y());
            }
            else {
                std::vector<VertexColor> y_triangles_1;
                on_y_space(y_triangles_1, &temp_v1_x, &temp_v2_x, &temp_v3_x);
                if (2 == nums_both) {
                    on_y_space(y_triangles_1, &x_triangles_1[3], &x_triangles_1[4], &x_triangles_1[5]);
                }
                for (std::vector<VertexColor>::iterator iter = y_triangles_1.begin();
                    iter != y_triangles_1.end();
                    iter += 3) {
                    on_z_space(triangles, &(*(iter)), &(*(iter + 1)), &(*(iter + 2)));
                }
                return;
            }

            left = lerp(temp_v3_x, temp_v2_x, t);
            std::vector<VertexColor> y_triangles_1;
            on_y_space(y_triangles_1, &temp_v1_x, &temp_v2_x, &left);
            on_y_space(y_triangles_1, &temp_v1_x, &left, &temp_v3_x);
            if (x_triangles_1.size() == 6) {
                on_y_space(y_triangles_1, &x_triangles_1[3], &x_triangles_1[4], &x_triangles_1[5]);
            }

            for (std::vector<VertexColor>::iterator iter = y_triangles_1.begin();
                iter != y_triangles_1.end();
                iter += 3) {
                on_z_space(triangles, &(*(iter)), &(*(iter + 1)), &(*(iter + 2)));
            }
            return;
        }

        VertexColor temp_v1(*v1);
        VertexColor temp_v2(*v2);
        VertexColor temp_v3(*v3);
        std::vector<VertexColor> x_triangles;
        assert(nums_both == 3);
        bool bx = on_x_space(x_triangles, &temp_v1, &temp_v2, &temp_v3, sign_both, nums_both);
        std::vector<VertexColor> y_triangles;
        if (3 == x_triangles.size()) {
            on_y_space(y_triangles, &x_triangles[0], &x_triangles[1], &x_triangles[2]);
        }
        else if (6 == x_triangles.size()) {
            on_y_space(y_triangles, &x_triangles[0], &x_triangles[1], &x_triangles[2]);
            on_y_space(y_triangles, &x_triangles[3], &x_triangles[4], &x_triangles[5]);
        }

        for (std::vector<VertexColor>::iterator iter = y_triangles.begin();
            iter != y_triangles.end();
            iter += 3) {
            on_z_space(triangles, &(*(iter)), &(*(iter + 1)), &(*(iter + 2)));
        }

    }

    void LhClip::on_z_space(std::vector<VertexColor>& triangles,
        VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        LhVertexInt3 sign;
        int nums = outside(sign, v1->postion.get_y(), v2->postion.get_y(), v3->postion.get_y());
        if (0 == nums)return;
        if (3 == nums) {
            triangles.emplace_back(*v1);
            triangles.emplace_back(*v2);
            triangles.emplace_back(*v3);
            return;
        }
        ORGANIZATION_POINTS(sign)
            if (1 == nums) {
                one_triangle_z(triangles, p0, p1, p2);
            }
            else if (2 == nums) {
                two_triangle_z(triangles, p0, p1, p2);
            }
    }

    void LhClip::triangle_clip_single_plane(std::vector<VertexColor>& triangles, VertexColor* v1, VertexColor* v2, VertexColor* v3) {
        std::vector<VertexColor> temp_triangles_des;
        std::vector<VertexColor> temp_triangles_src;
        temp_triangles_src.emplace_back(*v1);
        temp_triangles_src.emplace_back(*v2);
        temp_triangles_src.emplace_back(*v3);

        std::vector<VertexColor> temp_triangles_left;
        std::vector<VertexColor> temp_triangles_right;
        std::vector<VertexColor> temp_triangles_top;
        std::vector<VertexColor> temp_triangles_bottom;
        std::vector<VertexColor> temp_triangles_far;
        std::vector<VertexColor> temp_triangles_naer;

        clip_left(temp_triangles_left, temp_triangles_src);
        clip_right(temp_triangles_right, temp_triangles_left);
        clip_top(temp_triangles_top, temp_triangles_right);
        clip_bottom(temp_triangles_bottom, temp_triangles_top);
        clip_far(temp_triangles_far, temp_triangles_bottom);
        clip_near(triangles, temp_triangles_far);
    }

    int LhClip::signcheck_than(float v, float plane) {
        return v > plane ? CLIP_CODE_IN : CLIP_CODE_OUT;
    }
    int LhClip::signcheck_less(float v, float plane) {
        return v < plane ? CLIP_CODE_IN : CLIP_CODE_OUT;
    }

    void LhClip::clip_left(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_than(v1->postion.get_x(), -1.0f));
            sign.set_y(signcheck_than(v2->postion.get_x(), -1.0f));
            sign.set_z(signcheck_than(v3->postion.get_x(), -1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if(sign.get_y() == CLIP_CODE_IN){
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else{
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p2->postion.get_x()) / (p1->postion.get_x() - p2->postion.get_x());
                float t3 = (-1 - p3->postion.get_x()) / (p1->postion.get_x() - p3->postion.get_x());
                *p2 = lerp(*p2, *p1, t2);
                *p3 = lerp(*p3, *p1, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p1->postion.get_x()) / (p2->postion.get_x() - p1->postion.get_x());
                float t3 = (-1 - p1->postion.get_x()) / (p3->postion.get_x() - p1->postion.get_x());
                VertexColor left_2 = lerp(*p1, *p2, t2);
                VertexColor left_3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(left_3);
                des_triangle.emplace_back(*p3);

                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(*p2);
            }
        }
    }

    void LhClip::clip_right(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_less(v1->postion.get_x(), 1.0f));
            sign.set_y(signcheck_less(v2->postion.get_x(), 1.0f));
            sign.set_z(signcheck_less(v3->postion.get_x(), 1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_IN) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p1->postion.get_x()) / (p2->postion.get_x() - p1->postion.get_x());
                float t3 = (1 - p1->postion.get_x()) / (p3->postion.get_x() - p1->postion.get_x());
                *p2 = lerp(*p1, *p2, t2);
                *p3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p2->postion.get_x()) / (p1->postion.get_x() - p2->postion.get_x());
                float t3 = (1 - p3->postion.get_x()) / (p1->postion.get_x() - p3->postion.get_x());
                VertexColor right_2 = lerp(*p2, *p1, t2);
                VertexColor right_3 = lerp(*p3, *p1, t3);

                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(right_3);
                des_triangle.emplace_back(right_2);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(right_3);

            }
        }
    }


    void LhClip::clip_bottom(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_than(v1->postion.get_y(), -1.0f));
            sign.set_y(signcheck_than(v2->postion.get_y(), -1.0f));
            sign.set_z(signcheck_than(v3->postion.get_y(), -1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_IN) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p2->postion.get_y()) / (p1->postion.get_y() - p2->postion.get_y());
                float t3 = (-1 - p3->postion.get_y()) / (p1->postion.get_y() - p3->postion.get_y());
                *p2 = lerp(*p2, *p1, t2);
                *p3 = lerp(*p3, *p1, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p1->postion.get_y()) / (p2->postion.get_y() - p1->postion.get_y());
                float t3 = (-1 - p1->postion.get_y()) / (p3->postion.get_y() - p1->postion.get_y());
                VertexColor left_2 = lerp(*p1, *p2, t2);
                VertexColor left_3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(left_3);
                des_triangle.emplace_back(*p3);

                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(*p2);
            }
        }
    }

    void LhClip::clip_top(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_less(v1->postion.get_y(), 1.0f));
            sign.set_y(signcheck_less(v2->postion.get_y(), 1.0f));
            sign.set_z(signcheck_less(v3->postion.get_y(), 1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_IN) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p1->postion.get_y()) / (p2->postion.get_y() - p1->postion.get_y());
                float t3 = (1 - p1->postion.get_y()) / (p3->postion.get_y() - p1->postion.get_y());
                *p2 = lerp(*p1, *p2, t2);
                *p3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p2->postion.get_x()) / (p1->postion.get_y() - p2->postion.get_y());
                float t3 = (1 - p3->postion.get_x()) / (p1->postion.get_y() - p3->postion.get_y());
                VertexColor right_2 = lerp(*p2, *p1, t2);
                VertexColor right_3 = lerp(*p3, *p1, t3);

                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(right_3);
                des_triangle.emplace_back(right_2);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(right_3);
            }
        }
    }

    void LhClip::clip_near(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_than(v1->postion.get_z(), -1.0f));
            sign.set_y(signcheck_than(v2->postion.get_z(), -1.0f));
            sign.set_z(signcheck_than(v3->postion.get_z(), -1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_IN) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p2->postion.get_z()) / (p1->postion.get_z() - p2->postion.get_z());
                float t3 = (-1 - p3->postion.get_z()) / (p1->postion.get_z() - p3->postion.get_z());
                *p2 = lerp(*p2, *p1, t2);
                *p3 = lerp(*p3, *p1, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (-1 - p1->postion.get_z()) / (p2->postion.get_z() - p1->postion.get_z());
                float t3 = (-1 - p1->postion.get_z()) / (p3->postion.get_z() - p1->postion.get_z());
                VertexColor left_2 = lerp(*p1, *p2, t2);
                VertexColor left_3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(left_3);
                des_triangle.emplace_back(*p3);

                des_triangle.emplace_back(left_2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(*p2);
            }
        }
    }
    void LhClip::clip_far(std::vector<VertexColor>& des_triangle, std::vector<VertexColor>& sr_triangle) {
        for (std::vector<VertexColor>::iterator iter = sr_triangle.begin();
            iter != sr_triangle.end();
            iter += 3) {
            VertexColor* v1 = &(*(iter));
            VertexColor* v2 = &(*(iter + 1));
            VertexColor* v3 = &(*(iter + 2));
            LhVertexFloat3 sign;
            sign.set_x(signcheck_less(v1->postion.get_z(), 1.0f));
            sign.set_y(signcheck_less(v2->postion.get_z(), 1.0f));
            sign.set_z(signcheck_less(v3->postion.get_z(), 1.0f));
            int in_nums = sign.get_x() + sign.get_y() + sign.get_z();
            if (0 == in_nums)continue;
            if (3 == in_nums) {
                des_triangle.emplace_back(*v1);
                des_triangle.emplace_back(*v2);
                des_triangle.emplace_back(*v3);
            }

            VertexColor* p1;
            VertexColor* p2;
            VertexColor* p3;
            if (1 == in_nums) {
                if (sign.get_z() == CLIP_CODE_IN) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_IN) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p1->postion.get_z()) / (p2->postion.get_z() - p1->postion.get_z());
                float t3 = (1 - p1->postion.get_z()) / (p3->postion.get_z() - p1->postion.get_z());
                *p2 = lerp(*p1, *p2, t2);
                *p3 = lerp(*p1, *p3, t3);
                des_triangle.emplace_back(*p1);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
            }
            else if (2 == in_nums) {
                if (sign.get_z() == CLIP_CODE_OUT) {
                    p1 = v3;
                    p2 = v2;
                    p3 = v1;
                }
                else if (sign.get_y() == CLIP_CODE_OUT) {
                    p1 = v2;
                    p2 = v1;
                    p3 = v3;
                }
                else {
                    p1 = v1;
                    p2 = v2;
                    p3 = v3;
                }

                float t2 = (1 - p2->postion.get_z()) / (p1->postion.get_z() - p2->postion.get_z());
                float t3 = (1 - p3->postion.get_z()) / (p1->postion.get_z() - p3->postion.get_z());
                VertexColor right_2 = lerp(*p2, *p1, t2);
                VertexColor right_3 = lerp(*p3, *p1, t3);

                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(right_3);
                des_triangle.emplace_back(right_2);
                des_triangle.emplace_back(*p2);
                des_triangle.emplace_back(*p3);
                des_triangle.emplace_back(right_3);
            }
        }
    }
}