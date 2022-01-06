#pragma once

#include "glad.h"
#include <cassert>
#include <cstdint>

struct FrameBuffer {
    GLuint fbo = 0;
    GLuint tex = 0;

    void createFrameBuffer(uint16_t width, uint16_t height)
    {
        assert(!fbo);
        assert(!tex);

        glGenFramebuffersEXT(1, &fbo);
        assert(fbo);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);

        glGenTextures(1, &tex);
        assert(tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        assert(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }

    void destroyFrameBuffer()
    {
        assert(tex);
        glDeleteTextures(1, &tex);
        tex = 0;
        assert(fbo);
        glDeleteFramebuffersEXT(1, &fbo);
        fbo = 0;
    }
};
