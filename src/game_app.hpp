#pragma once

#include "glad.h"
#include <SDL.h>
#include <cassert>
#include <cstdio>
#include <cstdint>

enum GameAppKey {
    kGameAppKeyW = 0,
    kGameAppKeyA,
    kGameAppKeyS,
    kGameAppKeyD,
    kGameAppKeyX,
    kGameAppKeyC,
    kGameAppKeyRight,
    kGameAppKeyLeft,
    kGameAppKeyUp,
    kGameAppKeyDown,
    kGameAppKeySpace,
    kGameAppKeyLShift,
    kNumGameAppKey
};

struct GameAppConfig {
    uint32_t width = 0;
    uint32_t height = 0;
    const char* title = nullptr;
    bool debug_gl = false;
};

struct GameAppState {
    bool isPressedLMB = false;
    bool isPressedMMB = false;
    bool isPressedRMB = false;
    uint32_t mouseX = 0;
    uint32_t mouseY = 0;
    uint32_t mousePrevX = 0;
    uint32_t mousePrevY = 0;
    int32_t mouseWheelY = 0;
    bool isPressedKey[kNumGameAppKey] = {false};
    double dt = 0.0;
};

struct GameApp {
    void (*onInit)() = []() {};
    void (*onShutdown)() = []() {};
    void (*onUpdate)(const GameAppState&) = [](const GameAppState&) {};
};

extern void debugGLMessageCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

void runGameApp(GameApp app, GameAppConfig appConfig)
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    SDL_Init(SDL_INIT_EVERYTHING);

    if (appConfig.debug_gl)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow(appConfig.title, SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, appConfig.width,
                                          appConfig.height, SDL_WINDOW_OPENGL);
    assert(window);

    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    assert(glctx);
    SDL_GL_MakeCurrent(window, glctx);

    int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    assert(version != 0);
    printf("OpenGL: %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    assert(GLAD_GL_VERSION_2_1);
    assert(GLAD_GL_EXT_framebuffer_object);

    if (appConfig.debug_gl)
    {
        assert(GLAD_GL_KHR_debug);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugGLMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    app.onInit();

    double fpsCap = 1.0 / 60.0;
    uint64_t freq = SDL_GetPerformanceFrequency();
    uint64_t prevTime = SDL_GetPerformanceCounter();

    GameAppState state = {};

    for (;;)
    {
        uint64_t nowTime = SDL_GetPerformanceCounter();
        double deltaTime = double(nowTime - prevTime) / freq;
        prevTime = nowTime;

        while (deltaTime < fpsCap)
        {
            uint32_t ms = (uint32_t)((fpsCap - deltaTime) * 1000.0);
            SDL_Delay(ms);
            nowTime = SDL_GetPerformanceCounter();
            deltaTime += double(nowTime - prevTime) / freq;
            prevTime = nowTime;
        }

        state.mouseWheelY = 0;
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
            {
                goto app_quit;
            }
            else if (ev.type == SDL_MOUSEWHEEL)
            {
                state.mouseWheelY = ev.wheel.y;
                if (ev.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
                {
                    state.mouseWheelY *= -1;
                }
            }
        }

        int mx, my;
        Uint32 buttons = SDL_GetMouseState(&mx, &my);
        state.mousePrevX = state.mouseX;
        state.mousePrevY = state.mouseY;
        state.mouseX = uint32_t(mx);
        state.mouseY = uint32_t(my);
        state.isPressedLMB = ((buttons & SDL_BUTTON_LMASK) != 0);
        state.isPressedMMB = ((buttons & SDL_BUTTON_MMASK) != 0);
        state.isPressedRMB = ((buttons & SDL_BUTTON_RMASK) != 0);

        static const Uint8 skeys[] = {
            SDL_SCANCODE_W,
            SDL_SCANCODE_A,
            SDL_SCANCODE_S,
            SDL_SCANCODE_D,
            SDL_SCANCODE_X,
            SDL_SCANCODE_C,
            SDL_SCANCODE_RIGHT,
            SDL_SCANCODE_LEFT,
            SDL_SCANCODE_UP,
            SDL_SCANCODE_DOWN,
            SDL_SCANCODE_SPACE,
            SDL_SCANCODE_LSHIFT};
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        for (int i = 0; i < kNumGameAppKey; ++i)
        {
            state.isPressedKey[i] = keys[skeys[i]] > 0 ? true : false;
        }

        state.dt = deltaTime;
        app.onUpdate(state);

        SDL_GL_SwapWindow(window);
    }

app_quit:
    app.onShutdown();

    SDL_GL_DeleteContext(glctx);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

void debugGLMessageCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    printf("---------------\n");
    printf("Debug message (%u): %s\n", id, message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            printf("GL_DEBUG_SOURCE_API");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            printf("GL_DEBUG_SOURCE_APPLICATION");
            break;
        case GL_DEBUG_SOURCE_OTHER:
            printf("GL_DEBUG_SOURCE_OTHER");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            printf("GL_DEBUG_SOURCE_SHADER_COMPILER");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            printf("GL_DEBUG_SOURCE_THIRD_PARTY");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            printf("GL_DEBUG_SOURCE_WINDOW_SYSTEM");
            break;
        default:
            break;
    }
    printf("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            printf("GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR");
            break;
        case GL_DEBUG_TYPE_ERROR:
            printf("GL_DEBUG_TYPE_ERROR");
            break;
        case GL_DEBUG_TYPE_OTHER:
            printf("GL_DEBUG_TYPE_OTHER");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            printf("GL_DEBUG_TYPE_PERFORMANCE");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            printf("GL_DEBUG_TYPE_PORTABILITY");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            printf("GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR");
            break;
        default:
            break;
    }
    printf("\n");

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            printf("GL_DEBUG_SEVERITY_HIGH");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            printf("GL_DEBUG_SEVERITY_LOW");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            printf("GL_DEBUG_SEVERITY_MEDIUM");
            break;
        default:
            break;
    }
    printf("\n\n");
}
