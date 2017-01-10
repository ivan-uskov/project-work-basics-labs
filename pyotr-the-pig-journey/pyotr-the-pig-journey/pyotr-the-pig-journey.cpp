#include "stdafx.h"
#include "Application.h"

using namespace std;

int main()
{
    try
    {
//        thread mThread([] {
//            std::this_thread::sleep_for(chrono::seconds(20));
//        });

        Application app;
        app.initialize();
        app.run();

  //      mThread.join();
    }
    catch (std::exception& e)
    {
        cout << "\nEXCEPTION: " << e.what() << endl;
        return 1;
    }

    return 0;
}