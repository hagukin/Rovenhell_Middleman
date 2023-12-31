// Copyright 2023 Haguk Kim
// Author: Haguk Kim

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
		0,
		1,
		1,
		(uint16)sizeof(PacketHeader),
		HostTypeEnum::MIDDLEMAN_SERVER,
		this->GetSessionId(),
		PacketProtocol::MIDDLEMAN_EVENT,
		PacketType::SESSION_INFO,
		0.0f
	};
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	::memcpy(sendBuffer->Buffer(), &header, header.size);
	sendBuffer->Close(header.size);
	GSToCSessionManager.Send(this->GetSessionId(), sendBuffer);

	// 로직서버에게 새 클라이언트 접속을 알림
	PacketHeader connectedHeader = {
		0,
		1,
		1,
		(uint16)sizeof(PacketHeader),
		HostTypeEnum::MIDDLEMAN_SERVER,
		this->GetSessionId(),
		PacketProtocol::MIDDLEMAN_EVENT,
		PacketType::SESSION_CONNECTED,
		0.0f
	};
	SharedPtr<SendBuffer> connectedBuffer = GSendBufferManager->Open(connectedHeader.size);
	::memcpy(connectedBuffer->Buffer(), &connectedHeader, connectedHeader.size);
	connectedBuffer->Close(connectedHeader.size);
	GSToLSessionManager.Send(connectedBuffer);
}

void SToCSession::OnDisconnected()
{
	// 로직서버에게 클라이언트 연결 해제를 알림
	PacketHeader disconnectedHeader = {
		0,
		1,
		1,
		(uint16)sizeof(PacketHeader),
		HostTypeEnum::MIDDLEMAN_SERVER,
		this->GetSessionId(),
		PacketProtocol::MIDDLEMAN_EVENT,
		PacketType::SESSION_DISCONNECTED,
		0.0f
	};
	SharedPtr<SendBuffer> disconnectedBuffer = GSendBufferManager->Open(disconnectedHeader.size);
	::memcpy(disconnectedBuffer->Buffer(), &disconnectedHeader, disconnectedHeader.size);
	disconnectedBuffer->Close(disconnectedHeader.size);
	GSToLSessionManager.Send(disconnectedBuffer);

	// 세션 삭제
	GSToCSessionManager.Remove(static_pointer_cast<SToCSession>(shared_from_this()));
}

int32 SToCSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	// 클라이언트로부터 수신한 패킷을 그대로 로직서버에게 Send
	PacketHeader header = *((PacketHeader*)buffer);
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	uint8* sendData = sendBuffer->Buffer();
	::memcpy(sendData, &buffer[0], header.size);

	// cout << "Client Session ID " << this->GetSessionId() << " -> Logic 중개" << endl;

	sendBuffer->Close(header.size);
	GSToLSessionManager.Send(sendBuffer);
	return len;
}

void SToCSession::OnSend(int32 len)
{
}