#pragma once

#include <setjmp.h>
#include "daScript/misc/vectypes.h"
#include "daScript/misc/type_name.h"
#include "daScript/misc/arraytype.h"
#include "daScript/simulate/cast.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/heap.h"

#include "daScript/simulate/simulate_visit_op.h"

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

    #ifndef DAS_ENABLE_PROFILER
    #define DAS_ENABLE_PROFILER 0
    #endif

    #define MAX_FOR_ITERATORS   32

    #if DAS_ENABLE_PROFILER
        #define DAS_PROFILE_NODE    profileNode(context, this);
    #else
        #define DAS_PROFILE_NODE
    #endif

    class Context;
    struct SimNode;
    struct Block;
    struct SimVisitor;

    struct GlobalVariable {
        char *          name;
        VarInfo *       debugInfo;
        SimNode *       init;
        uint32_t        size;
        uint32_t        offset;
    };

    struct SimFunction {
        char *      name;
        char *      mangledName;
        SimNode *   code;
        FuncInfo *  debugInfo;
        uint32_t    stackSize;
        uint32_t    mangledNameHash;
        union {
            uint32_t    flags;
            struct {
                bool    aot : 1;
                bool    fastcall : 1;
            };
        };
    };

    struct SimNode {
        SimNode ( const LineInfo & at ) : debugInfo(at) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code );
        virtual vec4f eval ( Context & ) = 0;
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
        virtual bool rtti_isSourceBase() const { return false;  }
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
    ,   stopForContinue     = 1 << 2
    ,   jumpToLabel         = 1 << 3
    };

#define DAS_PROCESS_LOOP_FLAGS(howtocontinue) \
    { if (context.stopFlags) { \
        if (context.stopFlags & EvalFlags::stopForContinue) { \
            context.stopFlags &= ~EvalFlags::stopForContinue; \
            howtocontinue; \
        } else if (context.stopFlags&EvalFlags::jumpToLabel && context.gotoLabel<totalLabels) { \
            if ((body=list+labels[context.gotoLabel])>=list) { \
                context.stopFlags &= ~EvalFlags::jumpToLabel; \
                goto loopbegin; \
            } \
        } \
        goto loopend; \
    } }

#define DAS_PROCESS_LOOP1_FLAGS(howtocontinue) \
    { if (context.stopFlags) { \
        if (context.stopFlags & EvalFlags::stopForContinue) { \
            context.stopFlags &= ~EvalFlags::stopForContinue; \
            howtocontinue; \
        } \
        goto loopend; \
    } }

#if DAS_ENABLE_EXCEPTIONS
    class dasException : public runtime_error {
    public:
        dasException ( const char * why ) : runtime_error(why) {}
    };
#endif

    struct SimVisitor {
        virtual void preVisit ( SimNode * ) { }
        virtual void cr () {}
        virtual void op ( const char * /* name */, size_t /* sz */ = 0, const string & /* TT */ = string() ) {}
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
        virtual void sub ( SimNode ** nodes, uint32_t count, const char * );
        virtual SimNode * sub ( SimNode * node, const char * /* opN */ = "subexpr" ) { return node->visit(*this); }
        virtual SimNode * visit ( SimNode * node ) { return node; }
    };

    void printSimNode ( TextWriter & ss, SimNode * node );
    uint64_t getSemanticHash ( SimNode * node );

    class Context {
        template <typename TT> friend struct SimNode_GetGlobalR2V;
        friend struct SimNode_GetGlobal;
        friend struct SimNode_TryCatch;
        friend class Program;
    public:
        Context(uint32_t stackSize = 16*1024);
        Context(const Context &);
        Context & operator = (const Context &) = delete;
        virtual ~Context();

        uint32_t getGlobalSize() const {
            return globalsSize;
        }

        uint64_t getInitSemanticHash();

        __forceinline void * getVariable ( int index ) const {
            return (uint32_t(index)<uint32_t(totalVariables)) ? (globals + globalVariables[index].offset) : nullptr;
        }

        __forceinline VarInfo * getVariableInfo( int index ) const {
            return (uint32_t(index)<uint32_t(totalVariables)) ? globalVariables[index].debugInfo  : nullptr;;
        }

        __forceinline void simEnd() {
            thisProgram = nullptr;
            thisHelper = nullptr;
        }

        __forceinline void restart( ) {
            DAS_ASSERTF(stack.empty(),"can't reset context during the simulation");
            DAS_ASSERTF(insideContext==0,"can't reset locked context");
            stopFlags = 0;
            exception = nullptr;
            stack.reset();
        }

        __forceinline void restartHeaps() {
            DAS_ASSERTF(stack.empty(),"can't reset heaps context during the simulation");
            DAS_ASSERTF(insideContext==0,"can't reset heaps in locked context");
            heap.reset();
            stringHeap.reset();
        }

        __forceinline uint32_t tryRestartAndLock() {
            if (insideContext == 0) {
                restart();
            }
            return lock();
        }

        __forceinline uint32_t lock() {
            return insideContext ++;
        }

        __forceinline uint32_t unlock() {
            return insideContext --;
        }

        __forceinline vec4f eval ( const SimFunction * fnPtr, vec4f * args = nullptr, void * res = nullptr ) {
            return callWithCopyOnReturn(fnPtr, args, res, 0);
        }

        vec4f evalWithCatch ( SimFunction * fnPtr, vec4f * args = nullptr, void * res = nullptr );
        vec4f evalWithCatch ( SimNode * node );
        bool  runWithCatch ( const function<void()> & subexpr );

        void throw_error ( const char * message );
        void throw_error_ex ( const char * message, ... );
        void throw_error_at ( const LineInfo & at, const char * message, ... );

        __forceinline SimFunction * getFunction ( int index ) const {
            return (index>=0 && index<totalFunctions) ? functions + index : nullptr;
        }
        __forceinline int32_t getTotalFunctions() const {
            return totalFunctions;
        }
        __forceinline int32_t getTotalVariables() const {
            return totalVariables;
        }

        __forceinline uint64_t adBySid ( uint32_t sid ) const {
            uint32_t idx = rotl_c(sid, tabAdRot) & tabAdMask;
            return tabAdLookup[idx];
        }

        __forceinline int fnIdxByMangledName ( uint32_t mnh ) const {
            uint32_t idx = rotl_c(mnh, tabMnRot) & tabMnMask;
            return tabMnLookup[idx];
        }

        __forceinline int findIdxByMangledName ( uint32_t mnh ) const {
            for ( int index=0; index!=totalFunctions; ++index ) {
                if ( functions[index].mangledNameHash==mnh ) {
                    return index + 1;
                }
            }
            return 0;
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

        __forceinline vec4f * abiThisBlockArguments() {
            return abiThisBlockArg;
        }

        __forceinline vec4f & abiResult() {
            return result;
        }

        __forceinline char * abiCopyOrMoveResult() {
            return (char *) abiCMRES;
        }

        __forceinline vec4f call(const SimFunction * fn, vec4f * args, int line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error_ex("stack overflow at line %i",line);
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg;
            abiArg = args;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->arguments = args;
            pp->info = fn->debugInfo;
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

        __forceinline vec4f callOrFastcall(const SimFunction * fn, vec4f * args, int line) {
            if ( fn->fastcall ) {
                auto aa = abiArg;
                abiArg = args;
                result = fn->code->eval(*this);
                stopFlags = 0;
                abiArg = aa;
                return result;
            } else {
                // PUSH
                char * EP, *SP;
                if (!stack.push(fn->stackSize, EP, SP)) {
                    throw_error_ex("stack overflow at line %i",line);
                    return v_zero();
                }
                // fill prologue
                auto aa = abiArg;
                abiArg = args;
    #if DAS_ENABLE_STACK_WALK
                Prologue * pp = (Prologue *)stack.sp();
                pp->arguments = args;
                pp->info = fn->debugInfo;
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
        }

        __forceinline vec4f callWithCopyOnReturn(const SimFunction * fn, vec4f * args, void * cmres, int line) {
            // PUSH
            char * EP, *SP;
            if (!stack.push(fn->stackSize, EP, SP)) {
                throw_error_ex("stack overflow at line %i",line);
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg; auto acm = abiCMRES;
            abiArg = args; abiCMRES = cmres;
#if DAS_ENABLE_STACK_WALK
            Prologue * pp = (Prologue *)stack.sp();
            pp->arguments = args;
            pp->info = fn->debugInfo;
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
        __forceinline vec4f invoke(const Block &block, vec4f * args, void * cmres ) {
            char * EP, *SP;
            vec4f * TBA = nullptr;
            stack.invoke(block.stackOffset, EP, SP);
            BlockArguments * __restrict ba = nullptr;
            BlockArguments saveArguments;
            if ( block.argumentsOffset ) {
                ba = (BlockArguments *) ( stack.bottom() + block.argumentsOffset );
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
        vec4f invokeEx(const Block &block, vec4f * args, void * cmres, Fn && when);

        template <typename Fn>
        vec4f callEx(const SimFunction * fn, vec4f *args, void * cmres, int line, Fn && when) {
            // PUSH
            char * EP, *SP;
            if(!stack.push(fn->stackSize,EP,SP)) {
                throw_error_ex("stack overflow at line %i", line);
                return v_zero();
            }
            // fill prologue
            auto aa = abiArg; auto acm = cmres;
            abiArg = args;  abiCMRES = cmres;
    #if DAS_ENABLE_STACK_WALK
            Prologue * pp           = (Prologue *) stack.sp();
            pp->arguments           = args;
            pp->info                = fn->debugInfo;
            pp->line                = line;
    #endif
            // CALL
            when(fn->code);
            stopFlags = 0;
            // POP
            abiArg = aa; abiCMRES = acm;
            stack.pop(EP,SP);
            return result;
        }

        __forceinline const char * getException() const {
            return exception;
        }

        void relocateCode();
        void collectStringHeap();

        uint64_t getSharedMemorySize() const;
        uint64_t getUniqueMemorySize() const;


#if DAS_ENABLE_PROFILER
        map<LineInfo,uint64_t,cmpLineInfoWithoutColumn>  profileData;
#endif

        void resetProfiler();
        void dumpProfileInfo();

    public:
        uint64_t *                      annotationData = nullptr;
        StringAllocator                 stringHeap;
        HeapAllocator                   heap;
        char *                          globals = nullptr;
        shared_ptr<StringAllocator>     constStringHeap;
        shared_ptr<NodeAllocator>       code;
        shared_ptr<DebugInfoAllocator>  debugInfo;
        StackAllocator                  stack;
        uint32_t                        insideContext = 0;
    public:
        vec4f *         abiThisBlockArg;
        vec4f *         abiArg;
        void *          abiCMRES;
    public:
        const char *    exception = nullptr;
        string          lastError;
#if !DAS_ENABLE_EXCEPTIONS
        jmp_buf *       throwBuf = nullptr;
#endif
    protected:
        GlobalVariable * globalVariables = nullptr;
        uint32_t globalsSize = 0;
        uint32_t globalInitStackSize = 0;
        SimFunction * functions = nullptr;
        int totalVariables = 0;
        int totalFunctions = 0;
        SimNode * aotInitScript = nullptr;
    public:
        uint32_t *  tabMnLookup = nullptr;
        uint32_t    tabMnMask = 0;
        uint32_t    tabMnRot = 0;
        uint32_t    tabMnSize = 0;
    public:
        uint64_t *  tabAdLookup = nullptr;
        uint32_t    tabAdMask = 0;
        uint32_t    tabAdRot = 0;
        uint32_t    tabAdSize = 0;
    public:
        class Program * thisProgram = nullptr;
        class DebugInfoHelper * thisHelper = nullptr;
    public:
        uint32_t stopFlags = 0;
        uint32_t gotoLabel = 0;
        vec4f result;
    };

    struct Iterator {
        virtual ~Iterator() {}
        virtual bool first ( Context & context, char * value ) = 0;
        virtual bool next  ( Context & context, char * value ) = 0;
        virtual void close ( Context & context, char * value ) = 0;    // can't throw
    };

#if DAS_ENABLE_PROFILER

__forceinline void profileNode ( Context & context, SimNode * node ) {
    context.profileData[node->debugInfo] ++;
}

__forceinline void profileNode ( Context * context, SimNode * node ) {
    context->profileData[node->debugInfo] ++;
}

#endif

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
    virtual vec4f eval ( das::Context & context ) override {         \
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

    // FUNCTION CALL
    struct SimNode_CallBase : SimNode {
        SimNode_CallBase ( const LineInfo & at ) : SimNode(at) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitCall ( SimVisitor & vis );
        __forceinline void evalArgs ( Context & context, vec4f * argValues ) {
            for ( int i=0; i!=nArguments && !context.stopFlags; ++i ) {
                argValues[i] = arguments[i]->eval(context);
            }
        }
        SimNode * visitOp1 ( SimVisitor & vis, const char * op, int typeSize, const string & typeName );
        SimNode * visitOp2 ( SimVisitor & vis, const char * op, int typeSize, const string & typeName );
        SimNode * visitOp3 ( SimVisitor & vis, const char * op, int typeSize, const string & typeName );
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return cast<CTYPE>::to(eval(context));                  \
        }
        DAS_EVAL_NODE
#undef  EVAL_NODE
        SimNode ** arguments = nullptr;
        TypeInfo ** types = nullptr;
        SimFunction * fnPtr = nullptr;
        int32_t  fnIndex = -1;
        int32_t  nArguments = 0;
        SimNode * cmresEval = nullptr;
        // uint32_t stackTop = 0;
    };

    struct SimNode_Final : SimNode {
        SimNode_Final ( const LineInfo & a ) : SimNode(a) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitFinal ( SimVisitor & vis );
        virtual SimNode * visit ( SimVisitor & vis ) override;
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

    struct SimNode_Block : SimNode_Final {
        SimNode_Block ( const LineInfo & at ) : SimNode_Final(at) {}
        virtual SimNode * copyNode ( Context & context, NodeAllocator * code ) override;
        void visitBlock ( SimVisitor & vis );
        void visitLabels ( SimVisitor & vis );
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
        SimNode ** list = nullptr;
        uint32_t total = 0;
        uint32_t annotationDataSid = 0;
        uint32_t *  labels = nullptr;
        uint32_t    totalLabels = 0;
    };

    struct SimNode_BlockNF : SimNode_Block {
        SimNode_BlockNF ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_BlockWithLabels : SimNode_Block {
        SimNode_BlockWithLabels ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
    };

    struct SimNode_ForBase : SimNode_Block {
        SimNode_ForBase ( const LineInfo & at ) : SimNode_Block(at) {}
        SimNode * visitFor ( SimVisitor & vis, int total, const char * loopName );
        SimNode *   sources [MAX_FOR_ITERATORS];
        uint32_t    strides [MAX_FOR_ITERATORS];
        uint32_t    stackTop[MAX_FOR_ITERATORS];
        uint32_t    size;
    };

    struct SimNode_Delete : SimNode {
        SimNode_Delete ( const LineInfo & a, SimNode * s, uint32_t t )
            : SimNode(a), subexpr(s), total(t) {}
        virtual SimNode * visit ( SimVisitor & vis ) override;
        SimNode *   subexpr;
        uint32_t    total;
    };

    struct SimNode_ClosureBlock : SimNode_Block {
        SimNode_ClosureBlock ( const LineInfo & at, bool nr, bool c0, uint64_t ad )
            : SimNode_Block(at), annotationData(ad), flags(0) {
                this->needResult = nr;
                this->code0 = c0;
            }
        virtual SimNode * visit ( SimVisitor & vis ) override;
        virtual vec4f eval ( Context & context ) override;
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
    vec4f Context::invokeEx(const Block &block, vec4f * args, void * cmres, Fn && when) {
        char * EP, *SP;
        vec4f * TBA = nullptr;
        stack.invoke(block.stackOffset,EP,SP);
        BlockArguments * ba = nullptr;
        BlockArguments saveArguments;
        if ( block.argumentsOffset ) {
            ba = (BlockArguments *) ( stack.bottom() + block.argumentsOffset );
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
        return result;
    }
#ifdef _MSC_VER
#pragma warning(pop)
#endif
}

#include "daScript/simulate/simulate_visit_op_undef.h"


