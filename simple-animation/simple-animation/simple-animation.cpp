#include "stdafx.h"
#include "Application.h"

int main()
{
    try
    {
        Application().run();
    }
    catch (std::exception& e)
    {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

