#include "stdafx.h"
#include "BalisticMotion/MotionTimeIterator.h"

using namespace std;
using namespace BalisticMotion;

int main()
{
float maxHeight;

    cout << "Max height: ";
    if (!(cin >> maxHeight) || !(maxHeight > 0))
    {
        cout << endl << "Expected floating - point number greater then 0" << endl;
        return 1;
    }

    typedef Motion<float> FloatMotion;
    typedef MotionTimeIterator<FloatMotion> FloatMotionTimeIterator;
    auto motion = make_shared<FloatMotion>(maxHeight);

    for_each(FloatMotionTimeIterator(motion, 0.1f), FloatMotionTimeIterator(), [&motion] (float time) {
        cout << "t=" << time << ", h=" << motion->getHeightForTime(time) << endl;
    });

    return 0;
}

