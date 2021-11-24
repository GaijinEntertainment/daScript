#pragma once

#include "daScript/simulate/simulate.h"
#include "daScript/misc/fnv.h"

namespace das {
    __forceinline uint64_t hash_function ( Context &, const void * x, size_t size ) {
        return hash_block64((uint8_t *)x, size);
    }

    __forceinline uint32_t stringLength ( Context &, const char * str ) { // str!=nullptr
        return uint32_t(strlen(str));
    }

    __forceinline uint32_t stringLengthSafe ( Context & ctx, const char * str ) {//accepts nullptr
        return str ? stringLength(ctx,str) : 0;
    }

    template <typename TT>
    __forceinline uint64_t hash_function ( Context &, const TT x ) {
        return hash_block64((const uint8_t *)&x, sizeof(x));
    }

    template <>
    __forceinline uint64_t hash_function ( Context &, char * str ) {
        return str ? hash_blockz64((uint8_t *)str) : 1099511628211ul;
    }

    template <>
    __forceinline uint64_t hash_function ( Context &, const char * str ) {
        return str ? hash_blockz64((uint8_t *)str) : 1099511628211ul;
    }

    uint64_t hash_value ( Context & ctx, void * pX, TypeInfo * info );
    uint64_t hash_value ( Context & ctx, vec4f value, TypeInfo * info );
}

