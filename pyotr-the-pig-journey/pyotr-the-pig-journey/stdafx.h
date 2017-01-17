#pragma once

#include "targetver.h"

#ifdef _DEBUG
    #pragma comment(lib,"sfml-graphics-d.lib")
    #pragma comment(lib,"sfml-audio-d.lib")
    #pragma comment(lib,"sfml-system-d.lib")
    #pragma comment(lib,"sfml-window-d.lib")
    #pragma comment(lib,"sfml-network-d.lib")
#else
    #pragma comment(lib,"sfml-graphics.lib")
    #pragma comment(lib,"sfml-audio.lib")
    #pragma comment(lib,"sfml-system.lib")
    #pragma comment(lib,"sfml-window.lib")
    #pragma comment(lib,"sfml-network.lib")
#endif

#define WIN32_LEAN_AND_MEAN

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <future>
#include <atomic>
#include <chrono>