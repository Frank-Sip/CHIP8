#include "cmdLineParser.h"
#include "Chip8.h"
#include "SDLDisplay.h"
#include "KeyboardSDL.h"

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
    KeyboardSDL keyboard;
    emulator.SetKeyboard(&keyboard);
    emulator.Init(parser.GetRomFileName());
    emulator.Run();
    emulator.Deinit();

    return 0;
}