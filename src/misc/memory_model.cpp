#include "daScript/misc/platform.h"

#include "daScript/misc/memory_model.h"

namespace das {

    void Book::moveBook ( Book & b ) {
        pageSize = b.pageSize;
        totalPages = b.totalPages;
        totalSize = b.totalSize;
        totalFree = b.totalFree;
        freePageIndex = b.freePageIndex;
        data = b.data; b.data = nullptr;
        pages = b.pages; b.pages = nullptr;
    }

    Book::~Book() {
        if ( pages ) {
            delete [] pages;
            pages = nullptr;
        }
        if ( data ) {
            das_aligned_free16(data);
            data = nullptr;
        }
    }

    MemoryModel::MemoryModel ( uint32_t ps ) {
        DAS_ASSERTF(!(ps & 15), "page size must be 16 bytes aligned");
        pageSize = ps;
    }

    MemoryModel::~MemoryModel() {
        shelf.clear();
        for ( auto & itb : bigStuff ) {
            das_aligned_free16(itb.first);
        }
        bigStuff.clear();
    }

    void MemoryModel::setInitialSize ( uint32_t size ) {
        if ( shelf.empty() ) {
            uint32_t tp = (size+pageSize-1) / pageSize;
            shelf.emplace_back(pageSize, max(tp,1u));
        }
    }

    char * MemoryModel::allocate ( uint32_t size ) {
        if ( !size ) return nullptr;
        size = (size + 15) & ~15;
        if ( size > pageSize ) {
            char * ptr = (char *) das_aligned_alloc16(size);
            bigStuff[ptr] = size;
            return ptr;
        } else {
            for ( auto & book : shelf ) {
                if ( char * ptr = book.allocate(size) ) {
                    return ptr;
                }
            }
            uint32_t npc = shelf.empty() ? initial_page_count : (shelf.back().totalPages * 2);
            return shelf.emplace_back(pageSize,npc).allocate(size);
        }
    }

    bool MemoryModel::free ( char * ptr, uint32_t size ) {
        if ( !size ) return true;
        size = (size + 15) & ~15;
        for ( auto & book : shelf ) {
            if ( book.isOwnPtr(ptr) ) {
                book.free(ptr, size);
                return true;
            }
        }
        auto itb = bigStuff.find(ptr);
        if ( itb!=bigStuff.end() ) {
            DAS_ASSERTF(itb->second==size, "free size mismatch, %u allocated vs %u freed", itb->second, size );
            das_aligned_free16(itb->first);
            bigStuff.erase(itb);
            return true;
        }
        return false;
    }

    char * MemoryModel::reallocate ( char * ptr, uint32_t size, uint32_t nsize ) {
        if ( !ptr ) return allocate(nsize);
        size = (size + 15) & ~15;
        nsize = (nsize + 15) & ~15;
        if ( size<pageSize && nsize<pageSize ) {
            for ( auto & book : shelf ) {
                if ( auto nptr = book.reallocate(ptr, size, nsize) ) {
                    return nptr;
                }
            }
        }
        char * nptr = allocate(nsize);
        memcpy ( nptr, ptr, size );
        free(ptr, size);
        return nptr;
    }

    uint32_t MemoryModel::bytesAllocated() const {
        uint32_t total = 0;
        for ( const auto & book : shelf ) {
            total += book.totalSize - book.totalFree;
        }
        for ( const auto & itb : bigStuff ) {
            total += itb.second;
        }
        return total;
    }

    uint32_t MemoryModel::pagesAllocated() const {
        uint32_t total = 0;
        for ( const auto & book : shelf ) {
            for ( uint32_t i=0; i!=book.totalPages; ++i ) {
                const auto & page = book.pages[i];
                if ( page.offset ) total ++;
            }
        }
        return total;
    }
}

