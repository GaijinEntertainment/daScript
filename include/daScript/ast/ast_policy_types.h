#pragma once

namespace  das {

    template  <typename SimT, typename RetT, typename ...Args>
    class BuiltInFn : public BuiltInFunction {
    public:
        BuiltInFn(const string & fn, const ModuleLibrary & lib) : BuiltInFunction(fn) {
            this->result = makeType<RetT>(lib);
            this->totalStackSize = sizeof(Prologue);
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( size_t argi=0; argi != args.size(); ++argi ) {
                auto arg = make_shared<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                this->arguments.push_back(arg);
            }
            // copy on return and move on return
            if ( result->isRefType() ) {
                if ( result->canCopy() ) {
                    copyOnReturn = true;
                    moveOnReturn = false;
                } else if ( result->canMove() ) {
                    copyOnReturn = false;
                    moveOnReturn = true;
                } else {
                    DAS_FATAL_LOG("BuiltInFn %s can't be bound. It returns values which can't be copied or moved\n", fn.c_str());
                    DAS_FATAL_ERROR;
                }
            }
        }
        virtual SimNode * makeSimNode ( Context & context ) override {
            return context.code->makeNode<SimT>(at);
        }
    };

    // basic operations
    template <typename TT>
    void addFunctionBasic(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Equ<TT>,         bool, TT,  TT>  >("==",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_NotEqu<TT>,      bool, TT,  TT>  >("!=",     lib) );
    }

    // built-in boolean types
    template <typename TT>
    void addFunctionBoolean(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolNot<TT>,     TT,   TT>       >("!",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolAnd,         TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolOr,          TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolXor<TT>,     TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolAnd<TT>,  void, TT&, TT>  >("&=",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolOr<TT>,   void, TT&, TT>  >("|=",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolXor<TT>,  void, TT&, TT>  >("^=",     lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // ordered types
    template <typename TT>
    void addFunctionOrdered(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_GtEqu<TT>,      bool, TT,  TT>  >(">=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_LessEqu<TT>,    bool, TT,  TT>  >("<=",     lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Gt<TT>,         bool, TT,  TT>  >(">",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Less<TT>,       bool, TT,  TT>  >("<",      lib) );
    }

    // min-max types
    template <typename TT>
    void addFunctionMinMax(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Min<TT>,        TT,   TT,  TT>  >("min",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Max<TT>,        TT,   TT,  TT>  >("max",    lib) );
    }

    // concatination types
    template <typename TT>
    void addFunctionConcat(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Add<TT>,        TT,   TT,  TT>  >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetAdd<TT>,     void, TT&, TT>  >("+=",     lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // group by add
    template <typename TT>
    void addFunctionGroupByAdd(Module & mod, const ModuleLibrary & lib) {
        addFunctionConcat<TT>(mod,lib);
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<TT>,        TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<TT>,        TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<TT>,        TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<TT>,     void, TT&, TT>  >("-=",     lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // numeric types
    template <typename TT>
    void addFunctionNumeric(Module & mod, const ModuleLibrary & lib) {
        addFunctionGroupByAdd<TT>(mod,lib);
        //                                     policy           ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Mul<TT>,     TT,   TT,  TT>  >("*",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Div<TT>,     TT,   TT,  TT>  >("/",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMul<TT>,  void, TT&, TT>  >("*=",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDiv<TT>,  void, TT&, TT>  >("/=",     lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // numeric types
    template <typename TT>
    void addFunctionNumericWithMod(Module & mod, const ModuleLibrary & lib) {
        addFunctionNumeric<TT>(mod, lib);
        //                                     policy       ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Mod<TT>, TT,   TT,  TT>  >("%",      lib) );
    }

    // vector-scalar combinations
    template <typename TT, typename TTS>
    void addFunctionVecNumeric(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_MulScalVec<TT>,  TT,   TTS, TT>  >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivScalVec<TT>,  TT,   TTS, TT>  >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_MulVecScal<TT>,  TT,   TT,  TTS> >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivVecScal<TT>,  TT,   TT,  TTS> >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMulScal<TT>,  void, TT&, TTS>  >("*=",   lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDivScal<TT>,  void, TT&, TTS>  >("/=",   lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // inc-dec
    template <typename TT>
    void addFunctionIncDec(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1         name
        mod.addFunction( make_shared<BuiltInFn<Sim_Inc<TT>,        TT,   TT&>      >("++",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Dec<TT>,        TT,   TT&>      >("--",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_IncPost<TT>,    TT,   TT&>      >("+++",    lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DecPost<TT>,    TT,   TT&>      >("---",    lib)->setSideEffects(SideEffects::modifyArgument) );
    }

    // built-in numeric types
    template <typename TT>
    void addFunctionBit(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BinNot<TT>,     TT,   TT>       >("~",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinAnd<TT>,     TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinOr<TT>,      TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinXor<TT>,     TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinAnd<TT>,  void, TT,  TT&> >("&=",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinOr<TT>,   void, TT,  TT&> >("|=",     lib)->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinXor<TT>,  void, TT,  TT&> >("^=",     lib)->setSideEffects(SideEffects::modifyArgument) );
    }
}
