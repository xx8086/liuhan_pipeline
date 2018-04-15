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

void LhDrawPrimitive::setpixel(int y, int x, int color) {
    _frame_buffers[(x*_width + y + 1) * 4 + 0] = 0;//b
    _frame_buffers[(x*_width + y + 1) * 4 + 1] = 0;//g
    _frame_buffers[(x*_width + y + 1) * 4 + 2] = 255;//r
    _frame_buffers[(x*_width + y + 1) * 4 + 3] = 255;//a
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
.P1----
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
.P1------------------------
单位长度(1) dx / dm
*/
void LhDrawPrimitive::line_dda(int x1, int y1, int x2, int y2, int color) {
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


void LhDrawPrimitive::line_lh(int x1, int y1, int x2, int y2, unsigned int c) {
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
