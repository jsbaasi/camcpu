#include "SDL3/SDL.h" // IWYU pragma: keep
#include "SDL3/SDL_main.h" // IWYU pragma: keep

SDL_Window* gSDLWindow;
SDL_Renderer* gSDLRenderer;
static int gDone;
const int WINDOW_WIDTH = 1920 / 2;
const int WINDOW_HEIGHT = 1080 / 2;

bool update()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) return false;
        if (e.type == SDL_EVENT_KEY_UP && e.key.key == SDLK_ESCAPE) return false;
    }

    SDL_Delay(1);
    return true;
}

void draw(Uint64 aTicks)
{
    SDL_SetRenderDrawColor(gSDLRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gSDLRenderer);
    SDL_SetRenderDrawColor(gSDLRenderer, 0, 40, 255, 255);
    SDL_RenderLine(gSDLRenderer, 150, 150, 600, 150); // top
    SDL_RenderLine(gSDLRenderer, 600, 150, 600, 450); // right
    SDL_RenderLine(gSDLRenderer, 600, 450, 300, 450); // bottom
    SDL_RenderLine(gSDLRenderer, 300, 450, 300, 150); // left
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
