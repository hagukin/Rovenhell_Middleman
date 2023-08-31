#include "pch.h"
#include "SToLSession.h"
#include "SToLSessionManager.h"

void SToLSession::OnConnected()
{
	GSToLSessionManager.Set(static_pointer_cast<SToLSession>(shared_from_this()));
	cout << "Logic Server connected!" << endl;
	dynamic_cast<LogicServerService*>(GetService().get())->SetConnectedTo(true);
}

void SToLSession::OnDisconnected()
{
	GSToLSessionManager.Remove(static_pointer_cast<SToLSession>(shared_from_this()));
}

int32 SToLSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// 로직서버로부터 수신한 패킷을 그대로 클라이언트들에게 Broadcasting
	PacketHeader header = *((PacketHeader*)buffer);
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	uint8* sendData = sendBuffer->Buffer();
	::memcpy(sendData, &buffer[0], header.size);

	cout << "Logic -> Client 중개: ";
	for (int i = sizeof(PacketHeader); i < header.size; ++i)
	{
		cout << (char)sendData[i];
	}
	cout << endl;

	sendBuffer->Close(header.size);
	GSToCSessionManager.Broadcast(sendBuffer);
	return len;
}

void SToLSession::OnSend(int32 len)
{
}