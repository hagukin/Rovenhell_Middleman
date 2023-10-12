// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once

#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

#include "Types.h"
#include "Macros.h"
#include "TLS.h"
#include "Containers.h"
#include "GlobalManager.h"
#include "Lock.h"
#include "../Memory/ObjectPool.h"
#include "../Memory/MemoryManager.h"
#include "../NetCodes/SendBuffer.h"