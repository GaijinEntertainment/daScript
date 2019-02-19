#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

#include "module_builtin.cpp.inc"

namespace das
{
    // string
    DEFINE_OP2_EVAL_BASIC_POLICY(char *);
    DEFINE_OP2_EVAL_ORDERED_POLICY(char *);
    DEFINE_OP2_EVAL_GROUPBYADD_POLICY(char *);

    template <>
    struct cast <EnumStub> {
        static __forceinline struct EnumStub to ( vec4f x )               { union { struct EnumStub t; vec4f vec; } T; T.vec = x; return T.t; }
        static __forceinline vec4f EnumStub ( EnumStub x )         { union { struct EnumStub t; vec4f vec; } T; T.t = x; return T.vec; }
    };

    template<>
    struct SimPolicy<EnumStub> {
        static __forceinline int32_t to_enum ( vec4f val ) {
            return cast<int32_t>::to(val);
        }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return to_enum(a) == to_enum(b);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return to_enum(a) != to_enum(b);
        }
    };

    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ,EnumStub);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu,EnumStub);

    __forceinline int32_t enum_to_int ( EnumStub stub ) {
        return stub.value;
    }

    template <>
    struct SimPolicy<Func> {
        static __forceinline int32_t to_func ( vec4f val ) {
            return cast<Func>::to(val).index;
        }
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return to_func(a) == to_func(b);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return to_func(a) != to_func(b);
        }
    };

    IMPLEMENT_OP2_EVAL_BOOL_POLICY(Equ,Func);
    IMPLEMENT_OP2_EVAL_BOOL_POLICY(NotEqu,Func);

    struct Sim_EqFunPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_EqFunPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Func>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return !rv && lv.index==0;      // they only equal if both null
        }
    };

    struct Sim_NEqFunPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_NEqFunPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Func>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return rv || lv.index;
        }
    };

    struct Sim_EqLambdaPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_EqLambdaPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Lambda>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return !rv && !lv.capture;      // they only equal if both null
        }
    };

    struct Sim_NEqLambdaPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_NEqLambdaPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Lambda>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return rv || lv.capture;
        }
    };

    Module_BuiltIn::Module_BuiltIn() : Module("$") {
        ModuleLibrary lib;
        lib.addModule(this);
        // enum
        addFunctionBasic<EnumStub>(*this,lib);
        addExtern<DAS_BIND_FUN(enum_to_int)>(*this, lib, "int", SideEffects::none);
        // boolean
        addFunctionBasic<bool>(*this,lib);
        addFunctionBoolean<bool>(*this,lib);
        // pointer
        addFunctionBasic<void *>(*this,lib);
        // function
        addFunctionBasic<Func>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_EqFunPtr, bool,Func,void *>>("==",lib) );
        addFunction( make_shared<BuiltInFn<Sim_NEqFunPtr,bool,Func,void *>>("!=",lib) );
        // lambda
        addFunction( make_shared<BuiltInFn<Sim_EqLambdaPtr, bool,Lambda,void *>>("==",lib) );
        addFunction( make_shared<BuiltInFn<Sim_NEqLambdaPtr,bool,Lambda,void *>>("!=",lib) );
        // int32
        addFunctionBasic<int32_t>(*this,lib);
        addFunctionNumericWithMod<int32_t>(*this,lib);
        addFunctionIncDec<int32_t>(*this,lib);
        addFunctionOrdered<int32_t>(*this,lib);
        addFunctionMinMax<int32_t>(*this,lib);
        addFunctionBit<int32_t>(*this,lib);
        ADD_NUMERIC_CASTS(int, int32_t);
        // uint32
        addFunctionBasic<uint32_t>(*this,lib);
        addFunctionNumericWithMod<uint32_t>(*this,lib);
        addFunctionIncDec<uint32_t>(*this,lib);
        addFunctionOrdered<uint32_t>(*this,lib);
        addFunctionMinMax<uint32_t>(*this,lib);
        addFunctionBit<uint32_t>(*this,lib);
        ADD_NUMERIC_CASTS(uint, uint32_t);
        ADD_NUMERIC_CASTS(int64, int64_t);
        ADD_NUMERIC_CASTS(uint64, uint64_t);
        add64bitFunctions(lib);
        // float
        addFunctionBasic<float>(*this,lib);
        addFunctionNumeric<float>(*this,lib);
        addFunctionIncDec<float>(*this,lib);
        addFunctionOrdered<float>(*this,lib);
        addFunctionMinMax<float>(*this,lib);
        ADD_NUMERIC_CASTS(float, float)
        // double
        addFunctionBasic<double>(*this,lib);
        addFunctionNumeric<double>(*this,lib);
        addFunctionIncDec<double>(*this,lib);
        addFunctionOrdered<double>(*this,lib);
        addFunctionMinMax<double>(*this,lib);
        ADD_NUMERIC_CASTS(double, double)
        // string
        addFunctionBasic<char *>(*this,lib);
        addFunctionOrdered<char *>(*this,lib);
        addFunctionConcat<char *>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,   char *,int32_t>>    ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint32_t>,  char *,uint32_t>>   ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int64_t>,   char *,int64_t>>    ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint64_t>,  char *,uint64_t>>   ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<float>,     char *,float>>      ("string",lib) );
        // VECTOR & MATRIX TYPES
        addVectorTypes(lib);
        addMatrixTypes(lib);
        // ARRAYS
        addArrayTypes(lib);
        // RUNTIME
        addRuntime(lib);
        // TIME
        addTime(lib);
        // NOW, for the builtin module
        appendCompiledFunctions();
    }
}

REGISTER_MODULE_IN_NAMESPACE(Module_BuiltIn,das);

