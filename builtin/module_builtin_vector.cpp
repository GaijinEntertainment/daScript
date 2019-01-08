#include "precomp.h"

#include "module_builtin.h"

#include "ast_interop.h"
#include "ast_policy_types.h"

namespace das
{
    template <typename TT, int mask>
    struct SimPolicy_Vec {
        // basic
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return vec_equ_ps(a,b,mask);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return vec_nequ_ps(a,b,mask);
        }
        // numeric
        static __forceinline vec4f Unp ( vec4f x, Context & ) {
            return x;
        }
        static __forceinline vec4f Unm ( vec4f x, Context & ) {
            return vec_neg_ps(x);
        }
        static __forceinline vec4f Add ( vec4f a, vec4f b, Context & ) {
            return vec_add_ps(a,b);
        }
        static __forceinline vec4f Sub ( vec4f a, vec4f b, Context & ) {
            return vec_sub_ps(a,b);
        }
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return vec_div_ps(a,b);
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return vec_mod_ps(a,b);
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return vec_mul_ps(a,b);
        }
        static __forceinline void SetAdd  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *) a;
            *pa = cast<TT>::to ( vec_add_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetSub  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_sub_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_div_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_mul_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_mod_ps(cast<TT>::from(*pa), b));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_div_ps(a,vec_shuffle_ps_xxxx(b));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_mul_ps(a,vec_shuffle_ps_xxxx(b));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_div_ps(vec_shuffle_ps_xxxx(a),b);
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_mul_ps(vec_shuffle_ps_xxxx(a),b);
        }
        static __forceinline void SetDivScal  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_div_ps(cast<TT>::from(*pa), vec_shuffle_ps_xxxx(b)));
        }
        static __forceinline void SetMulScal  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( vec_mul_ps(cast<TT>::from(*pa), vec_shuffle_ps_xxxx(b)));
        }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_iVec {
        // basic
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return vec_equ_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b),mask);
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return vec_nequ_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b),mask);
        }
        // numeric
        static __forceinline vec4f Unp ( vec4f x, Context & ) {
            return x;
        }
        static __forceinline vec4f Unm ( vec4f x, Context & ) {
            return vec_cast_esi_ps(vec_neg_epi(vec_cast_ps_esi(x)));
        }
        static __forceinline vec4f Add ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_add_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Sub ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_sub_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mod_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epi(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline void SetAdd  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_add_epi(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetSub  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_sub_epi(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_div_epi(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mul_epi(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mod_epi(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epi(vec_cast_ps_esi(a),vec_shuffle_epi_xxxx(vec_cast_ps_esi(b))));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epi(vec_cast_ps_esi(a),vec_shuffle_epi_xxxx(vec_cast_ps_esi(b))));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epi(vec_shuffle_epi_xxxx(vec_cast_ps_esi(a)),vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epi(vec_shuffle_epi_xxxx(vec_cast_ps_esi(a)),vec_cast_ps_esi(b)));
        }
        static __forceinline void SetDivScal ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_div_epi(vec_cast_ps_esi(cast<TT>::from(*pa)),
                                                            vec_shuffle_epi_xxxx(vec_cast_ps_esi(b)))));
        }
        static __forceinline void SetMulScal ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mul_epi(vec_cast_ps_esi(cast<TT>::from(*pa)),
                                                            vec_shuffle_epi_xxxx(vec_cast_ps_esi(b)))));
        }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_uVec : SimPolicy_iVec<TT,mask> {
        // swapping some numeric operations
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epu(vec_cast_ps_esi(a), vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epu(vec_cast_ps_esi(a), vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mod_epu(vec_cast_ps_esi(a),vec_cast_ps_esi(b)));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_div_epu(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mul_epu(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mod_epu(vec_cast_ps_esi(cast<TT>::from(*pa)), vec_cast_ps_esi(b))));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epu(vec_cast_ps_esi(a),vec_shuffle_epi_xxxx(vec_cast_ps_esi(b))));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epu(vec_cast_ps_esi(a),vec_shuffle_epi_xxxx(vec_cast_ps_esi(b))));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_div_epu(vec_shuffle_epi_xxxx(vec_cast_ps_esi(a)), vec_cast_ps_esi(b)));
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return vec_cast_esi_ps(vec_mul_epu(vec_shuffle_epi_xxxx(vec_cast_ps_esi(a)), vec_cast_ps_esi(b)));
        }
        static __forceinline void SetDivScal ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_div_epu(vec_cast_ps_esi(cast<TT>::from(*pa)),
                                                            vec_shuffle_epi_xxxx(vec_cast_ps_esi(b)))));
        }
        static __forceinline void SetMulScal ( char * a, vec4f b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (vec_cast_esi_ps(vec_mul_epu(vec_cast_ps_esi(cast<TT>::from(*pa)),
                                                            vec_shuffle_epi_xxxx(vec_cast_ps_esi(b)))));
        }
    };
    
    struct SimPolicy_Range : SimPolicy_iVec<range,3> {};
    struct SimPolicy_URange : SimPolicy_uVec<urange,3> {};
    
    template <> struct SimPolicy<float2> : SimPolicy_Vec<float2,1+2> {};
    template <> struct SimPolicy<float3> : SimPolicy_Vec<float3,1+2+4> {};
    template <> struct SimPolicy<float4> : SimPolicy_Vec<float4,1+2+4+8> {};
    template <> struct SimPolicy<int2> : SimPolicy_iVec<int2,1+2> {};
    template <> struct SimPolicy<int3> : SimPolicy_iVec<int3,1+2+4> {};
    template <> struct SimPolicy<int4> : SimPolicy_iVec<int4,1+2+4+8> {};
    template <> struct SimPolicy<uint2> : SimPolicy_uVec<uint2,1+2> {};
    template <> struct SimPolicy<uint3> : SimPolicy_uVec<uint3,1+2+4> {};
    template <> struct SimPolicy<uint4> : SimPolicy_uVec<uint4,1+2+4+8> {};
    
    template <> struct SimPolicy<range> : SimPolicy_Range {};
    template <> struct SimPolicy<urange> : SimPolicy_URange {};

    DEFINE_VECTOR_POLICY(float2);
    DEFINE_VECTOR_POLICY(float3);
    DEFINE_VECTOR_POLICY(float4);
    DEFINE_VECTOR_POLICY(int2);
    DEFINE_VECTOR_POLICY(int3);
    DEFINE_VECTOR_POLICY(int4);
    DEFINE_VECTOR_POLICY(uint2);
    DEFINE_VECTOR_POLICY(uint3);
    DEFINE_VECTOR_POLICY(uint4);
    
    DEFINE_OP2_EVAL_BASIC_POLICY(range);
    DEFINE_OP2_EVAL_BASIC_POLICY(urange);
    
    void Module_BuiltIn::addVectorTypes(ModuleLibrary & lib) {
        // float2
        addFunctionBasic<float2>(*this,lib);
        addFunctionNumeric<float2>(*this,lib);
        addFunctionVecNumeric<float2,float>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,float2,float,float>>("float2",lib) );
        // float3
        addFunctionBasic<float3>(*this,lib);
        addFunctionNumeric<float3>(*this,lib);
        addFunctionVecNumeric<float3, float>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,float3,float,float,float>>("float3",lib) );
        // float4
        addFunctionBasic<float4>(*this,lib);
        addFunctionNumeric<float4>(*this,lib);
        addFunctionVecNumeric<float4, float>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,float4,float,float,float,float>>("float4",lib) );
        // int2
        addFunctionBasic<int2>(*this,lib);
        addFunctionNumeric<int2>(*this,lib);
        addFunctionVecNumeric<int2, int32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,int2,int32_t,int32_t>>("int2",lib) );
        // int3
        addFunctionBasic<int3>(*this,lib);
        addFunctionNumeric<int3>(*this,lib);
        addFunctionVecNumeric<int3, int32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,int3,int32_t,int32_t,int32_t>>("int3",lib) );
        // int4
        addFunctionBasic<int4>(*this,lib);
        addFunctionNumeric<int4>(*this,lib);
        addFunctionVecNumeric<int4,int32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,int4,int32_t,int32_t,int32_t,int32_t>>("int4",lib) );
        // uint2
        addFunctionBasic<uint2>(*this,lib);
        addFunctionNumeric<uint2>(*this,lib);
        addFunctionVecNumeric<uint2, uint32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,uint2,uint32_t,uint32_t>>("uint2",lib) );
        // uint3
        addFunctionBasic<uint3>(*this,lib);
        addFunctionNumeric<uint3>(*this,lib);
        addFunctionVecNumeric<uint3,uint32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<3>,uint3,uint32_t,uint32_t,uint32_t>>("uint3",lib) );
        // uint4
        addFunctionBasic<uint4>(*this,lib);
        addFunctionNumeric<uint4>(*this,lib);
        addFunctionVecNumeric<uint4, uint32_t>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<4>,uint4,uint32_t,uint32_t,uint32_t,uint32_t>>("uint4",lib) );
        // range
       addFunctionBasic<range>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,range,int32_t,int32_t>>("range",lib) );
        // urange
       // addFunctionBasic<urange,SimPolicy_URange>(*this,lib);
        addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<2>,urange,uint32_t,uint32_t>>("urange",lib) );
    }
}
