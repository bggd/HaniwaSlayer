#define SDL_MAIN_HANDLED
#include "game_app.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "sprite_sheet.hpp"

Camera gCam;
Sprite gSpr;
Sprite gTileSet[3];
TileMap gTileMap;

Player player;

Sprite gAtlas;
Sprite gSubSpr;
SpriteSheet gSprSheet;

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
    glClearColor(0.4F, 0.4F, 0.6F, 1.0F);
}

void onInit()
{
    initOpenGL();
    gSpr.loadSprite("icon.png");
    gTileSet[2].loadSprite("tile.png");
    gTileMap.loadTileMap("first.json");
    gTileMap.createWallEntities();
    gAtlas.loadSprite("playerRun.png");
    gSubSpr.loadSubSprite(gAtlas, 32, 0, 32, 32);
    gSprSheet.createSpriteSheet(gAtlas, 32, 32, 4);
    player.onPreload();
}

void onUpdate(const GameAppState& appState)
{
    glViewport(0, 0, 640, 480);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadMatrixf(mat4Ptr(gCam.getMVP()));
    float x = float(appState.mouseX) - 320.0F;
    float y = (float(appState.mouseY) - 240.0F) * -1.0F;
    gTileMap.drawTileMap(gTileSet);
    //gSpr.drawSprite(x, y, deg2Rad(90.0F));
    //gSpr.drawSprite(wall.position.x, wall.position.y);
    player.updateInput(appState);
    player.update();
    //gSpr.drawSprite(player.position.x, player.position.y);
    //gSprSheet.update();
    //gSprSheet.drawFrame(0, 0);
    for (auto& e : gTileMap.walls) {
        drawHitbox(e, 1.0F, 0.0F, 0.0F);
    }
    drawHitbox(player, 0.0F, 1.0F, 0.0F);
}

void onShutdown()
{
    gSpr.unloadSprite();
}

int main()
{
    stbi_set_flip_vertically_on_load(1);

    player.create();
    player.hitbox.x = -5.0F;
    player.hitbox.y = -5.0F;
    player.hitbox.w = 10.0F;
    player.hitbox.h = 9.0F;


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
