#include "precomp.h"

#include "module_builtin.h"

#include "ast_interop.h"
#include "ast_policy_types.h"

namespace yzg
{

    
    
/*
    DEFINE_OP2_POLICY(DivVecScal);
    DEFINE_OP2_POLICY(MulVecScal);
    DEFINE_OP2_POLICY(DivScalVec);
    DEFINE_OP2_POLICY(MulScalVec);
    DEFINE_OP2_SET_POLICY(SetBoolAnd);
    DEFINE_OP2_SET_POLICY(SetBoolOr);
    DEFINE_OP2_SET_POLICY(SetBoolXor);
    DEFINE_OP2_SET_POLICY(SetDivScal);
    DEFINE_OP2_SET_POLICY(SetMulScal);
*/
    
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
/*
        addFunctionBasic<char *,SimPolicy_String>(*this,lib);
        addFunctionOrdered<char *, SimPolicy_String>(*this,lib);
        addFunctionConcat<char *, SimPolicy_String>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,   char *,int32_t>>    ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint32_t>,  char *,uint32_t>>   ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<float>,     char *,float>>      ("string",lib) );
*/
 
        // VECTOR TYPES
        addVectorTypes(lib);
        // RUNTIME
        addRuntime(lib);
        // TIME
        addTime(lib);
    }
}

REGISTER_MODULE_IN_NAMESPACE(Module_BuiltIn,yzg);

