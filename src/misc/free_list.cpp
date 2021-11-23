#include "daScript/misc/platform.h"

#include "daScript/misc/free_list.h"

namespace das {

DAS_THREAD_LOCAL ReuseCache * tlsReuseCache = nullptr;


void reuse_cache_create() {
    if ( !tlsReuseCache ) {
        tlsReuseCache = (ReuseCache *) das_aligned_alloc16(sizeof(ReuseCache));
        memset(tlsReuseCache, 0, sizeof(ReuseCache));
    }
}

void reuse_cache_destroy() {
    if ( tlsReuseCache ) {
        for ( size_t bucket=0; bucket!=DAS_MAX_BUCKET_COUNT; ++bucket ) {
            ReuseChunk * & hold = tlsReuseCache->hold[bucket];
            while ( hold ) {
                auto ptr = hold;
                hold = hold->next;
                das_aligned_free16(ptr);
            }
        }
        das_aligned_free16(tlsReuseCache);
        tlsReuseCache = nullptr;
    }
}

}

void * operator new ( size_t size ) {
    return das::reuse_cache_allocate(size);
}
void * operator new[] ( size_t size ) {
    return das::reuse_cache_allocate(size);
}
void operator delete ( void * data ) {
    return das::reuse_cache_free(data);
}
void operator delete[] ( void * data ) {
    return das::reuse_cache_free(data);
}
void operator delete[] ( void * data, size_t size ) {
    return das::reuse_cache_free(data,size);
}
void operator delete ( void * data, size_t size ) {
    return das::reuse_cache_free(data, size);
}
