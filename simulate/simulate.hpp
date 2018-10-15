//
//  simulate.hpp
//  yzg
//
//  Created by Boris Batkin on 10/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef simulate_hpp
#define simulate_hpp

#include <xmmintrin.h>
#include <vector>

namespace yzg
{
    using namespace std;
    
    struct SimNode;
    
    class Context
    {
    public:
        Context();
        ~Context();
        
        inline void * allocate ( int size ) {
            size = (size + 0x0f) & ~0x0f;
            if ( linearAllocator - linearAllocatorBase + size > linearAllocatorSize )
                throw runtime_error("out of linear allocator space");
            void * result = linearAllocator;
            linearAllocator += size;
            return result;
        }
        
        template<typename TT, typename... Params>
        inline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }

    protected:
        int linearAllocatorSize = 1*1024*1024;
        char * linearAllocator = nullptr;
        char * linearAllocatorBase = nullptr;
    };
    
    struct SimNode
    {
        virtual __m128 eval ( Context & ) = 0;
    };
    
    template <typename TT> inline TT cast_to ( __m128 x );
    template <typename TT> inline __m128 cast_from ( TT v );
    template <> inline bool cast_to ( __m128 x )    { return ((int *)&x)[0] != 0; }
    template <> inline __m128 cast_from ( bool x )  { return _mm_set1_epi32(x); }
    template <> inline int cast_to ( __m128 x )     { return ((int *)&x)[0]; }
    template <> inline __m128 cast_from ( int x )   { return _mm_set1_epi32(x); }
    template <> inline float cast_to ( __m128 x )   { return ((float *)&x)[0]; }
    template <> inline __m128 cast_from ( float x ) { return _mm_set_ss(x); }
    
    template <typename TT> inline TT * ptr_cast_to ( __m128 a )     { return ((TT **)&a)[0]; }
    template <typename TT> inline __m128 ptr_cast_from ( TT * p )   { __m128 x; ((TT **)&x)[0] = p; return x; }
    
    // BLOCK
    struct SimNode_Block : SimNode {
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i != total; ++i )
                list[i]->eval(context);
            return _mm_setzero_ps();
        }
        SimNode ** list = nullptr;
        int total = 0;
    };
    
    // IF-THEN
    struct SimNode_IfThen : SimNode {
        SimNode_IfThen ( SimNode * c, SimNode * t ) : cond(c), if_true(t) {}
        virtual __m128 eval ( Context & context ) override {
            if ( cast_to<bool>(cond->eval(context)) ) {
                if_true->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * cond, * if_true;
    };
    
    // IF-THEN-ELSE
    struct SimNode_IfThenElse : SimNode {
        SimNode_IfThenElse ( SimNode * c, SimNode * t, SimNode * f ) : cond(c), if_true(t), if_false(f) {}
        virtual __m128 eval ( Context & context ) override {
            if ( cast_to<bool>(cond->eval(context)) ) {
                if_true->eval(context);
            } else {
                if_false->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * cond, * if_true, * if_false;
    };
    
    // WHILE
    struct SimNode_While : SimNode {
        SimNode_While ( SimNode * c, SimNode * b ) : cond(c), body(b) {}
        virtual __m128 eval ( Context & context ) override {
            while ( cast_to<bool>(cond->eval(context)) ) {
                body->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * cond, * body;
    };
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType
    {
        static inline __m128 Set     ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) =  cast_to<TT>(b); return a; }
        static inline __m128 Equ     ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)==cast_to<TT>(b));  }
        static inline __m128 NotEqu  ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)!=cast_to<TT>(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_CoreType<TT>
    {
        static inline __m128 Unp ( __m128 x ) { return x; }
        static inline __m128 Unm ( __m128 x ) { return cast_from<TT>( -cast_to<TT>(x)); }
        static inline __m128 Add ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)+cast_to<TT>(b)); }
        static inline __m128 Sub ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)-cast_to<TT>(b)); }
        static inline __m128 Div ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)/cast_to<TT>(b)); }
        static inline __m128 Mul ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)*cast_to<TT>(b)); }
        static inline __m128 LessEqu ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)<=cast_to<TT>(b));  }
        static inline __m128 GtEqu   ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)>=cast_to<TT>(b));  }
        static inline __m128 Less    ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)<cast_to<TT>(b));  }
        static inline __m128 Gt      ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)>cast_to<TT>(b));  }
        static inline __m128 SetAdd  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) += cast_to<TT>(b); return a; }
        static inline __m128 SetSub  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) -= cast_to<TT>(b); return a; }
        static inline __m128 SetDiv  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) *= cast_to<TT>(b); return a; }
        static inline __m128 SetMul  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) /= cast_to<TT>(b); return a; }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool>
    {
        static inline __m128 BoolNot ( __m128 x ) { return cast_from<bool>( !cast_to<bool>(x)); }
        static inline __m128 BoolAnd ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) && cast_to<bool>(b)); }
        static inline __m128 BoolOr  ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) || cast_to<bool>(b)); }
        static inline __m128 BoolXor ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) != cast_to<bool>(b)); }
        static inline __m128 SetBoolAnd  ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa && cast_to<bool>(b); return a; }
        static inline __m128 SetBoolOr   ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa || cast_to<bool>(b); return a; }
        static inline __m128 SetBoolXor  ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa != cast_to<bool>(b); return a; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT>
    {
        static inline __m128 BinNot ( __m128 x ) { return cast_from<TT>( ~cast_to<TT>(x)); }
        static inline __m128 BinAnd ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) & cast_to<TT>(b)); }
        static inline __m128 BinOr  ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) | cast_to<TT>(b)); }
        static inline __m128 BinXor ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) ^ cast_to<TT>(b)); }
        static inline __m128 SetBinAnd ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) &= cast_to<TT>(b); return a; }
        static inline __m128 SetBinOr  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) |= cast_to<TT>(b); return a; }
        static inline __m128 SetBinXor ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) ^= cast_to<TT>(b); return a; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int>
    {
    };
    
    struct SimPolicy_Uint : SimPolicy_Bin<unsigned int>
    {
    };
    
    struct SimPolicy_Float : SimPolicy_Type<float>
    {
        // TODO: verify optimization
        static inline __m128 Add ( __m128 a, __m128 b ) { return _mm_add_ss(a,b); }
        static inline __m128 Sub ( __m128 a, __m128 b ) { return _mm_sub_ss(a,b); }
        static inline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_ss(a,b); }
        static inline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_ss(a,b); }

    };

    // op1 policies
    
    struct SimNode_Op1 : SimNode {
        SimNode * x = nullptr;
    };
    
#define DEFINE_OP1_POLICY(CALL)                                         \
    template <typename SimPolicy>                                       \
    struct Sim_##CALL : SimNode_Op1 {                                   \
        virtual __m128 eval ( Context & context ) override {            \
            return SimPolicy::CALL ( x->eval(context) );                \
        }                                                               \
    };
    
    DEFINE_OP1_POLICY(Unp);
    DEFINE_OP1_POLICY(Unm);
    DEFINE_OP1_POLICY(BoolNot);
    DEFINE_OP1_POLICY(BinNot);
    
    // op2 policies
    
    struct SimNode_Op2 : SimNode {
        SimNode * l = nullptr;
        SimNode * r = nullptr;
    };
    
#define DEFINE_OP2_POLICY(CALL)                                             \
    template <typename SimPolicy>                                           \
    struct Sim_##CALL : SimNode_Op2 {                                       \
        virtual __m128 eval ( Context & context ) override {                \
            return SimPolicy::CALL ( l->eval(context), r->eval(context) );  \
        }                                                                   \
    };
    
    DEFINE_OP2_POLICY(Equ);
    DEFINE_OP2_POLICY(NotEqu);
    DEFINE_OP2_POLICY(LessEqu);
    DEFINE_OP2_POLICY(GtEqu);
    DEFINE_OP2_POLICY(Less);
    DEFINE_OP2_POLICY(Gt);
    DEFINE_OP2_POLICY(SetEqu);
    DEFINE_OP2_POLICY(Add);
    DEFINE_OP2_POLICY(Sub);
    DEFINE_OP2_POLICY(Div);
    DEFINE_OP2_POLICY(Mul);
    DEFINE_OP2_POLICY(BoolAnd);
    DEFINE_OP2_POLICY(BoolOr);
    DEFINE_OP2_POLICY(BoolXor);
    DEFINE_OP2_POLICY(BinAnd);
    DEFINE_OP2_POLICY(BinOr);
    DEFINE_OP2_POLICY(BinXor);
    DEFINE_OP2_POLICY(Set);
    DEFINE_OP2_POLICY(SetBoolAnd);
    DEFINE_OP2_POLICY(SetBoolOr);
    DEFINE_OP2_POLICY(SetBoolXor);
    DEFINE_OP2_POLICY(SetBinAnd);
    DEFINE_OP2_POLICY(SetBinOr);
    DEFINE_OP2_POLICY(SetBinXor);
    DEFINE_OP2_POLICY(SetAdd);
    DEFINE_OP2_POLICY(SetSub);
    DEFINE_OP2_POLICY(SetDiv);
    DEFINE_OP2_POLICY(SetMul);
}

#endif /* simulate_hpp */
