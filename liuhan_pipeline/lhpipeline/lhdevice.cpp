#include "lhdevice.h"
#include <assert.h>
#include <memory.h>
#include <stdlib.h >
#include <vector>

LhDevice::LhDevice()
{
}


LhDevice::~LhDevice()
{
}

void LhDevice::update_buffer(int w, int h, void* pbits) {
    LhDrawPrimitive::set_buffer(w, h, pbits);
    LhFrameBuffer::set_buffer(w, h, pbits);
    _piple.set_windows_size(w, h);
    update_vertex();
}

void LhDevice::draw_line(int x1, int y1, int x2, int y2, int color) {
    line_bresenham(x1, y1, x2, y2, lh_color(color));
    //line_dda(x1 + 2, y1, x2, y2 - 2, lh_color(0, 0, 255));
}

void insert_point(std::vector<float>& v, float a[3]) {
    v.push_back(a[0]);
    v.push_back(a[1]);
    v.push_back(a[2]);
}
void insert_quadrilateral(std::vector<float>& v, float a[3], float b[3], float c[3], float d[3]) {
    //abc, acd
    insert_point(v, a);
    insert_point(v, b);
    insert_point(v, c);

    insert_point(v, a);
    insert_point(v, c);
    insert_point(v, d);

}
void LhDevice::update_vertex(/*float* vertex, int triangle_counts*/) {
    float vertex[8][3] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
    };

    std::vector<float> vecs;
    insert_quadrilateral(vecs, vertex[0], vertex[1], vertex[2], vertex[3]);
    insert_quadrilateral(vecs, vertex[4], vertex[5], vertex[6], vertex[7]);
    insert_quadrilateral(vecs, vertex[0], vertex[4], vertex[5], vertex[1]);
    insert_quadrilateral(vecs, vertex[1], vertex[5], vertex[6], vertex[2]);
    insert_quadrilateral(vecs, vertex[2], vertex[6], vertex[7], vertex[3]);
    insert_quadrilateral(vecs, vertex[3], vertex[7], vertex[4], vertex[0]);

    update_triangle_vertex(vecs.data(), 12);

    //_piple.set_rotate(1, 1, 0);
    _piple.set_sale(1.0f, 1.0f, 1.0f);
    _piple.set_worldpos(0, 0, 0);
    _piple.set_camera(lh_pipeline::LhVertexFloat3(0.0f, 0.0f, -3.0f), lh_pipeline::LhVertexFloat3(0.0f, 0.0f, 1.0f), lh_pipeline::LhVertexFloat3(0.0f, 1.0f, 0.0f));
    lh_pipeline::PersProjInfo per;
    per.fov = 60.0f;
    per.width = get_width();
    per.height = get_height();
    per.z_near = 1.0f;
    per.z_far = 100.0f;
    _piple.set_perspective_proj(per);

}

bool LhDevice::get_pos(lh_pipeline::LhVertexFloat4& f4, lh_pipeline::LhVertexFloat3 f3){
    f4 = _piple.transformation_in_mvp(f3);
    /*if (0 == _piple.transformation_cut(f4)) {
        return false;
    }*/
    f4 = _piple.transformation_normalization(f4);
    return true;
}

void LhDevice::draw() {
    const float* v =get_vertex_buffers();
    const int counts = get_triangle_counts() * 9;
    _piple.get_wvp();
    lh_color lc1(255, 0, 0);
    lh_color lc2(0, 255, 0);
    lh_color lc3(0, 0, 255);
    int color1 = lc1.get_t<int>();
    int color2 = lc2.get_t<int>();
    int color3 = lc3.get_t<int>();
    for (int i = 0; i <counts; i += 9) {
        lh_pipeline::LhVertexFloat4 p1, p2, p3;
        if (get_pos(p1, lh_pipeline::LhVertexFloat3(v[i], v[i + 1], v[i + 2])) &&
            get_pos(p2, lh_pipeline::LhVertexFloat3(v[i + 3], v[i + 4], v[i + 5])) &&
            get_pos(p3, lh_pipeline::LhVertexFloat3(v[i + 6], v[i + 7], v[i + 8]))) {
            draw_line(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y(), color1);
            draw_line(p2.get_x(), p2.get_y(), p3.get_x(), p3.get_y(), color2);
            draw_line(p3.get_x(), p3.get_y(), p2.get_x(), p2.get_y(), color3);
        }
    }
}
