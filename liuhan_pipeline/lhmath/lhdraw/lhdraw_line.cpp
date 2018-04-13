#include "lhdraw_line.h"
#include <cmath>


dLhDrawLine::dLhDrawLine()
{
}


dLhDrawLine::~dLhDrawLine()
{
}

void dLhDrawLine::setpixel(int x, int y, int color) {
    ;
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
                        / |
                     /    |
                  /       |
               /          |
            /             | dy / dm 
         /                |
      /                   |
   /                      |
.P1------------------------
单位长度(1) dx / dm
*/
void dLhDrawLine::dda(int x1, int x2, int y1, int y2, int color) {
    int dm = 0;
    int tx = abs(x2 - x1);
    int ty = abs(y2 - y1);
    dm = (tx >= ty ? tx : ty);

    float x = x1 + 0.5;
    float y = y1 + 0.5;
    float dx = float(tx / dm);
    float dy = float(ty / dm);

    for (int i = 0; i < dm; i++) {
        setpixel((int)x, (int)y, color);
        x += dx;
        y += dy;
    }
}