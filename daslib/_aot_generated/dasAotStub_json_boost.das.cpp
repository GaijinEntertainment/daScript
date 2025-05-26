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
 // require json
 // require ast_boost
 // require contracts
 // require templates
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require apply
 // require json_boost

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
namespace _anon_11606215633914179217 {

namespace json_boost { struct BetterJsonMacro; };
namespace json_boost { struct JsonReader; };
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
namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_99_1; };
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
namespace templates { struct DecltypeMacro; };
namespace templates { struct DecltypeNoRefMacro; };
namespace templates { struct TemplateMacro; };
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
namespace apply { struct ApplyMacro; };
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
namespace ast {

struct AstVariantMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
// unused structure AstForLoopMacro
// unused structure AstCaptureMacro
// unused structure AstTypeMacro
// unused structure AstSimulateMacro
namespace ast {

struct AstReaderMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
// unused structure AstCommentReader
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
namespace json {

struct JsonValue {
    AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> value;
};
}
// unused structure TokenAt
// unused structure _lambda_json_99_1
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
// unused structure DecltypeMacro
// unused structure DecltypeNoRefMacro
// unused structure TemplateMacro
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
// unused structure DeferMacro
// unused structure DeferDeleteMacro
// unused structure ApplyMacro
namespace json_boost {

struct BetterJsonMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
}
namespace json_boost {

struct JsonReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}
extern TypeInfo __type_info__8d8d8008262e16ec;
extern TypeInfo __type_info__b68d800849332aec;

TypeInfo __type_info__8d8d8008262e16ec = { Type::tInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0x8d8d8008262e16ec) };
TypeInfo __type_info__b68d800849332aec = { Type::tUInt64, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 8, UINT64_C(0xb68d800849332aec) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__8d8d8008262e16ec };
TypeInfo * __tinfo_1[1] = { &__type_info__b68d800849332aec };

inline void _FuncbuiltinTickpushTick10769833213962245646_2dbef66663d682d8 ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9fd87b225af26eb8 ( Context * __context__, json_boost::BetterJsonMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532 ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d01402ef40f6562a ( Context * __context__, json_boost::JsonReader const  & __cl_rename_at_116_5 );
inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 );
inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 );
inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 );
inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 );
inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 );
inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 );
inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 );
inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 );
inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 );
inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 );
inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 );
inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 );
inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 );
inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 );
inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 );
inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 );
inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 );
inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 );
inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 );
inline json::JsonValue * JV_de08d1a11663707 ( Context * __context__, float __v_rename_at_469_48 );
inline json::JsonValue * JV_2fa74720cd155930 ( Context * __context__, int32_t __v_rename_at_473_49 );
inline json::JsonValue * JV_549d699b656608a2 ( Context * __context__, Bitfield __v_rename_at_477_50 );
inline json::JsonValue * JV_840ec78279ac852d ( Context * __context__, int8_t __val_rename_at_481_51 );
inline json::JsonValue * JV_526f504129917241 ( Context * __context__, uint8_t __val_rename_at_485_52 );
inline json::JsonValue * JV_17760da7cd1ea0ee ( Context * __context__, int16_t __val_rename_at_489_53 );
inline json::JsonValue * JV_30573b250fcf268b ( Context * __context__, uint16_t __val_rename_at_493_54 );
inline json::JsonValue * JV_c85629fe70fc95b5 ( Context * __context__, uint32_t __val_rename_at_497_55 );
inline json::JsonValue * JV_b0de7f0f7332ba2d ( Context * __context__, int64_t __val_rename_at_501_56 );
inline json::JsonValue * JV_8425ea1d753a21fb ( Context * __context__, uint64_t __val_rename_at_509_57 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_2dbef66663d682d8 ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_181_0, ast::AstVariantMacro * __value_rename_at_181_1 )
{
    das_copy(__Arr_rename_at_181_0(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_181_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_9fd87b225af26eb8 ( Context * __context__, json_boost::BetterJsonMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti))).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532 ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_d01402ef40f6562a ( Context * __context__, json_boost::JsonReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_21_10, char * const  __key_rename_at_21_11 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_21_10 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_21_10->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_21_10->value,__context__)),__key_rename_at_21_11,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_26_12, char * const  __key_rename_at_26_13 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_26_12 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_26_12->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_26_12->value,__context__)),__key_rename_at_26_13,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_3538a35cc5ccc9d5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_31_14, char * const  __key_rename_at_31_15 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_31_14 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_31_14->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_31_14->value,__context__)),__key_rename_at_31_15,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_5655a09dd3be2073 ( Context * __context__, json::JsonValue * __a_rename_at_36_16, char * const  __key_rename_at_36_17 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_36_16 != nullptr) && das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_36_16->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TTable<char *,json::JsonValue *>,0,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_36_16->value,__context__)),__key_rename_at_36_17,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_ba0b0aab6632dd31 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_41_18, int32_t __idx_rename_at_41_19 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_41_18 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_41_18->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_41_18->value,__context__)),__idx_rename_at_41_19,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_d744859b79ee77ab ( Context * __context__, json::JsonValue * __a_rename_at_46_20, int32_t __idx_rename_at_46_21 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_46_20 != nullptr) && das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_46_20->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_auto_variant_field<TArray<json::JsonValue *>,1,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_46_20->value,__context__)),__idx_rename_at_46_21,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline double _FuncQmarkQmark_11719d2be16bd01 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_51_22, double __val_rename_at_51_23 )
{
    return das_auto_cast<double>::cast(((__a_rename_at_51_22 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_51_22->value)) ? das_auto_cast<double>::cast(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_51_22->value,__context__)) : das_auto_cast<double>::cast(__val_rename_at_51_23));
}

inline float _FuncQmarkQmark_233f48b50ce5f404 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_56_24, float __val_rename_at_56_25 )
{
    return das_auto_cast<float>::cast(((__a_rename_at_56_24 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_56_24->value)) ? das_auto_cast<float>::cast(float(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_56_24->value,__context__))) : das_auto_cast<float>::cast(__val_rename_at_56_25));
}

inline int8_t _FuncQmarkQmark_1a8257b5434aa8b3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_61_26, int8_t __val_rename_at_61_27 )
{
    return das_auto_cast<int8_t>::cast(((__a_rename_at_61_26 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_61_26->value)) ? das_auto_cast<int8_t>::cast(int8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_61_26->value,__context__))) : das_auto_cast<int8_t>::cast(__val_rename_at_61_27));
}

inline int16_t _FuncQmarkQmark_f62e6510f721332c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_66_28, int16_t __val_rename_at_66_29 )
{
    return das_auto_cast<int16_t>::cast(((__a_rename_at_66_28 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_66_28->value)) ? das_auto_cast<int16_t>::cast(int16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_66_28->value,__context__))) : das_auto_cast<int16_t>::cast(__val_rename_at_66_29));
}

inline int32_t _FuncQmarkQmark_6a40c116b1d48e91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_30, int32_t __val_rename_at_71_31 )
{
    return das_auto_cast<int32_t>::cast(((__a_rename_at_71_30 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_71_30->value)) ? das_auto_cast<int32_t>::cast(int32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_71_30->value,__context__))) : das_auto_cast<int32_t>::cast(__val_rename_at_71_31));
}

inline int64_t _FuncQmarkQmark_e763b5dc20b3213 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_76_32, int64_t __val_rename_at_76_33 )
{
    return das_auto_cast<int64_t>::cast(((__a_rename_at_76_32 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_76_32->value)) ? das_auto_cast<int64_t>::cast(int64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_76_32->value,__context__))) : das_auto_cast<int64_t>::cast(__val_rename_at_76_33));
}

inline uint8_t _FuncQmarkQmark_e5a6fe7b7f0aecc6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_81_34, uint8_t __val_rename_at_81_35 )
{
    return das_auto_cast<uint8_t>::cast(((__a_rename_at_81_34 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_81_34->value)) ? das_auto_cast<uint8_t>::cast(uint8_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_81_34->value,__context__))) : das_auto_cast<uint8_t>::cast(__val_rename_at_81_35));
}

inline uint16_t _FuncQmarkQmark_b339f9b2027db27b ( Context * __context__, json::JsonValue const  * const  __a_rename_at_86_36, uint16_t __val_rename_at_86_37 )
{
    return das_auto_cast<uint16_t>::cast(((__a_rename_at_86_36 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_86_36->value)) ? das_auto_cast<uint16_t>::cast(uint16_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_86_36->value,__context__))) : das_auto_cast<uint16_t>::cast(__val_rename_at_86_37));
}

inline uint32_t _FuncQmarkQmark_bab9e0c43667d51a ( Context * __context__, json::JsonValue const  * const  __a_rename_at_91_38, uint32_t __val_rename_at_91_39 )
{
    return das_auto_cast<uint32_t>::cast(((__a_rename_at_91_38 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_91_38->value)) ? das_auto_cast<uint32_t>::cast(uint32_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_91_38->value,__context__))) : das_auto_cast<uint32_t>::cast(__val_rename_at_91_39));
}

inline uint64_t _FuncQmarkQmark_56f43cacc498853c ( Context * __context__, json::JsonValue const  * const  __a_rename_at_96_40, uint64_t __val_rename_at_96_41 )
{
    return das_auto_cast<uint64_t>::cast(((__a_rename_at_96_40 != nullptr) && das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_96_40->value)) ? das_auto_cast<uint64_t>::cast(uint64_t(das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_96_40->value,__context__))) : das_auto_cast<uint64_t>::cast(__val_rename_at_96_41));
}

inline bool _FuncQmarkQmark_4fa1031aa0e66319 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_101_42, bool __val_rename_at_101_43 )
{
    return das_auto_cast<bool>::cast(((__a_rename_at_101_42 != nullptr) && das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_101_42->value)) ? das_auto_cast<bool>::cast(das_get_auto_variant_field<bool,4,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_101_42->value,__context__)) : das_auto_cast<bool>::cast(__val_rename_at_101_43));
}

inline char * _FuncQmarkQmark_5415a6b0ff5b4eb5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_106_44, char * const  __val_rename_at_106_45 )
{
    return das_auto_cast<char *>::cast(((__a_rename_at_106_44 != nullptr) && das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::is(__a_rename_at_106_44->value)) ? das_auto_cast<char * const >::cast(das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::as(__a_rename_at_106_44->value,__context__)) : das_auto_cast<char * const >::cast(__val_rename_at_106_45));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  * _FuncQmarkDotTickvalue_7c5354fba05537e2 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_111_46 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> const  *>::cast(das_safe_navigation<json::JsonValue const ,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_111_46));
}

inline AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> * _FuncQmarkDotTickvalue_2bdf5bca6f9d0002 ( Context * __context__, json::JsonValue * __a_rename_at_116_47 )
{
    return das_auto_cast<AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> *>::cast(das_safe_navigation<json::JsonValue,AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_116_47));
}

inline json::JsonValue * JV_de08d1a11663707 ( Context * __context__, float __v_rename_at_469_48 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_470;
        das_zero(__mks_470);
        das_move((__mks_470.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_470;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_470) = double(__v_rename_at_469_48);
            return __mkv_470;
        })())));
        return __mks_470;
    })())));
}

inline json::JsonValue * JV_2fa74720cd155930 ( Context * __context__, int32_t __v_rename_at_473_49 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_474;
        das_zero(__mks_474);
        das_move((__mks_474.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_474;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_474) = double(__v_rename_at_473_49);
            return __mkv_474;
        })())));
        return __mks_474;
    })())));
}

inline json::JsonValue * JV_549d699b656608a2 ( Context * __context__, Bitfield __v_rename_at_477_50 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_478;
        das_zero(__mks_478);
        das_move((__mks_478.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_478;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_478) = double(__v_rename_at_477_50);
            return __mkv_478;
        })())));
        return __mks_478;
    })())));
}

inline json::JsonValue * JV_840ec78279ac852d ( Context * __context__, int8_t __val_rename_at_481_51 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_482;
        das_zero(__mks_482);
        das_move((__mks_482.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_482;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_482) = double(__val_rename_at_481_51);
            return __mkv_482;
        })())));
        return __mks_482;
    })())));
}

inline json::JsonValue * JV_526f504129917241 ( Context * __context__, uint8_t __val_rename_at_485_52 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_486;
        das_zero(__mks_486);
        das_move((__mks_486.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_486;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_486) = double(__val_rename_at_485_52);
            return __mkv_486;
        })())));
        return __mks_486;
    })())));
}

inline json::JsonValue * JV_17760da7cd1ea0ee ( Context * __context__, int16_t __val_rename_at_489_53 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_490;
        das_zero(__mks_490);
        das_move((__mks_490.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_490;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_490) = double(__val_rename_at_489_53);
            return __mkv_490;
        })())));
        return __mks_490;
    })())));
}

inline json::JsonValue * JV_30573b250fcf268b ( Context * __context__, uint16_t __val_rename_at_493_54 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_494;
        das_zero(__mks_494);
        das_move((__mks_494.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_494;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_494) = double(__val_rename_at_493_54);
            return __mkv_494;
        })())));
        return __mks_494;
    })())));
}

inline json::JsonValue * JV_c85629fe70fc95b5 ( Context * __context__, uint32_t __val_rename_at_497_55 )
{
    return das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_498;
        das_zero(__mks_498);
        das_move((__mks_498.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_498;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_498) = double(__val_rename_at_497_55);
            return __mkv_498;
        })())));
        return __mks_498;
    })())));
}

inline json::JsonValue * JV_b0de7f0f7332ba2d ( Context * __context__, int64_t __val_rename_at_501_56 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__val_rename_at_501_56 < INT64_C(-2147483648)) || (__val_rename_at_501_56 > INT64_C(2147483647))) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_503;
        das_zero(__mks_503);
        das_move((__mks_503.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_503;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_503) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_0, cast<int64_t>::from(__val_rename_at_501_56)));
            return __mkv_503;
        })())));
        return __mks_503;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_505;
        das_zero(__mks_505);
        das_move((__mks_505.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_505;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_505) = double(__val_rename_at_501_56);
            return __mkv_505;
        })())));
        return __mks_505;
    })()))));
}

inline json::JsonValue * JV_8425ea1d753a21fb ( Context * __context__, uint64_t __val_rename_at_509_57 )
{
    return das_auto_cast<json::JsonValue *>::cast((__val_rename_at_509_57 > UINT64_C(0xffffffff)) ? das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_511;
        das_zero(__mks_511);
        das_move((__mks_511.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_511;
            das_get_auto_variant_field<char *,2,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_511) = das_string_builder(__context__,SimNode_AotInterop<1>(__tinfo_1, cast<uint64_t>::from(__val_rename_at_509_57)));
            return __mkv_511;
        })())));
        return __mks_511;
    })()))) : das_auto_cast<json::JsonValue *>::cast(das_ascend<json::JsonValue,json::JsonValue>::make(__context__,nullptr,(([&]() -> json::JsonValue {
        json::JsonValue __mks_513;
        das_zero(__mks_513);
        das_move((__mks_513.value),((([&]() -> AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> {
            AutoVariant<TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *> __mkv_513;
            das_get_auto_variant_field<double,3,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,bool,void *>::set(__mkv_513) = double(__val_rename_at_509_57);
            return __mkv_513;
        })())));
        return __mks_513;
    })()))));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x9ffb73e1b24f1623] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_2dbef66663d682d8>>();
    };
    aotLib[0x8e9da741b004b839] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_9fd87b225af26eb8>>();
    };
    aotLib[0x27004c5f7ca4b606] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532>>();
    };
    aotLib[0x621bc12228fb29bb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_d01402ef40f6562a>>();
    };
    aotLib[0xf020fec97fe5f7be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_3538a35cc5ccc9d5>>();
    };
    aotLib[0x90ce6642cb555679] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_5655a09dd3be2073>>();
    };
    aotLib[0xf020fec97fe5f7be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDot_3538a35cc5ccc9d5>>();
    };
    aotLib[0x90ce6642cb555679] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDot_5655a09dd3be2073>>();
    };
    aotLib[0xa661d1639a571f4f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_ba0b0aab6632dd31>>();
    };
    aotLib[0xbb4bf8b0c412ba3f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkSqblSqbr_d744859b79ee77ab>>();
    };
    aotLib[0xf547858929db469a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_11719d2be16bd01>>();
    };
    aotLib[0x2052bfec09897f36] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_233f48b50ce5f404>>();
    };
    aotLib[0x5a64b7c5f16f6e6e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_1a8257b5434aa8b3>>();
    };
    aotLib[0x34750f0e2f5ba325] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_f62e6510f721332c>>();
    };
    aotLib[0x90f79a17f2b92360] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_6a40c116b1d48e91>>();
    };
    aotLib[0x73710cf5db3981a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_e763b5dc20b3213>>();
    };
    aotLib[0x4e8da125f981ecca] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_e5a6fe7b7f0aecc6>>();
    };
    aotLib[0x18007b7d06aaa2f2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_b339f9b2027db27b>>();
    };
    aotLib[0xb86241de1e2d6a52] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_bab9e0c43667d51a>>();
    };
    aotLib[0x98aebb33edcc1a02] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_56f43cacc498853c>>();
    };
    aotLib[0x96c21fd35188e801] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_4fa1031aa0e66319>>();
    };
    aotLib[0x788a314f4ed9aff0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkQmark_5415a6b0ff5b4eb5>>();
    };
    aotLib[0xaac7aaf08c3470bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDotTickvalue_7c5354fba05537e2>>();
    };
    aotLib[0xf4e4a4de8ae59bb1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncQmarkDotTickvalue_2bdf5bca6f9d0002>>();
    };
    aotLib[0xc4c8feb11410cd0a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_de08d1a11663707>>();
    };
    aotLib[0xa1a929ec68f6efa5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_2fa74720cd155930>>();
    };
    aotLib[0xac6ad6292aec793d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_549d699b656608a2>>();
    };
    aotLib[0xabb40867ec5a2a4b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_840ec78279ac852d>>();
    };
    aotLib[0x7eebfe09b878d154] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_526f504129917241>>();
    };
    aotLib[0xb187bc5951542f77] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_17760da7cd1ea0ee>>();
    };
    aotLib[0xb7aed417e8aa85cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_30573b250fcf268b>>();
    };
    aotLib[0xfba41459b91c9a0d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_c85629fe70fc95b5>>();
    };
    aotLib[0xa239927c68d23b11] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_b0de7f0f7332ba2d>>();
    };
    aotLib[0x432b947077ba23da] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&JV_8425ea1d753a21fb>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_11606215633914179217
AotListBase impl_aot_json_boost(_anon_11606215633914179217::registerAotFunctions);
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
