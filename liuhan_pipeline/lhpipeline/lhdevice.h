#pragma once
#include "..\lhmath\lhdraw\lhdraw_primitive.h"
#include "lhframe_buffer\lhframe_buffer.h"

typedef enum  lh_render_state {
    LH_POINTS = 0,
    LH_LINES = 1,
    LH_TRIANGLES = 2,
}LHRENDER_STATE;

class  LhDevice : protected LhFrameBuffer, LhDrawPrimitive
{
public:
     LhDevice();
    ~ LhDevice();
public:
    void update_buffer(int w, int h, void* pbits);
    void draw(int x1, int x2, int y1, int y2, int color);
public:
    LHRENDER_STATE _render_state = LH_LINES;
};