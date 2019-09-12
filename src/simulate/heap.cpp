#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/heap.h"

namespace das {

    char * HeapAllocator::allocateName ( const string & name ) {
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

    void StringAllocator::setInitialSize ( uint32_t size ) {
        if ( shelf.empty() ) {
            pageSize = size;
            shelf.emplace_back(pageSize,1);
        }
    }

    char * StringAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length ) {
            if (auto str = (char *)allocate(length + 1 + sizeof(StringHeader))) {
                StringHeader * header = (StringHeader *) str;
                header->length = length;
                header->hash = 0;
                str += sizeof(StringHeader);
                if ( text ) memcpy(str, text, length);
                str[length] = 0;
                return str;
            }
        }
        return nullptr;
    }

    void StringAllocator::freeString ( char * text, uint32_t length ) {
        free ( text - sizeof(StringHeader), length + 1 + sizeof(StringHeader) );
    }
}
