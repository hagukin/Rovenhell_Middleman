#include "../pch/pch.h"
#include "MemoryManager.h"
#include "MemoryHeader.h"

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
}

void* MemoryManager::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader);

#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	header = reinterpret_cast<MemoryHeader*>(BaseAllocator::Alloc(allocSize));
#endif	

	return MemoryHeader::AttachHeader(header, allocSize);
}

void MemoryManager::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	BaseAllocator::Release(header);
#endif	
}
