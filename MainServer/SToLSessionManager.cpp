#include "pch.h"
#include "SToLSessionManager.h"
#include "SToLSession.h"

SToLSessionManager GSToLSessionManager;

void SToLSessionManager::Set(SharedPtr<SToLSession> session)
{
	WRITE_LOCK;
	_session = session;
}

void SToLSessionManager::Remove(SharedPtr<SToLSession> session)
{
	WRITE_LOCK;
	_session = nullptr;
}

void SToLSessionManager::Send(SharedPtr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	_session->Send(sendBuffer);
}