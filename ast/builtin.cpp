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
    
    // POLICY BASED OPERATIONS
    
    template <typename TT>
    struct SimPolicy_CoreType {
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)==cast<TT>::to(b));  }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)!=cast<TT>::to(b));  }
    };
    
    template <typename TT>
    struct SimPolicy_Type : SimPolicy_CoreType<TT> {
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return cast<TT>::from( -cast<TT>::to(x)); }
        static __forceinline __m128 Inc ( __m128 x, Context & ) { (*cast<TT*>::to(x))++; return x; }
        static __forceinline __m128 Dec ( __m128 x, Context & ) { (*cast<TT*>::to(x))--; return x; }
        static __forceinline __m128 IncPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X++); }
        static __forceinline __m128 DecPost ( __m128 x, Context & ) { TT & X = *cast<TT*>::to(x); return cast<TT>::from(X--); }
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)+cast<TT>::to(b)); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)-cast<TT>::to(b)); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)/cast<TT>::to(b)); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)*cast<TT>::to(b)); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) += cast<TT>::to(b); return a; }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) -= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) *= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) /= cast<TT>::to(b); return a; }
        // ordered
        static __forceinline __m128 LessEqu ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<=cast<TT>::to(b));  }
        static __forceinline __m128 GtEqu   ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>=cast<TT>::to(b));  }
        static __forceinline __m128 Less    ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)<cast<TT>::to(b));  }
        static __forceinline __m128 Gt      ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<TT>::to(a)>cast<TT>::to(b));  }
    };
    
    struct SimPolicy_Bool : SimPolicy_CoreType<bool> {
        static __forceinline __m128 BoolNot ( __m128 x, Context & ) { return cast<bool>::from( !cast<bool>::to(x)); }
        static __forceinline __m128 BoolAnd ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) && cast<bool>::to(b)); }
        static __forceinline __m128 BoolOr  ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) || cast<bool>::to(b)); }
        static __forceinline __m128 BoolXor ( __m128 a, __m128 b, Context & ) { return cast<bool>::from(cast<bool>::to(a) != cast<bool>::to(b)); }
        static __forceinline __m128 SetBoolAnd  ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa && cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolOr   ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa || cast<bool>::to(b); return a; }
        static __forceinline __m128 SetBoolXor  ( __m128 a, __m128 b, Context & )
            { auto pa = cast<bool *>::to(a); *pa = *pa != cast<bool>::to(b); return a; }
    };
    
    template <typename TT>
    struct SimPolicy_Bin : SimPolicy_Type<TT> {
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a)%cast<TT>::to(b)); }
        static __forceinline __m128 BinNot ( __m128 x, Context & ) { return cast<TT>::from( ~cast<TT>::to(x)); }
        static __forceinline __m128 BinAnd ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) & cast<TT>::to(b)); }
        static __forceinline __m128 BinOr  ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) | cast<TT>::to(b)); }
        static __forceinline __m128 BinXor ( __m128 a, __m128 b, Context & ) { return cast<TT>::from(cast<TT>::to(a) ^ cast<TT>::to(b)); }
        static __forceinline __m128 SetBinAnd ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) &= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinOr  ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) |= cast<TT>::to(b); return a; }
        static __forceinline __m128 SetBinXor ( __m128 a, __m128 b, Context & ) { *cast<TT *>::to(a) ^= cast<TT>::to(b); return a; }
    };
    
    struct SimPolicy_Int : SimPolicy_Bin<int32_t> {};
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
    
    template <typename TT, int mask>
    struct SimPolicy_Vec {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128 _mm_mod_ps(__m128  a, __m128 aDiv) {
            __m128 c = _mm_div_ps(a,aDiv);
            __m128i i = _mm_cvttps_epi32(c);
            __m128 cTrunc = _mm_cvtepi32_ps(i);
            __m128 base = _mm_mul_ps(cTrunc, aDiv);
            __m128 r = _mm_sub_ps(a, base);
            return r;
        }
        // basic
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask ); }
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask ); }
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return _mm_sub_ps(_mm_setzero_ps(), x); }    // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_ps(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_ps(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_ps(a,b); }
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) { return _mm_mod_ps(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_ps(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_ps(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_ps(cast<TT>::from(*pa), b)); return a;   }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_iVec {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128 _mm_mul_epi32 ( __m128 a, __m128 b ) {
            __m128i A = a, B = b, C;
            C[0] = A[0]*B[0];   C[1] = A[1]*B[1];   C[2] = A[2]*B[2];   C[3] = A[3]*B[3];
            return C;
        }
        static __forceinline __m128 _mm_div_epi32 ( __m128 a, __m128 b ) {
            __m128i A = a, B = b, C;
            C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
            return C;
        }
        static __forceinline __m128 _mm_mod_epi32 ( __m128 a, __m128 b ) {
            __m128i A = a, B = b, C;
            C[0] = A[0]%B[0];   C[1] = A[1]%B[1];   C[2] = A[2]%B[2];   C[3] = A[3]%B[3];
            return C;
        }
        // basic
        static __forceinline __m128 Equ     ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)==mask ); }            // todo: verify
        static __forceinline __m128 NotEqu  ( __m128 a, __m128 b, Context & )
            { return cast<bool>::from( (_mm_movemask_ps(_mm_cmpeq_epi32(a,b)) & mask)!=mask ); }            // todo: verify
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) { return x; }
        static __forceinline __m128 Unm ( __m128 x, Context & ) { return _mm_sub_epi32(_mm_setzero_si128(), x); }      // todo: optimize?
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) { return _mm_add_epi32(a,b); }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) { return _mm_sub_epi32(a,b); }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_epi32(a,b); }
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) { return _mm_mod_epi32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_epi32(a,b); }
        static __forceinline __m128 SetAdd  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_add_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetSub  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_sub_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epi32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_epi32(cast<TT>::from(*pa), b)); return a;   }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_uVec : SimPolicy_iVec<TT,mask> {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128 _mm_div_epu32 ( __m128 a, __m128 b) {
            __m128 c;  uint32_t * A = (uint32_t *)&a, * B = (uint32_t *)&b, * C = (uint32_t *)&c;
            C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
            return c;
        }
        // swappmin some numeric operations
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) { return _mm_div_epu32(a,b); }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) { return _mm_mul_epu32(a,b); }
        static __forceinline __m128 SetDiv  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_div_epu32(cast<TT>::from(*pa), b)); return a;   }
        static __forceinline __m128 SetMul  ( __m128 a, __m128 b, Context & )
            {   TT * pa = cast<TT *>::to(a);   *pa = cast<TT>::to ( _mm_mul_epu32(cast<TT>::from(*pa), b)); return a;   }
    };
    
    struct SimPolicy_Pointer : SimPolicy_CoreType<void *> {};
    
    struct SimPolicy_Range : SimPolicy_iVec<range,3> {};
    struct SimPolicy_URange : SimPolicy_uVec<urange,3> {};
    
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
        // range
        addFunctionBasic<void *,SimPolicy_Range>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,range,int32_t,int32_t>>("range",lib) );
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,urange,uint32_t,uint32_t>>("urange",lib) );
        // urange
        addFunctionBasic<void *,SimPolicy_URange>(*this,lib);
        // RUNTIME
        addRuntime(lib);
    }
}
