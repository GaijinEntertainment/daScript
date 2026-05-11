options gen2

require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require daslib/clargs
require strings
require math

// utils/fix-lint-errors --
//
// Auto-fixes mechanical lint rules. Walks the AST of each input file via
// `compile_file`, identifies fixable patterns, and rewrites the source.
//
// Rules supported (v1):
//   STYLE005  if (cond) { <terminator> }   ->  if (cond) <terminator>
//
// Future rules (PERF017, PERF013, STYLE018, ...) layer on the same Edit
// pipeline — each visitor override pushes an Edit; apply_edits writes them
// back in reverse-position order.
//
// Out of scope: rules that need cross-statement restructuring (STYLE016
// guard combining, STYLE011/012/013 init patterns, PERF018 for-each).

[CommandLineArgs]
struct Config {
    @clarg_short = "q"
    @clarg_doc = "Suppress per-file PASS/PROCESS lines"
    quiet : bool

    @clarg_doc = "Print edits but don't write files"
    dry_run : bool

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct SourceEdit {
    start_line : int    // 1-based, inclusive
    start_col : int     // 1-based, inclusive
    end_line : int      // 1-based, inclusive
    end_col : int       // 1-based, exclusive (one past last char)
    rule : string       // e.g. "STYLE005"
}

class FixerVisitor : AstVisitor {
    //! Walks a compiled program collecting SourceEdit records. Each edit names
    //! a span to replace; the replacement text is computed at apply time from
    //! the inner expression's `at`. Per-file edits are sorted descending in
    //! `apply_edits` and applied in that order so earlier offsets stay valid.
    edits : array<SourceEdit>
    replacements : array<string>
    @do_not_delete current_function : Function?
    in_template : bool = false
    in_closure : int = 0

    def FixerVisitor() {
        pass
    }

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
        in_template = fn.moreFlags.isTemplate
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        in_template = false
        return <- fn
    }

    def override preVisitExprBlock(blk : ExprBlock?) : void {
        if (blk.blockFlags.isClosure) {
            in_closure++
        }
    }

    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (blk.blockFlags.isClosure) {
            in_closure--
        }
        return <- blk
    }

    // --- helpers ---

    def extract_block_inner(blk_at : LineInfo) : string {
        //! Returns the source bytes between an ExprBlock's `{` and `}` (exclusive
        //! on both ends), trimmed. For a single-statement block this yields the
        //! statement source — keyword + subexpr + any trailing `;` — which we use
        //! verbatim as the STYLE005 replacement.
        //!
        //! Why not `extract_text(inner_stmt.at)`? Because terminator LineInfos
        //! cover only the keyword (e.g. ExprReturn.at spans "return", not the
        //! subexpr). Going via the block bounds captures the full statement
        //! whatever its shape.
        //!
        //! Coordinate convention: daslang LineInfo columns are 0-based. `column`
        //! is the 0-indexed start; `last_column` is one past the last char
        //! (exclusive 0-indexed end). So `{` lives at index `column`; the byte
        //! immediately after `{` is at `column + 1`. The `}` is at
        //! `last_column - 1`; the slice we want ends at `last_column - 1`.
        return "" if (blk_at.fileInfo == null || blk_at.line == 0u)
        let start_line = int(blk_at.line)
        let end_line = int(blk_at.last_line)
        let inner_start = int(blk_at.column) + 1
        let inner_end = int(blk_at.last_column) - 1
        var result = ""
        if (start_line == end_line) {
            get_file_source_line(blk_at.fileInfo, blk_at.line) $(line) {
                let lo = max(0, inner_start)
                let hi = min(length(line), inner_end)
                if (hi > lo) {
                    result := slice(line, lo, hi)
                }
            }
            return strip(result)
        }
        for (ln in range(start_line, end_line + 1)) {
            get_file_source_line(blk_at.fileInfo, uint(ln)) $(line) {
                if (ln == start_line) {
                    let lo = max(0, inner_start)
                    if (lo < length(line)) {
                        result := slice(line, lo)
                    }
                } elif (ln == end_line) {
                    let hi = min(length(line), inner_end)
                    if (hi > 0) {
                        result := "{result}\n{slice(line, 0, hi)}"
                    }
                } else {
                    result := "{result}\n{line}"
                }
            }
        }
        return strip(result)
    }

    def push_edit(at : LineInfo; replacement : string; rule : string) : void {
        var e : SourceEdit
        e.start_line = int(at.line)
        e.start_col = int(at.column)
        e.end_line = int(at.last_line)
        e.end_col = int(at.last_column)
        e.rule = rule
        edits |> push(e)
        replacements |> push(replacement)
    }

    // --- STYLE005: braces around a single-statement early exit ---

    def override preVisitExprIfThenElse(ifte : ExprIfThenElse?) : void {
        return if (in_template || in_closure > 0)
        return if (ifte.if_flags.isStatic || ifte.if_false != null)
        return if (!(ifte.if_true is ExprBlock))
        let blk = ifte.if_true as ExprBlock
        return if (length(blk.list) != 1 || !empty(blk.finalList))
        let inner = blk.list[0]
        return if (!(inner is ExprReturn || inner is ExprBreak || inner is ExprContinue))
        // Synthetic Block (braceless / postfix-desugared) shares LineInfo with
        // the inner terminator — that's the AST signal we are NOT looking at
        // user-written braces. Skip.
        return if (blk.at.line == inner.at.line && blk.at.column == inner.at.column)
        let inner_text = self->extract_block_inner(blk.at)
        return if (empty(inner_text))
        self->push_edit(blk.at, inner_text, "STYLE005")
    }
}

// =====================================================================
// CLI
// =====================================================================

def is_skip_file(name : string) : bool {
    return name == "builtin.das"
}

def is_skip_dir(name : string) : bool {
    return name |> starts_with(".")
}

def has_expect_directive(file : string) : bool {
    //! Mirrors utils/lint/main.das — skip files with intentional compile errors.
    let text = fread(file)
    return false if (empty(text))
    let lines <- text |> split("\n")
    for (line in lines) {
        let trimmed = line |> strip
        return true if (trimmed |> starts_with("expect "))
    }
    return false
}

def scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>) {
    let st = stat(path)
    return if (!st.is_valid)
    if (st.is_reg) {
        if (path |> ends_with(".das") && !cache |> key_exists(path)) {
            let last_slash = max(rfind(path, "/"), rfind(path, "\\"))
            let fname = last_slash >= 0 ? slice(path, last_slash + 1) : path
            if (!is_skip_file(fname)) {
                cache |> insert(path, null)
                files |> push(path)
            }
        }
        return
    }
    return if (!st.is_dir)
    fio::dir(path) $(name) {
        return if (name == "." || name == ".." || name |> starts_with("_") || is_skip_dir(name))
        let full = "{path}/{name}"
        let fst = stat(full)
        return if (!fst.is_valid)
        if (fst.is_dir) {
            scan_das_files(full, files, cache)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(name) && !cache |> key_exists(full)) {
            cache |> insert(full, null)
            files |> push(full)
        }
    }
}

struct EditRecord {
    start_line : int
    start_col : int
    end_line : int
    end_col : int
    replacement : string
    rule : string
}

def collect_edits(file : string; var records : array<EditRecord>) : bool {
    //! Returns true if the file compiled (whether or not edits were collected).
    var ok_out = false
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.lint_check = true
            cop.no_optimizations = true
            cop.no_infer_time_folding = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                ok_out = ok
                return if (!ok)
                var astVisitor = new FixerVisitor()
                make_visitor(*astVisitor) $(adapter) {
                    visit(program, adapter)
                }
                let n = length(astVisitor.edits)
                records |> reserve(length(records) + n)
                for (i in range(n)) {
                    var r : EditRecord
                    r.start_line = astVisitor.edits[i].start_line
                    r.start_col = astVisitor.edits[i].start_col
                    r.end_line = astVisitor.edits[i].end_line
                    r.end_col = astVisitor.edits[i].end_col
                    r.replacement := astVisitor.replacements[i]
                    r.rule := astVisitor.edits[i].rule
                    records |> push(<- r)
                }
                unsafe {
                    delete astVisitor
                }
            }
        }
    }
    return ok_out
}

def apply_edits_to_file(file : string; var records : array<EditRecord>; dry_run : bool) : int {
    //! Applies all edits to a file's source, returning the number written.
    //! Sorts edits by descending end position so earlier-position edits stay
    //! valid through later applications.
    return 0 if (empty(records))
    let text = fread(file)
    return 0 if (empty(text))
    var lines <- text |> split("\n")
    // Sort by (end_line desc, end_col desc) — apply from end of file backwards.
    // Daslang has `sort` from daslib/sort_boost; here we just bubble for small N.
    let n = length(records)
    for (i in range(n)) {
        for (j in range(i + 1, n)) {
            let a = records[i]
            let b = records[j]
            let a_after_b = (a.end_line > b.end_line
                || (a.end_line == b.end_line && a.end_col > b.end_col))
            if (!a_after_b) {
                // swap
                var tmp <- records[i]
                records[i] <- records[j]
                records[j] <- tmp
            }
        }
    }
    for (r in records) {
        let s_idx = r.start_line - 1
        let e_idx = r.end_line - 1
        if (s_idx < 0 || e_idx >= length(lines)) {
            continue
        }
        let start_line_text = lines[s_idx]
        let end_line_text = lines[e_idx]
        // Coordinates are 0-based, last_column exclusive (see extract_block_inner).
        // start_col is the index of `{`; everything before is prefix.
        // end_col is one past `}`; everything from end_col on is suffix.
        let prefix_lo = max(0, r.start_col)
        let prefix = ((prefix_lo > length(start_line_text))
            ? start_line_text
            : slice(start_line_text, 0, prefix_lo))
        let suffix_lo = r.end_col
        let suffix = ((suffix_lo >= length(end_line_text))
            ? ""
            : slice(end_line_text, suffix_lo))
        let merged = "{prefix}{r.replacement}{suffix}"
        // Replace lines [s_idx .. e_idx] with the single merged line.
        // We rebuild via a fresh list.
        var new_lines : array<string>
        new_lines |> reserve(length(lines) - (e_idx - s_idx))
        for (k in range(length(lines))) {
            if (k < s_idx || k > e_idx) {
                new_lines |> push(lines[k])
            } elif (k == s_idx) {
                new_lines |> push(merged)
            }
        }
        lines <- new_lines
    }
    if (!dry_run) {
        let new_text = join(lines, "\n")
        fwrite(file, new_text)
    }
    return n
}

[export]
def main() : int {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "utils/fix-lint-errors/main.das")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print("Auto-fixer for mechanical lint rules (v1: STYLE005).\n")
        print("Walks the AST, identifies fixable patterns, rewrites the source.\n\n")
        print_help(get_command_info(type<Config>), "utils/fix-lint-errors/main.das")
        print("\nPositional arguments: one or more files or directories.\n")
        return 0
    }
    let cli_args <- get_user_args()
    var paths : array<string>
    for (arg in cli_args) {
        if (!starts_with(arg, "-")) {
            paths |> push(arg)
        }
    }
    if (empty(paths)) {
        print("Error: no files or directories specified\n\n")
        print_help(get_command_info(type<Config>), "utils/fix-lint-errors/main.das")
        return 1
    }
    var files : array<string>
    var cache : table<string; void?>
    for (p in paths) {
        scan_das_files(p, files, cache)
    }
    if (empty(files)) {
        print("Error: no .das files found\n")
        return 1
    }
    var total_edits = 0
    var total_files_edited = 0
    var total_files_failed = 0
    for (file in files) {
        if (has_expect_directive(file)) {
            print("SKIP {file} (expect-directive test fixture)\n") if (!cfg.quiet)
            continue
        }
        var records : array<EditRecord>
        let compiled = collect_edits(file, records)
        if (!compiled) {
            total_files_failed++
            print("FAIL {file} (compile error)\n")
            continue
        }
        if (empty(records)) {
            print("PASS {file}\n") if (!cfg.quiet)
            continue
        }
        // Group by rule for the per-file report.
        var rule_counts : table<string; int>
        for (rec in records) {
            if (key_exists(rule_counts, rec.rule)) {
                rule_counts[rec.rule]++
            } else {
                rule_counts[rec.rule] = 1
            }
        }
        // Build summary outside the keys() iteration to keep the table unlocked.
        var rule_strs : array<string>
        for (rule_name in keys(rule_counts)) {
            rule_strs |> push(string(rule_name))
        }
        var summary = ""
        for (rule_str in rule_strs) {
            let cnt = rule_counts[rule_str]
            summary = "{summary} {rule_str}={cnt}"
        }
        let action = cfg.dry_run ? "WOULD FIX" : "FIX"
        print("{action} {file} ({length(records)} edits:{summary})\n")
        let n = apply_edits_to_file(file, records, cfg.dry_run)
        total_edits += n
        total_files_edited++
    }
    print("\n--- Summary ---\n")
    print("{length(files)} files scanned, {total_files_edited} edited, ")
    print("{total_edits} edits applied, {total_files_failed} failed\n")
    if (cfg.dry_run) {
        print("(dry-run — no files written)\n")
    }
    return total_files_failed > 0 ? 1 : 0
}
