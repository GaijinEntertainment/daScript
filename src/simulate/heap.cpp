#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/heap.h"

namespace das {
    static const uint32_t min_page_size = 4096;

    HeapAllocator::HeapAllocator() {
      #if DAS_DISALLOW_EMPTY_BUDDY
        buddy.reset(min_page_size);
      #endif
    }

    HeapAllocator::~HeapAllocator() {
        reset();
    }

    void HeapAllocator::reset() {
        for ( auto & it : bigAllocations ) {
            auto ptr = it.first;
            das_aligned_free16( ptr );
        }
        bigAllocations.clear();
        uint32_t initialBuddySize = buddy.getInitialSize();
        initialBuddySize = initialBuddySize < min_page_size ? min_page_size : initialBuddySize;//small mem page
        const uint32_t bytesBuddyMaximum = buddy.calcUsed();
        buddy.reset(bytesBuddyMaximum ? initialBuddySize * (bytesBuddyMaximum + initialBuddySize-1)/initialBuddySize : 0);
        bytesTotal = 0;
    }

    char * HeapAllocator::allocate ( uint32_t size ) {
        size = (size + 0x0f) & ~0x0f;
        bytesTotal += size;
        if ( size < bigAllocationThreshold )
        {
            return buddy.allocate(size);
        } else {
            char * data = (char *) das_aligned_alloc16(size);
            bigAllocations[data] = size;
            return data;
        }
    }

    bool HeapAllocator::free ( char * data, uint32_t size ) {
        size = (size + 0x0f) & ~0x0f;
        if ( size >= bigAllocationThreshold ) {
            if ( bigAllocations.erase(data) ) {
                das_aligned_free16(data);
                bytesTotal -= size;
                return true;
            } else {
                return false;
            }
        } else {
            if ( buddy.free(data, size) ) {
                bytesTotal -= size;
                return true;
            } else {
                return false;
            }
        }
    }

    char * HeapAllocator::reallocate ( char * data, uint32_t size, uint32_t newSize ) {
        if ( !data ) return allocate(newSize);
        size = (size + 0x0f) & ~0x0f;
        newSize = (newSize + 0x0f) & ~0x0f;
        if ( size == newSize ) return data;
        bytesTotal = bytesTotal - size + newSize;
        // if both are small, and we can actually reallocate
        if ( size < bigAllocationThreshold && newSize < bigAllocationThreshold ) {
            if ( buddy.reallocate(data, size, newSize) )
                return data;
        }
        // slow path
        char * newData = allocate(newSize);
        memcpy ( newData, data, size );
        free(data, size);
        return newData;
    }

    bool HeapAllocator::isFastHeapPtr(const char *data) const {
        return  buddy.isHeapPtr(data);
    }

    bool HeapAllocator::isHeapPtr(const char *data) const {
        if ( !isFastHeapPtr(data) ) {
            for ( auto & it : bigAllocations ) {
                if ( uintptr_t(data - it.first) < uintptr_t(it.second) ) {
                    return true;
                }
            }
        }
        return false;
    }

    char * HeapAllocator::allocateName ( const string & name ) {
        if (!name.empty()) {
            auto size = uint32_t(name.length() + 1);
            if (auto str = (char *)allocate(size)) {
                memcpy(str, name.c_str(), size);
                return str;
            }
        }
        return nullptr;
    }

    char * HeapAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length==0 ) {
            return nullptr;
        } else if ( char * str = (char *) allocate(1+length+sizeof(StringHeader)) ) {
            auto header = (StringHeader *) str;
            header->hash = 0;
            header->length = length;
            auto stxt = str + sizeof(StringHeader);
            if ( text ) {
                memcpy ( stxt, text, length );
            }
            stxt[length] = 0;
            return stxt;
        } else {
            return nullptr;
        }
    }

    void HeapAllocator::setInitialSize ( uint32_t size ) {
        if (!buddy.calcUsed())
            buddy.reset(size);
    }
}
