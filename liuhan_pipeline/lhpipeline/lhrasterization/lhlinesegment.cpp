#include "lhlinesegment.h"
#include<algorithm>

LhLineSegment::LhLineSegment()
{
}


LhLineSegment::~LhLineSegment()
{
}

bool LhLineSegment::on_segment(Point Pi, Point Pj, Point Q)
{
    if ((Q.x - Pi.x) * (Pj.y - Pi.y) == (Pj.x - Pi.x) * (Q.y - Pi.y)  //叉乘   
                                                                      //保证Q点坐标在pi,pj之间   
        && std::min(Pi.x, Pj.x) <= Q.x && Q.x <= std::max(Pi.x, Pj.x)
        && std::min(Pi.y, Pj.y) <= Q.y && Q.y <= std::max(Pi.y, Pj.y))
        return true;
    else
        return false;
}


void LhLineSegment::getlinepara(Line &l)
{
    l.a = l.p1.y - l.p2.y;
    l.b = l.p2.x - l.p1.x;
    l.c = l.p1.x*l.p2.y - l.p1.y*l.p2.x;
}

bool LhLineSegment::getcrosspoint(Line &l1, Line &l2, Point& p)
{
    getlinepara(l1);
    getlinepara(l2);
    float D = l1.a*l2.b - l2.a*l1.b;
    if (0.0 == D)return false;
    p.x = (l1.b*l2.c - l2.b*l1.c) / D;
    p.y = (l1.c*l2.a - l2.c*l1.a) / D;
    return true;
}


bool LhLineSegment::line_segment_cross(float a_x, float a_y, float b_x, float b_y,
	float c_x, float c_y, float d_x, float d_y,
	float& x, float & y) {
	
    Point p;
    Point a(a_x, a_y);
    Point b(b_x, b_y);
    Point c(c_x, c_y);
    Point d(d_x, d_y);
    Line l1(a, b);
    Line l2(c, d);

    bool iscross = false;
    iscross = getcrosspoint(l1, l2, p);
    if (iscross) {
        iscross = on_segment(c, d, p);
    }

    return iscross;
}
bool LhLineSegment::line_cross(float a_x, float a_y, float b_x, float b_y,
	float c_x, float c_y, float d_x, float d_y,
	float& x, float & y) {
	//Point a(a_x, a_y);
	//Point b(b_x, b_y);
	//Point c(c_x, c_y);
	//Point d(d_x, d_y);
	Point p;
    Line l1(Point(a_x, a_y), Point(b_x, b_y));
    Line l2(Point(c_x, c_y), Point(d_x, d_y));

    bool iscross = false;
    iscross = getcrosspoint(l1, l2, p);
	return iscross;
}
