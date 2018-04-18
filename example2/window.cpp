#include <algorithm>

#include "SDL.h"
#include "SDL_opengl.h"
#ifdef __APPLE__
#	include <OpenGL/glu.h>
#else
#	include <GL/glu.h>
#endif

#include "imguiRenderGL.h"

int InitWindow() {
    // Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Could not initialise SDL.\nError: %s\n", SDL_GetError());
        return -1;
    }

    // Enable depth buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Set color channel depth.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // 4x MSAA.
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    bool presentationMode = false;
    Uint32 flags = SDL_WINDOW_OPENGL;
    int width;
    int height;
    if (presentationMode)
    {
        // Create a fullscreen window at the native resolution.
        width = displayMode.w;
        height = displayMode.h;
        flags |= SDL_WINDOW_FULLSCREEN;
    }
    else
    {
        float aspect = 16.0f / 9.0f;
        width = std::min(displayMode.w, (int)(displayMode.h * aspect)) - 80;
        height = displayMode.h - 80;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    int errorCode = SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer);

    if (errorCode != 0 || !window || !renderer)
    {
        printf("Could not initialise SDL opengl\nError: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_GL_CreateContext(window);

    if (!imguiRenderGLInit("DroidSans.ttf"))
    {
        printf("Could not init GUI renderer.\n");
        SDL_Quit();
        return -1;
    }


    return 0;
}