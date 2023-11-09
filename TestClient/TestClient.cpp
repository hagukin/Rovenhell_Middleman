// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "NetCodes/SendBuffer.h"
#include "CToSSession.h"
#include "CToSSessionManager.h"

#define TEST_CLIENT_COUNT 50
#define DESIRED_CLIENT_PACKET_SEND_CYCLE 100.0ms // 클라 패킷 발송 주기

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);

	this_thread::sleep_for(1s);

	SharedPtr<ClientService> service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<CToSSession>,
		TEST_CLIENT_COUNT);

	ASSERT_CRASH(service->Init());
	ASSERT_CRASH(service->Start());

	int threadCount = 2; // 미들맨의 SToC 스레드의 개수보다 크게 설정하는 것이 권장됨 (그래야 TestClient Recv 보틀넥을 테스트 환경에서 최대한 배제할 수 있음) 
	// 그러나 아무리 높게 잡아도 기본적으로 실제 환경(호스트들이 각자 PC에서 클라이언트를 하나씩만 실행시키는 경우)보다 크게 느릴 수 밖에 없다는 점에 유의

	for (int32 i = 0; i < threadCount; i++)
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

	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(512);
	uint8* buffer = sendBuffer->Buffer();
	((PacketHeader*)buffer)->uniqueId = 0;
	((PacketHeader*)buffer)->packetOrder = 1;
	((PacketHeader*)buffer)->fragmentCount = 1;
	((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
	((PacketHeader*)buffer)->senderType = 0;
	// senderId는 뒤에서
	((PacketHeader*)buffer)->protocol = PacketProtocol::CLIENT_ALLOW_MULTIPLE_PER_TICK;
	((PacketHeader*)buffer)->type = PacketType::CHAT_GLOBAL;
	((PacketHeader*)buffer)->hostTime = 0.0f;
	::memcpy(&buffer[sizeof(PacketHeader)], sendData, sizeof(sendData));
	sendBuffer->Close((uint32)((PacketHeader*)buffer)->size);

	// 확률적으로 일어나기 힘든 worst case의 경우를 테스트 중 (모든 클라가 동시에 패킷을 전송)
	std::chrono::system_clock::time_point startTime;
	std::chrono::system_clock::time_point endTime;
	std::chrono::duration<long long, std::milli> broadcastTime;
	while (true)
	{
		startTime = std::chrono::system_clock::now();
		GSessionManager.Broadcast(sendBuffer); // 생성된 모든 클라이언트가 동일한 패킷을 한 번씩 발송, senderId 여기서 초기화
		GSessionManager.PrintAvgRecvCycleTime();

		endTime = std::chrono::system_clock::now();
		broadcastTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
		if (broadcastTime < DESIRED_CLIENT_PACKET_SEND_CYCLE)
		{
			this_thread::sleep_for(DESIRED_CLIENT_PACKET_SEND_CYCLE - broadcastTime);
		}
		cout << "클라 Send 주기: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count() << "ms" << "설정값: " << DESIRED_CLIENT_PACKET_SEND_CYCLE.count() << "ms" << '\n';
	}

	GThreadManager->Join();
}