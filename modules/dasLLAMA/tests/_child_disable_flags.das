options gen2
options _dasllama_internal = true

module _child_disable_flags shared

require daslib/strings_boost   // join: an argv as one line
require _child_run   // child_cmd, child_exe_key, and daslang_argv through _daslang_argv
require _compares   // compile_argv: the compile child's argv

// The fixture test_run_suites.das spawns with DASLLAMA_DISABLE_MODULES set and unset: the knob is
// read at context init, so only a child shows each builder taking it - a daslang argv, a test's
// child command, the compile child's argv - and the test child's exe key moving with it.

def public child_disable_flags_report() {
    print("ARGV {join(daslang_argv("das", ["root.das"]), " ")}\n")
    let none : array<tuple<name : string; value : string>>
    print("CMD {child_cmd("_child_root.das", none)}\n")
    print("COMPILE {join(compile_argv("/x/p.das", ""), " ")}\n")
    print("KEY {child_exe_key()}\n")
    print("ROOT_DONE\n")
}
