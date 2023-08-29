#pragma once
#include "pch.h"

class SToLSession;

class SToLSessionManager
{
public:
	void Set(SharedPtr<SToLSession> session);
	void Remove(SharedPtr<SToLSession> session);
	void Send(SharedPtr<SendBuffer> sendBuffer);

private:
	USE_LOCK;
	SharedPtr<SToLSession> _session = nullptr;
};

extern SToLSessionManager GSToLSessionManager;
