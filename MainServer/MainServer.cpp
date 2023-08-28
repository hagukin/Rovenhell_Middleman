#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "SToCSession.h"
#include "SToCSessionManager.h"

int main()
{
	SharedPtr<ServerService> service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<SToCSession>,
		10);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
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

		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(16ms); // 60 frame
	}

	GThreadManager->Join();
}