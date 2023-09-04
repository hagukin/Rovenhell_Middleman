#include "pch.h"
#include "Middleman.h"

Middleman::Middleman()
{
}

Middleman::~Middleman()
{
}

bool Middleman::Init()
{
	_logicService = MakeShared<LogicServerService>(
		NetAddress(LOGIC_SERVER_ADDR, LOGIC_SERVER_PORT),
		MakeShared<IocpCore>(),
		MakeShared<SToLSession>,
		1);
	ASSERT_CRASH(_logicService->Init());
	for (int32 i = 0; i < CLIENT_THREAD_COUNT; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					_logicService->GetIocpCore()->Dispatch();
				}
			});
	}

	_serverService = MakeShared<ServerService>(
		NetAddress(CLIENT_ADDR, CLIENT_PORT),
		MakeShared<IocpCore>(),
		MakeShared<SToCSession>,
		MAX_CLIENT_COUNT);
	ASSERT_CRASH(_serverService->Init());
	for (int32 i = 0; i < LOGIC_SERVER_THREAD_COUNT; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					_serverService->GetIocpCore()->Dispatch();
				}
			});
	}
	return true;
}

bool Middleman::Start()
{
	ASSERT_CRASH(_logicService->Start());
	while (!_logicService->IsConnected())
	{
	}
	ASSERT_CRASH(_serverService->Start());
	return true;
}

bool Middleman::Close()
{
	GThreadManager->Join();
	return true;
}
