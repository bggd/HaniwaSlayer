#pragma once

#include "gmath.hpp"
#include "glad.h"
#include "stb_image.h"
#include <cstdint>
#include <cassert>

struct Sprite {
    Sprite* parent = nullptr;
    GLuint texID = 0;
    uint16_t srcX = 0;
    uint16_t srcY = 0;
    uint16_t width = 0;
    uint16_t height = 0;

    void loadSprite(const char* fileName)
    {
        assert(texID == 0);

        int x, y, c;
        stbi_uc* buffer = stbi_load(fileName, &x, &y, &c, 0);
        assert(buffer);
        assert(c == 4);

        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)buffer);
        glBindTexture(GL_TEXTURE_2D, 0);

        width = x;
        height = y;
        stbi_image_free(buffer);
        printf("loadSprite: %s, width: %d, height: %d\n", fileName, x, y);
    }

    void unloadSprite()
    {
        assert(texID);
        glDeleteTextures(1, &texID);
        texID = 0;
    }

    void loadSubSprite(Sprite& spr, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
        assert(x < parent->width);
        assert(y < parent->height);
        assert(x + w < parent->width);
        assert(y + h < parent->height);

        parent = &spr;
        srcX = x;
        srcY = y;
        width = w;
        height = h;
    }

    void drawSprite(float x, float y, float angleRad = 0.0F) const
    {
        float uvX = 0.0F, uvW = 1.0F;
        float uvY = 0.0F, uvH = 1.0F;
        if (parent)
        {
            uvX = srcX / parent->width;
            uvY = srcY / parent->height;
            uvW = uvX + width / parent->width;
            uvH = uvY + height / parent->height;
        }
        // CCW 2 triangle. top-right as first vtx.
        const Vector4 _pos[6] = {
            //x, y, z, w
            {0.5F, 0.5F, 0.0F, 1.0F},
            {-0.5F, 0.5F, 0.0F, 1.0F},
            {-0.5F, -0.5F, 0.0F, 1.0F},
            {-0.5F, -0.5F, 0.0F, 1.0F},
            {0.5F, -0.5F, 0.0F, 1.0F},
            {0.5F, 0.5F, 0.0F, 1.0F}};
        const Vector3 texCoords[6] = {
            {uvX + uvW, uvY + uvH, 0.0F},
            {uvX, uvY + uvH, 0.0F},
            {uvX, uvY, 0.0F},
            {uvX, uvY, 0.0F},
            {uvX + uvW, uvY, 0.0F},
            {uvX + uvW, uvY + uvH, 0.0F}};

        Vector4 pos[6];
        Matrix4 S = mat4CreateScale(vec3(width, height, 0.0F));
        Matrix4 R = mat4CreateFromAxisAngle(vec3(0.0F, 0.0F, 1.0F), angleRad);
        Matrix4 T = mat4CreateTranslation(vec3(x, y, 0.0F));
        Matrix4 xform = mat4Multiply(mat4Multiply(S, R), T);
        for (int i = 0; i < 6; ++i)
        {
            pos[i] = vec4Transform(_pos[i], xform);
        }

        glBindTexture(GL_TEXTURE_2D, texID);
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 6; ++i)
        {
            Vector3 t = texCoords[i];
            Vector4 p = pos[i];
            glTexCoord2f(t.x, t.y);
            glVertex3f(p.x, p.y, p.z);
        }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
