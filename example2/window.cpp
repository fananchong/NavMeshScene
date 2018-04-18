#include <algorithm>

#include "SDL.h"
#include "SDL_opengl.h"
#ifdef __APPLE__
#	include <OpenGL/glu.h>
#else
#	include <GL/glu.h>
#endif
#include "imgui.h"
#include "imguiRenderGL.h"
#include <thread>

template<class T> inline T rcClamp(T v, T mn, T mx) { return v < mn ? mn : (v > mx ? mx : v); }

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

    // Fog.
    float fogColor[4] = { 0.32f, 0.31f, 0.30f, 1.0f };
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 1000 * 0.1f);
    glFogf(GL_FOG_END, 1000 * 1.25f);
    glFogfv(GL_FOG_COLOR, fogColor);

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    float timeAcc = 0.0f;
    Uint32 prevFrameTime = SDL_GetTicks();

    int mousePos[2] = { 0, 0 };
    int origMousePos[2] = { 0, 0 }; // Used to compute mouse movement totals across frames.

    float cameraEulers[] = { 45, -45 };
    float cameraPos[] = { 0, 0, 0 };
    float camr = 1000;
    float origCameraEulers[] = { 0, 0 }; // Used to compute rotational changes across frames.

    float moveFront = 0.0f, moveBack = 0.0f, moveLeft = 0.0f, moveRight = 0.0f, moveUp = 0.0f, moveDown = 0.0f;

    float scrollZoom = 0;
    bool rotate = false;

    bool done = false;
    while (!done)
    {
        // Handle input events.
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                // Handle any key presses here.
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    done = true;
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y < 0)
                {
                    // wheel down
                    scrollZoom += 1.0f;
                }
                else
                {
                    scrollZoom -= 1.0f;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    // Rotate view
                    rotate = true;
                    origMousePos[0] = mousePos[0];
                    origMousePos[1] = mousePos[1];
                    origCameraEulers[0] = cameraEulers[0];
                    origCameraEulers[1] = cameraEulers[1];
                }
                break;

            case SDL_MOUSEBUTTONUP:
                // Handle mouse clicks here.
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    rotate = false;
                }
                break;

            case SDL_MOUSEMOTION:
                mousePos[0] = event.motion.x;
                mousePos[1] = height - 1 - event.motion.y;

                if (rotate)
                {
                    int dx = mousePos[0] - origMousePos[0];
                    int dy = mousePos[1] - origMousePos[1];
                    cameraEulers[0] = origCameraEulers[0] - dy * 0.25f;
                    cameraEulers[1] = origCameraEulers[1] + dx * 0.25f;
                }
                break;

            case SDL_QUIT:
                done = true;
                break;

            default:
                break;
            }
        }

        unsigned char mouseButtonMask = 0;
        if (SDL_GetMouseState(0, 0) & SDL_BUTTON_LMASK)
            mouseButtonMask |= IMGUI_MBUT_LEFT;
        if (SDL_GetMouseState(0, 0) & SDL_BUTTON_RMASK)
            mouseButtonMask |= IMGUI_MBUT_RIGHT;

        Uint32 time = SDL_GetTicks();
        float dt = (time - prevFrameTime) / 1000.0f;
        prevFrameTime = time;

        // Update simulation.
        const float SIM_RATE = 20;
        const float DELTA_TIME = 1.0f / SIM_RATE;
        timeAcc = rcClamp(timeAcc + dt, -1.0f, 1.0f);
        int simIter = 0;
        while (timeAcc > DELTA_TIME)
        {
            timeAcc -= DELTA_TIME;
            if (simIter < 5)
            {
                void OnUpdate(float delta);
                OnUpdate(DELTA_TIME);
            }
            simIter++;
        }

        // Clamp the framerate so that we do not hog all the CPU.
        const float MIN_FRAME_TIME = 1.0f / 40.0f;
        if (dt < MIN_FRAME_TIME)
        {
            int ms = (int)((MIN_FRAME_TIME - dt) * 1000.0f);
            if (ms > 10) ms = 10;
            if (ms >= 0) SDL_Delay(ms);
        }

        // Set the viewport.
        glViewport(0, 0, width, height);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // Clear the screen
        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        // Compute the projection matrix.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(50.0f, (float)width / (float)height, 1.0f, camr);
        GLdouble projectionMatrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);

        // Compute the modelview matrix.
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(cameraEulers[0], 1, 0, 0);
        glRotatef(cameraEulers[1], 0, 1, 0);
        glTranslatef(-cameraPos[0], -cameraPos[1], -cameraPos[2]);
        GLdouble modelviewMatrix[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);

        // Handle keyboard movement.
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        moveFront = rcClamp(moveFront + dt * 4 * ((keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) ? 1 : -1), 0.0f, 1.0f);
        moveLeft = rcClamp(moveLeft + dt * 4 * ((keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) ? 1 : -1), 0.0f, 1.0f);
        moveBack = rcClamp(moveBack + dt * 4 * ((keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) ? 1 : -1), 0.0f, 1.0f);
        moveRight = rcClamp(moveRight + dt * 4 * ((keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) ? 1 : -1), 0.0f, 1.0f);
        moveUp = rcClamp(moveUp + dt * 4 * ((keystate[SDL_SCANCODE_Q] || keystate[SDL_SCANCODE_PAGEUP]) ? 1 : -1), 0.0f, 1.0f);
        moveDown = rcClamp(moveDown + dt * 4 * ((keystate[SDL_SCANCODE_E] || keystate[SDL_SCANCODE_PAGEDOWN]) ? 1 : -1), 0.0f, 1.0f);

        float keybSpeed = 22.0f;
        if (SDL_GetModState() & KMOD_SHIFT)
        {
            keybSpeed *= 4.0f;
        }

        float movex = (moveRight - moveLeft) * keybSpeed * dt;
        float movey = (moveBack - moveFront) * keybSpeed * dt + scrollZoom * 2.0f;
        scrollZoom = 0;

        cameraPos[0] += movex * (float)modelviewMatrix[0];
        cameraPos[1] += movex * (float)modelviewMatrix[4];
        cameraPos[2] += movex * (float)modelviewMatrix[8];

        cameraPos[0] += movey * (float)modelviewMatrix[2];
        cameraPos[1] += movey * (float)modelviewMatrix[6];
        cameraPos[2] += movey * (float)modelviewMatrix[10];

        cameraPos[1] += (moveUp - moveDown) * keybSpeed * dt;

        glEnable(GL_FOG);

        void OnRender();
        OnRender();

        glDisable(GL_FOG);

        // Render GUI
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        imguiBeginFrame(mousePos[0], mousePos[1], mouseButtonMask, 0);

        void OnRenderOverlay(double* proj, double* model, int* view);
        OnRenderOverlay((double*)projectionMatrix, (double*)modelviewMatrix, (int*)viewport);

        imguiEndFrame();
        imguiRenderGLDraw();

        glEnable(GL_DEPTH_TEST);
        SDL_GL_SwapWindow(window);
    }

    imguiRenderGLDestroy();

    SDL_Quit();

    return 0;
}