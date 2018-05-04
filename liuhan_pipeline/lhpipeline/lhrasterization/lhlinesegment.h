#pragma once

class LhLineSegment
{
public:
	LhLineSegment();
	~LhLineSegment();
public:
	struct Point {
		float x = 0.0f;
		float y = 0.0f;
		Point() {}
		Point(float a, float b) : x(a), y(b) {}
	};
	struct stline
	{
		Point a;
		Point b;
	} line1, line2;

public:
	bool line_segment_cross(float a_x, float a_y, float b_x, float b_y, 
		float c_x, float c_y, float d_x, float d_y,
		float& x, float & y);
	bool line__cross(float a_x, float a_y, float b_x, float b_y,
		float c_x, float c_y, float d_x, float d_y,
		float& x, float & y);
	bool point_on_line_segment(float x, float  y, float a_x, float a_y, float b_x, float b_y);
private:
	int dblcmp(float a, float b);
	float dot(float x1, float y1, float x2, float y2);

	int point_on_line(Point a, Point b, Point c);
	float cross(float x1, float y1, float x2, float y2);
	float ab_cross_ac(Point a, Point b, Point c);
	int ab_cross_cd(Point& p, Point a, Point b, Point c, Point d);
};

////////////////