#include "precomp.h"

#include "module_builtin.h"

#include "ast_interop.h"
#include "ast_policy_types.h"

namespace yzg
{
    Module_BuiltIn::Module_BuiltIn() : Module("$") {
        ModuleLibrary lib;
        lib.addModule(this);
        // pointer
        addFunctionBasic<void *,SimPolicy_Pointer>(*this,lib);
        // string
        addFunctionBasic<char *,SimPolicy_String>(*this,lib);
        addFunctionOrdered<char *, SimPolicy_String>(*this,lib);
        addFunctionConcat<char *, SimPolicy_String>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,   char *,int32_t>>    ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<uint32_t>,  char *,uint32_t>>   ("string",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<float>,     char *,float>>      ("string",lib) );
        // boolean
        addFunctionBasic<bool, SimPolicy_Bool>(*this,lib);
        addFunctionBoolean<bool, SimPolicy_Bool>(*this,lib);
        // int32
        addFunctionBasic<int32_t, SimPolicy_Int>(*this,lib);
        addFunctionNumeric<int32_t, SimPolicy_Int>(*this,lib);
        addFunctionIncDec<int32_t, SimPolicy_Int>(*this,lib);
        addFunctionOrdered<int32_t, SimPolicy_Int>(*this,lib);
        addFunctionBit<int32_t, SimPolicy_Int>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int32_t,float>,int32_t,float>>("int",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int32_t,uint32_t>,int32_t,uint32_t>>("int",lib) );
        // uint32
        addFunctionBasic<uint32_t, SimPolicy_UInt>(*this,lib);
        addFunctionNumeric<uint32_t, SimPolicy_UInt>(*this,lib);
        addFunctionIncDec<uint32_t, SimPolicy_UInt>(*this,lib);
        addFunctionOrdered<uint32_t, SimPolicy_UInt>(*this,lib);
        addFunctionBit<uint32_t, SimPolicy_UInt>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,float>,uint32_t,float>>("uint",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,int32_t>,uint32_t,int32_t>>("uint",lib) );
        // int64
        addFunctionBasic<int64_t, SimPolicy_Int64>(*this,lib);
        addFunctionNumeric<int64_t, SimPolicy_Int64>(*this,lib);
        addFunctionIncDec<int64_t, SimPolicy_Int64>(*this,lib);
        addFunctionOrdered<int64_t, SimPolicy_Int64>(*this,lib);
        addFunctionBit<int64_t, SimPolicy_Int>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int64_t,float>,int64_t,float>>("int64",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<int64_t,uint64_t>,int64_t,uint64_t>>("int64",lib) );
        // uint64
        addFunctionBasic<uint64_t, SimPolicy_UInt64>(*this,lib);
        addFunctionNumeric<uint64_t, SimPolicy_UInt64>(*this,lib);
        addFunctionIncDec<uint64_t, SimPolicy_UInt64>(*this,lib);
        addFunctionOrdered<uint64_t, SimPolicy_UInt64>(*this,lib);
        addFunctionBit<uint64_t, SimPolicy_UInt64>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,float>,uint64_t,float>>("uint64",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,int64_t>,uint64_t,int64_t>>("uint64",lib) );
        // float
        addFunctionBasic<float, SimPolicy_Float>(*this,lib);
        addFunctionNumeric<float, SimPolicy_Float>(*this,lib);
        addFunctionIncDec<float, SimPolicy_Float>(*this,lib);
        addFunctionOrdered<float, SimPolicy_Float>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<float,int32_t>,float,int32_t>>("float",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_Cast<float,uint32_t>,float,uint32_t>>("float",lib) );
        // VECTOR TYPES
        addVectorTypes(lib);
        // RUNTIME
        addRuntime(lib);
    }
}

REGISTER_MODULE_IN_NAMESPACE(Module_BuiltIn,yzg);

