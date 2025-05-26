#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require uriparser
#include "daScript/simulate/aot_builtin_uriparser.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require strings_boost
 // require ast_boost
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require sort_boost
 // require uriparser_boost

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
namespace _anon_14054407420064321909 {

namespace ast { struct AstFunctionAnnotation; };
namespace ast { struct AstBlockAnnotation; };
namespace ast { struct AstStructureAnnotation; };
namespace ast { struct AstPassMacro; };
namespace ast { struct AstVariantMacro; };
namespace ast { struct AstForLoopMacro; };
namespace ast { struct AstCaptureMacro; };
namespace ast { struct AstTypeMacro; };
namespace ast { struct AstSimulateMacro; };
namespace ast { struct AstReaderMacro; };
namespace ast { struct AstCommentReader; };
namespace ast { struct AstCallMacro; };
namespace ast { struct AstTypeInfoMacro; };
namespace ast { struct AstEnumerationAnnotation; };
namespace ast { struct AstVisitor; };
namespace ast_boost { struct MacroMacro; };
namespace ast_boost { struct TagFunctionAnnotation; };
namespace ast_boost { struct TagStructureAnnotation; };
namespace ast_boost { struct SetupAnyAnnotation; };
namespace ast_boost { struct SetupFunctionAnnotation; };
namespace ast_boost { struct SetupBlockAnnotation; };
namespace ast_boost { struct SetupStructureAnnotation; };
namespace ast_boost { struct SetupEnumerationAnnotation; };
namespace ast_boost { struct SetupContractAnnotation; };
namespace ast_boost { struct SetupReaderMacro; };
namespace ast_boost { struct SetupCommentReader; };
namespace ast_boost { struct SetupVariantMacro; };
namespace ast_boost { struct SetupForLoopMacro; };
namespace ast_boost { struct SetupCaptureMacro; };
namespace ast_boost { struct SetupTypeMacro; };
namespace ast_boost { struct SetupSimulateMacro; };
namespace ast_boost { struct SetupCallMacro; };
namespace ast_boost { struct SetupTypeInfoMacro; };
namespace ast_boost { struct SetupInferMacro; };
namespace ast_boost { struct SetupDirtyInferMacro; };
namespace ast_boost { struct SetupLintMacro; };
namespace ast_boost { struct SetupGlobalLintMacro; };
namespace ast_boost { struct SetupOptimizationMacro; };
namespace ast_boost { struct TagFunctionMacro; };
namespace ast_boost { struct BetterRttiVisitor; };
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
namespace contracts { struct IsAnyType; };
namespace contracts { struct IsAnyArrayMacro; };
namespace contracts { struct IsAnyEnumMacro; };
namespace contracts { struct IsAnyBitfieldMacro; };
namespace contracts { struct IsAnyVectorType; };
namespace contracts { struct IsAnyStructMacro; };
namespace contracts { struct IsAnyNumericMacro; };
namespace contracts { struct IsAnyWorkhorse; };
namespace contracts { struct IsAnyWorkhorseNonPtrMacro; };
namespace contracts { struct IsAnyTupleNonPtrMacro; };
namespace contracts { struct IsAnyVariantNonPtrMacro; };
namespace contracts { struct IsAnyFunctionNonPtrMacro; };
namespace contracts { struct IsAnyLambdaMacro; };
namespace contracts { struct IsRefMacro; };
namespace contracts { struct IsPointer; };
namespace contracts { struct IsClass; };
namespace contracts { struct IsValueHandle; };
namespace templates_boost { struct Template; };
namespace templates_boost { struct TemplateVisitor; };
namespace templates_boost { struct RemoveDerefVisitor; };
namespace templates_boost { struct QRulesVisitor; };
namespace templates_boost { struct AstQCallMacro; };
namespace templates_boost { struct QMacro; };
namespace templates_boost { struct QBlockMacro; };
namespace templates_boost { struct QBlockToArrayMacro; };
namespace templates_boost { struct QBlockExprMacro; };
namespace templates_boost { struct QTypeMacro; };
namespace templates_boost { struct AstQNamedMacro; };
namespace templates_boost { struct AstQFunctionMacro; };
namespace templates_boost { struct AstQVariableMacro; };
namespace templates_boost { struct AstQNamedClassMacro; };
namespace templates_boost { struct AstQMethodMacro; };
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace sort_boost { struct QsortMacro; };
// unused enumeration ConversionResult
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
// unused enumeration CaptureMode
// unused enumeration SideEffects
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
// unused structure AstVariantMacro
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
// unused structure AstReaderMacro
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure MacroMacro
// unused structure TagFunctionAnnotation
// unused structure TagStructureAnnotation
// unused structure SetupAnyAnnotation
// unused structure SetupFunctionAnnotation
// unused structure SetupBlockAnnotation
// unused structure SetupStructureAnnotation
// unused structure SetupEnumerationAnnotation
// unused structure SetupContractAnnotation
// unused structure SetupReaderMacro
// unused structure SetupCommentReader
// unused structure SetupVariantMacro
// unused structure SetupForLoopMacro
// unused structure SetupCaptureMacro
// unused structure SetupTypeMacro
// unused structure SetupSimulateMacro
// unused structure SetupCallMacro
// unused structure SetupTypeInfoMacro
// unused structure SetupInferMacro
// unused structure SetupDirtyInferMacro
// unused structure SetupLintMacro
// unused structure SetupGlobalLintMacro
// unused structure SetupOptimizationMacro
// unused structure TagFunctionMacro
// unused structure BetterRttiVisitor
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
// unused structure IsAnyType
// unused structure IsAnyArrayMacro
// unused structure IsAnyEnumMacro
// unused structure IsAnyBitfieldMacro
// unused structure IsAnyVectorType
// unused structure IsAnyStructMacro
// unused structure IsAnyNumericMacro
// unused structure IsAnyWorkhorse
// unused structure IsAnyWorkhorseNonPtrMacro
// unused structure IsAnyTupleNonPtrMacro
// unused structure IsAnyVariantNonPtrMacro
// unused structure IsAnyFunctionNonPtrMacro
// unused structure IsAnyLambdaMacro
// unused structure IsRefMacro
// unused structure IsPointer
// unused structure IsClass
// unused structure IsValueHandle
// unused structure Template
// unused structure TemplateVisitor
// unused structure RemoveDerefVisitor
// unused structure QRulesVisitor
// unused structure AstQCallMacro
// unused structure QMacro
// unused structure QBlockMacro
// unused structure QBlockToArrayMacro
// unused structure QBlockExprMacro
// unused structure QTypeMacro
// unused structure AstQNamedMacro
// unused structure AstQFunctionMacro
// unused structure AstQVariableMacro
// unused structure AstQNamedClassMacro
// unused structure AstQMethodMacro
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure QsortMacro
extern TypeInfo __type_info__5eddda99b960ff80;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__37d36026a6078a42;
extern TypeInfo __type_info__af63ee4c86020b22;

TypeInfo __type_info__5eddda99b960ff80 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63ee4c86020b22, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x5eddda99b960ff80) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__37d36026a6078a42 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~strings::StringBuilderWriter"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 288, UINT64_C(0x37d36026a6078a42) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__37d36026a6078a42, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__5eddda99b960ff80 };
TypeInfo * __tinfo_1[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_3[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_4[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_6[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_7[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[2] = { &__type_info__37d36026a6078a42, &__type_info__af63ee4c86020b22 };
TypeInfo * __tinfo_9[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_15[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_16[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_17[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_18[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_19[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_20[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_21[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_22[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_23[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_24[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_25[2] = { &__type_info__37d36026a6078a42, &__type_info__af90fe4c864e9d52 };

inline void finalize_9eb64692a374c6cb ( Context * __context__, AutoTuple<char *,char *> & ____this_rename_at_1186_0 );
inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0 ( Context * __context__, TArray<char *> & __a_rename_at_50_1 );
inline void _FuncbuiltinTickpushTick10769833213962245646_2f7ec56c9ac7d6d0 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_181_2, AutoTuple<char *,char *> & __value_rename_at_181_3 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_baa2ae866f630441 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1182_4 );
inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> & __Arr_rename_at_165_6, char * const  __value_rename_at_165_7 );
inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  & __a_rename_at_1128_8 );
inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b ( Context * __context__, TTable<char *,char *> const  & __a_rename_at_1143_10 );
inline void _FuncbuiltinTicksortTick5995501125257068354_4524eaa061a094a8 ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1576_12, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const ,AutoTuple<char *,char *> const )) const  & __cmp_rename_at_1576_13 );
inline TArray<char *> uri_split_full_path_61c8cae43e9301c5 ( Context * __context__, Uri const  & __uri_rename_at_14_14 );
inline char * uri_compose_query_8e868129edf6b854 ( Context * __context__, TTable<char *,char *> const  & __query_rename_at_25_17 );
inline char * uri_compose_query_in_order_c110ae58bd0a3254 ( Context * __context__, TTable<char *,char *> const  & __query_rename_at_44_24 );
inline char * scheme_868a118e00451a9e ( Context * __context__, Uri const  & __uri_rename_at_77_36 );
inline char * user_info_4532747a877d046e ( Context * __context__, Uri const  & __uri_rename_at_82_37 );
inline char * host_d328a0026aab75fe ( Context * __context__, Uri const  & __uri_rename_at_87_38 );
inline char * port_57f83a3de58711ae ( Context * __context__, Uri const  & __uri_rename_at_92_39 );
inline char * path_26d1281b144ca20f ( Context * __context__, Uri const  & __uri_rename_at_97_40 );
inline char * query_70b3028abd53148e ( Context * __context__, Uri const  & __uri_rename_at_114_44 );
inline char * fragment_f9c74510412f8a1e ( Context * __context__, Uri const  & __uri_rename_at_119_45 );
inline Uri uri_compose_f6e4390f66d274f0 ( Context * __context__, char * const  __scheme_rename_at_124_46, char * const  __userInfo_rename_at_124_47, char * const  __hostText_rename_at_124_48, char * const  __portText_rename_at_124_49, char * const  __path_rename_at_124_50, char * const  __query_rename_at_124_51, char * const  __fragment_rename_at_124_52 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void finalize_9eb64692a374c6cb ( Context * __context__, AutoTuple<char *,char *> &  ____this_rename_at_1186_0 )
{
    memset(&(____this_rename_at_1186_0), 0, TypeSize<AutoTuple<char *,char *>>::size);
}

inline TArray<char *> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0 ( Context * __context__, TArray<char *> &  __a_rename_at_50_1 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<char *> &>::from(__a_rename_at_50_1)));
    return das_auto_cast_ref<TArray<char *> &>::cast(__a_rename_at_50_1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_2f7ec56c9ac7d6d0 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_181_2, AutoTuple<char *,char *> &  __value_rename_at_181_3 )
{
    das_copy(__Arr_rename_at_181_2(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_181_2),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_3);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_baa2ae866f630441 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1182_4 )
{
    {
        bool __need_loop_1184 = true;
        // aV: tuple<key:string;value:string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1182_4);
        AutoTuple<char *,char *> * __aV_rename_at_1184_5;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_5)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_5)) )
        {
            finalize_9eb64692a374c6cb(__context__,das_arg<AutoTuple<char *,char *>>::pass((*__aV_rename_at_1184_5)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_5));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1182_4),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af ( Context * __context__, TArray<char *> &  __Arr_rename_at_165_6, char * const  __value_rename_at_165_7 )
{
    das_copy(__Arr_rename_at_165_6(builtin_array_push_back(das_arg<TArray<char *>>::pass(__Arr_rename_at_165_6),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_7);
}

inline Sequence DAS_COMMENT((char *)) _FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888 ( Context * __context__, TTable<char *,char *> const  &  __a_rename_at_1128_8 )
{
    Sequence DAS_COMMENT((char *)) __it_rename_at_1129_9; das_zero(__it_rename_at_1129_9);
    builtin_table_keys(das_arg<Sequence DAS_COMMENT((char *))>::pass(__it_rename_at_1129_9),__a_rename_at_1128_8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char *))>::cast(__it_rename_at_1129_9);
}

inline Sequence DAS_COMMENT((char * &)) _FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b ( Context * __context__, TTable<char *,char *> const  &  __a_rename_at_1143_10 )
{
    Sequence DAS_COMMENT((char * *)) __it_rename_at_1144_11; das_zero(__it_rename_at_1144_11);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((char * const ))>::pass(__it_rename_at_1144_11),__a_rename_at_1143_10,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((char * &))>::cast(__it_rename_at_1144_11);
}

inline void _FuncbuiltinTicksortTick5995501125257068354_4524eaa061a094a8 ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1576_12, Block DAS_COMMENT((bool,AutoTuple<char *,char *> const ,AutoTuple<char *,char *> const )) const  &  __cmp_rename_at_1576_13 )
{
    builtin_sort_array_any_cblock_T(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1576_12),16,builtin_array_size(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1576_12)),__cmp_rename_at_1576_13,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline TArray<char *> uri_split_full_path_61c8cae43e9301c5 ( Context * __context__, Uri const  &  __uri_rename_at_14_14 )
{
    TArray<char *> __paths_rename_at_16_15; das_zero(__paths_rename_at_16_15);
    UriPathSegmentStructA const  * __pcur_rename_at_17_16 = __uri_rename_at_14_14.uri /*uri*/.pathHead /*pathHead*/;
    while ( __pcur_rename_at_17_16 != nullptr )
    {
        _FuncbuiltinTickpushTick14133213201864676143_d626258b591934af(__context__,das_arg<TArray<char *>>::pass(__paths_rename_at_16_15),((char * const )(text_range_to_string(__pcur_rename_at_17_16->text /*text*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
        das_copy(__pcur_rename_at_17_16,__pcur_rename_at_17_16->next /*next*/);
    };
    return /* <- */ das_auto_cast_move<TArray<char *>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0(__context__,das_arg<TArray<char *>>::pass(__paths_rename_at_16_15)));
}

inline char * uri_compose_query_8e868129edf6b854 ( Context * __context__, TTable<char *,char *> const  &  __query_rename_at_25_17 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __tw_rename_at_27_18) DAS_AOT_INLINE_LAMBDA -> void{
        Sequence DAS_COMMENT((char *)) _temp_make_local_29_27_112; _temp_make_local_29_27_112;
        Sequence DAS_COMMENT((char * *)) _temp_make_local_29_40_128; _temp_make_local_29_40_128;
        bool __first_rename_at_28_19 = true;
        {
            bool __need_loop_29 = true;
            // key: string
            das_iterator<Sequence DAS_COMMENT((char *))> __key_iterator((_temp_make_local_29_27_112 = (_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888(__context__,__query_rename_at_25_17))));
            char * __key_rename_at_29_22;
            __need_loop_29 = __key_iterator.first(__context__,(__key_rename_at_29_22)) && __need_loop_29;
            // value: string const&
            das_iterator<Sequence DAS_COMMENT((char * const ))> __value_iterator((_temp_make_local_29_40_128 = (_FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b(__context__,__query_rename_at_25_17))));
            char * const  * __value_rename_at_29_23;
            __need_loop_29 = __value_iterator.first(__context__,(__value_rename_at_29_23)) && __need_loop_29;
            for ( ; __need_loop_29 ; __need_loop_29 = __key_iterator.next(__context__,(__key_rename_at_29_22)) && __value_iterator.next(__context__,(__value_rename_at_29_23)) )
            {
                if ( __first_rename_at_28_19 )
                {
                    das_copy(__first_rename_at_28_19,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_1,cast<StringBuilderWriter &>::from(__tw_rename_at_27_18),cast<char * const >::from(((char *) "&"))));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_2,cast<StringBuilderWriter &>::from(__tw_rename_at_27_18),cast<char *>::from(__key_rename_at_29_22)));
                if ( !builtin_empty((*__value_rename_at_29_23)) )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_3,cast<StringBuilderWriter &>::from(__tw_rename_at_27_18),cast<char * const >::from(((char *) "="))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_4,cast<StringBuilderWriter &>::from(__tw_rename_at_27_18),cast<char * const >::from((*__value_rename_at_29_23))));
                };
            }
            __key_iterator.close(__context__,(__key_rename_at_29_22));
            __value_iterator.close(__context__,(__value_rename_at_29_23));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * uri_compose_query_in_order_c110ae58bd0a3254 ( Context * __context__, TTable<char *,char *> const  &  __query_rename_at_44_24 )
{
    Sequence DAS_COMMENT((char *)) _temp_make_local_47_23_64; _temp_make_local_47_23_64;
    Sequence DAS_COMMENT((char * *)) _temp_make_local_47_36_80; _temp_make_local_47_36_80;
    TArray<AutoTuple<char *,char *>> __vq_rename_at_46_25; das_zero(__vq_rename_at_46_25);
    {
        bool __need_loop_47 = true;
        // key: string
        das_iterator<Sequence DAS_COMMENT((char *))> __key_iterator((_temp_make_local_47_23_64 = (_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888(__context__,__query_rename_at_44_24))));
        char * __key_rename_at_47_28;
        __need_loop_47 = __key_iterator.first(__context__,(__key_rename_at_47_28)) && __need_loop_47;
        // value: string const&
        das_iterator<Sequence DAS_COMMENT((char * const ))> __value_iterator((_temp_make_local_47_36_80 = (_FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b(__context__,__query_rename_at_44_24))));
        char * const  * __value_rename_at_47_29;
        __need_loop_47 = __value_iterator.first(__context__,(__value_rename_at_47_29)) && __need_loop_47;
        for ( ; __need_loop_47 ; __need_loop_47 = __key_iterator.next(__context__,(__key_rename_at_47_28)) && __value_iterator.next(__context__,(__value_rename_at_47_29)) )
        {
            AutoTuple<char *,char *> _temp_make_local_48_20_128; _temp_make_local_48_20_128;
            _FuncbuiltinTickpushTick10769833213962245646_2f7ec56c9ac7d6d0(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__vq_rename_at_46_25),das_arg<AutoTuple<char *,char *>>::pass((([&]() -> AutoTuple<char *,char *>& {
                das_get_auto_tuple_field<char *,0,char *,char *>::get(_temp_make_local_48_20_128) = __key_rename_at_47_28;
                das_get_auto_tuple_field<char *,1,char *,char *>::get(_temp_make_local_48_20_128) = (*__value_rename_at_47_29);
                return _temp_make_local_48_20_128;
            })())));
        }
        __key_iterator.close(__context__,(__key_rename_at_47_28));
        __value_iterator.close(__context__,(__value_rename_at_47_29));
    };
    builtin_sort_array_any_cblock_T(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__vq_rename_at_46_25),16,0,[&](AutoTuple<char *,char *> const  & __a_rename_at_50_30, AutoTuple<char *,char *> const  & __b_rename_at_50_31) DAS_AOT_INLINE_LAMBDA -> bool{
        return das_auto_cast<bool>::cast((SimPolicy<char *>::Equ(cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get(__a_rename_at_50_30)),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get(__b_rename_at_50_31)),*__context__,nullptr)) ? das_auto_cast<bool>::cast((SimPolicy<char *>::Less(cast<char *>::from(das_get_auto_tuple_field<char *,1,char *,char *>::get(__a_rename_at_50_30)),cast<char *>::from(das_get_auto_tuple_field<char *,1,char *,char *>::get(__b_rename_at_50_31)),*__context__,nullptr))) : das_auto_cast<bool>::cast((SimPolicy<char *>::Less(cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get(__a_rename_at_50_30)),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get(__b_rename_at_50_31)),*__context__,nullptr))));
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    char * __res_rename_at_58_32 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __tw_rename_at_58_33) DAS_AOT_INLINE_LAMBDA -> void{
        bool __first_rename_at_59_34 = true;
        {
            bool __need_loop_60 = true;
            // kv: tuple<key:string;value:string>&
            das_iterator<TArray<AutoTuple<char *,char *>>> __kv_iterator(__vq_rename_at_46_25);
            AutoTuple<char *,char *> * __kv_rename_at_60_35;
            __need_loop_60 = __kv_iterator.first(__context__,(__kv_rename_at_60_35)) && __need_loop_60;
            for ( ; __need_loop_60 ; __need_loop_60 = __kv_iterator.next(__context__,(__kv_rename_at_60_35)) )
            {
                if ( __first_rename_at_59_34 )
                {
                    das_copy(__first_rename_at_59_34,false);
                } else {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_5,cast<StringBuilderWriter &>::from(__tw_rename_at_58_33),cast<char * const >::from(((char *) "&"))));
                };
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_6,cast<StringBuilderWriter &>::from(__tw_rename_at_58_33),cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,char *>::get((*__kv_rename_at_60_35)))));
                if ( !builtin_empty(das_get_auto_tuple_field<char *,1,char *,char *>::get((*__kv_rename_at_60_35))) )
                {
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_7,cast<StringBuilderWriter &>::from(__tw_rename_at_58_33),cast<char * const >::from(((char *) "="))));
                    das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_8,cast<StringBuilderWriter &>::from(__tw_rename_at_58_33),cast<char *>::from(das_get_auto_tuple_field<char *,1,char *,char *>::get((*__kv_rename_at_60_35)))));
                };
            }
            __kv_iterator.close(__context__,(__kv_rename_at_60_35));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    _FuncbuiltinTickfinalizeTick13836114024949725080_baa2ae866f630441(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__vq_rename_at_46_25));
    return das_auto_cast<char *>::cast(__res_rename_at_58_32);
}

inline char * scheme_868a118e00451a9e ( Context * __context__, Uri const  &  __uri_rename_at_77_36 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_77_36.uri /*uri*/.scheme /*scheme*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * user_info_4532747a877d046e ( Context * __context__, Uri const  &  __uri_rename_at_82_37 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_82_37.uri /*uri*/.userInfo /*userInfo*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * host_d328a0026aab75fe ( Context * __context__, Uri const  &  __uri_rename_at_87_38 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_87_38.uri /*uri*/.hostText /*hostText*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * port_57f83a3de58711ae ( Context * __context__, Uri const  &  __uri_rename_at_92_39 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_92_39.uri /*uri*/.portText /*portText*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * path_26d1281b144ca20f ( Context * __context__, Uri const  &  __uri_rename_at_97_40 )
{
    return das_auto_cast<char *>::cast(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __tw_rename_at_99_41) DAS_AOT_INLINE_LAMBDA -> void{
        UriPathSegmentStructA const  * __pcur_rename_at_100_42 = __uri_rename_at_97_40.uri /*uri*/.pathHead /*pathHead*/;
        bool __first_rename_at_101_43 = true;
        while ( __pcur_rename_at_100_42 != nullptr )
        {
            if ( __first_rename_at_101_43 )
            {
                das_copy(__first_rename_at_101_43,false);
            } else {
                das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_9,cast<StringBuilderWriter &>::from(__tw_rename_at_99_41),cast<char * const >::from(((char *) "/"))));
            };
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_10,cast<StringBuilderWriter &>::from(__tw_rename_at_99_41),cast<char * const >::from(((char * const )(text_range_to_string(__pcur_rename_at_100_42->text /*text*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))))));
            das_copy(__pcur_rename_at_100_42,__pcur_rename_at_100_42->next /*next*/);
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * query_70b3028abd53148e ( Context * __context__, Uri const  &  __uri_rename_at_114_44 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_114_44.uri /*uri*/.query /*query*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline char * fragment_f9c74510412f8a1e ( Context * __context__, Uri const  &  __uri_rename_at_119_45 )
{
    return das_auto_cast<char *>::cast(((char * const )(text_range_to_string(__uri_rename_at_119_45.uri /*uri*/.fragment /*fragment*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline Uri uri_compose_f6e4390f66d274f0 ( Context * __context__, char * const  __scheme_rename_at_124_46, char * const  __userInfo_rename_at_124_47, char * const  __hostText_rename_at_124_48, char * const  __portText_rename_at_124_49, char * const  __path_rename_at_124_50, char * const  __query_rename_at_124_51, char * const  __fragment_rename_at_124_52 )
{
    char * __text_rename_at_126_53 = ((char *)(char *)(((char * const )(builtin_build_string_T([&](StringBuilderWriter & __tw_rename_at_126_54) DAS_AOT_INLINE_LAMBDA -> void{
        if ( !builtin_empty(__scheme_rename_at_124_46) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_11,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__scheme_rename_at_124_46)));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_12,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) "://"))));
        };
        if ( !builtin_empty(__userInfo_rename_at_124_47) )
        {
            toLog(0,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_13, cast<char * const >::from(((char *) "uri_compose: USER = ")), cast<char * const >::from(__userInfo_rename_at_124_47), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_14,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__userInfo_rename_at_124_47)));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_15,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) "@"))));
        };
        if ( !builtin_empty(__hostText_rename_at_124_48) )
        {
            toLog(0,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_16, cast<char * const >::from(((char *) "uri_compose: HOST = ")), cast<char * const >::from(__hostText_rename_at_124_48), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_17,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__hostText_rename_at_124_48)));
        };
        if ( !builtin_empty(__portText_rename_at_124_49) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_18,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) ":"))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_19,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__portText_rename_at_124_49)));
        };
        if ( !builtin_empty(__path_rename_at_124_50) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_20,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) "/"))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_21,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__path_rename_at_124_50)));
        };
        if ( !builtin_empty(__query_rename_at_124_51) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_22,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) "?"))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_23,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__query_rename_at_124_51)));
        };
        if ( !builtin_empty(__fragment_rename_at_124_52) )
        {
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_24,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(((char *) "#"))));
            das_call_interop<StringBuilderWriter &>::call(&builtin_write_string,__context__,SimNode_AotInterop<2>(__tinfo_25,cast<StringBuilderWriter &>::from(__tw_rename_at_126_54),cast<char * const >::from(__fragment_rename_at_124_52)));
        };
    },__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
    return /* <- */ das_auto_cast_move<Uri>::cast(Uri(__text_rename_at_126_53));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xdcb94002410ce3ed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_9eb64692a374c6cb>>();
    };
    aotLib[0x8ff205c681819963] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_354a9a0b57d554a0>>();
    };
    aotLib[0x6e4c5e9811061cac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_2f7ec56c9ac7d6d0>>();
    };
    aotLib[0x777463895cad056a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_baa2ae866f630441>>();
    };
    aotLib[0x3c395992954dd60d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_d626258b591934af>>();
    };
    aotLib[0xeefb3e272bbfb4a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickkeysTick2205854368403803976_2df91d3f4a5c8888>>();
    };
    aotLib[0xc88537e2e7d634e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1935193042646774172_bb2c3e8e5405df3b>>();
    };
    aotLib[0xb58c472181b851f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicksortTick5995501125257068354_4524eaa061a094a8>>();
    };
    aotLib[0xc5c07999e07face8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&uri_split_full_path_61c8cae43e9301c5>>();
    };
    aotLib[0x3b153912aea97865] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uri_compose_query_8e868129edf6b854>>();
    };
    aotLib[0x70c811f5ee06ec12] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&uri_compose_query_in_order_c110ae58bd0a3254>>();
    };
    aotLib[0xa6f05457091c2006] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&scheme_868a118e00451a9e>>();
    };
    aotLib[0x211579d83341bcd1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&user_info_4532747a877d046e>>();
    };
    aotLib[0x779fde396ae60bb5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&host_d328a0026aab75fe>>();
    };
    aotLib[0xca780240902fad22] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&port_57f83a3de58711ae>>();
    };
    aotLib[0x902cd0d487f0f580] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&path_26d1281b144ca20f>>();
    };
    aotLib[0xd19b6c60bd237fe7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&query_70b3028abd53148e>>();
    };
    aotLib[0x4128f4c1e48b36be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fragment_f9c74510412f8a1e>>();
    };
    aotLib[0xd4c0fd34e01c1fc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&uri_compose_f6e4390f66d274f0>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_14054407420064321909
AotListBase impl_aot_uriparser_boost(_anon_14054407420064321909::registerAotFunctions);
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
