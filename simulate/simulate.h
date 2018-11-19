#pragma once

#include "vectypes.h"
#include "cast.h"
#include "runtime_string.h"
#include "debug_info.h"

namespace yzg
{
    using namespace std;
    
    class Context;
    struct SimNode;
    
    struct GlobalVariable
    {
        char *      name;
        __m128      value;
        uint32_t    size;
        VarInfo *   debug;
    };
    
    struct SimFunction
    {
        char *      name;
        SimNode *   code;
        uint32_t    stackSize;
        FuncInfo *  debug;
    };
    
    struct SimNode
    {
        SimNode ( long at ) : debug(at) {}
        virtual __m128 eval ( Context & ) = 0;
        long debug = 0;
    };
    
    class Context
    {
        friend class Program;
        friend struct SimNode_GetGlobal;
        friend struct SimNode_GetLocal;
        friend struct SimNode_GetArgument;
        friend struct SimNode_TryCatch;
        friend struct SimNode_Call;
        friend struct SimNode_InitLocal;
        friend struct SimNode_Return;
    public:
        Context(const string * lines);
        ~Context();
        
        __forceinline void * allocate ( uint32_t size ) {
            size = (size + 0x0f) & ~0x0f;
            if ( linearAllocator - linearAllocatorBase + size > linearAllocatorSize )
                throw runtime_error("out of linear allocator space");
            void * result = linearAllocator;
            linearAllocator += size;
            return result;
        }
        
        __forceinline char * allocateName ( const string & name ) {
            auto size = name.length() + 1;
            char * str = (char *) allocate(uint32_t(size+1));
            memcpy ( str, name.c_str(), size );
            return str;
        }
        
        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }
        
        __forceinline __m128 getVariable ( int index ) const { return globalVariables[index].value; }
        
        int findFunction ( const char * name ) const;
        int findVariable ( const char * name ) const;
        
        void stackWalk();

        __forceinline void restart() {
            linearAllocator = linearAllocatorExecuteBase;
            stackTop = stack + stackSize;
        }
        
        __forceinline __m128 eval ( int fnIndex, __m128 * args ) {
            // we are abusing the fact that node pointers are aligned
            SimNode * fakeNode = (SimNode *) ( uint64_t(functions[fnIndex].debug) | 1 );
            return call(fnIndex, args, fakeNode);
        }

    protected:
        
        __forceinline __m128 * abiArguments() {
            return *(__m128 **)stackTop;
        }
        
        __forceinline __m128 & abiResult() {
            return *(__m128 *)(stackTop + sizeof(__m128 *) + sizeof(SimNode *));
        }
        
        __forceinline __m128 call ( int fnIndex, __m128 * args, SimNode * THAT ) {
            assert(fnIndex>=0 && fnIndex<totalFunctions && "function index out of range");
            auto & fn = functions[fnIndex];
            // PUSH
            char * pushStack = stackTop;
            stackTop -= fn.stackSize;
            if ( stack - stackTop > stackSize )
                throw runtime_error("stack overflow");
            *(__m128 **)(stackTop) = args;                                                      // args
            *(SimNode **)(stackTop + sizeof(__m128 *)) = THAT;                                  // debug info
            *(__m128 *)(stackTop + sizeof(__m128 *) + sizeof(SimNode *)) = _mm_setzero_ps();    // result
            // CALL
            fn.code->eval(*this);
            __m128 result = abiResult();
            // POP
            stackTop = pushStack;
            return result;
        }
        
    protected:
        int linearAllocatorSize = 1*1024*1024;
        char * linearAllocator = nullptr;
        char * linearAllocatorBase = nullptr;
        char * linearAllocatorExecuteBase = nullptr;
        GlobalVariable * globalVariables = nullptr;
        SimFunction * functions = nullptr;
        char * stack = nullptr;
        char * stackTop = nullptr;
        int stackSize = 16*1024;
        int totalVariables = 0;
        int totalFunctions = 0;
        const string * debugInput = nullptr;
    };
    
    // field
    struct SimNode_Field : SimNode {
        SimNode_Field ( long at, SimNode * rv, uint32_t of ) : SimNode(at), value(rv), offset(of) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 rv = value->eval(context);
            char * prv = cast<char *>::to(rv);
            if ( !prv )
                throw runtime_error("dereferencing nil pointer");
            return cast<char *>::from( prv + offset );
        }
        SimNode *   value;
        uint32_t    offset;
    };
    
    // AT (INDEX)
    struct SimNode_At : SimNode {
        virtual __m128 eval ( Context & context ) override {
            char * pValue = cast<char *>::to(value->eval(context));
            uint32_t idx = cast<uint32_t>::to(index->eval(context));
            if ( idx >= range )
                throw runtime_error("index out of range");
            return cast<char *>::from(pValue + idx*stride);    // TODO: add range check
            
        }
        SimNode_At ( long at, SimNode * rv, SimNode * idx, uint32_t strd, uint32_t rng )
            : SimNode(at), value(rv), index(idx), stride(strd), range(rng) {}
        SimNode * value, * index;
        uint32_t  stride, range;
    };
    
    // FUNCTION CALL
    struct SimNode_Call : SimNode {
        SimNode_Call ( long at ) : SimNode(at) {}
        __forceinline void evalArgs ( Context & context ) {
            for ( int i=0; i!=nArguments; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            return context.call(fnIndex, argValues, this);
        }
        int fnIndex;
        int nArguments;
        SimNode ** arguments;
        __m128 * argValues;
    };
    
    // CAST
    template <typename CastTo, typename CastFrom>
    struct SimNode_Cast : SimNode_Call {
        SimNode_Cast ( long at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            CastTo value = (CastTo) cast<CastFrom>::to(res);
            return cast<CastTo>::from(value);
        }
    };
    
    // VECTOR C-TOR
    template <int vecS>
    struct SimNode_VecCtor : SimNode_Call {
        SimNode_VecCtor ( long at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            if ( vecS==2 )
                return _mm_setr_ps(cast<float>::to(argValues[0]),cast<float>::to(argValues[1]),
                                   0.0f,0.0f);
            else if ( vecS==3 )
                return _mm_setr_ps(cast<float>::to(argValues[0]),cast<float>::to(argValues[1]),
                                   cast<float>::to(argValues[2]),0.0f);
            else if ( vecS==4 )
                return _mm_setr_ps(cast<float>::to(argValues[0]),cast<float>::to(argValues[1]),
                                   cast<float>::to(argValues[2]),cast<float>::to(argValues[3]));
            else
                return _mm_setzero_ps();
        }
    };
    
    // "STACKWALK"
    struct SimNode_StackWalk : SimNode {
        SimNode_StackWalk ( long at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            context.stackWalk();
            return _mm_setzero_ps();
        }
    };
    
    // "DEBUG"
    template <typename TT>
    __forceinline void debug_out ( TT value ) { cout << value; }
    __forceinline void debug_out ( uint64_t value ) { cout << hex << "0x" << value << dec; }
    
    template <typename TT>
    struct SimNode_Debug : SimNode_Call {
        SimNode_Debug ( long at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            debug_out( cast<TT>::to(res) );
            return res;
        }
    };
    
    // LOCAL VARIABLE "GET"
    struct SimNode_GetLocal : SimNode {
        SimNode_GetLocal(long at, uint32_t sp) : SimNode(at), stackTop(sp) {}
        virtual __m128 eval ( Context & context ) override {
            return cast<char *>::from(context.stackTop + stackTop);
        }
        uint32_t stackTop;
    };
    
    // ZERO MEMORY OF UNITIALIZED LOCAL VARIABLE
    struct SimNode_InitLocal : SimNode {
        SimNode_InitLocal(long at, uint32_t sp, uint32_t sz) : SimNode(at), stackTop(sp), size(sz) {}
        virtual __m128 eval ( Context & context ) override {
            memset(context.stackTop + stackTop, 0, size);
            return _mm_setzero_ps();
        }
        uint32_t stackTop, size;
    };
    
    // ARGUMENT VARIABLE "GET"
    struct SimNode_GetArgument : SimNode {
        SimNode_GetArgument ( long at, int i ) : SimNode(at), index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.abiArguments()[index];
        }
        int index;
    };
    
    // RETURN VARIABLE GET
    struct SimNode_Return : SimNode {
        SimNode_Return ( long at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            return cast<__m128 *>::from(&context.abiResult());
        }
    };
    
    // GLOBAL VARIABLE "GET"
    struct SimNode_GetGlobal : SimNode {
        SimNode_GetGlobal ( long at, int i ) : SimNode(at), index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.globalVariables[index].value;
        }
        int index;
    };
    
    // DEREFERENCE
    template <typename TT>
    struct SimNode_Ref2Value : SimNode {      // &value -> value
        SimNode_Ref2Value ( long at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ptr = subexpr->eval(context);
            TT * pR = cast<TT *>::to(ptr);  // never null
            return cast<TT>::from(*pR);
        }
        SimNode * subexpr;
    };
    
    // POINTER TO REFERENCE (CAST)
    struct SimNode_Ptr2Ref : SimNode {      // ptr -> &value
        SimNode_Ptr2Ref ( long at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ptr = subexpr->eval(context);
            void * p = cast<void *>::to(ptr);
            if ( p == nullptr )
                throw runtime_error("dereferencing nil pointer");
            return ptr;
        }
        SimNode * subexpr;
    };
    
    // NEW
    struct SimNode_New : SimNode {
        SimNode_New ( long at, int32_t b ) : SimNode(at), bytes(b) {}
        virtual __m128 eval ( Context & context ) override {
            void * ptr = context.allocate(bytes);
            memset ( ptr, 0, bytes );
            return cast<void *>::from(ptr);
        }
        int32_t     bytes;
    };
    
    // CONST-VALUE
    template <typename TT>
    struct SimNode_ConstValue : SimNode {
        SimNode_ConstValue(long at, TT c) : SimNode(at), value(cast<TT>::from(c)) {}
        virtual __m128 eval ( Context & context ) override {
            return value;
        }
        __m128 value;
    };
    
    // COPY REFERENCE VALUE
    struct SimNode_CopyRefValue : SimNode {
        SimNode_CopyRefValue(long at, SimNode * ll, SimNode * rr, int sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            __m128 rr = r->eval(context);
            auto pl = cast<void *>::to(ll);
            auto pr = cast<void *>::to(rr);
            memcpy ( pl, pr, size );
            return ll;
        }
        SimNode * l, * r;
        int size;
    };
    
    // COPY VALUE
    template <typename TT>
    struct SimNode_CopyValue : SimNode {
        SimNode_CopyValue(long at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            __m128 rr = r->eval(context);
            TT * pl = cast<TT *>::to(ll);
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return ll;
        }
        SimNode * l, * r;
    };
    
    // BLOCK
    struct SimNode_Block : SimNode {
        SimNode_Block ( long at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i != total; ++i )
                list[i]->eval(context);
            return _mm_setzero_ps();
        }
        SimNode ** list = nullptr;
        uint32_t total = 0;
    };
    
    // LET
    struct SimNode_Let : SimNode_Block {
        SimNode_Let ( long at ) : SimNode_Block(at) {}
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i != total; ++i )
                list[i]->eval(context);
            return subexpr->eval(context);
        }
        SimNode * subexpr = nullptr;
    };
    
    // IF-THEN-ELSE
    struct SimNode_IfThenElse : SimNode {
        SimNode_IfThenElse ( long at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual __m128 eval ( Context & context ) override {
            if ( cast<bool>::to(cond->eval(context)) ) {
                if_true->eval(context);
            } else if ( if_false ){
                if_false->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * cond, * if_true, * if_false;
    };
    
    // WHILE
    struct SimNode_While : SimNode {
        SimNode_While ( long at, SimNode * c, SimNode * b ) : SimNode(at), cond(c), body(b) {}
        virtual __m128 eval ( Context & context ) override {
            while ( cast<bool>::to(cond->eval(context)) ) {
                body->eval(context);
            }
            return _mm_setzero_ps();
        }
        SimNode * cond, * body;
    };
    
    // TRY-CATCH
    struct SimNode_TryCatch : SimNode {
        SimNode_TryCatch ( long at, SimNode * t, SimNode * c ) : SimNode(at), try_this(t), catch_that(c) {}
        virtual __m128 eval ( Context & context ) override {
            try {
                auto sp = context.stackTop;
                try_this->eval(context);
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
        SimNode_Foreach ( long at, SimNode * h, SimNode * i, SimNode * b, int sz, int st )
            : SimNode(at), head(h), iter(i), body(b), size(sz), stride(st) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph = cast<char *>::to(head->eval(context));
            __m128 * pi = cast<__m128 *>::to(iter->eval(context));
            for ( int i=0; i!=size; ++i ) {
                *pi = cast<char *>::from(ph);
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
        static __forceinline __m128 Set     ( __m128 a, __m128 b ) { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)==cast<TT>::to(b));  }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)!=cast<TT>::to(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_CoreType<TT>
    {
        // numeric
        static __forceinline __m128 Unp ( __m128 x ) { return x; }
        static __forceinline __m128 Unm ( __m128 x ) { return cast<TT>::from( -cast<TT>::to(x)); }
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a)+cast<TT>::to(b)); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a)-cast<TT>::to(b)); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a)/cast<TT>::to(b)); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a)*cast<TT>::to(b)); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b ) { *cast<TT *>::to(a) += cast<TT>::to(b); return a; }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b ) { *cast<TT *>::to(a) -= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b ) { *cast<TT *>::to(a) *= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b ) { *cast<TT *>::to(a) /= cast<TT>::to(b); return a; }
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)<=cast<TT>::to(b));  }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)>=cast<TT>::to(b));  }
        static __forceinline __m128 Less    ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)<cast<TT>::to(b));  }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b ) { return cast<bool>::from(cast<TT>::to(a)>cast<TT>::to(b));  }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool>
    {
        static __forceinline __m128 BoolNot ( __m128 x ) { return cast<bool>::from( !cast<bool>::to(x)); }
        static __forceinline __m128 BoolAnd ( __m128 a, __m128 b ) { return cast<bool>::from(cast<bool>::to(a) && cast<bool>::to(b)); }
        static __forceinline __m128 BoolOr  ( __m128 a, __m128 b ) { return cast<bool>::from(cast<bool>::to(a) || cast<bool>::to(b)); }
        static __forceinline __m128 BoolXor ( __m128 a, __m128 b ) { return cast<bool>::from(cast<bool>::to(a) != cast<bool>::to(b)); }
        static __forceinline __m128 SetBoolAnd  ( __m128 a, __m128 b )
            { auto pa = cast<bool *>::to(a); *pa = *pa && cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolOr   ( __m128 a, __m128 b )
            { auto pa = cast<bool *>::to(a); *pa = *pa || cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolXor  ( __m128 a, __m128 b )
            { auto pa = cast<bool *>::to(a); *pa = *pa != cast<bool>::to(b); return a; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT>
    {
        static __forceinline __m128 BinNot ( __m128 x ) { return cast<TT>::from( ~cast<TT>::to(x)); }
        static __forceinline __m128 BinAnd ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a) & cast<TT>::to(b)); }
        static __forceinline __m128 BinOr  ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a) | cast<TT>::to(b)); }
        static __forceinline __m128 BinXor ( __m128 a, __m128 b ) { return cast<TT>::from(cast<TT>::to(a) ^ cast<TT>::to(b)); }
        static __forceinline __m128 SetBinAnd ( __m128 a, __m128 b ) { *cast<TT *>::to(a) &= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinOr  ( __m128 a, __m128 b ) { *cast<TT *>::to(a) |= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinXor ( __m128 a, __m128 b ) { *cast<TT *>::to(a) ^= cast<TT>::to(b); return a; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int32_t>
    {
    };
    
    struct SimPolicy_UInt : SimPolicy_Bin<uint32_t>
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
            { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask ); }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask ); }
        // numeric
        static __forceinline __m128 Unp ( __m128 x ) { return x; }
        static __forceinline __m128 Unm ( __m128 x ) { return _mm_sub_ps(_mm_setzero_ps(), x); }    // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return _mm_add_ps(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return _mm_sub_ps(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_ps(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_ps(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_ps(cast<TT>::from(*pa), b)); return a;   }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_iVec
    {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128 _mm_mul_epi32 ( __m128 a, __m128 b ) {
            __m128i A = a, B = b, C;
            C[0] = A[0]*B[0];   C[1] = A[1]*B[1];   C[2] = A[2]*B[2];   C[3] = A[3]*B[3];
            return C;
        }
        static __forceinline __m128 _mm_div_epi32 ( __m128 a, __m128 b ) {
            __m128i A = a, B = b, C;
            C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
            return C;
        }
        // basic
        static __forceinline __m128 Set     ( __m128 a, __m128 b )
            { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)==mask ); }            // todo: verify
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)!=mask ); }            // todo: verify
        // numeric
        static __forceinline __m128 Unp ( __m128 x ) { return x; }
        static __forceinline __m128 Unm ( __m128 x ) { return _mm_sub_epi32(_mm_setzero_si128(), x); }      // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b ) { return _mm_add_epi32(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b ) { return _mm_sub_epi32(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_epi32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_epi32(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_epi32(cast<TT>::from(*pa), b)); return a;   }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_uVec : SimPolicy_iVec<TT,mask>
    {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128 _mm_div_epu32 ( __m128 a, __m128 b) {
            __m128 c;  uint32_t * A = (uint32_t *)&a, * B = (uint32_t *)&b, * C = (uint32_t *)&c;
            C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
            return c;
        }
        // swappmin some numeric operations
        static __forceinline __m128 Div ( __m128 a, __m128 b ) { return _mm_div_epu32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b ) { return _mm_mul_epu32(a,b); }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epu32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_epu32(cast<TT>::from(*pa), b)); return a;   }
    };
    
    struct SimPolicy_Pointer : SimPolicy_CoreType<void *>
    {        
    };

    // op1 policies
    
    struct SimNode_Op1 : SimNode {
        SimNode_Op1 ( long at ) : SimNode(at) {}
        SimNode * x = nullptr;
    };
    
#define DEFINE_OP1_POLICY(CALL)                                         \
    template <typename SimPolicy>                                       \
    struct Sim_##CALL : SimNode_Op1 {                                   \
        Sim_##CALL ( long at ) : SimNode_Op1(at) {}                     \
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
        SimNode_Op2 ( long at ) : SimNode(at) {}
        SimNode * l = nullptr;
        SimNode * r = nullptr;
    };
    
#define DEFINE_OP2_POLICY(CALL)                                             \
    template <typename SimPolicy>                                           \
    struct Sim_##CALL : SimNode_Op2 {                                       \
        Sim_##CALL ( long at ) : SimNode_Op2(at) {}                         \
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
