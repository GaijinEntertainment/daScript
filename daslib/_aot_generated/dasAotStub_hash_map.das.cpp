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
 // require templates
 // require contracts
 // require array_boost
 // require algorithm
 // require templates_boost
 // require macro_boost
 // require defer
 // require helpers
 // require colors
 // require match
 // require utf8_utils
 // require meta_ast
 // require parser_generator
 // require parse_macro
 // require peg
 // require spoof
 // require random
 // require hash_map

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
namespace _anon_2409885462760430613 {

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
namespace helpers { struct _lambda_helpers_26_1; };
namespace match { struct MatchError; };
namespace match { struct MatchTo; };
namespace match { struct MatchMacro; };
namespace match { struct StaticMatchMacro; };
namespace match { struct MultiMatchMacro; };
namespace match { struct StaticMultiMatchMacro; };
namespace match { struct MatchAsIs; };
namespace match { struct MatchCopy; };
namespace meta_ast { struct Rule_; };
namespace meta_ast { struct Definition; };
namespace meta_ast { struct Alternative; };
namespace parser_generator { struct ParserGenerator; };
namespace parse_macro { struct MacroRule; };
namespace parse_macro { struct ParseMacro; };
namespace peg { struct ParsingError; };
namespace spoof { struct SpoofTemplateReader; };
namespace spoof { struct SpoofInvocation; };
namespace spoof { struct SpoofInstanceReader; };
namespace spoof { struct invocationParserTickid_0x0; };
namespace spoof { struct invocationParserTickid_0x1; };
namespace random { struct _lambda_random_111_1; };
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
// unused structure AstCommentReader
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
// unused structure _lambda_helpers_26_1
// unused enumeration MatchType
// unused structure MatchError
// unused structure MatchTo
// unused structure MatchMacro
// unused structure StaticMatchMacro
// unused structure MultiMatchMacro
// unused structure StaticMultiMatchMacro
// unused structure MatchAsIs
// unused structure MatchCopy
// unused structure Rule_
// unused structure Definition
// unused structure Alternative
// unused structure ParserGenerator
// unused structure MacroRule
// unused structure ParseMacro
// unused structure ParsingError
// unused structure SpoofTemplateReader
// unused structure SpoofInvocation
// unused structure SpoofInstanceReader
// unused structure invocationParser`id_0x0
// unused structure invocationParser`id_0x1
// unused structure _lambda_random_111_1


static void resolveTypeInfoAnnotations()
{
    vector<TypeInfo> annotations = {};
    for (auto& ann : annotations) {
        ann.resolveAnnotation();
    }
}


inline uint64_t hash0_9965be70952193f9 ( Context * __context__, char * const  __s_rename_at_367_0 );

void __init_script ( Context * __context__, bool __init_shared )
{
    das_global<char *,0x79f9c09d45b97f17>(__context__) = ((char *) "(FlatHashMap,KeyType,ValueType,HashFunction=hash)\n[skip_field_lock_check]\nstruct %FlatHashMap {\n    keys   : array<%KeyType>\n    hashes : array<uint64>\n    values : array<%ValueType>\n    mask : int\n    length : int\n    capacity : int\n    tombstones : int\n}\n\ndef %FlatHashMap ( data : tuple<k:%KeyType;v:%ValueType>[] ) : %FlatHashMap {\n    var self : %FlatHashMap\n    with (self) {\n        let bit = int(clz(uint(length(data))))\n        let initialSize = 1 << (32 - bit)\n        mask = initialSize - 1\n        length = 0\n        capacity = initialSize\n        keys |> resize(initialSize)\n        hashes |> resize(initialSize)\n        values |> resize(initialSize)\n        for (kv in data) {\n            self[kv.k] := kv.v\n        }\n    }\n    return <- self\n}\n\ndef %FlatHashMap ( data : array<tuple<k:%KeyType;v:%ValueType>> ) : %FlatHashMap {\n    var self : %FlatHashMap\n    with (self) {\n        let bit = int(clz(uint(length(data))))\n        let initialSize = 1 << (32 - bit)\n        mask = initialSize - 1\n        length = 0\n        capacity = initialSize\n        keys |> resize(initialSize)\n        hashes |> resize(initialSize)\n        values |> resize(initialSize)\n        for (kv in data) {\n            self[kv.k] := kv.v\n        }\n    }\n    return <- self\n}\n\ndef %FlatHashMap ( initialSize : int = 8 ) : %FlatHashMap {\n    assert((initialSize & (initialSize-1))==0, \"size must be a power of 2\")\n    var self : %FlatHashMap\n    with (self) {\n        capacity = initialSize\n        keys |> resize(initialSize)\n        hashes |> resize(initialSize)\n        values |> resize(initialSize)\n        mask = initialSize - 1\n    }\n    return <- self\n}\n\ndef empty ( var self:%FlatHashMap explicit ) : bool {\n    return self.length == 0\n}\n\ndef length ( var self:%FlatHashMap explicit ) : int {\n    return self.length\n}\n\ndef clear ( var self:%FlatHashMap explicit ) {\n    with (self) {\n        for (h in hashes) {\n            h = 0ul\n        }\n        length = 0\n        tombstones = 0\n    }\n}\n\ndef grow ( var self:%FlatHashMap explicit ) {\n    with (self) {\n        let newSize = capacity * 2\n        self |> reserve(newSize)\n    }\n}\n\ndef rehash ( var self:%FlatHashMap explicit ) {\n    with (self) {\n        self |> reserve(capacity)\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef reserve ( var self:%FlatHashMap explicit; newSize : int ) {\n    assert((newSize & (newSize-1))==0, \"size must be a power of 2\")\n    with (self) {\n        var newKeys : array<%KeyType>\n        var newHashes : array<uint64>\n        var newValues : array<%ValueType>\n        newKeys |> resize(newSize)\n        newHashes |> resize(newSize)\n        newValues |> resize(newSize)\n        mask = newSize - 1\n        swap(newKeys, keys)\n        swap(newHashes, hashes)\n        swap(newValues, values)\n        length = 0\n        tombstones = 0\n        capacity = newSize\n        for (k,v,h in newKeys, newValues, newHashes) {\n            if (h > 1ul) {\n                self[k] <- v\n            }\n        }\n        newKeys |> resize(0)\n        newHashes |> resize(0)\n        newValues |> resize(0)\n        delete newKeys\n        delete newHashes\n        delete newValues\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef key_index ( self:%FlatHashMap explicit; key : %KeyType ) {\n    with (self) {\n        let hash = %HashFunction(key)\n        var index = int(hash) & mask\n        while (true) {\n            let h = hashes[index]\n            if (h == hash) {\n                if (keys[index] == key) {   // otherwise skip matching hash collision\n                    return index\n                }\n            } elif (h==0ul) {                 // empty, key not found\n                break\n            }\n            // skip hash collision or tombstone(1)\n            index = (index + 1) & mask\n        }\n        return -1\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef key_exists ( self:%FlatHashMap explicit; key : %KeyType ) {\n    with (self) {\n        let index = self |> key_index(key)\n        return index != -1\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef get ( var self:%FlatHashMap ==const explicit; key : %KeyType; blk:block<(var v:%ValueType):void> ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            blk |> invoke(values[index])\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef get ( self:%FlatHashMap ==const explicit; key : %KeyType; blk:block<(v:%ValueType):void> ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            blk |> invoke(values[index])\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef erase ( var self:%FlatHashMap explicit; key : %KeyType ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            hashes[index] = 1ul   // tombstone\n            tombstones ++\n            length --\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef foreach ( self:%FlatHashMap ==const explicit; blk:block<(k:%KeyType;v:%ValueType):void> ) {\n    with (self) {\n        for (k,v,h in keys, values, hashes) {\n            if (h > 1ul) {\n                blk |> invoke(k,v)\n            }\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef foreach ( var self:%FlatHashMap ==const explicit; blk:block<(k:%KeyType;var v:%ValueType):void> ) {\n    with (self) {\n        for (k,v,h in keys, values, hashes) {\n            if (h > 1ul) {\n                blk |> invoke(k,v)\n            }\n        }\n    }\n}\n\n[unsafe_outside_of_for]\ndef keys ( var self:%FlatHashMap explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%KeyType&>() <| $() {\n        for (h,k in pself.hashes, pself.keys) {\n            if (h > 1ul) {\n                yield k\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[unsafe_outside_of_for]\ndef values ( var self:%FlatHashMap ==const explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%ValueType&>() <| $() {\n        for (h,v in pself.hashes, pself.values) {\n            if (h > 1ul) {\n                yield v\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[unsafe_outside_of_for]\ndef values ( self:%FlatHashMap ==const explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%ValueType const&>() <| $() {\n        for (h,v in pself.hashes, pself.values) {\n            if (h > 1ul) {\n                yield v\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator [] ( var self:%FlatHashMap ==const explicit; key : %KeyType ) : %ValueType& {\n    with (self) {\n        if (length > (capacity>>1)) { // ((mask+1)*2/3)\n            self |> grow()\n        } elif (((capacity - length)>>1) < tombstones) {\n            self |> rehash()\n        }\n        let hash = %HashFunction(key)\n        var index = int(hash) & mask\n        var lastTombstone = -1\n        while (true) {\n            let h = hashes[index]\n            if (h == hash) {\n                if (keys[index] == key) {   // otherwise skip matching hash collision\n                    unsafe {\n                        return values[index]\n                    }\n                }\n            } elif (h == 1ul) {\n                if (lastTombstone == -1) {\n                    lastTombstone = index\n                }\n            } elif (h == 0ul) {\n                if (lastTombstone != -1) {\n                    index = lastTombstone\n                    tombstones --\n                }\n                keys[index] := key\n                hashes[index] = hash\n                length ++\n                unsafe {\n                    return values[index]\n                }\n            }\n            index = (index + 1) & mask\n        }\n        panic(\"unreachable\")\n        unsafe {\n            return values[0] // we never get here due to panic\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator [] ( self:%FlatHashMap ==const explicit; key : %KeyType ) : %ValueType const& {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            unsafe {\n                return values[index]\n            }\n        }\n        panic(\"key not found\")\n        unsafe {\n            return values[0] // we never get here due to panic\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator ?[] ( var self:%FlatHashMap ==const explicit; key : %KeyType ) : %ValueType? {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            return unsafe(addr(values[index]))\n        } else {\n            return null\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator ?[] ( self:%FlatHashMap ==const explicit; key : %KeyType ) : %ValueType const? {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            return unsafe(addr(values[index]))\n        } else {\n            return null\n        }\n    }\n}\n");/*TFlatHashMap*/
    das_global<char *,0xaa4d5a8a082403a8>(__context__) = ((char *) "(CuckooHashMap,KeyType,ValueType,FirstHashFunction=hash0,SecondHashFunction=hash)\n\nstruct KHV_%CuckooHashMap {\n    key : %KeyType;\n    hash : uint64;\n    value : %ValueType;\n}\n\n[skip_field_lock_check]\nstruct %CuckooHashMap {\n    khv : array<KHV_%CuckooHashMap>\n    mask : int\n    length : int\n    seed : int4\n}\n\ndef %CuckooHashMap ( data : tuple<k:%KeyType;v:%ValueType>[] ) : %CuckooHashMap {\n    var self : %CuckooHashMap\n    with (self) {\n        seed = random_seed(13)\n        let bit = int(clz(uint(length(data))))\n        let initialSize = 1 << (32 - bit)\n        mask = initialSize - 1\n        length = 0\n        khv |> resize(initialSize)\n        for (kv in data) {\n            self[kv.k] := kv.v\n        }\n    }\n    return <- self\n}\n\ndef %CuckooHashMap ( data : array<tuple<k:%KeyType;v:%ValueType>> ) : %CuckooHashMap {\n    var self : %CuckooHashMap\n    with (self) {\n        seed = random_seed(13)\n        let bit = int(clz(uint(length(data))))\n        let initialSize = 1 << (32 - bit)\n        mask = initialSize - 1\n        length = 0\n        khv |> resize(initialSize)\n        for (kv in data) {\n            self[kv.k] := kv.v\n        }\n    }\n    return <- self\n}\n\ndef %CuckooHashMap ( initialSize : int = 64 ) : %CuckooHashMap {\n    assert((initialSize & (initialSize-1))==0, \"size must be a power of 2\")\n    var self : %CuckooHashMap\n    with (self) {\n        seed = random_seed(13)\n        khv |> resize(initialSize)\n        mask = initialSize - 1\n    }\n    return <- self\n}\n\ndef empty ( var self:%CuckooHashMap explicit ) : bool {\n    return self.length == 0\n}\n\ndef length ( var self:%CuckooHashMap explicit ) : int {\n    return self.length\n}\n\ndef clear ( var self:%CuckooHashMap explicit ) {\n    with (self) {\n        for (t in khv) {\n            t.hash = 0ul\n        }\n        length = 0\n    }\n}\n\ndef grow ( var self:%CuckooHashMap explicit ) {\n    with (self) {\n        let newSize = length(khv) * 2\n        self |> reserve(newSize)\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef reserve ( var self:%CuckooHashMap explicit; newSize : int ) {\n    assert((newSize & (newSize-1))==0, \"size must be a power of 2\")\n    with (self) {\n        var newKHV : array<KHV_%CuckooHashMap>\n        newKHV |> resize(newSize)\n        mask = newSize - 1\n        swap(newKHV, khv)\n        length = 0\n        for (t in newKHV) {\n            if (t.hash != 0ul) {\n                self[t.key] <- t.value\n            }\n        }\n        newKHV |> resize(0)\n        delete newKHV\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef key_index ( self:%CuckooHashMap explicit; key : %KeyType ) {\n    with (self) {\n        let firstHash = %FirstHashFunction(key)\n        var index = int(firstHash) & mask\n        if (khv[index].hash == firstHash && khv[index].key == key) {\n            return index\n        }\n        var secondHash = %SecondHashFunction(key)\n        index = int(secondHash) & mask\n        if (khv[index].hash == secondHash && khv[index].key == key) {\n            return index\n        }\n        return -1\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef key_exists ( self:%CuckooHashMap explicit; key : %KeyType ) {\n    with (self) {\n        let index = self |> key_index(key)\n        return index != -1\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef get ( var self:%CuckooHashMap ==const explicit; key : %KeyType; blk:block<(var v:%ValueType):void> ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            blk |> invoke(khv[index].value)\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef get ( self:%CuckooHashMap ==const explicit; key : %KeyType; blk:block<(v:%ValueType):void> ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            blk |> invoke(khv[index].value)\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef erase ( var self:%CuckooHashMap explicit; key : %KeyType ) : bool {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            khv[index].hash = 0ul\n            length --\n            return true\n        } else {\n            return false\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef foreach ( self:%CuckooHashMap ==const explicit; blk:block<(k:%KeyType;v:%ValueType):void> ) {\n    with (self) {\n        for (t in khv) {\n            if (t.hash != 0ul) {\n                blk |> invoke(t.key,t.value)\n            }\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef foreach ( var self:%CuckooHashMap ==const explicit; blk:block<(k:%KeyType;var v:%ValueType):void> ) {\n    with (self) {\n        for (t in khv) {\n            if (t.hash != 0ul) {\n                blk |> invoke(t.key,t.value)\n            }\n        }\n    }\n}\n\n[unsafe_outside_of_for]\ndef keys ( var self:%CuckooHashMap explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%KeyType&>() <| $() {\n        for (t in pself.khv) {\n            if (t.hash != 0ul) {\n                yield t.key\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[unsafe_outside_of_for]\ndef values ( var self:%CuckooHashMap ==const explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%ValueType&>() <| $() {\n        for (t in pself.khv) {\n            if (t.hash != 0ul) {\n                yield t.value\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[unsafe_outside_of_for]\ndef values ( self:%CuckooHashMap ==const explicit ) {\n    var pself = unsafe(addr(self))\n    return <- generator<%ValueType const&>() <| $() {\n        for (t in pself.khv) {\n            if (t.hash != 0ul) {\n                yield t.value\n            }\n        }\n        return false\n    } finally {\n        pself = null\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef insert ( var self:%CuckooHashMap explicit; _key : %KeyType; _value : %ValueType ) {\n    //! insert a key value pair into the hash map\n    with (self) {\n        var key := _key\n        var value := _value\n        while (true) {\n            for (i in range(length(khv)/2)) {\n                var firstHash = %FirstHashFunction(key)\n                var firstIndex = int(firstHash) & mask\n                if (khv[firstIndex].hash == 0ul) {\n                    khv[firstIndex].hash = firstHash\n                    khv[firstIndex].key <- key\n                    khv[firstIndex].value <- value\n                    length ++\n                    return\n                }\n                var secondHash = %SecondHashFunction(key)\n                var secondIndex = int(secondHash) & mask\n                if (khv[secondIndex].hash == 0ul) {\n                    khv[secondIndex].hash = secondHash\n                    khv[secondIndex].key <- key\n                    khv[secondIndex].value <- value\n                    length ++\n                    return\n                }\n                if ((random_int(seed) & 1) == 0) {\n                    swap(key, khv[firstIndex].key)\n                    swap(value, khv[firstIndex].value)\n                    khv[firstIndex].hash = firstHash\n                } else {\n                    swap(key, khv[secondIndex].key)\n                    swap(value, khv[secondIndex].value)\n                    khv[secondIndex].hash = secondHash\n                }\n            }\n            self |> grow()\n        }\n    }\n}\n\ndef describe ( var self:%CuckooHashMap ==const explicit ) {\n    with (self) {\n        return build_string <| $ ( writer ) {\n            writer |> write(\"\\{\\{\\n\")\n            for (t,index in khv,count()) {\n                if (t.hash != 0ul) {\n                    writer |> write(t.key)\n                    writer |> write(\"=>\")\n                    writer |> write(t.value)\n                    writer |> write(index)\n                    writer |> write(\";\")\n                }\n            }\n            writer |> write(\"\\}\\}\\n\")\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator [] ( var self:%CuckooHashMap ==const explicit; key : %KeyType ) : %ValueType& {\n    with (self) {\n        if (length > ((mask+1)>>1)) { // ((mask+1)*2/3)\n            self |> grow()\n        }\n        // if its already there\n        var firstHash = %FirstHashFunction(key)\n        var firstIndex = int(firstHash) & mask\n        if (khv[firstIndex].hash == firstHash && khv[firstIndex].key == key) {\n            unsafe {\n                return khv[firstIndex].value\n            }\n        }\n        var secondHash = %SecondHashFunction(key)\n        var secondIndex = int(secondHash) & mask\n        if (khv[secondIndex].hash == secondHash && khv[secondIndex].key == key) {\n            unsafe {\n                return khv[secondIndex].value\n            }\n        }\n        // now, if there is a room to insert\n        if (khv[firstIndex].hash == 0ul) {\n            khv[firstIndex].hash = firstHash\n            khv[firstIndex].key := key\n            length ++\n            unsafe {\n                return khv[firstIndex].value\n            }\n        }\n        if (khv[secondIndex].hash == 0ul) {\n            khv[secondIndex].hash = secondHash\n            khv[secondIndex].key := key\n            length ++\n            unsafe {\n                return khv[secondIndex].value\n            }\n        }\n        // now we have to kick out one of the existing keys\n        self |> insert(key, default<%ValueType>)\n        let index = self |> key_index(key)\n        unsafe {\n            return khv[index].value\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator [] ( self:%CuckooHashMap ==const explicit; key : %KeyType ) : %ValueType const& {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            unsafe {\n                return khv[index].value\n            }\n        }\n        panic(\"key not found\")\n        unsafe {\n            return khv[0].value // we never get here due to panic\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator ?[] ( var self:%CuckooHashMap ==const explicit; key : %KeyType ) : %ValueType? {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            return unsafe(addr(khv[index].value))\n        } else {\n            return null\n        }\n    }\n}\n\n[hint(unsafe_range_check,noalias=self)]\ndef operator ?[] ( self:%CuckooHashMap ==const explicit; key : %KeyType ) : %ValueType const? {\n    with (self) {\n        let index = self |> key_index(key)\n        if (index != -1) {\n            return unsafe(addr(khv[index].value))\n        } else {\n            return null\n        }\n    }\n}\n");/*TCuckooHashMap*/
}

inline uint64_t hash0_9965be70952193f9 ( Context * __context__, char * const  __s_rename_at_367_0 )
{
    uint64_t __a_rename_at_370_1 = UINT64_C(0x1);
    uint64_t __b_rename_at_371_2 = UINT64_C(0x0);
    {
        bool __need_loop_372 = true;
        // c: int const
        das_iterator<char * const > __c_iterator(__s_rename_at_367_0);
        int32_t __c_rename_at_372_3;
        __need_loop_372 = __c_iterator.first(__context__,(__c_rename_at_372_3)) && __need_loop_372;
        for ( ; __need_loop_372 ; __need_loop_372 = __c_iterator.next(__context__,(__c_rename_at_372_3)) )
        {
            das_copy(__a_rename_at_370_1,SimPolicy<uint64_t>::Mod((__a_rename_at_370_1 + uint64_t(__c_rename_at_372_3)),UINT64_C(0xffffffffffffffc5),*__context__,nullptr));
            das_copy(__b_rename_at_371_2,SimPolicy<uint64_t>::Mod((__b_rename_at_371_2 + __a_rename_at_370_1),UINT64_C(0xffffffffffffffc5),*__context__,nullptr));
        }
        __c_iterator.close(__context__,(__c_rename_at_372_3));
    };
    return das_auto_cast<uint64_t>::cast((uint64_t(__b_rename_at_371_2) << UINT64_C(0x20)) | uint64_t(__a_rename_at_370_1));
}

static void registerAotFunctions ( AotLibrary & aotLib ) {
    aotLib[0x8be35780fdf30ef] = +[](Context & ctx) -> SimNode* {
        return ctx.code->makeNode<AutoSimNode_Aot<&hash0_9965be70952193f9>>();
    };
    // [[ init script ]]
    aotLib[0x69c2daef64f43e84] = +[](Context & ctx) -> SimNode* {
        ctx.aotInitScript = ctx.code->makeNode<AutoSimNode_Aot<&__init_script>>();
        return ctx.aotInitScript;
    };
    resolveTypeInfoAnnotations();
}

} // namespace _anon_2409885462760430613
AotListBase impl_aot_hash_map(_anon_2409885462760430613::registerAotFunctions);
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
