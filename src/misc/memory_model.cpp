#include "daScript/misc/platform.h"

#include "daScript/misc/memory_model.h"
#include "daScript/misc/debug_break.h"

namespace das {

#if DAS_TRACK_ALLOCATIONS
    uint64_t    g_tracker = 0;
    uint64_t    g_breakpoint= -1ul;

    void das_track_breakpoint ( uint64_t id ) {
        g_breakpoint = id;
    }
#endif

    MemoryModel::MemoryModel ( uint32_t ps ) {
        DAS_ASSERTF(!(ps & 15), "page size must be 16 bytes aligned");
        pageSize = ps;
        alignMask = 15;
        totalAllocated = 0;
        maxAllocated = 0;
    }

    MemoryModel::~MemoryModel() {
        shoe.clear();
        for ( auto & itb : bigStuff ) {
            das_aligned_free16(itb.first);
        }
        bigStuff.clear();
#if DAS_SANITIZER
        for ( auto & itb : deletedBigStuff ) {
            das_aligned_free16(itb.first);
        }
        deletedBigStuff.clear();
#endif
    }

    void MemoryModel::setInitialSize ( uint32_t size ) {
        /*
        if ( size && shelf.empty() ) {
            uint32_t tp = (size+pageSize-1) / pageSize;
            shelf.emplace_back(pageSize, das::max(tp,1u));
            initialSize = size;
        }
        */
    }

    char * MemoryModel::allocate ( uint32_t size ) {
        if ( !size ) return nullptr;
        size = (size + alignMask) & ~alignMask;
        totalAllocated += size;
        maxAllocated = das::max(maxAllocated, totalAllocated);
        if ( size >= DAS_MAX_SHOE_ALLOCATION ) {
            char * ptr = (char *) das_aligned_alloc16(size);
            bigStuff[ptr] = size;
#if DAS_TRACK_ALLOCATIONS
            if ( g_tracker==g_breakpoint ) os_debug_break();
            bigStuffId[ptr] = g_tracker ++;
#endif
            return ptr;
        } else {
            return shoe.allocate(size);
        }
    }

    bool MemoryModel::free ( char * ptr, uint32_t size ) {
        if ( !size ) return true;
        size = (size + alignMask) & ~alignMask;

#if DAS_SANITIZER
        memset(ptr, 0xcd, size);
#endif
        if ( size < DAS_MAX_SHOE_ALLOCATION ) {
            shoe.free(ptr, size);
            totalAllocated -= size;
            return true;
        }
#if DAS_SANITIZER
        auto itd = deletedBigStuff.find(ptr);
        if ( itd!= deletedBigStuff.end() ) {
            os_debug_break();
        }
#endif
        auto itb = bigStuff.find(ptr);
        if ( itb!=bigStuff.end() ) {
            DAS_ASSERTF(itb->second==size, "free size mismatch, %u allocated vs %u freed", itb->second, size );
#if DAS_SANITIZER
            deletedBigStuff[itb->first] = itb->second;
#else
            das_aligned_free16(itb->first);
#endif
            bigStuff.erase(itb);
            totalAllocated -= size;
#if DAS_TRACK_ALLOCATIONS
            bigStuffId.erase(ptr);
            bigStuffAt.erase(ptr);
            bigStuffComment.erase(ptr);
#endif
            return true;
        }
        DAS_ASSERTF(0, "we are trying to delete pointer, which we did not allocate");
        return false;
    }

    char * MemoryModel::reallocate ( char * ptr, uint32_t size, uint32_t nsize ) {
        if ( !ptr ) return allocate(nsize);
        size = (size + alignMask) & ~alignMask;
        nsize = (nsize + alignMask) & ~alignMask;
        char * nptr = allocate(nsize);
        memcpy ( nptr, ptr, das::min(size,nsize) );
#if DAS_TRACK_ALLOCATIONS
        auto pAt = bigStuffAt.find(ptr);
        if ( pAt != bigStuffAt.end() ) {
            bigStuffAt[nptr] = pAt->second;
        }
        auto pCm = bigStuffComment.find(ptr);
        if ( pCm != bigStuffComment.end() ) {
            bigStuffComment[nptr] = pCm->second;
        }
#endif
        free(ptr, size);
        return nptr;
    }

    void MemoryModel::reset() {
        for ( auto & itb : bigStuff ) {
#if DAS_SANITIZER
            deletedBigStuff[itb.first] = itb.second;
#else
            das_aligned_free16(itb.first);
#endif
        }
        bigStuff.clear();
#if DAS_TRACK_ALLOCATIONS
        bigStuffId.clear();
        bigStuffAt.clear();
        bigStuffComment.clear();
#endif
        shoe.reset();
    }

    uint32_t MemoryModel::pagesTotal() const {
        return shoe.totalChunks();
    }

    uint32_t MemoryModel::pagesAllocated() const {
        return shoe.totalChunks();
    }

    uint64_t MemoryModel::totalAlignedMemoryAllocated() const {
        uint64_t mem = shoe.totalBytesAllocated();
        for (const auto & it : bigStuff) {
            mem += it.second;
        }
        return mem;
    }

    void MemoryModel::sweep() {
        totalAllocated = 0;
        for ( uint32_t si=0; si!=DAS_MAX_SHOE_CUNKS; ++si ) {   // we re-track all small allocations
            for ( auto ch=shoe.chunks[si]; ch; ch=ch->next ) {
                uint32_t utotal = ch->total / 32;
                for ( uint32_t i=0; i!=utotal; ++i ) {
                    uint32_t b = ch->bits[i];
                    for ( uint32_t j=0; j!=32; ++j ) {          // TODO: this is COUNTBITS * size
                        if ( b & (1<<j) ) {
                            totalAllocated += ch->size;
                        }
                    }
                }
            }
        }
        for ( auto it = bigStuff.begin(); it!=bigStuff.end() ; ) {
            if ( it->second & DAS_PAGE_GC_MASK ) {
                it->second &= ~DAS_PAGE_GC_MASK;
                totalAllocated += it->second;
                ++ it;
            } else {
                das_aligned_free16(it->first);
                it = bigStuff.erase(it);
            }
        }
    }

    void LinearChunkAllocator::free ( char * ptr, uint32_t s ) {
        for ( auto ch=chunk; ch; ch=ch->next ) {
            if ( ch->isOwnPtr(ptr) ) {
                ch->free(ptr,s);
                break;
            }
        }
    }

    char * LinearChunkAllocator::allocate ( uint32_t s ) {
        if ( !s ) return nullptr;
        s = (s + alignMask) & ~alignMask;
        if ( !chunk ) {
            chunk = new HeapChunk ( max(initialSize, s), nullptr );
        }
        for ( ;; ) {
            if ( char * res = chunk->allocate(s) ) {
                // DAS_ASSERTF((intptr_t(res) & alignMask) == 0, "allocated unaligned data");
                return res;
            }
            chunk = new HeapChunk ( max(growPages(chunk->size), s), chunk);
        }
    }

    void LinearChunkAllocator::reset() {
        if ( chunk ) {
            delete chunk;
            chunk = nullptr;
        }
    }

    char * LinearChunkAllocator::allocateName ( const string & name ) {
        if (!name.empty()) {
            auto length = uint32_t(name.length());
            if (auto str = (char *)allocate(length + 1)) {
                memcpy(str, name.c_str(), length);
                str[length] = 0;
                return str;
            }
        }
        return nullptr;
    }

    void LinearChunkAllocator::getStats ( uint32_t & depth, uint64_t & bytes, uint64_t & total )  const {
        depth = 0;
        bytes = 0;
        total = 0;
        for ( auto ch=chunk; ch; ch=ch->next ) {
            depth ++;
            bytes += ch->offset;
            total += ch->size;
        }
    }

    uint32_t LinearChunkAllocator::depth() const {
        uint32_t d; uint64_t b, t;
        getStats(d, b, t);
        return d;
    }

    uint64_t LinearChunkAllocator::bytesAllocated() const {
        uint32_t d; uint64_t b, t;
        getStats(d, b, t);
        return b;
    }

    uint64_t LinearChunkAllocator::totalAlignedMemoryAllocated() const {
        uint32_t d; uint64_t b, t;
        getStats(d, b, t);
        return t;
    }
}

