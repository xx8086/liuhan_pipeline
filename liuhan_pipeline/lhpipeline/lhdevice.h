﻿#pragma once
#include "..\lhmath\lhdraw\lhdraw_primitive.h"
#include "lhframe_buffer\lhframe_buffer.h"
#include "..\lhmath\lhdetail\lhmatrix_float4.h"
#include "lhpipeline.h"
#include <vector>

namespace lh_pipeline {
    typedef enum  lh_render_state {
        LH_POINTS = 0,
        LH_LINES = 1,
        LH_TRIANGLES = 2,
    }LHRENDER_STATE;

    class  LhDevice : protected LhFrameBuffer, LhDrawPrimitive
    {
    public:
        LhDevice();
        ~LhDevice();
    public:
        void update_buffer(int w, int h, void* pbits);
        void bind_vertex(float* vertex, int triangle_counts);
        void draw_line(int x1, int y1, int x2, int y2, int color);
        void draw();
    private:
        void insert_point(std::vector<float>& v, float a[3]);
        void insert_quadrilateral(std::vector<float>& v, float a[3], float b[3], float c[3], float d[3]);
        bool get_pos(lh_pipeline::LhVertexFloat4& f4, lh_pipeline::LhVertexFloat3 f3);
    public:
        LHRENDER_STATE _render_state = LH_LINES;
        lh_pipeline::LhPipeLine _piple;
    };
}