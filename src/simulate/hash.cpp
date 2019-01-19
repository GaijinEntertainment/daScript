#include "daScript/misc/platform.h"

#include "daScript/simulate/hash.h"
#include "daScript/simulate/runtime_string.h"

namespace das
{
    __forceinline uint32_t _rotl ( uint32_t value, int shift ) {
        return (value<<shift) | (value>>(32-shift));
    }
    
	uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info );
    
	uint32_t hash_structure ( Context & ctx, char * ps, StructInfo * info, int size, bool isPod ) {
        if ( isPod ) {
            return hash_function(ctx, ps, size);
        } else {
			uint32_t hash = 0;
            char * pf = ps;
            for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
                VarInfo * vi = info->fields[i];
                hash = _rotl(hash,2) ^ hash_value(ctx, pf, vi);
                pf += getTypeSize(vi);
            }
            return hash;
        }
    }
    
	uint32_t hash_array_value ( Context & ctx, void * pX, int stride, int count, TypeInfo * info ) {
        if ( info->isPod ) {
            return hash_function(ctx, pX, stride * count);
        } else {
			uint32_t hash = 0;
            char * pA = (char *) pX;
            for ( int i=0; i!=count; ++i ) {
                hash = _rotl(hash,2) ^ hash_value(ctx, pA, info);
                pA += stride;
            }
            return hash;
        }
    }
    
	uint32_t hash_dim_value ( Context & ctx, void * pX, TypeInfo * info ) {
        TypeInfo copyInfo = *info;
        assert(copyInfo.dimSize);
        copyInfo.dimSize --;
        int stride = getTypeBaseSize(info);
        int count = getDimSize(info);
        return hash_array_value(ctx, pX, stride, count, &copyInfo);
    }
    
	uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info ) {
        if ( info->ref ) {
            TypeInfo ti = *info;
            ti.ref = false;
            return hash_value(ctx, *(void **)pX, &ti);
        } else if ( info->dimSize ) {
            return hash_dim_value(ctx, pX, info);
        } else if ( info->type==Type::tArray ) {
            auto arr = (Array *) pX;
            return hash_array_value(ctx, arr->data, getTypeSize(info->firstType), arr->size, info->firstType);
        } else {
            switch ( info->type ) {
                case Type::tBool:       return hash_function(ctx,*((bool *)pX));
                case Type::tInt64:      return hash_function(ctx,*((int64_t *)pX));
                case Type::tUInt64:     return hash_function(ctx,*((uint64_t *)pX));
                case Type::tString:     return hash_function(ctx,safe_str(pX));
                case Type::tInt:        return hash_function(ctx,*((int32_t *)pX));
                case Type::tInt2:       return hash_function(ctx,*((int2 *)pX));
                case Type::tInt3:       return hash_function(ctx,*((int3 *)pX));
                case Type::tInt4:       return hash_function(ctx,*((int4 *)pX));
                case Type::tUInt:       return hash_function(ctx,*((uint32_t *)pX));
                case Type::tUInt2:      return hash_function(ctx,*((uint2 *)pX));
                case Type::tUInt3:      return hash_function(ctx,*((uint3 *)pX));
                case Type::tUInt4:      return hash_function(ctx,*((uint4 *)pX));
                case Type::tFloat:      return hash_function(ctx,*((float *)pX));
                case Type::tFloat2:     return hash_function(ctx,*((float2 *)pX));
                case Type::tFloat3:     return hash_function(ctx,*((float3 *)pX));
                case Type::tFloat4:     return hash_function(ctx,*((float4 *)pX));
                case Type::tRange:      return hash_function(ctx,*((range *)pX));
                case Type::tURange:     return hash_function(ctx,*((urange *)pX));
                case Type::tPointer:    return hash_function(ctx,intptr_t(pX));
                case Type::tIterator:   return hash_function(ctx,intptr_t(pX));
                case Type::tStructure:  return hash_structure(ctx,*(char **)pX, info->structType, getTypeSize(info), info->isPod);
                default:                assert(0 && "unsupported print type"); return 0;
            }
        }
    }
    
	uint32_t hash_function ( Context & ctx, void * data, TypeInfo * type ) {
        return hash_value(ctx, data, type);
    }
}

