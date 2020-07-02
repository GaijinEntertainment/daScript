#pragma once

namespace  das {

    template  <typename SimT, typename RetT, typename ...Args>
    class BuiltInFn : public BuiltInFunction {
    public:
        BuiltInFn(const string & fn, const ModuleLibrary & lib, const string & cna = string(), bool pbas = true)
        : BuiltInFunction(fn,cna) {
            this->policyBased = pbas;
            this->result = makeType<RetT>(lib);
            this->totalStackSize = sizeof(Prologue);
            vector<TypeDeclPtr> args = { makeType<Args>(lib)... };
            for ( size_t argi=0; argi != args.size(); ++argi ) {
                auto arg = make_smart<Variable>();
                arg->name = "arg" + to_string(argi);
                arg->type = args[argi];
                if ( arg->type->baseType==Type::fakeContext ) {
                    arg->init = make_smart<ExprFakeContext>(at);
                } else if ( arg->type->baseType==Type::fakeLineInfo ) {
                    arg->init = make_smart<ExprFakeLineInfo>(at);
                }
                if ( arg->type->isTempType() ) {
                    arg->type->implicit = true;
                }
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
                } else if ( !result->ref ) {
                    DAS_FATAL_LOG("BuiltInFn %s can't be bound. It returns values which can't be copied or moved\n", fn.c_str());
                    DAS_FATAL_ERROR;
                }
            }
        }
        virtual SimNode * makeSimNode ( Context & context, const vector<ExpressionPtr> & ) override {
            return context.code->makeNode<SimT>(at);
        }
    };

    // basic operations
    template <typename TT, typename PT = TT>
    void addFunctionBasic(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Equ<PT>,         bool, const TT,  const TT>  >("==",     lib, "Equ") );
        mod.addFunction( make_smart<BuiltInFn<Sim_NotEqu<PT>,      bool, const TT,  const TT>  >("!=",     lib, "NotEqu") );
    }

    // basic operations
    template <typename TT, typename QQ>
    void addFunctionBasicEx(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Equ<TT>,         bool, const TT,  const QQ>  >("==",     lib, "Equ") );
        mod.addFunction( make_smart<BuiltInFn<Sim_NotEqu<TT>,      bool, const TT,  const QQ>  >("!=",     lib, "NotEqu") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Equ<TT>,         bool, const QQ,  const TT>  >("==",     lib, "Equ") );
        mod.addFunction( make_smart<BuiltInFn<Sim_NotEqu<TT>,      bool, const QQ,  const TT>  >("!=",     lib, "NotEqu") );
    }

    // built-in boolean types
    template <typename TT>
    void addFunctionBoolean(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_BoolNot<TT>,     TT,   TT>       >("!",      lib, "BoolNot") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BoolAnd,         TT,   TT,  TT>  >("&&",     lib, "BoolAnd") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BoolOr,          TT,   TT,  TT>  >("||",     lib, "BoolOr") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BoolXor<TT>,     TT,   TT,  TT>  >("^^",     lib, "BoolXor") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBoolAnd,      void, TT&, TT>  >("&&=",    lib, "SetBoolAnd")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBoolOr,       void, TT&, TT>  >("||=",    lib, "SetBoorOr")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBoolXor<TT>,  void, TT&, TT>  >("^^=",    lib, "SetBoolXor")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // ordered types
    template <typename TT>
    void addFunctionOrdered(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_GtEqu<TT>,      bool, TT,  TT>  >(">=",     lib, "GtEqu") );
        mod.addFunction( make_smart<BuiltInFn<Sim_LessEqu<TT>,    bool, TT,  TT>  >("<=",     lib, "LessEqu") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Gt<TT>,         bool, TT,  TT>  >(">",      lib, "Gt") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Less<TT>,       bool, TT,  TT>  >("<",      lib, "Less") );
    }

    // concatination types
    template <typename TT>
    void addFunctionConcat(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Add<TT>,        TT,   TT,  TT>  >("+",      lib, "Add") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetAdd<TT>,     void, TT&, TT>  >("+=",     lib, "SetAdd")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // group by add
    template <typename TT>
    void addFunctionGroupByAdd(Module & mod, const ModuleLibrary & lib) {
        addFunctionConcat<TT>(mod,lib);
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Unp<TT>,        TT,   TT>       >("+",      lib, "Unp") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Unm<TT>,        TT,   TT>       >("-",      lib, "Unm") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Sub<TT>,        TT,   TT,  TT>  >("-",      lib, "Sub") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetSub<TT>,     void, TT&, TT>  >("-=",     lib, "SetSub")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // numeric types
    template <typename TT>
    void addFunctionNumeric(Module & mod, const ModuleLibrary & lib) {
        addFunctionGroupByAdd<TT>(mod,lib);
        //                                     policy           ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Mul<TT>,     TT,   TT,  TT>  >("*",      lib, "Mul") );
        mod.addFunction( make_smart<BuiltInFn<Sim_Div<TT>,     TT,   TT,  TT>  >("/",      lib, "Div") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetMul<TT>,  void, TT&, TT>  >("*=",     lib, "SetMul")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetDiv<TT>,  void, TT&, TT>  >("/=",     lib, "SetDiv")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // numeric types
    template <typename TT>
    void addFunctionNumericWithMod(Module & mod, const ModuleLibrary & lib) {
        addFunctionNumeric<TT>(mod, lib);
        //                                     policy       ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_Mod<TT>, TT,   TT,  TT>  >("%",      lib, "Mod") );
    }

    // vector-scalar combinations
    template <typename TT, typename TTS>
    void addFunctionVecNumeric(Module & mod, const ModuleLibrary & lib) {
        //                                     policy               ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_MulScalVec<TT>,  TT,   TTS, TT>  >("*",    lib, "MulScalVec") );
        mod.addFunction( make_smart<BuiltInFn<Sim_DivScalVec<TT>,  TT,   TTS, TT>  >("/",    lib, "DivScalVec") );
        mod.addFunction( make_smart<BuiltInFn<Sim_MulVecScal<TT>,  TT,   TT,  TTS> >("*",    lib, "MulVecScal") );
        mod.addFunction( make_smart<BuiltInFn<Sim_DivVecScal<TT>,  TT,   TT,  TTS> >("/",    lib, "DivVecScal") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetMulScal<TT>,  void, TT&, TTS>  >("*=",  lib, "SetMulScal")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetDivScal<TT>,  void, TT&, TTS>  >("/=",  lib, "SetDivScal")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // inc-dec
    template <typename TT>
    void addFunctionIncDec(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1         name
        mod.addFunction( make_smart<BuiltInFn<Sim_Inc<TT>,        TT,   TT&>      >("++",     lib, "Inc")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_Dec<TT>,        TT,   TT&>      >("--",     lib, "Dec")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_IncPost<TT>,    TT,   TT&>      >("+++",    lib, "IncPost")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_DecPost<TT>,    TT,   TT&>      >("---",    lib, "DecPost")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // built-in numeric types
    template <typename TT,typename PT = TT>
    void addFunctionBitLogic(Module & mod, const ModuleLibrary & lib) {
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_BinNot<PT>,     TT,   TT>       >("~",      lib, "BinNot") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinAnd<PT>,     TT,   TT,  TT>  >("&",      lib, "BinAnd") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinOr<PT>,      TT,   TT,  TT>  >("|",      lib, "BinOr") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinXor<PT>,     TT,   TT,  TT>  >("^",      lib, "BinXor") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinAnd<PT>,  void, TT&, TT>  >("&=",     lib, "SetBinAnd")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinOr<PT>,   void, TT&, TT>  >("|=",     lib, "SetBinOr")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinXor<PT>,  void, TT&, TT>  >("^=",     lib, "SetBinXor")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }

    // built-in numeric types
    template <typename TT,typename PT = TT>
    void addFunctionBit(Module & mod, const ModuleLibrary & lib) {
        addFunctionBitLogic<TT,PT>(mod,lib);
        //                                     policy              ret   arg1 arg2    name
        mod.addFunction( make_smart<BuiltInFn<Sim_BinShl<PT>,     TT,   TT,  TT>  >("<<",     lib, "BinShl") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinShr<PT>,     TT,   TT,  TT>  >(">>",     lib, "BinShr") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinRotl<PT>,    TT,   TT,  TT>  >("<<<",    lib, "BinRotl") );
        mod.addFunction( make_smart<BuiltInFn<Sim_BinRotr<PT>,    TT,   TT,  TT>  >(">>>",    lib, "BinRotr") );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinShl<PT>,  void, TT&, TT>  >("<<=",    lib, "SetBinShl")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinShr<PT>,  void, TT&, TT>  >(">>=",    lib, "SetBinShr")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinRotl<PT>,  void, TT&, TT>  >("<<<=",  lib, "SetBinRotl")
                        ->setSideEffects(SideEffects::modifyArgument) );
        mod.addFunction( make_smart<BuiltInFn<Sim_SetBinRotr<PT>,  void, TT&, TT>  >(">>>=",  lib, "SetBinRotr")
                        ->setSideEffects(SideEffects::modifyArgument) );
    }
}
