#include "pch.h"
#include "SToCSessionManager.h"
#include "SToCSession.h"

SToCSessionManager GSessionManager;

void SToCSessionManager::Add(SharedPtr<SToCSession> session)
{
	WRITE_LOCK;
	_sessions.insert(session);
}

void SToCSessionManager::Remove(SharedPtr<SToCSession> session)
{
	WRITE_LOCK;
	_sessions.erase(session);
}

void SToCSessionManager::Broadcast(SharedPtr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (SharedPtr<SToCSession> session : _sessions)
	{
		session->Send(sendBuffer);
	}
}