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

struct Player : Entity {

    Input input;
    SpriteSheet sprSheet;

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
        moveX(input.x, [](Entity*) { return true; });
        sprSheet.update();
    }
};
