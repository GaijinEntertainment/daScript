// Layout pin for the debug_info.h data structs.
#include <doctest/doctest.h>
#include "daScript/daScript.h"
#include "daScript/simulate/debug_info.h"
#include <cstddef>

using namespace das;

#if INTPTR_MAX == INT64_MAX

#define DAS_PIN_MSG "debug_info.h layout changed: re-pin this line in the same diff and state the per-consumer verdicts (include/daScript/simulate/REVIEW.md)"

static_assert(sizeof(AnnotationArgumentInfo) == 32, DAS_PIN_MSG);
static_assert(offsetof(AnnotationArgumentInfo, type) == 0, DAS_PIN_MSG);
static_assert(offsetof(AnnotationArgumentInfo, name) == 8, DAS_PIN_MSG);
static_assert(offsetof(AnnotationArgumentInfo, sValue) == 16, DAS_PIN_MSG);
static_assert(offsetof(AnnotationArgumentInfo, iValue) == 24, DAS_PIN_MSG);
static_assert(sizeof(AnnotationInfo) == 40, DAS_PIN_MSG);
static_assert(offsetof(AnnotationInfo, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(AnnotationInfo, module_name) == 8, DAS_PIN_MSG);
static_assert(offsetof(AnnotationInfo, arguments) == 16, DAS_PIN_MSG);
static_assert(offsetof(AnnotationInfo, count) == 24, DAS_PIN_MSG);
static_assert(offsetof(AnnotationInfo, resolved) == 32, DAS_PIN_MSG);
static_assert(sizeof(LineInfo) == 24, DAS_PIN_MSG);
static_assert(offsetof(LineInfo, fileInfo) == 0, DAS_PIN_MSG);
static_assert(offsetof(LineInfo, column) == 8, DAS_PIN_MSG);
static_assert(offsetof(LineInfo, line) == 12, DAS_PIN_MSG);
static_assert(offsetof(LineInfo, last_column) == 16, DAS_PIN_MSG);
static_assert(offsetof(LineInfo, last_line) == 20, DAS_PIN_MSG);
static_assert(sizeof(LineInfoArg) == 24, DAS_PIN_MSG);
static_assert(sizeof(TypeInfo) == 80, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, structType) == 0, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, firstType) == 8, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, secondType) == 16, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, argTypes) == 24, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, argNames) == 32, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, dim) == 40, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, hash) == 48, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, type) == 56, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, flags) == 60, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, size) == 64, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, argCount) == 68, DAS_PIN_MSG);
static_assert(offsetof(TypeInfo, dimSize) == 72, DAS_PIN_MSG);
static_assert(sizeof(VarInfo) == 128, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, value) == 80, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, sValue) == 80, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, name) == 96, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, annotation_arguments) == 104, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, annotation_argument_count) == 112, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, offset) == 116, DAS_PIN_MSG);
static_assert(offsetof(VarInfo, nextGcField) == 120, DAS_PIN_MSG);
static_assert(sizeof(StructInfo) == 72, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, module_name) == 8, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, fields) == 16, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, annotations) == 24, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, hash) == 32, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, init_mnh) == 40, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, flags) == 48, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, count) == 52, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, size) == 56, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, firstGcField) == 60, DAS_PIN_MSG);
static_assert(offsetof(StructInfo, annotation_count) == 64, DAS_PIN_MSG);
static_assert(sizeof(EnumValueInfo) == 16, DAS_PIN_MSG);
static_assert(offsetof(EnumValueInfo, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(EnumValueInfo, value) == 8, DAS_PIN_MSG);
static_assert(sizeof(EnumInfo) == 64, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, module_name) == 8, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, fields) == 16, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, count) == 24, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, hash) == 32, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, flags) == 40, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, annotations) == 48, DAS_PIN_MSG);
static_assert(offsetof(EnumInfo, annotation_count) == 56, DAS_PIN_MSG);
static_assert(sizeof(LocalVariableInfo) == 128, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, visibility) == 80, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, name) == 104, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, stackTop) == 112, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, localFlags) == 116, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, openPos) == 120, DAS_PIN_MSG);
static_assert(offsetof(LocalVariableInfo, closePos) == 124, DAS_PIN_MSG);
static_assert(sizeof(FramePosInterval) == 8, DAS_PIN_MSG);
static_assert(offsetof(FramePosInterval, open) == 0, DAS_PIN_MSG);
static_assert(offsetof(FramePosInterval, close) == 4, DAS_PIN_MSG);
static_assert(sizeof(FuncInfo) == 96, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, cppName) == 8, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, fields) == 16, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, result) == 24, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, locals) == 32, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, globals) == 40, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, annotations) == 48, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, hash) == 56, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, flags) == 64, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, count) == 68, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, stackSize) == 72, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, localCount) == 76, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, globalCount) == 80, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, annotation_count) == 84, DAS_PIN_MSG);
static_assert(offsetof(FuncInfo, spaceId) == 88, DAS_PIN_MSG);

// STL-bearing structs: values are per-STL (and per-CRT: /MDd inflates string/vector), so pin
// ONE ABI only — MSVC x64 Release runs on every PR, and one firing lane is enough to force
// the re-pin into the same diff
#if defined(_MSC_VER) && !DAS_ENABLE_PROFILER && _ITERATOR_DEBUG_LEVEL == 0
static_assert(sizeof(FileInfo) == 80, DAS_PIN_MSG);
static_assert(sizeof(TextFileInfo) == 96, DAS_PIN_MSG);
static_assert(sizeof(ModuleInfo) == 136, DAS_PIN_MSG);
static_assert(sizeof(BaseRequireRecord) == 64, DAS_PIN_MSG);
static_assert(sizeof(RequireRecord) == 72, DAS_PIN_MSG);
static_assert(sizeof(MissingRecord) == 136, DAS_PIN_MSG);
static_assert(sizeof(NamelessModuleReq) == 128, DAS_PIN_MSG);
static_assert(sizeof(NamelessMismatch) == 224, DAS_PIN_MSG);
static_assert(sizeof(BasicAnnotation) == 112, DAS_PIN_MSG);
// by-name offsets so a size-neutral field swap still fires
static_assert(offsetof(ModuleInfo, moduleName) == 0, DAS_PIN_MSG);
static_assert(offsetof(ModuleInfo, fileName) == 32, DAS_PIN_MSG);
static_assert(offsetof(ModuleInfo, importName) == 64, DAS_PIN_MSG);
static_assert(offsetof(ModuleInfo, extraDepModule) == 96, DAS_PIN_MSG);
static_assert(offsetof(ModuleInfo, requireName) == 104, DAS_PIN_MSG);
static_assert(offsetof(BaseRequireRecord, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(BaseRequireRecord, line) == 32, DAS_PIN_MSG);
static_assert(offsetof(BaseRequireRecord, chain) == 40, DAS_PIN_MSG);
static_assert(offsetof(RequireRecord, isPublic) == 64, DAS_PIN_MSG);
static_assert(offsetof(MissingRecord, hintType) == 64, DAS_PIN_MSG);
static_assert(offsetof(MissingRecord, hintName) == 72, DAS_PIN_MSG);
static_assert(offsetof(MissingRecord, hintName2) == 104, DAS_PIN_MSG);
static_assert(offsetof(NamelessModuleReq, name) == 0, DAS_PIN_MSG);
static_assert(offsetof(NamelessModuleReq, moduleName) == 32, DAS_PIN_MSG);
static_assert(offsetof(NamelessModuleReq, fileName) == 64, DAS_PIN_MSG);
static_assert(offsetof(NamelessModuleReq, fromFile) == 96, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, chain) == 0, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, line) == 24, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, name1) == 32, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, fileName1) == 64, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, fromFile1) == 96, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, name2) == 128, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, fileName2) == 160, DAS_PIN_MSG);
static_assert(offsetof(NamelessMismatch, fromFile2) == 192, DAS_PIN_MSG);

// derived probes lift protected members into offsetof reach; they add no members, so
// base-subobject layout is unchanged
struct FileInfoPins : FileInfo {
    using FileInfo::lineOffsets;
    using FileInfo::lineIndexBuilt;
};
struct TextFileInfoPins : TextFileInfo {
    using TextFileInfo::source;
    using TextFileInfo::sourceLength;
    using TextFileInfo::owner;
};
struct FileAccessPins : FileAccess {
    using FileAccess::files;
    using FileAccess::extraModules;
    using FileAccess::autoRequiredModules;
    using FileAccess::locked;
};
struct ModuleFileAccessPins : ModuleFileAccess {
    using ModuleFileAccess::context;
    using ModuleFileAccess::modGet;
    using ModuleFileAccess::includeGet;
    using ModuleFileAccess::moduleAllowed;
    using ModuleFileAccess::moduleUnsafe;
    using ModuleFileAccess::withModuleUnsafe;
    using ModuleFileAccess::canModuleBeRequired;
    using ModuleFileAccess::sameFileName;
    using ModuleFileAccess::optionAllowed;
    using ModuleFileAccess::optionBlocked;
    using ModuleFileAccess::annotationAllowed;
    using ModuleFileAccess::podInScopeAllowed;
    using ModuleFileAccess::dynModulesFolderGet;
};

static_assert(sizeof(FileAccess) == 248, DAS_PIN_MSG);
static_assert(sizeof(ModuleFileAccess) == 352, DAS_PIN_MSG);
static_assert(offsetof(FileInfo, name) == 8, DAS_PIN_MSG);
static_assert(offsetof(FileInfo, tabSize) == 40, DAS_PIN_MSG);
static_assert(offsetof(FileInfoPins, lineOffsets) == 48, DAS_PIN_MSG);
static_assert(offsetof(FileInfoPins, lineIndexBuilt) == 72, DAS_PIN_MSG);
static_assert(offsetof(TextFileInfoPins, source) == 80, DAS_PIN_MSG);
static_assert(offsetof(TextFileInfoPins, sourceLength) == 88, DAS_PIN_MSG);
static_assert(offsetof(TextFileInfoPins, owner) == 92, DAS_PIN_MSG);
static_assert(offsetof(BasicAnnotation, name) == 48, DAS_PIN_MSG);
static_assert(offsetof(BasicAnnotation, cppName) == 80, DAS_PIN_MSG);
static_assert(offsetof(FileAccessPins, files) == 48, DAS_PIN_MSG);
static_assert(offsetof(FileAccessPins, extraModules) == 144, DAS_PIN_MSG);
static_assert(offsetof(FileAccessPins, autoRequiredModules) == 168, DAS_PIN_MSG);
static_assert(offsetof(FileAccessPins, locked) == 240, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, context) == 248, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, modGet) == 256, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, includeGet) == 264, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, moduleAllowed) == 272, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, moduleUnsafe) == 280, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, withModuleUnsafe) == 288, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, canModuleBeRequired) == 296, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, sameFileName) == 304, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, optionAllowed) == 312, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, optionBlocked) == 320, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, annotationAllowed) == 328, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, podInScopeAllowed) == 336, DAS_PIN_MSG);
static_assert(offsetof(ModuleFileAccessPins, dynModulesFolderGet) == 344, DAS_PIN_MSG);
#endif

TEST_CASE("debug_info.h layout pins hold") {
    CHECK(sizeof(void *) == 8);
}

#else

TEST_CASE("debug_info.h layout pins skipped (64-bit only)") {
    MESSAGE("layout pins are 64-bit only; skipped");
}

#endif
