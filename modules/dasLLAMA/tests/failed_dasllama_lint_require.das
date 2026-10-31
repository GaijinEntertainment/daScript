options gen2

require dasllama/dasllama_gguf

//! Negative test — the default-on dasLLAMA facade lint must trip DASLLAMA001
//! (macro_error, code 50503) on a direct require of an engine internal from a file
//! carrying no ``options _dasllama_internal`` escape. The public entry points are the lint's
//! allowed table (``ALLOWED_DASLLAMA`` in ``dasllama/dasllama_lint.das``), which the
//! DASLLAMA001 text names one by one.

expect 50503

[export]
def main() {
    feint("never compiles\n")
}
