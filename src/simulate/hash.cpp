#include "daScript/misc/platform.h"


#include "daScript/ast/ast.h"
#include "daScript/simulate/hash.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/data_walker.h"

namespace das
{
    struct HashDataWalker : DataWalker {
        const uint64_t fnv_prime = 1099511628211ul;
        uint64_t fnv_bias = 14695981039346656037ul;
        template <typename TT>
        __forceinline void update ( TT & data ) {
            auto size = sizeof(TT);
            uint8_t * block = (uint8_t *) & data;
            while ( size-- ) {
                fnv_bias = ( fnv_bias ^ *block++ ) * fnv_prime;
            }
        }
        __forceinline void updateString ( char * & str ) {
            if ( !str ) {
                fnv_bias *= fnv_prime;
                return;
            }
            uint8_t * block = (uint8_t *) str;
            while ( *block ) {
                fnv_bias = ( fnv_bias ^ *block++ ) * fnv_prime;
            }
        }
        __forceinline uint64_t getHash ( void ) const {
            if (fnv_bias <= HASH_KILLED64) {
                return fnv_prime;
            }
            return fnv_bias;
        }
    // walker
        HashDataWalker ( Context & ctx ) {
            context = &ctx;
        }
    // data types
        virtual void Bool ( bool & t ) override         { update(t); }
        virtual void Int64 ( int64_t & t ) override     { update(t); }
        virtual void UInt64 ( uint64_t & t ) override   { update(t); }
        virtual void Float ( float & t ) override       { update(t); }
        virtual void Int ( int32_t & t ) override       { update(t); }
        virtual void UInt ( uint32_t & t ) override     { update(t); }
        virtual void Int2 ( int2 & t ) override         { update(t); }
        virtual void Int3 ( int3 & t ) override         { update(t); }
        virtual void Int4 ( int4 & t ) override         { update(t); }
        virtual void UInt2 ( uint2 & t ) override       { update(t); }
        virtual void UInt3 ( uint3 & t ) override       { update(t); }
        virtual void UInt4 ( uint4 & t ) override       { update(t); }
        virtual void Float2 ( float2 & t ) override     { update(t); }
        virtual void Float3 ( float3 & t ) override     { update(t); }
        virtual void Float4 ( float4 & t ) override     { update(t); }
        virtual void Range ( range & t ) override       { update(t); }
        virtual void URange ( urange & t ) override     { update(t); }
        virtual void Range64 ( range64 & t ) override   { update(t); }
        virtual void URange64 ( urange64 & t ) override { update(t); }
        virtual void String ( char * & t ) override     { updateString(t); }
    // unsupported
        virtual void beforeIterator ( Sequence *, TypeInfo * ) override { error("HASH, not expecting iterator"); }
        virtual void WalkBlock ( Block * ) override                     { error("HASH, not expecting block"); }
        virtual void FakeContext ( Context * ) override                 { error("HASH, not expecting context"); }
    };

    uint64_t hash_value ( Context & ctx, void * pX, TypeInfo * info ) {
        HashDataWalker walker(ctx);
        walker.walk((char*)pX,info);
        return walker.getHash();
    }

    uint64_t hash_value ( Context & ctx, vec4f value, TypeInfo * info ) {
        HashDataWalker walker(ctx);
        walker.walk(value,info);
        return walker.getHash();
    }
}

