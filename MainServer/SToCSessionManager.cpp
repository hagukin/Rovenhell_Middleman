// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "pch.h"
#include "SToCSessionManager.h"
#include "SToCSession.h"

SToCSessionManager GSToCSessionManager;

void SToCSessionManager::Add(SharedPtr<SToCSession> session)
{
	WRITE_LOCK;
	_sessions.insert(move(make_pair(session->GetSessionId(), session)));
}

void SToCSessionManager::Remove(SharedPtr<SToCSession> session)
{
	WRITE_LOCK;
	_sessions.erase(session->GetSessionId());
}

void SToCSessionManager::Broadcast(SharedPtr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (const auto& session : _sessions)
	{
		session.second->Send(sendBuffer);
	}
}

void SToCSessionManager::Send(uint64 sessionId, SharedPtr<SendBuffer> sendBuffer)
{
	const auto& session = _sessions.find(sessionId);
	if (session == _sessions.end())
	{
		cout << sessionId << " id를 가진 세션을 찾지 못했습니다. Send에 실패했습니다." << endl;
		return;
	}
	session->second->Send(sendBuffer);
}
