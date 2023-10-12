// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once

class CToSSession;

class CToSSessionManager
{
public:
	void Add(SharedPtr<CToSSession> session);
	void Remove(SharedPtr<CToSSession> session);
	void Broadcast(SharedPtr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	Set<SharedPtr<CToSSession>> _sessions;
};

extern CToSSessionManager GSessionManager;
