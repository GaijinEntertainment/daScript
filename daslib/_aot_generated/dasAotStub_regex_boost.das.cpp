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
namespace _anon_16694443867222988614 {

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
// unused enumeration Type
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
}
template <> struct cast< das::_anon_16694443867222988614::regex::ReOp > : cast_enum < das::_anon_16694443867222988614::regex::ReOp > {};
namespace _anon_16694443867222988614 {
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


inline void finalize_33949679bbb57f6f ( Context * __context__, AutoTuple<range,char *> & ____this_rename_at_1186_0 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_6f9d59028ece8191 ( Context * __context__, TArray<regex::ReNode *> & __a_rename_at_1182_1 );
inline void _FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532 ( Context * __context__, TArray<ast::AstReaderMacro *> & __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_36c9eb4e6b5afb54 ( Context * __context__, regex_boost::RegexReader const  & __cl_rename_at_116_5 );
inline void finalize_ce48cc2769f1ff86 ( Context * __context__, regex::ReNode & ____this_rename_at_29_6 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ced2994fb9435cfe ( Context * __context__, TArray<AutoTuple<range,char *>> & __a_rename_at_1182_7 );
inline void finalize_975dc1c3269122c3 ( Context * __context__, regex::ReNode * & ____this_rename_at_50_9 );
inline void finalize_53d748a680ec146b ( Context * __context__, regex::Regex & ____this_rename_at_48_10 );

void __init_script ( Context * __context__, bool __init_shared )
{
}

inline void finalize_33949679bbb57f6f ( Context * __context__, AutoTuple<range,char *> &  ____this_rename_at_1186_0 )
{
    memset(&(____this_rename_at_1186_0), 0, TypeSize<AutoTuple<range,char *>>::size);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_6f9d59028ece8191 ( Context * __context__, TArray<regex::ReNode *> &  __a_rename_at_1182_1 )
{
    {
        bool __need_loop_1184 = true;
        // aV: regex::ReNode? aka TT&
        das_iterator<TArray<regex::ReNode *>> __aV_iterator(__a_rename_at_1182_1);
        regex::ReNode * * __aV_rename_at_1184_2;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_2)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_2)) )
        {
            finalize_975dc1c3269122c3(__context__,(*__aV_rename_at_1184_2));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_2));
    };
    builtin_array_free(das_arg<TArray<regex::ReNode *>>::pass(__a_rename_at_1182_1),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532 ( Context * __context__, TArray<ast::AstReaderMacro *> &  __Arr_rename_at_181_3, ast::AstReaderMacro * __value_rename_at_181_4 )
{
    das_copy(__Arr_rename_at_181_3(builtin_array_push_back(das_arg<TArray<ast::AstReaderMacro *>>::pass(__Arr_rename_at_181_3),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_4);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_36c9eb4e6b5afb54 ( Context * __context__, regex_boost::RegexReader const  &  __cl_rename_at_116_5 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_5.__rtti))).getStructType())));
}

inline void finalize_ce48cc2769f1ff86 ( Context * __context__, regex::ReNode &  ____this_rename_at_29_6 )
{
    _FuncbuiltinTickfinalizeTick13836114024949725080_6f9d59028ece8191(__context__,das_arg<TArray<regex::ReNode *>>::pass(____this_rename_at_29_6.all));
    finalize_975dc1c3269122c3(__context__,____this_rename_at_29_6.left);
    finalize_975dc1c3269122c3(__context__,____this_rename_at_29_6.right);
    finalize_975dc1c3269122c3(__context__,____this_rename_at_29_6.subexpr);
    memset(&(____this_rename_at_29_6), 0, TypeSize<regex::ReNode>::size);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_ced2994fb9435cfe ( Context * __context__, TArray<AutoTuple<range,char *>> &  __a_rename_at_1182_7 )
{
    {
        bool __need_loop_1184 = true;
        // aV: tuple<range;string> aka TT&
        das_iterator<TArray<AutoTuple<range,char *>>> __aV_iterator(__a_rename_at_1182_7);
        AutoTuple<range,char *> * __aV_rename_at_1184_8;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_8)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_8)) )
        {
            finalize_33949679bbb57f6f(__context__,das_arg<AutoTuple<range,char *>>::pass((*__aV_rename_at_1184_8)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_8));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<range,char *>>>::pass(__a_rename_at_1182_7),16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_975dc1c3269122c3 ( Context * __context__, regex::ReNode * & ____this_rename_at_50_9 )
{
    if ( ____this_rename_at_50_9 != nullptr )
    {
        finalize_ce48cc2769f1ff86(__context__,das_arg<regex::ReNode>::pass(das_deref(__context__,____this_rename_at_50_9)));
        das_delete<regex::ReNode *>::clear(__context__,____this_rename_at_50_9);
        das_copy(____this_rename_at_50_9,nullptr);
    };
}

inline void finalize_53d748a680ec146b ( Context * __context__, regex::Regex &  ____this_rename_at_48_10 )
{
    finalize_975dc1c3269122c3(__context__,____this_rename_at_48_10.root);
    _FuncbuiltinTickfinalizeTick13836114024949725080_ced2994fb9435cfe(__context__,das_arg<TArray<AutoTuple<range,char *>>>::pass(____this_rename_at_48_10.groups));
    memset(&(____this_rename_at_48_10), 0, TypeSize<regex::Regex>::size);
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0xa9e3b6750d79e922] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_33949679bbb57f6f>>();
    };
    aotLib[0x1ec7afd9ea734cbd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_6f9d59028ece8191>>();
    };
    aotLib[0x27004c5f7ca4b606] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_360a0fc531bcb532>>();
    };
    aotLib[0x5152f0b4c062a0cd] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_36c9eb4e6b5afb54>>();
    };
    aotLib[0xba3f0e2f8f6c2af9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_ce48cc2769f1ff86>>();
    };
    aotLib[0x10077f308e6c3336] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_ced2994fb9435cfe>>();
    };
    aotLib[0x2f11042503d3f047] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_975dc1c3269122c3>>();
    };
    aotLib[0x7e949fe6fbaa5320] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_53d748a680ec146b>>();
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_16694443867222988614
AotListBase impl_aot_regex_boost(_anon_16694443867222988614::registerAotFunctions);
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
