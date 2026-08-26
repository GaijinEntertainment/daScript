#pragma once

// nano shadow of daScript/ast/ast.h.
//
// The real header is the compiler front-end: the AST, the type system, the
// module registry - tens of thousands of lines that a program which was already
// compiled has no use for. Two reused runtime sources still include it, and both
// want the same one thing from it: the type a handled (C++-bound) TypeInfo
// resolves to, so the data walker can hand a value to its annotation.
//
// nano has no modules, so nothing ever resolves, and the walk below stops the
// program instead of walking a type nobody described. BasicAnnotation itself
// comes from debug_info.h upstream; the two runtime includes are here because
// the real ast.h reaches Sequence and tableLiveSlot through its own chain, and
// the sources that include this file use them.

#include "daScript/misc/platform.h"
#include "daScript/misc/string_writer.h"
#include "daScript/simulate/debug_info.h"
#include "daScript/simulate/annotation_arguments.h"
#include "daScript/simulate/runtime_iterator.h"
#include "daScript/simulate/runtime_table.h"

namespace das {

    struct DataWalker;

    struct Module {
        string name;
    };

    struct Annotation : BasicAnnotation {
        Annotation ( const string & n, const string & cpn = "" ) : BasicAnnotation(n,cpn) {}
        virtual bool rtti_isHandledTypeAnnotation() const { return false; }
        Module * module = nullptr;
    };

    struct TypeAnnotation : Annotation {
        TypeAnnotation ( const string & n, const string & cpn = "" ) : Annotation(n,cpn) {}
        virtual bool rtti_isHandledTypeAnnotation() const override { return true; }
        virtual size_t getSizeOf() const { return sizeof(void *); }
        virtual size_t getAlignOf() const { return 1; }
        virtual void walk ( DataWalker &, void * ) {
            DAS_FATAL_ERROR("TypeAnnotation::walk: walking a handled type needs the full runtime, which nano is not");
        }
    };
}
