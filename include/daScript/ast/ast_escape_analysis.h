#pragma once

namespace das {

    class Program;
    class TextWriter;

    // Pure analysis: computes per-pointer escape and records it on Variable::does_not_escape /
    // escape_* / escape_no_stack. No AST mutation; the flags are the reusable result. Returns true if
    // any variable was marked. See ast_escape_analysis.cpp.
    bool escapeAnalysis ( Program * program, TextWriter & logs );

    // Optimization: consumes the analysis result (the Variable flags) and emits scope-exit frees +
    // stack relocation. Mutates the AST. Returns true if anything changed (the caller must re-infer).
    bool scopeFreeOptimization ( Program * program, TextWriter & logs );

}
