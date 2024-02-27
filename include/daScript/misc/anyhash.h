#pragma once

#include "daScript/misc/wyhash.h"

/*
    this is where we configure low level hash implementation
*/

#define HASH_EMPTY64     0
#define HASH_KILLED64    1
#define DAS_WYHASH_SEED  0x1234567890abcdeful

namespace das {

    static __forceinline uint64_t hash_block64 ( const uint8_t * block, size_t size ) {
        auto h = wyhash(block, size, DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    static __forceinline uint64_t hash_blockz64 ( const uint8_t * block ) {
        if ( !block ) block = (const uint8_t *) "";
        auto h = wyhash(block, strlen((char *)block), DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    static __forceinline uint64_t hash_uint32 ( uint32_t value ) {  // this is simplified, and not the same as wyhash(&value,4)
        auto h = _wymix(uint64_t(value), DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    static __forceinline uint64_t hash_uint64 ( uint64_t value ) { // this is simplified, and not the same as wyhash(&value,4)
        auto h = _wymix(value, DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    class HashBuilder {
        uint64_t seed = DAS_WYHASH_SEED;
    public:
        uint64_t getHash() {
            return seed <= HASH_KILLED64 ? 1099511628211ul : seed;
        }
        __forceinline void updateString ( const char * str ) {
            if (!str) str = "";
            seed = wyhash((const uint8_t *)str, strlen(str), seed);
        }
        template <typename TT>
        __forceinline void update ( TT & data ) {
            seed = wyhash((const uint8_t *)&data, sizeof(data), seed);
        }
    };
}
