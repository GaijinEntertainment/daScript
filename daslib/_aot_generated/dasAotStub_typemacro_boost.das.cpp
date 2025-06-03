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
 // require typemacro_boost

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
namespace _anon_9638349545935827793 {

namespace typemacro_boost { struct TypeMacroMacro; };
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
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
namespace typemacro_boost {

struct TypeMacroMacro {
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


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void _FuncbuiltinTickresizeTick4811697762258667383_ba6198643c09cf0e ( Context * __context__, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_2, smart_ptr_raw<Expression> const  __src_rename_at_1059_3 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_a870648d308a6130 ( Context * __context__, TDim<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>,1> const  & __a_rename_at_581_4 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_5, ast::AstFunctionAnnotation * __value_rename_at_181_6 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f6d476408197914e ( Context * __context__, typemacro_boost::TypeMacroMacro const  & __cl_rename_at_116_7 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_8e43a49b5b6891e2 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_8 );
inline TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> _FuncbuiltinTickto_array_moveTick3185538323411982277_77313f057747cf52 ( Context * __context__, TDim<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>,1> & __a_rename_at_1342_18 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_c545e308a61d940f ( Context * __context__, int32_t __value_rename_at_882_20 );
inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_48a5beb36940388d ( Context * __context__, char * const  __value_rename_at_882_21 );
inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_22, bool __extra_rename_at_38_23, bool __contracts_rename_at_38_24, bool __modules_rename_at_38_25 );
inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_1c4e5b57d2a9fc6f ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_26 );
inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1c1ac1d8f5641a51 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1732_28 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_ba6198643c09cf0e ( Context * __context__, TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_912660ab1e667e4b ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_1059_2, smart_ptr_raw<Expression> const  __src_rename_at_1059_3 )
{
    builtin_smart_ptr_clone(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_1059_2),das_auto_cast<smart_ptr_raw<void> const >::cast(__src_rename_at_1059_3),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_a870648d308a6130 ( Context * __context__, TDim<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>,1> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_5, ast::AstFunctionAnnotation * __value_rename_at_181_6 )
{
    das_copy(__Arr_rename_at_181_5(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_5),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_6);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f6d476408197914e ( Context * __context__, typemacro_boost::TypeMacroMacro const  &  __cl_rename_at_116_7 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_7.__rtti))).getStructType())));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickclone_to_moveTick2007252383599261567_8e43a49b5b6891e2 ( Context * __context__, smart_ptr_raw<Expression> const  __clone_src_rename_at_1056_8 )
{
    smart_ptr_raw<Expression> __clone_dest_rename_at_1058_9;das_zero(__clone_dest_rename_at_1058_9);
    clone_912660ab1e667e4b(__context__,__clone_dest_rename_at_1058_9,__clone_src_rename_at_1056_8);
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__clone_dest_rename_at_1058_9);
}

inline TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> _FuncbuiltinTickto_array_moveTick3185538323411982277_77313f057747cf52 ( Context * __context__, TDim<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>,1> &  __a_rename_at_1342_18 )
{
    TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> __arr_rename_at_1344_19;das_zero(__arr_rename_at_1344_19);
    _FuncbuiltinTickresizeTick4811697762258667383_ba6198643c09cf0e(__context__,das_arg<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>>>::pass(__arr_rename_at_1344_19),1);
    das_copy(das_cast<TDim<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>,1>>::cast(das_ref(__context__,__arr_rename_at_1344_19(0,__context__))),__a_rename_at_1342_18);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,AutoVariant<bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>>>::cast(__arr_rename_at_1344_19);
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_c545e308a61d940f ( Context * __context__, int32_t __value_rename_at_882_20 )
{
    LineInfo _temp_make_local_883_43_32; _temp_make_local_883_43_32;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<int32_t,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@typemacro_boost::ast_boost`convert_to_expression`16483834305137942954 C=Xi CH<rtti::LineInfo>*/ 0x343360d074394baf)),__value_rename_at_882_20,das_arg<LineInfo>::pass((_temp_make_local_883_43_32 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline smart_ptr_raw<Expression> _Funcast_boostTickconvert_to_expressionTick11707039267614988023_48a5beb36940388d ( Context * __context__, char * const  __value_rename_at_882_21 )
{
    LineInfo _temp_make_local_883_43_32; _temp_make_local_883_43_32;
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(das_invoke_function<smart_ptr_raw<Expression>>::invoke<char * const ,LineInfo const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@typemacro_boost::ast_boost`convert_to_expression`16483834305137942954 C=Xs CH<rtti::LineInfo>*/ 0x2103cf52d005bbd9)),__value_rename_at_882_21,das_arg<LineInfo>::pass((_temp_make_local_883_43_32 = (/*c-tor*/ LineInfo(/*end-c-tor*/))))));
}

inline char * _FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89 ( Context * __context__, smart_ptr_raw<TypeDecl> const  __decl_rename_at_38_22, bool __extra_rename_at_38_23, bool __contracts_rename_at_38_24, bool __modules_rename_at_38_25 )
{
    return das_auto_cast<char *>::cast(((char * const )(ast_describe_typedecl(__decl_rename_at_38_22,__extra_rename_at_38_23,__contracts_rename_at_38_24,__modules_rename_at_38_25,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline smart_ptr_raw<Expression> _FuncbuiltinTickadd_ptr_refTick918185754185293024_1c4e5b57d2a9fc6f ( Context * __context__, smart_ptr_raw<Expression> const  __src_rename_at_1744_26 )
{
    smart_ptr_raw<Expression> __dst_rename_at_1746_27; das_zero(__dst_rename_at_1746_27); das_move(__dst_rename_at_1746_27, _FuncbuiltinTickclone_to_moveTick2007252383599261567_8e43a49b5b6891e2(__context__,das_cast<smart_ptr_raw<Expression>>::cast(__src_rename_at_1744_26)));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<Expression>>::cast(__dst_rename_at_1746_27);
}

inline Structure * _FuncbuiltinTickget_ptrTick5807679485210906136_1c1ac1d8f5641a51 ( Context * __context__, smart_ptr_raw<Structure> __src_rename_at_1732_28 )
{
    return das_auto_cast<Structure *>::cast(das_cast<Structure *>::cast(__src_rename_at_1732_28));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x40de2454735fdda9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_ba6198643c09cf0e>>();
    };
    aotLib[0x74b4210f5379bdbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_912660ab1e667e4b>>();
    };
    aotLib[0xeef27ce9148978b1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_a870648d308a6130>>();
    };
    aotLib[0xd09660c6ccc577ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e64fa0322491795>>();
    };
    aotLib[0xf7fb031c9c6c344d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f6d476408197914e>>();
    };
    aotLib[0x5d1ff318890469bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickclone_to_moveTick2007252383599261567_8e43a49b5b6891e2>>();
    };
    aotLib[0x33fe5008bb200111] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_77313f057747cf52>>();
    };
    aotLib[0xc8cdf98124ca0d0e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick11707039267614988023_c545e308a61d940f>>();
    };
    aotLib[0xe4308c203c68cd59] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcast_boostTickconvert_to_expressionTick11707039267614988023_48a5beb36940388d>>();
    };
    aotLib[0xb9c50cbd3f619a9d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickdescribeTick2562845734617055679_2756234bd8d22e89>>();
    };
    aotLib[0xfee71086e95da09] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickadd_ptr_refTick918185754185293024_1c4e5b57d2a9fc6f>>();
    };
    aotLib[0xd96858997179015f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickget_ptrTick5807679485210906136_1c1ac1d8f5641a51>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_9638349545935827793
AotListBase impl_aot_typemacro_boost(_anon_9638349545935827793::registerAotFunctions);
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
