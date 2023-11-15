#include <stdio.h>
#include <stdlib.h>


/*This is the specs of the CPU*/

unsigned short opcode;

// The chip-8 has 4k so make this 4096
unsigned char memory[4096];

// This is the registers, there's V which has 15 for general use and the 16th is for the carry flag
unsigned char V[16];

// This is the index register and the program counter which can have a value from 0x000 to 0xFFF
unsigned short I;
unsigned short pc;

// This is the resolution of the screen, which is 2048px. Screen is in Black & White
unsigned char gfx[64 * 32];

// Two time registers that counts at 60Hz, when set above 0 will count down to zero
unsigned char delay_timer;
unsigned char sound_timer;

// This is the stack and stack pointer
unsigned short stack[16];
unsigned short sp;

// Chip 8 has a HEX based keypad (0x0-0xF), you can use an array to store the current state of the key
unsigned char key[16];

void initializeEmu()
{
    // Initialize registers and memory once

    pc = 0x200; // Program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;      // Reset index register
    sp = 0;     // Reset stack pointer

    printf("All initilialized!\n");

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset

    /*for (int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];
    */

    // Reset timers
}

void loadGame()
{

    FILE *file = fopen("Games/1-chip8-logo.ch8", "rb");
    if (file == NULL)
    {
        printf("error: Couldn't open %s\n", "pong");
    }

    // Get the file size and read it into a memory buffer
    fseek(file, 0L, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // This is the buffer
    unsigned char *buffer = (unsigned char *)malloc(fsize);

    fread(buffer, fsize, 1, file);
    fclose(file);

    // We load the buffer in the memory and start doing it at 0x200 in memory
    for (int i = 0; i < fsize; ++i)
    {
        memory[i + 512] = buffer[i];
    }

    printf("Loaded Game!\n");

    printf("First opcode of the game is 0x%x%x\n", memory[0 + 512], memory[1 + 512]);
}

void emulateCycle()
{

    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Decode opcode
    switch (opcode & 0xF000)
    {
        // Some opcodes //

    case 0x0000:
        switch (opcode & 0x000F)
        {
        case 0x0000:
            printf("Clear Screen\n");
            pc += 2;
            break;

        case 0x000E:
            printf("Returning from subRoutine\n");
            break;
        }
        break;

    case 0x1000:
        printf("Jump to memory: 0x%X\n", opcode & 0x0FFF);
        pc = opcode & 0x0FFF;
        printf("%X\n", pc);
        break;

    case 0x2000: // 0x2NNN: Makes a jump to the subroutine
        printf("Subroutine jump to : 0x%X\n", opcode & 0x0FFF);
        stack[sp] = pc;
        ++sp;
        pc = opcode & 0x0FFF;
        break;

    case 0x6000:
        V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
        printf("Valor V%X = %X\n", (opcode & 0x0F00), (opcode & 0x00FF));
        pc += 2;
        break;

    case 0x7000:
        V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
        printf("Valor V%X = %X\n", (opcode & 0x0F00), (opcode & 0x00FF));
        pc += 2;
        break;

    case 0xA000: // ANNN: Sets I to the address NNN
                 // Execute opcode
        printf("Know opcode: 0x%X ", opcode);

        I = opcode & 0x0FFF;
        pc += 2;
        printf("Index register = %x\n", I);
        break;

    default:
        printf("Unknown opcode: 0x%X\n", opcode);
        pc += 2;
    }

    // Update timers
    if (delay_timer > 0)
        --delay_timer;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}
