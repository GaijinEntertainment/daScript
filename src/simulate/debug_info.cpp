#include "daScript/misc/platform.h"

#include "daScript/simulate/debug_info.h"
#include "daScript/ast/ast.h"
#include "daScript/simulate/data_walker.h"
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

    class HeapAllocationPolicy {
    public:
        char * c_str() {
            if (!data) return nullptr;
            auto sh = ((StringHeader *)data) - 1;
            data[dataSize] = 0;
            sh->hash = 0;
            sh->length = dataSize;
            return data;
        }
        int tellp() const {
            return int(dataSize);
        }
    protected:
        void reserve(int newSize) {
            if (newSize > dataCapacity) {
                int newCapacity = max(dataCapacity * 2, newSize);
                if (data) {
                    char * oldDataBase = data - sizeof(StringHeader);
                    char * newDataBase = (char *)heap->reallocate(oldDataBase, dataCapacity + sizeof(StringHeader) + 1, newCapacity + sizeof(StringHeader) + 1);
                    if (newDataBase == nullptr) {
                        data = nullptr;
                        dataSize = 0;
                        return;
                    } else if (oldDataBase != newDataBase) {
                        data = newDataBase + sizeof(StringHeader);
                        memcpy(data, oldDataBase + sizeof(StringHeader), dataSize);
                    }
                }
                else {
                    data = (char *) heap->allocate(newCapacity + sizeof(StringHeader) + 1);
                    if (!data) {
                        dataSize = 0;
                        return;
                    }
                    data += sizeof(StringHeader);
                }
                dataCapacity = newCapacity;
            }
        }
        void append(const char * s, int l) {
            int newSize = dataSize + l;
            reserve(newSize);
            if (data) {
                memcpy(data + dataSize, s, l);
                dataSize += l;
            }
        }
        char * allocate (int l) {
            reserve(dataSize + l);
            if (!data) return nullptr;
            dataSize += l;
            return data + dataSize - l;
        }
        virtual void output() {}
    protected:
        LinearAllocator * heap = nullptr;
        char *  data = nullptr;
        int     dataSize = 0;
        int     dataCapacity = 0;
    };

    class StringBuilderWriter : public StringWriter<HeapAllocationPolicy> {
    public:
        StringBuilderWriter(LinearAllocator & h) { heap = &h; }
    };

    template <typename Writer>
    struct DebugDataWalker : DataWalker {
        Writer & ss;
        PrintFlags flags;
        DebugDataWalker() = delete;
        DebugDataWalker ( Writer & sss, PrintFlags f ) : ss(sss), flags(f) {}

        // data structures
        virtual void beforeStructure ( char *, StructInfo * info ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << info->name;
            }
        }
        virtual void afterStructure ( char *, StructInfo * ) override {
            ss << "]]";
        }
        virtual void beforeStructureField ( char *, StructInfo *, char *, VarInfo * vi, bool ) override {
            ss << " ";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << vi->name << " = ";
            }
        }
        virtual void afterStructureField ( char *, StructInfo *, char *, VarInfo *, bool last ) override {
            if ( !last )
                ss << ";";
        }
        virtual void beforeArrayElement ( char *, TypeInfo *, char *, uint32_t, bool ) override {
            ss << " ";
        }
        virtual void afterArrayElement ( char *, TypeInfo *, char *, uint32_t, bool last ) override {
            if ( !last )
                ss << ";";
        }
        virtual void beforeTableKey ( Table *, TypeInfo *, char *, TypeInfo *, bool ) override {
            ss << " ";
        }
        virtual void beforeTableValue ( Table *, TypeInfo *, char *, TypeInfo *, bool ) override {
            ss << " : ";
        }
        virtual void afterTableValue ( Table *, TypeInfo *, char *, TypeInfo *, bool last ) override {
            if ( !last )
                ss << ";";
        }
        virtual void beforeDim ( char *, TypeInfo * ti ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << debug_type(ti);
            }
        }
        virtual void afterDim ( char *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void beforeArray ( Array *, TypeInfo * ti ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << debug_type(ti);
            }
        }
        virtual void afterArray ( Array *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void beforeTable ( Table *, TypeInfo * ti ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << debug_type(ti);
            }
        }
        virtual void afterTable ( Table *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void beforeRef ( char * pa, TypeInfo * ti ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "(" << debug_type(ti) << " 0x" << HEX << intptr_t(pa) << DEC << " ref = ";
            }
        }
        virtual void afterRef ( char *, TypeInfo * ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << ")";
            }
        }
        virtual void beforePtr ( char * pa, TypeInfo * ti ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "(" << debug_type(ti) << " 0x" << HEX << intptr_t(pa) << DEC << " ptr = ";
            }
        }
        virtual void afterPtr ( char *, TypeInfo * ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << ")";
            }
        }
        virtual void beforeHandle ( char *, TypeInfo * ti ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "[[" << debug_type(ti) << " ";
            }
        }
        virtual void afterHandle ( char *, TypeInfo * ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "]]";
            }
        }
        // types
        virtual void Null ( TypeInfo * ) override {
            ss << "null";
        }
        virtual void Bool ( bool & b ) override {
            ss << (b ? "true" : "false");
        }
        virtual void Int64 ( int64_t & i ) override {
            ss << i;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "l";
            }
        }
        virtual void UInt64 ( uint64_t & ui ) override {
            ss << "0x" << HEX << ui << DEC;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "ul";
            }
        }
        virtual void String ( char * & str ) override {
            string text = str ? str : "";
            if ( int(flags) & int(PrintFlags::escapeString) ) {
                ss << "\"" << escapeString(text) << "\"";
            } else {
                ss << text;
            }
        }
        virtual void Float ( float & f ) override {
            ss << f;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "f";
            }
        }
        virtual void Int ( int32_t & i ) override {
            ss << i;
        }
        virtual void UInt ( uint32_t & ui ) override {
            ss << "0x" << HEX << ui << DEC;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "u";
            }
        }
        virtual void Int2 ( int2 & i ) override {
            ss << i;
        }
        virtual void Int3 ( int3 & i ) override {
            ss << i;
        }
        virtual void Int4 ( int4 & i ) override {
            ss << i;
        }
        virtual void UInt2 ( uint2 & ui ) override {
            ss << ui;
        }
        virtual void UInt3 ( uint3 & ui ) override {
            ss << ui;
        }
        virtual void UInt4 ( uint4 & ui ) override {
            ss << ui;
        }
        virtual void Float2 ( float2 & fv ) override {
            ss << fv;
        }
        virtual void Float3 ( float3 & fv ) override {
            ss << fv;
        }
        virtual void Float4 ( float4 & fv ) override {
            ss << fv;
        }
        virtual void Range ( range & ra ) override {
            ss << ra;
        }
        virtual void URange ( urange & ra ) override {
            ss << ra;
        }
        virtual void WalkIterator ( struct Iterator * ) override {
            ss << "iterator";
        }
        virtual void WalkBlock ( struct Block * pa ) override {
            ss << "block 0x" << HEX << intptr_t(pa->body) << DEC;
        }
    };

    char * build_string(Context & context, vec4f * values, TypeInfo ** infos, int nArguments, PrintFlags flags) {
        StringBuilderWriter writer(context.heap);
        DebugDataWalker<StringBuilderWriter> walker(writer, flags);
        for ( int i = 0; i!=nArguments; ++i ) {
            walker.walk(values[i], infos[i]);
        }
        return writer.c_str();
    }

    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk((char*)pX,info);
        return ss.str();
    }

    string debug_value ( vec4f value, TypeInfo * info, PrintFlags flags ) {
        TextWriter ss;
        DebugDataWalker<TextWriter> walker(ss,flags);
        walker.walk(value,info);
        return ss.str();
    }

    string debug_type ( TypeInfo * info ) {
        TextWriter stream;
        if ( info->type==Type::tHandle ) {
            stream << info->annotation->name;
        } else if ( info->type==Type::tStructure ) {
            stream << info->structType->name;
        } else if ( info->type==Type::tPointer ) {
            stream << debug_type(info->firstType) << " *";
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
        if ( info->ref )
            stream << " &";
        return stream.str();
    }

    string LineInfo::describe() const {
        TextWriter ss;
        ss << "line " << line << " column " << column;
        return ss.str();
    }
}
