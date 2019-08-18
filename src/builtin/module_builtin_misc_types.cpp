#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/simulate/sim_policy.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

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

    template <> struct typeName<EnumStub>  { static string name() { return "enum"; } };

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
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitOp2(vis, "EqFunPtr", sizeof(Func), "Func");
        }
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Func>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return !rv && lv.index==0;      // they only equal if both null
        }
    };

    struct Sim_NEqFunPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_NEqFunPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitOp2(vis, "NEqFunPtr", sizeof(Func), "Func");
        }
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Func>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return rv || lv.index;
        }
    };

    struct Sim_EqLambdaPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_EqLambdaPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitOp2(vis, "EqLambdaPtr", sizeof(Lambda), "Lambda");
        }
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Lambda>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return !rv && !lv.capture;      // they only equal if both null
        }
    };

    struct Sim_NEqLambdaPtr : SimNode_Op2 {
        DAS_BOOL_NODE;
        Sim_NEqLambdaPtr ( const LineInfo & at ) : SimNode_Op2(at) {}
        virtual SimNode * visit ( SimVisitor & vis ) override {
            return visitOp2(vis, "NEqLambdaPtr", sizeof(Lambda), "Lambda");
        }
        __forceinline bool compute ( Context & context ) {
            auto lv = cast<Lambda>::to(l->eval(context));
            auto rv = r->evalPtr(context);
            return rv || lv.capture;
        }
    };

    void Module_BuiltIn::addMiscTypes(ModuleLibrary & lib) {
        // enum
        addFunctionBasic<EnumStub>(*this,lib);
        addExtern<DAS_BIND_FUN(enum_to_int)>(*this, lib, "int", SideEffects::none, "int");
        // function
        addFunctionBasic<Func>(*this,lib);
        addFunction( make_shared<BuiltInFn<Sim_EqFunPtr, bool,const Func,const void *>>("==",lib,"==",false) );
        addFunction( make_shared<BuiltInFn<Sim_NEqFunPtr,bool,const Func,const void *>>("!=",lib,"!=",false) );
        // lambda
        addFunction( make_shared<BuiltInFn<Sim_EqLambdaPtr, bool,const Lambda,const void *>>("==",lib,"==",false) );
        addFunction( make_shared<BuiltInFn<Sim_NEqLambdaPtr,bool,const Lambda,const void *>>("!=",lib,"!=",false) );
        // string
        addFunctionBasic<char *>(*this,lib);
        addFunctionOrdered<char *>(*this,lib);
        addFunctionConcat<char *>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,   char *,int32_t,Context *>>    ("string",lib,"das_lexical_cast",false) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint32_t>,  char *,uint32_t,Context *>>   ("string",lib,"das_lexical_cast",false) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int64_t>,   char *,int64_t,Context *>>    ("string",lib,"das_lexical_cast",false) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint64_t>,  char *,uint64_t,Context *>>   ("string",lib,"das_lexical_cast",false) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<float>,     char *,float,Context *>>      ("string",lib,"das_lexical_cast",false) );
    }
}

