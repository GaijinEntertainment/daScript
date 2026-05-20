#pragma once

#include "daScript/misc/arraytype.h"
#include "daScript/simulate/hash.h"

namespace das
{

    // TODO:
    //  -   return correct insert index of original value? is this at all possible?
    //  -   throw runtime error in the context, when grow inside locked table (recover well)

    DAS_API extern const char * rts_null;

    template <typename KeyType>
    struct KeyCompare {
        __forceinline bool operator () ( const KeyType & a, const KeyType & b ) {
            return a == b;
        }
    };

    template <>
    struct KeyCompare <vec4f> {
        __forceinline bool operator () ( const vec4f & a, const vec4f & b ) {
            return v_signmask(v_cmp_eq(a, b)) == 0xF;
        }
    };

    template <>
    struct KeyCompare <char *> {
        __forceinline bool operator () ( const char * a, const char * b ) {
            if ( a==b ) return true;
            if ( !a || !b ) return false;
            return strcmp(a,b)==0;
        }
    };

    template <>
    struct KeyCompare <const char *> {
        __forceinline bool operator () ( const char * a, const char * b ) {
            if ( a==b ) return true;
            if ( !a || !b ) return false;
            return strcmp(a,b)==0;
        }
    };

    template <typename KeyType>
    class TableHash {
        Context *   context = nullptr;
        uint32_t    valueTypeSize = 0;
        enum {
            minCapacity = 8,
            minLookups = 4
        };
    public:
        TableHash () = delete;
        TableHash ( const TableHash & ) = delete;
        TableHash ( Context * ctx, uint32_t vs ) : context(ctx), valueTypeSize(vs) {}

        __forceinline TableHashKey hashToHashKey ( TableHashKey hash ) const {
            if ( sizeof(TableHashKey)==4 ) return hash <= 1 ? 16777619u : hash; // this should optimize out
            else return hash;
        }

        __forceinline int64_t find ( const Table & tab, KeyType key, uint64_t hash ) const {
            DAS_ASSERT(hash>1);
            if ( tab.capacity==0 ) return -1;
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
            auto pKeys = (const KeyType *) tab.keys;
            auto pHashes = tab.hashes;
            auto hashKey = hashToHashKey(TableHashKey(hash));
            while ( true ) {
                auto kh = pHashes[index];
                if ( kh==HASH_EMPTY64 ) {
                    return -1;
                } else if ( kh==hashKey && KeyCompare<KeyType>()(pKeys[index],key) ) {
                    return (int64_t) index;
                }
                index = (index + 1) & mask;
            }
        }

        __forceinline int64_t reserve ( Table & tab, KeyType key, uint64_t hash, LineInfo * at = nullptr ) {
            DAS_ASSERT(hash>1);
            if ( tab.size >= (tab.capacity/2) ) grow(tab, at);
            else if ( (tab.capacity-tab.size)/2 < tab.tombstones ) rehash(tab, at);
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
            uint64_t insertI = ~uint64_t(0);
            auto pKeys = (KeyType *) tab.keys;
            auto pHashes = tab.hashes;
            auto hashKey = hashToHashKey(TableHashKey(hash));
            while ( true ) {
                auto kh = pHashes[index];
                if (kh == HASH_EMPTY64 ) {
                    if ( tab.isLocked() ) context->throw_error_at(at, "can't insert into locked table");
                    if ( insertI != ~uint64_t(0) ) {
                        index = insertI;
                        tab.tombstones--;
                    }
                    pHashes[index] = hashKey;
                    pKeys[index] = key;
                    tab.size++;
                    return (int64_t)index;
                } else if (kh == HASH_KILLED64) {
                    if ( insertI == ~uint64_t(0) ) insertI = index;
                } else if (kh == hashKey && KeyCompare<KeyType>()(pKeys[index], key)) {
                    return (int64_t)index;
                }
                index = (index + 1) & mask;
            }
        }

        __forceinline int64_t erase ( Table & tab, KeyType key, uint64_t hash ) {
            DAS_ASSERT(hash>1);
            if ( tab.capacity==0 ) return -1;
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
            auto pKeys = (const KeyType *) tab.keys;
            auto pHashes = tab.hashes;
            auto hashKey = hashToHashKey(TableHashKey(hash));
            while ( true ) {
                auto kh = pHashes[index];
                if ( kh==HASH_EMPTY64 ) {
                    return -1;
                } else if ( kh==hashKey && KeyCompare<KeyType>()(pKeys[index],key) ) {
                    tab.size--;
                    tab.tombstones++;
                    pHashes[index] = HASH_KILLED64;
                    memset(tab.data + index*valueTypeSize, 0, valueTypeSize);
                    return (int64_t) index;
                }
                index = (index + 1) & mask;
            }
        }

        bool grow ( Table & tab, LineInfo * at ) {
            uint64_t newCapacity = das::max(uint64_t(minCapacity), tab.capacity*2);
            return reserveInternal(tab, newCapacity, at);
        }

        bool rehash ( Table & tab, LineInfo * at ) {
            return reserveInternal(tab, tab.capacity, at);
        }

        bool reserve(Table & tab, uint64_t size, LineInfo * at ) {
            if (size <= tab.capacity)
              return true;

            // Guard against overflow before doubling — otherwise huge `size` (e.g. a negative
            // signed input cast to uint64) would infinite-loop here as newCapacity wraps to 0.
            if (size > (uint64_t(1) << 62)) {
              context->throw_error_at(at, "table reserve: size %llu exceeds 2^62", (unsigned long long)size);
              return false;
            }
            uint64_t newCapacity = das::max(uint64_t(minCapacity), tab.capacity*2);
            while (newCapacity < size)
            {
              newCapacity *= 2;
            }

            return reserveInternal(tab, newCapacity, at);
        }

    private:
        // Returns the slot index. Always finds a slot (called only during rehash on a fresh
        // pre-allocated table), so signed int64_t can hold any value the uint64 capacity allows.
        __forceinline int64_t insertNew ( Table & tab, uint64_t hash ) const {
            // TODO: take key under account and be less aggressive?
            DAS_ASSERT(hash>1);
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
            auto pHashes = tab.hashes;
            while ( true ) {
                auto kh = pHashes[index];
                if ( kh==HASH_EMPTY64 ) {
                    return (int64_t) index;
                }
                index = (index + 1) & mask;
            }
        }

        bool reserveInternal(Table & tab, uint64_t newCapacity, LineInfo * at) {
            DAS_VERIFYF((newCapacity & (newCapacity) - 1) == 0, "newCapacity must be power of 2, and not %llu", (unsigned long long)newCapacity);
            if ( tab.magic!=0 || tab.lock!=0 ) {
                context->throw_error_at(at, "can't grow a locked table");
                return false;
            }
            Table newTab;
            uint64_t perSlot = uint64_t(valueTypeSize) + uint64_t(sizeof(KeyType)) + uint64_t(sizeof(TableHashKey));
            if ( perSlot && newCapacity > UINT64_MAX / perSlot ) {
                context->throw_error_ex("can't grow table, capacity*perSlot overflows uint64 [capacity=%llu]", (unsigned long long)newCapacity);
                return false;
            }
            uint64_t memSize64 = newCapacity * perSlot;
            const char * prev_comment = tab.data ? context->heap->get_comment(tab.data) : nullptr;
            newTab.data = (char *) context->allocate(memSize64, at);
            context->heap->mark_comment(newTab.data, prev_comment ? prev_comment : "table");
            newTab.keys = newTab.data + newCapacity * valueTypeSize;
            newTab.hashes = (TableHashKey *)(newTab.keys + newCapacity * sizeof(KeyType));
            newTab.size = tab.size;
            newTab.capacity = newCapacity;
            newTab.lock = tab.lock;
            newTab.magic = 0;
            newTab.flags = tab.flags;
            newTab.tombstones = 0;
            if ( valueTypeSize ) memset(newTab.data, 0, size_t(newCapacity)*size_t(valueTypeSize));
            auto pHashes = newTab.hashes;
            memset(pHashes, 0, size_t(newCapacity) * sizeof(TableHashKey));
            if ( tab.size ) {
                auto pKeys = (KeyType *) newTab.keys;
                auto pOldValues = tab.data;
                auto pValues = newTab.data;
                auto pOldKeys = (const KeyType *) tab.keys;
                auto pOldHashes = tab.hashes;
                for ( uint64_t i=0, is=tab.capacity; i!=is; ++i ) {
                    auto hash = pOldHashes[i];
                    if ( hash>HASH_KILLED64 ) {
                        int64_t index = insertNew(newTab, hash);
                        pHashes[index] = hash;
                        pKeys[index] = pOldKeys[i];
                        memcpy ( pValues + index*valueTypeSize, pOldValues + i*valueTypeSize, valueTypeSize );
                    }
                }
            }
            if (tab.capacity && !context->verySafeContext) {
                uint64_t oldSize = tab.capacity * perSlot;
                context->free(tab.data, oldSize, at);
            }
            swap ( newTab, tab );
            return true;
        }
    };
}



