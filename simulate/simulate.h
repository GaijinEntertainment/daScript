#pragma once

#include "vectypes.h"
#include "arraytype.h"
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
    
    #define MAX_FOR_ITERATORS   16
    
    using namespace std;
    
    class Context;
    struct SimNode;
    struct Block;
    
    struct GlobalVariable {
        char *          name;
        ValueVariant    value;
        uint32_t        size;
        VarInfo *       debug;
        SimNode *       init;
    };
    
    struct SimFunction {
        char *      name;
        SimNode *   code;
        uint32_t    stackSize;
        FuncInfo *  debug;
    };
    
    struct SimNode {
        SimNode ( const LineInfo & at ) : debug(at) {}
        virtual __m128 eval ( Context & ) = 0;
        virtual char * evalPtr ( Context & context );
        LineInfo debug;
    };
    
    struct Prologue {
        __m128      result;
        __m128 *    arguments;
        FuncInfo *  info;
        int32_t     line;
    };
    static_assert((sizeof(Prologue) & 0xf)==0, "it has to be 16 byte aligned");
    
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
        
        __forceinline __m128 getVariable ( int index ) const {
            assert(index>=0 && index<totalVariables && "variable index out of range");
            return globalVariables[index].value.dataVec;
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
        
        __forceinline ValueVariant * abiArgumentsVariant() {
            return (ValueVariant*)(((Prologue *)stackTop)->arguments);
        }

        
        __forceinline __m128 & abiResult() {
            return ((Prologue *)stackTop)->result;
        }
        
        __m128 call ( int fnIndex, __m128 * args, int line );
        __m128 callEx ( int fnIndex, __m128 * args, int line, function<void (SimNode *)> && when );
        __m128 invoke ( const Block & block, __m128 * args );
        __m128 invokeEx ( const Block &block, __m128 * args, function<void (SimNode *)> && when );
        
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
    };
    
#if YZG_ENABLE_EXCEPTIONS
    #define YZG_EXCEPTION_POINT \
        { if ( context.stopFlags ) return _mm_setzero_ps(); }
    #define YZG_PTR_EXCEPTION_POINT \
        { if ( context.stopFlags ) return nullptr; }
    #define YZG_ITERATOR_EXCEPTION_POINT \
        { if ( context.stopFlags ) return false; }
#else
    #define YZG_EXCEPTION_POINT
    #define YZG_PTR_EXCEPTION_POINT
    #define YZG_ITERATOR_EXCEPTION_POINT
#endif
    
#define YZG_PTR_NODE    \
    virtual __m128 eval ( Context & context ) override {    \
        return cast<char *>::from(compute(context));    \
    }   \
    virtual char * evalPtr ( Context & context ) override { \
        return compute(context);    \
    }
    
    // MakeBlock
    struct SimNode_MakeBlock : SimNode {
        SimNode_MakeBlock ( const LineInfo & at, SimNode * s, uint32_t a )
            : SimNode(at), subexpr(s), argStackTop(a) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode *       subexpr;
        uint32_t        argStackTop;
    };
    
    // ASSERT
    struct SimNode_Assert : SimNode {
        SimNode_Assert ( const LineInfo & at, SimNode * s, const char * m ) : SimNode(at), subexpr(s), message(m) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode *       subexpr;
        const char *    message;
    };
    
    // FIELD .
    struct SimNode_FieldDeref : SimNode {
        YZG_PTR_NODE;
        SimNode_FieldDeref ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode(at), value(rv), offset(of) {}
        __forceinline char * compute ( Context & context ) {
            auto prv = value->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            if ( prv ) {
                return prv + offset;
            } else {
                context.throw_error("dereferencing null pointer");
                return nullptr;
            }
        }
        SimNode *   value;
        uint32_t    offset;
    };
    
    template <typename TT>
    struct SimNode_FieldDerefR2V : SimNode_FieldDeref {
        SimNode_FieldDerefR2V ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        virtual __m128 eval ( Context & context ) override {
            auto prv = value->evalPtr(context);
            YZG_EXCEPTION_POINT;
            if ( prv ) {
                TT * pR = (TT *)( prv + offset );
                return cast<TT>::from(*pR);
            } else {
                context.throw_error("dereferencing null pointer");
                return _mm_setzero_ps();
            }
        }
        virtual char * evalPtr ( Context & context ) override {
            auto prv = value->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            if ( prv ) {
                return * (char **)( prv + offset );
            } else {
                context.throw_error("dereferencing null pointer");
                return nullptr;
            }
        }
    };
    
    // FIELD ?.
    struct SimNode_SafeFieldDeref : SimNode_FieldDeref {
        YZG_PTR_NODE;
        SimNode_SafeFieldDeref ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        __forceinline char * compute ( Context & context ) {
            auto prv = value->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            if ( prv ) {
                return prv + offset;
            } else {
                return nullptr;
            }
        }
    };
    
    // FIELD ?.->
    struct SimNode_SafeFieldDerefPtr : SimNode_FieldDeref {
        YZG_PTR_NODE;
        SimNode_SafeFieldDerefPtr ( const LineInfo & at, SimNode * rv, uint32_t of ) : SimNode_FieldDeref(at,rv,of) {}
        __forceinline char * compute ( Context & context ) {
            char ** prv = (char **) value->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            if ( prv ) {
                return *(prv + offset);
            } else {
                return nullptr;
            }
        }
    };
    
    // AT (INDEX)
    struct SimNode_At : SimNode {
		YZG_PTR_NODE;
        SimNode_At ( const LineInfo & at, SimNode * rv, SimNode * idx, uint32_t strd, uint32_t rng )
            : SimNode(at), value(rv), index(idx), stride(strd), range(rng) {}
		__forceinline char * compute (Context & context) {
			auto pValue = value->evalPtr(context);
			YZG_PTR_EXCEPTION_POINT;
			uint32_t idx = cast<uint32_t>::to(index->eval(context));
			YZG_PTR_EXCEPTION_POINT;
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
    struct SimNode_Call : SimNode {
        SimNode_Call ( const LineInfo & at ) : SimNode(at) {}
        void evalArgs ( Context & context, __m128 * argValues );
        virtual __m128 eval ( Context & context ) override;
        SimNode ** arguments;
        int32_t  fnIndex;
        int32_t  nArguments;
    };
    
    // Invoke
    struct SimNode_Invoke : SimNode_Call {
        SimNode_Invoke ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override;
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
    
    // LEXICAL CAST
    template <typename CastFrom>
    struct SimNode_LexicalCast : SimNode_Call {
        SimNode_LexicalCast ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 res = arguments[0]->eval(context);
            auto str = std::to_string ( cast<CastFrom>::to(res) );
            auto cpy = context.allocateName(str);
            return cast<char *>::from(cpy);
        }
    };
    
    // VECTOR C-TOR
    template <int vecS>
    struct SimNode_VecCtor : SimNode_Call {
        SimNode_VecCtor ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 argValues[vecS];
            evalArgs(context, argValues);
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
        virtual __m128 eval ( Context & context ) override;
        SimNode *       subexpr;
        TypeInfo *      typeInfo;
        const char *    message;
    };
    
    // LOCAL VARIABLE "GET"
    struct SimNode_GetLocal : SimNode {
        YZG_PTR_NODE;
        SimNode_GetLocal(const LineInfo & at, uint32_t sp) : SimNode(at), stackTop(sp) {}
        __forceinline char * compute ( Context & context ) {
            return context.stackTop + stackTop;
        }
        uint32_t stackTop;
    };
    
    template <typename TT>
    struct SimNode_GetLocalR2V : SimNode_GetLocal {
        SimNode_GetLocalR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp)  {}
        virtual __m128 eval ( Context & context ) override {
            TT * pR = (TT *)(context.stackTop + stackTop);
            return cast<TT>::from(*pR);
        }
        virtual char * evalPtr ( Context & context ) override {
            return *(char **)(context.stackTop + stackTop);
        }
    };
    
    // WHEN LOCAL VARIABLE STORES REFERENCE
    struct SimNode_GetLocalRef : SimNode_GetLocal {
        YZG_PTR_NODE;
        SimNode_GetLocalRef(const LineInfo & at, uint32_t sp) : SimNode_GetLocal(at,sp) {}
        __forceinline char * compute ( Context & context ) {
            return *(char **)(context.stackTop + stackTop);
        }
    };
    
    template <typename TT>
    struct SimNode_GetLocalRefR2V : SimNode_GetLocalRef {
        SimNode_GetLocalRefR2V(const LineInfo & at, uint32_t sp) : SimNode_GetLocalRef(at,sp) {}
        virtual __m128 eval ( Context & context ) override {
            TT * pR = *(TT **)(context.stackTop + stackTop);
            return cast<TT>::from(*pR);
        }
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
        virtual char * evalPtr ( Context & context ) override {
            return (context.abiArgumentsVariant()[index]).dataPtr;
        }
        int32_t index;
    };

	struct SimNode_GetArgumentRef : SimNode_GetArgument {
		SimNode_GetArgumentRef(const LineInfo & at, int32_t i) : SimNode_GetArgument(at,i) {}
		virtual __m128 eval(Context & context) override {
			return cast<void *>::from (&context.abiArguments()[index]);
		}
        virtual char * evalPtr ( Context & context ) override {
            return (char *) &context.abiArguments()[index];
        }
	};
    
    template <typename TT>
    struct SimNode_GetArgumentR2V : SimNode_GetArgument {
        SimNode_GetArgumentR2V ( const LineInfo & at, int32_t i ) : SimNode_GetArgument(at,i) {}
        virtual __m128 eval ( Context & context ) override {
            TT * pR = cast<TT *>::to(context.abiArguments()[index]);
            return cast<TT>::from(*pR);
        }
        virtual char * evalPtr ( Context & context ) override {
            assert(0 && "we should not even be here?");
            return *(char **)(context.abiArgumentsVariant()[index]).dataPtr;
        }
    };
    
    // BLOCK VARIABLE "GET"
    struct SimNode_GetBlockArgument : SimNode {
        SimNode_GetBlockArgument ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode(at), index(i), stackTop(sp) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 * args = *((__m128 **)(context.stackTop + stackTop));
            return args[index];
        }
        int32_t     index;
        uint32_t    stackTop;
    };
    
    template <typename TT>
    struct SimNode_GetBlockArgumentR2V : SimNode_GetBlockArgument {
        SimNode_GetBlockArgumentR2V ( const LineInfo & at, int32_t i, uint32_t sp )
            : SimNode_GetBlockArgument(at,i,sp) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 * args = *((__m128 **)(context.stackTop + stackTop));
            TT * pR = (TT *) cast<char *>::to(args[index]);
            return cast<TT>::from(*pR);
        }
    };
    
    // GLOBAL VARIABLE "GET"
    struct SimNode_GetGlobal : SimNode {
        SimNode_GetGlobal ( const LineInfo & at, int32_t i ) : SimNode(at), index(i) {}
        virtual __m128 eval ( Context & context ) override {
            return context.globalVariables[index].value.dataVec;
        }
		virtual char * evalPtr(Context & context) override {
			return context.globalVariables[index].value.dataPtr;
		}
        int32_t index;
    };
    
    template <typename TT>
    struct SimNode_GetGlobalR2V : SimNode_GetGlobal {
        SimNode_GetGlobalR2V ( const LineInfo & at, int32_t i ) : SimNode_GetGlobal(at,i) {}
        virtual __m128 eval ( Context & context ) override {
            TT * pR = (TT *) context.globalVariables[index].value.dataPtr;
            return cast<TT>::from(*pR);
        }
		virtual char * evalPtr(Context & context) override {
			return *(char **)context.globalVariables[index].value.dataPtr;
		}
    };
    
    // TRY-CATCH
    struct SimNode_TryCatch : SimNode {
        SimNode_TryCatch ( const LineInfo & at, SimNode * t, SimNode * c ) : SimNode(at), try_block(t), catch_block(c) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode * try_block, * catch_block;
    };
    
    // RETURN
    struct SimNode_Return : SimNode {
        SimNode_Return ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        virtual __m128 eval ( Context & context ) override {
            if ( subexpr ) context.abiResult() = subexpr->eval(context);
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
            TT * pR = (TT *) subexpr->evalPtr(context);
            YZG_EXCEPTION_POINT;
            return cast<TT>::from(*pR);
        }
		virtual char * evalPtr (Context & context) override {
			return *(char **)subexpr->evalPtr(context);
		}
        SimNode * subexpr;
    };
    
    // POINTER TO REFERENCE (CAST)
    struct SimNode_Ptr2Ref : SimNode {      // ptr -> &value
        YZG_PTR_NODE;
        SimNode_Ptr2Ref ( const LineInfo & at, SimNode * s ) : SimNode(at), subexpr(s) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
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
        virtual __m128 eval ( Context & context ) override {
            TT * pR = (TT *) subexpr->evalPtr(context);
            YZG_EXCEPTION_POINT;
            return pR ? cast<TT>::from(*pR) : value->eval(context);
        }
        virtual char * evalPtr ( Context & context ) override {
            assert(0 && "we should not even be here!");
            return nullptr;
        }
        SimNode * value;
    };
    
    // let(a:int?) x = a && default_a
    struct SimNode_NullCoalescingRef : SimNode_Ptr2Ref {
        YZG_PTR_NODE;
        SimNode_NullCoalescingRef ( const LineInfo & at, SimNode * s, SimNode * dv ) : SimNode_Ptr2Ref(at,s), value(dv) {}
        __forceinline char * compute ( Context & context ) {
            auto ptr = subexpr->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            return ptr ? ptr : value->evalPtr(context);
        }
        SimNode * value;
    };
    
    // NEW
    struct SimNode_New : SimNode {
        SimNode_New ( const LineInfo & at, int32_t b ) : SimNode(at), bytes(b) {}
        virtual __m128 eval ( Context & context ) override;
        int32_t     bytes;
    };
    
    // CONST-VALUE
    struct SimNode_ConstValue : SimNode {
        SimNode_ConstValue(const LineInfo & at, __m128 c) : SimNode(at) { value.dataVec = c; }
        virtual __m128 eval ( Context & context ) override {
            return value.dataVec;
        }
        ValueVariant value;
    };
    
    // COPY VALUE
    template <typename TT>
    struct SimNode_CopyValue : SimNode {
		YZG_PTR_NODE;
        SimNode_CopyValue(const LineInfo & at, SimNode * ll, SimNode * rr) : SimNode(at), l(ll), r(rr) {};
        __forceinline char * compute ( Context & context ) {
            TT * pl = (TT *) l->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            __m128 rr = r->eval(context);
            YZG_PTR_EXCEPTION_POINT;
            TT * pr = (TT *) &rr;
            *pl = *pr;
            return (char *) pl;
        }
        SimNode * l, * r;
    };
    
    // COPY REFERENCE VALUE
    struct SimNode_CopyRefValue : SimNode {
        SimNode_CopyRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
        uint32_t size;
    };
    
    template <typename TT>
    struct SimNode_CopyRefValueT : SimNode {
		YZG_PTR_NODE;
        SimNode_CopyRefValueT(const LineInfo & at, SimNode * ll, SimNode * rr)
			: SimNode(at), l(ll), r(rr) {};
        __forceinline char * compute ( Context & context ) {
            TT * pl = (TT *) l->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            TT * pr = (TT *) r->evalPtr(context);
            YZG_PTR_EXCEPTION_POINT;
            *pl = *pr;
            return (char *) pl;
        }
        SimNode * l, * r;
    };
    
    // MOVE REFERENCE VALUE
    struct SimNode_MoveRefValue : SimNode {
        SimNode_MoveRefValue(const LineInfo & at, SimNode * ll, SimNode * rr, uint32_t sz)
            : SimNode(at), l(ll), r(rr), size(sz) {};
        virtual __m128 eval ( Context & context ) override;
        SimNode * l, * r;
        uint32_t size;
    };
    
    // BLOCK
    struct SimNode_Block : SimNode {
        SimNode_Block ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode ** list = nullptr;
        uint32_t total = 0;
    };
    
    struct SimNode_ClosureBlock : SimNode_Block {
        SimNode_ClosureBlock ( const LineInfo & at, bool nr, void * ad )
            : SimNode_Block(at), needResult(nr), annotationData(ad) {}
        virtual __m128 eval ( Context & context ) override;
        bool needResult = false;
        void * annotationData = nullptr;
    };
    
    // LET
    struct SimNode_Let : SimNode_Block {
        SimNode_Let ( const LineInfo & at ) : SimNode_Block(at) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode * subexpr = nullptr;
    };
    
    // IF-THEN-ELSE (also Cond)
    struct SimNode_IfThenElse : SimNode {
        SimNode_IfThenElse ( const LineInfo & at, SimNode * c, SimNode * t, SimNode * f )
            : SimNode(at), cond(c), if_true(t), if_false(f) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode * cond, * if_true, * if_false;
    };
    
    // WHILE
    struct SimNode_While : SimNode {
        SimNode_While ( const LineInfo & at, SimNode * c, SimNode * b ) : SimNode(at), cond(c), body(b) {}
        virtual __m128 eval ( Context & context ) override;
        SimNode * cond, * body;
    };
        
    // iterator
    
    struct IteratorContext {
        __m128 value;
        union {
            __m128 tail;
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
        SimNode *   filter;
        uint32_t    size;
    };
    
    struct SimNode_ForWithIteratorBase : SimNode {
        SimNode_ForWithIteratorBase ( const LineInfo & at ) : SimNode(at) {}
        SimNode *   source_iterators[MAX_FOR_ITERATORS];
        SimNode *   body;
        SimNode *   filter;
        uint32_t    stackTop[MAX_FOR_ITERATORS];
    };
    
    template <int total>
    struct SimNode_ForWithIterator : SimNode_ForWithIteratorBase {
        SimNode_ForWithIterator ( const LineInfo & at ) : SimNode_ForWithIteratorBase(at) {}
        virtual __m128 eval ( Context & context ) override {
            __m128 * pi[total];
            for ( int t=0; t!=total; ++t ) {
                pi[t] = (__m128 *)(context.stackTop + stackTop[t]);
            }
            Iterator * sources[total] = {};
            for ( int t=0; t!=total; ++t ) {
                __m128 ll = source_iterators[t]->eval(context);
                YZG_EXCEPTION_POINT;
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
                if ( !filter || cast<bool>::to(filter->eval(context)) ) {
                    if ( !context.stopFlags ) {
                        body->eval(context);
                        if ( context.stopFlags ) goto loopend;
                    }
                }
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
            return _mm_setzero_ps();
        }
    };    

    // op1 policies
    
    struct SimNode_Op1 : SimNode {
        SimNode_Op1 ( const LineInfo & at ) : SimNode(at) {}
		virtual char * evalPtr(Context & context);
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
		virtual char * evalPtr(Context & context);
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
    DEFINE_OP2_POLICY(DivVecScal);
    DEFINE_OP2_POLICY(MulVecScal);
    DEFINE_OP2_POLICY(DivScalVec);
    DEFINE_OP2_POLICY(MulScalVec);
    DEFINE_OP2_POLICY(Mod);
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
    DEFINE_OP2_POLICY(SetDivScal);
    DEFINE_OP2_POLICY(SetMulScal);
    
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
