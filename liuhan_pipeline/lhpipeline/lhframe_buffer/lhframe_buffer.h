#pragma once

class LhFrameBuffer
{
public:
    LhFrameBuffer();
    ~LhFrameBuffer();
protected:
    void set_buffer(int w, int h, void* pbits);
    void update_vertex(float* vertex_buffer, /*float* color_buffer,*/ int counts);
    void clear_deep();
    bool deep_test(int x, int y, float z);
    const float* get_vertex_buffers() { return _vertex_buffers; }
    const int get_vertex_buffers_size() { return _vertex_buffers_size; }
protected:
    int get_width() const;
    int get_height() const;
    unsigned char * get_buffers();
private:
    int _width = 0;
    int _height = 0;
    unsigned char* _frame_buffers = nullptr;
    unsigned int* _frame_rgba = nullptr;
    float* _vertex_buffers = nullptr;//暂时只接收顶点和颜色分开放
    //float* _vertex_color_buffers = nullptr;
    int _vertex_buffers_size = 0;

    float* _frame_deep = nullptr;
};

