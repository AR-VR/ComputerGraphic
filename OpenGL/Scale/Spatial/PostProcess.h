#pragma once

class PostProcess
{
public:
    PostProcess(const unsigned int screen_width, const unsigned int screen_height);
    void Process();
    void Resize(const unsigned int screen_width, const unsigned int screen_height);
    ~PostProcess();
private:
    unsigned int fbo;
    unsigned int fbo_texture;
    unsigned int rbo_depth;
    unsigned int vbo_fbo_vertices;
};

