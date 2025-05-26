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
namespace _anon_13746017607201065299 {

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
}
template <> struct cast< das::_anon_13746017607201065299::rst_comment::ParserState > : cast_enum < das::_anon_13746017607201065299::rst_comment::ParserState > {};
namespace _anon_13746017607201065299 {
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
extern StructInfo __struct_info__2d3441ca6ad9634b;
extern StructInfo __struct_info__61a70b56f6923094;
extern TypeInfo __type_info__a8ccb9bba7fb58db;
extern TypeInfo __type_info__e266b5ccef058802;
extern TypeInfo __type_info__246dda13a8a4b104;
extern TypeInfo __type_info__9d10785eb07580e0;
extern TypeInfo __type_info__e57b0f261f47b890;
extern TypeInfo __type_info__d6b8ed05d16e9f27;
extern TypeInfo __type_info__af81fe4c86352052;
extern TypeInfo __type_info__af8afe4c86446b52;
extern TypeInfo __type_info__af90fe4c864e9d52;
extern TypeInfo __type_info__a57bf935c2dd03;
extern TypeInfo __type_info__60d16a2d23420951;
extern TypeInfo __type_info__8afce1a80940fc9e;
extern TypeInfo __type_info__125855d9cd771ead;
extern TypeInfo __type_info__f9ade15e6e3d3c78;
extern TypeInfo __type_info__8ef87efe522b67be;
extern TypeInfo __type_info__b5e7457401a3ad37;
extern TypeInfo __type_info__af63ee4c86020b22;
extern TypeInfo __type_info__af5be84c85f468f0;
extern TypeInfo __type_info__af63eb4c86020609;
extern VarInfo __var_info__92060eb1dd6013bd;
extern VarInfo __var_info__bd889e12aafcbc7d;
extern VarInfo __var_info__884a5399fc505f6;
extern VarInfo __var_info__68b72f5a533c2d84;
extern VarInfo __var_info__5183b72cf29c5739;
extern VarInfo __var_info__5df991a9039a81e0;
extern VarInfo __var_info__c5d893ae95f885c5;
extern VarInfo __var_info__d04e4c5995a62f2d;
extern VarInfo __var_info__69589294474e92c;
extern VarInfo __var_info__ae7ad5d14d65e87;
extern VarInfo __var_info__aed5967c4af2c0d0;
extern VarInfo __var_info__4a2883a93f165cf8;
extern VarInfo __var_info__4a9583a93fcf93f8;
extern VarInfo __var_info__f28480d6d4b23cfa;
extern VarInfo __var_info__7e7872cb89748b65;
extern VarInfo __var_info__7e6772cb8957a865;
extern VarInfo __var_info__528d468f215df29a;
extern VarInfo __var_info__2b6148b96979359f;
extern VarInfo __var_info__2e58eaa3710e51e6;
extern VarInfo __var_info__10fa9a2d00e4f0d9;
extern VarInfo __var_info__e31a6ac01cc43ae8;
extern VarInfo __var_info__b30478b93b50293b;
extern VarInfo __var_info__feae167dcb326df3;
extern VarInfo __var_info__bb1a66ab837919c7;
extern VarInfo __var_info__5232a29f2f2b0bda;
extern VarInfo __var_info__757869e405e55183;
extern VarInfo __var_info__31b2841aaba2a26e;
extern VarInfo __var_info__11740446154deac1;
extern VarInfo __var_info__3ab3de96309a00f1;
extern VarInfo __var_info__b80cab4771faba30;
extern VarInfo __var_info__b77ca786ffa11112;
extern VarInfo __var_info__a0a4cb037ec11d4f;
extern VarInfo __var_info__d78534af84b71b42;
extern VarInfo __var_info__c76262ce21036f69;
extern VarInfo __var_info__bf71fe47965078d0;
extern VarInfo __var_info__c2ef4435eb70e19f;
extern VarInfo __var_info__68717ae8c39c1ba;
extern VarInfo __var_info__33c4b1e56cee6ffe;
extern VarInfo __var_info__f8aedcc0764220be;
extern VarInfo __var_info__42f8e9c07e03fb19;
extern VarInfo __var_info__37e37d799eff20cf;
extern VarInfo __var_info__57ca5f5d9085e4c6;
extern VarInfo __var_info__5af7b9138dda5761;
extern VarInfo __var_info__cbe8e177831433fe;
extern VarInfo __var_info__6b7a89d83eb61a84;
extern VarInfo __var_info__2e41d1fbd4a1d815;
extern VarInfo __var_info__d0207c205be30794;
extern VarInfo __var_info__ddf94e4fc3dcca4b;
extern VarInfo __var_info__7be23cab45034901;
extern VarInfo __var_info__7c153cab4559f201;
extern VarInfo __var_info__b916cd9a6cc2e2dd;
extern VarInfo __var_info__a9802ca611fc378;
extern VarInfo __var_info__a8702ca6102e078;
extern VarInfo __var_info__f72b86dfabb3d1c9;
extern VarInfo __var_info__17efa68812eaf4d8;
extern VarInfo __var_info__2852a6f2c657398b;
extern VarInfo __var_info__3323d2a5d31ac3b0;
extern VarInfo __var_info__6f8cd011e69d9b31;
extern VarInfo __var_info__b7fdda04287af828;
extern VarInfo __var_info__62a94dd3f7d4a4c7;
extern VarInfo __var_info__5777c9acaa501022;
extern VarInfo __var_info__ca7f5a927daacf30;
extern VarInfo __var_info__7fa3df98c0363cb1;
extern VarInfo __var_info__6d2c2500829f7e58;
extern VarInfo __var_info__e9c7921be0396fcf;
extern VarInfo __var_info__a7ee791b51b3dc66;
extern VarInfo __var_info__6adda5e9b28e8a2a;
extern VarInfo __var_info__a6ea310bed575ee9;
extern VarInfo __var_info__4a78fc6745eced35;
extern VarInfo __var_info__64df14d74eb56d36;
extern VarInfo __var_info__38f2cb39e0811f91;
extern VarInfo __var_info__589d703e65694ea;
extern VarInfo __var_info__6f8b6a84bfea6d0d;
extern VarInfo __var_info__36c4cdff25e42b29;
extern VarInfo __var_info__5bc5a80160eadf5c;
extern VarInfo __var_info__40e9c9da604f99c;
extern VarInfo __var_info__f37e96b25df53e04;
extern VarInfo __var_info__c6bb284a6fcd0ab2;
extern VarInfo __var_info__1bb8e0a1b3583030;
extern VarInfo __var_info__85255ef214b627c3;
extern VarInfo __var_info__6bf300c2ea28357b;
extern VarInfo __var_info__6be400c2ea0eb87b;
extern VarInfo __var_info__ce0b213ac29a4853;
extern VarInfo __var_info__a388e7a92a1cdb1;
extern VarInfo __var_info__605760cd12e599b7;
extern VarInfo __var_info__53401a894a8a8bd6;
extern VarInfo __var_info__6c801d2c3abadb2c;
extern VarInfo __var_info__a49cb8007ec9dd13;
extern VarInfo __var_info__e5095f786fc75f97;
extern VarInfo __var_info__4369dfd4910588a7;
extern VarInfo __var_info__1bf54e03a1235573;
extern VarInfo __var_info__3c932bdd14c864e3;
extern EnumInfo __enum_info__4fb7b029dd403502;

EnumValueInfo __enum_info__4fb7b029dd403502_value_0 = { "none", 0 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_1 = { "structure", 1 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_2 = { "field", 2 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_3 = { "func", 3 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_4 = { "method", 4 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_5 = { "global", 5 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_6 = { "variant_alias", 6 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_7 = { "variant_entry", 7 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_8 = { "tuple_alias", 8 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_9 = { "tuple_entry", 9 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_10 = { "bitfield_alias", 10 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_11 = { "bitfield_entry", 11 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_12 = { "enum_decl", 12 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_13 = { "enum_entry", 13 };
EnumValueInfo __enum_info__4fb7b029dd403502_value_14 = { "type_alias", 14 };
EnumValueInfo * __enum_info__4fb7b029dd403502_values [] = { &__enum_info__4fb7b029dd403502_value_0, &__enum_info__4fb7b029dd403502_value_1, &__enum_info__4fb7b029dd403502_value_2, &__enum_info__4fb7b029dd403502_value_3, &__enum_info__4fb7b029dd403502_value_4, &__enum_info__4fb7b029dd403502_value_5, &__enum_info__4fb7b029dd403502_value_6, &__enum_info__4fb7b029dd403502_value_7, &__enum_info__4fb7b029dd403502_value_8, &__enum_info__4fb7b029dd403502_value_9, &__enum_info__4fb7b029dd403502_value_10, &__enum_info__4fb7b029dd403502_value_11, &__enum_info__4fb7b029dd403502_value_12, &__enum_info__4fb7b029dd403502_value_13, &__enum_info__4fb7b029dd403502_value_14 };
EnumInfo __enum_info__4fb7b029dd403502 = { "ParserState", "rst_comment", __enum_info__4fb7b029dd403502_values, 15, UINT64_C(0x4fb7b029dd403502) };
VarInfo __struct_info__2d3441ca6ad9634b_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xbd889e12aafcbc7d), "__rtti", offsetof(ast::AstCommentReader,__rtti), 37 };
TypeInfo * __type_info__92060eb1dd6013bd_arg_types_var_3257300768127542091[1] = { &__type_info__8ef87efe522b67be };
const char * __type_info__92060eb1dd6013bd_arg_names_var_3257300768127542091[1] = { "self" };
VarInfo __struct_info__2d3441ca6ad9634b_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__92060eb1dd6013bd_arg_types_var_3257300768127542091, __type_info__92060eb1dd6013bd_arg_names_var_3257300768127542091, 1, 0, nullptr, 12, 8, UINT64_C(0x92060eb1dd6013bd), "__finalize", offsetof(ast::AstCommentReader,__finalize), 0 };
TypeInfo * __type_info__68717ae8c39c1ba_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__68717ae8c39c1ba_arg_names_var_3257300768127542091[5] = { "self", "prog", "mod", "cpp", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__68717ae8c39c1ba_arg_types_var_3257300768127542091, __type_info__68717ae8c39c1ba_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x68717ae8c39c1ba), "open", offsetof(ast::AstCommentReader,open), 0 };
TypeInfo * __type_info__884a5399fc505f6_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__af8afe4c86446b52, &__type_info__246dda13a8a4b104 };
const char * __type_info__884a5399fc505f6_arg_names_var_3257300768127542091[5] = { "self", "prog", "mod", "ch", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__884a5399fc505f6_arg_types_var_3257300768127542091, __type_info__884a5399fc505f6_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x884a5399fc505f6), "accept", offsetof(ast::AstCommentReader,accept), 0 };
TypeInfo * __type_info__c2ef4435eb70e19f_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__c2ef4435eb70e19f_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c2ef4435eb70e19f_arg_types_var_3257300768127542091, __type_info__c2ef4435eb70e19f_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xc2ef4435eb70e19f), "close", offsetof(ast::AstCommentReader,close), 0 };
TypeInfo * __type_info__b80cab4771faba30_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__b80cab4771faba30_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b80cab4771faba30_arg_types_var_3257300768127542091, __type_info__b80cab4771faba30_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xb80cab4771faba30), "beforeStructure", offsetof(ast::AstCommentReader,beforeStructure), 0 };
TypeInfo * __type_info__f28480d6d4b23cfa_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__d6b8ed05d16e9f27, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__f28480d6d4b23cfa_arg_names_var_3257300768127542091[5] = { "self", "st", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f28480d6d4b23cfa_arg_types_var_3257300768127542091, __type_info__f28480d6d4b23cfa_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xf28480d6d4b23cfa), "afterStructure", offsetof(ast::AstCommentReader,afterStructure), 0 };
TypeInfo * __type_info__b77ca786ffa11112_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__b77ca786ffa11112_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b77ca786ffa11112_arg_types_var_3257300768127542091, __type_info__b77ca786ffa11112_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xb77ca786ffa11112), "beforeStructureFields", offsetof(ast::AstCommentReader,beforeStructureFields), 0 };
TypeInfo * __type_info__7e7872cb89748b65_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__7e7872cb89748b65_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e7872cb89748b65_arg_types_var_3257300768127542091, __type_info__7e7872cb89748b65_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x7e7872cb89748b65), "afterStructureField", offsetof(ast::AstCommentReader,afterStructureField), 0 };
TypeInfo * __type_info__7e6772cb8957a865_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__7e6772cb8957a865_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7e6772cb8957a865_arg_types_var_3257300768127542091, __type_info__7e6772cb8957a865_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x7e6772cb8957a865), "afterStructureFields", offsetof(ast::AstCommentReader,afterStructureFields), 0 };
TypeInfo * __type_info__11740446154deac1_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__11740446154deac1_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__11740446154deac1_arg_types_var_3257300768127542091, __type_info__11740446154deac1_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x11740446154deac1), "beforeFunction", offsetof(ast::AstCommentReader,beforeFunction), 0 };
TypeInfo * __type_info__aed5967c4af2c0d0_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__9d10785eb07580e0, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__aed5967c4af2c0d0_arg_names_var_3257300768127542091[5] = { "self", "fn", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__aed5967c4af2c0d0_arg_types_var_3257300768127542091, __type_info__aed5967c4af2c0d0_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xaed5967c4af2c0d0), "afterFunction", offsetof(ast::AstCommentReader,afterFunction), 0 };
TypeInfo * __type_info__3ab3de96309a00f1_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__3ab3de96309a00f1_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3ab3de96309a00f1_arg_types_var_3257300768127542091, __type_info__3ab3de96309a00f1_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x3ab3de96309a00f1), "beforeGlobalVariables", offsetof(ast::AstCommentReader,beforeGlobalVariables), 0 };
TypeInfo * __type_info__4a2883a93f165cf8_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__4a2883a93f165cf8_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4a2883a93f165cf8_arg_types_var_3257300768127542091, __type_info__4a2883a93f165cf8_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x4a2883a93f165cf8), "afterGlobalVariable", offsetof(ast::AstCommentReader,afterGlobalVariable), 0 };
TypeInfo * __type_info__4a9583a93fcf93f8_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__4a9583a93fcf93f8_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4a9583a93fcf93f8_arg_types_var_3257300768127542091, __type_info__4a9583a93fcf93f8_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x4a9583a93fcf93f8), "afterGlobalVariables", offsetof(ast::AstCommentReader,afterGlobalVariables), 0 };
TypeInfo * __type_info__c76262ce21036f69_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__c76262ce21036f69_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c76262ce21036f69_arg_types_var_3257300768127542091, __type_info__c76262ce21036f69_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xc76262ce21036f69), "beforeVariant", offsetof(ast::AstCommentReader,beforeVariant), 0 };
TypeInfo * __type_info__bf71fe47965078d0_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__bf71fe47965078d0_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bf71fe47965078d0_arg_types_var_3257300768127542091, __type_info__bf71fe47965078d0_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xbf71fe47965078d0), "beforeVariantEntries", offsetof(ast::AstCommentReader,beforeVariantEntries), 0 };
TypeInfo * __type_info__b30478b93b50293b_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__b30478b93b50293b_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b30478b93b50293b_arg_types_var_3257300768127542091, __type_info__b30478b93b50293b_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xb30478b93b50293b), "afterVariantEntry", offsetof(ast::AstCommentReader,afterVariantEntry), 0 };
TypeInfo * __type_info__e31a6ac01cc43ae8_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__e31a6ac01cc43ae8_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e31a6ac01cc43ae8_arg_types_var_3257300768127542091, __type_info__e31a6ac01cc43ae8_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xe31a6ac01cc43ae8), "afterVariantEntries", offsetof(ast::AstCommentReader,afterVariantEntries), 0 };
TypeInfo * __type_info__10fa9a2d00e4f0d9_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__10fa9a2d00e4f0d9_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__10fa9a2d00e4f0d9_arg_types_var_3257300768127542091, __type_info__10fa9a2d00e4f0d9_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x10fa9a2d00e4f0d9), "afterVariant", offsetof(ast::AstCommentReader,afterVariant), 0 };
TypeInfo * __type_info__a0a4cb037ec11d4f_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__a0a4cb037ec11d4f_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a0a4cb037ec11d4f_arg_types_var_3257300768127542091, __type_info__a0a4cb037ec11d4f_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xa0a4cb037ec11d4f), "beforeTuple", offsetof(ast::AstCommentReader,beforeTuple), 0 };
TypeInfo * __type_info__d78534af84b71b42_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__d78534af84b71b42_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d78534af84b71b42_arg_types_var_3257300768127542091, __type_info__d78534af84b71b42_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xd78534af84b71b42), "beforeTupleEntries", offsetof(ast::AstCommentReader,beforeTupleEntries), 0 };
TypeInfo * __type_info__2e58eaa3710e51e6_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__2e58eaa3710e51e6_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2e58eaa3710e51e6_arg_types_var_3257300768127542091, __type_info__2e58eaa3710e51e6_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x2e58eaa3710e51e6), "afterTupleEntry", offsetof(ast::AstCommentReader,afterTupleEntry), 0 };
TypeInfo * __type_info__2b6148b96979359f_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__2b6148b96979359f_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2b6148b96979359f_arg_types_var_3257300768127542091, __type_info__2b6148b96979359f_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x2b6148b96979359f), "afterTupleEntries", offsetof(ast::AstCommentReader,afterTupleEntries), 0 };
TypeInfo * __type_info__528d468f215df29a_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__528d468f215df29a_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_24 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__528d468f215df29a_arg_types_var_3257300768127542091, __type_info__528d468f215df29a_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x528d468f215df29a), "afterTuple", offsetof(ast::AstCommentReader,afterTuple), 0 };
TypeInfo * __type_info__bb1a66ab837919c7_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__bb1a66ab837919c7_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_25 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__bb1a66ab837919c7_arg_types_var_3257300768127542091, __type_info__bb1a66ab837919c7_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xbb1a66ab837919c7), "beforeBitfield", offsetof(ast::AstCommentReader,beforeBitfield), 0 };
TypeInfo * __type_info__5232a29f2f2b0bda_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__5232a29f2f2b0bda_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_26 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5232a29f2f2b0bda_arg_types_var_3257300768127542091, __type_info__5232a29f2f2b0bda_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x5232a29f2f2b0bda), "beforeBitfieldEntries", offsetof(ast::AstCommentReader,beforeBitfieldEntries), 0 };
TypeInfo * __type_info__c5d893ae95f885c5_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__c5d893ae95f885c5_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_27 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__c5d893ae95f885c5_arg_types_var_3257300768127542091, __type_info__c5d893ae95f885c5_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xc5d893ae95f885c5), "afterBitfieldEntry", offsetof(ast::AstCommentReader,afterBitfieldEntry), 0 };
TypeInfo * __type_info__5df991a9039a81e0_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__5df991a9039a81e0_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_28 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5df991a9039a81e0_arg_types_var_3257300768127542091, __type_info__5df991a9039a81e0_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x5df991a9039a81e0), "afterBitfieldEntries", offsetof(ast::AstCommentReader,afterBitfieldEntries), 0 };
TypeInfo * __type_info__5183b72cf29c5739_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__5183b72cf29c5739_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_29 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5183b72cf29c5739_arg_types_var_3257300768127542091, __type_info__5183b72cf29c5739_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x5183b72cf29c5739), "afterBitfield", offsetof(ast::AstCommentReader,afterBitfield), 0 };
TypeInfo * __type_info__757869e405e55183_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__757869e405e55183_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_30 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__757869e405e55183_arg_types_var_3257300768127542091, __type_info__757869e405e55183_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x757869e405e55183), "beforeEnumeration", offsetof(ast::AstCommentReader,beforeEnumeration), 0 };
TypeInfo * __type_info__31b2841aaba2a26e_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__31b2841aaba2a26e_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_31 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__31b2841aaba2a26e_arg_types_var_3257300768127542091, __type_info__31b2841aaba2a26e_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x31b2841aaba2a26e), "beforeEnumerationEntries", offsetof(ast::AstCommentReader,beforeEnumerationEntries), 0 };
TypeInfo * __type_info__ae7ad5d14d65e87_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__ae7ad5d14d65e87_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_32 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ae7ad5d14d65e87_arg_types_var_3257300768127542091, __type_info__ae7ad5d14d65e87_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xae7ad5d14d65e87), "afterEnumerationEntry", offsetof(ast::AstCommentReader,afterEnumerationEntry), 0 };
TypeInfo * __type_info__69589294474e92c_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__69589294474e92c_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_33 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__69589294474e92c_arg_types_var_3257300768127542091, __type_info__69589294474e92c_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0x69589294474e92c), "afterEnumerationEntries", offsetof(ast::AstCommentReader,afterEnumerationEntries), 0 };
TypeInfo * __type_info__d04e4c5995a62f2d_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__d04e4c5995a62f2d_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_34 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d04e4c5995a62f2d_arg_types_var_3257300768127542091, __type_info__d04e4c5995a62f2d_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0xd04e4c5995a62f2d), "afterEnumeration", offsetof(ast::AstCommentReader,afterEnumeration), 0 };
TypeInfo * __type_info__feae167dcb326df3_arg_types_var_3257300768127542091[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__feae167dcb326df3_arg_names_var_3257300768127542091[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_35 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__feae167dcb326df3_arg_types_var_3257300768127542091, __type_info__feae167dcb326df3_arg_names_var_3257300768127542091, 4, 0, nullptr, 12, 8, UINT64_C(0xfeae167dcb326df3), "beforeAlias", offsetof(ast::AstCommentReader,beforeAlias), 0 };
TypeInfo * __type_info__68b72f5a533c2d84_arg_types_var_3257300768127542091[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__68b72f5a533c2d84_arg_names_var_3257300768127542091[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__2d3441ca6ad9634b_field_36 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__68b72f5a533c2d84_arg_types_var_3257300768127542091, __type_info__68b72f5a533c2d84_arg_names_var_3257300768127542091, 5, 0, nullptr, 12, 8, UINT64_C(0x68b72f5a533c2d84), "afterAlias", offsetof(ast::AstCommentReader,afterAlias), 0 };
VarInfo * __struct_info__2d3441ca6ad9634b_fields[37] =  { &__struct_info__2d3441ca6ad9634b_field_0, &__struct_info__2d3441ca6ad9634b_field_1, &__struct_info__2d3441ca6ad9634b_field_2, &__struct_info__2d3441ca6ad9634b_field_3, &__struct_info__2d3441ca6ad9634b_field_4, &__struct_info__2d3441ca6ad9634b_field_5, &__struct_info__2d3441ca6ad9634b_field_6, &__struct_info__2d3441ca6ad9634b_field_7, &__struct_info__2d3441ca6ad9634b_field_8, &__struct_info__2d3441ca6ad9634b_field_9, &__struct_info__2d3441ca6ad9634b_field_10, &__struct_info__2d3441ca6ad9634b_field_11, &__struct_info__2d3441ca6ad9634b_field_12, &__struct_info__2d3441ca6ad9634b_field_13, &__struct_info__2d3441ca6ad9634b_field_14, &__struct_info__2d3441ca6ad9634b_field_15, &__struct_info__2d3441ca6ad9634b_field_16, &__struct_info__2d3441ca6ad9634b_field_17, &__struct_info__2d3441ca6ad9634b_field_18, &__struct_info__2d3441ca6ad9634b_field_19, &__struct_info__2d3441ca6ad9634b_field_20, &__struct_info__2d3441ca6ad9634b_field_21, &__struct_info__2d3441ca6ad9634b_field_22, &__struct_info__2d3441ca6ad9634b_field_23, &__struct_info__2d3441ca6ad9634b_field_24, &__struct_info__2d3441ca6ad9634b_field_25, &__struct_info__2d3441ca6ad9634b_field_26, &__struct_info__2d3441ca6ad9634b_field_27, &__struct_info__2d3441ca6ad9634b_field_28, &__struct_info__2d3441ca6ad9634b_field_29, &__struct_info__2d3441ca6ad9634b_field_30, &__struct_info__2d3441ca6ad9634b_field_31, &__struct_info__2d3441ca6ad9634b_field_32, &__struct_info__2d3441ca6ad9634b_field_33, &__struct_info__2d3441ca6ad9634b_field_34, &__struct_info__2d3441ca6ad9634b_field_35, &__struct_info__2d3441ca6ad9634b_field_36 };
StructInfo __struct_info__2d3441ca6ad9634b = {"AstCommentReader", "ast", 13, __struct_info__2d3441ca6ad9634b_fields, 37, 296, UINT64_C(0x0), nullptr, UINT64_C(0x2d3441ca6ad9634b), 0 };
VarInfo __struct_info__61a70b56f6923094_field_0 =  { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, nullptr, nullptr, 0, 0, nullptr, 8204, 8, UINT64_C(0xf8aedcc0764220be), "__rtti", offsetof(rst_comment::RstComment,__rtti), 37 };
TypeInfo * __type_info__33c4b1e56cee6ffe_arg_types_var_7036605410921558164[1] = { &__type_info__8ef87efe522b67be };
const char * __type_info__33c4b1e56cee6ffe_arg_names_var_7036605410921558164[1] = { "self" };
VarInfo __struct_info__61a70b56f6923094_field_1 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__33c4b1e56cee6ffe_arg_types_var_7036605410921558164, __type_info__33c4b1e56cee6ffe_arg_names_var_7036605410921558164, 1, 0, nullptr, 12, 8, UINT64_C(0x33c4b1e56cee6ffe), "__finalize", offsetof(rst_comment::RstComment,__finalize), 0 };
TypeInfo * __type_info__605760cd12e599b7_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__af81fe4c86352052, &__type_info__246dda13a8a4b104 };
const char * __type_info__605760cd12e599b7_arg_names_var_7036605410921558164[5] = { "self", "prog", "mod", "cpp", "info" };
VarInfo __struct_info__61a70b56f6923094_field_2 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__605760cd12e599b7_arg_types_var_7036605410921558164, __type_info__605760cd12e599b7_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x605760cd12e599b7), "open", offsetof(rst_comment::RstComment,open), 0 };
TypeInfo * __type_info__42f8e9c07e03fb19_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__af8afe4c86446b52, &__type_info__246dda13a8a4b104 };
const char * __type_info__42f8e9c07e03fb19_arg_names_var_7036605410921558164[5] = { "self", "prog", "mod", "ch", "info" };
VarInfo __struct_info__61a70b56f6923094_field_3 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__42f8e9c07e03fb19_arg_types_var_7036605410921558164, __type_info__42f8e9c07e03fb19_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x42f8e9c07e03fb19), "accept", offsetof(rst_comment::RstComment,accept), 0 };
TypeInfo * __type_info__f37e96b25df53e04_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__f37e96b25df53e04_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_4 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f37e96b25df53e04_arg_types_var_7036605410921558164, __type_info__f37e96b25df53e04_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xf37e96b25df53e04), "close", offsetof(rst_comment::RstComment,close), 0 };
TypeInfo * __type_info__a6ea310bed575ee9_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__a6ea310bed575ee9_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_5 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a6ea310bed575ee9_arg_types_var_7036605410921558164, __type_info__a6ea310bed575ee9_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xa6ea310bed575ee9), "beforeStructure", offsetof(rst_comment::RstComment,beforeStructure), 0 };
TypeInfo * __type_info__b916cd9a6cc2e2dd_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__d6b8ed05d16e9f27, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__b916cd9a6cc2e2dd_arg_names_var_7036605410921558164[5] = { "self", "st", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_6 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b916cd9a6cc2e2dd_arg_types_var_7036605410921558164, __type_info__b916cd9a6cc2e2dd_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xb916cd9a6cc2e2dd), "afterStructure", offsetof(rst_comment::RstComment,afterStructure), 0 };
TypeInfo * __type_info__4a78fc6745eced35_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__4a78fc6745eced35_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_7 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__4a78fc6745eced35_arg_types_var_7036605410921558164, __type_info__4a78fc6745eced35_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x4a78fc6745eced35), "beforeStructureFields", offsetof(rst_comment::RstComment,beforeStructureFields), 0 };
TypeInfo * __type_info__a9802ca611fc378_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__a9802ca611fc378_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_8 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a9802ca611fc378_arg_types_var_7036605410921558164, __type_info__a9802ca611fc378_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xa9802ca611fc378), "afterStructureField", offsetof(rst_comment::RstComment,afterStructureField), 0 };
TypeInfo * __type_info__a8702ca6102e078_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__a8702ca6102e078_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_9 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a8702ca6102e078_arg_types_var_7036605410921558164, __type_info__a8702ca6102e078_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xa8702ca6102e078), "afterStructureFields", offsetof(rst_comment::RstComment,afterStructureFields), 0 };
TypeInfo * __type_info__a7ee791b51b3dc66_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__a7ee791b51b3dc66_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_10 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__a7ee791b51b3dc66_arg_types_var_7036605410921558164, __type_info__a7ee791b51b3dc66_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xa7ee791b51b3dc66), "beforeFunction", offsetof(rst_comment::RstComment,beforeFunction), 0 };
TypeInfo * __type_info__ddf94e4fc3dcca4b_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__9d10785eb07580e0, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__ddf94e4fc3dcca4b_arg_names_var_7036605410921558164[5] = { "self", "fn", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_11 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ddf94e4fc3dcca4b_arg_types_var_7036605410921558164, __type_info__ddf94e4fc3dcca4b_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xddf94e4fc3dcca4b), "afterFunction", offsetof(rst_comment::RstComment,afterFunction), 0 };
TypeInfo * __type_info__6adda5e9b28e8a2a_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__6adda5e9b28e8a2a_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_12 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6adda5e9b28e8a2a_arg_types_var_7036605410921558164, __type_info__6adda5e9b28e8a2a_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x6adda5e9b28e8a2a), "beforeGlobalVariables", offsetof(rst_comment::RstComment,beforeGlobalVariables), 0 };
TypeInfo * __type_info__7be23cab45034901_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__7be23cab45034901_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_13 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7be23cab45034901_arg_types_var_7036605410921558164, __type_info__7be23cab45034901_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x7be23cab45034901), "afterGlobalVariable", offsetof(rst_comment::RstComment,afterGlobalVariable), 0 };
TypeInfo * __type_info__7c153cab4559f201_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__7c153cab4559f201_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_14 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7c153cab4559f201_arg_types_var_7036605410921558164, __type_info__7c153cab4559f201_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x7c153cab4559f201), "afterGlobalVariables", offsetof(rst_comment::RstComment,afterGlobalVariables), 0 };
TypeInfo * __type_info__589d703e65694ea_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__589d703e65694ea_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_15 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__589d703e65694ea_arg_types_var_7036605410921558164, __type_info__589d703e65694ea_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x589d703e65694ea), "beforeVariant", offsetof(rst_comment::RstComment,beforeVariant), 0 };
TypeInfo * __type_info__6f8b6a84bfea6d0d_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__6f8b6a84bfea6d0d_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_16 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6f8b6a84bfea6d0d_arg_types_var_7036605410921558164, __type_info__6f8b6a84bfea6d0d_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x6f8b6a84bfea6d0d), "beforeVariantEntries", offsetof(rst_comment::RstComment,beforeVariantEntries), 0 };
TypeInfo * __type_info__b7fdda04287af828_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__b7fdda04287af828_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_17 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__b7fdda04287af828_arg_types_var_7036605410921558164, __type_info__b7fdda04287af828_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xb7fdda04287af828), "afterVariantEntry", offsetof(rst_comment::RstComment,afterVariantEntry), 0 };
TypeInfo * __type_info__6f8cd011e69d9b31_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__6f8cd011e69d9b31_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_18 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6f8cd011e69d9b31_arg_types_var_7036605410921558164, __type_info__6f8cd011e69d9b31_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x6f8cd011e69d9b31), "afterVariantEntries", offsetof(rst_comment::RstComment,afterVariantEntries), 0 };
TypeInfo * __type_info__3323d2a5d31ac3b0_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__3323d2a5d31ac3b0_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_19 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3323d2a5d31ac3b0_arg_types_var_7036605410921558164, __type_info__3323d2a5d31ac3b0_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x3323d2a5d31ac3b0), "afterVariant", offsetof(rst_comment::RstComment,afterVariant), 0 };
TypeInfo * __type_info__64df14d74eb56d36_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__64df14d74eb56d36_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_20 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__64df14d74eb56d36_arg_types_var_7036605410921558164, __type_info__64df14d74eb56d36_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x64df14d74eb56d36), "beforeTuple", offsetof(rst_comment::RstComment,beforeTuple), 0 };
TypeInfo * __type_info__38f2cb39e0811f91_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__38f2cb39e0811f91_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_21 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__38f2cb39e0811f91_arg_types_var_7036605410921558164, __type_info__38f2cb39e0811f91_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x38f2cb39e0811f91), "beforeTupleEntries", offsetof(rst_comment::RstComment,beforeTupleEntries), 0 };
TypeInfo * __type_info__2852a6f2c657398b_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__2852a6f2c657398b_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_22 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2852a6f2c657398b_arg_types_var_7036605410921558164, __type_info__2852a6f2c657398b_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x2852a6f2c657398b), "afterTupleEntry", offsetof(rst_comment::RstComment,afterTupleEntry), 0 };
TypeInfo * __type_info__17efa68812eaf4d8_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__17efa68812eaf4d8_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_23 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__17efa68812eaf4d8_arg_types_var_7036605410921558164, __type_info__17efa68812eaf4d8_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x17efa68812eaf4d8), "afterTupleEntries", offsetof(rst_comment::RstComment,afterTupleEntries), 0 };
TypeInfo * __type_info__f72b86dfabb3d1c9_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__f72b86dfabb3d1c9_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_24 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__f72b86dfabb3d1c9_arg_types_var_7036605410921558164, __type_info__f72b86dfabb3d1c9_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xf72b86dfabb3d1c9), "afterTuple", offsetof(rst_comment::RstComment,afterTuple), 0 };
TypeInfo * __type_info__ca7f5a927daacf30_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__ca7f5a927daacf30_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_25 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__ca7f5a927daacf30_arg_types_var_7036605410921558164, __type_info__ca7f5a927daacf30_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xca7f5a927daacf30), "beforeBitfield", offsetof(rst_comment::RstComment,beforeBitfield), 0 };
TypeInfo * __type_info__7fa3df98c0363cb1_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__7fa3df98c0363cb1_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_26 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__7fa3df98c0363cb1_arg_types_var_7036605410921558164, __type_info__7fa3df98c0363cb1_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x7fa3df98c0363cb1), "beforeBitfieldEntries", offsetof(rst_comment::RstComment,beforeBitfieldEntries), 0 };
TypeInfo * __type_info__cbe8e177831433fe_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__cbe8e177831433fe_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_27 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__cbe8e177831433fe_arg_types_var_7036605410921558164, __type_info__cbe8e177831433fe_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xcbe8e177831433fe), "afterBitfieldEntry", offsetof(rst_comment::RstComment,afterBitfieldEntry), 0 };
TypeInfo * __type_info__5af7b9138dda5761_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__5af7b9138dda5761_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_28 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5af7b9138dda5761_arg_types_var_7036605410921558164, __type_info__5af7b9138dda5761_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x5af7b9138dda5761), "afterBitfieldEntries", offsetof(rst_comment::RstComment,afterBitfieldEntries), 0 };
TypeInfo * __type_info__57ca5f5d9085e4c6_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__57ca5f5d9085e4c6_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_29 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__57ca5f5d9085e4c6_arg_types_var_7036605410921558164, __type_info__57ca5f5d9085e4c6_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x57ca5f5d9085e4c6), "afterBitfield", offsetof(rst_comment::RstComment,afterBitfield), 0 };
TypeInfo * __type_info__6d2c2500829f7e58_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__6d2c2500829f7e58_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_30 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6d2c2500829f7e58_arg_types_var_7036605410921558164, __type_info__6d2c2500829f7e58_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x6d2c2500829f7e58), "beforeEnumeration", offsetof(rst_comment::RstComment,beforeEnumeration), 0 };
TypeInfo * __type_info__e9c7921be0396fcf_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__e9c7921be0396fcf_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_31 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__e9c7921be0396fcf_arg_types_var_7036605410921558164, __type_info__e9c7921be0396fcf_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0xe9c7921be0396fcf), "beforeEnumerationEntries", offsetof(rst_comment::RstComment,beforeEnumerationEntries), 0 };
TypeInfo * __type_info__d0207c205be30794_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__d0207c205be30794_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_32 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__d0207c205be30794_arg_types_var_7036605410921558164, __type_info__d0207c205be30794_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0xd0207c205be30794), "afterEnumerationEntry", offsetof(rst_comment::RstComment,afterEnumerationEntry), 0 };
TypeInfo * __type_info__2e41d1fbd4a1d815_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__2e41d1fbd4a1d815_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_33 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__2e41d1fbd4a1d815_arg_types_var_7036605410921558164, __type_info__2e41d1fbd4a1d815_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x2e41d1fbd4a1d815), "afterEnumerationEntries", offsetof(rst_comment::RstComment,afterEnumerationEntries), 0 };
TypeInfo * __type_info__6b7a89d83eb61a84_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__6b7a89d83eb61a84_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_34 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__6b7a89d83eb61a84_arg_types_var_7036605410921558164, __type_info__6b7a89d83eb61a84_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x6b7a89d83eb61a84), "afterEnumeration", offsetof(rst_comment::RstComment,afterEnumeration), 0 };
TypeInfo * __type_info__5777c9acaa501022_arg_types_var_7036605410921558164[4] = { &__type_info__8ef87efe522b67be, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__5777c9acaa501022_arg_names_var_7036605410921558164[4] = { "self", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_35 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__5777c9acaa501022_arg_types_var_7036605410921558164, __type_info__5777c9acaa501022_arg_names_var_7036605410921558164, 4, 0, nullptr, 12, 8, UINT64_C(0x5777c9acaa501022), "beforeAlias", offsetof(rst_comment::RstComment,beforeAlias), 0 };
TypeInfo * __type_info__37e37d799eff20cf_arg_types_var_7036605410921558164[5] = { &__type_info__8ef87efe522b67be, &__type_info__af90fe4c864e9d52, &__type_info__e57b0f261f47b890, &__type_info__e266b5ccef058802, &__type_info__246dda13a8a4b104 };
const char * __type_info__37e37d799eff20cf_arg_names_var_7036605410921558164[5] = { "self", "name", "prog", "mod", "info" };
VarInfo __struct_info__61a70b56f6923094_field_36 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__37e37d799eff20cf_arg_types_var_7036605410921558164, __type_info__37e37d799eff20cf_arg_names_var_7036605410921558164, 5, 0, nullptr, 12, 8, UINT64_C(0x37e37d799eff20cf), "afterAlias", offsetof(rst_comment::RstComment,afterAlias), 0 };
VarInfo __struct_info__61a70b56f6923094_field_37 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__af5be84c85f468f0, nullptr, nullptr, nullptr, 0, 0, nullptr, 40962, 24, UINT64_C(0x40e9c9da604f99c), "chars", offsetof(rst_comment::RstComment,chars), 39 };
VarInfo __struct_info__61a70b56f6923094_field_38 =  { Type::tEnumeration, nullptr, &__enum_info__4fb7b029dd403502, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 4, UINT64_C(0x53401a894a8a8bd6), "state", offsetof(rst_comment::RstComment,state), 0 };
VarInfo __struct_info__61a70b56f6923094_field_39 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xc6bb284a6fcd0ab2), "comment", offsetof(rst_comment::RstComment,comment), 40 };
VarInfo __struct_info__61a70b56f6923094_field_40 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x6c801d2c3abadb2c), "structure_comment", offsetof(rst_comment::RstComment,structure_comment), 41 };
VarInfo __struct_info__61a70b56f6923094_field_41 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xce0b213ac29a4853), "function_comment", offsetof(rst_comment::RstComment,function_comment), 42 };
VarInfo __struct_info__61a70b56f6923094_field_42 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xa388e7a92a1cdb1), "global_comment", offsetof(rst_comment::RstComment,global_comment), 43 };
VarInfo __struct_info__61a70b56f6923094_field_43 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x6bf300c2ea28357b), "field_comment", offsetof(rst_comment::RstComment,field_comment), 44 };
VarInfo __struct_info__61a70b56f6923094_field_44 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x4369dfd4910588a7), "variant_comment", offsetof(rst_comment::RstComment,variant_comment), 45 };
VarInfo __struct_info__61a70b56f6923094_field_45 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1bf54e03a1235573), "variant_entry_comment", offsetof(rst_comment::RstComment,variant_entry_comment), 46 };
VarInfo __struct_info__61a70b56f6923094_field_46 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xa49cb8007ec9dd13), "tuple_comment", offsetof(rst_comment::RstComment,tuple_comment), 47 };
VarInfo __struct_info__61a70b56f6923094_field_47 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xe5095f786fc75f97), "tuple_entry_comment", offsetof(rst_comment::RstComment,tuple_entry_comment), 48 };
VarInfo __struct_info__61a70b56f6923094_field_48 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x36c4cdff25e42b29), "bitfield_comment", offsetof(rst_comment::RstComment,bitfield_comment), 49 };
VarInfo __struct_info__61a70b56f6923094_field_49 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x5bc5a80160eadf5c), "bitfield_entry_comment", offsetof(rst_comment::RstComment,bitfield_entry_comment), 50 };
VarInfo __struct_info__61a70b56f6923094_field_50 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x1bb8e0a1b3583030), "enum_comment", offsetof(rst_comment::RstComment,enum_comment), 51 };
VarInfo __struct_info__61a70b56f6923094_field_51 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x85255ef214b627c3), "enum_entry_comment", offsetof(rst_comment::RstComment,enum_entry_comment), 52 };
VarInfo __struct_info__61a70b56f6923094_field_52 =  { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0x62a94dd3f7d4a4c7), "alias_comment", offsetof(rst_comment::RstComment,alias_comment), 53 };
VarInfo __struct_info__61a70b56f6923094_field_53 =  { Type::tArray, nullptr, nullptr, nullptr, &__type_info__f9ade15e6e3d3c78, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 24, UINT64_C(0x6be400c2ea0eb87b), "field_comments", offsetof(rst_comment::RstComment,field_comments), 55 };
TypeInfo * __type_info__3c932bdd14c864e3_arg_types_var_7036605410921558164[3] = { &__type_info__b5e7457401a3ad37, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };
const char * __type_info__3c932bdd14c864e3_arg_names_var_7036605410921558164[3] = { "self", "name", "text" };
VarInfo __struct_info__61a70b56f6923094_field_54 =  { Type::tFunction, nullptr, nullptr, nullptr, &__type_info__af63eb4c86020609, nullptr, (TypeInfo **)__type_info__3c932bdd14c864e3_arg_types_var_7036605410921558164, __type_info__3c932bdd14c864e3_arg_names_var_7036605410921558164, 3, 0, nullptr, 12, 8, UINT64_C(0x3c932bdd14c864e3), "write_to_detail", offsetof(rst_comment::RstComment,write_to_detail), 0 };
VarInfo * __struct_info__61a70b56f6923094_fields[55] =  { &__struct_info__61a70b56f6923094_field_0, &__struct_info__61a70b56f6923094_field_1, &__struct_info__61a70b56f6923094_field_2, &__struct_info__61a70b56f6923094_field_3, &__struct_info__61a70b56f6923094_field_4, &__struct_info__61a70b56f6923094_field_5, &__struct_info__61a70b56f6923094_field_6, &__struct_info__61a70b56f6923094_field_7, &__struct_info__61a70b56f6923094_field_8, &__struct_info__61a70b56f6923094_field_9, &__struct_info__61a70b56f6923094_field_10, &__struct_info__61a70b56f6923094_field_11, &__struct_info__61a70b56f6923094_field_12, &__struct_info__61a70b56f6923094_field_13, &__struct_info__61a70b56f6923094_field_14, &__struct_info__61a70b56f6923094_field_15, &__struct_info__61a70b56f6923094_field_16, &__struct_info__61a70b56f6923094_field_17, &__struct_info__61a70b56f6923094_field_18, &__struct_info__61a70b56f6923094_field_19, &__struct_info__61a70b56f6923094_field_20, &__struct_info__61a70b56f6923094_field_21, &__struct_info__61a70b56f6923094_field_22, &__struct_info__61a70b56f6923094_field_23, &__struct_info__61a70b56f6923094_field_24, &__struct_info__61a70b56f6923094_field_25, &__struct_info__61a70b56f6923094_field_26, &__struct_info__61a70b56f6923094_field_27, &__struct_info__61a70b56f6923094_field_28, &__struct_info__61a70b56f6923094_field_29, &__struct_info__61a70b56f6923094_field_30, &__struct_info__61a70b56f6923094_field_31, &__struct_info__61a70b56f6923094_field_32, &__struct_info__61a70b56f6923094_field_33, &__struct_info__61a70b56f6923094_field_34, &__struct_info__61a70b56f6923094_field_35, &__struct_info__61a70b56f6923094_field_36, &__struct_info__61a70b56f6923094_field_37, &__struct_info__61a70b56f6923094_field_38, &__struct_info__61a70b56f6923094_field_39, &__struct_info__61a70b56f6923094_field_40, &__struct_info__61a70b56f6923094_field_41, &__struct_info__61a70b56f6923094_field_42, &__struct_info__61a70b56f6923094_field_43, &__struct_info__61a70b56f6923094_field_44, &__struct_info__61a70b56f6923094_field_45, &__struct_info__61a70b56f6923094_field_46, &__struct_info__61a70b56f6923094_field_47, &__struct_info__61a70b56f6923094_field_48, &__struct_info__61a70b56f6923094_field_49, &__struct_info__61a70b56f6923094_field_50, &__struct_info__61a70b56f6923094_field_51, &__struct_info__61a70b56f6923094_field_52, &__struct_info__61a70b56f6923094_field_53, &__struct_info__61a70b56f6923094_field_54 };
StructInfo __struct_info__61a70b56f6923094 = {"RstComment", "rst_comment", 29, __struct_info__61a70b56f6923094_fields, 55, 472, UINT64_C(0x0), nullptr, UINT64_C(0x61a70b56f6923094), 0 };
TypeInfo __type_info__a8ccb9bba7fb58db = { Type::tStructure, &__struct_info__61a70b56f6923094, nullptr, nullptr, &__type_info__8ef87efe522b67be, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 472, UINT64_C(0xa8ccb9bba7fb58db) };
TypeInfo __type_info__e266b5ccef058802 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__8afce1a80940fc9e, nullptr, nullptr, nullptr, 0, 0, nullptr, 8236, 8, UINT64_C(0xe266b5ccef058802) };
TypeInfo __type_info__246dda13a8a4b104 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::LineInfo"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8230, 24, UINT64_C(0x246dda13a8a4b104) };
TypeInfo __type_info__9d10785eb07580e0 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__a57bf935c2dd03, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, 8, UINT64_C(0x9d10785eb07580e0) };
TypeInfo __type_info__e57b0f261f47b890 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__125855d9cd771ead, nullptr, nullptr, nullptr, 0, 0, nullptr, 9248, 8, UINT64_C(0xe57b0f261f47b890) };
TypeInfo __type_info__d6b8ed05d16e9f27 = { Type::tPointer, nullptr, nullptr, nullptr, &__type_info__60d16a2d23420951, nullptr, nullptr, nullptr, 0, 0, nullptr, 25632, 8, UINT64_C(0xd6b8ed05d16e9f27) };
TypeInfo __type_info__af81fe4c86352052 = { Type::tBool, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 1, UINT64_C(0xaf81fe4c86352052) };
TypeInfo __type_info__af8afe4c86446b52 = { Type::tInt, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 60, 4, UINT64_C(0xaf8afe4c86446b52) };
TypeInfo __type_info__af90fe4c864e9d52 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16420, 8, UINT64_C(0xaf90fe4c864e9d52) };
TypeInfo __type_info__a57bf935c2dd03 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Function"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 408, UINT64_C(0xa57bf935c2dd03) };
TypeInfo __type_info__60d16a2d23420951 = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~ast::Structure"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24578, 184, UINT64_C(0x60d16a2d23420951) };
TypeInfo __type_info__8afce1a80940fc9e = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Module"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 2, 1272, UINT64_C(0x8afce1a80940fc9e) };
TypeInfo __type_info__125855d9cd771ead = { Type::tHandle, nullptr, nullptr, DAS_MAKE_ANNOTATION("~rtti::Program"), nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 8194, 632, UINT64_C(0x125855d9cd771ead) };
TypeInfo * __type_info__f9ade15e6e3d3c78_arg_types[3] = { &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22, &__type_info__af63ee4c86020b22 };
const char * __type_info__f9ade15e6e3d3c78_arg_names[3] = { "kind", "name", "comment" };
TypeInfo __type_info__f9ade15e6e3d3c78 = { Type::tTuple, nullptr, nullptr, nullptr, nullptr, nullptr, (TypeInfo **)__type_info__f9ade15e6e3d3c78_arg_types, __type_info__f9ade15e6e3d3c78_arg_names, 3, 0, nullptr, 16390, 24, UINT64_C(0xf9ade15e6e3d3c78) };
TypeInfo __type_info__8ef87efe522b67be = { Type::tStructure, &__struct_info__2d3441ca6ad9634b, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 24590, 296, UINT64_C(0x8ef87efe522b67be) };
TypeInfo __type_info__b5e7457401a3ad37 = { Type::tStructure, &__struct_info__61a70b56f6923094, nullptr, nullptr, &__type_info__8ef87efe522b67be, nullptr, nullptr, nullptr, 0, 0, nullptr, 57346, 472, UINT64_C(0xb5e7457401a3ad37) };
TypeInfo __type_info__af63ee4c86020b22 = { Type::tString, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 16388, 8, UINT64_C(0xaf63ee4c86020b22) };
TypeInfo __type_info__af5be84c85f468f0 = { Type::tUInt8, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 1, UINT64_C(0xaf5be84c85f468f0) };
TypeInfo __type_info__af63eb4c86020609 = { Type::tVoid, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, nullptr, 28, 0, UINT64_C(0xaf63eb4c86020609) };

static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {__type_info__246dda13a8a4b104, __type_info__a57bf935c2dd03, __type_info__60d16a2d23420951, __type_info__8afce1a80940fc9e, __type_info__125855d9cd771ead, };
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}

TypeInfo * __tinfo_0[1] = { &__type_info__a8ccb9bba7fb58db };
TypeInfo * __tinfo_1[4] = { &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52, &__type_info__af90fe4c864e9d52 };

inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> & __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 );
inline void finalize_47949244503253d8 ( Context * __context__, AutoTuple<char *,char *,char *> & ____this_rename_at_1186_2 );
inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  & __a_rename_at_581_3 );
inline void _FuncbuiltinTickpushTick10769833213962245646_83228c404c1c5840 ( Context * __context__, TArray<ast::AstCommentReader *> & __Arr_rename_at_181_4, ast::AstCommentReader * __value_rename_at_181_5 );
inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_68ef95a75daadc0f ( Context * __context__, rst_comment::RstComment const  & __cl_rename_at_116_6 );
inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_8551a3ae50f28771 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> & __a_rename_at_1342_7 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> & __a_rename_at_1182_9 );
inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9f4e5aa8ea20038d ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> & __a_rename_at_1182_10 );
inline rst_comment::RstComment & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8b6108add31580fc ( Context * __context__, rst_comment::RstComment & __a_rename_at_50_12 );
inline smart_ptr_raw<CommentReader> _FuncastTickmake_comment_readerTick2128729122050972798_c601789b0f6f6081 ( Context * __context__, char * const  __name_rename_at_714_13, rst_comment::RstComment * __someClassPtr_rename_at_714_14 );
inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_165_16, uint8_t __value_rename_at_165_17 );
inline void _FuncbuiltinTickeraseTick16646986352019611268_8409073fa4044020 ( Context * __context__, TArray<uint8_t> & __Arr_rename_at_535_18, int32_t __at_rename_at_535_19 );
inline void _FuncbuiltinTickpushTick10769833213962245646_80924a0d6aecc2bf ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> & __Arr_rename_at_181_20, AutoTuple<char *,char *,char *> & __value_rename_at_181_21 );
inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_65bb98f9eb4f2359 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_22 );
inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_24, char * const  __mode_rename_at_12_25, Block DAS_COMMENT((void,FILE const  * const )) const  & __blk_rename_at_12_26 );
inline void _FuncastTickadd_new_comment_readerTick9279614113534996372_7ca33631a1409093 ( Context * __context__, char * const  __name_rename_at_900_28, rst_comment::RstComment * __someClassPtr_rename_at_900_29 );
inline void panic_rst_3161ddc7a131ce63 ( Context * __context__, char * const  __s_rename_at_41_31 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<bool,0x28b15bfe65ac9fe2>(__context__) = true;/*DEBUG_COMMENT_READER*/
}

inline void _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e ( Context * __context__, TArray<AutoTuple<char *,char *>> &  __Arr_rename_at_68_0, int32_t __newSize_rename_at_68_1 )
{
    builtin_array_resize(das_arg<TArray<AutoTuple<char *,char *>>>::pass(__Arr_rename_at_68_0),__newSize_rename_at_68_1,16,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void finalize_47949244503253d8 ( Context * __context__, AutoTuple<char *,char *,char *> &  ____this_rename_at_1186_2 )
{
    memset(&(____this_rename_at_1186_2), 0, TypeSize<AutoTuple<char *,char *,char *>>::size);
}

inline int32_t _FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> const  &  __a_rename_at_581_3 )
{
    return das_auto_cast<int32_t>::cast(1);
}

inline void _FuncbuiltinTickpushTick10769833213962245646_83228c404c1c5840 ( Context * __context__, TArray<ast::AstCommentReader *> &  __Arr_rename_at_181_4, ast::AstCommentReader * __value_rename_at_181_5 )
{
    das_copy(__Arr_rename_at_181_4(builtin_array_push_back(das_arg<TArray<ast::AstCommentReader *>>::pass(__Arr_rename_at_181_4),8,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_5);
}

inline StructInfo const  * _FuncrttiTickclass_infoTick15801393167907430156_68ef95a75daadc0f ( Context * __context__, rst_comment::RstComment const  &  __cl_rename_at_116_6 )
{
    return das_auto_cast<StructInfo const  *>::cast(das_cast<StructInfo const  *>::cast(((das_deref(__context__,das_cast<TypeInfo const  *>::cast(__cl_rename_at_116_6.__rtti))).getStructType())));
}

inline TArray<AutoTuple<char *,char *>> _FuncbuiltinTickto_array_moveTick3185538323411982277_8551a3ae50f28771 ( Context * __context__, TDim<AutoTuple<char *,char *>,1> &  __a_rename_at_1342_7 )
{
    TArray<AutoTuple<char *,char *>> __arr_rename_at_1344_8; das_zero(__arr_rename_at_1344_8);
    _FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e(__context__,das_arg<TArray<AutoTuple<char *,char *>>>::pass(__arr_rename_at_1344_8),1);
    das_copy(das_cast<TDim<AutoTuple<char *,char *>,1>>::cast(das_ref(__context__,__arr_rename_at_1344_8(0,__context__))),__a_rename_at_1342_7);
    return /* <- */ das_auto_cast_move<TArray<AutoTuple<char *,char *>>>::cast(__arr_rename_at_1344_8);
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb ( Context * __context__, TArray<uint8_t> &  __a_rename_at_1182_9 )
{
    builtin_array_free(das_arg<TArray<uint8_t>>::pass(__a_rename_at_1182_9),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickfinalizeTick13836114024949725080_9f4e5aa8ea20038d ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> &  __a_rename_at_1182_10 )
{
    {
        bool __need_loop_1184 = true;
        // aV: tuple<kind:string;name:string;comment:string> aka TT&
        das_iterator<TArray<AutoTuple<char *,char *,char *>>> __aV_iterator(__a_rename_at_1182_10);
        AutoTuple<char *,char *,char *> * __aV_rename_at_1184_11;
        __need_loop_1184 = __aV_iterator.first(__context__,(__aV_rename_at_1184_11)) && __need_loop_1184;
        for ( ; __need_loop_1184 ; __need_loop_1184 = __aV_iterator.next(__context__,(__aV_rename_at_1184_11)) )
        {
            finalize_47949244503253d8(__context__,das_arg<AutoTuple<char *,char *,char *>>::pass((*__aV_rename_at_1184_11)));
        }
        __aV_iterator.close(__context__,(__aV_rename_at_1184_11));
    };
    builtin_array_free(das_arg<TArray<AutoTuple<char *,char *,char *>>>::pass(__a_rename_at_1182_10),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline rst_comment::RstComment & _FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8b6108add31580fc ( Context * __context__, rst_comment::RstComment &  __a_rename_at_50_12 )
{
    das_call_interop<void>::call(&builtin_verify_locks,__context__,SimNode_AotInterop<1>(__tinfo_0,cast<rst_comment::RstComment &>::from(__a_rename_at_50_12)));
    return das_auto_cast_ref<rst_comment::RstComment &>::cast(__a_rename_at_50_12);
}

inline smart_ptr_raw<CommentReader> _FuncastTickmake_comment_readerTick2128729122050972798_c601789b0f6f6081 ( Context * __context__, char * const  __name_rename_at_714_13, rst_comment::RstComment * __someClassPtr_rename_at_714_14 )
{
    _FuncbuiltinTickpushTick10769833213962245646_83228c404c1c5840(__context__,das_arg<TArray<ast::AstCommentReader *>>::pass(das_global<TArray<ast::AstCommentReader *>,0xd806bcc7af45ea49>(__context__) /*gc_root_AstCommentReader*/),das_reinterpret<ast::AstCommentReader *>::pass(__someClassPtr_rename_at_714_14));
    StructInfo const  * __classInfo_rename_at_717_15 = ((StructInfo const  *)_FuncrttiTickclass_infoTick15801393167907430156_68ef95a75daadc0f(__context__,das_arg<rst_comment::RstComment>::pass(das_deref(__context__,__someClassPtr_rename_at_714_14))));
    return /* <- */ das_auto_cast_move<smart_ptr_raw<CommentReader>>::cast(makeCommentReader(das_auto_cast<void * const >::cast(__someClassPtr_rename_at_714_14),__classInfo_rename_at_717_15,__context__));
}

inline void _FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_165_16, uint8_t __value_rename_at_165_17 )
{
    das_copy(__Arr_rename_at_165_16(builtin_array_push_back(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_165_16),1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_165_17);
}

inline void _FuncbuiltinTickeraseTick16646986352019611268_8409073fa4044020 ( Context * __context__, TArray<uint8_t> &  __Arr_rename_at_535_18, int32_t __at_rename_at_535_19 )
{
    builtin_array_erase(das_arg<TArray<uint8_t>>::pass(__Arr_rename_at_535_18),__at_rename_at_535_19,1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void _FuncbuiltinTickpushTick10769833213962245646_80924a0d6aecc2bf ( Context * __context__, TArray<AutoTuple<char *,char *,char *>> &  __Arr_rename_at_181_20, AutoTuple<char *,char *,char *> &  __value_rename_at_181_21 )
{
    das_copy(__Arr_rename_at_181_20(builtin_array_push_back(das_arg<TArray<AutoTuple<char *,char *,char *>>>::pass(__Arr_rename_at_181_20),24,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__context__),__value_rename_at_181_21);
}

inline char * _FuncrstTickfunction_file_nameTick11623859247758201540_65bb98f9eb4f2359 ( Context * __context__, smart_ptr_raw<Function> const  __value_rename_at_69_22 )
{
    TArray<AutoTuple<char *,char *>> _temp_make_local_72_32_48; _temp_make_local_72_32_48;
    TDim<AutoTuple<char *,char *>,1> _temp_make_local_72_32_80; _temp_make_local_72_32_80;
    char * __mn_rename_at_71_23 = (char *)(((char * const )(to_das_string(__value_rename_at_69_22->name /*name*/,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))))));
    das_copy(__mn_rename_at_71_23,das_invoke_function<char * const >::invoke<char * const ,TArray<AutoTuple<char *,char *>> const  &>(__context__,nullptr,Func(__context__->fnByMangledName(/*@strings_boost::replace_multiple Cs C1<N<text;replacement>0<s;s>U>A*/ 0x510d9544ac4d5ab4)),__mn_rename_at_71_23,das_arg<TArray<AutoTuple<char *,char *>>>::pass((_temp_make_local_72_32_48 = (_FuncbuiltinTickto_array_moveTick3185538323411982277_8551a3ae50f28771(__context__,das_arg<TDim<AutoTuple<char *,char *>,1>>::pass((([&]() -> TDim<AutoTuple<char *,char *>,1>& {
        _temp_make_local_72_32_80(0,__context__) = (([&]() -> AutoTuple<char *,char *> {
            AutoTuple<char *,char *> __mkt_73;
            das_get_auto_tuple_field<char *,0,char *,char *>::get(__mkt_73) = ((char *) "?");
            das_get_auto_tuple_field<char *,1,char *,char *>::get(__mkt_73) = ((char *) "_qm_");
            return __mkt_73;
        })());
        return _temp_make_local_72_32_80;
    })()))))))));
    return das_auto_cast<char *>::cast(__mn_rename_at_71_23);
}

inline void _FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac ( Context * __context__, char * const  __name_rename_at_12_24, char * const  __mode_rename_at_12_25, Block DAS_COMMENT((void,FILE const  * const )) const  &  __blk_rename_at_12_26 )
{
    FILE const  * __f_rename_at_13_27 = ((FILE const  *)builtin_fopen(__name_rename_at_12_24,__mode_rename_at_12_25));
    das_invoke<void>::invoke<FILE const  * const >(__context__,nullptr,__blk_rename_at_12_26,__f_rename_at_13_27);
    if ( __f_rename_at_13_27 != nullptr )
    {
        builtin_fclose(__f_rename_at_13_27,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    };
}

inline void _FuncastTickadd_new_comment_readerTick9279614113534996372_7ca33631a1409093 ( Context * __context__, char * const  __name_rename_at_900_28, rst_comment::RstComment * __someClassPtr_rename_at_900_29 )
{
    smart_ptr_raw<CommentReader> __ann_rename_at_901_30; memset(&__ann_rename_at_901_30,0,sizeof(__ann_rename_at_901_30));
    /* finally */ auto __finally_900= das_finally([&](){
    das_delete_handle<smart_ptr_raw<CommentReader>>::clear(__context__,__ann_rename_at_901_30);
    /* end finally */ });
    __ann_rename_at_901_30; das_zero(__ann_rename_at_901_30); das_move(__ann_rename_at_901_30, _FuncastTickmake_comment_readerTick2128729122050972798_c601789b0f6f6081(__context__,__name_rename_at_900_28,__someClassPtr_rename_at_900_29));
    addModuleCommentReader(thisModule(__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL))),__ann_rename_at_901_30,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

inline void panic_rst_3161ddc7a131ce63 ( Context * __context__, char * const  __s_rename_at_41_31 )
{
    LineInfo _temp_make_local_43_35_32; _temp_make_local_43_35_32;
    toLog(40000,das_string_builder_temp(__context__,SimNode_AotInterop<4>(__tinfo_1, cast<char * const >::from(__s_rename_at_41_31), cast<char * const >::from(((char *) " at ")), cast<char * const >::from(((char * const )(builtin_debug_line(das_arg<LineInfo>::pass((_temp_make_local_43_35_32 = (rtti_get_line_info(1,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))),false,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)))))), cast<char * const >::from(((char *) "\n")))),__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
    builtin_throw(__s_rename_at_41_31,__context__,((LineInfoArg *)(&LineInfo::g_LineInfoNULL)));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x734e45a36711660] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickresizeTick4811697762258667383_79be43bff405345e>>();
    };
    aotLib[0xbb9cd50941a71ce6] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&finalize_47949244503253d8>>();
    };
    aotLib[0x393f34d50cd6136b] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTicklengthTick18150397773952384912_cd12b5c9a9152cf9>>();
    };
    aotLib[0x5efc799ca5255cf8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_83228c404c1c5840>>();
    };
    aotLib[0xb91dc9118b1b94ad] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrttiTickclass_infoTick15801393167907430156_68ef95a75daadc0f>>();
    };
    aotLib[0xc886291e17d1acd4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_AotCMRES<&_FuncbuiltinTickto_array_moveTick3185538323411982277_8551a3ae50f28771>>();
    };
    aotLib[0x1f8eb5798352d074] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_947eb3679f75acfb>>();
    };
    aotLib[0xa38e1903eccf01d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickfinalizeTick13836114024949725080_9f4e5aa8ea20038d>>();
    };
    aotLib[0xd1c13704b2e23709] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTick_return_with_lockcheckTick2939372000839727345_8b6108add31580fc>>();
    };
    aotLib[0x1b7b9a15993ea7d8] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickmake_comment_readerTick2128729122050972798_c601789b0f6f6081>>();
    };
    aotLib[0xcba4b4c693ad27e9] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick14133213201864676143_9fcb5919841caa67>>();
    };
    aotLib[0xb8c1a37525caa6d7] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickeraseTick16646986352019611268_8409073fa4044020>>();
    };
    aotLib[0xcd163b78e9968e88] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncbuiltinTickpushTick10769833213962245646_80924a0d6aecc2bf>>();
    };
    aotLib[0x7c119b95865a8c46] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncrstTickfunction_file_nameTick11623859247758201540_65bb98f9eb4f2359>>();
    };
    aotLib[0x7c2e30b4091b7513] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncfioTickfopenTick3937565566638487747_feeb71cd63fd8eac>>();
    };
    aotLib[0x2171884abbd589fc] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&_FuncastTickadd_new_comment_readerTick9279614113534996372_7ca33631a1409093>>();
    };
    aotLib[0xe1de6193d56181c4] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&panic_rst_3161ddc7a131ce63>>();
    };
    // [[ init script ]]
    aotLib[0x76b0efdfb9a9d6d8] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
	resolveTypeInfoAnnotations();
}

} // namespace _anon_13746017607201065299
AotListBase impl_aot_rst_comment(_anon_13746017607201065299::registerAotFunctions);
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
