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
        if ( needIntern && str && isOwnPtr(str) ) {
            internMap.insert(str);
        }
    }

    char * StringAllocator::allocateString ( const char * text, uint32_t length ) {
        if ( length ) {
            if ( needIntern && text ) {
                auto it = internMap.find(text);
                if ( it != internMap.end() ) {
                    return (char *) *it;
                }
            }
            if (auto str = (char *)allocate(length + 1 + sizeof(StringHeader))) {
                StringHeader * header = (StringHeader *) str;
                header->length = length;
                header->hash = 0;
                str += sizeof(StringHeader);
                if ( text ) memcpy(str, text, length);
                str[length] = 0;
                if ( needIntern && text ) internMap.insert(str);
                return str;
            }
        }
        return nullptr;
    }

    void StringAllocator::freeString ( char * text, uint32_t length ) {
        DAS_ASSERT( (((StringHeader *)text)-1)->length == length );
        if ( needIntern ) internMap.erase(text);
        free ( text - sizeof(StringHeader), length + 1 + sizeof(StringHeader) );
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

    void StringAllocator::reportAllocations() {
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
                        tout << "\t\t" << header->length << "\t" << HEX << header->hash << DEC
                            << "\t" << presentStr(buf,ch,32) << "\n";
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
    }
}
