// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include "Session.h"
#include "../Fundamentals/Enumerations.h"
#include <functional>

// Service의 maxSessionCount가 가질 수 있는 값의 최대치; 초과 시 Crash
#define SYSTEM_MAXIMUM_SESSION_COUNT UINT16_MAX

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
	uint16 GetFreeSessionId();

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
	int _lastUsedSessionId = 0; // sessionId 생성 시 되도록 중복을 피하기 위해 점점 커지는 방향으로 하나씩 뽑아씀 (circular queue)
	bool _sessionIdPool[SYSTEM_MAXIMUM_SESSION_COUNT] = { false, }; // sessionId 할당할 때 사용; true는 누군가 이미 할당받은 상태인 id임을 의미함
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