#pragma once
#include "pch.h"

class SToCSession;

class SToCSessionManager
{
public:
	void Add(SharedPtr<SToCSession> session);
	void Remove(SharedPtr<SToCSession> session);
	void Broadcast(SharedPtr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	Set<SharedPtr<SToCSession>> _sessions;
};

extern SToCSessionManager GSessionManager;
