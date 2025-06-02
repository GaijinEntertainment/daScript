#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
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
namespace _anon_14906104661355171097 {

namespace random { struct _lambda_random_111_1; };
namespace random {

struct _lambda_random_111_1 {
    Func DAS_COMMENT((bool,random::_lambda_random_111_1,uint32_t &)) __lambda;
    Func DAS_COMMENT((void,random::_lambda_random_111_1 *)) __finalize;
    int32_t __yield;
    int32_t rnd_seed;
    int4 seed;
};
}
extern StructInfo __struct_info__cbf23cd3d353cb53;
extern TypeInfo __type_info__af969b4c86582719;
extern TypeInfo __type_info__f1741aa759523270;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__9a1e4d1c219d09d2;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__e568005b4bb4b751;
extern VarInfo __var_info__476e1901ae5feddd;
extern VarInfo __var_info__a88be1f0aee7ad04;
extern VarInfo __var_info__f1dfb90660a0c61a;
extern VarInfo __var_info__158534154e75611d;

TypeInfo * __type_info__476e1901ae5feddd_arg_types_var_14695875414544599891[2] = { &__type_info__9a1e4d1c219d09d2, &__type_info__af969b4c86582719 };
const char * __type_info__476e1901ae5feddd_arg_names_var_14695875414544599891[2] = { "__this", "_yield_111" };
VarInfo __struct_info__cbf23cd3d353cb53_field_0 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af81fe4c86352052, nullptr, (TypeInfo **)__type_info__476e1901ae5feddd_arg_types_var_14695875414544599891, __type_info__476e1901ae5feddd_arg_names_var_14695875414544599891, 2, 0, nullptr, 12, 8, UINT64_C(0x476e1901ae5feddd), "__lambda", offsetof(random::_lambda_random_111_1,__lambda), 0 };
TypeInfo * __type_info__e568005b4bb4b751_arg_types_var_14695875414544599891[1] = { &__type_info__f1741aa759523270 };
const char * __type_info__e568005b4bb4b751_arg_names_var_14695875414544599891[1] = { "__this" };
VarInfo __struct_info__cbf23cd3d353cb53_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e568005b4bb4b751_arg_types_var_14695875414544599891, __type_info__e568005b4bb4b751_arg_names_var_14695875414544599891, 1, 0, nullptr, 12, 8, UINT64_C(0xe568005b4bb4b751), "__finalize", offsetof(random::_lambda_random_111_1,__finalize), 0 };
VarInfo __struct_info__cbf23cd3d353cb53_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xa88be1f0aee7ad04), "__yield", offsetof(random::_lambda_random_111_1,__yield), 0 };
VarInfo __struct_info__cbf23cd3d353cb53_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xf1dfb90660a0c61a), "rnd_seed", offsetof(random::_lambda_random_111_1,rnd_seed), 0 };
VarInfo __struct_info__cbf23cd3d353cb53_field_4 =  { Type::tInt4, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 16, UINT64_C(0x158534154e75611d), "seed", offsetof(random::_lambda_random_111_1,seed), 0 };
VarInfo * __struct_info__cbf23cd3d353cb53_fields[5] =  { &__struct_info__cbf23cd3d353cb53_field_0, &__struct_info__cbf23cd3d353cb53_field_1, &__struct_info__cbf23cd3d353cb53_field_2, &__struct_info__cbf23cd3d353cb53_field_3, &__struct_info__cbf23cd3d353cb53_field_4 };
StructInfo __struct_info__cbf23cd3d353cb53 = {"_lambda_random_111_1", "random", 14, __struct_info__cbf23cd3d353cb53_fields, 5, 40, UINT64_C(0x0), nullptr, UINT64_C(0xcbf23cd3d353cb53), 5 };
TypeInfo __type_info__af969b4c86582719 = { Type::tUInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 285, 4, UINT64_C(0xaf969b4c86582719) };
TypeInfo __type_info__f1741aa759523270 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__9a1e4d1c219d09d2, nullptr, nullptr, nullptr, 0, 0, nullptr, 24588, 8, UINT64_C(0xf1741aa759523270) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__9a1e4d1c219d09d2 = { Type::tStructure, &__struct_info__cbf23cd3d353cb53, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 40, UINT64_C(0x9a1e4d1c219d09d2) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline uint32_t _FuncrandomTickrandom_uintTick15141358123050100739_ace3444e720a3102 ( Context * __context__, int4 & __seed_rename_at_45_0 );
inline void finalize_4ae6526745d6b78f ( Context * __context__, random::_lambda_random_111_1 & ____this_rename_at_111_2 );
inline Sequence DAS_COMMENT((uint32_t)) _FuncbuiltinTickeachTick9663565701927713696_aee705fe23b1c7b9 ( Context * __context__, Lambda DAS_COMMENT((bool,uint32_t &)) const  __lam_rename_at_1289_3 );
inline int4 _FuncrandomTickrandom_seedTick15287272150245073176_dd30826a0cea51ed ( Context * __context__, int32_t __seed_rename_at_16_5 );
inline bool _Func_lambda_random_111_1Tickfunction_f401add82667d26 ( Context * __context__, random::_lambda_random_111_1 & ____this_rename_at_111_6, uint32_t & ___yield_111_rename_at_111_7 );
inline void _Func_lambda_random_111_1Tickfinalizer_a949ea6b2586918a ( Context * __context__, random::_lambda_random_111_1 * ____this_rename_at_111_8 );
inline Sequence DAS_COMMENT((uint32_t)) each_random_uint_afea9e974e8227bf ( Context * __context__, int32_t __rnd_seed_rename_at_110_9 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<int32_t,0xa100e9a22960fc22>(__context__) = 32767;/*LCG_RAND_MAX*/
    das_global<int32_t,0x6d7ed615eaf34962>(__context__) = 1073741823;/*LCG_RAND_MAX_BIG*/
    das_global<float,0xe18471da7dd7b153>(__context__) = 3.051851e-05f;/*LCG_IRAND_MAX_FLT*/
}

inline uint32_t _FuncrandomTickrandom_uintTick15141358123050100739_ace3444e720a3102 ( Context * __context__, int4 & __seed_rename_at_45_0 )
{
    das_copy(__seed_rename_at_45_0,SimPolicy<int4>::Add((SimPolicy<int4>::Mul(int4(214013,214013,214013,214013),__seed_rename_at_45_0,*__context__,nullptr)),int4(2531011,2531011,2531011,2531011),*__context__,nullptr));
    int3 __i123_rename_at_48_1 = ((int3)(SimPolicy<int3>::BinAnd((SimPolicy<int3>::BinShr(das_swizzle_seq<int3,int4,0>::swizzle(__seed_rename_at_45_0) /*xyz*/,cast<int32_t>::from(16),*__context__,nullptr)),int3(32767,32767,32767),*__context__,nullptr)));
    return das_auto_cast<uint32_t>::cast((uint32_t(v_extract_xi(v_cast_vec4i(__i123_rename_at_48_1)) /*x*/) ^ (uint32_t(v_extract_yi(v_cast_vec4i(__i123_rename_at_48_1)) /*y*/) << 0xfu)) ^ (uint32_t(v_extract_zi(v_cast_vec4i(__i123_rename_at_48_1)) /*z*/) << 0x1eu));
}

inline void finalize_4ae6526745d6b78f ( Context * __context__, random::_lambda_random_111_1 &  ____this_rename_at_111_2 )
{
    memset((void*)&(____this_rename_at_111_2), 0, TypeSize<random::_lambda_random_111_1>::size);
}

inline Sequence DAS_COMMENT((uint32_t)) _FuncbuiltinTickeachTick9663565701927713696_aee705fe23b1c7b9 ( Context * __context__, Lambda DAS_COMMENT((bool,uint32_t &)) const  __lam_rename_at_1289_3 )
{
    Sequence DAS_COMMENT((uint32_t)) __it_rename_at_1291_4;das_zero(__it_rename_at_1291_4);
    builtin_make_lambda_iterator(das_arg<Sequence DAS_COMMENT((uint32_t))>::pass(__it_rename_at_1291_4),__lam_rename_at_1289_3,4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((uint32_t))>::cast(__it_rename_at_1291_4);
}

inline int4 _FuncrandomTickrandom_seedTick15287272150245073176_dd30826a0cea51ed ( Context * __context__, int32_t __seed_rename_at_16_5 )
{
    return das_auto_cast<int4>::cast(int4(__seed_rename_at_16_5,__seed_rename_at_16_5 + 1,__seed_rename_at_16_5 + 2,__seed_rename_at_16_5 + 3));
}

inline bool _Func_lambda_random_111_1Tickfunction_f401add82667d26 ( Context * __context__, random::_lambda_random_111_1 &  ____this_rename_at_111_6, uint32_t & ___yield_111_rename_at_111_7 )
{
    switch (____this_rename_at_111_6.__yield) {
    case 0: goto label_0;
    case 1: goto label_1;
    case 3: goto label_3;
    case 2: goto label_2;
    default: __context__->throw_error("invalid label");
    };
    label_0:;;
    das_copy(____this_rename_at_111_6.seed,_FuncrandomTickrandom_seedTick15287272150245073176_dd30826a0cea51ed(__context__,____this_rename_at_111_6.rnd_seed));
    label_1:;;
    das_copy(___yield_111_rename_at_111_7,_FuncrandomTickrandom_uintTick15141358123050100739_ace3444e720a3102(__context__,____this_rename_at_111_6.seed));
    das_copy(____this_rename_at_111_6.__yield,3);
    return das_auto_cast<bool>::cast(true);
    label_3:;;
    goto label_1;
    label_2:;;
    return das_auto_cast<bool>::cast(false);
}

inline void _Func_lambda_random_111_1Tickfinalizer_a949ea6b2586918a ( Context * __context__, random::_lambda_random_111_1 * ____this_rename_at_111_8 )
{
    finalize_4ae6526745d6b78f(__context__,das_arg<random::_lambda_random_111_1>::pass(das_deref(__context__,____this_rename_at_111_8)));
    das_delete_lambda_struct<random::_lambda_random_111_1 *>::clear(__context__,____this_rename_at_111_8);
}

inline Sequence DAS_COMMENT((uint32_t)) each_random_uint_afea9e974e8227bf ( Context * __context__, int32_t __rnd_seed_rename_at_110_9 )
{
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((uint32_t))>::cast(_FuncbuiltinTickeachTick9663565701927713696_aee705fe23b1c7b9(__context__,das_ascend<bool,random::_lambda_random_111_1>::make(__context__,&__type_info__9a1e4d1c219d09d2,(([&]() -> random::_lambda_random_111_1 {
        random::_lambda_random_111_1 __mks_111;
        das_zero(__mks_111);
        das_copy((__mks_111.__lambda),(Func(__context__->fnByMangledName(/*@random::_lambda_random_111_1`function XS<random::_lambda_random_111_1> &u*/ 0xb554085ecf7a9fc8))));
        das_copy((__mks_111.__finalize),(Func(__context__->fnByMangledName(/*@random::_lambda_random_111_1`finalizer X1<S<random::_lambda_random_111_1>>?*/ 0x8b700a944df39047))));
        das_copy((__mks_111.rnd_seed),(__rnd_seed_rename_at_110_9));
        return __mks_111;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x390bb07ee4f51e50] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrandomTickrandom_uintTick15141358123050100739_ace3444e720a3102>>();
    };
    aotLib[0x78e003330fac06f1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_4ae6526745d6b78f>>();
    };
    aotLib[0xbdd618fbc7ac3e7d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickeachTick9663565701927713696_aee705fe23b1c7b9>>();
    };
    aotLib[0x6fb77c0f7353e48d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrandomTickrandom_seedTick15287272150245073176_dd30826a0cea51ed>>();
    };
    aotLib[0xabd94a563e16633e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_random_111_1Tickfunction_f401add82667d26>>();
    };
    aotLib[0xc930f58f7e2fe2df] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Func_lambda_random_111_1Tickfinalizer_a949ea6b2586918a>>();
    };
    aotLib[0x60d5f8aa8ce7975b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&each_random_uint_afea9e974e8227bf>>();
    };
    // [[ init script ]]
    aotLib[0xccb3379a77146546] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_14906104661355171097
AotListBase impl_aot_random(_anon_14906104661355171097::registerAotFunctions);
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
