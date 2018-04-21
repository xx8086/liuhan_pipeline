#pragma once

class LhFrameBuffer
{
public:
    LhFrameBuffer();
    ~LhFrameBuffer();
protected:
    void set_buffer(int w, int h, void* pbits);
    void update_vertex(const float* vertex_buffer, const unsigned int* vertex_buffer_color, const int counts);
    void clear_buffer();
    const float* get_vertex_buffers() { return _vertex_buffers; }
    const unsigned int*  get_vertex_color_buffers() { return _vertex_color_buffers; }
    const int get_vertex_buffers_size() { return _vertex_buffers_size; }
    unsigned char * get_frame_buffers();
    int get_width() const;
    int get_height() const;
private:
    void release();
private:
    int _width = 0;
    int _height = 0;
    unsigned char* _frame_buffers = nullptr;
    float* _vertex_buffers = nullptr;
    unsigned int* _vertex_color_buffers = nullptr;
    int _vertex_buffers_size = 0;
};

