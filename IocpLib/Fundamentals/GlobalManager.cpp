// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "../pch/pch.h"
#include "GlobalManager.h"
#include "ThreadManager.h"
#include "../Memory/MemoryManager.h"
#include "../NetCodes/SocketModifier.h"
#include "../NetCodes/SendBuffer.h"

ThreadManager* GThreadManager = nullptr;
MemoryManager* GMemoryManager = nullptr;
SendBufferManager* GSendBufferManager = nullptr;

class GlobalManager
{
public:
	GlobalManager()
	{
		GThreadManager = new ThreadManager();
		GMemoryManager = new MemoryManager();
		GSendBufferManager = new SendBufferManager();
		SocketModifier::Init();
	}
	~GlobalManager()
	{
		delete GThreadManager;
		delete GMemoryManager;
		delete GSendBufferManager;
		SocketModifier::Clear();
	}
} GGlobalManager;