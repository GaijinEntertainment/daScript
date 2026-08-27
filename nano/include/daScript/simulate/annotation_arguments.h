#pragma once

// nano shadow of daScript/simulate/annotation_arguments.h.
//
// The real list is a vector of AnnotationArgument, and every one of those holds
// two std::strings. Standalone AOT never fills it: the emitter resolves each
// script option at compile time and writes the answer into CodeOfPolicies, then
// passes a literal {} here. So nano keeps the type - generated code names it -
// and keeps nothing inside it.

namespace das {

    struct AnnotationArgumentList {};

}
