// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include "Session.h"
#include "../Fundamentals/Enumerations.h"
#include <functional>

using SessionFactory = function<SharedPtr<Session>(void)>;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount);
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
	bool CanAddNewSession() { READ_LOCK; return _sessionCount < _maxSessionCount; }

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
private:
	uint64 _lastSessionId = 0; // 1씩 increment 해나가면서 세션에 아이디를 배정한다
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
	ServerService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount);
	virtual ~ServerService() {}

	SharedPtr<ServerService> shared_from_this() { return static_pointer_cast<ServerService>(Service::shared_from_this()); }

	virtual bool Init() override;
	virtual bool Start() override;
	virtual void CloseService() override;

private:
	SharedPtr<Listener> _listener = nullptr;
};


class LogicServerService : public ServerService
{
public:
	LogicServerService(NetAddress targetAddress, SharedPtr<IocpCore> core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~LogicServerService() {}

	SharedPtr<LogicServerService> shared_from_this() { return static_pointer_cast<LogicServerService>(Service::shared_from_this()); }

	virtual bool Init() override;
	virtual bool Start() override;
	bool IsConnected() { return _isConnected.load(); }
	void SetConnectedTo(bool isConnected) { _isConnected.store(isConnected); }

private:
	Atomic<bool> _isConnected = false;
};