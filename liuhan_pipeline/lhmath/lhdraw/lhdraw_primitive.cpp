#include "lhdraw_primitive.h"
#include <cmath>


LhDrawPrimitive::LhDrawPrimitive()
{
}


LhDrawPrimitive::~LhDrawPrimitive()
{
}

void LhDrawPrimitive::set_buffer(int w, int h, void* pbits) {
    _width = w;
    _height = h;
    _frame_buffers = static_cast<unsigned char*>(pbits);
}

void LhDrawPrimitive::setpixel(int y, int x, lh_color color) {
    _frame_buffers[(x * _width + y + 1) * 4 + 0] = color.blue;//b
    _frame_buffers[(x * _width + y + 1) * 4 + 1] = color.green;//g
    _frame_buffers[(x * _width + y + 1) * 4 + 2] = color.red;//r
    _frame_buffers[(x * _width + y + 1) * 4 + 3] = color.alph;//a
}

/*
dx = (x(i+1) - x(i)) / (t(i+1) - t(i))
dy = (y(i+1) - y(i)) / (t(i+1) - t(i))
dm = max(|dx|, |dy|)
∴ 
x(i+1) = x(i) + dx / dm
y(i+1) = y(i) + dy / dm

|k| >= 1 (dy > dx, dm = dy)
          .P2
         /|
        / |
       /  |
      /   |
     /    |
    /     | 单位长度(1) dy / dm
   /      |
  /       |
 /        |
.P1--------
dx / dm 

|k| < 1 (dy < dx, dm = dx) 
                                       .P2
                                    /  |
                                /      |
                            /          |
                        /              |
                   /                   | dy / dm 
              /                        |
         /                             |
    /                                  |
.P1-------------------------------------
单位长度(1) dx / dm
*/
void LhDrawPrimitive::line_dda(int x1, int y1, int x2, int y2, lh_color color) {
    int dm = 0;
    int tx = abs(x2 - x1);
    int ty = abs(y2 - y1);
    dm = (tx >= ty ? tx : ty);

    float x = x1 + 0.5f;
    float y = y1 + 0.5f;
    float dx = (float)tx / dm;
    float dy = (float)ty / dm;

    for (int i = 0; i < dm; i++) {
        setpixel((int)x, (int)y, color);
        x += dx;
        y += dy;
    }
}

template <typename T>
void LhDrawPrimitive::swap_vaue(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/*
-------T----/--
|      |  /   |
|      P      |
|   /  |      |
|/     |      |
O------S-------
(xi, yi)

y = kx + b
直线与单位网格边线段TS相交与P(x, y)点，P点下第一个网格点S，P点上第一个点T，
s = |PS|, t = |PT|
O点坐标(xi, yi)
S点坐标(xi + 1, yi)
T点坐标(xi + 1, yi + 1)
P = s > t ? T : P// T.S谁距离P点进选谁作为P点

(yp) = k*(xp) + b  = k*(xi + 1) + b
s - t = (yp - yi) - (yi + 1 - yp) = 2*yp - 2*yi - 1
      = 2*k(xi + 1) + 2*b - 2*yi -1
dx = x2 - x1 
dy = y2 - y1
k = dy / dx

∴ dx*(s-t) = 2*dy*(xi + 1) + dx*(2*b - 2*yi -1)
            = 2*dy*xi - 2*dx*yi + 2*dy + dx(2*b - 1)
令常数 C = 2*dy + dx(2*b - 1)
令di = dx*(s-t)//当dx>0时di与s-t同符号
则di = 2*dy*xi - 2*dx*yi + C
d(i+1) =  2*dy*x(i+1) - 2*dx*y(i+1) + C
d(i+1) - di = 2*dy*(x(i+1) - xi) - 2*dx(y(i+1) - yi)
            = 2*dy - 2*dx(y(i+1) - yi)
            // x(i+1) - xi == |OS| == 1; y(i+1) - yi == |PS| <= 1
∴ d(i+1) = di + 2*dy - 2*dx(y(i+1) - yi)
当di>=0时s-t>=0,P = T,所以y(i+1) - yi == 1
即d(i+1) = di + 2*(dy - dx)
当di<0时s-t<0,P = S,所以y(i+1) - yi == 0
即d(i+1) = di + 2*dy
*/
void LhDrawPrimitive::line_bresenham(int x1, int y1, int x2, int y2, lh_color color) {
    setpixel(x1, y1, color);
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    if (dx == 0 && dy == 0) {
        return;
    }

    bool b_coorde_transf = false;
    if (dx < dy) {//斜率k > 1
        b_coorde_transf = true;
        swap_vaue(x1, y1);//交换x,y坐标，使0 <= |k| <= 1
        swap_vaue(x2, y2);
        swap_vaue(dx, dy);
    }

    int tx = (x2 - x1) > 0 ? 1 : -1;
    int ty = (y2 - y1) > 0 ? 1 : -1;
    int curx = x1;
    int cury = y1;
    int dS = 2 * dy;
    int dT = 2 * (dy - dx);
    int d = dS - dx;
    while (curx <= x2) {
        if (d < 0) {
            d += dS;
        }
        else {
            cury += ty;
            d += dT;
        }

        b_coorde_transf ? setpixel(cury, curx, color) : setpixel(curx, cury, color);
        curx += tx;
    }
}

// 绘制线段
void LhDrawPrimitive::line(int x1, int y1, int x2, int y2, lh_color c) {
    int x, y, rem = 0;
    if (x1 == x2 && y1 == y2) {
        setpixel(x1, y1, c);
    }
    else if (x1 == x2) {
        int inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc) setpixel(x1, y, c);
        setpixel(x2, y2, c);
    }
    else if (y1 == y2) {
        int inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc) setpixel(x, y1, c);
        setpixel(x2, y2, c);
    }
    else {
        int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
        if (dx >= dy) {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; x <= x2; x++) {
                setpixel(x, y, c);
                rem += dy;
                if (rem >= dx) {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    setpixel(x, y, c);
                }
            }
            setpixel(x2, y2, c);
        }
        else {
            if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; y <= y2; y++) {
                setpixel(x, y, c);
                rem += dx;
                if (rem >= dy) {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    setpixel(x, y, c);
                }
            }
            setpixel(x2, y2, c);
        }
    }
}

struct Scanline
{
    Scanline(lh_color color_, lh_color step_) :color(color_), step(step_) {}
    void to_next_color()
    {
        color += step;
    }
    int x;
    int y;
    lh_color color;
    lh_color step;
    int width;
};

void LhDrawPrimitive::scanline(float left, float right, float y, lh_color left_color, lh_color right_color) {
    float divide = 1.0f / (right - left);
    lh_color step_color = (right_color - left_color) * divide;
    lh_color color = left_color;
    int begin_x = static_cast<int>(left + 0.5f);
    int width = abs(right - left);
    int b = right > left ? 1 : -1;
    for (int i = 0; i <= width; i++) {
        setpixel(begin_x + b * i, y, color);
        color += step_color;
    }
}

//插值
VertexColor LhDrawPrimitive::interp(const VertexColor& from, const VertexColor& to, float factor)
{
    LhVertex<float, 3> postion;
    postion.set_x(from.postion.get_x() + (to.postion.get_x() - from.postion.get_x())*factor);
    postion.set_y(from.postion.get_y() + (to.postion.get_y() - from.postion.get_y())*factor);
    postion.set_z(from.postion.get_z() + (to.postion.get_z() - from.postion.get_z())*factor);

    lh_color color = from.color;
    VertexColor vc(postion, color.interp(to.color, factor));

    return vc;
}

void LhDrawPrimitive::left_top_fill() {
}

void LhDrawPrimitive::draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color) {
    if (y2 < y1) {
        swap_vaue(x1, x2);
        swap_vaue(y1, y2);
    }
    if (y3 < y1) {
        swap_vaue(x1, x3);
        swap_vaue(y1, y3);
    }

    if (y3 < y2) {
        swap_vaue(x2, x3);
        swap_vaue(y2, y3);
    }

    float x_min_clip = 0;
    float y_min_clip = 0;
    float x_max_clip = _width;
    float y_max_clip = _height;
    if (y3 < y_min_clip || y1 > y_max_clip ||
        (x1 < x_min_clip && x2 < x_min_clip && x3 < x_min_clip) ||
        (x1 > x_max_clip && x2 > x_max_clip && x3> x_max_clip))return;

    if (y1 == y2) {//平顶
        top_triangle(x1, y1, x2, y2, x3, y3, color);
    }
    else if (y2 == y3) {
        bottom_triangle(x1, y1, x2, y2, x3, y3, color);
    }
    else {
        float x_new = x1 + (y2 - y1)*(x3 - x1) / (y3 - y1);
        top_triangle(x1, y1, x_new, y2, x3, y3, color);
        bottom_triangle(x1, x_new, x2, y2, x3, y3, color);
    }
}

/*
v1---------v2
\          /
 \        /
  \      /
   \    /
    \  /
     v3
*/
void LhDrawPrimitive::top_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color) {
    float dx_right;
    float dx_left;
    float x_satrt;
    float x_end;
    float triangle_height;
    float x_min_clip = 0;
    float y_min_clip = 0;
    float x_max_clip = _width;
    float y_max_clip = _height;
    float right;
    float left;
    int iy1;
    int iy3;
    int y_loop;

    if (x2 < x1) {
        swap_vaue(x1, x2);
    }

    triangle_height = y3 - y1;
    dx_left = (x3 - x1) / triangle_height;
    dx_right = (x3 - x2) / triangle_height;
    x_satrt = x1;
    x_end = x2;

    // 垂直裁剪
    if (y1 < y_min_clip) {
        x_satrt = x_satrt + dx_left * (-y1 + y_min_clip);
        x_end = x_end + dx_right * (-y1 + y_min_clip);
        y1 = y_min_clip;
        iy1 = y1;
    }
    else {
        iy1 = ceil(y1);
        x_satrt = x1 + dx_left * (iy1 - y1);
        x_end = x2 + dx_right * (iy1 - y1);
    }

    if (y3 > y_max_clip) {
        y3 = y_max_clip;
        iy3 = y3 - 1;
    }
    else {
        iy3 = ceil(y3) - 1;
    }
    //---垂直裁剪--end

    // 执行垂直裁剪
    if (y1 < y_min_clip) {
        x_satrt = x1 + dx_left * (-y1 + y_min_clip);
        x_end = x2 + dx_right * (-y1 + y_min_clip);
        y1 = y_min_clip;
    }
    if (y3 > y_max_clip) {
        y3 = y_max_clip;
    }

    iy1 = ceil(y1);
    iy3 = ceil(y3) - 1;
    // 执行垂直裁剪---end


    if (x1 >= x_min_clip && x_max_clip >= x1 &&
        x2 >= x_min_clip && x_max_clip >= x2 &&
        x3 >= x_min_clip && x_max_clip >= x3) {//不需要水平裁剪
        for (y_loop = iy1; y_loop <= iy3; y_loop++) {
            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
            x_satrt += dx_left;
            x_end += dx_right;
        }
    }
    else {
        for (y_loop = iy1; y_loop <= iy3; y_loop++) {
            left = x_satrt;
            right = x_end;

            x_satrt += dx_left;
            x_end += dx_right;

            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
            x_satrt += dx_left;
            x_end += dx_right;
            if (left < x_min_clip) {
                left = x_min_clip;
            }
            if (right > x_max_clip) {
                right = x_max_clip;
            }

            if (right <= x_min_clip || x_max_clip <= left) {
                continue;
            }
            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
        }
    }
}


/*
    v1
    /\
   /  \
  /    \
 /      \
v2------v3
*/
void LhDrawPrimitive::bottom_triangle(float x1, float y1, float x2, float y2, float x3, float y3, lh_color color) {
    float dx_right;
    float dx_left;
    float x_satrt;
    float x_end;
    float triangle_height;
    float x_min_clip = 0;
    float y_min_clip = 0;
    float x_max_clip = _width;
    float y_max_clip = _height;
    float right;
    float left;
    int iy1;
    int iy3;
    int y_loop;

    if (x3 < x2) {
        swap_vaue(x2, x3);
    }
    
    triangle_height = y3 - y1;
    dx_left = (x2 - x1) / triangle_height;
    dx_right = (x3 - x1) / triangle_height;
    x_satrt = x1;
    x_end = x1;

    // 垂直裁剪
    if (y1 < y_min_clip) {
        x_satrt = x_satrt + dx_left * (-y1 + y_min_clip);
        x_end = x_end + dx_right * (-y1 + y_min_clip);
        y1 = y_min_clip;
        iy1 = y1;
    }
    else {
        iy1 = ceil(y1);
        x_satrt = x1 + dx_left * (iy1 - y1);
        x_end = x1 + dx_right * (iy1 - y1);
    }

    if (y3 > y_max_clip) {
        y3 = y_max_clip;
        iy3 = y3 - 1;
    }
    else {
        iy3 = ceil(y3) - 1;
    }
    //---垂直裁剪--end

    // 执行垂直裁剪
    if (y1 < y_min_clip) {
        x_satrt = x1 + dx_left * (-y1 + y_min_clip);
        x_end = x1 + dx_right * (-y1 + y_min_clip);
        y1 = y_min_clip;
    }
    if (y3 > y_max_clip) {
        y3 = y_max_clip;
    }

    iy1 = ceil(y1);
    iy3 = ceil(y3) - 1;
    // 执行垂直裁剪---end


    if (x1 >= x_min_clip && x_max_clip >= x1 &&
        x2 >= x_min_clip && x_max_clip >= x2 &&
        x3 >= x_min_clip && x_max_clip >= x3) {//不需要水平裁剪
        for (y_loop = iy1; y_loop <= iy3; y_loop++) {
            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
            x_satrt += dx_left;
            x_end += dx_right;
        }
    }
    else {
        for (y_loop = iy1; y_loop <= iy3; y_loop++) {
            left = x_satrt;
            right = x_end;

            x_satrt += dx_left;
            x_end += dx_right;

            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
            x_satrt += dx_left;
            x_end += dx_right;
            if (left < x_min_clip) {
                left = x_min_clip;
            }
            if (right > x_max_clip) {
                right = x_max_clip;
            }

            if (right <= x_min_clip || x_max_clip <= left) {
                continue;
            }
            line_bresenham(x_satrt, y_loop, x_end, y_loop, color);
        }
    }
}