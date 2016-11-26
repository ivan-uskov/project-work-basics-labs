#include "stdafx.h"
#include "../game-lib/Application.h"

int main()
{
    try
    {
        Application().run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}

