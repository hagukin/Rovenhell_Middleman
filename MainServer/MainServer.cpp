// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "Middleman.h"
#include "SToCSessionManager.h"

int main()
{
	UniquePtr<Middleman> middleman = std::make_unique<Middleman>();
	middleman->Init();
	middleman->Start();
	middleman->Close();
}