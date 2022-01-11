#define SDL_MAIN_HANDLED
#include "game_app.hpp"
#include "camera.hpp"
#include "sprite.hpp"
#include "tilemap.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "sprite_sheet.hpp"
#include "framebuffer.hpp"

#define VSCR_X 384
#define VSCR_Y 216
#define SCR_X (VSCR_X * 2)
#define SCR_Y (VSCR_Y * 2)

Camera gCam;
Sprite gSpr;
Sprite gTileSet[3];
TileMap gTileMap;

Player player;

Sprite gAtlas;
Sprite gSubSpr;
SpriteSheet gSprSheet;

FrameBuffer gFBO[2];

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

    gFBO[0].createFrameBuffer(VSCR_X, VSCR_Y);
    gFBO[1].createFrameBuffer(640, 480);
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

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gFBO[0].fbo);
    glViewport(0, 0, VSCR_X, VSCR_Y);
    glClear(GL_COLOR_BUFFER_BIT);
    Camera cam = gCam;
    cam.position.x -= 4.0F;
    cam.position.y += 4.0F;
    cam.setProjection(mat4CreateOrthographicOffCenter(-VSCR_X / 2.0F, VSCR_X / 2.0F, -VSCR_Y / 2.0F, VSCR_Y / 2.0F, 0.05F, 100.0F));
    glLoadMatrixf(mat4Ptr(cam.getMVP()));
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
    for (auto& e : gTileMap.walls)
    {
        drawHitbox(e, 1.0F, 0.0F, 0.0F, 0.5F);
    }
    //drawHitbox(player, 0.0F, 1.0F, 0.0F, 0.5F);

    glViewport(0, 0, SCR_X, SCR_Y);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    //gCam.setProjection(mat4CreateOrthographicOffCenter(-1.0F, 1.0F, -1.0F, 1.0F, 0.05F, 100.0F));
    //glLoadMatrixf(mat4Ptr(gCam.getMVP()));
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, gFBO[0].tex);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(1.0F, 1.0F, 0.0F);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-1.0F, 1.0F, 0.0F);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-1.0F, -1.0F, 0.0F);

    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-1.0F, -1.0F, 0.0F);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(1.0F, -1.0F, 0.0F);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(1.0F, 1.0F, 0.0F);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void onShutdown()
{
    gFBO[0].destroyFrameBuffer();
    gFBO[1].destroyFrameBuffer();
    gSpr.unloadSprite();
}

int main()
{
    stbi_set_flip_vertically_on_load(1);

    player.create();
    player.hitbox.x = -3.0F;
    player.hitbox.y = -4.0F;
    player.hitbox.w = 6.0F;
    player.hitbox.h = 9.0F;

    GameAppConfig appConfig;
    appConfig.width = SCR_X;
    appConfig.height = SCR_Y;
    appConfig.title = "Haniwa Slayer";
    appConfig.debug_gl = true;
    GameApp app = {};
    app.onInit = onInit;
    app.onUpdate = onUpdate;
    app.onShutdown = onShutdown;
    runGameApp(app, appConfig);
    return 0;
}
