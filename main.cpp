#include <SDL3/SDL.h>
#include <iostream>
#include "Logger.h"
#include "cmdLineParser.h"

int main(int argc, char** argv)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
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

    std::shared_ptr<Logger> logger = Logger::GetInstance();

    CMDLineParser parser;
    parser.Parse(argc, argv);

    if (parser.IsLogLevelSet())
    {
        switch (parser.GetLogLevel())
        {
            case 0: logger->SetLogLevel(ELogLevel::NONE); break;
            case 1: logger->SetLogLevel(ELogLevel::INFO); break;
            case 2: logger->SetLogLevel(ELogLevel::ERROR); break;
            case 3: logger->SetLogLevel(ELogLevel::WARNING); break;
            case 4: logger->SetLogLevel(ELogLevel::DEBUG); break;
        }
    }

    if (!parser.IsRomFileNameSet())
    {
        logger->Log("Error: ROM file name not set. Use -r or --rom-file-name to specify the ROM file.", ELogLevel::ERROR);
        exit(1);
    }

    logger->Log("Rom path: " + parser.GetRomFileName(), ELogLevel::INFO);

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