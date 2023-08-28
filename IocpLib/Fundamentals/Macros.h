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