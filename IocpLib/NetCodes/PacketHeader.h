// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "../pch/pch.h"

// 상세 주석은 로직 서버 소스코드 참고

struct PacketHeader
{
	uint8 uniqueId;
	uint8 packetOrder;
	uint8 fragmentCount;
	uint16 size;
	uint16 senderType;
	uint16 senderId;
	uint8 protocol;
	uint16 type;
	float hostTime;
};

enum PacketType : uint16
{
	DEFAULT,
	GAME_INPUT,
	CHAT_GLOBAL,
	GAME_STATE,
	SESSION_INFO,
	SESSION_CONNECTED,
	SESSION_DISCONNECTED,
};

enum PacketProtocol : uint8
{
	NO_PROTOCOL,
	CLIENT_ONCE_PER_TICK,
	CLIENT_ALLOW_MULTIPLE_PER_TICK,
	LOGIC_EVENT,
	MIDDLEMAN_EVENT,
};