// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "NetCodes/Service.h"
#include "Fundamentals/ThreadManager.h"
#include "SToLSession.h"
#include "SToCSession.h"

class Middleman
{
public:
	Middleman();
	~Middleman();

	bool Init();
	bool Start();
	bool Close();

public:
	SharedPtr<LogicServerService> _logicService;
	SharedPtr<ServerService> _serverService;
};

