#pragma once
/*
typedef struct { float m[4][4]; } matrix_t;
typedef struct {
    matrix_t world; // 世界坐标变换
    matrix_t view; // 摄影机坐标变换
    matrix_t projection; // 投影变换
    matrix_t transform; // transform = world * view * projection
    float w, h; // 屏幕大小
} transform_t;
*/

typedef enum  lh_render_state {
    LH_POINTS = 0,
    LH_LINES = 1,
    LH_TRIANGLES = 2,
}LHRENDER_STATE;

class LhDevice
{
public:
    LhDevice();
    ~LhDevice();
public:
    void init(int width, int height, void *fb);
    void release();
public:
    void set_texture(void *bits, long pitch, int w, int h);
    void clear_frame_and_z_buffer(int mode);
    void draw_pixel(int x, int y, unsigned int color);
    void draw_line(int x1, int y1, int x2, int y2, unsigned int c);
private:
    //transform_t transform; // 坐标变换器
    int _width; // 窗口宽度
    int _height; // 窗口高度
    unsigned int **_framebuffer; // 像素缓存：framebuffer[y] 代表第 y行
    float **_zbuffer; // 深度缓存：zbuffer[y] 为第 y行指针
    unsigned int **_texture; // 纹理：同样是每行索引
    int _tex_width; // 纹理宽度
    int _tex_height; // 纹理高度
    float _max_u; // 纹理最大宽度：tex_width - 1
    float _max_v; // 纹理最大高度：tex_height - 1
    LHRENDER_STATE _render_state; // 渲染状态
    unsigned int _background; // 背景颜色
    unsigned int _foreground; // 线框颜色
};

