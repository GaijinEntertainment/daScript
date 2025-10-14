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
 // require constant_expression

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
namespace _anon_6813154944585389674 {

namespace constant_expression { struct ConstExprAnnotation; };
namespace constant_expression { struct ConstantExpressionMacro; };
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
namespace ast {

struct AstFunctionAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,das::string)) transform;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) verifyCall;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) generic_apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) fixup;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) lint;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>>,AnnotationDeclaration const ,das::string)) isCompatible;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation)) isSpecialized;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function> const ,AnnotationDeclaration const ,das::string)) appendToMangledName;
};
}
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
namespace constant_expression {

struct ConstExprAnnotation {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,das::string)) transform;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) verifyCall;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) generic_apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) fixup;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) lint;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>>,AnnotationDeclaration const ,das::string)) isCompatible;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation)) isSpecialized;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function> const ,AnnotationDeclaration const ,das::string)) appendToMangledName;
};
}
namespace constant_expression {

struct ConstantExpressionMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,das::string)) transform;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<ExprCallFunc>,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) verifyCall;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,das::string)) generic_apply;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) finish;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string,bool &)) patch;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) fixup;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,ModuleGroup,AnnotationArgumentList const ,AnnotationArgumentList const ,das::string)) lint;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,smart_ptr_raw<Context>)) complete;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation,smart_ptr_raw<Function>,das::vector<smart_ptr<TypeDecl>>,AnnotationDeclaration const ,das::string)) isCompatible;
    Func DAS_COMMENT((bool,ast::AstFunctionAnnotation)) isSpecialized;
    Func DAS_COMMENT((void,ast::AstFunctionAnnotation,smart_ptr_raw<Function> const ,AnnotationDeclaration const ,das::string)) appendToMangledName;
};
}
extern TypeInfo __type_info__5e85fe99b916c6d6;
extern TypeInfo __type_info__86e65058575787a;
extern TypeInfo __type_info__67987f1269190aa2;
extern TypeInfo __type_info__c4e7914a3746eb3e;
extern TypeInfo __type_info__af63e44c8601fa24;

TypeInfo __type_info__5e85fe99b916c6d6 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<int32_t>>::size, UINT64_C(0x5e85fe99b916c6d6) };
TypeInfo __type_info__86e65058575787a = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x86e65058575787a) };
TypeInfo __type_info__67987f1269190aa2 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__86e65058575787a, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<int32_t>>::size, UINT64_C(0x67987f1269190aa2) };
TypeInfo __type_info__c4e7914a3746eb3e = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af63e44c8601fa24, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<int32_t>>::size, UINT64_C(0xc4e7914a3746eb3e) };
TypeInfo __type_info__af63e44c8601fa24 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xaf63e44c8601fa24) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__5e85fe99b916c6d6 };
TypeInfo * __tinfo_1[1] = { &__type_info__67987f1269190aa2 };
TypeInfo * __tinfo_2[1] = { &__type_info__c4e7914a3746eb3e };

inline void _FuncbuiltinTickpushTick10769833213962245646_d40201b8a0017941 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2f1759b663acef7b ( Context * __context__, constant_expression::ConstExprAnnotation const  & __cl_rename_at_116_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f0e54ceb7c91edfb ( Context * __context__, constant_expression::ConstantExpressionMacro const  & __cl_rename_at_116_3 );
inline TArray<int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5d06fec2ba1a7e3c ( Context * __context__, TArray<int32_t> & __a_rename_at_50_4 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_1bc848e6e14d78ef ( Context * __context__, TArray<int32_t> & __a_rename_at_32_5, TArray<int32_t> & __b_rename_at_32_6 );
inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a51ec9ade0e1225 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_377_7, int32_t __value_rename_at_377_8 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_42c000f34e3fa57e ( Context * __context__, TArray<int32_t> & __a_rename_at_1234_9 );
inline void _FuncbuiltinTicksortTick14088969635007481283_c264b31e59010594 ( Context * __context__, TArray<int32_t> & __a_rename_at_1569_10 );
inline TArray<int32_t> _FuncalgorithmTickuniqueTick8642070526899511001_c75df25731c3b529 ( Context * __context__, TArray<int32_t> & __a_rename_at_11_11 );
inline void _FuncbuiltinTickswapTick6899974565646937647_79a81e9e5e7373a6 ( Context * __context__, int32_t & __a_rename_at_1832_18, int32_t & __b_rename_at_1832_19 );
inline void clone_58dc1e91d22f4801 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_104_21, void * const  __src_rename_at_104_22 );
inline void clone_d24eb40023a75a7c ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_105_23, void * const  __src_rename_at_105_24 );
inline char * _FuncastTickdescribeTick842554968825501494_52275b23cd1cc350 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_25 );
inline void _FuncbuiltinTickpushTick10769833213962245646_5763498f690fb163 ( Context * __context__, TArray<int32_t> & __Arr_rename_at_181_26, int32_t __value_rename_at_181_27 );
inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_992b195fd9122c6a ( Context * __context__, TArray<int32_t> & __a_rename_at_24_28 );
inline void _FuncalgorithmTickreverseTick3930920687139572544_22a7250d2eb6512d ( Context * __context__, TArray<int32_t> & __a_rename_at_37_30 );
inline bool _FuncbuiltinTickemptyTick15399874715904164783_9960cef1f859592e ( Context * __context__, TArray<int32_t> const  & __a_rename_at_585_35 );
inline smart_ptr_raw<Function> _FuncastTickclone_functionTick17084718524013848691_ae0e473d0383aa0a ( Context * __context__, Function * const  __fn_rename_at_58_36 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d40201b8a0017941 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_0, ast::AstFunctionAnnotation * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_2f1759b663acef7b ( Context * __context__, constant_expression::ConstExprAnnotation const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f0e54ceb7c91edfb ( Context * __context__, constant_expression::ConstantExpressionMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline TArray<int32_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5d06fec2ba1a7e3c ( Context * __context__, TArray<int32_t> &  __a_rename_at_50_4 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<int32_t> &>::from(__a_rename_at_50_4)));
    return das_auto_cast_ref<TArray<int32_t> &>::cast(__a_rename_at_50_4);
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_1bc848e6e14d78ef ( Context * __context__, TArray<int32_t> &  __a_rename_at_32_5, TArray<int32_t> &  __b_rename_at_32_6 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<int32_t> &>::from(__a_rename_at_32_5)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<int32_t> &>::from(__b_rename_at_32_6)));
    das_move(__a_rename_at_32_5,__b_rename_at_32_6);
}

inline void _FuncbuiltinTickpush_cloneTick2035469273396957942_a51ec9ade0e1225 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_377_7, int32_t __value_rename_at_377_8 )
{
    das_copy(__Arr_rename_at_377_7(builtin_array_push_back_zero(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_377_7),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_377_8);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_42c000f34e3fa57e ( Context * __context__, TArray<int32_t> &  __a_rename_at_1234_9 )
{
    builtin_array_free(das_arg<TArray<int32_t>>::pass(__a_rename_at_1234_9),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTicksortTick14088969635007481283_c264b31e59010594 ( Context * __context__, TArray<int32_t> &  __a_rename_at_1569_10 )
{
    if ( builtin_array_size(das_arg<TArray<int32_t>>::pass(__a_rename_at_1569_10)) <= 1 )
    {
        return ;
    } else {
        builtin_array_lock(das_arg<TArray<int32_t>>::pass(__a_rename_at_1569_10),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        builtin_sort<int32_t>(das_ref(__context__,__a_rename_at_1569_10(0,__context__)),builtin_array_size(das_arg<TArray<int32_t>>::pass(__a_rename_at_1569_10)));
        builtin_array_unlock(das_arg<TArray<int32_t>>::pass(__a_rename_at_1569_10),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline TArray<int32_t> _FuncalgorithmTickuniqueTick8642070526899511001_c75df25731c3b529 ( Context * __context__, TArray<int32_t> &  __a_rename_at_11_11 )
{
    int32_t __pidx_rename_at_13_12 = -1;
    TArray<int32_t> __b_rename_at_14_13;das_zero(__b_rename_at_14_13);
    {
        bool __need_loop_15 = true;
        // e: int aka TT&
        das_iterator<TArray<int32_t>> __e_iterator(__a_rename_at_11_11);
        int32_t * __e_rename_at_15_16;
        __need_loop_15 = __e_iterator.first(__context__,(__e_rename_at_15_16)) && __need_loop_15;
        // eidx: int const
        das_iterator<range> __eidx_iterator(mk_range(builtin_array_size(das_arg<TArray<int32_t>>::pass(__a_rename_at_11_11))));
        int32_t __eidx_rename_at_15_17;
        __need_loop_15 = __eidx_iterator.first(__context__,(__eidx_rename_at_15_17)) && __need_loop_15;
        for ( ; __need_loop_15 ; __need_loop_15 = __e_iterator.next(__context__,(__e_rename_at_15_16)) && __eidx_iterator.next(__context__,(__eidx_rename_at_15_17)) )
        {
            if ( (__pidx_rename_at_13_12 == -1) || (__a_rename_at_11_11(__pidx_rename_at_13_12,__context__) != (*__e_rename_at_15_16)) )
            {
                das_copy(__pidx_rename_at_13_12,__eidx_rename_at_15_17);
                _FuncbuiltinTickpush_cloneTick2035469273396957942_a51ec9ade0e1225(__context__,das_arg<TArray<int32_t>>::pass(__b_rename_at_14_13),(*__e_rename_at_15_16));
            };
        }
        __e_iterator.close(__context__,(__e_rename_at_15_16));
        __eidx_iterator.close(__context__,(__eidx_rename_at_15_17));
    };
    return /* <- */ das_auto_cast_move<TArray<int32_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5d06fec2ba1a7e3c(__context__,das_arg<TArray<int32_t>>::pass(__b_rename_at_14_13)));
}

inline void _FuncbuiltinTickswapTick6899974565646937647_79a81e9e5e7373a6 ( Context * __context__, int32_t & __a_rename_at_1832_18, int32_t & __b_rename_at_1832_19 )
{
    int32_t __t_rename_at_1834_20; das_zero(__t_rename_at_1834_20); das_move(__t_rename_at_1834_20, __a_rename_at_1832_18);
    das_move(__a_rename_at_1832_18,__b_rename_at_1832_19);
    das_move(__b_rename_at_1832_19,__t_rename_at_1834_20);
}

inline void clone_58dc1e91d22f4801 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_104_21, void * const  __src_rename_at_104_22 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_104_21),__src_rename_at_104_22,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_d24eb40023a75a7c ( Context * __context__, smart_ptr_raw<TypeDecl> & __dest_rename_at_105_23, void * const  __src_rename_at_105_24 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_105_23),__src_rename_at_105_24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline char * _FuncastTickdescribeTick842554968825501494_52275b23cd1cc350 ( Context * __context__, smart_ptr_raw<Expression> const  __expr_rename_at_48_25 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_expression(__expr_rename_at_48_25,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_5763498f690fb163 ( Context * __context__, TArray<int32_t> &  __Arr_rename_at_181_26, int32_t __value_rename_at_181_27 )
{
    das_copy(__Arr_rename_at_181_26(builtin_array_push_back(das_arg<TArray<int32_t>>::pass(__Arr_rename_at_181_26),4,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_27);
}

inline void _FuncalgorithmTicksort_uniqueTick14985796887508933377_992b195fd9122c6a ( Context * __context__, TArray<int32_t> &  __a_rename_at_24_28 )
{
    if ( _FuncbuiltinTickemptyTick15399874715904164783_9960cef1f859592e(__context__,das_arg<TArray<int32_t>>::pass(__a_rename_at_24_28)) )
    {
        return ;
    } else {
        _FuncbuiltinTicksortTick14088969635007481283_c264b31e59010594(__context__,das_arg<TArray<int32_t>>::pass(__a_rename_at_24_28));
        TArray<int32_t> __b_rename_at_32_29; das_zero(__b_rename_at_32_29); das_move(__b_rename_at_32_29, _FuncalgorithmTickuniqueTick8642070526899511001_c75df25731c3b529(__context__,das_arg<TArray<int32_t>>::pass(__a_rename_at_24_28)));
        _FuncbuiltinTickfinalizeTick13836114024949725080_42c000f34e3fa57e(__context__,das_arg<TArray<int32_t>>::pass(__a_rename_at_24_28));
        _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_1bc848e6e14d78ef(__context__,das_arg<TArray<int32_t>>::pass(__a_rename_at_24_28),das_arg<TArray<int32_t>>::pass(__b_rename_at_32_29));
    };
}

inline void _FuncalgorithmTickreverseTick3930920687139572544_22a7250d2eb6512d ( Context * __context__, TArray<int32_t> &  __a_rename_at_37_30 )
{
    int32_t __l_rename_at_39_31 = ((int32_t)builtin_array_size(das_arg<TArray<int32_t>>::pass(__a_rename_at_37_30)));
    int32_t __half_rename_at_40_32 = ((int32_t)(SimPolicy<int32_t>::Div(__l_rename_at_39_31,2,*__context__,nullptr)));
    int32_t __lm1_rename_at_41_33 = ((int32_t)(__l_rename_at_39_31 - 1));
    {
        bool __need_loop_42 = true;
        // i: int const
        das_iterator<range> __i_iterator(mk_range(__half_rename_at_40_32));
        int32_t __i_rename_at_42_34;
        __need_loop_42 = __i_iterator.first(__context__,(__i_rename_at_42_34)) && __need_loop_42;
        for ( ; __need_loop_42 ; __need_loop_42 = __i_iterator.next(__context__,(__i_rename_at_42_34)) )
        {
            _FuncbuiltinTickswapTick6899974565646937647_79a81e9e5e7373a6(__context__,__a_rename_at_37_30(__i_rename_at_42_34,__context__),__a_rename_at_37_30((__lm1_rename_at_41_33 - __i_rename_at_42_34),__context__));
        }
        __i_iterator.close(__context__,(__i_rename_at_42_34));
    };
}

inline bool _FuncbuiltinTickemptyTick15399874715904164783_9960cef1f859592e ( Context * __context__, TArray<int32_t> const  &  __a_rename_at_585_35 )
{
    return das_auto_cast<bool>::cast(builtin_array_size(__a_rename_at_585_35) == 0);
}

inline smart_ptr_raw<Function> _FuncastTickclone_functionTick17084718524013848691_ae0e473d0383aa0a ( Context * __context__, Function * const  __fn_rename_at_58_36 )
{
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Function>>::cast(clone_function(das_cast<smart_ptr_raw<Function>>::cast(__fn_rename_at_58_36)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xf78433a6ab7a13b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_d40201b8a0017941>>();
    };
    aotLib[0x59e8fbe5ef19b89b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_2f1759b663acef7b>>();
    };
    aotLib[0x65689d184a47aa6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f0e54ceb7c91edfb>>();
    };
    aotLib[0xf4b1e69f12259c4c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5d06fec2ba1a7e3c>>();
    };
    aotLib[0xa06bdff24d41e786] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_1bc848e6e14d78ef>>();
    };
    aotLib[0x94fa6238946010a4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpush_cloneTick2035469273396957942_a51ec9ade0e1225>>();
    };
    aotLib[0x7e952db3ca7a3f94] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_42c000f34e3fa57e>>();
    };
    aotLib[0x73ed81399bec0e30] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicksortTick14088969635007481283_c264b31e59010594>>();
    };
    aotLib[0x41191182c2f23e4e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncalgorithmTickuniqueTick8642070526899511001_c75df25731c3b529>>();
    };
    aotLib[0xb849d8852e0f2a19] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickswapTick6899974565646937647_79a81e9e5e7373a6>>();
    };
    aotLib[0xe30cf4bc860ff7af] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_58dc1e91d22f4801>>();
    };
    aotLib[0xa8b94ec9fcdd6ab3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_d24eb40023a75a7c>>();
    };
    aotLib[0x21112ab2975ed6f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick842554968825501494_52275b23cd1cc350>>();
    };
    aotLib[0xa025313e9e4145c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_5763498f690fb163>>();
    };
    aotLib[0x1f780cf07391f969] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncalgorithmTicksort_uniqueTick14985796887508933377_992b195fd9122c6a>>();
    };
    aotLib[0x39a9d01741def597] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncalgorithmTickreverseTick3930920687139572544_22a7250d2eb6512d>>();
    };
    aotLib[0x664746804a6cdc1d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickemptyTick15399874715904164783_9960cef1f859592e>>();
    };
    aotLib[0xa8da7eb32fab6a98] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickclone_functionTick17084718524013848691_ae0e473d0383aa0a>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_6813154944585389674
AotListBase impl_aot_constant_expression(_anon_6813154944585389674::registerAotFunctions);
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
