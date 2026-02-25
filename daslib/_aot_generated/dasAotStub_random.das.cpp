#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require random

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100)   // unreferenced formal parameter
#pragma warning(disable:4189)   // local variable is initialized but not referenced
#pragma warning(disable:4244)   // conversion from 'int32_t' to 'float', possible loss of data
#pragma warning(disable:4114)   // same qualifier more than once
#pragma warning(disable:4623)   // default constructor was implicitly defined as deleted
#pragma warning(disable:4946)   // reinterpret_cast used between related classes
#pragma warning(disable:4269)   // 'const' automatic data initialized with compiler generated default constructor produces unreliable results
#pragma warning(disable:4555)   // result of expression not used
#endif
#if defined(__EDG__)
#pragma diag_suppress 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsubobject-linkage"
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wwritable-strings"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#pragma clang diagnostic ignored "-Wunsequenced"
#pragma clang diagnostic ignored "-Wunused-function"
#endif

namespace das {
namespace _anon_5070260718142466129 {

namespace random { struct _lambda_random_119_1; };
namespace random {

struct _lambda_random_119_1 {
    Func DAS_COMMENT((bool,random::_lambda_random_119_1,uint32_t &)) __lambda;
    Func DAS_COMMENT((void,random::_lambda_random_119_1 *)) __finalize;
    int32_t __yield;
    int32_t rnd_seed;
    int4 seed;
};
static_assert(sizeof(_lambda_random_119_1)==40,"structure size mismatch with DAS");
static_assert(offsetof(_lambda_random_119_1,__lambda)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_random_119_1,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_random_119_1,__yield)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_random_119_1,rnd_seed)==20,"structure field offset mismatch with DAS");
static_assert(offsetof(_lambda_random_119_1,seed)==24,"structure field offset mismatch with DAS");
}
extern StructInfo __struct_info__b0623cd3bb976353;
extern TypeInfo __type_info__af969b4c86582719;
extern TypeInfo __type_info__a853d89220ab5a70;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__c764d42a8c821d2;
extern TypeInfo __type_info__af63eb4c86020609;

TypeInfo * __type_info__a2ef3b1d9bff55dd_arg_types_var_12709787978475987795[2] = { &__type_info__c764d42a8c821d2, &__type_info__af969b4c86582719 };
const char * __type_info__a2ef3b1d9bff55dd_arg_names_var_12709787978475987795[2] = { "__this", "_yield_119" };
VarInfo __struct_info__b0623cd3bb976353_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__a2ef3b1d9bff55dd_arg_types_var_12709787978475987795, __type_info__a2ef3b1d9bff55dd_arg_names_var_12709787978475987795, 2, 0, nullptr, 12, 8, UINT64_C(0xa2ef3b1d9bff55dd), "__lambda", 0, 0 };
TypeInfo * __type_info__ebbe7b3aee07df51_arg_types_var_12709787978475987795[1] = { &__type_info__a853d89220ab5a70 };
const char * __type_info__ebbe7b3aee07df51_arg_names_var_12709787978475987795[1] = { "__this" };
VarInfo __struct_info__b0623cd3bb976353_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ebbe7b3aee07df51_arg_types_var_12709787978475987795, __type_info__ebbe7b3aee07df51_arg_names_var_12709787978475987795, 1, 0, nullptr, 12, 8, UINT64_C(0xebbe7b3aee07df51), "__finalize", 8, 0 };
VarInfo __struct_info__b0623cd3bb976353_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xd4a42e8dd5aab504), "__yield", 16, 0 };
VarInfo __struct_info__b0623cd3bb976353_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x994b5555086cbe1a), "rnd_seed", 20, 0 };
VarInfo __struct_info__b0623cd3bb976353_field_4 =  { Type::tInt4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 16, UINT64_C(0x6023c5555e5b391d), "seed", 24, 0 };
VarInfo * __struct_info__b0623cd3bb976353_fields[5] =  { &__struct_info__b0623cd3bb976353_field_0, &__struct_info__b0623cd3bb976353_field_1, &__struct_info__b0623cd3bb976353_field_2, &__struct_info__b0623cd3bb976353_field_3, &__struct_info__b0623cd3bb976353_field_4 };
StructInfo __struct_info__b0623cd3bb976353 = {"_lambda_random_119_1", "random", 14, __struct_info__b0623cd3bb976353_fields, 5, 40, UINT64_C(0x0), nullptr, UINT64_C(0xb0623cd3bb976353), 5 };
TypeInfo __type_info__af969b4c86582719 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 4, UINT64_C(0xaf969b4c86582719) };
TypeInfo __type_info__a853d89220ab5a70 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__c764d42a8c821d2, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xa853d89220ab5a70) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__c764d42a8c821d2 = { Type::tStructure, &__struct_info__b0623cd3bb976353, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 40, UINT64_C(0xc764d42a8c821d2) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline uint32_t _FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5 ( Context * __context__, int4 & __seed_rename_at_52_0 );
inline void finalize_9bf2da16dbbd227a ( Context * __context__, random::_lambda_random_119_1 & ____this_rename_at_119_2 );
inline Sequence DAS_COMMENT((uint32_t)) _FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9 ( Context * __context__, Lambda DAS_COMMENT((bool,uint32_t &)) const  __lam_rename_at_1464_3 );
inline int4 _FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99 ( Context * __context__, int32_t __seed_rename_at_23_5 );
inline bool _Func_lambda_random_119_1Tickfunction_da01c0afd021b134 ( Context * __context__, random::_lambda_random_119_1 & ____this_rename_at_119_6, uint32_t & ___yield_119_rename_at_119_7 );
inline void _Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a ( Context * __context__, random::_lambda_random_119_1 * ____this_rename_at_119_8 );
inline Sequence DAS_COMMENT((uint32_t)) each_random_uint_15e7334dd0fb1ddc ( Context * __context__, int32_t __rnd_seed_rename_at_117_9 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xa100e9a22960fc22>(__context__) = 32767;/*LCG_RAND_MAX*/
    das_global<int32_t,0x6d7ed615eaf34962>(__context__) = 1073741823;/*LCG_RAND_MAX_BIG*/
    das_global<float,0xe18471da7dd7b153>(__context__) = 3.051851e-05f;/*LCG_IRAND_MAX_FLT*/
}

inline uint32_t _FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5 ( Context * __context__, int4 & __seed_rename_at_52_0 )
{
    das_copy(__seed_rename_at_52_0,SimPolicy<int4>::Add((SimPolicy<int4>::Mul(int4(214013,214013,214013,214013),__seed_rename_at_52_0,*__context__,nullptr)),int4(2531011,2531011,2531011,2531011),*__context__,nullptr));
    int3 __i123_rename_at_55_1 = ((int3)(SimPolicy<int3>::BinAnd((SimPolicy<int3>::BinShr(das_swizzle_seq<int3,int4,0>::swizzle(__seed_rename_at_52_0) /*xyz*/,cast<int32_t>::from(16),*__context__,nullptr)),int3(32767,32767,32767),*__context__,nullptr)));
    return das_auto_cast<uint32_t>::cast((uint32_t(v_extract_xi(v_cast_vec4i(__i123_rename_at_55_1)) /*x*/) ^ (SimPolicy<uint32_t>::BinShl(uint32_t(v_extract_yi(v_cast_vec4i(__i123_rename_at_55_1)) /*y*/),0xfu,*__context__,nullptr))) ^ (SimPolicy<uint32_t>::BinShl(uint32_t(v_extract_zi(v_cast_vec4i(__i123_rename_at_55_1)) /*z*/),0x1eu,*__context__,nullptr)));
}

inline void finalize_9bf2da16dbbd227a ( Context * __context__, random::_lambda_random_119_1 &  ____this_rename_at_119_2 )
{
    memset((void*)&(____this_rename_at_119_2), 0, 40);
}

inline Sequence DAS_COMMENT((uint32_t)) _FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9 ( Context * __context__, Lambda DAS_COMMENT((bool,uint32_t &)) const  __lam_rename_at_1464_3 )
{
    Sequence DAS_COMMENT((uint32_t)) __it_rename_at_1466_4;das_zero(__it_rename_at_1466_4);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1466_4),__lam_rename_at_1464_3,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((uint32_t))>::cast(__it_rename_at_1466_4);
}

inline int4 _FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99 ( Context * __context__, int32_t __seed_rename_at_23_5 )
{
    return das_auto_cast<int4>::cast(int4(__seed_rename_at_23_5,__seed_rename_at_23_5 + 1,__seed_rename_at_23_5 + 2,__seed_rename_at_23_5 + 3));
}

inline bool _Func_lambda_random_119_1Tickfunction_da01c0afd021b134 ( Context * __context__, random::_lambda_random_119_1 &  ____this_rename_at_119_6, uint32_t & ___yield_119_rename_at_119_7 )
{
    switch (____this_rename_at_119_6.__yield) {
    case 0: goto label_0;
    case 1: goto label_1;
    case 3: goto label_3;
    case 2: goto label_2;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_119_6.seed,_FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99(__context__,____this_rename_at_119_6.rnd_seed));
    label_1:;;
    das_copy(___yield_119_rename_at_119_7,_FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5(__context__,____this_rename_at_119_6.seed));
    das_copy(____this_rename_at_119_6.__yield,3);
    return das_auto_cast<bool>::cast(true);
    label_3:;;
    goto label_1;
    label_2:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a ( Context * __context__, random::_lambda_random_119_1 * ____this_rename_at_119_8 )
{
    finalize_9bf2da16dbbd227a(__context__,das_arg<random::_lambda_random_119_1>::pass(das_deref(__context__,____this_rename_at_119_8,__FILE__,__LINE__)));
    das_delete_lambda_struct<random::_lambda_random_119_1 *>::clear(__context__,____this_rename_at_119_8);
}

inline Sequence DAS_COMMENT((uint32_t)) each_random_uint_15e7334dd0fb1ddc ( Context * __context__, int32_t __rnd_seed_rename_at_117_9 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((uint32_t))>::cast(_FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9(__context__,das_ascend<bool,random::_lambda_random_119_1>::make(__context__,&__type_info__c764d42a8c821d2,(([&]() -> random::_lambda_random_119_1 {
        random::_lambda_random_119_1 __mks_119;
        das_zero(__mks_119);
        das_copy((__mks_119.__lambda),(Func(__context__->fnByMangledName(/*@random::_lambda_random_119_1`function XS<random::_lambda_random_119_1> &u*/ 0xc0ae416d7d29d310))));
        das_copy((__mks_119.__finalize),(Func(__context__->fnByMangledName(/*@random::_lambda_random_119_1`finalizer X1<S<random::_lambda_random_119_1>>?*/ 0x64239f8adde42af7))));
        das_copy((__mks_119.rnd_seed),(__rnd_seed_rename_at_117_9));
        return __mks_119;
    })()))));
}
static vec4f __wrap__FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5 ( Context * __context__ ) {
    int4 & arg_seed = cast_aot_arg<int4 &>::to(*__context__,__context__->abiArguments()[0]);
    return cast<uint32_t>::from(_FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5(__context__, arg_seed));
}
static vec4f __wrap_finalize_9bf2da16dbbd227a ( Context * __context__ ) {
    random::_lambda_random_119_1 &  arg___this = cast_aot_arg<random::_lambda_random_119_1 & >::to(*__context__,__context__->abiArguments()[0]);
    finalize_9bf2da16dbbd227a(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap__FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9 ( Context * __context__ ) {
    Lambda DAS_COMMENT((bool,uint32_t &)) const  arg_lam = cast_aot_arg<Lambda DAS_COMMENT((bool,uint32_t &)) const >::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((uint32_t)) *) __context__->abiCMRES) = _FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9(__context__, arg_lam);
    return v_zero();
}
static vec4f __wrap__FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99 ( Context * __context__ ) {
    int32_t arg_seed = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    return cast<int4>::from(_FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99(__context__, arg_seed));
}
static vec4f __wrap__Func_lambda_random_119_1Tickfunction_da01c0afd021b134 ( Context * __context__ ) {
    random::_lambda_random_119_1 &  arg___this = cast_aot_arg<random::_lambda_random_119_1 & >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t & arg__yield_119 = cast_aot_arg<uint32_t &>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_Func_lambda_random_119_1Tickfunction_da01c0afd021b134(__context__, arg___this, arg__yield_119));
}
static vec4f __wrap__Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a ( Context * __context__ ) {
    random::_lambda_random_119_1 * arg___this = cast_aot_arg<random::_lambda_random_119_1 *>::to(*__context__,__context__->abiArguments()[0]);
    _Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a(__context__, arg___this);
    return v_zero();
}
static vec4f __wrap_each_random_uint_15e7334dd0fb1ddc ( Context * __context__ ) {
    int32_t arg_rnd_seed = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[0]);
    *((Sequence DAS_COMMENT((uint32_t)) *) __context__->abiCMRES) = each_random_uint_15e7334dd0fb1ddc(__context__, arg_rnd_seed);
    return v_zero();
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x76cda64f6dddfd9e, false, (void*)&_FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5, &__wrap__FuncrandomTickrandom_uintTick15141358123050100739_eed954c1588766f5 },
    { 0x68fdbb4834ea8148, false, (void*)&finalize_9bf2da16dbbd227a, &__wrap_finalize_9bf2da16dbbd227a },
    { 0x4b457e5c37621b5b, true, (void*)&_FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9, &__wrap__FuncbuiltinTickeachTick9663565701927713696_801edf485cd85df9 },
    { 0xfd37050dd7eb860a, false, (void*)&_FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99, &__wrap__FuncrandomTickrandom_seedTick15287272150245073176_6991510ddb1b5a99 },
    { 0x3c83128bb182e237, false, (void*)&_Func_lambda_random_119_1Tickfunction_da01c0afd021b134, &__wrap__Func_lambda_random_119_1Tickfunction_da01c0afd021b134 },
    { 0x3ab1d024618bb8ae, false, (void*)&_Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a, &__wrap__Func_lambda_random_119_1Tickfinalizer_7ae60c6323acf89a },
    { 0x87ed59a3ab002f74, true, (void*)&each_random_uint_15e7334dd0fb1ddc, &__wrap_each_random_uint_15e7334dd0fb1ddc },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_5070260718142466129
AotListBase impl_aot_random(_anon_5070260718142466129::registerAotFunctions);
} // namespace das

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__EDG__)
#pragma diag_default 826
#elif defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
