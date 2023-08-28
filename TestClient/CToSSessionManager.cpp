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
		session->Send(sendBuffer);
	}
}