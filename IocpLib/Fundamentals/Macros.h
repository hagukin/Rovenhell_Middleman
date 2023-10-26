// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once

#define OUT
#define _STOMP

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(condition)			\
{										\
	if (!(condition))					\
	{									\
		CRASH("ASSERT_CRASH");			\
		__analysis_assume(condition);	\
	}									\
}

#define USE_MULTIPLE_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MULTIPLE_LOCKS(1)
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)


#define LOGIC_SERVER_ADDR L"127.0.0.1"
#define CLIENT_ADDR L"127.0.0.1"
#define LOGIC_SERVER_PORT 8888
#define CLIENT_PORT 7777
#define MAX_CLIENT_COUNT 1024 // 시스템 상 허용가능한 최대 수는 65535
#define STOL_THREAD_COUNT 4
#define STOC_THREAD_COUNT 4