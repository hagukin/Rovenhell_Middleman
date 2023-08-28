#include "../pch/pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"


Service::Service(ServiceType type, NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddress(address), _iocpCore(core), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{

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
}


ClientService::ClientService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		SharedPtr<Session> session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

ServerService::ServerService(NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	_listener = MakeShared<Listener>();
	if (_listener == nullptr)
		return false;

	SharedPtr<ServerService> service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
		return false;

	return true;
}

void ServerService::CloseService()
{
	Service::CloseService();
}
