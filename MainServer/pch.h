#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "Debug\\IocpLib.lib")
#else
#pragma comment(lib, "Release\\IocpLib.lib")
#endif

#include "Fundamentals/CoreHeaders.h"