#include "pch.h"
#include "CToSSession.h"
#include "CToSSessionManager.h"

CToSSession::~CToSSession()
{
	cout << "~CToSSession" << endl;
}

void CToSSession::OnConnected()
{
	GSessionManager.Add(static_pointer_cast<CToSSession>(shared_from_this()));
}

void CToSSession::OnDisconnected()
{
	GSessionManager.Remove(static_pointer_cast<CToSSession>(shared_from_this()));
}

int32 CToSSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketHeader header = *((PacketHeader*)buffer);
	uint8 recvBuffer[4096];
	::memcpy(recvBuffer, &buffer[sizeof(PacketHeader)], header.size - sizeof(PacketHeader));
	cout << "RecvFromMiddleman: ";
	for (int i = 0; i < header.size - sizeof(PacketHeader); ++i)
	{
		cout << (char)recvBuffer[i];
	}
	cout << endl;
	return len;
}

void CToSSession::OnSend(int32 len)
{
}
