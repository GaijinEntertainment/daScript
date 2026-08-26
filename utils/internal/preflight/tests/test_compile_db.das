options gen2
options indenting = 4

require dastest/testing_boost public

require ../compile_db.das


// The tokenizer's failure mode is silent: a mangled path yields a token that simply does not
// name the file it should, and the gate then reports an error against the wrong input - or a
// spurious one. Windows is the case the author is least likely to be running, so it is pinned
// here rather than left to a red CI lane.
[test]
def test_cc_tokenize_windows_paths(t : T?) {
    t |> run("a backslash path survives intact") @(t : T?) {
        let tok <- cc_tokenize("clang-cl -IC:\\src\\daScript\\include C:\\src\\x.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[0], "clang-cl")
        t |> equal(tok[1], "-IC:\\src\\daScript\\include")
        t |> equal(tok[2], "C:\\src\\x.cpp")
    }
    t |> run("a trailing backslash is kept") @(t : T?) {
        let tok <- cc_tokenize("cl -IC:\\src\\")
        t |> equal(length(tok), 2)
        t |> equal(tok[1], "-IC:\\src\\")
    }
}


// A per-file output switch that survives into the grouped argv makes clang-cl reject the
// sweep for naming one output for many inputs, and the gate reports that as if the source
// were broken. The joined MSVC spellings are the ones a POSIX-only author misses.
[test]
def test_cc_is_compile_switch(t : T?) {
    t |> run("msvc spellings are stripped") @(t : T?) {
        t |> success(cc_is_compile_switch("/c"), "/c")
        t |> success(cc_is_compile_switch("/FoC:\\build\\x.obj"), "joined /Fo")
        t |> success(cc_is_compile_switch("-FoC:\\build\\x.obj"), "joined -Fo")
        t |> success(cc_is_compile_switch("/Fdvc140.pdb"), "joined /Fd")
        t |> success(cc_is_compile_switch("/showIncludes"), "/showIncludes")
    }
    t |> run("gnu spellings are stripped") @(t : T?) {
        t |> success(cc_is_compile_switch("-c"), "-c")
        t |> success(cc_is_compile_switch("-MD"), "-MD")
    }
    t |> run("real flags are kept") @(t : T?) {
        t |> success(!cc_is_compile_switch("/I../include"), "include dir")
        t |> success(!cc_is_compile_switch("-DFOO=1"), "define")
        t |> success(!cc_is_compile_switch("/std:c++17"), "std")
        t |> success(!cc_is_compile_switch("-fno-rtti"), "codegen flag")
    }
}


[test]
def test_cc_tokenize_quoting(t : T?) {
    t |> run("an escaped quote in a define becomes a literal quote") @(t : T?) {
        let tok <- cc_tokenize("clang++ -DDAS_INSTALL_BINDIR=\\\"/usr/local/bin\\\" -c a.cpp")
        t |> equal(length(tok), 4)
        t |> equal(tok[1], "-DDAS_INSTALL_BINDIR=\"/usr/local/bin\"")
    }
    t |> run("a quoted token keeps its spaces") @(t : T?) {
        let tok <- cc_tokenize("clang++ \"-IC:/Program Files/x\" a.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[1], "-IC:/Program Files/x")
    }
    t |> run("an escaped backslash collapses to one") @(t : T?) {
        let tok <- cc_tokenize("clang++ -DX=\\\\ a.cpp")
        t |> equal(length(tok), 3)
        t |> equal(tok[1], "-DX=\\")
    }
    t |> run("runs of whitespace do not mint empty tokens") @(t : T?) {
        let tok <- cc_tokenize("  clang++   -c    a.cpp  ")
        t |> equal(length(tok), 3)
        t |> equal(tok[0], "clang++")
        t |> equal(tok[2], "a.cpp")
    }
}
