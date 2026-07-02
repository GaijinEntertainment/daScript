// LSP validate subtool — compile one file, print LSP-shaped diagnostics JSON.
//
// Invocation (CLI flags like -project_root go BEFORE the script path):
//   daslang utils/lsp/subtools/validate.das -- <file> [project.das_project]
//
// Prints one JSON object to stdout: {"ok": bool, "diagnostics": [...]} with
// LSP positions (0-based lines, UTF-16 columns). Exits 0 even on compile
// errors — those ARE the payload; non-0 only on catastrophic failure.
//
// Fresh process per call by design (see utils/lsp/ROADMAP.md): no macro-state
// leaks, no binary locks, crash isolation.
options gen2
options rtti
options indenting = 4

require daslib/rtti
require daslib/ast
require daslib/fio
require daslib/strings_boost
require strings
require math

struct LspPosition {
    line : int
    character : int
}

struct LspRange {
    start : LspPosition
    end : LspPosition
}

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

// byte column (1-based) -> UTF-16 code units before it (LSP `character`)
def private to_utf16_character(line_text : string; byte_col_1based : int) : int {
    let target = byte_col_1based - 1
    if (target <= 0) {
        return 0
    }
    var units = 0
    peek_data(line_text) $(arr) {
        var off = 0
        for (b in arr) {
            break if (off >= target)
            let c = int(b)
            if ((c & int(0xC0)) != int(0x80)) {  // not a continuation byte: one new code point
                units += ((c & int(0xF8)) == int(0xF0)) ? 2 : 1  // 4-byte UTF-8 = surrogate pair
            }
            off++
        }
    }
    return units
}

def private make_position(lines : array<string>; line_1based, byte_col_1based : int) : LspPosition {
    let li = max(0, line_1based - 1)
    let text = li < length(lines) ? lines[li] : ""
    return LspPosition(line = li, character = to_utf16_character(text, byte_col_1based))
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

[export]
def main() : int {
    let raw <- get_command_line_arguments()
    var user : array<string>
    var seen_sep = false
    for (a in raw) {
        if (seen_sep) {
            user |> push(a)
        } elif (a == "--") {
            seen_sep = true
        }
    }
    if (empty(user)) {
        print(sprint_json(ValidateResult(ok = false), false))
        return 1
    }
    let file = user[0]
    let project = length(user) > 1 ? user[1] : ""

    let target = to_generic_path(file)
    var lines <- split(fread(file), "\n")

    var result = ValidateResult(ok = true)
    var inscope access <- make_file_access(project)
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.threadlock_context = true
            cop.ignore_shared_modules = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                result.ok = ok
                if (ok || program == null) {
                    if (!ok) {
                        result.diagnostics |> push(LspDiagnostic(
                            severity = 1,
                            source = "daslang",
                            message = "compilation failed: {issues}"))
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
                        diag._range.start = make_position(lines, int(err.at.line), int(err.at.column))
                        diag._range.end = (int(err.at.last_line) > 0 ?
                            make_position(lines, int(err.at.last_line), int(err.at.last_column)) :
                            LspPosition(line = diag._range.start.line, character = diag._range.start.character + 1))
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
