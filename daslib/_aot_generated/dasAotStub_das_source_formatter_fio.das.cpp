#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"

 // require builtin
 // require strings
#include "daScript/simulate/aot_builtin_string.h"
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require rtti
#include "daScript/simulate/aot_builtin_rtti.h"
#include "daScript/ast/ast.h"
#include "daScript/ast/ast_handle.h"
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
 // require das_source_formatter
 // require das_source_formatter_fio

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
namespace _anon_15787026382107470219 {

namespace fio { struct df_header; };
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
namespace das_source_formatter { struct TokenTemplate; };
namespace das_source_formatter { struct Token; };
namespace das_source_formatter { struct FormatterToken; };
namespace das_source_formatter { struct FormatterCtx; };
namespace das_source_formatter { struct ParenCounter; };
// unused enumeration ConversionResult
// unused structure df_header
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
extern TypeInfo __type_info__16d0aa3dd6b69257;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__72a21aed44084ca9;
extern VarInfo __var_info__b2cda9b3b20b803;
extern VarInfo __var_info__59a0b05855762103;
extern FuncInfo __func_info__592427a731466a4a;
extern FuncInfo __func_info__793c57c1b60ae74a;
extern FuncInfo __func_info__9acbc0a3950e2dcd;

VarInfo __func_info__592427a731466a4a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xb2cda9b3b20b803), "fr", 0, 0 };
VarInfo * __func_info__592427a731466a4a_fields[1] =  { &__func_info__592427a731466a4a_field_0 };
FuncInfo __func_info__592427a731466a4a = {"invoke block<(fr:fio::FILE const? const):void> const", "", __func_info__592427a731466a4a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x592427a731466a4a), 0x0 };
VarInfo __func_info__793c57c1b60ae74a_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__16d0aa3dd6b69257, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0x59a0b05855762103), "fw", 0, 0 };
VarInfo * __func_info__793c57c1b60ae74a_fields[1] =  { &__func_info__793c57c1b60ae74a_field_0 };
FuncInfo __func_info__793c57c1b60ae74a = {"invoke block<(fw:fio::FILE const? const):void> const", "", __func_info__793c57c1b60ae74a_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x793c57c1b60ae74a), 0x0 };
VarInfo __func_info__9acbc0a3950e2dcd_field_0 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 41026, 24, UINT64_C(0x72a21aed44084ca9), "data", 0, 0 };
VarInfo * __func_info__9acbc0a3950e2dcd_fields[1] =  { &__func_info__9acbc0a3950e2dcd_field_0 };
FuncInfo __func_info__9acbc0a3950e2dcd = {"invoke block<(var data:array<uint8>#):void> const", "", __func_info__9acbc0a3950e2dcd_fields, 1, 32, &__type_info__af63eb4c86020609, nullptr,0,UINT64_C(0x9acbc0a3950e2dcd), 0x0 };
TypeInfo __type_info__16d0aa3dd6b69257 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~fio::FILE"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 34, 16, UINT64_C(0x16d0aa3dd6b69257) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__16d0aa3dd6b69257, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
TypeInfo * __tinfo_1[3] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_0, char * const  __mode_rename_at_12_1, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_2 );
inline void format_file_50495df36efe9eb6 ( Context * __context__, char * const  __file_name_rename_at_15_4 );
inline void format_files_5658cd111c4b8369 ( Context * __context__, TArray<char *> const  & __file_names_rename_at_42_10 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_0, char * const  __mode_rename_at_12_1, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_2 )
{
    FILE const  * __f_rename_at_13_3 = ((FILE const  *)builtin_fopen(__name_rename_at_12_0,__mode_rename_at_12_1));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_2,__f_rename_at_13_3);
    if ( __f_rename_at_13_3 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_3,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void format_file_50495df36efe9eb6 ( Context * __context__, char * const  __file_name_rename_at_15_4 ) { das_stack_prologue __prologue(__context__,256,"format_file " DAS_FILE_LINE);
{
    char * __unformatted_rename_at_16_5 = (char *)(nullptr);
    char * __formatted_rename_at_17_6 = (char *)(nullptr);
    _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__file_name_rename_at_15_4,((char *) "rb"),das_make_block<void,FILE const  * const >(__context__,112,0,&__func_info__592427a731466a4a,[&](FILE const  * const  __fr_rename_at_18_7) -> void{
        if ( __fr_rename_at_18_7 == nullptr )
        {
            toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_0, cast<char * const >::from(((char *) "cannot open '")), cast<char * const >::from(__file_name_rename_at_15_4), cast<char * const >::from(((char *) "'\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        } else {
            builtin_map_file(__fr_rename_at_18_7,das_make_block<void,TArray<uint8_t> &>(__context__,176,0,&__func_info__9acbc0a3950e2dcd,[&](TArray<uint8_t> & __data_rename_at_22_8) -> void{
                das_copy(__unformatted_rename_at_16_5,((char * const )(builtin_string_from_array(das_arg<TArray<uint8_t>>::pass(__data_rename_at_22_8),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
                das_copy(__formatted_rename_at_17_6,das_invoke_function<char * const >::invoke<TArray<uint8_t> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@das_source_formatter::format_source CI1<u8>A*/ 0x15aef197c7778c5)),das_arg<TArray<uint8_t>>::pass(__data_rename_at_22_8)));
            }),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
        };
    }));
    if ( !(builtin_empty(__formatted_rename_at_17_6)) && (SimPolicy<char *>::NotEqu(cast<char *>::from(__formatted_rename_at_17_6),cast<char *>::from(__unformatted_rename_at_16_5),*__context__,nullptr)) )
    {
        _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac(__context__,__file_name_rename_at_15_4,((char *) "wb"),das_make_block<void,FILE const  * const >(__context__,240,0,&__func_info__793c57c1b60ae74a,[&](FILE const  * const  __fw_rename_at_30_9) -> void{
            if ( __fw_rename_at_30_9 == nullptr )
            {
                toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<3>(__tinfo_1, cast<char * const >::from(((char *) "cannot open '")), cast<char * const >::from(__file_name_rename_at_15_4), cast<char * const >::from(((char *) " for write'\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            } else {
                builtin_fprint(__fw_rename_at_30_9,__formatted_rename_at_17_6,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
            };
        }));
    };
}}

inline void format_files_5658cd111c4b8369 ( Context * __context__, TArray<char *> const  &  __file_names_rename_at_42_10 )
{
    {
        bool __need_loop_43 = true;
        // fname: string const&
        das_iterator<TArray<char *> const > __fname_iterator(__file_names_rename_at_42_10);
        char * const  * __fname_rename_at_43_11;
        __need_loop_43 = __fname_iterator.first(__context__,(__fname_rename_at_43_11)) && __need_loop_43;
        for ( ; __need_loop_43 ; __need_loop_43 = __fname_iterator.next(__context__,(__fname_rename_at_43_11)) )
        {
            format_file_50495df36efe9eb6(__context__,(*__fname_rename_at_43_11));
        }
        __fname_iterator.close(__context__,(__fname_rename_at_43_11));
    };
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x7c2e30b4091b7513] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac>>();
    };
    aotLib[0xe39299f0ef43be2b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_file_50495df36efe9eb6>>();
    };
    aotLib[0x4ab94649b2f6cd5f] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&format_files_5658cd111c4b8369>>();
    };
    // [[ init script ]]
    aotLib[0x4ec73327d84f23b2] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_15787026382107470219
AotListBase impl_aot_das_source_formatter_fio(_anon_15787026382107470219::registerAotFunctions);
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
