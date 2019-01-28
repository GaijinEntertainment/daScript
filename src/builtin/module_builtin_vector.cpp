#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

namespace das
{
    template <typename TT, int mask>
    struct SimPolicy_Vec {
        // setXYZW
        static __forceinline vec4f setXYZW ( float x, float y, float z, float w ) {
            return v_make_vec4f(x, y, z, w);
        }
        // basic
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return (v_signmask(v_cmp_eq(a,b)) & mask) == mask;
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return (v_signmask(v_cmp_eq(a, b)) & mask) != mask;
        }
        // numeric
        static __forceinline vec4f Unp ( vec4f x, Context & ) {
            return x;
        }
        static __forceinline vec4f Unm ( vec4f x, Context & ) {
            return v_neg(x);
        }
        static __forceinline vec4f Add ( vec4f a, vec4f b, Context & ) {
            return v_add(a,b);
        }
        static __forceinline vec4f Sub ( vec4f a, vec4f b, Context & ) {
            return v_sub(a,b);
        }
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return v_div(a,b);
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return v_mod(a,b);
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return v_mul(a,b);
        }
        static __forceinline void SetAdd  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *) a;
            *pa = cast<TT>::to ( v_add(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetSub  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_sub(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_div(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_mul(cast<TT>::from(*pa), b));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_mod(cast<TT>::from(*pa), b));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return v_div(a,v_splat_x(b));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return v_mul(a,v_splat_x(b));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return v_div(v_splat_x(a),b);
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return v_mul(v_splat_x(a),b);
        }
        static __forceinline void SetDivScal  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_div(cast<TT>::from(*pa), v_splat_x(b)));
        }
        static __forceinline void SetMulScal  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to ( v_mul(cast<TT>::from(*pa), v_splat_x(b)));
        }
    };

    template <typename TT, int mask>
    struct SimPolicy_iVec {
        // setXYZW
        static __forceinline vec4f setXYZW ( int32_t x, int32_t y, int32_t z, int32_t w ) {
            return v_cast_vec4f(v_make_vec4i(x, y, z, w));
        }
        // basic
        static __forceinline bool Equ     ( vec4f a, vec4f b, Context & ) {
            return (v_signmask(v_cast_vec4f(v_cmp_eqi(v_cast_vec4i(a),v_cast_vec4i(b)))) & mask) == mask;
        }
        static __forceinline bool NotEqu  ( vec4f a, vec4f b, Context & ) {
            return (v_signmask(v_cast_vec4f(v_cmp_eqi(v_cast_vec4i(a), v_cast_vec4i(b)))) & mask) != mask;
        }
        // numeric
        static __forceinline vec4f Unp ( vec4f x, Context & ) {
            return x;
        }
        static __forceinline vec4f Unm ( vec4f x, Context & ) {
            return v_cast_vec4f(v_negi(v_cast_vec4i(x)));
        }
        static __forceinline vec4f Add ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_addi(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline vec4f Sub ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_subi(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divi(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_modi(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_muli(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline void SetAdd  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_addi(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetSub  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_subi(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_divi(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_muli(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_modi(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divi(v_cast_vec4i(a),v_splat_xi(v_cast_vec4i(b))));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_muli(v_cast_vec4i(a),v_splat_xi(v_cast_vec4i(b))));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divi(v_splat_xi(v_cast_vec4i(a)),v_cast_vec4i(b)));
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_muli(v_splat_xi(v_cast_vec4i(a)),v_cast_vec4i(b)));
        }
        static __forceinline void SetDivScal ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_divi(v_cast_vec4i(cast<TT>::from(*pa)),
                                                            v_splat_xi(v_cast_vec4i(b)))));
        }
        static __forceinline void SetMulScal ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_muli(v_cast_vec4i(cast<TT>::from(*pa)),
                                                            v_splat_xi(v_cast_vec4i(b)))));
        }
    };

    template <typename TT, int mask>
    struct SimPolicy_uVec : SimPolicy_iVec<TT,mask> {
        // swapping some numeric operations
        static __forceinline vec4f Div ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divu(v_cast_vec4i(a), v_cast_vec4i(b)));
        }
        static __forceinline vec4f Mul ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_mulu(v_cast_vec4i(a), v_cast_vec4i(b)));
        }
        static __forceinline vec4f Mod ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_modu(v_cast_vec4i(a),v_cast_vec4i(b)));
        }
        static __forceinline void SetDiv  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_divu(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetMul  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_mulu(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        static __forceinline void SetMod  ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_modu(v_cast_vec4i(cast<TT>::from(*pa)), v_cast_vec4i(b))));
        }
        // vector-scalar
        static __forceinline vec4f DivVecScal ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divu(v_cast_vec4i(a),v_splat_xi(v_cast_vec4i(b))));
        }
        static __forceinline vec4f MulVecScal ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_mulu(v_cast_vec4i(a),v_splat_xi(v_cast_vec4i(b))));
        }
        static __forceinline vec4f DivScalVec ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_divu(v_splat_xi(v_cast_vec4i(a)), v_cast_vec4i(b)));
        }
        static __forceinline vec4f MulScalVec ( vec4f a, vec4f b, Context & ) {
            return v_cast_vec4f(v_mulu(v_splat_xi(v_cast_vec4i(a)), v_cast_vec4i(b)));
        }
        static __forceinline void SetDivScal ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_divu(v_cast_vec4i(cast<TT>::from(*pa)),
                                                            v_splat_xi(v_cast_vec4i(b)))));
        }
        static __forceinline void SetMulScal ( char * a, vec4f b, Context & ) {
            TT * pa = (TT *)a;
            *pa = cast<TT>::to (v_cast_vec4f(v_mulu(v_cast_vec4i(cast<TT>::from(*pa)),
                                                            v_splat_xi(v_cast_vec4i(b)))));
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

    // VECTOR C-TOR
    template <typename TT, typename Policy, int vecS>
    struct SimNode_VecCtor;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif

    template <typename TT, typename Policy>
    struct SimNode_VecCtor<TT,Policy,1> : SimNode_CallBase {
        SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual vec4f eval(Context & context) override {
            vec4f argValues[2];
            evalArgs(context, argValues);
            auto X = cast<TT>::to(argValues[0]);
            return Policy::setXYZW(X, X, X, X);
        }
    };

    template <typename TT, typename Policy>
    struct SimNode_VecCtor<TT,Policy,2> : SimNode_CallBase {
        SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual vec4f eval(Context & context) override {
            vec4f argValues[2];
            evalArgs(context, argValues);
            return Policy::setXYZW(cast<TT>::to(argValues[0]),
                                   cast<TT>::to(argValues[1]),
                                   0,
                                   0);
        }
    };

    template <typename TT, typename Policy>
    struct SimNode_VecCtor<TT,Policy,3> : SimNode_CallBase {
        SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual vec4f eval(Context & context) override {
            vec4f argValues[3];
            evalArgs(context, argValues);
            return Policy::setXYZW(cast<TT>::to(argValues[0]),
                                   cast<TT>::to(argValues[1]),
                                   cast<TT>::to(argValues[2]),
                                   0);
        }
    };

    template <typename TT, typename Policy>
    struct SimNode_VecCtor<TT,Policy,4> : SimNode_CallBase {
        SimNode_VecCtor(const LineInfo & at) : SimNode_CallBase(at) {}
        virtual vec4f eval(Context & context) override {
            vec4f argValues[4];
            evalArgs(context, argValues);
            return Policy::setXYZW(cast<TT>::to(argValues[0]),
                                   cast<TT>::to(argValues[1]),
                                   cast<TT>::to(argValues[2]),
                                   cast<TT>::to(argValues[3]));
        }
    };

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#define ADD_VEC_CTOR_1(VTYPE) \
addFunction ( make_shared<BuiltInFn<SimNode_Zero,VTYPE>> (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<float,   SimPolicy<VTYPE>,1>,VTYPE,float>>   (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int32_t, SimPolicy<VTYPE>,1>,VTYPE,int32_t>> (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint32_t,SimPolicy<VTYPE>,1>,VTYPE,uint32_t>>(#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int64_t, SimPolicy<VTYPE>,1>,VTYPE,int64_t>> (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint64_t,SimPolicy<VTYPE>,1>,VTYPE,uint64_t>>(#VTYPE,lib) );

#define ADD_VEC_CTOR_2(VTYPE) \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<float,   SimPolicy<VTYPE>,2>,VTYPE,float,float>>      (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int32_t, SimPolicy<VTYPE>,2>,VTYPE,int32_t,int32_t>>  (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint32_t,SimPolicy<VTYPE>,2>,VTYPE,uint32_t,uint32_t>>(#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int64_t, SimPolicy<VTYPE>,2>,VTYPE,int64_t,int64_t>>  (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint64_t,SimPolicy<VTYPE>,2>,VTYPE,uint64_t,uint64_t>>(#VTYPE,lib) );

#define ADD_VEC_CTOR_3(VTYPE) \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<float,   SimPolicy<VTYPE>,3>,VTYPE,float,float,float>>         (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int32_t, SimPolicy<VTYPE>,3>,VTYPE,int32_t,int32_t,int32_t>>   (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint32_t,SimPolicy<VTYPE>,3>,VTYPE,uint32_t,uint32_t,uint32_t>>(#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int64_t, SimPolicy<VTYPE>,3>,VTYPE,int64_t,int64_t,int64_t>>   (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint64_t,SimPolicy<VTYPE>,3>,VTYPE,uint64_t,uint64_t,uint64_t>>(#VTYPE,lib) );

#define ADD_VEC_CTOR_4(VTYPE) \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<float,   SimPolicy<VTYPE>,4>,VTYPE,float,float,float,float>>            (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int32_t, SimPolicy<VTYPE>,4>,VTYPE,int32_t,int32_t,int32_t,int32_t>>    (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint32_t,SimPolicy<VTYPE>,4>,VTYPE,uint32_t,uint32_t,uint32_t,uint32_t>>(#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<int64_t, SimPolicy<VTYPE>,4>,VTYPE,int64_t,int64_t,int64_t,int64_t>>    (#VTYPE,lib) ); \
addFunction ( make_shared<BuiltInFn<SimNode_VecCtor<uint64_t,SimPolicy<VTYPE>,4>,VTYPE,uint64_t,uint64_t,uint64_t,uint64_t>>(#VTYPE,lib) );

    void Module_BuiltIn::addVectorTypes(ModuleLibrary & lib) {
        // float2
        addFunctionBasic<float2>(*this,lib);
        addFunctionNumeric<float2>(*this,lib);
        addFunctionVecNumeric<float2,float>(*this,lib);
        ADD_VEC_CTOR_1(float2);
        ADD_VEC_CTOR_2(float2);
        // float3
        addFunctionBasic<float3>(*this,lib);
        addFunctionNumeric<float3>(*this,lib);
        addFunctionVecNumeric<float3, float>(*this,lib);
        ADD_VEC_CTOR_1(float3);
        ADD_VEC_CTOR_3(float3);
        // float4
        addFunctionBasic<float4>(*this,lib);
        addFunctionNumeric<float4>(*this,lib);
        addFunctionVecNumeric<float4, float>(*this,lib);
        ADD_VEC_CTOR_1(float4);
        ADD_VEC_CTOR_4(float4);
        // int2
        addFunctionBasic<int2>(*this,lib);
        addFunctionNumeric<int2>(*this,lib);
        addFunctionVecNumeric<int2, int32_t>(*this,lib);
        ADD_VEC_CTOR_1(int2);
        ADD_VEC_CTOR_2(int2);
        // int3
        addFunctionBasic<int3>(*this,lib);
        addFunctionNumeric<int3>(*this,lib);
        addFunctionVecNumeric<int3, int32_t>(*this,lib);
        ADD_VEC_CTOR_1(int3);
        ADD_VEC_CTOR_3(int3);
        // int4
        addFunctionBasic<int4>(*this,lib);
        addFunctionNumeric<int4>(*this,lib);
        addFunctionVecNumeric<int4,int32_t>(*this,lib);
        ADD_VEC_CTOR_1(int4);
        ADD_VEC_CTOR_4(int4);
        // uint2
        addFunctionBasic<uint2>(*this,lib);
        addFunctionNumeric<uint2>(*this,lib);
        addFunctionVecNumeric<uint2, uint32_t>(*this,lib);
        ADD_VEC_CTOR_1(uint2);
        ADD_VEC_CTOR_2(uint2);
        // uint3
        addFunctionBasic<uint3>(*this,lib);
        addFunctionNumeric<uint3>(*this,lib);
        addFunctionVecNumeric<uint3,uint32_t>(*this,lib);
        ADD_VEC_CTOR_1(uint3);
        ADD_VEC_CTOR_3(uint3);
        // uint4
        addFunctionBasic<uint4>(*this,lib);
        addFunctionNumeric<uint4>(*this,lib);
        addFunctionVecNumeric<uint4, uint32_t>(*this,lib);
        ADD_VEC_CTOR_1(uint4);
        ADD_VEC_CTOR_4(uint4);
        // range
        addFunctionBasic<range>(*this,lib);
        ADD_VEC_CTOR_1(range);
        ADD_VEC_CTOR_2(range);
        // urange
        addFunctionBasic<urange>(*this,lib);
        ADD_VEC_CTOR_1(urange);
        ADD_VEC_CTOR_2(urange);
    }
}
