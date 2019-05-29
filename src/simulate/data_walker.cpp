#include "daScript/misc/platform.h"

#include "daScript/simulate/data_walker.h"
#include "daScript/ast/ast.h"
#include "daScript/simulate/hash.h"

namespace das {

    void DataWalker::error ( const char * message ) {
        if ( context ) {
            context->throw_error(message);
        }
        cancel = true;
    }

    void DataWalker::walk ( vec4f x, TypeInfo * info ) {
        if ( info->flags & TypeInfo::flag_refType ) {
            walk(cast<char *>::to(x), info );
        } else {
            walk((char *)&x, info );
        }
    }

    void DataWalker::walk_struct ( char * ps, StructInfo * si ) {
        beforeStructure(ps, si);
        if ( cancel ) return;
        for ( uint32_t i=0; i!=si->count; ++i ) {
            bool last = i==(si->count-1);
            VarInfo * vi = si->fields[i];
            char * pf = ps + vi->offset;
            beforeStructureField(ps, si, pf, vi, last);
            if ( cancel ) return;
            walk(pf, vi);
            if ( cancel ) return;
            afterStructureField(ps, si, pf, vi, last);
            if ( cancel ) return;
        }
        afterStructure(ps, si);
    }

    void DataWalker::walk_array ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) {
        char * pe = pa;
        beforeArrayData(pa, stride, count, ti);
        if ( cancel ) return;
        for ( uint32_t i=0; i!=count; ++i ) {
            bool last = i==count-1;
            beforeArrayElement(pa, ti, pe, i, last);
            if ( cancel ) return;
            walk(pe, ti);
            if ( cancel ) return;
            afterArrayElement(pa, ti, pe, i, last);
            if ( cancel ) return;
            pe += stride;
        }
        afterArrayData(pa, stride, count, ti);
    }

    void DataWalker::walk_dim ( char * pa, TypeInfo * ti ) {
        beforeDim(pa, ti);
        if ( cancel ) return;
        TypeInfo copyInfo = *ti;
        DAS_ASSERT(copyInfo.dimSize);
        copyInfo.dimSize --;
        uint32_t stride = getTypeBaseSize(ti);
        uint32_t count = getDimSize(ti);
        walk_array(pa, stride, count, &copyInfo);
        if ( cancel ) return;
        afterDim(pa, ti);
    }

    void DataWalker::walk_table ( Table * tab, TypeInfo * info ) {
        int keySize = getTypeSize(info->firstType);
        int valueSize = getTypeSize(info->secondType);
        uint32_t count = 0;
        for ( uint32_t i=0; i!=tab->capacity; ++i ) {
            if ( tab->hashes[i] > HASH_KILLED32 ) {
                bool last = (count == (tab->size-1));
                // key
                char * key = tab->keys + i*keySize;
                beforeTableKey(tab, info, key, info->firstType, count, last);
                if ( cancel ) return;
                walk ( key, info->firstType );
                if ( cancel ) return;
                afterTableKey(tab, info, key, info->firstType, count, last);
                if ( cancel ) return;
                // value
                char * value = tab->data + i*valueSize;
                beforeTableValue(tab, info, value, info->secondType, count, last);
                if ( cancel ) return;
                walk ( value, info->secondType );
                if ( cancel ) return;
                afterTableValue(tab, info, value, info->secondType, count, last);
                if ( cancel ) return;
                // next
                count ++;
            }
        }
    }

    void DataWalker::walk ( char * pa, TypeInfo * info ) {
        if ( pa == nullptr ) {
            Null(info);
        } else if ( info->flags & TypeInfo::flag_ref ) {
            beforeRef(pa,info);
            if ( cancel ) return;
            TypeInfo ti = *info;
            ti.flags &= ~TypeInfo::flag_ref;
            walk(*(char **)pa, &ti);
            if ( cancel ) return;
            afterRef(pa,info);
        } else if ( info->dimSize ) {
            walk_dim(pa, info);
        } else if ( info->type==Type::tArray ) {
            auto arr = (Array *) pa;
            beforeArray(arr, info);
            if ( cancel ) return;
            walk_array(arr->data, getTypeSize(info->firstType), arr->size, info->firstType);
            if ( cancel ) return;
            afterArray(arr, info);
        } else if ( info->type==Type::tTable ) {
            auto tab = (Table *) pa;
            beforeTable(tab, info);
            if ( cancel ) return;
            walk_table(tab, info);
            if ( cancel ) return;
            afterTable(tab, info);
        } else {
            switch ( info->type ) {
                case Type::tBool:       Bool(*((bool *)pa)); break;
                case Type::tInt8:       Int8(*((int8_t *)pa)); break;
                case Type::tUInt8:      UInt8(*((uint8_t *)pa)); break;
                case Type::tInt16:      Int16(*((int16_t *)pa)); break;
                case Type::tUInt16:     UInt16(*((uint16_t *)pa)); break;
                case Type::tInt64:      Int64(*((int64_t *)pa)); break;
                case Type::tUInt64:     UInt64(*((uint64_t *)pa)); break;
                case Type::tString:     String(*((char **)pa)); break;          // TODO: verify!!!
                case Type::tInt:        Int(*((int32_t *)pa)); break;
                case Type::tInt2:       Int2(*((int2 *)pa)); break;
                case Type::tInt3:       Int3(*((int3 *)pa)); break;
                case Type::tInt4:       Int4(*((int4 *)pa)); break;
                case Type::tUInt:       UInt(*((uint32_t *)pa)); break;
                case Type::tUInt2:      UInt2(*((uint2 *)pa)); break;
                case Type::tUInt3:      UInt3(*((uint3 *)pa)); break;
                case Type::tUInt4:      UInt4(*((uint4 *)pa)); break;
                case Type::tFloat:      Float(*((float *)pa)); break;
                case Type::tFloat2:     Float2(*((float2 *)pa)); break;
                case Type::tFloat3:     Float3(*((float3 *)pa)); break;
                case Type::tFloat4:     Float4(*((float4 *)pa)); break;
                case Type::tDouble:     Double(*((double *)pa)); break;
                case Type::tRange:      Range(*((range *)pa)); break;
                case Type::tURange:     URange(*((urange *)pa)); break;
                case Type::tIterator:   WalkIterator((Iterator *)pa); break;     // TODO: verify
                case Type::tEnumeration:
                    WalkEnumeration(*((int32_t *)pa), info->enumType);
                    break;
                case Type::tPointer:
                    beforePtr(pa, info);
                    if ( cancel ) return;
                    if ( info->firstType ) {
                        walk(*(char**)pa, info->firstType);
                        if ( cancel ) return;
                    }
                    afterPtr(pa, info);
                    break;
                case Type::tStructure:  walk_struct(pa, info->structType); break;
                case Type::tBlock:      WalkBlock((Block *)pa); break;
                case Type::tFunction:   WalkFunction((Func *)pa); break;
                case Type::tLambda:     WalkLambda((Lambda *)pa); break;
                case Type::tHandle:
                    beforeHandle(pa, info);
                    if ( cancel ) return;
                    Module::resolveAnnotation(info)->walk(*this, pa);
                    if ( cancel ) return;
                    afterHandle(pa, info);
                    break;
                default:                DAS_ASSERTF(0, "unsupported print type"); break;
            }
        }
    }
}
