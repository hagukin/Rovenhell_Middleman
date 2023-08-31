#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "NetCodes/SendBuffer.h"
#include "CToSSession.h"
#include "CToSSessionManager.h"

#define TEST_CLIENT_COUNT 5

int main()
{
	this_thread::sleep_for(1s);

	SharedPtr<ClientService> service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<CToSSession>,
		TEST_CLIENT_COUNT);

	ASSERT_CRASH(service->Init());
	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	unsigned char sendData[6] = { 67, 76, 73, 69, 78, 84 };

	while (true)
	{
		SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(1000);

		uint8* buffer = sendBuffer->Buffer();
		((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
		((PacketHeader*)buffer)->senderType = 0;
		((PacketHeader*)buffer)->id = 1;
		::memcpy(&buffer[6], sendData, sizeof(sendData));
		sendBuffer->Close((uint32)((PacketHeader*)buffer)->size);
		GSessionManager.Broadcast(sendBuffer);

		this_thread::sleep_for(16ms); // 60 frame
	}

	GThreadManager->Join();
}