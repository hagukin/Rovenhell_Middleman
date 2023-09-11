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

	uint32 testTick = 0;
	while (true)
	{
		SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(1000);

		uint8* buffer = sendBuffer->Buffer();
		((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
		((PacketHeader*)buffer)->senderType = 0;
		// senderId는 뒤에서
		((PacketHeader*)buffer)->protocol = PacketProtocol::CLIENT_ONCE_PER_TICK;
		((PacketHeader*)buffer)->id = PacketId::CHAT_GLOBAL;
		((PacketHeader*)buffer)->tick = ++testTick;
		((PacketHeader*)buffer)->deltaTime = 0.016; // 60 frame
		::memcpy(&buffer[sizeof(PacketHeader)], sendData, sizeof(sendData));
		sendBuffer->Close((uint32)((PacketHeader*)buffer)->size);
		GSessionManager.Broadcast(sendBuffer); // 생성된 모든 클라이언트가 동일한 패킷을 한 번씩 발송, senderId 여기서 초기화

		this_thread::sleep_for(16ms); // 60 frame
	}

	GThreadManager->Join();
}