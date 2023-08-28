#include "pch.h"
#include "SToCSession.h"
#include "SToCSessionManager.h"

void SToCSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<SToCSession>(shared_from_this()));
	cout << "Client connected!" << endl;
}

void SToCSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<SToCSession>(shared_from_this()));
}

int32 SToCSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketHeader header = *((PacketHeader*)buffer);
	uint8 recvBuffer[4096];
	::memcpy(recvBuffer, &buffer[6], header.size - sizeof(PacketHeader));
	cout << "RecvFromClient: ";
	for (int i = 0; i < header.size - sizeof(PacketHeader); ++i)
	{
		cout << (char)recvBuffer[i];
	}
	cout << endl;
	return len;
}

void SToCSession::OnSend(int32 len)
{
	cout << "Send 완료, 길이: "<< len << endl;
}