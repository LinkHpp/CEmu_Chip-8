#include "chip.c"

int main()
{

    initializeEmu();
    loadGame();

    for (int i = 0;i <= 100;i++)
    {
        emulateCycle();
    }

    return 0;
}
