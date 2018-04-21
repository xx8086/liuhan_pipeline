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
   
protected:
    typedef enum TextureLevel {
        TEXTURE_LEVEL_0 = 0,
        TEXTURE_LEVEL_1 = 1,
        TEXTURE_LEVEL_2 = 2,
        TEXTURE_LEVEL_3 = 3,
        TEXTURE_LEVEL_4 = 4,
        TEXTURE_LEVEL_MAX,
    }TEXTURE_LEVEL;
    void set_texture(unsigned char* texture_datas, int texture_size);
private:
    int texture_size_check(int texture_size);
    void release();
private:
    int _width = 0;
    int _height = 0;
    unsigned char* _frame_buffers = nullptr;
    unsigned int* _vertex_color_buffers = nullptr;
    float* _vertex_buffers = nullptr;
    int _vertex_buffers_size = 0;
    unsigned char* _texture[TEXTURE_LEVEL_MAX] = { nullptr };
    int _texture_size[TEXTURE_LEVEL_MAX] = { 0 };
    
};

