#include "daScript/misc/platform.h"


#include "daScript/ast/ast.h"
#include "daScript/simulate/hash.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/data_walker.h"

namespace das
{
    struct HashDataWalker : DataWalker {
        const uint32_t fnv_prime = 16777619;
        uint32_t fnv_bias = 2166136261;
        template <typename TT>
        __forceinline void update ( TT & data ) {
            uint32_t size = sizeof(TT);
            uint8_t * block = (uint8_t *) & data;
            while ( size-- ) {
                fnv_bias = ( fnv_bias ^ *block++ ) * fnv_prime;
            }
        }
        __forceinline void updateString ( char * & str ) {
            uint8_t * block = (uint8_t *) str;
            while ( *block ) {
                fnv_bias = ( fnv_bias ^ *block++ ) * fnv_prime;
            }
        }
        __forceinline uint32_t getHash ( void ) const {
            if (fnv_bias <= HASH_KILLED32) {
                return fnv_prime;
            }
            return fnv_bias;
        }
    // walker
        HashDataWalker ( Context & ctx ) {
            context = &ctx;
        }
    // data types
        virtual void Bool ( bool & t )          { update(t); }
        virtual void Int64 ( int64_t & t )      { update(t); }
        virtual void UInt64 ( uint64_t & t )    { update(t); }
        virtual void Float ( float & t )        { update(t); }
        virtual void Int ( int32_t & t )        { update(t); }
        virtual void UInt ( uint32_t & t )      { update(t); }
        virtual void Int2 ( int2 & t )          { update(t); }
        virtual void Int3 ( int3 & t )          { update(t); }
        virtual void Int4 ( int4 & t )          { update(t); }
        virtual void UInt2 ( uint2 & t )        { update(t); }
        virtual void UInt3 ( uint3 & t )        { update(t); }
        virtual void UInt4 ( uint4 & t )        { update(t); }
        virtual void Float2 ( float2 & t )      { update(t); }
        virtual void Float3 ( float3 & t )      { update(t); }
        virtual void Float4 ( float4 & t )      { update(t); }
        virtual void Range ( range & t )        { update(t); }
        virtual void URange ( urange & t )      { update(t); }
        virtual void String ( char * & t )      { updateString(t); }
    // unsupported
        virtual void beforeIterator ( Sequence *, TypeInfo * ) { error("HASH, not expecting iterator"); }
        virtual void WalkBlock ( Block * )              { error("HASH, not expecting block"); }
    };

    uint32_t hash_value ( Context & ctx, void * pX, TypeInfo * info ) {
        HashDataWalker walker(ctx);
        walker.walk((char*)pX,info);
        return walker.getHash();
    }

    uint32_t hash_value ( Context & ctx, vec4f value, TypeInfo * info ) {
        HashDataWalker walker(ctx);
        walker.walk(value,info);
        return walker.getHash();
    }
}

