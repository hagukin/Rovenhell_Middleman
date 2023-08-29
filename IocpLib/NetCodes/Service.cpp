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
			return false;
	}

	return true;
}

ServerService::ServerService(NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

bool ServerService::Init()
{
	// Listener를 만들고, 로직 서버 같이 서버 동작에 필수적인 다른 서버들과 필요한 커넥션을 맺는다
	// 이 때는 IOCP를 사용해 멀티스레드에서의 커넥션을 처리하는 대신 본 스레드에서 바로 커넥션을 처리한다
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
