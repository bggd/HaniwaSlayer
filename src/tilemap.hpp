#pragma once

#include "sprite.hpp"
#include "entity.hpp"
#include <nlohmann/json.hpp>
#include <cstdint>
#include <fstream>
#include <vector>

struct TileMap {
    uint8_t* tileLayer = nullptr;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t tileWidth = 0;
    uint16_t tileHeight = 0;
    std::vector<Entity> walls;

    void loadTileMap(const char* fileName)
    {
        assert(tileLayer == nullptr);

        std::ifstream file(fileName);
        assert(file.good());
        nlohmann::json j;
        file >> j;

        width = j["width"].get<uint16_t>();
        height = j["height"].get<uint16_t>();
        tileWidth = j["tilewidth"].get<uint16_t>();
        tileHeight = j["tileheight"].get<uint16_t>();
        printf("loadTileMap: %s, width: %d, height: %d, tileWidth: %d, tileHeight: %d\n", fileName, width, height, tileWidth, tileHeight);
        tileLayer = new uint8_t[width * height];
        assert(tileLayer);
        for (int i = 0; i < width * height; ++i)
        {
            tileLayer[i] = j["layers"][0]["data"][i].get<uint8_t>();
        }
    }

    void unloadTileMap()
    {
        assert(tileLayer);
        delete[] tileLayer;
        tileLayer = nullptr;
    }

    // top-left is (-1, -1) in 2x2 tiles
    uint8_t getTile(int16_t x, int16_t y)
    {
        x += width / 2;
        y += height / 2;
        assert(x >= 0);
        assert(y >= 0);
        assert(x < width);
        assert(y < height);
        return tileLayer[width * y + x];
    }

    void createWallEntities()
    {
        float x = 0.0F;
        float y = 0.0F;
        float offsetX = width * tileWidth / 2.0F;
        float offsetY = height * tileHeight / 2.0F;
        for (uint16_t ty = 0; ty < height; ++ty)
        {
            y = float(ty) * tileHeight;
            for (uint16_t tx = 0; tx < width; ++tx)
            {
                x = float(tx) * tileWidth;
                uint8_t idx = tileLayer[width * ty + tx];
                if (idx > 0)
                {
                    Entity e;
                    e.id = Entity::genID();
                    e.position.x = x - offsetX;
                    e.position.y = (y - offsetY) * -1.0F;
                    e.hitbox.x = -4.0F;
                    e.hitbox.y = -4.0F;
                    e.hitbox.w = 8.0F;
                    e.hitbox.h = 8.0F;
                    walls.push_back(e);
                }
            }
        }
        for (Entity& e : walls)
        {
            Entity::addEntity(&e);
        }
    }

    void drawTileMap(const Sprite* tilesets)
    {
        float x = 0.0F;
        float y = 0.0F;
        float offsetX = width * tileWidth / 2.0F;
        float offsetY = height * tileHeight / 2.0F;
        for (uint16_t ty = 0; ty < height; ++ty)
        {
            y = float(ty) * tileHeight;
            for (uint16_t tx = 0; tx < width; ++tx)
            {
                x = float(tx) * tileWidth;
                uint8_t idx = tileLayer[width * ty + tx];
                if (idx > 0)
                {
                    tilesets[idx].drawSprite(x - offsetX, (y - offsetY) * -1.0F);
                }
            }
        }
    }
};
