#pragma once
#include "..\lhmath\lhdraw\lhdraw_primitive.h"
#include "lhframe_buffer\lhframe_buffer.h"
#include "..\lhmath\lhdetail\lhmatrix_float4.h"
#include "lhpipeline.h"

namespace lh_pipeline {
    typedef enum  lh_render_state {
        LH_POINTS = 0,
        LH_LINES = 1,
        LH_LINES_LOOP = 2,
        LH_TRIANGLES = 3,
        LH_TRIANGLES_FILL = 4,
        LG_TEST = 5,
    }LHRENDER_STATE;

    class  LhDevice : protected LhFrameBuffer, LhDrawPrimitive
    {
    public:
        LhDevice();
        ~LhDevice();
    public:
        void set_render_state(LHRENDER_STATE state);
        void update_buffer(int w, int h, void* pbits);
        void bind_vertex(float* vertex, int vertex_size);
        void draw();
    private:
        void draw_line(bool loop = false);
        void draw_line(int x1, int y1, int x2, int y2, int color);
        void draw_triangles_fill();
        void draw_triangles();
        bool get_pos(LhVertexFloat4& f4, LhVertexFloat3 f3);
    public:
        LHRENDER_STATE _render_state = LH_LINES;
        lh_pipeline::LhPipeLine _piple;
    };
}