#pragma once

#include "daScript/misc/wyhash.h"

/*
    this is where we configure low level hash implementation
*/

#define HASH_EMPTY64     0
#define HASH_KILLED64    1
#define DAS_WYHASH_SEED  0x1234567890abcdefull

namespace das {

    static __forceinline uint64_t hash_block64 ( const uint8_t * block, size_t size ) {
        auto h = wyhash(block, size, DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    static __forceinline uint64_t hash_blockz64 ( const uint8_t * block ) {
        if ( !block ) return 1099511628211ul;
        auto h = wyhash(block, strlen((char *)block), DAS_WYHASH_SEED);
        return h <= HASH_KILLED64 ? 1099511628211ul : h;
    }

    class HashBuilder {
        uint64_t seed = 0;
        static const uint64_t _wyp0 = 0xa0761d6478bd642full;
        static const uint64_t _wyp1 = 0xe7037ed1a0b428dbull;
        static const uint64_t _wyp2 = 0x8ebc6af09c88c6e3ull;
        static const uint64_t _wyp3 = 0x589965cc75374cc3ull;
        static const uint64_t _wyp4 = 0x1d8e4e27c47d124full;

    public:
        HashBuilder(uint64_t seed = DAS_WYHASH_SEED) : seed(seed) {}
        uint64_t getHash() {
            return seed <= HASH_KILLED64 ? 1099511628211ul : seed;
        }
        static __forceinline uint64_t _wymum ( uint64_t A, uint64_t B ) {
            ::_wymum(&A,&B);
            return A ^ B;
        }
        __forceinline void updateString ( const char * str ) {
            if (!str) {
                seed = _wymum(seed, _wyp4);
                return;
            }
            const uint8_t* ptr = (const uint8_t*)str;
            while (*ptr) {
                seed = _wymum(
                    _wymum(seed ^ _wyp0, (uint64_t)*ptr++ ^ _wyp1),
                    _wyp2
                );
            }
            seed = _wymum(seed ^ uint64_t(strlen(str)), _wyp4);
        }
        template <typename TT>
        __forceinline void update ( TT & data ) {
            auto size = sizeof(TT);
            uint8_t * block = (uint8_t *) & data;
            while ( size-- ) {
                seed = _wymum(
                    _wymum(seed ^ _wyp0, (uint64_t)*block++ ^ _wyp1),
                    _wyp2
                );
            }
            seed = _wymum(seed ^ uint64_t(sizeof(data)), _wyp4);
        }
    };
}
