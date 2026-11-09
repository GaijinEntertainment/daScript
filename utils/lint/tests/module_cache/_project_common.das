options gen2
options indenting = 4

module _project_common shared public

require dastest/testing_boost public
require daslib/clargs
require daslib/fio
require ../_lint_cli.das

struct LintRun {
    code : int
    output : string
}

def fixture(name : string) : string {
    return path_join(get_das_root(),
        "utils/lint/tests/module_cache/_fixtures/_fixture_{name}")
}

def build_tree(name, source : string; files : int = 1) : string {
    let root = path_join(test_temp_dir(), name)
    let src = path_join(root, "src")
    let dep = path_join(root, "dep")
    mkdir_rec(src)
    mkdir_rec(dep)
    fwrite(path_join(root, "README.md"), "## Outside \{#outside\}\n")
    fwrite(path_join(dep, "hidden.das"), fread(fixture("hidden.das")))
    fwrite(path_join(dep, "invalid.das"), fread(fixture("invalid.das")))
    fwrite(path_join(root, "map.das_project"),
        fread(fixture("map.das")))
    for (i in range(files)) {
        fwrite(path_join(src, "f{i}.das"), fread(fixture(source)))
    }
    return root
}

//! Fresh processes expose CLI exits and worker flag forwarding.
//! `exe_ok = false` for a run whose outcome depends on which native
//! modules the host can load: the standalone exe carries only its own
def run_lint(root : string; extra : array<string>; exe_ok : bool = true) : LintRun {
    var after <- ["--cache-dir", path_join(root, "cache"), path_join(root, "src")]
    after |> push_from(extra)
    var argv <- lint_cli_argv(["-no-module-cache"], after, exe_ok)
    let env <- ["TMPDIR={root}", "TMP={root}", "TEMP={root}"]
    var result : LintRun
    result.code = run_and_capture(argv, root, env, result.output)
    return result
}
