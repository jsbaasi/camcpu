#include "SDL3/SDL.h" // IWYU pragma: keep
#include "SDL3/SDL_main.h" // IWYU pragma: keep
#include "math.h"
#include <stdbool.h>
#include <stdio.h>

SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
static int gDone;
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;
static Vec4 square[4] = {
    {-2, 2, 3, 1},
    {2, 2, 3, 1},
    {2, -2, 3, 1},
    {-2, -2, 3, 1},
};
static const float n=1, l=-4, r=4, b=-4, t=4;
static const float X=0, Y=0, W=WINDOW_WIDTH, H=WINDOW_HEIGHT, N=0, F=1;
static Mat4 proj = {
    n/r,0,0,0,
    0,n/t,0,0,
    0,0,-1,-2*n,
    0,0,-1,0,
};
static Mat4 viewport = {
    W/2,0,0,X+W/2,
    0,H/2,0,Y+H/2,
    0,0,(F-N)/2,(F+N)/2,
    0,0,0,1,
};
static Vec4 viewportSquare[4] = {};
static int printed = 0;

// calculate where the points are
bool update()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) return false;
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) return false;
    }

    if (printed==0) {
        Vec4 projSquare[4] = {};
        Vec4 dehomoSquare[4] = {};
        for (int i = 0; i<4; i++) {
            printf("Point %d\n", i);
            vec4_print(&square[i]);
            printf("Projected VVV\n");
            mat4_vec4_mult(&proj, &square[i], &projSquare[i]);
            vec4_print(&projSquare[i]);
            vec4_dehomo(&projSquare[i], &dehomoSquare[i]);
            printf("Dehomogonized VVV\n");
            vec4_print(&dehomoSquare[i]);
            printf("Viewport VVV\n");
            mat4_vec4_mult(&viewport, &dehomoSquare[i], &viewportSquare[i]);
            vec4_print(&viewportSquare[i]);
            printf("\n\n\n");
        }
        printed = 1;
    }

    SDL_Delay(1);
    return true;
}

// draw the lines between the points calculated
void draw(Uint64 aTicks)
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
    draw(SDL_GetTicks());
}

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        return -1;
    }
    gSDLWindow = SDL_CreateWindow("SDL3 window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    gSDLRenderer = SDL_CreateRenderer(gSDLWindow, NULL);

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
