#include "pch.h"
#include "SToCSession.h"
#include "SToCSessionManager.h"
#include "Fundamentals/Enumerations.h"

void SToCSession::OnConnected()
{
	GSToCSessionManager.Add(static_pointer_cast<SToCSession>(shared_from_this()));
	cout << "Client Session Id: "<< this->GetSessionId() <<" connected!" << endl;

	// 최초 패킷은 미들맨에서 생성한 해당 호스트의 세션 정보 전송
	PacketHeader header = {
		(uint16)sizeof(PacketHeader),
		HostTypeEnum::MIDDLEMAN_SERVER,
		this->GetSessionId(),
		PacketProtocol::MIDDLEMAN_EVENT,
		PacketId::SESSION_INFO,
		(uint32)0
	};
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	::memcpy(sendBuffer->Buffer(), &header, header.size);
	sendBuffer->Close(header.size);
	GSToCSessionManager.Send(this->GetSessionId(), sendBuffer);
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

	cout << "Client Session ID " << this->GetSessionId() << " -> Logic 중개 : ";
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