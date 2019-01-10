#pragma once

#include "daScript/misc/vectypes.h"
#include "daScript/misc/arraytype.h"
#include "daScript/simulate/cast.h"
#include "daScript/simulate/runtime_string.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/sim_policy.h"

namespace das
{
    #ifndef DAS_ENABLE_STACK_WALK
    #define DAS_ENABLE_STACK_WALK   1
    #endif
    
    #ifndef DAS_ENABLE_EXCEPTIONS
    #define DAS_ENABLE_EXCEPTIONS   1
    #endif
    
    #define MAX_FOR_ITERATORS   16
    
    using namespace std;
    
    class Context;
    struct SimNode;
    struct Block;
    
    struct GlobalVariable {
        vec4f			value;
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
        virtual int32_t     evalInt ( Context & context );
        virtual uint32_t    evalUInt ( Context & context );
        virtual int64_t     evalInt64 ( Context & context );
        virtual uint64_t    evalUInt64 ( Context & context );
        LineInfo debug;
    };
    
    struct Prologue {
        vec4f *     arguments;
        char *      copyOrMoveResult;
#if DAS_ENABLE_STACK_WALK
        FuncInfo *  info;
        int32_t     line;
#endif
    };
    static_assert((sizeof(Prologue) & 0xf)==0, "it has to be 16 byte aligned");
    
    struct BlockArguments {
        vec4f *     arguments;
        char *      copyOrMoveResult;
    };
    
    enum EvalFlags : uint32_t {
        stopForBreak        = 1 << 0
    ,   stopForReturn       = 1 << 1
    ,   stopForThrow        = 1 << 2
    ,   stopForTerminate    = 1 << 3
    };
    
    class Context {
        template <typename TT> friend struct SimNode_GetGlobalR2V;
        friend struct SimNode_GetGlobal;
        friend class Program;
    public:
        Context(const string * lines, int las = 4*1024*1024);
        ~Context();
        
        void * reallocate ( void * oldData, uint32_t oldSize, uint32_t size );
        void * allocate ( uint32_t size );
        char * allocateName ( const string & name );
        
        template<typename TT, typename... Params>
        __forceinline TT * makeNode(Params... args) {
            return new (allocate(sizeof(TT))) TT(args...);
        }
        
        template < template <typename TT> class NodeType, typename... Params>
        __forceinline SimNode * makeValueNode(Type baseType, Params... args) {
            switch ( baseType ) {
                case Type::tBool:       return makeNode<NodeType<bool>>     (args...);
                case Type::tInt64:      return makeNode<NodeType<int64_t>>  (args...);
                case Type::tUInt64:     return makeNode<NodeType<uint64_t>> (args...);
                case Type::tInt:        return makeNode<NodeType<int32_t>>  (args...);
                case Type::tInt2:       return makeNode<NodeType<int2>>     (args...);
                case Type::tInt3:       return makeNode<NodeType<int3>>     (args...);
                case Type::tInt4:       return makeNode<NodeType<int4>>     (args...);
                case Type::tUInt:       return makeNode<NodeType<uint32_t>> (args...);
                case Type::tUInt2:      return makeNode<NodeType<uint2>>    (args...);
                case Type::tUInt3:      return makeNode<NodeType<uint3>>    (args...);
                case Type::tUInt4:      return makeNode<NodeType<uint4>>    (args...);
                case Type::tFloat:      return makeNode<NodeType<float>>    (args...);
                case Type::tFloat2:     return makeNode<NodeType<float2>>   (args...);
                case Type::tFloat3:     return makeNode<NodeType<float3>>   (args...);
                case Type::tFloat4:     return makeNode<NodeType<float4>>   (args...);
                case Type::tRange:      return makeNode<NodeType<range>>    (args...);
                case Type::tURange:     return makeNode<NodeType<urange>>   (args...);
                case Type::tString:     return makeNode<NodeType<char *>>   (args...);
                case Type::tPointer:    return makeNode<NodeType<void *>>   (args...);
                case Type::tBlock:      return makeNode<NodeType<Block>>    (args...);
                default:
                    assert(0 && "we should not even be here");
                    return nullptr;
            }
        }
        
        template < template <int TT> class NodeType, typename... Params>
        __forceinline SimNode * makeNodeUnroll ( int count, Params... args ) {
            switch ( count ) {
                case  1: return makeNode<NodeType< 1>>(args...);
                case  2: return makeNode<NodeType< 2>>(args...);
                case  3: return makeNode<NodeType< 3>>(args...);
                case  4: return makeNode<NodeType< 4>>(args...);
                case  5: return makeNode<NodeType< 5>>(args...);
                case  6: return makeNode<NodeType< 6>>(args...);
                case  7: return makeNode<NodeType< 7>>(args...);
                case  8: return makeNode<NodeType< 8>>(args...);
                case  9: return makeNode<NodeType< 9>>(args...);
                case 10: return makeNode<NodeType<10>>(args...);
                case 11: return makeNode<NodeType<11>>(args...);
                case 12: return makeNode<NodeType<12>>(args...);
                case 13: return makeNode<NodeType<13>>(args...);
                case 14: return makeNode<NodeType<14>>(args...);
                case 15: return makeNode<NodeType<15>>(args...);
                case 16: return makeNode<NodeType<16>>(args...);
                default:
                    assert(0 && "we should not even be here");
                    return nullptr;
            }
        }
        
        __forceinline vec4f getVariable ( int index ) const {
            assert(index>=0 && index<totalVariables && "variable index out of range");
            return globalVariables[index].value;
        }
        
        __forceinline void simEnd() {
            thisProgram = nullptr;
            linearAllocatorExecuteBase = linearAllocator;
        }
        
        __forceinline void restart( ) {
            linearAllocator = linearAllocatorExecuteBase;
            invokeStackTop = nullptr;
            stackTop = stack + stackSize;
            stopFlags = 0;
        }
        
        __forceinline vec4f eval ( int fnIndex, vec4f * args = nullptr, void * res = nullptr ) {
            return call(fnIndex, args, res, 0);
        }
        
        __forceinline void throw_error ( const char * message ) {
            exception = message;
            stopFlags |= EvalFlags::stopForThrow;
            #if !DAS_ENABLE_EXCEPTIONS
                throw runtime_error(message ? message : "");
            #endif
        }
        
        int findFunction ( const char * name ) const;
        int findVariable ( const char * name ) const;
        void stackWalk();
        string getStackWalk( bool args = true );
        void runInitScript ( void );
        
        virtual void to_out ( const char * message );           // output to stdout or equivalent
        virtual void to_err ( const char * message );           // output to stderr or equivalent
        virtual void breakPoint(int column, int line) const;    // what to do in case of breakpoint
        
        __forceinline vec4f * abiArguments() {
            return ((Prologue *)stackTop)->arguments;
        }

		__forceinline vec4f & abiResult() {
            return result;
        }
        
        __forceinline char * abiCopyOrMoveResult() {
            return ((Prologue *)stackTop)->copyOrMoveResult;
        }
        
        __forceinline vec4f call(int fnIndex, vec4f * args, void * cmres, int line) {
            assert(fnIndex >= 0 && fnIndex<totalFunctions && "function index out of range");
            auto & fn = functions[fnIndex];
            // PUSH
            char * top = invokeStackTop ? invokeStackTop : stackTop;
            if (stack - (top - fn.stackSize) > stackSize) {
                throw_error("stack overflow");
                return vec_setzero_ps();
            }
            char * pushStack = stackTop;
            char * pushInvokeStack = invokeStackTop;
            stackTop = top - fn.stackSize;
            assert(stackTop >= stack && stackTop < stackTop + stackSize);
            invokeStackTop = nullptr;
            // fill prologue
            Prologue * pp = (Prologue *)stackTop;
            pp->arguments = args;
            pp->copyOrMoveResult = (char *)cmres;
#if DAS_ENABLE_STACK_WALK
            pp->info = fn.debug;
            pp->line = line;
#endif
            // CALL
            fn.code->eval(*this);
            stopFlags &= ~(EvalFlags::stopForReturn | EvalFlags::stopForBreak);
            // POP
            invokeStackTop = pushInvokeStack;
            stackTop = pushStack;
            assert(stackTop >= stack && stackTop < stackTop + stackSize);
            return result;
        }

		__forceinline vec4f invoke(const Block &block, vec4f * args, void * cmres ) {
			char * saveSp = stackTop;
			char * saveISp = invokeStackTop;
			invokeStackTop = stackTop;
			stackTop = stack + block.stackOffset;
			assert(stackTop >= stack && stackTop < stackTop + stackSize);
            BlockArguments * __restrict ba = nullptr;
			BlockArguments saveArguments;
            if ( block.argumentsOffset ) {
                ba = (BlockArguments *) ( stack + block.argumentsOffset );
                saveArguments = *ba;
                ba->arguments = args;
                ba->copyOrMoveResult = (char *) cmres;
            }
			vec4f block_result = block.body->eval(*this);
			if (block.argumentsOffset) {
                *ba = saveArguments;
			}
			invokeStackTop = saveISp;
			stackTop = saveSp;
			assert(stackTop >= stack && stackTop < stackTop + stackSize);
			return block_result;
		}

        vec4f callEx ( int fnIndex, vec4f * args, void * cmres, int line, function<void (SimNode *)> && when );
        vec4f invokeEx ( const Block &block, vec4f * args, void * cmres, function<void (SimNode *)> && when );
        
        __forceinline const char * getException() const {
            return stopFlags & EvalFlags::stopForThrow ? exception : nullptr;
        }
        
    protected:
        int linearAllocatorSize;
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
        char * invokeStackTop = nullptr;
        char * stackTop = nullptr;
        char * stack = nullptr;
        int stackSize = 16*1024;
        uint32_t stopFlags = 0;
        vec4f result;
    };
    
#if DAS_ENABLE_EXCEPTIONS
    #define DAS_EXCEPTION_POINT \
        { if ( context.stopFlags ) return vec_setzero_ps(); }
    #define DAS_PTR_EXCEPTION_POINT \
        { if ( context.stopFlags ) return nullptr; }
    #define DAS_ITERATOR_EXCEPTION_POINT \
        { if ( context.stopFlags ) return false; }
    #define DAS_BOOL_EXCEPTION_POINT \
        { if ( context.stopFlags ) return false; }
    #define DAS_INT_EXCEPTION_POINT \
        { if ( context.stopFlags ) return 0; }
    #define DAS_NODE_EXCEPTION_POINT(CTYPE) \
        { if ( context.stopFlags ) return (CTYPE) 0; }
#else
    #define DAS_EXCEPTION_POINT
    #define DAS_PTR_EXCEPTION_POINT
    #define DAS_ITERATOR_EXCEPTION_POINT
    #define DAS_BOOL_EXCEPTION_POINT
    #define DAS_INT_EXCEPTION_POINT
    #define DAS_NODE_EXCEPTION_POINT(CTYPE)
#endif
    
#define DAS_EVAL_NODE               \
    EVAL_NODE(Ptr,char *);          \
    EVAL_NODE(Int,int32_t);         \
    EVAL_NODE(UInt,uint32_t);       \
    EVAL_NODE(Int64,int64_t);       \
    EVAL_NODE(UInt64,uint64_t);     \
    EVAL_NODE(Float,float);         \
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
            DAS_EXCEPTION_POINT;
            TT * pR = (TT *)( prv + offset );
            return cast<TT>::from(*pR);

        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            auto prv = value->evalPtr(context);                     \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                        \
            return * (CTYPE *)( prv + offset );                     \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
    };

	// PTR FIELD .
	struct SimNode_PtrFieldDeref : SimNode {
		DAS_PTR_NODE;
		SimNode_PtrFieldDeref(const LineInfo & at, SimNode * rv, uint32_t of) : SimNode(at), value(rv), offset(of) {}
		__forceinline char * compute(Context & context) {
			auto prv = value->evalPtr(context);
			DAS_PTR_EXCEPTION_POINT;
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
			DAS_EXCEPTION_POINT;
			if (prv) {
				TT * pR = (TT *)(prv + offset);
				return cast<TT>::from(*pR);
			}
			else {
				context.throw_error("dereferencing null pointer");
				return vec_setzero_ps();
			}
		}
		virtual char * evalPtr(Context & context) override {
			auto prv = value->evalPtr(context);
			DAS_PTR_EXCEPTION_POINT;
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
            DAS_PTR_EXCEPTION_POINT;
			return prv ? prv + offset : nullptr;
        }
    };
    
    // FIELD ?.->
    struct SimNode_SafeFieldDerefPtr : SimNode_FieldDeref {
        DAS_PTR_NODE;
        SimNode_SafeFieldDerefPtr ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        __forceinline char * compute ( Context & context ) {
            char ** prv = (char **) value->evalPtr(context);
            DAS_PTR_EXCEPTION_POINT;
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
			DAS_PTR_EXCEPTION_POINT;
			uint32_t idx = cast<uint32_t>::to(index->eval(context));
			DAS_PTR_EXCEPTION_POINT;
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
        DAS_EVAL_NODE;
#undef  EVAL_NODE
        SimNode ** arguments;
        int32_t  fnIndex;
        int32_t  nArguments;
        uint32_t stackTop;
    };
    
    // FUNCTION CALL
    struct SimNode_Call : SimNode_CallBase {
        SimNode_Call ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override;
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));                  \
                evalArgs(context, argValues);                                                   \
                DAS_NODE_EXCEPTION_POINT(CTYPE);                                                \
                return cast<CTYPE>::to(context.call(fnIndex, argValues, nullptr, debug.line));  \
        }
        DAS_EVAL_NODE;
#undef  EVAL_NODE
    };
    
    // FUNCTION CALL with copy-or-move-on-return
    struct SimNode_CallAndCopyOrMove : SimNode_CallBase {
        SimNode_CallAndCopyOrMove ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override;
#define EVAL_NODE(TYPE,CTYPE)\
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));                  \
                evalArgs(context, argValues);                                                   \
                DAS_NODE_EXCEPTION_POINT(CTYPE);                                                \
                auto cmres = context.stackTop + stackTop;                                       \
                return cast<CTYPE>::to(context.call(fnIndex, argValues, cmres, debug.line));    \
        }
        DAS_EVAL_NODE;
#undef  EVAL_NODE
    };
    
    // Invoke
    struct SimNode_Invoke : SimNode_CallBase {
        SimNode_Invoke ( const LineInfo & at ) : SimNode_CallBase(at) {}
        virtual vec4f eval ( Context & context ) override;
#define EVAL_NODE(TYPE,CTYPE)                                                                   \
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));                  \
            evalArgs(context, argValues);                                                       \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                                                    \
            Block block = cast<Block>::to(argValues[0]);                                        \
            if ( nArguments>1 ) {                                                               \
                return cast<CTYPE>::to(context.invoke(block, argValues + 1, nullptr));          \
            } else {                                                                            \
                return cast<CTYPE>::to(context.invoke(block, nullptr, nullptr));                \
            }                                                                                   \
        }
        DAS_EVAL_NODE;
#undef  EVAL_NODE
    };
    
    // Invoke with copy-or-move-on-return
    struct SimNode_InvokeAndCopyOrMove : SimNode_Invoke {
        SimNode_InvokeAndCopyOrMove ( const LineInfo & at, uint32_t sp )
            : SimNode_Invoke(at) { stackTop = sp; }
        virtual vec4f eval ( Context & context ) override;
#define EVAL_NODE(TYPE,CTYPE)                                                                   \
        virtual CTYPE eval##TYPE ( Context & context ) override {                               \
            vec4f * argValues = (vec4f *)(alloca(nArguments * sizeof(vec4f)));                  \
            evalArgs(context, argValues);                                                       \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                                                    \
            Block block = cast<Block>::to(argValues[0]);                                        \
            auto cmres = context.stackTop + stackTop;                                           \
            if ( nArguments>1 ) {                                                               \
                return cast<CTYPE>::to(context.invoke(block, argValues + 1, cmres));            \
            } else {                                                                            \
                return cast<CTYPE>::to(context.invoke(block, nullptr, cmres));                  \
            }                                                                                   \
        }
        DAS_EVAL_NODE;
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
            auto str = std::to_string ( cast<CastFrom>::to(res) );
            auto cpy = context.allocateName(str);
            return cast<char *>::from(cpy);
        }
    };
    
    // VECTOR C-TOR
    template <int vecS>
	struct SimNode_VecCtor;

	template <>
	struct SimNode_VecCtor<2> : SimNode_CallBase {
		SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
		virtual vec4f eval(Context & context) override {
			vec4f argValues[2];
			evalArgs(context, argValues);
			return vec_set_xyzw(cast<float>::to(argValues[0]), cast<float>::to(argValues[1]),0.0f, 0.0f);
		}
	};

	template <>
	struct SimNode_VecCtor<3> : SimNode_CallBase {
		SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
		virtual vec4f eval(Context & context) override {
			vec4f argValues[3];
			evalArgs(context, argValues);
			return vec_set_xyzw(cast<float>::to(argValues[0]), cast<float>::to(argValues[1]),
					cast<float>::to(argValues[2]), 0.0f);
		}
	};

	template <>
	struct SimNode_VecCtor<4> : SimNode_CallBase {
		SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
		virtual vec4f eval(Context & context) override {
			vec4f argValues[4];
			evalArgs(context, argValues);
			return vec_set_xyzw(cast<float>::to(argValues[0]), cast<float>::to(argValues[1]),
				cast<float>::to(argValues[2]), cast<float>::to(argValues[3]));
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
            return context.stackTop + stackTop;
        }
        uint32_t stackTop;
    };
    
    template <typename TT>
    struct SimNode_GetLocalR2V : SimNode_GetLocal {
        SimNode_GetLocalR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp)  {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = (TT *)(context.stackTop + stackTop);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return *(CTYPE *)(context.stackTop + stackTop);         \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
    };
    
    // WHEN LOCAL VARIABLE STORES REFERENCE
    struct SimNode_GetLocalRef : SimNode_GetLocal {
        DAS_PTR_NODE;
        SimNode_GetLocalRef(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp) {}
        __forceinline char * compute ( Context & context ) {
            return *(char **)(context.stackTop + stackTop);
        }
    };
    
    template <typename TT>
    struct SimNode_GetLocalRefR2V : SimNode_GetLocalRef {
        SimNode_GetLocalRefR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocalRef(at,sp) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = *(TT **)(context.stackTop + stackTop);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            return **(CTYPE **)(context.stackTop + stackTop);       \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
    };
    
    template <typename TT>
    struct SimNode_CopyLocal2LocalT : SimNode {
        SimNode_CopyLocal2LocalT(const LineInfo & at, uint32_t spL, uint32_t spR)
            : SimNode(at), stackTopLeft(spL), stackTopRight(spR) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) ( context.stackTop + stackTopLeft );
            TT * pr = (TT *) ( context.stackTop + stackTopRight );
            *pl = *pr;
            return vec_setzero_ps();
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
            DAS_EXCEPTION_POINT;
            TT * pl = (TT *) ( context.stackTop + stackTop );
            *pl = *pr;
            return vec_setzero_ps();
        }
        SimNode * r;
        uint32_t stackTop;
    };
    
    template <typename TT>
    struct SimNode_SetLocalValueT : SimNode {
        SimNode_SetLocalValueT(const LineInfo & at, SimNode * rv, uint32_t sp)
        : SimNode(at), r(rv), stackTop(sp) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f rres = r->eval(context);
            DAS_EXCEPTION_POINT;
            TT * pl = (TT *) ( context.stackTop + stackTop );
            *pl = cast<TT>::to(rres);
            return vec_setzero_ps();
        }
        SimNode * r;
        uint32_t stackTop;
    };

    
    // ZERO MEMORY OF UNITIALIZED LOCAL VARIABLE
    struct SimNode_InitLocal : SimNode {
        SimNode_InitLocal(const LineInfo & at, uint32_t sp, uint32_t sz) : SimNode(at), stackTop(sp), size(sz) {}
        virtual vec4f eval ( Context & context ) override {
            memset(context.stackTop + stackTop, 0, size);
            return vec_setzero_ps();
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
        DAS_EVAL_NODE;
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
        DAS_EVAL_NODE;
#undef EVAL_NODE
    };
    
    // BLOCK VARIABLE "GET"
    struct SimNode_GetBlockArgument : SimNode {
        SimNode_GetBlockArgument ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode(at), index(i), stackTop(sp) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * args = *((vec4f **)(context.stackTop + stackTop));
            return args[index];
        }
#define EVAL_NODE(TYPE,CTYPE)                                               \
        virtual CTYPE eval##TYPE ( Context & context ) override {           \
            vec4f * args = *((vec4f **)(context.stackTop + stackTop));    \
            return cast<CTYPE>::to(args[index]);                            \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
        int32_t     index;
        uint32_t    stackTop;
    };
    
    template <typename TT>
    struct SimNode_GetBlockArgumentR2V : SimNode_GetBlockArgument {
        SimNode_GetBlockArgumentR2V ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode_GetBlockArgument(at,i,sp) {}
        virtual vec4f eval ( Context & context ) override {
            vec4f * args = *((vec4f **)(context.stackTop + stackTop));
            TT * pR = (TT *) cast<char *>::to(args[index]);
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                               \
        virtual CTYPE eval##TYPE ( Context & context ) override {           \
            vec4f * args = *((vec4f **)(context.stackTop + stackTop));    \
            return * cast<CTYPE *>::to(args[index]);                        \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
    };
    
    struct SimNode_GetBlockArgumentRef : SimNode_GetBlockArgument {
        DAS_PTR_NODE;
        SimNode_GetBlockArgumentRef(const LineInfo & at, int32_t i, uint32_t sp)
            : SimNode_GetBlockArgument(at,i,sp) {}
        __forceinline char * compute(Context & context) {
            vec4f * args = *((vec4f **)(context.stackTop + stackTop));
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
        DAS_EVAL_NODE;
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
            return vec_setzero_ps();
        }
    };
    
    // DEREFERENCE
    template <typename TT>
    struct SimNode_Ref2Value : SimNode {      // &value -> value
        SimNode_Ref2Value ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual vec4f eval ( Context & context ) override {
            TT * pR = (TT *) subexpr->evalPtr(context);
            DAS_EXCEPTION_POINT;
            return cast<TT>::from(*pR);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE (Context & context) override {     \
			auto pR = (CTYPE *)subexpr->evalPtr(context);           \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                        \
            return *pR;                                             \
		}
        DAS_EVAL_NODE;
#undef EVAL_NODE
        SimNode * subexpr;
    };
    
    // POINTER TO REFERENCE (CAST)
    struct SimNode_Ptr2Ref : SimNode {      // ptr -> &value
        DAS_PTR_NODE;
        SimNode_Ptr2Ref ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            DAS_PTR_EXCEPTION_POINT;
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
            DAS_EXCEPTION_POINT;
            return pR ? cast<TT>::from(*pR) : value->eval(context);
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
            auto pR = (CTYPE *) subexpr->evalPtr(context);          \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                        \
            return pR ? *pR : value->eval##TYPE(context);           \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE

        SimNode * value;
    };
    
    // let(a:int?) x = a && default_a
    struct SimNode_NullCoalescingRef : SimNode_Ptr2Ref {
        DAS_PTR_NODE;
        SimNode_NullCoalescingRef ( const LineInfo & at, SimNode * s, SimNode * dv ) : SimNode_Ptr2Ref(at,s), value(dv) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            DAS_PTR_EXCEPTION_POINT;
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
        virtual vec4f eval ( Context & ) override {
            return value;
        }
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & ) override {			\
            return cast<CTYPE>::to(value);                          \
        }
        DAS_EVAL_NODE;
#undef EVAL_NODE
        vec4f value;
    };
    
    // COPY REFERENCE (int & a = b)
    struct SimNode_CopyReference : SimNode {
        SimNode_CopyReference(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual vec4f eval ( Context & context ) override {
            char  ** pl = (char **) l->evalPtr(context);
            DAS_EXCEPTION_POINT;
            char * pr = r->evalPtr(context);
            DAS_EXCEPTION_POINT;
            *pl = pr;
            return vec_setzero_ps();
        }
        SimNode * l, * r;
    };
    
    // COPY VALUE
    template <typename TT>
    struct SimNode_CopyValue : SimNode {
        SimNode_CopyValue(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        virtual vec4f eval ( Context & context ) override {
            TT * pl = (TT *) l->evalPtr(context);
            DAS_EXCEPTION_POINT;
            vec4f rr = r->eval(context);
            DAS_EXCEPTION_POINT;
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return vec_setzero_ps();
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
            DAS_EXCEPTION_POINT;
            TT * pr = (TT *) r->evalPtr(context);
            DAS_EXCEPTION_POINT;
            *pl = *pr;
            return vec_setzero_ps();
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
    
    // BLOCK
    struct SimNode_Block : SimNode {
        SimNode_Block ( const LineInfo & at ) : SimNode(at) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode ** list = nullptr;
        uint32_t total = 0;
    };
    
    struct SimNode_ClosureBlock : SimNode_Block {
        SimNode_ClosureBlock ( const LineInfo & at, bool nr, void * ad )
            : SimNode_Block(at), needResult(nr), annotationData(ad) {}
        virtual vec4f eval ( Context & context ) override;
        bool needResult = false;
        void * annotationData = nullptr;
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
        virtual vec4f eval ( Context & context ) override;
#define EVAL_NODE(TYPE,CTYPE)                                       \
        virtual CTYPE eval##TYPE ( Context & context ) override {   \
                bool cmp = cond->evalBool(context);                 \
                DAS_NODE_EXCEPTION_POINT(CTYPE);                    \
                if ( cmp ) {                                        \
                    return if_true->eval##TYPE(context);            \
                } else if ( if_false ) {                            \
                    return if_false->eval##TYPE(context);           \
                } else {                                            \
                    return (CTYPE) 0;                               \
                }                                                   \
            }
        DAS_EVAL_NODE;
#undef EVAL_NODE
        SimNode * cond, * if_true, * if_false;
    };
    
    // WHILE
    struct SimNode_While : SimNode {
        SimNode_While ( const LineInfo & at, SimNode * c, SimNode * b ) : SimNode(at), cond(c), body(b) {}
        virtual vec4f eval ( Context & context ) override;
        SimNode * cond, * body;
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
        virtual bool first ( Context & context, IteratorContext & itc ) = 0;
        virtual bool next  ( Context & context, IteratorContext & itc ) = 0;
        virtual void close ( Context & context, IteratorContext & itc ) = 0;    // can't throw
    };
    
    struct SimNode_ForBase : SimNode {
        SimNode_ForBase ( const LineInfo & at ) : SimNode(at) {}
        SimNode *   sources [MAX_FOR_ITERATORS];
        uint32_t    strides [MAX_FOR_ITERATORS];
        uint32_t    stackTop[MAX_FOR_ITERATORS];
        SimNode *   body;
        uint32_t    size;
    };
    
    struct SimNode_ForWithIteratorBase : SimNode {
        SimNode_ForWithIteratorBase ( const LineInfo & at ) : SimNode(at) {}
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
                pi[t] = (vec4f *)(context.stackTop + stackTop[t]);
            }
            Iterator * sources[total] = {};
            for ( int t=0; t!=total; ++t ) {
                vec4f ll = source_iterators[t]->eval(context);
                DAS_EXCEPTION_POINT;
                sources[t] = cast<Iterator *>::to(ll);
            }
            IteratorContext ph[total];
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
                body->eval(context);
                for ( int t=0; t!=total; ++t ){
                    if ( !sources[t]->next(context, ph[t]) ) goto loopend;
                    if ( context.stopFlags ) goto loopend;
                }
            }
        loopend:
            for ( int t=0; t!=total; ++t ) {
                sources[t]->close(context, ph[t]);
            }
            context.stopFlags &= ~EvalFlags::stopForBreak;
            return vec_setzero_ps();
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
                DAS_BOOL_EXCEPTION_POINT;
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
                DAS_BOOL_EXCEPTION_POINT;
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
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
            return SimPolicy<CTYPE>::CALL(val,context);                 \
        }                                                               \
    };
    
#define IMPLEMENT_OP1_SET_POLICY(CALL,TYPE,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op1 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto val = (CTYPE *) x->evalPtr(context);                   \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
            return SimPolicy<CTYPE>::CALL(*val,context);                \
        }                                                               \
    };
    
#define IMPLEMENT_OP1_EVAL_POLICY(CALL,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op1 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op1(at) {}         \
        virtual vec4f eval ( Context & context ) override {            \
            auto val = x->eval(context);                                \
            DAS_EXCEPTION_POINT;                                        \
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
    IMPLEMENT_OP1_POLICY(CALL,Float,float);
    
#define DEFINE_OP1_SET_NUMERIC_INTEGER(CALL)            \
    IMPLEMENT_OP1_SET_POLICY(CALL,Int,int32_t);         \
    IMPLEMENT_OP1_SET_POLICY(CALL,UInt,uint32_t);       \
    IMPLEMENT_OP1_SET_POLICY(CALL,Int64,int64_t);       \
    IMPLEMENT_OP1_SET_POLICY(CALL,UInt64,uint64_t);     \

#define DEFINE_OP1_SET_NUMERIC(CALL);                   \
    DEFINE_OP1_SET_NUMERIC_INTEGER(CALL);               \
    IMPLEMENT_OP1_SET_POLICY(CALL,Float,float);
    
#define IMPLEMENT_OP2_POLICY(CALL,TYPE,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto lv = l->eval##TYPE(context);                           \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
            auto rv = r->eval##TYPE(context);                           \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };
    
#define IMPLEMENT_OP2_SET_POLICY(CALL,TYPE,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_NODE(TYPE,CTYPE);                                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline CTYPE compute ( Context & context ) {             \
            auto lv = (CTYPE *) l->evalPtr(context);                    \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
            auto rv = r->eval##TYPE(context);                           \
            DAS_NODE_EXCEPTION_POINT(CTYPE);                            \
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
            DAS_BOOL_EXCEPTION_POINT;                                   \
            auto rv = r->eval##TYPE(context);                           \
            DAS_BOOL_EXCEPTION_POINT;                                   \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };
    
#define IMPLEMENT_OP2_EVAL_POLICY(CALL,CTYPE)                           \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        virtual vec4f eval ( Context & context ) override {            \
            auto lv = l->eval(context);                                 \
            DAS_EXCEPTION_POINT;                                        \
            auto rv = r->eval(context);                                 \
            DAS_EXCEPTION_POINT;                                        \
            return SimPolicy<CTYPE>::CALL(lv,rv,context);               \
        }                                                               \
    };
    
#define IMPLEMENT_OP2_EVAL_SET_POLICY(CALL,CTYPE)                       \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        virtual vec4f eval ( Context & context ) override {            \
            auto lv = l->evalPtr(context);                              \
            DAS_EXCEPTION_POINT;                                        \
            auto rv = r->eval(context);                                 \
            DAS_EXCEPTION_POINT;                                        \
            SimPolicy<CTYPE>::CALL(lv,rv,context);                      \
            return vec_setzero_ps();                                    \
        }                                                               \
    };
    
#define IMPLEMENT_OP2_EVAL_BOOL_POLICY(CALL,CTYPE)                      \
    template <>                                                         \
    struct Sim_##CALL <CTYPE> : SimNode_Op2 {                           \
        DAS_BOOL_NODE;                                                  \
        Sim_##CALL ( const LineInfo & at ) : SimNode_Op2(at) {}         \
        __forceinline bool compute ( Context & context ) {              \
            auto lv = l->eval(context);                                 \
            DAS_BOOL_EXCEPTION_POINT;                                   \
            auto rv = r->eval(context);                                 \
            DAS_BOOL_EXCEPTION_POINT;                                   \
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
    IMPLEMENT_OP2_POLICY(CALL,Float,float);
    
#define DEFINE_OP2_BOOL_NUMERIC_INTEGER(CALL)           \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Int,int32_t);        \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,UInt,uint32_t);      \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Int64,int64_t);      \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,UInt64,uint64_t);    \

#define DEFINE_OP2_BOOL_NUMERIC(CALL);                  \
    DEFINE_OP2_BOOL_NUMERIC_INTEGER(CALL);              \
    IMPLEMENT_OP2_BOOL_POLICY(CALL,Float,float);
    
#define DEFINE_OP2_SET_NUMERIC_INTEGER(CALL)            \
    IMPLEMENT_OP2_SET_POLICY(CALL,Int,int32_t);         \
    IMPLEMENT_OP2_SET_POLICY(CALL,UInt,uint32_t);       \
    IMPLEMENT_OP2_SET_POLICY(CALL,Int64,int64_t);       \
    IMPLEMENT_OP2_SET_POLICY(CALL,UInt64,uint64_t);     \

#define DEFINE_OP2_SET_NUMERIC(CALL);                   \
    DEFINE_OP2_SET_NUMERIC_INTEGER(CALL);               \
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
