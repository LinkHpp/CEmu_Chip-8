#include "chip8.h"

int main()
{

    initializeEmu();
    loadGame();

    for (;;)
    {
        emulateCycle();
    }

    return 0;
}
