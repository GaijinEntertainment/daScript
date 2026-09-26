options gen2
options _dasllama_internal = true

module _server_rig shared

require dastest/testing_boost public
require daslib/fio
require daslib/clargs   // get_host_binary: the daslang a child runs under
require math
require dasllama/dasllama_env

//! The server suites' model roots: ONE resolution - `DASLLAMA_MODELS_DIR`, else the rig default -
//! so a test file never spells a second one.
def models_dir() : string => models_dir_resolved()

//! The suites' model gate: a JIT-only run skips interpreted, a missing file skips by name.
def model_missing(t : T?; path : string) : bool {
    if (!jit_enabled()) {
        t |> skip("interpreted (dasLLAMA server test is JIT-only)")
        return true
    }
    if (!stat(path).is_valid) {
        t |> skip("{base_name(path)} not present (set DASLLAMA_MODELS_DIR)")
        return true
    }
    return false
}

//! The last `n` bytes of a child's output, for a failure message.
def tail_of(s : string; n : int = 800) : string => slice(s, max(0, length(s) - n))

//! A child daslang's argv: the daslang this test runs under, -jit, the flags a child needs
//! forwarded (-dasroot / -module-cache), `script`, then `args` verbatim.
def child_daslang_argv(script : string; args : array<string>) : array<string> {
    var out : array<string>
    out |> reserve(6 + length(args))
    out |> push(get_host_binary())
    out |> push("-jit")
    var carry = ""   // a forwarded flag, held until its value arrives on the next argv entry
    for (a in get_command_line_arguments()) {   // a LOCKED array: iterate it, never bind or delete it
        if (!empty(carry)) {
            out |> push(clone_string(carry))
            out |> push(clone_string(a))
            carry = ""
        } elif (a == "-dasroot" || a == "-module-cache") {
            carry = clone_string(a)
        }
    }
    out |> push(script)
    out |> push_from(args)
    return <- out
}
