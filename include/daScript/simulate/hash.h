#pragma once

#include "daScript/simulate/simulate.h"

namespace das
{
    #define HASH_EMPTY32    0
    #define HASH_KILLED32    1

    // ideas from http://isthe.com/chongo/tech/comp/fnv/

    __forceinline uint64_t hash_block64(const uint8_t * block, uint32_t size) {
        const uint64_t fnv_prime = 1099511628211ul;
        uint64_t offset_basis = 14695981039346656037ul;
        for ( ; size >=4; size-=4 ) {
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
        }
        if (size & 2u) {
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
        }
        if (size & 1u) {
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
        }
        if (offset_basis <= HASH_KILLED32) {
            return fnv_prime;
        }
        return offset_basis;
    }

    __forceinline uint32_t hash_block32(const uint8_t * block, uint32_t size) {
        const uint32_t fnv_prime = 16777619;
        const uint32_t fnv_bias = 2166136261;
        uint32_t offset_basis = fnv_bias;
        for ( ; size >=4; size-=4 ) {
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
        }
        if (size & 2u) {
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
        }
        if (size & 1u) {
            offset_basis = (offset_basis ^ *block++) * fnv_prime;
        }
        if (offset_basis <= HASH_KILLED32) {
            return fnv_prime;
        }
        return offset_basis;
    }

    __forceinline uint32_t hash_blockz32(const uint8_t * block) {
        const uint32_t fnv_prime = 16777619;
        const uint32_t fnv_bias = 2166136261;
        uint32_t offset_basis = fnv_bias;
        for (; *block; block++) {
            offset_basis = ( offset_basis ^ *block ) * fnv_prime;
        }
        if (offset_basis <= HASH_KILLED32) {
            return fnv_prime;
        }
        return offset_basis;
    }

    class HashBlock {
        const uint32_t fnv_prime = 16777619;
        uint32_t offset_basis = 2166136261;
    public:
        __forceinline void write ( const void * pb, uint32_t size ) {
            const uint8_t * block = (const uint8_t *) pb;
            while ( size-- ) {
                offset_basis = ( offset_basis ^ *block++ ) * fnv_prime;
            }
        }
        __forceinline void write ( const void * pb ) {
            const uint8_t * block = (const uint8_t *) pb;
            for (; *block; block++) {
                offset_basis = ( offset_basis ^ *block ) * fnv_prime;
            }
        }
        __forceinline uint32_t getHash() const  {
            return (offset_basis <= HASH_KILLED32) ? fnv_prime : offset_basis;
        }
    };

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
        char * hptr = (char *) ( str - sizeof(StringHeader) );
        if ( ctx.stringHeap.isOwnPtr(hptr) || ctx.constStringHeap->isOwnPtr(hptr) ) {
            auto header = (StringHeader *) hptr;
            auto hh = header->hash;
            if ( !hh ) {
                header->hash = hh = hash_block32((uint8_t *)str, header->length);
            }
            return hh;
        } else {
            return str ? hash_blockz32((uint8_t *)str) : 16777619;
        }
    }

    uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info );
    uint32_t hash_value ( Context & ctx, vec4f value, TypeInfo * info );
    uint32_t hash_value ( TypeInfo * info );
    uint32_t hash_value ( StructInfo * info );
    uint32_t hash_value ( EnumInfo * info );
    uint32_t hash_value ( FuncInfo * info );
}

