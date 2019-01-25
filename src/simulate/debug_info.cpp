#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_info.h"

#include "daScript/simulate/hash.h"
#include "daScript/misc/enums.h"
#include "daScript/simulate/cast.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/misc/arraytype.h"

namespace das
{
    Enum<Type> g_typeTable = {
        {   Type::autoinfer,    "auto"  },
        {   Type::alias,        "alias"  },
        {   Type::anyArgument,  "any"  },
        {   Type::tVoid,        "void"  },
        {   Type::tBool,        "bool"  },
        {   Type::tInt64,       "int64"  },
        {   Type::tUInt64,      "uint64"  },
        {   Type::tString,      "string" },
        {   Type::tPointer,     "pointer" },
        {   Type::tIterator,    "iterator" },
        {   Type::tArray,       "array" },
        {   Type::tTable,       "table" },
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
        {   Type::tFloat4,      "float4"},
        {   Type::tRange,       "range" },
        {   Type::tURange,      "urange"}
    };
    
    string to_string ( Type t ) {
        return g_typeTable.find(t);
    }
    
    Type nameToBasicType(const string & name) {
        return g_typeTable.find(name, Type::none);
    }
    
    int getTypeBaseSize ( Type type ) {
        switch ( type ) {
            case tPointer:      return sizeof(void *);
            case tIterator:     return sizeof(void *);          // Iterator *
            case tHandle:       assert(0 && "we should not be here"); return sizeof(void *);
            case tString:       return sizeof(char *);
            case tBool:         return sizeof(bool);            static_assert(sizeof(bool)==1,"4 byte bool");
            case tInt64:        return sizeof(int64_t);
            case tUInt64:       return sizeof(uint64_t);
            case tInt:          return sizeof(int);
            case tInt2:         return sizeof(int2);
            case tInt3:         return sizeof(int3);
            case tInt4:         return sizeof(int4);
            case tUInt:         return sizeof(uint32_t);
            case tUInt2:        return sizeof(uint2);
            case tUInt3:        return sizeof(uint3);
            case tUInt4:        return sizeof(uint4);
            case tFloat:        return sizeof(float);
            case tFloat2:       return sizeof(float2);
            case tFloat3:       return sizeof(float3);
            case tFloat4:       return sizeof(float4);
            case tRange:        return sizeof(range);
            case tURange:       return sizeof(urange);
            case tArray:        return sizeof(Array);
            case tTable:        return sizeof(Table);
            case tStructure:    return 0;
            case tVoid:         return 0;
            case tBlock:        return sizeof(Block);
            default:
                assert(0 && "not implemented");
                return 0;
        }
    }
    
    int getTypeBaseAlign ( Type type ) {
        switch ( type ) {
            case tPointer:      return alignof(void *);
            case tIterator:     return alignof(void *);          // Iterator *
            case tHandle:       assert(0 && "we should not be here"); return alignof(void *);
            case tString:       return alignof(char *);
            case tBool:         return alignof(bool);            static_assert(alignof(bool)==1,"4 byte bool");
            case tInt64:        return alignof(int64_t);
            case tUInt64:       return alignof(uint64_t);
            case tInt:          return alignof(int32_t);
            case tInt2:         return alignof(int2);
            case tInt3:         return alignof(int3);
            case tInt4:         return alignof(int4);
            case tUInt:         return alignof(uint32_t);
            case tUInt2:        return alignof(uint2);
            case tUInt3:        return alignof(uint3);
            case tUInt4:        return alignof(uint4);
            case tFloat:        return alignof(float);
            case tFloat2:       return alignof(float2);
            case tFloat3:       return alignof(float3);
            case tFloat4:       return alignof(float4);
            case tRange:        return alignof(range);
            case tURange:       return alignof(urange);
            case tArray:        return alignof(Array);
            case tTable:        return alignof(Table);
            case tStructure:    return 1;
            case tVoid:         return 1;
            case tBlock:        return alignof(Block);
            default:
                assert(0 && "not implemented");
                return 0;
        }
    }
    
    int getStructAlign ( StructInfo * info ) {
        int al = 0;
        for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
            al = max ( al, getTypeAlign(info->fields[i]) );
        }
        return al;
    }
    
    int getStructSize ( StructInfo * info ) {
        int size = 0;
        for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
            int al = getTypeAlign(info->fields[i]) - 1;
            size = (size + al) & ~al;
            size += getTypeSize(info->fields[i]);
        }
        int al = getStructAlign(info) - 1;
        size = (size + al) & ~al;
        return size;
    }
    
    int getTypeBaseSize ( TypeInfo * info ) {
        return info->type!=Type::tStructure ? getTypeBaseSize(info->type) : getStructSize(info->structType);
    }
    
    int getTypeBaseAlign ( TypeInfo * info ) {
        return info->type!=Type::tStructure ? getTypeBaseAlign(info->type) : getStructAlign(info->structType);
    }
    
    int getDimSize ( TypeInfo * info ) {
        int size = 1;
        if ( info->dimSize ) {
            for ( uint32_t i=0; i!=info->dimSize; ++i ) {
                size *= info->dim[i];
            }
        }
        return size;
    }
    
    int getTypeSize ( TypeInfo * info ) {
        return getDimSize(info) * getTypeBaseSize(info);
    }
    
    int getTypeAlign ( TypeInfo * info ) {
        return getTypeBaseAlign(info);
    }
    
    void debugType ( TypeAnnotation *, stringstream & , void *, PrintFlags );
    void debugType ( TypeAnnotation *, stringstream &, vec4f, PrintFlags );
    void debug_structure ( stringstream & ss, char * ps, StructInfo * info, PrintFlags flags );
    void debug_value ( stringstream & ss, void * pX, TypeInfo * info, PrintFlags flags );
    
    void debug_structure ( stringstream & ss, char * ps, StructInfo * info, PrintFlags flags ) {
        ss << "(";
        for ( uint32_t i=0; i!=info->fieldsSize; ++i ) {
            VarInfo * vi = info->fields[i];
            char * pf = ps + vi->offset;
            if ( i ) ss << " ";
            ss << "(" << vi->name;
            ss << " ";
            debug_value(ss, pf, vi, flags);
            ss << ")";
        }
        ss << ")";
    }
    
    void debug_array_value ( stringstream & ss, void * pX, int stride, int count, TypeInfo * info, PrintFlags flags ) {
        char * pA = (char *) pX;
        ss << "(";
        if ( int(flags) & int(PrintFlags::debugger) ) {
            ss << "[size=" << count << ",stride=" << stride << "] ";
        }
        for ( int i=0; i!=count; ++i ) {
            if ( i ) ss << " ";
            debug_value(ss, pA, info, flags);
            pA += stride;
        }
        ss << ")";
    }
    
    void debug_dim_value ( stringstream & ss, void * pX, TypeInfo * info, PrintFlags flags ) {
        TypeInfo copyInfo = *info;
        assert(copyInfo.dimSize);
        copyInfo.dimSize --;
        int stride = getTypeBaseSize(info);
        int count = getDimSize(info);
        debug_array_value(ss, pX, stride, count, &copyInfo, flags);
    }
    
    void debug_value ( stringstream & ss, void * pX, TypeInfo * info, PrintFlags flags ) {
        if ( pX == nullptr ) {
            ss << "null";
            return;
        }
        if ( info->ref ) {
            TypeInfo ti = *info;
            ti.ref = false;
            debug_value(ss, *(void **)pX, &ti, flags);
        } else if ( info->dimSize ) {
            debug_dim_value(ss, pX, info, flags);
        } else if ( info->type==Type::tArray ) {
            auto arr = (Array *) pX;
            debug_array_value(ss, arr->data, getTypeSize(info->firstType), arr->size, info->firstType, flags);
        } else {
            switch ( info->type ) {
                case Type::tBool:       ss << (*((bool *)pX) ? "true" : "false"); break;
                case Type::tInt64:      ss << *((int64_t *)pX); break;
                case Type::tUInt64:     ss << "0x" << hex << *((uint64_t *)pX) << dec; break;
                case Type::tString:
                    if ( int(flags) & int(PrintFlags::escapeString) ) {
                        ss << "\"" << escapeString(safe_str(pX)) << "\"";
                    } else {
                        ss << safe_str(pX);
                    }
                    break;
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
                case Type::tRange:      ss << *((range *)pX); break;
                case Type::tURange:     ss << *((urange *)pX); break;
                case Type::tIterator:   ss << "iterator"; break;
                case Type::tPointer:    ss << "*" << hex << intptr_t(pX) << dec;
                                        if ( info->firstType ) {
                                            ss << " -> (";
                                            debug_value(ss, *(void**)pX, info->firstType, flags);
                                            ss << ")";
                                        }
                                        break;
                case Type::tStructure:  debug_structure(ss, (char *)pX, info->structType, flags); break;
                case Type::tBlock:      ss << "block " << hex << intptr_t(((Block *)pX)->body) << dec;
                case Type::tHandle:     debugType(info->annotation, ss, pX, flags); break;
                default:                assert(0 && "unsupported print type"); break;
            }
        }
    }

    void debug_table_value (  stringstream & ss, Table & tab, TypeInfo * info, PrintFlags flags ) {
        ss << "([" << tab.size << " of " << tab.capacity << "] ";
        bool first = true;
        int keySize = getTypeSize(info->firstType);
        int valueSize = getTypeSize(info->secondType);
        for ( uint32_t i=0; i!=tab.capacity; ++i ) {
			if ( tab.hashes[i] > HASH_KILLED32 ) {
                if ( !first ) ss << " "; first = false;
                ss << "("; // ss << "(@ " << i << " ";
                debug_value ( ss, tab.keys + i*keySize, info->firstType, flags );
                ss << " -> ";
                debug_value ( ss, tab.data + i*valueSize, info->secondType, flags );
                ss << ")";
            }
        }
        ss << ")";
    }
    
    void debug_value ( stringstream & ss, vec4f x, TypeInfo * info, PrintFlags flags ) {
        if ( info->ref ) {
            TypeInfo ti = *info;
            ti.ref = false;
            debug_value(ss, cast<void *>::to(x), &ti, flags);
        } else if ( info->dimSize ) {
            debug_dim_value(ss, cast<void *>::to(x), info, flags);
        } else if ( info->type==Type::tArray ) {
            auto arr = cast<Array *>::to(x);
            debug_array_value(ss, arr->data, getTypeSize(info->firstType), arr->size, info->firstType, flags);
        } else if ( info->type==Type::tTable ) {
            auto tab = cast<Table *>::to(x);
            debug_table_value(ss, *tab, info, flags);
        } else {
            switch ( info->type ) {
                case Type::tBool:       ss << (cast<bool>::to(x) ? "true" : "false"); break;
                case Type::tInt64:      ss << cast<int64_t>::to(x); break;
                case Type::tUInt64:     ss << "0x" << hex << cast<uint64_t>::to(x) << dec; break;
                case Type::tString:
                    if ( int(flags) & int(PrintFlags::escapeString) ) {
                        ss << "\"" << escapeString(to_rts(x)) << "\"";
                    } else {
                        ss << to_rts(x);
                    }
                    break;
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
                case Type::tRange:      ss << cast<range>::to(x); break;
                case Type::tURange:     ss << cast<urange>::to(x); break;
                case Type::tIterator:   ss << "iterator"; break;
                case Type::tPointer:    ss << "*" << hex << intptr_t(cast<void *>::to(x)) << dec << " ";
                                        if ( info->firstType ) {
                                            ss << " -> (";
                                            debug_value(ss, cast<void *>::to(x), info->firstType, flags);
                                            ss << ")";
                                        }
                                        break;
                case Type::tStructure:  debug_structure(ss, cast<char *>::to(x), info->structType, flags); break;
                case Type::tVoid:       ss << "void"; break;
                case Type::tBlock:      ss << "block"; break;
                case Type::tHandle:     debugType(info->annotation, ss, x, flags); break;
                case Type::fakeContext: ss << "__context__"; break;
                default:                assert(0 && "unsupported print type"); break;
            }
        }
    }
    
    string debug_value ( vec4f x, TypeInfo * info, PrintFlags flags ) {
        stringstream ss;
        debug_value(ss, x, info, flags);
        return ss.str();
    }
    
    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags ) {
        stringstream ss;
        debug_value(ss, pX, info, flags);
        return ss.str();
    }
    
    string debug_type ( TypeInfo * info ) {
        stringstream stream;
        if ( info->type==Type::tHandle ) {
            stream << "handle"; // TODO: << info->annotation->name << ">";
        } else if ( info->type==Type::tStructure ) {
            stream << info->structType->name;
        } else if ( info->type==Type::tPointer ) {
            stream << debug_type(info->firstType) << " *";
        } else if ( info->type==Type::tArray ) {
            stream << "array <" << debug_type(info->firstType) << ">";
        } else if ( info->type==Type::tTable ) {
            stream << "table <" << debug_type(info->firstType) << "," << debug_type(info->secondType) << ">";
        } else if ( info->type==Type::tIterator ) {
            stream << "iterator <" << debug_type(info->firstType) << ">";
        } else {
            stream << to_string(info->type);
        }
        for ( uint32_t i=0; i!=info->dimSize; ++i ) {
            stream << " " << info->dim[i];
        }
        if ( info->ref )
            stream << " &";
        return stream.str();
    }
    
    string LineInfo::describe() const {
        stringstream ss;
        ss << "line " << line << " column " << column;
        return ss.str();
    }
}
