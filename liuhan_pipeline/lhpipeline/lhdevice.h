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
        LH_TRIANGLES_TEXTURE_FILL = 5,
        LH_GRID = 6,
        LH_TEST = 7,
        LH_OFF_DRAW,
    }LHRENDER_STATE;

    class  LhDevice : protected LhFrameBuffer, LhDrawPrimitive
    {
    public:
        LhDevice();
        ~LhDevice();
    public:
        void set_render_state(LHRENDER_STATE state);
        void update_buffer(int w, int h, void* pbits);
        void update_texture(unsigned char* texture_datas, int texture_size);
        void bind_vertex(const float* vertex, const unsigned int* vertex_color, const float* vertex_uv, const int vertex_size);
        void draw();
        void keyboard(/*char vk, */char key);
        void set_front_begin(float, float);
        void set_front(float, float);
    private:
        void draw_line(bool loop = false);
        void draw_line(int x1, int y1, int x2, int y2, int color);
        void draw_triangles_fill();
        void draw_trangles_texture_fill();
        void draw_triangles();
        void z_mip();
        bool get_pos(LhVertexFloat4& f4, LhVertexFloat3 f3);
        void draw_grid();
        void draw_floor();
    public:
        LHRENDER_STATE _render_state = LH_LINES;
        lh_pipeline::LhPipeLine _piple;
    private:
        float _draw_cost_time = 0.0f;
        float _fps = 0.0f;
        float _last_x = 0.0f;
        float _last_y = 0.0f;
		float _r_x = 0;
		float _r_y = 0;
		float _r_z = 0;
		float _m_x = 0;
		float _m_y = 0;
		float _m_z = 0;
		bool _print_log = false;
		bool _grid = false;
		bool _front = true;
    };
}