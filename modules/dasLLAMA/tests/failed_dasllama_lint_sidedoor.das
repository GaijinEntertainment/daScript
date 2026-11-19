options gen2

require dasllama/dasllama_lint
require ./_child_run.das

//! Negative test - the facade lint's TREE guard: a path-require resolving into
//! ``modules/dasLLAMA/`` kabooms (DASLLAMA001, code 50503) even when the module's
//! name carries no ``dasllama`` prefix. The name check alone would walk past this.
//! The module it reaches is one that parses nothing of the engine, and the lint itself
//! is required by name: the fixture's compile is the lint's, not the engine's.

expect 50503

[export]
def main() {
    feint("never compiles\n")
}
