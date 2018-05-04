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

    struct Line
    {
        Point p1;
        Point p2;
        float a;
        float b;
        float c;

        Line() {}
        Line(Point _p1, Point _p2) :p1(_p1), p2(_p2) {}
    };

public:
	bool line_segment_cross(float a_x, float a_y, float b_x, float b_y, 
		float c_x, float c_y, float d_x, float d_y,
		float& x, float & y);
	bool line_cross(float a_x, float a_y, float b_x, float b_y,
		float c_x, float c_y, float d_x, float d_y,
		float& x, float & y);
private:

    void getlinepara(Line &l);
    bool getcrosspoint(Line &l1, Line &l2, Point& p);
    bool on_segment(Point Pi, Point Pj, Point Q);
};

////////////////