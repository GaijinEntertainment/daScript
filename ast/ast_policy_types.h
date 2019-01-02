#pragma once

namespace  yzg {
    
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
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType {
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)==cast<TT>::to(b));  }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)!=cast<TT>::to(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_GroupByAdd : SimPolicy_CoreType<TT> {
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return cast<TT>::from( -cast<TT>::to(x)); }
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)+cast<TT>::to(b)); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)-cast<TT>::to(b)); }
        static __forceinline void SetAdd  ( char * a, __m128 b, Context & ) { *((TT *)a) += cast<TT>::to(b); }
        static __forceinline void SetSub  ( char * a, __m128 b, Context & ) { *((TT *)a) -= cast<TT>::to(b); }
    };
    
    template <typename TT>
    struct SimPolicy_Ordered {
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<=cast<TT>::to(b));  }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>=cast<TT>::to(b));  }
        static __forceinline __m128 Less    ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<cast<TT>::to(b));  }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>cast<TT>::to(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_GroupByAdd<TT>, SimPolicy_Ordered<TT> {
        // numeric
        static __forceinline __m128 Inc ( __m128 x, Context & ) { (*cast<TT*>::to(x))++; return x; }
        static __forceinline __m128 Dec ( __m128 x, Context & ) { (*cast<TT*>::to(x))--; return x; }
        static __forceinline __m128 IncPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X++); }
        static __forceinline __m128 DecPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X--); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)/cast<TT>::to(b)); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)*cast<TT>::to(b)); }
        static __forceinline void SetDiv  ( char * a, __m128 b, Context & ) { *((TT *)a) *= cast<TT>::to(b); }
        static __forceinline void SetMul  ( char * a, __m128 b, Context & ) { *((TT *)a) /= cast<TT>::to(b); }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool> {
        static __forceinline __m128 BoolNot ( __m128 x, Context & ) { return cast<bool>::from( !cast<bool>::to(x)); }
        static __forceinline __m128 BoolAnd ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) && cast<bool>::to(b)); }
        static __forceinline __m128 BoolOr  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) || cast<bool>::to(b)); }
        static __forceinline __m128 BoolXor ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) != cast<bool>::to(b)); }
        static __forceinline void SetBoolAnd  ( char * a, __m128 b, Context & )
        { auto pa = (bool *) a; *pa = *pa && cast<bool>::to(b); }
        static __forceinline void SetBoolOr   ( char * a, __m128 b, Context & )
        { auto pa = (bool *) a; *pa = *pa || cast<bool>::to(b);  }
        static __forceinline void SetBoolXor  ( char * a, __m128 b, Context & )
        { auto pa = (bool *) a; *pa = *pa != cast<bool>::to(b); }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT> {
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)%cast<TT>::to(b)); }
        static __forceinline __m128 BinNot ( __m128 x, Context & ) { return cast<TT>::from( ~cast<TT>::to(x)); }
        static __forceinline __m128 BinAnd ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) & cast<TT>::to(b)); }
        static __forceinline __m128 BinOr  ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) | cast<TT>::to(b)); }
        static __forceinline __m128 BinXor ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) ^ cast<TT>::to(b)); }
        static __forceinline void SetBinAnd ( char * a, __m128 b, Context & ) { *((TT *)a) &= cast<TT>::to(b); }
        static __forceinline void SetBinOr  ( char * a, __m128 b, Context & ) { *((TT *)a) |= cast<TT>::to(b); }
        static __forceinline void SetBinXor ( char * a, __m128 b, Context & ) { *((TT *)a) ^= cast<TT>::to(b); }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int32_t> {
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_add_epi32(_mm_castps_si128(a), _mm_castps_si128(b)));
        }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_sub_epi32(_mm_castps_si128(a), _mm_castps_si128(b)));
        }
    };
    struct SimPolicy_UInt : SimPolicy_Bin<uint32_t> {};
    struct SimPolicy_Int64 : SimPolicy_Bin<int64_t> {};
    struct SimPolicy_UInt64 : SimPolicy_Bin<uint64_t> {};
    
    struct SimPolicy_Float : SimPolicy_Type<float> {
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_ss(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_ss(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_ss(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_ss(a,b); }
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) {
            return cast<float>::from(fmod(cast<float>::to(a),cast<float>::to(b)));
        }
    };
    
    struct SimPolicy_Pointer : SimPolicy_CoreType<void *> {};
    
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
        mod.addFunction( make_shared<BuiltInFn<Sim_SetAdd<SimPolicy_TT>,     void, TT&, TT>  >("+=",     lib)->sideEffects(true) );
    }
    
    // group by add
    template <typename TT, typename SimPolicy_TT>
    void addFunctionGroupByAdd(Module & mod, const ModuleLibrary & lib, bool hasMod = true) {
        addFunctionConcat<TT,SimPolicy_TT>(mod,lib);
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<SimPolicy_TT>,        TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<SimPolicy_TT>,        TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<SimPolicy_TT>,        TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<SimPolicy_TT>,     void, TT&, TT>  >("-=",     lib)->sideEffects(true) );
    }
    
    // numeric types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionNumeric(Module & mod, const ModuleLibrary & lib, bool hasMod = true) {
        addFunctionGroupByAdd<TT,SimPolicy_TT>(mod,lib);
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Unp<SimPolicy_TT>,        TT,   TT>       >("+",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Unm<SimPolicy_TT>,        TT,   TT>       >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Sub<SimPolicy_TT>,        TT,   TT,  TT>  >("-",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Mul<SimPolicy_TT>,        TT,   TT,  TT>  >("*",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Div<SimPolicy_TT>,        TT,   TT,  TT>  >("/",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetSub<SimPolicy_TT>,     void, TT&, TT>  >("-=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMul<SimPolicy_TT>,     void, TT&, TT>  >("*=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDiv<SimPolicy_TT>,     void, TT&, TT>  >("/=",     lib)->sideEffects(true) );
        // optional
        if ( hasMod ) {
            mod.addFunction( make_shared<BuiltInFn<Sim_Mod<SimPolicy_TT>,        TT,   TT,  TT>  >("%",      lib) );
        }
    }
    
    // vector-scalar combinations
    template <typename TT, typename TTS, typename SimPolicy_TT>
    void addFunctionVecNumeric(Module & mod, const ModuleLibrary & lib, bool hasMod = true) {
        addFunctionGroupByAdd<TT,SimPolicy_TT>(mod,lib);
        //                                     policy                           ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_MulScalVec<SimPolicy_TT>,    TT,   TTS, TT>  >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivScalVec<SimPolicy_TT>,    TT,   TTS, TT>  >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_MulVecScal<SimPolicy_TT>,    TT,   TT,  TTS> >("*",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DivVecScal<SimPolicy_TT>,    TT,   TT,  TTS> >("/",    lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetMulScal<SimPolicy_TT>,    void, TT&, TT>  >("*=",   lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetDivScal<SimPolicy_TT>,    void, TT&, TT>  >("/=",   lib)->sideEffects(true) );
    }
    
    // inc-dec
    template <typename TT, typename SimPolicy_TT>
    void addFunctionIncDec(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_Inc<SimPolicy_TT>,        TT&,  TT&>      >("++",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_Dec<SimPolicy_TT>,        TT&,  TT&>      >("--",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_IncPost<SimPolicy_TT>,    TT,   TT&>      >("+++",    lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_DecPost<SimPolicy_TT>,    TT,   TT&>      >("---",    lib)->sideEffects(true) );
    }
    
    // built-in numeric types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionBit(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BinNot<SimPolicy_TT>,     TT,   TT>       >("~",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinAnd<SimPolicy_TT>,     TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinOr<SimPolicy_TT>,      TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BinXor<SimPolicy_TT>,     TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinAnd<SimPolicy_TT>,  void, TT,  TT&> >("&=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinOr<SimPolicy_TT>,   void, TT,  TT&> >("|=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBinXor<SimPolicy_TT>,  void, TT,  TT&> >("^=",     lib)->sideEffects(true) );
    }
    
    // built-in boolean types
    template <typename TT, typename SimPolicy_TT>
    void addFunctionBoolean(Module & mod, const ModuleLibrary & lib) {
        //                                    policy                        ret   arg1 arg2    name
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolNot<SimPolicy_Bool>,  TT,   TT>       >("!",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolAnd,                  TT,   TT,  TT>  >("&",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolOr,                   TT,   TT,  TT>  >("|",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_BoolXor<SimPolicy_Bool>,  TT,   TT,  TT>  >("^",      lib) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolAnd<SimPolicy_Bool>,void,TT&, TT>  >("&=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolOr<SimPolicy_Bool>, void,TT&, TT>  >("|=",     lib)->sideEffects(true) );
        mod.addFunction( make_shared<BuiltInFn<Sim_SetBoolXor<SimPolicy_Bool>,void,TT&, TT>  >("^=",     lib)->sideEffects(true) );
    }
}
