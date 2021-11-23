#pragma once

#define DAS_FREE_LIST   0

#if DAS_FREE_LIST

namespace das {
    class FreeList {
    public:
        FreeList( uint32_t os, uint32_t gs = 64 ) {
            objectSize = os;
            growSpeed = gs;
        }
        ~FreeList() {
            clear();
        }
        void clear() {
            DAS_ASSERT(totalObjects == 0);
            for ( auto chunk : chunks ) {
                das_aligned_free16(chunk);
            }
            chunks.clear();
        }
        __forceinline char * allocate () {
            if ( !freeList ) grow();
            char * result = freeList;
            freeList = *(char **)freeList;
            totalObjects ++;
            return result;
        }
        __forceinline void free ( char * ptr ) {
            *(char **)ptr = freeList;
            freeList = ptr;
            DAS_ASSERT(totalObjects > 0);
            totalObjects --;
        }
        void grow() {
            char * chunk = (char *) das_aligned_alloc16(objectSize * growSpeed);
            chunks.push_back(chunk);
            char * head = chunk;
            char * last = chunk + (growSpeed-1) * objectSize;
            while ( head != last ) {
                *((char **)head) = head + objectSize;
                head += objectSize;
            }
            *((char **)head) = freeList;
            freeList = chunk;
        }
    protected:
        char *          freeList = nullptr;
        uint32_t        objectSize = 0;
        uint32_t        growSpeed = 64;
        uint64_t        totalObjects = 0;
        vector<char *>  chunks;
    };
}

#endif

#define DAS_MAX_REUSE_SIZE      256
#define DAS_MAX_BUCKET_COUNT    (DAS_MAX_REUSE_SIZE>>4)

namespace das {
    struct ReuseChunk {
        ReuseChunk * next;
    };

    struct ReuseCache {
        ReuseChunk *    hold[DAS_MAX_BUCKET_COUNT];
    };

    extern DAS_THREAD_LOCAL ReuseCache * tlsReuseCache;

    __forceinline void * reuse_cache_allocate ( size_t size ) {
        if ( size==0 ) return nullptr;
        size = (size+15) & ~15;
        if ( size<=DAS_MAX_REUSE_SIZE && tlsReuseCache ) {
            auto bucket = (size >> 4) - 1;
            auto & hold = tlsReuseCache->hold[bucket];
            if ( hold ) {
                void * data = hold;
                hold = hold->next;
                return data;
            } else {
                return das_aligned_alloc16(size);
            }
        } else {
            return das_aligned_alloc16(size);
        }
    }

    __forceinline void reuse_cache_free ( void * ptr, size_t size ) {
        if ( ptr==nullptr ) return;
        size = (size+15) & ~15;
        if ( size<=DAS_MAX_REUSE_SIZE && tlsReuseCache ) {
            auto bucket = (size >> 4) - 1;
            auto & hold = tlsReuseCache->hold[bucket];
            auto next = hold;
            hold = (ReuseChunk *) ptr;
            hold->next = next;
        } else {
            return das_aligned_free16(ptr);
        }
    }

    __forceinline void reuse_cache_free ( void * ptr ) {
        return reuse_cache_free(ptr, das_aligned_memsize(ptr));
    }

    void reuse_cache_create();
    void reuse_cache_destroy();

    template <typename TT>
    struct ReuseAllocator {
        void * operator new ( size_t size ) {
            DAS_ASSERT(size == sizeof(TT));
            return reuse_cache_allocate(size);
        }
        void * operator new[] ( size_t size ) {
            return reuse_cache_allocate(size);
        }
        void operator delete ( void * data ) {
            return reuse_cache_free(data, sizeof(TT));
        }
        void operator delete ( void * data, size_t size ) {
            return reuse_cache_free(data, size);
        }
    };
}
