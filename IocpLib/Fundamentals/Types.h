// Copyright 2023 Haguk Kim
// Author: Haguk Kim

#pragma once
#include <mutex>
#include <atomic>

using BYTE = unsigned char;
using int8 = __int8;
using uint8 = unsigned __int8;
using int16 = __int16;
using uint16 = unsigned __int16;
using int32 = __int32;
using uint32 = unsigned __int32;
using int64 = __int64;
using uint64 = unsigned __int64;

template <typename T>
using SharedPtr = std::shared_ptr<T>;
template <typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using LockGuard = std::lock_guard<std::mutex>;