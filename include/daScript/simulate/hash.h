#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/fnv.h"

namespace das {
    __forceinline uint32_t hash_function ( Context &, const void * x, size_t size ) {
        return hash_block32((uint8_t *)x, uint32_t(size));
    }

    __forceinline uint32_t stringLength ( Context &, const char * str ) { // str!=nullptr
        return uint32_t(strlen(str));
    }

    __forceinline uint32_t stringLengthSafe ( Context & ctx, const char * str ) {//accepts nullptr
        return str ? stringLength(ctx,str) : 0;
    }

    template <typename TT>
    __forceinline uint32_t hash_function ( Context &, const TT x ) {
        return hash_block32((const uint8_t *)&x, sizeof(x));
    }

    template <>
    __forceinline uint32_t hash_function ( Context &, char * str ) {
        return str ? hash_blockz32((uint8_t *)str) : 16777619;
    }

    template <>
    __forceinline uint32_t hash_function ( Context &, const char * str ) {
        return str ? hash_blockz32((uint8_t *)str) : 16777619;
    }

    uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info );
    uint32_t hash_value ( Context & ctx, vec4f value, TypeInfo * info );
}

