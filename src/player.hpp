#pragma once

#include "entity.hpp"
#include "game_app.hpp"
#include "sprite_sheet.hpp"
#include "helper.hpp"
#include <functional>

struct Input {
    float x = 0.0F;
    float prevX = 0.0F;
    bool turnedX = false;
    bool jumpBtn = false;
    bool jumpBtnP = false;
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

    if (input.jumpBtn == false && appState.isPressedKey[kGameAppKeySpace])
    {
        input.jumpBtnP = true;
    }
    else
    {
        input.jumpBtnP = false;
    }

    if (appState.isPressedKey[kGameAppKeySpace])
    {
        input.jumpBtn = true;
    }
    else
    {
        input.jumpBtn = false;
    }
    return input;
}

enum PlayerSpriteSheet {
    kPlayerSpriteSheetRun = 0,
    kNumPlayerSpriteSheet
};

struct Player : Entity {

    float hsp = 0.0F;
    float vsp = 0.0F;
    float grv = -0.3F;
    float walksp = 4.0F;
    Input input;
    PlayerSpriteSheet currentSpriteSheet = kPlayerSpriteSheetRun;
    Sprite sprites[kNumPlayerSpriteSheet];
    SpriteSheet sprSheets[kNumPlayerSpriteSheet];

    void onPreload() override
    {
        if (!sprites[0].isLoaded())
        {
            sprites[0].loadSprite("playerRun.png");
            sprSheets[0].createSpriteSheet(sprites[0], 32, 32, 4);
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
        moveX(hsp, [this](Entity*) {
            hsp = 0.0F;
            return true;
        });

        vsp = vsp + grv;

        if (onGround() && input.jumpBtnP)
        {
            vsp += 4.0F;
        }

        moveY(vsp, [this](Entity*) {
            vsp = 0.0F;
            return true;
        });

        sprSheets[currentSpriteSheet].update();
        sprSheets[currentSpriteSheet].drawFrame(floorf(position.x), floorf(position.y));
    }

    bool onGround()
    {
        for (Entity* e : Entity::entities)
        {
            if (id == e->id)
            {
                continue;
            }
            Rect hurtbox = Rect(hitbox.x + position.x, hitbox.y + position.y + -1.0F, hitbox.w, hitbox.h);
            if (hurtbox.isHit(e->getHitArea()))
            {
                return true;
            }
        }
        return false;
    }
};
