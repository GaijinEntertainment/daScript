options gen2

require dastest/testing_boost
require daslib/clargs
require daslib/fio
require daslib/strings_boost
require strings

def private compile_tool(b : B?; entry : string) {
    let bin = get_host_binary()
    let path = path_join(path_join(get_das_root(), "utils"), entry)
    let cmd = "\"{bin}\" -compile-only -no-module-cache \"{path}\" 2>&1"
    var rc = 0
    var first_error = ""
    b |> run(entry) {
        first_error = ""
        rc = unsafe(popen_timeout(cmd, 600.0) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    let ln = fgets(f)
                    if (empty(first_error) && find(ln, "error[") >= 0) {
                        first_error = strip(ln)
                    }
                }
            }
        })
    }
    b |> equal(rc, 0, "{entry}: daslang -compile-only exited with {rc}{empty(first_error) ? "" : " - " + first_error}")
}

[benchmark]
def compile_aot(b : B?) {
    compile_tool(b, "aot/main.das")
}

[benchmark]
def compile_benchctl(b : B?) {
    compile_tool(b, "benchctl/main.das")
}

[benchmark]
def compile_dascov(b : B?) {
    compile_tool(b, "dascov/main.das")
}

[benchmark]
def compile_daspkg(b : B?) {
    compile_tool(b, "daspkg/main.das")
}

[benchmark]
def compile_detect_dupe(b : B?) {
    compile_tool(b, "detect-dupe/main.das")
}

[benchmark]
def compile_fix_lint_errors(b : B?) {
    compile_tool(b, "fix-lint-errors/main.das")
}

[benchmark]
def compile_gen1_to_gen2(b : B?) {
    compile_tool(b, "gen1-to-gen2/main.das")
}

[benchmark]
def compile_lint(b : B?) {
    compile_tool(b, "lint/main.das")
}

[benchmark]
def compile_watchdog(b : B?) {
    compile_tool(b, "watchdog/main.das")
}

[benchmark]
def compile_das_fmt(b : B?) {
    compile_tool(b, "das-fmt/dasfmt.das")
}

[benchmark]
def compile_arch_extract(b : B?) {
    compile_tool(b, "internal/arch-extract/main.das")
}

[benchmark]
def compile_ast_fuzz(b : B?) {
    compile_tool(b, "internal/ast-fuzz/main.das")
}

[benchmark]
def compile_dasweb_verify(b : B?) {
    compile_tool(b, "internal/dasweb-verify/main.das")
}

[benchmark]
def compile_doc_verify(b : B?) {
    compile_tool(b, "internal/doc-verify/main.das")
}

[benchmark]
def compile_flatten_fuzz(b : B?) {
    compile_tool(b, "internal/flatten-fuzz/main.das")
}

[benchmark]
def compile_hygiene(b : B?) {
    compile_tool(b, "internal/hygiene/main.das")
}

[benchmark]
def compile_jit(b : B?) {
    compile_tool(b, "internal/jit/main.das")
}

[benchmark]
def compile_lineinfo_audit(b : B?) {
    compile_tool(b, "internal/lineinfo-audit/main.das")
}

[benchmark]
def compile_make_pr(b : B?) {
    compile_tool(b, "internal/make-pr/main.das")
}

[benchmark]
def compile_pr_babysit(b : B?) {
    compile_tool(b, "internal/pr-babysit/main.das")
}

[benchmark]
def compile_preflight(b : B?) {
    compile_tool(b, "internal/preflight/main.das")
}

[benchmark]
def compile_requirefix(b : B?) {
    compile_tool(b, "internal/requirefix/main.das")
}

[benchmark]
def compile_review_md(b : B?) {
    compile_tool(b, "internal/review-md/main.das")
}

[benchmark]
def compile_test_release(b : B?) {
    compile_tool(b, "internal/test-release/main.das")
}
