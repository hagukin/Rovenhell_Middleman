#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include "Session.h"
#include <functional>

using SessionFactory = function<SharedPtr<Session>(void)>;

enum class ServiceType : uint8
{
	Server,
	Client
};

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool Init() abstract;
	virtual bool Start() abstract;
	bool IsValid() { return _sessionFactory != nullptr; }

	virtual void CloseService();
	void SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	SharedPtr<Session> CreateSession();
	void AddSession(SharedPtr<Session> session);
	void ReleaseSession(SharedPtr<Session> session);
	int32 GetCurrentSessionCount() { return _sessionCount; }
	int32 GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType GetServiceType() { return _type; }
	NetAddress GetNetAddress() { return _netAddress; }
	SharedPtr<IocpCore>& GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;
	ServiceType _type;
	NetAddress _netAddress = {};
	SharedPtr<IocpCore> _iocpCore;

	Set<SharedPtr<Session>> _sessions;
	int32 _sessionCount = 0;
	int32 _maxSessionCount = 0;
	SessionFactory _sessionFactory = nullptr;
};


class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	SharedPtr<ClientService> shared_from_this() { return static_pointer_cast<ClientService>(Service::shared_from_this()); }

	virtual bool Init() override;
	virtual bool Start() override;
};


class ServerService : public Service
{
public:
	ServerService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}

	SharedPtr<ServerService> shared_from_this() { return static_pointer_cast<ServerService>(Service::shared_from_this()); }

	virtual bool Init() override;
	virtual bool Start() override;
	virtual void CloseService() override;

private:
	SharedPtr<Listener> _listener = nullptr;
};