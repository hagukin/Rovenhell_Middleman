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

void CToSSessionManager::PrintAvgRecvCycleTime()
{
	READ_LOCK;
	if (_sessions.size() == 0) return;

	long long cycleTimeSum = 0;
	for (SharedPtr<CToSSession> session : _sessions)
	{
		cycleTimeSum += session->GetRecvCycleTime();
	}
	double cycleAvg = round(cycleTimeSum / _sessions.size());

	cout << "클라 평균 Recv 주기: " << (int) cycleAvg << "ms" << '\n';
	// 주의: 이 값은 fragment 하나를 받는 시간이기 때문에 실질적인 단일 패킷 수신시간을 구하려면 이 값에 fragment 개수를 곱해야 한다
}
