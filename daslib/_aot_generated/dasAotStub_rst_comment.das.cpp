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
 // require fio
#include "daScript/misc/performance_time.h"
#include "daScript/simulate/aot_builtin_fio.h"
 // require strings_boost
 // require ast_boost
 // require regex
 // require regex_boost
 // require rst
 // require rst_comment

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
namespace _anon_3442873993894616589 {

namespace rst_comment { struct RstComment; };
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
namespace fio { struct df_header; };
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
namespace regex_boost { struct RegexReader; };
namespace rst { struct DocGroup; };
namespace rst { struct _lambda_rst_1216_1; };
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
// unused structure AstReaderMacro
namespace ast {

struct AstCommentReader {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCommentReader)) __finalize;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,bool,LineInfo const )) open;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) close;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeStructure;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Structure> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructure;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeStructureFields;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructureField;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructureFields;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeFunction;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Function> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterFunction;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeGlobalVariables;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterGlobalVariable;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterGlobalVariables;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeVariant;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeVariantEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariantEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariantEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariant;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeTuple;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeTupleEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTupleEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTupleEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTuple;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeBitfield;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeBitfieldEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfieldEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfieldEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfield;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeEnumeration;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeEnumerationEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumerationEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumerationEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumeration;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeAlias;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterAlias;
};
}
// unused structure AstCallMacro
// unused structure AstTypeInfoMacro
// unused structure AstEnumerationAnnotation
// unused structure AstVisitor
// unused structure df_header
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
// unused enumeration ReOp
// unused structure ReNode
// unused structure Regex
// unused structure RegexReader
// unused structure DocGroup
// unused structure _lambda_rst_1216_1
namespace rst_comment {

enum class ParserState : int32_t {
    none = int32_t(0),
    structure = int32_t(1),
    field = int32_t(2),
    func = int32_t(3),
    method = int32_t(4),
    global = int32_t(5),
    variant_alias = int32_t(6),
    variant_entry = int32_t(7),
    tuple_alias = int32_t(8),
    tuple_entry = int32_t(9),
    bitfield_alias = int32_t(10),
    bitfield_entry = int32_t(11),
    enum_decl = int32_t(12),
    enum_entry = int32_t(13),
    type_alias = int32_t(14),
};
}
namespace rst_comment {

struct RstComment {
    void * __rtti;
    Func DAS_COMMENT((void,ast::AstCommentReader)) __finalize;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,bool,LineInfo const )) open;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,int32_t,LineInfo const )) accept;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) close;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeStructure;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Structure> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructure;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeStructureFields;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructureField;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterStructureFields;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeFunction;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Function> const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterFunction;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeGlobalVariables;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterGlobalVariable;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterGlobalVariables;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeVariant;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeVariantEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariantEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariantEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterVariant;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeTuple;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeTupleEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTupleEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTupleEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterTuple;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeBitfield;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeBitfieldEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfieldEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfieldEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterBitfield;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeEnumeration;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeEnumerationEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumerationEntry;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumerationEntries;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterEnumeration;
    Func DAS_COMMENT((void,ast::AstCommentReader,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) beforeAlias;
    Func DAS_COMMENT((void,ast::AstCommentReader,char * const ,smart_ptr_raw<Program> const ,Module * const ,LineInfo const )) afterAlias;
    TArray<uint8_t> chars;
    DAS_COMMENT(enum) rst_comment::ParserState state;
    char * comment;
    char * structure_comment;
    char * function_comment;
    char * global_comment;
    char * field_comment;
    char * variant_comment;
    char * variant_entry_comment;
    char * tuple_comment;
    char * tuple_entry_comment;
    char * bitfield_comment;
    char * bitfield_entry_comment;
    char * enum_comment;
    char * enum_entry_comment;
    char * alias_comment;
    TArray<AutoTuple<char *,char *,char *>> field_comments;
    Func DAS_COMMENT((void,rst_comment::RstComment,char * const ,char * const )) write_to_detail;
};
}
extern TypeInfo __type_info__af90fe4c864e9d52;

TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, TypeSize<char *>::size, UINT64_C(0xaf90fe4c864e9d52) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_ae4622b87d5045ec ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void finalize_83696062e344b3c4 ( Context * __context__, AutoTuple<char *,char *,char *> & ____this_rename_at_1238_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_e3abad888c8eec55 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  & __a_rename_at_581_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_111a5743258d9ed4 ( Context * __context__, TArray<ast::AstCommentReader *> & __Arr_rename_at_181_4, ast::AstCommentReader * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5c7f548c8502e1a9 ( Context * __context__, rst_comment::RstComment const  & __cl_rename_at_116_6 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_c562e1bb9a69fcf2 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> & __a_rename_at_1394_7 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e7318b7a6abc1eef ( Context * __context__, TArray<uint8_t> & __a_rename_at_1234_9 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_63555a7fd208695d ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> & __a_rename_at_1234_10 );
inline smart_ptr_raw<CommentReader> _FuncastTickmake_comment_readerTick2128729122050972798_e4b73d881f9f1977 ( Context * __context__, char * const  __name_rename_at_714_12, rst_comment::RstComment * __someClassPtr_rename_at_714_13 );
inline void _FuncbuiltinTickpushTick14133213201864676143_e6a1a245cf63942c ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_15, uint8_t __value_rename_at_165_16 );
inline void _FuncbuiltinTickeraseTick16646986352019611268_6e8bd72874affc48 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_535_17, int32_t __at_rename_at_535_18 );
inline void _FuncbuiltinTickpushTick10769833213962245646_3caac662fb9db99 ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> & __Arr_rename_at_181_19, AutoTuple<char *,char *,char *> & __value_rename_at_181_20 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_df2834602d620674 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_21 );
inline void _FuncfioTickfopenTick3937565566638487747_453d8f2336972b96 ( Context * __context__, char * const  __name_rename_at_12_23, char * const  __mode_rename_at_12_24, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_25 );
inline void _FuncastTickadd_new_comment_readerTick9279614113534996372_8ea5b888d51c4277 ( Context * __context__, char * const  __name_rename_at_900_27, rst_comment::RstComment * __someClassPtr_rename_at_900_28 );
inline void panic_rst_83c9cac04dcedf3e ( Context * __context__, char * const  __s_rename_at_41_30 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x28b15bfe65ac9fe2>(__context__) = true;/*DEBUG_COMMENT_READER*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_ae4622b87d5045ec ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_83696062e344b3c4 ( Context * __context__, AutoTuple<char *,char *,char *> &  ____this_rename_at_1238_2 )
{
    memset((void*)&(____this_rename_at_1238_2), 0, TypeSize<AutoTuple<char *,char *,char *>>::size);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_e3abad888c8eec55 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  &  __a_rename_at_581_3 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_111a5743258d9ed4 ( Context * __context__, TArray<ast::AstCommentReader *> &  __Arr_rename_at_181_4, ast::AstCommentReader * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstCommentReader *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_5c7f548c8502e1a9 ( Context * __context__, rst_comment::RstComment const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_c562e1bb9a69fcf2 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> &  __a_rename_at_1394_7 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1396_8;das_zero(__arr_rename_at_1396_8);
    _FuncbuiltinTickresizeTick4811697762258667383_ae4622b87d5045ec(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1396_8),1);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1396_8(0,__context__))),__a_rename_at_1394_7);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1396_8);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_e7318b7a6abc1eef ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1234_9 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1234_9),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_63555a7fd208695d ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> &  __a_rename_at_1234_10 )
{
    {
        bool __need_loop_1236 = true;
        // aV: tuple<kind:string;name:string;comment:string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *,char *>>> __aV_iterator(__a_rename_at_1234_10);
        AutoTuple<char *,char *,char *> * __aV_rename_at_1236_11;
        __need_loop_1236 = __aV_iterator.first(__context__,(__aV_rename_at_1236_11)) && __need_loop_1236;
        for ( ; __need_loop_1236 ; __need_loop_1236 = __aV_iterator.next(__context__,(__aV_rename_at_1236_11)) )
        {
            finalize_83696062e344b3c4(__context__,das_arg<AutoTuple<char *,char *,char *>>::pass((*__aV_rename_at_1236_11)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1236_11));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *,char *>>>::pass(__a_rename_at_1234_10),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline smart_ptr_raw<CommentReader> _FuncastTickmake_comment_readerTick2128729122050972798_e4b73d881f9f1977 ( Context * __context__, char * const  __name_rename_at_714_12, rst_comment::RstComment * __someClassPtr_rename_at_714_13 )
{
    _FuncbuiltinTickpushTick10769833213962245646_111a5743258d9ed4(__context__,das_arg<TArray<ast::AstCommentReader *>>::pass(das_global<TArray<ast::AstCommentReader *>,0xd806bcc7af45ea49>(__context__) /*gc_root_AstCommentReader*/),das_reinterpret<ast::AstCommentReader *>::pass(__someClassPtr_rename_at_714_13));
    StructInfo const  * __classInfo_rename_at_717_14 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_5c7f548c8502e1a9(__context__,das_arg<rst_comment::RstComment>::pass(das_deref(__context__,__someClassPtr_rename_at_714_13))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<CommentReader>>::cast(makeCommentReader(das_auto_cast<void * const >::cast(__someClassPtr_rename_at_714_13),__classInfo_rename_at_717_14,__context__));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_e6a1a245cf63942c ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_15, uint8_t __value_rename_at_165_16 )
{
    das_copy(__Arr_rename_at_165_15(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_15),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_16);
}

inline void _FuncbuiltinTickeraseTick16646986352019611268_6e8bd72874affc48 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_535_17, int32_t __at_rename_at_535_18 )
{
    builtin_array_erase(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_535_17),__at_rename_at_535_18,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_3caac662fb9db99 ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> &  __Arr_rename_at_181_19, AutoTuple<char *,char *,char *> &  __value_rename_at_181_20 )
{
    das_copy(__Arr_rename_at_181_19(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *,char *>>>::pass(__Arr_rename_at_181_19),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_20);
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_df2834602d620674 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_21 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_72_32_0; _temp_make_local_72_32_0;
    TDim<AutoTuple<char *,char *>,1> _temp_make_local_72_32_1; _temp_make_local_72_32_1;
    char * __mn_rename_at_71_22 = (char *)(((char * const )(to_das_string(__value_rename_at_69_21->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_22,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_71_22,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_72_32_0 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_c562e1bb9a69fcf2(__context__,das_arg<TDim<AutoTuple<char *,char *>,1>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,1>& {
        _temp_make_local_72_32_1(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_73;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_73) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_1;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_22);
}

inline void _FuncfioTickfopenTick3937565566638487747_453d8f2336972b96 ( Context * __context__, char * const  __name_rename_at_12_23, char * const  __mode_rename_at_12_24, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_25 )
{
    FILE const  * __f_rename_at_13_26 = ((FILE const  *)builtin_fopen(__name_rename_at_12_23,__mode_rename_at_12_24));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_25,__f_rename_at_13_26);
    if ( __f_rename_at_13_26 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_26,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void _FuncastTickadd_new_comment_readerTick9279614113534996372_8ea5b888d51c4277 ( Context * __context__, char * const  __name_rename_at_900_27, rst_comment::RstComment * __someClassPtr_rename_at_900_28 )
{
    smart_ptr_raw<CommentReader> __ann_rename_at_901_29; memset((void*)&__ann_rename_at_901_29,0,sizeof(__ann_rename_at_901_29));
    /* finally */ auto __finally_900= das_finally([&](){
    das_delete_handle<smart_ptr_raw<CommentReader>>::clear(__context__,__ann_rename_at_901_29);
    /* end finally */ });
    __ann_rename_at_901_29; das_zero(__ann_rename_at_901_29); das_move(__ann_rename_at_901_29, _FuncastTickmake_comment_readerTick2128729122050972798_e4b73d881f9f1977(__context__,__name_rename_at_900_27,__someClassPtr_rename_at_900_28));
    addModuleCommentReader(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_901_29,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void panic_rst_83c9cac04dcedf3e ( Context * __context__, char * const  __s_rename_at_41_30 )
{
    LineInfo _temp_make_local_43_35_2; _temp_make_local_43_35_2;
    toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_0, cast<char * const >::from(__s_rename_at_41_30), cast<char * const >::from(((char *) " at ")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass((_temp_make_local_43_35_2 = (rtti_get_line_info(1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_throw(__s_rename_at_41_30,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x4a7efccd9dc91144] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_ae4622b87d5045ec>>();
    };
    aotLib[0xb93752b16a347a2b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_83696062e344b3c4>>();
    };
    aotLib[0xd9814426ac88e21a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_e3abad888c8eec55>>();
    };
    aotLib[0x8293a60c87739446] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_111a5743258d9ed4>>();
    };
    aotLib[0xc2f162b6707a8f6a] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_5c7f548c8502e1a9>>();
    };
    aotLib[0x5f20816007018ee0] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_c562e1bb9a69fcf2>>();
    };
    aotLib[0xbfa45b6e12740302] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_e7318b7a6abc1eef>>();
    };
    aotLib[0x1dab98c3c2e05fff] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_63555a7fd208695d>>();
    };
    aotLib[0xca64ed8275521e93] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_comment_readerTick2128729122050972798_e4b73d881f9f1977>>();
    };
    aotLib[0x54e487e5023fffc5] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_e6a1a245cf63942c>>();
    };
    aotLib[0xf3fc1cf577f5a633] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick16646986352019611268_6e8bd72874affc48>>();
    };
    aotLib[0x355868a357ff6a29] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_3caac662fb9db99>>();
    };
    aotLib[0xe5c11d7e5f3d2eeb] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_file_nameTick11623859247758201540_df2834602d620674>>();
    };
    aotLib[0x6e0e9b88cf2e4e88] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_453d8f2336972b96>>();
    };
    aotLib[0x54b6e8889099a348] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickadd_new_comment_readerTick9279614113534996372_8ea5b888d51c4277>>();
    };
    aotLib[0x52ec7785e043755d] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&panic_rst_83c9cac04dcedf3e>>();
    };
    // [[ init script ]]
    aotLib[0xb5c51712223fb564] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_3442873993894616589
AotListBase impl_aot_rst_comment(_anon_3442873993894616589::registerAotFunctions);
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
