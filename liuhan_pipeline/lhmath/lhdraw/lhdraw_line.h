#pragma once
class dLhDrawLine
{
public:
    dLhDrawLine();
    ~dLhDrawLine();
private:
    void setpixel(int x, int y, int color);
private:
    void dda(int x1, int x2, int y1, int y2, int color);

};

