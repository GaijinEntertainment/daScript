#include "daScript/misc/platform.h"

#include "daScript/misc/string_writer.h"

namespace das {
    StringWriterTag HEX;
    StringWriterTag DEC;
    StringWriterTag FIXEDFP;
    StringWriterTag SCIENTIFIC;

    mutex TextPrinter::pmut;

    void TextPrinter::output() {
        lock_guard<mutex> guard(pmut);
        uint64_t newPos = tellp();
        if (newPos != pos) {
            string st(data() + pos, newPos - pos);
            printf("%s", st.c_str());
            fflush(stdout);
            pos = newPos;
        }
    }

    char * StringBuilderPolicy::c_str() {
        if ( size < capacity ) {
            largeBuffer[size] = 0;
            return largeBuffer;
        } else {
            char * newBuffer = (char *) das_aligned_alloc16(size + 1);
            memcpy(newBuffer, largeBuffer, size);
            newBuffer[size] = 0;
            if ( largeBuffer != fixedBuffer ) {
                das_aligned_free16(largeBuffer);
            }
            largeBuffer = newBuffer;
            capacity = size + 1;
            return largeBuffer;
        }
    }

    char * StringBuilderPolicy::allocate (int l) {
        // keep data in the fixed buffer, if it fits
        if ( size + l <= capacity ) {
            char * res = largeBuffer + size;
            size += l;
            return res;
        } else {
            // if it does not fit, allocate a new buffer
            int32_t newCapacity = capacity * 2;
            if ( newCapacity < size + l ) {
                newCapacity = size + l;
            }
            char * newBuffer = (char *) das_aligned_alloc16(newCapacity);
            if ( largeBuffer != fixedBuffer ) {
                memcpy(newBuffer, largeBuffer, size);
                das_aligned_free16(largeBuffer);
            } else {
                memcpy(newBuffer, fixedBuffer, size);
            }
            largeBuffer = newBuffer;
            capacity = newCapacity;
            char * res = largeBuffer + size;
            size += l;
            return res;
        }
    }
}
