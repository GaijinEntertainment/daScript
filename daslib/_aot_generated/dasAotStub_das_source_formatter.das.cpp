#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require math
#include "daScript/simulate/aot_builtin_math.h"
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
 // require defer
 // require das_source_formatter

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
namespace _anon_15051537126579498552 {

namespace das_source_formatter { struct TokenTemplate; };
namespace das_source_formatter { struct Token; };
namespace das_source_formatter { struct FormatterToken; };
namespace das_source_formatter { struct FormatterCtx; };
namespace das_source_formatter { struct ParenCounter; };
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
// unused structure DeferMacro
// unused structure DeferDeleteMacro
namespace das_source_formatter {

enum class TokenType : int32_t {
    UNKNOWN = int32_t(0),
    KEYWORD_OR_IDENTIFIER = int32_t(1),
    NUMBER = int32_t(2),
    COMMENT = int32_t(3),
    STRING = int32_t(4),
};
}
namespace das_source_formatter {

struct TokenTemplate {
    int32_t firstChar;
    char * str;
    TArray<uint8_t> codes;
    int32_t length;
};
}
namespace das_source_formatter {

struct Token {
    char * str;
    int32_t spaces;
    int32_t newLines;
    int32_t lineInSource;
    int32_t column;
    DAS_COMMENT(enum) das_source_formatter::TokenType tokenType;
    bool isInFunctionParam;
    bool isInType;
    bool dontFormat;
    bool dontAddSpacesAround;
};
}
namespace das_source_formatter {

struct FormatterToken {
    int32_t tokenIndex;
    char * command;
};
}
namespace das_source_formatter {

struct FormatterCtx {
    TArray<das_source_formatter::TokenTemplate> tokenTemplates;
    TArray<das_source_formatter::Token> tokens;
    TArray<das_source_formatter::FormatterToken> formatterTokens;
    int32_t pos;
    int32_t c;
    TArray<uint8_t> data;
    bool eof;
    bool haveUtf8Bom;
    int32_t newLineCounter;
    int32_t spaceCounter;
    int32_t srcLine;
    int32_t curColumn;
    int32_t curLineIndex;
    bool debugMode;
    int32_t indenting;
    bool insideOptions;
    int32_t crCount;
    int32_t lfCount;
};
}
namespace das_source_formatter {

struct ParenCounter {
    int32_t angle;
    int32_t paren;
    int32_t square;
    int32_t curly;
};
}
extern StructInfo __struct_info__bf67b648e8f141ed;
extern TypeInfo __type_info__44217c02c8384397;
extern TypeInfo __type_info__4ac1d999a882997b;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__6cf57a81c8f2e7f7;
extern TypeInfo __type_info__a8d2247d5b8ed91f;
extern TypeInfo __type_info__7dcc2c583c294a1f;
extern TypeInfo __type_info__af5be84c85f468f0;
extern VarInfo __var_info__e807e1b774135f34;
extern VarInfo __var_info__f1744ecb99b5ee52;
extern VarInfo __var_info__12c1af86475d4616;
extern VarInfo __var_info__993f8b12277aa369;

VarInfo __struct_info__bf67b648e8f141ed_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xf1744ecb99b5ee52), "firstChar", offsetof(das_source_formatter::TokenTemplate,firstChar), 0 };
VarInfo __struct_info__bf67b648e8f141ed_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x993f8b12277aa369), "str", offsetof(das_source_formatter::TokenTemplate,str), 2 };
VarInfo __struct_info__bf67b648e8f141ed_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xe807e1b774135f34), "codes", offsetof(das_source_formatter::TokenTemplate,codes), 4 };
VarInfo __struct_info__bf67b648e8f141ed_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x12c1af86475d4616), "length", offsetof(das_source_formatter::TokenTemplate,length), 0 };
VarInfo * __struct_info__bf67b648e8f141ed_fields[4] =  { &__struct_info__bf67b648e8f141ed_field_0, &__struct_info__bf67b648e8f141ed_field_1, &__struct_info__bf67b648e8f141ed_field_2, &__struct_info__bf67b648e8f141ed_field_3 };
StructInfo __struct_info__bf67b648e8f141ed = {"TokenTemplate", "das_source_formatter", 28, __struct_info__bf67b648e8f141ed_fields, 4, 48, UINT64_C(0x0), nullptr, UINT64_C(0xbf67b648e8f141ed), 1 };
TypeInfo __type_info__44217c02c8384397 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6cf57a81c8f2e7f7, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x44217c02c8384397) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__6cf57a81c8f2e7f7 = { Type::tStructure, &__struct_info__bf67b648e8f141ed, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x6cf57a81c8f2e7f7) };
TypeInfo __type_info__a8d2247d5b8ed91f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xa8d2247d5b8ed91f) };
TypeInfo __type_info__7dcc2c583c294a1f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x7dcc2c583c294a1f) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__4ac1d999a882997b };
TypeInfo * __tinfo_1[1] = { &__type_info__44217c02c8384397 };
TypeInfo * __tinfo_2[1] = { &__type_info__a8d2247d5b8ed91f };
TypeInfo * __tinfo_3[1] = { &__type_info__7dcc2c583c294a1f };
TypeInfo * __tinfo_4[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void finalize_6a9d04f899c816ac ( Context * __context__, das_source_formatter::TokenTemplate & ____this_rename_at_40_0 );
inline void finalize_597eb4e562707157 ( Context * __context__, das_source_formatter::Token & ____this_rename_at_48_1 );
inline void finalize_d6c2d9467df86ea8 ( Context * __context__, das_source_formatter::FormatterToken & ____this_rename_at_62_2 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_3 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_37238c3cdaec478a ( Context * __context__, TDim<char *,22> const  & __a_rename_at_581_4 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_94b9be375031ddbc ( Context * __context__, TDim<char *,21> const  & __a_rename_at_581_5 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_9778601c0167a631 ( Context * __context__, TDim<char *,11> const  & __a_rename_at_581_6 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_7, int32_t __newSize_rename_at_68_8 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5f7a4e8904f8977e ( Context * __context__, TArray<das_source_formatter::TokenTemplate> & __a_rename_at_1182_9 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_d46b5589c5f4f04b ( Context * __context__, TArray<das_source_formatter::Token> & __a_rename_at_1182_11 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_b4e09f66b8118eb0 ( Context * __context__, TArray<das_source_formatter::FormatterToken> & __a_rename_at_1182_13 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_4a7556d56e5a6ac1 ( Context * __context__, TArray<das_source_formatter::TokenTemplate> & __Arr_rename_at_68_15, int32_t __newSize_rename_at_68_16 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53 ( Context * __context__, TTable<char *,void> const  & __Tab_rename_at_1014_17, char * const  __at_rename_at_1014_18 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c ( Context * __context__, TArray<uint8_t> & __a_rename_at_32_19, TArray<uint8_t> & __b_rename_at_32_20 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  & __a_rename_at_581_21 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_540ecf6ef9e01052 ( Context * __context__, TDim<char *,40> const  & __a_rename_at_581_22 );
inline void _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_165_23, das_source_formatter::Token const  & __value_rename_at_165_24 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a ( Context * __context__, TDim<char *,22> const  & __arr_rename_at_1658_25, char * const  __key_rename_at_1658_26 );
inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768 ( Context * __context__, TDim<char *,8> const  & __a_rename_at_1417_28 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_125_31, int32_t __newSize_rename_at_125_32 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_33 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3 ( Context * __context__, TDim<char *,21> const  & __arr_rename_at_1658_34, char * const  __key_rename_at_1658_35 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622 ( Context * __context__, TDim<char *,11> const  & __arr_rename_at_1658_37, char * const  __key_rename_at_1658_38 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_40, TArray<uint8_t> const  & __b_rename_at_1080_41 );
inline void _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_181_43, das_source_formatter::Token & __value_rename_at_181_44 );
inline void _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce ( Context * __context__, TArray<das_source_formatter::FormatterToken> & __Arr_rename_at_181_45, das_source_formatter::FormatterToken & __value_rename_at_181_46 );
inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_944_47, int32_t __i_rename_at_944_48 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_49, uint8_t __value_rename_at_165_50 );
inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_936_51, int32_t __i_rename_at_936_52 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856 ( Context * __context__, TDim<char *,3> const  & __arr_rename_at_1658_53, char * const  __key_rename_at_1658_54 );
inline void finalize_b9f001def6083360 ( Context * __context__, das_source_formatter::FormatterCtx & ____this_rename_at_68_56 );
inline void initialize_token_templates_cd924eed93b743df ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_89_57 );
inline bool isUtf8Bom_fc921f39aa33ab23 ( Context * __context__, TArray<uint8_t> & __text_rename_at_103_64 );
inline bool eq_1a08ceae13f47e89 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_109_65, int32_t __index_rename_at_109_66, char * const  __s_rename_at_109_67 );
inline bool eq_32a38f27b5380f0f ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_114_68, int32_t __index_rename_at_114_69, char * const  __s0_rename_at_114_70, char * const  __s1_rename_at_114_71 );
inline bool eq_f43d812e3a992e4 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_120_72, int32_t __index_rename_at_120_73, char * const  __s0_rename_at_120_74, char * const  __s1_rename_at_120_75, char * const  __s2_rename_at_120_76 );
inline bool eq_886fe8d1ff63f5fd ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_126_77, int32_t __index_rename_at_126_78, char * const  __s0_rename_at_126_79, char * const  __s1_rename_at_126_80, char * const  __s2_rename_at_126_81, char * const  __s3_rename_at_126_82 );
inline bool new_line_before_56d9a06feda36687 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_132_83, int32_t __index_rename_at_132_84 );
inline void next_char_e34bb0a357da1559 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_137_85 );
inline void skip_spaces_a5027bfd73b7bde6 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_179_87 );
inline void on_token_added_6925eddc2a7ca309 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_186_88 );
inline void push_token_92f9637f04a53e36 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_204_89, int32_t __from_char_idx_rename_at_204_90, int32_t __to_char_idx_rename_at_204_91, int32_t __space_count_rename_at_204_92, int32_t __new_lines_rename_at_204_93, int32_t __src_line_rename_at_204_94, int32_t __column_rename_at_204_95, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_96 );
inline void parse_token_95969fa7ff7b12c4 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_220_100 );
inline void parse_all_tokens_b5d5b68330cdfcd0 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_344_119 );
inline void update_paren_62e64d469829ae3d ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_388_122, int32_t __index_rename_at_388_123, das_source_formatter::ParenCounter & __paren_counter_rename_at_388_124 );
inline int32_t find_pair_paren_90acbe7a3b864d93 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_425_125, int32_t __from_index_rename_at_425_126 );
inline int32_t find_pair_square_68d9ea1d7c30e49f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_437_129, int32_t __from_index_rename_at_437_130 );
inline int32_t find_pair_angle_89495f7d100107b7 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_449_133, int32_t __from_index_rename_at_449_134 );
inline void mark_range_as_type_24b8f876667238a0 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_461_137, int32_t __from_rename_at_461_138, int32_t __to_rename_at_461_139 );
inline void mark_tokens_as_type_5e4d056374f6927c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_470_141, int32_t __i_rename_at_470_142 );
inline void mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_528_145, int32_t __i_rename_at_528_146 );
inline int32_t search_token_in_line_3a9d4fd472c436b3 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_542_149, int32_t __from_index_rename_at_542_150, char * const  __str_rename_at_542_151 );
inline void process_formatter_tokens_a63e2ffa4c8206b6 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_564_154 );
inline bool have_formatter_token_296d7fc158126671 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_581_160, char * const  __command_rename_at_581_161 );
inline void mark_token_context_7dbf70435b518a0a ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_591_163 );
inline void fmt_function_arguments_7d9f22d6302573cb ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_791_197 );
inline void fmt_remove_space_before_args_26ba700ce1a53dbc ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_832_203 );
inline void debug_print_tokens_927617e00a5e50a7 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_841_205 );
inline char * generate_source_64706a1ce7db4983 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_848_207 );
inline bool need_spaces_around_7179a668ae90ccd6 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_891_216, int32_t __index_rename_at_891_217 );
inline bool need_space_only_before_584e48acb8e8a70 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_912_218, int32_t __index_rename_at_912_219 );
inline bool is_unary_62ad92645c7f9ee6 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_956_220, int32_t __index_rename_at_956_221 );
inline void remove_spaces_around_1c96d2bcc36445d1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_979_223, int32_t __index_rename_at_979_224 );
inline void make_spaces_around_9b0360b9b4d718c3 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_989_225, int32_t __index_rename_at_989_226 );
inline void fmt_space_after_comma_7aa6a2faf3ce3f4f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_999_227 );
inline void fmt_space_after_semicolon_29caf5fd09f6497e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1008_229 );
inline void fmt_remove_space_before_comma_54b9cd4ba70794e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1017_231 );
inline void fmt_remove_space_before_semicolon_132b81d1431b5d49 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1026_233 );
inline void fmt_type_colon_a5b252566f4d584 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1035_235 );
inline void fmt_function_decl_param_paren_6c291b7c3a975893 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1047_237 );
inline void fmt_function_call_param_paren_call_f740219cd0547b48 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1062_239 );
inline void fmt_spaces_around_operators_9837b8f174165da7 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1073_242 );
inline void fmt_space_after_keyword_2aab289129cf4fc8 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1092_244 );
inline void fmt_space_after_paren_a2ef6e80a339831f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1109_246 );
inline void fmt_space_after_square_brackets_15fb170c80c43d93 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1118_248 );
inline void fmt_remove_space_inside_parens_fc44c6bd3c600b9c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1127_250 );
inline void fmt_remove_space_before_as_bb9fc0cabc4ff729 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1139_252 );
inline void fmt_remove_space_before_end_of_object_64e24a59c1d5b6b ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1148_254 );
inline void fmt_remove_space_before_object_type_298f1dcfc2f7829c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1158_256 );
inline void fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1168_258 );
inline void fmt_glue_type_specifiers_cb3a1c8b879530ae ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1180_260 );
inline void fmt_space_after_cast_type_446660f13b2ec290 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1200_262 );
inline void fmt_destructure_space_e9871049a6de7d91 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1215_265 );
inline void do_format_4835418a8469e5fa ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1224_267 );
inline char * format_source_string_f69d100c71c99a8c ( Context * __context__, char * const  & __file_data_rename_at_1267_271 );
inline char * format_source_58c20f6bcb79eabd ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1276_275, TArray<uint8_t> const  & __file_data_rename_at_1276_276 );
inline char * format_source_f3ae203349d40bcb ( Context * __context__, TArray<uint8_t> const  & __file_data_rename_at_1284_277 );
inline das_source_formatter::TokenTemplate TokenTemplate_a02a7415fdb9ba5a ( Context * __context__ );
inline das_source_formatter::Token Token_c2eeda936a4bfe12 ( Context * __context__ );
inline das_source_formatter::FormatterCtx FormatterCtx_d786b6c38dffada3 ( Context * __context__ );
inline das_source_formatter::ParenCounter ParenCounter_77d11a7861c772f0 ( Context * __context__ );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<TDim<char *,40>,0x3e3c534a20a075ae>(__context__) = (([&]() -> TDim<char *,40> {
        TDim<char *,40> __mka_14;
        __mka_14(0,__context__) = ((char *) "<<<=");
        __mka_14(1,__context__) = ((char *) ">>>=");
        __mka_14(2,__context__) = ((char *) "<<=");
        __mka_14(3,__context__) = ((char *) ">>=");
        __mka_14(4,__context__) = ((char *) "&&=");
        __mka_14(5,__context__) = ((char *) "||=");
        __mka_14(6,__context__) = ((char *) "^^=");
        __mka_14(7,__context__) = ((char *) "<<<");
        __mka_14(8,__context__) = ((char *) ">>>");
        __mka_14(9,__context__) = ((char *) "::");
        __mka_14(10,__context__) = ((char *) "+=");
        __mka_14(11,__context__) = ((char *) "-=");
        __mka_14(12,__context__) = ((char *) "/=");
        __mka_14(13,__context__) = ((char *) "*=");
        __mka_14(14,__context__) = ((char *) "%=");
        __mka_14(15,__context__) = ((char *) "|=");
        __mka_14(16,__context__) = ((char *) "&=");
        __mka_14(17,__context__) = ((char *) "^=");
        __mka_14(18,__context__) = ((char *) "<<");
        __mka_14(19,__context__) = ((char *) ">>");
        __mka_14(20,__context__) = ((char *) "++");
        __mka_14(21,__context__) = ((char *) "--");
        __mka_14(22,__context__) = ((char *) "<=");
        __mka_14(23,__context__) = ((char *) ">=");
        __mka_14(24,__context__) = ((char *) "==");
        __mka_14(25,__context__) = ((char *) "!=");
        __mka_14(26,__context__) = ((char *) "->");
        __mka_14(27,__context__) = ((char *) "<-");
        __mka_14(28,__context__) = ((char *) "??");
        __mka_14(29,__context__) = ((char *) "?.");
        __mka_14(30,__context__) = ((char *) "?[");
        __mka_14(31,__context__) = ((char *) "<|");
        __mka_14(32,__context__) = ((char *) "|>");
        __mka_14(33,__context__) = ((char *) ":=");
        __mka_14(34,__context__) = ((char *) "=>");
        __mka_14(35,__context__) = ((char *) "@@");
        __mka_14(36,__context__) = ((char *) "&&");
        __mka_14(37,__context__) = ((char *) "||");
        __mka_14(38,__context__) = ((char *) "^^");
        __mka_14(39,__context__) = ((char *) "%%");
        return __mka_14;
    })());/*sorted_tokens_source*/
    das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) = (([&]() -> TDim<char *,21> {
        TDim<char *,21> __mka_21;
        __mka_21(0,__context__) = ((char *) "(");
        __mka_21(1,__context__) = ((char *) ")");
        __mka_21(2,__context__) = ((char *) "[");
        __mka_21(3,__context__) = ((char *) "]");
        __mka_21(4,__context__) = ((char *) "{");
        __mka_21(5,__context__) = ((char *) "}");
        __mka_21(6,__context__) = ((char *) ".");
        __mka_21(7,__context__) = ((char *) ";");
        __mka_21(8,__context__) = ((char *) ",");
        __mka_21(9,__context__) = ((char *) "`");
        __mka_21(10,__context__) = ((char *) "::");
        __mka_21(11,__context__) = ((char *) "++");
        __mka_21(12,__context__) = ((char *) "--");
        __mka_21(13,__context__) = ((char *) "?.");
        __mka_21(14,__context__) = ((char *) "?[");
        __mka_21(15,__context__) = ((char *) "@@");
        __mka_21(16,__context__) = ((char *) "!");
        __mka_21(17,__context__) = ((char *) "~");
        __mka_21(18,__context__) = ((char *) "#");
        __mka_21(19,__context__) = ((char *) "->");
        __mka_21(20,__context__) = ((char *) "$");
        return __mka_21;
    })());/*dont_need_space_around*/
    das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) = (([&]() -> TDim<char *,22> {
        TDim<char *,22> __mka_25;
        __mka_25(0,__context__) = ((char *) "generator");
        __mka_25(1,__context__) = ((char *) "cast");
        __mka_25(2,__context__) = ((char *) "upcast");
        __mka_25(3,__context__) = ((char *) "smart_ptr");
        __mka_25(4,__context__) = ((char *) "match_type");
        __mka_25(5,__context__) = ((char *) "type");
        __mka_25(6,__context__) = ((char *) "reinterpret");
        __mka_25(7,__context__) = ((char *) "variant");
        __mka_25(8,__context__) = ((char *) "variant_index");
        __mka_25(9,__context__) = ((char *) "function");
        __mka_25(10,__context__) = ((char *) "has_field");
        __mka_25(11,__context__) = ((char *) "default");
        __mka_25(12,__context__) = ((char *) "array");
        __mka_25(13,__context__) = ((char *) "fixed_array");
        __mka_25(14,__context__) = ((char *) "table");
        __mka_25(15,__context__) = ((char *) "iterator");
        __mka_25(16,__context__) = ((char *) "tuple");
        __mka_25(17,__context__) = ((char *) "struct_get_annotation_argument");
        __mka_25(18,__context__) = ((char *) "struct_has_annotation_argument");
        __mka_25(19,__context__) = ((char *) "class");
        __mka_25(20,__context__) = ((char *) "block");
        __mka_25(21,__context__) = ((char *) "lambda");
        return __mka_25;
    })());/*type_after_keyword*/
    das_global<TDim<char *,21>,0xd52f4fdee217c055>(__context__) = (([&]() -> TDim<char *,21> {
        TDim<char *,21> __mka_952;
        __mka_952(0,__context__) = ((char *) "(");
        __mka_952(1,__context__) = ((char *) "[");
        __mka_952(2,__context__) = ((char *) "?[");
        __mka_952(3,__context__) = ((char *) "{");
        __mka_952(4,__context__) = ((char *) "auto");
        __mka_952(5,__context__) = ((char *) "int");
        __mka_952(6,__context__) = ((char *) "float");
        __mka_952(7,__context__) = ((char *) "int16");
        __mka_952(8,__context__) = ((char *) "int32");
        __mka_952(9,__context__) = ((char *) "int8");
        __mka_952(10,__context__) = ((char *) ";");
        __mka_952(11,__context__) = ((char *) ",");
        __mka_952(12,__context__) = ((char *) "if");
        __mka_952(13,__context__) = ((char *) "elif");
        __mka_952(14,__context__) = ((char *) "while");
        __mka_952(15,__context__) = ((char *) "switch");
        __mka_952(16,__context__) = ((char *) "return");
        __mka_952(17,__context__) = ((char *) "yield");
        __mka_952(18,__context__) = ((char *) "in");
        __mka_952(19,__context__) = ((char *) "=");
        __mka_952(20,__context__) = ((char *) "!");
        return __mka_952;
    })());/*tokens_before_unary*/
    das_global<TDim<char *,11>,0x94e2878ce5158877>(__context__) = (([&]() -> TDim<char *,11> {
        TDim<char *,11> __mka_1089;
        __mka_1089(0,__context__) = ((char *) "if");
        __mka_1089(1,__context__) = ((char *) "elif");
        __mka_1089(2,__context__) = ((char *) "while");
        __mka_1089(3,__context__) = ((char *) "switch");
        __mka_1089(4,__context__) = ((char *) "with");
        __mka_1089(5,__context__) = ((char *) "yield");
        __mka_1089(6,__context__) = ((char *) "catch");
        __mka_1089(7,__context__) = ((char *) "static_if");
        __mka_1089(8,__context__) = ((char *) "static_elif");
        __mka_1089(9,__context__) = ((char *) "for");
        __mka_1089(10,__context__) = ((char *) "in");
        return __mka_1089;
    })());/*space_after_keywords*/
    das_global<TDim<char *,3>,0xca374635aa15cd66>(__context__) = (([&]() -> TDim<char *,3> {
        TDim<char *,3> __mka_1178;
        __mka_1178(0,__context__) = ((char *) "?");
        __mka_1178(1,__context__) = ((char *) "&");
        __mka_1178(2,__context__) = ((char *) "#");
        return __mka_1178;
    })());/*type_specifiers*/
}

inline void finalize_6a9d04f899c816ac ( Context * __context__, das_source_formatter::TokenTemplate &  ____this_rename_at_40_0 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_40_0.codes));
    memset((void*)&(____this_rename_at_40_0), 0, TypeSize<das_source_formatter::TokenTemplate>::size);
}

inline void finalize_597eb4e562707157 ( Context * __context__, das_source_formatter::Token &  ____this_rename_at_48_1 )
{
    memset((void*)&(____this_rename_at_48_1), 0, TypeSize<das_source_formatter::Token>::size);
}

inline void finalize_d6c2d9467df86ea8 ( Context * __context__, das_source_formatter::FormatterToken &  ____this_rename_at_62_2 )
{
    memset((void*)&(____this_rename_at_62_2), 0, TypeSize<das_source_formatter::FormatterToken>::size);
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_3 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_3)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_37238c3cdaec478a ( Context * __context__, TDim<char *,22> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(22);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_94b9be375031ddbc ( Context * __context__, TDim<char *,21> const  &  __a_rename_at_581_5 )
{
    return das_auto_cast<int32_t>::cast(21);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_9778601c0167a631 ( Context * __context__, TDim<char *,11> const  &  __a_rename_at_581_6 )
{
    return das_auto_cast<int32_t>::cast(11);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_7, int32_t __newSize_rename_at_68_8 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_7),__newSize_rename_at_68_8,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5f7a4e8904f8977e ( Context * __context__, TArray<das_source_formatter::TokenTemplate> &  __a_rename_at_1182_9 )
{
    {
        bool __need_loop_1184 = true;
        // aV: das_source_formatter::TokenTemplate aka TT&
        das_iterator<TArray<das_source_formatter::TokenTemplate>> __aV_iterator(__a_rename_at_1182_9);
        das_source_formatter::TokenTemplate * __aV_rename_at_1184_10;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_10)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_10)) )
        {
            finalize_6a9d04f899c816ac(__context__,das_arg<das_source_formatter::TokenTemplate>::pass((*__aV_rename_at_1184_10)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_10));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__a_rename_at_1182_9),48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_d46b5589c5f4f04b ( Context * __context__, TArray<das_source_formatter::Token> &  __a_rename_at_1182_11 )
{
    {
        bool __need_loop_1184 = true;
        // aV: das_source_formatter::Token aka TT&
        das_iterator<TArray<das_source_formatter::Token>> __aV_iterator(__a_rename_at_1182_11);
        das_source_formatter::Token * __aV_rename_at_1184_12;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_12)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_12)) )
        {
            finalize_597eb4e562707157(__context__,das_arg<das_source_formatter::Token>::pass((*__aV_rename_at_1184_12)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_12));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::Token>>::pass(__a_rename_at_1182_11),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_b4e09f66b8118eb0 ( Context * __context__, TArray<das_source_formatter::FormatterToken> &  __a_rename_at_1182_13 )
{
    {
        bool __need_loop_1184 = true;
        // aV: das_source_formatter::FormatterToken aka TT&
        das_iterator<TArray<das_source_formatter::FormatterToken>> __aV_iterator(__a_rename_at_1182_13);
        das_source_formatter::FormatterToken * __aV_rename_at_1184_14;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_14)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_14)) )
        {
            finalize_d6c2d9467df86ea8(__context__,das_arg<das_source_formatter::FormatterToken>::pass((*__aV_rename_at_1184_14)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_14));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__a_rename_at_1182_13),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_4a7556d56e5a6ac1 ( Context * __context__, TArray<das_source_formatter::TokenTemplate> &  __Arr_rename_at_68_15, int32_t __newSize_rename_at_68_16 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<das_source_formatter::TokenTemplate> &>::from(__Arr_rename_at_68_15)));
    builtin_array_resize(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__Arr_rename_at_68_15),__newSize_rename_at_68_16,48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53 ( Context * __context__, TTable<char *,void> const  &  __Tab_rename_at_1014_17, char * const  __at_rename_at_1014_18 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1014_17,__at_rename_at_1014_18));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c ( Context * __context__, TArray<uint8_t> &  __a_rename_at_32_19, TArray<uint8_t> &  __b_rename_at_32_20 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<uint8_t> &>::from(__a_rename_at_32_19)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_3,cast<TArray<uint8_t> &>::from(__b_rename_at_32_20)));
    das_move(__a_rename_at_32_19,__b_rename_at_32_20);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  &  __a_rename_at_581_21 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_540ecf6ef9e01052 ( Context * __context__, TDim<char *,40> const  &  __a_rename_at_581_22 )
{
    return das_auto_cast<int32_t>::cast(40);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_165_23, das_source_formatter::Token const  &  __value_rename_at_165_24 )
{
    das_copy(__Arr_rename_at_165_23(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_165_23),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_24);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a ( Context * __context__, TDim<char *,22> const  &  __arr_rename_at_1658_25, char * const  __key_rename_at_1658_26 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,22));
        int32_t __i_rename_at_1659_27;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_27)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_27)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_25(__i_rename_at_1659_27,__context__)),cast<char *>::from(__key_rename_at_1658_26),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_27);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_27));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768 ( Context * __context__, TDim<char *,8> const  &  __a_rename_at_1417_28 )
{
    TTable<char *,void> __tab_rename_at_1418_29;das_zero(__tab_rename_at_1418_29);
    {
        bool __need_loop_1419 = true;
        // at: string aka keyT const&
        das_iterator<TDim<char *,8> const > __at_iterator(__a_rename_at_1417_28);
        char * const  * __at_rename_at_1419_30;
        __need_loop_1419 = __at_iterator.first(__context__,(__at_rename_at_1419_30)) && __need_loop_1419;
        for ( ; __need_loop_1419 ; __need_loop_1419 = __at_iterator.next(__context__,(__at_rename_at_1419_30)) )
        {
            __builtin_table_set_insert(__context__,__tab_rename_at_1418_29,(*__at_rename_at_1419_30));
        }
        __at_iterator.close(__context__,(__at_rename_at_1419_30));
    };
    return /* <- */ das_auto_cast_move<TTable<char *,void>>::cast(__tab_rename_at_1418_29);
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_125_31, int32_t __newSize_rename_at_125_32 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_125_31),__newSize_rename_at_125_32,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_33 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_33),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3 ( Context * __context__, TDim<char *,21> const  &  __arr_rename_at_1658_34, char * const  __key_rename_at_1658_35 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,21));
        int32_t __i_rename_at_1659_36;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_36)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_36)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_34(__i_rename_at_1659_36,__context__)),cast<char *>::from(__key_rename_at_1658_35),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_36);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_36));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622 ( Context * __context__, TDim<char *,11> const  &  __arr_rename_at_1658_37, char * const  __key_rename_at_1658_38 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,11));
        int32_t __i_rename_at_1659_39;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_39)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_39)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_37(__i_rename_at_1659_39,__context__)),cast<char *>::from(__key_rename_at_1658_38),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_39);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_39));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_40, TArray<uint8_t> const  &  __b_rename_at_1080_41 )
{
    int32_t __ln_rename_at_1081_42 = ((int32_t)builtin_array_size(__b_rename_at_1080_41));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_40),__ln_rename_at_1081_42);
    if ( __ln_rename_at_1081_42 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_40(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_41(0,__context__))),__ln_rename_at_1081_42 * 1);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_181_43, das_source_formatter::Token &  __value_rename_at_181_44 )
{
    das_copy(__Arr_rename_at_181_43(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_181_43),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_44);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce ( Context * __context__, TArray<das_source_formatter::FormatterToken> &  __Arr_rename_at_181_45, das_source_formatter::FormatterToken &  __value_rename_at_181_46 )
{
    das_copy(__Arr_rename_at_181_45(builtin_array_push_back(das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__Arr_rename_at_181_45),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_46);
}

inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_944_47, int32_t __i_rename_at_944_48 )
{
    if ( (__ctx_rename_at_944_47.tokens(__i_rename_at_944_48,__context__).dontFormat || __ctx_rename_at_944_47.tokens((__i_rename_at_944_48 - 1),__context__).dontFormat) || new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_944_47),__i_rename_at_944_48) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_944_47.tokens(__i_rename_at_944_48,__context__).spaces,0);
    };
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_49, uint8_t __value_rename_at_165_50 )
{
    das_copy(__Arr_rename_at_165_49(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_49),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_50);
}

inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_936_51, int32_t __i_rename_at_936_52 )
{
    if ( (__ctx_rename_at_936_51.tokens(__i_rename_at_936_52,__context__).dontFormat || __ctx_rename_at_936_51.tokens((__i_rename_at_936_52 - 1),__context__).dontFormat) || new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_936_51),__i_rename_at_936_52) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_936_51.tokens(__i_rename_at_936_52,__context__).spaces,1);
    };
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856 ( Context * __context__, TDim<char *,3> const  &  __arr_rename_at_1658_53, char * const  __key_rename_at_1658_54 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,3));
        int32_t __i_rename_at_1659_55;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_55)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_55)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_53(__i_rename_at_1659_55,__context__)),cast<char *>::from(__key_rename_at_1658_54),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_55);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_55));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void finalize_b9f001def6083360 ( Context * __context__, das_source_formatter::FormatterCtx &  ____this_rename_at_68_56 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_5f7a4e8904f8977e(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(____this_rename_at_68_56.tokenTemplates));
    _FuncbuiltinTickfinalizeTick13836114024949725080_d46b5589c5f4f04b(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(____this_rename_at_68_56.tokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_b4e09f66b8118eb0(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(____this_rename_at_68_56.formatterTokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_68_56.data));
    memset((void*)&(____this_rename_at_68_56), 0, TypeSize<das_source_formatter::FormatterCtx>::size);
}

inline void initialize_token_templates_cd924eed93b743df ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_89_57 )
{
    if ( builtin_array_size(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_57.tokenTemplates)) > 0 )
    {
        return ;
    } else {
        _FuncbuiltinTickresizeTick4811697762258667383_4a7556d56e5a6ac1(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_57.tokenTemplates),40);
        {
            bool __need_loop_94 = true;
            // s: string const&
            das_iterator<TDim<char *,40> const > __s_iterator(das_global<TDim<char *,40>,0x3e3c534a20a075ae>(__context__) /*sorted_tokens_source*/);
            char * const  * __s_rename_at_94_60;
            __need_loop_94 = __s_iterator.first(__context__,(__s_rename_at_94_60)) && __need_loop_94;
            // tt: das_source_formatter::TokenTemplate&
            das_iterator<TArray<das_source_formatter::TokenTemplate>> __tt_iterator(__ctx_rename_at_89_57.tokenTemplates);
            das_source_formatter::TokenTemplate * __tt_rename_at_94_61;
            __need_loop_94 = __tt_iterator.first(__context__,(__tt_rename_at_94_61)) && __need_loop_94;
            for ( ; __need_loop_94 ; __need_loop_94 = __s_iterator.next(__context__,(__s_rename_at_94_60)) && __tt_iterator.next(__context__,(__tt_rename_at_94_61)) )
            {
                das_copy((*__tt_rename_at_94_61).str,(*__s_rename_at_94_60));
                _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c(__context__,das_arg<TArray<uint8_t>>::pass((*__tt_rename_at_94_61).codes),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
                    TArray<uint8_t> ____acomp_96_21_rename_at_96_62;das_zero(____acomp_96_21_rename_at_96_62);
                    builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62),false);
                    {
                        bool __need_loop_96 = true;
                        // c: int const
                        das_iterator<char * const > __c_iterator((*__s_rename_at_94_60));
                        int32_t __c_rename_at_96_63;
                        __need_loop_96 = __c_iterator.first(__context__,(__c_rename_at_96_63)) && __need_loop_96;
                        for ( ; __need_loop_96 ; __need_loop_96 = __c_iterator.next(__context__,(__c_rename_at_96_63)) )
                        {
                            _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62),uint8_t(__c_rename_at_96_63));
                        }
                        __c_iterator.close(__context__,(__c_rename_at_96_63));
                    };
                    builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62),true);
                    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62)));
                })));
                das_copy((*__tt_rename_at_94_61).firstChar,int32_t((*__tt_rename_at_94_61).codes(0,__context__)));
                das_copy((*__tt_rename_at_94_61).length,builtin_string_length((*__s_rename_at_94_60),__context__));
            }
            __s_iterator.close(__context__,(__s_rename_at_94_60));
            __tt_iterator.close(__context__,(__tt_rename_at_94_61));
        };
    };
}

inline bool isUtf8Bom_fc921f39aa33ab23 ( Context * __context__, TArray<uint8_t> &  __text_rename_at_103_64 )
{
    return das_auto_cast<bool>::cast((((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__text_rename_at_103_64)) >= 3) && (uint32_t(__text_rename_at_103_64(0,__context__)) == 0xefu)) && (uint32_t(__text_rename_at_103_64(1,__context__)) == 0xbbu)) && (uint32_t(__text_rename_at_103_64(2,__context__)) == 0xbfu));
}

inline bool eq_1a08ceae13f47e89 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_109_65, int32_t __index_rename_at_109_66, char * const  __s_rename_at_109_67 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_109_66 < 0) || (__index_rename_at_109_66 >= builtin_array_size(__ctx_rename_at_109_65.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_109_65.tokens(__index_rename_at_109_66,__context__).str),cast<char *>::from(__s_rename_at_109_67),*__context__,nullptr))));
}

inline bool eq_32a38f27b5380f0f ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_114_68, int32_t __index_rename_at_114_69, char * const  __s0_rename_at_114_70, char * const  __s1_rename_at_114_71 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_114_69 < 0) || (__index_rename_at_114_69 >= builtin_array_size(__ctx_rename_at_114_68.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_68.tokens(__index_rename_at_114_69,__context__).str),cast<char *>::from(__s0_rename_at_114_70),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_68.tokens(__index_rename_at_114_69,__context__).str),cast<char *>::from(__s1_rename_at_114_71),*__context__,nullptr)))));
}

inline bool eq_f43d812e3a992e4 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_120_72, int32_t __index_rename_at_120_73, char * const  __s0_rename_at_120_74, char * const  __s1_rename_at_120_75, char * const  __s2_rename_at_120_76 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_120_73 < 0) || (__index_rename_at_120_73 >= builtin_array_size(__ctx_rename_at_120_72.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s0_rename_at_120_74),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s1_rename_at_120_75),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s2_rename_at_120_76),*__context__,nullptr)))));
}

inline bool eq_886fe8d1ff63f5fd ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_126_77, int32_t __index_rename_at_126_78, char * const  __s0_rename_at_126_79, char * const  __s1_rename_at_126_80, char * const  __s2_rename_at_126_81, char * const  __s3_rename_at_126_82 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_126_78 < 0) || (__index_rename_at_126_78 >= builtin_array_size(__ctx_rename_at_126_77.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s0_rename_at_126_79),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s1_rename_at_126_80),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s2_rename_at_126_81),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s3_rename_at_126_82),*__context__,nullptr)))));
}

inline bool new_line_before_56d9a06feda36687 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_132_83, int32_t __index_rename_at_132_84 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_132_84 <= 1) || (__index_rename_at_132_84 >= builtin_array_size(__ctx_rename_at_132_83.tokens))) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast((__ctx_rename_at_132_83.tokens(__index_rename_at_132_84,__context__).newLines > 0)));
}

inline void next_char_e34bb0a357da1559 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_137_85 )
{
    das_copy(__ctx_rename_at_137_85.c,int32_t(__ctx_rename_at_137_85.data(__ctx_rename_at_137_85.pos,__context__)));
    if ( __ctx_rename_at_137_85.c == 0 )
    {
        das_copy(__ctx_rename_at_137_85.eof,true);
        return ;
    } else {
        if ( (__ctx_rename_at_137_85.c == 13) && (int32_t(__ctx_rename_at_137_85.data((__ctx_rename_at_137_85.pos + 1),__context__)) == 10) )
        {
            ++__ctx_rename_at_137_85.crCount;
            ++__ctx_rename_at_137_85.pos;
            das_copy(__ctx_rename_at_137_85.c,int32_t(__ctx_rename_at_137_85.data(__ctx_rename_at_137_85.pos,__context__)));
        };
        if ( __ctx_rename_at_137_85.c == 32 )
        {
            ++__ctx_rename_at_137_85.spaceCounter;
        };
        if ( __ctx_rename_at_137_85.c == 9 )
        {
            int32_t __indent_rename_at_155_86 = ((int32_t)((__ctx_rename_at_137_85.indenting != 0) ? das_auto_cast<int32_t>::cast(__ctx_rename_at_137_85.indenting) : das_auto_cast<int32_t>::cast(4)));
            __ctx_rename_at_137_85.spaceCounter += __indent_rename_at_155_86;
            __ctx_rename_at_137_85.curColumn += (__indent_rename_at_155_86 - 1);
        };
        if ( __ctx_rename_at_137_85.c == 10 )
        {
            ++__ctx_rename_at_137_85.lfCount;
            ++__ctx_rename_at_137_85.pos;
            das_copy(__ctx_rename_at_137_85.c,int32_t(__ctx_rename_at_137_85.data(__ctx_rename_at_137_85.pos,__context__)));
            das_copy(__ctx_rename_at_137_85.curLineIndex,__ctx_rename_at_137_85.pos);
            das_copy(__ctx_rename_at_137_85.spaceCounter,0);
            ++__ctx_rename_at_137_85.newLineCounter;
            ++__ctx_rename_at_137_85.srcLine;
            das_copy(__ctx_rename_at_137_85.curColumn,0);
            return ;
        } else {
            ++__ctx_rename_at_137_85.pos;
            das_copy(__ctx_rename_at_137_85.c,int32_t(__ctx_rename_at_137_85.data(__ctx_rename_at_137_85.pos,__context__)));
            ++__ctx_rename_at_137_85.curColumn;
            return ;
        };
    };
}

inline void skip_spaces_a5027bfd73b7bde6 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_179_87 )
{
    while ( (((__ctx_rename_at_179_87.c == 32) || (__ctx_rename_at_179_87.c == 9)) || (__ctx_rename_at_179_87.c == 13)) || (__ctx_rename_at_179_87.c == 10) )
    {
        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_179_87));
    };
}

inline void on_token_added_6925eddc2a7ca309 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_186_88 )
{
    if ( (((__ctx_rename_at_186_88.indenting == 0) && (__ctx_rename_at_186_88.newLineCounter > 0)) && (__ctx_rename_at_186_88.spaceCounter > 0)) && (__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        das_copy(__ctx_rename_at_186_88.indenting,__ctx_rename_at_186_88.spaceCounter);
    };
    if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_186_88),builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1) )
    {
        das_copy(__ctx_rename_at_186_88.insideOptions,false);
        if ( SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).str),cast<char *>::from(((char *) "options")),*__context__,nullptr) )
        {
            das_copy(__ctx_rename_at_186_88.insideOptions,true);
        };
    };
    if ( (((__ctx_rename_at_186_88.insideOptions && (__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::NUMBER)) && (builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) >= 3)) && (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 3),__context__).str),cast<char *>::from(((char *) "indenting")),*__context__,nullptr))) && (builtin_string_length(__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).str,__context__) == 1) )
    {
        das_copy(__ctx_rename_at_186_88.indenting,SimPolicy<int32_t>::Clamp(fast_to_int(__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).str,false),1,8,*__context__,nullptr));
    };
}

inline void push_token_92f9637f04a53e36 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_204_89, int32_t __from_char_idx_rename_at_204_90, int32_t __to_char_idx_rename_at_204_91, int32_t __space_count_rename_at_204_92, int32_t __new_lines_rename_at_204_93, int32_t __src_line_rename_at_204_94, int32_t __column_rename_at_204_95, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_96 )
{
    TArray<uint8_t> __buf_rename_at_205_97; memset((void*)&__buf_rename_at_205_97,0,sizeof(__buf_rename_at_205_97));
    das_source_formatter::Token _temp_make_local_206_23_176; _temp_make_local_206_23_176;
    /* finally */ auto __finally_204= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_97));
    /* end finally */ });
    __buf_rename_at_205_97; das_zero(__buf_rename_at_205_97); das_move(__buf_rename_at_205_97, das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
        TArray<uint8_t> ____acomp_205_17_rename_at_205_98;das_zero(____acomp_205_17_rename_at_205_98);
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_98),false);
        {
            bool __need_loop_205 = true;
            // i: int const
            das_iterator<range> __i_iterator(range(__from_char_idx_rename_at_204_90,__to_char_idx_rename_at_204_91));
            int32_t __i_rename_at_205_99;
            __need_loop_205 = __i_iterator.first(__context__,(__i_rename_at_205_99)) && __need_loop_205;
            for ( ; __need_loop_205 ; __need_loop_205 = __i_iterator.next(__context__,(__i_rename_at_205_99)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_98),uint8_t(__ctx_rename_at_204_89.data(__i_rename_at_205_99,__context__)));
            }
            __i_iterator.close(__context__,(__i_rename_at_205_99));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_98),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(____acomp_205_17_rename_at_205_98);
    }));
    _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_204_89.tokens),das_arg<das_source_formatter::Token>::pass((([&]() -> das_source_formatter::Token& {
        _temp_make_local_206_23_176 = Token_c2eeda936a4bfe12(__context__);
        das_copy((_temp_make_local_206_23_176.str),(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_97),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
        das_copy((_temp_make_local_206_23_176.spaces),(__space_count_rename_at_204_92));
        das_copy((_temp_make_local_206_23_176.newLines),(__new_lines_rename_at_204_93));
        das_copy((_temp_make_local_206_23_176.lineInSource),(__src_line_rename_at_204_94));
        das_copy((_temp_make_local_206_23_176.column),(__column_rename_at_204_95));
        das_copy((_temp_make_local_206_23_176.tokenType),(__token_type_rename_at_204_96));
        return _temp_make_local_206_23_176;
    })())));
    on_token_added_6925eddc2a7ca309(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_204_89));
    das_copy(__ctx_rename_at_204_89.spaceCounter,0);
    das_copy(__ctx_rename_at_204_89.newLineCounter,0);
}

inline void parse_token_95969fa7ff7b12c4 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_220_100 )
{
    skip_spaces_a5027bfd73b7bde6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
    if ( __ctx_rename_at_220_100.eof )
    {
        _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_220_100.tokens),das_arg<das_source_formatter::Token>::pass(__ctx_rename_at_220_100.tokens(0,__context__)));
        return ;
    } else {
        int32_t __from_rename_at_227_101 = ((int32_t)__ctx_rename_at_220_100.pos);
        int32_t __fromColumn_rename_at_228_102 = ((int32_t)__ctx_rename_at_220_100.curColumn);
        if ( (is_alpha(__ctx_rename_at_220_100.c) || (__ctx_rename_at_220_100.c == 95)) || (__ctx_rename_at_220_100.c == 96) )
        {
            while ( !(__ctx_rename_at_220_100.eof) && (((is_alpha(__ctx_rename_at_220_100.c) || is_number(__ctx_rename_at_220_100.c)) || (__ctx_rename_at_220_100.c == 95)) || (__ctx_rename_at_220_100.c == 96)) )
            {
                next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
            };
            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER);
            return ;
        } else {
            if ( is_number(__ctx_rename_at_220_100.c) || ((__ctx_rename_at_220_100.c == 46) && is_number(int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)))) )
            {
                while ( !(__ctx_rename_at_220_100.eof) && (((is_number(__ctx_rename_at_220_100.c) || is_alpha(__ctx_rename_at_220_100.c)) || (__ctx_rename_at_220_100.c == 46)) || (((__ctx_rename_at_220_100.c == 45) || (__ctx_rename_at_220_100.c == 43)) && ((int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos - 1),__context__)) == 101) || (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos - 1),__context__)) == 69)))) )
                {
                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                };
                push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::NUMBER);
                return ;
            } else {
                if ( (__ctx_rename_at_220_100.c == 34) || (__ctx_rename_at_220_100.c == 39) )
                {
                    int32_t __savedLine_rename_at_251_103 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                    int32_t __savedSpace_rename_at_252_104 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                    int32_t __openChar_rename_at_253_105 = ((int32_t)__ctx_rename_at_220_100.c);
                    int32_t __depth_rename_at_254_106 = 0;
                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                    while ( !(__ctx_rename_at_220_100.eof) && ((__ctx_rename_at_220_100.c != __openChar_rename_at_253_105) || (__depth_rename_at_254_106 > 0)) )
                    {
                        if ( (__ctx_rename_at_220_100.c == 123) && (__openChar_rename_at_253_105 == 34) )
                        {
                            ++__depth_rename_at_254_106;
                        };
                        if ( (__ctx_rename_at_220_100.c == 125) && (__openChar_rename_at_253_105 == 34) )
                        {
                            --__depth_rename_at_254_106;
                        };
                        if ( __ctx_rename_at_220_100.c == 92 )
                        {
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                        };
                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                    };
                    if ( __ctx_rename_at_220_100.c == __openChar_rename_at_253_105 )
                    {
                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                    };
                    push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_252_104,__savedLine_rename_at_251_103,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::STRING);
                    return ;
                } else {
                    if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 47) )
                    {
                        int32_t __savedLine_rename_at_279_107 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                        int32_t __savedSpace_rename_at_280_108 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                        while ( (!(__ctx_rename_at_220_100.eof) && (__ctx_rename_at_220_100.c != 10)) && (__ctx_rename_at_220_100.c != 13) )
                        {
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                        };
                        push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_280_108,__savedLine_rename_at_279_107,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
                        return ;
                    } else {
                        if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 42) )
                        {
                            int32_t __savedLine_rename_at_290_109 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                            int32_t __savedSpace_rename_at_291_110 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            int32_t __depth_rename_at_294_111 = 1;
                            while ( !__ctx_rename_at_220_100.eof )
                            {
                                if ( (__ctx_rename_at_220_100.c == 42) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 47) )
                                {
                                    __depth_rename_at_294_111 -= 1;
                                    if ( __depth_rename_at_294_111 == 0 )
                                    {
                                        break;
                                    } else {
                                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                    };
                                };
                                if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 42) )
                                {
                                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                    __depth_rename_at_294_111 += 1;
                                };
                                next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            };
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_291_110,__savedLine_rename_at_290_109,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
                            return ;
                        } else {
                            {
                                bool __need_loop_318 = true;
                                // tt: das_source_formatter::TokenTemplate&
                                das_iterator<TArray<das_source_formatter::TokenTemplate>> __tt_iterator(__ctx_rename_at_220_100.tokenTemplates);
                                das_source_formatter::TokenTemplate * __tt_rename_at_318_112;
                                __need_loop_318 = __tt_iterator.first(__context__,(__tt_rename_at_318_112)) && __need_loop_318;
                                for ( ; __need_loop_318 ; __need_loop_318 = __tt_iterator.next(__context__,(__tt_rename_at_318_112)) )
                                {
                                    if ( __ctx_rename_at_220_100.c == (*__tt_rename_at_318_112).firstChar )
                                    {
                                        bool __match_rename_at_320_113 = true;
                                        {
                                            bool __need_loop_321 = true;
                                            // i: int const
                                            das_iterator<range> __i_iterator(range(__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.pos + (*__tt_rename_at_318_112).length));
                                            int32_t __i_rename_at_321_116;
                                            __need_loop_321 = __i_iterator.first(__context__,(__i_rename_at_321_116)) && __need_loop_321;
                                            // code: uint8&
                                            das_iterator<TArray<uint8_t>> __code_iterator((*__tt_rename_at_318_112).codes);
                                            uint8_t * __code_rename_at_321_117;
                                            __need_loop_321 = __code_iterator.first(__context__,(__code_rename_at_321_117)) && __need_loop_321;
                                            for ( ; __need_loop_321 ; __need_loop_321 = __i_iterator.next(__context__,(__i_rename_at_321_116)) && __code_iterator.next(__context__,(__code_rename_at_321_117)) )
                                            {
                                                if ( das_nequ_val(__ctx_rename_at_220_100.data(__i_rename_at_321_116,__context__),(*__code_rename_at_321_117)) )
                                                {
                                                    das_copy(__match_rename_at_320_113,false);
                                                    break;
                                                };
                                            }
                                            __i_iterator.close(__context__,(__i_rename_at_321_116));
                                            __code_iterator.close(__context__,(__code_rename_at_321_117));
                                        };
                                        if ( __match_rename_at_320_113 )
                                        {
                                            {
                                                bool __need_loop_328 = true;
                                                // _: int const
                                                das_iterator<range> ____iterator(range(0,(*__tt_rename_at_318_112).length));
                                                int32_t ____rename_at_328_118;
                                                __need_loop_328 = ____iterator.first(__context__,(____rename_at_328_118)) && __need_loop_328;
                                                for ( ; __need_loop_328 ; __need_loop_328 = ____iterator.next(__context__,(____rename_at_328_118)) )
                                                {
                                                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                                }
                                                ____iterator.close(__context__,(____rename_at_328_118));
                                            };
                                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                                            return ;
                                        };
                                    };
                                }
                                __tt_iterator.close(__context__,(__tt_rename_at_318_112));
                            };
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                            return ;
                        };
                    };
                };
            };
        };
    };
}

inline void parse_all_tokens_b5d5b68330cdfcd0 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_344_119 )
{
    initialize_token_templates_cd924eed93b743df(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_119));
    das_source_formatter::Token __emptyToken_rename_at_346_120_ConstRef; das_zero(__emptyToken_rename_at_346_120_ConstRef); das_move(__emptyToken_rename_at_346_120_ConstRef, ((das_source_formatter::Token)(([&]() -> das_source_formatter::Token {
        das_source_formatter::Token __mks_346 = Token_c2eeda936a4bfe12(__context__);
        return __mks_346;
    })())));
    das_source_formatter::Token const  & __emptyToken_rename_at_346_120 = __emptyToken_rename_at_346_120_ConstRef; ;
    builtin_array_clear(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__emptyToken_rename_at_346_120);
    das_copy(__ctx_rename_at_344_119.pos,0);
    das_copy(__ctx_rename_at_344_119.eof,false);
    das_copy(__ctx_rename_at_344_119.haveUtf8Bom,false);
    das_copy(__ctx_rename_at_344_119.newLineCounter,0);
    das_copy(__ctx_rename_at_344_119.spaceCounter,0);
    das_copy(__ctx_rename_at_344_119.srcLine,1);
    das_copy(__ctx_rename_at_344_119.curColumn,0);
    das_copy(__ctx_rename_at_344_119.indenting,0);
    das_copy(__ctx_rename_at_344_119.insideOptions,false);
    das_copy(__ctx_rename_at_344_119.crCount,0);
    das_copy(__ctx_rename_at_344_119.lfCount,0);
    if ( isUtf8Bom_fc921f39aa33ab23(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_344_119.data)) )
    {
        __ctx_rename_at_344_119.pos += 3;
        das_copy(__ctx_rename_at_344_119.haveUtf8Bom,true);
    };
    das_copy(__ctx_rename_at_344_119.c,int32_t(__ctx_rename_at_344_119.data(__ctx_rename_at_344_119.pos,__context__)));
    das_copy(__ctx_rename_at_344_119.curLineIndex,__ctx_rename_at_344_119.pos);
    while ( !__ctx_rename_at_344_119.eof )
    {
        parse_token_95969fa7ff7b12c4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_119));
    };
    {
        bool __need_loop_373 = true;
        // _: int const
        das_iterator<range> ____iterator(range(0,8));
        int32_t ____rename_at_373_121;
        __need_loop_373 = ____iterator.first(__context__,(____rename_at_373_121)) && __need_loop_373;
        for ( ; __need_loop_373 ; __need_loop_373 = ____iterator.next(__context__,(____rename_at_373_121)) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__emptyToken_rename_at_346_120);
        }
        ____iterator.close(__context__,(____rename_at_373_121));
    };
}

inline void update_paren_62e64d469829ae3d ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_388_122, int32_t __index_rename_at_388_123, das_source_formatter::ParenCounter &  __paren_counter_rename_at_388_124 )
{
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "<")) )
    {
        ++__paren_counter_rename_at_388_124.angle;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">")) )
    {
        --__paren_counter_rename_at_388_124.angle;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">>")) )
    {
        __paren_counter_rename_at_388_124.angle -= 2;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">>>")) )
    {
        __paren_counter_rename_at_388_124.angle -= 3;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "(")) )
    {
        ++__paren_counter_rename_at_388_124.paren;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ")")) )
    {
        --__paren_counter_rename_at_388_124.paren;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "[")) )
    {
        ++__paren_counter_rename_at_388_124.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "?[")) )
    {
        ++__paren_counter_rename_at_388_124.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "]")) )
    {
        --__paren_counter_rename_at_388_124.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "{")) )
    {
        ++__paren_counter_rename_at_388_124.curly;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "}")) )
    {
        --__paren_counter_rename_at_388_124.curly;
    };
}

inline int32_t find_pair_paren_90acbe7a3b864d93 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_425_125, int32_t __from_index_rename_at_425_126 )
{
    das_source_formatter::ParenCounter __pc_rename_at_426_127;das_zero(__pc_rename_at_426_127);
    {
        bool __need_loop_427 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_425_126 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_425_125.tokens))));
        int32_t __j_rename_at_427_128;
        __need_loop_427 = __j_iterator.first(__context__,(__j_rename_at_427_128)) && __need_loop_427;
        for ( ; __need_loop_427 ; __need_loop_427 = __j_iterator.next(__context__,(__j_rename_at_427_128)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_425_125),__j_rename_at_427_128,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_426_127));
            if ( __pc_rename_at_426_127.paren < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_427_128);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_427_128));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_425_126);
}

inline int32_t find_pair_square_68d9ea1d7c30e49f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_437_129, int32_t __from_index_rename_at_437_130 )
{
    das_source_formatter::ParenCounter __pc_rename_at_438_131;das_zero(__pc_rename_at_438_131);
    {
        bool __need_loop_439 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_437_130 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_437_129.tokens))));
        int32_t __j_rename_at_439_132;
        __need_loop_439 = __j_iterator.first(__context__,(__j_rename_at_439_132)) && __need_loop_439;
        for ( ; __need_loop_439 ; __need_loop_439 = __j_iterator.next(__context__,(__j_rename_at_439_132)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_437_129),__j_rename_at_439_132,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_438_131));
            if ( __pc_rename_at_438_131.square < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_439_132);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_439_132));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_437_130);
}

inline int32_t find_pair_angle_89495f7d100107b7 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_449_133, int32_t __from_index_rename_at_449_134 )
{
    das_source_formatter::ParenCounter __pc_rename_at_450_135;das_zero(__pc_rename_at_450_135);
    {
        bool __need_loop_451 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_449_134 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_449_133.tokens))));
        int32_t __j_rename_at_451_136;
        __need_loop_451 = __j_iterator.first(__context__,(__j_rename_at_451_136)) && __need_loop_451;
        for ( ; __need_loop_451 ; __need_loop_451 = __j_iterator.next(__context__,(__j_rename_at_451_136)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_449_133),__j_rename_at_451_136,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_450_135));
            if ( __pc_rename_at_450_135.angle < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_451_136);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_451_136));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_449_134);
}

inline void mark_range_as_type_24b8f876667238a0 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_461_137, int32_t __from_rename_at_461_138, int32_t __to_rename_at_461_139 )
{
    {
        bool __need_loop_462 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(__from_rename_at_461_138,__to_rename_at_461_139));
        int32_t __i_rename_at_462_140;
        __need_loop_462 = __i_iterator.first(__context__,(__i_rename_at_462_140)) && __need_loop_462;
        for ( ; __need_loop_462 ; __need_loop_462 = __i_iterator.next(__context__,(__i_rename_at_462_140)) )
        {
            if ( __ctx_rename_at_461_137.tokens(__i_rename_at_462_140,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT )
            {
                das_copy(__ctx_rename_at_461_137.tokens(__i_rename_at_462_140,__context__).isInType,true);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_462_140));
    };
}

inline void mark_tokens_as_type_5e4d056374f6927c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_470_141, int32_t __i_rename_at_470_142 )
{
    int32_t __pos_rename_at_471_143 = __i_rename_at_470_142;
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "auto")) )
    {
        ++__pos_rename_at_471_143;
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "(")) )
        {
            das_copy(__pos_rename_at_471_143,find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143));
            ++__pos_rename_at_471_143;
        };
    } else {
        while ( __pos_rename_at_471_143 < builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_470_141.tokens)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143) )
            {
                break;
            } else {
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "::")) || (__ctx_rename_at_470_141.tokens(__pos_rename_at_471_143,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
                {
                    ++__pos_rename_at_471_143;
                } else {
                    break;
                };
            };
        };
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "<")) )
        {
            das_copy(__pos_rename_at_471_143,find_pair_angle_89495f7d100107b7(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143));
            ++__pos_rename_at_471_143;
        };
    };
    if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143) )
    {
        mark_range_as_type_24b8f876667238a0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__i_rename_at_470_142,__pos_rename_at_471_143);
        return ;
    } else {
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "[")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143 + 1,((char *) "]")) )
        {
            __pos_rename_at_471_143 += 2;
        };
        {
            bool __need_loop_507 = true;
            // j: int const
            das_iterator<range> __j_iterator(range(__pos_rename_at_471_143,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_470_141.tokens))));
            int32_t __j_rename_at_507_144;
            __need_loop_507 = __j_iterator.first(__context__,(__j_rename_at_507_144)) && __need_loop_507;
            for ( ; __need_loop_507 ; __need_loop_507 = __j_iterator.next(__context__,(__j_rename_at_507_144)) )
            {
                if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "-"),((char *) "==")) && eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144 + 1,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) )
                {
                    das_copy(__pos_rename_at_471_143,__j_rename_at_507_144 + 2);
                } else if ( eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "implicit")) )
                {
                    das_copy(__pos_rename_at_471_143,__j_rename_at_507_144 + 1);
                } else {
                    break;
                };
            }
            __j_iterator.close(__context__,(__j_rename_at_507_144));
        };
        mark_range_as_type_24b8f876667238a0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__i_rename_at_470_142,__pos_rename_at_471_143);
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "|")) )
        {
            das_copy(__ctx_rename_at_470_141.tokens(__pos_rename_at_471_143,__context__).spaces,1);
            ++__pos_rename_at_471_143;
            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143);
        };
    };
}

inline void mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_528_145, int32_t __i_rename_at_528_146 )
{
    das_source_formatter::ParenCounter __pc_rename_at_529_147;das_zero(__pc_rename_at_529_147);
    {
        bool __need_loop_530 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__i_rename_at_528_146,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_528_145.tokens))));
        int32_t __j_rename_at_530_148;
        __need_loop_530 = __j_iterator.first(__context__,(__j_rename_at_530_148)) && __need_loop_530;
        for ( ; __need_loop_530 ; __need_loop_530 = __j_iterator.next(__context__,(__j_rename_at_530_148)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_528_145),__j_rename_at_530_148,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_529_147));
            if ( __ctx_rename_at_528_145.tokens(__j_rename_at_530_148,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT )
            {
                das_copy(__ctx_rename_at_528_145.tokens(__j_rename_at_530_148,__context__).isInType,true);
            };
            if ( __pc_rename_at_529_147.angle <= 0 )
            {
                break;
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_530_148));
    };
}

inline int32_t search_token_in_line_3a9d4fd472c436b3 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_542_149, int32_t __from_index_rename_at_542_150, char * const  __str_rename_at_542_151 )
{
    das_source_formatter::ParenCounter __pc_rename_at_543_152;das_zero(__pc_rename_at_543_152);
    {
        bool __need_loop_544 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_542_150,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_542_149.tokens))));
        int32_t __j_rename_at_544_153;
        __need_loop_544 = __j_iterator.first(__context__,(__j_rename_at_544_153)) && __need_loop_544;
        for ( ; __need_loop_544 ; __need_loop_544 = __j_iterator.next(__context__,(__j_rename_at_544_153)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_543_152));
            if ( (((__j_rename_at_544_153 > __from_index_rename_at_542_150) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153)) && (((__pc_rename_at_543_152.paren <= 0) || (__pc_rename_at_543_152.square <= 0)) || (__pc_rename_at_543_152.curly <= 0))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153 - 1,((char *) ","))) )
            {
                break;
            } else {
                if ( ((__pc_rename_at_543_152.paren < 0) || (__pc_rename_at_543_152.square < 0)) || (__pc_rename_at_543_152.curly < 0) )
                {
                    break;
                } else {
                    if ( ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,((char *) ";")) && (__pc_rename_at_543_152.paren == 0)) && (__pc_rename_at_543_152.square == 0)) && (__pc_rename_at_543_152.curly == 0) )
                    {
                        break;
                    } else {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,__str_rename_at_542_151) )
                        {
                            return das_auto_cast<int32_t>::cast(__j_rename_at_544_153);
                        };
                    };
                };
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_544_153));
    };
    return das_auto_cast<int32_t>::cast(0);
}

inline void process_formatter_tokens_a63e2ffa4c8206b6 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_564_154 )
{
    {
        bool __need_loop_567 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_564_154.tokens))));
        int32_t __i_rename_at_567_155;
        __need_loop_567 = __i_iterator.first(__context__,(__i_rename_at_567_155)) && __need_loop_567;
        for ( ; __need_loop_567 ; __need_loop_567 = __i_iterator.next(__context__,(__i_rename_at_567_155)) )
        {
            das_source_formatter::Token __token_rename_at_568_156_ConstRef = ((das_source_formatter::Token)__ctx_rename_at_564_154.tokens(__i_rename_at_567_155,__context__));
    das_source_formatter::Token const  & __token_rename_at_568_156 = __token_rename_at_568_156_ConstRef; ;
            char * __str_rename_at_569_157 = ((char *)(char *)(__token_rename_at_568_156.str));
            int32_t __strSize_rename_at_570_158 = ((int32_t)builtin_string_length(__str_rename_at_569_157,__context__));
            if ( builtin_string_startswith(__str_rename_at_569_157,((char *) "//fmt:"),__context__) )
            {
                das_source_formatter::FormatterToken _temp_make_local_573_40_128; _temp_make_local_573_40_128;
                char * __command_rename_at_572_159 = ((char *)(char *)(((char * const )(builtin_string_chop(__str_rename_at_569_157,6,__strSize_rename_at_570_158 - 6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__ctx_rename_at_564_154.formatterTokens),das_arg<das_source_formatter::FormatterToken>::pass((([&]() -> das_source_formatter::FormatterToken& {
                    das_copy((_temp_make_local_573_40_128.tokenIndex),(__i_rename_at_567_155));
                    das_copy((_temp_make_local_573_40_128.command),(__command_rename_at_572_159));
                    return _temp_make_local_573_40_128;
                })())));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_567_155));
    };
}

inline bool have_formatter_token_296d7fc158126671 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_581_160, char * const  __command_rename_at_581_161 )
{
    {
        bool __need_loop_582 = true;
        // formatterToken: das_source_formatter::FormatterToken const&
        das_iterator<TArray<das_source_formatter::FormatterToken> const > __formatterToken_iterator(__ctx_rename_at_581_160.formatterTokens);
        das_source_formatter::FormatterToken const  * __formatterToken_rename_at_582_162;
        __need_loop_582 = __formatterToken_iterator.first(__context__,(__formatterToken_rename_at_582_162)) && __need_loop_582;
        for ( ; __need_loop_582 ; __need_loop_582 = __formatterToken_iterator.next(__context__,(__formatterToken_rename_at_582_162)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from((*__formatterToken_rename_at_582_162).command),cast<char *>::from(__command_rename_at_581_161),*__context__,nullptr) )
            {
                return das_auto_cast<bool>::cast(true);
            };
        }
        __formatterToken_iterator.close(__context__,(__formatterToken_rename_at_582_162));
    };
    return das_auto_cast<bool>::cast(false);
}

inline void mark_token_context_7dbf70435b518a0a ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_591_163 )
{
    TDim<char *,8> _temp_make_local_782_24_576; _temp_make_local_782_24_576;
    {
        bool __need_loop_593 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 1));
        int32_t __i_rename_at_593_164;
        __need_loop_593 = __i_iterator.first(__context__,(__i_rename_at_593_164)) && __need_loop_593;
        for ( ; __need_loop_593 ; __need_loop_593 = __i_iterator.next(__context__,(__i_rename_at_593_164)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164) && (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "require")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "expect"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "label"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "include"))) )
            {
                {
                    bool __need_loop_595 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_593_164 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_595_165;
                    __need_loop_595 = __j_iterator.first(__context__,(__j_rename_at_595_165)) && __need_loop_595;
                    for ( ; __need_loop_595 ; __need_loop_595 = __j_iterator.next(__context__,(__j_rename_at_595_165)) )
                    {
                        if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_595_165) )
                        {
                            break;
                        } else {
                            das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_595_165,__context__).dontFormat,true);
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_595_165));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_593_164));
    };
    {
        bool __need_loop_605 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 4));
        int32_t __i_rename_at_605_166;
        __need_loop_605 = __i_iterator.first(__context__,(__i_rename_at_605_166)) && __need_loop_605;
        for ( ; __need_loop_605 ; __need_loop_605 = __i_iterator.next(__context__,(__i_rename_at_605_166)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_605_166,((char *) "%")) && (__ctx_rename_at_591_163.tokens((__i_rename_at_605_166 + 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_605_166 + 2,((char *) "~")) )
            {
                {
                    bool __need_loop_607 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_605_166,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_607_167;
                    __need_loop_607 = __j_iterator.first(__context__,(__j_rename_at_607_167)) && __need_loop_607;
                    for ( ; __need_loop_607 ; __need_loop_607 = __j_iterator.next(__context__,(__j_rename_at_607_167)) )
                    {
                        das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_607_167,__context__).dontFormat,true);
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_607_167,((char *) "%%")) )
                        {
                            break;
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_607_167));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_605_166));
    };
    {
        bool __need_loop_617 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_617_168;
        __need_loop_617 = __i_iterator.first(__context__,(__i_rename_at_617_168)) && __need_loop_617;
        for ( ; __need_loop_617 ; __need_loop_617 = __i_iterator.next(__context__,(__i_rename_at_617_168)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "let")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "var"))) )
            {
                int32_t __assignPos0_rename_at_619_169 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "=")));
                int32_t __assignPos1_rename_at_620_170 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) ":=")));
                int32_t __assignPos2_rename_at_621_171 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "<-")));
                int32_t __colonPos_rename_at_622_172 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) ":")));
                if ( (((__colonPos_rename_at_622_172 > 0) && ((__assignPos0_rename_at_619_169 > __colonPos_rename_at_622_172) || (__assignPos0_rename_at_619_169 <= 0))) && ((__assignPos1_rename_at_620_170 > __colonPos_rename_at_622_172) || (__assignPos1_rename_at_620_170 <= 0))) && ((__assignPos2_rename_at_621_171 > __colonPos_rename_at_622_172) || (__assignPos2_rename_at_621_171 <= 0)) )
                {
                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_622_172 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_617_168));
    };
    {
        bool __need_loop_633 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_633_173;
        __need_loop_633 = __i_iterator.first(__context__,(__i_rename_at_633_173)) && __need_loop_633;
        for ( ; __need_loop_633 ; __need_loop_633 = __i_iterator.next(__context__,(__i_rename_at_633_173)) )
        {
            if ( __ctx_rename_at_591_163.tokens(__i_rename_at_633_173,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER )
            {
                if ( (_FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_591_163.tokens(__i_rename_at_633_173,__context__).str) >= 0) || ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173,((char *) "new")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173,((char *) "struct"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173 + 1,((char *) "<"))) )
                {
                    mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_633_173));
    };
    {
        bool __need_loop_644 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_644_174;
        __need_loop_644 = __i_iterator.first(__context__,(__i_rename_at_644_174)) && __need_loop_644;
        for ( ; __need_loop_644 ; __need_loop_644 = __i_iterator.next(__context__,(__i_rename_at_644_174)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174,((char *) "$")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 1,((char *) "t"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 2,((char *) "(")) )
            {
                int32_t __endParams_rename_at_646_175 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 2));
                {
                    bool __need_loop_647 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_644_174,__endParams_rename_at_646_175 + 1));
                    int32_t __j_rename_at_647_176;
                    __need_loop_647 = __j_iterator.first(__context__,(__j_rename_at_647_176)) && __need_loop_647;
                    for ( ; __need_loop_647 ; __need_loop_647 = __j_iterator.next(__context__,(__j_rename_at_647_176)) )
                    {
                        das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_647_176,__context__).isInType,true);
                    }
                    __j_iterator.close(__context__,(__j_rename_at_647_176));
                };
                {
                    bool __need_loop_650 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__endParams_rename_at_646_175 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_650_177;
                    __need_loop_650 = __j_iterator.first(__context__,(__j_rename_at_650_177)) && __need_loop_650;
                    for ( ; __need_loop_650 ; __need_loop_650 = __j_iterator.next(__context__,(__j_rename_at_650_177)) )
                    {
                        if ( !(eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177,((char *) "-"),((char *) "#"),((char *) "&"),((char *) "?")) || eq_f43d812e3a992e4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177,((char *) "=="),((char *) "const"),((char *) "implicit"))) )
                        {
                            break;
                        } else {
                            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177) )
                            {
                                break;
                            } else {
                                das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_650_177,__context__).isInType,true);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_650_177));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_644_174));
    };
    {
        bool __need_loop_663 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_663_178;
        __need_loop_663 = __i_iterator.first(__context__,(__i_rename_at_663_178)) && __need_loop_663;
        for ( ; __need_loop_663 ; __need_loop_663 = __i_iterator.next(__context__,(__i_rename_at_663_178)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178,((char *) "[")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 1,((char *) "["),((char *) "{"))) || (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178,((char *) "{")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 1,((char *) "{"))) )
            {
                mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 2);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_663_178));
    };
    {
        bool __need_loop_670 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_670_179;
        __need_loop_670 = __i_iterator.first(__context__,(__i_rename_at_670_179)) && __need_loop_670;
        for ( ; __need_loop_670 ; __need_loop_670 = __i_iterator.next(__context__,(__i_rename_at_670_179)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179) )
            {
                int32_t __skip_rename_at_672_180 = 0;
                if ( ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "struct")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "class"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "variant"))) || (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "var"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "let"))) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1)) )
                {
                    das_copy(__skip_rename_at_672_180,1);
                };
                if ( (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "var"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "let"))) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1,((char *) "private")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1,((char *) "public")))) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 2) )
                {
                    das_copy(__skip_rename_at_672_180,2);
                };
                if ( __skip_rename_at_672_180 == 0 )
                {
                    continue;
                } else {
                    int32_t __ident_rename_at_687_181 = ((int32_t)__ctx_rename_at_591_163.tokens(__i_rename_at_670_179,__context__).spaces);
                    {
                        bool __need_loop_688 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__i_rename_at_670_179 + __skip_rename_at_672_180,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
                        int32_t __j_rename_at_688_182;
                        __need_loop_688 = __j_iterator.first(__context__,(__j_rename_at_688_182)) && __need_loop_688;
                        for ( ; __need_loop_688 ; __need_loop_688 = __j_iterator.next(__context__,(__j_rename_at_688_182)) )
                        {
                            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182) )
                            {
                                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182,((char *) "def")) )
                                {
                                    break;
                                } else {
                                    if ( __ctx_rename_at_591_163.tokens(__j_rename_at_688_182,__context__).spaces <= __ident_rename_at_687_181 )
                                    {
                                        break;
                                    } else {
                                        int32_t __colonPos_rename_at_696_183 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182,eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                                        if ( __colonPos_rename_at_696_183 > 0 )
                                        {
                                            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_696_183 + 1);
                                        };
                                    };
                                };
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_688_182));
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_670_179));
    };
    {
        bool __need_loop_706 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_706_184;
        __need_loop_706 = __i_iterator.first(__context__,(__i_rename_at_706_184)) && __need_loop_706;
        for ( ; __need_loop_706 ; __need_loop_706 = __i_iterator.next(__context__,(__i_rename_at_706_184)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184,((char *) "typedef")) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184 + 1))) )
            {
                {
                    bool __need_loop_708 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_706_184 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
                    int32_t __j_rename_at_708_185;
                    __need_loop_708 = __j_iterator.first(__context__,(__j_rename_at_708_185)) && __need_loop_708;
                    for ( ; __need_loop_708 ; __need_loop_708 = __j_iterator.next(__context__,(__j_rename_at_708_185)) )
                    {
                        if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_708_185) )
                        {
                            break;
                        } else {
                            int32_t __colonPos_rename_at_712_186 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_708_185,eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                            if ( __colonPos_rename_at_712_186 > 0 )
                            {
                                mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_712_186 + 1);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_708_185));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_706_184));
    };
    {
        bool __need_loop_721 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
        int32_t __i_rename_at_721_187;
        __need_loop_721 = __i_iterator.first(__context__,(__i_rename_at_721_187)) && __need_loop_721;
        for ( ; __need_loop_721 ; __need_loop_721 = __i_iterator.next(__context__,(__i_rename_at_721_187)) )
        {
            int32_t __pos_rename_at_722_188 = -1;
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187,((char *) "def")) )
            {
                das_copy(__pos_rename_at_722_188,__i_rename_at_721_187 + 1);
            } else if ( eq_f43d812e3a992e4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187,((char *) "$"),((char *) "@"),((char *) "@@")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187 + 1,((char *) "("),((char *) "[")) )
            {
                das_copy(__pos_rename_at_722_188,__i_rename_at_721_187);
            };
            if ( __pos_rename_at_722_188 == -1 )
            {
                continue;
            } else {
                while ( eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "private"),((char *) "public"),((char *) "const"),((char *) "implicit")) || eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "abstract"),((char *) "override")) )
                {
                    ++__pos_rename_at_722_188;
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "operator")) )
                {
                    das_copy(__pos_rename_at_722_188,search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "(")) - 1);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) "[")) )
                {
                    das_copy(__pos_rename_at_722_188,find_pair_square_68d9ea1d7c30e49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1));
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) ":")) )
                {
                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187 + 2);
                } else if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) "(")) )
                {
                    int32_t __beginParams_rename_at_748_189 = ((int32_t)(__pos_rename_at_722_188 + 1));
                    das_copy(__ctx_rename_at_591_163.tokens(__beginParams_rename_at_748_189,__context__).dontAddSpacesAround,true);
                    int32_t __endParams_rename_at_750_190 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1));
                    {
                        bool __need_loop_751 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__beginParams_rename_at_748_189,__endParams_rename_at_750_190 + 1));
                        int32_t __j_rename_at_751_191;
                        __need_loop_751 = __j_iterator.first(__context__,(__j_rename_at_751_191)) && __need_loop_751;
                        for ( ; __need_loop_751 ; __need_loop_751 = __j_iterator.next(__context__,(__j_rename_at_751_191)) )
                        {
                            das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_751_191,__context__).isInFunctionParam,true);
                            if ( ((__j_rename_at_751_191 - 1) == __beginParams_rename_at_748_189) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_751_191 - 1,((char *) ";")) )
                            {
                                int32_t __colonPos_rename_at_754_192 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_751_191,((char *) ":")));
                                if ( __colonPos_rename_at_754_192 > 0 )
                                {
                                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_754_192 + 1);
                                };
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_751_191));
                    };
                    if ( __endParams_rename_at_750_190 < (builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 1) )
                    {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__endParams_rename_at_750_190 + 1,((char *) ":")) )
                        {
                            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__endParams_rename_at_750_190 + 2);
                        };
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_721_187));
    };
    {
        bool __need_loop_769 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 1));
        int32_t __i_rename_at_769_193;
        __need_loop_769 = __i_iterator.first(__context__,(__i_rename_at_769_193)) && __need_loop_769;
        for ( ; __need_loop_769 ; __need_loop_769 = __i_iterator.next(__context__,(__i_rename_at_769_193)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193,((char *) "[")) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193 + 1,((char *) "[")))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193 + 1,((char *) "{"))) )
            {
                {
                    bool __need_loop_771 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_769_193 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_771_194;
                    __need_loop_771 = __j_iterator.first(__context__,(__j_rename_at_771_194)) && __need_loop_771;
                    for ( ; __need_loop_771 ; __need_loop_771 = __j_iterator.next(__context__,(__j_rename_at_771_194)) )
                    {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_771_194,((char *) "]")) )
                        {
                            break;
                        } else {
                            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_771_194,((char *) "=")) )
                            {
                                das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_771_194,__context__).dontAddSpacesAround,true);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_771_194));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_769_193));
    };
    TTable<char *,void> __macroEscapes_rename_at_782_195_ConstRef; das_zero(__macroEscapes_rename_at_782_195_ConstRef); das_move(__macroEscapes_rename_at_782_195_ConstRef, ((TTable<char *,void>)_FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768(__context__,das_arg<TDim<char *,8>>::pass((([&]() -> TDim<char *,8>& {
        _temp_make_local_782_24_576(0,__context__) = ((char *) "i");
        _temp_make_local_782_24_576(1,__context__) = ((char *) "v");
        _temp_make_local_782_24_576(2,__context__) = ((char *) "e");
        _temp_make_local_782_24_576(3,__context__) = ((char *) "c");
        _temp_make_local_782_24_576(4,__context__) = ((char *) "t");
        _temp_make_local_782_24_576(5,__context__) = ((char *) "f");
        _temp_make_local_782_24_576(6,__context__) = ((char *) "b");
        _temp_make_local_782_24_576(7,__context__) = ((char *) "a");
        return _temp_make_local_782_24_576;
    })())))));
    TTable<char *,void> const  & __macroEscapes_rename_at_782_195 = __macroEscapes_rename_at_782_195_ConstRef; ;
    {
        bool __need_loop_783 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
        int32_t __i_rename_at_783_196;
        __need_loop_783 = __i_iterator.first(__context__,(__i_rename_at_783_196)) && __need_loop_783;
        for ( ; __need_loop_783 ; __need_loop_783 = __i_iterator.next(__context__,(__i_rename_at_783_196)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_783_196)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_783_196 - 1,((char *) "$"))) && _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53(__context__,__macroEscapes_rename_at_782_195,__ctx_rename_at_591_163.tokens(__i_rename_at_783_196,__context__).str) )
            {
                das_copy(__ctx_rename_at_591_163.tokens((__i_rename_at_783_196 - 1),__context__).dontAddSpacesAround,true);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_783_196));
    };
}

inline void fmt_function_arguments_7d9f22d6302573cb ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_791_197 )
{
    int32_t __lastNewLineToken_rename_at_793_198 = 0;
    int32_t __i_rename_at_794_199 = 0;
    int32_t __n_rename_at_795_200 = ((int32_t)(builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_791_197.tokens)) - 3));
    while ( __i_rename_at_794_199 < __n_rename_at_795_200 )
    {
        if ( __ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).newLines > 0 )
        {
            das_copy(__lastNewLineToken_rename_at_793_198,__i_rename_at_794_199);
        };
        if ( __ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).isInFunctionParam )
        {
            int32_t __firstBracketColumn_rename_at_802_201 = -1;
            while ( (__i_rename_at_794_199 < __n_rename_at_795_200) && __ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).isInFunctionParam )
            {
                if ( __ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).newLines > 0 )
                {
                    das_copy(__lastNewLineToken_rename_at_793_198,__i_rename_at_794_199);
                };
                if ( __firstBracketColumn_rename_at_802_201 == -1 )
                {
                    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_791_197),__i_rename_at_794_199,((char *) "(")) )
                    {
                        das_copy(__firstBracketColumn_rename_at_802_201,0);
                        {
                            bool __need_loop_811 = true;
                            // k: int const
                            das_iterator<range> __k_iterator(range(__lastNewLineToken_rename_at_793_198,__i_rename_at_794_199 + 1));
                            int32_t __k_rename_at_811_202;
                            __need_loop_811 = __k_iterator.first(__context__,(__k_rename_at_811_202)) && __need_loop_811;
                            for ( ; __need_loop_811 ; __need_loop_811 = __k_iterator.next(__context__,(__k_rename_at_811_202)) )
                            {
                                if ( __ctx_rename_at_791_197.tokens(__k_rename_at_811_202,__context__).newLines > 0 )
                                {
                                    das_copy(__firstBracketColumn_rename_at_802_201,0);
                                };
                                __firstBracketColumn_rename_at_802_201 += (__ctx_rename_at_791_197.tokens(__k_rename_at_811_202,__context__).spaces + builtin_string_length(__ctx_rename_at_791_197.tokens(__k_rename_at_811_202,__context__).str,__context__));
                            }
                            __k_iterator.close(__context__,(__k_rename_at_811_202));
                        };
                    };
                } else if ( __ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).newLines > 0 )
                {
                    das_copy(__lastNewLineToken_rename_at_793_198,__i_rename_at_794_199);
                    das_copy(__ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).spaces,__firstBracketColumn_rename_at_802_201 + (__ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).isInType ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(0)));
                    das_copy(__ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).column,__firstBracketColumn_rename_at_802_201 + (__ctx_rename_at_791_197.tokens(__i_rename_at_794_199,__context__).isInType ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(0)));
                };
                ++__i_rename_at_794_199;
            };
        };
        ++__i_rename_at_794_199;
    };
}

inline void fmt_remove_space_before_args_26ba700ce1a53dbc ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_832_203 )
{
    {
        bool __need_loop_833 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_832_203.tokens)) - 1));
        int32_t __i_rename_at_833_204;
        __need_loop_833 = __i_iterator.first(__context__,(__i_rename_at_833_204)) && __need_loop_833;
        for ( ; __need_loop_833 ; __need_loop_833 = __i_iterator.next(__context__,(__i_rename_at_833_204)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_203),__i_rename_at_833_204,((char *) "(")) && !(__ctx_rename_at_832_203.tokens(__i_rename_at_833_204,__context__).isInType)) && __ctx_rename_at_832_203.tokens((__i_rename_at_833_204 - 1),__context__).isInType )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_203),__i_rename_at_833_204);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_833_204));
    };
}

inline void debug_print_tokens_927617e00a5e50a7 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_841_205 )
{
    {
        bool __need_loop_842 = true;
        // t: das_source_formatter::Token const&
        das_iterator<TArray<das_source_formatter::Token> const > __t_iterator(__ctx_rename_at_841_205.tokens);
        das_source_formatter::Token const  * __t_rename_at_842_206;
        __need_loop_842 = __t_iterator.first(__context__,(__t_rename_at_842_206)) && __need_loop_842;
        for ( ; __need_loop_842 ; __need_loop_842 = __t_iterator.next(__context__,(__t_rename_at_842_206)) )
        {
            toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_4, cast<char * const >::from(((*__t_rename_at_842_206).isInFunctionParam ? das_auto_cast<char * const >::cast(((char *) ">")) : das_auto_cast<char * const >::cast(((char *) " ")))), cast<char * const >::from(((*__t_rename_at_842_206).isInType ? das_auto_cast<char * const >::cast(((char *) "*")) : das_auto_cast<char * const >::cast(((char *) " ")))), cast<char * const >::from(((char *) " '")), cast<char * const >::from((*__t_rename_at_842_206).str), cast<char * const >::from(((char *) "'\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __t_iterator.close(__context__,(__t_rename_at_842_206));
    };
}

inline char * generate_source_64706a1ce7db4983 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_848_207 )
{
    TArray<uint8_t> __res_rename_at_849_208; memset((void*)&__res_rename_at_849_208,0,sizeof(__res_rename_at_849_208));
    bool __useCrLf_rename_at_857_209; memset((void*)&__useCrLf_rename_at_857_209,0,sizeof(__useCrLf_rename_at_857_209));
    int32_t __column_rename_at_858_210; memset((void*)&__column_rename_at_858_210,0,sizeof(__column_rename_at_858_210));
    int32_t __spaces_rename_at_869_213; memset((void*)&__spaces_rename_at_869_213,0,sizeof(__spaces_rename_at_869_213));
    /* finally */ auto __finally_848= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208));
    /* end finally */ });
    das_zero(__res_rename_at_849_208);
    _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),(builtin_array_size(__ctx_rename_at_848_207.tokens) * 8) + 1000);
    if ( __ctx_rename_at_848_207.haveUtf8Bom )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xef);
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xbb);
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xbf);
    };
    __useCrLf_rename_at_857_209 = ((bool)((__ctx_rename_at_848_207.crCount * 2) > __ctx_rename_at_848_207.lfCount));
    __column_rename_at_858_210 = 0;
    {
        bool __need_loop_860 = true;
        // t: das_source_formatter::Token const&
        das_iterator<TArray<das_source_formatter::Token> const > __t_iterator(__ctx_rename_at_848_207.tokens);
        das_source_formatter::Token const  * __t_rename_at_860_211;
        __need_loop_860 = __t_iterator.first(__context__,(__t_rename_at_860_211)) && __need_loop_860;
        for ( ; __need_loop_860 ; __need_loop_860 = __t_iterator.next(__context__,(__t_rename_at_860_211)) )
        {
            {
                bool __need_loop_861 = true;
                // _: int const
                das_iterator<range> ____iterator(mk_range((*__t_rename_at_860_211).newLines));
                int32_t ____rename_at_861_212;
                __need_loop_861 = ____iterator.first(__context__,(____rename_at_861_212)) && __need_loop_861;
                for ( ; __need_loop_861 ; __need_loop_861 = ____iterator.next(__context__,(____rename_at_861_212)) )
                {
                    if ( __useCrLf_rename_at_857_209 )
                    {
                        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xd);
                    };
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xa);
                    das_copy(__column_rename_at_858_210,0);
                }
                ____iterator.close(__context__,(____rename_at_861_212));
            };
            __spaces_rename_at_869_213 = (*__t_rename_at_860_211).spaces;
            if ( ((*__t_rename_at_860_211).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) && builtin_string_startswith((*__t_rename_at_860_211).str,((char *) "//"),__context__) )
            {
                das_copy(__spaces_rename_at_869_213,SimPolicy<int32_t>::Max((*__t_rename_at_860_211).column - __column_rename_at_858_210,0,*__context__,nullptr));
            };
            {
                bool __need_loop_874 = true;
                // _: int const
                das_iterator<range> ____iterator(mk_range(__spaces_rename_at_869_213));
                int32_t ____rename_at_874_214;
                __need_loop_874 = ____iterator.first(__context__,(____rename_at_874_214)) && __need_loop_874;
                for ( ; __need_loop_874 ; __need_loop_874 = ____iterator.next(__context__,(____rename_at_874_214)) )
                {
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0x20);
                    ++__column_rename_at_858_210;
                }
                ____iterator.close(__context__,(____rename_at_874_214));
            };
            {
                bool __need_loop_879 = true;
                // c: int const
                das_iterator<char * const > __c_iterator((*__t_rename_at_860_211).str);
                int32_t __c_rename_at_879_215;
                __need_loop_879 = __c_iterator.first(__context__,(__c_rename_at_879_215)) && __need_loop_879;
                for ( ; __need_loop_879 ; __need_loop_879 = __c_iterator.next(__context__,(__c_rename_at_879_215)) )
                {
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),uint8_t(__c_rename_at_879_215));
                    ++__column_rename_at_858_210;
                }
                __c_iterator.close(__context__,(__c_rename_at_879_215));
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_860_211));
    };
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool need_spaces_around_7179a668ae90ccd6 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_891_216, int32_t __index_rename_at_891_217 )
{
    if ( ((__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).dontFormat) || __ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).dontAddSpacesAround )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str) >= 0 )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( __ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).isInType && ((((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) ":")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) "<")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || builtin_empty(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str)) )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( __index_rename_at_891_217 < (builtin_array_size(__ctx_rename_at_891_216.tokens) - 1) )
                {
                    if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str),cast<char *>::from(((char *) "@")),*__context__,nullptr)) && (__ctx_rename_at_891_216.tokens((__index_rename_at_891_217 + 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
                    {
                        return das_auto_cast<bool>::cast(false);
                    };
                };
                return das_auto_cast<bool>::cast(true);
            };
        };
    };
}

inline bool need_space_only_before_584e48acb8e8a70 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_912_218, int32_t __index_rename_at_912_219 )
{
    if ( !__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).isInType )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( ((__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).dontFormat) || __ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).dontAddSpacesAround )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str) >= 0 )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( ((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str),cast<char *>::from(((char *) "<")),*__context__,nullptr))) || builtin_empty(__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str) )
                {
                    return das_auto_cast<bool>::cast(false);
                } else {
                    if ( __index_rename_at_912_219 < (builtin_array_size(__ctx_rename_at_912_218.tokens) - 1) )
                    {
                        if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens((__index_rename_at_912_219 + 1),__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens((__index_rename_at_912_219 + 1),__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens((__index_rename_at_912_219 + 1),__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_218.tokens((__index_rename_at_912_219 + 1),__context__).str),cast<char *>::from(((char *) ";")),*__context__,nullptr)) )
                        {
                            return das_auto_cast<bool>::cast(true);
                        };
                    };
                    return das_auto_cast<bool>::cast(false);
                };
            };
        };
    };
}

inline bool is_unary_62ad92645c7f9ee6 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_956_220, int32_t __index_rename_at_956_221 )
{
    if ( eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "!")) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "~")) )
    {
        return das_auto_cast<bool>::cast(true);
    } else {
        if ( (eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "*")) && (__index_rename_at_956_221 > 0)) && __ctx_rename_at_956_220.tokens((__index_rename_at_956_221 - 1),__context__).isInType )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            if ( (eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "+")) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "-"))) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "*")) )
            {
                int32_t __i_rename_at_966_222 = __index_rename_at_956_221;
                while ( (((__i_rename_at_966_222 - 1) >= 0) && (__ctx_rename_at_956_220.tokens((__i_rename_at_966_222 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT)) && !(new_line_before_56d9a06feda36687(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222)) )
                {
                    --__i_rename_at_966_222;
                };
                return das_auto_cast<bool>::cast(((_FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0xd52f4fdee217c055>(__context__) /*tokens_before_unary*/,__ctx_rename_at_956_220.tokens((__i_rename_at_966_222 - 1),__context__).str) >= 0) || new_line_before_56d9a06feda36687(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222)) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast(need_spaces_around_7179a668ae90ccd6(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222 - 1)));
            } else {
                return das_auto_cast<bool>::cast(false);
            };
        };
    };
}

inline void remove_spaces_around_1c96d2bcc36445d1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_979_223, int32_t __index_rename_at_979_224 )
{
    if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224);
    };
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224 + 1)) && (__ctx_rename_at_979_223.tokens((__index_rename_at_979_224 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224 + 1);
    };
}

inline void make_spaces_around_9b0360b9b4d718c3 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_989_225, int32_t __index_rename_at_989_226 )
{
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226)) && (__ctx_rename_at_989_225.tokens(__index_rename_at_989_226,__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226);
    };
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226 + 1)) && (__ctx_rename_at_989_225.tokens((__index_rename_at_989_226 + 1),__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226 + 1);
    };
}

inline void fmt_space_after_comma_7aa6a2faf3ce3f4f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_999_227 )
{
    {
        bool __need_loop_1000 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_999_227.tokens))));
        int32_t __i_rename_at_1000_228;
        __need_loop_1000 = __i_iterator.first(__context__,(__i_rename_at_1000_228)) && __need_loop_1000;
        for ( ; __need_loop_1000 ; __need_loop_1000 = __i_iterator.next(__context__,(__i_rename_at_1000_228)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228 - 1,((char *) ","))) && (__ctx_rename_at_999_227.tokens(__i_rename_at_1000_228,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1000_228));
    };
}

inline void fmt_space_after_semicolon_29caf5fd09f6497e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1008_229 )
{
    {
        bool __need_loop_1009 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1008_229.tokens))));
        int32_t __i_rename_at_1009_230;
        __need_loop_1009 = __i_iterator.first(__context__,(__i_rename_at_1009_230)) && __need_loop_1009;
        for ( ; __need_loop_1009 ; __need_loop_1009 = __i_iterator.next(__context__,(__i_rename_at_1009_230)) )
        {
            if ( ((!(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230,nullptr)) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230 - 1,((char *) ";"))) && (__ctx_rename_at_1008_229.tokens(__i_rename_at_1009_230,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1009_230));
    };
}

inline void fmt_remove_space_before_comma_54b9cd4ba70794e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1017_231 )
{
    {
        bool __need_loop_1018 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1017_231.tokens))));
        int32_t __i_rename_at_1018_232;
        __need_loop_1018 = __i_iterator.first(__context__,(__i_rename_at_1018_232)) && __need_loop_1018;
        for ( ; __need_loop_1018 ; __need_loop_1018 = __i_iterator.next(__context__,(__i_rename_at_1018_232)) )
        {
            if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232,((char *) ",")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1018_232));
    };
}

inline void fmt_remove_space_before_semicolon_132b81d1431b5d49 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1026_233 )
{
    {
        bool __need_loop_1027 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1026_233.tokens))));
        int32_t __i_rename_at_1027_234;
        __need_loop_1027 = __i_iterator.first(__context__,(__i_rename_at_1027_234)) && __need_loop_1027;
        for ( ; __need_loop_1027 ; __need_loop_1027 = __i_iterator.next(__context__,(__i_rename_at_1027_234)) )
        {
            if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234,((char *) ";")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1027_234));
    };
}

inline void fmt_type_colon_a5b252566f4d584 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1035_235 )
{
    {
        bool __need_loop_1036 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1035_235.tokens)) - 1));
        int32_t __i_rename_at_1036_236;
        __need_loop_1036 = __i_iterator.first(__context__,(__i_rename_at_1036_236)) && __need_loop_1036;
        for ( ; __need_loop_1036 ; __need_loop_1036 = __i_iterator.next(__context__,(__i_rename_at_1036_236)) )
        {
            if ( __ctx_rename_at_1035_235.tokens(__i_rename_at_1036_236,__context__).isInType && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236,((char *) ":")) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236);
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1036_236));
    };
}

inline void fmt_function_decl_param_paren_6c291b7c3a975893 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1047_237 )
{
    {
        bool __need_loop_1048 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1047_237.tokens)) - 1));
        int32_t __i_rename_at_1048_238;
        __need_loop_1048 = __i_iterator.first(__context__,(__i_rename_at_1048_238)) && __need_loop_1048;
        for ( ; __need_loop_1048 ; __need_loop_1048 = __i_iterator.next(__context__,(__i_rename_at_1048_238)) )
        {
            if ( (__ctx_rename_at_1047_237.tokens(__i_rename_at_1048_238,__context__).isInFunctionParam && !(__ctx_rename_at_1047_237.tokens((__i_rename_at_1048_238 - 1),__context__).isInFunctionParam)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238,((char *) "(")) )
            {
                remove_spaces_around_1c96d2bcc36445d1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238 - 1,((char *) "]")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
                };
            };
            if ( (__ctx_rename_at_1047_237.tokens(__i_rename_at_1048_238,__context__).isInFunctionParam && !(__ctx_rename_at_1047_237.tokens((__i_rename_at_1048_238 + 1),__context__).isInFunctionParam)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1048_238));
    };
}

inline void fmt_function_call_param_paren_call_f740219cd0547b48 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1062_239 )
{
    {
        bool __need_loop_1063 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1062_239.tokens)) - 1));
        int32_t __i_rename_at_1063_240;
        __need_loop_1063 = __i_iterator.first(__context__,(__i_rename_at_1063_240)) && __need_loop_1063;
        for ( ; __need_loop_1063 ; __need_loop_1063 = __i_iterator.next(__context__,(__i_rename_at_1063_240)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240,((char *) "(")) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240))) && (__ctx_rename_at_1062_239.tokens((__i_rename_at_1063_240 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                remove_spaces_around_1c96d2bcc36445d1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240);
                int32_t __closingParen_rename_at_1066_241 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240));
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__closingParen_rename_at_1066_241);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1063_240));
    };
}

inline void fmt_spaces_around_operators_9837b8f174165da7 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1073_242 )
{
    {
        bool __need_loop_1074 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1073_242.tokens)) - 1));
        int32_t __i_rename_at_1074_243;
        __need_loop_1074 = __i_iterator.first(__context__,(__i_rename_at_1074_243)) && __need_loop_1074;
        for ( ; __need_loop_1074 ; __need_loop_1074 = __i_iterator.next(__context__,(__i_rename_at_1074_243)) )
        {
            if ( !builtin_empty(__ctx_rename_at_1073_242.tokens(__i_rename_at_1074_243,__context__).str) )
            {
                if ( is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243 + 1);
                };
                if ( need_space_only_before_584e48acb8e8a70(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243);
                } else if ( need_spaces_around_7179a668ae90ccd6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    make_spaces_around_9b0360b9b4d718c3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1074_243));
    };
}

inline void fmt_space_after_keyword_2aab289129cf4fc8 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1092_244 )
{
    {
        bool __need_loop_1093 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1092_244.tokens)) - 1));
        int32_t __i_rename_at_1093_245;
        __need_loop_1093 = __i_iterator.first(__context__,(__i_rename_at_1093_245)) && __need_loop_1093;
        for ( ; __need_loop_1093 ; __need_loop_1093 = __i_iterator.next(__context__,(__i_rename_at_1093_245)) )
        {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622(__context__,das_global<TDim<char *,11>,0x94e2878ce5158877>(__context__) /*space_after_keywords*/,__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).str) >= 0 )
            {
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1);
                };
            };
            if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).str),cast<char *>::from(((char *) "return")),*__context__,nullptr)) && (builtin_string_length(__ctx_rename_at_1092_244.tokens((__i_rename_at_1093_245 + 1),__context__).str,__context__) > 0) )
            {
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1093_245));
    };
}

inline void fmt_space_after_paren_a2ef6e80a339831f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1109_246 )
{
    {
        bool __need_loop_1110 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1109_246.tokens)) - 1));
        int32_t __i_rename_at_1110_247;
        __need_loop_1110 = __i_iterator.first(__context__,(__i_rename_at_1110_247)) && __need_loop_1110;
        for ( ; __need_loop_1110 ; __need_loop_1110 = __i_iterator.next(__context__,(__i_rename_at_1110_247)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1109_246),__i_rename_at_1110_247,((char *) ")")) && (__ctx_rename_at_1109_246.tokens((__i_rename_at_1110_247 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1109_246),__i_rename_at_1110_247 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1110_247));
    };
}

inline void fmt_space_after_square_brackets_15fb170c80c43d93 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1118_248 )
{
    {
        bool __need_loop_1119 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1118_248.tokens)) - 1));
        int32_t __i_rename_at_1119_249;
        __need_loop_1119 = __i_iterator.first(__context__,(__i_rename_at_1119_249)) && __need_loop_1119;
        for ( ; __need_loop_1119 ; __need_loop_1119 = __i_iterator.next(__context__,(__i_rename_at_1119_249)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1118_248),__i_rename_at_1119_249,((char *) "]")) && (__ctx_rename_at_1118_248.tokens((__i_rename_at_1119_249 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1118_248),__i_rename_at_1119_249 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1119_249));
    };
}

inline void fmt_remove_space_inside_parens_fc44c6bd3c600b9c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1127_250 )
{
    {
        bool __need_loop_1128 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1127_250.tokens)) - 1));
        int32_t __i_rename_at_1128_251;
        __need_loop_1128 = __i_iterator.first(__context__,(__i_rename_at_1128_251)) && __need_loop_1128;
        for ( ; __need_loop_1128 ; __need_loop_1128 = __i_iterator.next(__context__,(__i_rename_at_1128_251)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_250),__i_rename_at_1128_251,((char *) "(")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_250),__i_rename_at_1128_251 + 1);
            };
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_250),__i_rename_at_1128_251,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_250),__i_rename_at_1128_251);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1128_251));
    };
}

inline void fmt_remove_space_before_as_bb9fc0cabc4ff729 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1139_252 )
{
    {
        bool __need_loop_1140 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1139_252.tokens))));
        int32_t __i_rename_at_1140_253;
        __need_loop_1140 = __i_iterator.first(__context__,(__i_rename_at_1140_253)) && __need_loop_1140;
        for ( ; __need_loop_1140 ; __need_loop_1140 = __i_iterator.next(__context__,(__i_rename_at_1140_253)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_252),__i_rename_at_1140_253)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_252),__i_rename_at_1140_253,((char *) "as"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_252),__i_rename_at_1140_253 - 1,((char *) "?")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_252),__i_rename_at_1140_253);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1140_253));
    };
}

inline void fmt_remove_space_before_end_of_object_64e24a59c1d5b6b ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1148_254 )
{
    {
        bool __need_loop_1150 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1148_254.tokens)) - 2));
        int32_t __i_rename_at_1150_255;
        __need_loop_1150 = __i_iterator.first(__context__,(__i_rename_at_1150_255)) && __need_loop_1150;
        for ( ; __need_loop_1150 ; __need_loop_1150 = __i_iterator.next(__context__,(__i_rename_at_1150_255)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_254),__i_rename_at_1150_255)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_254),__i_rename_at_1150_255,((char *) "]"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_254),__i_rename_at_1150_255 + 1,((char *) "]"))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_254),__i_rename_at_1150_255 - 1,((char *) "]"))) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_254),__i_rename_at_1150_255);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1150_255));
    };
}

inline void fmt_remove_space_before_object_type_298f1dcfc2f7829c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1158_256 )
{
    {
        bool __need_loop_1160 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1158_256.tokens)) - 1));
        int32_t __i_rename_at_1160_257;
        __need_loop_1160 = __i_iterator.first(__context__,(__i_rename_at_1160_257)) && __need_loop_1160;
        for ( ; __need_loop_1160 ; __need_loop_1160 = __i_iterator.next(__context__,(__i_rename_at_1160_257)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_256),__i_rename_at_1160_257)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_256),__i_rename_at_1160_257 - 2,((char *) "["))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_256),__i_rename_at_1160_257 - 1,((char *) "["))) && (__ctx_rename_at_1158_256.tokens(__i_rename_at_1160_257,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_256),__i_rename_at_1160_257);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1160_257));
    };
}

inline void fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1168_258 )
{
    {
        bool __need_loop_1170 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1168_258.tokens)) - 2));
        int32_t __i_rename_at_1170_259;
        __need_loop_1170 = __i_iterator.first(__context__,(__i_rename_at_1170_259)) && __need_loop_1170;
        for ( ; __need_loop_1170 ; __need_loop_1170 = __i_iterator.next(__context__,(__i_rename_at_1170_259)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_258),__i_rename_at_1170_259)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_258),__i_rename_at_1170_259 - 1,((char *) ">"))) && __ctx_rename_at_1168_258.tokens((__i_rename_at_1170_259 - 1),__context__).isInType) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_258),__i_rename_at_1170_259,((char *) "[")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_258),__i_rename_at_1170_259);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1170_259));
    };
}

inline void fmt_glue_type_specifiers_cb3a1c8b879530ae ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1180_260 )
{
    {
        bool __need_loop_1181 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1180_260.tokens))));
        int32_t __i_rename_at_1181_261;
        __need_loop_1181 = __i_iterator.first(__context__,(__i_rename_at_1181_261)) && __need_loop_1181;
        for ( ; __need_loop_1181 ; __need_loop_1181 = __i_iterator.next(__context__,(__i_rename_at_1181_261)) )
        {
            if ( __ctx_rename_at_1180_260.tokens(__i_rename_at_1181_261,__context__).isInType )
            {
                if ( (__ctx_rename_at_1180_260.tokens(__i_rename_at_1181_261,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) && (_FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856(__context__,das_global<TDim<char *,3>,0xca374635aa15cd66>(__context__) /*type_specifiers*/,__ctx_rename_at_1180_260.tokens(__i_rename_at_1181_261,__context__).str) >= 0) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261,((char *) "const")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261 - 1,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261);
                };
                if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261 - 1,((char *) "|")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_260),__i_rename_at_1181_261);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1181_261));
    };
}

inline void fmt_space_after_cast_type_446660f13b2ec290 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1200_262 )
{
    {
        bool __need_loop_1201 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1200_262.tokens)) - 3));
        int32_t __i_rename_at_1201_263;
        __need_loop_1201 = __i_iterator.first(__context__,(__i_rename_at_1201_263)) && __need_loop_1201;
        for ( ; __need_loop_1201 ; __need_loop_1201 = __i_iterator.next(__context__,(__i_rename_at_1201_263)) )
        {
            if ( __ctx_rename_at_1200_262.tokens(__i_rename_at_1201_263,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER )
            {
                if ( _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_1200_262.tokens(__i_rename_at_1201_263,__context__).str) >= 0 )
                {
                    {
                        bool __need_loop_1204 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__i_rename_at_1201_263 + 2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1200_262.tokens))));
                        int32_t __j_rename_at_1204_264;
                        __need_loop_1204 = __j_iterator.first(__context__,(__j_rename_at_1204_264)) && __need_loop_1204;
                        for ( ; __need_loop_1204 ; __need_loop_1204 = __j_iterator.next(__context__,(__j_rename_at_1204_264)) )
                        {
                            if ( !(__ctx_rename_at_1200_262.tokens(__j_rename_at_1204_264,__context__).isInType) && (__ctx_rename_at_1200_262.tokens(__j_rename_at_1204_264,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
                            {
                                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1200_262),__j_rename_at_1204_264);
                                break;
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_1204_264));
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1201_263));
    };
}

inline void fmt_destructure_space_e9871049a6de7d91 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1215_265 )
{
    {
        bool __need_loop_1216 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1215_265.tokens)) - 1));
        int32_t __i_rename_at_1216_266;
        __need_loop_1216 = __i_iterator.first(__context__,(__i_rename_at_1216_266)) && __need_loop_1216;
        for ( ; __need_loop_1216 ; __need_loop_1216 = __i_iterator.next(__context__,(__i_rename_at_1216_266)) )
        {
            if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1215_265),__i_rename_at_1216_266,((char *) "let"),((char *) "var")) && (__ctx_rename_at_1215_265.tokens((__i_rename_at_1216_266 + 1),__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1215_265),__i_rename_at_1216_266 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1216_266));
    };
}

inline void do_format_4835418a8469e5fa ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1224_267 )
{
    parse_all_tokens_b5d5b68330cdfcd0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
    process_formatter_tokens_a63e2ffa4c8206b6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
    if ( have_formatter_token_296d7fc158126671(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267),((char *) "ignore-file")) )
    {
        return ;
    } else {
        mark_token_context_7dbf70435b518a0a(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_spaces_around_operators_9837b8f174165da7(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_function_call_param_paren_call_f740219cd0547b48(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_comma_7aa6a2faf3ce3f4f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_semicolon_29caf5fd09f6497e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_function_decl_param_paren_6c291b7c3a975893(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_type_colon_a5b252566f4d584(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_cast_type_446660f13b2ec290(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_comma_54b9cd4ba70794e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_semicolon_132b81d1431b5d49(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_inside_parens_fc44c6bd3c600b9c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_as_bb9fc0cabc4ff729(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_glue_type_specifiers_cb3a1c8b879530ae(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_keyword_2aab289129cf4fc8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_paren_a2ef6e80a339831f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_space_after_square_brackets_15fb170c80c43d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_function_arguments_7d9f22d6302573cb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_args_26ba700ce1a53dbc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_end_of_object_64e24a59c1d5b6b(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_object_type_298f1dcfc2f7829c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        fmt_destructure_space_e9871049a6de7d91(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_267));
        int32_t __top_rename_at_1256_268 = ((int32_t)(builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1224_267.tokens)) - 1));
        {
            bool __need_loop_1257 = true;
            // it: int const
            das_iterator<range> __it_iterator(range(0,16));
            int32_t __it_rename_at_1257_269;
            __need_loop_1257 = __it_iterator.first(__context__,(__it_rename_at_1257_269)) && __need_loop_1257;
            for ( ; __need_loop_1257 ; __need_loop_1257 = __it_iterator.next(__context__,(__it_rename_at_1257_269)) )
            {
                int32_t __i_rename_at_1258_270 = ((int32_t)(__top_rename_at_1256_268 - __it_rename_at_1257_269));
                if ( (__i_rename_at_1258_270 < 0) || !(builtin_empty(__ctx_rename_at_1224_267.tokens(__i_rename_at_1258_270,__context__).str)) )
                {
                    break;
                } else {
                    das_copy(__ctx_rename_at_1224_267.tokens(__i_rename_at_1258_270,__context__).spaces,0);
                };
            }
            __it_iterator.close(__context__,(__it_rename_at_1257_269));
        };
    };
}

inline char * format_source_string_f69d100c71c99a8c ( Context * __context__, char * const  & __file_data_rename_at_1267_271 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1268_272; memset((void*)&__ctx_rename_at_1268_272,0,sizeof(__ctx_rename_at_1268_272));
    /* finally */ auto __finally_1267= das_finally([&](){
    finalize_b9f001def6083360(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_272));
    /* end finally */ });
    das_zero(__ctx_rename_at_1268_272);
    _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1268_272.data),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
        TArray<uint8_t> ____acomp_1269_18_rename_at_1269_273;das_zero(____acomp_1269_18_rename_at_1269_273);
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_273),false);
        {
            bool __need_loop_1269 = true;
            // c: int const
            das_iterator<char * const > __c_iterator(__file_data_rename_at_1267_271);
            int32_t __c_rename_at_1269_274;
            __need_loop_1269 = __c_iterator.first(__context__,(__c_rename_at_1269_274)) && __need_loop_1269;
            for ( ; __need_loop_1269 ; __need_loop_1269 = __c_iterator.next(__context__,(__c_rename_at_1269_274)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_273),uint8_t(__c_rename_at_1269_274));
            }
            __c_iterator.close(__context__,(__c_rename_at_1269_274));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_273),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_273)));
    })));
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1268_272.data),0x0);
    do_format_4835418a8469e5fa(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_272));
    return das_auto_cast<char *>::cast(generate_source_64706a1ce7db4983(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_272)));
}

inline char * format_source_58c20f6bcb79eabd ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1276_275, TArray<uint8_t> const  &  __file_data_rename_at_1276_276 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1276_275.data),__file_data_rename_at_1276_276);
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1276_275.data),0x0);
    do_format_4835418a8469e5fa(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1276_275));
    return das_auto_cast<char *>::cast(generate_source_64706a1ce7db4983(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1276_275)));
}

inline char * format_source_f3ae203349d40bcb ( Context * __context__, TArray<uint8_t> const  &  __file_data_rename_at_1284_277 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1285_278; memset((void*)&__ctx_rename_at_1285_278,0,sizeof(__ctx_rename_at_1285_278));
    /* finally */ auto __finally_1284= das_finally([&](){
    finalize_b9f001def6083360(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1285_278));
    /* end finally */ });
    das_zero(__ctx_rename_at_1285_278);
    return das_auto_cast<char *>::cast(format_source_58c20f6bcb79eabd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1285_278),__file_data_rename_at_1284_277));
}

inline das_source_formatter::TokenTemplate TokenTemplate_a02a7415fdb9ba5a ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<das_source_formatter::TokenTemplate>::cast((([&]() -> das_source_formatter::TokenTemplate {
        das_source_formatter::TokenTemplate __mks_40;
        das_zero(__mks_40);
        das_copy((__mks_40.firstChar),(0));
        das_copy((__mks_40.str),(nullptr));
        das_copy((__mks_40.length),(0));
        return __mks_40;
    })()));
}

inline das_source_formatter::Token Token_c2eeda936a4bfe12 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<das_source_formatter::Token>::cast((([&]() -> das_source_formatter::Token {
        das_source_formatter::Token __mks_48;
        das_zero(__mks_48);
        das_copy((__mks_48.spaces),(0));
        das_copy((__mks_48.newLines),(0));
        das_copy((__mks_48.lineInSource),(1));
        das_copy((__mks_48.column),(1));
        das_copy((__mks_48.tokenType),(DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN));
        das_copy((__mks_48.isInFunctionParam),(false));
        das_copy((__mks_48.isInType),(false));
        das_copy((__mks_48.dontFormat),(false));
        das_copy((__mks_48.dontAddSpacesAround),(false));
        return __mks_48;
    })()));
}

inline das_source_formatter::FormatterCtx FormatterCtx_d786b6c38dffada3 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<das_source_formatter::FormatterCtx>::cast((([&]() -> das_source_formatter::FormatterCtx {
        das_source_formatter::FormatterCtx __mks_68;
        das_zero(__mks_68);
        das_copy((__mks_68.debugMode),(false));
        das_copy((__mks_68.indenting),(0));
        das_copy((__mks_68.insideOptions),(false));
        das_copy((__mks_68.crCount),(0));
        das_copy((__mks_68.lfCount),(0));
        return __mks_68;
    })()));
}

inline das_source_formatter::ParenCounter ParenCounter_77d11a7861c772f0 ( Context * __context__ )
{
    return /* <- */ das_auto_cast_move<das_source_formatter::ParenCounter>::cast((([&]() -> das_source_formatter::ParenCounter {
        das_source_formatter::ParenCounter __mks_380;
        das_copy((__mks_380.angle),(0));
        das_copy((__mks_380.paren),(0));
        das_copy((__mks_380.square),(0));
        das_copy((__mks_380.curly),(0));
        return __mks_380;
    })()));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xb78809ccdf1ed490] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_6a9d04f899c816ac>>();
    };
    aotLib[0x7c39494f594b2264] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_597eb4e562707157>>();
    };
    aotLib[0x4173506ace9573dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_d6c2d9467df86ea8>>();
    };
    aotLib[0xa8f929474bbb24c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e>>();
    };
    aotLib[0x36d68b931e0b4d3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_37238c3cdaec478a>>();
    };
    aotLib[0x8e2ff1ed2afbacc1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_94b9be375031ddbc>>();
    };
    aotLib[0xc3a1b928acaebfd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_9778601c0167a631>>();
    };
    aotLib[0xc502128bbf7c5d21] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176>>();
    };
    aotLib[0xa411e29d5b200de2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_5f7a4e8904f8977e>>();
    };
    aotLib[0x2c818f1a3ce08b0f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_d46b5589c5f4f04b>>();
    };
    aotLib[0x1e3b54847e499688] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_b4e09f66b8118eb0>>();
    };
    aotLib[0xa9429ad4e94b7726] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_4a7556d56e5a6ac1>>();
    };
    aotLib[0x45d41e57e6a0547a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53>>();
    };
    aotLib[0x8532fc80bb7a983f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c>>();
    };
    aotLib[0x913a8f2e9b96def6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d>>();
    };
    aotLib[0xc101143b8c47b79d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_540ecf6ef9e01052>>();
    };
    aotLib[0x4193f1bbb72e5078] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753>>();
    };
    aotLib[0x49a0a29a0e94f01d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a>>();
    };
    aotLib[0xbcc5889fbf62b93f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768>>();
    };
    aotLib[0xfd084015992baa1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0xcc074a37999289f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3>>();
    };
    aotLib[0xebe627d0e2eb8824] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622>>();
    };
    aotLib[0x79f2dbbc7a75a980] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65>>();
    };
    aotLib[0x209025c6d67841b0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673>>();
    };
    aotLib[0xd145739ee948bbc6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce>>();
    };
    aotLib[0xfda4983ce7ef7c6d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224>>();
    };
    aotLib[0xcba4b4c693ad27e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67>>();
    };
    aotLib[0x5cc7448b9c1eb4a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f>>();
    };
    aotLib[0xb547c53e3d68e8cc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856>>();
    };
    aotLib[0xd182f1155fbf3f83] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_b9f001def6083360>>();
    };
    aotLib[0xebf5b2593bb8dc0f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&initialize_token_templates_cd924eed93b743df>>();
    };
    aotLib[0x13fbbc0c63da3118] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isUtf8Bom_fc921f39aa33ab23>>();
    };
    aotLib[0xdf0e8bf73e50b162] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_1a08ceae13f47e89>>();
    };
    aotLib[0xc91df3ab4e854102] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_32a38f27b5380f0f>>();
    };
    aotLib[0x2ad99e5429080c63] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_f43d812e3a992e4>>();
    };
    aotLib[0x6f2a1442ff77bc9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_886fe8d1ff63f5fd>>();
    };
    aotLib[0xf3e9621e4b5e66bf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&new_line_before_56d9a06feda36687>>();
    };
    aotLib[0x45bdf460a7ac3379] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&next_char_e34bb0a357da1559>>();
    };
    aotLib[0xd9615e11e6866095] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&skip_spaces_a5027bfd73b7bde6>>();
    };
    aotLib[0xebf4684b0cc035d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&on_token_added_6925eddc2a7ca309>>();
    };
    aotLib[0xeeda67f43c5c866a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&push_token_92f9637f04a53e36>>();
    };
    aotLib[0x4cd95b149ca726e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&parse_token_95969fa7ff7b12c4>>();
    };
    aotLib[0x52d8f0772589855e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&parse_all_tokens_b5d5b68330cdfcd0>>();
    };
    aotLib[0x8ef31d506691d6ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&update_paren_62e64d469829ae3d>>();
    };
    aotLib[0x2b882c644a7adfac] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_paren_90acbe7a3b864d93>>();
    };
    aotLib[0xf24a0c0aa689fa7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_square_68d9ea1d7c30e49f>>();
    };
    aotLib[0x95ec388eb8f5ee01] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_angle_89495f7d100107b7>>();
    };
    aotLib[0x862582ac2965df08] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_range_as_type_24b8f876667238a0>>();
    };
    aotLib[0xddab6cda8c22bd55] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_tokens_as_type_5e4d056374f6927c>>();
    };
    aotLib[0x3252c8719473f327] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb>>();
    };
    aotLib[0x8157b91b926f811d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&search_token_in_line_3a9d4fd472c436b3>>();
    };
    aotLib[0x53b7dd25c9bdd4ae] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&process_formatter_tokens_a63e2ffa4c8206b6>>();
    };
    aotLib[0x9e31b01c3bf1c57] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&have_formatter_token_296d7fc158126671>>();
    };
    aotLib[0x7f7f61fe2bb52c8a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_token_context_7dbf70435b518a0a>>();
    };
    aotLib[0x759f4146b99597f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_arguments_7d9f22d6302573cb>>();
    };
    aotLib[0xda3f5313e831cdcd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_args_26ba700ce1a53dbc>>();
    };
    aotLib[0x929083012e3b9e12] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_print_tokens_927617e00a5e50a7>>();
    };
    aotLib[0xc0f03fce14842ae8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&generate_source_64706a1ce7db4983>>();
    };
    aotLib[0x860f3b59063c225b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&need_spaces_around_7179a668ae90ccd6>>();
    };
    aotLib[0x2f30a576df51b8de] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&need_space_only_before_584e48acb8e8a70>>();
    };
    aotLib[0x97f7f44ac8b05577] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_unary_62ad92645c7f9ee6>>();
    };
    aotLib[0x852e1a05423ed300] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&remove_spaces_around_1c96d2bcc36445d1>>();
    };
    aotLib[0xc6f5214396d63e44] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_spaces_around_9b0360b9b4d718c3>>();
    };
    aotLib[0x8e1b59f43d841956] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_comma_7aa6a2faf3ce3f4f>>();
    };
    aotLib[0xcd63919adbcd7219] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_semicolon_29caf5fd09f6497e>>();
    };
    aotLib[0x9b6b65512db6483e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_comma_54b9cd4ba70794e>>();
    };
    aotLib[0xd7464f544ade359b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_semicolon_132b81d1431b5d49>>();
    };
    aotLib[0x61b3fa779d780a56] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_type_colon_a5b252566f4d584>>();
    };
    aotLib[0x2829808f398f0a03] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_decl_param_paren_6c291b7c3a975893>>();
    };
    aotLib[0xc1764d5124503184] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_call_param_paren_call_f740219cd0547b48>>();
    };
    aotLib[0xc4f6b82a1c21b819] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_spaces_around_operators_9837b8f174165da7>>();
    };
    aotLib[0x653899930a808ccf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_keyword_2aab289129cf4fc8>>();
    };
    aotLib[0x166f8e87303b8e25] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_paren_a2ef6e80a339831f>>();
    };
    aotLib[0x9234aa466b5c4975] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_square_brackets_15fb170c80c43d93>>();
    };
    aotLib[0xcbec5c1aa73e9dd9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_inside_parens_fc44c6bd3c600b9c>>();
    };
    aotLib[0x984d83a065c4f46d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_as_bb9fc0cabc4ff729>>();
    };
    aotLib[0x8e2f92f73eabb71a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_end_of_object_64e24a59c1d5b6b>>();
    };
    aotLib[0xe364a5cf53ef43ed] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_object_type_298f1dcfc2f7829c>>();
    };
    aotLib[0xa7d13fcbb75338bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1>>();
    };
    aotLib[0x454312f88c424971] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_glue_type_specifiers_cb3a1c8b879530ae>>();
    };
    aotLib[0x80c3da5513cb2f43] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_cast_type_446660f13b2ec290>>();
    };
    aotLib[0xd91867955dba6780] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_destructure_space_e9871049a6de7d91>>();
    };
    aotLib[0x76299977413a1e82] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&do_format_4835418a8469e5fa>>();
    };
    aotLib[0x26d8db9170833ef9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_string_f69d100c71c99a8c>>();
    };
    aotLib[0xbed3d0168e4fcf75] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_58c20f6bcb79eabd>>();
    };
    aotLib[0x400629cb54db70cf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_f3ae203349d40bcb>>();
    };
    aotLib[0xb985c77f13386e53] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&TokenTemplate_a02a7415fdb9ba5a>>();
    };
    aotLib[0x25de37d249637ff5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Token_c2eeda936a4bfe12>>();
    };
    aotLib[0x194c5a22b76bc6f5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&FormatterCtx_d786b6c38dffada3>>();
    };
    aotLib[0xb694cd730b4a30bc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ParenCounter_77d11a7861c772f0>>();
    };
    // [[ init script ]]
    aotLib[0x4ec73327d84f23b2] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_15051537126579498552
AotListBase impl_aot_das_source_formatter(_anon_15051537126579498552::registerAotFunctions);
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
