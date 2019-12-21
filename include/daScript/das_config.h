#pragma once

#ifndef DAS_SKA_HASH
#define DAS_SKA_HASH    1
#endif

#if DAS_SKA_HASH
#ifdef _MSC_VER
#pragma warning(disable:4503)    // decorated name length exceeded, name was truncated
#endif
#include <flat_hash_map\flat_hash_map.hpp>
template <typename K, typename V>
using das_map = ska::flat_hash_map<K,V>;
template <typename K>
using das_set = ska::flat_hash_set<K>;
#endif

#if DAS_USE_EASTL
    #include <EASTL/unordered_map.h>
    #include <EASTL/unordered_set.h>
#else
    #include <unordered_map>
    #include <unordered_set>
#endif

#include <string>
#include <memory>
#include <vector>
#include <type_traits>
#include <initializer_list>
#include <functional>
#include <algorithm>
namespace das {using namespace std;}
#define DAS_STD_HAS_BIND 1

#ifndef DAS_FUSION
  #define DAS_FUSION  1
#endif
