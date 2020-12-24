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
        {   Type::tInt8,        "int8"  },
        {   Type::tUInt8,       "uint8"  },
        {   Type::tInt16,       "int16"  },
        {   Type::tUInt16,      "uint16"  },
        {   Type::tInt64,       "int64"  },
        {   Type::tUInt64,      "uint64"  },
        {   Type::tString,      "string" },
        {   Type::tPointer,     "pointer" },
        {   Type::tEnumeration, "enumeration" },
        {   Type::tEnumeration8,"enumeration8" },
        {   Type::tEnumeration16, "enumeration16" },
        {   Type::tIterator,    "iterator" },
        {   Type::tArray,       "array" },
        {   Type::tTable,       "table" },
        {   Type::tInt,         "int"   },
        {   Type::tInt2,        "int2"  },
        {   Type::tInt3,        "int3"  },
        {   Type::tInt4,        "int4"  },
        {   Type::tUInt,        "uint"  },
        {   Type::tBitfield,    "bitfield"  },
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
        {   Type::tLambda,      "lambda"},
        {   Type::tTuple,       "tuple"},
        {   Type::tVariant,     "variant"},
        {   Type::fakeContext,  "__context"},
    };

    TypeAnnotation * TypeInfo::getAnnotation() const {
        return Module::resolveAnnotation(this);
    }

    string das_to_string ( Type t ) {
        return g_typeTable.find(t);
    }

    Type nameToBasicType(const string & name) {
        return g_typeTable.find(name, Type::none);
    }

    int getTypeBaseSize ( Type type ) {
        switch ( type ) {
            case anyArgument:   return sizeof(vec4f);
            case tPointer:      return sizeof(void *);
            case tIterator:     return sizeof(Sequence);
            case tHandle:       DAS_ASSERTF(0, "we should not be here. if this happens, iterator was somehow placed on stack. how?");
                                return sizeof(void *);
            case tString:       return sizeof(char *);
            case tBool:         return sizeof(bool);            static_assert(sizeof(bool)==1,"4 byte bool");
            case tInt8:         return sizeof(int8_t);
            case tUInt8:        return sizeof(uint8_t);
            case tInt16:        return sizeof(int16_t);
            case tUInt16:       return sizeof(uint16_t);
            case tInt64:        return sizeof(int64_t);
            case tUInt64:       return sizeof(uint64_t);
            case tEnumeration:  return sizeof(int32_t);
            case tEnumeration8: return sizeof(int8_t);
            case tEnumeration16:return sizeof(int16_t);
            case tInt:          return sizeof(int);
            case tInt2:         return sizeof(int2);
            case tInt3:         return sizeof(int3);
            case tInt4:         return sizeof(int4);
            case tUInt:         return sizeof(uint32_t);
            case tBitfield:     return sizeof(uint32_t);
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
            case tVoid:         return 0;
            case tBlock:        return sizeof(Block);
            case tFunction:     return sizeof(Func);
            case tLambda:       return sizeof(Lambda);
            case tStructure:    return 0;
            case tTuple:        return 0;
            case tVariant:      return 0;
            case fakeContext:   return 0;
            case fakeLineInfo:  return 0;
            default:
                DAS_ASSERTF(0, "not implemented. likely new built-intype been added, and support has not been updated.");
                return 0;
        }
    }

    int getTypeBaseAlign ( Type type ) {
        switch ( type ) {
            case anyArgument:   return alignof(vec4f);
            case tPointer:      return alignof(void *);
            case tIterator:     return alignof(Sequence);
            case tHandle:       DAS_ASSERTF(0, "we should not be here. if this happens iterator was somehow placed on stack. how?");
                                return alignof(void *);
            case tString:       return alignof(char *);
            case tBool:         return alignof(bool);            static_assert(alignof(bool)==1,"4 byte bool");
            case tInt8:         return alignof(int8_t);
            case tUInt8:        return alignof(uint8_t);
            case tInt16:        return alignof(int16_t);
            case tUInt16:       return alignof(uint16_t);
            case tInt64:        return alignof(int64_t);
            case tUInt64:       return alignof(uint64_t);
            case tEnumeration:  return alignof(int32_t);
            case tEnumeration8: return alignof(int8_t);
            case tEnumeration16:return alignof(int16_t);
            case tInt:          return alignof(int32_t);
            case tInt2:         return alignof(int2);
            case tInt3:         return alignof(int3);
            case tInt4:         return alignof(int4);
            case tUInt:         return alignof(uint32_t);
            case tBitfield:     return alignof(uint32_t);
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
            case tVoid:         return 1;
            case tBlock:        return alignof(Block);
            case tFunction:     return alignof(Func);
            case tLambda:       return alignof(Lambda);
            case tStructure:    return 1;
            case tTuple:        return 1;
            case tVariant:      return 1;
            case fakeContext:   return 1;
            case fakeLineInfo:  return 1;
            default:
                DAS_ASSERTF(0, "not implemented. likely new built-intype been added, and support has not been updated.");
                return 1;
        }
    }

    int getStructAlign ( StructInfo * info ) {
        int al = 0;
        for ( uint32_t i=0; i!=info->count; ++i ) {
            al = das::max ( al, getTypeAlign(info->fields[i]) );
        }
        return al;
    }

    int getTupleAlign ( TypeInfo * info ) {
        int al = 0;
        for ( uint32_t i=0; i!=info->argCount; ++i ) {
            al = das::max ( al, getTypeAlign(info->argTypes[i]) );
        }
        return al;
    }

    int getTupleSize ( TypeInfo * info ) {
        int size = 0;
        for ( uint32_t i=0; i!=info->argCount; ++i ) {
            int al = getTypeAlign(info->argTypes[i]) - 1;
            size = (size + al) & ~al;
            size += getTypeSize(info->argTypes[i]);
        }
        int al = getTupleAlign(info) - 1;
        size = (size + al) & ~al;
        return size;
    }

    int getVariantAlign ( TypeInfo * info ) {
        int al = getTypeBaseAlign(Type::tInt);
        for ( uint32_t i=0; i!=info->argCount; ++i ) {
            al = das::max ( al, getTypeAlign(info->argTypes[i]) );
        }
        return al;
    }

    int getVariantSize ( TypeInfo * info ) {
        int maxSize = 0;
        int al = getVariantAlign(info) - 1;
        for ( uint32_t i=0; i!=info->argCount; ++i ) {
            int size = (getTypeBaseSize(Type::tInt) + al) & ~al;
            size += getTypeSize(info->argTypes[i]);
            maxSize = das::max(size, maxSize);
        }
        maxSize = (maxSize + al) & ~al;
        return maxSize;
    }

    int getTypeBaseSize ( TypeInfo * info ) {
        if ( info->type==Type::tHandle ) {
            return int(info->getAnnotation()->getSizeOf());
        } else if ( info->type==Type::tStructure ) {
            return info->structType->size;
        } else if ( info->type==Type::tTuple ) {
            return getTupleSize(info);
        } else if ( info->type==Type::tVariant ) {
            return getVariantSize(info);
        } else {
            return getTypeBaseSize(info->type);
        }
    }

    int getTypeBaseAlign ( TypeInfo * info ) {
        if ( info->type==Type::tHandle ) {
            return int(info->getAnnotation()->getAlignOf());
        } else if ( info->type==Type::tStructure ) {
            return getStructAlign(info->structType);
        } else if ( info->type==Type::tTuple ) {
            return getTupleAlign(info);
        } else if ( info->type==Type::tVariant ) {
            return getVariantAlign(info);
        } else {
            return getTypeBaseAlign(info->type);
        }
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

    bool isVoid ( const TypeInfo * THIS ) {
        return (THIS->type==Type::tVoid) && (THIS->dimSize==0);
    }

    bool isValidArgumentType ( TypeInfo * argType, TypeInfo * passType ) {
        // passing non-ref to ref, or passing not the same type
        if ( (argType->isRef() && !passType->isRef()) || !isSameType(argType,passType,RefMatters::no, ConstMatters::no, TemporaryMatters::no,false) ) {
            return false;
        }
        // ref or pointer can only add const
        if ( (argType->isRef() || argType->type==Type::tPointer) && !argType->isConst() && passType->isConst() ) {
            return false;
        }
        // all good
        return true;
    }

    bool isSameType ( const TypeInfo * THIS,
                     const TypeInfo * decl,
                     RefMatters refMatters,
                     ConstMatters constMatters,
                     TemporaryMatters temporaryMatters,
                     bool topLevel ) {
        if ( topLevel && THIS->isRef() ) {
            constMatters = ConstMatters::yes;
        }
        if ( THIS->type != decl->type ) {
            return false;
        }
        if ( THIS->type==Type::tHandle && THIS->getAnnotation()!=decl->getAnnotation() ) {
            return false;
        }
        if ( THIS->type==Type::tStructure && THIS->structType!=decl->structType ) {
            return false;
        }
        if ( THIS->type==Type::tPointer || THIS->type==Type::tIterator ) {
            if ( (THIS->firstType && !isVoid(THIS->firstType))
                && (decl->firstType && !isVoid(decl->firstType))
                && !isSameType(THIS->firstType, decl->firstType, RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, false) ) {
                return false;
            }

        }
        if ( THIS->type==Type::tEnumeration || THIS->type==Type::tEnumeration8 ||
            THIS->type==Type::tEnumeration16 ) {
            if ( THIS->type != decl->type ) {
                return false;
            }
            if ( THIS->enumType && decl->enumType && THIS->enumType!=decl->enumType ) {
                return false;
            }
        }
        if ( THIS->type==Type::tArray ) {
            if ( THIS->firstType && decl->firstType && !isSameType(THIS->firstType, decl->firstType,
                                                                   RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, false) ) {
                return false;
            }
        }
        if ( THIS->type==Type::tTable ) {
            if ( THIS->firstType && decl->firstType && !isSameType(THIS->firstType, decl->firstType,
                                                                   RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, false) ) {
                return false;
            }
            if ( THIS->secondType && decl->secondType && !isSameType(THIS->secondType, decl->secondType,
                                                                     RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, false) ) {
                return false;
            }
        }
        if ( THIS->type==Type::tBlock || THIS->type==Type::tFunction
            || THIS->type==Type::tLambda || THIS->type==Type::tTuple
            || THIS->type==Type::tVariant ) {
            if ( THIS->firstType && decl->firstType && !isSameType(THIS->firstType, decl->firstType,
                                                                   RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes, false) ) {
                return false;
            }
            if ( THIS->firstType || THIS->argCount) {    // if not any block or any function
                if ( THIS->argCount != decl->argCount ) {
                    return false;
                }
                for ( uint32_t i=0; i != THIS->argCount; ++i ) {
                    auto arg = THIS->argTypes[i];
                    auto declArg = decl->argTypes[i];
                    if ( !isSameType(arg,declArg,RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes,true) ) {
                        return false;
                    }
                }
            }
        }
        if ( THIS->dimSize != decl->dimSize ) {
            return false;
        } else if ( THIS->dim ) {
            for ( uint32_t i=0; i!=THIS->dimSize; ++i ) {
                if ( THIS->dim[i] != decl->dim[i] ) {
                    return false;
                }
            }
        }
        if ( refMatters == RefMatters::yes ) {
            if ( THIS->isRef() != decl->isRef() ) {
                return false;
            }
        }
        if ( constMatters == ConstMatters::yes ) {
            if ( THIS->isConst() != decl->isConst() ) {
                return false;
            }
        }
        if ( temporaryMatters == TemporaryMatters::yes ) {
            if ( THIS->isTemp() != decl->isTemp() ) {
                return false;
            }
        }
        return true;
    }

    bool isCompatibleCast ( const StructInfo * THIS, const StructInfo * castS ) {
        if ( castS->count < THIS->count ) {
            return false;
        }
        for ( uint32_t i=0; i!=THIS->count; ++i ) {
            VarInfo * fd = THIS->fields[i];
            VarInfo * cfd = castS->fields[i];
            if ( strcmp(fd->name,cfd->name)!=0 ) {
                return false;
            }
            if ( !isSameType(fd,cfd,RefMatters::yes, ConstMatters::yes, TemporaryMatters::yes,true) ) {
                return false;
            }
        }
        return true;
    }

    string debug_type ( TypeInfo * info ) {
        TextWriter stream;
        if ( info->type==Type::tHandle ) {
            stream << info->getAnnotation()->name;
        } else if ( info->type==Type::tStructure ) {
            stream << info->structType->name;
        } else if ( info->type==Type::tPointer ) {
            stream << debug_type(info->firstType) << " *";
        } else if ( info->type==Type::tEnumeration || info->type==Type::tEnumeration8 || info->type==Type::tEnumeration16 ) {
            stream << info->enumType->name;
        } else if ( info->type==Type::tArray ) {
            stream << "Array<" << debug_type(info->firstType) << ">";
        } else if ( info->type==Type::tTable ) {
            stream << "Table<" << debug_type(info->firstType) << "," << debug_type(info->secondType) << ">";
        } else if ( info->type==Type::tIterator ) {
            stream << "Iterator<" << (info->firstType ? debug_type(info->firstType) : "") << ">";
        } else {
            stream << das_to_string(info->type);
        }
        for ( uint32_t i=0; i!=info->dimSize; ++i ) {
            stream << "[" << info->dim[i] << "]";
        }
        if ( info->flags & TypeInfo::flag_ref )
            stream << " &";
        if ( info->flags & TypeInfo::flag_isTemp ) {
            stream << " #";
        }
        if ( info->flags & TypeInfo::flag_isImplicit ) {
            stream << " !";
        }
        return stream.str();
    }

    string LineInfo::describe(bool fully) const {
        if ( fileInfo ) {
            TextWriter ss;
            ss << fileInfo->name << ":" << line << ":" << column;
            if ( fully ) ss << "-" << last_line << ":" << last_column;
            return ss.str();
        } else {
            return string();
        }
    }

    bool LineInfo::operator < ( const LineInfo & info ) const {
        if ( fileInfo && info.fileInfo && fileInfo->name != info.fileInfo->name)
            return fileInfo->name<info.fileInfo->name;
        return (line==info.line) ? column<info.column : line<info.line;
    }
    bool LineInfo::operator == ( const LineInfo & info ) const {
        if ( ((bool)fileInfo) != ((bool)info.fileInfo))
          return false;
        if ( fileInfo && fileInfo->name!=info.fileInfo->name ) return false;
        return line==info.line && column==info.column;
    }
    bool LineInfo::operator != ( const LineInfo & info ) const {
        return !(*this == info);
    }

    bool LineInfo::inside ( const LineInfo & info ) const {
        if ( fileInfo && fileInfo != info.fileInfo ) return false;
        if ( line < info.line || line > info.last_line ) return false;
        uint32_t from = line==info.line ? info.column : 0;
        uint32_t to = line==info.last_line ? info.last_column : 100500;
        return (column>=from) && (column<=to);
    }

    void FileInfo::reserveProfileData() {
#if DAS_ENABLE_PROFILER
        if ( source ) {
            uint32_t nl = 0;
            auto se = source + sourceLength;
            for ( auto si=source; si!=se; ++si ) {
                if ( *si=='\n' ) {
                    nl ++;
                }
            }
            profileData.reserve(nl + 2);
        }
#endif
    }

    FileInfoPtr FileAccess::letGoOfFileInfo ( const string & fileName ) {
        auto it = files.find(fileName);
        if ( it == files.end() ) return nullptr;
        return move(it->second);
    }

    FileInfo * FileAccess::setFileInfo ( const string & fileName, FileInfoPtr && info ) {
        // TODO: test. for now we need to allow replace
        // if ( files.find(fileName)!=files.end() ) return nullptr;
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
        auto ni = getNewFileInfo(fileName);
        if ( ni ) {
            ni->reserveProfileData();
        }
        return ni;
    }

    string FileAccess::getIncludeFileName ( const string & fileName, const string & incFileName ) const {
        auto np = fileName.find_last_of("\\/");
        if ( np != string::npos ) {
            return fileName.substr(0,np+1) + incFileName;
        } else {
            return incFileName;
        }
    }

    ModuleInfo FileAccess::getModuleInfo ( const string & req, const string & from ) const {
        auto mod = getModuleName(req);
        string modFName = getModuleFileName(req);
        string modFn = getIncludeFileName(from, modFName) + ".das";
        ModuleInfo info;
        info.moduleName = mod;
        info.fileName = modFn;
        return info;
    }

    void FileAccess::freeSourceData() {
        for ( auto & fp : files ) {
            fp.second->freeSourceData();
        }
    }
}
