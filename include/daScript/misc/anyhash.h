#pragma once

#include "daScript/misc/wyhash.h"

/*
    this is where we configure low level hash implementation
*/

#define HASH_EMPTY64     0
#define HASH_KILLED64    1
#define DAS_WYHASH_SEED  UINT64_C(0x1234567890abcdef)

#ifndef DAS_SAFE_HASH
#define DAS_SAFE_HASH    0
#endif

namespace das {

    static __forceinline uint64_t hash_block64 ( const uint8_t * block, size_t size ) {
        auto h = wyhash(block, size, DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? UINT64_C(1099511628211) : h;
    }

    static NO_ASAN_INLINE uint64_t hash_blockz64 ( const uint8_t * block ) {
        auto FNV_offset_basis = UINT64_C(14695981039346656037);
        auto FNV_prime = UINT64_C(1099511628211);
        if ( !block ) return FNV_offset_basis;
        auto h = FNV_offset_basis;
#if DAS_SAFE_HASH
        while ( *block ) {
            h ^= *block++;
            h *= FNV_prime;
        }
#else
        while ( true ) {
            uint64_t v = *(uint16_t *)block;
            if ( (v & 0xff)==0 ) break;
            h ^= v;
            h *= FNV_prime;
            if ( v < 0x100 ) break;
            block += 2;
        }
#endif
        return h <= HASH_KILLED64 ? UINT64_C(1099511628211) : h;
    }

    static __forceinline uint64_t hash_uint32 ( uint32_t value ) {  // this is simplified, and not the same as wyhash(&value,4)
        auto h = _wymix(uint64_t(value), DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? UINT64_C(1099511628211) : h;
    }

    static __forceinline uint64_t hash_uint64 ( uint64_t value ) { // this is simplified, and not the same as wyhash(&value,4)
        auto h = _wymix(value, DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? UINT64_C(1099511628211) : h;
    }

    class HashBuilder {
        uint64_t seed = DAS_WYHASH_SEED;
    public:
        uint64_t getHash() {
            return seed <= HASH_KILLED64 ? UINT64_C(1099511628211) : seed;
        }
        __forceinline void updateString ( const char * str ) {
            if (!str) str = "";
            seed = wyhash((const uint8_t *)str, strlen(str), seed);
        }
        __forceinline void updateString ( const char * str, size_t len) {
            if (!str) str = "";
            seed = wyhash((const uint8_t *)str, len, seed);
        }
        __forceinline void updateString ( const string & str ) {
            seed = wyhash((const uint8_t *)str.c_str(), str.size(), seed);
        }
        template <typename TT>
        __forceinline void update ( const TT & data ) {
            seed = wyhash((const uint8_t *)&data, sizeof(data), seed);
        }
    };
}
