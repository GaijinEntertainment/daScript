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
 // require regex
 // require regex_boost

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
namespace _anon_14781170621065626316 {

namespace regex_boost { struct RegexReader; };
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
namespace regex { struct ReNode; };
namespace regex { struct Regex; };
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
// unused structure AstFunctionAnnotation
// unused structure AstBlockAnnotation
// unused structure AstStructureAnnotation
// unused structure AstPassMacro
// unused structure AstVariantMacro
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
namespace regex {

enum class ReOp : int32_t {
    Char = int32_t(0),
    Set = int32_t(1),
    Any = int32_t(2),
    Eos = int32_t(3),
    Group = int32_t(4),
    Plus = int32_t(5),
    Star = int32_t(6),
    Question = int32_t(7),
    Concat = int32_t(8),
    Union = int32_t(9),
};
}
namespace regex {

struct ReNode {
    DAS_COMMENT(enum) regex::ReOp op;
    int32_t id;
    Func DAS_COMMENT((uint8_t const  *,regex::Regex,regex::ReNode *,uint8_t const  * const )) fun2;
    Func DAS_COMMENT((void,regex::ReNode *,Sequence DAS_COMMENT((uint32_t)),StringBuilderWriter)) gen2;
    range at;
    char * text;
    int32_t textLen;
    TArray<regex::ReNode *> all;
    regex::ReNode * left;
    regex::ReNode * right;
    regex::ReNode * subexpr;
    regex::ReNode * next;
    TDim<uint32_t,8> cset;
    int32_t index;
    uint8_t const  * tail;
};
}
namespace regex {

struct Regex {
    regex::ReNode * root;
    uint8_t const  * match;
    TArray<AutoTuple<range,char *>> groups;
    TDim<uint32_t,8> earlyOut;
    bool canEarlyOut;
};
}
namespace regex_boost {

struct RegexReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstReaderMacro)) __finalize;
    Func DAS_COMMENT((bool,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((char *,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,ExprReader * const ,LineInfo const ,int32_t &,FileInfo * &)) suffix;
    Func DAS_COMMENT((smart_ptr_raw<Expression>,ast::AstReaderMacro,smart_ptr_raw<Program> const ,Module * const ,smart_ptr_raw<ExprReader> const )) visit;
};
}


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline void finalize_8b9cedd0cf437d31 ( Context * __context__, AutoTuple<range,char *> & ____this_rename_at_1196_0 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5376afc4b00003d6 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1192_1 );
inline void _FuncbuiltinTickpushTick10769833213962245646_8cb1c72b64cb3c2c ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc3cd36d34abfa4a ( Context * __context__, regex_boost::RegexReader const  & __cl_rename_at_116_5 );
inline void finalize_3c5c513e0f978714 ( Context * __context__, regex::ReNode & ____this_rename_at_29_6 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3c06f24438531651 ( Context * __context__, TArray<AutoTuple<range,char *>> & __a_rename_at_1192_7 );
inline void finalize_16930d316174ad9e ( Context * __context__, regex::ReNode * & ____this_rename_at_50_9 );
inline void finalize_f59291860c48a2f1 ( Context * __context__, regex::Regex & ____this_rename_at_48_10 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void finalize_8b9cedd0cf437d31 ( Context * __context__, AutoTuple<range,char *> &  ____this_rename_at_1196_0 )
{
    memset((void*)&(____this_rename_at_1196_0), 0, TypeSize<AutoTuple<range,char *>>::size);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_5376afc4b00003d6 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1192_1 )
{
    {
        bool __need_loop_1194 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1192_1);
        regex::ReNode * * __aV_rename_at_1194_2;
        __need_loop_1194 = __aV_iterator.first(__context__,(__aV_rename_at_1194_2)) && __need_loop_1194;
        for ( ; __need_loop_1194 ; __need_loop_1194 = __aV_iterator.next(__context__,(__aV_rename_at_1194_2)) )
        {
            finalize_16930d316174ad9e(__context__,(*__aV_rename_at_1194_2));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1194_2));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1192_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_8cb1c72b64cb3c2c ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_cc3cd36d34abfa4a ( Context * __context__, regex_boost::RegexReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline void finalize_3c5c513e0f978714 ( Context * __context__, regex::ReNode &  ____this_rename_at_29_6 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_5376afc4b00003d6(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_29_6.all));
    finalize_16930d316174ad9e(__context__,____this_rename_at_29_6.left);
    finalize_16930d316174ad9e(__context__,____this_rename_at_29_6.right);
    finalize_16930d316174ad9e(__context__,____this_rename_at_29_6.subexpr);
    memset((void*)&(____this_rename_at_29_6), 0, TypeSize<regex::ReNode>::size);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_3c06f24438531651 ( Context * __context__, TArray<AutoTuple<range,char *>> &  __a_rename_at_1192_7 )
{
    {
        bool __need_loop_1194 = true;
        // aV: tuple<range;string> aka TT&
        das_iterator<TArray<AutoTuple<range,char *>>> __aV_iterator(__a_rename_at_1192_7);
        AutoTuple<range,char *> * __aV_rename_at_1194_8;
        __need_loop_1194 = __aV_iterator.first(__context__,(__aV_rename_at_1194_8)) && __need_loop_1194;
        for ( ; __need_loop_1194 ; __need_loop_1194 = __aV_iterator.next(__context__,(__aV_rename_at_1194_8)) )
        {
            finalize_8b9cedd0cf437d31(__context__,das_arg<AutoTuple<range,char *>>::pass((*__aV_rename_at_1194_8)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1194_8));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<range,char *>>>::pass(__a_rename_at_1192_7),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_16930d316174ad9e ( Context * __context__, regex::ReNode * & ____this_rename_at_50_9 )
{
    if ( ____this_rename_at_50_9 != nullptr )
    {
        finalize_3c5c513e0f978714(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_50_9)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_50_9);
        das_copy(____this_rename_at_50_9,nullptr);
    };
}

inline void finalize_f59291860c48a2f1 ( Context * __context__, regex::Regex &  ____this_rename_at_48_10 )
{
    finalize_16930d316174ad9e(__context__,____this_rename_at_48_10.root);
    _FuncbuiltinTickfinalizeTick13836114024949725080_3c06f24438531651(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(____this_rename_at_48_10.groups));
    memset((void*)&(____this_rename_at_48_10), 0, TypeSize<regex::Regex>::size);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x33dfe0fcc19c6c3e] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_8b9cedd0cf437d31>>();
    };
    aotLib[0x81b584d807f697d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_5376afc4b00003d6>>();
    };
    aotLib[0xebacb5d095f2c1e5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_8cb1c72b64cb3c2c>>();
    };
    aotLib[0xaac46f3dfda6d142] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_cc3cd36d34abfa4a>>();
    };
    aotLib[0x933a012f8f2d87b7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_3c5c513e0f978714>>();
    };
    aotLib[0x1e95318a496dc0e0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_3c06f24438531651>>();
    };
    aotLib[0xeccfb236b9a7e81c] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_16930d316174ad9e>>();
    };
    aotLib[0xcd04464b4e307937] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_f59291860c48a2f1>>();
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_14781170621065626316
AotListBase impl_aot_regex_boost(_anon_14781170621065626316::registerAotFunctions);
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
