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

    Rect getHitArea() {
        return {hitbox.x + position.x, hitbox.y + position.y, hitbox.w, hitbox.h};
    }

    bool moveX(float x, bool (*onCollide)(Entity* other), float step = 1.0F)
    {
        float total = 0.0F;
        float sign = x > 0.0 ? 1.0F : 0.0F;

        for (;;) {
            float mx = sign * step;
            if (fabs(total + sign * step) > fabs(x)) {
                mx = (x - total) * sign;
            }

            for (Entity* e : Entity::entities) {
                if (id != e->id) { continue; }
                Rect hurtbox = Rect(hitbox.x + position.x + mx, hitbox.y + position.y, hitbox.w, hitbox.h);
                if (hurtbox.isHit(e->getHitArea())) {
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
        float sign = y > 0.0 ? 1.0F : 0.0F;

        for (;;) {
            float my = sign * step;
            if (fabs(total + sign * step) > fabs(y)) {
                my = (y - total) * sign;
            }

            for (Entity* e : Entity::entities) {
                if (id != e->id) { continue; }
                Rect hurtbox = Rect(hitbox.x + position.x, hitbox.y + position.y + my, hitbox.w, hitbox.h);
                if (hurtbox.isHit(e->getHitArea())) {
                    onCollide(e);
                    return true;
                }
            }

            position.x += my;
            total += my;
        }

        return false;
    }
};
