#include "stdafx.h"
#include "Application.h"

using namespace std;

int main()
{
    try
    {
        Application().run();
    }
    catch (std::exception& e)
    {
        cout << "\nEXCEPTION: " << e.what() << endl;
        return 1;
    }

    return 0;
}