// LSP validate subtool — compile one file, print LSP-shaped diagnostics JSON.
//
// Invocation (CLI flags like -project_root go BEFORE the script path):
//   daslang utils/lsp/subtools/validate.das -- <file> [project.das_project] [--overlay <path>]
//
// --overlay <path> holds the client's unsaved buffer text for <file>; it is
// compiled (via set_file_source) and position-mapped in place of the disk
// content, so diagnostics track the editor buffer, not the last save.
//
// Prints one JSON object to stdout: {"ok": bool, "diagnostics": [...]} with
// LSP positions (0-based lines, UTF-16 columns). Compile errors are Error
// severity; on a clean compile the paranoid/perf/style lint passes run on
// the same program and report as Warning (LINT/PERF) / Information (STYLE),
// honoring repo policy (.lint_config + default-off rules). Exits 0 even on
// compile errors — those ARE the payload; non-0 only on catastrophic failure.
//
// Fresh process per call by design (see utils/lsp/ROADMAP.md): no macro-state
// leaks, no binary locks, crash isolation.
options gen2
options rtti
options indenting = 4

require lsp_common
require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require daslib/lint
require daslib/perf_lint
require daslib/style_lint
require daslib/lint_config
require strings

struct LspDiagnostic {
    @rename = "range" _range : LspRange
    severity : int
    code : string
    source : string
    message : string
}

struct ValidateResult {
    ok : bool
    diagnostics : array<LspDiagnostic>
}

def private build_message(what, extra, fixme : string) : string {
    var msg = what
    if (!empty(extra)) {
        msg = "{msg}\n{trim(extra)}"
    }
    if (!empty(fixme)) {
        msg = "{msg}\n{trim(fixme)}"
    }
    return msg
}

// dastest negative tests declare intentional compile errors via `expect
// <code>:<count>` — diagnostics on those are pure noise (same skip rule as
// utils/lint).
def private has_expect_directive(lines : array<string>) : bool {
    for (line in lines) {
        return true if (line |> strip |> starts_with("expect "))
    }
    return false
}

def private lint_severity(code : string) : int {
    return code |> starts_with("STYLE") ? 3 : 2  // Information : Warning
}

def private push_lint_issues(var result : ValidateResult; issues : array<LintIssue>;
                             lines : array<string>; target : string) {
    var seen : table<string>
    for (issue in issues) {
        // lint walks thisModule only, but guard anyway: positions in another
        // file can't be mapped onto this document
        continue if (to_generic_path(issue.file) != target)
        // paranoid lint can repeat an entry verbatim across generic instantiations
        let key = "{issue.line}:{issue.column}:{issue.message}"
        continue if (key_exists(seen, key))
        seen |> insert(key)
        // the code renders separately in LSP clients — drop the "CODE: " message prefix
        var msg = issue.message
        if (!empty(issue.code) && msg |> starts_with("{issue.code}: ")) {
            msg = slice(msg, length(issue.code) + 2)
        }
        result.diagnostics |> push(LspDiagnostic(
            _range = make_range(lines, issue.line, issue.column, issue.last_line, issue.last_column),
            severity = lint_severity(issue.code),
            code = issue.code,
            source = "daslang-lint",
            message = msg))
    }
}

[export]
def main() : int {
    let raw <- get_command_line_arguments()
    var user <- lsp_user_args(raw)
    let overlay_path = lsp_extract_overlay(user)
    if (empty(user)) {
        print(sprint_json(ValidateResult(ok = false), false))
        return 1
    }
    let file = user[0]
    let project = length(user) > 1 ? user[1] : ""

    let target = to_generic_path(file)
    let has_overlay = !empty(overlay_path)
    let source = has_overlay ? fread(overlay_path) : fread(file)
    var lines <- split(source, "\n")

    var result = ValidateResult(ok = true)
    if (has_expect_directive(lines)) {
        result.diagnostics |> push(LspDiagnostic(
            _range = LspRange(end = LspPosition(character = 1)),
            severity = 3,
            source = "daslang-lsp",
            message = "expect-file: compile errors here are intentional (dastest negative test); diagnostics suppressed"))
        print(sprint_json(result, false))
        delete lines
        return 0
    }

    // repo lint policy: default-off rules + .lint_config overrides
    var disabled_codes : table<string>
    let enabled_codes : table<string>
    seed_default_disabled(disabled_codes)
    load_lint_config(disabled_codes)

    var inscope access <- make_file_access(project)
    if (has_overlay) {
        set_file_source(access, file, source)
    }
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            // lint-profile compile (same as utils/lint): unoptimized AST so
            // source-shape rules fire; errors surface at infer, before either
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                result.ok = ok
                if (ok || program == null) {
                    if (!ok) {
                        result.diagnostics |> push(LspDiagnostic(
                            severity = 1,
                            source = "daslang",
                            message = "compilation failed: {issues}"))
                    } elif (program != null) {
                        var lint_issues : array<LintIssue>
                        paranoid_collect_issues(program, lint_issues, disabled_codes, enabled_codes)
                        perf_lint_collect_issues(program, lint_issues, disabled_codes, enabled_codes)
                        let hygiene = program._options |> find_arg("_comment_hygiene") ?as tBool ?? false
                        style_lint_collect_issues(program, lint_issues, disabled_codes, enabled_codes, hygiene)
                        push_lint_issues(result, lint_issues, lines, target)
                    }
                    return
                }
                for (err in program.errors) {
                    let err_file = err.at.fileInfo != null ? to_generic_path("{err.at.fileInfo.name}") : ""
                    var diag = LspDiagnostic(
                        severity = 1,
                        code = "{int(err.cerr)}",
                        source = "daslang",
                        message = build_message("{err.what}", "{err.extra}", "{err.fixme}"))
                    if (err_file == target) {
                        diag._range = make_range(lines, int(err.at.line), int(err.at.column),
                            int(err.at.last_line), int(err.at.last_column))
                    } elif (!empty(err_file)) {
                        // error in a required file: surface at the top of the edited one
                        diag.message = "in {err_file}:{int(err.at.line)}: {diag.message}"
                        diag._range.end.character = 1
                    }
                    result.diagnostics |> push(diag)
                }
            }
        }
    }
    print(sprint_json(result, false))
    delete lines
    return 0
}
