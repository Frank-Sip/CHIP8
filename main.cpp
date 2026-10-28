#include <SDL3/SDL.h>
#include <iostream>

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL initialization failed: "
                  << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", 640, 320, 0);

    if (!window)
    {
        std::cerr << "Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}