#pragma once

#include "daScript/simulate/debug_info.h"

#include "daScript/misc/arraytype.h"
#include "daScript/misc/vectypes.h"
#include "daScript/misc/rangetype.h"

namespace das {
    
    // NOTE: parameters here are unreferenced for a reason
    //            the idea is you copy the function defintion, and paste to your code
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)    // unreferenced formal parameter
#elif defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
    
    struct DataWalker {
    // data structures
        virtual void beforeStructure ( char * ps, StructInfo * si ) {}
        virtual void afterStructure ( char * ps, StructInfo * si ) {}
        virtual void beforeStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) {}
        virtual void afterStructureField ( char * ps, StructInfo * si, char * pv, VarInfo * vi, bool last ) {}
        virtual void beforeArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) {}
        virtual void afterArrayData ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti ) {}
        virtual void beforeArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) {}
        virtual void afterArrayElement ( char * pa, TypeInfo * ti, char * pe, uint32_t index, bool last ) {}
        virtual void beforeDim ( char * pa, TypeInfo * ti ) {}
        virtual void afterDim ( char * pa, TypeInfo * ti ) {}
        virtual void beforeArray ( Array * pa, TypeInfo * ti ) {}
        virtual void afterArray ( Array * pa, TypeInfo * ti ) {}
        virtual void beforeTable ( Table * pa, TypeInfo * ti ) {}
        virtual void afterTable ( Table * pa, TypeInfo * ti ) {}
        virtual void beforeRef ( char * pa, TypeInfo * ti ) {}
        virtual void afterRef ( char * pa, TypeInfo * ti ) {}
        virtual void beforePtr ( char * pa, TypeInfo * ti ) {}
        virtual void afterPtr ( char * pa, TypeInfo * ti ) {}
    // types
        virtual void null ( TypeInfo * ti ) {}
        virtual void Bool ( bool & ) {}
        virtual void Int64 ( int64_t & ) {}
        virtual void UInt64 ( uint64_t & ) {}
        virtual void String ( char * & ) {}
        virtual void Float ( float & ) {}
        virtual void Int ( int32_t & ) {}
        virtual void UInt ( uint32_t & ) {}
        virtual void Int2 ( int2 & ) {}
        virtual void Int3 ( int3 & ) {}
        virtual void Int4 ( int4 & ) {}
        virtual void UInt2 ( uint2 & ) {}
        virtual void UInt3 ( uint3 & ) {}
        virtual void UInt4 ( uint4 & ) {}
        virtual void Float2 ( float2 & ) {}
        virtual void Float3 ( float3 & ) {}
        virtual void Float4 ( float4 & ) {}
        virtual void Range ( range & ) {}
        virtual void URange ( urange & ) {}
        virtual void WalkIterator ( struct Iterator * ) {}
        virtual void WalkBlock ( Block * ) {}
        virtual void Handle ( char * pa, TypeAnnotation * ) {}
    // walk
        virtual void walk ( char * pf, TypeInfo * ti );
        virtual void walk ( vec4f f, TypeInfo * ti );
        virtual void walk_struct ( char * ps, StructInfo * si );
        virtual void walk_array ( char * pa, uint32_t stride, uint32_t count, TypeInfo * ti );
        virtual void walk_dim ( char * pa, TypeInfo * ti );
        virtual void walk_table ( Table * tab, TypeInfo * info );
    };
    
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__APPLE__)
#pragma clang diagnostic pop
#endif
}
