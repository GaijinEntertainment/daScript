#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_print.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/debug_break.h"

namespace das {

    char * HeapWriterPolicy::c_str() {
        if (!data) return nullptr;
        auto sh = ((StringHeader *)data) - 1;
        data[dataSize] = 0;
        sh->hash = 0;
        sh->length = dataSize;
#if DAS_TRACK_ALLOCATIONS
        if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
        sh->tracking_id = g_tracker_string ++;
#endif
        heap->recognize(data);
        return data;
    }

    int HeapWriterPolicy::tellp() const {
        return int(dataSize);
    }

    void HeapWriterPolicy::_reserve(int newSize) {
        int newCapacity = newSize;
        if ( newCapacity > int(heap->pageSize) ) {
            // if we exceed page size, we go to double-or-nothing grow mode
            // that way string heap pages stay without holes, but big pages get allocated less often
            newCapacity = das::max(dataCapacity * 2, newSize);
        }
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
#if DAS_TRACK_ALLOCATIONS
                StringHeader * header = (StringHeader *) newDataBase;
                if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
                header->tracking_id = g_tracker_string ++;
#endif
            }
        } else {
            data = (char *) heap->allocate(newCapacity + sizeof(StringHeader) + 1);
            if (!data) {
                dataSize = 0;
                return;
            }
#if DAS_TRACK_ALLOCATIONS
            StringHeader * header = (StringHeader *) data;
            if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
            header->tracking_id = g_tracker_string ++;
#endif
            data += sizeof(StringHeader);
        }
        dataCapacity = newCapacity;
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
