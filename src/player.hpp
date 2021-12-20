#pragma once

#include "entity.hpp"
#include "game_app.hpp"
#include "sprite_sheet.hpp"

struct Input {
    float x = 0.0F;
    float prevX = 0.0F;
    bool turnedX = false;
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
    return input;
}

enum PlayerSpriteSheet {
    kPlayerSpriteSheetRun = 0,
    kNumPlayerSpriteSheet
};

struct Player : Entity {

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
        printf("player id: %d\n", id);
        addEntity(this);
    }

    void updateInput(const GameAppState& appState)
    {
        input = handleInput(appState, input);
    }

    void update()
    {
        moveX(input.x, [](Entity*) { return true; });
        moveY(-1.0F, [](Entity*) { return true; });
        sprSheets[currentSpriteSheet].update();
        sprSheets[currentSpriteSheet].drawFrame(position.x, position.y);
    }
};
