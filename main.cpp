#include <SDL3/SDL.h>
#include <iostream>

#include "cmdLineParser.h"
#include "Chip8.h"
#include "SDLDisplay.h"

int main(int argc, char** argv)
{
    std::shared_ptr<Logger> logger = Logger::GetInstance();

    CMDLineParser parser;
    parser.Parse(argc, argv);

    if (parser.IsLogLevelSet())
    {
        switch (parser.GetLogLevel())
        {
            case 0: logger->SetLogLevel(ELogLevel::NONE); break;
            case 1: logger->SetLogLevel(ELogLevel::INFO); break;
            case 2: logger->SetLogLevel(ELogLevel::WARNING); break;
            case 3: logger->SetLogLevel(ELogLevel::ERROR); break;
            case 4: logger->SetLogLevel(ELogLevel::DEBUG); break;
        }
    }
   
    if (!parser.IsRomFileNameSet())
    {
        logger->Log("ROM file name not set. Use -r or --rom-file-name to specify the ROM file.", ELogLevel::ERROR);

        return 1;
    }

    logger->Log("ROM path: " + parser.GetRomFileName(), ELogLevel::INFO);
    
    DisplaySDL display;
    Chip8 emulator;
    emulator.SetDisplay(&display);
    emulator.Init(parser.GetRomFileName());
    emulator.Run();
    emulator.Deinit();

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        logger->Log("SDL initialization failed: " + std::string(SDL_GetError()), ELogLevel::ERROR);
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", 640, 320, 0);

    if (!window)
    {
        logger->Log("Window creation failed: " + std::string(SDL_GetError()), ELogLevel::ERROR);
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

    return 0;
}
