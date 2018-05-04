#include "lhlinesegment.h"
//#include <stdio.h>
#include <math.h>


LhLineSegment::LhLineSegment()
{
}


LhLineSegment::~LhLineSegment()
{
}


int LhLineSegment::dblcmp(float a, float b)
{
	if (fabs(a - b) <= 1E-6) return 0;
	if (a>b) return 1;
	else return -1;
}
//***************����е��Ƿ����߶���***************
float LhLineSegment::dot(float x1, float y1, float x2, float y2) //���
{
	return x1 * x2 + y1 * y2;
}

int LhLineSegment::point_on_line(Point a, Point b, Point c) //��a���ǲ������߶�bc�ϣ�>0���ڣ�=0��˵��غϣ�<0�ڡ�
{
	return dblcmp(dot(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y), 0);
}
//**************************************************
float LhLineSegment::cross(float x1, float y1, float x2, float y2)
{
	return x1 * y2 - x2 * y1;
}
float LhLineSegment::ab_cross_ac(Point a, Point b, Point c) //ab��ac�Ĳ��
{
	return cross(b.x - a.x, b.y - a.y, c.x - a.x, c.y - a.y);
}
int LhLineSegment::ab_cross_cd(Point& p, Point a, Point b, Point c, Point d) //��ab�Ƿ���cd�ཻ������Ϊp��1�淶�ཻ��0������һ�߶εĶ˵㣬-1���ཻ��
{
	float s1, s2, s3, s4;
	int d1, d2, d3, d4;
	d1 = dblcmp(s1 = ab_cross_ac(a, b, c), 0);
	d2 = dblcmp(s2 = ab_cross_ac(a, b, d), 0);
	d3 = dblcmp(s3 = ab_cross_ac(c, d, a), 0);
	d4 = dblcmp(s4 = ab_cross_ac(c, d, b), 0);

	//Point p;
	//����淶�ཻ���󽻵�
	if ((d1^d2) == -2 && (d3^d4) == -2)
	{
		p.x = (c.x*s2 - d.x*s1) / (s2 - s1);
		p.y = (c.y*s2 - d.y*s1) / (s2 - s1);
		return 1;
	}

	//������淶�ཻ
	if (d1 == 0 && point_on_line(c, a, b) <= 0)
	{
		p = c;
		return 0;
	}
	if (d2 == 0 && point_on_line(d, a, b) <= 0)
	{
		p = d;
		return 0;
	}
	if (d3 == 0 && point_on_line(a, c, d) <= 0)
	{
		p = a;
		return 0;
	}
	if (d4 == 0 && point_on_line(b, c, d) <= 0)
	{
		p = b;
		return 0;
	}
	//������ཻ
	return -1;
}


bool LhLineSegment::point_on_line_segment(float x, float  y, float a_x, float a_y, float b_x, float b_y) {
	Point a(a_x, a_y);
	Point b(b_x, b_y);
	Point p(x, y);
	return point_on_line(p, a, b) <=0 ? true : false;
}

bool LhLineSegment::line_segment_cross(float a_x, float a_y, float b_x, float b_y,
	float c_x, float c_y, float d_x, float d_y,
	float& x, float & y) {
	Point a(a_x, a_y);
	Point b(b_x, b_y);
	Point c(c_x, c_y);
	Point d(d_x, d_y);
	Point p;
	if (1 == ab_cross_cd(p, a, b, c, d)) {
		return point_on_line(p, a, b) <= 0 ? true : false;
	}
	return false;
}
bool LhLineSegment::line__cross(float a_x, float a_y, float b_x, float b_y,
	float c_x, float c_y, float d_x, float d_y,
	float& x, float & y) {
	Point a(a_x, a_y);
	Point b(b_x, b_y);
	Point c(c_x, c_y);
	Point d(d_x, d_y);
	Point p;
	if (1 == ab_cross_cd(p, a, b, c, d)) {
		return true;
	}
	return false;
}


/*

struct Point {
Point operator -(const Point &b) const {
struct Line {
double Cross(Point a, Point b) { //���
double esp = 1e-5;
double tmp3 = Cross(L1.a-L2.a, L2.b-L2.a);


*/