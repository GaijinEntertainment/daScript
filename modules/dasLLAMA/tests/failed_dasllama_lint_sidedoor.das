options gen2

require ../performance/profile_common.das

//! Negative test — the facade lint's TREE guard: a path-require resolving into
//! ``modules/dasLLAMA/`` kabooms (DASLLAMA001, code 50503) even when the module's
//! name carries no ``dasllama`` prefix. The name check alone would walk past this.

expect 50503

[export]
def main() {
    feint("never compiles\n")
}
