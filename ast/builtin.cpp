#include "precomp.h"

#include "ast.h"

namespace yzg
{
    // "PRINT"
    struct SimNode_Print : SimNode_Call {
        SimNode_Print ( const LineInfo & at ) : SimNode_Call(at) {}
        virtual __m128 eval ( Context & context ) override {
            evalArgs(context);
            YZG_EXCEPTION_POINT;
            context.to_out(to_rts(abiArgValues(context)[0]));
            return _mm_setzero_ps();
        }
    };
    
    // "BREAKPOINT"
    struct SimNode_BreakPoint : SimNode {
        SimNode_BreakPoint ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            context.breakPoint(debug.column, debug.line);
            return _mm_setzero_ps();
        }
    };
    
    // "STACKWALK"
    struct SimNode_StackWalk : SimNode {
        SimNode_StackWalk ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            context.stackWalk();
            return _mm_setzero_ps();
        }
    };
    
    // "TERMINATE"
    struct SimNode_Terminate : SimNode {
        SimNode_Terminate ( const LineInfo & at ) : SimNode(at) {}
        virtual __m128 eval ( Context & context ) override {
            context.stopFlags |= EvalFlags::stopForTerminate;
            return _mm_setzero_ps();
        }
    };

    void Program::addBuiltinFunctions()
    {
        addBuiltIn(make_shared<BuiltInFn<SimNode_Print,void,char *>>("print", *this));
        addBuiltIn(make_shared<BuiltInFn<SimNode_StackWalk,void>>("stackwalk", *this));
        addBuiltIn(make_shared<BuiltInFn<SimNode_BreakPoint,void>>("breakpoint", *this));
        addBuiltIn(make_shared<BuiltInFn<SimNode_Terminate,void>>("terminate", *this));
    }
    
    // basic operations
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInBasic(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
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
    
    // inc-dec
    template <typename TT, typename SimPolicy_TT>
    void addBuiltInIncDec(Program & prg)
    {
        //                                    policy                        ret   arg1 arg2    name
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Inc<SimPolicy_TT>,        TT&,  TT&>      >("++",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_Dec<SimPolicy_TT>,        TT&,  TT&>      >("--",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_IncPost<SimPolicy_TT>,    TT,   TT&>      >("+++",    prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_DecPost<SimPolicy_TT>,    TT,   TT&>      >("---",    prg) );
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
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolAnd,                  TT,   TT,  TT>  >("&",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolOr,                   TT,   TT,  TT>  >("|",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_BoolXor<SimPolicy_Bool>,  TT,   TT,  TT>  >("^",      prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolAnd<SimPolicy_Bool>,TT&, TT&, TT>  >("&=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolOr<SimPolicy_Bool>, TT&, TT&, TT>  >("|=",     prg) );
        prg.addBuiltIn( make_shared<BuiltInFn<Sim_SetBoolXor<SimPolicy_Bool>,TT&, TT&, TT>  >("^=",     prg) );
    }
    
    void Program::addBuiltinOperators()
    {
        // pointer
        addBuiltInBasic<void *,SimPolicy_Pointer>(*this);
        // string
        addBuiltInBasic<char *,SimPolicy_String>(*this);
        addBuiltInOrdered<char *, SimPolicy_String>(*this);
        // boolean
        addBuiltInBasic<bool, SimPolicy_Bool>(*this);
        addBuiltInBoolean<bool, SimPolicy_Bool>(*this);
        // int32
        addBuiltInBasic<int32_t, SimPolicy_Int>(*this);
        addBuiltInNumeric<int32_t, SimPolicy_Int>(*this);
        addBuiltInIncDec<int32_t, SimPolicy_Int>(*this);
        addBuiltInOrdered<int32_t, SimPolicy_Int>(*this);
        addBuiltInBit<int32_t, SimPolicy_Int>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int32_t,float>,int32_t,float>>("int",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<int32_t,uint32_t>,int32_t,uint32_t>>("int",*this) );
        // uint32
        addBuiltInBasic<uint32_t, SimPolicy_UInt>(*this);
        addBuiltInNumeric<uint32_t, SimPolicy_UInt>(*this);
        addBuiltInIncDec<uint32_t, SimPolicy_UInt>(*this);
        addBuiltInOrdered<uint32_t, SimPolicy_UInt>(*this);
        addBuiltInBit<uint32_t, SimPolicy_UInt>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,float>,uint32_t,float>>("uint",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<uint32_t,int32_t>,uint32_t,int32_t>>("uint",*this) );
        // float
        addBuiltInBasic<float, SimPolicy_Float>(*this);
        addBuiltInNumeric<float, SimPolicy_Float>(*this);
        addBuiltInIncDec<float, SimPolicy_Float>(*this);
        addBuiltInOrdered<float, SimPolicy_Float>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,int32_t>,float,int32_t>>("float",*this) );
        addBuiltIn ( make_shared<BuiltInFn<SimNode_Cast<float,uint32_t>,float,uint32_t>>("float",*this) );
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
        // int2
        addBuiltInBasic<int2, SimPolicy_iVec<int2,3>>(*this);
        addBuiltInNumeric<int2, SimPolicy_iVec<int2,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<2>,int2,int32_t,int32_t>>("int2",*this) );
        // int3
        addBuiltInBasic<int3, SimPolicy_iVec<int3,7>>(*this);
        addBuiltInNumeric<int3, SimPolicy_iVec<int3,7>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<3>,int3,int32_t,int32_t,int32_t>>("int3",*this) );
        // int4
        addBuiltInBasic<int4, SimPolicy_iVec<int4,3>>(*this);
        addBuiltInNumeric<int4, SimPolicy_iVec<int4,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<4>,int4,int32_t,int32_t,int32_t,int32_t>>("int4",*this) );
        // uint2
        addBuiltInBasic<uint2, SimPolicy_uVec<uint2,3>>(*this);
        addBuiltInNumeric<uint2, SimPolicy_uVec<uint2,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<2>,uint2,uint32_t,uint32_t>>("uint2",*this) );
        // uint3
        addBuiltInBasic<uint3, SimPolicy_uVec<uint3,7>>(*this);
        addBuiltInNumeric<uint3, SimPolicy_uVec<uint3,7>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<3>,uint3,uint32_t,uint32_t,uint32_t>>("uint3",*this) );
        // uint4
        addBuiltInBasic<uint4, SimPolicy_uVec<uint4,3>>(*this);
        addBuiltInNumeric<uint4, SimPolicy_uVec<uint4,3>>(*this);
        addBuiltIn ( make_shared<BuiltInFn<SimNode_VecCtor<4>,uint4,uint32_t,uint32_t,uint32_t,uint32_t>>("uint4",*this) );
    }

}
