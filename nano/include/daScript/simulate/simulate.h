#pragma once

// nano shadow of daScript/simulate/simulate.h.
//
// The real header carries the whole interpreter's Context: debug agents, stack
// walkers, GC roots, job-fork pools, the profiler, JIT hooks, instrumentation.
// Standalone AOT output touches none of that - it needs a stack, two heaps, the
// function and global tables, and a panic path. This header is that subset,
// with the names and member layout of the original so the reused headers
// (aot.h, heap.h, runtime_*.h, data_walker.h) compile against it unmodified.
//
// The rule for editing it: a name here exists because something in the reuse
// set or in generated code refers to it. Nothing is added speculatively, and
// nothing that IS here may be renamed - the reuse set is verbatim upstream.

#include "daScript/misc/platform.h"
#include "daScript/misc/vectypes.h"
#include "daScript/misc/type_name.h"
#include "daScript/misc/arraytype.h"
#include "daScript/simulate/cast.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/heap.h"
#include "daScript/simulate/code_of_policies.h"

#include "daScript/simulate/simulate_visit_op.h"

namespace das
{
    #define DAS_BIND_FUN(a)                     decltype(&a), a
    #define DAS_BIND_MEMBER_FUN(a)              decltype(&a), &a
    #define DAS_BIND_PROP(BIGTYPE,FIELDNAME)    decltype(&BIGTYPE::FIELDNAME), &BIGTYPE::FIELDNAME
    #define DAS_BIND_FIELD(BIGTYPE,FIELDNAME)   decltype(das::declval<BIGTYPE>().FIELDNAME), offsetof(BIGTYPE,FIELDNAME)

    #define DAS_CALL_METHOD(mname)              DAS_BIND_FUN(mname::invoke)

    #ifndef DAS_ENABLE_STACK_WALK
    #define DAS_ENABLE_STACK_WALK   1
    #endif

    #define DAS_PROFILE_NODE
    #define DAS_KEEPALIVE_CALL(context)
    #define DAS_KEEPALIVE_LOOP(context)

    class Context;
    struct SimNode;
    struct Block;
    struct SimVisitor;

    enum class ContextCategory : uint32_t {
        none =              0
    ,   dead =              (1<<0)
    };

    struct GlobalVariable {
        char *          name;
        VarInfo *       debugInfo;
        SimNode *       init;
        uint64_t        mangledNameHash;
        uint32_t        size;
        uint32_t        offset;
        union {
            struct {
                bool    shared : 1;
            };
            uint32_t    flags;
        };
    };

    struct SimFunction {
        char *      name;
        char *      mangledName;
        SimNode *   code;
        FuncInfo *  debugInfo;
        uint64_t    mangledNameHash;
        void *      aotFunction;
        void *      jitFunction;
        uint32_t    stackSize;
        union {
            uint32_t    flags;
            struct {
                bool    aot : 1;
                bool    fastcall : 1;
                bool    builtin : 1;
                bool    jit : 1;
                bool    unsafe : 1;
                bool    cmres : 1;
                bool    pinvoke : 1;
            };
        };
        const LineInfo * getLineInfo() const;
    };

    struct DAS_API SimNode {
        SimNode ( const LineInfo & at ) : debugInfo(at) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code );
        DAS_EVAL_ABI virtual vec4f eval ( Context & ) = 0;
        virtual SimNode * visit ( SimVisitor & vis );
        virtual char *      evalPtr ( Context & context );
        virtual bool        evalBool ( Context & context );
        virtual float       evalFloat ( Context & context );
        virtual double      evalDouble ( Context & context );
        virtual int32_t     evalInt ( Context & context );
        virtual uint32_t    evalUInt ( Context & context );
        virtual int64_t     evalInt64 ( Context & context );
        virtual uint64_t    evalUInt64 ( Context & context );
        LineInfo debugInfo;
        virtual bool rtti_node_isSourceBase() const { return false;  }
        virtual bool rtti_node_isBlock() const { return false; }
        virtual bool rtti_node_isIf() const { return false; }
        virtual bool rtti_node_isInstrument() const { return false; }
        virtual bool rtti_node_isInstrumentFunction() const { return false; }
        virtual bool rtti_node_isJit() const { return false; }
        virtual bool rtti_node_isKeepAlive() const { return false; }
        virtual bool rtti_node_isCallBase() const { return false; }
        virtual bool rtti_node_isErrorMessage() const { return false; }
    protected:
        virtual ~SimNode() {}
    };

    template <typename TT> struct evalNode;
    template <> struct evalNode<bool> { static __forceinline bool eval(Context & context, SimNode * node) { return node->evalBool(context); } };
    template <> struct evalNode<int32_t> { static __forceinline int32_t eval(Context & context, SimNode * node) { return node->evalInt(context); } };
    template <> struct evalNode<uint32_t> { static __forceinline uint32_t eval(Context & context, SimNode * node) { return node->evalUInt(context); } };
    template <> struct evalNode<int64_t> { static __forceinline int64_t eval(Context & context, SimNode * node) { return node->evalInt64(context); } };
    template <> struct evalNode<uint64_t> { static __forceinline uint64_t eval(Context & context, SimNode * node) { return node->evalUInt64(context); } };
    template <> struct evalNode<float> { static __forceinline float eval(Context & context, SimNode * node) { return node->evalFloat(context); } };
    template <> struct evalNode<double> { static __forceinline double eval(Context & context, SimNode * node) { return node->evalDouble(context); } };

    struct alignas(16) Prologue {
        union {
            FuncInfo *  info;
            Block *     block;
        };
        union {
            struct {
                const char * fileName;
                LineInfo *   functionLine;
                int32_t      stackSize;
                union {
                    uint32_t flags;
                    struct {
                        bool is_jit : 1;
                    };
                };
            };
            struct {
                vec4f *     arguments;
                void *      cmres;
                LineInfo *  line;
            };
        };
    };

    struct BlockArguments {
        vec4f *     arguments;
        char *      copyOrMoveResult;
    };

    enum EvalFlags : uint32_t {
        stopForBreak        = 1 << 0
    ,   stopForReturn       = 1 << 1
    ,   stopForContinue     = 1 << 2
    ,   jumpToLabel         = 1 << 3
    ,   yield               = 1 << 4
    };

#define DAS_PROCESS_LOOP_FLAGS_LABELED(beginLabel,endLabel,howtocontinue) \
    {   if (context.stopFlags) { \
        if (context.stopFlags & EvalFlags::stopForContinue) { \
            context.stopFlags &= ~EvalFlags::stopForContinue; \
            howtocontinue; \
        } else if (context.stopFlags&EvalFlags::jumpToLabel && context.gotoLabel<this->totalLabels) { \
            if ((body=this->list+this->labels[context.gotoLabel])>=this->list) { \
                context.stopFlags &= ~EvalFlags::jumpToLabel; \
                goto beginLabel; \
            } \
        } \
        goto endLabel; \
    } }

#define DAS_PROCESS_LOOP_FLAGS(howtocontinue) \
    DAS_PROCESS_LOOP_FLAGS_LABELED(loopbegin,loopend,howtocontinue)

#define DAS_PROCESS_LOOP1_FLAGS(howtocontinue) \
    {   if (context.stopFlags) { \
        if (context.stopFlags & EvalFlags::stopForContinue) { \
            context.stopFlags &= ~EvalFlags::stopForContinue; \
            howtocontinue; \
        } \
        goto loopend; \
    } }

#define DAS_PROCESS_KEEPALIVE_LOOP1_FLAGS(howtocontinue) \
    DAS_PROCESS_LOOP1_FLAGS(howtocontinue)

    struct DAS_API SimVisitor {
        virtual ~SimVisitor () = default;
        virtual void preVisit ( SimNode * ) { }
        virtual void cr () {}
        virtual void op ( const char * /* name */, uint32_t /* sz */ = 0, const string & /* TT */ = string() ) {}
        virtual void sp ( uint32_t /* stackTop */,  const char * /* op */ = "#sp" ) { }
        virtual void arg ( int32_t /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( uint32_t /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( const char * /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( vec4f /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( int64_t /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( uint64_t /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( float /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( double /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( bool /* argV */,  const char * /* argN */  ) { }
        virtual void arg ( Func /* fun */,  const char * /* mangledName */, const char * /* argN */ ) { }
        virtual void arg ( Func /* fun */,  uint32_t /* mangledName */, const char * /* argN */ ) { }
        virtual void sub ( SimNode ** nodes, uint32_t count, const char * );
        virtual SimNode * sub ( SimNode * node, const char * /* opN */ = "subexpr" ) { return node->visit(*this); }
        virtual SimNode * visit ( SimNode * node ) { return node; }
    };

    DAS_API uint64_t getSemanticHash ( SimNode * node, Context * context );

    class DAS_API Context {
    public:
        static constexpr uint32_t CONTEXT_MAGIC = 0xDA514C09;  // "das" + "ctx" + version
        uint32_t context_magic = CONTEXT_MAGIC;
        Context(uint32_t stackSize = 16*1024, bool ph = false);
        Context(const Context &) = delete;
        Context & operator = (const Context &) = delete;
        virtual ~Context();
        void setup(int totalVars, uint32_t globalStringHeapSize, CodeOfPolicies policies, AnnotationArgumentList options);

        uint64_t getGlobalSize() const { return globalsSize; }
        uint64_t getSharedSize() const { return sharedSize; }
        void updateSharedGlobalSize(uint64_t sharedDiff, uint64_t globalDiff) {
            sharedSize += sharedDiff;
            globalsSize += globalDiff;
        }

        __forceinline char * allocateIterator ( uint64_t size, const char * iterName, const LineInfo * at ) {
            auto aptr = heap->impl_allocateIterator(size, iterName);
            if ( !aptr ) throw_out_of_memory(false, size + 16, at);
            return aptr;
        }

        __forceinline void freeIterator ( char * ptr, const LineInfo * ) {
            heap->impl_freeIterator(ptr);
        }

        __forceinline char * allocate ( uint64_t size, const LineInfo * at = nullptr ) {
            auto aptr = heap->impl_allocate(size);
            if ( !aptr && size ) throw_out_of_memory(false, size, at);
            return aptr;
        }

        __forceinline char * reallocate ( char * ptr, uint64_t oldSize, uint64_t size, const LineInfo * at ) {
            auto aptr = heap->impl_reallocate(ptr, oldSize, size);
            if ( !aptr && size ) throw_out_of_memory(false, size, at);
            return aptr;
        }

        __forceinline void free ( char * ptr, uint64_t size, const LineInfo * = nullptr ) {
            heap->impl_free(ptr, size);
        }

        __forceinline char * allocateString ( const char * text, uint64_t length, const LineInfo * at, bool = false ) {
            auto astr = stringHeap->impl_allocateString(this, text, length, at);
            if ( !astr && length ) throw_out_of_memory(true, length+1, at);
            return astr;
        }

        __forceinline char * allocateString ( const string & str, const LineInfo * at, bool = false ) {
            auto astr = stringHeap->impl_allocateString(this, str.c_str(), uint64_t(str.size()), at);
            if ( !astr && str.size() ) throw_out_of_memory(true, uint64_t(str.size()+1), at);
            return astr;
        }

        __forceinline char * allocateTempString ( const char * text, uint64_t length, const LineInfo * at ) {
            return allocateString(text, length, at, /*temp*/true);
        }

        __forceinline bool freeString ( char * ptr, uint64_t length, const LineInfo *, bool = false ) {
            uint64_t size = length + 1;
            size = (size + 15) & ~15;
            if (stringHeap->isOwnPtr(ptr, size)) {
                stringHeap->impl_freeString(ptr, length);
                return true;
            }
            return false;
        }

        __forceinline void freeTempString ( char * ptr, const LineInfo * at ) {
            if ( stringHeap->isIntern() || stringHeap->isReclaimDisabled() ) return;
            if ( stringDisposeQue ) freeString(stringDisposeQue,(uint64_t)strlen(stringDisposeQue),at, /*temp*/true);
            stringDisposeQue = ptr;
        }

        __forceinline void * getVariable ( int index ) const {
            if ( uint32_t(index)<uint32_t(totalVariables) ) {
                const auto & gvar = globalVariables[index];
                return (gvar.shared ? shared : globals) + gvar.offset;
            } else {
                return nullptr;
            }
        }

        __forceinline VarInfo * getVariableInfo( int index ) const {
            return (uint32_t(index)<uint32_t(totalVariables)) ? globalVariables[index].debugInfo  : nullptr;
        }

        __forceinline const GlobalVariable getGlobalVariable( int index ) const {
            return globalVariables[index];
        }

        __forceinline void restart( ) {
            stopFlags = 0;
            exception = nullptr;
            last_exception = nullptr;
        }

        __forceinline uint32_t lock() {
            return insideContext ++;
        }

        virtual uint32_t unlock() {
            return insideContext --;
        }

        DAS_EVAL_ABI __forceinline vec4f eval ( const SimFunction * fnPtr, vec4f * args = nullptr, void * res = nullptr ) {
            return callWithCopyOnReturn(fnPtr, args, res, 0);
        }

        DAS_NORETURN_PREFIX void throw_error ( const char * message ) DAS_NORETURN_SUFFIX;
        DAS_NORETURN_PREFIX void throw_error_ex ( DAS_FORMAT_STRING_PREFIX const char * message, ... ) DAS_NORETURN_SUFFIX DAS_FORMAT_PRINT_ATTRIBUTE(2,3);
        DAS_NORETURN_PREFIX void throw_error_at ( const LineInfo & at, DAS_FORMAT_STRING_PREFIX const char * message, ... ) DAS_NORETURN_SUFFIX DAS_FORMAT_PRINT_ATTRIBUTE(3,4);
        DAS_NORETURN_PREFIX void throw_error_at ( const LineInfo * at, DAS_FORMAT_STRING_PREFIX const char * message, ... ) DAS_NORETURN_SUFFIX DAS_FORMAT_PRINT_ATTRIBUTE(3,4);
        DAS_NORETURN_PREFIX void throw_fatal_error ( const char * message, const LineInfo & at ) DAS_NORETURN_SUFFIX;
        DAS_NORETURN_PREFIX void rethrow () DAS_NORETURN_SUFFIX;
        DAS_NORETURN_PREFIX void throw_out_of_memory ( bool stringHeap, uint64_t size, const LineInfo * at=nullptr ) DAS_NORETURN_SUFFIX;

        __forceinline SimFunction * getFunction ( int index ) const {
            return (index>=0 && index<totalFunctions) ? functions + index : nullptr;
        }
        __forceinline int32_t getTotalFunctions() const {
            return totalFunctions;
        }
        __forceinline int32_t getTotalVariables() const {
            return totalVariables;
        }

        __forceinline uint32_t globalOffsetByMangledName ( uint64_t mnh ) const {
            auto it = tabGMnLookup->find(mnh);
            DAS_ASSERT(it!=tabGMnLookup->end());
            return it->second;
        }
        __forceinline uint64_t adBySid ( uint64_t sid ) const {
            auto it = tabAdLookup->find(sid);
            DAS_ASSERT(it!=tabAdLookup->end());
            return it->second;
        }
        __forceinline SimFunction * fnByMangledName ( uint64_t mnh ) {
            if ( mnh==0 ) return nullptr;
            auto it = tabMnLookup->find(mnh);
            return it!=tabMnLookup->end() ? it->second : nullptr;
        }

        SimFunction * findFunction ( const char * name ) const;
        SimFunction * findFunction ( const char * name, bool & isUnique ) const;
        int findVariable ( const char * name ) const;
        void stackWalk ( const LineInfo * at, bool showArguments, bool showLocalVariables );

        virtual void to_out ( const LineInfo * at, int level, const char * message );
        void to_out ( const LineInfo * at, const char * message ) {
            to_out(at, LogLevel::defaultPrint, message);
        }
        virtual void to_err ( const LineInfo * at, const char * message ) {
            to_out(at, LogLevel::error, message);
        }
        virtual void breakPoint(const LineInfo & info, const char * reason = "breakpoint", const char * text = "");

        __forceinline vec4f * abiArguments() {
            return abiArg;
        }

        __forceinline vec4f * abiThisBlockArguments() {
            return abiThisBlockArg;
        }

        __forceinline vec4f & abiResult() {
            return result;
        }

        __forceinline char * abiCopyOrMoveResult() {
            return (char *) abiCMRES;
        }

        DAS_EVAL_ABI __forceinline vec4f call(const SimFunction * fn, vec4f * args, LineInfo * line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error_at(line, "stack overflow while calling %s",fn->mangledName);
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg;
            abiArg = args;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->info = fn->debugInfo;
            pp->arguments = args;
            pp->cmres = nullptr;
            pp->line = line;
#endif
            // CALL
            fn->code->eval(*this);
            stopFlags = 0;
            // POP
            abiArg = aa;
            stack.pop(EP, SP);
            return result;
        }

        DAS_EVAL_ABI __forceinline vec4f callOrFastcall(const SimFunction * fn, vec4f * args, LineInfo * line) {
            if ( fn->fastcall ) {
                auto aa = abiArg;
                abiArg = args;
                result = fn->code->eval(*this);
                stopFlags = 0;
                abiArg = aa;
                return result;
            } else {
                return call(fn, args, line);
            }
        }

        DAS_EVAL_ABI __forceinline vec4f callWithCopyOnReturn(const SimFunction * fn, vec4f * args, void * cmres, LineInfo * line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error_at(line, "stack overflow while calling %s",fn->mangledName);
            }
            // fill prologue
            auto aa = abiArg; auto acm = abiCMRES;
            abiArg = args; abiCMRES = cmres;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->info = fn->debugInfo;
            pp->arguments = args;
            pp->cmres = cmres;
            pp->line = line;
#endif
            // CALL
            fn->code->eval(*this);
            stopFlags = 0;
            // POP
            abiArg = aa; abiCMRES = acm;
            stack.pop(EP, SP);
            return result;
        }

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4701)
#pragma warning(disable:4324)
#endif

        DAS_EVAL_ABI __forceinline vec4f invoke(const Block &block, vec4f * args, void * cmres, LineInfo * line ) {
            char * EP, *SP;
            vec4f * TBA = nullptr;
            char * STB = stack.bottom();
#if DAS_ENABLE_STACK_WALK
            if (!stack.push_invoke(sizeof(Prologue), block.stackOffset, EP, SP)) {
                throw_error_at(line, "stack overflow during invoke");
            }
            Prologue * pp = (Prologue *)stack.ap();
            pp->block = (Block *)(intptr_t(&block) | 1);
            pp->arguments = args;
            pp->cmres = cmres;
            pp->line = line;
#else
            stack.invoke(block.stackOffset, EP, SP);
#endif
            BlockArguments * __restrict ba = nullptr;
            BlockArguments saveArguments;
            if ( block.argumentsOffset || cmres ) {
                ba = (BlockArguments *) ( STB + block.argumentsOffset );
                saveArguments = *ba;
                ba->arguments = args;
                ba->copyOrMoveResult = (char *) cmres;
                TBA = abiThisBlockArg;
                abiThisBlockArg = args;
            }
            vec4f * __restrict saveFunctionArguments = abiArg;
            abiArg = block.functionArguments;
            vec4f block_result = block.body->eval(*this);
            abiArg = saveFunctionArguments;
            if ( ba ) {
                *ba = saveArguments;
                abiThisBlockArg = TBA;
            }
            stack.pop(EP, SP);
            return block_result;
        }
#ifdef _MSC_VER
#pragma warning(pop)
#endif

        template <typename Fn>
        DAS_EVAL_ABI vec4f invokeEx(const Block &block, vec4f * args, void * cmres, Fn && when, LineInfo * line);

        __forceinline const char * getException() const {
            return exception;
        }

        void freeGlobalsAndShared();
        void allocateGlobalsAndShared();

        __forceinline void singleStep ( const LineInfo &, bool ) { }
        __forceinline bool isGlobalPtr ( char * ptr ) const { return globals<=ptr && ptr<(globals+globalsSize); }
        __forceinline bool isSharedPtr ( char * ptr ) const { return shared<=ptr && ptr<(shared+sharedSize); }
    public:
        unique_ptr<StringHeapAllocator>  stringHeap;
        unique_ptr<AnyHeapAllocator>     heap;
        shared_ptr<ConstStringAllocator> constStringHeap;
        shared_ptr<NodeAllocator>       code;
        shared_ptr<DebugInfoAllocator>  debugInfo;
        char *                          stringDisposeQue = nullptr;
        uint64_t *                      annotationData = nullptr;
        char *                          globals = nullptr;
        char *                          shared = nullptr;
        StackAllocator                  stack;
        uint32_t                        insideContext = 0;
        bool                            persistent = false;
        bool                            ownStack = false;
        bool                            shutdown = false;
        bool                            failed = false;
        bool                            verySafeContext = false;    // when true, array and table reserves don't free memory (unless the container's scratch flag or a scratch_* one-shot opts out)
        uint64_t                        maxUnreservedSize = 64ull<<20;  // mirrors CodeOfPolicies::max_unreserved_size
    public:
        vec4f *         abiThisBlockArg;
        vec4f *         abiArg;
        void *          abiCMRES;
    public:
        LineInfo        exceptionAt;
        const char *    exception = nullptr;
        const char *    last_exception = nullptr;
        jmp_buf *       throwBuf = nullptr;
        static constexpr int EXCEPTION_MESSAGE_SIZE = 256;
        char            exceptionMessage[EXCEPTION_MESSAGE_SIZE] = {};
    protected:
        GlobalVariable * globalVariables = nullptr;
        SimFunction * functions = nullptr;
        SimFunction ** initFunctions = nullptr;
        uint64_t sharedSize = 0;
        uint64_t globalsSize = 0;
        uint32_t globalInitStackSize = 0;
        int totalVariables = 0;
        int totalFunctions = 0;
        int totalInitFunctions = 0;
        bool    globalsOwner = true;
        bool    sharedOwner = true;
    public:
        SimNode * aotInitScript = nullptr;
    public:
        shared_ptr<das_hash_map<uint64_t,SimFunction *>> tabMnLookup;
        shared_ptr<das_hash_map<uint64_t,uint32_t>> tabGMnLookup;
        shared_ptr<das_hash_map<uint64_t,uint64_t>> tabAdLookup;
    public:
        vec4f result;
        uint32_t stopFlags = 0;
        uint32_t gotoLabel = 0;
    public:
        recursive_mutex * contextMutex = nullptr;
    public:
        int32_t         fnDepth = 0;
    };

    struct DataWalker;

#define DAS_EVAL_NODE               \
    EVAL_NODE(Ptr,char *);          \
    EVAL_NODE(Int,int32_t);         \
    EVAL_NODE(UInt,uint32_t);       \
    EVAL_NODE(Int64,int64_t);       \
    EVAL_NODE(UInt64,uint64_t);     \
    EVAL_NODE(Float,float);         \
    EVAL_NODE(Double,double);       \
    EVAL_NODE(Bool,bool);

#define DAS_NODE(TYPE,CTYPE)                                         \
    DAS_EVAL_ABI virtual vec4f eval ( das::Context & context ) override {         \
        return das::cast<CTYPE>::from(compute(context));             \
    }                                                                \
    virtual CTYPE eval##TYPE ( das::Context & context ) override {   \
        return compute(context);                                     \
    }

#define DAS_PTR_NODE    DAS_NODE(Ptr,char *)
#define DAS_BOOL_NODE   DAS_NODE(Bool,bool)
#define DAS_INT_NODE    DAS_NODE(Int,int32_t)
#define DAS_FLOAT_NODE  DAS_NODE(Float,float)
#define DAS_DOUBLE_NODE DAS_NODE(Double,double)

#define DAS_SINGLE_STEP(context,at,forceStep)

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
    template <>
    struct EvalTT<char *> { static __forceinline char * eval ( Context & context, SimNode * node ) {
        return node->evalPtr(context); }};

    // ERROR MESSAGE
    struct DAS_API SimNode_WithErrorMessage : SimNode {
        SimNode_WithErrorMessage ( const LineInfo & at, const char * em )
            : SimNode(at), errorMessage(em) {}
        virtual bool rtti_node_isErrorMessage() const override { return true; }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        const char * errorMessage = "";
    };

    // FUNCTION CALL
    struct DAS_API SimNode_CallBase : SimNode_WithErrorMessage {
        SimNode_CallBase ( const LineInfo & at, const char * msg ) : SimNode_WithErrorMessage(at,msg) {}
        virtual bool rtti_node_isCallBase() const override { return true; }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitCall ( SimVisitor & vis );
        __forceinline void evalArgs ( Context & context, vec4f * argValues ) {
            for ( int i=0, is=nArguments; i!=is && !context.stopFlags; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
        SimNode * visitOp1 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName );
        SimNode * visitOp2 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName );
        SimNode * visitOp3 ( SimVisitor & vis, const char * op, int typeSize, const char * typeName );
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return cast<CTYPE>::to(eval(context));                  \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
        SimNode ** arguments = nullptr;
        TypeInfo ** types = nullptr;
        SimFunction * fnPtr = nullptr;
        int32_t  nArguments = 0;
        SimNode * cmresEval = nullptr;
        void * aotFunction = nullptr;
    };

    struct DAS_API SimNode_Final : SimNode {
        SimNode_Final ( const LineInfo & a ) : SimNode(a) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitFinal ( SimVisitor & vis );
        virtual SimNode * visit ( SimVisitor & vis ) override;
        __forceinline void evalFinal ( Context & context ) {
            if ( totalFinal ) {
                auto SF = context.stopFlags;
                auto RE = context.abiResult();
                context.stopFlags = 0;
                for ( uint32_t i=0, is=totalFinal; i!=is; ++i ) {
                    finalList[i]->eval(context);
                }
                context.stopFlags = SF;
                context.abiResult() = RE;
            }
        }
        SimNode ** finalList = nullptr;
        uint32_t totalFinal = 0;
    };

    struct DAS_API SimNode_Block : SimNode_Final {
        SimNode_Block ( const LineInfo & at ) : SimNode_Final(at) {}
        virtual bool rtti_node_isBlock() const override { return true; }
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitBlock ( SimVisitor & vis );
        void visitLabels ( SimVisitor & vis );
        virtual SimNode * visit ( SimVisitor & vis ) override;
        DAS_EVAL_ABI virtual vec4f eval ( Context & context ) override;
        SimNode ** list = nullptr;
        uint32_t total = 0;
        uint64_t annotationDataSid = 0;
        uint32_t *  labels = nullptr;
        uint32_t    totalLabels = 0;
    };

    struct DAS_API SimNode_BlockNF : SimNode_Block {
        SimNode_BlockNF ( const LineInfo & at ) : SimNode_Block(at) {}
        DAS_EVAL_ABI virtual vec4f eval ( Context & context ) override;
    };

    struct DAS_API SimNode_BlockWithLabels : SimNode_Block {
        SimNode_BlockWithLabels ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        DAS_EVAL_ABI virtual vec4f eval ( Context & context ) override;
    };

    struct DAS_API SimNode_ForBase : SimNode_Block {
        SimNode_ForBase ( const LineInfo & at ) : SimNode_Block(at) {}
        SimNode * visitFor ( SimVisitor & vis, int total, const char * loopName );
        void allocateFor ( NodeAllocator * code, uint32_t t );
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        SimNode **  sources = nullptr;
        uint32_t *  strides = nullptr;
        uint32_t *  stackTop = nullptr;
        uint32_t    size;
        uint32_t    totalSources;
    };

    struct DAS_API SimNode_Delete : SimNode_WithErrorMessage {
        SimNode_Delete ( const LineInfo & a, SimNode * s, uint32_t t, const char * em )
            : SimNode_WithErrorMessage(a,em), subexpr(s), total(t) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        SimNode *   subexpr;
        uint32_t    total;
    };

    struct DAS_API SimNode_ClosureBlock : SimNode_Block {
        SimNode_ClosureBlock ( const LineInfo & at, bool nr, bool c0, uint64_t ad )
            : SimNode_Block(at), annotationData(ad), flags(0) {
                this->needResult = nr;
                this->code0 = c0;
            }
        virtual SimNode * visit ( SimVisitor & vis ) override;
        DAS_EVAL_ABI virtual vec4f eval ( Context & context ) override;
        uint64_t annotationData = 0;
        union {
            uint32_t flags;
            struct {
                bool needResult : 1;
                bool code0 : 1;
            };
        };
    };

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4701)
#pragma warning(disable:4324)
#endif
    template <typename Fn>
    DAS_EVAL_ABI vec4f Context::invokeEx(const Block &block, vec4f * args, void * cmres, Fn && when, LineInfo * line ) {
        char * EP, *SP;
        vec4f * TBA = nullptr;
        char * STB = stack.bottom();
#if DAS_ENABLE_STACK_WALK
        if (!stack.push_invoke(sizeof(Prologue), block.stackOffset, EP, SP)) {
            throw_error_at(line, "stack overflow during invokeEx");
        }
        Prologue * pp = (Prologue *)stack.ap();
        pp->block = (Block *)(intptr_t(&block) | 1);
        pp->arguments = args;
        pp->cmres = cmres;
        pp->line = line;
#else
        stack.invoke(block.stackOffset, EP, SP);
#endif
        BlockArguments * ba = nullptr;
        BlockArguments saveArguments;
        if ( block.argumentsOffset || cmres ) {
            ba = (BlockArguments *) ( STB + block.argumentsOffset );
            saveArguments = *ba;
            ba->arguments = args;
            ba->copyOrMoveResult = (char *) cmres;
            TBA = abiThisBlockArg;
            abiThisBlockArg = args;
        }
        vec4f * __restrict saveFunctionArguments = abiArg;
        abiArg = block.functionArguments;
        SimNode_ClosureBlock * cb = (SimNode_ClosureBlock *) block.body;
        when(cb->code0 ? cb->list[0] : block.body);
        abiArg = saveFunctionArguments;
        if ( ba ) {
            *ba = saveArguments;
            abiThisBlockArg = TBA;
        }
        stack.pop(EP, SP);
        return result;
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#include "daScript/simulate/simulate_visit_op_undef.h"
