// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "pch.h"
#include "SToLSession.h"
#include "SToLSessionManager.h"
#include "Fundamentals/Enumerations.h"

void SToLSession::OnConnected()
{
	GSToLSessionManager.Set(static_pointer_cast<SToLSession>(shared_from_this()));
	cout << "Logic Server connected!" << endl;
	dynamic_cast<LogicServerService*>(GetService().get())->SetConnectedTo(true);

	// 최초 패킷은 미들맨에서 생성한 해당 호스트의 세션 정보 전송
	PacketHeader header = { 
		0,
		1,
		1,
		(uint16) sizeof(PacketHeader), 
		HostTypeEnum::MIDDLEMAN_SERVER,
		this->GetSessionId(),
		PacketProtocol::MIDDLEMAN_EVENT,
		PacketType::SESSION_INFO,
		0.0f
	};
	SharedPtr<SendBuffer> sendBuffer = GSendBufferManager->Open(header.size);
	::memcpy(sendBuffer->Buffer(), &header, header.size);
	sendBuffer->Close(header.size);
	GSToLSessionManager.Send(sendBuffer);
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

	// cout << "Logic -> Clients 브로드캐스팅" << endl;

	sendBuffer->Close(header.size);
	GSToCSessionManager.Broadcast(sendBuffer);
	return len;
}

void SToLSession::OnSend(int32 len)
{
}