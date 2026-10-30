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
    m_keyboard = nullptr;
    m_sound = nullptr;
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
    m_keyboard->Init();
    m_sound->Init();
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
        
        m_keyboard->Update(m_keys, &m_emulatorRunning);
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
                m_sound->PlayTune();
                m_sound_timer--;
            }
            else
            {
                m_sound->PauseTune();
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
    m_cpu->Deinit();
    m_display->Deinit();
    m_keyboard->Deinit();
    m_sound->Deinit();
}

void Chip8::SetDisplay(Display* display)
{
    m_display = display;
}

void Chip8::SetKeyboard(Keyboard* keyboard)
{
    m_keyboard = keyboard;
}

void Chip8::SetSound(Sound* sound)
{
    m_sound = sound;
}