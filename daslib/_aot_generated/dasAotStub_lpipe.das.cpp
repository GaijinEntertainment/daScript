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
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require lpipe

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
namespace _anon_8787716374767917637 {

namespace lpipe { struct LpipeMacro; };
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
// unused enumeration CompilationError
// unused enumeration ConstMatters
// unused enumeration RefMatters
// unused enumeration TemporaryMatters
// unused enumeration Type
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
namespace ast {

struct AstCallMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}
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
namespace lpipe {

struct LpipeMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCallMacro)) __finalize;
    Func DAS_COMMENT((void,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) preVisit;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstCallMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprCallMacro> const )) visit;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const ,int32_t)) canVisitArgument;
    Func DAS_COMMENT((bool,ast::AstCallMacro,smart_ptr_raw<ExprCallMacro> const )) canFoldReturnResult;
};
}
extern TypeInfo __type_info__6b1c7db4b71a781f;

TypeInfo __type_info__6b1c7db4b71a781f = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~$::das_string"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 258, TypeSize<das::string>::size, UINT64_C(0x6b1c7db4b71a781f) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__6b1c7db4b71a781f, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__6b1c7db4b71a781f };

inline void _FuncbuiltinTickpushTick10769833213962245646_d7dff826198d4fd2 ( Context * __context__, TArray<ast::AstCallMacro *> & __Arr_rename_at_181_0, ast::AstCallMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b0b2ab2e5d9e80a2 ( Context * __context__, lpipe::LpipeMacro const  & __cl_rename_at_116_2 );
inline smart_ptr_raw<Expression> lpipe_expr_164f00156c60e835 ( Context * __context__, smart_ptr_raw<Expression> & __fnCall_rename_at_15_3, smart_ptr_raw<Expression> & __arg_rename_at_15_4 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d7dff826198d4fd2 ( Context * __context__, TArray<ast::AstCallMacro *> &  __Arr_rename_at_181_0, ast::AstCallMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstCallMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_b0b2ab2e5d9e80a2 ( Context * __context__, lpipe::LpipeMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline smart_ptr_raw<Expression> lpipe_expr_164f00156c60e835 ( Context * __context__, smart_ptr_raw<Expression> & __fnCall_rename_at_15_3, smart_ptr_raw<Expression> & __arg_rename_at_15_4 )
{
    ExprVar * __pVar_rename_at_17_5; memset((void*)&__pVar_rename_at_17_5,0,sizeof(__pVar_rename_at_17_5));
    smart_ptr_raw<Expression> __pCall_rename_at_18_6; memset((void*)&__pCall_rename_at_18_6,0,sizeof(__pCall_rename_at_18_6));
    ExprLooksLikeCall * __pCallFunc_rename_at_20_7; memset((void*)&__pCallFunc_rename_at_20_7,0,sizeof(__pCallFunc_rename_at_20_7));
    smart_ptr_raw<Expression> __cCopy_rename_at_24_8; memset((void*)&__cCopy_rename_at_24_8,0,sizeof(__cCopy_rename_at_24_8));
    smart_ptr_raw<Expression> __cCopy_rename_at_28_9; memset((void*)&__cCopy_rename_at_28_9,0,sizeof(__cCopy_rename_at_28_9));
    smart_ptr_raw<Expression> __cCopy_rename_at_32_10; memset((void*)&__cCopy_rename_at_32_10,0,sizeof(__cCopy_rename_at_32_10));
    smart_ptr_raw<Expression> ___VarcLetTickatTick41Tick20_rename_at_41_11; memset((void*)&___VarcLetTickatTick41Tick20_rename_at_41_11,0,sizeof(___VarcLetTickatTick41Tick20_rename_at_41_11));
    ExprLet * __pLet_rename_at_36_12; memset((void*)&__pLet_rename_at_36_12,0,sizeof(__pLet_rename_at_36_12));
    if ( SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr) )
    {
        /* finally */ auto __finally_16= das_finally([&](){
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__pCall_rename_at_18_6);
        /* end finally */ });
        __pVar_rename_at_17_5 = (((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fnCall_rename_at_15_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprVar")),*__context__,nullptr))) ? das_auto_cast<ExprVar *>::cast(das_cast<ExprVar *>::cast(__fnCall_rename_at_15_3)) : das_auto_cast<ExprVar *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))));
        __pCall_rename_at_18_6; das_zero(__pCall_rename_at_18_6); das_move(__pCall_rename_at_18_6, makeCall(das_arg<LineInfo>::pass(__pVar_rename_at_17_5->at /*at*/),das_string_builder_temp(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<das::string &>::from(__pVar_rename_at_17_5->name /*name*/)))));
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__fnCall_rename_at_15_3);
        __pCallFunc_rename_at_20_7 = das_cast<ExprLooksLikeCall *>::cast(__pCall_rename_at_18_6);
        das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__pCallFunc_rename_at_20_7->arguments /*arguments*/),__arg_rename_at_15_4);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__pCall_rename_at_18_6);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCopy")),*__context__,nullptr) )
    {
        /* finally */ auto __finally_23= das_finally([&](){
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__cCopy_rename_at_24_8);
        /* end finally */ });
        __cCopy_rename_at_24_8; das_zero(__cCopy_rename_at_24_8); das_move(__cCopy_rename_at_24_8, clone_expression(__fnCall_rename_at_15_3));
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__cCopy_rename_at_24_8),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__cCopy_rename_at_24_8->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCopy")),*__context__,nullptr))) ? das_auto_cast<ExprCopy *>::cast(das_cast<ExprCopy *>::cast(__cCopy_rename_at_24_8)) : das_auto_cast<ExprCopy *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/),das_auto_cast<smart_ptr_raw<void> const >::cast(lpipe_expr_164f00156c60e835(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fnCall_rename_at_15_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprCopy")),*__context__,nullptr))) ? das_auto_cast<ExprCopy *>::cast(das_cast<ExprCopy *>::cast(__fnCall_rename_at_15_3)) : das_auto_cast<ExprCopy *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/,__arg_rename_at_15_4)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__cCopy_rename_at_24_8);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMove")),*__context__,nullptr) )
    {
        /* finally */ auto __finally_27= das_finally([&](){
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__cCopy_rename_at_28_9);
        /* end finally */ });
        __cCopy_rename_at_28_9; das_zero(__cCopy_rename_at_28_9); das_move(__cCopy_rename_at_28_9, clone_expression(__fnCall_rename_at_15_3));
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__cCopy_rename_at_28_9),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__cCopy_rename_at_28_9->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMove")),*__context__,nullptr))) ? das_auto_cast<ExprMove *>::cast(das_cast<ExprMove *>::cast(__cCopy_rename_at_28_9)) : das_auto_cast<ExprMove *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/),das_auto_cast<smart_ptr_raw<void> const >::cast(lpipe_expr_164f00156c60e835(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fnCall_rename_at_15_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprMove")),*__context__,nullptr))) ? das_auto_cast<ExprMove *>::cast(das_cast<ExprMove *>::cast(__fnCall_rename_at_15_3)) : das_auto_cast<ExprMove *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/,__arg_rename_at_15_4)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__cCopy_rename_at_28_9);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprClone")),*__context__,nullptr) )
    {
        /* finally */ auto __finally_31= das_finally([&](){
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__cCopy_rename_at_32_10);
        /* end finally */ });
        __cCopy_rename_at_32_10; das_zero(__cCopy_rename_at_32_10); das_move(__cCopy_rename_at_32_10, clone_expression(__fnCall_rename_at_15_3));
        builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__cCopy_rename_at_32_10),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__cCopy_rename_at_32_10->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprClone")),*__context__,nullptr))) ? das_auto_cast<ExprClone *>::cast(das_cast<ExprClone *>::cast(__cCopy_rename_at_32_10)) : das_auto_cast<ExprClone *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/),das_auto_cast<smart_ptr_raw<void> const >::cast(lpipe_expr_164f00156c60e835(__context__,(((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fnCall_rename_at_15_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprClone")),*__context__,nullptr))) ? das_auto_cast<ExprClone *>::cast(das_cast<ExprClone *>::cast(__fnCall_rename_at_15_3)) : das_auto_cast<ExprClone *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->right /*right*/,__arg_rename_at_15_4)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__cCopy_rename_at_32_10);
    } else if ( SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprLet")),*__context__,nullptr) )
    {
        /* finally */ auto __finally_35= das_finally([&](){
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,___VarcLetTickatTick41Tick20_rename_at_41_11);
        /* end finally */ });
        das_zero(___VarcLetTickatTick41Tick20_rename_at_41_11);
        __pLet_rename_at_36_12 = (((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(__fnCall_rename_at_15_3),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(__fnCall_rename_at_15_3->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprLet")),*__context__,nullptr))) ? das_auto_cast<ExprLet *>::cast(das_cast<ExprLet *>::cast(__fnCall_rename_at_15_3)) : das_auto_cast<ExprLet *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))));
        if ( (das_vector_length(das_arg<das::vector<smart_ptr<Variable>>>::pass(__pLet_rename_at_36_12->variables /*variables*/)) != 1) || (equ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(das_index<das::vector<smart_ptr<Variable>>>::at(__pLet_rename_at_36_12->variables /*variables*/,0,__context__)->init /*init*/),das_auto_cast<void * const >::cast(nullptr))) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__arg_rename_at_15_4);
            return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
        } else {
            das_move(___VarcLetTickatTick41Tick20_rename_at_41_11,clone_expression(__fnCall_rename_at_15_3));
            builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(das_index<das::vector<smart_ptr<Variable>>>::at((((nequ_sptr_ptr(das_auto_cast<smart_ptr_raw<void> const >::cast(___VarcLetTickatTick41Tick20_rename_at_41_11),das_auto_cast<void * const >::cast(nullptr))) && (SimPolicy<char *>::Equ(cast<char *>::from(___VarcLetTickatTick41Tick20_rename_at_41_11->__rtti /*__rtti*/),cast<char *>::from(((char *) "ExprLet")),*__context__,nullptr))) ? das_auto_cast<ExprLet *>::cast(das_cast<ExprLet *>::cast(___VarcLetTickatTick41Tick20_rename_at_41_11)) : das_auto_cast<ExprLet *>::cast(das_invoke_function<void *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::panic_expr_as*/ 0x299199196cf0e19)))))->variables /*variables*/,0,__context__)->init /*init*/),das_auto_cast<smart_ptr_raw<void> const >::cast(lpipe_expr_164f00156c60e835(__context__,das_index<das::vector<smart_ptr<Variable>>>::at(__pLet_rename_at_36_12->variables /*variables*/,0,__context__)->init /*init*/,__arg_rename_at_15_4)),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(___VarcLetTickatTick41Tick20_rename_at_41_11);
        };
    } else if ( isExprLikeCall(__fnCall_rename_at_15_3) )
    {
        ExprLooksLikeCall * __pCall_rename_at_45_13 = das_cast<ExprLooksLikeCall *>::cast(__fnCall_rename_at_15_3);
        if ( SimPolicy<char *>::Equ(cast<char *>::from(((char * const )(to_das_string(das_arg<das::string>::pass(__pCall_rename_at_45_13->name /*name*/),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),cast<char *>::from(((char *) "lpipe")),*__context__,nullptr) )
        {
            return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
        } else {
            das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__pCall_rename_at_45_13->arguments /*arguments*/),__arg_rename_at_15_4);
            return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__fnCall_rename_at_15_3);
        };
    } else {
        das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,__arg_rename_at_15_4);
        return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(nullptr);
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xbd07f1914ed1e4d4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_d7dff826198d4fd2>>();
    };
    aotLib[0xd8d328ecd322f48c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_b0b2ab2e5d9e80a2>>();
    };
    aotLib[0xb52dce1a3bf58f9f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&lpipe_expr_164f00156c60e835>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_8787716374767917637
AotListBase impl_aot_lpipe(_anon_8787716374767917637::registerAotFunctions);
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
