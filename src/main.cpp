#define SDL_MAIN_HANDLED
#include "game_app.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"
#include "entity.hpp"

Camera gCam;
Sprite gSpr;
Sprite gTileSet[3];
TileMap gTileMap;

void initOpenGL()
{
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    //glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0F, 1.0F, 1.0F, 1.0F);
}

void onInit()
{
    initOpenGL();
    gSpr.loadSprite("icon.png");
    gTileSet[2].loadSprite("tile.png");
    gTileMap.loadTileMap("first.json");
}

void onUpdate(const GameAppState& appState)
{
    glViewport(0, 0, 640, 480);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadMatrixf(mat4Ptr(gCam.getMVP()));
    float x = float(appState.mouseX) - 320.0F;
    float y = (float(appState.mouseY) - 240.0F) * -1.0F;
    gTileMap.drawTileMap(gTileSet);
    gSpr.drawSprite(x, y, deg2Rad(90.0F));
}

void onShutdown()
{
    gSpr.unloadSprite();
}

int main()
{
    stbi_set_flip_vertically_on_load(1);
    Entity e;
    e.id = Entity::genID();
    Entity::addEntity(&e);
    Entity::removeEntity(&e);
    GameAppConfig appConfig;
    appConfig.width = 640;
    appConfig.height = 480;
    appConfig.title = "Haniwa Slayer";
    GameApp app = {};
    app.onInit = onInit;
    app.onUpdate = onUpdate;
    app.onShutdown = onShutdown;
    runGameApp(app, appConfig);
    return 0;
}
