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
 // require defer
 // require instance_function

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
namespace _anon_1774704132612841535 {

namespace instance_function { struct InstanceFunctionAnnotation; };
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
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
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
namespace templates_boost {

struct Template {
    TTable<char *,AutoTuple<char *,char *>> kaboomVar;
    TTable<char *,char *> call2name;
    TTable<char *,char *> field2name;
    TTable<char *,char *> var2name;
    TTable<char *,smart_ptr_raw<Expression>> var2expr;
    TTable<char *,TArray<smart_ptr_raw<Expression>>> var2exprList;
    TTable<char *,char *> type2type;
    TTable<char *,smart_ptr_raw<TypeDecl>> type2etype;
    TTable<char *,char *> blockArgName;
    TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))> annArg;
    TTable<char *,TArray<smart_ptr_raw<Variable>>> blkArg;
    TTable<char *,smart_ptr_raw<Expression>> tag2expr;
};
}
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
// unused structure DeferMacro
// unused structure DeferDeleteMacro
namespace instance_function {

struct InstanceFunctionAnnotation {
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


inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a2b1a689e20a575 ( Context * __context__, TArray<smart_ptr_raw<Expression>> & __a_rename_at_1234_0 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_83c1ba40716e7527 ( Context * __context__, TArray<smart_ptr_raw<Variable>> & __a_rename_at_1234_2 );
inline void finalize_f275d25d3bab896e ( Context * __context__, AutoTuple<char *,char *> & ____this_rename_at_1249_4 );
inline Sequence DAS_COMMENT((AutoTuple<char *,char *> &)) _FuncbuiltinTickvaluesTick1351216622833168869_d3fa58e24f43cfee ( Context * __context__, TTable<char *,AutoTuple<char *,char *>> & __a_rename_at_1202_5 );
inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_2443fe1c4bbe3c50 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1202_7 );
inline Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>> &)) _FuncbuiltinTickvaluesTick1351216622833168869_31929a03d4bf2826 ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Expression>>> & __a_rename_at_1202_9 );
inline Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl> &)) _FuncbuiltinTickvaluesTick1351216622833168869_964e59bd28b9a8be ( Context * __context__, TTable<char *,smart_ptr_raw<TypeDecl>> & __a_rename_at_1202_11 );
inline Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration)) &)) _FuncbuiltinTickvaluesTick1351216622833168869_fa0ab3ba9cc3baa4 ( Context * __context__, TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))> & __a_rename_at_1202_13 );
inline Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>> &)) _FuncbuiltinTickvaluesTick1351216622833168869_3b30db89fc0cf777 ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Variable>>> & __a_rename_at_1202_15 );
inline void _FuncbuiltinTickpushTick10769833213962245646_d3fcaf31d1a83d76 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> & __Arr_rename_at_181_17, ast::AstFunctionAnnotation * __value_rename_at_181_18 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f32abc59f616d068 ( Context * __context__, instance_function::InstanceFunctionAnnotation const  & __cl_rename_at_116_19 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b079da9a6c215cc9 ( Context * __context__, TTable<char *,AutoTuple<char *,char *>> & __a_rename_at_1245_20 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7 ( Context * __context__, TTable<char *,char *> & __a_rename_at_1245_22 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_c5dc232fb1c2fc9c ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> & __a_rename_at_1245_23 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_a4db0c6508d148ca ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Expression>>> & __a_rename_at_1245_25 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_4c2646c3fc9405d1 ( Context * __context__, TTable<char *,smart_ptr_raw<TypeDecl>> & __a_rename_at_1245_27 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_6137e5fabd52bf42 ( Context * __context__, TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))> & __a_rename_at_1245_29 );
inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b263245eff9d431c ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Variable>>> & __a_rename_at_1245_31 );
inline void clone_739c026e2336afa1 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_52_33, void * const  __src_rename_at_52_34 );
inline void finalize_1f80cc7beabc6f42 ( Context * __context__, templates_boost::Template & ____this_rename_at_13_35 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_a2b1a689e20a575 ( Context * __context__, TArray<smart_ptr_raw<Expression>> &  __a_rename_at_1234_0 )
{
    {
        bool __need_loop_1236 = true;
        // aV: smart_ptr<ast::Expression> aka TT&
        das_iterator<TArray<smart_ptr_raw<Expression>>> __aV_iterator(__a_rename_at_1234_0);
        smart_ptr_raw<Expression> * __aV_rename_at_1236_1;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_1)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_1)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1236_1));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_1));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Expression>>>::pass(__a_rename_at_1234_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_83c1ba40716e7527 ( Context * __context__, TArray<smart_ptr_raw<Variable>> &  __a_rename_at_1234_2 )
{
    {
        bool __need_loop_1236 = true;
        // aV: smart_ptr<ast::Variable> aka TT&
        das_iterator<TArray<smart_ptr_raw<Variable>>> __aV_iterator(__a_rename_at_1234_2);
        smart_ptr_raw<Variable> * __aV_rename_at_1236_3;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_3)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_3)) )
        {
            das_delete_handle<smart_ptr_raw<Variable>>::clear(__context__,(*__aV_rename_at_1236_3));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_3));
    };
    builtin_array_free(das_arg<TArray<smart_ptr_raw<Variable>>>::pass(__a_rename_at_1234_2),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_f275d25d3bab896e ( Context * __context__, AutoTuple<char *,char *> &  ____this_rename_at_1249_4 )
{
    memset((void*)&(____this_rename_at_1249_4), 0, TypeSize<AutoTuple<char *,char *>>::size);
}

inline Sequence DAS_COMMENT((AutoTuple<char *,char *> &)) _FuncbuiltinTickvaluesTick1351216622833168869_d3fa58e24f43cfee ( Context * __context__, TTable<char *,AutoTuple<char *,char *>> &  __a_rename_at_1202_5 )
{
    Sequence DAS_COMMENT((AutoTuple<char *,char *> *)) __it_rename_at_1203_6;das_zero(__it_rename_at_1203_6);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((AutoTuple<char *,char *>))>::pass(__it_rename_at_1203_6),das_arg<TTable<char *,AutoTuple<char *,char *>>>::pass(__a_rename_at_1202_5),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((AutoTuple<char *,char *> &))>::cast(__it_rename_at_1203_6);
}

inline Sequence DAS_COMMENT((smart_ptr_raw<Expression> &)) _FuncbuiltinTickvaluesTick1351216622833168869_2443fe1c4bbe3c50 ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1202_7 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) __it_rename_at_1203_8;das_zero(__it_rename_at_1203_8);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))>::pass(__it_rename_at_1203_8),das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1202_7),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<Expression> &))>::cast(__it_rename_at_1203_8);
}

inline Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>> &)) _FuncbuiltinTickvaluesTick1351216622833168869_31929a03d4bf2826 ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Expression>>> &  __a_rename_at_1202_9 )
{
    Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>> *)) __it_rename_at_1203_10;das_zero(__it_rename_at_1203_10);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>>))>::pass(__it_rename_at_1203_10),das_arg<TTable<char *,TArray<smart_ptr_raw<Expression>>>>::pass(__a_rename_at_1202_9),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>> &))>::cast(__it_rename_at_1203_10);
}

inline Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl> &)) _FuncbuiltinTickvaluesTick1351216622833168869_964e59bd28b9a8be ( Context * __context__, TTable<char *,smart_ptr_raw<TypeDecl>> &  __a_rename_at_1202_11 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl> *)) __it_rename_at_1203_12;das_zero(__it_rename_at_1203_12);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl>))>::pass(__it_rename_at_1203_12),das_arg<TTable<char *,smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1202_11),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl> &))>::cast(__it_rename_at_1203_12);
}

inline Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration)) &)) _FuncbuiltinTickvaluesTick1351216622833168869_fa0ab3ba9cc3baa4 ( Context * __context__, TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))> &  __a_rename_at_1202_13 )
{
    Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration)) *)) __it_rename_at_1203_14;das_zero(__it_rename_at_1203_14);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration))))>::pass(__it_rename_at_1203_14),das_arg<TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))>>::pass(__a_rename_at_1202_13),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration)) &))>::cast(__it_rename_at_1203_14);
}

inline Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>> &)) _FuncbuiltinTickvaluesTick1351216622833168869_3b30db89fc0cf777 ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Variable>>> &  __a_rename_at_1202_15 )
{
    Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>> *)) __it_rename_at_1203_16;das_zero(__it_rename_at_1203_16);
    builtin_table_values(das_arg<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>>))>::pass(__it_rename_at_1203_16),das_arg<TTable<char *,TArray<smart_ptr_raw<Variable>>>>::pass(__a_rename_at_1202_15),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    return /* <- */ das_auto_cast_move<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>> &))>::cast(__it_rename_at_1203_16);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_d3fcaf31d1a83d76 ( Context * __context__, TArray<ast::AstFunctionAnnotation *> &  __Arr_rename_at_181_17, ast::AstFunctionAnnotation * __value_rename_at_181_18 )
{
    das_copy(__Arr_rename_at_181_17(builtin_array_push_back(das_arg<TArray<ast::AstFunctionAnnotation *>>::pass(__Arr_rename_at_181_17),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_18);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_f32abc59f616d068 ( Context * __context__, instance_function::InstanceFunctionAnnotation const  &  __cl_rename_at_116_19 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_19.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b079da9a6c215cc9 ( Context * __context__, TTable<char *,AutoTuple<char *,char *>> &  __a_rename_at_1245_20 )
{
    Sequence DAS_COMMENT((AutoTuple<char *,char *> *)) _temp_make_local_1247_19_0; _temp_make_local_1247_19_0;
    {
        bool __need_loop_1247 = true;
        // aV: tuple<prefix:string;suffix:string>&
        das_iterator<Sequence DAS_COMMENT((AutoTuple<char *,char *>))> __aV_iterator((_temp_make_local_1247_19_0 = (_FuncbuiltinTickvaluesTick1351216622833168869_d3fa58e24f43cfee(__context__,das_arg<TTable<char *,AutoTuple<char *,char *>>>::pass(__a_rename_at_1245_20)))));
        AutoTuple<char *,char *> * __aV_rename_at_1247_21;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_21)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_21)) )
        {
            finalize_f275d25d3bab896e(__context__,das_arg<AutoTuple<char *,char *>>::pass((*__aV_rename_at_1247_21)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_21));
    };
    builtin_table_free(das_arg<TTable<char *,AutoTuple<char *,char *>>>::pass(__a_rename_at_1245_20),8,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7 ( Context * __context__, TTable<char *,char *> &  __a_rename_at_1245_22 )
{
    builtin_table_free(das_arg<TTable<char *,char *>>::pass(__a_rename_at_1245_22),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_c5dc232fb1c2fc9c ( Context * __context__, TTable<char *,smart_ptr_raw<Expression>> &  __a_rename_at_1245_23 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<Expression> *)) _temp_make_local_1247_19_1; _temp_make_local_1247_19_1;
    {
        bool __need_loop_1247 = true;
        // aV: smart_ptr<ast::Expression>&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<Expression>))> __aV_iterator((_temp_make_local_1247_19_1 = (_FuncbuiltinTickvaluesTick1351216622833168869_2443fe1c4bbe3c50(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1245_23)))));
        smart_ptr_raw<Expression> * __aV_rename_at_1247_24;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_24)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_24)) )
        {
            das_delete_handle<smart_ptr_raw<Expression>>::clear(__context__,(*__aV_rename_at_1247_24));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_24));
    };
    builtin_table_free(das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(__a_rename_at_1245_23),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_a4db0c6508d148ca ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Expression>>> &  __a_rename_at_1245_25 )
{
    Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>> *)) _temp_make_local_1247_19_2; _temp_make_local_1247_19_2;
    {
        bool __need_loop_1247 = true;
        // aV: array<smart_ptr<ast::Expression>>&
        das_iterator<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Expression>>))> __aV_iterator((_temp_make_local_1247_19_2 = (_FuncbuiltinTickvaluesTick1351216622833168869_31929a03d4bf2826(__context__,das_arg<TTable<char *,TArray<smart_ptr_raw<Expression>>>>::pass(__a_rename_at_1245_25)))));
        TArray<smart_ptr_raw<Expression>> * __aV_rename_at_1247_26;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_26)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_26)) )
        {
            _FuncbuiltinTickfinalizeTick13836114024949725080_a2b1a689e20a575(__context__,das_arg<TArray<smart_ptr_raw<Expression>>>::pass((*__aV_rename_at_1247_26)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_26));
    };
    builtin_table_free(das_arg<TTable<char *,TArray<smart_ptr_raw<Expression>>>>::pass(__a_rename_at_1245_25),8,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_4c2646c3fc9405d1 ( Context * __context__, TTable<char *,smart_ptr_raw<TypeDecl>> &  __a_rename_at_1245_27 )
{
    Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl> *)) _temp_make_local_1247_19_3; _temp_make_local_1247_19_3;
    {
        bool __need_loop_1247 = true;
        // aV: smart_ptr<ast::TypeDecl>&
        das_iterator<Sequence DAS_COMMENT((smart_ptr_raw<TypeDecl>))> __aV_iterator((_temp_make_local_1247_19_3 = (_FuncbuiltinTickvaluesTick1351216622833168869_964e59bd28b9a8be(__context__,das_arg<TTable<char *,smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1245_27)))));
        smart_ptr_raw<TypeDecl> * __aV_rename_at_1247_28;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_28)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_28)) )
        {
            das_delete_handle<smart_ptr_raw<TypeDecl>>::clear(__context__,(*__aV_rename_at_1247_28));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_28));
    };
    builtin_table_free(das_arg<TTable<char *,smart_ptr_raw<TypeDecl>>>::pass(__a_rename_at_1245_27),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_6137e5fabd52bf42 ( Context * __context__, TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))> &  __a_rename_at_1245_29 )
{
    Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration)) *)) _temp_make_local_1247_19_4; _temp_make_local_1247_19_4;
    {
        bool __need_loop_1247 = true;
        // aV: lambda<(var ann:rtti::AnnotationDeclaration -const):void>&
        das_iterator<Sequence DAS_COMMENT((Lambda DAS_COMMENT((void,AnnotationDeclaration))))> __aV_iterator((_temp_make_local_1247_19_4 = (_FuncbuiltinTickvaluesTick1351216622833168869_fa0ab3ba9cc3baa4(__context__,das_arg<TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))>>::pass(__a_rename_at_1245_29)))));
        Lambda DAS_COMMENT((void,AnnotationDeclaration)) * __aV_rename_at_1247_30;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_30)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_30)) )
        {
            das_delete<Lambda DAS_COMMENT((void,AnnotationDeclaration))>::clear(__context__,(*__aV_rename_at_1247_30));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_30));
    };
    builtin_table_free(das_arg<TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))>>::pass(__a_rename_at_1245_29),8,8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick5454204887383796109_b263245eff9d431c ( Context * __context__, TTable<char *,TArray<smart_ptr_raw<Variable>>> &  __a_rename_at_1245_31 )
{
    Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>> *)) _temp_make_local_1247_19_5; _temp_make_local_1247_19_5;
    {
        bool __need_loop_1247 = true;
        // aV: array<smart_ptr<ast::Variable> aka VariablePtr>&
        das_iterator<Sequence DAS_COMMENT((TArray<smart_ptr_raw<Variable>>))> __aV_iterator((_temp_make_local_1247_19_5 = (_FuncbuiltinTickvaluesTick1351216622833168869_3b30db89fc0cf777(__context__,das_arg<TTable<char *,TArray<smart_ptr_raw<Variable>>>>::pass(__a_rename_at_1245_31)))));
        TArray<smart_ptr_raw<Variable>> * __aV_rename_at_1247_32;
        __need_loop_1247 = __aV_iterator.first(__context__,(__aV_rename_at_1247_32)) && __need_loop_1247;
        for ( ; __need_loop_1247 ; __need_loop_1247 = __aV_iterator.next(__context__,(__aV_rename_at_1247_32)) )
        {
            _FuncbuiltinTickfinalizeTick13836114024949725080_83c1ba40716e7527(__context__,das_arg<TArray<smart_ptr_raw<Variable>>>::pass((*__aV_rename_at_1247_32)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1247_32));
    };
    builtin_table_free(das_arg<TTable<char *,TArray<smart_ptr_raw<Variable>>>>::pass(__a_rename_at_1245_31),8,24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void clone_739c026e2336afa1 ( Context * __context__, smart_ptr_raw<Expression> & __dest_rename_at_52_33, void * const  __src_rename_at_52_34 )
{
    builtin_smart_ptr_clone_ptr(das_auto_cast<smart_ptr_raw<void> &>::cast(__dest_rename_at_52_33),__src_rename_at_52_34,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_1f80cc7beabc6f42 ( Context * __context__, templates_boost::Template &  ____this_rename_at_13_35 )
{
    _FuncbuiltinTickfinalizeTick5454204887383796109_b079da9a6c215cc9(__context__,das_arg<TTable<char *,AutoTuple<char *,char *>>>::pass(____this_rename_at_13_35.kaboomVar));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7(__context__,das_arg<TTable<char *,char *>>::pass(____this_rename_at_13_35.call2name));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7(__context__,das_arg<TTable<char *,char *>>::pass(____this_rename_at_13_35.field2name));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7(__context__,das_arg<TTable<char *,char *>>::pass(____this_rename_at_13_35.var2name));
    _FuncbuiltinTickfinalizeTick5454204887383796109_c5dc232fb1c2fc9c(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(____this_rename_at_13_35.var2expr));
    _FuncbuiltinTickfinalizeTick5454204887383796109_a4db0c6508d148ca(__context__,das_arg<TTable<char *,TArray<smart_ptr_raw<Expression>>>>::pass(____this_rename_at_13_35.var2exprList));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7(__context__,das_arg<TTable<char *,char *>>::pass(____this_rename_at_13_35.type2type));
    _FuncbuiltinTickfinalizeTick5454204887383796109_4c2646c3fc9405d1(__context__,das_arg<TTable<char *,smart_ptr_raw<TypeDecl>>>::pass(____this_rename_at_13_35.type2etype));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7(__context__,das_arg<TTable<char *,char *>>::pass(____this_rename_at_13_35.blockArgName));
    _FuncbuiltinTickfinalizeTick5454204887383796109_6137e5fabd52bf42(__context__,das_arg<TTable<char *,Lambda DAS_COMMENT((void,AnnotationDeclaration))>>::pass(____this_rename_at_13_35.annArg));
    _FuncbuiltinTickfinalizeTick5454204887383796109_b263245eff9d431c(__context__,das_arg<TTable<char *,TArray<smart_ptr_raw<Variable>>>>::pass(____this_rename_at_13_35.blkArg));
    _FuncbuiltinTickfinalizeTick5454204887383796109_c5dc232fb1c2fc9c(__context__,das_arg<TTable<char *,smart_ptr_raw<Expression>>>::pass(____this_rename_at_13_35.tag2expr));
    memset((void*)&(____this_rename_at_13_35), 0, TypeSize<templates_boost::Template>::size);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x16b14c6c80aee23f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_a2b1a689e20a575>>();
    };
    aotLib[0x84c6062039edc782] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_83c1ba40716e7527>>();
    };
    aotLib[0x6c850b22d6d55545] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_f275d25d3bab896e>>();
    };
    aotLib[0xb918781f88ac24c3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_d3fa58e24f43cfee>>();
    };
    aotLib[0x577ea742b96d525b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_2443fe1c4bbe3c50>>();
    };
    aotLib[0xf561bea068936f7d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_31929a03d4bf2826>>();
    };
    aotLib[0x4eaac4982d61912f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_964e59bd28b9a8be>>();
    };
    aotLib[0xa56eb8b1b6a2f71e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_fa0ab3ba9cc3baa4>>();
    };
    aotLib[0x88b67cb48d83c3b1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickvaluesTick1351216622833168869_3b30db89fc0cf777>>();
    };
    aotLib[0xd1e84bbc37fe0a0b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_d3fcaf31d1a83d76>>();
    };
    aotLib[0x1501f84cdffc47d5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_f32abc59f616d068>>();
    };
    aotLib[0x171ae5c0b336bae1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_b079da9a6c215cc9>>();
    };
    aotLib[0xfb42ad302f0bcaf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_6adfe5e445e6a5f7>>();
    };
    aotLib[0x406ce9ce9c9f9440] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_c5dc232fb1c2fc9c>>();
    };
    aotLib[0x5ed562ea1e25a25c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_a4db0c6508d148ca>>();
    };
    aotLib[0xa2d117854eefbf6d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_4c2646c3fc9405d1>>();
    };
    aotLib[0x3a8b3cbbc0ae3492] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_6137e5fabd52bf42>>();
    };
    aotLib[0x88917c7de4daeb46] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick5454204887383796109_b263245eff9d431c>>();
    };
    aotLib[0x4a5c11ae9676d5a1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&clone_739c026e2336afa1>>();
    };
    aotLib[0x6635956d9d0df384] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_1f80cc7beabc6f42>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_1774704132612841535
AotListBase impl_aot_instance_function(_anon_1774704132612841535::registerAotFunctions);
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
