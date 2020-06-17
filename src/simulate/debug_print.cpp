#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_print.h"
#include "daScript/misc/fpe.h"
#include "daScript/misc/debug_break.h"

namespace das {

    char * HeapWriterPolicy::c_str() {
        if (!data) return nullptr;
        if ( dataCapacity > int(heap->pageSize) ) {
            _resize(dataSize);
        }
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

    void HeapWriterPolicy::_resize(int newCapacity) {
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
        // temporary header, so that GC does not crash if we call before doing '.c_str()'
        auto sh = ((StringHeader *)data) - 1;
        data[dataSize] = 0;
        sh->hash = 0;
        sh->length = newCapacity;
    }

    void HeapWriterPolicy::_reserve(int newSize) {
        int newCapacity = newSize;
        if ( newCapacity > int(heap->pageSize) ) {
            // if we exceed page size, we go to double-or-nothing grow mode
            // that way string heap pages stay without holes, but big pages get allocated less often
            newCapacity = das::max(dataCapacity * 2, newSize);
        }
        _resize(newCapacity);
    }

    string human_readable_formatting ( const string & str ) {
        string result;
        string tab;
        auto it = str.begin();
        auto tail = str.end();
        while ( it != tail ) {
            while ( it!=tail && *it==' ') {
                it ++;
            }
            while ( it!=tail && *it!='\n' ) {
                auto Ch = *it;
                result += Ch;
                if ( Ch=='[' ) tab += "  ";
                else if ( Ch==']' ) tab.resize(tab.size()-2);
                it ++;
            }
            if ( it == tail ) break;
            while ( it!=tail && *it=='\n' ) {
                it ++;
            }
            result += "\n";
            result += tab;
        }
        return result;
    }

    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk((char*)pX,info);
        if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
            return human_readable_formatting(ss.str());
        } else {
            return ss.str();
        }
    }

    string debug_value ( vec4f value, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk(value,info);
        if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
            return human_readable_formatting(ss.str());
        } else {
            return ss.str();
        }
    }
}
