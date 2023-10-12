// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include "../pch/pch.h"
#include "Allocator.h"

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();

	void* Allocate(int32 size);
	void Release(void* ptr);
};

template<typename Type, typename... Args>
Type* xnew(Args&&... args)
{
	Type* memory = static_cast<Type*>(BaseAllocator::Alloc(sizeof(Type)));
	new(memory)Type(forward<Args>(args)...);
	return memory;
}

template<typename Type>
void xdelete(Type* obj)
{
	obj->~Type();
	BaseAllocator::Release(obj);
}

template<typename Type, typename... Args>
SharedPtr<Type> MakeShared(Args&&... args)
{
	return shared_ptr<Type>{ xnew<Type>(forward<Args>(args)...), xdelete<Type> };
}