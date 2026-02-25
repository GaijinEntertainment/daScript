#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
#include "daScript/simulate/bin_serializer.h"
#include "daScript/simulate/runtime_profile.h"
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
 // require contracts
 // require templates
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require json
 // require apply
 // require static_let
 // require enum_trait
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
namespace _anon_14538537767599647147 {

namespace json_boost { struct BetterJsonMacro; };
namespace json_boost { struct JsonReader; };
namespace json_boost { struct JsonFieldState; };
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
namespace templates_boost { struct AstQNamedTemplateFunctionMacro; };
namespace templates_boost { struct AstQNamedTemplateClassMacro; };
namespace macro_boost { struct MacroVerifyMacro; };
namespace macro_boost { struct CaptureBlock; };
namespace macro_boost { struct CapturedVariable; };
namespace macro_boost { struct ColletFinally; };
namespace macro_boost { struct ColletLabels; };
namespace macro_boost { struct ReturnSkipLockcheck; };
namespace defer { struct DeferMacro; };
namespace defer { struct DeferDeleteMacro; };
namespace json { struct JsonValue; };
namespace json { struct TokenAt; };
namespace json { struct _lambda_json_180_1; };
namespace apply { struct ApplyMacro; };
namespace static_let { struct StaticLetMacro; };
namespace enum_trait { struct EnumFromStringConstruction; };
namespace enum_trait { struct TypeInfoGetEnumLength; };
namespace enum_trait { struct TypeInfoGetEnumNames; };
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
static_assert(sizeof(AstVariantMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstVariantMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
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
static_assert(sizeof(AstReaderMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(AstReaderMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,accept)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,suffix)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(AstReaderMacro,visit)==32,"structure field offset mismatch with DAS");
}
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
// unused structure AstQNamedTemplateFunctionMacro
// unused structure AstQNamedTemplateClassMacro
// unused structure MacroVerifyMacro
// unused structure CaptureBlock
// unused structure CapturedVariable
// unused structure ColletFinally
// unused structure ColletLabels
// unused structure ReturnSkipLockcheck
// unused structure DeferMacro
// unused structure DeferDeleteMacro
namespace json {

struct JsonValue {
    TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> value;
};
static_assert(sizeof(JsonValue)==56,"structure size mismatch with DAS");
static_assert(offsetof(JsonValue,value)==0,"structure field offset mismatch with DAS");
}
// unused structure TokenAt
// unused structure _lambda_json_180_1
// unused structure ApplyMacro
// unused structure StaticLetMacro
// unused structure EnumFromStringConstruction
// unused structure TypeInfoGetEnumLength
// unused structure TypeInfoGetEnumNames
namespace json_boost {

struct BetterJsonMacro {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstVariantMacro)) __finalize;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprIsVariant> const )) visitExprIsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprAsVariant> const )) visitExprAsVariant;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstVariantMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprSafeAsVariant> const )) visitExprSafeAsVariant;
};
static_assert(sizeof(BetterJsonMacro)==40,"structure size mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprIsVariant)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprAsVariant)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(BetterJsonMacro,visitExprSafeAsVariant)==32,"structure field offset mismatch with DAS");
}
namespace json_boost {

struct JsonReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
static_assert(sizeof(JsonReader)==40,"structure size mismatch with DAS");
static_assert(offsetof(JsonReader,__rtti)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,__finalize)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,accept)==16,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,suffix)==24,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonReader,visit)==32,"structure field offset mismatch with DAS");
}
namespace json_boost {

struct JsonFieldState {
    char * argName;
    bool enumAsInt;
    bool unescape;
    bool embed;
    bool optional;
};
static_assert(sizeof(JsonFieldState)==16,"structure size mismatch with DAS");
static_assert(offsetof(JsonFieldState,argName)==0,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonFieldState,enumAsInt)==8,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonFieldState,unescape)==9,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonFieldState,embed)==10,"structure field offset mismatch with DAS");
static_assert(offsetof(JsonFieldState,optional)==11,"structure field offset mismatch with DAS");
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__, TArray<ast::AstVariantMacro *> & __Arr_rename_at_193_0, ast::AstVariantMacro * __value_rename_at_193_1 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda ( Context * __context__, json_boost::BetterJsonMacro const  & __cl_rename_at_116_2 );
inline void _FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024 ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_193_3, ast::AstReaderMacro * __value_rename_at_193_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3 ( Context * __context__, json_boost::JsonReader const  & __cl_rename_at_116_5 );
inline double _Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_6, double __val_rename_at_59_7 );
inline float _Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_8, float __val_rename_at_59_9 );
inline int8_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_10, int8_t __val_rename_at_59_11 );
inline int16_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_12, int16_t __val_rename_at_59_13 );
inline int32_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_14, int32_t __val_rename_at_59_15 );
inline int64_t _Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_16, int64_t __val_rename_at_71_17 );
inline uint8_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_18, uint8_t __val_rename_at_59_19 );
inline uint16_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_20, uint16_t __val_rename_at_59_21 );
inline uint32_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_22, uint32_t __val_rename_at_59_23 );
inline uint64_t _Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_24, uint64_t __val_rename_at_71_25 );
inline json::JsonValue * _FuncQmarkSqblSqbr_753e250850106ed ( Context * __context__, json::JsonValue const  * const  __a_rename_at_29_30, char * const  __key_rename_at_29_31 );
inline json::JsonValue * _FuncQmarkSqblSqbr_a2af24ac467f0328 ( Context * __context__, json::JsonValue * __a_rename_at_34_32, char * const  __key_rename_at_34_33 );
inline json::JsonValue * _FuncQmarkDot_753e250850106ed ( Context * __context__, json::JsonValue const  * const  __a_rename_at_39_34, char * const  __key_rename_at_39_35 );
inline json::JsonValue * _FuncQmarkDot_a2af24ac467f0328 ( Context * __context__, json::JsonValue * __a_rename_at_44_36, char * const  __key_rename_at_44_37 );
inline json::JsonValue * _FuncQmarkSqblSqbr_3019c0cddac9a58d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_49_38, int32_t __idx_rename_at_49_39 );
inline json::JsonValue * _FuncQmarkSqblSqbr_7fe159c3c5b62bfc ( Context * __context__, json::JsonValue * __a_rename_at_54_40, int32_t __idx_rename_at_54_41 );
inline double _FuncQmarkQmark_76b74321baba1a32 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_85_42, double __val_rename_at_85_43 );
inline float _FuncQmarkQmark_b332904ef8eb0140 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_90_44, float __val_rename_at_90_45 );
inline int8_t _FuncQmarkQmark_54044768a2cafa63 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_95_46, int8_t __val_rename_at_95_47 );
inline int16_t _FuncQmarkQmark_b60ba49ced728a6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_100_48, int16_t __val_rename_at_100_49 );
inline int32_t _FuncQmarkQmark_20384941ec2b26f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_105_50, int32_t __val_rename_at_105_51 );
inline int64_t _FuncQmarkQmark_6f3aa361c92be15d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_110_52, int64_t __val_rename_at_110_53 );
inline uint8_t _FuncQmarkQmark_6eaffbb8cc7e4bc7 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_115_54, uint8_t __val_rename_at_115_55 );
inline uint16_t _FuncQmarkQmark_fcb53d6726dbe186 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_120_56, uint16_t __val_rename_at_120_57 );
inline uint32_t _FuncQmarkQmark_d52965ea200d6e17 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_125_58, uint32_t __val_rename_at_125_59 );
inline uint64_t _FuncQmarkQmark_5166c54d651a53b1 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_130_60, uint64_t __val_rename_at_130_61 );
inline bool _FuncQmarkQmark_635809dfed370d6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_135_62, bool __val_rename_at_135_63 );
inline char * _FuncQmarkQmark_eb9a1255c8c914ea ( Context * __context__, json::JsonValue const  * const  __a_rename_at_140_64, char * const  __val_rename_at_140_65 );
inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> const  * _FuncQmarkDotTickvalue_5189e68ec5a54919 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_145_66 );
inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> * _FuncQmarkDotTickvalue_9c3125530216994c ( Context * __context__, json::JsonValue * __a_rename_at_150_67 );
inline json_boost::JsonFieldState parse_json_annotation_24121682ba3116e0 ( Context * __context__, char * const  __name_rename_at_434_68, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & __annotation_rename_at_434_69 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__, TArray<ast::AstVariantMacro *> &  __Arr_rename_at_193_0, ast::AstVariantMacro * __value_rename_at_193_1 )
{
    das_copy(__Arr_rename_at_193_0(builtin_array_push_back(das_arg<TArray<ast::AstVariantMacro *>>::pass(__Arr_rename_at_193_0),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_1);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda ( Context * __context__, json_boost::BetterJsonMacro const  &  __cl_rename_at_116_2 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_2.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024 ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_193_3, ast::AstReaderMacro * __value_rename_at_193_4 )
{
    das_copy(__Arr_rename_at_193_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_193_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_193_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3 ( Context * __context__, json_boost::JsonReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti),__FILE__,__LINE__)).getStructType())));
}

inline double _Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_6, double __val_rename_at_59_7 )
{
    if ( __a_rename_at_59_6 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_6->value) )
        {
            return das_auto_cast<double>::cast(double(das_get_variant_field<double,8,3>::as(__a_rename_at_59_6->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_6->value) )
        {
            return das_auto_cast<double>::cast(double(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_6->value,__context__)));
        };
    };
    return das_auto_cast<double>::cast(__val_rename_at_59_7);
}

inline float _Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_8, float __val_rename_at_59_9 )
{
    if ( __a_rename_at_59_8 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_8->value) )
        {
            return das_auto_cast<float>::cast(float(das_get_variant_field<double,8,3>::as(__a_rename_at_59_8->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_8->value) )
        {
            return das_auto_cast<float>::cast(float(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_8->value,__context__)));
        };
    };
    return das_auto_cast<float>::cast(__val_rename_at_59_9);
}

inline int8_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_10, int8_t __val_rename_at_59_11 )
{
    if ( __a_rename_at_59_10 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_10->value) )
        {
            return das_auto_cast<int8_t>::cast(int8_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_10->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_10->value) )
        {
            return das_auto_cast<int8_t>::cast(int8_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_10->value,__context__)));
        };
    };
    return das_auto_cast<int8_t>::cast(__val_rename_at_59_11);
}

inline int16_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_12, int16_t __val_rename_at_59_13 )
{
    if ( __a_rename_at_59_12 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_12->value) )
        {
            return das_auto_cast<int16_t>::cast(int16_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_12->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_12->value) )
        {
            return das_auto_cast<int16_t>::cast(int16_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_12->value,__context__)));
        };
    };
    return das_auto_cast<int16_t>::cast(__val_rename_at_59_13);
}

inline int32_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_14, int32_t __val_rename_at_59_15 )
{
    if ( __a_rename_at_59_14 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_14->value) )
        {
            return das_auto_cast<int32_t>::cast(int32_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_14->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_14->value) )
        {
            return das_auto_cast<int32_t>::cast(int32_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_14->value,__context__)));
        };
    };
    return das_auto_cast<int32_t>::cast(__val_rename_at_59_15);
}

inline int64_t _Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_16, int64_t __val_rename_at_71_17 )
{
    if ( __a_rename_at_71_16 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_71_16->value) )
        {
            return das_auto_cast<int64_t>::cast(int64_t(das_get_variant_field<double,8,3>::as(__a_rename_at_71_16->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_71_16->value) )
        {
            return das_auto_cast<int64_t>::cast(int64_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_71_16->value,__context__)));
        } else if ( das_get_variant_field<char *,8,2>::is(__a_rename_at_71_16->value) )
        {
            return das_auto_cast<int64_t>::cast(string_to_int64(das_get_variant_field<char *,8,2>::as(__a_rename_at_71_16->value,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        };
    };
    return das_auto_cast<int64_t>::cast(__val_rename_at_71_17);
}

inline uint8_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_18, uint8_t __val_rename_at_59_19 )
{
    if ( __a_rename_at_59_18 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_18->value) )
        {
            return das_auto_cast<uint8_t>::cast(uint8_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_18->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_18->value) )
        {
            return das_auto_cast<uint8_t>::cast(uint8_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_18->value,__context__)));
        };
    };
    return das_auto_cast<uint8_t>::cast(__val_rename_at_59_19);
}

inline uint16_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_20, uint16_t __val_rename_at_59_21 )
{
    if ( __a_rename_at_59_20 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_20->value) )
        {
            return das_auto_cast<uint16_t>::cast(uint16_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_20->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_20->value) )
        {
            return das_auto_cast<uint16_t>::cast(uint16_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_20->value,__context__)));
        };
    };
    return das_auto_cast<uint16_t>::cast(__val_rename_at_59_21);
}

inline uint32_t _Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_59_22, uint32_t __val_rename_at_59_23 )
{
    if ( __a_rename_at_59_22 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_59_22->value) )
        {
            return das_auto_cast<uint32_t>::cast(uint32_t(das_get_variant_field<double,8,3>::as(__a_rename_at_59_22->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_59_22->value) )
        {
            return das_auto_cast<uint32_t>::cast(uint32_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_59_22->value,__context__)));
        };
    };
    return das_auto_cast<uint32_t>::cast(__val_rename_at_59_23);
}

inline uint64_t _Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_71_24, uint64_t __val_rename_at_71_25 )
{
    if ( __a_rename_at_71_24 != nullptr )
    {
        if ( das_get_variant_field<double,8,3>::is(__a_rename_at_71_24->value) )
        {
            return das_auto_cast<uint64_t>::cast(uint64_t(das_get_variant_field<double,8,3>::as(__a_rename_at_71_24->value,__context__)));
        } else if ( das_get_variant_field<int64_t,8,4>::is(__a_rename_at_71_24->value) )
        {
            return das_auto_cast<uint64_t>::cast(uint64_t(das_get_variant_field<int64_t,8,4>::as(__a_rename_at_71_24->value,__context__)));
        } else if ( das_get_variant_field<char *,8,2>::is(__a_rename_at_71_24->value) )
        {
            return das_auto_cast<uint64_t>::cast(string_to_uint64(das_get_variant_field<char *,8,2>::as(__a_rename_at_71_24->value,__context__),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))));
        };
    };
    return das_auto_cast<uint64_t>::cast(__val_rename_at_71_25);
}

inline json::JsonValue * _FuncQmarkSqblSqbr_753e250850106ed ( Context * __context__, json::JsonValue const  * const  __a_rename_at_29_30, char * const  __key_rename_at_29_31 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_29_30 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_29_30->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_29_30->value,__context__)),__key_rename_at_29_31,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_a2af24ac467f0328 ( Context * __context__, json::JsonValue * __a_rename_at_34_32, char * const  __key_rename_at_34_33 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_34_32 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_34_32->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_34_32->value,__context__)),__key_rename_at_34_33,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_753e250850106ed ( Context * __context__, json::JsonValue const  * const  __a_rename_at_39_34, char * const  __key_rename_at_39_35 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_39_34 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_39_34->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_39_34->value,__context__)),__key_rename_at_39_35,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkDot_a2af24ac467f0328 ( Context * __context__, json::JsonValue * __a_rename_at_44_36, char * const  __key_rename_at_44_37 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_44_36 != nullptr) && das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::is(__a_rename_at_44_36->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TTable<char *,json::JsonValue *>::safe_index(&(das_get_variant_field<TTable<char *,json::JsonValue *>,8,0>::as(__a_rename_at_44_36->value,__context__)),__key_rename_at_44_37,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_3019c0cddac9a58d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_49_38, int32_t __idx_rename_at_49_39 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_49_38 != nullptr) && das_get_variant_field<TArray<json::JsonValue *>,8,1>::is(__a_rename_at_49_38->value)) ? das_auto_cast<json::JsonValue * const >::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__a_rename_at_49_38->value,__context__)),__idx_rename_at_49_39,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue * const >::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline json::JsonValue * _FuncQmarkSqblSqbr_7fe159c3c5b62bfc ( Context * __context__, json::JsonValue * __a_rename_at_54_40, int32_t __idx_rename_at_54_41 )
{
    return das_auto_cast<json::JsonValue *>::cast(((__a_rename_at_54_40 != nullptr) && das_get_variant_field<TArray<json::JsonValue *>,8,1>::is(__a_rename_at_54_40->value)) ? das_auto_cast<json::JsonValue *>::cast(das_null_coalescing<json::JsonValue *>::get(TArray<json::JsonValue *>::safe_index(&(das_get_variant_field<TArray<json::JsonValue *>,8,1>::as(__a_rename_at_54_40->value,__context__)),__idx_rename_at_54_41,__context__),das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877))))) : das_auto_cast<json::JsonValue *>::cast(das_invoke_function<json::JsonValue *>::invoke(__context__,nullptr,Func(__context__->fnByMangledName(/*@json::JVNull*/ 0xf1223cf59835e877)))));
}

inline double _FuncQmarkQmark_76b74321baba1a32 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_85_42, double __val_rename_at_85_43 )
{
    return das_auto_cast<double>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33(__context__,__a_rename_at_85_42,__val_rename_at_85_43));
}

inline float _FuncQmarkQmark_b332904ef8eb0140 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_90_44, float __val_rename_at_90_45 )
{
    return das_auto_cast<float>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052(__context__,__a_rename_at_90_44,__val_rename_at_90_45));
}

inline int8_t _FuncQmarkQmark_54044768a2cafa63 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_95_46, int8_t __val_rename_at_95_47 )
{
    return das_auto_cast<int8_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466(__context__,__a_rename_at_95_46,__val_rename_at_95_47));
}

inline int16_t _FuncQmarkQmark_b60ba49ced728a6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_100_48, int16_t __val_rename_at_100_49 )
{
    return das_auto_cast<int16_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d(__context__,__a_rename_at_100_48,__val_rename_at_100_49));
}

inline int32_t _FuncQmarkQmark_20384941ec2b26f3 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_105_50, int32_t __val_rename_at_105_51 )
{
    return das_auto_cast<int32_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5(__context__,__a_rename_at_105_50,__val_rename_at_105_51));
}

inline int64_t _FuncQmarkQmark_6f3aa361c92be15d ( Context * __context__, json::JsonValue const  * const  __a_rename_at_110_52, int64_t __val_rename_at_110_53 )
{
    return das_auto_cast<int64_t>::cast(_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf(__context__,__a_rename_at_110_52,__val_rename_at_110_53));
}

inline uint8_t _FuncQmarkQmark_6eaffbb8cc7e4bc7 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_115_54, uint8_t __val_rename_at_115_55 )
{
    return das_auto_cast<uint8_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91(__context__,__a_rename_at_115_54,__val_rename_at_115_55));
}

inline uint16_t _FuncQmarkQmark_fcb53d6726dbe186 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_120_56, uint16_t __val_rename_at_120_57 )
{
    return das_auto_cast<uint16_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674(__context__,__a_rename_at_120_56,__val_rename_at_120_57));
}

inline uint32_t _FuncQmarkQmark_d52965ea200d6e17 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_125_58, uint32_t __val_rename_at_125_59 )
{
    return das_auto_cast<uint32_t>::cast(_Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3(__context__,__a_rename_at_125_58,__val_rename_at_125_59));
}

inline uint64_t _FuncQmarkQmark_5166c54d651a53b1 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_130_60, uint64_t __val_rename_at_130_61 )
{
    return das_auto_cast<uint64_t>::cast(_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5(__context__,__a_rename_at_130_60,__val_rename_at_130_61));
}

inline bool _FuncQmarkQmark_635809dfed370d6 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_135_62, bool __val_rename_at_135_63 )
{
    return das_auto_cast<bool>::cast(((__a_rename_at_135_62 != nullptr) && das_get_variant_field<bool,8,5>::is(__a_rename_at_135_62->value)) ? das_auto_cast<bool>::cast(das_get_variant_field<bool,8,5>::as(__a_rename_at_135_62->value,__context__)) : das_auto_cast<bool>::cast(__val_rename_at_135_63));
}

inline char * _FuncQmarkQmark_eb9a1255c8c914ea ( Context * __context__, json::JsonValue const  * const  __a_rename_at_140_64, char * const  __val_rename_at_140_65 )
{
    return das_auto_cast<char *>::cast(((__a_rename_at_140_64 != nullptr) && das_get_variant_field<char *,8,2>::is(__a_rename_at_140_64->value)) ? das_auto_cast<char * const >::cast(das_get_variant_field<char *,8,2>::as(__a_rename_at_140_64->value,__context__)) : das_auto_cast<char * const >::cast(__val_rename_at_140_65));
}

inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> const  * _FuncQmarkDotTickvalue_5189e68ec5a54919 ( Context * __context__, json::JsonValue const  * const  __a_rename_at_145_66 )
{
    return das_auto_cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> const  *>::cast(das_safe_navigation<json::JsonValue const ,TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_145_66));
}

inline TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> * _FuncQmarkDotTickvalue_9c3125530216994c ( Context * __context__, json::JsonValue * __a_rename_at_150_67 )
{
    return das_auto_cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> *>::cast(das_safe_navigation<json::JsonValue,TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *>,&json::JsonValue::value>::get(__a_rename_at_150_67));
}

inline json_boost::JsonFieldState parse_json_annotation_24121682ba3116e0 ( Context * __context__, char * const  __name_rename_at_434_68, TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  __annotation_rename_at_434_69 )
{
    json_boost::JsonFieldState __fieldState_rename_at_436_70;das_zero(__fieldState_rename_at_436_70);
    das_copy(__fieldState_rename_at_436_70.argName,__name_rename_at_434_68);
    {
        bool __need_loop_438 = true;
        // ann: tuple<name:string;data:variant<tBool:bool;tInt:int;tUInt:uint;tInt64:int64;tUInt64:uint64;tFloat:float;tDouble:double;tString:string;nothing:any> aka RttiValue> const&
        das_iterator<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const > __ann_iterator(__annotation_rename_at_434_69);
        TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>> const  * __ann_rename_at_438_71;
        __need_loop_438 = __ann_iterator.first(__context__,(__ann_rename_at_438_71)) && __need_loop_438;
        for ( ; __need_loop_438 ; __need_loop_438 = __ann_iterator.next(__context__,(__ann_rename_at_438_71)) )
        {
            if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,0>::get((*__ann_rename_at_438_71))),cast<char *>::from(((char *) "rename")),*__context__,nullptr)) && das_get_variant_field<char *,16,7>::is(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71))) )
            {
                das_copy(__fieldState_rename_at_436_70.argName,das_get_variant_field<char *,16,7>::as(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71)),__context__));
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,0>::get((*__ann_rename_at_438_71))),cast<char *>::from(((char *) "enum_as_int")),*__context__,nullptr)) && false )
            {
                das_copy(__fieldState_rename_at_436_70.enumAsInt,das_get_variant_field<bool,16,0>::as(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71)),__context__));
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,0>::get((*__ann_rename_at_438_71))),cast<char *>::from(((char *) "unescape")),*__context__,nullptr)) && false )
            {
                das_copy(__fieldState_rename_at_436_70.unescape,das_get_variant_field<bool,16,0>::as(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71)),__context__));
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,0>::get((*__ann_rename_at_438_71))),cast<char *>::from(((char *) "embed")),*__context__,nullptr)) && false )
            {
                das_copy(__fieldState_rename_at_436_70.embed,das_get_variant_field<bool,16,0>::as(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71)),__context__));
            } else if ( (SimPolicy<char *>::Equ(cast<char *>::from(das_get_tuple_field<char *,0>::get((*__ann_rename_at_438_71))),cast<char *>::from(((char *) "optional")),*__context__,nullptr)) && false )
            {
                das_copy(__fieldState_rename_at_436_70.optional,das_get_variant_field<bool,16,0>::as(das_get_tuple_field<TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>,16>::get((*__ann_rename_at_438_71)),__context__));
            };
        }
        __ann_iterator.close(__context__,(__ann_rename_at_438_71));
    };
    return /* <- */ das_auto_cast_move<json_boost::JsonFieldState>::cast(__fieldState_rename_at_436_70);
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e ( Context * __context__ ) {
    TArray<ast::AstVariantMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstVariantMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstVariantMacro * arg_value = cast_aot_arg<ast::AstVariantMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda ( Context * __context__ ) {
    json_boost::BetterJsonMacro const  &  arg_cl = cast_aot_arg<json_boost::BetterJsonMacro const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda(__context__, arg_cl));
}
static vec4f __wrap__FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024 ( Context * __context__ ) {
    TArray<ast::AstReaderMacro *> &  arg_Arr = cast_aot_arg<TArray<ast::AstReaderMacro *> & >::to(*__context__,__context__->abiArguments()[0]);
    ast::AstReaderMacro * arg_value = cast_aot_arg<ast::AstReaderMacro *>::to(*__context__,__context__->abiArguments()[1]);
    _FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024(__context__, arg_Arr, arg_value);
    return v_zero();
}
static vec4f __wrap__FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3 ( Context * __context__ ) {
    json_boost::JsonReader const  &  arg_cl = cast_aot_arg<json_boost::JsonReader const  & >::to(*__context__,__context__->abiArguments()[0]);
    return cast<StructInfo const  *>::from(_FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3(__context__, arg_cl));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    double arg_val = cast_aot_arg<double>::to(*__context__,__context__->abiArguments()[1]);
    return cast<double>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    float arg_val = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[1]);
    return cast<float>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int8_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int16_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_val = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int64_t>::from(_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint8_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint16_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint32_t>::from(_Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3(__context__, arg_a, arg_val));
}
static vec4f __wrap__Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint64_t>::from(_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkSqblSqbr_753e250850106ed ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_753e250850106ed(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkSqblSqbr_a2af24ac467f0328 ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_a2af24ac467f0328(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkDot_753e250850106ed ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkDot_753e250850106ed(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkDot_a2af24ac467f0328 ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_key = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkDot_a2af24ac467f0328(__context__, arg_a, arg_key));
}
static vec4f __wrap__FuncQmarkSqblSqbr_3019c0cddac9a58d ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_idx = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_3019c0cddac9a58d(__context__, arg_a, arg_idx));
}
static vec4f __wrap__FuncQmarkSqblSqbr_7fe159c3c5b62bfc ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_idx = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<json::JsonValue *>::from(_FuncQmarkSqblSqbr_7fe159c3c5b62bfc(__context__, arg_a, arg_idx));
}
static vec4f __wrap__FuncQmarkQmark_76b74321baba1a32 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    double arg_val = cast_aot_arg<double>::to(*__context__,__context__->abiArguments()[1]);
    return cast<double>::from(_FuncQmarkQmark_76b74321baba1a32(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_b332904ef8eb0140 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    float arg_val = cast_aot_arg<float>::to(*__context__,__context__->abiArguments()[1]);
    return cast<float>::from(_FuncQmarkQmark_b332904ef8eb0140(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_54044768a2cafa63 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int8_t arg_val = cast_aot_arg<int8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int8_t>::from(_FuncQmarkQmark_54044768a2cafa63(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_b60ba49ced728a6 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int16_t arg_val = cast_aot_arg<int16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int16_t>::from(_FuncQmarkQmark_b60ba49ced728a6(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_20384941ec2b26f3 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int32_t arg_val = cast_aot_arg<int32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int32_t>::from(_FuncQmarkQmark_20384941ec2b26f3(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_6f3aa361c92be15d ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    int64_t arg_val = cast_aot_arg<int64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<int64_t>::from(_FuncQmarkQmark_6f3aa361c92be15d(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_6eaffbb8cc7e4bc7 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint8_t arg_val = cast_aot_arg<uint8_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint8_t>::from(_FuncQmarkQmark_6eaffbb8cc7e4bc7(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_fcb53d6726dbe186 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint16_t arg_val = cast_aot_arg<uint16_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint16_t>::from(_FuncQmarkQmark_fcb53d6726dbe186(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_d52965ea200d6e17 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint32_t arg_val = cast_aot_arg<uint32_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint32_t>::from(_FuncQmarkQmark_d52965ea200d6e17(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_5166c54d651a53b1 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    uint64_t arg_val = cast_aot_arg<uint64_t>::to(*__context__,__context__->abiArguments()[1]);
    return cast<uint64_t>::from(_FuncQmarkQmark_5166c54d651a53b1(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_635809dfed370d6 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    bool arg_val = cast_aot_arg<bool>::to(*__context__,__context__->abiArguments()[1]);
    return cast<bool>::from(_FuncQmarkQmark_635809dfed370d6(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkQmark_eb9a1255c8c914ea ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    char * const  arg_val = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[1]);
    return cast<char *>::from(_FuncQmarkQmark_eb9a1255c8c914ea(__context__, arg_a, arg_val));
}
static vec4f __wrap__FuncQmarkDotTickvalue_5189e68ec5a54919 ( Context * __context__ ) {
    json::JsonValue const  * const  arg_a = cast_aot_arg<json::JsonValue const  * const >::to(*__context__,__context__->abiArguments()[0]);
    return cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> const  *>::from(_FuncQmarkDotTickvalue_5189e68ec5a54919(__context__, arg_a));
}
static vec4f __wrap__FuncQmarkDotTickvalue_9c3125530216994c ( Context * __context__ ) {
    json::JsonValue * arg_a = cast_aot_arg<json::JsonValue *>::to(*__context__,__context__->abiArguments()[0]);
    return cast<TVariant<56,8,TTable<char *,json::JsonValue *>,TArray<json::JsonValue *>,char *,double,int64_t,bool,void *> *>::from(_FuncQmarkDotTickvalue_9c3125530216994c(__context__, arg_a));
}
static vec4f __wrap_parse_json_annotation_24121682ba3116e0 ( Context * __context__ ) {
    char * const  arg_name = cast_aot_arg<char * const >::to(*__context__,__context__->abiArguments()[0]);
    TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  &  arg_annotation = cast_aot_arg<TArray<TTuple<48,char *,TVariant<32,16,bool,int32_t,uint32_t,int64_t,uint64_t,float,double,char *,vec4f>>> const  & >::to(*__context__,__context__->abiArguments()[1]);
    *((json_boost::JsonFieldState *) __context__->abiCMRES) = parse_json_annotation_24121682ba3116e0(__context__, arg_name, arg_annotation);
    return v_zero();
}

#pragma optimize("", off)
struct AotFunction { uint64_t hash; bool is_cmres; void * fn; vec4f (*wrappedFn)(Context*); };
static AotFunction functions[] = {
    { 0x4bc10cea9c6c5a94, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e, &__wrap__FuncbuiltinTickpushTick10769833213962245646_7c8959f47ea9218e },
    { 0x683811fa0be41c33, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_669e6d53ae064eda },
    { 0x8c4eedb94286e01, false, (void*)&_FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024, &__wrap__FuncbuiltinTickpushTick10769833213962245646_ab4200c8fbb65024 },
    { 0xdba804280866317d, false, (void*)&_FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3, &__wrap__FuncrttiTickclass_infoTick15801393167907430156_4db5277856321b3 },
    { 0x6ade125f957ecb47, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_ae338a9988af0e33 },
    { 0x56ae60ba3c0d6c22, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a1ccaaed10857052 },
    { 0x1ad7818d37db8bf1, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_1494c2d88e6f2466 },
    { 0xa94ce42a831b09c7, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a5733ffe0103b68d },
    { 0xfca3269f08ccdae2, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_dfde84f6740636a5 },
    { 0xf8dd6edc954cab16, false, (void*)&_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf, &__wrap__Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_3d9bcef4d5be24cf },
    { 0xa72dc6691dcbb1d7, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_67c2e92220650f91 },
    { 0x980a76bc87136f1, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_a973192a3169d674 },
    { 0x57fef537832fd5e2, false, (void*)&_Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3, &__wrap__Funcjson_boostTicknull_coalescingTick1995797521634879908_336685577521e9f3 },
    { 0xa1c1b4c6a3a84a30, false, (void*)&_Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5, &__wrap__Funcjson_boostTicknull_coalescing_with_stringTick3281489010087647503_f23de3d46dae4fd5 },
    { 0x5c62d71e88b06664, false, (void*)&_FuncQmarkSqblSqbr_753e250850106ed, &__wrap__FuncQmarkSqblSqbr_753e250850106ed },
    { 0xcb87d31d2468865, false, (void*)&_FuncQmarkSqblSqbr_a2af24ac467f0328, &__wrap__FuncQmarkSqblSqbr_a2af24ac467f0328 },
    { 0x5c62d71e88b06664, false, (void*)&_FuncQmarkDot_753e250850106ed, &__wrap__FuncQmarkDot_753e250850106ed },
    { 0xcb87d31d2468865, false, (void*)&_FuncQmarkDot_a2af24ac467f0328, &__wrap__FuncQmarkDot_a2af24ac467f0328 },
    { 0x70999a99bc0e53b4, false, (void*)&_FuncQmarkSqblSqbr_3019c0cddac9a58d, &__wrap__FuncQmarkSqblSqbr_3019c0cddac9a58d },
    { 0x32ac3346567329d5, false, (void*)&_FuncQmarkSqblSqbr_7fe159c3c5b62bfc, &__wrap__FuncQmarkSqblSqbr_7fe159c3c5b62bfc },
    { 0xbfec861f10808753, false, (void*)&_FuncQmarkQmark_76b74321baba1a32, &__wrap__FuncQmarkQmark_76b74321baba1a32 },
    { 0xda0c4fafe676db21, false, (void*)&_FuncQmarkQmark_b332904ef8eb0140, &__wrap__FuncQmarkQmark_b332904ef8eb0140 },
    { 0x339a8dc9087204f6, false, (void*)&_FuncQmarkQmark_54044768a2cafa63, &__wrap__FuncQmarkQmark_54044768a2cafa63 },
    { 0xdf1a23bd3a919eeb, false, (void*)&_FuncQmarkQmark_b60ba49ced728a6, &__wrap__FuncQmarkQmark_b60ba49ced728a6 },
    { 0x9921e8c23aabf827, false, (void*)&_FuncQmarkQmark_20384941ec2b26f3, &__wrap__FuncQmarkQmark_20384941ec2b26f3 },
    { 0x2e4ff1d8cc1a2978, false, (void*)&_FuncQmarkQmark_6f3aa361c92be15d, &__wrap__FuncQmarkQmark_6f3aa361c92be15d },
    { 0x71c94332f1479b2c, false, (void*)&_FuncQmarkQmark_6eaffbb8cc7e4bc7, &__wrap__FuncQmarkQmark_6eaffbb8cc7e4bc7 },
    { 0x68d02a178d6d0a3e, false, (void*)&_FuncQmarkQmark_fcb53d6726dbe186, &__wrap__FuncQmarkQmark_fcb53d6726dbe186 },
    { 0xff2f4a97b181587a, false, (void*)&_FuncQmarkQmark_d52965ea200d6e17, &__wrap__FuncQmarkQmark_d52965ea200d6e17 },
    { 0x4da071a42539c7e0, false, (void*)&_FuncQmarkQmark_5166c54d651a53b1, &__wrap__FuncQmarkQmark_5166c54d651a53b1 },
    { 0xb9b4623fe7981a45, false, (void*)&_FuncQmarkQmark_635809dfed370d6, &__wrap__FuncQmarkQmark_635809dfed370d6 },
    { 0x650dfca550d83e9d, false, (void*)&_FuncQmarkQmark_eb9a1255c8c914ea, &__wrap__FuncQmarkQmark_eb9a1255c8c914ea },
    { 0xc33c470a5a27fd54, false, (void*)&_FuncQmarkDotTickvalue_5189e68ec5a54919, &__wrap__FuncQmarkDotTickvalue_5189e68ec5a54919 },
    { 0x899e617ee4a7f78f, false, (void*)&_FuncQmarkDotTickvalue_9c3125530216994c, &__wrap__FuncQmarkDotTickvalue_9c3125530216994c },
    { 0x31fe09c09a0e8c08, true, (void*)&parse_json_annotation_24121682ba3116e0, &__wrap_parse_json_annotation_24121682ba3116e0 },
};
#pragma optimize("", on)

static void registerAotFunctions ( AotLibrary & aotLib ) {
    for (const auto &[hash, cmres, fn1, fn2] : functions) {
        aotLib.emplace(hash, AotFactory(cmres, fn1, fn2));
    }
    resolveTypeInfoAnnotations();
}

} // namespace _anon_14538537767599647147
AotListBase impl_aot_json_boost(_anon_14538537767599647147::registerAotFunctions);
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
