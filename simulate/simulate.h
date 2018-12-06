#pragma once

#include "vectypes.h"
#include "cast.h"
#include "runtime_string.h"
#include "debug_info.h"

namespace yzg
{
    #ifndef YZG_ENABLE_STACK_WALK
    #define YZG_ENABLE_STACK_WALK   1
    #endif
    
    #ifndef YZG_ENABLE_EXCEPTIONS
    #define YZG_ENABLE_EXCEPTIONS   1
    #endif
    
    #define MAX_FOR_ITERATORS   4
    
    using namespace std;
    
    class Context;
    struct SimNode;
    
    struct GlobalVariable
    {
        char *      name;
        __m128      value;
        uint32_t    size;
        VarInfo *   debug;
        SimNode *   init;
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
        SimNode ( const LineInfo & at ) : debug(at) {}
        virtual __m128 eval ( Context & ) = 0;
        LineInfo debug;
    };
    
    struct Prologue
    {
        __m128      result;
        __m128 *    arguments;
        FuncInfo *  info;
        int32_t     line;
    };
    static_assert((sizeof(Prologue) & 0xf)==0, "it has to be 16 byte aligned");
    
    enum EvalFlags : uint32_t
    {
        stopForBreak        = 1 << 0
    ,   stopForReturn       = 1 << 1
    ,   stopForThrow        = 1 << 2
    ,   stopForTerminate    = 1 << 3
    };
    
    class Context
    {
        friend struct SimNode_GetGlobal;
        friend class Program;
    public:
        Context(const string * lines);
        ~Context();
        
        __forceinline void * reallocate ( void * oldData, uint32_t oldSize, uint32_t size )
        {
            if ( size <= oldSize ) return oldData;
            size = (size + 0x0f) & ~0x0f;
            oldSize = (oldSize + 0x0f) & ~0x0f;
            if ( oldData && (oldData == linearAllocator - oldSize) ) {
                uint32_t extra = size - oldSize;
                if ( linearAllocator - linearAllocatorBase + extra > linearAllocatorSize ) {
                    throw_error("out of linear allocator space");
                    return nullptr;
                }
                linearAllocator += extra;
                return oldData;
            } else {
                void * data = allocate(size);
                memcpy ( data, oldData, oldSize );
                return data;
            }
        }
        
        __forceinline void * allocate ( uint32_t size ) {
            size = (size + 0x0f) & ~0x0f;
            if ( linearAllocator - linearAllocatorBase + size > linearAllocatorSize ) {
                throw_error("out of linear allocator space");
                return nullptr;
            }
            void * result = linearAllocator;
            linearAllocator += size;
            return result;
        }
        
        __forceinline char * allocateName ( const string & name ) {
            if ( name.empty() )
                return nullptr;
            auto size = name.length() + 1;
            if ( char * str = (char *) allocate(uint32_t(size+1)) ) {
                memcpy ( str, name.c_str(), size );
                return str;
            } else {
                return nullptr;
            }
        }
        
        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }
        
        __forceinline __m128 getVariable ( int index ) const {
            assert(index>=0 && index<totalVariables && "variable index out of range");
            return globalVariables[index].value;
        }
        
        __forceinline void restart( ) {
            linearAllocator = linearAllocatorExecuteBase;
            stackTop = stack + stackSize;
            stopFlags = 0;
        }
        
        __forceinline __m128 eval ( int fnIndex, __m128 * args ) {
            return call(fnIndex, args, 0);
        }
        
        __forceinline void throw_error ( const char * message ) {
            exception = message;
            stopFlags |= EvalFlags::stopForThrow;
            #if !YZG_ENABLE_EXCEPTIONS
                throw runtime_error(message ? message : "");
            #endif
        }
        
        int findFunction ( const char * name ) const;
        int findVariable ( const char * name ) const;
        void stackWalk();
        void runInitScript ( void );
        
        virtual void to_out ( const char * message );           // output to stdout or equivalent
        virtual void to_err ( const char * message );           // output to stderr or equivalent
        virtual void breakPoint(int column, int line) const;    // what to do in case of breakpoint
        
        __forceinline __m128 * abiArguments() {
            return ((Prologue *)stackTop)->arguments;
        }
        
        __forceinline __m128 & abiResult() {
            return ((Prologue *)stackTop)->result;
        }
        
        __forceinline __m128 call ( int fnIndex, __m128 * args, int line ) {
            assert(fnIndex>=0 && fnIndex<totalFunctions && "function index out of range");
            auto & fn = functions[fnIndex];
            // PUSH
            if ( stack - ( stackTop - fn.stackSize ) > stackSize ) {
                throw_error("stack overflow");
                return _mm_setzero_ps();
            }
            char * pushStack = stackTop;
            stackTop -= fn.stackSize;
            // fill prologue
            Prologue * pp = (Prologue *) stackTop;
            pp->result =        _mm_setzero_ps();
            pp->arguments =     args;
        #if YZG_ENABLE_STACK_WALK
            pp->info =          fn.debug;
            pp->line =          line;
        #endif
            // CALL
            fn.code->eval(*this);
            __m128 result = abiResult();
            stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            // POP
            stackTop = pushStack;
            return result;
        }
        
        __forceinline const char * getException() const {
            return stopFlags & EvalFlags::stopForThrow ? exception : nullptr;
        }
        
    protected:
        int linearAllocatorSize = 1*1024*1024;
        char * linearAllocator = nullptr;
        char * linearAllocatorBase = nullptr;
        char * linearAllocatorExecuteBase = nullptr;
        GlobalVariable * globalVariables = nullptr;
        SimFunction * functions = nullptr;
        int totalVariables = 0;
        int totalFunctions = 0;
        const char * exception = nullptr;
    public:
        const string * debugInput = nullptr;
        class Program * thisProgram = nullptr;
        char * stackTop = nullptr;
        char * stack = nullptr;
        int stackSize = 16*1024;
        uint32_t stopFlags = 0;
    };
    
#if YZG_ENABLE_EXCEPTIONS
    #define YZG_EXCEPTION_POINT \
        { if ( context.stopFlags ) return _mm_setzero_ps(); }
    #define YZG_ITERATOR_EXCEPTION_POINT \
        { if ( context.stopFlags ) return nullptr; }
#else
    #define YZG_EXCEPTION_POINT
    #define YZG_ITERATOR_EXCEPTION_POINT
#endif
    
    struct SimNode_Assert : SimNode {
        SimNode_Assert ( const LineInfo & at, SimNode * s, const char * m ) : SimNode(at), subexpr(s), message(m) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode *       subexpr;
        const char *    message;
    };
    
    // field
    template <bool checkForNull>
    struct SimNode_FieldDeref : SimNode {
        SimNode_FieldDeref ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode(at), value(rv), offset(of) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 rv = value->eval(context);
            YZG_EXCEPTION_POINT;
            char * prv = cast<char *>::to(rv);
            if ( checkForNull && !prv ) {
                context.throw_error("dereferencing nil pointer");
                return _mm_setzero_ps();
            } else {
                return cast<char *>::from( prv + offset );
            }
        }
        SimNode *   value;
        uint32_t    offset;
    };
    typedef SimNode_FieldDeref<false> SimNode_Field;
    typedef SimNode_FieldDeref<true> SimNode_PtrField;
    
    // AT (INDEX)
    struct SimNode_At : SimNode {
        SimNode_At ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t strd, uint32_t rng )
            : SimNode(at), value(rv), index(idx), stride(strd), range(rng) {}
        virtual __m128 eval ( Context & context ) override {
            char * pValue = cast<char *>::to(value->eval(context));
            YZG_EXCEPTION_POINT;
            uint32_t idx = cast<uint32_t>::to(index->eval(context));
            YZG_EXCEPTION_POINT;
            if ( idx >= range ) {
                context.throw_error("index out of range");
                return _mm_setzero_ps();
            } else {
                return cast<char *>::from(pValue + idx*stride);
            }
        }
        SimNode * value, * index;
        uint32_t  stride, range;
    };
    
    // FUNCTION CALL
    struct SimNode_Call : SimNode {
        SimNode_Call ( const LineInfo & at ) : SimNode(at) {}
        __forceinline __m128 * abiArgValues ( Context & context ) const { return (__m128 *)(context.stackTop+stackTop); }
        __forceinline void evalArgs ( Context & context ) {
            __m128 * argValues = abiArgValues(context);
            for ( int i=0; i!=nArguments && !context.stopFlags; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
        virtual __m128 eval ( Context & context ) override {
            __m128 * argValues = abiArgValues(context);
            evalArgs(context);
            YZG_EXCEPTION_POINT;
            return context.call(fnIndex, argValues, debug.line);
        }
        SimNode ** arguments;
        int32_t  fnIndex;
        int32_t  nArguments;
        uint32_t stackTop;
    };
    
    // CAST
    template <typename CastTo, typename CastFrom>
    struct SimNode_Cast : SimNode_Call {
        SimNode_Cast ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            CastTo value = (CastTo) cast<CastFrom>::to(res);
            return cast<CastTo>::from(value);
        }
    };
    
    // VECTOR C-TOR
    template <int vecS>
    struct SimNode_VecCtor : SimNode_Call {
        SimNode_VecCtor ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 * argValues = abiArgValues(context);
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
    
    // "DEBUG"
    struct SimNode_Debug : SimNode {
        SimNode_Debug ( const LineInfo & at, SimNode * s, TypeInfo * ti, char * msg )
            : SimNode(at), subexpr(s), typeInfo(ti), message(msg) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 res = subexpr->eval(context);
            YZG_EXCEPTION_POINT;
            stringstream ssw;
            if ( message ) ssw << message << " ";
            ssw << debug_type(typeInfo) << " = " << debug_value(res, typeInfo) << " at " << debug.describe() << "\n";
            context.to_out(ssw.str().c_str());
            return res;
        }
        SimNode *       subexpr;
        TypeInfo *      typeInfo;
        const char *    message;
    };
    
    // LOCAL VARIABLE "GET"
    struct SimNode_GetLocal : SimNode {
        SimNode_GetLocal(const LineInfo & at, uint32_t sp) : SimNode(at), stackTop(sp) {}
        virtual __m128 eval ( Context & context ) override {
            return cast<char *>::from(context.stackTop + stackTop);
        }
        uint32_t stackTop;
    };
    
    // WHEN LOCAL VARIABLE STORES REFERENCE
    struct SimNode_GetLocalRef : SimNode {
        SimNode_GetLocalRef(const LineInfo & at, uint32_t sp) : SimNode(at), stackTop(sp) {}
        virtual __m128 eval ( Context & context ) override {
            return *(__m128 *)(context.stackTop + stackTop);
        }
        uint32_t stackTop;
    };
    
    // ZERO MEMORY OF UNITIALIZED LOCAL VARIABLE
    struct SimNode_InitLocal : SimNode {
        SimNode_InitLocal(const LineInfo & at, uint32_t sp, uint32_t sz) : SimNode(at), stackTop(sp), size(sz) {}
        virtual __m128 eval ( Context & context ) override {
            memset(context.stackTop + stackTop, 0, size);
            return _mm_setzero_ps();
        }
        uint32_t stackTop, size;
    };
    
    // ARGUMENT VARIABLE "GET"
    struct SimNode_GetArgument : SimNode {
        SimNode_GetArgument ( const LineInfo & at, int32_t i ) : SimNode(at), index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.abiArguments()[index];
        }
        int32_t index;
    };
    
    // GLOBAL VARIABLE "GET"
    struct SimNode_GetGlobal : SimNode {
        SimNode_GetGlobal ( const LineInfo & at, int32_t i ) : SimNode(at), index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.globalVariables[index].value;
        }
        int32_t index;
    };
    
    // TRY-CATCH
    struct SimNode_TryCatch : SimNode {
        SimNode_TryCatch ( const LineInfo & at, SimNode * t, SimNode * c ) : SimNode(at), try_block(t), catch_block(c) {}
        virtual __m128 eval ( Context & context ) override {
            #if YZG_ENABLE_EXCEPTIONS
                try_block->eval(context);
                if ( context.stopFlags & EvalFlags::stopForThrow ) {
                    context.stopFlags &= ~(EvalFlags::stopForThrow | EvalFlags::stopForReturn | EvalFlags::stopForBreak);
                    catch_block->eval(context);
                }
            #else
                try {
                    try_block->eval(context);
                } catch ( const runtime_error & ) {
                    context.stopFlags &= ~(EvalFlags::stopForThrow | EvalFlags::stopForReturn | EvalFlags::stopForBreak);
                    catch_block->eval(context);
                }
            #endif
            return _mm_setzero_ps();
        }
        SimNode * try_block, * catch_block;
    };
    
    // RETURN
    struct SimNode_Return : SimNode {
        SimNode_Return ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            if ( subexpr )
                context.abiResult() = subexpr->eval(context);
            context.stopFlags |= EvalFlags::stopForReturn;
            return _mm_setzero_ps();
        }
        SimNode * subexpr;
    };
    
    // BREAK
    struct SimNode_Break : SimNode {
        SimNode_Break ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            context.stopFlags |= EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };
    
    // DEREFERENCE
    template <typename TT>
    struct SimNode_Ref2Value : SimNode {      // &value -> value
        SimNode_Ref2Value ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ptr = subexpr->eval(context);
            TT * pR = cast<TT *>::to(ptr);  // never null
            return cast<TT>::from(*pR);
        }
        SimNode * subexpr;
    };
    
    // POINTER TO REFERENCE (CAST)
    struct SimNode_Ptr2Ref : SimNode {      // ptr -> &value
        SimNode_Ptr2Ref ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 ptr = subexpr->eval(context);
            void * p = cast<void *>::to(ptr);
            if ( p == nullptr ) {
                context.throw_error("dereferencing nil pointer");
                return _mm_setzero_ps();
            }
            return ptr;
        }
        SimNode * subexpr;
    };
    
    // NEW
    struct SimNode_New : SimNode {
        SimNode_New ( const LineInfo & at, int32_t b ) : SimNode(at), bytes(b) {}
        virtual __m128 eval ( Context & context ) override {
            if ( void * ptr = context.allocate(bytes) ) {
                memset ( ptr, 0, bytes );
                return cast<void *>::from(ptr);
            } else {
                context.throw_error("out of memory");
                return _mm_setzero_ps();
            }
        }
        int32_t     bytes;
    };
    
    // CONST-VALUE
    template <typename TT>
    struct SimNode_ConstValue : SimNode {
        SimNode_ConstValue(const LineInfo & at, TT c) : SimNode(at), value(cast<TT>::from(c)) {}
        virtual __m128 eval ( Context & context ) override {
            return value;
        }
        __m128 value;
    };
    
    // COPY VALUE
    template <typename TT>
    struct SimNode_CopyValue : SimNode {
        SimNode_CopyValue(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            TT * pl = cast<TT *>::to(ll);
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return ll;
        }
        SimNode * l, * r;
    };
    
    // COPY REFERENCE VALUE
    struct SimNode_CopyRefValue : SimNode {
        SimNode_CopyRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
        : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override {
            __m128 ll = l->eval(context);
            YZG_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_EXCEPTION_POINT;
            auto pl = cast<void *>::to(ll);
            auto pr = cast<void *>::to(rr);
            memcpy ( pl, pr, size );
            return ll;
        }
        SimNode * l, * r;
        uint32_t size;
    };
    
    // BLOCK
    struct SimNode_Block : SimNode {
        SimNode_Block ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i!=total && !context.stopFlags; ++i )
                list[i]->eval(context);
            return _mm_setzero_ps();
        }
        SimNode ** list = nullptr;
        uint32_t total = 0;
    };
    
    // LET
    struct SimNode_Let : SimNode_Block {
        SimNode_Let ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual __m128 eval ( Context & context ) override {
            for ( int i = 0; i!=total && !context.stopFlags; ++i )
                list[i]->eval(context);
            if ( context.stopFlags ) return _mm_setzero_ps();
            return subexpr->eval(context);
        }
        SimNode * subexpr = nullptr;
    };
    
    // IF-THEN-ELSE
    struct SimNode_IfThenElse : SimNode {
        SimNode_IfThenElse ( const LineInfo & at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual __m128 eval ( Context & context ) override {
            bool cmp = cast<bool>::to(cond->eval(context));
            YZG_EXCEPTION_POINT;
            if ( cmp ) {
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
        SimNode_While ( const LineInfo & at, SimNode * c, SimNode * b ) : SimNode(at), cond(c), body(b) {}
        virtual __m128 eval ( Context & context ) override {
            while ( cast<bool>::to(cond->eval(context)) && !context.stopFlags ) {
                body->eval(context);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
        SimNode * cond, * body;
    };
    
    // FOR BASE
    struct SimNode_ForBase : SimNode {
        SimNode_ForBase ( const LineInfo & at ) : SimNode(at) {}
        SimNode *   sources [MAX_FOR_ITERATORS];
        uint32_t    strides [MAX_FOR_ITERATORS];
        uint32_t    stackTop[MAX_FOR_ITERATORS];
        SimNode *   body;
        SimNode *   filter;
        uint32_t    size;
    };

    template <int total>
    struct SimNode_ForGoodArray : public SimNode_ForBase {
        SimNode_ForGoodArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            Array * pha[total];
            char * ph[total];
            for ( int t=0; t!=total; ++t ) {
                pha[t] = cast<Array *>::to(sources[t]->eval(context));
                YZG_EXCEPTION_POINT;
                array_lock(context, *pha[t]);
                YZG_EXCEPTION_POINT;
                ph[t]  = pha[t]->data;
            }
            char ** pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stackTop + stackTop[t]);
            }
            for ( int i=0; !context.stopFlags; ++i ) {
                for ( int t=0; t!=total; ++t ){
                    if ( i > pha[t]->size ) goto loopOver;
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                if ( !filter || cast<bool>::to(filter->eval(context)) )
                    if ( !context.stopFlags )
                        body->eval(context);
            }
        loopOver:
            for ( int t=0; t!=total; ++t ) {
                array_unlock(context, *pha[t]);
                YZG_EXCEPTION_POINT;
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };
    
    // FOR
    template <int total>
    struct SimNode_ForFixedArray : SimNode_ForBase {
        SimNode_ForFixedArray ( const LineInfo & at ) : SimNode_ForBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph[total];
            for ( int t=0; t!=total; ++t ) {
                ph[t] = cast<char *>::to(sources[t]->eval(context));
                YZG_EXCEPTION_POINT;
            }
            char ** pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stackTop + stackTop[t]);
            }
            for ( int i=0; i!=size && !context.stopFlags; ++i ) {
                for ( int t=0; t!=total; ++t ){
                    *pi[t] = ph[t];
                    ph[t] += strides[t];
                }
                if ( !filter || cast<bool>::to(filter->eval(context)) )
                    if ( !context.stopFlags )
                        body->eval(context);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };
    
    // iterator
    
    struct Iterator {
        virtual void * first ( Context & context ) = 0;
        virtual void * next  ( Context & context ) = 0;
        virtual void close ( Context & context ) = 0;
    };
    
    struct FixedArrayIterator : Iterator {
        virtual void * first ( Context & context ) override {
            __m128 ll = source->eval(context);
            YZG_ITERATOR_EXCEPTION_POINT;
            data = cast<char *>::to(ll);
            dataEnd = data + size * stride;
            return data;
        }
        virtual void * next  ( Context & context ) override {
            if ( data>=dataEnd ) return nullptr;
            data += stride;
            return data;
        }
        virtual void close ( Context & context ) override {
        }
        SimNode *   source;
        char *      data;
        char *      dataEnd;
        uint32_t    size;
        uint32_t    stride;
    };
    
    struct SimNode_ForWithIteratorBase : SimNode {
        SimNode_ForWithIteratorBase ( const LineInfo & at ) : SimNode(at) {}
        Iterator *  sources[MAX_FOR_ITERATORS];
        SimNode *   body;
        SimNode *   filter;
        uint32_t    stackTop[MAX_FOR_ITERATORS];
    };
    
    template <int total>
    struct SimNode_ForWithIterator : SimNode_ForWithIteratorBase {
        SimNode_ForWithIterator ( const LineInfo & at ) : SimNode_ForWithIteratorBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph[total];
            for ( int t=0; t!=total; ++t ) {
                ph[t] = (char *) sources[t]->first(context);
                YZG_EXCEPTION_POINT;
            }
            char ** pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (char **)(context.stackTop + stackTop[t]);
            }
            for ( int i=0; !context.stopFlags; ++i ) {
                for ( int t=0; t!=total; ++t ){
                    *pi[t] = ph[t];
                    ph[t] = (char *) sources[t]->next(context);
                    YZG_EXCEPTION_POINT;
                    if ( !ph[t] ) goto loopend;
                }
                if ( !filter || cast<bool>::to(filter->eval(context)) )
                    if ( !context.stopFlags )
                        body->eval(context);
            }
        loopend:
            for ( int t=0; t!=total; ++t ) {
                sources[t]->close(context);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
    };
    
    // FOREACH
    struct SimNode_Foreach : SimNode {
        SimNode_Foreach ( const LineInfo & at, SimNode * h, SimNode * i, SimNode * b, int sz, int st, int ts )
            : SimNode(at), head(h), iter(i), body(b), size(sz), stride(st), typeSize(ts) {}
        virtual __m128 eval ( Context & context ) override {
            char * ph = cast<char *>::to(head->eval(context));
            YZG_EXCEPTION_POINT;
            char * pi = cast<char *>::to(iter->eval(context));
            YZG_EXCEPTION_POINT;
            for ( int i=0; i!=size && !context.stopFlags; ++i ) {
                memcpy(pi,ph,typeSize);
                body->eval(context);
                ph += stride;
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return _mm_setzero_ps();
        }
        SimNode * head, * iter, * body;
        int size, stride, typeSize;
    };
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType
    {
        static __forceinline __m128 Set     ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)==cast<TT>::to(b));  }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)!=cast<TT>::to(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_CoreType<TT>
    {
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return cast<TT>::from( -cast<TT>::to(x)); }
        static __forceinline __m128 Inc ( __m128 x, Context & ) { (*cast<TT*>::to(x))++; return x; }
        static __forceinline __m128 Dec ( __m128 x, Context & ) { (*cast<TT*>::to(x))--; return x; }
        static __forceinline __m128 IncPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X++); }
        static __forceinline __m128 DecPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X--); }
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)+cast<TT>::to(b)); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)-cast<TT>::to(b)); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)/cast<TT>::to(b)); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)*cast<TT>::to(b)); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) += cast<TT>::to(b); return a; }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) -= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) *= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) /= cast<TT>::to(b); return a; }
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<=cast<TT>::to(b));  }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>=cast<TT>::to(b));  }
        static __forceinline __m128 Less    ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<cast<TT>::to(b));  }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>cast<TT>::to(b));  }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool>
    {
        static __forceinline __m128 BoolNot ( __m128 x, Context & ) { return cast<bool>::from( !cast<bool>::to(x)); }
        static __forceinline __m128 BoolAnd ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) && cast<bool>::to(b)); }
        static __forceinline __m128 BoolOr  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) || cast<bool>::to(b)); }
        static __forceinline __m128 BoolXor ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) != cast<bool>::to(b)); }
        static __forceinline __m128 SetBoolAnd  ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa && cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolOr   ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa || cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolXor  ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa != cast<bool>::to(b); return a; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT>
    {
        static __forceinline __m128 BinNot ( __m128 x, Context & ) { return cast<TT>::from( ~cast<TT>::to(x)); }
        static __forceinline __m128 BinAnd ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) & cast<TT>::to(b)); }
        static __forceinline __m128 BinOr  ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) | cast<TT>::to(b)); }
        static __forceinline __m128 BinXor ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) ^ cast<TT>::to(b)); }
        static __forceinline __m128 SetBinAnd ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) &= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinOr  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) |= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinXor ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) ^= cast<TT>::to(b); return a; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int32_t>
    {
    };
    
    struct SimPolicy_UInt : SimPolicy_Bin<uint32_t>
    {
    };
    
    struct SimPolicy_Float : SimPolicy_Type<float>
    {
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_ss(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_ss(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_ss(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_ss(a,b); }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_Vec
    {
        // basic
        static __forceinline __m128 Set     ( __m128 a, __m128 b, Context & )
            { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask ); }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask ); }
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return _mm_sub_ps(_mm_setzero_ps(), x); }    // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_ps(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_ps(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_ps(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_ps(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
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
        static __forceinline __m128 Set     ( __m128 a, __m128 b, Context & )
            { *cast<TT *>::to(a) =  cast<TT>::to(b); return a; }
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)==mask ); }            // todo: verify
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)!=mask ); }            // todo: verify
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return _mm_sub_epi32(_mm_setzero_si128(), x); }      // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_epi32(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_epi32(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_epi32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_epi32(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
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
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_epu32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_epu32(a,b); }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epu32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_epu32(cast<TT>::from(*pa), b)); return a;   }
    };
    
    struct SimPolicy_Pointer : SimPolicy_CoreType<void *> {};

    // op1 policies
    
    struct SimNode_Op1 : SimNode {
        SimNode_Op1 ( const LineInfo & at ) : SimNode(at) {}
        SimNode * x = nullptr;
    };
    
#define DEFINE_OP1_POLICY(CALL)                                         \
    template <typename SimPolicy>                                       \
    struct Sim_##CALL : SimNode_Op1 {                                   \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        virtual __m128 eval ( Context & context ) override {            \
            __m128 val = x->eval(context);                              \
            YZG_EXCEPTION_POINT;                                        \
            return SimPolicy::CALL(val,context);                        \
        }                                                               \
    };
    
    DEFINE_OP1_POLICY(Unp);
    DEFINE_OP1_POLICY(Unm);
    DEFINE_OP1_POLICY(Inc);
    DEFINE_OP1_POLICY(Dec);
    DEFINE_OP1_POLICY(IncPost);
    DEFINE_OP1_POLICY(DecPost);
    DEFINE_OP1_POLICY(BoolNot);
    DEFINE_OP1_POLICY(BinNot);
    
    // op2 policies
    
    struct SimNode_Op2 : SimNode {
        SimNode_Op2 ( const LineInfo & at ) : SimNode(at) {}
        SimNode * l = nullptr;
        SimNode * r = nullptr;
    };
    
#define DEFINE_OP2_POLICY(CALL)                                             \
    template <typename SimPolicy>                                           \
    struct Sim_##CALL : SimNode_Op2 {                                       \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}             \
        virtual __m128 eval ( Context & context ) override {                \
            __m128 lv = l->eval(context);                                   \
            YZG_EXCEPTION_POINT;                                            \
            __m128 rv = r->eval(context);                                   \
            YZG_EXCEPTION_POINT;                                            \
            return SimPolicy::CALL ( lv, rv, context );                     \
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
    
    struct Sim_BoolAnd : SimNode_Op2 {
        Sim_BoolAnd ( const LineInfo & at ) : SimNode_Op2(at) {}
        virtual __m128 eval ( Context & context ) override {
            if ( !cast<bool>::to(l->eval(context)) ) {  // if not left, then false
                return cast<bool>::from(false);
            } else {
                YZG_EXCEPTION_POINT; 
                return r->eval(context);                // if left, then right
            }
        }
    };
    
    struct Sim_BoolOr : SimNode_Op2 {
        Sim_BoolOr ( const LineInfo & at ) : SimNode_Op2(at) {}
        virtual __m128 eval ( Context & context ) override {
            if ( cast<bool>::to(l->eval(context)) ) {   // if left, then true
                return cast<bool>::from(true);
            } else {
                YZG_EXCEPTION_POINT;
                return r->eval(context);                // if not left, then right
            }
        }
    };

}
