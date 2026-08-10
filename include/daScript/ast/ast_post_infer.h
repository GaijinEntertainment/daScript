#pragma once

namespace das {
    class Program;

    // Runs every [post_infer_macro] over the program. Called once inference is done and
    // again inside the optimizer, before the passes that read expr->type unguarded.
    void applyPostInferMacros ( Program * program );

    // Runs every [post_compile_macro] over the program. Called at the end of
    // compileDaScript, once the module's gc root holds only live nodes, so a pass may
    // reason about what the finished AST still owns.
    void applyPostCompileMacros ( Program * program );
}
