// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "pch.h"

class SToCSession;

class SToCSessionManager
{
public:
	void Add(SharedPtr<SToCSession> session);
	void Remove(SharedPtr<SToCSession> session);
	void Broadcast(SharedPtr<SendBuffer> sendBuffer);
	void Send(uint16 sessionId, SharedPtr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	HashMap<uint16, SharedPtr<SToCSession>> _sessions;
};

extern SToCSessionManager GSToCSessionManager;
