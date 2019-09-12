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

    char * StringAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length ) {
            if (auto str = (char *)allocate(length + 1)) {
                if ( text ) memcpy(str, text, length);
                str[length] = 0;
                return str;
            }
        }
        return nullptr;
    }
}
