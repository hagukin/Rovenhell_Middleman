// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#include "../pch/pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"


/* Service */
Service::Service(ServiceType type, NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddress(address), _iocpCore(core), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{
	ASSERT_CRASH(maxSessionCount <= SYSTEM_MAXIMUM_SESSION_COUNT);
}

Service::~Service()
{
}

void Service::CloseService()
{
}

SharedPtr<Session> Service::CreateSession()
{
	SharedPtr<Session> session = _sessionFactory();
	session->SetService(shared_from_this());
	session->SetSessionId(GetFreeSessionId()); // id는 1 이상

	if (_iocpCore->Register(session) == false)
		return nullptr;

	return session;
}

void Service::AddSession(SharedPtr<Session> session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);
}

void Service::ReleaseSession(SharedPtr<Session> session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
	_sessionIdPool[session->GetSessionId()] = false;
}

uint16 Service::GetFreeSessionId()
{
	WRITE_LOCK;
	for (uint16 i = 1; i < SYSTEM_MAXIMUM_SESSION_COUNT; ++i)
	{
		uint16 nextIdx = (uint16) ((_lastUsedSessionId + i) % SYSTEM_MAXIMUM_SESSION_COUNT);
		if (!_sessionIdPool[nextIdx])
		{
			_sessionIdPool[nextIdx] = true;
			_lastUsedSessionId = nextIdx;
			return nextIdx;
		}
	}
	return 0; // free id not found
}

/* ClientService*/
ClientService::ClientService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

bool ClientService::Init()
{
	if (!IsValid()) return false;
	return true;
}

bool ClientService::Start()
{
	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		SharedPtr<Session> session = CreateSession();
		if (session->Connect() == false)
		{
			cout << "클라이언트 세션이 Connect에 실패했습니다." << '\n';
			return false;
		}
	}

	return true;
}


/* ServerService */
ServerService::ServerService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Middleman, targetAddress, core, factory, maxSessionCount)
{
}

bool ServerService::Init()
{
	if (!IsValid())
		return false;
	_listener = MakeShared<Listener>();
	if (_listener == nullptr)
		return false;
	if (_listener->Init(shared_from_this()) == false)
		return false;
	return true;
}

bool ServerService::Start()
{
	if (_listener->StartAccept() == false)
		return false;
	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}


/* LogicServerService */
LogicServerService::LogicServerService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: ServerService(targetAddress, core, factory, maxSessionCount)
{
	_type = ServiceType::LogicServer;
}

bool LogicServerService::Init()
{
	return ServerService::Init();
}

bool LogicServerService::Start()
{
	return ServerService::Start();
}