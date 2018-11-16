//
//  debug_info.cpp
//  yzg
//
//  Created by Boris Batkin on 11/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "debug_info.hpp"

#include "enums.h"
#include "cast.h"
#include "runtime_string.hpp"

namespace yzg
{
    Enum<Type> g_typeTable = {
        {   Type::tVoid,        "void"  },
        {   Type::tBool,        "bool"  },
        {   Type::tString,      "string" },
        {   Type::tPointer,     "pointer" },
        {   Type::tInt,         "int"   },
        {   Type::tInt2,        "int2"  },
        {   Type::tInt3,        "int3"  },
        {   Type::tInt4,        "int4"  },
        {   Type::tUInt,        "uint"  },
        {   Type::tUInt2,       "uint2" },
        {   Type::tUInt3,       "uint3" },
        {   Type::tUInt4,       "uint4" },
        {   Type::tFloat,       "float" },
        {   Type::tFloat2,      "float2"},
        {   Type::tFloat3,      "float3"},
        {   Type::tFloat4,      "float4"}
    };
    
    string to_string ( Type t )
    {
        return g_typeTable.find(t);
    }
    
    Type nameToBasicType(const string & name)
    {
        return g_typeTable.find(name, Type::none);
    }
    
    int getTypeBaseSize ( Type type )
    {
        switch ( type ) {
            case tPointer:      return sizeof(void *);
            case tString:       return sizeof(char *);
            case tBool:         return sizeof(bool);
            case tInt:          return sizeof(int);
            case tInt2:         return sizeof(int) * 2;
            case tInt3:         return sizeof(int) * 3;
            case tInt4:         return sizeof(int) * 4;
            case tUInt:         return sizeof(uint);
            case tUInt2:        return sizeof(uint) * 2;
            case tUInt3:        return sizeof(uint) * 3;
            case tUInt4:        return sizeof(uint) * 4;
            case tFloat:        return sizeof(float);
            case tFloat2:       return sizeof(float) * 2;
            case tFloat3:       return sizeof(float) * 3;
            case tFloat4:       return sizeof(float) * 4;
            case tStructure:    return 0;
            default:
                throw runtime_error("not implemented");
                return 0;
        }
    }
    
    int getStructSize ( StructInfo * info )
    {
        int size = 0;
        for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
            size += getTypeSize(info->fields[i]);
        }
        return size;
    }
    
    int getTypeBaseSize ( TypeInfo * info )
    {
        return info->type!=Type::tStructure ? getTypeBaseSize(info->type) : getStructSize(info->structType);
    }
    
    int getDimSize ( TypeInfo * info )
    {
        int size = 1;
        if ( info->dimSize ) {
            for ( uint32_t i=0; i!=info->dimSize; ++i ) {
                size *= info->dim[i];
            }
        }
        return size;
    }
    
    int getTypeSize ( TypeInfo * info )
    {
        return getDimSize(info) * getTypeBaseSize(info);
    }
    
    const char * safe_str ( void * pX ) {
        const char * s = *((char **)pX);
        return s ? s : rts_null;
    }
    
    void debug_structure ( stringstream & ss, char * ps, StructInfo * info );
    void debug_value ( stringstream & ss, void * pX, TypeInfo * info, bool useDim = true );
    
    void debug_structure ( stringstream & ss, char * ps, StructInfo * info )
    {
        char * pf = ps;
        ss << "(";
        for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
            VarInfo * vi = info->fields[i];
            if ( i ) ss << " ";
            ss << "(" << vi->name << " ";
            debug_value(ss, pf, vi);
            ss << ")";
            pf += getTypeSize(vi);
        }
        ss << ")";
    }
    
    void debug_dim_value ( stringstream & ss, void * pX, TypeInfo * info )
    {
        char * pA = (char *) pX;
        int stride = getTypeBaseSize(info);
        int count = getDimSize(info);
        ss << "(";
        for ( int i=0; i!=count; ++i ) {
            if ( i ) ss << " ";
            debug_value(ss, pA, info, false);
            pA += stride;
        }
        ss << ")";
    }
    
    void debug_value ( stringstream & ss, void * pX, TypeInfo * info, bool useDim )
    {
        if ( info->dimSize && useDim ) {
            debug_dim_value(ss, pX, info);
        } else {
            switch ( info->type ) {
                case Type::tBool:       ss << *((bool *)pX); break;
                case Type::tString:     ss << "\"" << escapeString(safe_str(pX)) << "\""; break;
                case Type::tPointer:    ss << "*" << hex << uint64_t(*((uint64_t *)pX)) << dec; break;    // todo: better pointer?
                case Type::tInt:        ss << *((int32_t *)pX); break;
                case Type::tInt2:       ss << *((int2 *)pX); break;
                case Type::tInt3:       ss << *((int3 *)pX); break;
                case Type::tInt4:       ss << *((int4 *)pX); break;
                case Type::tUInt:       ss << *((uint32_t *)pX); break;
                case Type::tUInt2:      ss << *((uint2 *)pX); break;
                case Type::tUInt3:      ss << *((uint3 *)pX); break;
                case Type::tUInt4:      ss << *((uint4 *)pX); break;
                case Type::tFloat:      ss << *((float *)pX); break;
                case Type::tFloat2:     ss << *((float2 *)pX); break;
                case Type::tFloat3:     ss << *((float3 *)pX); break;
                case Type::tFloat4:     ss << *((float4 *)pX);; break;
                case Type::tStructure:  debug_structure(ss, (char *)pX, info->structType); break;
                default:                throw runtime_error("unsupported print type");
            }
        }
    }
    
    void debug_value ( stringstream & ss, __m128 x, TypeInfo * info )
    {
        if ( info->dimSize ) {
            debug_dim_value(ss, cast<void *>::to(x), info);
        } else {
            switch ( info->type ) {
                case Type::tBool:       ss << cast<bool>::to(x); break;
                case Type::tString:     ss << "\"" << escapeString(to_rts(x)) << "\""; break;
                case Type::tPointer:    ss << "*" << hex << uint64_t(cast<void *>::to(x)) << dec; break;    // todo: better pointer?
                case Type::tInt:        ss << cast<int32_t>::to(x); break;
                case Type::tInt2:       ss << cast<int2>::to(x); break;
                case Type::tInt3:       ss << cast<int3>::to(x); break;
                case Type::tInt4:       ss << cast<int4>::to(x); break;
                case Type::tUInt:       ss << cast<uint32_t>::to(x); break;
                case Type::tUInt2:      ss << cast<uint2>::to(x); break;
                case Type::tUInt3:      ss << cast<uint3>::to(x); break;
                case Type::tUInt4:      ss << cast<uint4>::to(x); break;
                case Type::tFloat:      ss << cast<float>::to(x); break;
                case Type::tFloat2:     ss << cast<float2>::to(x); break;
                case Type::tFloat3:     ss << cast<float3>::to(x); break;
                case Type::tFloat4:     ss << cast<float4>::to(x); break;
                case Type::tStructure:  debug_structure(ss, cast<char *>::to(x), info->structType); break;
                default:                throw runtime_error("unsupported print type");
            }
        }
    }
    
    string debug_value ( __m128 x, TypeInfo * info )
    {
        stringstream ss;
        debug_value(ss, x, info);
        return ss.str();
    }
    
    string debug_value ( void * pX, TypeInfo * info )
    {
        stringstream ss;
        debug_value(ss, pX, info);
        return ss.str();
    }
    
    string debug_type ( TypeInfo * info )
    {
        stringstream stream;
        if ( info->type==Type::tStructure || info->type==Type::tPointer ) {
            stream << info->structType->name;
        } else {
            stream << to_string(info->type);
        }
        if ( info->type ==Type::tPointer )
            stream << " *";
        for ( uint32_t i=0; i!=info->dimSize; ++i ) {
            stream << " " << info->dim[i];
        }
        if ( info->ref )
            stream << " &";
        return stream.str();
    }
}
