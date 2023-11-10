#include "chip.c"

int main()
{

    initializeEmu();
    loadGame();

    for (int i = 0; i <= 20; i++)
    {
        emulateCycle();
    }

    return 0;
}
