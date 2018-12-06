#pragma once

#include "simulate.h"

namespace yzg
{
    using namespace std;
    
    __forceinline uint64_t hash_function ( const void * x, size_t size ) {
        return hash<string_view>()(string_view((const char *)x,size));
    }
    
    template <typename TT>
    __forceinline uint64_t hash_function ( TT x ) {
        return hash<TT>()(x);
    }
    
    template <>
    __forceinline uint64_t hash_function ( char * x ) {
        return hash<string_view>()(string_view(x));
    }
    
    template <typename QQ>
    __forceinline uint64_t hash_function ( const vec2<QQ> & x) {
        return hash_function(&x, sizeof(vec2<QQ>));
    }
    
    template <typename QQ>
    __forceinline uint64_t hash_function ( const vec3<QQ> & x) {
        return hash_function(&x, sizeof(vec3<QQ>));
    }
    
    template <typename QQ>
    __forceinline uint64_t hash_function ( const vec4<QQ> & x) {
        return hash_function(&x, sizeof(vec4<QQ>));
    }
    
    uint64_t hash_function ( void * data, TypeInfo * type );
    
    template <typename TT>
    struct SimNode_HashOfValue : SimNode {
        SimNode_HashOfValue ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            // note, exception point not nessaray. wrong value is still ok to cast
            TT res = cast<TT>::to(subexpr->eval(context));
            return cast<uint64_t>::from ( hash_function(res) );
        }
        SimNode * subexpr;
    };
    
    struct SimNode_HashOfRef : SimNode {
        SimNode_HashOfRef ( const LineInfo & at, SimNode * s, uint32_t sz ) : SimNode(at), subexpr(s), size(sz) {}
        virtual __m128 eval ( Context & context ) override {
            char * data = cast<char *>::to(subexpr->eval(context));
            YZG_EXCEPTION_POINT;
            return cast<uint64_t>::from ( hash_function(data,size) );
        }
        SimNode *   subexpr;
        uint32_t    size;
    };
    
    struct SimNode_HashOfMixedType : SimNode {
        SimNode_HashOfMixedType ( const LineInfo & at, SimNode * s, TypeInfo *t ) : SimNode(at), subexpr(s), typeInfo(t) {}
        virtual __m128 eval ( Context & context ) override {
            char * data = cast<char *>::to(subexpr->eval(context));
            YZG_EXCEPTION_POINT;
            return cast<uint64_t>::from ( hash_function(data,typeInfo) );
        }
        SimNode *   subexpr;
        TypeInfo *  typeInfo;
    };
}

