#pragma once

#ifndef DAS_SKA_HASH
#define DAS_SKA_HASH    1
#endif

#if DAS_USE_EASTL
#include <EASTL/unordered_map.h>
#include <EASTL/unordered_set.h>
namespace das {
template <typename K, typename V>
using das_map = eastl::unordered_map<K,V>;
template <typename K>
using das_set = eastl::unordered_set<K>;
}
#else
#include <unordered_map>
#include <unordered_set>
namespace das {
template <typename K, typename V>
using das_map = std::unordered_map<K,V>;
template <typename K>
using das_set = std::unordered_set<K>;
}
#endif

#include <string>
#include <memory>
#include <vector>
#include <type_traits>
#include <initializer_list>
#include <functional>
#include <algorithm>

namespace das {using namespace std;}

#if DAS_SKA_HASH
#ifdef _MSC_VER
#pragma warning(disable:4503)    // decorated name length exceeded, name was truncated
#endif
#include <ska/flat_hash_map.hpp>
template <typename K, typename V>
using das_hash_map = ska::flat_hash_map<K,V>;
template <typename K>
using das_hash_set = ska::flat_hash_set<K>;
#endif

#define DAS_STD_HAS_BIND 1

#ifndef DAS_FUSION
  #define DAS_FUSION  1
#endif
