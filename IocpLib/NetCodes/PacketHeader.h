#pragma once
#include "../pch/pch.h"

struct PacketHeader
{
	uint16 size;
	uint16 senderType;
	uint16 id;
};

enum PacketId
{
	DEFAULT,
	CHAT_GLOBAL,
};