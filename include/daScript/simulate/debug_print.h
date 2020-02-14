#pragma once

#include "daScript/simulate/data_walker.h"
#include "daScript/simulate/heap.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/simulate.h"

namespace das {

    class HeapWriterPolicy {
    public:
        char * c_str();
        int tellp() const;
    protected:
        void reserve(int newSize);
        void append(const char * s, int l);
        char * allocate (int l);
        virtual void output() {}
    protected:
        StringAllocator * heap = nullptr;
        char *  data = nullptr;
        int     dataSize = 0;
        int     dataCapacity = 0;
    };

    class StringBuilderWriter : public StringWriter<HeapWriterPolicy> {
    public:
        StringBuilderWriter(StringAllocator & h) { heap = &h; }
    };

    template <typename Writer>
    struct DebugDataWalker : DataWalker {
        Writer & ss;
        PrintFlags flags;
        DebugDataWalker() = delete;
        DebugDataWalker ( Writer & sss, PrintFlags f ) : ss(sss), flags(f) {}
    // data structures
        virtual void beforeStructure ( char * ps, StructInfo * info ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << info->name;
            }
            if ( int(flags) & int(PrintFlags::refAddresses) ) {
                ss << " at 0x" << HEX << intptr_t(ps) << DEC;
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

        virtual void beforeTuple ( char * ps, TypeInfo * ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "tuple";
            }
            if ( int(flags) & int(PrintFlags::refAddresses) ) {
                ss << " at 0x" << HEX << intptr_t(ps) << DEC;
            }
        }
        virtual void afterTuple ( char *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void beforeTupleEntry ( char *, TypeInfo *, char *, TypeInfo *, bool ) override {
            ss << " ";
        }
        virtual void afterTupleEntry ( char *, TypeInfo *, char *, TypeInfo *, bool last ) override {
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
        virtual void beforeTableKey ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t, bool ) override {
            ss << " ";
        }
        virtual void beforeTableValue ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t, bool ) override {
            ss << " : ";
        }
        virtual void afterTableValue ( Table *, TypeInfo *, char *, TypeInfo *, uint32_t, bool last ) override {
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
        virtual void beforeArray ( Array * arr, TypeInfo * ti ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << debug_type(ti);
                if ( arr->shared ) ss << " /*shared*/ ";
            }
        }
        virtual void afterArray ( Array *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void beforeTable ( Table * tab, TypeInfo * ti ) override {
            ss << "[[";
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << debug_type(ti);
                if ( tab->shared ) ss << " /*shared*/ ";
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
        virtual void beforeLambda ( Lambda *, TypeInfo * ti ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << "(" << debug_type(ti) << " ";
            }
        }
        virtual void afterLambda ( Lambda *, TypeInfo * ) override {
            if ( int(flags) & int(PrintFlags::namesAndDimensions) ) {
                ss << ")";
            }
        }
    // types
        virtual void Null ( TypeInfo * ) override {
            ss << "null";
        }
        virtual void Bool ( bool & b ) override {
            ss << (b ? "true" : "false");
        }
        virtual void Int8 ( int8_t & i ) override {
            ss << int32_t(i);
        }
        virtual void UInt8 ( uint8_t & ui ) override {
            ss << "0x" << HEX << uint32_t(ui) << DEC;
        }
        virtual void Int16 ( int16_t & i ) override {
            ss << int32_t(i);
        }
        virtual void UInt16 ( uint16_t & ui ) override {
            ss << "0x" << HEX << uint32_t(ui) << DEC;
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
            if ( int(flags) & int(PrintFlags::refAddresses) ) {
                ss << " /*0x" << HEX << intptr_t(str) << DEC << "*/";
            }
        }
        virtual void Float ( float & f ) override {
            ss << f;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "f";
            }
        }
        virtual void Double ( double & f ) override {
            ss << f;
            if ( int(flags) & int(PrintFlags::typeQualifiers) ) {
                ss << "lf";
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
        virtual void beforeIterator ( Iterator *, TypeInfo * ) override {
            ss << "iterator [[";
        }
        virtual void afterIterator ( Iterator *, TypeInfo * ) override {
            ss << "]]";
        }
        virtual void WalkBlock ( struct Block * pa ) override {
            ss << "block 0x" << HEX << intptr_t(pa->body) << DEC;
        }
        virtual void WalkEnumeration ( int32_t & value, EnumInfo * info ) override {
            for ( uint32_t t=0; t!=info->count; ++t ) {
                if ( value == info->fields[t]->value ) {
                    ss << info->fields[t]->name;
                    return;
                }
            }
            ss << "enum " << value;
        }
        virtual void WalkEnumeration8 ( int8_t & value, EnumInfo * info ) override {
            for ( uint32_t t=0; t!=info->count; ++t ) {
                if ( value == info->fields[t]->value ) {
                    ss << info->fields[t]->name;
                    return;
                }
            }
            ss << "enum " << value;
        }
        virtual void WalkEnumeration16 ( int16_t & value, EnumInfo * info ) override {
            for ( uint32_t t=0; t!=info->count; ++t ) {
                if ( value == info->fields[t]->value ) {
                    ss << info->fields[t]->name;
                    return;
                }
            }
            ss << "enum " << value;
        }
        virtual void WalkEnumeration64 ( int64_t & value, EnumInfo * info ) override {
            for ( uint32_t t=0; t!=info->count; ++t ) {
                if ( value == info->fields[t]->value ) {
                    ss << info->fields[t]->name;
                    return;
                }
            }
            ss << "enum " << value;
        }
        virtual void WalkFunction ( Func * fn ) override {
            if (fn->index) {
                if (context) {
                    auto simFn = context->getFunction(fn->index - 1);
                    ss << simFn->name;
                }
                else {
                    ss << fn->index - 1;
                }
            }   
            else {
                ss << "null";
            }
        }
    };

    string debug_value ( void * pX, TypeInfo * info, PrintFlags flags );
    string debug_value ( vec4f value, TypeInfo * info, PrintFlags flags );
}
