options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module cbind_boost shared

require cbind public

// CXTranslationUnit
let CXTranslationUnit_None = 0x0
let CXTranslationUnit_DetailedPreprocessingRecord = 0x01
let CXTranslationUnit_Incomplete = 0x02
let CXTranslationUnit_PrecompiledPreamble = 0x04
let CXTranslationUnit_CacheCompletionResults = 0x08
let CXTranslationUnit_ForSerialization = 0x10
let CXTranslationUnit_CXXChainedPCH = 0x20
let CXTranslationUnit_SkipFunctionBodies = 0x40
let CXTranslationUnit_IncludeBriefCommentsInCodeCompletion = 0x80
let CXTranslationUnit_CreatePreambleOnFirstParse = 0x100
let CXTranslationUnit_KeepGoing = 0x200
let CXTranslationUnit_SingleFileParse = 0x400
let CXTranslationUnit_LimitSkipFunctionBodiesToPreamble = 0x800
let CXTranslationUnit_IncludeAttributedTypes = 0x1000
let CXTranslationUnit_VisitImplicitAttributes = 0x2000
let CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles = 0x4000
let CXTranslationUnit_RetainExcludedConditionalBlocks = 0x8000

def describe ( var c:CXCursor )
    var sp = clang_getCursorSpelling(c)
    var kd = clang_getCursorKindSpelling(clang_getCursorKind(c))
    return "<{string(sp)}>:`{string(kd)}`"
