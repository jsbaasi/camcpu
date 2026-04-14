#include "SDL3/SDL.h" // IWYU pragma: keep
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_main.h" // IWYU pragma: keep
#include "SDL3/SDL_timer.h"
#include "linear.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
static int gDone;
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;
static float depth = 0.30f;
static Vec4 square[4] = {
    {-4, 4, -2, 1},
    {4, 4, -8, 1},
    {4, -4, -8, 1},
    {-4, -4, -8, 1},
};
static const float n=1, l=-8, r=8, b=-4.5f, t=4.5f; // 16/9 (1920x1080) viewport
static Mat4 proj = {
    (2*n)/(r-l),0,(r+l)/(r-l),0,
    0,(2*n)/(t-b),(t+b)/(t-b),0,
    0,0,-1,-2*n,
    0,0,-1,0,
};
static const float X=0, Y=0, W=WINDOW_WIDTH, H=WINDOW_HEIGHT, N=1, F=1;
static Mat4 viewport = {
    W/2,0,0,X+W/2,
    0,H/2,0,Y+H/2,
    0,0,(F-N)/2,(F+N)/2,
    0,0,0,1,
};
static Mat4 view = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
};
static Vec4 viewportSquare[4] = {};
static int printed = 0;

static Vec4 cameraPos = {0,0,0,1};
static Vec4 cameraForwardBasis = {0,0,1,1};
static Vec4 cameraUpBasis = {0,1,0,1};
static Vec4 cameraLeftBasis = {1,0,0,1};
static unsigned int currentTime, dt=0;
static float yaw = -90.0f;
static float pitch = 0;
static float sensitivity = 0.1f;

// calculate where the points are
// relx > how many units was moved in the x axis of the 2d screen
// > 
bool update()
{
    currentTime = SDL_GetTicks();
    dt = currentTime - dt;

    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) return false;
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) return false;
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_S) {
            vec4_scalar_add(&cameraPos, &cameraForwardBasis, 1, &cameraPos);
        }
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_D) {
            vec4_scalar_add(&cameraPos, &cameraLeftBasis, 1, &cameraPos);
        }
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_W) {
            vec4_scalar_subtract(&cameraPos, &cameraForwardBasis, 1, &cameraPos);
        }
        if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_A) {
            vec4_scalar_subtract(&cameraPos, &cameraLeftBasis, 1, &cameraPos);
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION) {
            printf("Relative delta: %f, %f\n", e.motion.xrel, e.motion.yrel);
            yaw += e.motion.xrel * sensitivity;
            pitch += e.motion.yrel * sensitivity;
            if (pitch>89.9) pitch = 89.9;
            if (pitch<-89.9) pitch = -89.9;
        }
    }

    /*
     * create our view matrix
     * i x j = k
     * j = the up vector here as we don't roll
     * we cross product forward and up to get the k vector
     */
    set_camera_basis(&cameraForwardBasis, &cameraUpBasis, &cameraLeftBasis, yaw, pitch);




    view.x3 = -cameraPos.x;
    view.y3 = -cameraPos.y;
    view.z3 = -cameraPos.z;

    /*
     *  a square is a collection of 4 vertices which is distinct from a matrix
    */

    Vec4 viewSquare[4] = {};
    Vec4 projSquare[4] = {};
    Vec4 dehomoSquare[4] = {};
    for (int i = 0; i<4; i++) { // loop over all vertices, in our case it's just 4 for the square
        mat4_vec4_mult(&view, &square[i], &viewSquare[i]);
        // printf("viewsquare%d", i);
        // vec4_print(&viewSquare[i]);
        mat4_vec4_mult(&proj, &viewSquare[i], &projSquare[i]);
        // printf("projsquare%d", i);
        // vec4_print(&projSquare[i]);
        vec4_dehomo(&projSquare[i], &dehomoSquare[i]);
        mat4_vec4_mult(&viewport, &dehomoSquare[i], &viewportSquare[i]);
    }
    printf("Square\n");
    square_print(square);
    printf("View Square\n");
    square_print(viewSquare);
    printf("Projected + Clipped Square\n");
    square_print(projSquare);
    printf("Normalised Device Coordinate Square\n");
    square_print(dehomoSquare);
/*
 *        *
viewsquare0Vec4(x=-2.000000,y=2.000000,z=-1.000000,w=1.000000)
projsquare0Vec4(x=-0.250000,y=0.444444,z=-1.000000,w=1.000000)
*/
    SDL_Delay(1);
    return true;
}

// draw the lines between the points calculated
void draw()
{
    SDL_SetRenderDrawColor(gSDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gSDLRenderer);
    SDL_SetRenderDrawColor(gSDLRenderer, 0, 40, 255, 255);

    SDL_RenderLine(gSDLRenderer, viewportSquare[0].x, viewportSquare[0].y, viewportSquare[1].x, viewportSquare[1].y); // top
    SDL_RenderLine(gSDLRenderer, viewportSquare[1].x, viewportSquare[1].y, viewportSquare[2].x, viewportSquare[2].y); // top
    SDL_RenderLine(gSDLRenderer, viewportSquare[2].x, viewportSquare[2].y, viewportSquare[3].x, viewportSquare[3].y); // top
    SDL_RenderLine(gSDLRenderer, viewportSquare[3].x, viewportSquare[3].y, viewportSquare[0].x, viewportSquare[0].y); // top

    SDL_RenderPresent(gSDLRenderer);
}

// update first then render
// copy the frame buffer into the texture to be drawn
void loop()
{
    if (!update()) {
        gDone = 1;
    }
    draw();
}

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        return -1;
    }
    gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);
    SDL_SetWindowRelativeMouseMode(gSDLWindow, true);

    if (!gSDLWindow || !gSDLRenderer)
        return -1;

    gDone = 0;
    while (!gDone) {
        loop();
    }

    SDL_DestroyRenderer(gSDLRenderer);
    SDL_DestroyWindow(gSDLWindow);
    SDL_Quit();
    return 0;
}
