// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "pch.h"
#include "CToSSessionManager.h"
#include "CToSSession.h"

CToSSessionManager GSessionManager;

void CToSSessionManager::Add(SharedPtr<CToSSession> session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void CToSSessionManager::Remove(SharedPtr<CToSSession> session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void CToSSessionManager::Broadcast(SharedPtr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (SharedPtr<CToSSession> session : _sessions)
	{
		uint8* buffer = sendBuffer->Buffer();
		((PacketHeader*)buffer)->senderId = session->GetSessionId();
		session->Send(sendBuffer);
	}
}