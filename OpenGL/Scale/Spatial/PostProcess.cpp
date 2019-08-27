#include "PostProcess.h"
#include "GLErrorCheck.h"


PostProcess::PostProcess(const unsigned int screen_width, const unsigned int screen_height)
{
    /* Texture */
    GL_EXEC(glActiveTexture(GL_TEXTURE0));
    GL_EXEC(glGenTextures(1, &fbo_texture));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, fbo_texture));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_EXEC(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));

    /* Depth buffer */
    GL_EXEC(glGenRenderbuffers(1, &rbo_depth));
    GL_EXEC(glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth));
    GL_EXEC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width, screen_height));
    GL_EXEC(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    /* Framebuffer to link everything together */
    GL_EXEC(glGenFramebuffers(1, &fbo));
    GL_EXEC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    GL_EXEC(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0));
    GL_EXEC(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth));
    GL_CheckFameBufferStatus();
    GL_EXEC(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    /* init_resources */
    GLfloat fbo_vertices[] = {
      -1, -1,
       1, -1,
      -1,  1,
       1,  1,
    };
    GL_EXEC(glGenBuffers(1, &vbo_fbo_vertices));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices));
    GL_EXEC(glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW));
    GL_EXEC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void PostProcess::Process()
{
    GL_EXEC(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    // draw (without glutSwapBuffers)
    GL_EXEC(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PostProcess::Resize(const unsigned int screen_width, const unsigned int screen_height)
{
    // Rescale FBO and RBO as well
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, fbo_texture));
    GL_EXEC(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GL_EXEC(glBindTexture(GL_TEXTURE_2D, 0));

    GL_EXEC(glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth));
    GL_EXEC(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width, screen_height));
    GL_EXEC(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

PostProcess::~PostProcess()
{
    /* free_resources */
    GL_EXEC(glDeleteBuffers(1, &vbo_fbo_vertices));

    GL_EXEC(glDeleteRenderbuffers(1, &rbo_depth));
    GL_EXEC(glDeleteTextures(1, &fbo_texture));
    GL_EXEC(glDeleteFramebuffers(1, &fbo));
}
