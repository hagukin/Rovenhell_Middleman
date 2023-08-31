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

private:
	const wstring LOGIC_SERVER_ADDR = L"127.0.0.1";
	const wstring CLIENT_ADDR = L"127.0.0.1";
	const int32 LOGIC_SERVER_PORT = 8888;
	const int32 CLIENT_PORT = 7777;
	const int32 MAX_CLIENT_COUNT = 32;

public:
	SharedPtr<LogicServerService> _logicService;
	SharedPtr<ServerService> _serverService;
};

