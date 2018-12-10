#include "precomp.h"

#include "hash.h"
#include "runtime_string.h"

namespace yzg
{
    __forceinline uint64_t _rotl ( uint64_t value, int shift ) {
        return (value<<shift) | (value>>(64-shift));
    }
    
    uint64_t hash_value ( void * pX, TypeInfo * info );
    
    uint64_t hash_structure ( char * ps, StructInfo * info, int size, bool isPod ) {
        if ( isPod ) {
            return hash_function(ps, size);
        } else {
            uint64_t hash = 0;
            char * pf = ps;
            for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
                VarInfo * vi = info->fields[i];
                hash = _rotl(hash,2) ^ hash_value(pf, vi);
                pf += getTypeSize(vi);
            }
            return hash;
        }
    }
    
    uint64_t hash_array_value ( void * pX, int stride, int count, TypeInfo * info ) {
        if ( info->isPod ) {
            return hash_function(pX, stride * count);
        } else {
            uint64_t hash = 0;
            char * pA = (char *) pX;
            for ( int i=0; i!=count; ++i ) {
                hash = _rotl(hash,2) ^ hash_value(pA, info);
                pA += stride;
            }
            return hash;
        }
    }

    
    uint64_t hash_dim_value ( void * pX, TypeInfo * info ) {
        TypeInfo copyInfo = *info;
        assert(copyInfo.dimSize);
        copyInfo.dimSize --;
        int stride = getTypeBaseSize(info);
        int count = getDimSize(info);
        return hash_array_value(pX, stride, count, &copyInfo);
    }
    
    uint64_t hash_value ( void * pX, TypeInfo * info ) {
        if ( info->ref ) {
            TypeInfo ti = *info;
            ti.ref = false;
            return hash_value(*(void **)pX, &ti);
        } else if ( info->dimSize ) {
            return hash_dim_value(pX, info);
        } else if ( info->type==Type::tArray ) {
            auto arr = (Array *) pX;
            return hash_array_value(arr->data, getTypeSize(info->firstType), arr->size, info->firstType);
        } else {
            switch ( info->type ) {
                case Type::tBool:       return hash_function(*((bool *)pX));
                case Type::tInt64:      return hash_function(*((int64_t *)pX));
                case Type::tUInt64:     return hash_function(*((uint64_t *)pX));
                case Type::tString:     return hash_function(safe_str(pX));
                case Type::tInt:        return hash_function(*((int32_t *)pX));
                case Type::tInt2:       return hash_function(*((int2 *)pX));
                case Type::tInt3:       return hash_function(*((int3 *)pX));
                case Type::tInt4:       return hash_function(*((int4 *)pX));
                case Type::tUInt:       return hash_function(*((uint32_t *)pX));
                case Type::tUInt2:      return hash_function(*((uint2 *)pX));
                case Type::tUInt3:      return hash_function(*((uint3 *)pX));
                case Type::tUInt4:      return hash_function(*((uint4 *)pX));
                case Type::tFloat:      return hash_function(*((float *)pX));
                case Type::tFloat2:     return hash_function(*((float2 *)pX));
                case Type::tFloat3:     return hash_function(*((float3 *)pX));
                case Type::tFloat4:     return hash_function(*((float4 *)pX));
                case Type::tPointer:    return hash_function(intptr_t(pX));
                case Type::tIterator:   return hash_function(intptr_t(pX));
                case Type::tStructure:  return hash_structure(*(char **)pX, info->structType, getTypeSize(info), info->isPod);
                default:                assert(0 && "unsupported print type"); return 0;
            }
        }
    }
    
    uint64_t hash_function ( void * data, TypeInfo * type ) {
        return hash_value(data, type);
    }
}

