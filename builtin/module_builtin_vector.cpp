#include "precomp.h"

#include "module_builtin.h"

#include "ast_interop.h"
#include "ast_policy_types.h"

namespace yzg
{
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
        static __forceinline bool Equ     ( __m128 a, __m128 b, Context & ) {
            return (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)==mask;
        }
        static __forceinline bool NotEqu  ( __m128 a, __m128 b, Context & ) {
            return (_mm_movemask_ps(_mm_cmpeq_ps(a,b)) & mask)!=mask;
        }
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) {
            return x;
        }
        static __forceinline __m128 Unm ( __m128 x, Context & ) {   // todo: optimize?
            return _mm_sub_ps(_mm_setzero_ps(), x);
        }
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) {
            return _mm_add_ps(a,b);
        }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) {
            return _mm_sub_ps(a,b);
        }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) {
            return _mm_div_ps(a,b);
        }
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) {
            return _mm_mod_ps(a,b);
        }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) {
            return _mm_mul_ps(a,b);
        }
        static __forceinline void SetAdd  ( char * a, __m128 b, Context & ) {
            TT * pa = (TT *) a;
            *pa = cast<TT>::to ( _mm_add_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetSub  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_sub_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetDiv  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_div_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMul  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_mul_ps(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMod  ( char * a, __m128 b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_mod_ps(cast<TT>::from(*pa), b));
        }
        // vector-scalar
        static __forceinline __m128 DivVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_div_ps(a,_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)));
        }
        static __forceinline __m128 MulVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_mul_ps(a,_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)));
        }
        static __forceinline __m128 DivScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_div_ps(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0)),b);
        }
        static __forceinline __m128 MulScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_mul_ps(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0)),b);
        }
        static __forceinline void SetDivScal  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_div_ps(cast<TT>::from(*pa), _mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))));
        }
        static __forceinline void SetMulScal  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to ( _mm_mul_ps(cast<TT>::from(*pa), _mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))));
        }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_iVec {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128i _mm_mul_epi32 ( __m128i a, __m128i b ) {
            __m128i c;
			uint32_t * A = (uint32_t *)&a;
			uint32_t * B = (uint32_t *)&b;
			uint32_t * C = (uint32_t *)&c;
            C[0] = A[0]*B[0];   C[1] = A[1]*B[1];   C[2] = A[2]*B[2];   C[3] = A[3]*B[3];
            return c;
        }
        static __forceinline __m128i _mm_div_epi32 ( __m128i a, __m128i b ) {
			__m128i c;
			uint32_t * A = (uint32_t *)&a;
			uint32_t * B = (uint32_t *)&b;
			uint32_t * C = (uint32_t *)&c;
            C[0] = A[0]/B[0];   C[1] = A[1]/B[1];   C[2] = A[2]/B[2];   C[3] = A[3]/B[3];
            return c;
        }
        static __forceinline __m128i _mm_mod_epi32 ( __m128i a, __m128i b ) {
			__m128i c;
			uint32_t * A = (uint32_t *)&a;
			uint32_t * B = (uint32_t *)&b;
			uint32_t * C = (uint32_t *)&c;
            C[0] = A[0]%B[0];   C[1] = A[1]%B[1];   C[2] = A[2]%B[2];   C[3] = A[3]%B[3];
            return c;
        }
        // basic
        static __forceinline bool Equ     ( __m128 a, __m128 b, Context & ) { // todo: verify
            return (_mm_movemask_ps(_mm_castsi128_ps(
                    _mm_cmpeq_epi32(_mm_castps_si128(a), _mm_castps_si128(b)))) & mask)==mask;
        }
        static __forceinline bool NotEqu  ( __m128 a, __m128 b, Context & ) { // todo: verify
            return (_mm_movemask_ps(_mm_castsi128_ps(
                    _mm_cmpeq_epi32(_mm_castps_si128(a), _mm_castps_si128(b)))) & mask)!=mask;
        }
        // numeric
        static __forceinline __m128 Unp ( __m128 x, Context & ) {
            return x;
        }
        static __forceinline __m128 Unm ( __m128 x, Context & ) {   // todo: optimize?
            return _mm_castsi128_ps(_mm_sub_epi32(_mm_setzero_si128(), _mm_castps_si128(x)));
        }
        static __forceinline __m128 Add ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_add_epi32(_mm_castps_si128(a),_mm_castps_si128(b)));
        }
        static __forceinline __m128 Sub ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_sub_epi32(_mm_castps_si128(a),_mm_castps_si128(b)));
        }
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epi32(_mm_castps_si128(a),_mm_castps_si128(b)));
        }
        static __forceinline __m128 Mod ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mod_epi32(_mm_castps_si128(a),_mm_castps_si128(b)));
        }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epi32(_mm_castps_si128(a),_mm_castps_si128(b)));
        }
        static __forceinline void SetAdd  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_add_epi32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        static __forceinline void SetSub  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_sub_epi32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        static __forceinline void SetDiv  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_div_epi32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        static __forceinline void SetMul  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_mul_epi32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        static __forceinline void SetMod  ( char * a, __m128 b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_mod_epi32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        // vector-scalar
        static __forceinline __m128 DivVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epi32(_mm_castps_si128(a),_mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)))));
        }
        static __forceinline __m128 MulVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epi32(_mm_castps_si128(a),_mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)))));
        }
        static __forceinline __m128 DivScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epi32(_mm_castps_si128(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0))),_mm_castps_si128(b)));
        }
        static __forceinline __m128 MulScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epi32(_mm_castps_si128(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0))),_mm_castps_si128(b)));
        }
        static __forceinline void SetDivScal ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_div_epi32(_mm_castps_si128(cast<TT>::from(*pa)),
                                                               _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))))));
        }
        static __forceinline void SetMulScal ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_mul_epi32(_mm_castps_si128(cast<TT>::from(*pa)),
                                                               _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))))));
        }
    };
    
    template <typename TT, int mask>
    struct SimPolicy_uVec : SimPolicy_iVec<TT,mask> {
        // this is missing in SSE2 (but exists in SSE4?)
        static __forceinline __m128i _mm_div_epu32 ( __m128i a, __m128i b) {
            __m128i c;  uint32_t * A = (uint32_t *)&a, * B = (uint32_t *)&b, * C = (uint32_t *)&c;
            C[0] = A[0]/B[0];
            C[1] = A[1]/B[1];
            C[2] = A[2]/B[2];
            C[3] = A[3]/B[3];
            return c;
        }
        // swapping some numeric operations
        static __forceinline __m128 Div ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epu32(_mm_castps_si128(a), _mm_castps_si128(b)));
        }
        static __forceinline __m128 Mul ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epu32(_mm_castps_si128(a), _mm_castps_si128(b)));
        }
        static __forceinline void SetDiv  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_div_epu32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        static __forceinline void SetMul  ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_mul_epu32(_mm_castps_si128(cast<TT>::from(*pa)), _mm_castps_si128(b))));
        }
        // vector-scalar
        static __forceinline __m128 DivVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epu32(_mm_castps_si128(a), _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)))));
        }
        static __forceinline __m128 MulVecScal ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epu32(_mm_castps_si128(a), _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0)))));
        }
        static __forceinline __m128 DivScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_div_epu32(_mm_castps_si128(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0))), _mm_castps_si128(b)));
        }
        static __forceinline __m128 MulScalVec ( __m128 a, __m128 b, Context & ) {
            return _mm_castsi128_ps(_mm_mul_epu32(_mm_castps_si128(_mm_shuffle_ps(a,a,_MM_SHUFFLE(0,0,0,0))), _mm_castps_si128(b)));
        }
        static __forceinline void SetDivScal ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_div_epu32(_mm_castps_si128(cast<TT>::from(*pa)),
                                                               _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))))));
        }
        static __forceinline void SetMulScal ( char * a, __m128 b, Context & ) {
			TT * pa = (TT *)a;
            *pa = cast<TT>::to (_mm_castsi128_ps(_mm_mul_epu32(_mm_castps_si128(cast<TT>::from(*pa)),
                                                               _mm_castps_si128(_mm_shuffle_ps(b,b,_MM_SHUFFLE(0,0,0,0))))));
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
