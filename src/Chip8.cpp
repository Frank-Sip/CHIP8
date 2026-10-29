#include "Chip8.h"
#include "CPU.h"

#include <ctime>
#include <chrono>
#include <thread>

Chip8::Chip8()
{
    m_logger = Logger::GetInstance();
    m_cpu = new CPU(this);
    m_emulatorRunning = true;
    m_display = nullptr;
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

    m_cpu->Init();

    m_loader = new RomLoader();
    m_loader->LoadRom(rom_path, m_ram + PROGRAM_START_ADDRESS);
    delete m_loader;

    m_display->Init();
}

void Chip8::Run()
{
    using clock = std::chrono::high_resolution_clock;
    clock::time_point start, end;
    const std::chrono::milliseconds desired_cycle_time(1);

    int display_update_delat_time = 0;

    while (m_emulatorRunning)
    {
        start = clock::now();
        
        m_cpu->Fetch();
        m_cpu->Decode();
        m_cpu->Execute();

        if (display_update_delat_time >= 20)
        {
            display_update_delat_time = 0;
            m_display->Draw(m_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
            m_display->Update();

            if (m_delay_timer > 0)
            {
                m_delay_timer--;
            }

            if (m_sound_timer > 0)
            {
                m_sound_timer--;
            }
        }

        end = clock::now();

        std::chrono::duration<double, std::milli> loop_time = end - start;
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(loop_time);
        auto sleep_time = desired_cycle_time - elapsed_time;

        if (sleep_time.count() > 0)
        {
            std::this_thread::sleep_for(sleep_time);
        }

        display_update_delat_time++;
    }
}
void Chip8::Deinit()
{
    m_cpu->Init();
    m_display->Deinit();
}

void Chip8::SetDisplay(Display* display)
{
    m_display = display;
}
