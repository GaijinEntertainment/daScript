options gen2

require strings
require daslib/strings_boost
require daslib/fio
require dastest/review_gate

// The mechanical half of REVIEW.md at the repo root (contract: REVIEW_COMMON.md beside it).
// Run from the repo root: bin/daslang REVIEW.das - exit 0 clean, 1 with findings.

struct private SharedTwin {
    name : string   //!< the shared module name two files may both declare
    file : string   //!< one of those files; a name's rows together are the whole set allowed to declare it
    why : string    //!< why no process compiles two of them by accident
}

// A third declaration of a tolerated name is still a finding: the rows are the whole allowed set.
var private TOLERATED_SHARED_TWINS <- [
    SharedTwin(name = "foo", file = "tests/module_tests/_modules/conflict/pkgA/foo.das", why = "the module tests compile the pair on purpose, to assert the conflict error"),
    SharedTwin(name = "foo", file = "tests/module_tests/_modules/conflict/pkgB/foo.das", why = "the module tests compile the pair on purpose, to assert the conflict error"),
    SharedTwin(name = "dupfoo", file = "tests/module_tests/_modules/extra_errors/pkgA/dupfoo.das", why = "the module tests compile the pair on purpose, to assert the conflict error"),
    SharedTwin(name = "dupfoo", file = "tests/module_tests/_modules/extra_errors/pkgB/dupfoo.das", why = "the module tests compile the pair on purpose, to assert the conflict error"),
    SharedTwin(name = "testing_boost", file = "dastest/testing_boost.das", why = "dastest_wasm/ is the wasm build's own root; the two roots are never compiled together"),
    SharedTwin(name = "testing_boost", file = "dastest_wasm/testing_boost.das", why = "dastest_wasm/ is the wasm build's own root; the two roots are never compiled together"),
    SharedTwin(name = "sql_provider", file = "daslib/sql_provider.das", why = "the dasSQLITE copy is a deprecation stub that fails to compile by design, pointing here"),
    SharedTwin(name = "sql_provider", file = "modules/dasSQLITE/daslib/sql_provider.das", why = "the dasSQLITE copy is a deprecation stub that fails to compile by design, pointing here")
]

//! true when every file declaring `name` is one of the rows tolerating it
def private is_tolerated_set(name : string; files : array<string>) : bool {
    for (f in files) {
        var listed = false
        for (row in TOLERATED_SHARED_TWINS) {
            listed ||= row.name == name && row.file == f
        }
        return false if (!listed)
    }
    return true
}

//! every tracked .das in the tree - the generated and vendored copies git ignores stay out;
//! a git that fails is a finding, never an empty tree
def private tracked_das_files : array<string> {
    var out : string
    let rc = run_and_capture(["git", "ls-files", "--", "*.das"], out, 60.0)
    var inscope files <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]
    if (rc != 0 || empty(files)) {
        gate_finding("REVIEW.das", "git ls-files failed (exit {rc}, {length(files)} files) - the shared-module check ran over nothing")
    }
    return <- files
}

struct private SharedDecl {
    name : string
    line : int
}

//! the `module <name> ... shared ...` declaration of a file, name "" when it declares none
def private shared_module_decl(path : string) : SharedDecl {
    var at = 0
    for (line in split(strip_line_comments(fread(path)), "\n")) {
        at++
        let t = strip(line)
        continue if (!(t |> starts_with("module ")))
        var inscope toks <- [for (tok in split(t, " ")); tok; where !empty(tok)]
        return SharedDecl() if (length(toks) < 3)
        for (i in range(2, length(toks))) {
            return SharedDecl(name = toks[1], line = at) if (toks[i] == "shared")
        }
        return SharedDecl()
    }
    return SharedDecl()
}

def private check_shared_module_names {
    var inscope files <- tracked_das_files()
    var sites : table<string; array<string>>
    for (f in files) {
        let d = shared_module_decl(f)
        continue if (empty(d.name))
        sites[d.name] |> push("{f}:{d.line}")
    }
    for (name, decls in keys(sites), values(sites)) {
        continue if (length(decls) < 2)
        var inscope declaring <- [for (s in decls); slice(s, 0, rfind(s, ":"))]
        continue if (is_tolerated_set(name, declaring))
        for (s in decls) {
            let colon = rfind(s, ":")
            let others = join([for (o in decls); o; where o != s], ", ")
            gate_finding(slice(s, 0, colon), to_int(slice(s, colon + 1)),
                "shared module `{name}` is also declared by {others} - the first file a process compiles that declares it becomes that process's module, and every later file declaring or requiring the name gets that module; rename one, or list every file that may share the name in TOLERATED_SHARED_TWINS with why no process compiles two of them by accident")
        }
    }
}

[export]
def main {
    if (!fexist("REVIEW_COMMON.md") || !fexist("CMakeLists.txt")) {
        to_log(LOG_ERROR, "REVIEW.das: run from the repo root\n")
        return 2
    }
    check_shared_module_names()
    return gate_verdict("repo root")
}
