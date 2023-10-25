// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"
#include "RecvBuffer.h"
#include "PacketHeader.h"

class Service;

class Session : public IocpObject
{
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64킬로바이트
	};

public:
	Session();
	virtual ~Session();

public:
	void Send(SharedPtr<SendBuffer> sendBuffer);
	bool Connect();
	void Disconnect(const WCHAR* cause);

	SharedPtr<Service> GetService() { return _service.lock(); }
	void SetService(SharedPtr<Service> service) { _service = service; }

public:
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET GetSocket() { return _socket; }
	bool IsConnected() { return _connected; }
	SharedPtr<Session> GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }
	const uint16 GetSessionId() { return _sessionId; }
	bool SetSessionId(uint16 id) { _sessionId = id; return true; }

private:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterRecv();
	void RegisterSend();

	void ProcessConnect();
	void ProcessDisconnect();
	void ProcessRecv(int32 numOfBytes);
	void ProcessSend(int32 numOfBytes);

	void HandleError(int32 errorCode);

protected:
	virtual void OnConnected() { }
	virtual int32 OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void OnSend(int32 len) { }
	virtual void OnDisconnected() { }

private:
	weak_ptr<Service> _service;
	SOCKET _socket = INVALID_SOCKET;
	NetAddress _netAddress = {};
	Atomic<bool> _connected = false;

private:
	USE_LOCK;

	RecvBuffer _recvBuffer;
	Queue<SharedPtr<SendBuffer>> _sendQueue;
	Atomic<bool> _sendRegistered = false;

private:
	ConnectEvent _connectEvent;
	DisconnectEvent _disconnectEvent;
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
	uint16 _sessionId = 0; // 0: 초기화 안됨
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	SharedPtr<PacketSession> GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32 OnRecv(BYTE* buffer, int32 len) sealed;
	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) abstract;
};