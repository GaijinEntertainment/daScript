#pragma once

#include <setjmp.h>
#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/simulate/cast.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/sim_policy.h"
#include "daScript/simulate/heap.h"

namespace das
{
    #define DAS_BIND_FUN(a)                     decltype(&a), a
    #define DAS_BIND_PROP(BIGTYPE,FIELDNAME)    decltype(&BIGTYPE::FIELDNAME), &BIGTYPE::FIELDNAME

    template <class T, class M> M get_member_type(M T:: *);
    #define GET_TYPE_OF(mem) decltype(das::get_member_type(mem))
    #define DAS_BIND_FIELD(BIGTYPE,FIELDNAME)   GET_TYPE_OF(&BIGTYPE::FIELDNAME),offsetof(BIGTYPE,FIELDNAME)

    #ifndef DAS_ENABLE_STACK_WALK
    #define DAS_ENABLE_STACK_WALK   1
    #endif

    #ifndef DAS_ENABLE_EXCEPTIONS
    #define DAS_ENABLE_EXCEPTIONS   0
    #endif

    #define MAX_FOR_ITERATORS   16

    class Context;
    struct SimNode;
    struct Block;

    struct GlobalVariable {
        vec4f            value;
        char *          name;
        VarInfo *       debug;
        SimNode *       init;
        uint32_t        size;
    };

    struct SimFunction {
        char *      name;
        SimNode *   code;
        uint32_t    stackSize;
        FuncInfo *  debug;
    };

    struct SimNode {
        SimNode ( const LineInfo & at ) : debug(at) {}
        virtual vec4f eval ( Context & ) = 0;
        virtual char *      evalPtr ( Context & context );
        virtual bool        evalBool ( Context & context );
        virtual float       evalFloat ( Context & context );
        virtual double      evalDouble ( Context & context );
        virtual int32_t     evalInt ( Context & context );
        virtual uint32_t    evalUInt ( Context & context );
        virtual int64_t     evalInt64 ( Context & context );
        virtual uint64_t    evalUInt64 ( Context & context );
        LineInfo debug;
    protected:
        virtual ~SimNode() {}
    };

    struct Prologue {
        union {
            struct {
                vec4f *     arguments;
                FuncInfo *  info;
                int32_t     line;
            };
            vec4f           dummy;
        };
    };
    static_assert((sizeof(Prologue) & 0xf)==0, "it has to be 16 byte aligned");

    struct BlockArguments {
        vec4f *     arguments;
        char *      copyOrMoveResult;
    };

    enum EvalFlags : uint32_t {
        stopForBreak        = 1 << 0
    ,   stopForReturn       = 1 << 1
    };

#if DAS_ENABLE_EXCEPTIONS
    class dasException : public runtime_error {
    public:
        dasException ( const char * why ) : runtime_error(why) {}
    };
#endif

    class Context {
        template <typename TT> friend struct SimNode_GetGlobalR2V;
        friend struct SimNode_GetGlobal;
        friend struct SimNode_TryCatch;
        friend class Program;
    public:
        Context(const string * lines, uint32_t heapSize = 4*1024*1024);
        Context(const Context &) = delete;
        Context & operator = (const Context &) = delete;

        __forceinline vec4f getVariable ( int index ) const {
            assert(index>=0 && index<totalVariables && "variable index out of range");
            return globalVariables[index].value;
        }

        __forceinline void simEnd() {
            thisProgram = nullptr;
            thisHelper = nullptr;
            heapWatermark = heap.getWatermark();
        }

        __forceinline void restart( ) {
            stopFlags = 0;
            exception = nullptr;
            stack.reset();
            heap.setWatermark(heapWatermark);
        }

        __forceinline vec4f eval ( SimFunction * fnPtr, vec4f * args = nullptr, void * res = nullptr ) {
            return callWithCopyOnReturn(fnPtr, args, res, 0);
        }

        void fakeCall ( FuncInfo * info, int line, vec4f * args, char * & EP, char * & SP );

        vec4f evalWithCatch ( SimFunction * fnPtr, vec4f * args = nullptr, void * res = nullptr );
        vec4f evalWithCatch ( SimNode * node );

        void throw_error ( const char * message );

        __forceinline SimFunction * getFunction ( int index ) const {
            return (index>=0 && index<totalFunctions) ? functions + index : nullptr;
        }

        SimFunction * findFunction ( const char * name ) const;
        int findVariable ( const char * name ) const;
        void stackWalk();
        string getStackWalk( bool args = true );
        void runInitScript ( void );

        virtual void to_out ( const char * message );           // output to stdout or equivalent
        virtual void to_err ( const char * message );           // output to stderr or equivalent
        virtual void breakPoint(int column, int line) const;    // what to do in case of breakpoint

        __forceinline vec4f * abiArguments() {
            return abiArg;
        }

        __forceinline vec4f & abiResult() {
            return result;
        }

        __forceinline char * abiCopyOrMoveResult() {
            return (char *) abiCMRES;
        }

        __forceinline vec4f call(SimFunction * fn, vec4f * args, int line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error("stack overflow");
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg;
            abiArg = args;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->arguments = args;
            pp->info = fn->debug;
            pp->line = line;
#endif
            // CALL
            fn->code->eval(*this);
            stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            // POP
            abiArg = aa;
            stack.pop(EP, SP);
            return result;
        }

        __forceinline vec4f callWithCopyOnReturn(SimFunction * fn, vec4f * args, void * cmres, int line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error("stack overflow");
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg; auto acm = abiCMRES;
            abiArg = args; abiCMRES = cmres;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->arguments = args;
            pp->info = fn->debug;
            pp->line = line;
#endif
            // CALL
            fn->code->eval(*this);
            stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            // POP
            abiArg = aa; abiCMRES = acm;
            stack.pop(EP, SP);
            return result;
        }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4701)
#endif
        __forceinline vec4f invoke(const Block &block, vec4f * args, void * cmres ) {
            char * EP, *SP;
            stack.invoke(block.stackOffset, EP, SP);
            BlockArguments * __restrict ba = nullptr;
            BlockArguments saveArguments;
            if ( block.argumentsOffset ) {
                ba = (BlockArguments *) ( stack.bottom() + block.argumentsOffset );
                saveArguments = *ba;
                ba->arguments = args;
                ba->copyOrMoveResult = (char *) cmres;
            }
            vec4f block_result = block.body->eval(*this);
            if ( ba ) {
                *ba = saveArguments;
            }
            stack.pop(EP, SP);
            return block_result;
        }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

        vec4f callEx ( SimFunction * fn, vec4f * args, void * cmres, int line, function<void (SimNode *)> && when );
        vec4f invokeEx ( const Block &block, vec4f * args, void * cmres, function<void (SimNode *)> && when );

        __forceinline const char * getException() const {
            return exception;
        }

    public:
        LinearAllocator    heap;
        NodeAllocator    code;
        NodeAllocator    debugInfo;
        StackAllocator    stack;
        void *            heapWatermark = nullptr;
    public:
        vec4f *         abiArg;
        void *          abiCMRES;
    protected:
        GlobalVariable * globalVariables = nullptr;
        uint32_t globalInitStackSize = 0;
        SimFunction * functions = nullptr;
        int totalVariables = 0;
        int totalFunctions = 0;
        const char * exception = nullptr;
#if !DAS_ENABLE_EXCEPTIONS
        jmp_buf * throwBuf = nullptr;
#endif
    public:
        const string * debugInput = nullptr;
        class Program * thisProgram = nullptr;
        class DebugInfoHelper * thisHelper = nullptr;
    public:
        uint32_t stopFlags = 0;
        vec4f result;
    };

#define DAS_EVAL_NODE               \
    EVAL_NODE(Ptr,char *);          \
    EVAL_NODE(Int,int32_t);         \
    EVAL_NODE(UInt,uint32_t);       \
    EVAL_NODE(Int64,int64_t);       \
    EVAL_NODE(UInt64,uint64_t);     \
    EVAL_NODE(Float,float);         \
    EVAL_NODE(Double,double);       \
    EVAL_NODE(Bool,bool);

#define DAS_NODE(TYPE,CTYPE)                                    \
    virtual vec4f eval ( Context & context ) override {        \
        return cast<CTYPE>::from(compute(context));             \
    }                                                           \
    virtual CTYPE eval##TYPE ( Context & context ) override {   \
        return compute(context);                                \
    }

#define DAS_PTR_NODE    DAS_NODE(Ptr,char *)
#define DAS_BOOL_NODE   DAS_NODE(Bool,bool)
#define DAS_INT_NODE    DAS_NODE(Int,int32_t)
#define DAS_FLOAT_NODE  DAS_NODE(Float,float)
#define DAS_DOUBLE_NODE DAS_NODE(Double,double)

    template <typename TT>
    struct EvalTT { static __forceinline TT eval ( Context & context, SimNode * node ) {
        return cast<TT>::to(node->eval(context)); }};
    template <>
    struct EvalTT<int32_t> { static __forceinline int32_t eval ( Context & context, SimNode * node ) {
        return node->evalInt(context); }};
    template <>
    struct EvalTT<uint32_t> { static __forceinline uint32_t eval ( Context & context, SimNode * node ) {
        return node->evalUInt(context); }};
    template <>
    struct EvalTT<int64_t> { static __forceinline int64_t eval ( Context & context, SimNode * node ) {
        return node->evalInt64(context); }};
    template <>
    struct EvalTT<uint64_t> { static __forceinline uint64_t eval ( Context & context, SimNode * node ) {
        return node->evalUInt64(context); }};
    template <>
    struct EvalTT<float> { static __forceinline float eval ( Context & context, SimNode * node ) {
        return node->evalFloat(context); }};
    template <>
    struct EvalTT<double> { static __forceinline double eval ( Context & context, SimNode * node ) {
        return node->evalDouble(context); }};
    template <>
    struct EvalTT<bool> { static __forceinline bool eval ( Context & context, SimNode * node ) {
        return node->evalBool(context); }};

    // Delete
    struct SimNode_Delete : SimNode {
        SimNode_Delete ( const LineInfo & a, SimNode * s, uint32_t t )
            : SimNode(a), subexpr(s), total(t) {}
        SimNode *   subexpr;
        uint32_t    total;
    };

    // Delete structures
    struct SimNode_DeleteStructPtr : SimNode_Delete {
        SimNode_DeleteStructPtr ( const LineInfo & a, SimNode * s, uint32_t t, uint32_t ss )
            : SimNode_Delete(a,s,t), structSize(ss) {}
        virtual vec4f eval ( Context & context ) override;
        uint32_t    structSize;
    };

    // New handle, default
    template <typename TT>
    struct SimNode_NewHandle : SimNode {
        DAS_PTR_NODE;
        SimNode_NewHandle ( const LineInfo & a ) : SimNode(a) {}
        __forceinline char * compute ( Context & ) {
            return (char *) new TT();
        }
    };

    // Delete handle, default
    template <typename TT>
    struct SimNode_DeleteHandlePtr : SimNode_Delete {
        SimNode_DeleteHandlePtr ( const LineInfo & a, SimNode * s, uint32_t t )
            : SimNode_Delete(a,s,t) {}
        virtual vec4f eval ( Context & context ) override {
            auto pH = (TT **) subexpr->evalPtr(context);
            for ( uint32_t i=0; i!=total; ++i, pH++ ) {
                if ( *pH ) {
                    delete * pH;
                    *pH = nullptr;
                }
            }
            return v_zero();
        }
    };

    // MakeBlock
    struct SimNode_MakeBlock : SimNode {
        SimNode_MakeBlock ( const LineInfo & at, SimNode * s, uint32_t a )
            : SimNode(at), subexpr(s), argStackTop(a) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode *       subexpr;
        uint32_t        argStackTop;
    };

    // ASSERT
    struct SimNode_Assert : SimNode {
        SimNode_Assert ( const LineInfo & at, SimNode * s, const char * m ) : SimNode(at), subexpr(s), message(m) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode *       subexpr;
        const char *    message;
    };

    // VECTOR SWIZZLE
    // TODO: make at least 3 different versions
    struct SimNode_Swizzle : SimNode {
        SimNode_Swizzle ( const LineInfo & at, SimNode * rv, uint8_t * fi )
            : SimNode(at), value(rv) {
                fields[0] = fi[0];
                fields[1] = fi[1];
                fields[2] = fi[2];
                fields[3] = fi[3];
            }
        virtual vec4f eval ( Context & context ) override;
        SimNode *   value;
        uint8_t     fields[4];
    };

    // FIELD .
    struct SimNode_FieldDeref : SimNode {
        DAS_PTR_NODE;
        SimNode_FieldDeref ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode(at), value(rv), offset(of) {}
        __forceinline char * compute ( Context & context ) {
            return value->evalPtr(context) + offset;
        }
        SimNode *   value;
        uint32_t    offset;
    };

    template <typename TT>
    struct SimNode_FieldDerefR2V : SimNode_FieldDeref {
        SimNode_FieldDerefR2V ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        virtual vec4f eval ( Context & context ) override {
            auto prv = value->evalPtr(context);
            TT * pR = (TT *)( prv + offset );
            return cast<TT>::from(*pR);

        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            auto prv = value->evalPtr(context);                     \
            return * (CTYPE *)( prv + offset );                     \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    // PTR FIELD .
    struct SimNode_PtrFieldDeref : SimNode {
        DAS_PTR_NODE;
        SimNode_PtrFieldDeref(const LineInfo & at, SimNode * rv, uint32_t of) : SimNode(at), value(rv), offset(of) {}
        __forceinline char * compute(Context & context) {
            auto prv = value->evalPtr(context);
            if (prv) {
                return prv + offset;
            }
            else {
                context.throw_error("dereferencing null pointer");
                return nullptr;
            }
        }
        SimNode *   value;
        uint32_t    offset;
    };

    template <typename TT>
    struct SimNode_PtrFieldDerefR2V : SimNode_PtrFieldDeref {
        SimNode_PtrFieldDerefR2V(const LineInfo & at, SimNode * rv, uint32_t of) : SimNode_PtrFieldDeref(at, rv, of) {}
        virtual vec4f eval(Context & context) override {
            auto prv = value->evalPtr(context);
            if (prv) {
                TT * pR = (TT *)(prv + offset);
                return cast<TT>::from(*pR);
            }
            else {
                context.throw_error("dereferencing null pointer");
                return v_zero();
            }
        }
        virtual char * evalPtr(Context & context) override {
            auto prv = value->evalPtr(context);
            if (prv) {
                return *(char **)(prv + offset);
            }
            else {
                context.throw_error("dereferencing null pointer");
                return nullptr;
            }
        }
    };

    // FIELD ?.
    struct SimNode_SafeFieldDeref : SimNode_FieldDeref {
        DAS_PTR_NODE;
        SimNode_SafeFieldDeref ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        __forceinline char * compute ( Context & context ) {
            auto prv = value->evalPtr(context);
            return prv ? prv + offset : nullptr;
        }
    };

    // FIELD ?.->
    struct SimNode_SafeFieldDerefPtr : SimNode_FieldDeref {
        DAS_PTR_NODE;
        SimNode_SafeFieldDerefPtr ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        __forceinline char * compute ( Context & context ) {
            char ** prv = (char **) value->evalPtr(context);
            return prv ? *(prv + offset) : nullptr;
        }
    };

    // AT (INDEX)
    struct SimNode_At : SimNode {
        DAS_PTR_NODE;
        SimNode_At ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t strd, uint32_t rng )
            : SimNode(at), value(rv), index(idx), stride(strd), range(rng) {}
        __forceinline char * compute (Context & context) {
            auto pValue = value->evalPtr(context);
            uint32_t idx = cast<uint32_t>::to(index->eval(context));
            if (idx >= range) {
                context.throw_error("index out of range");
                return nullptr;
            } else {
                return pValue + idx*stride;
            }
        }
        SimNode * value, * index;
        uint32_t  stride, range;
    };

    // AT (INDEX)
    template <typename TT>
    struct SimNode_AtVector;

#define SIM_NODE_AT_VECTOR(TYPE,CTYPE)                                                          \
    template <>                                                                                 \
    struct SimNode_AtVector<CTYPE> : SimNode {                                                  \
        SimNode_AtVector ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t rng )     \
            : SimNode(at), value(rv), index(idx), range(rng) {}                                 \
        __forceinline CTYPE compute ( Context & context ) {                                     \
            auto vec = value->eval(context);                                                    \
            uint32_t idx = cast<uint32_t>::to(index->eval(context));                            \
            if (idx >= range) {                                                                 \
                context.throw_error("index out of range");                                      \
                return (CTYPE) 0;                                                               \
            } else {                                                                            \
                CTYPE * pv = (CTYPE *) &vec;                                                    \
                return pv[idx];                                                                 \
            }                                                                                   \
        }                                                                                       \
        DAS_NODE(TYPE, CTYPE)                                                                   \
        SimNode * value, * index;                                                               \
        uint32_t  range;                                                                        \
    };

SIM_NODE_AT_VECTOR(Int,   int32_t)
SIM_NODE_AT_VECTOR(UInt,  uint32_t)
SIM_NODE_AT_VECTOR(Float, float)

    template <int nElem>
    struct EvalBlock { static __forceinline void eval(Context & context, SimNode ** arguments, vec4f * argValues) {
            for (int i = 0; i != nElem && !context.stopFlags; ++i)
                argValues[i] = arguments[i]->eval(context);
    }};
    template <>    struct EvalBlock<0> { static __forceinline void eval(Context &, SimNode **, vec4f *) {} };
    template <> struct EvalBlock<1> {    static __forceinline void eval(Context & context, SimNode ** arguments, vec4f * argValues) {
                                    argValues[0] = arguments[0]->eval(context);
    }};
    template <> struct EvalBlock<2> {    static __forceinline void eval(Context & context, SimNode ** arguments, vec4f * argValues) {
                                    argValues[0] = arguments[0]->eval(context);
            if (!context.stopFlags) argValues[1] = arguments[1]->eval(context);
    }};
    template <> struct EvalBlock<3> {    static __forceinline void eval(Context & context, SimNode ** arguments, vec4f * argValues) {
                                    argValues[0] = arguments[0]->eval(context);
            if (!context.stopFlags) argValues[1] = arguments[1]->eval(context);
            if (!context.stopFlags) argValues[2] = arguments[2]->eval(context);
    }};
    template <> struct EvalBlock<4> {    static __forceinline void eval(Context & context, SimNode ** arguments, vec4f * argValues) {
                                    argValues[0] = arguments[0]->eval(context);
            if (!context.stopFlags) argValues[1] = arguments[1]->eval(context);
            if (!context.stopFlags) argValues[2] = arguments[2]->eval(context);
            if (!context.stopFlags) argValues[3] = arguments[3]->eval(context);
    }};

    // FUNCTION CALL
    struct SimNode_CallBase : SimNode {
        SimNode_CallBase ( const LineInfo & at ) : SimNode(at) {}
        __forceinline void evalArgs ( Context & context, vec4f * argValues ) {
            for ( int i=0; i!=nArguments && !context.stopFlags; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return cast<CTYPE>::to(eval(context));                  \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
        SimNode ** arguments;
        TypeInfo ** types;
        SimFunction * fnPtr;
        int32_t  nArguments;
        uint32_t stackTop;
    };

    // FUNCTION CALL via FASTCALL convention
    template <int argCount>
    struct SimNode_FastCall : SimNode_CallBase {
        SimNode_FastCall ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f argValues[argCount ? argCount : 1];
            EvalBlock<argCount>::eval(context, arguments, argValues);
            auto aa = context.abiArg;
            context.abiArg = argValues;
            auto res = fnPtr->code->eval(context);
            context.stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            context.abiArg = aa;
            return res;
        }
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
                vec4f argValues[argCount ? argCount : 1];                                       \
                EvalBlock<argCount>::eval(context, arguments, argValues);                       \
                auto aa = context.abiArg;                                                       \
                context.abiArg = argValues;                                                     \
                auto res = EvalTT<CTYPE>::eval(context, fnPtr->code);                           \
                context.stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);     \
                context.abiArg = aa;                                                            \
                return res;                                                                     \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
    };

    // FUNCTION CALL
    template <int argCount>
    struct SimNode_Call : SimNode_CallBase {
        SimNode_Call ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f argValues[argCount ? argCount : 1];
            EvalBlock<argCount>::eval(context, arguments, argValues);
            return context.call(fnPtr, argValues, debug.line);
        }
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
                vec4f argValues[argCount ? argCount : 1];                                       \
                EvalBlock<argCount>::eval(context, arguments, argValues);                       \
                return cast<CTYPE>::to(context.call(fnPtr, argValues, debug.line));    \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
    };

    // FUNCTION CALL with copy-or-move-on-return
    template <int argCount>
    struct SimNode_CallAndCopyOrMove : SimNode_CallBase {
        DAS_PTR_NODE;
        SimNode_CallAndCopyOrMove ( const LineInfo & at ) : SimNode_CallBase(at) {}
        __forceinline char * compute ( Context & context ) {
                vec4f argValues[argCount ? argCount : 1];
                EvalBlock<argCount>::eval(context, arguments, argValues);
                auto cmres = context.stack.sp() + stackTop;
                return cast<char *>::to(context.callWithCopyOnReturn(fnPtr, argValues, cmres, debug.line));
        }
    };

    // Invoke
    template <int argCount>
    struct SimNode_Invoke : SimNode_CallBase {
        SimNode_Invoke ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f argValues[argCount ? argCount : 1];
            EvalBlock<argCount>::eval(context, arguments, argValues);
            Block block = cast<Block>::to(argValues[0]);
            if ( argCount>1 ) {
                return context.invoke(block, argValues + 1, nullptr);
            } else {
                return context.invoke(block, nullptr, nullptr);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)                                                                   \
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f argValues[argCount ? argCount : 1];                                           \
            EvalBlock<argCount>::eval(context, arguments, argValues);                           \
            Block block = cast<Block>::to(argValues[0]);                                        \
            if ( argCount>1 ) {                                                                 \
                return cast<CTYPE>::to(context.invoke(block, argValues + 1, nullptr));          \
            } else {                                                                            \
                return cast<CTYPE>::to(context.invoke(block, nullptr, nullptr));                \
            }                                                                                   \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
    };

    // Invoke with copy-or-move-on-return
    template <int argCount>
    struct SimNode_InvokeAndCopyOrMove : SimNode_CallBase {
        SimNode_InvokeAndCopyOrMove ( const LineInfo & at, uint32_t sp )
            : SimNode_CallBase(at) { stackTop = sp; }
        virtual vec4f eval ( Context & context ) override {
            vec4f argValues[argCount ? argCount : 1];
            EvalBlock<argCount>::eval(context, arguments, argValues);
            Block block = cast<Block>::to(argValues[0]);
            auto cmres = context.stack.sp() + stackTop;
            if ( argCount>1 ) {
                return context.invoke(block, argValues + 1, cmres);
            } else {
                return context.invoke(block, nullptr, cmres);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)                                                                   \
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f argValues[argCount ? argCount : 1];                                           \
            EvalBlock<argCount>::eval(context, arguments, argValues);                           \
            Block block = cast<Block>::to(argValues[0]);                                        \
            auto cmres = context.stack.sp() + stackTop;                                         \
            if ( argCount>1 ) {                                                                 \
                return cast<CTYPE>::to(context.invoke(block, argValues + 1, cmres));            \
            } else {                                                                            \
                return cast<CTYPE>::to(context.invoke(block, nullptr, cmres));                  \
            }                                                                                   \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
    };

    // StringBuilder
    struct SimNode_StringBuilder : SimNode_CallBase {
        SimNode_StringBuilder ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override;
        TypeInfo ** types;
    };

    // CAST
    template <typename CastTo, typename CastFrom>
    struct SimNode_Cast : SimNode_CallBase {
        SimNode_Cast ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f res = arguments[0]->eval(context);
            CastTo value = (CastTo) cast<CastFrom>::to(res);
            return cast<CastTo>::from(value);
        }
    };

    // LEXICAL CAST
    template <typename CastFrom>
    struct SimNode_LexicalCast : SimNode_CallBase {
        SimNode_LexicalCast ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f res = arguments[0]->eval(context);
            auto str = to_string ( cast<CastFrom>::to(res) );
            auto cpy = context.heap.allocateString(str);
            if ( !cpy ) {
                context.throw_error("can't cast to string, out of heap");
                return v_zero();
            }
            return cast<char *>::from(cpy);
        }
    };

    // "DEBUG"
    struct SimNode_Debug : SimNode {
        SimNode_Debug ( const LineInfo & at, SimNode * s, TypeInfo * ti, char * msg )
            : SimNode(at), subexpr(s), typeInfo(ti), message(msg) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode *       subexpr;
        TypeInfo *      typeInfo;
        const char *    message;
    };

    // LOCAL VARIABLE "GET"
    struct SimNode_GetLocal : SimNode {
        DAS_PTR_NODE;
        SimNode_GetLocal(const LineInfo & at, uint32_t sp) : SimNode(at), stackTop(sp) {}
        __forceinline char * compute ( Context & context ) {
            return context.stack.sp() + stackTop;
        }
        uint32_t stackTop;
    };

    template <typename TT>
    struct SimNode_GetLocalR2V : SimNode_GetLocal {
        SimNode_GetLocalR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp)  {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = (TT *)(context.stack.sp() + stackTop);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return *(CTYPE *)(context.stack.sp() + stackTop);       \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    // WHEN LOCAL VARIABLE STORES REFERENCE
    struct SimNode_GetLocalRef : SimNode_GetLocal {
        DAS_PTR_NODE;
        SimNode_GetLocalRef(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp) {}
        __forceinline char * compute ( Context & context ) {
            return *(char **)(context.stack.sp() + stackTop);
        }
    };

    template <typename TT>
    struct SimNode_GetLocalRefR2V : SimNode_GetLocalRef {
        SimNode_GetLocalRefR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocalRef(at,sp) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = *(TT **)(context.stack.sp() + stackTop);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return **(CTYPE **)(context.stack.sp() + stackTop);     \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    template <typename TT>
    struct SimNode_CopyLocal2LocalT : SimNode {
        SimNode_CopyLocal2LocalT(const LineInfo & at, uint32_t spL, uint32_t spR)
            : SimNode(at), stackTopLeft(spL), stackTopRight(spR) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) ( context.stack.sp() + stackTopLeft );
            TT * pr = (TT *) ( context.stack.sp() + stackTopRight );
            *pl = *pr;
            return v_zero();
        }
        SimNode * r;
        uint32_t stackTopLeft, stackTopRight;
    };

    template <typename TT>
    struct SimNode_SetLocalRefT : SimNode {
        SimNode_SetLocalRefT(const LineInfo & at, SimNode * rv, uint32_t sp)
            : SimNode(at), r(rv), stackTop(sp) {}
        virtual vec4f eval ( Context & context ) override {
            auto pr = (TT *) r->evalPtr(context);
            TT * pl = (TT *) ( context.stack.sp() + stackTop );
            *pl = *pr;
            return v_zero();
        }
        SimNode * r;
        uint32_t stackTop;
    };

    template <typename TT>
    struct SimNode_SetLocalValueT : SimNode {
        SimNode_SetLocalValueT(const LineInfo & at, SimNode * rv, uint32_t sp)
        : SimNode(at), r(rv), stackTop(sp) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) ( context.stack.sp() + stackTop );
            *pl = EvalTT<TT>::eval(context, r);
            return v_zero();
        }
        SimNode * r;
        uint32_t stackTop;
    };


    // ZERO MEMORY OF UNITIALIZED LOCAL VARIABLE
    struct SimNode_InitLocal : SimNode {
        SimNode_InitLocal(const LineInfo & at, uint32_t sp, uint32_t sz) : SimNode(at), stackTop(sp), size(sz) {}
        virtual vec4f eval ( Context & context ) override {
            memset(context.stack.sp() + stackTop, 0, size);
            return v_zero();
        }
        uint32_t stackTop, size;
    };

    // ARGUMENT VARIABLE "GET"
    struct SimNode_GetArgument : SimNode {
        SimNode_GetArgument ( const LineInfo & at, int32_t i ) : SimNode(at), index(i) {}
        virtual vec4f eval ( Context & context ) override {
            return context.abiArguments()[index];
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return cast<CTYPE>::to(context.abiArguments()[index]);  \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
        int32_t index;
    };

    struct SimNode_GetArgumentRef : SimNode_GetArgument {
        DAS_PTR_NODE;
        SimNode_GetArgumentRef(const LineInfo & at, int32_t i) : SimNode_GetArgument(at,i) {}
        __forceinline char * compute(Context & context) {
            return (char *)(&context.abiArguments()[index]);
        }
    };

    template <typename TT>
    struct SimNode_GetArgumentR2V : SimNode_GetArgument {
        SimNode_GetArgumentR2V ( const LineInfo & at, int32_t i ) : SimNode_GetArgument(at,i) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = cast<TT *>::to(context.abiArguments()[index]);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                               \
        virtual CTYPE eval##TYPE ( Context & context ) override {           \
            return * cast<CTYPE *>::to(context.abiArguments()[index]);      \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    // BLOCK VARIABLE "GET"
    struct SimNode_GetBlockArgument : SimNode {
        SimNode_GetBlockArgument ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode(at), index(i), stackTop(sp) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * args = *((vec4f **)(context.stack.sp() + stackTop));
            return args[index];
        }
#define EVAL_NODE(TYPE,CTYPE)                                               \
        virtual CTYPE eval##TYPE ( Context & context ) override {           \
            vec4f * args = *((vec4f **)(context.stack.sp() + stackTop));    \
            return cast<CTYPE>::to(args[index]);                            \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
        int32_t     index;
        uint32_t    stackTop;
    };

    template <typename TT>
    struct SimNode_GetBlockArgumentR2V : SimNode_GetBlockArgument {
        SimNode_GetBlockArgumentR2V ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode_GetBlockArgument(at,i,sp) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * args = *((vec4f **)(context.stack.sp() + stackTop));
            TT * pR = (TT *) cast<char *>::to(args[index]);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                               \
        virtual CTYPE eval##TYPE ( Context & context ) override {           \
            vec4f * args = *((vec4f **)(context.stack.sp() + stackTop));    \
            return * cast<CTYPE *>::to(args[index]);                        \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    struct SimNode_GetBlockArgumentRef : SimNode_GetBlockArgument {
        DAS_PTR_NODE;
        SimNode_GetBlockArgumentRef(const LineInfo & at, int32_t i, uint32_t sp)
            : SimNode_GetBlockArgument(at,i,sp) {}
        __forceinline char * compute(Context & context) {
            vec4f * args = *((vec4f **)(context.stack.sp() + stackTop));
            return (char *)(&args[index]);
        }
    };

    // GLOBAL VARIABLE "GET"
    struct SimNode_GetGlobal : SimNode {
        SimNode_GetGlobal ( const LineInfo & at, int32_t i ) : SimNode(at), index(i) {}
        virtual vec4f eval ( Context & context ) override {
            return context.globalVariables[index].value;
        }
        virtual char * evalPtr(Context & context) override {
            return cast<char *>::to(context.globalVariables[index].value);
        }
        int32_t index;
    };

    template <typename TT>
    struct SimNode_GetGlobalR2V : SimNode_GetGlobal {
        SimNode_GetGlobalR2V ( const LineInfo & at, int32_t i ) : SimNode_GetGlobal(at,i) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = cast<TT *>::to(context.globalVariables[index].value);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                                   \
        virtual CTYPE eval##TYPE ( Context & context ) override {               \
            return *cast<CTYPE *>::to(context.globalVariables[index].value);    \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    // TRY-CATCH
    struct SimNode_TryCatch : SimNode {
        SimNode_TryCatch ( const LineInfo & at, SimNode * t, SimNode * c ) : SimNode(at), try_block(t), catch_block(c) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode * try_block, * catch_block;
    };

    // RETURN
    struct SimNode_Return : SimNode {
        SimNode_Return ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode * subexpr;
    };

    struct SimNode_ReturnConst : SimNode {
        SimNode_ReturnConst ( const LineInfo & at, vec4f v ) : SimNode(at), value(v) {}
        virtual vec4f eval ( Context & context ) override;
        vec4f value;
    };

    struct SimNode_ReturnAndCopy : SimNode_Return {
        SimNode_ReturnAndCopy ( const LineInfo & at, SimNode * s, uint32_t sz )
            : SimNode_Return(at,s), size(sz) {}
        virtual vec4f eval ( Context & context ) override;
        uint32_t size;
    };

    struct SimNode_ReturnAndMove : SimNode_ReturnAndCopy {
        SimNode_ReturnAndMove ( const LineInfo & at, SimNode * s, uint32_t sz )
            : SimNode_ReturnAndCopy(at,s,sz) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ReturnReference : SimNode_Return {
        SimNode_ReturnReference ( const LineInfo & at, SimNode * s ) : SimNode_Return(at,s) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ReturnAndCopyFromBlock : SimNode_ReturnAndCopy {
        SimNode_ReturnAndCopyFromBlock ( const LineInfo & at, SimNode * s, uint32_t sz, uint32_t asp )
            : SimNode_ReturnAndCopy(at,s,sz), argStackTop(asp) {}
        virtual vec4f eval ( Context & context ) override;
        uint32_t argStackTop;
    };

    struct SimNode_ReturnAndMoveFromBlock : SimNode_ReturnAndCopyFromBlock {
        SimNode_ReturnAndMoveFromBlock ( const LineInfo & at, SimNode * s, uint32_t sz, uint32_t asp )
            : SimNode_ReturnAndCopyFromBlock(at,s,sz, asp) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ReturnReferenceFromBlock : SimNode_Return {
        SimNode_ReturnReferenceFromBlock ( const LineInfo & at, SimNode * s ) : SimNode_Return(at,s) {}
        virtual vec4f eval ( Context & context ) override;
    };

    // BREAK
    struct SimNode_Break : SimNode {
        SimNode_Break ( const LineInfo & at ) : SimNode(at) {}
        virtual vec4f eval ( Context & context ) override {
            context.stopFlags |= EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    // DEREFERENCE
    template <typename TT>
    struct SimNode_Ref2Value : SimNode {      // &value -> value
        SimNode_Ref2Value ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = (TT *) subexpr->evalPtr(context);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE (Context & context) override {     \
            auto pR = (CTYPE *)subexpr->evalPtr(context);           \
            return *pR;                                             \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
        SimNode * subexpr;
    };

    // POINTER TO REFERENCE (CAST)
    struct SimNode_Ptr2Ref : SimNode {      // ptr -> &value
        DAS_PTR_NODE;
        SimNode_Ptr2Ref ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            if ( !ptr ) {
                context.throw_error("dereferencing null pointer");
            }
            return ptr;
        }
        SimNode * subexpr;
    };

    // let(a:int?) x = a && 0
    template <typename TT>
    struct SimNode_NullCoalescing : SimNode_Ptr2Ref {
        SimNode_NullCoalescing ( const LineInfo & at, SimNode * s, SimNode * dv ) : SimNode_Ptr2Ref(at,s), value(dv) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = (TT *) subexpr->evalPtr(context);
            return pR ? cast<TT>::from(*pR) : value->eval(context);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            auto pR = (CTYPE *) subexpr->evalPtr(context);          \
            return pR ? *pR : value->eval##TYPE(context);           \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE

        SimNode * value;
    };

    // let(a:int?) x = a && default_a
    struct SimNode_NullCoalescingRef : SimNode_Ptr2Ref {
        DAS_PTR_NODE;
        SimNode_NullCoalescingRef ( const LineInfo & at, SimNode * s, SimNode * dv ) : SimNode_Ptr2Ref(at,s), value(dv) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            return ptr ? ptr : value->evalPtr(context);
        }
        SimNode * value;
    };

    // NEW
    struct SimNode_New : SimNode {
        SimNode_New ( const LineInfo & at, int32_t b ) : SimNode(at), bytes(b) {}
        virtual vec4f eval ( Context & context ) override;
        int32_t     bytes;
    };

    // CONST-VALUE
    struct SimNode_ConstValue : SimNode {
        SimNode_ConstValue(const LineInfo & at, vec4f c) : SimNode(at), value(c) { }
        virtual vec4f       eval ( Context & )      override { return value; }
        virtual char *      evalPtr(Context &)      override { return valuePtr; }
        virtual int32_t     evalInt(Context &)      override { return valueI; }
        virtual uint32_t    evalUInt(Context &)     override { return valueU; }
        virtual int64_t     evalInt64(Context &)    override { return valueI64; }
        virtual uint64_t    evalUInt64(Context &)   override { return valueU64; }
        virtual float       evalFloat(Context &)    override { return valueF; }
        virtual double      evalDouble(Context &)   override { return valueLF; }
        virtual bool        evalBool(Context &)     override { return valueB; }
        union {
            vec4f       value;
            char *      valuePtr;
            int32_t     valueI;
            uint32_t    valueU;
            int64_t     valueI64;
            uint64_t    valueU64;
            float       valueF;
            double      valueLF;
            bool        valueB;
        };
    };

    struct SimNode_Zero : SimNode {
        SimNode_Zero(const LineInfo & at) : SimNode(at) { }
        virtual vec4f eval ( Context & ) override {
            return v_zero();
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & ) override {            \
            return 0;                                               \
        }
        DAS_EVAL_NODE
#undef EVAL_NODE
    };

    // COPY REFERENCE (int & a = b)
    struct SimNode_CopyReference : SimNode {
        SimNode_CopyReference(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual vec4f eval ( Context & context ) override {
            char  ** pl = (char **) l->evalPtr(context);
            char * pr = r->evalPtr(context);
            *pl = pr;
            return v_zero();
        }
        SimNode * l, * r;
    };

    // COPY VALUE
    template <typename TT>
    struct SimNode_CopyValue : SimNode {
        SimNode_CopyValue(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) l->evalPtr(context);
            vec4f rr = r->eval(context);
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return v_zero();
        }
        SimNode * l, * r;
    };

    // COPY REFERENCE VALUE
    struct SimNode_CopyRefValue : SimNode {
        SimNode_CopyRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual vec4f eval ( Context & context ) override;
        SimNode * l, * r;
        uint32_t size;
    };

    template <typename TT>
    struct SimNode_CopyRefValueT : SimNode {
        SimNode_CopyRefValueT(const LineInfo & at, SimNode * ll, SimNode * rr)
            : SimNode(at), l(ll), r(rr) {};
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) l->evalPtr(context);
            TT * pr = (TT *) r->evalPtr(context);
            *pl = *pr;
            return v_zero();
        }
        SimNode * l, * r;
    };

    // MOVE REFERENCE VALUE
    struct SimNode_MoveRefValue : SimNode {
        SimNode_MoveRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual vec4f eval ( Context & context ) override;
        SimNode * l, * r;
        uint32_t size;
    };

    struct SimNode_Final : SimNode {
        SimNode_Final ( const LineInfo & a ) : SimNode(a) {}
        __forceinline void evalFinal ( Context & context ) {
            if ( totalFinal ) {
                auto SF = context.stopFlags;
                context.stopFlags = 0;
                for ( uint32_t i=0; i!=totalFinal; ++i ) {
                    finalList[i]->eval(context);
                }
                context.stopFlags = SF;
            }
        }
        SimNode ** finalList = nullptr;
        uint32_t totalFinal = 0;
    };

    // BLOCK
    struct SimNode_Block : SimNode_Final {
        SimNode_Block ( const LineInfo & at ) : SimNode_Final(at) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode ** list = nullptr;
        uint32_t total = 0;
        SimNode ** finalList = nullptr;
        uint32_t totalFinal = 0;
    };

    struct SimNode_ClosureBlock : SimNode_Block {
        SimNode_ClosureBlock ( const LineInfo & at, bool nr, void * ad )
            : SimNode_Block(at), needResult(nr), annotationData(ad) {}
        virtual vec4f eval ( Context & context ) override;
        bool needResult = false;
        void * annotationData = nullptr;
    };

    struct SimNode_MakeLocal : SimNode_Block {
        DAS_PTR_NODE;
        SimNode_MakeLocal ( const LineInfo & at, uint32_t sp )
            : SimNode_Block(at), stackTop(sp) {}
        __forceinline char * compute ( Context & context ) {
            for ( uint32_t i = 0; i!=total && !context.stopFlags; ) {
                list[i++]->eval(context);
            }
            return context.stack.sp() + stackTop;
        }
        uint32_t stackTop;
    };

    // LET
    struct SimNode_Let : SimNode_Block {
        SimNode_Let ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode * subexpr = nullptr;
    };

    // IF-THEN-ELSE (also Cond)
    struct SimNode_IfThenElse : SimNode {
        SimNode_IfThenElse ( const LineInfo & at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual vec4f eval ( Context & context ) override {
            bool cmp = cond->evalBool(context);
            if ( cmp ) {
                return if_true->eval(context);
            } else {
                return if_false->eval(context);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
                bool cmp = cond->evalBool(context);                 \
                if ( cmp ) {                                        \
                    return if_true->eval##TYPE(context);            \
                } else {                                            \
                    return if_false->eval##TYPE(context);           \
                }                                                   \
            }
        DAS_EVAL_NODE
#undef EVAL_NODE
        SimNode * cond, * if_true, * if_false;
    };

    template <typename TT>
    struct SimNode_IfZeroThenElse : SimNode {
        SimNode_IfZeroThenElse ( const LineInfo & at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual vec4f eval ( Context & context ) override {
            auto res = EvalTT<TT>::eval(context,cond);
            if ( res == 0 ) {
                return if_true->eval(context);
            } else {
                return if_false->eval(context);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
                auto res = EvalTT<TT>::eval(context,cond);          \
                if ( res==0 ) {                                     \
                    return if_true->eval##TYPE(context);            \
                } else {                                            \
                    return if_false->eval##TYPE(context);           \
                }                                                   \
            }
        DAS_EVAL_NODE
#undef EVAL_NODE
        SimNode * cond, * if_true, * if_false;
    };

    template <typename TT>
    struct SimNode_IfNotZeroThenElse : SimNode {
        SimNode_IfNotZeroThenElse ( const LineInfo & at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual vec4f eval ( Context & context ) override {
            auto res = EvalTT<TT>::eval(context,cond);
            if ( res != 0 ) {
                return if_true->eval(context);
            } else {
                return if_false->eval(context);
            }
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
                auto res = EvalTT<TT>::eval(context,cond);          \
                if ( res!=0 ) {                                     \
                    return if_true->eval##TYPE(context);            \
                } else {                                            \
                    return if_false->eval##TYPE(context);           \
                }                                                   \
            }
        DAS_EVAL_NODE
#undef EVAL_NODE
        SimNode * cond, * if_true, * if_false;
    };

    // IF-THEN
    struct SimNode_IfThen : SimNode {
        SimNode_IfThen ( const LineInfo & at, SimNode * c, SimNode * t )
            : SimNode(at), cond(c), if_true(t) {}
        virtual vec4f eval ( Context & context ) override {
            bool cmp = cond->evalBool(context);
            if ( cmp ) {
                return if_true->eval(context);
            } else {
                return v_zero();
            }
        }
        SimNode * cond, * if_true;
    };

    template <typename TT>
    struct SimNode_IfZeroThen : SimNode {
        SimNode_IfZeroThen ( const LineInfo & at, SimNode * c, SimNode * t )
            : SimNode(at), cond(c), if_true(t) {}
        virtual vec4f eval ( Context & context ) override {
            auto res = EvalTT<TT>::eval(context,cond);
            if ( res==0 ) {
                return if_true->eval(context);
            } else {
                return v_zero();
            }
        }
        SimNode * cond, * if_true;
    };

    template <typename TT>
    struct SimNode_IfNotZeroThen : SimNode {
        SimNode_IfNotZeroThen ( const LineInfo & at, SimNode * c, SimNode * t )
        : SimNode(at), cond(c), if_true(t) {}
        virtual vec4f eval ( Context & context ) override {
            auto res = EvalTT<TT>::eval(context,cond);
            if ( res != 0 ) {
                return if_true->eval(context);
            } else {
                return v_zero();
            }
        }
        SimNode * cond, * if_true;
    };


    // WHILE
    struct SimNode_While : SimNode_Final {
        SimNode_While ( const LineInfo & at, SimNode * c, SimNode * b ) : SimNode_Final(at), cond(c), body(b) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode * cond, * body;
    };

       template <typename OT, typename Fun, Fun PROP, bool SAFE, typename CTYPE>
    struct SimNode_PropertyImpl : SimNode {
        SimNode_PropertyImpl(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {}
        virtual vec4f eval ( Context & context ) override {
            auto pv = (OT *) subexpr->evalPtr(context);
            if ( !pv ) {
                if ( !SAFE ) {
                    context.throw_error("Property, dereferencing null pointer");
                }
                return v_zero();
            }
            auto value = (pv->*PROP)();
            return cast<CTYPE>::from(value);
        }
        SimNode * subexpr;
    };

#define IMPLEMENT_PROPERTY(TYPE,CTYPE) \
    template <typename OT, typename Fun, Fun PROP, bool SAFE> \
    struct SimNode_PropertyImpl<OT,Fun,PROP,SAFE,CTYPE> : SimNode { \
        DAS_NODE(TYPE,CTYPE); \
        SimNode_PropertyImpl(const LineInfo & a, SimNode * se) : SimNode(a), subexpr(se) {} \
        __forceinline CTYPE compute(Context & context) { \
            auto pv = (OT *) subexpr->evalPtr(context); \
            if ( !pv ) { \
                if ( !SAFE ) { \
                    context.throw_error("Property, dereferencing null pointer"); \
                } \
                return (CTYPE) 0; \
            } \
            return (pv->*PROP)(); \
        } \
        SimNode * subexpr; \
    };

    IMPLEMENT_PROPERTY(Bool,    bool);
    IMPLEMENT_PROPERTY(Int,     int32_t);
    IMPLEMENT_PROPERTY(UInt,    uint32_t);
    IMPLEMENT_PROPERTY(Int64,   int64_t);
    IMPLEMENT_PROPERTY(UInt64,  uint64_t);
    IMPLEMENT_PROPERTY(Float,   float);
    IMPLEMENT_PROPERTY(Ptr,     char *);

    template <typename OT, typename FunT, FunT PROPT, bool SAFET>
    struct SimNode_Property : SimNode_PropertyImpl<OT, FunT, PROPT, SAFET, decltype((((OT *)0)->*PROPT)())> {
        using returnType = decltype((((OT *)0)->*PROPT)());
        SimNode_Property(const LineInfo & a, SimNode * se) :
            SimNode_PropertyImpl<OT,FunT,PROPT,SAFET,returnType>(a,se) {
        }
    };

    // iterator

    struct IteratorContext {
        vec4f value;
        union {
            vec4f tail;
            struct {
                char *  table_end;
                Table * table;
            };
            struct {
                char *  array_end;
                Array * array;
            };
            struct {
                char *  fixed_array_end;
            };
            struct {
                int32_t range_to;
            };
        };
    };

    struct Iterator {
          virtual ~Iterator() {}
        virtual bool first ( Context & context, IteratorContext & itc ) = 0;
        virtual bool next  ( Context & context, IteratorContext & itc ) = 0;
        virtual void close ( Context & context, IteratorContext & itc ) = 0;    // can't throw
    };

    struct SimNode_ForBase : SimNode_Final {
        SimNode_ForBase ( const LineInfo & at ) : SimNode_Final(at) {}
        SimNode *   sources [MAX_FOR_ITERATORS];
        uint32_t    strides [MAX_FOR_ITERATORS];
        uint32_t    stackTop[MAX_FOR_ITERATORS];
        SimNode *   body;
        uint32_t    size;
    };

    struct SimNode_ForWithIteratorBase : SimNode_Final {
        SimNode_ForWithIteratorBase ( const LineInfo & at ) : SimNode_Final(at) {}
        SimNode *   source_iterators[MAX_FOR_ITERATORS];
        SimNode *   body;
        uint32_t    stackTop[MAX_FOR_ITERATORS];
    };

    template <int total>
    struct SimNode_ForWithIterator : SimNode_ForWithIteratorBase {
        SimNode_ForWithIterator ( const LineInfo & at ) : SimNode_ForWithIteratorBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (vec4f *)(context.stack.sp() + stackTop[t]);
            }
            Iterator * sources[total] = {};
            for ( int t=0; t!=total; ++t ) {
                vec4f ll = source_iterators[t]->eval(context);
                sources[t] = cast<Iterator *>::to(ll);
            }
            IteratorContext ph[total];
            SimNode * __restrict pbody = body;
            bool needLoop = true;
            for ( int t=0; t!=total; ++t ) {
                needLoop = sources[t]->first(context, ph[t]) && needLoop;
                if ( context.stopFlags ) goto loopend;
            }
            if ( !needLoop ) goto loopend;
            for ( int i=0; !context.stopFlags; ++i ) {
                for ( int t=0; t!=total; ++t ){
                    *pi[t] = ph[t].value;
                }
                pbody->eval(context);
                for ( int t=0; t!=total; ++t ){
                    if ( !sources[t]->next(context, ph[t]) ) goto loopend;
                    if ( context.stopFlags ) goto loopend;
                }
            }
        loopend:
            evalFinal(context);
            for ( int t=0; t!=total; ++t ) {
                sources[t]->close(context, ph[t]);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForWithIterator<0> : SimNode_ForWithIteratorBase {
        SimNode_ForWithIterator ( const LineInfo & at ) : SimNode_ForWithIteratorBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            evalFinal(context);
            return v_zero();
        }
    };

    template <>
    struct SimNode_ForWithIterator<1> : SimNode_ForWithIteratorBase {
        SimNode_ForWithIterator ( const LineInfo & at ) : SimNode_ForWithIteratorBase(at) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * pi = (vec4f *)(context.stack.sp() + stackTop[0]);
            Iterator * sources;
            vec4f ll = source_iterators[0]->eval(context);
            sources = cast<Iterator *>::to(ll);
            IteratorContext ph;
            bool needLoop = true;
            SimNode * __restrict pbody = body;
            needLoop = sources->first(context, ph) && needLoop;
            if ( context.stopFlags ) goto loopend;
            for ( int i=0; !context.stopFlags; ++i ) {
                *pi = ph.value;
                pbody->eval(context);
                if ( !sources->next(context, ph) ) goto loopend;
                if ( context.stopFlags ) goto loopend;
            }
        loopend:
            evalFinal(context);
            sources->close(context, ph);
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return v_zero();
        }
    };

    struct SimNode_Op1 : SimNode {
        SimNode_Op1 ( const LineInfo & at ) : SimNode(at) {}
        SimNode * x = nullptr;
    };

    struct SimNode_Op2 : SimNode {
        SimNode_Op2 ( const LineInfo & at ) : SimNode(at) {}
        SimNode * l = nullptr;
        SimNode * r = nullptr;
    };

    struct Sim_BoolAnd : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_BoolAnd ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context ) {
            if ( !l->evalBool(context) ) {      // if not left, then false
                return false;
            } else {
                return r->evalBool(context);    // if left, then right
            }
        }
    };

    struct Sim_BoolOr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_BoolOr ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context )  {
            if ( l->evalBool(context) ) {       // if left, then true
                return true;
            } else {
                return r->evalBool(context);    // if not left, then right
            }
        }
    };

#define DEFINE_POLICY(CALL) template <typename SimPolicy> struct Sim_##CALL;

#define IMPLEMENT_OP1_POLICY(CALL,TYPE,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op1 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto val = x->eval##TYPE(context);                          \
            return SimPolicy<CTYPE>::CALL(val,context);                 \
        }                                                               \
    };

#define IMPLEMENT_OP1_FUNCTION_POLICY(CALL,TYPE,CTYPE)                  \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_CallBase {                      \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_CallBase(at) {}    \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto val = arguments[0]->eval##TYPE(context);               \
            return SimPolicy<CTYPE>::CALL(val,context);                 \
        }                                                               \
    };

#define IMPLEMENT_OP1_FUNCTION_POLICY_EX(CALL,TYPE,CTYPE,ATYPE,ACTYPE)        \
    template <>                                                                \
    struct Sim_##CALL <ACTYPE> : SimNode_CallBase {                            \
        DAS_NODE(TYPE,CTYPE);                                                \
        Sim_##CALL ( const LineInfo & at ) : SimNode_CallBase(at) {}        \
        __forceinline CTYPE compute ( Context & context ) {                    \
            auto val = arguments[0]->eval##ATYPE(context);                    \
            return SimPolicy<ACTYPE>::CALL(val,context);                    \
        }                                                                    \
    };

#define IMPLEMENT_OP1_SET_POLICY(CALL,TYPE,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op1 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto val = (CTYPE *) x->evalPtr(context);                   \
            return SimPolicy<CTYPE>::CALL(*val,context);                \
        }                                                               \
    };

#define IMPLEMENT_OP1_EVAL_POLICY(CALL,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op1 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        virtual vec4f eval ( Context & context ) override {             \
            auto val = x->eval(context);                                \
            return SimPolicy<CTYPE>::CALL(val,context);                 \
        }                                                               \
    };

#define IMPLEMENT_OP1_EVAL_FUNCTION_POLICY(CALL,CTYPE)                  \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_CallBase {                      \
        Sim_##CALL ( const LineInfo & at ) : SimNode_CallBase(at) {}    \
        virtual vec4f eval ( Context & context ) override {             \
            auto val = arguments[0]->eval(context);                     \
            return SimPolicy<CTYPE>::CALL(val,context);                 \
        }                                                               \
    };

#define DEFINE_OP1_NUMERIC_INTEGER(CALL)                \
    IMPLEMENT_OP1_POLICY(CALL,Int,int32_t);             \
    IMPLEMENT_OP1_POLICY(CALL,UInt,uint32_t);           \
    IMPLEMENT_OP1_POLICY(CALL,Int64,int64_t);           \
    IMPLEMENT_OP1_POLICY(CALL,UInt64,uint64_t);         \

#define DEFINE_OP1_NUMERIC(CALL);                       \
    DEFINE_OP1_NUMERIC_INTEGER(CALL);                   \
    IMPLEMENT_OP1_POLICY(CALL,Double,double);           \
    IMPLEMENT_OP1_POLICY(CALL,Float,float);

#define DEFINE_OP1_SET_NUMERIC_INTEGER(CALL)            \
    IMPLEMENT_OP1_SET_POLICY(CALL,Int,int32_t);         \
    IMPLEMENT_OP1_SET_POLICY(CALL,UInt,uint32_t);       \
    IMPLEMENT_OP1_SET_POLICY(CALL,Int64,int64_t);       \
    IMPLEMENT_OP1_SET_POLICY(CALL,UInt64,uint64_t);     \

#define DEFINE_OP1_SET_NUMERIC(CALL);                   \
    DEFINE_OP1_SET_NUMERIC_INTEGER(CALL);               \
    IMPLEMENT_OP1_SET_POLICY(CALL,Double,double);       \
    IMPLEMENT_OP1_SET_POLICY(CALL,Float,float);

#define IMPLEMENT_OP2_POLICY(CALL,TYPE,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto lv = l->eval##TYPE(context);                           \
            auto rv = r->eval##TYPE(context);                           \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };

#define IMPLEMENT_OP2_FUNCTION_POLICY(CALL,TYPE,CTYPE)                    \
    template <>                                                            \
    struct Sim_##CALL <CTYPE> : SimNode_CallBase {                      \
        DAS_NODE(TYPE,CTYPE);                                            \
        Sim_##CALL ( const LineInfo & at ) : SimNode_CallBase(at) {}    \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto lv = arguments[0]->eval##TYPE(context);                \
            auto rv = arguments[1]->eval##TYPE(context);                \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);                \
        }                                                                \
    };

#define IMPLEMENT_OP2_SET_POLICY(CALL,TYPE,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto lv = (CTYPE *) l->evalPtr(context);                    \
            auto rv = r->eval##TYPE(context);                           \
            SimPolicy<CTYPE>::CALL(*lv,rv,context);                     \
            return CTYPE();                                             \
        }                                                               \
    };

#define IMPLEMENT_OP2_BOOL_POLICY(CALL,TYPE,CTYPE)                      \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_BOOL_NODE;                                                  \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline bool compute ( Context & context ) {              \
            auto lv = l->eval##TYPE(context);                           \
            auto rv = r->eval##TYPE(context);                           \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };

#define IMPLEMENT_OP2_EVAL_POLICY(CALL,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        virtual vec4f eval ( Context & context ) override {             \
            auto lv = l->eval(context);                                 \
            auto rv = r->eval(context);                                 \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };

#define IMPLEMENT_OP2_EVAL_FUNCTION_POLICY(CALL,CTYPE)                  \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_CallBase {                      \
        Sim_##CALL ( const LineInfo & at ) : SimNode_CallBase(at) {}    \
        virtual vec4f eval ( Context & context ) override {             \
            auto lv = arguments[0]->eval(context);                      \
            auto rv = arguments[1]->eval(context);                      \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };

#define IMPLEMENT_OP2_EVAL_SET_POLICY(CALL,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        virtual vec4f eval ( Context & context ) override {             \
            auto lv = l->evalPtr(context);                              \
            auto rv = r->eval(context);                                 \
            SimPolicy<CTYPE>::CALL(lv,rv,context);                      \
            return v_zero();                                            \
        }                                                               \
    };

#define IMPLEMENT_OP2_EVAL_BOOL_POLICY(CALL,CTYPE)                      \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_BOOL_NODE;                                                  \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline bool compute ( Context & context ) {              \
            auto lv = l->eval(context);                                 \
            auto rv = r->eval(context);                                 \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };

#define DEFINE_OP2_NUMERIC_INTEGER(CALL)                \
    IMPLEMENT_OP2_POLICY(CALL,Int,int32_t);             \
    IMPLEMENT_OP2_POLICY(CALL,UInt,uint32_t);           \
    IMPLEMENT_OP2_POLICY(CALL,Int64,int64_t);           \
    IMPLEMENT_OP2_POLICY(CALL,UInt64,uint64_t);         \

#define DEFINE_OP2_NUMERIC(CALL);                       \
    DEFINE_OP2_NUMERIC_INTEGER(CALL);                   \
    IMPLEMENT_OP2_POLICY(CALL,Double,double);           \
    IMPLEMENT_OP2_POLICY(CALL,Float,float);

#define DEFINE_OP2_FUNCTION_NUMERIC_INTEGER(CALL)                \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,Int,int32_t);             \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,UInt,uint32_t);           \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,Int64,int64_t);           \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,UInt64,uint64_t);         \

#define DEFINE_OP2_FUNCTION_NUMERIC(CALL);                       \
    DEFINE_OP2_FUNCTION_NUMERIC_INTEGER(CALL);                   \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,Double,double);           \
    IMPLEMENT_OP2_FUNCTION_POLICY(CALL,Float,float);

#define DEFINE_OP2_BOOL_NUMERIC_INTEGER(CALL)           \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Int,int32_t);        \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,UInt,uint32_t);      \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Int64,int64_t);      \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,UInt64,uint64_t);    \

#define DEFINE_OP2_BOOL_NUMERIC(CALL);                  \
    DEFINE_OP2_BOOL_NUMERIC_INTEGER(CALL);              \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Double,double);      \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Float,float);

#define DEFINE_OP2_SET_NUMERIC_INTEGER(CALL)            \
    IMPLEMENT_OP2_SET_POLICY(CALL,Int,int32_t);         \
    IMPLEMENT_OP2_SET_POLICY(CALL,UInt,uint32_t);       \
    IMPLEMENT_OP2_SET_POLICY(CALL,Int64,int64_t);       \
    IMPLEMENT_OP2_SET_POLICY(CALL,UInt64,uint64_t);     \

#define DEFINE_OP2_SET_NUMERIC(CALL);                   \
    DEFINE_OP2_SET_NUMERIC_INTEGER(CALL);               \
    IMPLEMENT_OP2_SET_POLICY(CALL,Double,double);       \
    IMPLEMENT_OP2_SET_POLICY(CALL,Float,float);

#define DEFINE_OP2_BASIC_POLICY(TYPE,CTYPE)             \
    IMPLEMENT_OP2_BOOL_POLICY(Equ, TYPE, CTYPE);        \
    IMPLEMENT_OP2_BOOL_POLICY(NotEqu, TYPE, CTYPE);

#define DEFINE_OP2_EVAL_BASIC_POLICY(CTYPE)             \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ, CTYPE);         \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu, CTYPE);

#define DEFINE_OP2_EVAL_ORDERED_POLICY(CTYPE)           \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(LessEqu,CTYPE);      \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(GtEqu,CTYPE);        \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Less,CTYPE);         \
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Gt,CTYPE);

#define DEFINE_OP2_EVAL_GROUPBYADD_POLICY(CTYPE)        \
    IMPLEMENT_OP2_EVAL_POLICY(Add, CTYPE);              \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetAdd, CTYPE);

#define DEFINE_OP2_EVAL_NUMERIC_POLICY(CTYPE)           \
    DEFINE_OP2_EVAL_GROUPBYADD_POLICY(CTYPE);           \
    IMPLEMENT_OP1_EVAL_POLICY(Unp, CTYPE);              \
    IMPLEMENT_OP1_EVAL_POLICY(Unm, CTYPE);              \
    IMPLEMENT_OP2_EVAL_POLICY(Sub, CTYPE);              \
    IMPLEMENT_OP2_EVAL_POLICY(Div, CTYPE);              \
    IMPLEMENT_OP2_EVAL_POLICY(Mul, CTYPE);              \
    IMPLEMENT_OP2_EVAL_POLICY(Mod, CTYPE);              \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetSub, CTYPE);       \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetDiv, CTYPE);       \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetMul, CTYPE);       \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetMod, CTYPE);

#define DEFINE_OP2_EVAL_VECNUMERIC_POLICY(CTYPE)        \
    IMPLEMENT_OP2_EVAL_POLICY(DivVecScal, CTYPE);       \
    IMPLEMENT_OP2_EVAL_POLICY(MulVecScal, CTYPE);       \
    IMPLEMENT_OP2_EVAL_POLICY(DivScalVec, CTYPE);       \
    IMPLEMENT_OP2_EVAL_POLICY(MulScalVec, CTYPE);       \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetDivScal, CTYPE);   \
    IMPLEMENT_OP2_EVAL_SET_POLICY(SetMulScal, CTYPE);

#define DEFINE_VECTOR_POLICY(CTYPE)             \
    DEFINE_OP2_EVAL_BASIC_POLICY(CTYPE);        \
    DEFINE_OP2_EVAL_NUMERIC_POLICY(CTYPE);      \
    DEFINE_OP2_EVAL_VECNUMERIC_POLICY(CTYPE);

    // unary
    DEFINE_POLICY(Unp);
    DEFINE_POLICY(Unm);
    DEFINE_POLICY(Inc);
    DEFINE_POLICY(Dec);
    DEFINE_POLICY(IncPost);
    DEFINE_POLICY(DecPost);
    DEFINE_POLICY(BinNot);
    DEFINE_POLICY(BoolNot);
    // binary
    // +,-,*,/,%
    DEFINE_POLICY(Add);
    DEFINE_POLICY(Sub);
    DEFINE_POLICY(Mul);
    DEFINE_POLICY(Div);
    DEFINE_POLICY(Mod);
    DEFINE_POLICY(SetAdd);
    DEFINE_POLICY(SetSub);
    DEFINE_POLICY(SetMul);
    DEFINE_POLICY(SetDiv);
    DEFINE_POLICY(SetMod);
    // comparisons
    DEFINE_POLICY(Equ);
    DEFINE_POLICY(NotEqu);
    DEFINE_POLICY(LessEqu);
    DEFINE_POLICY(GtEqu);
    DEFINE_POLICY(Less);
    DEFINE_POLICY(Gt);
    DEFINE_POLICY(Min);
    DEFINE_POLICY(Max);
    // binary and, or, xor
    DEFINE_POLICY(BinAnd);
    DEFINE_POLICY(BinOr);
    DEFINE_POLICY(BinXor);
    DEFINE_POLICY(SetBinAnd);
    DEFINE_POLICY(SetBinOr);
    DEFINE_POLICY(SetBinXor);
    // boolean and, or, xor
    DEFINE_POLICY(SetBoolAnd);
    DEFINE_POLICY(SetBoolOr);
    DEFINE_POLICY(SetBoolXor);
    DEFINE_POLICY(BoolXor);
    // vector*scalar, scalar*vector
    DEFINE_POLICY(DivVecScal);
    DEFINE_POLICY(MulVecScal);
    DEFINE_POLICY(DivScalVec);
    DEFINE_POLICY(MulScalVec);
    DEFINE_POLICY(SetBoolAnd);
    DEFINE_POLICY(SetBoolOr);
    DEFINE_POLICY(SetBoolXor);
    DEFINE_POLICY(SetDivScal);
    DEFINE_POLICY(SetMulScal);
}
