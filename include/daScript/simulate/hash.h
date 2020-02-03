#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/fnv.h"

namespace das {
    __forceinline uint32_t hash_function ( Context &, const void * x, size_t size ) {
        return hash_block32((uint8_t *)x, uint32_t(size));
    }

    __forceinline uint32_t stringLength ( Context & ctx, const char * str ) { // str!=nullptr
        char * hptr = (char *) ( str - sizeof(StringHeader) );
        if ( ctx.stringHeap.isOwnPtr(hptr) || ctx.constStringHeap->isOwnPtr(hptr) ) {
            auto header = (StringHeader *) hptr;
            return header->length;
        } else {
            return uint32_t(strlen(str));
        }
    }

    __forceinline uint32_t stringLengthSafe ( Context & ctx, const char * str ) {//accepts nullptr
        return str ? stringLength(ctx,str) : 0;
    }

    template <typename TT>
    __forceinline uint32_t hash_function ( Context &, const TT x ) {
        return hash_block32((const uint8_t *)&x, sizeof(x));
    }

    template <>
    __forceinline uint32_t hash_function ( Context & ctx, char * str ) {
        if ( !str ) return 16777619;
        char * hptr = (char *) ( str - sizeof(StringHeader) );
        if ( ctx.stringHeap.isOwnPtr(hptr) || ctx.constStringHeap->isOwnPtr(hptr) ) {
            auto header = (StringHeader *) hptr;
            auto hh = header->hash;
            if ( !hh ) {
                header->hash = hh = hash_block32((uint8_t *)str, header->length);
            }
            return hh;
        } else {
            return hash_blockz32((uint8_t *)str);
        }
    }

    uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info );
    uint32_t hash_value ( Context & ctx, vec4f value, TypeInfo * info );
    uint32_t hash_value ( TypeInfo * info );
    uint32_t hash_value ( StructInfo * info );
    uint32_t hash_value ( EnumInfo * info );
    uint32_t hash_value ( FuncInfo * info );
}

