#include "daScript/misc/platform.h"

#include "daScript/misc/memory_model.h"

namespace das {

    void Book::reset() {
        totalSize = totalFree = pageSize * totalPages;
        freePageIndex = 0;
        memset ( pages, 0, sizeof(Page)*totalPages );
    }

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
        alignMask = 15;
        totalAllocated = 0;
        maxAllocated = 0;
    }

    MemoryModel::~MemoryModel() {
        shelf.clear();
        for ( auto & itb : bigStuff ) {
            das_aligned_free16(itb.first);
        }
        bigStuff.clear();
    }

    void MemoryModel::setInitialSize ( uint32_t size ) {
        if ( size && shelf.empty() ) {
            uint32_t tp = (size+pageSize-1) / pageSize;
            shelf.emplace_back(pageSize, das::max(tp,1u));
            initialSize = size;
        }
    }

    char * MemoryModel::allocate ( uint32_t size ) {
        if ( !size ) return nullptr;
        size = (size + alignMask) & ~alignMask;
        totalAllocated += size;
        maxAllocated = das::max(maxAllocated, totalAllocated);
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
            uint32_t npc = shelf.empty() ? initial_page_count : growPages(shelf.back().totalPages);
            shelf.emplace_back(pageSize, npc);
            return shelf.back().allocate(size);
        }
    }

    bool MemoryModel::free ( char * ptr, uint32_t size ) {
        if ( !size ) return true;
        size = (size + alignMask) & ~alignMask;
        for ( auto & book : shelf ) {
            if ( book.isOwnPtr(ptr) ) {
                book.free(ptr, size);
                totalAllocated -= size;
                return true;
            }
        }
        auto itb = bigStuff.find(ptr);
        if ( itb!=bigStuff.end() ) {
            DAS_ASSERTF(itb->second==size, "free size mismatch, %u allocated vs %u freed", itb->second, size );
            das_aligned_free16(itb->first);
            bigStuff.erase(itb);
            totalAllocated -= size;
            return true;
        }
        return false;
    }

    char * MemoryModel::reallocate ( char * ptr, uint32_t size, uint32_t nsize ) {
        if ( !ptr ) return allocate(nsize);
        size = (size + alignMask) & ~alignMask;
        nsize = (nsize + alignMask) & ~alignMask;
        totalAllocated = totalAllocated - size + nsize;
        maxAllocated = das::max(maxAllocated, totalAllocated);
        if ( size<pageSize && nsize<pageSize ) {
            for ( auto & book : shelf ) {
                if ( book.isOwnPtr(ptr) ) {
                    if ( auto nptr = book.reallocate(ptr, size, nsize) ) {
                        return nptr;
                    }
                }
            }
        }
        char * nptr = allocate(nsize);
        memcpy ( nptr, ptr, size );
        free(ptr, size);
        return nptr;
    }

    void MemoryModel::reset() {
        for ( auto & itb : bigStuff ) {
            das_aligned_free16(itb.first);
        }
        bigStuff.clear();
        if ( shelf.size()!=1 ) {
            uint32_t pages = pagesTotal();
            if ( pages ) {
                shelf.clear();
                shelf.emplace_back(pageSize, pages);
            }
        } else {
            shelf[0].reset();
        }
    }

    uint32_t MemoryModel::pagesTotal() const {
        uint32_t total = 0;
        for ( const auto & book : shelf ) {
            total += book.totalPages;
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

    uint64_t MemoryModel::totalAlignedMemoryAllocated() const {
        uint64_t mem = 0;
        for (const auto & book : shelf) {
            mem += book.totalSize;
        }
        for (const auto & it : bigStuff) {
            mem += it.second;
        }
        return mem;
    }

    void MemoryModel::sweep() {
        totalAllocated = 0;
        for ( auto & book : shelf ) {
            for ( uint32_t i=0; i!=book.totalPages; ++i ) {
                auto & page = book.pages[i];
                if ( page.total & DAS_PAGE_GC_MASK ) {
                    page.total &= ~DAS_PAGE_GC_MASK;
                    totalAllocated += page.total;
                } else {
                    page.offset = 0;
                    page.total = 0;
                }
            }
        }
        for ( auto it = bigStuff.begin(); it!=bigStuff.end() ; ) {
            if ( it->second & DAS_PAGE_GC_MASK ) {
                it->second &= ~DAS_PAGE_GC_MASK;
                totalAllocated += it->second;
                ++ it;
            } else {
                it = bigStuff.erase(it);
            }
        }
    }
}

