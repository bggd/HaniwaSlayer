#pragma once

#include "sprite.hpp"
#include <cstdint>
#include <cmath>

struct SpriteSheet {
    Sprite* source = nullptr;
    uint16_t frameWidth = 0;
    uint16_t frameHeight = 0;
    uint16_t columns = 0;
    uint16_t rows = 0;
    uint16_t frameMax = 0;
    uint32_t frameIndex = 0;
    uint32_t _i = 0;
    uint32_t frameRate = 0;

    void createSpriteSheet(Sprite& source, uint16_t frameWidth, uint16_t frameHeight, uint32_t frameRate)
    {
        printf("createSpriteSheet: frameWidth: %d, frameHeight: %d, frameRate: %d\n", frameWidth, frameHeight, frameRate);
        columns = ceilf(source.width / float(frameWidth));
        rows = ceilf(source.height / float(frameHeight));
        frameMax = columns * rows;
        this->source = &source;
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;
        this->frameRate = frameRate;
    }

    void update()
    {
        _i++;
        if (_i >= frameRate)
        {
            _i = 0;
            frameIndex++;
        }
        if (frameIndex >= frameMax)
        {
            frameIndex = 0;
        }
    }

    void drawFrame(float dx, float dy)
    {
        assert(frameIndex < frameMax);

        uint16_t x = frameIndex % columns;
        uint16_t y = floorf(frameIndex / float(columns));
        float sx = frameWidth * float(x);
        float sy = frameHeight * float(y);
        Sprite spr;
        spr.loadSubSprite(*source, sx, sy, frameWidth, frameHeight);
        spr.drawSprite(dx, dy);
    }
};
