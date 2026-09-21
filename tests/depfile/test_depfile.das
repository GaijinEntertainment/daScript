options gen2
options indenting = 4
options no_aot = true

require dastest/testing_boost public
require daslib/defer
require daslib/fio
require daslib/strings_boost
require strings

//! `daslang -MD -MF <path>` writes what the run read, the way a C compiler's -MD does, and a
//! build system stats that list to decide the run can be skipped. These arms drive a child
//! daslang and read the rule it left: nothing here inspects the recorder, only its output.

def private das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! the environment form is the one a build edge uses, because it reaches the processes the edge
//! spawns. It rides a command prefix so this process never arms its own recorder
def private env_prefix(name : string; value : string) : string {
    return get_platform_name() == "windows" ? "set \"{name}={value}\"&& " : "{name}=\"{value}\" "
}

def private run(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout(cmd, 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

//! the recorder writes `/`, because a backslash is an escape in a Make depfile - and a windows
//! temp path spelled into a generated source would be one too
def private slashes(path : string) : string {
    return path |> replace("\\", "/")
}

def private tree(t : T?; prefix : string) : string {
    var error : string
    let tmp = create_temp_directory(prefix, error)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {error}")
    }
    return slashes(tmp)
}

def private drop(tmp : string) {
    var error : string
    rmdir_rec(tmp, error)
}

//! the rule's inputs, one per line, unescaped. The separator search skips a drive letter for
//! the same reason the reader in dep_recorder.cpp does
def private inputs_of(depfile : string) : array<string> {
    var found : array<string>
    let text = fread(depfile)
    let from = length(text) > 1 && text |> slice(1, 2) == ":" ? 2 : 0
    let colon = find(text, ":", from)
    if (colon < 0) return <- found
    for (line in split(slice(text, colon + 1), "\n")) {
        var one = line |> replace(" \\", "") |> strip()
        one = one |> replace("\\ ", " ")
        if (!empty(one)) {
            found |> push(one)
        }
    }
    return <- found
}

def private lists(depfile : string; path : string) : bool {
    let want = slashes(get_full_file_name(path))
    for (one in inputs_of(depfile)) {
        if (one == want || one == path) return true
    }
    return false
}

//! the rule's target. A drive letter's colon opens a windows target and is not the separator,
//! so the search starts past it - the same rule the reader in dep_recorder.cpp follows
def private target_of(depfile : string) : string {
    let text = fread(depfile)
    let from = length(text) > 1 && text |> slice(1, 2) == ":" ? 2 : 0
    let colon = find(text, ":", from)
    return colon < 0 ? "" : slice(text, 0, colon)
}

[test]
def test_md_names_every_source_the_compile_read(t : T?) {
    let tmp = tree(t, "das_depfile_read")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let leaf = path_join(tmp, "dep_leaf.das")
    let root = path_join(tmp, "dep_root.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(leaf, "options gen2\nmodule dep_leaf\ndef public leaf() : int \{\n    return 1\n\}\n")
    fwrite(root, "options gen2\nrequire dep_leaf\n[export]\ndef main \{\n    print(\"leaf \{leaf()\}\")\n\}\n")
    var out : string
    let rc = run("{das_exe()} -MD -MF {depfile} -compile-only {root}", out)
    t |> equal(rc, 0, "the compile succeeds:\n{out}")
    t |> success(lists(depfile, root), "the root is an input:\n{fread(depfile)}")
    t |> success(lists(depfile, leaf), "a required module is an input")
    t |> success(lists(depfile, "{get_das_root()}/daslib/builtin.das"), "so is builtin.das, which no source requires by name")
}

[test]
def test_target_defaults_to_the_path_without_its_d(t : T?) {
    let tmp = tree(t, "das_depfile_target")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let root = path_join(tmp, "dep_root.das")
    fwrite(root, "options gen2\n[export]\ndef main \{\n\}\n")
    let stamp = path_join(tmp, "check.stamp")
    var out : string
    run("{das_exe()} -MD -MF {stamp}.d -compile-only {root}", out)
    t |> equal(target_of("{stamp}.d"), slashes(stamp), "the rule names the stamp, the way -MD names its object")
    let named = path_join(tmp, "named.d")
    run("{das_exe()} -MD -MF {named} -MT build/some.stamp -compile-only {root}", out)
    t |> equal(target_of(named), "build/some.stamp", "-MT overrides it")
}

//! a path in the merged list that no longer exists is dropped at write time: ninja treats a
//! missing dependency as dirty, so naming one would re-run the check forever
[test]
def test_a_vanished_file_is_not_an_input(t : T?) {
    let tmp = tree(t, "das_depfile_gone")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let leaf = path_join(tmp, "dep_gone_leaf.das")
    let first = path_join(tmp, "first.das")
    let second = path_join(tmp, "second.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(leaf, "options gen2\nmodule dep_gone_leaf\ndef public leaf() : int \{\n    return 1\n\}\n")
    fwrite(first, "options gen2\nrequire dep_gone_leaf\n[export]\ndef main \{\n    print(\"leaf \{leaf()\}\")\n\}\n")
    fwrite(second, "options gen2\n[export]\ndef main \{\n\}\n")
    var out : string
    run("{das_exe()} -MD -MF {depfile} -MT build/one.stamp -compile-only {first}", out)
    t |> success(lists(depfile, leaf), "the module it required is an input while it exists:\n{fread(depfile)}")
    remove(leaf)
    run("{das_exe()} -MD -MF {depfile} -MT build/one.stamp -compile-only {second}", out)
    t |> success(!lists(depfile, leaf), "the merge drops a path that no longer exists")
    t |> success(lists(depfile, second), "and keeps what this run read")
}

//! a data file a PROGRAM reads is not what the compile required, and a build artifact is not an
//! input at all - a check that named the jitted object cache among its inputs would never be up
//! to date again
[test]
def test_a_runtime_read_is_not_an_input(t : T?) {
    let tmp = tree(t, "das_depfile_runtime")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let data = path_join(tmp, "payload.txt")
    let root = path_join(tmp, "dep_root.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(data, "read me at runtime\n")
    fwrite(root, "options gen2\nrequire daslib/fio\n[export]\ndef main \{\n    let text = fread(\"{slashes(data)}\")\n    print(\"read \{length(text)\}\")\n\}\n")
    var out : string
    let rc = run("{das_exe()} -MD -MF {depfile} {root}", out)
    t |> equal(rc, 0, "the run succeeds:\n{out}")
    t |> success(lists(depfile, root), "the compiled source is an input:\n{fread(depfile)}")
    t |> success(lists(depfile, "{get_das_root()}/daslib/fio.das"), "so is the module it requires")
    t |> success(!lists(depfile, data), "a file the program read at runtime is not")
}

[test]
def test_the_environment_arms_a_spawned_process(t : T?) {
    let tmp = tree(t, "das_depfile_env")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let root = path_join(tmp, "dep_root.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(root, "options gen2\n[export]\ndef main \{\n\}\n")
    var out : string
    let rc = run("{env_prefix("DAS_DEPFILE", depfile)}{das_exe()} -compile-only {root}", out)
    t |> equal(rc, 0, "the compile succeeds:\n{out}")
    t |> success(lists(depfile, root), "DAS_DEPFILE alone arms the run - no flag reaches a process a build edge spawns:\n{fread(depfile)}")
}

[test]
def test_excluded_directories_stay_out(t : T?) {
    let tmp = tree(t, "das_depfile_exclude")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let generated = path_join(tmp, "generated")
    mkdir_rec(generated)
    let hidden = path_join(generated, "hidden.das")
    let root = path_join(tmp, "dep_root.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(hidden, "options gen2\nmodule hidden\n")
    fwrite(root, "options gen2\nrequire hidden\n[export]\ndef main \{\n\}\n")
    var out : string
    run("{env_prefix("DAS_DEPFILE_EXCLUDE", generated)}{das_exe()} -MD -MF {depfile} -compile-only {root}", out)
    t |> success(lists(depfile, root), "the root is still an input:\n{fread(depfile)}")
    t |> success(!lists(depfile, hidden), "a build directory named in DAS_DEPFILE_EXCLUDE contributes nothing")
}

[test]
def test_two_runs_merge_into_one_rule(t : T?) {
    let tmp = tree(t, "das_depfile_merge")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let first = path_join(tmp, "first.das")
    let second = path_join(tmp, "second.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(first, "options gen2\n[export]\ndef main \{\n\}\n")
    fwrite(second, "options gen2\n[export]\ndef main \{\n\}\n")
    var out : string
    run("{das_exe()} -MD -MF {depfile} -MT build/one.stamp -compile-only {first}", out)
    run("{das_exe()} -MD -MF {depfile} -MT build/one.stamp -compile-only {second}", out)
    let text = fread(depfile)
    t |> equal(count_substrings(text, "build/one.stamp:"), 1, "one rule, never two - several rules naming one target is ninja's deprecated depfilemulti:\n{text}")
    t |> success(lists(depfile, first) && lists(depfile, second), "and it holds what both runs read")
}

//! a windows target opens with a drive letter, and its colon is not the rule's separator. The
//! shape is reproducible on any host: what matters is a colon inside the target
[test]
def test_a_target_with_a_drive_letter_merges(t : T?) {
    let tmp = tree(t, "das_depfile_drive")
    if (empty(tmp)) return
    defer() {
        drop(tmp)
    }
    let first = path_join(tmp, "first.das")
    let second = path_join(tmp, "second.das")
    let depfile = path_join(tmp, "out.d")
    fwrite(first, "options gen2\n[export]\ndef main \{\n\}\n")
    fwrite(second, "options gen2\n[export]\ndef main \{\n\}\n")
    var out : string
    run("{das_exe()} -MD -MF {depfile} -MT C:/build/one.stamp -compile-only {first}", out)
    run("{das_exe()} -MD -MF {depfile} -MT C:/build/one.stamp -compile-only {second}", out)
    let text = fread(depfile)
    t |> equal(target_of(depfile), "C:/build/one.stamp", "the rule still names the drive-lettered target:\n{text}")
    t |> success(lists(depfile, first) && lists(depfile, second), "and the merge kept what both runs read")
    for (one in inputs_of(depfile)) {
        t |> success(find(one, "build/one.stamp") < 0, "the target's own tail is not an input: {one}")
    }
}

def private count_substrings(text : string; what : string) : int {
    var n = 0
    var at = 0
    while (true) {
        let found = find(text, what, at)
        if (found < 0) break
        n++
        at = found + length(what)
    }
    return n
}
