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
 // require static_let

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
namespace _anon_4288214345843065048 {

namespace static_let { struct StaticLetMacro; };
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
namespace static_let {

struct StaticLetMacro {
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
extern TypeInfo __type_info__af90fe4c864e9d52;

TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[2] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void finalize_aada8fc8749ebe98 ( Context * __context__, AutoTuple<char *,char *> & ____this_rename_at_1238_0 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e865d5b324eb2c9d ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_1, ast::AstFunctionAnnotation * __value_rename_at_181_2 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e3218278b077e791 ( Context * __context__, static_let::StaticLetMacro const  & __cl_rename_at_116_3 );
inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_250fb88a42bc64c1 ( Context * __context__, char * const  __name_rename_at_631_4, static_let::StaticLetMacro * __someClassPtr_rename_at_631_5 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_54aa55563e419d3f ( Context * __context__, TArray<AutoTuple<char *,char *>> & __a_rename_at_1234_7 );
inline void _FuncbuiltinTickpushTick10769833213962245646_1c0d62e5328f10ed ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_181_9, AutoTuple<char *,char *> & __value_rename_at_181_10 );
inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_ef2768dad1bf24c1 ( Context * __context__, char * const  __name_rename_at_240_11, char * const  __tag_rename_at_240_12, static_let::StaticLetMacro * __classPtr_rename_at_240_13 );
inline void static_let_7d74e9ce705e7092 ( Context * __context__, Block DAS_COMMENT((void)) const  & __blk_rename_at_15_15 );
inline void static_let_finalize_6a0be0cd6921cad1 ( Context * __context__, Block DAS_COMMENT((void)) const  & __blk_rename_at_22_16 );
inline bool make_shutdown_function_42ca262d3cc1da03 ( Context * __context__, char * const  __varname_rename_at_27_17, LineInfo const  & __at_rename_at_27_18 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void finalize_aada8fc8749ebe98 ( Context * __context__, AutoTuple<char *,char *> &  ____this_rename_at_1238_0 )
{
    memset((void*)&(____this_rename_at_1238_0), 0, TypeSize<AutoTuple<char *,char *>>::size);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e865d5b324eb2c9d ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_1, ast::AstFunctionAnnotation * __value_rename_at_181_2 )
{
    das_copy(__Arr_rename_at_181_1(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_2);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_e3218278b077e791 ( Context * __context__, static_let::StaticLetMacro const  &  __cl_rename_at_116_3 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_3.__rtti))).getStructType())));
}

inline smart_ptr_raw<FunctionAnnotation> _FuncastTickmake_function_annotationTick3074191368936885601_250fb88a42bc64c1 ( Context * __context__, char * const  __name_rename_at_631_4, static_let::StaticLetMacro * __someClassPtr_rename_at_631_5 )
{
    _FuncbuiltinTickpushTick10769833213962245646_e865d5b324eb2c9d(__context__,das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(das_global<TArray<ast::AstFunctionAnnotation *>,0x70a17c19e372bb43>(__context__) /*gc_root_AstFunctionAnnotation*/),das_reinterpret<ast::AstFunctionAnnotation *>::pass(__someClassPtr_rename_at_631_5));
    StructInfo const  * __classInfo_rename_at_634_6 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_e3218278b077e791(__context__,das_arg<static_let::StaticLetMacro>::pass(das_deref(__context__,__someClassPtr_rename_at_631_5))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<FunctionAnnotation>>::cast(makeFunctionAnnotation(__name_rename_at_631_4,das_auto_cast<void * const >::cast(__someClassPtr_rename_at_631_5),__classInfo_rename_at_634_6,__context__));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_54aa55563e419d3f ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __a_rename_at_1234_7 )
{
    {
        bool __need_loop_1236 = true;
        // aV: tuple<old_name:string;new_name:string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *>>> __aV_iterator(__a_rename_at_1234_7);
        AutoTuple<char *,char *> * __aV_rename_at_1236_8;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_8)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_8)) )
        {
            finalize_aada8fc8749ebe98(__context__,das_arg<AutoTuple<char *,char *>>::pass((*__aV_rename_at_1236_8)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_8));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__a_rename_at_1234_7),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_1c0d62e5328f10ed ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_181_9, AutoTuple<char *,char *> &  __value_rename_at_181_10 )
{
    das_copy(__Arr_rename_at_181_9(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_181_9),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_10);
}

inline void _Funcast_boostTicksetup_tag_annotationTick4095297895764883997_ef2768dad1bf24c1 ( Context * __context__, char * const  __name_rename_at_240_11, char * const  __tag_rename_at_240_12, static_let::StaticLetMacro * __classPtr_rename_at_240_13 )
{
    smart_ptr_raw<FunctionAnnotation> __ann_rename_at_241_14; memset((void*)&__ann_rename_at_241_14,0,sizeof(__ann_rename_at_241_14));
    /* finally */ auto __finally_240= das_finally([&](){
    das_delete_handle<smart_ptr_raw<FunctionAnnotation>>::clear(__context__,__ann_rename_at_241_14);
    /* end finally */ });
    __ann_rename_at_241_14; das_zero(__ann_rename_at_241_14); das_move(__ann_rename_at_241_14, _FuncastTickmake_function_annotationTick3074191368936885601_250fb88a42bc64c1(__context__,__name_rename_at_240_11,__classPtr_rename_at_240_13));
    das_invoke_function<void>::invoke<char * const ,smart_ptr_raw<FunctionAnnotation>>(__context__,nullptr,Func(__context__->fnByMangledName(/*@ast_boost::apply_tag_annotation Cs Y<FunctionAnnotationPtr>1<H<ast::FunctionAnnotation>>?M*/ 0x7678364e276db9c2)),__tag_rename_at_240_12,__ann_rename_at_241_14);
    addModuleFunctionAnnotation(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_241_14,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void static_let_7d74e9ce705e7092 ( Context * __context__, Block DAS_COMMENT((void)) const  &  __blk_rename_at_15_15 )
{
}

inline void static_let_finalize_6a0be0cd6921cad1 ( Context * __context__, Block DAS_COMMENT((void)) const  &  __blk_rename_at_22_16 )
{
}

inline bool make_shutdown_function_42ca262d3cc1da03 ( Context * __context__, char * const  __varname_rename_at_27_17, LineInfo const  &  __at_rename_at_27_18 ) { das_stack_prologue __prologue(__context__,288,"make_shutdown_function " DAS_FILE_LINE);
{
    smart_ptr_raw<Function> __fn_rename_at_28_19; memset((void*)&__fn_rename_at_28_19,0,sizeof(__fn_rename_at_28_19));
    smart_ptr_raw<ExprBlock> __blk_rename_at_32_21; memset((void*)&__blk_rename_at_32_21,0,sizeof(__blk_rename_at_32_21));
    smart_ptr_raw<ExprDelete> __delv_rename_at_33_22; memset((void*)&__delv_rename_at_33_22,0,sizeof(__delv_rename_at_33_22));
    /* finally */ auto __finally_27= das_finally([&](){
    das_delete_handle<smart_ptr_raw<ExprDelete>>::clear(__context__,__delv_rename_at_33_22);
    das_delete_handle<smart_ptr_raw<ExprBlock>>::clear(__context__,__blk_rename_at_32_21);
    das_delete_handle<smart_ptr_raw<Function>>::clear(__context__,__fn_rename_at_28_19);
    /* end finally */ });
    __fn_rename_at_28_19; das_zero(__fn_rename_at_28_19); das_move(__fn_rename_at_28_19, das_ascend_handle<true,smart_ptr_raw<Function>>::make(__context__,nullptr,(([&](Function & __mks_28) {
        das_copy((__mks_28.at /*at*/),(__at_rename_at_27_18));
        das_copy((__mks_28.atDecl /*atDecl*/),(__at_rename_at_27_18));
        {
            set_das_string(das_arg<das::string>::pass(__mks_28.name /*name*/),das_string_builder_temp(__context__,SimNode_AotInterop<2>(__tinfo_0, cast<char * const >::from(((char *) "shutdown`")), cast<char * const >::from(__varname_rename_at_27_17))));
        }    }))));
    __fn_rename_at_28_19->flags /*flags*/ |= 0x200000u;
    __fn_rename_at_28_19->flags /*flags*/ |= 0x20000000u;
    builtin_smart_ptr_move_new(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_28_19->result /*result*/),das_auto_cast<smart_ptr_raw<void> const >::cast(das_ascend_handle<true,smart_ptr_raw<TypeDecl>>::make(__context__,nullptr,(([&](TypeDecl & __mks_31) {
        das_copy((__mks_31.baseType /*baseType*/),(DAS_COMMENT(bound_enum) das::Type::tVoid));
        das_copy((__mks_31.at /*at*/),(__at_rename_at_27_18));
    })))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    __blk_rename_at_32_21; das_zero(__blk_rename_at_32_21); das_move(__blk_rename_at_32_21, das_ascend_handle<true,smart_ptr_raw<ExprBlock>>::make(__context__,nullptr,(([&](ExprBlock & __mks_32) {
        das_copy((__mks_32.at /*at*/),(__at_rename_at_27_18));
    }))));
    __delv_rename_at_33_22; das_zero(__delv_rename_at_33_22); das_move(__delv_rename_at_33_22, das_ascend_handle<true,smart_ptr_raw<ExprDelete>>::make(__context__,nullptr,(([&](ExprDelete & __mks_33) {
        das_copy((__mks_33.at /*at*/),(__at_rename_at_27_18));
        das_move((__mks_33.subexpr /*subexpr*/),(das_ascend_handle<true,smart_ptr_raw<ExprVar>>::make(__context__,nullptr,(([&](ExprVar & __mks_34) {
            das_copy((__mks_34.at /*at*/),(__at_rename_at_27_18));
            {
                set_das_string(das_arg<das::string>::pass(__mks_34.name /*name*/),__varname_rename_at_27_17);
            }        })))));
    }))));
    __delv_rename_at_33_22->genFlags /*genFlags*/ |= 0x1u;
    das_vector_emplace_back(das_arg<das::vector<smart_ptr<Expression>>>::pass(__blk_rename_at_32_21->list /*list*/),das_reinterpret<smart_ptr_raw<Expression>>::pass(__delv_rename_at_33_22));
    builtin_smart_ptr_move(das_auto_cast<smart_ptr_raw<void> &>::cast(__fn_rename_at_28_19->body /*body*/),das_auto_cast<smart_ptr_raw<void> &>::cast(__blk_rename_at_32_21),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return das_auto_cast<bool>::cast(addModuleFunction(compileModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__fn_rename_at_28_19,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
}}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x37265a4d2fe09d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_aada8fc8749ebe98>>();
    };
    aotLib[0x28b114ce682bff62] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e865d5b324eb2c9d>>();
    };
    aotLib[0x615979fc506e7e53] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_e3218278b077e791>>();
    };
    aotLib[0x17c3d48eb893cb74] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_function_annotationTick3074191368936885601_250fb88a42bc64c1>>();
    };
    aotLib[0x38e84f9cf5aa0981] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_54aa55563e419d3f>>();
    };
    aotLib[0xa84ccc7a896cddee] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_1c0d62e5328f10ed>>();
    };
    aotLib[0xf7b8f714f5a9862c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTicksetup_tag_annotationTick4095297895764883997_ef2768dad1bf24c1>>();
    };
    aotLib[0xbe4de82e5e4f086c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&static_let_7d74e9ce705e7092>>();
    };
    aotLib[0xd52d0ceb3baac388] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&static_let_finalize_6a0be0cd6921cad1>>();
    };
    aotLib[0x226de040e5d9087e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_shutdown_function_42ca262d3cc1da03>>();
    };
    // [[ init script ]]
    aotLib[0x5464bbe131808aa5] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_4288214345843065048
AotListBase impl_aot_static_let(_anon_4288214345843065048::registerAotFunctions);
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
