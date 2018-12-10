#include "precomp.h"

#include "ast.h"

namespace yzg
{
    template  <typename SimT, typename RetT, typename ...Args>
    class BuiltInFn : public BuiltInFunction {
    public:
        BuiltInFn(const string & fn, const ModuleLibrary & lib) : BuiltInFunction(fn) {
            this->result = makeType<RetT>(lib);
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( int argi=0; argi != args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + std::to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.makeNode<SimT>(at);
        }
    };
    
    // basic operations
    template <typename TT, typename SimPolicy_TT>
    void addFunctionBasic(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Equ<SimPolicy_TT>,        bool, TT,  TT>  >("==",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_NotEqu<SimPolicy_TT>,     bool, TT,  TT>  >("!=",     lib) );
    }
    
    // ordered types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionOrdered(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_GtEqu<SimPolicy_TT>,      bool, TT,  TT>  >(">=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_LessEqu<SimPolicy_TT>,    bool, TT,  TT>  >("<=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Gt<SimPolicy_TT>,         bool, TT,  TT>  >(">",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Less<SimPolicy_TT>,       bool, TT,  TT>  >("<",      lib) );
    }
    
    // concatination types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionConcat(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Add<SimPolicy_TT>,        TT,   TT,  TT>  >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetAdd<SimPolicy_TT>,     TT&,  TT&, TT>  >("+=",     lib) );
    }
    
    // numeric types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionNumeric(Module & mod, const ModuleLibrary & lib, bool hasMod = true) {
        addFunctionConcat<TT,SimPolicy_TT>(mod,lib);
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<SimPolicy_TT>,        TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<SimPolicy_TT>,        TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<SimPolicy_TT>,        TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Mul<SimPolicy_TT>,        TT,   TT,  TT>  >("*",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Div<SimPolicy_TT>,        TT,   TT,  TT>  >("/",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<SimPolicy_TT>,     TT&,  TT&, TT>  >("-=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMul<SimPolicy_TT>,     TT&,  TT&, TT>  >("*=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDiv<SimPolicy_TT>,     TT&,  TT&, TT>  >("/=",     lib) );
        // optional
        if ( hasMod ) {
        mod.addFunction( make_shared<BuiltInFn<Sim_Mod<SimPolicy_TT>,        TT,   TT,  TT>  >("%",      lib) );
        }
    }
    
    // inc-dec
    template <typename TT, typename SimPolicy_TT>
    void addFunctionIncDec(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Inc<SimPolicy_TT>,        TT&,  TT&>      >("++",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Dec<SimPolicy_TT>,        TT&,  TT&>      >("--",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_IncPost<SimPolicy_TT>,    TT,   TT&>      >("+++",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DecPost<SimPolicy_TT>,    TT,   TT&>      >("---",    lib) );
    }
    
    // built-in numeric types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionBit(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BinNot<SimPolicy_TT>,     TT,   TT>       >("~",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinAnd<SimPolicy_TT>,     TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinOr<SimPolicy_TT>,      TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinXor<SimPolicy_TT>,     TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinAnd<SimPolicy_TT>,  TT&,  TT,  TT&> >("&=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinOr<SimPolicy_TT>,   TT&,  TT,  TT&> >("|=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinXor<SimPolicy_TT>,  TT&,  TT,  TT&> >("^=",     lib) );
    }
    
    // built-in boolean types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionBoolean(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolNot<SimPolicy_Bool>,  TT,   TT>       >("!",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolAnd,                  TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolOr,                   TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolXor<SimPolicy_Bool>,  TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolAnd<SimPolicy_Bool>,TT&, TT&, TT>  >("&=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolOr<SimPolicy_Bool>, TT&, TT&, TT>  >("|=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolXor<SimPolicy_Bool>,TT&, TT&, TT>  >("^=",     lib) );
    }
    
    Module_BuiltIn::Module_BuiltIn() {
        ModuleLibrary lib;
        // pointer
        addFunctionBasic<void *,SimPolicy_Pointer>(*this,lib);
        // string
        addFunctionBasic<char *,SimPolicy_String>(*this,lib);
        addFunctionOrdered<char *, SimPolicy_String>(*this,lib);
        addFunctionConcat<char *, SimPolicy_String>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_LexicalCast<int32_t>,char *,int32_t>>("string",lib) );
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
        // float2
        addFunctionBasic<float2, SimPolicy_Vec<float2,3>>(*this,lib);
        addFunctionNumeric<float2, SimPolicy_Vec<float2,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,float2,float,float>>("float2",lib) );
        // float3
        addFunctionBasic<float3, SimPolicy_Vec<float3,7>>(*this,lib);
        addFunctionNumeric<float3, SimPolicy_Vec<float3,7>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,float3,float,float,float>>("float3",lib) );
        // float4
        addFunctionBasic<float4, SimPolicy_Vec<float4,3>>(*this,lib);
        addFunctionNumeric<float4, SimPolicy_Vec<float4,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,float4,float,float,float,float>>("float4",lib) );
        // int2
        addFunctionBasic<int2, SimPolicy_iVec<int2,3>>(*this,lib);
        addFunctionNumeric<int2, SimPolicy_iVec<int2,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,int2,int32_t,int32_t>>("int2",lib) );
        // int3
        addFunctionBasic<int3, SimPolicy_iVec<int3,7>>(*this,lib);
        addFunctionNumeric<int3, SimPolicy_iVec<int3,7>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,int3,int32_t,int32_t,int32_t>>("int3",lib) );
        // int4
        addFunctionBasic<int4, SimPolicy_iVec<int4,3>>(*this,lib);
        addFunctionNumeric<int4, SimPolicy_iVec<int4,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,int4,int32_t,int32_t,int32_t,int32_t>>("int4",lib) );
        // uint2
        addFunctionBasic<uint2, SimPolicy_uVec<uint2,3>>(*this,lib);
        addFunctionNumeric<uint2, SimPolicy_uVec<uint2,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,uint2,uint32_t,uint32_t>>("uint2",lib) );
        // uint3
        addFunctionBasic<uint3, SimPolicy_uVec<uint3,7>>(*this,lib);
        addFunctionNumeric<uint3, SimPolicy_uVec<uint3,7>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,uint3,uint32_t,uint32_t,uint32_t>>("uint3",lib) );
        // uint4
        addFunctionBasic<uint4, SimPolicy_uVec<uint4,3>>(*this,lib);
        addFunctionNumeric<uint4, SimPolicy_uVec<uint4,3>>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,uint4,uint32_t,uint32_t,uint32_t,uint32_t>>("uint4",lib) );
        // RUNTIME
        addRuntime(lib);
    }
}
