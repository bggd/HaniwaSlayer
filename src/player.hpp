#pragma once

#include "entity.hpp"
#include "game_app.hpp"
#include "sprite_sheet.hpp"
#include "helper.hpp"
#include <functional>
#include <algorithm>

struct Input {
    float x = 0.0F;
    float prevX = 0.0F;
    bool turnedX = false;
    bool jumpBtn = false;
    bool prevJumpBtn = false;
    bool jumpBtnJust = false;
    int8_t jumpBuffer = 0;
};

Input handleInput(const GameAppState& appState, Input input)
{
    if (appState.isPressedKey[kGameAppKeyLeft])
    {
        if (appState.isPressedKey[kGameAppKeyRight])
        {
            if (input.turnedX)
            {
                input.x = input.prevX;
            }
            else
            {
                input.x = -input.prevX;
                input.turnedX = true;
            }
        }
        else
        {
            input.x = -1.0F;
            input.turnedX = false;
        }
    }
    else if (appState.isPressedKey[kGameAppKeyRight])
    {
        input.x = 1.0F;
        input.turnedX = false;
    }
    else
    {
        input.x = 0.0F;
        input.turnedX = false;
    }
    input.prevX = input.x;

    input.jumpBtn = appState.isPressedKey[kGameAppKeyUp];
    if (!input.prevJumpBtn && input.jumpBtn)
    {
        input.jumpBtnJust = true;
        input.jumpBuffer = 8;
    }
    else
    {
        input.jumpBtnJust = false;
        input.jumpBuffer = std::max(0, input.jumpBuffer - 1);
    }
    input.prevJumpBtn = input.jumpBtn;

    return input;
}

enum PlayerSpriteSheet {
    kPlayerSpriteSheetIdle = 0,
    kPlayerSpriteSheetRun,
    kPlayerSpriteSheetJump,
    kNumPlayerSpriteSheet
};

struct Player : Entity {

    float hsp = 0.0F;
    float vsp = 0.0F;
    float grv = -0.25F;
    float walksp = 1.25F;
    float direction = 1.0F;
    int8_t coyoteTime = 0;
    Input input;
    PlayerSpriteSheet currentSpriteSheet = kPlayerSpriteSheetIdle;
    Sprite sprites[kNumPlayerSpriteSheet];
    SpriteSheet sprSheets[kNumPlayerSpriteSheet];

    void onPreload() override
    {
        if (!sprites[kPlayerSpriteSheetIdle].isLoaded())
        {
            sprites[kPlayerSpriteSheetIdle].loadSprite("playerIdle.png");
            sprSheets[kPlayerSpriteSheetIdle].createSpriteSheet(sprites[kPlayerSpriteSheetIdle], 32, 32, 1);
        }
        if (!sprites[kPlayerSpriteSheetRun].isLoaded())
        {
            sprites[kPlayerSpriteSheetRun].loadSprite("playerRun.png");
            sprSheets[kPlayerSpriteSheetRun].createSpriteSheet(sprites[kPlayerSpriteSheetRun], 32, 32, 4);
        }
        if (!sprites[kPlayerSpriteSheetJump].isLoaded())
        {
            sprites[kPlayerSpriteSheetJump].loadSprite("playerJump.png");
            sprSheets[kPlayerSpriteSheetJump].createSpriteSheet(sprites[kPlayerSpriteSheetJump], 32, 32, 1);
        }
    }

    void create()
    {
        id = Entity::genID();
        addEntity(this);
    }

    void updateInput(const GameAppState& appState)
    {
        input = handleInput(appState, input);
    }

    void update()
    {
        hsp = input.x * walksp;

        moveX(hsp, [this](Entity* e) {
            if (hsp > 0.0F)
            {
                Rect r = e->getHitArea();
                position.x = r.x - hitbox.x - hitbox.w;
            }
            else if (hsp < 0.0F)
            {
                Rect r = e->getHitArea();
                position.x = r.x + r.w - hitbox.x;
            }
            hsp = 0.0F;
            return true;
        });

        vsp = vsp + grv;

        bool on_ground = onGround();
        if (on_ground)
        {
            coyoteTime = 8;
        }
        else
        {
            coyoteTime = std::max(0, coyoteTime - 1);
        }
        if (coyoteTime > 0 && (input.jumpBtnJust || input.jumpBuffer > 0))
        {
            input.jumpBuffer = 0;
            coyoteTime = 0;
            vsp = 4.0F;
        }

        moveY(vsp, [this](Entity* e) {
            if (vsp > 0.0F)
            {
                Rect r = e->getHitArea();
                position.y = r.y - hitbox.h;
            }
            else if (vsp < 0.0F)
            {
                Rect r = e->getHitArea();
                position.y = r.y + r.h - hitbox.y;
            }
            vsp = 0.0F;
            currentSpriteSheet = kPlayerSpriteSheetIdle;
            return true;
        });

        if (!floatEqual(input.x, 0.0F))
        {
            currentSpriteSheet = kPlayerSpriteSheetRun;
            float prevDirection = direction;
            direction = input.x;
            if (prevDirection != input.x)
            {
                sprSheets[kPlayerSpriteSheetRun].reset();
            }
        }
        else
        {
            sprSheets[kPlayerSpriteSheetRun].reset();
            currentSpriteSheet = kPlayerSpriteSheetIdle;
        }

        if (!floatEqual(vsp, 0.0F) || !on_ground)
        {
            currentSpriteSheet = kPlayerSpriteSheetJump;
        }

        sprSheets[currentSpriteSheet].update();
        sprSheets[currentSpriteSheet].drawFrame(floorf(position.x), floorf(position.y), 0.0F, direction < 0.0F);
    }

    bool onGround()
    {
        for (Entity* e : Entity::entities)
        {
            if (id == e->id)
            {
                continue;
            }
            Rect hurtbox = Rect(hitbox.x + position.x, hitbox.y + position.y - 0.00001F, hitbox.w, hitbox.h);
            if (hurtbox.isHit(e->getHitArea()))
            {
                return true;
            }
        }
        return false;
    }
};
