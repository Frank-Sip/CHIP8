#include "Chip8.h"
#include "Logger.h"

Chip8::Chip8()
{
    m_logger = Logger::GetInstance();
    m_emulatorRunning = false;
}

Chip8::~Chip8() {}

void Chip8::Init(std::string rom_path)
{
    //Clear screen
    for (auto i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (auto j = 0; j < SCREEN_WIDTH; j++)
        {
            m_screen[i][j] = 0;
        }
    }

    //Clear stack
    for (auto i = 0; i < STACK_SIZE; i++)
    {
        m_stack[i] = 0;
    }

    //Clear RAM
    for (auto i = 0; i < TOTAL_RAM; i++)
    {
        m_ram[i] = 0;
    }

    //Clear fontset
    for (auto i = 0; i < FONTSET_SIZE; i++)
    {
        m_ram[i] = FONTSET[i];
    }

    //Clear timers
    for (auto i = 0; i < NUM_KEYS; i++)
    {
        m_keys[i] = 0;
    }

    m_keyPressed = false;

    m_loader = new RomLoader();
    m_loader->LoadRom(rom_path, m_ram + PROGRAM_START_ADDRESS);
    delete m_loader;
}

void Chip8::Run() {}
void Chip8::Deinit() {}