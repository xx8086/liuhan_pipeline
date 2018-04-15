#pragma once

class LhFrameBuffer
{
public:
    LhFrameBuffer();
    ~LhFrameBuffer();
protected:
    void set_buffer(int w, int h, void* pbits);
    void update();
protected:
    int get_width() const;
    int get_height() const;
    unsigned char * get_buffers();
private:
    int _width = 0;
    int _height = 0;
    unsigned char *_frame_buffers = nullptr;
    unsigned int* _frame_rgba = nullptr;
};

