#pragma once

#include "targetver.h"

#ifdef _DEBUG
    #pragma comment(lib,"sfml-graphics-d.lib")
    #pragma comment(lib,"sfml-audio-d.lib")
    #pragma comment(lib,"sfml-system-d.lib")
    #pragma comment(lib,"sfml-window-d.lib")
#else
    #pragma comment(lib,"sfml-graphics.lib")
    #pragma comment(lib,"sfml-audio.lib")
    #pragma comment(lib,"sfml-system.lib")
    #pragma comment(lib,"sfml-window.lib")
#endif


#include <stdio.h>
#include <tchar.h>

#include <iostream>
#include <exception>
#include <memory>
#include <functional>
#include <algorithm>
#include <iterator>
#include <vector>
#include <array>

#include <SFML/System/Time.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
