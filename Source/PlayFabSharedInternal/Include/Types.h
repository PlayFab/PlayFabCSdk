#pragma once

#include <list>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <chrono>
#include "Memory.h"
#include "Nlohmann.h"

namespace PlayFab
{

template<class C, class TRAITS = std::char_traits<C>>
using BasicString = std::basic_string<C, TRAITS, Allocator<C>>;
using String = BasicString<char>;
using WString = BasicString<wchar_t>;

template<class C, class TRAITS = std::char_traits<C>>
using BasicStringsteam = std::basic_stringstream<C, TRAITS, Allocator<C>>;
using Stringstream = BasicStringsteam<char>;
using WStringstream = BasicStringsteam<wchar_t>;

template<class C, class TRAITS = std::char_traits<C>>
using BasicStringView = std::basic_string_view<C, TRAITS>;
using StringRefType = BasicStringView<char>;

template<typename T>
struct Hash : public std::hash<T>{};

template<typename T>
struct Hash<BasicString<T>> : public std::hash<std::basic_string_view<T>>{};

template<class T>
using Vector = std::vector<T, Allocator<T>>;

template<class K, class V, class LESS = std::less<K>>
using Map = std::map<K, V, LESS, Allocator<std::pair<K const, V>>>;

template<class K, class LESS = std::less<K>>
using Set = std::set<K, LESS, Allocator<K>>;

template<class K, class V, class HASH = Hash<K>, class EQUAL = std::equal_to<K>>
using UnorderedMap = std::unordered_map<K, V, HASH, EQUAL, Allocator<std::pair<K const, V>>>;

template<class K, class HASH = Hash<K>, class EQUAL = std::equal_to<K>>
using UnorderedSet = std::unordered_set<K, HASH, EQUAL, Allocator<K>>;

template<class T>
using Deque = std::deque<T, Allocator<T>>;

template<class T>
using Queue = std::queue<T, Deque<T>>;

template<class T>
using List = std::list<T, Allocator<T>>;

using JsonDocument = nlohmann::json;
using JsonValue = nlohmann::json;

using Clock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<SystemClock>;

}
