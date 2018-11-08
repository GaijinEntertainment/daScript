//
//  builtin.cpp
//  yzg
//
//  Created by Boris Batkin on 11/1/18.
//  Copyright © 2018 Boris Batkin. All rights reserved.
//

#include "ast.hpp"

namespace yzg
{
    // basic operations
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBasic(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<SimNode_Debug<TT>,            TT,   TT>       >("debug",  prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Set<SimPolicy_TT>,        TT&,  TT&, TT>  >("=",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Equ<SimPolicy_TT>,        bool, TT,  TT>  >("==",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_NotEqu<SimPolicy_TT>,     bool, TT,  TT>  >("!=",     prg) );
    }
    
    // ordered types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInOrdered(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_GtEqu<SimPolicy_TT>,      bool, TT,  TT>  >(">=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_LessEqu<SimPolicy_TT>,    bool, TT,  TT>  >("<=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Gt<SimPolicy_TT>,         bool, TT,  TT>  >(">",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Less<SimPolicy_TT>,       bool, TT,  TT>  >("<",      prg) );
    }
    
    // numeric types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInNumeric(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Unp<SimPolicy_TT>,        TT,   TT>       >("+",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Unm<SimPolicy_TT>,        TT,   TT>       >("-",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Add<SimPolicy_TT>,        TT,   TT,  TT>  >("+",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Sub<SimPolicy_TT>,        TT,   TT,  TT>  >("-",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Mul<SimPolicy_TT>,        TT,   TT,  TT>  >("*",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Div<SimPolicy_TT>,        TT,   TT,  TT>  >("/",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetAdd<SimPolicy_TT>,     TT&,  TT&, TT>  >("+=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetSub<SimPolicy_TT>,     TT&,  TT&, TT>  >("-=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetMul<SimPolicy_TT>,     TT&,  TT&, TT>  >("*=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetDiv<SimPolicy_TT>,     TT&,  TT&, TT>  >("/=",     prg) );
    }
    
    // built-in numeric types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBit(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinNot<SimPolicy_TT>,     TT,   TT>       >("~",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinAnd<SimPolicy_TT>,     TT,   TT,  TT>  >("&",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinOr<SimPolicy_TT>,      TT,   TT,  TT>  >("|",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BinXor<SimPolicy_TT>,     TT,   TT,  TT>  >("^",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinAnd<SimPolicy_TT>,  TT&,  TT,  TT&> >("&=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinOr<SimPolicy_TT>,   TT&,  TT,  TT&> >("|=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBinXor<SimPolicy_TT>,  TT&,  TT,  TT&> >("^=",     prg) );
    }
    
    // built-in boolean types
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBoolean(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolNot<SimPolicy_Bool>,  TT,   TT>       >("!",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolAnd<SimPolicy_Bool>,  TT,   TT,  TT>  >("&",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolOr<SimPolicy_Bool>,   TT,   TT,  TT>  >("|",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolXor<SimPolicy_Bool>,  TT,   TT,  TT>  >("^",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolAnd<SimPolicy_Bool>,TT&, TT&, TT>  >("&=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolOr<SimPolicy_Bool>, TT&, TT&, TT>  >("|=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolXor<SimPolicy_Bool>,TT&, TT&, TT>  >("^=",     prg) );
    }
    
    void Program::addBuiltinOperators()
    {
        // boolean
        addBuiltInBasic<bool, SimPolicy_Bool>(*this);
        addBuiltInBoolean<bool, SimPolicy_Bool>(*this);
        // int64
        addBuiltInBasic<int64_t, SimPolicy_Int>(*this);
        addBuiltInNumeric<int64_t, SimPolicy_Int>(*this);
        addBuiltInOrdered<int64_t, SimPolicy_Int>(*this);
        addBuiltInBit<int64_t, SimPolicy_Int>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int64_t,float>,int64_t,float>>("int",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int64_t,uint64_t>,int64_t,uint64_t>>("int",*this) );
        // uint64
        addBuiltInBasic<uint64_t, SimPolicy_UInt>(*this);
        addBuiltInNumeric<uint64_t, SimPolicy_UInt>(*this);
        addBuiltInOrdered<uint64_t, SimPolicy_UInt>(*this);
        addBuiltInBit<uint64_t, SimPolicy_UInt>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,float>,uint64_t,float>>("uint",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint64_t,int64_t>,uint64_t,int64_t>>("uint",*this) );
        // float
        addBuiltInBasic<float, SimPolicy_Float>(*this);
        addBuiltInNumeric<float, SimPolicy_Float>(*this);
        addBuiltInOrdered<float, SimPolicy_Float>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,int64_t>,float,int64_t>>("float",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,uint64_t>,float,uint64_t>>("float",*this) );
        // float2
        addBuiltInBasic<float2, SimPolicy_Vec<float2,3>>(*this);
        addBuiltInNumeric<float2, SimPolicy_Vec<float2,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<2>,float2,float,float>>("float2",*this) );
        // float3
        addBuiltInBasic<float3, SimPolicy_Vec<float3,7>>(*this);
        addBuiltInNumeric<float3, SimPolicy_Vec<float3,7>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<3>,float3,float,float,float>>("float3",*this) );
        // float4
        addBuiltInBasic<float4, SimPolicy_Vec<float4,3>>(*this);
        addBuiltInNumeric<float4, SimPolicy_Vec<float4,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<4>,float4,float,float,float,float>>("float4",*this) );
    }

}
