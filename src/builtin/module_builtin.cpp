#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das
{
    // unary
    DEFINE_OP1_NUMERIC(Unp);
    DEFINE_OP1_NUMERIC(Unm);
    DEFINE_OP1_SET_NUMERIC(Inc);
    DEFINE_OP1_SET_NUMERIC(Dec);
    DEFINE_OP1_SET_NUMERIC(IncPost);
    DEFINE_OP1_SET_NUMERIC(DecPost);
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
        
#define ADD_NUMERIC_CASTS(TYPE,CTYPE)                                                                   \
    addFunction ( make_shared<BuiltInFn<SimNode_Zero,CTYPE>>(#TYPE,lib) );                              \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,float>,CTYPE,float>>(#TYPE,lib) );           \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,int32_t>,CTYPE,int32_t>>(#TYPE,lib) );       \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,uint32_t>,CTYPE,uint32_t>>(#TYPE,lib) );     \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,int64_t>,CTYPE,int64_t>>(#TYPE,lib) );       \
    addFunction ( make_shared<BuiltInFn<SimNode_Cast<CTYPE,uint64_t>,CTYPE,uint64_t>>(#TYPE,lib) );
        
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
        ADD_NUMERIC_CASTS(int, int32_t);
        // uint32
        addFunctionBasic<uint32_t>(*this,lib);
        addFunctionNumericWithMod<uint32_t>(*this,lib);
        addFunctionIncDec<uint32_t>(*this,lib);
        addFunctionOrdered<uint32_t>(*this,lib);
        addFunctionBit<uint32_t>(*this,lib);
        ADD_NUMERIC_CASTS(uint, uint32_t);
        // int64
        addFunctionBasic<int64_t>(*this,lib);
        addFunctionNumericWithMod<int64_t>(*this,lib);
        addFunctionIncDec<int64_t>(*this,lib);
        addFunctionOrdered<int64_t>(*this,lib);
        addFunctionBit<int64_t>(*this,lib);
        ADD_NUMERIC_CASTS(int64, int64_t);
        // uint64
        addFunctionBasic<uint64_t>(*this,lib);
        addFunctionNumericWithMod<uint64_t>(*this,lib);
        addFunctionIncDec<uint64_t>(*this,lib);
        addFunctionOrdered<uint64_t>(*this,lib);
        addFunctionBit<uint64_t>(*this,lib);
        ADD_NUMERIC_CASTS(uint64, uint64_t);
        // float
        addFunctionBasic<float>(*this,lib);
        addFunctionNumeric<float>(*this,lib);
        addFunctionIncDec<float>(*this,lib);
        addFunctionOrdered<float>(*this,lib);
        ADD_NUMERIC_CASTS(float, float)
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
        // RUNTIME
        addRuntime(lib);
        // TIME
        addTime(lib);
		// NOW, for the builtin module
		appendCompiledFunctions();
    }
}

REGISTER_MODULE_IN_NAMESPACE(Module_BuiltIn,das);

