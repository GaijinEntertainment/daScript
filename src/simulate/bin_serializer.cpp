#include "daScript/misc/platform.h"

#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/simulate.h"
#include "daScript/simulate/hash.h"

namespace das {

    struct BinDataWriter : DataWalker {
        char * bytesAt = nullptr;
        uint32_t bytesAllocated = 0;
        uint32_t bytesWritten = 0;
        uint32_t bytesGrow = 1024;
    // writer
        BinDataWriter ( Context & ctx ) {
            context = &ctx;
            reading = false;
        }
        void write ( void * data, uint32_t size ) {
            if ( bytesWritten + size > bytesAllocated ) {
                uint32_t newSize = max ( bytesAllocated + bytesGrow, bytesWritten + size );
                bytesAt = context->heap.reallocate(bytesAt, bytesAllocated, newSize);
                bytesAllocated = newSize;
            }
            memcpy ( bytesAt + bytesWritten, data, size );
            bytesWritten += size;
        }
        void close () {
            if ( bytesAt ) {
                bytesAt = context->heap.reallocate(bytesAt, bytesAllocated, bytesWritten);
            }
        }
    // data structures
        virtual void beforeStructure ( char *, StructInfo * si ) override {
            uint32_t hash = si->hash;
            write ( &hash, sizeof(uint32_t) );
        }
        virtual void beforeDim ( char *, TypeInfo * ti ) override {
            uint32_t hash = ti->hash;
            write ( &hash, sizeof(uint32_t) );
            uint32_t length = ti->dimSize;
            write ( &length, sizeof(length) );
        }
        virtual void beforeArray ( Array * pa, TypeInfo * ti ) override {
            uint32_t hash = ti->hash;
            write ( &hash, sizeof(uint32_t) );
            uint32_t length = pa->size;
            write ( &length, sizeof(length) );
        }
        virtual void beforeTable ( Table * pa, TypeInfo * ti ) override {
            uint32_t hash = ti->hash;
            write ( &hash, sizeof(uint32_t) );
            uint32_t length = pa->size;
            write ( &length, sizeof(length) );
        }
        virtual void beforePtr ( char *, TypeInfo * ti ) override {
            uint32_t hash = ti->hash;
            write ( &hash, sizeof(uint32_t) );
        }
        virtual void beforeHandle ( char *, TypeInfo * ti ) override {
            uint32_t hash = ti->hash;
            write ( &hash, sizeof(uint32_t) );
        }
    // types
        virtual void Null ( TypeInfo * ) override {
            void * data = nullptr;
            write ( &data, sizeof(data) );
        }
        virtual void Bool ( bool & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Int64 ( int64_t & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void UInt64 ( uint64_t & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void String ( char * & data ) override {
            uint32_t length = stringLength(*context, data);
            write ( &length, sizeof(length) );
            write ( &data, sizeof(data) );
        }
        virtual void Double ( double & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Float ( float & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Int ( int32_t & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void UInt ( uint32_t & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Int2 ( int2 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Int3 ( int3 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Int4 ( int4 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void UInt2 ( uint2 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void UInt3 ( uint3 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void UInt4 ( uint4 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Float2 ( float2 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Float3 ( float3 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Float4 ( float4 & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void Range ( range & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void URange ( urange & data ) override {
            write ( &data, sizeof(data) );
        }
        virtual void WalkIterator ( struct Iterator * ) override {
            context->throw_error("unsupported binary serialization type");
        }
        virtual void WalkBlock ( Block * ) override {
            context->throw_error("unsupported binary serialization type");
        }
        virtual void WalkEnumeration ( int32_t &, EnumInfo * ) override {
            context->throw_error("unsupported binary serialization type");
        }
    };

}
