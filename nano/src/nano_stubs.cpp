#include "daScript/misc/platform.h"

#include "daScript/simulate/simulate.h"
#include "daScript/simulate/aot.h"
#include "daScript/simulate/aot_library.h"
#include "daScript/ast/ast.h"
#include "daScript/nano_print.h"

#include <stdio.h>

// The seams where nano ends: the print sink, the storage src/misc/globals.cpp owns
// upstream, and the definitions that stop the program rather than pretend. Which
// is which, and why each one is here, is nano/ARCHITECTURE.md.

namespace das {

    static void nano_default_print ( const char * text ) {
#if defined(__GLIBCXX__) && !defined(_GLIBCXX_HAS_GTHREADS)
        // No stdout to write to, and reaching for one pulls all of stdio in.
        // An embedder on such a target sets the sink before anything can print.
        (void) text;
#else
        fputs(text, stdout);
#endif
    }

    static das_nano_print_sink g_nano_sink = &nano_default_print;

    void das_nano_set_print ( das_nano_print_sink sink ) {
        g_nano_sink = sink ? sink : &nano_default_print;
    }

    void das_nano_write ( int level, const char * prefix, const char * text ) {
        (void) level;
        if ( prefix && *prefix ) g_nano_sink(prefix);
        if ( text ) g_nano_sink(text);
    }

    SimNode * SimNode::copyNode ( Context &, NodeAllocator * ) {
        DAS_FATAL_ERROR("SimNode::copyNode: nano does not relocate code");
        return nullptr;
    }

    SimNode * SimNode::visit ( SimVisitor & ) {
        DAS_FATAL_ERROR("SimNode::visit: nano has no simulation visitor");
        return nullptr;
    }

    bool SimNode::evalBool ( Context & context ) { return cast<bool>::to(eval(context)); }
    float SimNode::evalFloat ( Context & context ) { return cast<float>::to(eval(context)); }
    double SimNode::evalDouble ( Context & context ) { return cast<double>::to(eval(context)); }
    int32_t SimNode::evalInt ( Context & context ) { return cast<int32_t>::to(eval(context)); }
    uint32_t SimNode::evalUInt ( Context & context ) { return cast<uint32_t>::to(eval(context)); }
    int64_t SimNode::evalInt64 ( Context & context ) { return cast<int64_t>::to(eval(context)); }
    uint64_t SimNode::evalUInt64 ( Context & context ) { return cast<uint64_t>::to(eval(context)); }
    char * SimNode::evalPtr ( Context & context ) { return cast<char *>::to(eval(context)); }

    SimNode * SimNode_WithErrorMessage::copyNode ( Context &, NodeAllocator * ) {
        DAS_FATAL_ERROR("SimNode_WithErrorMessage::copyNode: nano does not relocate code");
        return nullptr;
    }

    SimNode * SimNode_CallBase::copyNode ( Context &, NodeAllocator * ) {
        DAS_FATAL_ERROR("SimNode_CallBase::copyNode: nano does not relocate code");
        return nullptr;
    }

    void SimVisitor::sub ( SimNode **, uint32_t, const char * ) {
        DAS_FATAL_ERROR("SimVisitor::sub: nano has no simulation visitor");
    }

    const LineInfo * SimFunction::getLineInfo() const { return &code->debugInfo; }

    uint64_t getSemanticHash ( SimNode *, Context * ) {
        DAS_FATAL_ERROR("getSemanticHash: nano carries no semantic hashes");
        return 0;
    }

    TypeAnnotation * TypeInfo::getAnnotation() const {
        if ( type != Type::tHandle || !annotation_info ) return nullptr;
        return (TypeAnnotation *) annotation_info->resolved;
    }

    void TypeInfo::resolveAnnotation() const {
    }

    uint64_t getCancelLimit() {
        return 0;
    }

    SimNode * makeAotJitNode ( Context &, void * ) {
        DAS_FATAL_ERROR("makeAotJitNode: nano has no JIT - regenerate the standalone context without JIT");
        return nullptr;
    }

    // debug_info.cpp defines each of these classes' first non-inline virtual, so
    // it is where clang and gcc emit their vtables - and a vtable references every
    // virtual whether or not anything calls it. Their real bodies live in TUs that
    // need a file system and a serializer, so nano defines them here or does not
    // link at all. MSVC hides this: it emits a vtable only where one is used.
    void FileInfo::serialize ( AstSerializer & ) {
        DAS_FATAL_ERROR("FileInfo::serialize: nano carries no serializer");
    }

    void TextFileInfo::serialize ( AstSerializer & ) {
        DAS_FATAL_ERROR("TextFileInfo::serialize: nano carries no serializer");
    }

    void FileAccess::serialize ( AstSerializer & ) {
        DAS_FATAL_ERROR("FileAccess::serialize: nano carries no serializer");
    }

    int64_t FileAccess::getFileMtime ( const string & ) const {
        DAS_FATAL_ERROR("FileAccess::getFileMtime: nano has no file system");
        return -1;
    }

    int64_t FileAccess::getFileSize ( const string & ) const {
        DAS_FATAL_ERROR("FileAccess::getFileSize: nano has no file system");
        return -1;
    }
}

void os_debug_break() {
#if defined(_MSC_VER)
    __debugbreak();
#elif defined(__GNUC__) || defined(__clang__)
    __builtin_trap();
#endif
}

void print_current_stack_trace() {
}

namespace das {
    uint64_t ptr_ref_count::ref_count_total = 0;
    uint64_t ptr_ref_count::ref_count_track = 0;
    uint64_t ptr_ref_count::ref_count_track_destructor = 0;
    ptr_ref_count * ptr_ref_count::ref_count_head = nullptr;
    mutex ptr_ref_count::ref_count_mutex;

    DAS_API atomic<uint64_t> g_smart_ptr_total {0};
}
