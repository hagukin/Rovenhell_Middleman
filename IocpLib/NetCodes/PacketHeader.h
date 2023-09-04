#pragma once
#include "../pch/pch.h"

struct PacketHeader
{
	uint16 size;
	uint16 senderType;
	uint16 id;
	uint32 tick;
};

enum PacketId : uint8
{
	DEFAULT,
	CHAT_GLOBAL,
	CLIENT_EVENT
};