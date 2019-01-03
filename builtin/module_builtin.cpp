#include "precomp.h"

#include "module_builtin.h"

#include "ast_interop.h"
#include "ast_policy_types.h"

namespace yzg
{
    // unary
    DEFINE_OP1_NUMERIC(Unp);
    DEFINE_OP1_NUMERIC(Unm);
    DEFINE_OP1_SET_NUMERIC(Inc);
    DEFINE_OP1_SET_NUMERIC(Dec);
    DEFINE_OP1_POSTSET_NUMERIC(IncPost);
    DEFINE_OP1_POSTSET_NUMERIC(DecPost);
    DEFINE_OP1_NUMERIC_INTEGER(BinNot);
    DEFINE_POLICY(BoolNot);
    IMPLEMENT_OP1_POLICY(BoolNot, Bool, bool);
    // binary
    // +,-,*,/,%
    DEFINE_OP2_NUMERIC(Add);
    DEFINE_OP2_NUMERIC(Sub);
    DEFINE_OP2_NUMERIC(Mul);
    DEFINE_OP2_NUMERIC(Div);
    DEFINE_OP2_NUMERIC(Mod);
    DEFINE_OP2_SET_NUMERIC(SetAdd);
    DEFINE_OP2_SET_NUMERIC(SetSub);
    DEFINE_OP2_SET_NUMERIC(SetMul);
    DEFINE_OP2_SET_NUMERIC(SetDiv);
    DEFINE_OP2_SET_NUMERIC(SetMod);
    // comparisons
    DEFINE_OP2_BOOL_NUMERIC(Equ);
    DEFINE_OP2_BOOL_NUMERIC(NotEqu);
    DEFINE_OP2_BOOL_NUMERIC(LessEqu);
    DEFINE_OP2_BOOL_NUMERIC(GtEqu);
    DEFINE_OP2_BOOL_NUMERIC(Less);
    DEFINE_OP2_BOOL_NUMERIC(Gt);
    DEFINE_OP2_BASIC_POLICY(Bool,bool);
    DEFINE_OP2_BASIC_POLICY(Ptr,void *);
    // binary and, or, xor
    DEFINE_OP2_NUMERIC_INTEGER(BinAnd);
    DEFINE_OP2_NUMERIC_INTEGER(BinOr);
    DEFINE_OP2_NUMERIC_INTEGER(BinXor);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinAnd);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinOr);
    DEFINE_OP2_SET_NUMERIC_INTEGER(SetBinXor);
    // boolean and, or, xor
    DEFINE_POLICY(SetBoolAnd);
    IMPLEMENT_OP2_SET_POLICY(SetBoolAnd, Bool, bool);
    DEFINE_POLICY(SetBoolOr);
    IMPLEMENT_OP2_SET_POLICY(SetBoolOr, Bool, bool);
    DEFINE_POLICY(SetBoolXor);
    IMPLEMENT_OP2_SET_POLICY(SetBoolXor, Bool, bool);
    DEFINE_POLICY(BoolXor);
    IMPLEMENT_OP2_POLICY(BoolXor, Bool, bool);
    // string
    DEFINE_OP2_EVAL_BASIC_POLICY(char *);
    DEFINE_OP2_EVAL_ORDERED_POLICY(char *);
    DEFINE_OP2_EVAL_GROUPBYADD_POLICY(char *);
    
    Module_BuiltIn::Module_BuiltIn() : Module("$") {
        ModuleLibrary lib;
        lib.addModule(this);
        
        // boolean
        addFunctionBasic<bool>(*this,lib);
        addFunctionBoolean<bool>(*this,lib);
        // pointer
        addFunctionBasic<void *>(*this,lib);
        // int32
        addFunctionBasic<int32_t>(*this,lib);
        addFunctionNumericWithMod<int32_t>(*this,lib);
        addFunctionIncDec<int32_t>(*this,lib);
        addFunctionOrdered<int32_t>(*this,lib);
        addFunctionBit<int32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int32_t,float>,int32_t,float>>("int",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int32_t,uint32_t>,int32_t,uint32_t>>("int",lib) );
        // uint32
        addFunctionBasic<uint32_t>(*this,lib);
        addFunctionNumericWithMod<uint32_t>(*this,lib);
        addFunctionIncDec<uint32_t>(*this,lib);
        addFunctionOrdered<uint32_t>(*this,lib);
        addFunctionBit<uint32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,float>,uint32_t,float>>("uint",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,int32_t>,uint32_t,int32_t>>("uint",lib) );
        // int64
        addFunctionBasic<int64_t>(*this,lib);
        addFunctionNumericWithMod<int64_t>(*this,lib);
        addFunctionIncDec<int64_t>(*this,lib);
        addFunctionOrdered<int64_t>(*this,lib);
        addFunctionBit<int64_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int64_t,float>,int64_t,float>>("int64",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int64_t,uint64_t>,int64_t,uint64_t>>("int64",lib) );
        // uint64
        addFunctionBasic<uint64_t>(*this,lib);
        addFunctionNumericWithMod<uint64_t>(*this,lib);
        addFunctionIncDec<uint64_t>(*this,lib);
        addFunctionOrdered<uint64_t>(*this,lib);
        addFunctionBit<uint64_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,float>,uint64_t,float>>("uint64",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,int64_t>,uint64_t,int64_t>>("uint64",lib) );
        // float
        addFunctionBasic<float>(*this,lib);
        addFunctionNumeric<float>(*this,lib);
        addFunctionIncDec<float>(*this,lib);
        addFunctionOrdered<float>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<float,int32_t>,float,int32_t>>("float",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<float,uint32_t>,float,uint32_t>>("float",lib) );
        // string
        addFunctionBasic<char *>(*this,lib);
        addFunctionOrdered<char *>(*this,lib);
        addFunctionConcat<char *>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,   char *,int32_t>>    ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint32_t>,  char *,uint32_t>>   ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<float>,     char *,float>>      ("string",lib) );
        // VECTOR TYPES
        addVectorTypes(lib);
        // RUNTIME
        addRuntime(lib);
        // TIME
        addTime(lib);
    }
}

REGISTER_MODULE_IN_NAMESPACE(Module_BuiltIn,yzg);

