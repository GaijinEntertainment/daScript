#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_print.h"

namespace das {

    char * HeapWriterPolicy::c_str() {
        if (!data) return nullptr;
        auto sh = ((StringHeader *)data) - 1;
        data[dataSize] = 0;
        sh->hash = 0;
        sh->length = dataSize;
        return data;
    }

    int HeapWriterPolicy::tellp() const {
        return int(dataSize);
    }

    void HeapWriterPolicy::reserve(int newSize) {
        if (newSize > dataCapacity) {
            int newCapacity = das::max(dataCapacity * 2, newSize);
            if (data) {
                char * oldDataBase = data - sizeof(StringHeader);
                char * newDataBase = (char *)heap->reallocate(oldDataBase,
                    dataCapacity + sizeof(StringHeader) + 1, newCapacity + sizeof(StringHeader) + 1);
                if (newDataBase == nullptr) {
                    data = nullptr;
                    dataSize = 0;
                    return;
                } else if (oldDataBase != newDataBase) {
                    data = newDataBase + sizeof(StringHeader);
                }
            } else {
                data = (char *) heap->allocate(newCapacity + sizeof(StringHeader) + 1);
                if (!data) {
                    dataSize = 0;
                    return;
                }
                data += sizeof(StringHeader);
            }
            dataCapacity = newCapacity;
        }
    }

    void HeapWriterPolicy::append(const char * s, int l) {
        int newSize = dataSize + l;
        reserve(newSize);
        if (data) {
            memcpy(data + dataSize, s, l);
            dataSize += l;
        }
    }

    char * HeapWriterPolicy::allocate (int l) {
        reserve(dataSize + l);
        if (!data) return nullptr;
        dataSize += l;
        return data + dataSize - l;
    }

    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk((char*)pX,info);
        return ss.str();
    }

    string debug_value ( vec4f value, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk(value,info);
        return ss.str();
    }
}
