#include "pch.h"
#include "SToCSession.h"
#include "SToCSessionManager.h"

void SToCSession::OnConnected()
{
	GSToCSessionManager.Add(static_pointer_cast<SToCSession>(shared_from_this()));
	cout << "Client connected!" << endl;
}

void SToCSession::OnDisconnected()
{
	GSToCSessionManager.Remove(static_pointer_cast<SToCSession>(shared_from_this()));
}

int32 SToCSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// 클라이언트로부터 수신한 패킷을 그대로 로직서버에게 Send
	PacketHeader header = *((PacketHeader*)buffer);
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	uint8* sendData = sendBuffer->Buffer();
	::memcpy(sendData, &buffer[0], header.size);

	cout << "Client -> Logic 중개: ";
	for (int i = sizeof(PacketHeader); i < header.size; ++i)
	{
		cout << (char)sendData[i];
	}
	cout << endl;

	sendBuffer->Close(header.size);
	GSToLSessionManager.Send(sendBuffer);
	return len;
}

void SToCSession::OnSend(int32 len)
{
}