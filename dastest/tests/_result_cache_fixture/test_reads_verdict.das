options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/rtti
require strings

def private beside(name : string) : string =>
    path_join(dir_name(string(get_line_info().fileInfo.name)), name)

//! the trace of a program that ran, left only in a test's copy of
//! this folder - the one that holds a verdict file
[init]
def leave_a_trace() {
    if (fexist(beside("verdict.txt"))) {
        fwrite(beside("started.txt"), "started")
    }
}

[test]
def test_verdict_file(t : T?) {
    let verdict = strip(fread(beside("verdict.txt")))
    if (verdict == "panic") {
        panic("the verdict file asks for a panic")
    }
    t |> equal(verdict, "pass")
}
