#include "lhdraw_primitive.h"
#include <cmath>
#include <memory.h>

LhDrawPrimitive::LhDrawPrimitive()
{
}


LhDrawPrimitive::~LhDrawPrimitive()
{
    if (nullptr != _frame_deep_buffers) {
        delete[] _frame_deep_buffers;
        _frame_deep_buffers = nullptr;
    }
}

void LhDrawPrimitive::set_buffer(int w, int h, void* pbits) {
    if (_width*_height < w) {
        delete[] _frame_deep_buffers;
    }
    _frame_deep_buffers = new float[w*h];

    _width = w;
    _height = h;
    _z_far_clip = 100.0f;
    _z_near_clip = 1.0f;
    _frame_buffers = static_cast<unsigned char*>(pbits);
    set_clip_window(0, 0, w, h);
}

void LhDrawPrimitive::clear_deep() {
    /*if (nullptr != _frame_deep_buffers) {
        memset(_frame_deep_buffers, 0, sizeof(float) * _width * _height);
    }*/
   
    int size = _width * _height -1;
    while (size-- > 0)
    {
        _frame_deep_buffers[size] = 100.0f;
    }
    
}

bool LhDrawPrimitive::deeptest(int x, int y, float z, lh_color color) {
    if (!_deep_test) {
        return true;
    }

    int deep_pos = y * _width + x + 1;
    if (/*z < _z_far_clip &&
        _z_near_clip < z &&*/
        _frame_deep_buffers[deep_pos] > z) {//z轴朝里
        _frame_deep_buffers[deep_pos] = z;
    }
    else {
        return false;
    }

    return true;
}

void LhDrawPrimitive::setpixel(int x, int y, float z, lh_color color) {
    if (deeptest(x, y, z, color)) {
        setpixel(x, y, color);
    }
}

void LhDrawPrimitive::setpixel(float x, float y, lh_color color) {
    
    setpixel((int)x, (int)y, color);
}

void LhDrawPrimitive::setpixel(int x, int y, lh_color color) {
    if (y >= _y_max_clip ||
        y < _y_min_clip ||
        x >= _x_max_clip ||
        x < _x_min_clip) {
        return;
    }

    int pos = (y * _width + x + 1) * 4;
    _frame_buffers[pos] = color.blue;//b
    _frame_buffers[pos + 1] = color.green;//g
    _frame_buffers[pos + 2] = color.red;//r
    _frame_buffers[pos + 3] = color.alph;//a
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
        setpixel(x, y, color);
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
void LhDrawPrimitive::line_bresenham(float x1, float y1, float x2, float y2, lh_color color) {
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


void LhDrawPrimitive::draw_line(int x1, int y1, int x2, int y2, lh_color c) {
    line(x1, y1, x2, y2, c);
}

void LhDrawPrimitive::set_clip_window(float x_min, float y_min, float x_max, float y_max) {
    _x_min_clip = x_min;
    _y_min_clip = y_min;
    _x_max_clip = x_max;
    _y_max_clip = y_max;
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

    if (y3 < _y_min_clip || y1 > _y_max_clip ||
        (x1 < _x_min_clip && x2 < _x_min_clip && x3 < _x_min_clip) ||
        (x1 > _x_max_clip && x2 > _x_max_clip && x3 > _x_max_clip))return;

    if (y1 == y2) {//平顶
        top_triangle(x1, y1, x2, y2, x3, y3, color);
    }
    else if (y2 == y3) {
        bottom_triangle(x1, y1, x2, y2, x3, y3, color);
    }
    else {
        assert(y2 >= y1);
        float line_x = x1 + (x3 - x1)*(y2 - y1) / (y3 - y1);
        bottom_triangle(x1, y1,
            line_x, y2,
            x2, y2,
            color);
        top_triangle(line_x, y2,
            x2, y2,
            x3, y3,
            color);

        if (x2 > x1)
        {//right
            
        }
        else {//left
            ;
        }

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
    int iy1;
    int iy3;

    if (x2 < x1) {
        swap_vaue(x1, x2);
    }

    triangle_height = y3 - y1;
    dx_left = (x3 - x1) / triangle_height;
    dx_right = (x3 - x2) / triangle_height;
    x_satrt = x1;
    x_end = x2;
    /*
    //垂直裁剪
    if (y1 < _y_min_clip) {
        x_satrt = x_satrt + dx_left * (-y1 + _y_min_clip);
        x_end = x_end + dx_right * (-y1 + _y_min_clip);
        y1 = _y_min_clip;
        iy1 = y1;
    }
    else {
        iy1 = ceil(y1);
        x_satrt = x_satrt + dx_left * (iy1 - y1);
        x_end = x_end + dx_right * (iy1 - y1);
    }

    if (y3 > _y_max_clip) {
        y3 = _y_max_clip;
        iy3 = y3 - 1;
    }
    else {
        iy3 = y3;// ceil(y3) - 1;
    }
    //-- - 垂直裁剪--end

    //    执行垂直裁剪
    if (y1 < _y_min_clip) {
        x_satrt = x_satrt + dx_left * (-y1 + _y_min_clip);
        x_end = x_end + dx_right * (-y1 + _y_min_clip);
        y1 = _y_min_clip;
    }
    if (y3 > _y_max_clip) {
        y3 = _y_max_clip;
    }*/

    iy1 = ceil(y1);
    iy3 = ceil(y3);
    //执行垂直裁剪-- - end

    if (x1 >= _x_min_clip && _x_max_clip >= x1 &&
        x2 >= _x_min_clip && _x_max_clip >= x2 &&
        x3 >= _x_min_clip && _x_max_clip >= x3) {//不需要水平裁剪
        for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
            for (int x = x_satrt; x < x_end; x++) {
                setpixel(x, y_loop, color);
            }
            x_satrt += dx_left;
            x_end += dx_right;
        }
    }
    else {/*
        for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
            float left = x_satrt;
            float right = x_end;
            x_satrt += dx_left;
            x_end += dx_right;

            if (left < _x_min_clip) {
                left = _x_min_clip;
            }
            if (right > _x_max_clip) {
                right = _x_max_clip;
            }

            if (right <= _x_min_clip || _x_max_clip <= left) {
                continue;
            }
            draw_line(left, y_loop, right, y_loop, color);
        }*/
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
    int iy1;
    int iy3;

    if (x3 < x2) {
        swap_vaue(x2, x3);
    }

    triangle_height = y3 - y1;
    dx_left = (x2 - x1) / triangle_height;
    dx_right = (x3 - x1) / triangle_height;
    x_satrt = x1;
    x_end = x1;

    /*
    // 垂直裁剪
    if (y1 < _y_min_clip) {
        x_satrt = x_satrt + dx_left * (_y_min_clip - y1);
        x_end = x_end + dx_right * (_y_min_clip - y1);
        y1 = _y_min_clip;
        iy1 = y1;
    }
    else {
        iy1 = ceil(y1);
        x_satrt = x_satrt + dx_left * (iy1 - y1);
        x_end = x_end + dx_right * (iy1 - y1);
    }

    if (y3 > _y_max_clip) {
        y3 = _y_max_clip;
        iy3 = y3 - 1;
    }
    else {
        iy3 = ceil(y3) - 1;
    }
    //---垂直裁剪--end

    // 执行垂直裁剪
    if (y1 < _y_min_clip) {
        x_satrt = x_satrt + dx_left * (_y_min_clip - y1);
        x_end = x_end + dx_right * (_y_min_clip - y1);
        y1 = _y_min_clip;
    }
    if (y3 > _y_max_clip) {
        y3 = _y_max_clip;
    } */

    iy1 = ceil(y1);
    iy3 = ceil(y3) - 1;
    // 执行垂直裁剪---end

    if (x1 >= _x_min_clip && _x_max_clip >= x1 &&
        x2 >= _x_min_clip && _x_max_clip >= x2 &&
        x3 >= _x_min_clip && _x_max_clip >= x3) {//不需要水平裁剪
        for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
            for (int x = x_satrt; x < x_end; x++) {
                setpixel(x, y_loop, color);
            }
            x_satrt += dx_left;
            x_end += dx_right;
        }
    }
    else {/*
        for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
            float left = x_satrt;
            float right = x_end;

            x_satrt += dx_left;
            x_end += dx_right;

            if (left < _x_min_clip) {
                left = _x_min_clip;
            }
            if (right > _x_max_clip) {
                right = _x_max_clip;
            }

            if (right <= _x_min_clip || _x_max_clip <= left) {
                continue;
            }
            draw_line(left, y_loop, right, y_loop, color);
        }*/
    }
}


void LhDrawPrimitive::draw_triangle(VertexColor v1, VertexColor v2, VertexColor v3) {
    if (v2.postion.get_y() < v1.postion.get_y()) {
        swap_vaue(v1.postion, v2.postion);
    }
    if (v3.postion.get_y() < v1.postion.get_y()) {
        swap_vaue(v1.postion, v3.postion);
    }
    if (v3.postion.get_y() < v2.postion.get_y()) {
        swap_vaue(v2.postion, v3.postion);
    }//v3.y > v2.y > v1.y

    if (v3.postion.get_y() < _y_min_clip || v1.postion.get_y() > _y_max_clip ||
        (v1.postion.get_x() < _x_min_clip && v2.postion.get_x() < _x_min_clip && v3.postion.get_x() < _x_min_clip) ||
        (v1.postion.get_x() > _x_max_clip && v2.postion.get_x() > _x_max_clip && v3.postion.get_x() > _x_max_clip)) {
        return;
    }

    if (v2.postion.get_y() == v1.postion.get_y()) {//平顶
        top_triangle(v1, v2, v3);
    }
    else if (v3.postion.get_y() == v2.postion.get_y()) {
        bottom_triangle(v1, v2, v3);
    }
    else {
        assert(v2.postion.get_y() > v1.postion.get_y());
        //float line_x = x1 + (x3 - x1)*(y2 - y1) / (y3 - y1);
        float tan_x = (v3.postion.get_x() - v1.postion.get_x()) / (v3.postion.get_y() - v1.postion.get_y());
        float tan_z = (v3.postion.get_z() - v1.postion.get_z()) / (v3.postion.get_y() - v1.postion.get_y());
        lh_color tan_color = (v3.color - v1.color) / (v3.postion.get_y() - v1.postion.get_y());

        float line_x = v1.postion.get_x() + (v2.postion.get_y() - v1.postion.get_y()) * tan_x;
        float line_z = v1.postion.get_z() + (v2.postion.get_y() - v1.postion.get_y()) * tan_z;
        lh_color line_color = v1.color + tan_color * (v2.postion.get_y() - v1.postion.get_y());
        VertexColor interp_v(LhVertex<float, 3>(line_x, v2.postion.get_y(), line_z), line_color);
        bottom_triangle(v1, interp_v, v2);
        top_triangle(interp_v, v2, v3);
    }
}

void LhDrawPrimitive::top_triangle(VertexColor v1, VertexColor v2, VertexColor v3) {
    if (v2.postion.get_x() < v1.postion.get_x()) {
        swap_vaue(v1, v2);
    }

    float tan_left = (v3.postion.get_x() - v1.postion.get_x()) / (v3.postion.get_y() - v1.postion.get_y());
    float tan_right = (v3.postion.get_x() - v2.postion.get_x()) / (v3.postion.get_y() - v2.postion.get_y());
    float tan_left_z = (v3.postion.get_z() - v1.postion.get_z()) / (v3.postion.get_y() - v1.postion.get_y());
    float tan_right_z = (v3.postion.get_z() - v2.postion.get_z()) / (v3.postion.get_y() - v2.postion.get_y());
    lh_color tan_left_color = (v3.color - v1.color) / (v3.postion.get_y() - v1.postion.get_y());
    lh_color tan_right_color = (v3.color - v2.color) / (v3.postion.get_y() - v2.postion.get_y());

    float left_x = v1.postion.get_x();
    float right_x = v2.postion.get_x();
    float left_z = v1.postion.get_z();
    float right_z = v2.postion.get_z();
    lh_color left_color = v1.color;
    lh_color right_color = v2.color;

    int iy1 = ceil(v1.postion.get_y());
    int iy3 = ceil(v3.postion.get_y()) - 1;
    for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
        draw_interp_scanline(
            VertexColor(LhVertex<float, 3>(left_x, y_loop, left_z), left_color),
            VertexColor(LhVertex<float, 3>(right_x, y_loop, right_z), right_color));
        left_x = left_x + tan_left;
        right_x = right_x + tan_right;
        left_z = left_z + tan_left_z;
        right_z = right_z + tan_right_z;
        left_color = left_color + tan_right_color;
        right_color = right_color + tan_right_color;
    }
}
void LhDrawPrimitive::bottom_triangle(VertexColor v1, VertexColor v2, VertexColor v3) {
    if (v3.postion.get_x() < v2.postion.get_x()) {
        swap_vaue(v2, v3);
    }

    float tan_left = (v2.postion.get_x() - v1.postion.get_x()) / (v2.postion.get_y() - v1.postion.get_y());
    float tan_right = (v3.postion.get_x() - v1.postion.get_x()) / (v3.postion.get_y() - v1.postion.get_y());
    float tan_left_z = (v2.postion.get_z() - v1.postion.get_z()) / (v2.postion.get_y() - v1.postion.get_y());
    float tan_right_z = (v3.postion.get_z() - v1.postion.get_z()) / (v3.postion.get_y() - v1.postion.get_y());
    lh_color tan_left_color = (v2.color - v1.color) / (v2.postion.get_y() - v1.postion.get_y());
    lh_color tan_right_color = (v3.color - v1.color) / (v3.postion.get_y() - v1.postion.get_y());
    
    float left_x = v1.postion.get_x();
    float right_x = v1.postion.get_x();
    float left_z = v1.postion.get_z();
    float right_z = v1.postion.get_z();
    lh_color left_color = v1.color;
    lh_color right_color = v1.color;

    int iy1 = ceil(v1.postion.get_y());
    int iy3 = ceil(v3.postion.get_y()) - 1;
    for (int y_loop = iy1; y_loop <= iy3; y_loop++) {
        draw_interp_scanline(
            VertexColor(LhVertex<float, 3>(left_x, y_loop, left_z), left_color),
            VertexColor(LhVertex<float, 3>(right_x, y_loop, right_z), right_color));
        left_x = left_x + tan_left;
        right_x = right_x + tan_right;
        left_z = left_z + tan_left_z;
        right_z = right_z + tan_right_z;
        left_color = left_color + tan_right_color;
        right_color = right_color + tan_right_color;
    }
}

void LhDrawPrimitive::draw_interp_scanline(VertexColor left, VertexColor right) {
    int y = left.postion.get_y();
    int x_begin = left.postion.get_x();
    int x_end = right.postion.get_x();
    float z_begin = left.postion.get_z();
    float z_end = right.postion.get_z();
    float fctor = (z_end - z_begin)/(x_end - x_begin);
    lh_color dcolor = (right.color - left.color) / (x_end - x_begin);
    lh_color color = left.color;
    for (int i = x_begin; i < x_end; i++) {
        float z = z_begin + (i - x_begin)*fctor;
        setpixel(i, y, z,color);
        color += dcolor;
    }
}