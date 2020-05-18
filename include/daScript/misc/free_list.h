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
