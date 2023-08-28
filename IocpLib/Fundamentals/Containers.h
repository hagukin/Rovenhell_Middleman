#pragma once

#include "../pch/pch.h"
#include "../Memory/Allocator.h"

#include <array>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

using String = basic_string<char, char_traits<char>, StlAllocator<char>>;

template<typename Type, uint32 Size>
using Array = array<Type, Size>;

template<typename Type>
using Vector = vector<Type, StlAllocator<Type>>;

template<typename Type>
using List = list<Type, StlAllocator<Type>>;

template<typename Key, typename Type, typename Pred = less<Key>>
using Map = map<Key, Type, Pred, StlAllocator<pair<const Key, Type>>>;

template<typename Key, typename Pred = less<Key>>
using Set = set<Key, Pred, StlAllocator<Key>>;

template<typename Type>
using Deque = deque<Type, StlAllocator<Type>>;

template<typename Type, typename Container = Deque<Type>>
using Queue = queue<Type, Container>;

template<typename Type, typename Container = Deque<Type>>
using Stack = stack<Type, Container>;

template<typename Type, typename Container = Vector<Type>, typename Pred = less<typename Container::value_type>>
using PriorityQueue = priority_queue<Type, Container, Pred>;

template<typename Key, typename Type, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashMap = unordered_map<Key, Type, Hasher, KeyEq, StlAllocator<pair<const Key, Type>>>;

template<typename Key, typename Hasher = hash<Key>, typename KeyEq = equal_to<Key>>
using HashSet = unordered_set<Key, Hasher, KeyEq, StlAllocator<Key>>;