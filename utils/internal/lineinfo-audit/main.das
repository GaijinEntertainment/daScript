options gen2
options rtti = true
options strict_smart_pointers = false
options persistent_heap

require daslib/clargs
require daslib/rtti
require daslib/ast
require daslib/ast_boost
require daslib/fio
require daslib/strings_boost
require strings


[CommandLineArgs]
struct Config {
    @clarg_short = "p"
    @clarg_doc = "Path(s) to scan (file or directory). Repeatable."
    path : array<string>

    @clarg_short = "s"
    @clarg_doc = "Max sample locations printed per violation group (default 3)"
    samples : int = 3

    @clarg_doc = "Stop after N files (0 = all); quick triage runs"
    max_files : int = 0

    @clarg_short = "v"
    @clarg_doc = "Per-file progress + compile failure details"
    verbose : bool

    @clarg_doc = "List files that failed to compile"
    fails : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}


def normalize_path(p : string) : string {
    return normalize(p) |> replace("\\", "/")
}


def has_expect_directive(file : string) : bool {
    let text = fread(file)
    if (empty(text)) return false
    let lines <- text |> split("\n")
    for (line in lines) {
        if (line |> strip |> starts_with("expect ")) return true
    }
    return false
}


def is_skip_file(path : string; basename : string) : bool {
    if (basename == "builtin.das") return true
    // debugger/profiler install thread-local agents at compile time; a
    // second install in the same process aborts the scan
    let normalized = normalize_path(path)
    for (skip in ["debugger.das", "profiler.das"]) {
        let prefix = "daslib/{skip}"
        if (normalized == prefix || normalized |> ends_with("/{prefix}")) return true
    }
    return false
}


def scan_das_files(path : string; var files : array<string>; var cache : table<string; void?>) {
    let st = stat(path)
    if (!st.is_valid) return
    if (st.is_reg) {
        if (path |> ends_with(".das")) {
            let np = normalize_path(path)
            if (!cache |> key_exists(np) && !is_skip_file(np, base_name(np))) {
                cache |> insert(np, null)
                files |> push(np)
            }
        }
        return
    }
    if (!st.is_dir) return
    fio::dir(path) $(name) {
        if (name == "." || name == ".." || name |> starts_with("_") || name |> starts_with(".")) return
        let full = path_join(path, name)
        let fst = stat(full)
        if (!fst.is_valid) return
        if (fst.is_dir) {
            scan_das_files(full, files, cache)
        } elif (fst.is_reg && name |> ends_with(".das") && !is_skip_file(full, name)) {
            let np = normalize_path(full)
            if (!cache |> key_exists(np)) {
                cache |> insert(np, null)
                files |> push(np)
            }
        }
    }
}


// ===== the audit visitor =====
//
// Ground-truth conventions (probed against the gen2 lexer/parser):
//   lines are 1-based, columns 0-based BYTE offsets, ranges end-EXCLUSIVE.
//   Most nodes carry a single-token anchor (ExprOp2.at spans just the
//   operator, ExprIfThenElse.at just the `if`); full spans live only on
//   ExprBlock, Function.atDecl and ExprLooksLikeCall.atEnclosure.
//
// Checks:
//   anchor    — source text at `at` must equal the token the node names
//   bounds    — range is ordered and inside the file
//   colspill  — a range endpoint points past the end of its source line
//   envelope  — expression lies inside its function's atDecl span
//   block     — ExprBlock starts at `{` and ends at `}`
//   enclosure — call atEnclosure ends at `)`

struct Violation {
    file : string
    line : int
    column : int
    check : string
    node : string
    expected : string
    actual : string
}


class AuditVisitor : AstVisitor {
    src_lines : array<string>
    file_norm : string
    fn_generated : bool = true
    fn_decl_valid : bool = false
    fn_decl_l0 : int
    fn_decl_c0 : int
    fn_decl_l1 : int
    fn_decl_c1 : int
    sb_depth : int = 0
    seen : table<string; void?>
    out : array<Violation>

    def same_file(at : LineInfo) : bool {
        if (at.fileInfo == null) return false
        return normalize_path(string(at.fileInfo.name)) == file_norm
    }

    def report(check, node, expected, actual : string; at : LineInfo) {
        let key = "{check}|{node}|{int(at.line)}|{int(at.column)}"
        if (key_exists(seen, key)) return
        seen |> insert(key, null)
        out |> push(Violation(file = file_norm, line = int(at.line), column = int(at.column),
            check = check, node = node, expected = expected, actual = actual))
    }

    def line_of(l : int) : string {
        return (l >= 1 && l <= length(src_lines)) ? src_lines[l - 1] : ""
    }

    // single-line slice [column, last_column); diagnostic tag when the
    // range itself is malformed
    def text_at(at : LineInfo) : string {
        let l = int(at.line)
        if (l < 1 || l > length(src_lines)) return "<line-oob>"
        if (int(at.last_line) != l) return "<multiline>"
        let ln = src_lines[l - 1]
        let c0 = int(at.column)
        let c1 = int(at.last_column)
        if (c0 < 0 || c1 > length(ln) || c1 <= c0) return "<col-oob:{c0}..{c1}/{length(ln)}>"
        return slice(ln, c0, c1)
    }

    def check_anchor(node : string; at : LineInfo; expected : string) {
        let actual = text_at(at)
        if (actual != expected) {
            report("anchor", node, expected, actual, at)
        }
    }

    def check_anchor2(node : string; at : LineInfo; expected, alt : string) {
        let actual = text_at(at)
        if (actual != expected && actual != alt) {
            report("anchor", node, expected, actual, at)
        }
    }

    def first_char(at : LineInfo) : string {
        let ln = line_of(int(at.line))
        let c = int(at.column)
        if (c < 0 || c >= length(ln)) return ""
        return slice(ln, c, c + 1)
    }

// generic per-expression checks
    def override preVisitExpression(expr : ExpressionPtr) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let node = expr.__rtti
        let l = int(expr.at.line)
        let c = int(expr.at.column)
        let ll = int(expr.at.last_line)
        let lc = int(expr.at.last_column)
        if (l < 1 || l > length(src_lines) || ll < l || ll > length(src_lines) || (ll == l && lc <= c)) {
            report("bounds", node, "ordered in-file range", "{l}:{c}..{ll}:{lc}", expr.at)
            return
        }
        if (c > length(line_of(l)) || lc > length(line_of(ll))) {
            report("colspill", node, "cols within line", "{l}:{c}..{ll}:{lc} (lines {length(line_of(l))}/{length(line_of(ll))})", expr.at)
        }
        if (fn_decl_valid) {
            let before = (l < fn_decl_l0) || (l == fn_decl_l0 && c < fn_decl_c0)
            let after = (ll > fn_decl_l1) || (ll == fn_decl_l1 && lc > fn_decl_c1)
            if (before || after) {
                report("envelope", node, "within {fn_decl_l0}:{fn_decl_c0}..{fn_decl_l1}:{fn_decl_c1}", "{l}:{c}..{ll}:{lc}", expr.at)
            }
        }
    }

// name-bearing anchors
    // macro rewrites and generic reification mangle call/var names
    // (`__::testing`success`<hash>`) while `at` stays at the source
    // spelling — correct lineinfo. Accept any backtick/:: segment.
    def name_segment_matches(mangled : string; actual : string) : bool {
        if (mangled == actual) return true
        // `fio::fopen` in source vs `__::fio`fopen`<hash>` in the node —
        // compare the actual's last :: segment against mangled segments too
        var actual_leaf = actual
        let aix = rfind(actual, "::")
        if (aix >= 0) {
            actual_leaf = slice(actual, aix + 2, length(actual))
        }
        for (seg in split(replace(mangled, "::", "`"), "`")) {
            if (!empty(seg) && (seg == actual || seg == actual_leaf)) return true
        }
        return false
    }

    def override preVisitExprVar(expr : ExprVar?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let nm = string(expr.name)
        if (nm |> starts_with("__")) return  // compiler-internal temps
        let actual = text_at(expr.at)
        if (actual |> starts_with("<col-oob") || actual |> starts_with("<line-oob")) return  // bounds check owns malformed ranges
        // segment match covers aka-mangled names (`access`at`176`24`)
        if (!name_segment_matches(nm, actual)) {
            report("anchor", "ExprVar", nm, actual, expr.at)
        }
    }

    def override preVisitExprCall(expr : ExprCall?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let nm = string(expr.name)
        let actual = text_at(expr.at)
        // malformed ranges belong to the bounds check; `[`/`{` are
        // array/table literal ctor desugar anchored at the bracket;
        // `.` is dot-sugar method/property lowering anchored at the dot
        if (actual |> starts_with("<col-oob") || actual |> starts_with("<line-oob")
                || actual == "[" || actual == "\{" || actual == ".") return
        if (!name_segment_matches(nm, actual)) {
            report("anchor", "ExprCall", nm, actual, expr.at)
        }
        // full-span sanity: enclosure must end at the closing paren
        let el = int(expr.atEnclosure.last_line)
        let elc = int(expr.atEnclosure.last_column)
        if (el >= 1 && same_file(expr.atEnclosure)) {
            let ln = line_of(el)
            let closer = (elc >= 1 && elc <= length(ln)) ? slice(ln, elc - 1, elc) : "<oob>"
            if (closer != ")") {
                report("enclosure", "ExprCall", ")", closer, expr.atEnclosure)
            }
        }
    }

    def override preVisitExprField(expr : ExprField?) {
        if (expr.genFlags.generated || !same_file(expr.atField)) return
        check_anchor("ExprField", expr.atField, string(expr.name))
    }

// declarations
    def override preVisitFunction(fun : FunctionPtr) {
        fn_generated = fun.flags.generated
        fn_decl_valid = false
        if (fn_generated || fun.at.fileInfo == null || !same_file(fun.at)) return
        let nm = string(fun.name)
        let actual = text_at(fun.at)
        // `def operator X` anchors at the `operator` keyword by convention
        if (!name_segment_matches(nm, actual) && !(actual |> starts_with("operator"))) {
            report("anchor", "Function", nm, actual, fun.at)
        }
        fn_decl_l0 = int(fun.atDecl.line)
        fn_decl_c0 = int(fun.atDecl.column)
        fn_decl_l1 = int(fun.atDecl.last_line)
        fn_decl_c1 = int(fun.atDecl.last_column)
        fn_decl_valid = fn_decl_l0 >= 1
    }

    def override preVisitFunctionArgument(fun : FunctionPtr; arg : VariablePtr; lastArg : bool) {
        if (fn_generated || !same_file(arg.at)) return
        let nm = string(arg.name)
        if (nm == "self" || nm |> starts_with("__")) return  // implicit method/internal args
        check_anchor("FunctionArgument", arg.at, nm)
    }

    def override preVisitExprLet(expr : ExprLet?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // comprehension/generator desugar plants non-generated lets at `for`
        if (actual != "let" && actual != "var" && actual != "for") {
            report("anchor", "ExprLet", "let|var", actual, expr.at)
        }
        for (v in expr.variables) {
            if (same_file(v.at) && !(v.name |> starts_with("__"))) {
                let vnm = string(v.name)
                let vtext = text_at(v.at)
                // segment match covers aka-mangled names
                if (!name_segment_matches(vnm, vtext)) {
                    report("anchor", "LetVariable", vnm, vtext, v.at)
                }
            }
        }
        // atInit spans the declaration; a phantom lexer-synthesized `;`
        // makes it overshoot the line end — measure how often
        let il = int(expr.atInit.last_line)
        let ilc = int(expr.atInit.last_column)
        if (il >= 1 && same_file(expr.atInit) && ilc > length(line_of(il))) {
            report("colspill", "ExprLet.atInit", "cols within line", "..{il}:{ilc} (line {length(line_of(il))})", expr.atInit)
        }
    }

// operator anchors
    def override preVisitExprOp1(expr : ExprOp1?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        var op = string(expr.op)
        if (op == "+++") {
            op = "++"
        } elif (op == "---") {
            op = "--"
        }
        check_anchor("ExprOp1", expr.at, op)
    }

    def override preVisitExprOp2(expr : ExprOp2?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // variant `is`/`as` tests lower to == on the index, anchor intact
        if (expr.op != actual && actual != "is" && actual != "as") {
            report("anchor", "ExprOp2", string(expr.op), actual, expr.at)
        }
    }

    def override preVisitExprCopy(expr : ExprCopy?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // property/clone sugar can lower `:=`/`<-` to a copy, anchor intact
        if (actual != "=" && actual != ":=" && actual != "<-") {
            report("anchor", "ExprCopy", "=", actual, expr.at)
        }
    }

    def override preVisitExprMove(expr : ExprMove?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // infer promotes `=` on non-copyable types to a move, anchor intact
        if (actual != "<-" && actual != "=") {
            report("anchor", "ExprMove", "<-", actual, expr.at)
        }
    }

    def override preVisitExprClone(expr : ExprClone?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        check_anchor("ExprClone", expr.at, ":=")
    }

// keyword anchors
    def override preVisitExprReturn(expr : ExprReturn?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // `=>` arrow bodies synthesize the return at the arrow;
        // comprehension desugar plants one at `for`
        if (actual != "return" && actual != "=>" && actual != "for" && actual != "<-") {
            report("anchor", "ExprReturn", "return", actual, expr.at)
        }
    }

    def override preVisitExprBreak(expr : ExprBreak?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        check_anchor("ExprBreak", expr.at, "break")
    }

    def override preVisitExprContinue(expr : ExprContinue?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        check_anchor("ExprContinue", expr.at, "continue")
    }

    def override preVisitExprIfThenElse(expr : ExprIfThenElse?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        // `for` — comprehension where-filter desugar
        if (actual != "if" && actual != "elif" && actual != "static_if" && actual != "static_elif" && actual != "for") {
            report("anchor", "ExprIfThenElse", "if|elif|static_if|static_elif", actual, expr.at)
        }
    }

    def override preVisitExprWhile(expr : ExprWhile?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        check_anchor("ExprWhile", expr.at, "while")
    }

    def override preVisitExprFor(expr : ExprFor?) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        check_anchor("ExprFor", expr.at, "for")
    }

// block braces
    def override preVisitExprBlock(blk : ExprBlock?) {
        if (blk.genFlags.generated || !same_file(blk.at)) return
        let l = int(blk.at.line)
        let ll = int(blk.at.last_line)
        let lc = int(blk.at.last_column)
        if (l < 1 || ll < 1) return
        let opener = first_char(blk.at)
        let end_ln = line_of(ll)
        let closer = (lc >= 1 && lc <= length(end_ln)) ? slice(end_ln, lc - 1, lc) : "<oob>"
        if (opener != "\{" || closer != "\}") {
            report("block", "ExprBlock", "\{...\}", "'{opener}'..'{closer}'", blk.at)
        }
    }

// literal anchors
    def override preVisitExprStringBuilder(expr : ExprStringBuilder?) {
        sb_depth++
    }
    def override visitExprStringBuilder(var expr : ExprStringBuilder?) : ExpressionPtr {
        sb_depth--
        return expr
    }

    // constant folding (typeinfo, enum compares, unary +/-) legitimately
    // leaves the folded const anchored on the folded expression's range —
    // only structurally-broken ranges are reportable here
    def check_const_range(node : string; expr : ExpressionPtr) {
        if (expr.genFlags.generated || !same_file(expr.at)) return
        let actual = text_at(expr.at)
        if (actual == "<multiline>") {
            report("anchor", node, "single-token literal", "{int(expr.at.line)}:{int(expr.at.column)}..{int(expr.at.last_line)}:{int(expr.at.last_column)}", expr.at)
        }
    }

    def override preVisitExprConstInt(expr : ExprConstInt?) {
        check_const_range("ExprConstInt", expr)
    }
    def override preVisitExprConstUInt(expr : ExprConstUInt?) {
        check_const_range("ExprConstUInt", expr)
    }
    def override preVisitExprConstInt64(expr : ExprConstInt64?) {
        check_const_range("ExprConstInt64", expr)
    }
    def override preVisitExprConstUInt64(expr : ExprConstUInt64?) {
        check_const_range("ExprConstUInt64", expr)
    }
    def override preVisitExprConstFloat(expr : ExprConstFloat?) {
        check_const_range("ExprConstFloat", expr)
    }
    def override preVisitExprConstDouble(expr : ExprConstDouble?) {
        check_const_range("ExprConstDouble", expr)
    }
    def override preVisitExprConstBool(expr : ExprConstBool?) {
        check_const_range("ExprConstBool", expr)
    }

    def override preVisitExprConstString(expr : ExprConstString?) {
        if (sb_depth > 0) return  // interpolation pieces carry quote-less sub-ranges
        check_const_range("ExprConstString", expr)
    }
}


// ===== per-file driver =====

// visit one module function if it belongs to the audited file; generic
// templates and their reified instances share (line:column) — first wins
def visit_one(func : FunctionPtr; var v : AuditVisitor?;
              var seen_fn : table<string; void?>; file_norm : string) {
    // generated functions (class ctors/finalizers, lifted lambdas) are
    // desugar output — user-written lambda bodies are still covered via
    // the ExprMakeBlock in the enclosing real function
    if (func.flags.builtIn || func.flags.generated || func.at.fileInfo == null
            || normalize_path(string(func.at.fileInfo.name)) != file_norm) return
    let loc = "{int(func.at.line)}:{int(func.at.column)}"
    if (key_exists(seen_fn, loc)) return
    seen_fn |> insert(loc, null)
    make_visitor(*v) $(adapter) {
        visit(func, adapter)
    }
}


def audit_file(file : string; verbose : bool;
               var counts : table<string; int>;
               var samples : table<string; array<string>>;
               max_samples : int;
               var n_violations : int&) : bool {
    let text = fread(file)
    var src_lines <- text |> split("\n")
    for (ln in src_lines) {
        if (ln |> ends_with("\r")) {
            ln = slice(ln, 0, length(ln) - 1)
        }
    }
    var inscope access <- make_file_access("")
    var ok_compile = true
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.ignore_shared_modules = true
            cop.export_all = true
            cop.version_2_syntax = true
            cop.no_optimizations = true
            compile_file(file, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    ok_compile = false
                    if (verbose) {
                        to_log(LOG_ERROR, "FAIL {file}\n  {issues}\n")
                    }
                    return
                }
                let thisMod = get_this_module(program)
                let file_norm = normalize_path(file)
                var v = new AuditVisitor()
                v.src_lines := src_lines
                v.file_norm = file_norm
                var seen_fn : table<string; void?>
                for_each_generic(thisMod, "") $(func) {
                    visit_one(func, v, seen_fn, file_norm)
                }
                for_each_function(thisMod, "") $(func) {
                    visit_one(func, v, seen_fn, file_norm)
                }
                for (viol in v.out) {
                    let key = "{viol.check}/{viol.node}"
                    if (key_exists(counts, key)) {
                        counts[key] = counts[key] + 1
                    } else {
                        counts |> insert(key, 1)
                    }
                    n_violations++
                    if (length(samples[key]) < max_samples) {
                        samples[key] |> push("{viol.file}:{viol.line}:{viol.column} expected {viol.expected} got {viol.actual}")
                    }
                }
                unsafe {
                    delete v
                }
            }
        }
    }
    return ok_compile
}


[export]
def main() : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "error: {args_r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "lineinfo-audit")
        return 1
    }
    var cfg <- args_r |> move_unwrap
    if (cfg.help || empty(cfg.path)) {
        print_help(get_command_info(type<Config>), "lineinfo-audit")
        return cfg.help ? 0 : 1
    }

    var files : array<string>
    var cache : table<string; void?>
    for (p in cfg.path) {
        scan_das_files(p, files, cache)
    }
    if (empty(files)) {
        to_log(LOG_ERROR, "error: no .das files under given paths\n")
        return 1
    }
    sort(files)
    if (cfg.max_files > 0 && length(files) > cfg.max_files) {
        files |> resize(cfg.max_files)
    }

    var counts : table<string; int>
    var samples : table<string; array<string>>
    var n_ok = 0
    var n_fail = 0
    var n_skip = 0
    var n_violations = 0
    var failed_files : array<string>
    to_log(LOG_INFO, "auditing {length(files)} file(s)...\n")
    for (i in range(length(files))) {
        let file = files[i]
        if (has_expect_directive(file)) {
            n_skip++
            continue
        }
        if (cfg.verbose) {
            to_log(LOG_INFO, "[{i + 1}/{length(files)}] {file}\n")
        }
        if (audit_file(file, cfg.verbose, counts, samples, cfg.samples, n_violations)) {
            n_ok++
        } else {
            n_fail++
            failed_files |> push(file)
        }
    }

    to_log(LOG_INFO, "\n=== lineinfo audit ===\n")
    to_log(LOG_INFO, "files: {n_ok} audited, {n_fail} failed to compile, {n_skip} skipped (expect-directive)\n")
    to_log(LOG_INFO, "violations: {n_violations} in {length(counts)} group(s)\n\n")

    var groups <- [for (k in keys(counts)); (counts?[k] ?? 0, k)]
    sort(groups) $(a, b) => a._0 > b._0
    for (g in groups) {
        to_log(LOG_INFO, "{g._1}: {g._0}\n")
        for (s in samples[g._1]) {
            to_log(LOG_INFO, "    {s}\n")
        }
    }

    if (cfg.fails && !empty(failed_files)) {
        to_log(LOG_INFO, "\ncompile failures:\n")
        for (f in failed_files) {
            to_log(LOG_INFO, "    {f}\n")
        }
    }
    return 0
}
