#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_info.h"
#include "daScript/ast/ast.h"

#include "daScript/misc/enums.h"
#include "daScript/misc/arraytype.h"
#include "daScript/misc/vectypes.h"
#include "daScript/misc/rangetype.h"

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
        {   Type::tEnumeration, "enumeration" },
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
        {   Type::tDouble,      "double" },
        {   Type::tRange,       "range" },
        {   Type::tURange,      "urange"},
        {   Type::tBlock,       "block"},
        {   Type::tFunction,    "function"},
        {   Type::tLambda,      "lambda"}
    };

    string das_to_string ( Type t ) {
        return g_typeTable.find(t);
    }

    Type nameToBasicType(const string & name) {
        return g_typeTable.find(name, Type::none);
    }

    int getTypeBaseSize ( Type type ) {
        switch ( type ) {
            case tPointer:      return sizeof(void *);
            case tIterator:     return sizeof(void *);          // Iterator *
            case tHandle:       DAS_ASSERTF(0, "we should not be here. if this happens, iterator was somehow placed on stack. how?");
                                return sizeof(void *);
            case tString:       return sizeof(char *);
            case tBool:         return sizeof(bool);            static_assert(sizeof(bool)==1,"4 byte bool");
            case tInt64:        return sizeof(int64_t);
            case tUInt64:       return sizeof(uint64_t);
            case tEnumeration:  return sizeof(int32_t);
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
            case tDouble:       return sizeof(double);
            case tRange:        return sizeof(range);
            case tURange:       return sizeof(urange);
            case tArray:        return sizeof(Array);
            case tTable:        return sizeof(Table);
            case tStructure:    return 0;
            case tVoid:         return 0;
            case tBlock:        return sizeof(Block);
            case tFunction:     return sizeof(Func);
            case tLambda:       return sizeof(Lambda);
            default:
                DAS_ASSERTF(0, "not implemented. likely new built-intype been added, and support has not been updated.");
                return 0;
        }
    }

    int getTypeBaseAlign ( Type type ) {
        switch ( type ) {
            case tPointer:      return alignof(void *);
            case tIterator:     return alignof(void *);          // Iterator *
            case tHandle:       DAS_ASSERTF(0, "we should not be here. if this happens iterator was somehow placed on stack. how?");
                                return alignof(void *);
            case tString:       return alignof(char *);
            case tBool:         return alignof(bool);            static_assert(alignof(bool)==1,"4 byte bool");
            case tInt64:        return alignof(int64_t);
            case tUInt64:       return alignof(uint64_t);
            case tEnumeration:  return alignof(int32_t);
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
            case tDouble:       return alignof(double);
            case tRange:        return alignof(range);
            case tURange:       return alignof(urange);
            case tArray:        return alignof(Array);
            case tTable:        return alignof(Table);
            case tStructure:    return 1;
            case tVoid:         return 1;
            case tBlock:        return alignof(Block);
            case tFunction:     return alignof(Func);
            case tLambda:       return alignof(Lambda);
            default:
                DAS_ASSERTF(0, "not implemented. likely new built-intype been added, and support has not been updated.");
                return 0;
        }
    }

    int getStructAlign ( StructInfo * info ) {
        int al = 0;
        for ( uint32_t i=0; i!=info->count; ++i ) {
            al = das::max ( al, getTypeAlign(info->fields[i]) );
        }
        return al;
    }

    int getStructSize ( StructInfo * info ) {
        int size = 0;
        for ( uint32_t i=0; i!=info->count; ++i ) {
            int al = getTypeAlign(info->fields[i]) - 1;
            size = (size + al) & ~al;
            size += getTypeSize(info->fields[i]);
        }
        int al = getStructAlign(info) - 1;
        size = (size + al) & ~al;
        return size;
    }

    int getTypeBaseSize ( TypeInfo * info ) {
        if ( info->type==Type::tHandle ) {
            return Module::resolveAnnotation(info)->getSizeOf();
        } else if ( info->type==Type::tStructure ) {
            return getStructSize(info->structType);
        } else {
            return getTypeBaseSize(info->type);
        }
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

    string debug_type ( TypeInfo * info ) {
        TextWriter stream;
        if ( info->type==Type::tHandle ) {
            stream << Module::resolveAnnotation(info)->name;
        } else if ( info->type==Type::tStructure ) {
            stream << info->structType->name;
        } else if ( info->type==Type::tPointer ) {
            stream << debug_type(info->firstType) << " *";
        } else if ( info->type==Type::tEnumeration ) {
            stream << info->enumType->name;
        } else if ( info->type==Type::tArray ) {
            stream << "Array<" << debug_type(info->firstType) << ">";
        } else if ( info->type==Type::tTable ) {
            stream << "Table<" << debug_type(info->firstType) << "," << debug_type(info->secondType) << ">";
        } else if ( info->type==Type::tIterator ) {
            stream << "Iterator<" << debug_type(info->firstType) << ">";
        } else {
            stream << das_to_string(info->type);
        }
        for ( uint32_t i=0; i!=info->dimSize; ++i ) {
            stream << "[" << info->dim[i] << "]";
        }
        if ( info->flags & TypeInfo::flag_ref )
            stream << " &";
        return stream.str();
    }

    string LineInfo::describe() const {
        if ( fileInfo ) {
            TextWriter ss;
            ss << fileInfo->name << ":" << line << ":" << column;
            return ss.str();
        } else {
            return string();
        }
    }

    bool LineInfo::operator < ( const LineInfo & info ) const {
        if ( fileInfo && info.fileInfo && fileInfo->name<info.fileInfo->name ) return true;
        return (line==info.line) ? column<info.column : line<info.line;
    }
    bool LineInfo::operator == ( const LineInfo & info ) const {
        if ( fileInfo && info.fileInfo && fileInfo->name==info.fileInfo->name ) return false;
        return line==info.line && column==info.column;
    }
    bool LineInfo::operator != ( const LineInfo & info ) const {
        if ( fileInfo && info.fileInfo && fileInfo->name!=info.fileInfo->name ) return true;
        return line!=info.line || column!=info.column;
    }

    FileInfoPtr FileAccess::letGoOfFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it == files.end() ) return nullptr;
        return move(it->second);
    }

    FileInfo * FileAccess::setFileInfo ( const string & fileName, FileInfoPtr && info ) {
        if ( files.find(fileName)!=files.end() ) return nullptr;
        files[fileName] = move(info);
        auto ins = files.find(fileName);
        ins->second->name = (char *) ins->first.c_str();
        return ins->second.get();
    }

    FileInfo * FileAccess::getFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it != files.end() ) {
            return it->second.get();
        }
        return getNewFileInfo(fileName);
    }

    string FileAccess::getIncludeFileName ( const string & fileName, const string & incFileName ) const {
        auto np = fileName.find_last_of("\\/");
        if ( np != string::npos ) {
            return fileName.substr(0,np+1) + incFileName;
        } else {
            return incFileName;
        }
    }

    void FileAccess::freeSourceData() {
        for ( auto & fp : files ) {
            fp.second->freeSourceData();
        }
    }
}
