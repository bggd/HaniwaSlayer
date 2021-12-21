#pragma once

#include "gmath.hpp"
#include "glad.h"
#include <vector>
#include <cassert>
#include <cmath>

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
        return (x + w > other.x &&
                y + h > other.y &&
                x < other.x + other.w &&
                y < other.y + other.h);
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

    uint64_t id = 0;
    Vector3 position = vec3Zero();
    Rect hitbox;

    ~Entity()
    {
        assert(id);
    }

    virtual void onPreload() {}

    Rect getHitArea()
    {
        return {hitbox.x + position.x, hitbox.y + position.y, hitbox.w, hitbox.h};
    }

    bool moveX(float x, bool (*onCollide)(Entity* other), float step = 1.0F)
    {
        float total = 0.0F;
        float sign = x > 0.0 ? 1.0F : -1.0F;

        for (bool finished = false; !finished;)
        {
            float mx = sign * step;
            if (fabs(total + sign * step) > fabs(x))
            {
                mx = (fabs(x) - fabs(total)) * sign;
                finished = true;
            }

            for (Entity* e : Entity::entities)
            {
                if (id == e->id)
                {
                    continue;
                }
                Rect hurtbox = Rect(hitbox.x + position.x + mx, hitbox.y + position.y, hitbox.w, hitbox.h);
                if (hurtbox.isHit(e->getHitArea()))
                {
                    onCollide(e);
                    return true;
                }
            }

            position.x += mx;
            total += mx;
        }

        return false;
    }

    bool moveY(float y, bool (*onCollide)(Entity* other), float step = 1.0F)
    {
        float total = 0.0F;
        float sign = y > 0.0 ? 1.0F : -1.0F;

        for (bool finished = false; !finished;)
        {
            float my = sign * step;
            if (fabs(total + sign * step) > fabs(y))
            {
                my = (fabs(y) - fabs(total)) * sign;
                finished = true;
            }

            for (Entity* e : Entity::entities)
            {
                if (id == e->id)
                {
                    continue;
                }
                Rect hurtbox = Rect(hitbox.x + position.x, hitbox.y + position.y + my, hitbox.w, hitbox.h);
                if (hurtbox.isHit(e->getHitArea()))
                {
                    onCollide(e);
                    return true;
                }
            }

            position.y += my;
            total += my;
        }

        return false;
    }
};

inline void drawHitbox(const Entity& e, float r = 1.0F, float g = 1.0F, float b = 1.0F, float a = 1.0F)
{
    float x = floorf(e.position.x) + floorf(e.hitbox.x) + 0.5F;
    float y = floorf(e.position.y) + floorf(e.hitbox.y) + 0.5F;
    float w = floorf(e.hitbox.w) - 1.0F;
    float h = floorf(e.hitbox.h) - 1.0F;
    glBegin(GL_LINES);
    glColor4f(r, g, b, a);
    glVertex3f(x, y, 0.0F);
    glVertex3f(x + w, y, 0.0F);
    glVertex3f(x + w, y, 0.0F);
    glVertex3f(x + w, y + h, 0.0F);
    glVertex3f(x + w, y + h, 0.0F);
    glVertex3f(x, y + h, 0.0F);
    glVertex3f(x, y + h, 0.0F);
    glVertex3f(x, y, 0.0F);
    glEnd();
    glBegin(GL_LINES);
    glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
    glEnd();
}
