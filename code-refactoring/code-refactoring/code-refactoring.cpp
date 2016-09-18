#include "stdafx.h"

using namespace std;

int main(int argc, char * argv[])
{
    const float timeDiscretizationFrequency = 0.1f;
    const float g = 9.8f;

    float maxHeight;

    cout << "Max height: ";
    if (!(cin >> maxHeight))
    {
        cout << endl << "Expected floating - point number" << endl;
        return 1;
    }

    const float timeWhenMaxHeight = sqrt(maxHeight * 2 / g);
    cout << "T=" << timeWhenMaxHeight << endl;

    const float startSpeed = g * timeWhenMaxHeight;

    auto printState = [startSpeed, timeWhenMaxHeight, g](auto time)
    {
        auto currentHeight = [startSpeed, timeWhenMaxHeight, g](float time)
        {
            return startSpeed * time - 0.5 * g * time * time;
        };

        cout << "t=" << time << ", h=" << currentHeight(time) << endl;
    };

    bool isMaxHeightStatePrinted = false;
    for (float t = 0; t < timeWhenMaxHeight * 2; t += timeDiscretizationFrequency)
    {
        if (t > timeWhenMaxHeight && !isMaxHeightStatePrinted)
        {
            isMaxHeightStatePrinted = true;
            printState(timeWhenMaxHeight);
        }

        printState(t);
    }

    printState(timeWhenMaxHeight * 2);

    return 0;
}