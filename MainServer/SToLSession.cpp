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
	PacketHeader header = *((PacketHeader*)buffer);
	uint8 recvBuffer[4096];
	::memcpy(recvBuffer, &buffer[6], header.size - sizeof(PacketHeader));
	cout << "RecvFromLogicServer: ";
	for (int i = 0; i < header.size - sizeof(PacketHeader); ++i)
	{
		cout << (char)recvBuffer[i];
	}
	cout << endl;
	return len;
}

void SToLSession::OnSend(int32 len)
{
	cout << "Send 완료, 길이: " << len << endl;
}