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
namespace _anon_6388250703603292906 {

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
}
template <> struct cast< das::_anon_6388250703603292906::das_source_formatter::TokenType > : cast_enum < das::_anon_6388250703603292906::das_source_formatter::TokenType > {};
namespace _anon_6388250703603292906 {
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
extern StructInfo __struct_info__b9dac5daee1d371a;
extern StructInfo __struct_info__2367b22fd4eff4ce;
extern StructInfo __struct_info__176707c7dda89612;
extern StructInfo __struct_info__bf67b648e8f141ed;
extern TypeInfo __type_info__44217c02c8384397;
extern TypeInfo __type_info__4ac1d999a882997b;
extern TypeInfo __type_info__283b9666b80e9f63;
extern TypeInfo __type_info__56de687fda5d1b6a;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__df09f6a53fd00b75;
extern TypeInfo __type_info__f0ab76dcfe357749;
extern TypeInfo __type_info__b6a37ccbdf57026;
extern TypeInfo __type_info__6cf57a81c8f2e7f7;
extern TypeInfo __type_info__a8d2247d5b8ed91f;
extern TypeInfo __type_info__7dcc2c583c294a1f;
extern TypeInfo __type_info__af5be84c85f468f0;
extern VarInfo __var_info__aa16fbca48b57cf1;
extern VarInfo __var_info__deb0bdcbe4856f84;
extern VarInfo __var_info__9dfb83f74e72584a;
extern VarInfo __var_info__86f1ddb827d11b8d;
extern VarInfo __var_info__8995c44481004423;
extern VarInfo __var_info__f4c3c73c56a69822;
extern VarInfo __var_info__b41876b987355c7a;
extern VarInfo __var_info__bba175978cc9bdf;
extern VarInfo __var_info__45614084424a6abc;
extern VarInfo __var_info__67535897a592bade;
extern VarInfo __var_info__35f4d54720d4ce02;
extern VarInfo __var_info__588aa92f9692dee0;
extern VarInfo __var_info__7809663f162689b8;
extern VarInfo __var_info__fb0d76b9c3254e7a;
extern VarInfo __var_info__6073ecdc66e5f496;
extern VarInfo __var_info__44874708190da933;
extern VarInfo __var_info__3c9c27239aaf26cb;
extern VarInfo __var_info__3c23aa0cd445013a;
extern VarInfo __var_info__b683e4075133543d;
extern VarInfo __var_info__30f851260c7e5ebc;
extern VarInfo __var_info__256181fb5ddb9a2d;
extern VarInfo __var_info__16c21f9979f084eb;
extern VarInfo __var_info__70660f38157de5fe;
extern VarInfo __var_info__2fd549ae4eafc95b;
extern VarInfo __var_info__c234c0d4cfcb7a33;
extern VarInfo __var_info__d388499218704e4b;
extern VarInfo __var_info__19d9bf62421ca514;
extern VarInfo __var_info__1da4e59cc3480a2c;
extern VarInfo __var_info__36fc981c907ec0f8;
extern VarInfo __var_info__62acd8d846494554;
extern VarInfo __var_info__e807e1b774135f34;
extern VarInfo __var_info__f1744ecb99b5ee52;
extern VarInfo __var_info__12c1af86475d4616;
extern VarInfo __var_info__993f8b12277aa369;
extern EnumInfo __enum_info__9829b9b77f647adc;

EnumValueInfo __enum_info__9829b9b77f647adc_value_0 = { "UNKNOWN", 0 };
EnumValueInfo __enum_info__9829b9b77f647adc_value_1 = { "KEYWORD_OR_IDENTIFIER", 1 };
EnumValueInfo __enum_info__9829b9b77f647adc_value_2 = { "NUMBER", 2 };
EnumValueInfo __enum_info__9829b9b77f647adc_value_3 = { "COMMENT", 3 };
EnumValueInfo __enum_info__9829b9b77f647adc_value_4 = { "STRING", 4 };
EnumValueInfo * __enum_info__9829b9b77f647adc_values [] = { &__enum_info__9829b9b77f647adc_value_0, &__enum_info__9829b9b77f647adc_value_1, &__enum_info__9829b9b77f647adc_value_2, &__enum_info__9829b9b77f647adc_value_3, &__enum_info__9829b9b77f647adc_value_4 };
EnumInfo __enum_info__9829b9b77f647adc = { "TokenType", "das_source_formatter", __enum_info__9829b9b77f647adc_values, 5, UINT64_C(0x9829b9b77f647adc) };
VarInfo __struct_info__b9dac5daee1d371a_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__b6a37ccbdf57026, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x3c9c27239aaf26cb), "tokenTemplates", offsetof(das_source_formatter::FormatterCtx,tokenTemplates), 1 };
VarInfo __struct_info__b9dac5daee1d371a_field_1 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f0ab76dcfe357749, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x3c23aa0cd445013a), "tokens", offsetof(das_source_formatter::FormatterCtx,tokens), 2 };
VarInfo __struct_info__b9dac5daee1d371a_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__df09f6a53fd00b75, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0xbba175978cc9bdf), "formatterTokens", offsetof(das_source_formatter::FormatterCtx,formatterTokens), 5 };
VarInfo __struct_info__b9dac5daee1d371a_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xfb0d76b9c3254e7a), "pos", offsetof(das_source_formatter::FormatterCtx,pos), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_4 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xaa16fbca48b57cf1), "c", offsetof(das_source_formatter::FormatterCtx,c), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_5 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x8995c44481004423), "data", offsetof(das_source_formatter::FormatterCtx,data), 18 };
VarInfo __struct_info__b9dac5daee1d371a_field_6 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xb41876b987355c7a), "eof", offsetof(das_source_formatter::FormatterCtx,eof), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_7 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x45614084424a6abc), "haveUtf8Bom", offsetof(das_source_formatter::FormatterCtx,haveUtf8Bom), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_8 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x7809663f162689b8), "newLineCounter", offsetof(das_source_formatter::FormatterCtx,newLineCounter), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_9 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x6073ecdc66e5f496), "spaceCounter", offsetof(das_source_formatter::FormatterCtx,spaceCounter), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_10 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x44874708190da933), "srcLine", offsetof(das_source_formatter::FormatterCtx,srcLine), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_11 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x9dfb83f74e72584a), "curColumn", offsetof(das_source_formatter::FormatterCtx,curColumn), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_12 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x86f1ddb827d11b8d), "curLineIndex", offsetof(das_source_formatter::FormatterCtx,curLineIndex), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_13 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xf4c3c73c56a69822), "debugMode", offsetof(das_source_formatter::FormatterCtx,debugMode), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_14 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x67535897a592bade), "indenting", offsetof(das_source_formatter::FormatterCtx,indenting), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_15 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x35f4d54720d4ce02), "insideOptions", offsetof(das_source_formatter::FormatterCtx,insideOptions), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_16 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xdeb0bdcbe4856f84), "crCount", offsetof(das_source_formatter::FormatterCtx,crCount), 0 };
VarInfo __struct_info__b9dac5daee1d371a_field_17 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x588aa92f9692dee0), "lfCount", offsetof(das_source_formatter::FormatterCtx,lfCount), 0 };
VarInfo * __struct_info__b9dac5daee1d371a_fields[18] =  { &__struct_info__b9dac5daee1d371a_field_0, &__struct_info__b9dac5daee1d371a_field_1, &__struct_info__b9dac5daee1d371a_field_2, &__struct_info__b9dac5daee1d371a_field_3, &__struct_info__b9dac5daee1d371a_field_4, &__struct_info__b9dac5daee1d371a_field_5, &__struct_info__b9dac5daee1d371a_field_6, &__struct_info__b9dac5daee1d371a_field_7, &__struct_info__b9dac5daee1d371a_field_8, &__struct_info__b9dac5daee1d371a_field_9, &__struct_info__b9dac5daee1d371a_field_10, &__struct_info__b9dac5daee1d371a_field_11, &__struct_info__b9dac5daee1d371a_field_12, &__struct_info__b9dac5daee1d371a_field_13, &__struct_info__b9dac5daee1d371a_field_14, &__struct_info__b9dac5daee1d371a_field_15, &__struct_info__b9dac5daee1d371a_field_16, &__struct_info__b9dac5daee1d371a_field_17 };
StructInfo __struct_info__b9dac5daee1d371a = {"FormatterCtx", "das_source_formatter", 28, __struct_info__b9dac5daee1d371a_fields, 18, 152, UINT64_C(0x0), nullptr, UINT64_C(0xb9dac5daee1d371a), 0 };
VarInfo __struct_info__2367b22fd4eff4ce_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x30f851260c7e5ebc), "tokenIndex", offsetof(das_source_formatter::FormatterToken,tokenIndex), 0 };
VarInfo __struct_info__2367b22fd4eff4ce_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xb683e4075133543d), "command", offsetof(das_source_formatter::FormatterToken,command), 2 };
VarInfo * __struct_info__2367b22fd4eff4ce_fields[2] =  { &__struct_info__2367b22fd4eff4ce_field_0, &__struct_info__2367b22fd4eff4ce_field_1 };
StructInfo __struct_info__2367b22fd4eff4ce = {"FormatterToken", "das_source_formatter", 8, __struct_info__2367b22fd4eff4ce_fields, 2, 16, UINT64_C(0x0), nullptr, UINT64_C(0x2367b22fd4eff4ce), 1 };
VarInfo __struct_info__176707c7dda89612_field_0 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x36fc981c907ec0f8), "str", offsetof(das_source_formatter::Token,str), 10 };
VarInfo __struct_info__176707c7dda89612_field_1 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x1da4e59cc3480a2c), "spaces", offsetof(das_source_formatter::Token,spaces), 0 };
VarInfo __struct_info__176707c7dda89612_field_2 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x19d9bf62421ca514), "newLines", offsetof(das_source_formatter::Token,newLines), 0 };
VarInfo __struct_info__176707c7dda89612_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xd388499218704e4b), "lineInSource", offsetof(das_source_formatter::Token,lineInSource), 0 };
VarInfo __struct_info__176707c7dda89612_field_4 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x256181fb5ddb9a2d), "column", offsetof(das_source_formatter::Token,column), 0 };
VarInfo __struct_info__176707c7dda89612_field_5 =  { Type::tEnumeration, nullptr, &__enum_info__9829b9b77f647adc, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x62acd8d846494554), "tokenType", offsetof(das_source_formatter::Token,tokenType), 0 };
VarInfo __struct_info__176707c7dda89612_field_6 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x2fd549ae4eafc95b), "isInFunctionParam", offsetof(das_source_formatter::Token,isInFunctionParam), 0 };
VarInfo __struct_info__176707c7dda89612_field_7 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xc234c0d4cfcb7a33), "isInType", offsetof(das_source_formatter::Token,isInType), 0 };
VarInfo __struct_info__176707c7dda89612_field_8 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x70660f38157de5fe), "dontFormat", offsetof(das_source_formatter::Token,dontFormat), 0 };
VarInfo __struct_info__176707c7dda89612_field_9 =  { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0x16c21f9979f084eb), "dontAddSpacesAround", offsetof(das_source_formatter::Token,dontAddSpacesAround), 0 };
VarInfo * __struct_info__176707c7dda89612_fields[10] =  { &__struct_info__176707c7dda89612_field_0, &__struct_info__176707c7dda89612_field_1, &__struct_info__176707c7dda89612_field_2, &__struct_info__176707c7dda89612_field_3, &__struct_info__176707c7dda89612_field_4, &__struct_info__176707c7dda89612_field_5, &__struct_info__176707c7dda89612_field_6, &__struct_info__176707c7dda89612_field_7, &__struct_info__176707c7dda89612_field_8, &__struct_info__176707c7dda89612_field_9 };
StructInfo __struct_info__176707c7dda89612 = {"Token", "das_source_formatter", 8, __struct_info__176707c7dda89612_fields, 10, 32, UINT64_C(0x0), nullptr, UINT64_C(0x176707c7dda89612), 0 };
VarInfo __struct_info__bf67b648e8f141ed_field_0 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0xf1744ecb99b5ee52), "firstChar", offsetof(das_source_formatter::TokenTemplate,firstChar), 0 };
VarInfo __struct_info__bf67b648e8f141ed_field_1 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x993f8b12277aa369), "str", offsetof(das_source_formatter::TokenTemplate,str), 2 };
VarInfo __struct_info__bf67b648e8f141ed_field_2 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0xe807e1b774135f34), "codes", offsetof(das_source_formatter::TokenTemplate,codes), 4 };
VarInfo __struct_info__bf67b648e8f141ed_field_3 =  { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x12c1af86475d4616), "length", offsetof(das_source_formatter::TokenTemplate,length), 0 };
VarInfo * __struct_info__bf67b648e8f141ed_fields[4] =  { &__struct_info__bf67b648e8f141ed_field_0, &__struct_info__bf67b648e8f141ed_field_1, &__struct_info__bf67b648e8f141ed_field_2, &__struct_info__bf67b648e8f141ed_field_3 };
StructInfo __struct_info__bf67b648e8f141ed = {"TokenTemplate", "das_source_formatter", 28, __struct_info__bf67b648e8f141ed_fields, 4, 48, UINT64_C(0x0), nullptr, UINT64_C(0xbf67b648e8f141ed), 1 };
TypeInfo __type_info__44217c02c8384397 = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__6cf57a81c8f2e7f7, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x44217c02c8384397) };
TypeInfo __type_info__4ac1d999a882997b = { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x4ac1d999a882997b) };
TypeInfo __type_info__283b9666b80e9f63 = { Type::tStructure, &__struct_info__b9dac5daee1d371a, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 152, UINT64_C(0x283b9666b80e9f63) };
TypeInfo __type_info__56de687fda5d1b6a = { Type::tStructure, &__struct_info__bf67b648e8f141ed, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0x56de687fda5d1b6a) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__df09f6a53fd00b75 = { Type::tStructure, &__struct_info__2367b22fd4eff4ce, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 16, UINT64_C(0xdf09f6a53fd00b75) };
TypeInfo __type_info__f0ab76dcfe357749 = { Type::tStructure, &__struct_info__176707c7dda89612, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16390, 32, UINT64_C(0xf0ab76dcfe357749) };
TypeInfo __type_info__b6a37ccbdf57026 = { Type::tStructure, &__struct_info__bf67b648e8f141ed, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 48, UINT64_C(0xb6a37ccbdf57026) };
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
TypeInfo * __tinfo_4[1] = { &__type_info__56de687fda5d1b6a };
TypeInfo * __tinfo_5[1] = { &__type_info__283b9666b80e9f63 };
TypeInfo * __tinfo_6[5] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

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
inline das_source_formatter::TokenTemplate & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_6dd7c9d0804a0304 ( Context * __context__, das_source_formatter::TokenTemplate & __a_rename_at_50_21 );
inline das_source_formatter::FormatterCtx & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab33edb522746663 ( Context * __context__, das_source_formatter::FormatterCtx & __a_rename_at_50_22 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  & __a_rename_at_581_23 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_540ecf6ef9e01052 ( Context * __context__, TDim<char *,40> const  & __a_rename_at_581_24 );
inline void _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_165_25, das_source_formatter::Token const  & __value_rename_at_165_26 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a ( Context * __context__, TDim<char *,22> const  & __arr_rename_at_1658_27, char * const  __key_rename_at_1658_28 );
inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768 ( Context * __context__, TDim<char *,8> const  & __a_rename_at_1417_30 );
inline void _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_125_33, int32_t __newSize_rename_at_125_34 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_35 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3 ( Context * __context__, TDim<char *,21> const  & __arr_rename_at_1658_36, char * const  __key_rename_at_1658_37 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622 ( Context * __context__, TDim<char *,11> const  & __arr_rename_at_1658_39, char * const  __key_rename_at_1658_40 );
inline void _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65 ( Context * __context__, TArray<uint8_t> & __a_rename_at_1080_42, TArray<uint8_t> const  & __b_rename_at_1080_43 );
inline void _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673 ( Context * __context__, TArray<das_source_formatter::Token> & __Arr_rename_at_181_45, das_source_formatter::Token & __value_rename_at_181_46 );
inline void _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce ( Context * __context__, TArray<das_source_formatter::FormatterToken> & __Arr_rename_at_181_47, das_source_formatter::FormatterToken & __value_rename_at_181_48 );
inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_944_49, int32_t __i_rename_at_944_50 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_51, uint8_t __value_rename_at_165_52 );
inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_936_53, int32_t __i_rename_at_936_54 );
inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856 ( Context * __context__, TDim<char *,3> const  & __arr_rename_at_1658_55, char * const  __key_rename_at_1658_56 );
inline void finalize_b9f001def6083360 ( Context * __context__, das_source_formatter::FormatterCtx & ____this_rename_at_68_58 );
inline void initialize_token_templates_cd924eed93b743df ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_89_59 );
inline bool isUtf8Bom_fc921f39aa33ab23 ( Context * __context__, TArray<uint8_t> & __text_rename_at_103_66 );
inline bool eq_1a08ceae13f47e89 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_109_67, int32_t __index_rename_at_109_68, char * const  __s_rename_at_109_69 );
inline bool eq_32a38f27b5380f0f ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_114_70, int32_t __index_rename_at_114_71, char * const  __s0_rename_at_114_72, char * const  __s1_rename_at_114_73 );
inline bool eq_f43d812e3a992e4 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_120_74, int32_t __index_rename_at_120_75, char * const  __s0_rename_at_120_76, char * const  __s1_rename_at_120_77, char * const  __s2_rename_at_120_78 );
inline bool eq_886fe8d1ff63f5fd ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_126_79, int32_t __index_rename_at_126_80, char * const  __s0_rename_at_126_81, char * const  __s1_rename_at_126_82, char * const  __s2_rename_at_126_83, char * const  __s3_rename_at_126_84 );
inline bool new_line_before_56d9a06feda36687 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_132_85, int32_t __index_rename_at_132_86 );
inline void next_char_e34bb0a357da1559 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_137_87 );
inline void skip_spaces_a5027bfd73b7bde6 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_179_89 );
inline void on_token_added_6925eddc2a7ca309 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_186_90 );
inline void push_token_92f9637f04a53e36 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_204_91, int32_t __from_char_idx_rename_at_204_92, int32_t __to_char_idx_rename_at_204_93, int32_t __space_count_rename_at_204_94, int32_t __new_lines_rename_at_204_95, int32_t __src_line_rename_at_204_96, int32_t __column_rename_at_204_97, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_98 );
inline void parse_token_95969fa7ff7b12c4 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_220_102 );
inline void parse_all_tokens_b5d5b68330cdfcd0 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_344_121 );
inline void update_paren_62e64d469829ae3d ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_388_124, int32_t __index_rename_at_388_125, das_source_formatter::ParenCounter & __paren_counter_rename_at_388_126 );
inline int32_t find_pair_paren_90acbe7a3b864d93 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_425_127, int32_t __from_index_rename_at_425_128 );
inline int32_t find_pair_square_68d9ea1d7c30e49f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_437_131, int32_t __from_index_rename_at_437_132 );
inline int32_t find_pair_angle_89495f7d100107b7 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_449_135, int32_t __from_index_rename_at_449_136 );
inline void mark_range_as_type_24b8f876667238a0 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_461_139, int32_t __from_rename_at_461_140, int32_t __to_rename_at_461_141 );
inline void mark_tokens_as_type_5e4d056374f6927c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_470_143, int32_t __i_rename_at_470_144 );
inline void mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_528_147, int32_t __i_rename_at_528_148 );
inline int32_t search_token_in_line_3a9d4fd472c436b3 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_542_151, int32_t __from_index_rename_at_542_152, char * const  __str_rename_at_542_153 );
inline void process_formatter_tokens_a63e2ffa4c8206b6 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_564_156 );
inline bool have_formatter_token_296d7fc158126671 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_581_162, char * const  __command_rename_at_581_163 );
inline void mark_token_context_7dbf70435b518a0a ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_591_165 );
inline void fmt_function_arguments_7d9f22d6302573cb ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_791_199 );
inline void fmt_remove_space_before_args_26ba700ce1a53dbc ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_832_205 );
inline void debug_print_tokens_927617e00a5e50a7 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_841_207 );
inline char * generate_source_64706a1ce7db4983 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_848_209 );
inline bool need_spaces_around_7179a668ae90ccd6 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_891_218, int32_t __index_rename_at_891_219 );
inline bool need_space_only_before_584e48acb8e8a70 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_912_220, int32_t __index_rename_at_912_221 );
inline bool is_unary_62ad92645c7f9ee6 ( Context * __context__, das_source_formatter::FormatterCtx const  & __ctx_rename_at_956_222, int32_t __index_rename_at_956_223 );
inline void remove_spaces_around_1c96d2bcc36445d1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_979_225, int32_t __index_rename_at_979_226 );
inline void make_spaces_around_9b0360b9b4d718c3 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_989_227, int32_t __index_rename_at_989_228 );
inline void fmt_space_after_comma_7aa6a2faf3ce3f4f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_999_229 );
inline void fmt_space_after_semicolon_29caf5fd09f6497e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1008_231 );
inline void fmt_remove_space_before_comma_54b9cd4ba70794e ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1017_233 );
inline void fmt_remove_space_before_semicolon_132b81d1431b5d49 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1026_235 );
inline void fmt_type_colon_a5b252566f4d584 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1035_237 );
inline void fmt_function_decl_param_paren_6c291b7c3a975893 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1047_239 );
inline void fmt_function_call_param_paren_call_f740219cd0547b48 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1062_241 );
inline void fmt_spaces_around_operators_9837b8f174165da7 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1073_244 );
inline void fmt_space_after_keyword_2aab289129cf4fc8 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1092_246 );
inline void fmt_space_after_paren_a2ef6e80a339831f ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1109_248 );
inline void fmt_space_after_square_brackets_15fb170c80c43d93 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1118_250 );
inline void fmt_remove_space_inside_parens_fc44c6bd3c600b9c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1127_252 );
inline void fmt_remove_space_before_as_bb9fc0cabc4ff729 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1139_254 );
inline void fmt_remove_space_before_end_of_object_64e24a59c1d5b6b ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1148_256 );
inline void fmt_remove_space_before_object_type_298f1dcfc2f7829c ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1158_258 );
inline void fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1168_260 );
inline void fmt_glue_type_specifiers_cb3a1c8b879530ae ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1180_262 );
inline void fmt_space_after_cast_type_446660f13b2ec290 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1200_264 );
inline void fmt_destructure_space_e9871049a6de7d91 ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1215_267 );
inline void do_format_4835418a8469e5fa ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1224_269 );
inline char * format_source_string_f69d100c71c99a8c ( Context * __context__, char * const  & __file_data_rename_at_1267_273 );
inline char * format_source_58c20f6bcb79eabd ( Context * __context__, das_source_formatter::FormatterCtx & __ctx_rename_at_1276_277, TArray<uint8_t> const  & __file_data_rename_at_1276_278 );
inline char * format_source_f3ae203349d40bcb ( Context * __context__, TArray<uint8_t> const  & __file_data_rename_at_1284_279 );
inline das_source_formatter::TokenTemplate TokenTemplate_bb156f8df8893c2e ( Context * __context__ );
inline das_source_formatter::Token Token_c2eeda936a4bfe12 ( Context * __context__ );
inline das_source_formatter::FormatterCtx FormatterCtx_fd4f7dd8826cf847 ( Context * __context__ );
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
    memset(&(____this_rename_at_40_0), 0, TypeSize<das_source_formatter::TokenTemplate>::size);
}

inline void finalize_597eb4e562707157 ( Context * __context__, das_source_formatter::Token &  ____this_rename_at_48_1 )
{
    memset(&(____this_rename_at_48_1), 0, TypeSize<das_source_formatter::Token>::size);
}

inline void finalize_d6c2d9467df86ea8 ( Context * __context__, das_source_formatter::FormatterToken &  ____this_rename_at_62_2 )
{
    memset(&(____this_rename_at_62_2), 0, TypeSize<das_source_formatter::FormatterToken>::size);
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

inline das_source_formatter::TokenTemplate & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_6dd7c9d0804a0304 ( Context * __context__, das_source_formatter::TokenTemplate &  __a_rename_at_50_21 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_4,cast<das_source_formatter::TokenTemplate &>::from(__a_rename_at_50_21)));
    return das_auto_cast_ref<das_source_formatter::TokenTemplate &>::cast(__a_rename_at_50_21);
}

inline das_source_formatter::FormatterCtx & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab33edb522746663 ( Context * __context__, das_source_formatter::FormatterCtx &  __a_rename_at_50_22 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_5,cast<das_source_formatter::FormatterCtx &>::from(__a_rename_at_50_22)));
    return das_auto_cast_ref<das_source_formatter::FormatterCtx &>::cast(__a_rename_at_50_22);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_3422e7731ae3fc3d ( Context * __context__, TDim<char *,3> const  &  __a_rename_at_581_23 )
{
    return das_auto_cast<int32_t>::cast(3);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_540ecf6ef9e01052 ( Context * __context__, TDim<char *,40> const  &  __a_rename_at_581_24 )
{
    return das_auto_cast<int32_t>::cast(40);
}

inline void _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_165_25, das_source_formatter::Token const  &  __value_rename_at_165_26 )
{
    das_copy(__Arr_rename_at_165_25(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_165_25),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_26);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a ( Context * __context__, TDim<char *,22> const  &  __arr_rename_at_1658_27, char * const  __key_rename_at_1658_28 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,22));
        int32_t __i_rename_at_1659_29;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_29)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_29)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_27(__i_rename_at_1659_29,__context__)),cast<char *>::from(__key_rename_at_1658_28),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_29);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_29));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline TTable<char *,void> _FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768 ( Context * __context__, TDim<char *,8> const  &  __a_rename_at_1417_30 )
{
    TTable<char *,void> __tab_rename_at_1418_31; das_zero(__tab_rename_at_1418_31);
    {
        bool __need_loop_1419 = true;
        // at: string aka keyT const&
        das_iterator<TDim<char *,8> const > __at_iterator(__a_rename_at_1417_30);
        char * const  * __at_rename_at_1419_32;
        __need_loop_1419 = __at_iterator.first(__context__,(__at_rename_at_1419_32)) && __need_loop_1419;
        for ( ; __need_loop_1419 ; __need_loop_1419 = __at_iterator.next(__context__,(__at_rename_at_1419_32)) )
        {
            __builtin_table_set_insert(__context__,__tab_rename_at_1418_31,(*__at_rename_at_1419_32));
        }
        __at_iterator.close(__context__,(__at_rename_at_1419_32));
    };
    return /* <- */ das_auto_cast_move<TTable<char *,void>>::cast(__tab_rename_at_1418_31);
}

inline void _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_125_33, int32_t __newSize_rename_at_125_34 )
{
    builtin_array_reserve(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_125_33),__newSize_rename_at_125_34,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_35 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_35),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3 ( Context * __context__, TDim<char *,21> const  &  __arr_rename_at_1658_36, char * const  __key_rename_at_1658_37 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,21));
        int32_t __i_rename_at_1659_38;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_38)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_38)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_36(__i_rename_at_1659_38,__context__)),cast<char *>::from(__key_rename_at_1658_37),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_38);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_38));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622 ( Context * __context__, TDim<char *,11> const  &  __arr_rename_at_1658_39, char * const  __key_rename_at_1658_40 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,11));
        int32_t __i_rename_at_1659_41;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_41)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_41)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_39(__i_rename_at_1659_41,__context__)),cast<char *>::from(__key_rename_at_1658_40),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_41);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_41));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65 ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1080_42, TArray<uint8_t> const  &  __b_rename_at_1080_43 )
{
    int32_t __ln_rename_at_1081_44 = ((int32_t)builtin_array_size(__b_rename_at_1080_43));
    _FuncbuiltinTickresizeTick4811697762258667383_2333bfdfd8018176(__context__,das_arg<TArray<uint8_t>>::pass(__a_rename_at_1080_42),__ln_rename_at_1081_44);
    if ( __ln_rename_at_1081_44 == 0 )
    {
        return ;
    } else {
        das_memcpy(das_auto_cast<void * const >::cast(das_ref(__context__,__a_rename_at_1080_42(0,__context__))),das_auto_cast<void * const >::cast(das_ref(__context__,__b_rename_at_1080_43(0,__context__))),__ln_rename_at_1081_44 * 1);
    };
}

inline void _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673 ( Context * __context__, TArray<das_source_formatter::Token> &  __Arr_rename_at_181_45, das_source_formatter::Token &  __value_rename_at_181_46 )
{
    das_copy(__Arr_rename_at_181_45(builtin_array_push_back(das_arg<TArray<das_source_formatter::Token>>::pass(__Arr_rename_at_181_45),32,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_46);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce ( Context * __context__, TArray<das_source_formatter::FormatterToken> &  __Arr_rename_at_181_47, das_source_formatter::FormatterToken &  __value_rename_at_181_48 )
{
    das_copy(__Arr_rename_at_181_47(builtin_array_push_back(das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__Arr_rename_at_181_47),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_48);
}

inline void _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_944_49, int32_t __i_rename_at_944_50 )
{
    if ( (__ctx_rename_at_944_49.tokens(__i_rename_at_944_50,__context__).dontFormat || __ctx_rename_at_944_49.tokens((__i_rename_at_944_50 - 1),__context__).dontFormat) || new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_944_49),__i_rename_at_944_50) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_944_49.tokens(__i_rename_at_944_50,__context__).spaces,0);
    };
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_51, uint8_t __value_rename_at_165_52 )
{
    das_copy(__Arr_rename_at_165_51(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_51),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_52);
}

inline void _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_936_53, int32_t __i_rename_at_936_54 )
{
    if ( (__ctx_rename_at_936_53.tokens(__i_rename_at_936_54,__context__).dontFormat || __ctx_rename_at_936_53.tokens((__i_rename_at_936_54 - 1),__context__).dontFormat) || new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_936_53),__i_rename_at_936_54) )
    {
        return ;
    } else {
        das_copy(__ctx_rename_at_936_53.tokens(__i_rename_at_936_54,__context__).spaces,1);
    };
}

inline int32_t _FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856 ( Context * __context__, TDim<char *,3> const  &  __arr_rename_at_1658_55, char * const  __key_rename_at_1658_56 )
{
    {
        bool __need_loop_1659 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,3));
        int32_t __i_rename_at_1659_57;
        __need_loop_1659 = __i_iterator.first(__context__,(__i_rename_at_1659_57)) && __need_loop_1659;
        for ( ; __need_loop_1659 ; __need_loop_1659 = __i_iterator.next(__context__,(__i_rename_at_1659_57)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from(__arr_rename_at_1658_55(__i_rename_at_1659_57,__context__)),cast<char *>::from(__key_rename_at_1658_56),*__context__,nullptr) )
            {
                return das_auto_cast<int32_t>::cast(__i_rename_at_1659_57);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1659_57));
    };
    return das_auto_cast<int32_t>::cast(-1);
}

inline void finalize_b9f001def6083360 ( Context * __context__, das_source_formatter::FormatterCtx &  ____this_rename_at_68_58 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_5f7a4e8904f8977e(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(____this_rename_at_68_58.tokenTemplates));
    _FuncbuiltinTickfinalizeTick13836114024949725080_d46b5589c5f4f04b(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(____this_rename_at_68_58.tokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_b4e09f66b8118eb0(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(____this_rename_at_68_58.formatterTokens));
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(____this_rename_at_68_58.data));
    memset(&(____this_rename_at_68_58), 0, TypeSize<das_source_formatter::FormatterCtx>::size);
}

inline void initialize_token_templates_cd924eed93b743df ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_89_59 )
{
    if ( builtin_array_size(das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_59.tokenTemplates)) > 0 )
    {
        return ;
    } else {
        _FuncbuiltinTickresizeTick4811697762258667383_4a7556d56e5a6ac1(__context__,das_arg<TArray<das_source_formatter::TokenTemplate>>::pass(__ctx_rename_at_89_59.tokenTemplates),40);
        {
            bool __need_loop_94 = true;
            // s: string const&
            das_iterator<TDim<char *,40> const > __s_iterator(das_global<TDim<char *,40>,0x3e3c534a20a075ae>(__context__) /*sorted_tokens_source*/);
            char * const  * __s_rename_at_94_62;
            __need_loop_94 = __s_iterator.first(__context__,(__s_rename_at_94_62)) && __need_loop_94;
            // tt: das_source_formatter::TokenTemplate&
            das_iterator<TArray<das_source_formatter::TokenTemplate>> __tt_iterator(__ctx_rename_at_89_59.tokenTemplates);
            das_source_formatter::TokenTemplate * __tt_rename_at_94_63;
            __need_loop_94 = __tt_iterator.first(__context__,(__tt_rename_at_94_63)) && __need_loop_94;
            for ( ; __need_loop_94 ; __need_loop_94 = __s_iterator.next(__context__,(__s_rename_at_94_62)) && __tt_iterator.next(__context__,(__tt_rename_at_94_63)) )
            {
                das_copy((*__tt_rename_at_94_63).str,(*__s_rename_at_94_62));
                _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c(__context__,das_arg<TArray<uint8_t>>::pass((*__tt_rename_at_94_63).codes),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
                    TArray<uint8_t> ____acomp_96_21_rename_at_96_64; das_zero(____acomp_96_21_rename_at_96_64);
                    builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_64),false);
                    {
                        bool __need_loop_96 = true;
                        // c: int const
                        das_iterator<char * const > __c_iterator((*__s_rename_at_94_62));
                        int32_t __c_rename_at_96_65;
                        __need_loop_96 = __c_iterator.first(__context__,(__c_rename_at_96_65)) && __need_loop_96;
                        for ( ; __need_loop_96 ; __need_loop_96 = __c_iterator.next(__context__,(__c_rename_at_96_65)) )
                        {
                            _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_64),uint8_t(__c_rename_at_96_65));
                        }
                        __c_iterator.close(__context__,(__c_rename_at_96_65));
                    };
                    builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_64),true);
                    return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_96_21_rename_at_96_64)));
                })));
                das_copy((*__tt_rename_at_94_63).firstChar,int32_t((*__tt_rename_at_94_63).codes(0,__context__)));
                das_copy((*__tt_rename_at_94_63).length,builtin_string_length((*__s_rename_at_94_62),__context__));
            }
            __s_iterator.close(__context__,(__s_rename_at_94_62));
            __tt_iterator.close(__context__,(__tt_rename_at_94_63));
        };
    };
}

inline bool isUtf8Bom_fc921f39aa33ab23 ( Context * __context__, TArray<uint8_t> &  __text_rename_at_103_66 )
{
    return das_auto_cast<bool>::cast((((builtin_array_size(das_arg<TArray<uint8_t>>::pass(__text_rename_at_103_66)) >= 3) && (uint32_t(__text_rename_at_103_66(0,__context__)) == 0xefu)) && (uint32_t(__text_rename_at_103_66(1,__context__)) == 0xbbu)) && (uint32_t(__text_rename_at_103_66(2,__context__)) == 0xbfu));
}

inline bool eq_1a08ceae13f47e89 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_109_67, int32_t __index_rename_at_109_68, char * const  __s_rename_at_109_69 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_109_68 < 0) || (__index_rename_at_109_68 >= builtin_array_size(__ctx_rename_at_109_67.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_109_67.tokens(__index_rename_at_109_68,__context__).str),cast<char *>::from(__s_rename_at_109_69),*__context__,nullptr))));
}

inline bool eq_32a38f27b5380f0f ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_114_70, int32_t __index_rename_at_114_71, char * const  __s0_rename_at_114_72, char * const  __s1_rename_at_114_73 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_114_71 < 0) || (__index_rename_at_114_71 >= builtin_array_size(__ctx_rename_at_114_70.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_70.tokens(__index_rename_at_114_71,__context__).str),cast<char *>::from(__s0_rename_at_114_72),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_114_70.tokens(__index_rename_at_114_71,__context__).str),cast<char *>::from(__s1_rename_at_114_73),*__context__,nullptr)))));
}

inline bool eq_f43d812e3a992e4 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_120_74, int32_t __index_rename_at_120_75, char * const  __s0_rename_at_120_76, char * const  __s1_rename_at_120_77, char * const  __s2_rename_at_120_78 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_120_75 < 0) || (__index_rename_at_120_75 >= builtin_array_size(__ctx_rename_at_120_74.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_74.tokens(__index_rename_at_120_75,__context__).str),cast<char *>::from(__s0_rename_at_120_76),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_74.tokens(__index_rename_at_120_75,__context__).str),cast<char *>::from(__s1_rename_at_120_77),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_120_74.tokens(__index_rename_at_120_75,__context__).str),cast<char *>::from(__s2_rename_at_120_78),*__context__,nullptr)))));
}

inline bool eq_886fe8d1ff63f5fd ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_126_79, int32_t __index_rename_at_126_80, char * const  __s0_rename_at_126_81, char * const  __s1_rename_at_126_82, char * const  __s2_rename_at_126_83, char * const  __s3_rename_at_126_84 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_126_80 < 0) || (__index_rename_at_126_80 >= builtin_array_size(__ctx_rename_at_126_79.tokens))) ? das_auto_cast<bool>::cast(false) : das_auto_cast<bool>::cast(((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_79.tokens(__index_rename_at_126_80,__context__).str),cast<char *>::from(__s0_rename_at_126_81),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_79.tokens(__index_rename_at_126_80,__context__).str),cast<char *>::from(__s1_rename_at_126_82),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_79.tokens(__index_rename_at_126_80,__context__).str),cast<char *>::from(__s2_rename_at_126_83),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_126_79.tokens(__index_rename_at_126_80,__context__).str),cast<char *>::from(__s3_rename_at_126_84),*__context__,nullptr)))));
}

inline bool new_line_before_56d9a06feda36687 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_132_85, int32_t __index_rename_at_132_86 )
{
    return das_auto_cast<bool>::cast(((__index_rename_at_132_86 <= 1) || (__index_rename_at_132_86 >= builtin_array_size(__ctx_rename_at_132_85.tokens))) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast((__ctx_rename_at_132_85.tokens(__index_rename_at_132_86,__context__).newLines > 0)));
}

inline void next_char_e34bb0a357da1559 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_137_87 )
{
    das_copy(__ctx_rename_at_137_87.c,int32_t(__ctx_rename_at_137_87.data(__ctx_rename_at_137_87.pos,__context__)));
    if ( __ctx_rename_at_137_87.c == 0 )
    {
        das_copy(__ctx_rename_at_137_87.eof,true);
        return ;
    } else {
        if ( (__ctx_rename_at_137_87.c == 13) && (int32_t(__ctx_rename_at_137_87.data((__ctx_rename_at_137_87.pos + 1),__context__)) == 10) )
        {
            ++__ctx_rename_at_137_87.crCount;
            ++__ctx_rename_at_137_87.pos;
            das_copy(__ctx_rename_at_137_87.c,int32_t(__ctx_rename_at_137_87.data(__ctx_rename_at_137_87.pos,__context__)));
        };
        if ( __ctx_rename_at_137_87.c == 32 )
        {
            ++__ctx_rename_at_137_87.spaceCounter;
        };
        if ( __ctx_rename_at_137_87.c == 9 )
        {
            int32_t __indent_rename_at_155_88 = ((int32_t)((__ctx_rename_at_137_87.indenting != 0) ? das_auto_cast<int32_t>::cast(__ctx_rename_at_137_87.indenting) : das_auto_cast<int32_t>::cast(4)));
            __ctx_rename_at_137_87.spaceCounter += __indent_rename_at_155_88;
            __ctx_rename_at_137_87.curColumn += (__indent_rename_at_155_88 - 1);
        };
        if ( __ctx_rename_at_137_87.c == 10 )
        {
            ++__ctx_rename_at_137_87.lfCount;
            ++__ctx_rename_at_137_87.pos;
            das_copy(__ctx_rename_at_137_87.c,int32_t(__ctx_rename_at_137_87.data(__ctx_rename_at_137_87.pos,__context__)));
            das_copy(__ctx_rename_at_137_87.curLineIndex,__ctx_rename_at_137_87.pos);
            das_copy(__ctx_rename_at_137_87.spaceCounter,0);
            ++__ctx_rename_at_137_87.newLineCounter;
            ++__ctx_rename_at_137_87.srcLine;
            das_copy(__ctx_rename_at_137_87.curColumn,0);
            return ;
        } else {
            ++__ctx_rename_at_137_87.pos;
            das_copy(__ctx_rename_at_137_87.c,int32_t(__ctx_rename_at_137_87.data(__ctx_rename_at_137_87.pos,__context__)));
            ++__ctx_rename_at_137_87.curColumn;
            return ;
        };
    };
}

inline void skip_spaces_a5027bfd73b7bde6 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_179_89 )
{
    while ( (((__ctx_rename_at_179_89.c == 32) || (__ctx_rename_at_179_89.c == 9)) || (__ctx_rename_at_179_89.c == 13)) || (__ctx_rename_at_179_89.c == 10) )
    {
        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_179_89));
    };
}

inline void on_token_added_6925eddc2a7ca309 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_186_90 )
{
    if ( (((__ctx_rename_at_186_90.indenting == 0) && (__ctx_rename_at_186_90.newLineCounter > 0)) && (__ctx_rename_at_186_90.spaceCounter > 0)) && (__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        das_copy(__ctx_rename_at_186_90.indenting,__ctx_rename_at_186_90.spaceCounter);
    };
    if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_186_90),builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1) )
    {
        das_copy(__ctx_rename_at_186_90.insideOptions,false);
        if ( SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1),__context__).str),cast<char *>::from(((char *) "options")),*__context__,nullptr) )
        {
            das_copy(__ctx_rename_at_186_90.insideOptions,true);
        };
    };
    if ( (((__ctx_rename_at_186_90.insideOptions && (__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::NUMBER)) && (builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) >= 3)) && (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 3),__context__).str),cast<char *>::from(((char *) "indenting")),*__context__,nullptr))) && (builtin_string_length(__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1),__context__).str,__context__) == 1) )
    {
        das_copy(__ctx_rename_at_186_90.indenting,SimPolicy<int32_t>::Clamp(fast_to_int(__ctx_rename_at_186_90.tokens((builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_186_90.tokens)) - 1),__context__).str,false),1,8,*__context__,nullptr));
    };
}

inline void push_token_92f9637f04a53e36 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_204_91, int32_t __from_char_idx_rename_at_204_92, int32_t __to_char_idx_rename_at_204_93, int32_t __space_count_rename_at_204_94, int32_t __new_lines_rename_at_204_95, int32_t __src_line_rename_at_204_96, int32_t __column_rename_at_204_97, DAS_COMMENT(enum) das_source_formatter::TokenType __token_type_rename_at_204_98 )
{
    TArray<uint8_t> __buf_rename_at_205_99; memset(&__buf_rename_at_205_99,0,sizeof(__buf_rename_at_205_99));
    das_source_formatter::Token _temp_make_local_206_23_176; _temp_make_local_206_23_176;
    /* finally */ auto __finally_204= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_99));
    /* end finally */ });
    __buf_rename_at_205_99; das_zero(__buf_rename_at_205_99); das_move(__buf_rename_at_205_99, das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
        TArray<uint8_t> ____acomp_205_17_rename_at_205_100; das_zero(____acomp_205_17_rename_at_205_100);
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_100),false);
        {
            bool __need_loop_205 = true;
            // i: int const
            das_iterator<range> __i_iterator(range(__from_char_idx_rename_at_204_92,__to_char_idx_rename_at_204_93));
            int32_t __i_rename_at_205_101;
            __need_loop_205 = __i_iterator.first(__context__,(__i_rename_at_205_101)) && __need_loop_205;
            for ( ; __need_loop_205 ; __need_loop_205 = __i_iterator.next(__context__,(__i_rename_at_205_101)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_100),uint8_t(__ctx_rename_at_204_91.data(__i_rename_at_205_101,__context__)));
            }
            __i_iterator.close(__context__,(__i_rename_at_205_101));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_205_17_rename_at_205_100),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(____acomp_205_17_rename_at_205_100);
    }));
    _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_204_91.tokens),das_arg<das_source_formatter::Token>::pass((([&]() -> das_source_formatter::Token& {
        _temp_make_local_206_23_176 = Token_c2eeda936a4bfe12(__context__);
        das_copy((_temp_make_local_206_23_176.str),(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__buf_rename_at_205_99),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
        das_copy((_temp_make_local_206_23_176.spaces),(__space_count_rename_at_204_94));
        das_copy((_temp_make_local_206_23_176.newLines),(__new_lines_rename_at_204_95));
        das_copy((_temp_make_local_206_23_176.lineInSource),(__src_line_rename_at_204_96));
        das_copy((_temp_make_local_206_23_176.column),(__column_rename_at_204_97));
        das_copy((_temp_make_local_206_23_176.tokenType),(__token_type_rename_at_204_98));
        return _temp_make_local_206_23_176;
    })())));
    on_token_added_6925eddc2a7ca309(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_204_91));
    das_copy(__ctx_rename_at_204_91.spaceCounter,0);
    das_copy(__ctx_rename_at_204_91.newLineCounter,0);
}

inline void parse_token_95969fa7ff7b12c4 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_220_102 )
{
    skip_spaces_a5027bfd73b7bde6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
    if ( __ctx_rename_at_220_102.eof )
    {
        _FuncbuiltinTickpushTick10769833213962245646_23aa2befa5110673(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_220_102.tokens),das_arg<das_source_formatter::Token>::pass(__ctx_rename_at_220_102.tokens(0,__context__)));
        return ;
    } else {
        int32_t __from_rename_at_227_103 = ((int32_t)__ctx_rename_at_220_102.pos);
        int32_t __fromColumn_rename_at_228_104 = ((int32_t)__ctx_rename_at_220_102.curColumn);
        if ( (is_alpha(__ctx_rename_at_220_102.c) || (__ctx_rename_at_220_102.c == 95)) || (__ctx_rename_at_220_102.c == 96) )
        {
            while ( !(__ctx_rename_at_220_102.eof) && (((is_alpha(__ctx_rename_at_220_102.c) || is_number(__ctx_rename_at_220_102.c)) || (__ctx_rename_at_220_102.c == 95)) || (__ctx_rename_at_220_102.c == 96)) )
            {
                next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
            };
            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__ctx_rename_at_220_102.spaceCounter,__ctx_rename_at_220_102.newLineCounter,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER);
            return ;
        } else {
            if ( is_number(__ctx_rename_at_220_102.c) || ((__ctx_rename_at_220_102.c == 46) && is_number(int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos + 1),__context__)))) )
            {
                while ( !(__ctx_rename_at_220_102.eof) && (((is_number(__ctx_rename_at_220_102.c) || is_alpha(__ctx_rename_at_220_102.c)) || (__ctx_rename_at_220_102.c == 46)) || (((__ctx_rename_at_220_102.c == 45) || (__ctx_rename_at_220_102.c == 43)) && ((int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos - 1),__context__)) == 101) || (int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos - 1),__context__)) == 69)))) )
                {
                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                };
                push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__ctx_rename_at_220_102.spaceCounter,__ctx_rename_at_220_102.newLineCounter,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::NUMBER);
                return ;
            } else {
                if ( (__ctx_rename_at_220_102.c == 34) || (__ctx_rename_at_220_102.c == 39) )
                {
                    int32_t __savedLine_rename_at_251_105 = ((int32_t)__ctx_rename_at_220_102.newLineCounter);
                    int32_t __savedSpace_rename_at_252_106 = ((int32_t)__ctx_rename_at_220_102.spaceCounter);
                    int32_t __openChar_rename_at_253_107 = ((int32_t)__ctx_rename_at_220_102.c);
                    int32_t __depth_rename_at_254_108 = 0;
                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                    while ( !(__ctx_rename_at_220_102.eof) && ((__ctx_rename_at_220_102.c != __openChar_rename_at_253_107) || (__depth_rename_at_254_108 > 0)) )
                    {
                        if ( (__ctx_rename_at_220_102.c == 123) && (__openChar_rename_at_253_107 == 34) )
                        {
                            ++__depth_rename_at_254_108;
                        };
                        if ( (__ctx_rename_at_220_102.c == 125) && (__openChar_rename_at_253_107 == 34) )
                        {
                            --__depth_rename_at_254_108;
                        };
                        if ( __ctx_rename_at_220_102.c == 92 )
                        {
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                        };
                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                    };
                    if ( __ctx_rename_at_220_102.c == __openChar_rename_at_253_107 )
                    {
                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                    };
                    push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__savedSpace_rename_at_252_106,__savedLine_rename_at_251_105,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::STRING);
                    return ;
                } else {
                    if ( (__ctx_rename_at_220_102.c == 47) && (int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos + 1),__context__)) == 47) )
                    {
                        int32_t __savedLine_rename_at_279_109 = ((int32_t)__ctx_rename_at_220_102.newLineCounter);
                        int32_t __savedSpace_rename_at_280_110 = ((int32_t)__ctx_rename_at_220_102.spaceCounter);
                        while ( (!(__ctx_rename_at_220_102.eof) && (__ctx_rename_at_220_102.c != 10)) && (__ctx_rename_at_220_102.c != 13) )
                        {
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                        };
                        push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__savedSpace_rename_at_280_110,__savedLine_rename_at_279_109,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
                        return ;
                    } else {
                        if ( (__ctx_rename_at_220_102.c == 47) && (int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos + 1),__context__)) == 42) )
                        {
                            int32_t __savedLine_rename_at_290_111 = ((int32_t)__ctx_rename_at_220_102.newLineCounter);
                            int32_t __savedSpace_rename_at_291_112 = ((int32_t)__ctx_rename_at_220_102.spaceCounter);
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            int32_t __depth_rename_at_294_113 = 1;
                            while ( !__ctx_rename_at_220_102.eof )
                            {
                                if ( (__ctx_rename_at_220_102.c == 42) && (int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos + 1),__context__)) == 47) )
                                {
                                    __depth_rename_at_294_113 -= 1;
                                    if ( __depth_rename_at_294_113 == 0 )
                                    {
                                        break;
                                    } else {
                                        next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                                    };
                                };
                                if ( (__ctx_rename_at_220_102.c == 47) && (int32_t(__ctx_rename_at_220_102.data((__ctx_rename_at_220_102.pos + 1),__context__)) == 42) )
                                {
                                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                                    __depth_rename_at_294_113 += 1;
                                };
                                next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            };
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__savedSpace_rename_at_291_112,__savedLine_rename_at_290_111,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT);
                            return ;
                        } else {
                            {
                                bool __need_loop_318 = true;
                                // tt: das_source_formatter::TokenTemplate&
                                das_iterator<TArray<das_source_formatter::TokenTemplate>> __tt_iterator(__ctx_rename_at_220_102.tokenTemplates);
                                das_source_formatter::TokenTemplate * __tt_rename_at_318_114;
                                __need_loop_318 = __tt_iterator.first(__context__,(__tt_rename_at_318_114)) && __need_loop_318;
                                for ( ; __need_loop_318 ; __need_loop_318 = __tt_iterator.next(__context__,(__tt_rename_at_318_114)) )
                                {
                                    if ( __ctx_rename_at_220_102.c == (*__tt_rename_at_318_114).firstChar )
                                    {
                                        bool __match_rename_at_320_115 = true;
                                        {
                                            bool __need_loop_321 = true;
                                            // i: int const
                                            das_iterator<range> __i_iterator(range(__ctx_rename_at_220_102.pos,__ctx_rename_at_220_102.pos + (*__tt_rename_at_318_114).length));
                                            int32_t __i_rename_at_321_118;
                                            __need_loop_321 = __i_iterator.first(__context__,(__i_rename_at_321_118)) && __need_loop_321;
                                            // code: uint8&
                                            das_iterator<TArray<uint8_t>> __code_iterator((*__tt_rename_at_318_114).codes);
                                            uint8_t * __code_rename_at_321_119;
                                            __need_loop_321 = __code_iterator.first(__context__,(__code_rename_at_321_119)) && __need_loop_321;
                                            for ( ; __need_loop_321 ; __need_loop_321 = __i_iterator.next(__context__,(__i_rename_at_321_118)) && __code_iterator.next(__context__,(__code_rename_at_321_119)) )
                                            {
                                                if ( das_nequ_val(__ctx_rename_at_220_102.data(__i_rename_at_321_118,__context__),(*__code_rename_at_321_119)) )
                                                {
                                                    das_copy(__match_rename_at_320_115,false);
                                                    break;
                                                };
                                            }
                                            __i_iterator.close(__context__,(__i_rename_at_321_118));
                                            __code_iterator.close(__context__,(__code_rename_at_321_119));
                                        };
                                        if ( __match_rename_at_320_115 )
                                        {
                                            {
                                                bool __need_loop_328 = true;
                                                // _: int const
                                                das_iterator<range> ____iterator(range(0,(*__tt_rename_at_318_114).length));
                                                int32_t ____rename_at_328_120;
                                                __need_loop_328 = ____iterator.first(__context__,(____rename_at_328_120)) && __need_loop_328;
                                                for ( ; __need_loop_328 ; __need_loop_328 = ____iterator.next(__context__,(____rename_at_328_120)) )
                                                {
                                                    next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                                                }
                                                ____iterator.close(__context__,(____rename_at_328_120));
                                            };
                                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__ctx_rename_at_220_102.spaceCounter,__ctx_rename_at_220_102.newLineCounter,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                                            return ;
                                        };
                                    };
                                }
                                __tt_iterator.close(__context__,(__tt_rename_at_318_114));
                            };
                            next_char_e34bb0a357da1559(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102));
                            push_token_92f9637f04a53e36(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_220_102),__from_rename_at_227_103,__ctx_rename_at_220_102.pos,__ctx_rename_at_220_102.spaceCounter,__ctx_rename_at_220_102.newLineCounter,__ctx_rename_at_220_102.srcLine,__fromColumn_rename_at_228_104,DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN);
                            return ;
                        };
                    };
                };
            };
        };
    };
}

inline void parse_all_tokens_b5d5b68330cdfcd0 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_344_121 )
{
    initialize_token_templates_cd924eed93b743df(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_121));
    das_source_formatter::Token __emptyToken_rename_at_346_122_ConstRef; das_zero(__emptyToken_rename_at_346_122_ConstRef); das_move(__emptyToken_rename_at_346_122_ConstRef, ((das_source_formatter::Token)(([&]() -> das_source_formatter::Token {
        das_source_formatter::Token __mks_346 = Token_c2eeda936a4bfe12(__context__);
        return __mks_346;
    })())));
    das_source_formatter::Token const  & __emptyToken_rename_at_346_122 = __emptyToken_rename_at_346_122_ConstRef; ;
    builtin_array_clear(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_121.tokens),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_121.tokens),__emptyToken_rename_at_346_122);
    das_copy(__ctx_rename_at_344_121.pos,0);
    das_copy(__ctx_rename_at_344_121.eof,false);
    das_copy(__ctx_rename_at_344_121.haveUtf8Bom,false);
    das_copy(__ctx_rename_at_344_121.newLineCounter,0);
    das_copy(__ctx_rename_at_344_121.spaceCounter,0);
    das_copy(__ctx_rename_at_344_121.srcLine,1);
    das_copy(__ctx_rename_at_344_121.curColumn,0);
    das_copy(__ctx_rename_at_344_121.indenting,0);
    das_copy(__ctx_rename_at_344_121.insideOptions,false);
    das_copy(__ctx_rename_at_344_121.crCount,0);
    das_copy(__ctx_rename_at_344_121.lfCount,0);
    if ( isUtf8Bom_fc921f39aa33ab23(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_344_121.data)) )
    {
        __ctx_rename_at_344_121.pos += 3;
        das_copy(__ctx_rename_at_344_121.haveUtf8Bom,true);
    };
    das_copy(__ctx_rename_at_344_121.c,int32_t(__ctx_rename_at_344_121.data(__ctx_rename_at_344_121.pos,__context__)));
    das_copy(__ctx_rename_at_344_121.curLineIndex,__ctx_rename_at_344_121.pos);
    while ( !__ctx_rename_at_344_121.eof )
    {
        parse_token_95969fa7ff7b12c4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_344_121));
    };
    {
        bool __need_loop_373 = true;
        // _: int const
        das_iterator<range> ____iterator(range(0,8));
        int32_t ____rename_at_373_123;
        __need_loop_373 = ____iterator.first(__context__,(____rename_at_373_123)) && __need_loop_373;
        for ( ; __need_loop_373 ; __need_loop_373 = ____iterator.next(__context__,(____rename_at_373_123)) )
        {
            _FuncbuiltinTickpushTick14133213201864676143_6efdca5732854753(__context__,das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_344_121.tokens),__emptyToken_rename_at_346_122);
        }
        ____iterator.close(__context__,(____rename_at_373_123));
    };
}

inline void update_paren_62e64d469829ae3d ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_388_124, int32_t __index_rename_at_388_125, das_source_formatter::ParenCounter &  __paren_counter_rename_at_388_126 )
{
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "<")) )
    {
        ++__paren_counter_rename_at_388_126.angle;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) ">")) )
    {
        --__paren_counter_rename_at_388_126.angle;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) ">>")) )
    {
        __paren_counter_rename_at_388_126.angle -= 2;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) ">>>")) )
    {
        __paren_counter_rename_at_388_126.angle -= 3;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "(")) )
    {
        ++__paren_counter_rename_at_388_126.paren;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) ")")) )
    {
        --__paren_counter_rename_at_388_126.paren;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "[")) )
    {
        ++__paren_counter_rename_at_388_126.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "?[")) )
    {
        ++__paren_counter_rename_at_388_126.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "]")) )
    {
        --__paren_counter_rename_at_388_126.square;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "{")) )
    {
        ++__paren_counter_rename_at_388_126.curly;
    };
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_388_124),__index_rename_at_388_125,((char *) "}")) )
    {
        --__paren_counter_rename_at_388_126.curly;
    };
}

inline int32_t find_pair_paren_90acbe7a3b864d93 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_425_127, int32_t __from_index_rename_at_425_128 )
{
    das_source_formatter::ParenCounter __pc_rename_at_426_129; das_zero(__pc_rename_at_426_129);
    {
        bool __need_loop_427 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_425_128 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_425_127.tokens))));
        int32_t __j_rename_at_427_130;
        __need_loop_427 = __j_iterator.first(__context__,(__j_rename_at_427_130)) && __need_loop_427;
        for ( ; __need_loop_427 ; __need_loop_427 = __j_iterator.next(__context__,(__j_rename_at_427_130)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_425_127),__j_rename_at_427_130,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_426_129));
            if ( __pc_rename_at_426_129.paren < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_427_130);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_427_130));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_425_128);
}

inline int32_t find_pair_square_68d9ea1d7c30e49f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_437_131, int32_t __from_index_rename_at_437_132 )
{
    das_source_formatter::ParenCounter __pc_rename_at_438_133; das_zero(__pc_rename_at_438_133);
    {
        bool __need_loop_439 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_437_132 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_437_131.tokens))));
        int32_t __j_rename_at_439_134;
        __need_loop_439 = __j_iterator.first(__context__,(__j_rename_at_439_134)) && __need_loop_439;
        for ( ; __need_loop_439 ; __need_loop_439 = __j_iterator.next(__context__,(__j_rename_at_439_134)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_437_131),__j_rename_at_439_134,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_438_133));
            if ( __pc_rename_at_438_133.square < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_439_134);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_439_134));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_437_132);
}

inline int32_t find_pair_angle_89495f7d100107b7 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_449_135, int32_t __from_index_rename_at_449_136 )
{
    das_source_formatter::ParenCounter __pc_rename_at_450_137; das_zero(__pc_rename_at_450_137);
    {
        bool __need_loop_451 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_449_136 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_449_135.tokens))));
        int32_t __j_rename_at_451_138;
        __need_loop_451 = __j_iterator.first(__context__,(__j_rename_at_451_138)) && __need_loop_451;
        for ( ; __need_loop_451 ; __need_loop_451 = __j_iterator.next(__context__,(__j_rename_at_451_138)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_449_135),__j_rename_at_451_138,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_450_137));
            if ( __pc_rename_at_450_137.angle < 0 )
            {
                return das_auto_cast<int32_t>::cast(__j_rename_at_451_138);
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_451_138));
    };
    return das_auto_cast<int32_t>::cast(__from_index_rename_at_449_136);
}

inline void mark_range_as_type_24b8f876667238a0 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_461_139, int32_t __from_rename_at_461_140, int32_t __to_rename_at_461_141 )
{
    {
        bool __need_loop_462 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(__from_rename_at_461_140,__to_rename_at_461_141));
        int32_t __i_rename_at_462_142;
        __need_loop_462 = __i_iterator.first(__context__,(__i_rename_at_462_142)) && __need_loop_462;
        for ( ; __need_loop_462 ; __need_loop_462 = __i_iterator.next(__context__,(__i_rename_at_462_142)) )
        {
            if ( __ctx_rename_at_461_139.tokens(__i_rename_at_462_142,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT )
            {
                das_copy(__ctx_rename_at_461_139.tokens(__i_rename_at_462_142,__context__).isInType,true);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_462_142));
    };
}

inline void mark_tokens_as_type_5e4d056374f6927c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_470_143, int32_t __i_rename_at_470_144 )
{
    int32_t __pos_rename_at_471_145 = __i_rename_at_470_144;
    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "auto")) )
    {
        ++__pos_rename_at_471_145;
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "(")) )
        {
            das_copy(__pos_rename_at_471_145,find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145));
            ++__pos_rename_at_471_145;
        };
    } else {
        while ( __pos_rename_at_471_145 < builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_470_143.tokens)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145) )
            {
                break;
            } else {
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "::")) || (__ctx_rename_at_470_143.tokens(__pos_rename_at_471_145,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
                {
                    ++__pos_rename_at_471_145;
                } else {
                    break;
                };
            };
        };
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "<")) )
        {
            das_copy(__pos_rename_at_471_145,find_pair_angle_89495f7d100107b7(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145));
            ++__pos_rename_at_471_145;
        };
    };
    if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145) )
    {
        mark_range_as_type_24b8f876667238a0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__i_rename_at_470_144,__pos_rename_at_471_145);
        return ;
    } else {
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "[")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145 + 1,((char *) "]")) )
        {
            __pos_rename_at_471_145 += 2;
        };
        {
            bool __need_loop_507 = true;
            // j: int const
            das_iterator<range> __j_iterator(range(__pos_rename_at_471_145,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_470_143.tokens))));
            int32_t __j_rename_at_507_146;
            __need_loop_507 = __j_iterator.first(__context__,(__j_rename_at_507_146)) && __need_loop_507;
            for ( ; __need_loop_507 ; __need_loop_507 = __j_iterator.next(__context__,(__j_rename_at_507_146)) )
            {
                if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__j_rename_at_507_146,((char *) "-"),((char *) "==")) && eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__j_rename_at_507_146 + 1,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) )
                {
                    das_copy(__pos_rename_at_471_145,__j_rename_at_507_146 + 2);
                } else if ( eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__j_rename_at_507_146,((char *) "const"),((char *) "&"),((char *) "#"),((char *) "?")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__j_rename_at_507_146,((char *) "implicit")) )
                {
                    das_copy(__pos_rename_at_471_145,__j_rename_at_507_146 + 1);
                } else {
                    break;
                };
            }
            __j_iterator.close(__context__,(__j_rename_at_507_146));
        };
        mark_range_as_type_24b8f876667238a0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__i_rename_at_470_144,__pos_rename_at_471_145);
        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145,((char *) "|")) )
        {
            das_copy(__ctx_rename_at_470_143.tokens(__pos_rename_at_471_145,__context__).spaces,1);
            ++__pos_rename_at_471_145;
            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_470_143),__pos_rename_at_471_145);
        };
    };
}

inline void mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_528_147, int32_t __i_rename_at_528_148 )
{
    das_source_formatter::ParenCounter __pc_rename_at_529_149; das_zero(__pc_rename_at_529_149);
    {
        bool __need_loop_530 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__i_rename_at_528_148,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_528_147.tokens))));
        int32_t __j_rename_at_530_150;
        __need_loop_530 = __j_iterator.first(__context__,(__j_rename_at_530_150)) && __need_loop_530;
        for ( ; __need_loop_530 ; __need_loop_530 = __j_iterator.next(__context__,(__j_rename_at_530_150)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_528_147),__j_rename_at_530_150,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_529_149));
            if ( __ctx_rename_at_528_147.tokens(__j_rename_at_530_150,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT )
            {
                das_copy(__ctx_rename_at_528_147.tokens(__j_rename_at_530_150,__context__).isInType,true);
            };
            if ( __pc_rename_at_529_149.angle <= 0 )
            {
                break;
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_530_150));
    };
}

inline int32_t search_token_in_line_3a9d4fd472c436b3 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_542_151, int32_t __from_index_rename_at_542_152, char * const  __str_rename_at_542_153 )
{
    das_source_formatter::ParenCounter __pc_rename_at_543_154; das_zero(__pc_rename_at_543_154);
    {
        bool __need_loop_544 = true;
        // j: int const
        das_iterator<range> __j_iterator(range(__from_index_rename_at_542_152,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_542_151.tokens))));
        int32_t __j_rename_at_544_155;
        __need_loop_544 = __j_iterator.first(__context__,(__j_rename_at_544_155)) && __need_loop_544;
        for ( ; __need_loop_544 ; __need_loop_544 = __j_iterator.next(__context__,(__j_rename_at_544_155)) )
        {
            update_paren_62e64d469829ae3d(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_151),__j_rename_at_544_155,das_arg<das_source_formatter::ParenCounter>::pass(__pc_rename_at_543_154));
            if ( (((__j_rename_at_544_155 > __from_index_rename_at_542_152) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_151),__j_rename_at_544_155)) && (((__pc_rename_at_543_154.paren <= 0) || (__pc_rename_at_543_154.square <= 0)) || (__pc_rename_at_543_154.curly <= 0))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_151),__j_rename_at_544_155 - 1,((char *) ","))) )
            {
                break;
            } else {
                if ( ((__pc_rename_at_543_154.paren < 0) || (__pc_rename_at_543_154.square < 0)) || (__pc_rename_at_543_154.curly < 0) )
                {
                    break;
                } else {
                    if ( ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_151),__j_rename_at_544_155,((char *) ";")) && (__pc_rename_at_543_154.paren == 0)) && (__pc_rename_at_543_154.square == 0)) && (__pc_rename_at_543_154.curly == 0) )
                    {
                        break;
                    } else {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_542_151),__j_rename_at_544_155,__str_rename_at_542_153) )
                        {
                            return das_auto_cast<int32_t>::cast(__j_rename_at_544_155);
                        };
                    };
                };
            };
        }
        __j_iterator.close(__context__,(__j_rename_at_544_155));
    };
    return das_auto_cast<int32_t>::cast(0);
}

inline void process_formatter_tokens_a63e2ffa4c8206b6 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_564_156 )
{
    {
        bool __need_loop_567 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_564_156.tokens))));
        int32_t __i_rename_at_567_157;
        __need_loop_567 = __i_iterator.first(__context__,(__i_rename_at_567_157)) && __need_loop_567;
        for ( ; __need_loop_567 ; __need_loop_567 = __i_iterator.next(__context__,(__i_rename_at_567_157)) )
        {
            das_source_formatter::Token __token_rename_at_568_158_ConstRef = ((das_source_formatter::Token)__ctx_rename_at_564_156.tokens(__i_rename_at_567_157,__context__));
    das_source_formatter::Token const  & __token_rename_at_568_158 = __token_rename_at_568_158_ConstRef; ;
            char * __str_rename_at_569_159 = ((char *)(char *)(__token_rename_at_568_158.str));
            int32_t __strSize_rename_at_570_160 = ((int32_t)builtin_string_length(__str_rename_at_569_159,__context__));
            if ( builtin_string_startswith(__str_rename_at_569_159,((char *) "//fmt:"),__context__) )
            {
                das_source_formatter::FormatterToken _temp_make_local_573_40_128; _temp_make_local_573_40_128;
                char * __command_rename_at_572_161 = ((char *)(char *)(((char * const )(builtin_string_chop(__str_rename_at_569_159,6,__strSize_rename_at_570_160 - 6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))));
                _FuncbuiltinTickpushTick10769833213962245646_a661f9640a1075ce(__context__,das_arg<TArray<das_source_formatter::FormatterToken>>::pass(__ctx_rename_at_564_156.formatterTokens),das_arg<das_source_formatter::FormatterToken>::pass((([&]() -> das_source_formatter::FormatterToken& {
                    das_copy((_temp_make_local_573_40_128.tokenIndex),(__i_rename_at_567_157));
                    das_copy((_temp_make_local_573_40_128.command),(__command_rename_at_572_161));
                    return _temp_make_local_573_40_128;
                })())));
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_567_157));
    };
}

inline bool have_formatter_token_296d7fc158126671 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_581_162, char * const  __command_rename_at_581_163 )
{
    {
        bool __need_loop_582 = true;
        // formatterToken: das_source_formatter::FormatterToken const&
        das_iterator<TArray<das_source_formatter::FormatterToken> const > __formatterToken_iterator(__ctx_rename_at_581_162.formatterTokens);
        das_source_formatter::FormatterToken const  * __formatterToken_rename_at_582_164;
        __need_loop_582 = __formatterToken_iterator.first(__context__,(__formatterToken_rename_at_582_164)) && __need_loop_582;
        for ( ; __need_loop_582 ; __need_loop_582 = __formatterToken_iterator.next(__context__,(__formatterToken_rename_at_582_164)) )
        {
            if ( SimPolicy<char *>::Equ(cast<char *>::from((*__formatterToken_rename_at_582_164).command),cast<char *>::from(__command_rename_at_581_163),*__context__,nullptr) )
            {
                return das_auto_cast<bool>::cast(true);
            };
        }
        __formatterToken_iterator.close(__context__,(__formatterToken_rename_at_582_164));
    };
    return das_auto_cast<bool>::cast(false);
}

inline void mark_token_context_7dbf70435b518a0a ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_591_165 )
{
    TDim<char *,8> _temp_make_local_782_24_576; _temp_make_local_782_24_576;
    {
        bool __need_loop_593 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 1));
        int32_t __i_rename_at_593_166;
        __need_loop_593 = __i_iterator.first(__context__,(__i_rename_at_593_166)) && __need_loop_593;
        for ( ; __need_loop_593 ; __need_loop_593 = __i_iterator.next(__context__,(__i_rename_at_593_166)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_593_166) && (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_593_166,((char *) "require")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_593_166,((char *) "expect"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_593_166,((char *) "label"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_593_166,((char *) "include"))) )
            {
                {
                    bool __need_loop_595 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_593_166 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens))));
                    int32_t __j_rename_at_595_167;
                    __need_loop_595 = __j_iterator.first(__context__,(__j_rename_at_595_167)) && __need_loop_595;
                    for ( ; __need_loop_595 ; __need_loop_595 = __j_iterator.next(__context__,(__j_rename_at_595_167)) )
                    {
                        if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_595_167) )
                        {
                            break;
                        } else {
                            das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_595_167,__context__).dontFormat,true);
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_595_167));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_593_166));
    };
    {
        bool __need_loop_605 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 4));
        int32_t __i_rename_at_605_168;
        __need_loop_605 = __i_iterator.first(__context__,(__i_rename_at_605_168)) && __need_loop_605;
        for ( ; __need_loop_605 ; __need_loop_605 = __i_iterator.next(__context__,(__i_rename_at_605_168)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_605_168,((char *) "%")) && (__ctx_rename_at_591_165.tokens((__i_rename_at_605_168 + 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_605_168 + 2,((char *) "~")) )
            {
                {
                    bool __need_loop_607 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_605_168,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens))));
                    int32_t __j_rename_at_607_169;
                    __need_loop_607 = __j_iterator.first(__context__,(__j_rename_at_607_169)) && __need_loop_607;
                    for ( ; __need_loop_607 ; __need_loop_607 = __j_iterator.next(__context__,(__j_rename_at_607_169)) )
                    {
                        das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_607_169,__context__).dontFormat,true);
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_607_169,((char *) "%%")) )
                        {
                            break;
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_607_169));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_605_168));
    };
    {
        bool __need_loop_617 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_617_170;
        __need_loop_617 = __i_iterator.first(__context__,(__i_rename_at_617_170)) && __need_loop_617;
        for ( ; __need_loop_617 ; __need_loop_617 = __i_iterator.next(__context__,(__i_rename_at_617_170)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) "let")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) "var"))) )
            {
                int32_t __assignPos0_rename_at_619_171 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) "=")));
                int32_t __assignPos1_rename_at_620_172 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) ":=")));
                int32_t __assignPos2_rename_at_621_173 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) "<-")));
                int32_t __colonPos_rename_at_622_174 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_617_170,((char *) ":")));
                if ( (((__colonPos_rename_at_622_174 > 0) && ((__assignPos0_rename_at_619_171 > __colonPos_rename_at_622_174) || (__assignPos0_rename_at_619_171 <= 0))) && ((__assignPos1_rename_at_620_172 > __colonPos_rename_at_622_174) || (__assignPos1_rename_at_620_172 <= 0))) && ((__assignPos2_rename_at_621_173 > __colonPos_rename_at_622_174) || (__assignPos2_rename_at_621_173 <= 0)) )
                {
                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__colonPos_rename_at_622_174 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_617_170));
    };
    {
        bool __need_loop_633 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_633_175;
        __need_loop_633 = __i_iterator.first(__context__,(__i_rename_at_633_175)) && __need_loop_633;
        for ( ; __need_loop_633 ; __need_loop_633 = __i_iterator.next(__context__,(__i_rename_at_633_175)) )
        {
            if ( __ctx_rename_at_591_165.tokens(__i_rename_at_633_175,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER )
            {
                if ( (_FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_591_165.tokens(__i_rename_at_633_175,__context__).str) >= 0) || ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_633_175,((char *) "new")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_633_175,((char *) "struct"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_633_175 + 1,((char *) "<"))) )
                {
                    mark_tokens_as_type_inside_angle_53305ce8d8ddd8cb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_633_175 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_633_175));
    };
    {
        bool __need_loop_644 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_644_176;
        __need_loop_644 = __i_iterator.first(__context__,(__i_rename_at_644_176)) && __need_loop_644;
        for ( ; __need_loop_644 ; __need_loop_644 = __i_iterator.next(__context__,(__i_rename_at_644_176)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_644_176,((char *) "$")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_644_176 + 1,((char *) "t"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_644_176 + 2,((char *) "(")) )
            {
                int32_t __endParams_rename_at_646_177 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_644_176 + 2));
                {
                    bool __need_loop_647 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_644_176,__endParams_rename_at_646_177 + 1));
                    int32_t __j_rename_at_647_178;
                    __need_loop_647 = __j_iterator.first(__context__,(__j_rename_at_647_178)) && __need_loop_647;
                    for ( ; __need_loop_647 ; __need_loop_647 = __j_iterator.next(__context__,(__j_rename_at_647_178)) )
                    {
                        das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_647_178,__context__).isInType,true);
                    }
                    __j_iterator.close(__context__,(__j_rename_at_647_178));
                };
                {
                    bool __need_loop_650 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__endParams_rename_at_646_177 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens))));
                    int32_t __j_rename_at_650_179;
                    __need_loop_650 = __j_iterator.first(__context__,(__j_rename_at_650_179)) && __need_loop_650;
                    for ( ; __need_loop_650 ; __need_loop_650 = __j_iterator.next(__context__,(__j_rename_at_650_179)) )
                    {
                        if ( !(eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_650_179,((char *) "-"),((char *) "#"),((char *) "&"),((char *) "?")) || eq_f43d812e3a992e4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_650_179,((char *) "=="),((char *) "const"),((char *) "implicit"))) )
                        {
                            break;
                        } else {
                            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_650_179) )
                            {
                                break;
                            } else {
                                das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_650_179,__context__).isInType,true);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_650_179));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_644_176));
    };
    {
        bool __need_loop_663 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_663_180;
        __need_loop_663 = __i_iterator.first(__context__,(__i_rename_at_663_180)) && __need_loop_663;
        for ( ; __need_loop_663 ; __need_loop_663 = __i_iterator.next(__context__,(__i_rename_at_663_180)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_663_180,((char *) "[")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_663_180 + 1,((char *) "["),((char *) "{"))) || (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_663_180,((char *) "{")) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_663_180 + 1,((char *) "{"))) )
            {
                mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_663_180 + 2);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_663_180));
    };
    {
        bool __need_loop_670 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_670_181;
        __need_loop_670 = __i_iterator.first(__context__,(__i_rename_at_670_181)) && __need_loop_670;
        for ( ; __need_loop_670 ; __need_loop_670 = __i_iterator.next(__context__,(__i_rename_at_670_181)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181) )
            {
                int32_t __skip_rename_at_672_182 = 0;
                if ( ((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "struct")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "class"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "variant"))) || (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "typedef")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "var"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "let"))) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181 + 1)) )
                {
                    das_copy(__skip_rename_at_672_182,1);
                };
                if ( (((eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "typedef")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "var"))) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "let"))) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181 + 1,((char *) "private")) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181 + 1,((char *) "public")))) && new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181 + 2) )
                {
                    das_copy(__skip_rename_at_672_182,2);
                };
                if ( __skip_rename_at_672_182 == 0 )
                {
                    continue;
                } else {
                    int32_t __ident_rename_at_687_183 = ((int32_t)__ctx_rename_at_591_165.tokens(__i_rename_at_670_181,__context__).spaces);
                    {
                        bool __need_loop_688 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__i_rename_at_670_181 + __skip_rename_at_672_182,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
                        int32_t __j_rename_at_688_184;
                        __need_loop_688 = __j_iterator.first(__context__,(__j_rename_at_688_184)) && __need_loop_688;
                        for ( ; __need_loop_688 ; __need_loop_688 = __j_iterator.next(__context__,(__j_rename_at_688_184)) )
                        {
                            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_688_184) )
                            {
                                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_688_184,((char *) "def")) )
                                {
                                    break;
                                } else {
                                    if ( __ctx_rename_at_591_165.tokens(__j_rename_at_688_184,__context__).spaces <= __ident_rename_at_687_183 )
                                    {
                                        break;
                                    } else {
                                        int32_t __colonPos_rename_at_696_185 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_688_184,eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_670_181,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                                        if ( __colonPos_rename_at_696_185 > 0 )
                                        {
                                            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__colonPos_rename_at_696_185 + 1);
                                        };
                                    };
                                };
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_688_184));
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_670_181));
    };
    {
        bool __need_loop_706 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_706_186;
        __need_loop_706 = __i_iterator.first(__context__,(__i_rename_at_706_186)) && __need_loop_706;
        for ( ; __need_loop_706 ; __need_loop_706 = __i_iterator.next(__context__,(__i_rename_at_706_186)) )
        {
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_706_186) && (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_706_186,((char *) "typedef")) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_706_186 + 1))) )
            {
                {
                    bool __need_loop_708 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_706_186 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
                    int32_t __j_rename_at_708_187;
                    __need_loop_708 = __j_iterator.first(__context__,(__j_rename_at_708_187)) && __need_loop_708;
                    for ( ; __need_loop_708 ; __need_loop_708 = __j_iterator.next(__context__,(__j_rename_at_708_187)) )
                    {
                        if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_708_187) )
                        {
                            break;
                        } else {
                            int32_t __colonPos_rename_at_712_188 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_708_187,eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_706_186,((char *) "typedef")) ? das_auto_cast<char * const >::cast(((char *) "=")) : das_auto_cast<char * const >::cast(((char *) ":"))));
                            if ( __colonPos_rename_at_712_188 > 0 )
                            {
                                mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__colonPos_rename_at_712_188 + 1);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_708_187));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_706_186));
    };
    {
        bool __need_loop_721 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 3));
        int32_t __i_rename_at_721_189;
        __need_loop_721 = __i_iterator.first(__context__,(__i_rename_at_721_189)) && __need_loop_721;
        for ( ; __need_loop_721 ; __need_loop_721 = __i_iterator.next(__context__,(__i_rename_at_721_189)) )
        {
            int32_t __pos_rename_at_722_190 = -1;
            if ( new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_721_189) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_721_189,((char *) "def")) )
            {
                das_copy(__pos_rename_at_722_190,__i_rename_at_721_189 + 1);
            } else if ( eq_f43d812e3a992e4(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_721_189,((char *) "$"),((char *) "@"),((char *) "@@")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_721_189 + 1,((char *) "("),((char *) "[")) )
            {
                das_copy(__pos_rename_at_722_190,__i_rename_at_721_189);
            };
            if ( __pos_rename_at_722_190 == -1 )
            {
                continue;
            } else {
                while ( eq_886fe8d1ff63f5fd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190,((char *) "private"),((char *) "public"),((char *) "const"),((char *) "implicit")) || eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190,((char *) "abstract"),((char *) "override")) )
                {
                    ++__pos_rename_at_722_190;
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190,((char *) "operator")) )
                {
                    das_copy(__pos_rename_at_722_190,search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190,((char *) "(")) - 1);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190 + 1,((char *) "[")) )
                {
                    das_copy(__pos_rename_at_722_190,find_pair_square_68d9ea1d7c30e49f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190 + 1));
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190 + 1,((char *) ":")) )
                {
                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_721_189 + 2);
                } else if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190 + 1,((char *) "(")) )
                {
                    int32_t __beginParams_rename_at_748_191 = ((int32_t)(__pos_rename_at_722_190 + 1));
                    das_copy(__ctx_rename_at_591_165.tokens(__beginParams_rename_at_748_191,__context__).dontAddSpacesAround,true);
                    int32_t __endParams_rename_at_750_192 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__pos_rename_at_722_190 + 1));
                    {
                        bool __need_loop_751 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__beginParams_rename_at_748_191,__endParams_rename_at_750_192 + 1));
                        int32_t __j_rename_at_751_193;
                        __need_loop_751 = __j_iterator.first(__context__,(__j_rename_at_751_193)) && __need_loop_751;
                        for ( ; __need_loop_751 ; __need_loop_751 = __j_iterator.next(__context__,(__j_rename_at_751_193)) )
                        {
                            das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_751_193,__context__).isInFunctionParam,true);
                            if ( ((__j_rename_at_751_193 - 1) == __beginParams_rename_at_748_191) || eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_751_193 - 1,((char *) ";")) )
                            {
                                int32_t __colonPos_rename_at_754_194 = ((int32_t)search_token_in_line_3a9d4fd472c436b3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_751_193,((char *) ":")));
                                if ( __colonPos_rename_at_754_194 > 0 )
                                {
                                    mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__colonPos_rename_at_754_194 + 1);
                                };
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_751_193));
                    };
                    if ( __endParams_rename_at_750_192 < (builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 1) )
                    {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__endParams_rename_at_750_192 + 1,((char *) ":")) )
                        {
                            mark_tokens_as_type_5e4d056374f6927c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__endParams_rename_at_750_192 + 2);
                        };
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_721_189));
    };
    {
        bool __need_loop_769 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens)) - 1));
        int32_t __i_rename_at_769_195;
        __need_loop_769 = __i_iterator.first(__context__,(__i_rename_at_769_195)) && __need_loop_769;
        for ( ; __need_loop_769 ; __need_loop_769 = __i_iterator.next(__context__,(__i_rename_at_769_195)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_769_195,((char *) "[")) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_769_195 + 1,((char *) "[")))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_769_195 + 1,((char *) "{"))) )
            {
                {
                    bool __need_loop_771 = true;
                    // j: int const
                    das_iterator<range> __j_iterator(range(__i_rename_at_769_195 + 1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens))));
                    int32_t __j_rename_at_771_196;
                    __need_loop_771 = __j_iterator.first(__context__,(__j_rename_at_771_196)) && __need_loop_771;
                    for ( ; __need_loop_771 ; __need_loop_771 = __j_iterator.next(__context__,(__j_rename_at_771_196)) )
                    {
                        if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_771_196,((char *) "]")) )
                        {
                            break;
                        } else {
                            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__j_rename_at_771_196,((char *) "=")) )
                            {
                                das_copy(__ctx_rename_at_591_165.tokens(__j_rename_at_771_196,__context__).dontAddSpacesAround,true);
                            };
                        };
                    }
                    __j_iterator.close(__context__,(__j_rename_at_771_196));
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_769_195));
    };
    TTable<char *,void> __macroEscapes_rename_at_782_197_ConstRef; das_zero(__macroEscapes_rename_at_782_197_ConstRef); das_move(__macroEscapes_rename_at_782_197_ConstRef, ((TTable<char *,void>)_FuncbuiltinTickto_table_moveTick3386430563931768014_5f654e62afe15768(__context__,das_arg<TDim<char *,8>>::pass((([&]() -> TDim<char *,8>& {
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
    TTable<char *,void> const  & __macroEscapes_rename_at_782_197 = __macroEscapes_rename_at_782_197_ConstRef; ;
    {
        bool __need_loop_783 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_591_165.tokens))));
        int32_t __i_rename_at_783_198;
        __need_loop_783 = __i_iterator.first(__context__,(__i_rename_at_783_198)) && __need_loop_783;
        for ( ; __need_loop_783 ; __need_loop_783 = __i_iterator.next(__context__,(__i_rename_at_783_198)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_783_198)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_591_165),__i_rename_at_783_198 - 1,((char *) "$"))) && _FuncbuiltinTickkey_existsTick16808803843923989214_7d01ceb11c7f2e53(__context__,__macroEscapes_rename_at_782_197,__ctx_rename_at_591_165.tokens(__i_rename_at_783_198,__context__).str) )
            {
                das_copy(__ctx_rename_at_591_165.tokens((__i_rename_at_783_198 - 1),__context__).dontAddSpacesAround,true);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_783_198));
    };
}

inline void fmt_function_arguments_7d9f22d6302573cb ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_791_199 )
{
    int32_t __lastNewLineToken_rename_at_793_200 = 0;
    int32_t __i_rename_at_794_201 = 0;
    int32_t __n_rename_at_795_202 = ((int32_t)(builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_791_199.tokens)) - 3));
    while ( __i_rename_at_794_201 < __n_rename_at_795_202 )
    {
        if ( __ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).newLines > 0 )
        {
            das_copy(__lastNewLineToken_rename_at_793_200,__i_rename_at_794_201);
        };
        if ( __ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).isInFunctionParam )
        {
            int32_t __firstBracketColumn_rename_at_802_203 = -1;
            while ( (__i_rename_at_794_201 < __n_rename_at_795_202) && __ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).isInFunctionParam )
            {
                if ( __ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).newLines > 0 )
                {
                    das_copy(__lastNewLineToken_rename_at_793_200,__i_rename_at_794_201);
                };
                if ( __firstBracketColumn_rename_at_802_203 == -1 )
                {
                    if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_791_199),__i_rename_at_794_201,((char *) "(")) )
                    {
                        das_copy(__firstBracketColumn_rename_at_802_203,0);
                        {
                            bool __need_loop_811 = true;
                            // k: int const
                            das_iterator<range> __k_iterator(range(__lastNewLineToken_rename_at_793_200,__i_rename_at_794_201 + 1));
                            int32_t __k_rename_at_811_204;
                            __need_loop_811 = __k_iterator.first(__context__,(__k_rename_at_811_204)) && __need_loop_811;
                            for ( ; __need_loop_811 ; __need_loop_811 = __k_iterator.next(__context__,(__k_rename_at_811_204)) )
                            {
                                if ( __ctx_rename_at_791_199.tokens(__k_rename_at_811_204,__context__).newLines > 0 )
                                {
                                    das_copy(__firstBracketColumn_rename_at_802_203,0);
                                };
                                __firstBracketColumn_rename_at_802_203 += (__ctx_rename_at_791_199.tokens(__k_rename_at_811_204,__context__).spaces + builtin_string_length(__ctx_rename_at_791_199.tokens(__k_rename_at_811_204,__context__).str,__context__));
                            }
                            __k_iterator.close(__context__,(__k_rename_at_811_204));
                        };
                    };
                } else if ( __ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).newLines > 0 )
                {
                    das_copy(__lastNewLineToken_rename_at_793_200,__i_rename_at_794_201);
                    das_copy(__ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).spaces,__firstBracketColumn_rename_at_802_203 + (__ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).isInType ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(0)));
                    das_copy(__ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).column,__firstBracketColumn_rename_at_802_203 + (__ctx_rename_at_791_199.tokens(__i_rename_at_794_201,__context__).isInType ? das_auto_cast<int32_t>::cast(2) : das_auto_cast<int32_t>::cast(0)));
                };
                ++__i_rename_at_794_201;
            };
        };
        ++__i_rename_at_794_201;
    };
}

inline void fmt_remove_space_before_args_26ba700ce1a53dbc ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_832_205 )
{
    {
        bool __need_loop_833 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_832_205.tokens)) - 1));
        int32_t __i_rename_at_833_206;
        __need_loop_833 = __i_iterator.first(__context__,(__i_rename_at_833_206)) && __need_loop_833;
        for ( ; __need_loop_833 ; __need_loop_833 = __i_iterator.next(__context__,(__i_rename_at_833_206)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_205),__i_rename_at_833_206,((char *) "(")) && !(__ctx_rename_at_832_205.tokens(__i_rename_at_833_206,__context__).isInType)) && __ctx_rename_at_832_205.tokens((__i_rename_at_833_206 - 1),__context__).isInType )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_832_205),__i_rename_at_833_206);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_833_206));
    };
}

inline void debug_print_tokens_927617e00a5e50a7 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_841_207 )
{
    {
        bool __need_loop_842 = true;
        // t: das_source_formatter::Token const&
        das_iterator<TArray<das_source_formatter::Token> const > __t_iterator(__ctx_rename_at_841_207.tokens);
        das_source_formatter::Token const  * __t_rename_at_842_208;
        __need_loop_842 = __t_iterator.first(__context__,(__t_rename_at_842_208)) && __need_loop_842;
        for ( ; __need_loop_842 ; __need_loop_842 = __t_iterator.next(__context__,(__t_rename_at_842_208)) )
        {
            toLog(10000,das_string_builder_temp(__context__,SimNode_AotInterop<5>(__tinfo_6, cast<char * const >::from(((*__t_rename_at_842_208).isInFunctionParam ? das_auto_cast<char * const >::cast(((char *) ">")) : das_auto_cast<char * const >::cast(((char *) " ")))), cast<char * const >::from(((*__t_rename_at_842_208).isInType ? das_auto_cast<char * const >::cast(((char *) "*")) : das_auto_cast<char * const >::cast(((char *) " ")))), cast<char * const >::from(((char *) " '")), cast<char * const >::from((*__t_rename_at_842_208).str), cast<char * const >::from(((char *) "'\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        }
        __t_iterator.close(__context__,(__t_rename_at_842_208));
    };
}

inline char * generate_source_64706a1ce7db4983 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_848_209 )
{
    TArray<uint8_t> __res_rename_at_849_210; memset(&__res_rename_at_849_210,0,sizeof(__res_rename_at_849_210));
    bool __useCrLf_rename_at_857_211; memset(&__useCrLf_rename_at_857_211,0,sizeof(__useCrLf_rename_at_857_211));
    int32_t __column_rename_at_858_212; memset(&__column_rename_at_858_212,0,sizeof(__column_rename_at_858_212));
    int32_t __spaces_rename_at_869_215; memset(&__spaces_rename_at_869_215,0,sizeof(__spaces_rename_at_869_215));
    /* finally */ auto __finally_848= das_finally([&](){
    _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210));
    /* end finally */ });
    __res_rename_at_849_210; das_zero(__res_rename_at_849_210);
    _FuncbuiltinTickreserveTick3994685146752941225_54cb508dc62508e6(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),(builtin_array_size(__ctx_rename_at_848_209.tokens) * 8) + 1000);
    if ( __ctx_rename_at_848_209.haveUtf8Bom )
    {
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0xef);
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0xbb);
        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0xbf);
    };
    __useCrLf_rename_at_857_211 = ((bool)((__ctx_rename_at_848_209.crCount * 2) > __ctx_rename_at_848_209.lfCount));
    __column_rename_at_858_212 = 0;
    {
        bool __need_loop_860 = true;
        // t: das_source_formatter::Token const&
        das_iterator<TArray<das_source_formatter::Token> const > __t_iterator(__ctx_rename_at_848_209.tokens);
        das_source_formatter::Token const  * __t_rename_at_860_213;
        __need_loop_860 = __t_iterator.first(__context__,(__t_rename_at_860_213)) && __need_loop_860;
        for ( ; __need_loop_860 ; __need_loop_860 = __t_iterator.next(__context__,(__t_rename_at_860_213)) )
        {
            {
                bool __need_loop_861 = true;
                // _: int const
                das_iterator<range> ____iterator(mk_range((*__t_rename_at_860_213).newLines));
                int32_t ____rename_at_861_214;
                __need_loop_861 = ____iterator.first(__context__,(____rename_at_861_214)) && __need_loop_861;
                for ( ; __need_loop_861 ; __need_loop_861 = ____iterator.next(__context__,(____rename_at_861_214)) )
                {
                    if ( __useCrLf_rename_at_857_211 )
                    {
                        _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0xd);
                    };
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0xa);
                    das_copy(__column_rename_at_858_212,0);
                }
                ____iterator.close(__context__,(____rename_at_861_214));
            };
            __spaces_rename_at_869_215 = (*__t_rename_at_860_213).spaces;
            if ( ((*__t_rename_at_860_213).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) && builtin_string_startswith((*__t_rename_at_860_213).str,((char *) "//"),__context__) )
            {
                das_copy(__spaces_rename_at_869_215,SimPolicy<int32_t>::Max((*__t_rename_at_860_213).column - __column_rename_at_858_212,0,*__context__,nullptr));
            };
            {
                bool __need_loop_874 = true;
                // _: int const
                das_iterator<range> ____iterator(mk_range(__spaces_rename_at_869_215));
                int32_t ____rename_at_874_216;
                __need_loop_874 = ____iterator.first(__context__,(____rename_at_874_216)) && __need_loop_874;
                for ( ; __need_loop_874 ; __need_loop_874 = ____iterator.next(__context__,(____rename_at_874_216)) )
                {
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),0x20);
                    ++__column_rename_at_858_212;
                }
                ____iterator.close(__context__,(____rename_at_874_216));
            };
            {
                bool __need_loop_879 = true;
                // c: int const
                das_iterator<char * const > __c_iterator((*__t_rename_at_860_213).str);
                int32_t __c_rename_at_879_217;
                __need_loop_879 = __c_iterator.first(__context__,(__c_rename_at_879_217)) && __need_loop_879;
                for ( ; __need_loop_879 ; __need_loop_879 = __c_iterator.next(__context__,(__c_rename_at_879_217)) )
                {
                    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),uint8_t(__c_rename_at_879_217));
                    ++__column_rename_at_858_212;
                }
                __c_iterator.close(__context__,(__c_rename_at_879_217));
            };
        }
        __t_iterator.close(__context__,(__t_rename_at_860_213));
    };
    return das_auto_cast<char *>::cast(((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__res_rename_at_849_210),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
}

inline bool need_spaces_around_7179a668ae90ccd6 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_891_218, int32_t __index_rename_at_891_219 )
{
    if ( ((__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).dontFormat) || __ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).dontAddSpacesAround )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str) >= 0 )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( __ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).isInType && ((((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) ":")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) "<")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || builtin_empty(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str)) )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( __index_rename_at_891_219 < (builtin_array_size(__ctx_rename_at_891_218.tokens) - 1) )
                {
                    if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_891_218.tokens(__index_rename_at_891_219,__context__).str),cast<char *>::from(((char *) "@")),*__context__,nullptr)) && (__ctx_rename_at_891_218.tokens((__index_rename_at_891_219 + 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
                    {
                        return das_auto_cast<bool>::cast(false);
                    };
                };
                return das_auto_cast<bool>::cast(true);
            };
        };
    };
}

inline bool need_space_only_before_584e48acb8e8a70 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_912_220, int32_t __index_rename_at_912_221 )
{
    if ( !__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).isInType )
    {
        return das_auto_cast<bool>::cast(false);
    } else {
        if ( ((__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) || __ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).dontFormat) || __ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).dontAddSpacesAround )
        {
            return das_auto_cast<bool>::cast(false);
        } else {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0x9d441a57052b5c37>(__context__) /*dont_need_space_around*/,__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str) >= 0 )
            {
                return das_auto_cast<bool>::cast(false);
            } else {
                if ( ((((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str),cast<char *>::from(((char *) "<")),*__context__,nullptr))) || builtin_empty(__ctx_rename_at_912_220.tokens(__index_rename_at_912_221,__context__).str) )
                {
                    return das_auto_cast<bool>::cast(false);
                } else {
                    if ( __index_rename_at_912_221 < (builtin_array_size(__ctx_rename_at_912_220.tokens) - 1) )
                    {
                        if ( (((SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens((__index_rename_at_912_221 + 1),__context__).str),cast<char *>::from(((char *) ">")),*__context__,nullptr)) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens((__index_rename_at_912_221 + 1),__context__).str),cast<char *>::from(((char *) ">>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens((__index_rename_at_912_221 + 1),__context__).str),cast<char *>::from(((char *) ">>>")),*__context__,nullptr))) || (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_912_220.tokens((__index_rename_at_912_221 + 1),__context__).str),cast<char *>::from(((char *) ";")),*__context__,nullptr)) )
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

inline bool is_unary_62ad92645c7f9ee6 ( Context * __context__, das_source_formatter::FormatterCtx const  &  __ctx_rename_at_956_222, int32_t __index_rename_at_956_223 )
{
    if ( eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "!")) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "~")) )
    {
        return das_auto_cast<bool>::cast(true);
    } else {
        if ( (eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "*")) && (__index_rename_at_956_223 > 0)) && __ctx_rename_at_956_222.tokens((__index_rename_at_956_223 - 1),__context__).isInType )
        {
            return das_auto_cast<bool>::cast(true);
        } else {
            if ( (eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "+")) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "-"))) || eq_1a08ceae13f47e89(__context__,__ctx_rename_at_956_222,__index_rename_at_956_223,((char *) "*")) )
            {
                int32_t __i_rename_at_966_224 = __index_rename_at_956_223;
                while ( (((__i_rename_at_966_224 - 1) >= 0) && (__ctx_rename_at_956_222.tokens((__i_rename_at_966_224 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT)) && !(new_line_before_56d9a06feda36687(__context__,__ctx_rename_at_956_222,__i_rename_at_966_224)) )
                {
                    --__i_rename_at_966_224;
                };
                return das_auto_cast<bool>::cast(((_FuncbuiltinTickfind_indexTick6703785223819426183_5eb3be401f8b11e3(__context__,das_global<TDim<char *,21>,0xd52f4fdee217c055>(__context__) /*tokens_before_unary*/,__ctx_rename_at_956_222.tokens((__i_rename_at_966_224 - 1),__context__).str) >= 0) || new_line_before_56d9a06feda36687(__context__,__ctx_rename_at_956_222,__i_rename_at_966_224)) ? das_auto_cast<bool>::cast(true) : das_auto_cast<bool>::cast(need_spaces_around_7179a668ae90ccd6(__context__,__ctx_rename_at_956_222,__i_rename_at_966_224 - 1)));
            } else {
                return das_auto_cast<bool>::cast(false);
            };
        };
    };
}

inline void remove_spaces_around_1c96d2bcc36445d1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_979_225, int32_t __index_rename_at_979_226 )
{
    if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_225),__index_rename_at_979_226) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_225),__index_rename_at_979_226);
    };
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_225),__index_rename_at_979_226 + 1)) && (__ctx_rename_at_979_225.tokens((__index_rename_at_979_226 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
    {
        _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_979_225),__index_rename_at_979_226 + 1);
    };
}

inline void make_spaces_around_9b0360b9b4d718c3 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_989_227, int32_t __index_rename_at_989_228 )
{
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_227),__index_rename_at_989_228)) && (__ctx_rename_at_989_227.tokens(__index_rename_at_989_228,__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_227),__index_rename_at_989_228);
    };
    if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_227),__index_rename_at_989_228 + 1)) && (__ctx_rename_at_989_227.tokens((__index_rename_at_989_228 + 1),__context__).spaces == 0) )
    {
        _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_989_227),__index_rename_at_989_228 + 1);
    };
}

inline void fmt_space_after_comma_7aa6a2faf3ce3f4f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_999_229 )
{
    {
        bool __need_loop_1000 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_999_229.tokens))));
        int32_t __i_rename_at_1000_230;
        __need_loop_1000 = __i_iterator.first(__context__,(__i_rename_at_1000_230)) && __need_loop_1000;
        for ( ; __need_loop_1000 ; __need_loop_1000 = __i_iterator.next(__context__,(__i_rename_at_1000_230)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_229),__i_rename_at_1000_230)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_229),__i_rename_at_1000_230 - 1,((char *) ","))) && (__ctx_rename_at_999_229.tokens(__i_rename_at_1000_230,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_999_229),__i_rename_at_1000_230);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1000_230));
    };
}

inline void fmt_space_after_semicolon_29caf5fd09f6497e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1008_231 )
{
    {
        bool __need_loop_1009 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1008_231.tokens))));
        int32_t __i_rename_at_1009_232;
        __need_loop_1009 = __i_iterator.first(__context__,(__i_rename_at_1009_232)) && __need_loop_1009;
        for ( ; __need_loop_1009 ; __need_loop_1009 = __i_iterator.next(__context__,(__i_rename_at_1009_232)) )
        {
            if ( ((!(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_231),__i_rename_at_1009_232,nullptr)) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_231),__i_rename_at_1009_232))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_231),__i_rename_at_1009_232 - 1,((char *) ";"))) && (__ctx_rename_at_1008_231.tokens(__i_rename_at_1009_232,__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1008_231),__i_rename_at_1009_232);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1009_232));
    };
}

inline void fmt_remove_space_before_comma_54b9cd4ba70794e ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1017_233 )
{
    {
        bool __need_loop_1018 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1017_233.tokens))));
        int32_t __i_rename_at_1018_234;
        __need_loop_1018 = __i_iterator.first(__context__,(__i_rename_at_1018_234)) && __need_loop_1018;
        for ( ; __need_loop_1018 ; __need_loop_1018 = __i_iterator.next(__context__,(__i_rename_at_1018_234)) )
        {
            if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_233),__i_rename_at_1018_234)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_233),__i_rename_at_1018_234,((char *) ",")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1017_233),__i_rename_at_1018_234);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1018_234));
    };
}

inline void fmt_remove_space_before_semicolon_132b81d1431b5d49 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1026_235 )
{
    {
        bool __need_loop_1027 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1026_235.tokens))));
        int32_t __i_rename_at_1027_236;
        __need_loop_1027 = __i_iterator.first(__context__,(__i_rename_at_1027_236)) && __need_loop_1027;
        for ( ; __need_loop_1027 ; __need_loop_1027 = __i_iterator.next(__context__,(__i_rename_at_1027_236)) )
        {
            if ( !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_235),__i_rename_at_1027_236)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_235),__i_rename_at_1027_236,((char *) ";")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1026_235),__i_rename_at_1027_236);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1027_236));
    };
}

inline void fmt_type_colon_a5b252566f4d584 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1035_237 )
{
    {
        bool __need_loop_1036 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1035_237.tokens)) - 1));
        int32_t __i_rename_at_1036_238;
        __need_loop_1036 = __i_iterator.first(__context__,(__i_rename_at_1036_238)) && __need_loop_1036;
        for ( ; __need_loop_1036 ; __need_loop_1036 = __i_iterator.next(__context__,(__i_rename_at_1036_238)) )
        {
            if ( __ctx_rename_at_1035_237.tokens(__i_rename_at_1036_238,__context__).isInType && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_237),__i_rename_at_1036_238,((char *) ":")) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_237),__i_rename_at_1036_238);
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_237),__i_rename_at_1036_238 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1035_237),__i_rename_at_1036_238 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1036_238));
    };
}

inline void fmt_function_decl_param_paren_6c291b7c3a975893 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1047_239 )
{
    {
        bool __need_loop_1048 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1047_239.tokens)) - 1));
        int32_t __i_rename_at_1048_240;
        __need_loop_1048 = __i_iterator.first(__context__,(__i_rename_at_1048_240)) && __need_loop_1048;
        for ( ; __need_loop_1048 ; __need_loop_1048 = __i_iterator.next(__context__,(__i_rename_at_1048_240)) )
        {
            if ( (__ctx_rename_at_1047_239.tokens(__i_rename_at_1048_240,__context__).isInFunctionParam && !(__ctx_rename_at_1047_239.tokens((__i_rename_at_1048_240 - 1),__context__).isInFunctionParam)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240,((char *) "(")) )
            {
                remove_spaces_around_1c96d2bcc36445d1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240);
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240 - 1,((char *) "]")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240);
                };
            };
            if ( (__ctx_rename_at_1047_239.tokens(__i_rename_at_1048_240,__context__).isInFunctionParam && !(__ctx_rename_at_1047_239.tokens((__i_rename_at_1048_240 + 1),__context__).isInFunctionParam)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1047_239),__i_rename_at_1048_240);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1048_240));
    };
}

inline void fmt_function_call_param_paren_call_f740219cd0547b48 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1062_241 )
{
    {
        bool __need_loop_1063 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1062_241.tokens)) - 1));
        int32_t __i_rename_at_1063_242;
        __need_loop_1063 = __i_iterator.first(__context__,(__i_rename_at_1063_242)) && __need_loop_1063;
        for ( ; __need_loop_1063 ; __need_loop_1063 = __i_iterator.next(__context__,(__i_rename_at_1063_242)) )
        {
            if ( (eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_241),__i_rename_at_1063_242,((char *) "(")) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_241),__i_rename_at_1063_242))) && (__ctx_rename_at_1062_241.tokens((__i_rename_at_1063_242 - 1),__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                remove_spaces_around_1c96d2bcc36445d1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_241),__i_rename_at_1063_242);
                int32_t __closingParen_rename_at_1066_243 = ((int32_t)find_pair_paren_90acbe7a3b864d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_241),__i_rename_at_1063_242));
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1062_241),__closingParen_rename_at_1066_243);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1063_242));
    };
}

inline void fmt_spaces_around_operators_9837b8f174165da7 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1073_244 )
{
    {
        bool __need_loop_1074 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1073_244.tokens)) - 1));
        int32_t __i_rename_at_1074_245;
        __need_loop_1074 = __i_iterator.first(__context__,(__i_rename_at_1074_245)) && __need_loop_1074;
        for ( ; __need_loop_1074 ; __need_loop_1074 = __i_iterator.next(__context__,(__i_rename_at_1074_245)) )
        {
            if ( !builtin_empty(__ctx_rename_at_1073_244.tokens(__i_rename_at_1074_245,__context__).str) )
            {
                if ( is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245) && !(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245)) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245 + 1);
                };
                if ( need_space_only_before_584e48acb8e8a70(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245) && !(is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245)) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245);
                } else if ( need_spaces_around_7179a668ae90ccd6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245) && !(is_unary_62ad92645c7f9ee6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245)) )
                {
                    make_spaces_around_9b0360b9b4d718c3(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1073_244),__i_rename_at_1074_245);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1074_245));
    };
}

inline void fmt_space_after_keyword_2aab289129cf4fc8 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1092_246 )
{
    {
        bool __need_loop_1093 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1092_246.tokens)) - 1));
        int32_t __i_rename_at_1093_247;
        __need_loop_1093 = __i_iterator.first(__context__,(__i_rename_at_1093_247)) && __need_loop_1093;
        for ( ; __need_loop_1093 ; __need_loop_1093 = __i_iterator.next(__context__,(__i_rename_at_1093_247)) )
        {
            if ( _FuncbuiltinTickfind_indexTick6703785223819426183_716a62e4be337622(__context__,das_global<TDim<char *,11>,0x94e2878ce5158877>(__context__) /*space_after_keywords*/,__ctx_rename_at_1092_246.tokens(__i_rename_at_1093_247,__context__).str) >= 0 )
            {
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_246),__i_rename_at_1093_247 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_246),__i_rename_at_1093_247 + 1);
                };
            };
            if ( (SimPolicy<char *>::Equ(cast<char *>::from(__ctx_rename_at_1092_246.tokens(__i_rename_at_1093_247,__context__).str),cast<char *>::from(((char *) "return")),*__context__,nullptr)) && (builtin_string_length(__ctx_rename_at_1092_246.tokens((__i_rename_at_1093_247 + 1),__context__).str,__context__) > 0) )
            {
                if ( !new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_246),__i_rename_at_1093_247 + 1) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1092_246),__i_rename_at_1093_247 + 1);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1093_247));
    };
}

inline void fmt_space_after_paren_a2ef6e80a339831f ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1109_248 )
{
    {
        bool __need_loop_1110 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1109_248.tokens)) - 1));
        int32_t __i_rename_at_1110_249;
        __need_loop_1110 = __i_iterator.first(__context__,(__i_rename_at_1110_249)) && __need_loop_1110;
        for ( ; __need_loop_1110 ; __need_loop_1110 = __i_iterator.next(__context__,(__i_rename_at_1110_249)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1109_248),__i_rename_at_1110_249,((char *) ")")) && (__ctx_rename_at_1109_248.tokens((__i_rename_at_1110_249 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1109_248),__i_rename_at_1110_249 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1110_249));
    };
}

inline void fmt_space_after_square_brackets_15fb170c80c43d93 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1118_250 )
{
    {
        bool __need_loop_1119 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1118_250.tokens)) - 1));
        int32_t __i_rename_at_1119_251;
        __need_loop_1119 = __i_iterator.first(__context__,(__i_rename_at_1119_251)) && __need_loop_1119;
        for ( ; __need_loop_1119 ; __need_loop_1119 = __i_iterator.next(__context__,(__i_rename_at_1119_251)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1118_250),__i_rename_at_1119_251,((char *) "]")) && (__ctx_rename_at_1118_250.tokens((__i_rename_at_1119_251 + 1),__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1118_250),__i_rename_at_1119_251 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1119_251));
    };
}

inline void fmt_remove_space_inside_parens_fc44c6bd3c600b9c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1127_252 )
{
    {
        bool __need_loop_1128 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1127_252.tokens)) - 1));
        int32_t __i_rename_at_1128_253;
        __need_loop_1128 = __i_iterator.first(__context__,(__i_rename_at_1128_253)) && __need_loop_1128;
        for ( ; __need_loop_1128 ; __need_loop_1128 = __i_iterator.next(__context__,(__i_rename_at_1128_253)) )
        {
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_252),__i_rename_at_1128_253,((char *) "(")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_252),__i_rename_at_1128_253 + 1);
            };
            if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_252),__i_rename_at_1128_253,((char *) ")")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1127_252),__i_rename_at_1128_253);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1128_253));
    };
}

inline void fmt_remove_space_before_as_bb9fc0cabc4ff729 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1139_254 )
{
    {
        bool __need_loop_1140 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1139_254.tokens))));
        int32_t __i_rename_at_1140_255;
        __need_loop_1140 = __i_iterator.first(__context__,(__i_rename_at_1140_255)) && __need_loop_1140;
        for ( ; __need_loop_1140 ; __need_loop_1140 = __i_iterator.next(__context__,(__i_rename_at_1140_255)) )
        {
            if ( (!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_254),__i_rename_at_1140_255)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_254),__i_rename_at_1140_255,((char *) "as"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_254),__i_rename_at_1140_255 - 1,((char *) "?")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1139_254),__i_rename_at_1140_255);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1140_255));
    };
}

inline void fmt_remove_space_before_end_of_object_64e24a59c1d5b6b ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1148_256 )
{
    {
        bool __need_loop_1150 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1148_256.tokens)) - 2));
        int32_t __i_rename_at_1150_257;
        __need_loop_1150 = __i_iterator.first(__context__,(__i_rename_at_1150_257)) && __need_loop_1150;
        for ( ; __need_loop_1150 ; __need_loop_1150 = __i_iterator.next(__context__,(__i_rename_at_1150_257)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_256),__i_rename_at_1150_257)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_256),__i_rename_at_1150_257,((char *) "]"))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_256),__i_rename_at_1150_257 + 1,((char *) "]"))) && !(eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_256),__i_rename_at_1150_257 - 1,((char *) "]"))) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1148_256),__i_rename_at_1150_257);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1150_257));
    };
}

inline void fmt_remove_space_before_object_type_298f1dcfc2f7829c ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1158_258 )
{
    {
        bool __need_loop_1160 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1158_258.tokens)) - 1));
        int32_t __i_rename_at_1160_259;
        __need_loop_1160 = __i_iterator.first(__context__,(__i_rename_at_1160_259)) && __need_loop_1160;
        for ( ; __need_loop_1160 ; __need_loop_1160 = __i_iterator.next(__context__,(__i_rename_at_1160_259)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_258),__i_rename_at_1160_259)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_258),__i_rename_at_1160_259 - 2,((char *) "["))) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_258),__i_rename_at_1160_259 - 1,((char *) "["))) && (__ctx_rename_at_1158_258.tokens(__i_rename_at_1160_259,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1158_258),__i_rename_at_1160_259);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1160_259));
    };
}

inline void fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1168_260 )
{
    {
        bool __need_loop_1170 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1168_260.tokens)) - 2));
        int32_t __i_rename_at_1170_261;
        __need_loop_1170 = __i_iterator.first(__context__,(__i_rename_at_1170_261)) && __need_loop_1170;
        for ( ; __need_loop_1170 ; __need_loop_1170 = __i_iterator.next(__context__,(__i_rename_at_1170_261)) )
        {
            if ( ((!(new_line_before_56d9a06feda36687(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_260),__i_rename_at_1170_261)) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_260),__i_rename_at_1170_261 - 1,((char *) ">"))) && __ctx_rename_at_1168_260.tokens((__i_rename_at_1170_261 - 1),__context__).isInType) && eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_260),__i_rename_at_1170_261,((char *) "[")) )
            {
                _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1168_260),__i_rename_at_1170_261);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1170_261));
    };
}

inline void fmt_glue_type_specifiers_cb3a1c8b879530ae ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1180_262 )
{
    {
        bool __need_loop_1181 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(1,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1180_262.tokens))));
        int32_t __i_rename_at_1181_263;
        __need_loop_1181 = __i_iterator.first(__context__,(__i_rename_at_1181_263)) && __need_loop_1181;
        for ( ; __need_loop_1181 ; __need_loop_1181 = __i_iterator.next(__context__,(__i_rename_at_1181_263)) )
        {
            if ( __ctx_rename_at_1180_262.tokens(__i_rename_at_1181_263,__context__).isInType )
            {
                if ( (__ctx_rename_at_1180_262.tokens(__i_rename_at_1181_263,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::UNKNOWN) && (_FuncbuiltinTickfind_indexTick6703785223819426183_ef73ab5093802856(__context__,das_global<TDim<char *,3>,0xca374635aa15cd66>(__context__) /*type_specifiers*/,__ctx_rename_at_1180_262.tokens(__i_rename_at_1181_263,__context__).str) >= 0) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263,((char *) "const")) && eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263 - 1,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickremove_space_beforeTick16814636979153707331_7012dc41ffc1b224(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263);
                };
                if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263,((char *) "-"),((char *) "==")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263);
                };
                if ( eq_1a08ceae13f47e89(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263 - 1,((char *) "|")) )
                {
                    _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1180_262),__i_rename_at_1181_263);
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1181_263));
    };
}

inline void fmt_space_after_cast_type_446660f13b2ec290 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1200_264 )
{
    {
        bool __need_loop_1201 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1200_264.tokens)) - 3));
        int32_t __i_rename_at_1201_265;
        __need_loop_1201 = __i_iterator.first(__context__,(__i_rename_at_1201_265)) && __need_loop_1201;
        for ( ; __need_loop_1201 ; __need_loop_1201 = __i_iterator.next(__context__,(__i_rename_at_1201_265)) )
        {
            if ( __ctx_rename_at_1200_264.tokens(__i_rename_at_1201_265,__context__).tokenType == DAS_COMMENT(enum) das_source_formatter::TokenType::KEYWORD_OR_IDENTIFIER )
            {
                if ( _FuncbuiltinTickfind_indexTick6703785223819426183_a18ce15a2242b33a(__context__,das_global<TDim<char *,22>,0xced6bfad8d3e8a64>(__context__) /*type_after_keyword*/,__ctx_rename_at_1200_264.tokens(__i_rename_at_1201_265,__context__).str) >= 0 )
                {
                    {
                        bool __need_loop_1204 = true;
                        // j: int const
                        das_iterator<range> __j_iterator(range(__i_rename_at_1201_265 + 2,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1200_264.tokens))));
                        int32_t __j_rename_at_1204_266;
                        __need_loop_1204 = __j_iterator.first(__context__,(__j_rename_at_1204_266)) && __need_loop_1204;
                        for ( ; __need_loop_1204 ; __need_loop_1204 = __j_iterator.next(__context__,(__j_rename_at_1204_266)) )
                        {
                            if ( !(__ctx_rename_at_1200_264.tokens(__j_rename_at_1204_266,__context__).isInType) && (__ctx_rename_at_1200_264.tokens(__j_rename_at_1204_266,__context__).tokenType != DAS_COMMENT(enum) das_source_formatter::TokenType::COMMENT) )
                            {
                                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1200_264),__j_rename_at_1204_266);
                                break;
                            };
                        }
                        __j_iterator.close(__context__,(__j_rename_at_1204_266));
                    };
                };
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1201_265));
    };
}

inline void fmt_destructure_space_e9871049a6de7d91 ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1215_267 )
{
    {
        bool __need_loop_1216 = true;
        // i: int const
        das_iterator<range> __i_iterator(range(0,builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1215_267.tokens)) - 1));
        int32_t __i_rename_at_1216_268;
        __need_loop_1216 = __i_iterator.first(__context__,(__i_rename_at_1216_268)) && __need_loop_1216;
        for ( ; __need_loop_1216 ; __need_loop_1216 = __i_iterator.next(__context__,(__i_rename_at_1216_268)) )
        {
            if ( eq_32a38f27b5380f0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1215_267),__i_rename_at_1216_268,((char *) "let"),((char *) "var")) && (__ctx_rename_at_1215_267.tokens((__i_rename_at_1216_268 + 1),__context__).spaces == 0) )
            {
                _Funcdas_source_formatterTickadd_space_beforeTick12675954149209894053_6c24d8b6bd713a0f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1215_267),__i_rename_at_1216_268 + 1);
            };
        }
        __i_iterator.close(__context__,(__i_rename_at_1216_268));
    };
}

inline void do_format_4835418a8469e5fa ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1224_269 )
{
    parse_all_tokens_b5d5b68330cdfcd0(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
    process_formatter_tokens_a63e2ffa4c8206b6(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
    if ( have_formatter_token_296d7fc158126671(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269),((char *) "ignore-file")) )
    {
        return ;
    } else {
        mark_token_context_7dbf70435b518a0a(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_spaces_around_operators_9837b8f174165da7(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_function_call_param_paren_call_f740219cd0547b48(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_comma_7aa6a2faf3ce3f4f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_semicolon_29caf5fd09f6497e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_function_decl_param_paren_6c291b7c3a975893(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_type_colon_a5b252566f4d584(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_cast_type_446660f13b2ec290(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_comma_54b9cd4ba70794e(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_semicolon_132b81d1431b5d49(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_inside_parens_fc44c6bd3c600b9c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_as_bb9fc0cabc4ff729(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_glue_type_specifiers_cb3a1c8b879530ae(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_keyword_2aab289129cf4fc8(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_paren_a2ef6e80a339831f(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_space_after_square_brackets_15fb170c80c43d93(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_function_arguments_7d9f22d6302573cb(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_args_26ba700ce1a53dbc(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_end_of_object_64e24a59c1d5b6b(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_object_type_298f1dcfc2f7829c(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_remove_space_before_array_of_type_ccdc053ec8afd2e1(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        fmt_destructure_space_e9871049a6de7d91(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1224_269));
        int32_t __top_rename_at_1256_270 = ((int32_t)(builtin_array_size(das_arg<TArray<das_source_formatter::Token>>::pass(__ctx_rename_at_1224_269.tokens)) - 1));
        {
            bool __need_loop_1257 = true;
            // it: int const
            das_iterator<range> __it_iterator(range(0,16));
            int32_t __it_rename_at_1257_271;
            __need_loop_1257 = __it_iterator.first(__context__,(__it_rename_at_1257_271)) && __need_loop_1257;
            for ( ; __need_loop_1257 ; __need_loop_1257 = __it_iterator.next(__context__,(__it_rename_at_1257_271)) )
            {
                int32_t __i_rename_at_1258_272 = ((int32_t)(__top_rename_at_1256_270 - __it_rename_at_1257_271));
                if ( (__i_rename_at_1258_272 < 0) || !(builtin_empty(__ctx_rename_at_1224_269.tokens(__i_rename_at_1258_272,__context__).str)) )
                {
                    break;
                } else {
                    das_copy(__ctx_rename_at_1224_269.tokens(__i_rename_at_1258_272,__context__).spaces,0);
                };
            }
            __it_iterator.close(__context__,(__it_rename_at_1257_271));
        };
    };
}

inline char * format_source_string_f69d100c71c99a8c ( Context * __context__, char * const  & __file_data_rename_at_1267_273 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1268_274; memset(&__ctx_rename_at_1268_274,0,sizeof(__ctx_rename_at_1268_274));
    /* finally */ auto __finally_1267= das_finally([&](){
    finalize_b9f001def6083360(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_274));
    /* end finally */ });
    __ctx_rename_at_1268_274; das_zero(__ctx_rename_at_1268_274);
    _FuncbuiltinTick_move_with_lockcheckTick3467971516570048129_f314c103a27e7f0c(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1268_274.data),das_arg<TArray<uint8_t>>::pass(das_invoke<TArray<uint8_t>>::invoke_cmres(__context__,nullptr,[&]() DAS_AOT_INLINE_LAMBDA -> TArray<uint8_t>{
        TArray<uint8_t> ____acomp_1269_18_rename_at_1269_275; das_zero(____acomp_1269_18_rename_at_1269_275);
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_275),false);
        {
            bool __need_loop_1269 = true;
            // c: int const
            das_iterator<char * const > __c_iterator(__file_data_rename_at_1267_273);
            int32_t __c_rename_at_1269_276;
            __need_loop_1269 = __c_iterator.first(__context__,(__c_rename_at_1269_276)) && __need_loop_1269;
            for ( ; __need_loop_1269 ; __need_loop_1269 = __c_iterator.next(__context__,(__c_rename_at_1269_276)) )
            {
                _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_275),uint8_t(__c_rename_at_1269_276));
            }
            __c_iterator.close(__context__,(__c_rename_at_1269_276));
        };
        builtin_set_verify_array_locks(das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_275),true);
        return /* <- */ das_auto_cast_move<TArray<uint8_t>>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_5aac315099132c4e(__context__,das_arg<TArray<uint8_t>>::pass(____acomp_1269_18_rename_at_1269_275)));
    })));
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1268_274.data),0x0);
    do_format_4835418a8469e5fa(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_274));
    return das_auto_cast<char *>::cast(generate_source_64706a1ce7db4983(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1268_274)));
}

inline char * format_source_58c20f6bcb79eabd ( Context * __context__, das_source_formatter::FormatterCtx &  __ctx_rename_at_1276_277, TArray<uint8_t> const  &  __file_data_rename_at_1276_278 )
{
    _FuncbuiltinTickcloneTick3038771811667655495_872ca77a25597c65(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1276_277.data),__file_data_rename_at_1276_278);
    _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67(__context__,das_arg<TArray<uint8_t>>::pass(__ctx_rename_at_1276_277.data),0x0);
    do_format_4835418a8469e5fa(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1276_277));
    return das_auto_cast<char *>::cast(generate_source_64706a1ce7db4983(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1276_277)));
}

inline char * format_source_f3ae203349d40bcb ( Context * __context__, TArray<uint8_t> const  &  __file_data_rename_at_1284_279 )
{
    das_source_formatter::FormatterCtx __ctx_rename_at_1285_280; memset(&__ctx_rename_at_1285_280,0,sizeof(__ctx_rename_at_1285_280));
    /* finally */ auto __finally_1284= das_finally([&](){
    finalize_b9f001def6083360(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1285_280));
    /* end finally */ });
    __ctx_rename_at_1285_280; das_zero(__ctx_rename_at_1285_280);
    return das_auto_cast<char *>::cast(format_source_58c20f6bcb79eabd(__context__,das_arg<das_source_formatter::FormatterCtx>::pass(__ctx_rename_at_1285_280),__file_data_rename_at_1284_279));
}

inline das_source_formatter::TokenTemplate TokenTemplate_bb156f8df8893c2e ( Context * __context__ )
{
    das_source_formatter::TokenTemplate _temp_make_local_40_6_32; _temp_make_local_40_6_32;
    return /* <- */ das_auto_cast_move<das_source_formatter::TokenTemplate>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_6dd7c9d0804a0304(__context__,das_arg<das_source_formatter::TokenTemplate>::pass((([&]() -> das_source_formatter::TokenTemplate& {
        das_zero(_temp_make_local_40_6_32);
        das_copy((_temp_make_local_40_6_32.firstChar),(0));
        das_copy((_temp_make_local_40_6_32.str),(nullptr));
        das_copy((_temp_make_local_40_6_32.length),(0));
        return _temp_make_local_40_6_32;
    })()))));
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

inline das_source_formatter::FormatterCtx FormatterCtx_fd4f7dd8826cf847 ( Context * __context__ )
{
    das_source_formatter::FormatterCtx _temp_make_local_68_6_32; _temp_make_local_68_6_32;
    return /* <- */ das_auto_cast_move<das_source_formatter::FormatterCtx>::cast(_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab33edb522746663(__context__,das_arg<das_source_formatter::FormatterCtx>::pass((([&]() -> das_source_formatter::FormatterCtx& {
        das_zero(_temp_make_local_68_6_32);
        das_copy((_temp_make_local_68_6_32.debugMode),(false));
        das_copy((_temp_make_local_68_6_32.indenting),(0));
        das_copy((_temp_make_local_68_6_32.insideOptions),(false));
        das_copy((_temp_make_local_68_6_32.crCount),(0));
        das_copy((_temp_make_local_68_6_32.lfCount),(0));
        return _temp_make_local_68_6_32;
    })()))));
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
    aotLib[0x2883944a0d21893e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_6dd7c9d0804a0304>>();
    };
    aotLib[0xfa65eb6a26373b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_ab33edb522746663>>();
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
    aotLib[0x166ffa5f73fdaec] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&TokenTemplate_bb156f8df8893c2e>>();
    };
    aotLib[0x25de37d249637ff5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&Token_c2eeda936a4bfe12>>();
    };
    aotLib[0x9cdfea7a8d43d4e6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&FormatterCtx_fd4f7dd8826cf847>>();
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

} // namespace _anon_6388250703603292906
AotListBase impl_aot_das_source_formatter(_anon_6388250703603292906::registerAotFunctions);
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
