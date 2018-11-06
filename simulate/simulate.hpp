//
//  simulate.hpp
//  yzg
//
//  Created by Boris Batkin on 10/14/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#ifndef simulate_hpp
#define simulate_hpp

#include "platform.h"
#include "vectypes.h"

#include <xmmintrin.h>
#include <vector>
#include <string>
#include <iostream>

namespace yzg
{
    using namespace std;
    
    struct Context;
    struct SimNode;
    
    struct GlobalVariable
    {
        char *  name;
        __m128  value;
        size_t  size;
    };
    
    struct SimFunction
    {
        char *      name;
        SimNode *   code;
        size_t      stackSize;
    };
    
    struct SimNode
    {
        virtual __m128 eval ( Context & ) = 0;
    };
    
    class Context
    {
        friend class Program;
        friend class SimNode_GetGlobal;
        friend class SimNode_GetLocal;
        friend class SimNode_GetArgument;
        friend class SimNode_TryCatch;
    public:
        Context();
        ~Context();
        
        __forceinline void * allocate ( size_t size ) {
            size = (size + 0x0f) & ~0x0f;
            if ( linearAllocator - linearAllocatorBase + size > linearAllocatorSize )
                throw runtime_error("out of linear allocator space");
            void * result = linearAllocator;
            linearAllocator += size;
            return result;
        }
        
        __forceinline char * allocateName ( const string & name ) {
            auto size = name.length() + 1;
            char * str = (char *) allocate(size+1);
            memcpy ( str, name.c_str(), size );
            return str;
        }
        
        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }
        
        __forceinline __m128 call ( int fnIndex, __m128 * args ) {
            auto & fn = functions[fnIndex];
            // PUSH
            __m128 * pushArg = arguments;
            char * pushStack = stackTop;
            arguments = args;
            stackTop += fn.stackSize;   // TODO: check for stack overflow
            // CALL
            __m128 result = fn.code->eval(*this);
            // POP
            stackTop = pushStack;
            arguments = pushArg;
            return result;
        }
        
        __forceinline __m128 getVariable ( int index ) const { return globalVariables[index].value; }
        
        int findFunction ( const char * name ) const;
        int findVariable ( const char * name ) const;
        
    protected:
        int linearAllocatorSize = 1*1024*1024;
        char * linearAllocator = nullptr;
        char * linearAllocatorBase = nullptr;
        char * linearAllocatorExecuteBase = nullptr;
        GlobalVariable * globalVariables = nullptr;
        SimFunction * functions = nullptr;
        char * stackTop = nullptr;
        int stackSize = 16*1024;
        __m128 * arguments = nullptr;
        int totalVariables = 0;
        int totalFunctions = 0;
    };
    
    template <typename TT> __forceinline TT cast_to ( __m128 x );
    template <typename TT> __forceinline __m128 cast_from ( TT v );
    template <> __forceinline bool cast_to ( __m128 x )    { return *((int *)&x) != 0; }
    template <> __forceinline __m128 cast_from ( bool x )  { return _mm_set1_epi32(x); }
    template <> __forceinline int64_t cast_to ( __m128 x )     { return *((int64_t *)&x); }
    template <> __forceinline __m128 cast_from ( int64_t x )   { __m128 a; *((int64_t *)&a) = x; return a; }
    template <> __forceinline uint64_t cast_to ( __m128 x )    { return *((uint64_t *)&x); }
    template <> __forceinline __m128 cast_from ( uint64_t x )  { __m128 a; *((uint64_t *)&a) = x; return a; }
    template <> __forceinline float cast_to ( __m128 x )   { return *((float *)&x); }
    template <> __forceinline __m128 cast_from ( float x ) { return _mm_set_ss(x); }
    template <> __forceinline float2 cast_to ( __m128 x ) { return *((float2 *)&x); }
    template <> __forceinline __m128 cast_from ( float2 x ) { return _mm_loadu_ps(&x.x); }
    template <> __forceinline float3 cast_to ( __m128 x ) { return *((float3 *)&x); }
    template <> __forceinline __m128 cast_from ( float3 x ) {  return _mm_loadu_ps(&x.x); }
    template <> __forceinline float4 cast_to ( __m128 x ) { return *((float4 *)&x); }
    template <> __forceinline __m128 cast_from ( float4 x ) { return _mm_loadu_ps(&x.x); }

    template <typename TT> __forceinline TT * ptr_cast_to ( __m128 a )     { return ((TT **)&a)[0]; }
    template <typename TT> __forceinline __m128 ptr_cast_from ( TT * p )   { __m128 x; ((TT **)&x)[0] = p; return x; }
    
    // field
    struct SimNode_Field : SimNode {
        SimNode_Field ( SimNode * rv, uint64_t of ) : rvalue(rv), offset(of) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 rv = rvalue->eval(context);
            char * prv = ptr_cast_to<char>(rv);
            if ( !prv )
                throw runtime_error("dereferencing nil pointer");
            return ptr_cast_from( prv + offset );
        }
        SimNode *   rvalue;
        uint64_t    offset;
    };
    
    // AT (INDEX)
    struct SimNode_At : SimNode {
        virtual __m128 eval ( Context & context ) override {
            char * pValue = ptr_cast_to<char>(rvalue->eval(context));
            uint64_t idx = cast_to<uint64_t>(index->eval(context));
            if ( idx >= range )
                throw runtime_error("index out of range");
            return ptr_cast_from<char>(pValue + idx*stride);    // TODO: add range check
            
        }
        SimNode_At ( SimNode * rv, SimNode * idx, uint64_t strd, uint64_t rng )
            : rvalue(rv), index(idx), stride(strd), range(rng) {}
        SimNode * rvalue, * index;
        uint64_t  stride, range;
    };
    
    // FUNCTION CALL
    struct SimNode_Call : SimNode {
        __forceinline void evalArgs ( Context & context ) {
            for ( int i=0; i!=nArguments; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            return context.call(fnIndex, argValues);
        }
        int fnIndex;
        int nArguments;
        SimNode ** arguments;
        __m128 * argValues;
    };
    
    // CAST
    template <typename CastTo, typename CastFrom>
    struct SimNode_Cast : SimNode_Call {
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            CastTo value = (CastTo) cast_to<CastFrom>(res);
            return cast_from(value);
        }
    };
    
    // VECTOR C-TOR
    template <int vecS>
    struct SimNode_VecCtor : SimNode_Call {
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            if ( vecS==2 )
                return _mm_setr_ps(cast_to<float>(argValues[0]),cast_to<float>(argValues[1]),
                                   0.0f,0.0f);
            else if ( vecS==3 )
                return _mm_setr_ps(cast_to<float>(argValues[0]),cast_to<float>(argValues[1]),
                                   cast_to<float>(argValues[2]),0.0f);
            else if ( vecS==4 )
                return _mm_setr_ps(cast_to<float>(argValues[0]),cast_to<float>(argValues[1]),
                                   cast_to<float>(argValues[2]),cast_to<float>(argValues[3]));
            else
                return _mm_setzero_ps();
        }
    };
    
    // "DEBUG"
    template <typename TT>
    struct SimNode_Debug : SimNode_Call {
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            auto value = cast_to<TT>(res);
            cout << "debug: " << value << "\n";
            return res;
        }
    };
    
    // LOCAL VARIABLE "GET"
    struct SimNode_GetLocal : SimNode {
        SimNode_GetLocal(size_t sp) : stackTop(sp) {}
        virtual __m128 eval ( Context & context ) override {
            return ptr_cast_from(context.stackTop + stackTop);
        }
        size_t stackTop;
    };
    
    // ARGUMENT VARIABLE "GET"
    struct SimNode_GetArgument : SimNode {
        SimNode_GetArgument ( int i ) : index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.arguments[index];
        }
        int index;
    };
    
    // GLOBAL VARIABLE "GET"
    struct SimNode_GetGlobal : SimNode {
        SimNode_GetGlobal ( int i ) : index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.globalVariables[index].value;
        }
        int index;
    };
    
    // R2L
    template <typename TT>
    struct SimNode_R2L : SimNode {
        SimNode_R2L ( SimNode * s ) : subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ptr = subexpr->eval(context);
            TT * pR = ptr_cast_to<TT>(ptr);
            if ( !pR )
                throw runtime_error("dereferencing nil pointer");
            return cast_from<TT>(*pR);
        }
        SimNode * subexpr;
    };
    
    // CONST-VALUE
    template <typename TT>
    struct SimNode_ConstValue : SimNode {
        SimNode_ConstValue(TT c) : value(cast_from<TT>(c)) {}
        virtual __m128 eval ( Context & context ) override {
            return value;
        }
        __m128 value;
    };
    
    // COPY R-VALUE
    struct SimNode_CopyRValue : SimNode {
        SimNode_CopyRValue(SimNode * ll, SimNode * rr, int sz) : l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            __m128 rr = r->eval(context);
            auto pl = ptr_cast_to<void>(ll);
            auto pr = ptr_cast_to<void>(rr);
            memcpy ( pl, pr, size );
            return ll;
        }
        SimNode * l, * r;
        int size;
    };
    
    // COPY L-VALUE
    template <typename TT>
    struct SimNode_CopyLValue : SimNode {
        SimNode_CopyLValue(SimNode * ll, SimNode * rr) : l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            __m128 rr = r->eval(context);
            TT * pl = ptr_cast_to<TT>(ll);
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return ll;
        }
        SimNode * l, * r;
    };
    
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
    
    // LET
    struct SimNode_Let : SimNode_Block {
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i != total; ++i )
                list[i]->eval(context);
            return subexpr->eval(context);
        }
        SimNode * subexpr = nullptr;
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
    
    // TRY-CATCH
    struct SimNode_TryCatch : SimNode {
        SimNode_TryCatch ( SimNode * t, SimNode * c ) : try_this(t), catch_that(c) {}
        virtual __m128 eval ( Context & context ) override {
            try {
                auto sp = context.stackTop;
                auto ar = context.arguments;
                try_this->eval(context);
                context.arguments = ar;
                context.stackTop = sp;
            } catch ( const runtime_error & err ) {
                catch_that->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * try_this, * catch_that;
    };
    
    // FOREACH
    struct SimNode_Foreach : SimNode {
        SimNode_Foreach ( SimNode * h, SimNode * i, SimNode * b, int sz, int st )
            : head(h), iter(i), body(b), size(sz), stride(st) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph = ptr_cast_to<char>(head->eval(context));
            __m128 * pi = ptr_cast_to<__m128>(iter->eval(context));
            for ( int i=0; i!=size; ++i ) {
                *pi = ptr_cast_from(ph);
                body->eval(context);
                ph += stride;
            }
            return _mm_setzero_ps();
        }
        SimNode * head, * iter, * body;
        int size, stride;
    };
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType
    {
        static __forceinline __m128 Set     ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) =  cast_to<TT>(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)==cast_to<TT>(b));  }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)!=cast_to<TT>(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_CoreType<TT>
    {
        // numeric
        static __forceinline __m128 Unp ( __m128 x ) { return x; }
        static __forceinline __m128 Unm ( __m128 x ) { return cast_from<TT>( -cast_to<TT>(x)); }
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)+cast_to<TT>(b)); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)-cast_to<TT>(b)); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)/cast_to<TT>(b)); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a)*cast_to<TT>(b)); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) += cast_to<TT>(b); return a; }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) -= cast_to<TT>(b); return a; }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) *= cast_to<TT>(b); return a; }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) /= cast_to<TT>(b); return a; }
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)<=cast_to<TT>(b));  }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)>=cast_to<TT>(b));  }
        static __forceinline __m128 Less    ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)<cast_to<TT>(b));  }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<TT>(a)>cast_to<TT>(b));  }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool>
    {
        static __forceinline __m128 BoolNot ( __m128 x ) { return cast_from<bool>( !cast_to<bool>(x)); }
        static __forceinline __m128 BoolAnd ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) && cast_to<bool>(b)); }
        static __forceinline __m128 BoolOr  ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) || cast_to<bool>(b)); }
        static __forceinline __m128 BoolXor ( __m128 a, __m128 b ) { return cast_from<bool>(cast_to<bool>(a) != cast_to<bool>(b)); }
        static __forceinline __m128 SetBoolAnd  ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa && cast_to<bool>(b); return a; }
        static __forceinline __m128 SetBoolOr   ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa || cast_to<bool>(b); return a; }
        static __forceinline __m128 SetBoolXor  ( __m128 a, __m128 b ) { auto pa = ptr_cast_to<bool>(a); *pa = *pa != cast_to<bool>(b); return a; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT>
    {
        static __forceinline __m128 BinNot ( __m128 x ) { return cast_from<TT>( ~cast_to<TT>(x)); }
        static __forceinline __m128 BinAnd ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) & cast_to<TT>(b)); }
        static __forceinline __m128 BinOr  ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) | cast_to<TT>(b)); }
        static __forceinline __m128 BinXor ( __m128 a, __m128 b ) { return cast_from<TT>(cast_to<TT>(a) ^ cast_to<TT>(b)); }
        static __forceinline __m128 SetBinAnd ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) &= cast_to<TT>(b); return a; }
        static __forceinline __m128 SetBinOr  ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) |= cast_to<TT>(b); return a; }
        static __forceinline __m128 SetBinXor ( __m128 a, __m128 b ) { *ptr_cast_to<TT>(a) ^= cast_to<TT>(b); return a; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int64_t>
    {
    };
    
    struct SimPolicy_UInt : SimPolicy_Bin<uint64_t>
    {
    };
    
    struct SimPolicy_Float : SimPolicy_Type<float>
    {
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return _mm_add_ss(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return _mm_sub_ss(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_ss(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_ss(a,b); }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_Vec
    {
        // basic
        static __forceinline __m128 Set     ( __m128 a, __m128 b )
            { *ptr_cast_to<TT>(a) =  cast_to<TT>(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b )
            { return cast_from<bool>( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask ); }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b )
            { return cast_from<bool>( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask ); }
        // numeric
        static __forceinline __m128 Unp ( __m128 x ) { return x; }
        static __forceinline __m128 Unm ( __m128 x ) { return _mm_sub_ps(_mm_setzero_ps(), x); }    // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return _mm_add_ps(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return _mm_sub_ps(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_ps(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_ps(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b )
            {   TT * pa = ptr_cast_to<TT>(a);   *pa = cast_to<TT> ( _mm_add_ps(cast_from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b )
            {   TT * pa = ptr_cast_to<TT>(a);   *pa = cast_to<TT> ( _mm_sub_ps(cast_from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b )
            {   TT * pa = ptr_cast_to<TT>(a);   *pa = cast_to<TT> ( _mm_div_ps(cast_from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b )
            {   TT * pa = ptr_cast_to<TT>(a);   *pa = cast_to<TT> ( _mm_mul_ps(cast_from(*pa), b)); return a;   }
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
