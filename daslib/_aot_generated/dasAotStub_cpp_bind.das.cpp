#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require cpp_bind

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
namespace _anon_10965320641594931681 {

namespace fio { struct df_header; };
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
// unused enumeration ConversionResult
// unused structure df_header
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
extern TypeInfo __type_info__3c61146b2bdfb90;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af63e44c8601fa24;
extern TypeInfo __type_info__af63ee4c86020b22;

TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, 32, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaf63e44c8601fa24) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__3c61146b2bdfb90, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_2[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_3[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_4[1] = { &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_5[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_6[1] = { &__type_info__3c61146b2bdfb90 };
TypeInfo * __tinfo_7[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_8[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_9[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af8afe4c86446b52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_10[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_11[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52, &__type_info__af63e44c8601fa24, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_12[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_13[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_14[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af63ee4c86020b22 };

inline void clone_c29a2ce3ce04a328 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_367_0, smart_ptr_raw<TypeDecl> const  __src_rename_at_367_1 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9 ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> & __a_rename_at_1182_2 );
inline char * _FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_43_4, bool __substitureRef_rename_at_43_5, bool __skipRef_rename_at_43_6, bool __skipConst_rename_at_43_7, bool __redundantConst_rename_at_43_8, bool __chooseSmartPtr_rename_at_43_9 );
inline void _FuncbuiltinTickpushTick10769833213962245646_b3fa6014e447ff2d ( Context * __context__, TArray<AutoTuple<char *,int32_t>> & __Arr_rename_at_181_10, AutoTuple<char *,int32_t> & __value_rename_at_181_11 );
inline void _FuncbuiltinTickpush_cloneTick15769051505004837089_338aab8aa242d2bb ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> & __Arr_rename_at_361_12, smart_ptr_raw<TypeDecl> const  __value_rename_at_361_13 );
inline void log_cpp_class_method_307c561cdfa4a8db ( Context * __context__, FILE const  * const  __cpp_file_rename_at_15_14, char * const  __name_rename_at_15_15, smart_ptr_raw<TypeDecl> const  __fntype_rename_at_15_16 );
inline void log_cpp_class_method_call_811ba437c8a50b47 ( Context * __context__, FILE const  * const  __cpp_file_rename_at_36_29, char * const  __name_rename_at_36_30, smart_ptr_raw<TypeDecl> const  __fntype_rename_at_36_31 );
inline void log_cpp_class_adapter_92277e6cbae7f863 ( Context * __context__, FILE const  * const  __cpp_file_rename_at_66_47, char * const  __name_rename_at_66_48, smart_ptr_raw<TypeDecl> const  __cinfo_rename_at_66_49 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void clone_c29a2ce3ce04a328 ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_367_0, smart_ptr_raw<TypeDecl> const  __src_rename_at_367_1 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_367_0),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_367_1),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9 ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> &  __a_rename_at_1182_2 )
{
    {
        bool __need_loop_1184 = true;
        // aV: smart_ptr<ast::TypeDecl> aka TT&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __aV_iterator(__a_rename_at_1182_2);
        smart_ptr_raw<TypeDecl> * __aV_rename_at_1184_3;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_3)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_3)) )
        {
            das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,(*__aV_rename_at_1184_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_3));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1182_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_43_4, bool __substitureRef_rename_at_43_5, bool __skipRef_rename_at_43_6, bool __skipConst_rename_at_43_7, bool __redundantConst_rename_at_43_8, bool __chooseSmartPtr_rename_at_43_9 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl_cpp(__decl_rename_at_43_4,__substitureRef_rename_at_43_5,__skipRef_rename_at_43_6,__skipConst_rename_at_43_7,__redundantConst_rename_at_43_8,__chooseSmartPtr_rename_at_43_9,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_b3fa6014e447ff2d ( Context * __context__, TArray<AutoTuple<char *,int32_t>> &  __Arr_rename_at_181_10, AutoTuple<char *,int32_t> &  __value_rename_at_181_11 )
{
    das_copy(__Arr_rename_at_181_10(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,int32_t>>>::pass(__Arr_rename_at_181_10),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_11);
}

inline void _FuncbuiltinTickpush_cloneTick15769051505004837089_338aab8aa242d2bb ( Context * __context__, TArray<smart_ptr_raw<TypeDecl>> &  __Arr_rename_at_361_12, smart_ptr_raw<TypeDecl> const  __value_rename_at_361_13 )
{
    clone_c29a2ce3ce04a328(__context__,__Arr_rename_at_361_12(builtin_array_push_back_zero(das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__Arr_rename_at_361_12),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_361_13);
}

inline void log_cpp_class_method_307c561cdfa4a8db ( Context * __context__, FILE const  * const  __cpp_file_rename_at_15_14, char * const  __name_rename_at_15_15, smart_ptr_raw<TypeDecl> const  __fntype_rename_at_15_16 )
{
    smart_ptr_raw<TypeDecl> __cft_rename_at_16_17; memset((void*)&__cft_rename_at_16_17,0,sizeof(__cft_rename_at_16_17));
    char * __rettd_rename_at_18_18; memset((void*)&__rettd_rename_at_18_18,0,sizeof(__rettd_rename_at_18_18));
    char * __argtd_rename_at_25_28; memset((void*)&__argtd_rename_at_25_28,0,sizeof(__argtd_rename_at_25_28));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_20_63_64; _temp_make_local_20_63_64;
    /* finally */ auto __finally_15= das_finally([&](){
    das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,__cft_rename_at_16_17);
    /* end finally */ });
    __cft_rename_at_16_17; das_zero(__cft_rename_at_16_17); das_move(__cft_rename_at_16_17, clone_type(__fntype_rename_at_15_16->firstType /*firstType*/));
    __cft_rename_at_16_17->flags /*flags*/ |= 0x800u;
    __rettd_rename_at_18_18 = ((char *)(char *)(_FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262(__context__,__cft_rename_at_16_17,false,false,false,true,true)));
    builtin_fwrite(__cpp_file_rename_at_15_14,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_0, cast<char * const >::from(__rettd_rename_at_18_18), cast<char * const >::from(((char *) " ")), cast<char * const >::from(__name_rename_at_15_15), cast<char * const >::from(((char *) " ( Context * __context__, Func __funcCall__")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_20 = true;
        // argN: $::das_string const&
        das_iterator<das::vector<das::string> const > __argN_iterator(__fntype_rename_at_15_16->argNames /*argNames*/);
        das::string const  * __argN_rename_at_20_25;
        __need_loop_20 = __argN_iterator.first(__context__,(__argN_rename_at_20_25)) && __need_loop_20;
        // argT: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __argT_iterator(__fntype_rename_at_15_16->argTypes /*argTypes*/);
        smart_ptr_raw<TypeDecl> const  * __argT_rename_at_20_26;
        __need_loop_20 = __argT_iterator.first(__context__,(__argT_rename_at_20_26)) && __need_loop_20;
        // argI: int
        das_iterator_count DAS_COMMENT((_temp_make_local_20_63_64 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argI_iterator(0,1);
        int32_t __argI_rename_at_20_27;
        __need_loop_20 = __argI_iterator.first(__context__,(__argI_rename_at_20_27)) && __need_loop_20;
        for ( ; __need_loop_20 ; __need_loop_20 = __argN_iterator.next(__context__,(__argN_rename_at_20_25)) && __argT_iterator.next(__context__,(__argT_rename_at_20_26)) && __argI_iterator.next(__context__,(__argI_rename_at_20_27)) )
        {
            builtin_fwrite(__cpp_file_rename_at_15_14,((char *) ", "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            if ( __argI_rename_at_20_27 == 0 )
            {
                builtin_fwrite(__cpp_file_rename_at_15_14,((char *) "void * "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                __argtd_rename_at_25_28 = ((char *)(char *)(_FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262(__context__,(*__argT_rename_at_20_26),false,false,false,true,true)));
                builtin_fwrite(__cpp_file_rename_at_15_14,das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_1, cast<char * const >::from(__argtd_rename_at_25_28), cast<char * const >::from(((char *) " ")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                if ( ((das_deref(__context__,(*__argT_rename_at_20_26))).isRefType()) )
                {
                    builtin_fwrite(__cpp_file_rename_at_15_14,((char *) "& "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
            builtin_fwrite(__cpp_file_rename_at_15_14,das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_2, cast<das::string const  &>::from((*__argN_rename_at_20_25)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __argN_iterator.close(__context__,(__argN_rename_at_20_25));
        __argT_iterator.close(__context__,(__argT_rename_at_20_26));
        __argI_iterator.close(__context__,(__argI_rename_at_20_27));
    };
    builtin_fwrite(__cpp_file_rename_at_15_14,((char *) " )"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void log_cpp_class_method_call_811ba437c8a50b47 ( Context * __context__, FILE const  * const  __cpp_file_rename_at_36_29, char * const  __name_rename_at_36_30, smart_ptr_raw<TypeDecl> const  __fntype_rename_at_36_31 )
{
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_39_63_48; _temp_make_local_39_63_48;
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_54_40_144; _temp_make_local_54_40_144;
    char * __rettd_rename_at_37_32 = ((char *)(char *)(_FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262(__context__,__fntype_rename_at_36_31->firstType /*firstType*/,false,false,false,true,true)));
    builtin_fwrite(__cpp_file_rename_at_36_29,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_3, cast<char * const >::from(((char *) "das_invoke_function<")), cast<char * const >::from(__rettd_rename_at_37_32), cast<char * const >::from(((char *) ">::invoke\n      <")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_39 = true;
        // argN: $::das_string const&
        das_iterator<das::vector<das::string> const > __argN_iterator(__fntype_rename_at_36_31->argNames /*argNames*/);
        das::string const  * __argN_rename_at_39_39;
        __need_loop_39 = __argN_iterator.first(__context__,(__argN_rename_at_39_39)) && __need_loop_39;
        // argT: smart_ptr<ast::TypeDecl> const&
        das_iterator<das::vector<smart_ptr<TypeDecl>> const > __argT_iterator(__fntype_rename_at_36_31->argTypes /*argTypes*/);
        smart_ptr_raw<TypeDecl> const  * __argT_rename_at_39_40;
        __need_loop_39 = __argT_iterator.first(__context__,(__argT_rename_at_39_40)) && __need_loop_39;
        // argI: int
        das_iterator_count DAS_COMMENT((_temp_make_local_39_63_48 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argI_iterator(0,1);
        int32_t __argI_rename_at_39_41;
        __need_loop_39 = __argI_iterator.first(__context__,(__argI_rename_at_39_41)) && __need_loop_39;
        for ( ; __need_loop_39 ; __need_loop_39 = __argN_iterator.next(__context__,(__argN_rename_at_39_39)) && __argT_iterator.next(__context__,(__argT_rename_at_39_40)) && __argI_iterator.next(__context__,(__argI_rename_at_39_41)) )
        {
            if ( __argI_rename_at_39_41 != 0 )
            {
                builtin_fwrite(__cpp_file_rename_at_36_29,((char *) ","),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            if ( __argI_rename_at_39_41 == 0 )
            {
                builtin_fwrite(__cpp_file_rename_at_36_29,((char *) "void *"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                char * __argtd_rename_at_46_42 = ((char *)(char *)(_FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262(__context__,(*__argT_rename_at_39_40),false,false,false,true,true)));
                builtin_fwrite(__cpp_file_rename_at_36_29,das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_4, cast<char * const >::from(__argtd_rename_at_46_42))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                if ( ((das_deref(__context__,(*__argT_rename_at_39_40))).isRefType()) )
                {
                    builtin_fwrite(__cpp_file_rename_at_36_29,((char *) " &"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
                };
            };
        }
        __argN_iterator.close(__context__,(__argN_rename_at_39_39));
        __argT_iterator.close(__context__,(__argT_rename_at_39_40));
        __argI_iterator.close(__context__,(__argI_rename_at_39_41));
    };
    builtin_fwrite(__cpp_file_rename_at_36_29,((char *) ">\n        (__context__,nullptr,__funcCall__,\n          "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_54 = true;
        // argN: $::das_string const&
        das_iterator<das::vector<das::string> const > __argN_iterator(__fntype_rename_at_36_31->argNames /*argNames*/);
        das::string const  * __argN_rename_at_54_45;
        __need_loop_54 = __argN_iterator.first(__context__,(__argN_rename_at_54_45)) && __need_loop_54;
        // argI: int
        das_iterator_count DAS_COMMENT((_temp_make_local_54_40_144 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __argI_iterator(0,1);
        int32_t __argI_rename_at_54_46;
        __need_loop_54 = __argI_iterator.first(__context__,(__argI_rename_at_54_46)) && __need_loop_54;
        for ( ; __need_loop_54 ; __need_loop_54 = __argN_iterator.next(__context__,(__argN_rename_at_54_45)) && __argI_iterator.next(__context__,(__argI_rename_at_54_46)) )
        {
            if ( __argI_rename_at_54_46 != 0 )
            {
                builtin_fwrite(__cpp_file_rename_at_36_29,((char *) ","),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            builtin_fwrite(__cpp_file_rename_at_36_29,das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_5, cast<das::string const  &>::from((*__argN_rename_at_54_45)))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __argN_iterator.close(__context__,(__argN_rename_at_54_45));
        __argI_iterator.close(__context__,(__argI_rename_at_54_46));
    };
    builtin_fwrite(__cpp_file_rename_at_36_29,((char *) ")"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    if ( das_get_bitfield(__fntype_rename_at_36_31->firstType /*firstType*/->flags /*flags*/,1u << 10) )
    {
        builtin_fwrite(__cpp_file_rename_at_36_29,((char *) ".marshal()"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void log_cpp_class_adapter_92277e6cbae7f863 ( Context * __context__, FILE const  * const  __cpp_file_rename_at_66_47, char * const  __name_rename_at_66_48, smart_ptr_raw<TypeDecl> const  __cinfo_rename_at_66_49 )
{
    TArray<AutoTuple<char *,int32_t>> __methods_rename_at_72_50; memset((void*)&__methods_rename_at_72_50,0,sizeof(__methods_rename_at_72_50));
    TArray<smart_ptr_raw<TypeDecl>> __types_rename_at_73_51; memset((void*)&__types_rename_at_73_51,0,sizeof(__types_rename_at_73_51));
    int32_t __nmet_rename_at_81_56; memset((void*)&__nmet_rename_at_81_56,0,sizeof(__nmet_rename_at_81_56));
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_74_46_96; _temp_make_local_74_46_96;
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_85_29_272; _temp_make_local_85_29_272;
    Sequence DAS_COMMENT((int32_t)) _temp_make_local_93_29_320; _temp_make_local_93_29_320;
    /* finally */ auto __finally_66= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9(__context__,das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__types_rename_at_73_51));
    /* end finally */ });
    DAS_ASSERTF((((das_deref(__context__,__cinfo_rename_at_66_49)).isClass())),(((char *) "can only make adapter out of a class")));
    das_zero(__methods_rename_at_72_50);
    das_zero(__types_rename_at_73_51);
    {
        bool __need_loop_74 = true;
        // fld: ast::FieldDeclaration const&
        das_iterator<das::vector<Structure::FieldDeclaration> const > __fld_iterator(__cinfo_rename_at_66_49->structType /*structType*/->fields /*fields*/);
        Structure::FieldDeclaration const  * __fld_rename_at_74_54;
        __need_loop_74 = __fld_iterator.first(__context__,(__fld_rename_at_74_54)) && __need_loop_74;
        // idx: int
        das_iterator_count DAS_COMMENT((_temp_make_local_74_46_96 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __idx_iterator(0,1);
        int32_t __idx_rename_at_74_55;
        __need_loop_74 = __idx_iterator.first(__context__,(__idx_rename_at_74_55)) && __need_loop_74;
        for ( ; __need_loop_74 ; __need_loop_74 = __fld_iterator.next(__context__,(__fld_rename_at_74_54)) && __idx_iterator.next(__context__,(__idx_rename_at_74_55)) )
        {
            if ( (!(((das_deref(__context__,(*__fld_rename_at_74_54).type /*_type*/)).isFunction())) || (eq_dstr_str((*__fld_rename_at_74_54).name /*name*/,((char *) "__finalize")))) || (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast((*__fld_rename_at_74_54).init /*init*/),das_auto_cast<void * const >::cast(nullptr))) )
            {
                continue;
            } else {
                AutoTuple<char *,int32_t> _temp_make_local_78_23_240; _temp_make_local_78_23_240;
                _FuncbuiltinTickpushTick10769833213962245646_b3fa6014e447ff2d(__context__,das_arg<TArray<AutoTuple<char *,int32_t>>>::pass(__methods_rename_at_72_50),das_arg<AutoTuple<char *,int32_t>>::pass((([&]() -> AutoTuple<char *,int32_t>& {
                    das_get_auto_tuple_field<char *,0,char *,int32_t>::get(_temp_make_local_78_23_240) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_6, cast<das::string const  &>::from((*__fld_rename_at_74_54).name /*name*/)));
                    das_get_auto_tuple_field<int32_t,1,char *,int32_t>::get(_temp_make_local_78_23_240) = __idx_rename_at_74_55;
                    return _temp_make_local_78_23_240;
                })())));
                _FuncbuiltinTickpush_cloneTick15769051505004837089_338aab8aa242d2bb(__context__,das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__types_rename_at_73_51),(*__fld_rename_at_74_54).type /*_type*/);
            };
        }
        __fld_iterator.close(__context__,(__fld_rename_at_74_54));
        __idx_iterator.close(__context__,(__idx_rename_at_74_55));
    };
    __nmet_rename_at_81_56 = ((int32_t)builtin_array_size(das_arg<TArray<AutoTuple<char *,int32_t>>>::pass(__methods_rename_at_72_50)));
    builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_7, cast<char * const >::from(((char *) "class ")), cast<char * const >::from(__name_rename_at_66_48), cast<char * const >::from(((char *) " {\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "protected:\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  enum {\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_85 = true;
        // mn: tuple<name:string;index:int>&
        das_iterator<TArray<AutoTuple<char *,int32_t>>> __mn_iterator(__methods_rename_at_72_50);
        AutoTuple<char *,int32_t> * __mn_rename_at_85_59;
        __need_loop_85 = __mn_iterator.first(__context__,(__mn_rename_at_85_59)) && __need_loop_85;
        // mni: int
        das_iterator_count DAS_COMMENT((_temp_make_local_85_29_272 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __mni_iterator(0,1);
        int32_t __mni_rename_at_85_60;
        __need_loop_85 = __mni_iterator.first(__context__,(__mni_rename_at_85_60)) && __need_loop_85;
        for ( ; __need_loop_85 ; __need_loop_85 = __mn_iterator.next(__context__,(__mn_rename_at_85_59)) && __mni_iterator.next(__context__,(__mni_rename_at_85_60)) )
        {
            builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_8, cast<char * const >::from(((char *) "    __fn_")), cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_85_59))), cast<char * const >::from(((char *) " = ")), cast<int32_t>::from(__mni_rename_at_85_60), cast<char * const >::from(((char *) ",\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __mn_iterator.close(__context__,(__mn_rename_at_85_59));
        __mni_iterator.close(__context__,(__mni_rename_at_85_60));
    };
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  };\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "protected:\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_9, cast<char * const >::from(((char *) "  int _das_class_method_offset[")), cast<int32_t>::from(__nmet_rename_at_81_56), cast<char * const >::from(((char *) "];\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "public:\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_10, cast<char * const >::from(((char *) "  ")), cast<char * const >::from(__name_rename_at_66_48), cast<char * const >::from(((char *) " ( const StructInfo * info ) {\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_93 = true;
        // mn: tuple<name:string;index:int>&
        das_iterator<TArray<AutoTuple<char *,int32_t>>> __mn_iterator(__methods_rename_at_72_50);
        AutoTuple<char *,int32_t> * __mn_rename_at_93_63;
        __need_loop_93 = __mn_iterator.first(__context__,(__mn_rename_at_93_63)) && __need_loop_93;
        // mni: int
        das_iterator_count DAS_COMMENT((_temp_make_local_93_29_320 = (builtin_count(0,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))) __mni_iterator(0,1);
        int32_t __mni_rename_at_93_64;
        __need_loop_93 = __mni_iterator.first(__context__,(__mni_rename_at_93_64)) && __need_loop_93;
        for ( ; __need_loop_93 ; __need_loop_93 = __mn_iterator.next(__context__,(__mn_rename_at_93_63)) && __mni_iterator.next(__context__,(__mni_rename_at_93_64)) )
        {
            builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_11, cast<char * const >::from(((char *) "      _das_class_method_offset[__fn_")), cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_93_63))), cast<char * const >::from(((char *) "] = info->fields[")), cast<int32_t>::from(das_get_auto_tuple_field<int32_t,1,char *,int32_t>::get((*__mn_rename_at_93_63))), cast<char * const >::from(((char *) "]->offset;\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __mn_iterator.close(__context__,(__mn_rename_at_93_63));
        __mni_iterator.close(__context__,(__mni_rename_at_93_64));
    };
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  }\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    {
        bool __need_loop_97 = true;
        // mn: tuple<name:string;index:int>&
        das_iterator<TArray<AutoTuple<char *,int32_t>>> __mn_iterator(__methods_rename_at_72_50);
        AutoTuple<char *,int32_t> * __mn_rename_at_97_67;
        __need_loop_97 = __mn_iterator.first(__context__,(__mn_rename_at_97_67)) && __need_loop_97;
        // mt: smart_ptr<ast::TypeDecl> aka TypeDeclPtr&
        das_iterator<TArray<smart_ptr_raw<TypeDecl>>> __mt_iterator(__types_rename_at_73_51);
        smart_ptr_raw<TypeDecl> * __mt_rename_at_97_68;
        __need_loop_97 = __mt_iterator.first(__context__,(__mt_rename_at_97_68)) && __need_loop_97;
        for ( ; __need_loop_97 ; __need_loop_97 = __mn_iterator.next(__context__,(__mn_rename_at_97_67)) && __mt_iterator.next(__context__,(__mt_rename_at_97_68)) )
        {
            builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_12, cast<char * const >::from(((char *) "  __forceinline Func get_")), cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_97_67))), cast<char * const >::from(((char *) " ( void * self ) const {\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__cpp_file_rename_at_66_47,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_13, cast<char * const >::from(((char *) "    return getDasClassMethod(self,_das_class_method_offset[__fn_")), cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_97_67))), cast<char * const >::from(((char *) "]);\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  }\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  __forceinline "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            log_cpp_class_method_307c561cdfa4a8db(__context__,__cpp_file_rename_at_66_47,das_string_builder(__context__,SimNode_AotInterop<2>(__tinfo_14, cast<char * const >::from(((char *) "invoke_")), cast<char *>::from(das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_97_67))))),(*__mt_rename_at_97_68));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) " const {\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "    "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            if ( !((das_deref(__context__,(*__mt_rename_at_97_68)->firstType /*firstType*/)).isVoid()) )
            {
                builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "return "),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
            log_cpp_class_method_call_811ba437c8a50b47(__context__,__cpp_file_rename_at_66_47,das_get_auto_tuple_field<char *,0,char *,int32_t>::get((*__mn_rename_at_97_67)),(*__mt_rename_at_97_68));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) ";\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "  }\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __mn_iterator.close(__context__,(__mn_rename_at_97_67));
        __mt_iterator.close(__context__,(__mt_rename_at_97_68));
    };
    builtin_fwrite(__cpp_file_rename_at_66_47,((char *) "};\n\n"),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9(__context__,das_arg<TArray<smart_ptr_raw<TypeDecl>>>::pass(__types_rename_at_73_51));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xa6586de04a0e7816] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_c29a2ce3ce04a328>>();
    };
    aotLib[0x36832c0122a65a24] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_615e0425516708d9>>();
    };
    aotLib[0xaef82688fd0c9c40] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribe_cppTick8220285150532477039_64c421c61ecd7262>>();
    };
    aotLib[0x28a367e82588d43d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_b3fa6014e447ff2d>>();
    };
    aotLib[0x65b03675437c90e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick15769051505004837089_338aab8aa242d2bb>>();
    };
    aotLib[0x73b9b7466a89eca0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&log_cpp_class_method_307c561cdfa4a8db>>();
    };
    aotLib[0xe890efab589dbb45] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&log_cpp_class_method_call_811ba437c8a50b47>>();
    };
    aotLib[0xbc45178c1c5cf58f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&log_cpp_class_adapter_92277e6cbae7f863>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_10965320641594931681
AotListBase impl_aot_cpp_bind(_anon_10965320641594931681::registerAotFunctions);
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
