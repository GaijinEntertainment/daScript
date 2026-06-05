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

    // Tables with capacity <= this use packed storage: keys/values/hashes are dense in
    // [0,size) (insertion order, load factor 1.0) and find/erase are a flat linear scan
    // instead of a hash probe. Crossing this on insert promotes to open addressing.
    // It must equal TableHash::minCapacity (static_assert below) so every packed table is
    // exactly this many slots — PackedFind scans a fixed count and would read out of bounds
    // otherwise. (Decoupling them — e.g. a smaller minCapacity for memory — means switching
    // PackedFind back to a capacity-bounded scan, losing the fixed unroll.)
    constexpr static uint32_t TABLE_MAX_LINEAR_CAPACITY = 8;

    // Packed (small-table) find, specialized per key type so each instantiation is a flat,
    // unrollable/vectorizable scan rather than a generic loop with a per-element compare.
    // String keys scan the hash slots and trust the 64-bit hash (no strcmp); the empty tail
    // (hash 0) never matches a real key hash (>1), so the loop runs the full fixed capacity.
    // Every other key type compares keys directly over the live prefix [0,size) and never
    // touches the hash array (the `i<sz` guard short-circuits the stale tail).
    template <typename KeyType>
    struct PackedFind {
        static __forceinline int64_t find ( const Table & tab, const KeyType & key, TableHashKey ) {
            auto pKeys = (const KeyType *) tab.keys;
            uint32_t sz = (uint32_t) tab.size;
            for ( uint32_t i=0; i!=TABLE_MAX_LINEAR_CAPACITY; ++i ) {
                if ( i<sz && KeyCompare<KeyType>()(pKeys[i], key) ) return (int64_t) i;
            }
            return -1;
        }
    };

    template <>
    struct PackedFind<char *> {
        static __forceinline int64_t find ( const Table & tab, char * const &, TableHashKey hashKey ) {
            auto pHashes = tab.hashes;
            for ( uint32_t i=0; i!=TABLE_MAX_LINEAR_CAPACITY; ++i ) {
                if ( pHashes[i]==hashKey ) return (int64_t) i;
            }
            return -1;
        }
    };

    template <>
    struct PackedFind<const char *> {
        static __forceinline int64_t find ( const Table & tab, const char * const &, TableHashKey hashKey ) {
            auto pHashes = tab.hashes;
            for ( uint32_t i=0; i!=TABLE_MAX_LINEAR_CAPACITY; ++i ) {
                if ( pHashes[i]==hashKey ) return (int64_t) i;
            }
            return -1;
        }
    };

    template <typename KeyType>
    class TableHash {
        Context *   context = nullptr;
        uint32_t    valueTypeSize = 0;
        enum {
            minCapacity = 8
        };
        static_assert(minCapacity == TABLE_MAX_LINEAR_CAPACITY,
            "PackedFind scans a fixed TABLE_MAX_LINEAR_CAPACITY slots, so every packed table must be exactly that many slots");
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
            auto hashKey = hashToHashKey(TableHashKey(hash));
            if ( tab.capacity <= TABLE_MAX_LINEAR_CAPACITY ) {  // packed
                return PackedFind<KeyType>::find(tab, key, hashKey);
            }
            auto pKeys = (const KeyType *) tab.keys;
            auto pHashes = tab.hashes;
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
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
            auto hashKey = hashToHashKey(TableHashKey(hash));
            if ( tab.capacity <= TABLE_MAX_LINEAR_CAPACITY ) {  // packed: dense, load factor 1.0
                if ( tab.capacity != 0 ) {  // dedup against existing (skip on an unallocated table)
                    int64_t found = PackedFind<KeyType>::find(tab, key, hashKey);
                    if ( found>=0 ) return found;
                }
                if ( tab.isLocked() ) context->throw_error_at(at, "can't insert into locked table");
                if ( tab.size >= tab.capacity ) {
                    // cap-0 -> first allocation (stays packed); a full packed table promotes
                    // to open addressing. *4 gives the hashed table load-factor headroom so it
                    // does not immediately re-grow (cap*2 would land exactly on the 0.5 trigger).
                    uint64_t newCapacity = (tab.capacity == 0) ? minCapacity : tab.capacity*4;
                    reserveInternal(tab, newCapacity, at);
                    return reserve(tab, key, hash, at);  // re-dispatch: now hashed (or the freshly allocated packed table)
                }
                uint64_t i = tab.size;
                tab.hashes[i] = hashKey;
                ((KeyType *) tab.keys)[i] = key;
                tab.size++;
                return (int64_t) i;
            }
            if ( tab.size >= (tab.capacity/2) ) grow(tab, at);
            else if ( (tab.capacity-tab.size)/2 < tab.tombstones ) rehash(tab, at);
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
            uint64_t insertI = ~uint64_t(0);
            auto pKeys = (KeyType *) tab.keys;
            auto pHashes = tab.hashes;
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
            auto hashKey = hashToHashKey(TableHashKey(hash));
            if ( tab.capacity <= TABLE_MAX_LINEAR_CAPACITY ) {  // packed: locate, then swap-remove the last live slot into the hole
                int64_t fidx = PackedFind<KeyType>::find(tab, key, hashKey);
                if ( fidx<0 ) return -1;
                uint64_t i = (uint64_t) fidx;
                auto pHashes = tab.hashes;
                auto pKeys = (KeyType *) tab.keys;
                uint64_t last = tab.size - 1;
                if ( i != last ) {
                    pHashes[i] = pHashes[last];
                    pKeys[i] = pKeys[last];
                    memcpy(tab.data + i*valueTypeSize, tab.data + last*valueTypeSize, valueTypeSize);
                }
                pHashes[last] = HASH_EMPTY64;
                memset(tab.data + last*valueTypeSize, 0, valueTypeSize);
                tab.size--;
                return (int64_t) i;
            }
            auto pKeys = (KeyType *) tab.keys;
            auto pHashes = tab.hashes;
            uint64_t mask = tab.capacity - 1;
            uint64_t index = hash & mask;
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
            // Guard against capacity*2 overflow — at uint64_t the wrap-to-zero produces
            // a non-power-of-two `newCapacity` that would trip reserveInternal's verify
            // and, before that, make `mask = capacity - 1` underflow into all-ones.
            if ( tab.capacity > (uint64_t(1) << 62) ) {
                context->throw_error_at(at, "table grow: capacity %llu exceeds 2^62", (unsigned long long)tab.capacity);
                return false;
            }
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
            DAS_VERIFYF(newCapacity != 0 && (newCapacity & (newCapacity - 1)) == 0, "newCapacity must be a nonzero power of 2, and not %llu", (unsigned long long)newCapacity);
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
                // Entries are only ever copied into a hashed (open-addressed) target: the
                // sole packed target is the cap-0 -> minCapacity first allocation, which has
                // no entries. So a packed-dense copy path here would be dead code.
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



