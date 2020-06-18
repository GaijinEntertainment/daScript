#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/heap.h"
#include "daScript/misc/debug_break.h"

namespace das {

#if DAS_TRACK_ALLOCATIONS
    uint64_t    g_tracker_string = 0;
    uint64_t    g_breakpoint_string = -1ul;

    void das_track_string_breakpoint ( uint64_t id ) {
        g_breakpoint_string = id;
    }
#endif

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

    void StringAllocator::setIntern(bool on) {
        needIntern = on;
        if ( !needIntern ) {
            das_string_set empty;
            swap ( internMap, empty );
        }
    }

    void StringAllocator::reset() {
        HeapAllocator::reset();
        das_string_set empty;
        swap ( internMap, empty );
    }

    char * StringAllocator::intern(const char * str) const {
        if ( needIntern ) {
            auto it = internMap.find(str);
            return it != internMap.end() ? ((char *)*it) : nullptr;
        } else {
            return nullptr;
        }
    }

    void StringAllocator::recognize ( char * str ) {
        if ( !str ) return;
        if ( needIntern && str && isOwnPtr(str, strlen(str)+1) ) {
            internMap.insert(str);
        }
    }

    char * ConstStringAllocator::intern(const char * str) const {
        auto it = internMap.find(str);
        return it != internMap.end() ? ((char *)*it) : nullptr;
    }

    void ConstStringAllocator::reset () {
        LinearChunkAllocator::reset();
        das_string_set dummy;
        swap(internMap, dummy);
    }

    char * ConstStringAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length ) {
            if ( text ) {
                auto it = internMap.find(text);
                if ( it != internMap.end() ) {
                    return (char *) *it;
                }
            }
            if ( auto str = (char *)allocate(length + 1) ) {
                if ( text ) memcpy(str, text, length);
                str[length] = 0;
                internMap.insert(str);
                return str;
            }
        }
        return nullptr;
    }

    char * StringAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length ) {
            if ( needIntern && text ) {
                auto it = internMap.find(text);
                if ( it != internMap.end() ) {
                    return (char *) *it;
                }
            }
            if ( auto str = (char *)allocate(length + 1) ) {
#if DAS_TRACK_ALLOCATIONS
                if ( g_tracker_string==g_breakpoint_string ) os_debug_break();
                header->tracking_id = g_tracker_string ++;
#endif
                if ( text ) memcpy(str, text, length);
                str[length] = 0;
                if ( needIntern && text ) internMap.insert(str);
                return str;
            }
        }
        return nullptr;
    }

    void StringAllocator::freeString ( char * text, uint32_t length ) {
        if ( needIntern ) internMap.erase(text);
        free ( text, length + 1 );
    }

    char * presentStr ( char * buf, char * ch, int size ) {
        auto str = escapeString(ch);
        strncpy(buf,str.c_str(),size);
        buf[size-4] = '.';
        buf[size-3] = '.';
        buf[size-2] = '.';
        buf[size-1] = 0;
        return buf;
    }

    void StringAllocator::forEachString ( function<void (const char *)> && fn ) {
        /*
        for ( size_t bi=0; bi!=shelf.size(); ++bi ) {
            auto & book = shelf[bi];
            for ( uint32_t i=0; i!=book.totalPages; ++i ) {
                const auto & page = book.pages[i];
                if ( page.total ) {
                    uint32_t dofs = 0;
                    while ( dofs != page.offset ) {
                        char * ch = book.data + i*book.pageSize + dofs;
                        auto header = (StringHeader *) ch;
                        ch += sizeof(StringHeader);
                        fn ( ch );
                        uint32_t bytes = sizeof(StringHeader) + header->length + 1;
                        bytes = (bytes + alignMask) & ~alignMask;
                        dofs += bytes;
                        DAS_ASSERT(dofs <= page.offset);
                    }
                }
            }
        }
        if ( !bigStuff.empty() ) {
            for ( auto it : bigStuff ) {
                char * ch = (char *)it.first;
                ch += sizeof(StringHeader);
                fn ( ch );
            }
        }
        */
    }

    void StringAllocator::sweep() {
        HeapAllocator::sweep();
        if ( needIntern ) {
            das_string_set empty;
            swap ( internMap, empty );
            forEachString([&](const char * str){
                internMap.insert(str);
            });
        }
    }

    void HeapAllocator::reportAllocations() {
/*
        TextPrinter tout;
        for ( size_t bi=0; bi!=shelf.size(); ++bi ) {
            auto & book = shelf[bi];
            tout << "book " << int(bi) << ": " << book.totalPages << " pages, " << book.pageSize << " bytes each\n";
            for ( uint32_t i=0; i!=book.totalPages; ++i ) {
                const auto & page = book.pages[i];
                if ( page.total ) {
                    tout << "\tpage " << i << ": " << page.total << " of " << page.offset << " bytes\n";
                } else {
                    tout << "\tpage " << i << ": empty\n";
                }
            }
        }
        if ( !bigStuff.empty() ) {
            tout << "big stuff:\n";
#if DAS_TRACK_ALLOCATIONS
            tout << "size\tpointer\t\tid\n";
#else
            tout << "size\tpointern";
#endif
            for ( const auto & it : bigStuff ) {
#if DAS_TRACK_ALLOCATIONS
                auto itId = bigStuffId.find(it.first);
                uint64_t eeid = itId==bigStuffId.end() ? 0 : itId->second;
                tout << "\t" << it.second << "\t0x" << HEX << intptr_t(it.first) << DEC
                    << "\t" << eeid;
                auto itComment = bigStuffComment.find(it.first);
                if ( itComment != bigStuffComment.end() ) {
                    tout << "\t" << itComment->second;
                }
                auto itLoc = bigStuffAt.find(it.first);
                if ( itLoc != bigStuffAt.end() ) {
                    tout << "\t" << itLoc->second->describe();
                }
                tout << "\n";
#else
                tout << "\t" << it.second << "\t0x" << HEX << intptr_t(it.first) << DEC
                    << "\n";
#endif
            }
#if DAS_TRACK_ALLOCATIONS
            das_safe_map<LineInfo,int64_t> bytesPerLocation;
            for ( const auto & ppl : bigStuffAt) {
                auto ptr = ppl.first;
                auto bytes = bigStuff[ptr];
                bytesPerLocation[*(ppl.second)] += bytes;
            }
            if ( !bytesPerLocation.empty() ) {
                vector<pair<int64_t,LineInfo>> bplv;
                bplv.reserve(bytesPerLocation.size());
                for ( const auto & bpl : bytesPerLocation ) {
                    bplv.emplace_back(make_pair(bpl.second,bpl.first));
                }
                sort(bplv.begin(), bplv.end(), [&]( auto a, auto b){
                    return a.first > b.first;
                });
                tout << "bytes per location:\n";
                for ( const auto & bpl : bplv ) {
                    tout << bpl.first << "\t" << bpl.second.describe() << "\n";
                }
            }
#endif
        }
*/
    }

    void StringAllocator::reportAllocations() {
/*
        TextPrinter tout;
        char buf[33];
        for ( size_t bi=0; bi!=shelf.size(); ++bi ) {
            auto & book = shelf[bi];
            tout << "book " << int(bi) << ": " << book.totalPages << " pages, " << book.pageSize << " bytes each\n";
            for ( uint32_t i=0; i!=book.totalPages; ++i ) {
                const auto & page = book.pages[i];
                if ( page.total ) {
                    tout << "\tpage " << i << ": " << page.total << " of " << page.offset << " bytes\n";
                    uint32_t dofs = 0;
                    while ( dofs != page.offset ) {
                        char * ch = book.data + i*book.pageSize + dofs;
                        auto header = (StringHeader *) ch;
                        ch += sizeof(StringHeader);
#if DAS_TRACK_ALLOCATIONS
                        tout << "\t\t" << header->length << "\t" << HEX << header->hash << DEC
                            << "\t" << header->tracking_id << "\t" << presentStr(buf,ch,32) << "\n";
#else
                        tout << "\t\t" << header->length << "\t" << HEX << header->hash << DEC
                            << "\t" << presentStr(buf,ch,32) << "\n";
#endif
                        uint32_t bytes = sizeof(StringHeader) + header->length + 1;
                        bytes = (bytes + alignMask) & ~alignMask;
                        dofs += bytes;
                        DAS_ASSERT(dofs <= page.offset);
                    }
                } else {
                    tout << "\tpage " << i << ": empty\n";
                }
            }
        }
        if ( !bigStuff.empty() ) {
            tout << "big stuff:\n";
            for ( auto it : bigStuff ) {
                char * ch = (char *)it.first;
                auto header = (StringHeader *) ch;
                ch += sizeof(StringHeader);
                strncpy(buf,ch,32);
                buf[32] = 0;
                tout << "\t" << header->length << "\t" << HEX << header->hash << DEC
                    << "\t" << presentStr(buf,ch,32) << "\n";
            }
        }
*/
    }
}
