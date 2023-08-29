#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "SToCSession.h"
#include "SToCSessionManager.h"
#include "SToLSession.h"
#include "SToLSessionManager.h"

int main()
{
	SharedPtr<LogicServerService> logicService = MakeShared<LogicServerService>(
		NetAddress(L"127.0.0.1", 8888),
		MakeShared<IocpCore>(),
		MakeShared<SToLSession>,
		1);

	ASSERT_CRASH(logicService->Init()); // TODO
	ASSERT_CRASH(logicService->Start()); // TODO

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					logicService->GetIocpCore()->Dispatch();
				}
			});
	}

	while (!logicService->IsConnected())
	{
	}

	SharedPtr<ServerService> service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<SToCSession>,
		3);

	ASSERT_CRASH(service->Init());
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 4; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}				
			});
	}	

	uint8 sendData[10] = {67, 67, 68, 68, 69, 69, 70, 70, 71, 71};

	while (true)
	{
		SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(500);

		uint8* buffer = sendBuffer->Buffer();
		((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
		((PacketHeader*)buffer)->senderType = 0;
		((PacketHeader*)buffer)->id = 1;
		::memcpy(&buffer[6], sendData, sizeof(sendData));
		sendBuffer->Close((sizeof(sendData) + sizeof(PacketHeader)));

		GSToCSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(16ms); // 60 frame
	}

	GThreadManager->Join();
}