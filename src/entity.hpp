#pragma once

#include "gmath.hpp"
#include <vector>
#include <cassert>

struct Rect {
    float x, y, w, h;

    Rect()
    {
        x = 0.0F;
        y = 0.0F;
        w = 0.0F;
        h = 0.0F;
    }

    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    bool isHit(const Rect& other)
    {
        return (x + w >= other.x &&
                x <= other.x + other.w &&
                y + h >= other.h &&
                y <= other.y + other.h);
    }
};

struct Entity {
    static std::vector<Entity*> entities;
    static uint64_t uid;

    static uint64_t genID()
    {
        return ++uid;
    }

    static void addEntity(Entity* e)
    {
        assert(e->id);
        entities.push_back(e);
    }

    static void removeEntity(Entity* e)
    {
        assert(e->id);

        for (size_t i = 0; i < entities.size(); ++i)
        {
            if (e->id == entities[i]->id)
            {
                entities[i] = entities.back();
                entities.pop_back();
                return;
            }
        }
        abort();
    }

    ~Entity()
    {
        assert(id);
    }

    uint64_t id = 0;
    Vector3 position = vec3Zero();
    Rect hitbox;

    void moveBy(Vector3 amount)
    {
        assert(id);

        float x = 0.0F;
        for (;;)
        {
            float sign = (amount.x > 0.0F)? 1.0F : -1.0F;
            if (fabs(x) > fabs(amount.x))
            {
                Rect hurtbox = Rect(position.x + x, position.y, hitbox.w, hitbox.y);
                for (Entity* e : entities)
                {
                    if (e->id == id)
                    {
                        continue;
                    }
                    if (hurtbox.isHit(e->hitbox))
                    {
                        if (onMoveCollideX(*e))
                        {
                            goto GOTO_FINISH_X;
                        }
                    }
                }
                x += sign;
            }
            else
            {
                break;
            }
        }
    GOTO_FINISH_X:
        position.x += x;
        float y = 0.0F;
        for (;;)
        {
            float sign = (amount.y > 0.0F)? 1.0F : -1.0F;
            if (fabs(y) > fabs(amount.y))
            {
                Rect hurtbox = Rect(position.x, position.y + y, hitbox.w, hitbox.y);
                for (Entity* e : entities)
                {
                    if (e->id == id)
                    {
                        continue;
                    }
                    if (hurtbox.isHit(e->hitbox))
                    {
                        if (onMoveCollideY(*e))
                        {
                            goto GOTO_FINISH_Y;
                        }
                    }
                }
                y += sign;
            }
            else
            {
                break;
            }
        }
    GOTO_FINISH_Y:
        position.y += y;
    }

    void moveBy(float x, float y)
    {
        moveBy(vec3(x, y, 0.0F));
    }

    virtual bool onMoveCollideX(Entity& other)
    {
        return true;
    }

    virtual bool onMoveCollideY(Entity& other)
    {
        return true;
    }

    virtual void onUpdate() {
    }
};
