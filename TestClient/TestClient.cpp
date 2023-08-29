#include "pch.h"
#include "Fundamentals/ThreadManager.h"
#include "NetCodes/Service.h"
#include "NetCodes/Session.h"
#include "CToSSession.h"
#include "CToSSessionManager.h"

int main()
{
	this_thread::sleep_for(1s);

	SharedPtr<ClientService> service = MakeShared<ClientService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<CToSSession>,
		5);

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

	//char sendData[1000] = "Hello from Client";

	//while (true)
	//{
	//	SendBufferRef sendBuffer = GSendBufferManager->Open(4096);

	//	BYTE* buffer = sendBuffer->Buffer();
	//	((PacketHeader*)buffer)->size = (sizeof(sendData) + sizeof(PacketHeader));
	//	((PacketHeader*)buffer)->id = 1; // 1 : Hello Msg
	//	::memcpy(&buffer[4], sendData, sizeof(sendData));
	//	sendBuffer->Close((sizeof(sendData) + sizeof(PacketHeader)));

	//	GSessionManager.Broadcast(sendBuffer);

	//	this_thread::sleep_for(250ms);
	//}

	GThreadManager->Join();
}