#pragma once

namespace  das {
    
    template  <typename SimT, typename RetT, typename ...Args>
    class BuiltInFn : public BuiltInFunction {
    public:
        BuiltInFn(const string & fn, const ModuleLibrary & lib) : BuiltInFunction(fn) {
            noSideEffects = true;
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
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolAnd<TT>,  void, TT&, TT>  >("&=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolOr<TT>,   void, TT&, TT>  >("|=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolXor<TT>,  void, TT&, TT>  >("^=",     lib)->sideEffects(true) );
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
    

    // concatination types
    template <typename TT>
    void addFunctionConcat(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Add<TT>,        TT,   TT,  TT>  >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetAdd<TT>,     void, TT&, TT>  >("+=",     lib)->sideEffects(true) );
    }
    
    // group by add
    template <typename TT>
    void addFunctionGroupByAdd(Module & mod, const ModuleLibrary & lib) {
        addFunctionConcat<TT>(mod,lib);
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<TT>,        TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<TT>,        TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<TT>,        TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<TT>,     void, TT&, TT>  >("-=",     lib)->sideEffects(true) );
    }
    
    // numeric types
    template <typename TT>
    void addFunctionNumeric(Module & mod, const ModuleLibrary & lib) {
        addFunctionGroupByAdd<TT>(mod,lib);
        //                                     policy           ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<TT>,     TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<TT>,     TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<TT>,     TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Mul<TT>,     TT,   TT,  TT>  >("*",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Div<TT>,     TT,   TT,  TT>  >("/",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<TT>,  void, TT&, TT>  >("-=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMul<TT>,  void, TT&, TT>  >("*=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDiv<TT>,  void, TT&, TT>  >("/=",     lib)->sideEffects(true) );
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
    void addFunctionVecNumeric(Module & mod, const ModuleLibrary & lib, bool hasMod = true) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_MulScalVec<TT>,  TT,   TTS, TT>  >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivScalVec<TT>,  TT,   TTS, TT>  >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_MulVecScal<TT>,  TT,   TT,  TTS> >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivVecScal<TT>,  TT,   TT,  TTS> >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMulScal<TT>,  void, TT&, TT>  >("*=",   lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDivScal<TT>,  void, TT&, TT>  >("/=",   lib)->sideEffects(true) );
    }
    
    // inc-dec
    template <typename TT>
    void addFunctionIncDec(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1         name
        mod.addFunction( make_shared<BuiltInFn<Sim_Inc<TT>,        TT,   TT&>      >("++",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Dec<TT>,        TT,   TT&>      >("--",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_IncPost<TT>,    TT,   TT&>      >("+++",    lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DecPost<TT>,    TT,   TT&>      >("---",    lib)->sideEffects(true) );
    }
    
    // built-in numeric types
    template <typename TT>
    void addFunctionBit(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BinNot<TT>,     TT,   TT>       >("~",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinAnd<TT>,     TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinOr<TT>,      TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinXor<TT>,     TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinAnd<TT>,  void, TT,  TT&> >("&=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinOr<TT>,   void, TT,  TT&> >("|=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinXor<TT>,  void, TT,  TT&> >("^=",     lib)->sideEffects(true) );
    }
}
