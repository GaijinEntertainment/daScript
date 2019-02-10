#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/heap.h"

namespace das {

    BuddyAllocator::BuddyAllocator ( BuddyAllocator * j, uint32_t size ) {
        linearAllocatorSize = size;
        linearAllocator = linearAllocatorBase = (char*)das_aligned_alloc16(linearAllocatorSize);
        linearAllocatorEnd = linearAllocatorBase + linearAllocatorSize;
        junior = j;
        if ( junior ) juniorBytes = junior->bytesFree();
    }

    BuddyAllocator::~BuddyAllocator () {
        das_aligned_free16(linearAllocatorBase);
        if ( junior ) delete junior;
    }

    int BuddyAllocator::depth() const {
        return junior ? junior->depth()+1 : 1;
    }

    uint32_t BuddyAllocator::bytesFree() const {
        uint32_t bf = uint32_t(linearAllocatorSize - (linearAllocator - linearAllocatorBase));
        if ( junior ) bf += junior->bytesFree();
        return bf;
    }

    bool BuddyAllocator::isHeapPtr ( const char * data ) const {
        if ( data >= linearAllocatorBase && data < linearAllocatorEnd ) {
            return true;
        }
        return junior ? junior->isHeapPtr(data) : false;
    }

    char * BuddyAllocator::allocate ( uint32_t size ) {
        if ( junior && juniorBytes >= size ) {
            if ( char * res = junior->allocate(size) ) {
                juniorBytes -= size;
                assert ( juniorBytes == junior->bytesFree() );
                return res;
            }
        }
        if ( linearAllocator + size <= linearAllocatorEnd ) {
            char * res = linearAllocator;
            linearAllocator += size;
            return res;
        }
        return nullptr;
    }

    bool BuddyAllocator::reallocate ( char * data, uint32_t size, uint32_t newSize ) {
        if ( data >= linearAllocatorBase && data < linearAllocatorEnd ) {
            if ( data == linearAllocator - size ) {
                if ( data + newSize <= linearAllocatorEnd ) {
                    linearAllocator = data + newSize;
                    return true;
                }
            }
            return false;
        }
        if ( junior && junior->reallocate(data,size,newSize) ) {
            juniorBytes = juniorBytes - size + newSize;
            assert ( juniorBytes == junior->bytesFree() );
            return true;
        } else {
            return false;
        }
    }

    bool BuddyAllocator::free ( char * data, uint32_t size ) {
        if ( data >= linearAllocatorBase && data < linearAllocatorEnd ) {
            if ( data == linearAllocator - size ) {
                linearAllocator = data;
                return true;
            }
            return false;
        }
        return junior ? junior->free(data,size) : false;
    }

    HeapAllocator::HeapAllocator() {
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
        if ( buddy ) {
            delete buddy;
            buddy = nullptr;
        }
        bytesTotal = 0;
        bytesBuddyTotal = 0;
        initialBuddySize = max(initialBuddySize, bytesBuddyMaximum);
    }

    char * HeapAllocator::allocate ( uint32_t size ) {
        size = (size + 0x0f) & ~0x0f;
        bytesTotal += size;
        if ( size >= bigAllocationThreshold ) {
            char * data = (char *) das_aligned_alloc16(size);
            bigAllocations[data] = size;
            return data;
        } else {
            bytesBuddyTotal += size;
            bytesBuddyMaximum = max(bytesBuddyMaximum, bytesBuddyTotal);
            for ( ;; ) {
                char * res = buddy ? buddy->allocate(size) : nullptr;
                if ( res ) return res;
                uint32_t newBuddySize = buddy ? buddy->bytesTotal()*2 : initialBuddySize;
                buddy = new BuddyAllocator(buddy, newBuddySize);
            }
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
            if ( buddy && buddy->free(data,size) ) {
                bytesTotal -= size;
                bytesBuddyTotal -= size;
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
            if ( buddy && buddy->reallocate(data, size, newSize) ) {
                bytesBuddyTotal = bytesBuddyTotal - size + newSize;
                bytesBuddyMaximum = max(bytesBuddyMaximum, bytesBuddyTotal);
                return data;
            }
        }
        // slow path
        char * newData = allocate(newSize);
        memcpy ( newData, data, size );
        free(data, size);
        return newData;
    }

    bool HeapAllocator::isFastHeapPtr(const char *data) const {
        return  buddy && buddy->isHeapPtr(data);
    }

    bool HeapAllocator::isHeapPtr(const char *data) const {
        if ( !isFastHeapPtr(data) ) {
            for ( auto & it : bigAllocations ) {
                auto from = it.first;
                auto to = from + it.second;
                if ( data >= from && data < to ) {
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
        initialBuddySize = size;
    }

    int HeapAllocator::depth() const {
        return buddy ? buddy->depth() : 0;
    }
}
