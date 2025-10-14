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
namespace _anon_6368649739184819056 {

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

VarInfo __struct_info__bf67b648e8f141ed_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0xf1744ecb99b5ee52), "firstChar", offsetof(das_source_formatter::TokenTemplate,firstChar), 0 };
VarInfo __struct_info__bf67b648e8f141ed_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, TypeSize<char *>::size, UINT64_C(0x993f8b12277aa369), "str", offsetof(das_source_formatter::TokenTemplate,str), 2 };
VarInfo __struct_info__bf67b648e8f141ed_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xe807e1b774135f34), "codes", offsetof(das_source_formatter::TokenTemplate,codes), 4 };
VarInfo __struct_info__bf67b648e8f141ed_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<int32_t>::size, UINT64_C(0x12c1af86475d4616), "length", offsetof(das_source_formatter::TokenTemplate,length), 0 };
VarInfo * __struct_info__bf67b648e8f141ed_fields[4] =  { &__struct_info__bf67b648e8f141ed_field_0, &__struct_info__bf67b648e8f141ed_field_1, &__struct_info__bf67b648e8f141ed_field_2, &__struct_info__bf67b648e8f141ed_field_3 };
StructInfo __struct_info__bf67b648e8f141ed = {"TokenTemplate", "das_source_formatter", 28, __struct_info__bf67b648e8f141ed_fields, 4, TypeSize<das_source_formatter::TokenTemplate>::size, UINT64_C(0x0), nullptr, UINT64_C(0xbf67b648e8f141ed), 1 };
TypeInfo __type_info__44217c02c8384397 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6cf57a81c8f2e7f7, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<TArray<das_source_formatter::TokenTemplate>>::size, UINT64_C(0x44217c02c8384397) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__6cf57a81c8f2e7f7 = { Type::tStructure, &__struct_info__bf67b648e8f141ed, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, TypeSize<das_source_formatter::TokenTemplate>::size, UINT64_C(0x6cf57a81c8f2e7f7) };
TypeInfo __type_info__a8d2247d5b8ed91f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0xa8d2247d5b8ed91f) };
TypeInfo __type_info__7dcc2c583c294a1f = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, TypeSize<TArray<uint8_t>>::size, UINT64_C(0x7dcc2c583c294a1f) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, TypeSize<uint8_t>::size, UINT64_C(0xaf5be84c85f468f0) };

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

inline void finalize_470a2c87c10459f3 ( Context * __context__, das_source_formatter::TokenTemplate & ____this_rename_at_40_0 );
inline void finalize_90b2200a89e5f7af ( Context * __context__, das_source_formatter::Token & ____this_rename_at_48_1 );
inline void finalize_8ce52f97d3e92f29 ( Context * __context__, das_source_formatter::FormatterToken & ____this_rename_at_62_2 );
inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab8b80dafe942dee ( Context * __context__, TArray<uint8_t> & __a_rename_at_50_3 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_e9811f8b8637d85b ( Context * __context__, TDim<char *,22> const  & __a_rename_at_581_4 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_4839b0184671562d ( Context * __context__, TDim<char *,21> const  & __a_rename_at_581_5 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_b38e09a15f381650 ( Context * __context__, TDim<char *,11> const  & __a_rename_at_581_6 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ee4a37bbcf70fe7d ( Context * __context__, TArray<das_source_formatter::TokenTemplate> & __a_rename_at_1234_7 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3c986bfd8563d8b2 ( Context * __context__, TArray<das_source_formatter::Token> & __a_rename_at_1234_9 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93cd492387de05e8 ( Context * __context__, TArray<das_source_formatter::FormatterToken> & __a_rename_at_1234_11 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_fb7a66fc6a63821a ( Context * __context__, TArray<das_source_formatter::TokenTemplate> & __Arr_rename_at_68_13, int32_t __newSize_rename_at_68_14 );
inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_5f8f761d5f23964f ( Context * __context__, TTable<char *,void> const  & __Tab_rename_at_1047_15, char * const  __at_rename_at_1047_16 );
inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_7926e574de26b25d ( Context * __context__, TArray<uint8_t> & __a_rename_at_32_17, TArray<uint8_t> & __b_rename_at_32_18 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_6a9e877c15ce25b4 ( Context * __context__, TDim<char *,3> const  & __a_rename_at_581_19 );
inline void _FuncbuiltinTickresizeTick4811697762258667383_cb951de01ca87a4e ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_7e6de3088c45b5fe ( Context * __context__, TDim<char *,40> const  & __a_rename_at_581_22 );
inline void _FuncbuiltinTickpushTick14133213201864676143_7d58ec4da1ec1b17 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_165_23, das_source_formatter::Token const  & __value_rename_at_165_24 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_7889f698fd41c370 ( Context * __context__, TDim<char *,22> const  & __arr_rename_at_1710_25, char * const  __key_rename_at_1710_26 );
inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_c018a9675934bba9 ( Context * __context__, TDim<char *,8> const  & __a_rename_at_1469_28 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_adfb084cc10e1733 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_125_31, int32_t __newSize_rename_at_125_32 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e ( Context * __context__, TArray<uint8_t> & __a_rename_at_1234_33 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9 ( Context * __context__, TDim<char *,21> const  & __arr_rename_at_1710_34, char * const  __key_rename_at_1710_35 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_feb73043e4854e1f ( Context * __context__, TDim<char *,11> const  & __arr_rename_at_1710_37, char * const  __key_rename_at_1710_38 );
inline void _FuncbuiltinTickpushTick10769833213962245646_e919271fa2eab772 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_181_40, das_source_formatter::Token & __value_rename_at_181_41 );
inline void _FuncbuiltinTickpushTick10769833213962245646_fc8141dc0153320a ( Context * __context__, TArray<das_source_formatter::FormatterToken> & __Arr_rename_at_181_42, das_source_formatter::FormatterToken & __value_rename_at_181_43 );
inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_944_44, int32_t __i_rename_at_944_45 );
inline void _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_46, uint8_t __value_rename_at_165_47 );
inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_936_48, int32_t __i_rename_at_936_49 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a4c49c41e04684c4 ( Context * __context__, TDim<char *,3> const  & __arr_rename_at_1710_50, char * const  __key_rename_at_1710_51 );
inline void finalize_db456bcf8e957ca4 ( Context * __context__, das_source_formatter::FormatterCtx & ____this_rename_at_68_53 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_11c19903beee414c ( Context * __context__, TArray<uint8_t> & __a_rename_at_1113_54, TArray<uint8_t> const  & __b_rename_at_1113_55 );
inline void initialize_token_templates_c0f484bb6b01366 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_89_57 );
inline bool isUtf8Bom_d46057f250b5bf59 ( Context * __context__, TArray<uint8_t> & __text_rename_at_103_64 );
inline bool eq_5251ca869c24b49f ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_109_65, int32_t __index_rename_at_109_66, char * const  __s_rename_at_109_67 );
inline bool eq_3d631d49cf7e1d ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_114_68, int32_t __index_rename_at_114_69, char * const  __s0_rename_at_114_70, char * const  __s1_rename_at_114_71 );
inline bool eq_cecc7c5a47abfe6c ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_120_72, int32_t __index_rename_at_120_73, char * const  __s0_rename_at_120_74, char * const  __s1_rename_at_120_75, char * const  __s2_rename_at_120_76 );
inline bool eq_5ccd37f4ada4b6c8 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_126_77, int32_t __index_rename_at_126_78, char * const  __s0_rename_at_126_79, char * const  __s1_rename_at_126_80, char * const  __s2_rename_at_126_81, char * const  __s3_rename_at_126_82 );
inline bool new_line_before_74a9e3638b00c26f ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_132_83, int32_t __index_rename_at_132_84 );
inline void next_char_a68ee7affe47d0eb ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_137_85 );
inline void skip_spaces_2fa97eb70a48fea7 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_179_87 );
inline void on_token_added_ec8ba2110b19573e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_186_88 );
inline void push_token_cc86b5bb25bf7adc ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_204_89, int32_t __from_char_idx_rename_at_204_90, int32_t __to_char_idx_rename_at_204_91, int32_t __space_count_rename_at_204_92, int32_t __new_lines_rename_at_204_93, int32_t __src_line_rename_at_204_94, int32_t __column_rename_at_204_95, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_96 );
inline void parse_token_321b59f95fe107c9 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_220_100 );
inline void parse_all_tokens_29fbd1ed87913a0a ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_344_119 );
inline void update_paren_3503759ee55e89b1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_388_122, int32_t __index_rename_at_388_123, das_source_formatter::ParenCounter & __paren_counter_rename_at_388_124 );
inline int32_t find_pair_paren_ff2776f9d2856b4e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_425_125, int32_t __from_index_rename_at_425_126 );
inline int32_t find_pair_square_89e4c86c49f03d61 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_437_129, int32_t __from_index_rename_at_437_130 );
inline int32_t find_pair_angle_43f908d2d3e3a500 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_449_133, int32_t __from_index_rename_at_449_134 );
inline void mark_range_as_type_1c15599df80afc04 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_461_137, int32_t __from_rename_at_461_138, int32_t __to_rename_at_461_139 );
inline void mark_tokens_as_type_ff7631c34d0e20a8 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_470_141, int32_t __i_rename_at_470_142 );
inline void mark_tokens_as_type_inside_angle_5588509bf83aae77 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_528_145, int32_t __i_rename_at_528_146 );
inline int32_t search_token_in_line_41b34300f8492a5f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_542_149, int32_t __from_index_rename_at_542_150, char * const  __str_rename_at_542_151 );
inline void process_formatter_tokens_b7e8fa22d8896a75 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_564_154 );
inline bool have_formatter_token_68c38613f91cfa3f ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_581_160, char * const  __command_rename_at_581_161 );
inline void mark_token_context_7d9da6c542abc3d5 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_591_163 );
inline void fmt_function_arguments_393a0d20cb9876da ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_791_197 );
inline void fmt_remove_space_before_args_b613d1f002f609af ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_832_203 );
inline void debug_print_tokens_d5c3886aafc47666 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_841_205 );
inline char * generate_source_d9e23c7a7a794181 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_848_207 );
inline bool need_spaces_around_c85752147cfda42d ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_891_216, int32_t __index_rename_at_891_217 );
inline bool need_space_only_before_a3979a2c621820ef ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_912_218, int32_t __index_rename_at_912_219 );
inline bool is_unary_b0c7219745029c11 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_956_220, int32_t __index_rename_at_956_221 );
inline void remove_spaces_around_203ede7b7eebc7c5 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_979_223, int32_t __index_rename_at_979_224 );
inline void make_spaces_around_80d61c2838123a7d ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_989_225, int32_t __index_rename_at_989_226 );
inline void fmt_space_after_comma_f91c855ec419635f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_999_227 );
inline void fmt_space_after_semicolon_23f19dedec62b617 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1008_229 );
inline void fmt_remove_space_before_comma_e4ab43db37a70c74 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1017_231 );
inline void fmt_remove_space_before_semicolon_e5d8826c5665e6be ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1026_233 );
inline void fmt_type_colon_400b4390d7623d95 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1035_235 );
inline void fmt_function_decl_param_paren_dac5114d61e417ed ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1047_237 );
inline void fmt_function_call_param_paren_call_347b6df2d6483625 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1062_239 );
inline void fmt_spaces_around_operators_53d08ef306f852f9 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1073_242 );
inline void fmt_space_after_keyword_2536c1e9542cdc3b ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1092_244 );
inline void fmt_space_after_paren_82d46f74b7d5652 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1114_246 );
inline void fmt_space_after_square_brackets_fdbea403935e8e12 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1123_248 );
inline void fmt_remove_space_inside_parens_cb72085f2dcb9ecd ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1132_250 );
inline void fmt_remove_space_before_as_865a2960d4580501 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1144_252 );
inline void fmt_remove_space_before_end_of_object_510336d353cc43f1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1153_254 );
inline void fmt_remove_space_before_object_type_fff6f6cd3369398c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1163_256 );
inline void fmt_remove_space_before_array_of_type_fa116a3241d9e90c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1173_258 );
inline void fmt_glue_type_specifiers_538bf5888d1be714 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1185_260 );
inline void fmt_space_after_cast_type_5d3d2f4d38f83d01 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1205_262 );
inline void fmt_destructure_space_2fcf524fd08df174 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1220_265 );
inline void do_format_ff67931097f9f551 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1229_267 );
inline char * format_source_string_53ea2afc74eb7140 ( Context * __context__, char * const  & __file_data_rename_at_1272_271 );
inline char * format_source_555ab24d2bad5d9a ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1281_275, TArray<uint8_t> const  & __file_data_rename_at_1281_276 );
inline char * format_source_82aae480574c279c ( Context * __context__, TArray<uint8_t> const  & __file_data_rename_at_1289_277 );
inline das_source_formatter::TokenTemplate TokenTemplate_bb20a1d61f51dcf0 ( Context * __context__ );
inline das_source_formatter::Token Token_43fa8756d37aefe0 ( Context * __context__ );
inline das_source_formatter::FormatterCtx FormatterCtx_7640c15af9f3493d ( Context * __context__ );
inline das_source_formatter::ParenCounter ParenCounter_76ee88379e1e5165 ( Context * __context__ );

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
        TDim<char *,3> __mka_1183;
        __mka_1183(0,__context__) = ((char *) "?");
        __mka_1183(1,__context__) = ((char *) "&");
        __mka_1183(2,__context__) = ((char *) "#");
        return __mka_1183;
    })());/*type_specifiers*/
}

inline void finalize_470a2c87c10459f3 ( Context * __context__, das_source_formatter::TokenTemplate &  ____this_rename_at_40_0 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_40_0.codes));
    memset((void*)&(____this_rename_at_40_0), 0, TypeSize<das_source_formatter::TokenTemplate>::size);
}

inline void finalize_90b2200a89e5f7af ( Context * __context__, das_source_formatter::Token &  ____this_rename_at_48_1 )
{
    memset((void*)&(____this_rename_at_48_1), 0, TypeSize<das_source_formatter::Token>::size);
}

inline void finalize_8ce52f97d3e92f29 ( Context * __context__, das_source_formatter::FormatterToken &  ____this_rename_at_62_2 )
{
    memset((void*)&(____this_rename_at_62_2), 0, TypeSize<das_source_formatter::FormatterToken>::size);
}

inline TArray<uint8_t> & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab8b80dafe942dee ( Context * __context__, TArray<uint8_t> &  __a_rename_at_50_3 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<TArray<uint8_t> &>::from(__a_rename_at_50_3)));
    return das_auto_cast_ref<TArray<uint8_t> &>::cast(__a_rename_at_50_3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_e9811f8b8637d85b ( Context * __context__, TDim<char *,22> const  &  __a_rename_at_581_4 )
{
    return das_auto_cast<int32_t>::cast(22);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_4839b0184671562d ( Context * __context__, TDim<char *,21> const  &  __a_rename_at_581_5 )
{
    return das_auto_cast<int32_t>::cast(21);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_b38e09a15f381650 ( Context * __context__, TDim<char *,11> const  &  __a_rename_at_581_6 )
{
    return das_auto_cast<int32_t>::cast(11);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ee4a37bbcf70fe7d ( Context * __context__, TArray<das_source_formatter::TokenTemplate> &  __a_rename_at_1234_7 )
{
    {
        bool __need_loop_1236 = true;
        // aV: das_source_formatter::TokenTemplate aka TT&
        das_iterator<TArray<das_source_formatter::TokenTemplate>> __aV_iterator(__a_rename_at_1234_7);
        das_source_formatter::TokenTemplate * __aV_rename_at_1236_8;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_8)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_8)) )
        {
            finalize_470a2c87c10459f3(__context__,das_arg<das_source_formatter::TokenTemplate>::pass((*__aV_rename_at_1236_8)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_8));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__a_rename_at_1234_7),48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3c986bfd8563d8b2 ( Context * __context__, TArray<das_source_formatter::Token> &  __a_rename_at_1234_9 )
{
    {
        bool __need_loop_1236 = true;
        // aV: das_source_formatter::Token aka TT&
        das_iterator<TArray<das_source_formatter::Token>> __aV_iterator(__a_rename_at_1234_9);
        das_source_formatter::Token * __aV_rename_at_1236_10;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_10)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_10)) )
        {
            finalize_90b2200a89e5f7af(__context__,das_arg<das_source_formatter::Token>::pass((*__aV_rename_at_1236_10)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_10));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::Token>>::pass(__a_rename_at_1234_9),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_93cd492387de05e8 ( Context * __context__, TArray<das_source_formatter::FormatterToken> &  __a_rename_at_1234_11 )
{
    {
        bool __need_loop_1236 = true;
        // aV: das_source_formatter::FormatterToken aka TT&
        das_iterator<TArray<das_source_formatter::FormatterToken>> __aV_iterator(__a_rename_at_1234_11);
        das_source_formatter::FormatterToken * __aV_rename_at_1236_12;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_12)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_12)) )
        {
            finalize_8ce52f97d3e92f29(__context__,das_arg<das_source_formatter::FormatterToken>::pass((*__aV_rename_at_1236_12)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_12));
    };
    builtin_array_free(das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__a_rename_at_1234_11),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_fb7a66fc6a63821a ( Context * __context__, TArray<das_source_formatter::TokenTemplate> &  __Arr_rename_at_68_13, int32_t __newSize_rename_at_68_14 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_1,cast<TArray<das_source_formatter::TokenTemplate> &>::from(__Arr_rename_at_68_13)));
    builtin_array_resize(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__Arr_rename_at_68_13),__newSize_rename_at_68_14,48,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline bool _FuncbuiltinTickkey_existsTick16808803843923989214_5f8f761d5f23964f ( Context * __context__, TTable<char *,void> const  &  __Tab_rename_at_1047_15, char * const  __at_rename_at_1047_16 )
{
    return das_auto_cast<bool>::cast(__builtin_table_key_exists(__context__,__Tab_rename_at_1047_15,__at_rename_at_1047_16));
}

inline void _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_7926e574de26b25d ( Context * __context__, TArray<uint8_t> &  __a_rename_at_32_17, TArray<uint8_t> &  __b_rename_at_32_18 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_2,cast<TArray<uint8_t> &>::from(__a_rename_at_32_17)));
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_3,cast<TArray<uint8_t> &>::from(__b_rename_at_32_18)));
    das_move(__a_rename_at_32_17,__b_rename_at_32_18);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_6a9e877c15ce25b4 ( Context * __context__, TDim<char *,3> const  &  __a_rename_at_581_19 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_cb951de01ca87a4e ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_68_20, int32_t __newSize_rename_at_68_21 )
{
    builtin_array_resize(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_68_20),__newSize_rename_at_68_21,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_7e6de3088c45b5fe ( Context * __context__, TDim<char *,40> const  &  __a_rename_at_581_22 )
{
    return das_auto_cast<int32_t>::cast(40);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_7d58ec4da1ec1b17 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_165_23, das_source_formatter::Token const  &  __value_rename_at_165_24 )
{
    das_copy(__Arr_rename_at_165_23(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_165_23),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_24);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_7889f698fd41c370 ( Context * __context__, TDim<char *,22> const  &  __arr_rename_at_1710_25, char * const  __key_rename_at_1710_26 )
{
    {
        bool __need_loop_1711 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,22));
        int32_t __i_rename_at_1711_27;
        __need_loop_1711 = __i_iterator.first(__context__,(__i_rename_at_1711_27)) && __need_loop_1711;
        for ( ; __need_loop_1711 ; __need_loop_1711 = __i_iterator.next(__context__,(__i_rename_at_1711_27)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1710_25(__i_rename_at_1711_27,__context__)),cast<char *>::from(__key_rename_at_1710_26),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1711_27);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1711_27));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_c018a9675934bba9 ( Context * __context__, TDim<char *,8> const  &  __a_rename_at_1469_28 )
{
    TTable<char *,void> __tab_rename_at_1470_29;das_zero(__tab_rename_at_1470_29);
    {
        bool __need_loop_1471 = true;
        // at: string aka keyT const&
        das_iterator<TDim<char *,8> const > __at_iterator(__a_rename_at_1469_28);
        char * const  * __at_rename_at_1471_30;
        __need_loop_1471 = __at_iterator.first(__context__,(__at_rename_at_1471_30)) && __need_loop_1471;
        for ( ; __need_loop_1471 ; __need_loop_1471 = __at_iterator.next(__context__,(__at_rename_at_1471_30)) )
        {
            __builtin_table_set_insert(__context__,__tab_rename_at_1470_29,(*__at_rename_at_1471_30));
        }
        __at_iterator.close(__context__,(__at_rename_at_1471_30));
    };
    return /* <- */ das_auto_cast_move<TTable<char *,void>>::cast(__tab_rename_at_1470_29);
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_adfb084cc10e1733 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_125_31, int32_t __newSize_rename_at_125_32 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_125_31),__newSize_rename_at_125_32,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1234_33 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1234_33),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9 ( Context * __context__, TDim<char *,21> const  &  __arr_rename_at_1710_34, char * const  __key_rename_at_1710_35 )
{
    {
        bool __need_loop_1711 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,21));
        int32_t __i_rename_at_1711_36;
        __need_loop_1711 = __i_iterator.first(__context__,(__i_rename_at_1711_36)) && __need_loop_1711;
        for ( ; __need_loop_1711 ; __need_loop_1711 = __i_iterator.next(__context__,(__i_rename_at_1711_36)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1710_34(__i_rename_at_1711_36,__context__)),cast<char *>::from(__key_rename_at_1710_35),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1711_36);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1711_36));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_feb73043e4854e1f ( Context * __context__, TDim<char *,11> const  &  __arr_rename_at_1710_37, char * const  __key_rename_at_1710_38 )
{
    {
        bool __need_loop_1711 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,11));
        int32_t __i_rename_at_1711_39;
        __need_loop_1711 = __i_iterator.first(__context__,(__i_rename_at_1711_39)) && __need_loop_1711;
        for ( ; __need_loop_1711 ; __need_loop_1711 = __i_iterator.next(__context__,(__i_rename_at_1711_39)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1710_37(__i_rename_at_1711_39,__context__)),cast<char *>::from(__key_rename_at_1710_38),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1711_39);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1711_39));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_e919271fa2eab772 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_181_40, das_source_formatter::Token &  __value_rename_at_181_41 )
{
    das_copy(__Arr_rename_at_181_40(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_181_40),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_41);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_fc8141dc0153320a ( Context * __context__, TArray<das_source_formatter::FormatterToken> &  __Arr_rename_at_181_42, das_source_formatter::FormatterToken &  __value_rename_at_181_43 )
{
    das_copy(__Arr_rename_at_181_42(builtin_array_push_back(das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__Arr_rename_at_181_42),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_43);
}

inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_944_44, int32_t __i_rename_at_944_45 )
{
    if ( (__ctx_rename_at_944_44.tokens(__i_rename_at_944_45,__context__).dontFormat || __ctx_rename_at_944_44.tokens((__i_rename_at_944_45 - 1),__context__).dontFormat) || new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_944_44),__i_rename_at_944_45) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_944_44.tokens(__i_rename_at_944_45,__context__).spaces,0);
    };
}

inline void _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_46, uint8_t __value_rename_at_165_47 )
{
    das_copy(__Arr_rename_at_165_46(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_46),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_47);
}

inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_936_48, int32_t __i_rename_at_936_49 )
{
    if ( (__ctx_rename_at_936_48.tokens(__i_rename_at_936_49,__context__).dontFormat || __ctx_rename_at_936_48.tokens((__i_rename_at_936_49 - 1),__context__).dontFormat) || new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_936_48),__i_rename_at_936_49) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_936_48.tokens(__i_rename_at_936_49,__context__).spaces,1);
    };
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a4c49c41e04684c4 ( Context * __context__, TDim<char *,3> const  &  __arr_rename_at_1710_50, char * const  __key_rename_at_1710_51 )
{
    {
        bool __need_loop_1711 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,3));
        int32_t __i_rename_at_1711_52;
        __need_loop_1711 = __i_iterator.first(__context__,(__i_rename_at_1711_52)) && __need_loop_1711;
        for ( ; __need_loop_1711 ; __need_loop_1711 = __i_iterator.next(__context__,(__i_rename_at_1711_52)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1710_50(__i_rename_at_1711_52,__context__)),cast<char *>::from(__key_rename_at_1710_51),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1711_52);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1711_52));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void finalize_db456bcf8e957ca4 ( Context * __context__, das_source_formatter::FormatterCtx &  ____this_rename_at_68_53 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_ee4a37bbcf70fe7d(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(____this_rename_at_68_53.tokenTemplates));
    _FuncbuiltinTickfinalizeTick13836114024949725080_3c986bfd8563d8b2(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(____this_rename_at_68_53.tokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_93cd492387de05e8(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(____this_rename_at_68_53.formatterTokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_68_53.data));
    memset((void*)&(____this_rename_at_68_53), 0, TypeSize<das_source_formatter::FormatterCtx>::size);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_11c19903beee414c ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1113_54, TArray<uint8_t> const  &  __b_rename_at_1113_55 )
{
    int32_t __ln_rename_at_1114_56 = ((int32_t)builtin_array_size(__b_rename_at_1113_55));
    _FuncbuiltinTickresizeTick4811697762258667383_cb951de01ca87a4e(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1113_54),__ln_rename_at_1114_56);
    if ( __ln_rename_at_1114_56 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1113_54(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1113_55(0,__context__))),__ln_rename_at_1114_56 * 1);
    };
}

inline void initialize_token_templates_c0f484bb6b01366 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_89_57 )
{
    if ( builtin_array_size(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_57.tokenTemplates)) > 0 )
    {
        return ;
    } else {
        _FuncbuiltinTickresizeTick4811697762258667383_fb7a66fc6a63821a(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_57.tokenTemplates),40);
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
                _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_7926e574de26b25d(__context__,das_arg<TArray<uint8_t>>::pass((*__tt_rename_at_94_61).codes),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
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
                            _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62),uint8_t(__c_rename_at_96_63));
                        }
                        __c_iterator.close(__context__,(__c_rename_at_96_63));
                    };
                    builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62),true);
                    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab8b80dafe942dee(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_62)));
                })));
                das_copy((*__tt_rename_at_94_61).firstChar,int32_t((*__tt_rename_at_94_61).codes(0,__context__)));
                das_copy((*__tt_rename_at_94_61).length,builtin_string_length((*__s_rename_at_94_60),__context__));
            }
            __s_iterator.close(__context__,(__s_rename_at_94_60));
            __tt_iterator.close(__context__,(__tt_rename_at_94_61));
        };
    };
}

inline bool isUtf8Bom_d46057f250b5bf59 ( Context * __context__, TArray<uint8_t> &  __text_rename_at_103_64 )
{
    return das_auto_cast<bool>::cast((((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__text_rename_at_103_64)) >= 3) && (uint32_t(__text_rename_at_103_64(0,__context__)) == 0xefu)) && (uint32_t(__text_rename_at_103_64(1,__context__)) == 0xbbu)) && (uint32_t(__text_rename_at_103_64(2,__context__)) == 0xbfu));
}

inline bool eq_5251ca869c24b49f ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_109_65, int32_t __index_rename_at_109_66, char * const  __s_rename_at_109_67 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_109_66 < 0) || (__index_rename_at_109_66 >= builtin_array_size(__ctx_rename_at_109_65.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_109_65.tokens(__index_rename_at_109_66,__context__).str),cast<char *>::from(__s_rename_at_109_67),*__context__,nullptr))));
}

inline bool eq_3d631d49cf7e1d ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_114_68, int32_t __index_rename_at_114_69, char * const  __s0_rename_at_114_70, char * const  __s1_rename_at_114_71 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_114_69 < 0) || (__index_rename_at_114_69 >= builtin_array_size(__ctx_rename_at_114_68.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_68.tokens(__index_rename_at_114_69,__context__).str),cast<char *>::from(__s0_rename_at_114_70),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_68.tokens(__index_rename_at_114_69,__context__).str),cast<char *>::from(__s1_rename_at_114_71),*__context__,nullptr)))));
}

inline bool eq_cecc7c5a47abfe6c ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_120_72, int32_t __index_rename_at_120_73, char * const  __s0_rename_at_120_74, char * const  __s1_rename_at_120_75, char * const  __s2_rename_at_120_76 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_120_73 < 0) || (__index_rename_at_120_73 >= builtin_array_size(__ctx_rename_at_120_72.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s0_rename_at_120_74),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s1_rename_at_120_75),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_72.tokens(__index_rename_at_120_73,__context__).str),cast<char *>::from(__s2_rename_at_120_76),*__context__,nullptr)))));
}

inline bool eq_5ccd37f4ada4b6c8 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_126_77, int32_t __index_rename_at_126_78, char * const  __s0_rename_at_126_79, char * const  __s1_rename_at_126_80, char * const  __s2_rename_at_126_81, char * const  __s3_rename_at_126_82 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_126_78 < 0) || (__index_rename_at_126_78 >= builtin_array_size(__ctx_rename_at_126_77.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s0_rename_at_126_79),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s1_rename_at_126_80),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s2_rename_at_126_81),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_77.tokens(__index_rename_at_126_78,__context__).str),cast<char *>::from(__s3_rename_at_126_82),*__context__,nullptr)))));
}

inline bool new_line_before_74a9e3638b00c26f ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_132_83, int32_t __index_rename_at_132_84 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_132_84 <= 1) || (__index_rename_at_132_84 >= builtin_array_size(__ctx_rename_at_132_83.tokens))) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast((__ctx_rename_at_132_83.tokens(__index_rename_at_132_84,__context__).newLines > 0)));
}

inline void next_char_a68ee7affe47d0eb ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_137_85 )
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

inline void skip_spaces_2fa97eb70a48fea7 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_179_87 )
{
    while ( (((__ctx_rename_at_179_87.c == 32) || (__ctx_rename_at_179_87.c == 9)) || (__ctx_rename_at_179_87.c == 13)) || (__ctx_rename_at_179_87.c == 10) )
    {
        next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_179_87));
    };
}

inline void on_token_added_ec8ba2110b19573e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_186_88 )
{
    if ( (((__ctx_rename_at_186_88.indenting == 0) && (__ctx_rename_at_186_88.newLineCounter > 0)) && (__ctx_rename_at_186_88.spaceCounter > 0)) && (__ctx_rename_at_186_88.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        das_copy(__ctx_rename_at_186_88.indenting,__ctx_rename_at_186_88.spaceCounter);
    };
    if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_186_88),builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_88.tokens)) - 1) )
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

inline void push_token_cc86b5bb25bf7adc ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_204_89, int32_t __from_char_idx_rename_at_204_90, int32_t __to_char_idx_rename_at_204_91, int32_t __space_count_rename_at_204_92, int32_t __new_lines_rename_at_204_93, int32_t __src_line_rename_at_204_94, int32_t __column_rename_at_204_95, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_96 )
{
    TArray<uint8_t> __buf_rename_at_205_97; memset((void*)&__buf_rename_at_205_97,0,sizeof(__buf_rename_at_205_97));
    das_source_formatter::Token _temp_make_local_206_23_0; _temp_make_local_206_23_0;
    /* finally */ auto __finally_204= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e(__context__,das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_97));
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
                _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_98),uint8_t(__ctx_rename_at_204_89.data(__i_rename_at_205_99,__context__)));
            }
            __i_iterator.close(__context__,(__i_rename_at_205_99));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_98),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(____acomp_205_17_rename_at_205_98);
    }));
    _FuncbuiltinTickpushTick10769833213962245646_e919271fa2eab772(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_204_89.tokens),das_arg<das_source_formatter::Token>::pass((([&]() -> das_source_formatter::Token& {
        _temp_make_local_206_23_0 = Token_43fa8756d37aefe0(__context__);
        das_copy((_temp_make_local_206_23_0.str),(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_97),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
        das_copy((_temp_make_local_206_23_0.spaces),(__space_count_rename_at_204_92));
        das_copy((_temp_make_local_206_23_0.newLines),(__new_lines_rename_at_204_93));
        das_copy((_temp_make_local_206_23_0.lineInSource),(__src_line_rename_at_204_94));
        das_copy((_temp_make_local_206_23_0.column),(__column_rename_at_204_95));
        das_copy((_temp_make_local_206_23_0.tokenType),(__token_type_rename_at_204_96));
        return _temp_make_local_206_23_0;
    })())));
    on_token_added_ec8ba2110b19573e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_204_89));
    das_copy(__ctx_rename_at_204_89.spaceCounter,0);
    das_copy(__ctx_rename_at_204_89.newLineCounter,0);
}

inline void parse_token_321b59f95fe107c9 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_220_100 )
{
    skip_spaces_2fa97eb70a48fea7(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
    if ( __ctx_rename_at_220_100.eof )
    {
        _FuncbuiltinTickpushTick10769833213962245646_e919271fa2eab772(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_220_100.tokens),das_arg<das_source_formatter::Token>::pass(__ctx_rename_at_220_100.tokens(0,__context__)));
        return ;
    } else {
        int32_t __from_rename_at_227_101 = ((int32_t)__ctx_rename_at_220_100.pos);
        int32_t __fromColumn_rename_at_228_102 = ((int32_t)__ctx_rename_at_220_100.curColumn);
        if ( (is_alpha(__ctx_rename_at_220_100.c) || (__ctx_rename_at_220_100.c == 95)) || (__ctx_rename_at_220_100.c == 96) )
        {
            while ( !(__ctx_rename_at_220_100.eof) && (((is_alpha(__ctx_rename_at_220_100.c) || is_number(__ctx_rename_at_220_100.c)) || (__ctx_rename_at_220_100.c == 95)) || (__ctx_rename_at_220_100.c == 96)) )
            {
                next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
            };
            push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER);
            return ;
        } else {
            if ( is_number(__ctx_rename_at_220_100.c) || ((__ctx_rename_at_220_100.c == 46) && is_number(int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)))) )
            {
                while ( !(__ctx_rename_at_220_100.eof) && (((is_number(__ctx_rename_at_220_100.c) || is_alpha(__ctx_rename_at_220_100.c)) || (__ctx_rename_at_220_100.c == 46)) || (((__ctx_rename_at_220_100.c == 45) || (__ctx_rename_at_220_100.c == 43)) && ((int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos - 1),__context__)) == 101) || (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos - 1),__context__)) == 69)))) )
                {
                    next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                };
                push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::NUMBER);
                return ;
            } else {
                if ( (__ctx_rename_at_220_100.c == 34) || (__ctx_rename_at_220_100.c == 39) )
                {
                    int32_t __savedLine_rename_at_251_103 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                    int32_t __savedSpace_rename_at_252_104 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                    int32_t __openChar_rename_at_253_105 = ((int32_t)__ctx_rename_at_220_100.c);
                    int32_t __depth_rename_at_254_106 = 0;
                    next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
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
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                        };
                        next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                    };
                    if ( __ctx_rename_at_220_100.c == __openChar_rename_at_253_105 )
                    {
                        next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                    };
                    push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_252_104,__savedLine_rename_at_251_103,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::STRING);
                    return ;
                } else {
                    if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 47) )
                    {
                        int32_t __savedLine_rename_at_279_107 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                        int32_t __savedSpace_rename_at_280_108 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                        while ( (!(__ctx_rename_at_220_100.eof) && (__ctx_rename_at_220_100.c != 10)) && (__ctx_rename_at_220_100.c != 13) )
                        {
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                        };
                        push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_280_108,__savedLine_rename_at_279_107,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
                        return ;
                    } else {
                        if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 42) )
                        {
                            int32_t __savedLine_rename_at_290_109 = ((int32_t)__ctx_rename_at_220_100.newLineCounter);
                            int32_t __savedSpace_rename_at_291_110 = ((int32_t)__ctx_rename_at_220_100.spaceCounter);
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
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
                                        next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                    };
                                };
                                if ( (__ctx_rename_at_220_100.c == 47) && (int32_t(__ctx_rename_at_220_100.data((__ctx_rename_at_220_100.pos + 1),__context__)) == 42) )
                                {
                                    next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                    __depth_rename_at_294_111 += 1;
                                };
                                next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            };
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__savedSpace_rename_at_291_110,__savedLine_rename_at_290_109,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
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
                                                    next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                                                }
                                                ____iterator.close(__context__,(____rename_at_328_118));
                                            };
                                            push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                                            return ;
                                        };
                                    };
                                }
                                __tt_iterator.close(__context__,(__tt_rename_at_318_112));
                            };
                            next_char_a68ee7affe47d0eb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100));
                            push_token_cc86b5bb25bf7adc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_100),__from_rename_at_227_101,__ctx_rename_at_220_100.pos,__ctx_rename_at_220_100.spaceCounter,__ctx_rename_at_220_100.newLineCounter,__ctx_rename_at_220_100.srcLine,__fromColumn_rename_at_228_102,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                            return ;
                        };
                    };
                };
            };
        };
    };
}

inline void parse_all_tokens_29fbd1ed87913a0a ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_344_119 )
{
    initialize_token_templates_c0f484bb6b01366(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_119));
    das_source_formatter::Token __emptyToken_rename_at_346_120_ConstRef; das_zero(__emptyToken_rename_at_346_120_ConstRef); das_move(__emptyToken_rename_at_346_120_ConstRef, ((das_source_formatter::Token)(([&]() -> das_source_formatter::Token {
        das_source_formatter::Token __mks_346 = Token_43fa8756d37aefe0(__context__);
        return __mks_346;
    })())));
    das_source_formatter::Token const  & __emptyToken_rename_at_346_120 = __emptyToken_rename_at_346_120_ConstRef; ;
    builtin_array_clear(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickpushTick14133213201864676143_7d58ec4da1ec1b17(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__emptyToken_rename_at_346_120);
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
    if ( isUtf8Bom_d46057f250b5bf59(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_344_119.data)) )
    {
        __ctx_rename_at_344_119.pos += 3;
        das_copy(__ctx_rename_at_344_119.haveUtf8Bom,true);
    };
    das_copy(__ctx_rename_at_344_119.c,int32_t(__ctx_rename_at_344_119.data(__ctx_rename_at_344_119.pos,__context__)));
    das_copy(__ctx_rename_at_344_119.curLineIndex,__ctx_rename_at_344_119.pos);
    while ( !__ctx_rename_at_344_119.eof )
    {
        parse_token_321b59f95fe107c9(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_119));
    };
    {
        bool __need_loop_373 = true;
        // _: int const
        das_iterator<range> ____iterator(range(0,8));
        int32_t ____rename_at_373_121;
        __need_loop_373 = ____iterator.first(__context__,(____rename_at_373_121)) && __need_loop_373;
        for ( ; __need_loop_373 ; __need_loop_373 = ____iterator.next(__context__,(____rename_at_373_121)) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_7d58ec4da1ec1b17(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_119.tokens),__emptyToken_rename_at_346_120);
        }
        ____iterator.close(__context__,(____rename_at_373_121));
    };
}

inline void update_paren_3503759ee55e89b1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_388_122, int32_t __index_rename_at_388_123, das_source_formatter::ParenCounter &  __paren_counter_rename_at_388_124 )
{
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "<")) )
    {
        ++__paren_counter_rename_at_388_124.angle;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">")) )
    {
        --__paren_counter_rename_at_388_124.angle;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">>")) )
    {
        __paren_counter_rename_at_388_124.angle -= 2;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ">>>")) )
    {
        __paren_counter_rename_at_388_124.angle -= 3;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "(")) )
    {
        ++__paren_counter_rename_at_388_124.paren;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) ")")) )
    {
        --__paren_counter_rename_at_388_124.paren;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "[")) )
    {
        ++__paren_counter_rename_at_388_124.square;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "?[")) )
    {
        ++__paren_counter_rename_at_388_124.square;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "]")) )
    {
        --__paren_counter_rename_at_388_124.square;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "{")) )
    {
        ++__paren_counter_rename_at_388_124.curly;
    };
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_122),__index_rename_at_388_123,((char *) "}")) )
    {
        --__paren_counter_rename_at_388_124.curly;
    };
}

inline int32_t find_pair_paren_ff2776f9d2856b4e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_425_125, int32_t __from_index_rename_at_425_126 )
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
            update_paren_3503759ee55e89b1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_425_125),__j_rename_at_427_128,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_426_127));
            if ( __pc_rename_at_426_127.paren < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_427_128);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_427_128));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_425_126);
}

inline int32_t find_pair_square_89e4c86c49f03d61 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_437_129, int32_t __from_index_rename_at_437_130 )
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
            update_paren_3503759ee55e89b1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_437_129),__j_rename_at_439_132,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_438_131));
            if ( __pc_rename_at_438_131.square < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_439_132);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_439_132));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_437_130);
}

inline int32_t find_pair_angle_43f908d2d3e3a500 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_449_133, int32_t __from_index_rename_at_449_134 )
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
            update_paren_3503759ee55e89b1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_449_133),__j_rename_at_451_136,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_450_135));
            if ( __pc_rename_at_450_135.angle < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_451_136);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_451_136));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_449_134);
}

inline void mark_range_as_type_1c15599df80afc04 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_461_137, int32_t __from_rename_at_461_138, int32_t __to_rename_at_461_139 )
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

inline void mark_tokens_as_type_ff7631c34d0e20a8 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_470_141, int32_t __i_rename_at_470_142 )
{
    int32_t __pos_rename_at_471_143 = __i_rename_at_470_142;
    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "auto")) )
    {
        ++__pos_rename_at_471_143;
        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "(")) )
        {
            das_copy(__pos_rename_at_471_143,find_pair_paren_ff2776f9d2856b4e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143));
            ++__pos_rename_at_471_143;
        };
    } else {
        while ( __pos_rename_at_471_143 < builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_470_141.tokens)) )
        {
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143) )
            {
                break;
            } else {
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "::")) || (__ctx_rename_at_470_141.tokens(__pos_rename_at_471_143,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
                {
                    ++__pos_rename_at_471_143;
                } else {
                    break;
                };
            };
        };
        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "<")) )
        {
            das_copy(__pos_rename_at_471_143,find_pair_angle_43f908d2d3e3a500(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143));
            ++__pos_rename_at_471_143;
        };
    };
    if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143) )
    {
        mark_range_as_type_1c15599df80afc04(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__i_rename_at_470_142,__pos_rename_at_471_143);
        return ;
    } else {
        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "[")) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143 + 1,((char *) "]")) )
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
                if ( eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "-"),((char *) "==")) && eq_5ccd37f4ada4b6c8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144 + 1,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) )
                {
                    das_copy(__pos_rename_at_471_143,__j_rename_at_507_144 + 2);
                } else if ( eq_5ccd37f4ada4b6c8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__j_rename_at_507_144,((char *) "implicit")) )
                {
                    das_copy(__pos_rename_at_471_143,__j_rename_at_507_144 + 1);
                } else {
                    break;
                };
            }
            __j_iterator.close(__context__,(__j_rename_at_507_144));
        };
        mark_range_as_type_1c15599df80afc04(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__i_rename_at_470_142,__pos_rename_at_471_143);
        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143,((char *) "|")) )
        {
            das_copy(__ctx_rename_at_470_141.tokens(__pos_rename_at_471_143,__context__).spaces,1);
            ++__pos_rename_at_471_143;
            mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_141),__pos_rename_at_471_143);
        };
    };
}

inline void mark_tokens_as_type_inside_angle_5588509bf83aae77 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_528_145, int32_t __i_rename_at_528_146 )
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
            update_paren_3503759ee55e89b1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_528_145),__j_rename_at_530_148,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_529_147));
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

inline int32_t search_token_in_line_41b34300f8492a5f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_542_149, int32_t __from_index_rename_at_542_150, char * const  __str_rename_at_542_151 )
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
            update_paren_3503759ee55e89b1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_543_152));
            if ( (((__j_rename_at_544_153 > __from_index_rename_at_542_150) && new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153)) && (((__pc_rename_at_543_152.paren <= 0) || (__pc_rename_at_543_152.square <= 0)) || (__pc_rename_at_543_152.curly <= 0))) && !(eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153 - 1,((char *) ","))) )
            {
                break;
            } else {
                if ( ((__pc_rename_at_543_152.paren < 0) || (__pc_rename_at_543_152.square < 0)) || (__pc_rename_at_543_152.curly < 0) )
                {
                    break;
                } else {
                    if ( ((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,((char *) ";")) && (__pc_rename_at_543_152.paren == 0)) && (__pc_rename_at_543_152.square == 0)) && (__pc_rename_at_543_152.curly == 0) )
                    {
                        break;
                    } else {
                        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_149),__j_rename_at_544_153,__str_rename_at_542_151) )
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

inline void process_formatter_tokens_b7e8fa22d8896a75 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_564_154 )
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
                das_source_formatter::FormatterToken _temp_make_local_573_40_1; _temp_make_local_573_40_1;
                char * __command_rename_at_572_159 = ((char *)(char *)(((char * const )(builtin_string_chop(__str_rename_at_569_157,6,__strSize_rename_at_570_158 - 6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                _FuncbuiltinTickpushTick10769833213962245646_fc8141dc0153320a(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__ctx_rename_at_564_154.formatterTokens),das_arg<das_source_formatter::FormatterToken>::pass((([&]() -> das_source_formatter::FormatterToken& {
                    das_copy((_temp_make_local_573_40_1.tokenIndex),(__i_rename_at_567_155));
                    das_copy((_temp_make_local_573_40_1.command),(__command_rename_at_572_159));
                    return _temp_make_local_573_40_1;
                })())));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_567_155));
    };
}

inline bool have_formatter_token_68c38613f91cfa3f ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_581_160, char * const  __command_rename_at_581_161 )
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

inline void mark_token_context_7d9da6c542abc3d5 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_591_163 )
{
    TDim<char *,8> _temp_make_local_782_24_2; _temp_make_local_782_24_2;
    {
        bool __need_loop_593 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 1));
        int32_t __i_rename_at_593_164;
        __need_loop_593 = __i_iterator.first(__context__,(__i_rename_at_593_164)) && __need_loop_593;
        for ( ; __need_loop_593 ; __need_loop_593 = __i_iterator.next(__context__,(__i_rename_at_593_164)) )
        {
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164) && (((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "require")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "expect"))) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "label"))) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_593_164,((char *) "include"))) )
            {
                {
                    bool __need_loop_595 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_593_164 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_595_165;
                    __need_loop_595 = __j_iterator.first(__context__,(__j_rename_at_595_165)) && __need_loop_595;
                    for ( ; __need_loop_595 ; __need_loop_595 = __j_iterator.next(__context__,(__j_rename_at_595_165)) )
                    {
                        if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_595_165) )
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
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_605_166,((char *) "%")) && (__ctx_rename_at_591_163.tokens((__i_rename_at_605_166 + 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_605_166 + 2,((char *) "~")) )
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
                        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_607_167,((char *) "%%")) )
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
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168) && (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "let")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "var"))) )
            {
                int32_t __assignPos0_rename_at_619_169 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "=")));
                int32_t __assignPos1_rename_at_620_170 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) ":=")));
                int32_t __assignPos2_rename_at_621_171 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) "<-")));
                int32_t __colonPos_rename_at_622_172 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_617_168,((char *) ":")));
                if ( (((__colonPos_rename_at_622_172 > 0) && ((__assignPos0_rename_at_619_169 > __colonPos_rename_at_622_172) || (__assignPos0_rename_at_619_169 <= 0))) && ((__assignPos1_rename_at_620_170 > __colonPos_rename_at_622_172) || (__assignPos1_rename_at_620_170 <= 0))) && ((__assignPos2_rename_at_621_171 > __colonPos_rename_at_622_172) || (__assignPos2_rename_at_621_171 <= 0)) )
                {
                    mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_622_172 + 1);
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
                if ( (_FuncbuiltinTickfind_indexTick6703785223819426183_7889f698fd41c370(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_591_163.tokens(__i_rename_at_633_173,__context__).str) >= 0) || ((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173,((char *) "new")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173,((char *) "struct"))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173 + 1,((char *) "<"))) )
                {
                    mark_tokens_as_type_inside_angle_5588509bf83aae77(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_633_173 + 1);
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
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174,((char *) "$")) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 1,((char *) "t"))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 2,((char *) "(")) )
            {
                int32_t __endParams_rename_at_646_175 = ((int32_t)find_pair_paren_ff2776f9d2856b4e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_644_174 + 2));
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
                        if ( !(eq_5ccd37f4ada4b6c8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177,((char *) "-"),((char *) "#"),((char *) "&"),((char *) "?")) || eq_cecc7c5a47abfe6c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177,((char *) "=="),((char *) "const"),((char *) "implicit"))) )
                        {
                            break;
                        } else {
                            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_650_177) )
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
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178,((char *) "[")) && eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 1,((char *) "["),((char *) "{"))) || (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178,((char *) "{")) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 1,((char *) "{"))) )
            {
                mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_663_178 + 2);
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
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179) )
            {
                int32_t __skip_rename_at_672_180 = 0;
                if ( ((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "struct")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "class"))) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "variant"))) || (((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "var"))) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "let"))) && new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1)) )
                {
                    das_copy(__skip_rename_at_672_180,1);
                };
                if ( (((eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "var"))) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "let"))) && (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1,((char *) "private")) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 1,((char *) "public")))) && new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179 + 2) )
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
                            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182) )
                            {
                                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182,((char *) "def")) )
                                {
                                    break;
                                } else {
                                    if ( __ctx_rename_at_591_163.tokens(__j_rename_at_688_182,__context__).spaces <= __ident_rename_at_687_181 )
                                    {
                                        break;
                                    } else {
                                        int32_t __colonPos_rename_at_696_183 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_688_182,eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_670_179,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                                        if ( __colonPos_rename_at_696_183 > 0 )
                                        {
                                            mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_696_183 + 1);
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
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184) && (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184,((char *) "typedef")) && !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184 + 1))) )
            {
                {
                    bool __need_loop_708 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_706_184 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 3));
                    int32_t __j_rename_at_708_185;
                    __need_loop_708 = __j_iterator.first(__context__,(__j_rename_at_708_185)) && __need_loop_708;
                    for ( ; __need_loop_708 ; __need_loop_708 = __j_iterator.next(__context__,(__j_rename_at_708_185)) )
                    {
                        if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_708_185) )
                        {
                            break;
                        } else {
                            int32_t __colonPos_rename_at_712_186 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_708_185,eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_706_184,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                            if ( __colonPos_rename_at_712_186 > 0 )
                            {
                                mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_712_186 + 1);
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
            if ( new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187,((char *) "def")) )
            {
                das_copy(__pos_rename_at_722_188,__i_rename_at_721_187 + 1);
            } else if ( eq_cecc7c5a47abfe6c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187,((char *) "$"),((char *) "@"),((char *) "@@")) && eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187 + 1,((char *) "("),((char *) "[")) )
            {
                das_copy(__pos_rename_at_722_188,__i_rename_at_721_187);
            };
            if ( __pos_rename_at_722_188 == -1 )
            {
                continue;
            } else {
                while ( eq_5ccd37f4ada4b6c8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "private"),((char *) "public"),((char *) "const"),((char *) "implicit")) || eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "abstract"),((char *) "override")) )
                {
                    ++__pos_rename_at_722_188;
                };
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "operator")) )
                {
                    das_copy(__pos_rename_at_722_188,search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188,((char *) "(")) - 1);
                };
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) "[")) )
                {
                    das_copy(__pos_rename_at_722_188,find_pair_square_89e4c86c49f03d61(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1));
                };
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) ":")) )
                {
                    mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_721_187 + 2);
                } else if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1,((char *) "(")) )
                {
                    int32_t __beginParams_rename_at_748_189 = ((int32_t)(__pos_rename_at_722_188 + 1));
                    das_copy(__ctx_rename_at_591_163.tokens(__beginParams_rename_at_748_189,__context__).dontAddSpacesAround,true);
                    int32_t __endParams_rename_at_750_190 = ((int32_t)find_pair_paren_ff2776f9d2856b4e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__pos_rename_at_722_188 + 1));
                    {
                        bool __need_loop_751 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__beginParams_rename_at_748_189,__endParams_rename_at_750_190 + 1));
                        int32_t __j_rename_at_751_191;
                        __need_loop_751 = __j_iterator.first(__context__,(__j_rename_at_751_191)) && __need_loop_751;
                        for ( ; __need_loop_751 ; __need_loop_751 = __j_iterator.next(__context__,(__j_rename_at_751_191)) )
                        {
                            das_copy(__ctx_rename_at_591_163.tokens(__j_rename_at_751_191,__context__).isInFunctionParam,true);
                            if ( ((__j_rename_at_751_191 - 1) == __beginParams_rename_at_748_189) || eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_751_191 - 1,((char *) ";")) )
                            {
                                int32_t __colonPos_rename_at_754_192 = ((int32_t)search_token_in_line_41b34300f8492a5f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_751_191,((char *) ":")));
                                if ( __colonPos_rename_at_754_192 > 0 )
                                {
                                    mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__colonPos_rename_at_754_192 + 1);
                                };
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_751_191));
                    };
                    if ( __endParams_rename_at_750_190 < (builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens)) - 1) )
                    {
                        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__endParams_rename_at_750_190 + 1,((char *) ":")) )
                        {
                            mark_tokens_as_type_ff7631c34d0e20a8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__endParams_rename_at_750_190 + 2);
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
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193,((char *) "[")) && !(eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193 + 1,((char *) "[")))) && !(eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_769_193 + 1,((char *) "{"))) )
            {
                {
                    bool __need_loop_771 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_769_193 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_163.tokens))));
                    int32_t __j_rename_at_771_194;
                    __need_loop_771 = __j_iterator.first(__context__,(__j_rename_at_771_194)) && __need_loop_771;
                    for ( ; __need_loop_771 ; __need_loop_771 = __j_iterator.next(__context__,(__j_rename_at_771_194)) )
                    {
                        if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_771_194,((char *) "]")) )
                        {
                            break;
                        } else {
                            if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__j_rename_at_771_194,((char *) "=")) )
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
    TTable<char *,void> __macroEscapes_rename_at_782_195_ConstRef; das_zero(__macroEscapes_rename_at_782_195_ConstRef); das_move(__macroEscapes_rename_at_782_195_ConstRef, ((TTable<char *,void>)_FuncbuiltinTickto_table_moveTick3386430563931768014_c018a9675934bba9(__context__,das_arg<TDim<char *,8>>::pass((([&]() -> TDim<char *,8>& {
        _temp_make_local_782_24_2(0,__context__) = ((char *) "i");
        _temp_make_local_782_24_2(1,__context__) = ((char *) "v");
        _temp_make_local_782_24_2(2,__context__) = ((char *) "e");
        _temp_make_local_782_24_2(3,__context__) = ((char *) "c");
        _temp_make_local_782_24_2(4,__context__) = ((char *) "t");
        _temp_make_local_782_24_2(5,__context__) = ((char *) "f");
        _temp_make_local_782_24_2(6,__context__) = ((char *) "b");
        _temp_make_local_782_24_2(7,__context__) = ((char *) "a");
        return _temp_make_local_782_24_2;
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
            if ( (!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_783_196)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_163),__i_rename_at_783_196 - 1,((char *) "$"))) && _FuncbuiltinTickkey_existsTick16808803843923989214_5f8f761d5f23964f(__context__,__macroEscapes_rename_at_782_195,__ctx_rename_at_591_163.tokens(__i_rename_at_783_196,__context__).str) )
            {
                das_copy(__ctx_rename_at_591_163.tokens((__i_rename_at_783_196 - 1),__context__).dontAddSpacesAround,true);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_783_196));
    };
}

inline void fmt_function_arguments_393a0d20cb9876da ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_791_197 )
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
                    if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_791_197),__i_rename_at_794_199,((char *) "(")) )
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

inline void fmt_remove_space_before_args_b613d1f002f609af ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_832_203 )
{
    {
        bool __need_loop_833 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_832_203.tokens)) - 1));
        int32_t __i_rename_at_833_204;
        __need_loop_833 = __i_iterator.first(__context__,(__i_rename_at_833_204)) && __need_loop_833;
        for ( ; __need_loop_833 ; __need_loop_833 = __i_iterator.next(__context__,(__i_rename_at_833_204)) )
        {
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_203),__i_rename_at_833_204,((char *) "(")) && !(__ctx_rename_at_832_203.tokens(__i_rename_at_833_204,__context__).isInType)) && __ctx_rename_at_832_203.tokens((__i_rename_at_833_204 - 1),__context__).isInType )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_203),__i_rename_at_833_204);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_833_204));
    };
}

inline void debug_print_tokens_d5c3886aafc47666 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_841_205 )
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

inline char * generate_source_d9e23c7a7a794181 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_848_207 )
{
    TArray<uint8_t> __res_rename_at_849_208; memset((void*)&__res_rename_at_849_208,0,sizeof(__res_rename_at_849_208));
    bool __useCrLf_rename_at_857_209; memset((void*)&__useCrLf_rename_at_857_209,0,sizeof(__useCrLf_rename_at_857_209));
    int32_t __column_rename_at_858_210; memset((void*)&__column_rename_at_858_210,0,sizeof(__column_rename_at_858_210));
    int32_t __spaces_rename_at_869_213; memset((void*)&__spaces_rename_at_869_213,0,sizeof(__spaces_rename_at_869_213));
    /* finally */ auto __finally_848= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208));
    /* end finally */ });
    das_zero(__res_rename_at_849_208);
    _FuncbuiltinTickreserveTick3994685146752941225_adfb084cc10e1733(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),(builtin_array_size(__ctx_rename_at_848_207.tokens) * 8) + 1000);
    if ( __ctx_rename_at_848_207.haveUtf8Bom )
    {
        _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xef);
        _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xbb);
        _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xbf);
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
                        _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xd);
                    };
                    _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0xa);
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
                    _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),0x20);
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
                    _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),uint8_t(__c_rename_at_879_215));
                    ++__column_rename_at_858_210;
                }
                __c_iterator.close(__context__,(__c_rename_at_879_215));
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_860_211));
    };
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_208),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool need_spaces_around_c85752147cfda42d ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_891_216, int32_t __index_rename_at_891_217 )
{
    if ( ((__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).dontFormat) || __ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).dontAddSpacesAround )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( _FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_891_216.tokens(__index_rename_at_891_217,__context__).str) >= 0 )
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

inline bool need_space_only_before_a3979a2c621820ef ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_912_218, int32_t __index_rename_at_912_219 )
{
    if ( !__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).isInType )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( ((__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).dontFormat) || __ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).dontAddSpacesAround )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_912_218.tokens(__index_rename_at_912_219,__context__).str) >= 0 )
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

inline bool is_unary_b0c7219745029c11 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_956_220, int32_t __index_rename_at_956_221 )
{
    if ( eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "!")) || eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "~")) )
    {
        return das_auto_cast<bool>::cast(true);
    } else {
        if ( (eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "*")) && (__index_rename_at_956_221 > 0)) && __ctx_rename_at_956_220.tokens((__index_rename_at_956_221 - 1),__context__).isInType )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            if ( (eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "+")) || eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "-"))) || eq_5251ca869c24b49f(__context__,__ctx_rename_at_956_220,__index_rename_at_956_221,((char *) "*")) )
            {
                int32_t __i_rename_at_966_222 = __index_rename_at_956_221;
                while ( (((__i_rename_at_966_222 - 1) >= 0) && (__ctx_rename_at_956_220.tokens((__i_rename_at_966_222 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT)) && !(new_line_before_74a9e3638b00c26f(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222)) )
                {
                    --__i_rename_at_966_222;
                };
                return das_auto_cast<bool>::cast(((_FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9(__context__,das_global<TDim<char *,21>,0xd52f4fdee217c055>(__context__) /*tokens_before_unary*/,__ctx_rename_at_956_220.tokens((__i_rename_at_966_222 - 1),__context__).str) >= 0) || new_line_before_74a9e3638b00c26f(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222)) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast(need_spaces_around_c85752147cfda42d(__context__,__ctx_rename_at_956_220,__i_rename_at_966_222 - 1)));
            } else {
                return das_auto_cast<bool>::cast(false);
            };
        };
    };
}

inline void remove_spaces_around_203ede7b7eebc7c5 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_979_223, int32_t __index_rename_at_979_224 )
{
    if ( !new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224);
    };
    if ( !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224 + 1)) && (__ctx_rename_at_979_223.tokens((__index_rename_at_979_224 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_223),__index_rename_at_979_224 + 1);
    };
}

inline void make_spaces_around_80d61c2838123a7d ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_989_225, int32_t __index_rename_at_989_226 )
{
    if ( !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226)) && (__ctx_rename_at_989_225.tokens(__index_rename_at_989_226,__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226);
    };
    if ( !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226 + 1)) && (__ctx_rename_at_989_225.tokens((__index_rename_at_989_226 + 1),__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_225),__index_rename_at_989_226 + 1);
    };
}

inline void fmt_space_after_comma_f91c855ec419635f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_999_227 )
{
    {
        bool __need_loop_1000 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_999_227.tokens))));
        int32_t __i_rename_at_1000_228;
        __need_loop_1000 = __i_iterator.first(__context__,(__i_rename_at_1000_228)) && __need_loop_1000;
        for ( ; __need_loop_1000 ; __need_loop_1000 = __i_iterator.next(__context__,(__i_rename_at_1000_228)) )
        {
            if ( (!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228 - 1,((char *) ","))) && (__ctx_rename_at_999_227.tokens(__i_rename_at_1000_228,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_227),__i_rename_at_1000_228);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1000_228));
    };
}

inline void fmt_space_after_semicolon_23f19dedec62b617 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1008_229 )
{
    {
        bool __need_loop_1009 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1008_229.tokens))));
        int32_t __i_rename_at_1009_230;
        __need_loop_1009 = __i_iterator.first(__context__,(__i_rename_at_1009_230)) && __need_loop_1009;
        for ( ; __need_loop_1009 ; __need_loop_1009 = __i_iterator.next(__context__,(__i_rename_at_1009_230)) )
        {
            if ( ((!(eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230,nullptr)) && !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230 - 1,((char *) ";"))) && (__ctx_rename_at_1008_229.tokens(__i_rename_at_1009_230,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_229),__i_rename_at_1009_230);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1009_230));
    };
}

inline void fmt_remove_space_before_comma_e4ab43db37a70c74 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1017_231 )
{
    {
        bool __need_loop_1018 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1017_231.tokens))));
        int32_t __i_rename_at_1018_232;
        __need_loop_1018 = __i_iterator.first(__context__,(__i_rename_at_1018_232)) && __need_loop_1018;
        for ( ; __need_loop_1018 ; __need_loop_1018 = __i_iterator.next(__context__,(__i_rename_at_1018_232)) )
        {
            if ( !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232,((char *) ",")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_231),__i_rename_at_1018_232);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1018_232));
    };
}

inline void fmt_remove_space_before_semicolon_e5d8826c5665e6be ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1026_233 )
{
    {
        bool __need_loop_1027 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1026_233.tokens))));
        int32_t __i_rename_at_1027_234;
        __need_loop_1027 = __i_iterator.first(__context__,(__i_rename_at_1027_234)) && __need_loop_1027;
        for ( ; __need_loop_1027 ; __need_loop_1027 = __i_iterator.next(__context__,(__i_rename_at_1027_234)) )
        {
            if ( !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234,((char *) ";")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_233),__i_rename_at_1027_234);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1027_234));
    };
}

inline void fmt_type_colon_400b4390d7623d95 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1035_235 )
{
    {
        bool __need_loop_1036 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1035_235.tokens)) - 1));
        int32_t __i_rename_at_1036_236;
        __need_loop_1036 = __i_iterator.first(__context__,(__i_rename_at_1036_236)) && __need_loop_1036;
        for ( ; __need_loop_1036 ; __need_loop_1036 = __i_iterator.next(__context__,(__i_rename_at_1036_236)) )
        {
            if ( __ctx_rename_at_1035_235.tokens(__i_rename_at_1036_236,__context__).isInType && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236,((char *) ":")) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236);
                if ( !new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_235),__i_rename_at_1036_236 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1036_236));
    };
}

inline void fmt_function_decl_param_paren_dac5114d61e417ed ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1047_237 )
{
    {
        bool __need_loop_1048 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1047_237.tokens)) - 1));
        int32_t __i_rename_at_1048_238;
        __need_loop_1048 = __i_iterator.first(__context__,(__i_rename_at_1048_238)) && __need_loop_1048;
        for ( ; __need_loop_1048 ; __need_loop_1048 = __i_iterator.next(__context__,(__i_rename_at_1048_238)) )
        {
            if ( (__ctx_rename_at_1047_237.tokens(__i_rename_at_1048_238,__context__).isInFunctionParam && !(__ctx_rename_at_1047_237.tokens((__i_rename_at_1048_238 - 1),__context__).isInFunctionParam)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238,((char *) "(")) )
            {
                remove_spaces_around_203ede7b7eebc7c5(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238 - 1,((char *) "]")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
                };
            };
            if ( (__ctx_rename_at_1047_237.tokens(__i_rename_at_1048_238,__context__).isInFunctionParam && !(__ctx_rename_at_1047_237.tokens((__i_rename_at_1048_238 + 1),__context__).isInFunctionParam)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_237),__i_rename_at_1048_238);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1048_238));
    };
}

inline void fmt_function_call_param_paren_call_347b6df2d6483625 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1062_239 )
{
    {
        bool __need_loop_1063 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1062_239.tokens)) - 1));
        int32_t __i_rename_at_1063_240;
        __need_loop_1063 = __i_iterator.first(__context__,(__i_rename_at_1063_240)) && __need_loop_1063;
        for ( ; __need_loop_1063 ; __need_loop_1063 = __i_iterator.next(__context__,(__i_rename_at_1063_240)) )
        {
            if ( (eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240,((char *) "(")) && !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240))) && (__ctx_rename_at_1062_239.tokens((__i_rename_at_1063_240 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                remove_spaces_around_203ede7b7eebc7c5(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240);
                int32_t __closingParen_rename_at_1066_241 = ((int32_t)find_pair_paren_ff2776f9d2856b4e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__i_rename_at_1063_240));
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_239),__closingParen_rename_at_1066_241);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1063_240));
    };
}

inline void fmt_spaces_around_operators_53d08ef306f852f9 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1073_242 )
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
                if ( is_unary_b0c7219745029c11(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243 + 1);
                };
                if ( need_space_only_before_a3979a2c621820ef(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(is_unary_b0c7219745029c11(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243);
                } else if ( need_spaces_around_c85752147cfda42d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243) && !(is_unary_b0c7219745029c11(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243)) )
                {
                    make_spaces_around_80d61c2838123a7d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_242),__i_rename_at_1074_243);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1074_243));
    };
}

inline void fmt_space_after_keyword_2536c1e9542cdc3b ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1092_244 )
{
    {
        bool __need_loop_1093 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1092_244.tokens)) - 1));
        int32_t __i_rename_at_1093_245;
        __need_loop_1093 = __i_iterator.first(__context__,(__i_rename_at_1093_245)) && __need_loop_1093;
        for ( ; __need_loop_1093 ; __need_loop_1093 = __i_iterator.next(__context__,(__i_rename_at_1093_245)) )
        {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_feb73043e4854e1f(__context__,das_global<TDim<char *,11>,0x94e2878ce5158877>(__context__) /*space_after_keywords*/,__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).str) >= 0 )
            {
                if ( !new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1);
                };
            };
            if ( ((__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).spaces >= 2) && (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).str),cast<char *>::from(((char *) "match")),*__context__,nullptr))) && ((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_1092_244.tokens((__i_rename_at_1093_245 + 1),__context__).str),cast<char *>::from(((char *) "(")),*__context__,nullptr)) && !(__ctx_rename_at_1092_244.tokens((__i_rename_at_1093_245 + 1),__context__).isInFunctionParam)) )
            {
                if ( !new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1);
                };
            };
            if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_1092_244.tokens(__i_rename_at_1093_245,__context__).str),cast<char *>::from(((char *) "return")),*__context__,nullptr)) && (builtin_string_length(__ctx_rename_at_1092_244.tokens((__i_rename_at_1093_245 + 1),__context__).str,__context__) > 0) )
            {
                if ( !new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_244),__i_rename_at_1093_245 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1093_245));
    };
}

inline void fmt_space_after_paren_82d46f74b7d5652 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1114_246 )
{
    {
        bool __need_loop_1115 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1114_246.tokens)) - 1));
        int32_t __i_rename_at_1115_247;
        __need_loop_1115 = __i_iterator.first(__context__,(__i_rename_at_1115_247)) && __need_loop_1115;
        for ( ; __need_loop_1115 ; __need_loop_1115 = __i_iterator.next(__context__,(__i_rename_at_1115_247)) )
        {
            if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1114_246),__i_rename_at_1115_247,((char *) ")")) && (__ctx_rename_at_1114_246.tokens((__i_rename_at_1115_247 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1114_246),__i_rename_at_1115_247 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1115_247));
    };
}

inline void fmt_space_after_square_brackets_fdbea403935e8e12 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1123_248 )
{
    {
        bool __need_loop_1124 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1123_248.tokens)) - 1));
        int32_t __i_rename_at_1124_249;
        __need_loop_1124 = __i_iterator.first(__context__,(__i_rename_at_1124_249)) && __need_loop_1124;
        for ( ; __need_loop_1124 ; __need_loop_1124 = __i_iterator.next(__context__,(__i_rename_at_1124_249)) )
        {
            if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1123_248),__i_rename_at_1124_249,((char *) "]")) && (__ctx_rename_at_1123_248.tokens((__i_rename_at_1124_249 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1123_248),__i_rename_at_1124_249 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1124_249));
    };
}

inline void fmt_remove_space_inside_parens_cb72085f2dcb9ecd ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1132_250 )
{
    {
        bool __need_loop_1133 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1132_250.tokens)) - 1));
        int32_t __i_rename_at_1133_251;
        __need_loop_1133 = __i_iterator.first(__context__,(__i_rename_at_1133_251)) && __need_loop_1133;
        for ( ; __need_loop_1133 ; __need_loop_1133 = __i_iterator.next(__context__,(__i_rename_at_1133_251)) )
        {
            if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1132_250),__i_rename_at_1133_251,((char *) "(")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1132_250),__i_rename_at_1133_251 + 1);
            };
            if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1132_250),__i_rename_at_1133_251,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1132_250),__i_rename_at_1133_251);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1133_251));
    };
}

inline void fmt_remove_space_before_as_865a2960d4580501 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1144_252 )
{
    {
        bool __need_loop_1145 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1144_252.tokens))));
        int32_t __i_rename_at_1145_253;
        __need_loop_1145 = __i_iterator.first(__context__,(__i_rename_at_1145_253)) && __need_loop_1145;
        for ( ; __need_loop_1145 ; __need_loop_1145 = __i_iterator.next(__context__,(__i_rename_at_1145_253)) )
        {
            if ( (!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1144_252),__i_rename_at_1145_253)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1144_252),__i_rename_at_1145_253,((char *) "as"))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1144_252),__i_rename_at_1145_253 - 1,((char *) "?")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1144_252),__i_rename_at_1145_253);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1145_253));
    };
}

inline void fmt_remove_space_before_end_of_object_510336d353cc43f1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1153_254 )
{
    {
        bool __need_loop_1155 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1153_254.tokens)) - 2));
        int32_t __i_rename_at_1155_255;
        __need_loop_1155 = __i_iterator.first(__context__,(__i_rename_at_1155_255)) && __need_loop_1155;
        for ( ; __need_loop_1155 ; __need_loop_1155 = __i_iterator.next(__context__,(__i_rename_at_1155_255)) )
        {
            if ( ((!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1153_254),__i_rename_at_1155_255)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1153_254),__i_rename_at_1155_255,((char *) "]"))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1153_254),__i_rename_at_1155_255 + 1,((char *) "]"))) && !(eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1153_254),__i_rename_at_1155_255 - 1,((char *) "]"))) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1153_254),__i_rename_at_1155_255);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1155_255));
    };
}

inline void fmt_remove_space_before_object_type_fff6f6cd3369398c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1163_256 )
{
    {
        bool __need_loop_1165 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1163_256.tokens)) - 1));
        int32_t __i_rename_at_1165_257;
        __need_loop_1165 = __i_iterator.first(__context__,(__i_rename_at_1165_257)) && __need_loop_1165;
        for ( ; __need_loop_1165 ; __need_loop_1165 = __i_iterator.next(__context__,(__i_rename_at_1165_257)) )
        {
            if ( ((!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1163_256),__i_rename_at_1165_257)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1163_256),__i_rename_at_1165_257 - 2,((char *) "["))) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1163_256),__i_rename_at_1165_257 - 1,((char *) "["))) && (__ctx_rename_at_1163_256.tokens(__i_rename_at_1165_257,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1163_256),__i_rename_at_1165_257);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1165_257));
    };
}

inline void fmt_remove_space_before_array_of_type_fa116a3241d9e90c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1173_258 )
{
    {
        bool __need_loop_1175 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1173_258.tokens)) - 2));
        int32_t __i_rename_at_1175_259;
        __need_loop_1175 = __i_iterator.first(__context__,(__i_rename_at_1175_259)) && __need_loop_1175;
        for ( ; __need_loop_1175 ; __need_loop_1175 = __i_iterator.next(__context__,(__i_rename_at_1175_259)) )
        {
            if ( ((!(new_line_before_74a9e3638b00c26f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1173_258),__i_rename_at_1175_259)) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1173_258),__i_rename_at_1175_259 - 1,((char *) ">"))) && __ctx_rename_at_1173_258.tokens((__i_rename_at_1175_259 - 1),__context__).isInType) && eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1173_258),__i_rename_at_1175_259,((char *) "[")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1173_258),__i_rename_at_1175_259);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1175_259));
    };
}

inline void fmt_glue_type_specifiers_538bf5888d1be714 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1185_260 )
{
    {
        bool __need_loop_1186 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1185_260.tokens))));
        int32_t __i_rename_at_1186_261;
        __need_loop_1186 = __i_iterator.first(__context__,(__i_rename_at_1186_261)) && __need_loop_1186;
        for ( ; __need_loop_1186 ; __need_loop_1186 = __i_iterator.next(__context__,(__i_rename_at_1186_261)) )
        {
            if ( __ctx_rename_at_1185_260.tokens(__i_rename_at_1186_261,__context__).isInType )
            {
                if ( (__ctx_rename_at_1185_260.tokens(__i_rename_at_1186_261,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) && (_FuncbuiltinTickfind_indexTick6703785223819426183_a4c49c41e04684c4(__context__,das_global<TDim<char *,3>,0xca374635aa15cd66>(__context__) /*type_specifiers*/,__ctx_rename_at_1185_260.tokens(__i_rename_at_1186_261,__context__).str) >= 0) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261);
                };
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261,((char *) "const")) && eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261 - 1,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261);
                };
                if ( eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261);
                };
                if ( eq_5251ca869c24b49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261 - 1,((char *) "|")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1185_260),__i_rename_at_1186_261);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1186_261));
    };
}

inline void fmt_space_after_cast_type_5d3d2f4d38f83d01 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1205_262 )
{
    {
        bool __need_loop_1206 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1205_262.tokens)) - 3));
        int32_t __i_rename_at_1206_263;
        __need_loop_1206 = __i_iterator.first(__context__,(__i_rename_at_1206_263)) && __need_loop_1206;
        for ( ; __need_loop_1206 ; __need_loop_1206 = __i_iterator.next(__context__,(__i_rename_at_1206_263)) )
        {
            if ( __ctx_rename_at_1205_262.tokens(__i_rename_at_1206_263,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER )
            {
                if ( _FuncbuiltinTickfind_indexTick6703785223819426183_7889f698fd41c370(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_1205_262.tokens(__i_rename_at_1206_263,__context__).str) >= 0 )
                {
                    {
                        bool __need_loop_1209 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__i_rename_at_1206_263 + 2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1205_262.tokens))));
                        int32_t __j_rename_at_1209_264;
                        __need_loop_1209 = __j_iterator.first(__context__,(__j_rename_at_1209_264)) && __need_loop_1209;
                        for ( ; __need_loop_1209 ; __need_loop_1209 = __j_iterator.next(__context__,(__j_rename_at_1209_264)) )
                        {
                            if ( !(__ctx_rename_at_1205_262.tokens(__j_rename_at_1209_264,__context__).isInType) && (__ctx_rename_at_1205_262.tokens(__j_rename_at_1209_264,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
                            {
                                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1205_262),__j_rename_at_1209_264);
                                break;
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_1209_264));
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1206_263));
    };
}

inline void fmt_destructure_space_2fcf524fd08df174 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1220_265 )
{
    {
        bool __need_loop_1221 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1220_265.tokens)) - 1));
        int32_t __i_rename_at_1221_266;
        __need_loop_1221 = __i_iterator.first(__context__,(__i_rename_at_1221_266)) && __need_loop_1221;
        for ( ; __need_loop_1221 ; __need_loop_1221 = __i_iterator.next(__context__,(__i_rename_at_1221_266)) )
        {
            if ( eq_3d631d49cf7e1d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1220_265),__i_rename_at_1221_266,((char *) "let"),((char *) "var")) && (__ctx_rename_at_1220_265.tokens((__i_rename_at_1221_266 + 1),__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1220_265),__i_rename_at_1221_266 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1221_266));
    };
}

inline void do_format_ff67931097f9f551 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1229_267 )
{
    parse_all_tokens_29fbd1ed87913a0a(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
    process_formatter_tokens_b7e8fa22d8896a75(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
    if ( have_formatter_token_68c38613f91cfa3f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267),((char *) "ignore-file")) )
    {
        return ;
    } else {
        mark_token_context_7d9da6c542abc3d5(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_spaces_around_operators_53d08ef306f852f9(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_function_call_param_paren_call_347b6df2d6483625(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_comma_f91c855ec419635f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_semicolon_23f19dedec62b617(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_function_decl_param_paren_dac5114d61e417ed(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_type_colon_400b4390d7623d95(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_cast_type_5d3d2f4d38f83d01(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_comma_e4ab43db37a70c74(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_semicolon_e5d8826c5665e6be(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_inside_parens_cb72085f2dcb9ecd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_as_865a2960d4580501(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_glue_type_specifiers_538bf5888d1be714(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_keyword_2536c1e9542cdc3b(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_paren_82d46f74b7d5652(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_space_after_square_brackets_fdbea403935e8e12(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_function_arguments_393a0d20cb9876da(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_args_b613d1f002f609af(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_end_of_object_510336d353cc43f1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_object_type_fff6f6cd3369398c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_remove_space_before_array_of_type_fa116a3241d9e90c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        fmt_destructure_space_2fcf524fd08df174(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1229_267));
        int32_t __top_rename_at_1261_268 = ((int32_t)(builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1229_267.tokens)) - 1));
        {
            bool __need_loop_1262 = true;
            // it: int const
            das_iterator<range> __it_iterator(range(0,16));
            int32_t __it_rename_at_1262_269;
            __need_loop_1262 = __it_iterator.first(__context__,(__it_rename_at_1262_269)) && __need_loop_1262;
            for ( ; __need_loop_1262 ; __need_loop_1262 = __it_iterator.next(__context__,(__it_rename_at_1262_269)) )
            {
                int32_t __i_rename_at_1263_270 = ((int32_t)(__top_rename_at_1261_268 - __it_rename_at_1262_269));
                if ( (__i_rename_at_1263_270 < 0) || !(builtin_empty(__ctx_rename_at_1229_267.tokens(__i_rename_at_1263_270,__context__).str)) )
                {
                    break;
                } else {
                    das_copy(__ctx_rename_at_1229_267.tokens(__i_rename_at_1263_270,__context__).spaces,0);
                };
            }
            __it_iterator.close(__context__,(__it_rename_at_1262_269));
        };
    };
}

inline char * format_source_string_53ea2afc74eb7140 ( Context * __context__, char * const  & __file_data_rename_at_1272_271 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1273_272; memset((void*)&__ctx_rename_at_1273_272,0,sizeof(__ctx_rename_at_1273_272));
    /* finally */ auto __finally_1272= das_finally([&](){
    finalize_db456bcf8e957ca4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1273_272));
    /* end finally */ });
    das_zero(__ctx_rename_at_1273_272);
    _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_7926e574de26b25d(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1273_272.data),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
        TArray<uint8_t> ____acomp_1274_18_rename_at_1274_273;das_zero(____acomp_1274_18_rename_at_1274_273);
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1274_18_rename_at_1274_273),false);
        {
            bool __need_loop_1274 = true;
            // c: int const
            das_iterator<char * const > __c_iterator(__file_data_rename_at_1272_271);
            int32_t __c_rename_at_1274_274;
            __need_loop_1274 = __c_iterator.first(__context__,(__c_rename_at_1274_274)) && __need_loop_1274;
            for ( ; __need_loop_1274 ; __need_loop_1274 = __c_iterator.next(__context__,(__c_rename_at_1274_274)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1274_18_rename_at_1274_273),uint8_t(__c_rename_at_1274_274));
            }
            __c_iterator.close(__context__,(__c_rename_at_1274_274));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1274_18_rename_at_1274_273),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab8b80dafe942dee(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1274_18_rename_at_1274_273)));
    })));
    _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1273_272.data),0x0);
    do_format_ff67931097f9f551(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1273_272));
    return das_auto_cast<char *>::cast(generate_source_d9e23c7a7a794181(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1273_272)));
}

inline char * format_source_555ab24d2bad5d9a ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1281_275, TArray<uint8_t> const  &  __file_data_rename_at_1281_276 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_11c19903beee414c(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1281_275.data),__file_data_rename_at_1281_276);
    _FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1281_275.data),0x0);
    do_format_ff67931097f9f551(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1281_275));
    return das_auto_cast<char *>::cast(generate_source_d9e23c7a7a794181(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1281_275)));
}

inline char * format_source_82aae480574c279c ( Context * __context__, TArray<uint8_t> const  &  __file_data_rename_at_1289_277 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1290_278; memset((void*)&__ctx_rename_at_1290_278,0,sizeof(__ctx_rename_at_1290_278));
    /* finally */ auto __finally_1289= das_finally([&](){
    finalize_db456bcf8e957ca4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1290_278));
    /* end finally */ });
    das_zero(__ctx_rename_at_1290_278);
    return das_auto_cast<char *>::cast(format_source_555ab24d2bad5d9a(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1290_278),__file_data_rename_at_1289_277));
}

inline das_source_formatter::TokenTemplate TokenTemplate_bb20a1d61f51dcf0 ( Context * __context__ )
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

inline das_source_formatter::Token Token_43fa8756d37aefe0 ( Context * __context__ )
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

inline das_source_formatter::FormatterCtx FormatterCtx_7640c15af9f3493d ( Context * __context__ )
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

inline das_source_formatter::ParenCounter ParenCounter_76ee88379e1e5165 ( Context * __context__ )
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
    aotLib[0x4aa25fac6f779bb0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_470a2c87c10459f3>>();
    };
    aotLib[0xb25e62c34835a4c0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_90b2200a89e5f7af>>();
    };
    aotLib[0x50fa518e09d52602] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_8ce52f97d3e92f29>>();
    };
    aotLib[0x736762683c24897e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab8b80dafe942dee>>();
    };
    aotLib[0x78b5bc4058f435fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_e9811f8b8637d85b>>();
    };
    aotLib[0x18df4ecc6397afc8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_4839b0184671562d>>();
    };
    aotLib[0x5094b41c7c1d5a1e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_b38e09a15f381650>>();
    };
    aotLib[0x9bf1d8b29aed8904] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_ee4a37bbcf70fe7d>>();
    };
    aotLib[0x23aa1048f8cb5edb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_3c986bfd8563d8b2>>();
    };
    aotLib[0xd1880c21eca14451] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_93cd492387de05e8>>();
    };
    aotLib[0xe44be04d8967857c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_fb7a66fc6a63821a>>();
    };
    aotLib[0xcb8c69afb656ecc1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickkey_existsTick16808803843923989214_5f8f761d5f23964f>>();
    };
    aotLib[0xbf11061381c6f0d3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_7926e574de26b25d>>();
    };
    aotLib[0xc0e83cc2459a2298] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_6a9e877c15ce25b4>>();
    };
    aotLib[0x8232e66bbea70b3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_cb951de01ca87a4e>>();
    };
    aotLib[0xe66ae9ae7f9b4090] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_7e6de3088c45b5fe>>();
    };
    aotLib[0x245f6c342c4fc47d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_7d58ec4da1ec1b17>>();
    };
    aotLib[0x6f8fd977899adec5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_7889f698fd41c370>>();
    };
    aotLib[0xf355fd66f709339f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_table_moveTick3386430563931768014_c018a9675934bba9>>();
    };
    aotLib[0xa7eef801007fb7f0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickreserveTick3994685146752941225_adfb084cc10e1733>>();
    };
    aotLib[0xe802aa1e42316a8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_19613b645e94680e>>();
    };
    aotLib[0x5b81ab1732e3add2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_eca839f98c5c2bc9>>();
    };
    aotLib[0xee2e9089f4ae9184] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_feb73043e4854e1f>>();
    };
    aotLib[0xa8d1bef95d45e76b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_e919271fa2eab772>>();
    };
    aotLib[0x736e9e79037419fb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_fc8141dc0153320a>>();
    };
    aotLib[0xd2b9abf5fc284bcf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_497548c4fac9c297>>();
    };
    aotLib[0x740a9c0d0a23719a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_833f2ca565fa23fb>>();
    };
    aotLib[0x24494df3d692d9c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_cfe2c69de3466279>>();
    };
    aotLib[0x7b7dd1e87c97d2b2] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfind_indexTick6703785223819426183_a4c49c41e04684c4>>();
    };
    aotLib[0x4d59b8ff59f743a3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_db456bcf8e957ca4>>();
    };
    aotLib[0x9410e993ae427200] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickcloneTick3038771811667655495_11c19903beee414c>>();
    };
    aotLib[0x2b67c4c2e6f82f05] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&initialize_token_templates_c0f484bb6b01366>>();
    };
    aotLib[0x362197137a3187d6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&isUtf8Bom_d46057f250b5bf59>>();
    };
    aotLib[0xe212b373667d8cda] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_5251ca869c24b49f>>();
    };
    aotLib[0xfcdbfa3221b55c38] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_3d631d49cf7e1d>>();
    };
    aotLib[0xa001d7a1047e2737] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_cecc7c5a47abfe6c>>();
    };
    aotLib[0x817b9d8f0734abaf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&eq_5ccd37f4ada4b6c8>>();
    };
    aotLib[0x8f4998ebc8e9692] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&new_line_before_74a9e3638b00c26f>>();
    };
    aotLib[0x36e75659e21ce1dc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&next_char_a68ee7affe47d0eb>>();
    };
    aotLib[0x3a595e2ab1f7c988] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&skip_spaces_2fa97eb70a48fea7>>();
    };
    aotLib[0x44380b9d762416dc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&on_token_added_ec8ba2110b19573e>>();
    };
    aotLib[0xa8bb24e388ec4d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&push_token_cc86b5bb25bf7adc>>();
    };
    aotLib[0xd9dd697f9bf31dcf] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&parse_token_321b59f95fe107c9>>();
    };
    aotLib[0x43e32108e79dd9f3] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&parse_all_tokens_29fbd1ed87913a0a>>();
    };
    aotLib[0xebb1d04b56891e1d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&update_paren_3503759ee55e89b1>>();
    };
    aotLib[0x2fb4da040dfbb63a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_paren_ff2776f9d2856b4e>>();
    };
    aotLib[0x42b26af16bf9cb44] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_square_89e4c86c49f03d61>>();
    };
    aotLib[0xa037baa886bde5b9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&find_pair_angle_43f908d2d3e3a500>>();
    };
    aotLib[0x1aa7e0a3973ee31e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_range_as_type_1c15599df80afc04>>();
    };
    aotLib[0xa9b10018b3aada51] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_tokens_as_type_ff7631c34d0e20a8>>();
    };
    aotLib[0x389ae1646111c52b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_tokens_as_type_inside_angle_5588509bf83aae77>>();
    };
    aotLib[0x21f00cef608f986d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&search_token_in_line_41b34300f8492a5f>>();
    };
    aotLib[0x443b871f81dfb9d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&process_formatter_tokens_b7e8fa22d8896a75>>();
    };
    aotLib[0x8c573025f7cef085] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&have_formatter_token_68c38613f91cfa3f>>();
    };
    aotLib[0x7b3a34dafc5316ab] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&mark_token_context_7d9da6c542abc3d5>>();
    };
    aotLib[0x2c18dd2e5db3a5e1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_arguments_393a0d20cb9876da>>();
    };
    aotLib[0x26004449ec407e17] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_args_b613d1f002f609af>>();
    };
    aotLib[0xd2ce7bd84f7f0f71] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&debug_print_tokens_d5c3886aafc47666>>();
    };
    aotLib[0xa3ff9c43ba3a01b1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&generate_source_d9e23c7a7a794181>>();
    };
    aotLib[0xdaa42b910b6a5443] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&need_spaces_around_c85752147cfda42d>>();
    };
    aotLib[0xd62b4948b08b32c7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&need_space_only_before_a3979a2c621820ef>>();
    };
    aotLib[0x5842f33ae696c111] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&is_unary_b0c7219745029c11>>();
    };
    aotLib[0x29ab249ea159c581] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&remove_spaces_around_203ede7b7eebc7c5>>();
    };
    aotLib[0xed11787ca1073ed8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&make_spaces_around_80d61c2838123a7d>>();
    };
    aotLib[0x35176c5a0d612c61] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_comma_f91c855ec419635f>>();
    };
    aotLib[0x159591694515b20e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_semicolon_23f19dedec62b617>>();
    };
    aotLib[0xb64ecf742a9caea1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_comma_e4ab43db37a70c74>>();
    };
    aotLib[0x581343b9f64885d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_semicolon_e5d8826c5665e6be>>();
    };
    aotLib[0xfb8771e003b8c04d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_type_colon_400b4390d7623d95>>();
    };
    aotLib[0x1bdfe060f0a8f55b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_decl_param_paren_dac5114d61e417ed>>();
    };
    aotLib[0x57e6a634ed2c43c6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_function_call_param_paren_call_347b6df2d6483625>>();
    };
    aotLib[0x7e23ec22c4c3ec5e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_spaces_around_operators_53d08ef306f852f9>>();
    };
    aotLib[0x3370471a0ad46ae9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_keyword_2536c1e9542cdc3b>>();
    };
    aotLib[0xea20f814e47d44c1] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_paren_82d46f74b7d5652>>();
    };
    aotLib[0xc48b81824a27eadd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_square_brackets_fdbea403935e8e12>>();
    };
    aotLib[0x6cfd0126eb4bf52a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_inside_parens_cb72085f2dcb9ecd>>();
    };
    aotLib[0x69767b5638f21b8d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_as_865a2960d4580501>>();
    };
    aotLib[0xb0fa9790354c00ff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_end_of_object_510336d353cc43f1>>();
    };
    aotLib[0xab3e0b23f75577e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_object_type_fff6f6cd3369398c>>();
    };
    aotLib[0x2f909eeb6e054ca9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_remove_space_before_array_of_type_fa116a3241d9e90c>>();
    };
    aotLib[0xf169a4e4fb9131b6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_glue_type_specifiers_538bf5888d1be714>>();
    };
    aotLib[0xf76a56e9b31e093f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_space_after_cast_type_5d3d2f4d38f83d01>>();
    };
    aotLib[0xf8626bcb721252dd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&fmt_destructure_space_2fcf524fd08df174>>();
    };
    aotLib[0x7e638a0b0782cb1a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&do_format_ff67931097f9f551>>();
    };
    aotLib[0x9cb42ccc8c21d4df] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_string_53ea2afc74eb7140>>();
    };
    aotLib[0x6674c826ed098555] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_555ab24d2bad5d9a>>();
    };
    aotLib[0x4448541f4d9bb865] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_source_82aae480574c279c>>();
    };
    aotLib[0x84f290f2c58a3a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&TokenTemplate_bb20a1d61f51dcf0>>();
    };
    aotLib[0x24b28b92ca3d936c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Token_43fa8756d37aefe0>>();
    };
    aotLib[0x57f9d0835efac403] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&FormatterCtx_7640c15af9f3493d>>();
    };
    aotLib[0xe58443c1815e45be] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&ParenCounter_76ee88379e1e5165>>();
    };
    // [[ init script ]]
    aotLib[0xa523129a946db789] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_6368649739184819056
AotListBase impl_aot_das_source_formatter(_anon_6368649739184819056::registerAotFunctions);
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
