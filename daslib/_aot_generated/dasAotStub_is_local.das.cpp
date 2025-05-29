#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require ast
#include "daScript/ast/ast.h"
#include "daScript/simulate/aot_builtin_ast.h"
#include "daScript/ast/ast_generate.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
 // require strings_boost
 // require ast_boost
 // require is_local

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
namespace _anon_6451985058328174572 {

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
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
#if 0 // external enum
namespace rtti {

enum class Type : int32_t {
    none = int32_t(INT64_C(0)),
    autoinfer = int32_t(INT64_C(1)),
    alias = int32_t(INT64_C(2)),
    option = int32_t(INT64_C(3)),
    typeDecl = int32_t(INT64_C(4)),
    typeMacro = int32_t(INT64_C(5)),
    fakeContext = int32_t(INT64_C(6)),
    fakeLineInfo = int32_t(INT64_C(7)),
    anyArgument = int32_t(INT64_C(8)),
    tVoid = int32_t(INT64_C(9)),
    tBool = int32_t(INT64_C(10)),
    tInt8 = int32_t(INT64_C(11)),
    tUInt8 = int32_t(INT64_C(12)),
    tInt16 = int32_t(INT64_C(13)),
    tUInt16 = int32_t(INT64_C(14)),
    tInt64 = int32_t(INT64_C(15)),
    tUInt64 = int32_t(INT64_C(16)),
    tInt = int32_t(INT64_C(17)),
    tInt2 = int32_t(INT64_C(18)),
    tInt3 = int32_t(INT64_C(19)),
    tInt4 = int32_t(INT64_C(20)),
    tUInt = int32_t(INT64_C(21)),
    tUInt2 = int32_t(INT64_C(22)),
    tUInt3 = int32_t(INT64_C(23)),
    tUInt4 = int32_t(INT64_C(24)),
    tFloat = int32_t(INT64_C(25)),
    tFloat2 = int32_t(INT64_C(26)),
    tFloat3 = int32_t(INT64_C(27)),
    tFloat4 = int32_t(INT64_C(28)),
    tDouble = int32_t(INT64_C(29)),
    tRange = int32_t(INT64_C(30)),
    tURange = int32_t(INT64_C(31)),
    tRange64 = int32_t(INT64_C(32)),
    tURange64 = int32_t(INT64_C(33)),
    tString = int32_t(INT64_C(34)),
    tStructure = int32_t(INT64_C(35)),
    tHandle = int32_t(INT64_C(36)),
    tEnumeration = int32_t(INT64_C(37)),
    tEnumeration8 = int32_t(INT64_C(38)),
    tEnumeration16 = int32_t(INT64_C(39)),
    tEnumeration64 = int32_t(INT64_C(40)),
    tBitfield = int32_t(INT64_C(41)),
    tPointer = int32_t(INT64_C(42)),
    tFunction = int32_t(INT64_C(43)),
    tLambda = int32_t(INT64_C(44)),
    tIterator = int32_t(INT64_C(45)),
    tArray = int32_t(INT64_C(46)),
    tTable = int32_t(INT64_C(47)),
    tBlock = int32_t(INT64_C(48)),
    tTuple = int32_t(INT64_C(49)),
    tVariant = int32_t(INT64_C(50)),
};
}
#endif // external enum
// unused enumeration ConversionResult
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
extern TypeInfo __type_info__af90fe4c864e9d52;

TypeInfo __type_info__3c61146b2bdfb90 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 290, 32, UINT64_C(0x3c61146b2bdfb90) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__3c61146b2bdfb90, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[5] = { &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52, &__type_info__3c61146b2bdfb90, &__type_info__af90fe4c864e9d52 };

inline bool is_temp_safe_b0f7cb3017427de3 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_14_0 );
inline bool is_shared_expr_ac3976ba23032451 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_37_3 );
inline bool is_local_expr_25fd8383ba183774 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_72_8 );
inline bool is_local_or_global_expr_c8e623604fd9000 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_92_11 );
inline bool is_scope_expr_6a03a2370c1995a ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_114_16 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline bool is_temp_safe_b0f7cb3017427de3 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_14_0 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(das_get_bitfield((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_14_0),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr))) ? das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__expr_rename_at_14_0)) : das_auto_cast<ExprVar *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->varFlags /*varFlags*/,1u << 0));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr) )
    {
        ExprAt * __ea_rename_at_20_1 = ((ExprAt *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_14_0),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr))) ? das_auto_cast<ExprAt *>::cast(das_cast<ExprAt *>::cast(__expr_rename_at_14_0)) : das_auto_cast<ExprAt *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ea_rename_at_20_1->subexpr /*subexpr*/->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) && (das_vector_length(__ea_rename_at_20_1->subexpr /*subexpr*/->type /*_type*/->dim /*dim*/) != 0) )
        {
            return das_auto_cast<bool>::cast(is_temp_safe_b0f7cb3017427de3(__context__,__ea_rename_at_20_1->subexpr /*subexpr*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr) )
    {
        ExprField * __ef_rename_at_25_2 = ((ExprField *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_14_0),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr))) ? das_auto_cast<ExprField *>::cast(das_cast<ExprField *>::cast(__expr_rename_at_14_0)) : das_auto_cast<ExprField *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( !((__ef_rename_at_25_2->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle)) || ((das_deref(__context__,__ef_rename_at_25_2->value /*value*/->type /*_type*/)).isLocal()) )
        {
            return das_auto_cast<bool>::cast(is_temp_safe_b0f7cb3017427de3(__context__,__ef_rename_at_25_2->value /*value*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(is_temp_safe_b0f7cb3017427de3(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_14_0),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr))) ? das_auto_cast<ExprSwizzle *>::cast(das_cast<ExprSwizzle *>::cast(__expr_rename_at_14_0)) : das_auto_cast<ExprSwizzle *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->value /*value*/));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_14_0->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(((das_deref(__context__,__expr_rename_at_14_0->type /*_type*/)).isRef()));
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_shared_expr_ac3976ba23032451 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_37_3 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        ExprVar * __evar_rename_at_40_4 = ((ExprVar *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_37_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr))) ? das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__expr_rename_at_37_3)) : das_auto_cast<ExprVar *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        return das_auto_cast<bool>::cast(das_get_bitfield(__evar_rename_at_40_4->variable /*variable*/->flags /*flags*/,1u << 5) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__evar_rename_at_40_4->variable /*variable*/->source /*source*/),das_auto_cast<void * const >::cast(nullptr))) ? das_auto_cast<bool>::cast(is_shared_expr_ac3976ba23032451(__context__,__evar_rename_at_40_4->variable /*variable*/->source /*source*/)) : das_auto_cast<bool>::cast(false))));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr) )
    {
        ExprCall * __ecall_rename_at_49_5 = ((ExprCall *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_37_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCall")),*__context__,nullptr))) ? das_auto_cast<ExprCall *>::cast(das_cast<ExprCall *>::cast(__expr_rename_at_37_3)) : das_auto_cast<ExprCall *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        builtin_print(das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_0, cast<char * const >::from(((char *) "ecall `")), cast<das::string const  &>::from(__ecall_rename_at_49_5->func /*func*/->name /*name*/), cast<char * const >::from(((char *) "` `")), cast<das::string const  &>::from(__ecall_rename_at_49_5->func /*func*/->module /*_module*/->name /*name*/), cast<char * const >::from(((char *) "`\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return das_auto_cast<bool>::cast((((__ecall_rename_at_49_5->func /*func*/ != nullptr) && (__ecall_rename_at_49_5->func /*func*/->module /*_module*/ != nullptr)) && (builtin_string_startswith(((char * const )(to_das_string(__ecall_rename_at_49_5->func /*func*/->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "builtin`keys`"),__context__) || builtin_string_startswith(((char * const )(to_das_string(__ecall_rename_at_49_5->func /*func*/->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))),((char *) "builtin`values`"),__context__))) ? das_auto_cast<bool>::cast(is_shared_expr_ac3976ba23032451(__context__,das_index<das::vector<smart_ptr<Expression>> const >::at(__ecall_rename_at_49_5->arguments /*arguments*/,0,__context__))) : das_auto_cast<bool>::cast(false));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr) )
    {
        ExprAt * __ea_rename_at_57_6 = ((ExprAt *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_37_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr))) ? das_auto_cast<ExprAt *>::cast(das_cast<ExprAt *>::cast(__expr_rename_at_37_3)) : das_auto_cast<ExprAt *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ea_rename_at_57_6->subexpr /*subexpr*/->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) && ((__ea_rename_at_57_6->subexpr /*subexpr*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tArray) || ((das_deref(__context__,__ea_rename_at_57_6->subexpr /*subexpr*/->type /*_type*/)).isArray())) )
        {
            return das_auto_cast<bool>::cast(is_shared_expr_ac3976ba23032451(__context__,__ea_rename_at_57_6->subexpr /*subexpr*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr) )
    {
        ExprField * __ef_rename_at_62_7 = ((ExprField *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_37_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr))) ? das_auto_cast<ExprField *>::cast(das_cast<ExprField *>::cast(__expr_rename_at_37_3)) : das_auto_cast<ExprField *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( !((__ef_rename_at_62_7->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle)) || ((das_deref(__context__,__ef_rename_at_62_7->value /*value*/->type /*_type*/)).isLocal()) )
        {
            return das_auto_cast<bool>::cast(is_shared_expr_ac3976ba23032451(__context__,__ef_rename_at_62_7->value /*value*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(is_shared_expr_ac3976ba23032451(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_37_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_37_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr))) ? das_auto_cast<ExprSwizzle *>::cast(das_cast<ExprSwizzle *>::cast(__expr_rename_at_37_3)) : das_auto_cast<ExprSwizzle *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->value /*value*/));
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_local_expr_25fd8383ba183774 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_72_8 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(das_get_bitfield((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_72_8),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr))) ? das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__expr_rename_at_72_8)) : das_auto_cast<ExprVar *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->varFlags /*varFlags*/,1u << 0));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr) )
    {
        ExprAt * __ea_rename_at_77_9 = ((ExprAt *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_72_8),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr))) ? das_auto_cast<ExprAt *>::cast(das_cast<ExprAt *>::cast(__expr_rename_at_72_8)) : das_auto_cast<ExprAt *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ea_rename_at_77_9->subexpr /*subexpr*/->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) && (das_vector_length(__ea_rename_at_77_9->subexpr /*subexpr*/->type /*_type*/->dim /*dim*/) != 0) )
        {
            return das_auto_cast<bool>::cast(is_local_expr_25fd8383ba183774(__context__,__ea_rename_at_77_9->subexpr /*subexpr*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr) )
    {
        ExprField * __ef_rename_at_82_10 = ((ExprField *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_72_8),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr))) ? das_auto_cast<ExprField *>::cast(das_cast<ExprField *>::cast(__expr_rename_at_72_8)) : das_auto_cast<ExprField *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( !((__ef_rename_at_82_10->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle)) || ((das_deref(__context__,__ef_rename_at_82_10->value /*value*/->type /*_type*/)).isLocal()) )
        {
            return das_auto_cast<bool>::cast(is_local_expr_25fd8383ba183774(__context__,__ef_rename_at_82_10->value /*value*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(is_local_expr_25fd8383ba183774(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_72_8),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_72_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr))) ? das_auto_cast<ExprSwizzle *>::cast(das_cast<ExprSwizzle *>::cast(__expr_rename_at_72_8)) : das_auto_cast<ExprSwizzle *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->value /*value*/));
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_local_or_global_expr_c8e623604fd9000 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_92_11 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        ExprVar * __ev_rename_at_95_12 = ((ExprVar *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_92_11),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr))) ? das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__expr_rename_at_92_11)) : das_auto_cast<ExprVar *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        return das_auto_cast<bool>::cast(das_get_bitfield(__ev_rename_at_95_12->varFlags /*varFlags*/,1u << 0) || !((das_get_bitfield(__ev_rename_at_95_12->varFlags /*varFlags*/,1u << 1) || das_get_bitfield(__ev_rename_at_95_12->varFlags /*varFlags*/,1u << 2))));
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr) )
    {
        ExprAt * __ea_rename_at_98_13 = ((ExprAt *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_92_11),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr))) ? das_auto_cast<ExprAt *>::cast(das_cast<ExprAt *>::cast(__expr_rename_at_92_11)) : das_auto_cast<ExprAt *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ea_rename_at_98_13->subexpr /*subexpr*/->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) && (das_vector_length(__ea_rename_at_98_13->subexpr /*subexpr*/->type /*_type*/->dim /*dim*/) != 0) )
        {
            return das_auto_cast<bool>::cast(is_local_or_global_expr_c8e623604fd9000(__context__,__ea_rename_at_98_13->subexpr /*subexpr*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr) )
    {
        ExprField * __ef_rename_at_103_14 = ((ExprField *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_92_11),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr))) ? das_auto_cast<ExprField *>::cast(das_cast<ExprField *>::cast(__expr_rename_at_92_11)) : das_auto_cast<ExprField *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( !((__ef_rename_at_103_14->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle)) || ((das_deref(__context__,__ef_rename_at_103_14->value /*value*/->type /*_type*/)).isLocal()) )
        {
            return das_auto_cast<bool>::cast(is_local_or_global_expr_c8e623604fd9000(__context__,__ef_rename_at_103_14->value /*value*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr) )
    {
        ExprSwizzle * __ef_rename_at_108_15 = ((ExprSwizzle *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_92_11),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_92_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr))) ? das_auto_cast<ExprSwizzle *>::cast(das_cast<ExprSwizzle *>::cast(__expr_rename_at_92_11)) : das_auto_cast<ExprSwizzle *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        return das_auto_cast<bool>::cast(is_local_or_global_expr_c8e623604fd9000(__context__,__ef_rename_at_108_15->value /*value*/));
    };
    return das_auto_cast<bool>::cast(false);
}

inline bool is_scope_expr_6a03a2370c1995a ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_114_16 )
{
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        return das_auto_cast<bool>::cast(true);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr) )
    {
        ExprAt * __ea_rename_at_119_17 = ((ExprAt *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_114_16),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprAt")),*__context__,nullptr))) ? das_auto_cast<ExprAt *>::cast(das_cast<ExprAt *>::cast(__expr_rename_at_114_16)) : das_auto_cast<ExprAt *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( (nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__ea_rename_at_119_17->subexpr /*subexpr*/->type /*_type*/),das_auto_cast<void * const >::cast(nullptr))) && (das_vector_length(__ea_rename_at_119_17->subexpr /*subexpr*/->type /*_type*/->dim /*dim*/) != 0) )
        {
            return das_auto_cast<bool>::cast(is_scope_expr_6a03a2370c1995a(__context__,__ea_rename_at_119_17->subexpr /*subexpr*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr) )
    {
        ExprField * __ef_rename_at_124_18 = ((ExprField *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_114_16),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprField")),*__context__,nullptr))) ? das_auto_cast<ExprField *>::cast(das_cast<ExprField *>::cast(__expr_rename_at_114_16)) : das_auto_cast<ExprField *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        if ( !((__ef_rename_at_124_18->value /*value*/->type /*_type*/->baseType /*baseType*/ == DAS_COMMENT(bound_enum) das::Type::tHandle)) || ((das_deref(__context__,__ef_rename_at_124_18->value /*value*/->type /*_type*/)).isLocal()) )
        {
            return das_auto_cast<bool>::cast(is_scope_expr_6a03a2370c1995a(__context__,__ef_rename_at_124_18->value /*value*/));
        };
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr) )
    {
        ExprSwizzle * __ef_rename_at_129_19 = ((ExprSwizzle *)(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__expr_rename_at_114_16),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__expr_rename_at_114_16->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprSwizzle")),*__context__,nullptr))) ? das_auto_cast<ExprSwizzle *>::cast(das_cast<ExprSwizzle *>::cast(__expr_rename_at_114_16)) : das_auto_cast<ExprSwizzle *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19))))));
        return das_auto_cast<bool>::cast(is_scope_expr_6a03a2370c1995a(__context__,__ef_rename_at_129_19->value /*value*/));
    };
    return das_auto_cast<bool>::cast(false);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x4224116f65267573] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_temp_safe_b0f7cb3017427de3>>();
    };
    aotLib[0x69fa66a3b34f5c55] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_shared_expr_ac3976ba23032451>>();
    };
    aotLib[0x4bc354b59a650f6a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_local_expr_25fd8383ba183774>>();
    };
    aotLib[0xeb5eec3cff58022] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_local_or_global_expr_c8e623604fd9000>>();
    };
    aotLib[0x1ea8ec7e7de45378] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_scope_expr_6a03a2370c1995a>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_6451985058328174572
AotListBase impl_aot_is_local(_anon_6451985058328174572::registerAotFunctions);
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
