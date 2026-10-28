options gen2
options indenting = 4
options strict_smart_pointers = true

module style_lint shared private

//! Style lint module.
//!
//! Detects non-idiomatic patterns in daslang code at compile time.
//! When this module is required, a lint pass runs after compilation and reports
//! warnings as ``CompilationError::runtime_macro_style`` (error code 31209).
//!
//! Rules:
//!   STYLE001 — unnecessary <| pipe before block argument
//!   STYLE002 — <| pipe before parameterless block (remove pipe and $())
//!   STYLE003 — redundant $() on parameterless block argument
//!   STYLE005 — braces around a single-statement early exit ('if (cond) { return }') are noise — drop the braces or use postfix
//!   STYLE006 — string(__rtti) comparison should use `is` operator
//!   STYLE010 — if (true) should be a bare block
//!   STYLE011 — variable declaration followed by immediate assignment
//!   STYLE012 — array<T> var initialized via push/emplace; use an array literal
//!   STYLE013 — struct (or `new`-allocated struct pointer) var with default/empty init followed by a run of field assignments
//!   STYLE014 — comment block exceeds 3 lines at module/public scope (on by default under daslib/, opt-in elsewhere via ``options _comment_hygiene = true``; suppress with '//!@nolint' or '// nolint:STYLE014')
//!   STYLE015 — comment block inside a 'def private' over its cap: 3 lines for a '//!' contract doc, 1 line for a plain '//' block (on by default under daslib/, opt-in elsewhere via ``options _comment_hygiene = true``; suppress with '// nolint:STYLE015')
//!   STYLE016 — adjacent guards leading to identical early-exit can be combined with '||'
//!   STYLE017 — 'if (cond) return true; else return false' — use 'return cond' (or 'return !cond')
//!   STYLE018 — redundant boolean comparison ('b == true' / 'b != false') — use 'b' / '!b'
//!   STYLE019 — nested min/max — use 'clamp(x, lo, hi)' from math module
//!   STYLE020 — from_JV(v, type<T>, defV) on a scalar — use 'v ?? defV' (operator ?? from json_boost)
//!   STYLE021 — table<string; JsonValue?> built by repeated insert — use 'var v = JV((k1=..., k2=...))'
//!   STYLE022 — bitfield compound-bit-set/clear ('foo |= BfT.m' / 'foo &= ~BfT.m') — use field-assign 'foo.m = true' / 'foo.m = false'
//!   STYLE023 — bitfield test via int-cast ('int(bf & BfT.m) != 0' / '== 0') — use 'bf.m' / '!bf.m' directly
//!   STYLE024 — redundant 'unsafe' wrap; inner expression / block body has no operation requiring unsafe — drop the wrap
//!   STYLE025 — 'unsafe { ... }' block where only one statement needs unsafe — narrow to 'unsafe(<sub-expr>)'
//!   STYLE026 — nested 'unsafe { ... }' block; outer wrap already covers the scope — drop the inner
//!   STYLE027 — var array<T> / table<K;V> with empty default-init followed by a for-loop that only push/insert into it; use a comprehension
//!   STYLE028 — 'self->method(...)' inside a class method is optional — drop 'self->' and call 'method(...)' directly (compiler auto-promotes to the same invoke)
//!   STYLE029 — non-public 'require X' used only for ONE module X re-exports — require it directly and drop X (skipped when ≥2 re-exports are used: aggregation facade; when X provides macros or [init]; or when X is a member of a 'require [group]', whose members are one unit)
//!   STYLE030 — non-public 'require X' that is entirely unused — no symbol from X (or anything it re-exports) is referenced; drop it (skipped when X provides any macro, or an [init]/[finalize]/[before_reload]/[after_reload]/[live_command] function, or only re-exports builtins used through it, or X or its public re-export closure exports a function matching an unresolved call inside an uninstanced generic body). A 'require [group]' is one unit: reported only when no member is used. Suppress a deliberate keep with '// nolint:STYLE030'
//!   STYLE031 — table<K;V> (or table<K> set) var initialized via a run of insert / '[]=' statements; use a table (set) literal move-assign
//!   STYLE032 — array<T> with empty default-init then a single 'push_from' / 'push_clone_from' from another array<T>; that is a clone — use 'var w := SRC' (clone-assign). Cloneable elements only (a copyable-but-not-cloneable element such as a lambda stays silent — ':=' would not compile)
//!   STYLE033 — run of >=2 'push_from' / 'push_clone_from' from arrays; into a fresh-empty array that is a concatenation (use 'var c <- concat(s1, s2, ...)', needs 'require daslib/linq'; cloneable elements only, as STYLE032); into an existing array — plain var or a pure chain like 'obj.arr' / 'bags[0].nums' — collapse to one variadic call of the same name ('dst |> push_from(...)' or 'dst |> push_clone_from(...)')
//!   STYLE034 — reinterpret<T?>(addr(x)) — use addr<T?>(x); one unsafe() covers both halves (pointer targets only; the sugar's own desugared output is exempt via fromAddrSugar)
//!   STYLE035 — non-int numeric variable compared with a built-in numeric cast of '<char>' — make the variable int and compare directly with the character literal
//!   STYLE036 — type contract ('-const', '-&', '-[]', '-#', '==const', '==&') on an already-resolved cast/reinterpret/upcast/addr target does nothing — drop it
//!   STYLE037 — function or closure cyclomatic complexity exceeds the limit (override per module with 'options _cyclomatic_complexity = N'; 0 disables) — split it into smaller functions
//!   STYLE038 — function exceeds the line-count limit (physical lines; override per module with 'options _function_length = N'; 0 disables) — split it into smaller functions
//!   STYLE040 — a run of statements duplicated elsewhere in the module that a helper function could absorb (on by default under daslib/ and utils/, override per module with 'options _duplicate_regions'; thresholds via 'options _dupe_min_nodes' / '_dupe_min_statements'; suppress with '// nolint:STYLE040' on the reported line)
//!   STYLE039 — non-ASCII byte in a string literal (on by default under daslib/ and modules/, override per module with 'options _ascii_strings'; suppress with '// nolint:STYLE039') — emitted strings and error messages must be ASCII unless intended
//!   STYLE044 — 'match' block written with the deprecated 'if (pattern) { body }' arms — write 'pattern => $ { body }' arms ('$v(n)' becomes the bare name 'n'; a bare name that compared becomes 'match_expr(name)')
//!   STYLE041 — bool flag initialized false, set true on terminal paths, then consumed by a single 'if (flag) return ...' — a return value in disguise; return directly at each set site (walk-abort callbacks, negative polarity, non-terminal sets, and init-true flags are exempt)
//!   STYLE046 — a statement-form 'match' arm whose '$ { }' block
//!     holds one expression - write 'pattern => EXPR'
//!   STYLE047 — a comparator block or an 'operator <' comparing
//!     two values key by key - write 'cmp_by(...)' or '[ordered]'
//!     (daslib/cmp)
//!   STYLE045 — a &&/||/! condition over one int variable that an
//!     interval lattice collapses to a single comparison ('x != 1 &&
//!     x > 0' is 'x >= 2')
//!   STYLE048 — if/elif chain (nested ifs that are a whole arm body
//!     flattened in) whose arms test the same subjects against
//!     constants, ranges or variant cases — rewrite as a 'match'
//!     (default off)

require daslib/ast_boost
require daslib/dupe_detect
require daslib/fio
require daslib/is_local
require daslib/lint_config
require daslib/lint_fixups
require daslib/match_solver
require daslib/option
require daslib/result
require daslib/strings_boost
require math
require strings
require daslib/cmp

let STYLE042_MAX_LINE = 100

let private INT64_MIN_V = -9223372036854775807l - 1l
let private INT64_MAX_V = 9223372036854775807l

// One inclusive interval of the STYLE045 value lattice. int64
// endpoints so `c - 1` and `c + 1` cannot wrap at an int boundary.
struct private Ivl {
    lo : int64
    hi : int64
}

// STYLE033 arity caps — never suggest a call shape with no matching overload.
let MAX_CONCAT_ARITY = 8
let MAX_VARIADIC_PUSH_ARITY = 4

let STYLE037_DEFAULT_MAX = 20

let ST041_MAX_ARG_DEPTH = 8

let STYLE038_DEFAULT_MAX_LINES = 80

struct UnsafeFrame {
    //! Per-expression subtree summary tracked on `StyleLintVisitor.unsafe_stack`.
    count : int
    has_non_local_let_ref : bool
    @do_not_delete op : Expression?    //!< the node that marked unsafe, when count is 1
    @do_not_delete node : Expression?  //!< the node this frame summarizes
}

struct St041Scan {
    //! Transient per-candidate classification state for check_style041_flag_return.
    rejected : bool
    write_count : int
    write_top : int = -1
    writes_terminal : bool = true
    guard_stack : array<bool>
    plumb_count : int
    plumb_tops : array<int>
    consume_count : int
    consume_top : int = -1
    consume_top_level : bool
    consume_ok : bool
    consume_moved : bool
}

enum private St041Terminal {
    not_terminal
    then_break
    then_continue
    arm_end
}

enum private CtorFieldState {
    unknown
    default_empty
    initialized
}

struct St041Candidate {
    //! Armed candidate awaiting the visitFunction reference-count cross-check.
    classified_refs : int
    observed_refs : int
    poisoned : bool
    msg : string
}

def private byte_at(s : string; i : int) : int {
    var b = -1
    peek_data(s) $(d) {
        if (i >= 0 && i < length(d)) {
            b = int(d[i])
        }
    }
    return b
}

def private code_find(s, two : string) : int {
    let first_ch = byte_at(two, 0)
    let second_ch = byte_at(two, 1)
    var found = -1
    peek_data(s) $(d) {
        var quote = 0
        var escaped = false
        for (c, i in d, count()) {
            let ch = int(c)
            if (quote != 0) {
                if (escaped) {
                    escaped = false
                } elif (ch == '\\') {
                    escaped = true
                } elif (ch == quote) {
                    quote = 0
                }
            } elif (ch == '"' || ch == '\'') {
                quote = ch
            } elif (ch == first_ch && i + 1 < length(d) && int(d[i + 1]) == second_ch) {
                found = i
                break
            }
        }
    }
    return found
}

//! STYLE047 - one key of a hand-written ordering, spelled over
//! ``_``, the compared element.
struct private St047Key {
    text : string
    greater : bool
}

//! STYLE047 - spells a key: the compared elements become ``_`` and
//! ``#``, let-bound locals their initializers; hidden arguments go.
class private St047Speller : AstVisitor {
    va : uint64
    vb : uint64
    flip : bool
    @do_not_delete lets : table<uint64; Expression?>
    def override visitExprVar(var expr : ExprVar?) : Expression? {
        let v = intptr(expr.variable)
        let name = (v == va) != flip ? "_" : "#"
        if (v == va || v == vb) return new ExprVar(at = expr.at, name := name)
        var init = unsafe(lets?[v]) ?? null
        if (init == null) return expr
        var spelled = clone_expression(init)
        make_visitor(self) $(adapter) {
            spelled = visit(spelled, adapter)
        }
        return spelled
    }
    def override visitExprCall(var expr : ExprCall?) : Expression? {
        while (!empty(expr.arguments) && (back(expr.arguments) is ExprFakeContext
                || back(expr.arguments) is ExprFakeLineInfo)) {
            expr.arguments |> pop()
        }
        return expr
    }
}

//! ``e`` spelled over one compared element (``flip`` picks which);
//! "" when it reads none or both.
def private st047_spell(var sp : St047Speller?; e : Expression?; flip : bool) : string {
    var text = ""
    ast_gc_guard() {
        var spelled = clone_expression(e)
        sp.flip = flip
        make_visitor(*sp) $(adapter) {
            spelled = visit(spelled, adapter)
        }
        let refs <- find_var_refs(spelled)
        let reads_self = find_index_if(refs, $(v) => v.name == "_") >= 0
        if (reads_self && find_index_if(refs, $(v) => v.name == "#") < 0) {
            text = describe(spelled)
            text = spelled is ExprOp2 ? slice(text, 1, -1) : text
        }
    }
    return text
}

//! The key ``l`` and ``r`` order by: the same expression, each over
//! one compared element; empty otherwise.
def private st047_key(var sp : St047Speller?; l, r : Expression?; greater : bool) : St047Key {
    for (flip in [false, true]) {
        let text = st047_spell(sp, l, flip)
        if (!empty(text) && text == st047_spell(sp, r, !flip)) {
            return St047Key(text = text, greater = greater != flip)
        }
    }
    return St047Key()
}

//! The key an ``op`` (``<``, ``>``, ``!=``, ``==``) of two keys
//! compares; empty otherwise.
def private st047_op(var sp : St047Speller?; e : Expression?; op : string) : St047Key {
    if (!(e is ExprOp2) || (e as ExprOp2).op != op) return St047Key()
    return st047_key(sp, (e as ExprOp2).left, (e as ExprOp2).right, op == ">")
}

//! ``cond ? t : f``: ``cond`` tests the one key a branch orders by,
//! the other branch (none for a guard) orders by the rest.
def private st047_branch(var sp : St047Speller?; cond, t, f : Expression?;
                         var keys : array<St047Key>) : bool {
    let ne = st047_op(sp, cond, "!=").text
    let tested = empty(ne) ? st047_op(sp, cond, "==").text : ne
    var one : array<St047Key>
    if (empty(tested) || !st047_keys(sp, empty(ne) ? f : t, one)
            || length(one) != 1 || one[0].text != tested) return false
    keys |> push(one[0])
    let rest = empty(ne) ? t : f
    return rest == null || st047_keys(sp, rest, keys)
}

//! Appends the keys a block orders by: let-bound locals and guards
//! ``if (k != k') return k < k'``, then a last statement that does.
def private st047_block_keys(var sp : St047Speller?; blk : ExprBlock?;
                             var keys : array<St047Key>) : bool {
    for (s, i in blk.list, count()) {
        if (i + 1 == length(blk.list)) return st047_keys(sp, s, keys)
        if (s is ExprLet) {
            for (v in (s as ExprLet).variables) {
                return false if (v.init == null)
                sp.lets[intptr(v)] = v.init
            }
        } elif (!(s is ExprIfThenElse)) {
            return false
        } else {
            let ite = s as ExprIfThenElse
            let guard = ite.if_false == null && st047_branch(sp, ite.cond, ite.if_true, null, keys)
            return false if (!guard)
        }
    }
    return false
}

//! Appends the keys ``e`` orders by - a block, an ``if``, ``?:``,
//! ``k < k' || (k == k' && rest)``, or one comparison; false when it
//! is not a key-by-key ordering.
def private st047_keys(var sp : St047Speller?; e : Expression?; var keys : array<St047Key>) : bool {
    if (e is ExprBlock) return st047_block_keys(sp, e as ExprBlock, keys)
    if (e is ExprReturn) return st047_keys(sp, (e as ExprReturn).subexpr, keys)
    if (e is ExprIfThenElse) {
        let ite = e as ExprIfThenElse
        return ite.if_false != null && st047_branch(sp, ite.cond, ite.if_true, ite.if_false, keys)
    }
    if (e is ExprOp3) {
        let op3 = e as ExprOp3
        return st047_branch(sp, op3.subexpr, op3.left, op3.right, keys)
    }
    if (e is ExprOp2 && (e as ExprOp2).op == "||") {
        let rest = (e as ExprOp2).right
        if (!(rest is ExprOp2) || (rest as ExprOp2).op != "&&") return false
        let tie = rest as ExprOp2
        return st047_branch(sp, tie.left, tie.right, (e as ExprOp2).left, keys)
    }
    var key = st047_op(sp, e, "<")
    key = empty(key.text) ? st047_op(sp, e, ">") : key
    keys |> push(key)
    return !empty(key.text)
}

//! The keys a comparator body orders by its arguments ``va`` and
//! ``vb``; empty when it is not a key-by-key ordering.
def private st047_body_keys(body : Expression?; va, vb : Variable?) : array<St047Key> {
    var keys : array<St047Key>
    var sp = new St047Speller(va = intptr(va), vb = intptr(vb))
    if (!st047_keys(sp, body, keys)) {
        keys |> clear()
    }
    unsafe {
        delete sp
    }
    return <- keys
}

//! The source text between two ``(line, column)`` points of the file
//! of ``at``.
def private st047_span_text(at : LineInfo; from, to : int2) : string {
    return build_string() $(var w) {
        for (ln in range(from.x, to.x + 1)) {
            let src = lint_source_line(at, ln)
            w |> write(slice(src, ln == from.x ? from.y : 0, ln == to.x ? to.y : length(src)))
            w |> write(ln != to.x ? "\n" : "")
        }
    }
}

//! STYLE047 - a finding collected during the walk, reported once
//! the walk is done.
struct private St047Site {
    @do_not_delete fn : Function?
    @do_not_delete node : Expression?
    message : string
    edits : array<LintFix>
}

let ST048_MAX_ALTS = 8
let ST048_MAX_SUBJECTS = 4

enum private St048Kind {
    eq
    lo
    hi
    variant_case
}

struct St048Atom {
    //! One test a match pattern can hold: `subject` against a constant, a
    //! range bound or a variant case. `text` is what the hint prints,
    //! `cell` what the solver compares, `column` the subject's domain.
    subject : string
    kind : St048Kind
    text : string
    cell : SolverCell
    column : SolverColumn
}

struct St048Alt {
    //! One AND-group of an arm's condition: its atoms, and whether
    //! anything is left to a guard.
    atoms : array<St048Atom>
    guard : bool
}

struct St048Arm {
    //! One arm of the chain; `body` is the described body, "" when the
    //! arm runs nothing.
    alts : array<St048Alt>
    is_else : bool
    //! A `synthetic` arm is the fall-through a flattened inner chain
    //! with no `else` leaves.
    synthetic : bool
    body : string
    //! `node` is the `if` the arm's test belongs to.
    @do_not_delete node : ExprIfThenElse const?
}

struct St048Chain {
    //! The arms of an if/elif chain, inner chains flattened in; no arms
    //! when it has no match spelling.
    arms : array<St048Arm>
    flattened : bool
}

struct St048Pattern {
    //! One AND-group as per-subject patterns - `pats` to print, `cells`
    //! to solve - plus whether anything is left to a guard.
    pats : table<string; string>
    cells : table<string; SolverCell>
    guard : bool
}

struct St048Row {
    //! Arm `src` of the chain as the hint prints it (`cells`, a row of
    //! per-subject patterns per alternative) and as the solver sees it
    //! (`arm`).
    src : int
    cells : array<array<string>>
    arm : SolverArm
    arms : array<int>
}

def private st048_subject_path(e : Expression?) : bool {
    if (e is ExprRef2Value) return st048_subject_path((e as ExprRef2Value).subexpr)
    if (e is ExprVar) {
        let v = (e as ExprVar).variable
        return v != null && !v.flags.generated && !starts_with(string(v.name), "__")
    }
    return (e is ExprField && (e as ExprField).name != "__rtti" &&
            st048_subject_path((e as ExprField).value))
}

def private st048_subject_text(e : Expression?) : string {
    if (e is ExprRef2Value) return st048_subject_text((e as ExprRef2Value).subexpr)
    return describe(e)
}

def private st048_const_text(e : Expression?) : string {
    //! The constant as written, so a character literal stays 'w' and an
    //! enum value keeps its short name.
    if (e is ExprRef2Value) return st048_const_text((e as ExprRef2Value).subexpr)
    var text = ""
    if (e.at.fileInfo != null && e.at.line != 0u && e.at.line == e.at.last_line &&
            e.at.last_column > e.at.column) {
        get_file_source_line(e.at.fileInfo, e.at.line) $(line) {
            let start = int(e.at.column)
            let finish = int(e.at.last_column)
            if (start >= 0 && finish <= length(line)) {
                text = strip(slice(line, start, finish))
            }
        }
    }
    return empty(text) ? describe(e) : text
}

def private st048_succ_text(text : string; past : Option<int64>) : string {
    //! The bound one past an inclusive one, spelled as the source spelled
    //! it: hex stays hex, and a type suffix stays.
    return text if (is_none(past))
    var suffix = ""
    for (s in ["ul", "UL", "u", "U", "l", "L"]) {
        if (ends_with(text, s)) {
            suffix = s
            break
        }
    }
    let hex = starts_with(text, "0x") || starts_with(text, "0X")
    return hex ? "0x{fmt(":X", unwrap(past))}{suffix}" : "{unwrap(past)}{suffix}"
}

def private st048_int(e : Expression?) : Option<int64> {
    //! An integer constant's value; none when it does not fit an int64.
    if (e is ExprRef2Value) return st048_int((e as ExprRef2Value).subexpr)
    return some(int64((e as ExprConstInt).value)) if (e is ExprConstInt)
    return some(int64((e as ExprConstUInt).value)) if (e is ExprConstUInt)
    return some((e as ExprConstInt64).value) if (e is ExprConstInt64)
    if (e is ExprConstUInt64 && (e as ExprConstUInt64).value <= uint64(LONG_MAX)) {
        return some(int64((e as ExprConstUInt64).value))
    }
    return none(type<int64>)
}

def private st048_key(e : Expression?) : string {
    //! The name the solver compares a non-integer constant by; an enum
    //! value is spelled as its domain lists it.
    if (e is ExprRef2Value) return st048_key((e as ExprRef2Value).subexpr)
    if (e is ExprConstEnumeration) {
        let ce = e as ExprConstEnumeration
        return "{ce.enumType.name}.{ce.value}"
    }
    return describe(e)
}

def private st048_const(e : Expression?) : bool {
    if (e is ExprRef2Value) return st048_const((e as ExprRef2Value).subexpr)
    return is_expr_const(e)
}

def private st048_column(t : TypeDeclPtr) : SolverColumn {
    //! The solver's column for a subject of type `t`: every value of a
    //! finite type, spelled as `st048_key` spells its constants.
    if (t.isEnum && t.enumType != null) {
        let names <- [for (e in t.enumType.list); "{t.enumType.name}.{e.name}"]
        return <- SolverColumn(domain <- some(names))
    } elif (t.baseType == Type.tBool) {
        return <- SolverColumn(domain <- some(["true", "false"]))
    } elif (t.isVariant) {
        return <- SolverColumn(domain <- some([for (n in t.argNames); string(n)]))
    }
    return <- SolverColumn()
}

def private st048_span(sname : string; kind : St048Kind; text : string;
                       lo, hi : Option<int64>) : Option<St048Atom> {
    //! An atom over the integers from `lo` up to `hi`; none when a bound
    //! did not fit an int64.
    return none(type<St048Atom>) if (is_none(lo) || is_none(hi))
    return some(St048Atom(subject = sname, kind = kind, text = text,
                          cell = SolverCell(spans <- [interval(unwrap(lo), unwrap(hi))])))
}

def private st048_compare(op : string; subj, k : Expression?) : Option<St048Atom> {
    return none(type<St048Atom>) if (!st048_subject_path(subj) || !st048_const(k))
    let t = subj._type
    let sname = st048_subject_text(subj)
    let ktext = st048_const_text(k)
    let integral = (t != null && (t.baseType == Type.tInt || t.baseType == Type.tUInt ||
                    t.baseType == Type.tInt64 || t.baseType == Type.tUInt64))
    if (integral) {
        let n = st048_int(k)
        let past = is_some(n) && unwrap(n) < LONG_MAX ? some(unwrap(n) + 1l) : none(type<int64>)
        if (op == "==") return st048_span(sname, St048Kind.eq, ktext, n, past)
        if (op == ">=") return st048_span(sname, St048Kind.lo, ktext, n, some(LONG_MAX))
        if (op == ">") {
            let lo_text = st048_succ_text(ktext, past)
            return st048_span(sname, St048Kind.lo, lo_text, past, some(LONG_MAX))
        }
        if (op == "<") return st048_span(sname, St048Kind.hi, ktext, some(LONG_MIN), n)
        if (op == "<=") {
            let hi_text = st048_succ_text(ktext, past)
            return st048_span(sname, St048Kind.hi, hi_text, some(LONG_MIN), past)
        }
        return none(type<St048Atom>)
    }
    let keyed = (t != null &&
                 (t.isInteger || t.isEnum || t.isString || t.baseType == Type.tBool))
    return none(type<St048Atom>) if (op != "==" || !keyed)
    return some(St048Atom(subject = sname, kind = St048Kind.eq, text = ktext,
                          cell = SolverCell(keys <- [st048_key(k)]), column = st048_column(t)))
}

def private st048_flag(e : Expression?) : Option<St048Atom> {
    //! A bool subject tested bare - `b` is `b == true`, `!b` is
    //! `b == false`, the spelling STYLE018 asks for.
    let negated = e is ExprOp1 && (e as ExprOp1).op == "!"
    let flag = negated ? (e as ExprOp1).subexpr : e
    let bool_flag = (st048_subject_path(flag) && flag._type != null &&
                     flag._type.baseType == Type.tBool)
    return none(type<St048Atom>) if (!bool_flag)
    let value = negated ? "false" : "true"
    return some(St048Atom(subject = st048_subject_text(flag), kind = St048Kind.eq, text = value,
                          cell = SolverCell(keys <- [value]), column = st048_column(flag._type)))
}

def private st048_variant(e : ExprIsVariant?) : Option<St048Atom> {
    return none(type<St048Atom>) if (!st048_subject_path(e.value))
    let name = string(e.name)
    return some(St048Atom(subject = st048_subject_text(e.value), kind = St048Kind.variant_case,
                          text = name,
                          cell = SolverCell(keys <- [name]), column = st048_column(e.value._type)))
}

def private st048_leaf(e : Expression?) : Option<St048Atom> {
    if (e is ExprRef2Value) return st048_leaf((e as ExprRef2Value).subexpr)
    return st048_variant(e as ExprIsVariant) if (e is ExprIsVariant)
    var flag <- st048_flag(e)
    return <- flag if (is_some(flag))
    return none(type<St048Atom>) if (!(e is ExprOp2))
    let op = string((e as ExprOp2).op)
    var direct <- st048_compare(op, (e as ExprOp2).left, (e as ExprOp2).right)
    return <- direct if (is_some(direct))
    let flipped = op == "<" ? ">" : op == "<=" ? ">=" : op == ">" ? "<" : op == ">=" ? "<=" : op
    return st048_compare(flipped, (e as ExprOp2).right, (e as ExprOp2).left)
}

def private st048_cross(a, b : array<St048Alt>) : array<St048Alt> {
    //! Every AND-group of `a` joined with every AND-group of `b`.
    var res : array<St048Alt>
    res |> reserve(length(a) * length(b))
    for (x in a) {
        for (y in b) {
            var atoms := x.atoms
            atoms |> push_clone_from(y.atoms)
            res |> emplace(St048Alt(atoms <- atoms, guard = x.guard || y.guard))
        }
    }
    return <- res
}

def private st048_dnf(e : Expression?) : array<St048Alt> {
    //! The condition as an OR of AND-groups; empty when it expands past
    //! ST048_MAX_ALTS.
    if (e is ExprRef2Value) return <- st048_dnf((e as ExprRef2Value).subexpr)
    let is_or = e is ExprOp2 && (e as ExprOp2).op == "||"
    if (is_or || (e is ExprOp2 && (e as ExprOp2).op == "&&")) {
        var l <- st048_dnf((e as ExprOp2).left)
        let r <- st048_dnf((e as ExprOp2).right)
        let width = is_or ? length(l) + length(r) : length(l) * length(r)
        if (empty(l) || empty(r) || width > ST048_MAX_ALTS) {
            return <- array<St048Alt>()
        }
        return <- st048_cross(l, r) if (!is_or)
        l |> push_clone_from(r)
        return <- l
    }
    var leaf <- st048_leaf(e)
    return <- [St048Alt(atoms <- [move_unwrap(leaf)])] if (is_some(leaf))
    return <- [St048Alt(guard = true)]
}

def private st048_inner_if(body : Expression?) : ExprIfThenElse? {
    //! The `if` that is the whole of `body`, when it is one.
    var b = body
    return null if (b == null)
    while (b is ExprBlock) {
        let blk = b as ExprBlock
        return null if (length(blk.list) != 1 || !empty(blk.finalList))
        b = blk.list[0]
    }
    return (b as ExprIfThenElse) if (b is ExprIfThenElse &&
                                     !(b as ExprIfThenElse).if_flags.isStatic)
    return null
}

def private st048_body_text(body : Expression?) : string {
    let nothing = body == null || (body is ExprBlock && empty((body as ExprBlock).list))
    return nothing ? "" : describe(body)
}

def private st048_flatten(alts : array<St048Alt>; body : Expression?; node : ExprIfThenElse const?;
                          var seen : table<uint64>) : St048Chain {
    //! The arms of an inner chain that is the whole `body` of an outer
    //! arm, each crossed with the outer `alts`; no arms when it does not
    //! flatten - only an inner chain that leaves nothing to a guard does.
    let inner = st048_inner_if(body)
    return St048Chain() if (inner == null)
    var inscope inner_seen : table<uint64>
    var nested <- st048_collect(inner, inner_seen)
    let guarded = has_value([for (arm in nested.arms);
                             has_value([for (alt in arm.alts); alt.guard], true)], true)
    return St048Chain() if (empty(nested.arms) || guarded)
    for (k in keys(inner_seen)) {
        seen |> insert(k)
    }
    var res = St048Chain(flattened = true)
    res.arms |> reserve(length(nested.arms) + 1)
    for (ia in nested.arms) {
        var arm_alts <- ia.is_else ? clone_to_move(alts) : st048_cross(alts, ia.alts)
        res.arms |> emplace(St048Arm(alts <- arm_alts, body = ia.body, node = ia.node))
    }
    if (!nested.arms[length(nested.arms) - 1].is_else) {
        res.arms |> emplace(St048Arm(alts := alts, synthetic = true, node = node))
    }
    return <- res
}

def private st048_collect(head : ExprIfThenElse?; var seen : table<uint64>) : St048Chain {
    //! The arms of the if/elif chain at `head`, an inner chain that is a
    //! whole arm body flattened in; no arms when a condition expands too
    //! far.
    var chain : St048Chain
    var node = head
    while (node != null) {
        seen |> insert(intptr(node))
        var alts <- st048_dnf(node.cond)
        return <- St048Chain() if (empty(alts))
        var nested <- st048_flatten(alts, node.if_true, node, seen)
        if (empty(nested.arms)) {
            let body = st048_body_text(node.if_true)
            chain.arms |> emplace(St048Arm(alts <- alts, body = body, node = node))
        } else {
            chain.flattened = true
            chain.arms |> push_clone_from(nested.arms)
        }
        let if_false = node.if_false
        var next : ExprIfThenElse?
        if (if_false != null) {
            next = (if_false is ExprIfThenElse ? (if_false as ExprIfThenElse)
                                               : st048_inner_if(if_false))
        }
        if (next == null && if_false != null) {
            let body = st048_body_text(if_false)
            chain.arms |> emplace(St048Arm(is_else = true, body = body, node = node))
        }
        node = next
    }
    return <- chain
}

def private st048_fold(alt : St048Alt) : St048Pattern {
    //! One AND-group as per-subject patterns: a `lo` and a `hi` bound on
    //! one subject intersect into a range; a second test of a subject, or
    //! a one-sided bound, goes to the guard.
    var res = St048Pattern(guard = alt.guard)
    var inscope lo, hi : table<string; string>
    var inscope bounds : table<string; SolverCell>
    for (a in alt.atoms) {
        if (a.kind == St048Kind.lo || a.kind == St048Kind.hi) {
            let lo_bound = a.kind == St048Kind.lo
            let taken = lo_bound ? key_exists(lo, a.subject) : key_exists(hi, a.subject)
            if (taken) {
                res.guard = true
                continue
            }
            if (a.kind == St048Kind.lo) {
                lo |> insert(a.subject, a.text)
            } else {
                hi |> insert(a.subject, a.text)
            }
            if (!key_exists(bounds, a.subject)) {
                bounds |> emplace(a.subject, SolverCell())
            }
            bounds[a.subject].all_of |> push_clone(a.cell)
        } elif (key_exists(res.pats, a.subject)) {
            res.guard = true
        } else {
            let pat = a.kind == St048Kind.variant_case ? "{a.text}(...)" : a.text
            res.pats |> insert(a.subject, pat)
            res.cells |> insert_clone(a.subject, a.cell)
        }
    }
    for (s in keys(bounds)) {
        if (!key_exists(lo, s) || !key_exists(hi, s) || key_exists(res.pats, s)) {
            res.guard = true
            continue
        }
        res.pats |> insert(s, "{lo?[s] ?? ""}..{hi?[s] ?? ""}")
        get(bounds, s) $(cell) {
            res.cells |> insert_clone(s, cell)
        }
    }
    return <- res
}

def private st048_subjects(chain : St048Chain;
                           folded : array<array<St048Pattern>>) : array<string> {
    //! The match header in first-test order: a subject two or more arms
    //! test - any count in a flattened chain; a subject one arm tests
    //! stays in that arm's guard.
    var inscope arms_testing : table<string; int>
    for (pats in folded) {
        var inscope tested : table<string>
        for (p in pats) {
            for (s in keys(p.pats)) {
                tested |> insert(s)
            }
        }
        for (s in keys(tested)) {
            arms_testing[s] = (arms_testing?[s] ?? 0) + 1
        }
    }
    let least = chain.flattened ? 1 : 2
    var subjects : array<string>
    for (arm in chain.arms) {
        for (alt in arm.alts) {
            for (a in alt.atoms) {
                if ((arms_testing?[a.subject] ?? 0) >= least && !has_value(subjects, a.subject)) {
                    subjects |> push(a.subject)
                }
            }
        }
    }
    return <- subjects
}

def private st048_rows(chain : St048Chain; folded : array<array<St048Pattern>>;
                       subjects : array<string>) : array<St048Row> {
    //! Each arm's patterns over the header; an arm is guarded when it
    //! leaves an atom no pattern holds, or tests a subject the header
    //! dropped.
    var rows : array<St048Row>
    rows |> reserve(length(chain.arms))
    for (arm, pats, ai in chain.arms, folded, count()) {
        var guarded = false
        var alts : array<array<SolverCell>>
        alts |> reserve(length(pats) + 1)
        for (p in pats) {
            let dropped = has_value([for (s in keys(p.pats)); has_value(subjects, s)], false)
            guarded ||= p.guard || dropped
            var row : array<SolverCell>
            row |> reserve(length(subjects))
            for (s in subjects) {
                var cell = SolverCell(any = true)
                get(p.cells, s) $(c) {
                    cell := c
                }
                row |> emplace(cell)
            }
            alts |> emplace(row)
        }
        if (arm.is_else) {
            alts |> emplace([for (_s in subjects); SolverCell(any = true)])
        }
        var guard <- guarded ? SolverGuard(opaque) : SolverGuard(unguarded)
        rows |> emplace(St048Row(src = ai,
                                 cells <- [for (p in pats);
                                           [for (s in subjects); p.pats?[s] ?? "_"]],
                                 arm = SolverArm(rows <- alts, guard <- guard)))
    }
    return <- rows
}

def private st048_guard(row : St048Row) : bool => !(row.arm.guard is unguarded)

def private st048_pays(chain : St048Chain; rows : array<St048Row>) : bool {
    //! Three arms that run something, or two when the chain flattens a
    //! nested if or matches variant cases, with guards on at most half; a
    //! guarded arm with alternatives has no match spelling.
    var arms = 0
    var guarded = 0
    var has_variant = false
    for (row in rows) {
        assume arm = chain.arms[row.src]
        continue if (arm.is_else)
        return false if (st048_guard(row) && length(row.cells) > 1)
        arms++ if (!empty(arm.body))
        guarded++ if (st048_guard(row))
        has_variant ||= has_value([for (alt in arm.alts);
                                   has_value([for (a in alt.atoms);
                                              a.kind == St048Kind.variant_case], true)], true)
    }
    return guarded * 2 <= arms && (arms >= 3 || (arms >= 2 && (chain.flattened || has_variant)))
}

def private st048_requires_match(prog : ProgramPtr; mod : Module?) : bool {
    //! STYLE048 suggests ``match`` only to a module that already requires
    //! ``daslib/match`` itself.
    var seen = false
    if (mod != null) {
        program_for_each_module(prog) $(m) {
            seen ||= m.name == "match" && is_visible_directly(mod, m)
        }
    }
    return seen
}

def private st048_coverage(chain : St048Chain; rows : array<St048Row>;
                           subjects : array<string>) : Result<SolverCoverage; SolverUnsupported> {
    //! The solver's answer for the rows; a subject's column - its
    //! domain - comes from the atoms that test it.
    var inscope columns : array<SolverColumn>
    columns |> reserve(length(subjects))
    for (s in subjects) {
        var column : SolverColumn
        for (arm in chain.arms) {
            for (alt in arm.alts) {
                for (a in alt.atoms) {
                    if (a.subject == s && column.domain is none) {
                        column := a.column
                    }
                }
            }
        }
        columns |> emplace(column)
    }
    var inscope arms <- [for (row in rows); clone_to_move(row.arm)]
    return <- solve_coverage(columns, arms)
}

def private st048_clear_between(rows : array<St048Row>; cov : SolverCoverage; i, j : int) : bool {
    //! No arm between `i` and `j` shares a value with `j`, so moving
    //! `j`'s patterns up to `i` changes no value's arm. `arms` are the
    //! solver's arm numbers a row holds.
    for (k in range(i + 1, j)) {
        for (a in rows[k].arms) {
            for (b in rows[j].arms) {
                return false if (arms_overlap(cov, a, b))
            }
        }
    }
    return true
}

def private st048_merge_bodies(chain : St048Chain; var rows : array<St048Row>;
                               cov : SolverCoverage) : void {
    //! An arm whose body repeats an earlier arm's joins it with `|` - the
    //! earliest such arm that no arm in between shares a value with.
    var j = 1
    while (j < length(rows)) {
        var target = -1
        let body = chain.arms[rows[j].src].body
        if (!chain.arms[rows[j].src].is_else && !st048_guard(rows[j]) && !empty(body)) {
            for (i in range(j)) {
                let same = !st048_guard(rows[i]) && chain.arms[rows[i].src].body == body
                if (same && st048_clear_between(rows, cov, i, j)) {
                    target = i
                    break
                }
            }
        }
        if (target < 0) {
            j++
            continue
        }
        rows[target].cells |> push_clone_from(rows[j].cells)
        rows[target].arm.rows |> push_clone_from(rows[j].arm.rows)
        rows[target].arms |> push_from(rows[j].arms)
        rows |> erase(j)
    }
}

def private st048_single_diff(a, b : array<string>) : int {
    //! The one subject where two alternatives differ, both holding a
    //! pattern there; -1 otherwise.
    var slot = -1
    for (x, y, k in a, b, count()) {
        continue if (x == y)
        return -1 if (slot >= 0 || x == "_" || y == "_")
        slot = k
    }
    return slot
}

def private st048_factor(var cells : array<array<string>>) : void {
    //! Two alternatives that differ in one subject fold into one, that
    //! subject's patterns joined with `|`.
    var a = 0
    while (a < length(cells)) {
        var merged = false
        for (b in range(a + 1, length(cells))) {
            let slot = st048_single_diff(cells[a], cells[b])
            if (slot >= 0) {
                cells[a][slot] = "{cells[a][slot]} | {cells[b][slot]}"
                cells |> erase(b)
                merged = true
                break
            }
        }
        a = merged ? 0 : a + 1
    }
}

def private st048_render(chain : St048Chain; rows : array<St048Row>; subjects : array<string>;
                         cov : SolverCoverage; else_dead : bool) : string {
    //! The suggested match, one line: arms separated by ` / `, a
    //! `_ => {}` arm when some value reaches none.
    var arms : array<string>
    arms |> reserve(length(rows) + 1)
    for (row in rows) {
        assume arm = chain.arms[row.src]
        if (arm.is_else) {
            arms |> push("_")
            continue
        }
        var inscope alts <- [for (c in row.cells);
                             length(subjects) > 1 ? "({join(c, ", ")})" : c[0]]
        let guard = st048_guard(row) ? " && <guard>" : ""
        let nothing = empty(arm.body) ? " => \{\}" : ""
        arms |> push("{join(alts, " | ")}{guard}{nothing}")
    }
    let has_else = !empty(rows) && chain.arms[rows[length(rows) - 1].src].is_else
    if (!has_else && !is_exhaustive(cov)) {
        arms |> push("_ => \{\}")
    }
    let listed = !has_else && missing_fully_listed(cov) && cov.all_finite
    let unmatched = listed ? " [unmatched: {(cov.missing as named).text}]" : ""
    let dead_else = else_dead ? " [the else branch never runs]" : ""
    let note = chain.flattened ? " [nested if flattened]" : ""
    return "match ({join(subjects, ", ")}) \{ {join(arms, " / ")} \}{unmatched}{dead_else}{note}"
}

//! Whether ``e`` reads or declares a name spelled like ``v``.
def private fix_reads(var e : Expression?; v : Variable?) : bool =>
    key_exists(lint_name_counts(e), string(v.name))

class StyleLintVisitor : AstVisitor {
    compile_time_errors : bool
    comment_hygiene : bool = false
    ascii_strings : bool = false
    strict_smart_pointers : bool = true
    with_module_is_unsafe : bool = false
    warning_count : int = 0
    collect_warnings : bool = false
    warnings : array<string>
    issues : array<LintIssue>
    reported_locations : table<uint64>
    @do_not_delete current_function : Function?
    @do_not_delete pending_uninit_vars : array<Variable?>
    @do_not_delete skip_userSaidItsSafe : array<Expression?>
    @do_not_delete unsafeExprs : table<Expression?; UnsafeFrame>
    unsafe_stack : array<UnsafeFrame>
    unsafe_block_stack : array<int>
    complexity_stack : array<int>
    max_complexity : int = STYLE037_DEFAULT_MAX
    max_function_length : int = STYLE038_DEFAULT_MAX_LINES
    analyze_requires : bool = false
    @do_not_delete this_module : Module?
    used_modules : table<string>
    unresolved_call_names : table<string>
    @do_not_delete st041_candidates : table<Variable?; St041Candidate>
    st048_seen : table<uint64>
    st048_sees_match : bool
    st047_sites : array<St047Site>

    //! STYLE047 - a block literal comparing two elements key by key.
    def st047_check_comparator(var expr : ExprCall?) : void {
        if (expr.genFlags.generated || current_function == null
                || root_generic(current_function)._module != this_module) return
        var written = length(expr.arguments) - 1
        while (written >= 0 && (expr.arguments[written] is ExprFakeContext
                || expr.arguments[written] is ExprFakeLineInfo)) {
            written--
        }
        if (written < 0 || !(expr.arguments[written] is ExprMakeBlock)) return
        var mb = expr.arguments[written] as ExprMakeBlock
        var blk = mb._block as ExprBlock
        if (mb.at.fileInfo != current_function.at.fileInfo || length(blk.arguments) != 2
                || blk.returnType.baseType != Type.tBool) return
        let keys <- st047_body_keys(blk, blk.arguments[0], blk.arguments[1])
        if (empty(keys)) return
        let args <- [for (k in keys); !k.greater ? k.text
            : starts_with(k.text, "-") || find(k.text, " ") >= 0 ? "-({k.text})" : "-{k.text}"]
        let text = "cmp_by({join(args, ", ")})"
        let identity = length(keys) == 1 && keys[0].text == "_" && !keys[0].greater
        let message = (identity ? "STYLE047: this comparator is the default '<' - drop it"
            : "STYLE047: this comparator compares keys one by one; "
            + "write it as '{text}' (daslib/cmp)")
        var site = St047Site(fn = current_function, node = mb, message = message)
        let enc = expr.atEnclosure
        let after = enc.fileInfo == mb.at.fileInfo && (mb.at.line > enc.last_line
            || (mb.at.line == enc.last_line && mb.at.column >= enc.last_column))
        let first = blk.arguments[0].at
        let from = (after ? int2(int(enc.last_line), int(enc.last_column))
            : int2(int(first.line), max(int(first.column) - 2, 0)))
        let to = int2(int(mb.at.last_line), int(mb.at.last_column))
        let span = st047_span_text(mb.at, from, to)
        let head = strip_left(span)
        let opens = starts_with(head, "$(") || (after && starts_with(head, "<|"))
        let commented = code_find(span, "//") >= 0 || code_find(span, "/*") >= 0
        if (opens && (after || !identity) && !commented) {
            site.edits |> push(LintFix(line = from.x, column = from.y,
                                       last_line = to.x, last_column = to.y,
                                       replacement = identity ? "" : after ? " <| {text}" : text))
        }
        st047_sites |> emplace(site)
    }

    //! STYLE047 - an ``operator <`` over two values of a structure
    //! that compares its fields key by key.
    def st047_check_operator(var fn : FunctionPtr) : void {
        if (fn.name != "<" || fn.flags.generated || fn.fromGeneric != null
                || fn._module != this_module || length(fn.arguments) != 2) return
        let st = fn.arguments[0]._type.structType
        if (st == null || st.flags.isClass || fn.arguments[1]._type.structType != st
                || fn.result.baseType != Type.tBool) return
        let keys <- st047_body_keys(fn.body, fn.arguments[0], fn.arguments[1])
        if (empty(keys)) return
        var args : array<string>
        var in_order = length(keys) == length(st.fields)
        for (k, i in keys, count()) {
            let name = slice(k.text, 2)
            if (starts_with(k.text, "_.") && find_structure_field(st, name) != null) {
                in_order &&= !k.greater && st.fields[i].name == name
                args |> push(k.greater ? "{name} = greater" : name)
            } elif (!k.greater && ends_with(k.text, "(_)")
                    && find(k.text, "(") == length(k.text) - 3) {
                in_order = false
                args |> push("key = {slice(k.text, 0, -3)}")
            } else {
                return
            }
        }
        let annotation = in_order ? "[ordered]" : "[ordered({join(args, ", ")})]"
        let message = ("STYLE047: operator < on '{st.name}' compares its fields one by one; "
            + "annotate the structure '{annotation}' instead (daslib/cmp)")
        var site = St047Site(fn = fn, message = message)
        let decl = fn.atDecl
        let first = int(decl.line)
        let last_at = int2(int(decl.last_line), int(decl.last_column))
        let code = st047_span_text(fn.at, int2(first, 0), last_at)
        let tail = slice(lint_source_line(fn.at, last_at.x), last_at.y)
        let commented = code_find(code, "//") >= 0 || code_find(code, "/*") >= 0
        if (st.at.fileInfo == fn.at.fileInfo && !commented && empty(strip(tail))) {
            let st_line = int(st.at.line)
            let src = lint_source_line(st.at, st_line)
            let indent = slice(src, 0, length(src) - length(strip_left(src)))
            var last = int(decl.last_line) + 1
            let blank_before = empty(strip(lint_source_line(fn.at, first - 1)))
            if (blank_before && empty(strip(lint_source_line(fn.at, last)))) {
                last++
            }
            site.edits <- [
                LintFix(line = st_line, last_line = st_line,
                        replacement = "{indent}{annotation}\n"),
                LintFix(line = first, last_line = last)
            ]
        }
        st047_sites |> emplace(site)
    }

    //! STYLE047 - reports the findings; the first fix of a module that
    //! does not see ``daslib/cmp`` also adds its ``require``.
    def st047_report(prog : ProgramPtr) : void {
        if (empty(st047_sites)) return
        sort(st047_sites) <| cmp_by(int(_.node != null ? _.node.at.line : _.fn.at.line))
        var need_require = this_module != null
        program_for_each_module(prog) $(m) {
            need_require &&= !(m.name == "cmp" && is_visible_directly(this_module, m))
        }
        var saved = current_function
        for (site in st047_sites) {
            current_function = site.fn
            let at = site.node != null ? site.node.at : site.fn.at
            var inscope edits := site.edits
            if (!empty(edits) && need_require) {
                var ln = 1
                for (i in range(1, int(at.line))) {
                    let text = lint_source_line(at, i)
                    if (starts_with(text, "require ")) {
                        ln = i
                        break
                    }
                    if (starts_with(text, "options ") || starts_with(text, "module ")
                            || starts_with(text, "expect ")) {
                        ln = i + 1
                    }
                }
                edits |> push(LintFix(line = ln, last_line = ln,
                                      replacement = "require daslib/cmp\n"))
                need_require = false
            }
            var fix = LintFix()
            if (!empty(edits)) {
                sort(edits) <| cmp_by(_.line, _.column)
                let text = build_string() $(var w) {
                    for (e, i in edits, count()) {
                        if (i > 0) {
                            let prev = edits[i - 1]
                            let gap = st047_span_text(at, int2(prev.last_line, prev.last_column),
                                                      int2(e.line, e.column))
                            w |> write(gap)
                        }
                        w |> write(e.replacement)
                    }
                }
                let last = edits |> back()
                fix = LintFix(line = edits[0].line, column = edits[0].column,
                              last_line = last.last_line, last_column = last.last_column,
                              replacement = text)
            }
            style_warning(site.message, at, fix)
        }
        current_function = saved
    }

    def report_key(at : LineInfo; code : string) : uint64 {
        //! fileInfo is part of the key, where line|column alone collides across files.
        let pos = uint64(at.line) | (uint64(at.column) << uint64(20))
        return (pos ^ (intptr(at.fileInfo) * 1099511628211ul)) ^ hash(code)
    }

    disabled_codes : table<string>
    enabled_codes : table<string>

    def is_suppressed(text : string; at : LineInfo) : bool {
        let code = extract_lint_code(text)
        if (!empty(code)) {
            return true if (key_exists(disabled_codes, code)
                            || (!empty(enabled_codes) && !key_exists(enabled_codes, code)))
        }
        return is_lint_suppressed(at, code)
    }

    def style_warning(text : string; at : LineInfo; fix : LintFix = LintFix()) : void {
        if (!is_user_authored_body(current_function) || is_suppressed(text, at)) return
        let key = report_key(at, extract_lint_code(text))
        if (key_exists(reported_locations, key)) return
        reported_locations |> insert(key)
        warning_count++
        var msg = text
        if (current_function != null && current_function.fromGeneric != null && !empty(current_function.inferStack)) {
            msg = build_string() $(var w) {
                w |> write(text)
                w |> write("\n  while compiling {current_function.name}")
                for (ih in current_function.inferStack) {
                    w |> write("\n  instanced from {ih.func.name} at {describe(ih.at)}")
                }
            }
        }
        if (compile_time_errors) {
            compiling_program() |> macro_style_warning(at, lint_fix_message(msg, fix))
        } elif (collect_warnings) {
            warnings |> push("style warning: {lint_fix_message(msg, fix)} at {describe(at)}")
            var issue = make_lint_issue(msg, at)
            issue.fix = fix
            issues |> push(issue)
        } else {
            to_log(LOG_WARNING, "style warning: {lint_fix_message(msg, fix)} at {describe(at)}\n")
        }
    }

    def source_line_between(at_start : LineInfo; at_end : LineInfo) : string {
        //! Reads the source text between two line infos (potentially multi-line).
        var result = ""
        if (at_start.fileInfo == null) return result
        for (ln in range(int(at_start.line), int(at_end.line) + 1)) {
            get_file_source_line(at_start.fileInfo, uint(ln)) $(line) {
                result = "{result}{line}\n"
            }
        }
        return result
    }

    //! The source of ``e``, a value wrapper peeled, as an operand of a
    //! tighter-binding operator; "" for a generated node or a comment.
    def fix_operand(e : Expression? const) : string {
        let text = lint_operand_text(peel_ref2value_const(e))
        return lint_has_comment(text) ? "" : text
    }

    //! Whether ``e`` reads unparenthesized under ``parent``: an
    //! initializer, a returned value, a statement, the value of an
    //! assignment, or a call argument.
    def fix_isolated(e : Expression? const; parent : Expression? const) : bool {
        if (parent == null) return false
        if (parent is ExprLet || parent is ExprReturn || parent is ExprBlock) return true
        if (parent is ExprCopy) return (parent as ExprCopy).right == e
        if (parent is ExprMove) return (parent as ExprMove).right == e
        if (parent is ExprClone) return (parent as ExprClone).right == e
        return parent is ExprCall && (e.at.line != parent.at.line
                                      || e.at.column != parent.at.column)
    }

    //! The node ``depth`` levels above the innermost one being visited.
    def fix_parent(depth : int) : Expression? {
        let n = length(unsafe_stack)
        return n > depth ? unsafe_stack[n - 1 - depth].node : null
    }

    //! The operator of an assignment statement, read from its node, and
    //! its value; an empty operator when a copy may be a lowered clone.
    def fix_assignment(stmt : Expression? const) : tuple<op : string; value : Expression?> {
        if (stmt is ExprMove) return (op = "<-", value = (stmt as ExprMove).right)
        if (stmt is ExprClone) return (op = ":=", value = (stmt as ExprClone).right)
        if (stmt is ExprCopy && !(stmt as ExprCopy).copy_flags.allowCopyTemp) return (op = "=",
            value = (stmt as ExprCopy).right)
        return (op = "", value = null)
    }

    //! Whether statements ``first .. first + count`` of ``blk`` each fill
    //! one source line, on consecutive lines.
    def fix_run_ok(blk : ExprBlock?; first, count : int) : bool {
        for (k in range(first, first + count)) {
            if (!lint_fills_lines(blk.list[k], true)
                || (k > first
                    && blk.list[k].at.line != blk.list[k - 1].at.last_line + 1u)) return false
        }
        return true
    }

    //! Whether ``e`` is a ``let`` that declares ``v`` and nothing else.
    def fix_declares_only(e : Expression?; v : Variable?) : bool =>
        e is ExprLet && length((e as ExprLet).variables) == 1 && (e as ExprLet).variables[0] == v

    //! Drops the ``<|`` before block ``mk``, and for a parameterless
    //! block the ``$()`` or ``$`` it leaves redundant.
    def st001_pipe_fix(mk : ExprMakeBlock?; eblk : ExprBlock?) : LintFix {
        if (mk.at.column < 3u) return LintFix()
        let to = empty(eblk.arguments) ? eblk.at : mk.at
        var at = mk.at
        at.column = mk.at.column - 3u
        at.last_line = to.line
        at.last_column = to.column
        let text = lint_span_text(at)
        return text == "<| " || text == "<| $() " || text == "<| $ " ? lint_fix(at, "") : LintFix()
    }

    def st005_fix(ifte : ExprIfThenElse?; blk : ExprBlock?; term : Expression? const) : LintFix =>
        blk.at.line == ifte.at.line ? lint_replace_fix(blk, lint_expr_text(term)) : LintFix()

    def st006_fix(expr : ExprOp2?) : LintFix {
        let left_is_call = is_string_rtti_comparison(expr.left)
        let call = peel_ref2value_const(left_is_call ? expr.left : expr.right)
        let named = peel_ref2value_const(left_is_call ? expr.right : expr.left)
        if (!lint_fix_site_ok(expr, current_function, false) || !(named is ExprConstString)
            || !lint_module_visible(current_function, "ast_boost")) return LintFix()
        let name = string((named as ExprConstString).value)
        if (find_annotation("ast_core", name) == null
            || lint_expr_text(named) != "\"{name}\"") return LintFix()
        let field = peel_ref2value_const((call as ExprCall).arguments[0]) as ExprField
        let subject = fix_operand(field.value)
        return empty(subject) ? LintFix() : lint_replace_fix(expr, "{subject} is {name}")
    }

    //! ``if (true) B`` becomes ``B``.
    def st010_fix(ifte : ExprIfThenElse?) : LintFix {
        if (!(ifte.if_true is ExprBlock) || lint_expr_text(ifte.cond) != "true") return LintFix()
        let blk = ifte.if_true as ExprBlock
        return lint_replace_fix(ifte, blk.genFlags.generated && length(blk.list) == 1
                ? lint_expr_text(blk.list[0]) : lint_expr_text(blk))
    }

    //! Folds assignment ``assign`` into the declaration of ``v`` on the
    //! line before it.
    def st011_fix(blk : ExprBlock?; assign : Expression?; v : Variable?) : LintFix {
        for (k in range(1, length(blk.list))) {
            continue if (blk.list[k] != assign)
            let decl = blk.list[k - 1]
            var a = fix_assignment(assign)
            if (!fix_run_ok(blk, k - 1, 2) || !fix_declares_only(decl, v) || v.init != null
                || a.value == null || fix_reads(a.value, v)) return LintFix()
            let value = lint_expr_text(a.value)
            return empty(value) ? LintFix() : lint_fix(lint_range(decl.at, assign.at),
                                                       "{lint_expr_text(decl)} {a.op} {value}")
        }
        return LintFix()
    }

    //! Folds a run of ``push`` / ``emplace`` lines into the array's
    //! declaration as a literal.
    def st012_fix(var blk : ExprBlock?; idx, count : int; v : Variable?) : LintFix {
        if (!fix_run_ok(blk, idx, count + 1) || !fix_declares_only(blk.list[idx], v)
            || v.init != null || v._type.firstType == null) return LintFix()
        var items : array<string>
        items |> reserve(count)
        for (k in range(count)) {
            var call = blk.list[idx + 1 + k] as ExprCall
            let arg = peel_ref2value_const(call.arguments[1])
            let text = lint_expr_text(arg)
            let is_emplace = call.func.fromGeneric.name == "emplace"
            if (empty(text) || fix_reads(call.arguments[1], v) || (is_emplace && !(arg is ExprVar))
                    || !is_same_type(v._type.firstType, arg._type, RefMatters.no, ConstMatters.no,
                                     TemporaryMatters.no)) return LintFix()
            items |> push(is_emplace ? "<-{text}" : text)
        }
        return lint_fix(lint_range(blk.list[idx].at, blk.list[idx + count].at),
                        "{lint_expr_text(blk.list[idx])} <- [{join(items, ", ")}]")
    }

    //! Folds a run of field assignments into the ``var a = T()`` / ``var
    //! a = new T()`` declaration as a named-field constructor.
    def st013_fix(blk : ExprBlock?; idx, count : int; v : Variable?) : LintFix {
        let is_new = v.init != null && v.init is ExprNew
        let st = is_new ? v._type.firstType.structType : v._type.structType
        if (!fix_run_ok(blk, idx, count + 1) || !fix_declares_only(blk.list[idx], v) || st == null
            || st.flags.isClass || !(is_new || (v.init != null && v.init is ExprMakeStruct))
                || is_new && (v.init as ExprNew).func != null
                && !(v.init as ExprNew).func.flags.generated) return LintFix()
        let init = lint_expr_text(v.init)
        if (!ends_with(init, "()")) return LintFix()
        var fields : array<string>
        fields |> reserve(count)
        var seen : table<string>
        for (k in range(count)) {
            let stmt = blk.list[idx + 1 + k]
            let field = field_name_of(stmt)
            var a = fix_assignment(stmt)
            let value = lint_expr_text(a.value)
            if (empty(value) || key_exists(seen, field) || fix_reads(a.value, v)) return LintFix()
            seen |> insert(field)
            fields |> push("{field} {a.op} {value}")
        }
        let assign = is_new || v._type.canCopy ? "=" : "<-"
        let head = lint_span_text(lint_range(blk.list[idx].at, v.at))
        return lint_fix(lint_range(blk.list[idx].at, blk.list[idx + count].at),
                        "{head} {assign} {slice(init, 0, length(init) - 2)}({join(fields, ", ")})")
    }

    def st016_or_operand(cond : Expression?) : string {
        let text = lint_expr_text(cond)
        if (lint_has_comment(text)) return ""
        return peel_ref2value_const(cond) is ExprOp3 ? "({text})" : text
    }

    //! Two adjacent guards with one exit become the first guard over ``a
    //! || b``.
    def st016_fix(first, second : ExprIfThenElse?) : LintFix {
        let a = st016_or_operand(first.cond)
        let b = st016_or_operand(second.cond)
        if (empty(a) || empty(b) || first.genFlags.generated
            || second.genFlags.generated) return LintFix()
        var dropped = second.at
        dropped.line = first.at.last_line
        dropped.column = first.at.last_column
        if (lint_has_comment(lint_span_text(dropped))) return LintFix()
        return lint_merge_fixes(first.at,
                                [lint_fix(first.cond.at, "{a} || {b}"), lint_fix(dropped, "")])
    }

    //! ``if (c) return b1`` closed by ``return !b1`` (an else arm or the
    //! next statement ``last``) becomes ``return c`` / ``return !c``.
    def st017_fix(ifte : ExprIfThenElse?; last : Expression?; b1 : bool) : LintFix {
        let cond = b1 ? lint_expr_text(ifte.cond) : fix_operand(ifte.cond)
        if (ifte.genFlags.generated || last.genFlags.generated || empty(cond)) return LintFix()
        let at = lint_range(ifte.at, last == ifte.if_false ? ifte.at : last.at)
        return lint_span_fix(at, b1 ? "return {cond}" : "return !{cond}")
    }

    def st018_fix(expr : ExprOp2?; operand, literal : Expression? const;
                  positive : bool) : LintFix {
        let lit = lint_expr_text(literal)
        let text = fix_operand(operand)
        if (expr.genFlags.generated || (lit != "true" && lit != "false")
            || empty(text)) return LintFix()
        return lint_replace_fix(expr, positive ? text : "!{text}")
    }

    def fix_number(e0 : Expression? const; var value : double&) : bool {
        let e = peel_ref2value_const(e0)
        if (e is ExprOp1 && (e as ExprOp1).op == "-") {
            let ok = fix_number((e as ExprOp1).subexpr, value)
            value = -value
            return ok
        }
        if (e is ExprConstInt) {
            value = double((e as ExprConstInt).value)
        } elif (e is ExprConstUInt) {
            value = double((e as ExprConstUInt).value)
        } elif (e is ExprConstInt64) {
            value = double((e as ExprConstInt64).value)
        } elif (e is ExprConstUInt64) {
            value = double((e as ExprConstUInt64).value)
        } elif (e is ExprConstFloat) {
            value = double((e as ExprConstFloat).value)
        } elif (e is ExprConstDouble) {
            value = (e as ExprConstDouble).value
        } else {
            return false
        }
        return true
    }

    //! ``min(max(x, lo), hi)``, constant ``lo <= hi``, -> ``clamp``;
    //! ints also take the commuted and ``max(min(...))`` spellings,
    //! floats only the one matching ``clamp``'s NaN and -0 behaviour.
    def st019_fix(expr : ExprCall?) : LintFix {
        let bt = expr._type != null ? expr._type.baseType : Type.none
        let is_float = bt == Type.tFloat || bt == Type.tDouble
        if (!lint_fix_site_ok(expr, current_function, false)
            || !(is_float || bt == Type.tInt || bt == Type.tUInt || bt == Type.tInt64
                 || bt == Type.tUInt64)) return LintFix()
        let outer_min = expr.func.name == "min"
        let inner_first = is_math_call(expr.arguments[0], outer_min ? "max" : "min")
        let inner = peel_ref2value_const(expr.arguments[inner_first ? 0 : 1]) as ExprCall
        let bound = expr.arguments[inner_first ? 1 : 0]
        var bound_value = 0.0lf
        var limit_value = 0.0lf
        let x_first = !fix_number(inner.arguments[0], limit_value)
        if (x_first && !fix_number(inner.arguments[1], limit_value)
            || !fix_number(bound, bound_value)
            || (is_float && (!outer_min || !inner_first || !x_first))) return LintFix()
        let lo = outer_min ? limit_value : bound_value
        let hi = outer_min ? bound_value : limit_value
        if (is_float ? lo >= hi : lo > hi) return LintFix()
        let x = lint_expr_text(inner.arguments[x_first ? 0 : 1])
        let limit = lint_expr_text(inner.arguments[x_first ? 1 : 0])
        let bound_text = lint_expr_text(bound)
        if (empty(x) || empty(limit) || empty(bound_text)) return LintFix()
        return lint_replace_fix(expr, outer_min ? "clamp({x}, {limit}, " +
            "{bound_text})" : "clamp({x}, {bound_text}, {limit})")
    }

    def st020_fix(expr : ExprCall?) : LintFix {
        let n = length(expr.arguments)
        if (!lint_fix_site_ok(expr, current_function, false) || n < 2
            || expr.arguments[n - 1]._type == null
                || expr.arguments[n - 1].at.fileInfo != expr.at.fileInfo
                || expr.arguments[n - 1]._type.baseType != expr._type.baseType) return LintFix()
        let value = fix_operand(expr.arguments[0])
        var number = 0.0lf
        let fallback = fix_number(expr.arguments[n - 1],
            number) ? lint_expr_text(expr.arguments[n - 1]) : fix_operand(expr.arguments[n - 1])
        if (empty(value) || empty(fallback)) return LintFix()
        return lint_replace_fix(expr, fix_isolated(expr, fix_parent(1)) ? "{value} ?? " +
            "{fallback}" : "({value} ?? {fallback})")
    }

    def st022_fix(expr : ExprOp2?; name : string) : LintFix {
        let target = fix_operand(expr.left)
        let rhs = peel_ref2value_const(expr.right)
        let bit = rhs is ExprConstBitfield || (rhs is ExprOp1 && (rhs as ExprOp1).op == "~"
                && peel_ref2value_const((rhs as ExprOp1).subexpr) is ExprConstBitfield)
        if (expr.genFlags.generated || empty(target) || !bit) return LintFix()
        return lint_replace_fix(expr, "{target}.{name} = {expr.op == "|=" ? "true" : "false"}")
    }

    def st023_fix(expr : ExprOp2?; op2 : ExprOp2 const?; name : string) : LintFix {
        let target = fix_operand(op2.left)
        if (expr.genFlags.generated || empty(target)
            || !(peel_ref2value_const(op2.right) is ExprConstBitfield)) return LintFix()
        return lint_replace_fix(expr, expr.op == "!=" ? "{target}.{name}" : "!{target}.{name}")
    }

    def st042_written_body(fn : FunctionPtr) : ExprBlock? {
        if (fn.flags.generated || fn.body == null || !(fn.body is ExprBlock) || foreign_module_code()) return null
        var body = fn.body as ExprBlock
        if (fn.flags.isClassMethod && length(body.list) == 1 && body.list[0] is ExprWith) {
            let wbody = (body.list[0] as ExprWith).body
            return null if (wbody == null || !(wbody is ExprBlock))
            body = wbody as ExprBlock
        }
        return body
    }

    def st042_fix(body : ExprBlock?) : LintFix {
        let first_ln = int(body.at.line)
        let last_ln = int(body.at.last_line)
        let first = lint_source_line(body.at, first_ln)
        let last = lint_source_line(body.at, last_ln)
        let open_col = int(body.at.column)
        var close_col = int(body.at.last_column) - 1
        if (byte_at(last, close_col) != '}') {
            close_col++
        }
        if (byte_at(first, open_col) != '{' || byte_at(last, close_col) != '}') return LintFix()
        let middle = last_ln - first_ln == 2 ? lint_source_line(body.at, first_ln + 1) : ""
        let pieces <- (first_ln == last_ln ? [slice(first, open_col + 1, close_col)]
            : [slice(first, open_col + 1), middle, slice(last, 0, close_col)])
        var inner, comment : string
        for (piece in pieces) {
            let at = code_find(piece, "//")
            let code = strip(at >= 0 ? slice(piece, 0, at) : piece)
            if ((at >= 0 && !empty(comment)) || (!empty(code) && !empty(inner))) return LintFix()
            if (at >= 0) {
                comment = strip(slice(piece, at))
            }
            if (!empty(code)) {
                inner = code
            }
        }
        if (!starts_with(inner, "return ")) return LintFix()
        var expr = strip(slice(inner, 7))
        if (ends_with(expr, ";")) {
            expr = strip(slice(expr, 0, length(expr) - 1))
        }
        let tail_text = slice(last, close_col + 1)
        if (starts_with(expr, "\{") || starts_with(expr, "<- \{") || code_find(expr, "..") >= 0) {
            return LintFix(line = -1)
        }
        if (empty(expr) || code_find(expr, "/*") >= 0 || (!empty(comment) && !empty(strip(tail_text)))) return LintFix()
        if (!empty(comment)) {
            expr = "{expr}  {comment}"
        }
        let tail = length(tail_text)
        let indent = length(first) - length(strip_left(first))
        let one_line = open_col + 3 + length(expr) + tail <= STYLE042_MAX_LINE
        if (!one_line && code_find(pieces[0], "//") >= 0) return LintFix(line = -1)
        let lead = byte_at(first, open_col - 1) == ' ' ? "" : " "
        let replacement = one_line ? "{lead}=> {expr}" : "{lead}=>\n{repeat(" ", indent + 4)}{expr}"
        return LintFix(line = first_ln, column = open_col, last_line = last_ln, last_column = close_col + 1, replacement = replacement)
    }

    //! ``unsafe(E)`` -> ``E``, kept in parentheses unless ``E`` is an
    //! operand or stands alone under ``parent``.
    def st024_unwrap_expr_fix(expr : Expression?; parent : Expression? const) : LintFix {
        if (expr.genFlags.generated || lint_has_comment(lint_expr_text(expr))) return LintFix()
        let ln = int(expr.at.line)
        let col = int(expr.at.column) - 7
        let head = lint_token_fix(expr.at, ln, col, "unsafe(", "")
        let tail = lint_token_fix(expr.at, int(expr.at.last_line), int(expr.at.last_column), ")",
                                  "")
        if (head.line == 0 || tail.line == 0) return LintFix()
        if (!lint_is_operand(peel_ref2value_const(expr))
            && !fix_isolated(expr, parent)) return lint_token_fix(expr.at, ln, col, "unsafe", "")
        return lint_merge_fixes(expr.at, [head, tail])
    }

    //! Edits dropping the braces of ``unsafe { ... }`` and dedenting
    //! its body; none when the wrap shares lines with other code, a
    //! statement spans lines, or a name it declares is used outside.
    def st024_unwrap_edits(var u : ExprUnsafe?) : array<LintFix> {
        var edits : array<LintFix>
        if (current_function == null || current_function.body == null || u.body == null
            || !(u.body is ExprBlock)) return <- edits
        var blk = u.body as ExprBlock
        if (!lint_fills_lines(u, false) || empty(blk.list) || !empty(blk.finalList)) return <- edits
        let first = blk.list[0]
        let last = blk.list |> back()
        var head = u.at
        head.last_line = first.at.line
        head.last_column = first.at.column
        var tail = u.at
        tail.line = last.at.last_line
        tail.column = last.at.last_column
        if (first.genFlags.generated || last.genFlags.generated
            || strip(lint_span_text(head)) != "unsafe \{"
            || strip(lint_span_text(tail)) != "\}") return <- edits
        let fn_names <- lint_name_counts(current_function.body)
        let blk_names <- lint_name_counts(blk)
        for (st in blk.list) {
            return <- edits if (st.at.line != st.at.last_line)
            continue if (!(st is ExprLet))
            for (v in (st as ExprLet).variables) {
                return <- edits if (v.flags.inScope
                        || fn_names?[string(v.name)] ?? 0 != blk_names?[string(v.name)] ?? 0)
            }
        }
        let top = int(u.at.line)
        let bottom = int(u.at.last_line)
        let col = int(u.at.column)
        if (top == bottom) {
            edits |> push(lint_fix(head, ""))
            edits |> push(lint_fix(tail, ""))
            return <- edits
        }
        if (int(first.at.line) != top + 1 || int(last.at.last_line) != bottom - 1) return <- edits
        edits |> push(
                      LintFix(line = top, column = 0, last_line = top + 1, last_column = 0,
                              replacement = ""))
        for (ln in range(int(first.at.line), int(last.at.last_line) + 1)) {
            let line = lint_source_line(u.at, ln)
            continue if (empty(strip(line)))
            if (lint_indent_of(line) < col + 4) {
                edits |> clear()
                return <- edits
            }
            edits |> push(
                          LintFix(line = ln, column = col, last_line = ln, last_column = col + 4,
                                  replacement = ""))
        }
        edits |> push(
                      LintFix(line = bottom, column = 0, last_line = bottom + 1, last_column = 0,
                              replacement = ""))
        return <- edits
    }

    def st024_unwrap_block_fix(var u : ExprUnsafe?) : LintFix =>
        lint_merge_fixes(u.at, st024_unwrap_edits(u))

    //! Drops the braces of ``unsafe { ... }`` and wraps ``op`` - the one
    //! operation in it that needs unsafe - in ``unsafe(...)``.
    def st025_fix(var u : ExprUnsafe?; op : Expression?) : LintFix {
        if (op == null
            || !(op is ExprRef2Ptr || op is ExprAddr || op is ExprCall || op is ExprCast
                 || op is ExprAt || op is ExprSafeAt
                || op is ExprAsVariant || op is ExprSafeAsVariant || op is ExprField
                || op is ExprNew
                || op is ExprOp1 || op is ExprOp2 || op is ExprOp3)) return LintFix()
        var edits <- st024_unwrap_edits(u)
        let wrap = lint_replace_fix(op, "unsafe({lint_expr_text(op)})")
        if (empty(edits) || wrap.line == 0) return LintFix()
        edits |> push(wrap)
        return lint_merge_fixes(u.at, edits)
    }

    //! ``reinterpret<T?>(unsafe(addr(x)))`` -> ``addr<T?>(x)``.
    def st034_fix(expr : ExprCast?) : LintFix {
        if (expr.genFlags.generated || expr.subexpr.genFlags.generated) return LintFix()
        var head_at = expr.at
        head_at.last_line = expr.subexpr.at.line
        head_at.last_column = expr.subexpr.at.column
        let head = lint_span_text(head_at)
        let wrapped = ends_with(head, ">(unsafe(")
        if (!starts_with(head, "reinterpret<")
            || !(wrapped || ends_with(head, ">("))) return LintFix()
        let target = lint_expr_text((expr.subexpr as ExprRef2Ptr).subexpr)
        return lint_replace_fix(expr,
                                "addr{slice(head, 11, length(head) - (wrapped ? 8 : 1))}({target})")
    }

    //! Drops the one spelling of the inert contract ``inert`` from the
    //! cast's target type.
    def st036_fix(expr : ExprCast?; inert : string) : LintFix {
        if (expr.genFlags.generated || expr.subexpr.genFlags.generated
            || expr.subexpr.at.line != expr.at.line) return LintFix()
        let ln = int(expr.at.line)
        let col = int(expr.at.column)
        let head = slice(lint_source_line(expr.at, ln), col, int(expr.subexpr.at.column))
        let at = find(head, " {inert}")
        if (at < 0 || find(head, " {inert}", at + 1) >= 0) return LintFix()
        return LintFix(line = ln, column = col + at, last_line = ln,
                       last_column = col + at + length(inert) + 1, replacement = "")
    }

    def st_same_type(a, b : TypeDecl?) : bool =>
        a != null && b != null && b.canCopy && is_same_type(a, b, RefMatters.no, ConstMatters.no,
                                                            TemporaryMatters.no)

    //! ``K => V`` - or ``K`` for a push or a set insert - of an
    //! ``insert`` / ``push`` call or a ``c[K] = V`` copy into container
    //! ``v``; "" for another shape.
    def st_item_text(var stmt : Expression?; v : Variable?) : string {
        let set_like = v._type.secondType == null || v._type.secondType.baseType == Type.tVoid
        if (stmt is ExprCall) {
            var call = stmt as ExprCall
            let fname = call.func.fromGeneric != null ? string(call.func.fromGeneric.name) : ""
            let n = length(call.arguments)
            return "" if ((fname != "push" && fname != "insert") || set_like != (n == 2))
            return st_item_of(call.arguments[1], n == 3 ? call.arguments[2] : null, v)
        }
        if (!set_like && stmt is ExprCopy && (stmt as ExprCopy).left is ExprAt) {
            var copy = stmt as ExprCopy
            return st_item_of((copy.left as ExprAt).index, copy.right, v)
        }
        return ""
    }

    //! ``key => value`` - or ``key`` when ``value`` is null - when both
    //! match the types of container ``v`` and neither reads ``v``; ""
    //! otherwise.
    def st_item_of(var key, value : Expression?; v : Variable?) : string {
        let key_text = lint_expr_text(key)
        if (empty(key_text) || !st_same_type(key._type, v._type.firstType)
            || fix_reads(key, v)) return ""
        if (value == null) return key_text
        let value_text = lint_expr_text(value)
        return empty(value_text) || !st_same_type(value._type,
            v._type.secondType) || fix_reads(value, v) ? "" : "{key_text} => {value_text}"
    }

    //! How a file requiring ``mod`` as ``req_name`` spells a require
    //! of ``target``: a built-in module's name, or the directory of
    //! ``req_name`` plus the stem of a module beside ``mod``; "" else.
    def st029_require_spelling(req_name : string implicit; mod, target : Module?) : string {
        if (target == null || mod == null) return ""
        let tfile = string(target.fileName)
        if (empty(tfile)) return string(target.name)
        let mfile = string(mod.fileName)
        if (empty(mfile) || dir_name(tfile) != dir_name(mfile)
            || stem(mfile) != base_name(req_name)) return ""
        return "{slice(req_name, 0, length(req_name) - length(base_name(req_name)))}{stem(tfile)}"
    }

    //! ``require X`` alone on its line, ``X`` respelled as ``spelling``.
    def st029_fix(at : LineInfo; req_name : string implicit; spelling : string) : LintFix {
        if (empty(spelling)
            || strip(lint_source_line(at, int(at.line))) != "require {req_name}") return LintFix()
        return lint_token_fix(at, int(at.line), int(at.column), req_name, spelling)
    }

    //! Deletes the line of an unused ``require req_name`` - or of a
    //! ``require [group]`` when ``req_name`` is empty - when nothing else
    //! shares it.
    def st030_fix(at : LineInfo; req_name : string implicit) : LintFix {
        let ln = int(at.line)
        let line = strip(lint_source_line(at, ln))
        let whole = (empty(req_name) ? starts_with(line, "require [") && ends_with(line, "]")
                     && !lint_has_comment(line)
            : line == "require {req_name}")
        return whole ? LintFix(line = ln, column = 0, last_line = ln + 1, last_column = 0,
                               replacement = "") : LintFix()
    }

    //! A one-line string literal span with every non-ASCII byte spelled
    //! as a ``\xNN`` escape - the same value in ASCII source.
    def st039_fix(at : LineInfo) : LintFix {
        let ln = int(at.line)
        let line = lint_source_line(at, ln)
        let from = int(at.column)
        let to = int(at.last_column)
        if (int(at.last_line) != ln || to <= from || to > length(line)
            || find(slice(line, 0, from + 1), "\"") < 0) return LintFix()
        let text = build_string() $(var w) {
            peek_data(line) $(d) {
                for (i in range(from, to)) {
                    let b = int(d[i])
                    if (b < 128) {
                        w |> write_char(b)
                    } else {
                        w |> write("\\x{b:02X}")
                    }
                }
            }
        }
        return LintFix(line = ln, column = from, last_line = ln, last_column = to,
                       replacement = text)
    }

    //! ``var a : array<T>`` + a for-loop that only pushes/inserts into it
    //! -> one comprehension; no fix for comments inside, element types
    //! that do not match the pushed values, or values that read ``a``.
    def st027_fix(var blk : ExprBlock?; idx : int; v : Variable?; isArray : bool) : LintFix {
        let decl = blk.list[idx]
        var forE = blk.list[idx + 1] as ExprFor
        var fbody = forE.body as ExprBlock
        if (!lint_fills_lines(decl, true) || !fix_declares_only(decl, v) || forE.genFlags.generated
            || v.init != null || forE.at.line != decl.at.line + 1u
            || length(fbody.list) != 1) return LintFix()
        let span = lint_range(decl.at, forE.at)
        var stmt = fbody.list[0]
        var ifte = stmt is ExprIfThenElse ? stmt as ExprIfThenElse : null
        if (ifte != null
            && (ifte.if_false != null || !(ifte.if_true is ExprBlock)
                || length((ifte.if_true as ExprBlock).list) != 1
            || fix_reads(ifte.cond, v))) return LintFix()
        let where_clause = ifte != null ? "; where {lint_expr_text(ifte.cond)}" : ""
        let item = st_item_text(ifte != null ? (ifte.if_true as ExprBlock).list[0] : stmt, v)
        var header_at = forE.at
        header_at.last_line = fbody.at.line
        header_at.last_column = fbody.at.column
        let header = strip(lint_span_text(header_at))
        if (empty(item)) return LintFix()
        let text = (isArray ? "var {v.name} <- [{header}; {item}{where_clause}]"
                : "var {v.name} <- \{{header}; {item}{where_clause}\}")
        return lint_span_fix(span, text)
    }

    //! ``var t : table<K; V>`` + a run of inserts / ``t[k] = v`` -> one
    //! table (set) literal.
    def st031_fix(var blk : ExprBlock?; idx, count : int; v : Variable?) : LintFix {
        if (!fix_run_ok(blk, idx, count + 1) || !fix_declares_only(blk.list[idx], v)
            || v.init != null) return LintFix()
        var items : array<string>
        items |> reserve(count)
        for (k in range(count)) {
            let item = st_item_text(blk.list[idx + 1 + k], v)
            if (empty(item)) return LintFix()
            items |> push(item)
        }
        let one_line = "var {v.name} <- \{ {join(items, ", ")} \}"
        let col = int(blk.list[idx].at.column)
        let pad = repeat(" ", col + 4)
        let sep = ",\n{pad}"
        return lint_fix(lint_range(blk.list[idx].at, blk.list[idx + count].at),
                        (col + length(one_line) <= STYLE042_MAX_LINE ? one_line
            : "var {v.name} <- \{\n{pad}{join(items, sep)}\n{repeat(" ", col)}\}"))
    }

    //! ``var w : array<T>`` + ``push_from`` / ``push_clone_from`` runs ->
    //! ``var w := SRC`` or ``var w <- concat(...)``.
    def st032_fix(var blk : ExprBlock?; idx, count : int; v : Variable?) : LintFix {
        if (!fix_run_ok(blk, idx, count + 1) || !fix_declares_only(blk.list[idx], v)
            || v.init != null
            || (count > 1 && !lint_module_visible(current_function, "linq"))) return LintFix()
        var sources : array<string>
        sources |> reserve(count)
        for (k in range(count)) {
            var src = (blk.list[idx + 1 + k] as ExprCall).arguments[1]
            let text = lint_expr_text(src)
            if (empty(text) || fix_reads(src, v)) return LintFix()
            sources |> push(text)
        }
        let init = count == 1 ? ":= {sources[0]}" : "<- concat({join(sources, ", ")})"
        return lint_fix(lint_range(blk.list[idx].at, blk.list[idx + count].at),
                        "var {v.name} {init}")
    }

    //! A run of ``count`` bulk appends into one receiver -> one variadic
    //! call.
    def st033_fix(var blk : ExprBlock?; i, count : int; fname : string) : LintFix {
        var recv = (blk.list[i] as ExprCall).arguments[0]
        let recv_names <- lint_name_counts(recv)
        if (!fix_run_ok(blk, i, count) || empty(recv_names)) return LintFix()
        var sources : array<string>
        sources |> reserve(count)
        for (k in range(count)) {
            var src = (blk.list[i + k] as ExprCall).arguments[1]
            let text = lint_expr_text(src)
            if (empty(text)) return LintFix()
            for (name in keys(lint_name_counts(src))) {
                return LintFix() if (key_exists(recv_names, name))
            }
            sources |> push(text)
        }
        return lint_fix(lint_range(blk.list[i].at, blk.list[i + count - 1].at),
                        "{lint_expr_text(recv)} |> {fname}({join(sources, ", ")})")
    }

    def check_style042_expression_body(fn : FunctionPtr) : void {
        let body = st042_written_body(fn)
        if (body == null || body.genFlags.generated || length(body.list) != 1 || !empty(body.finalList)
                || int(body.at.last_line) - int(body.at.line) > 2) return
        let ret = body.list[0]
        if (!(ret is ExprReturn) || ret.genFlags.generated) return
        let r = ret as ExprReturn
        if (r.subexpr == null) return
        let full = string(root_generic(fn).name)
        let name = fn.flags.isClassMethod ? slice(full, rfind(full, "`") + 1) : full
        let fix = st042_fix(body)
        return if (fix.line < 0)
        let arrow = r.returnFlags.moveSemantics ? "=> <- EXPR" : "=> EXPR"
        style_warning("STYLE042: function '{name}' body is a single 'return'; write it as an expression body - 'def {name}(...) {arrow}'", fn.at, fix)
    }

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
        unsafe_block_stack |> push(0)
        complexity_stack |> push(1)
        st041_candidates |> clear()
        check_defer_pipe(fn)
        check_legacy_match_arms(fn)
        check_block_match_arms(fn)
        check_style042_expression_body(fn)
        st047_check_operator(fn)
        if (analyze_requires && fn._module == this_module) {
            for (a in fn.annotations) {
                if (a != null && a.annotation != null) st029_note_module(a.annotation._module)
            }
        }
    }

    def written_span(fn : FunctionPtr) : LineInfo =>
        fn.atDecl.line != 0u ? fn.atDecl : fn.body.at

    def code_line(at : LineInfo; ln : int) : string {
        let src = lint_source_line(at, ln)
        let comment_pos = code_find(src, "//")
        return strip(comment_pos >= 0 ? slice(src, 0, comment_pos) : src)
    }

    def match_head_open(code : string) : int {
        //! Where the ``(`` of a ``match (...)`` / ``static_match (...)`` head opens on the line, or -1.
        let kw = starts_with(code, "static_match") ? 12 : (starts_with(code, "match") ? 5 : -1)
        return -1 if (kw < 0)
        let rest = strip_left(slice(code, kw))
        return starts_with(rest, "(") ? length(code) - length(rest) : -1
    }

    def match_arms_first_line(at : LineInfo; ln, last : int) : string {
        //! The first arm of the match whose head starts at ``ln``: the code after the ``{`` that follows the head's closing paren - empty when the head opens no arms block.
        var depth = 0
        for (l in range(ln, last + 1)) {
            let code = code_line(at, l)
            let start = l == ln ? match_head_open(code) : 0
            var close = -1
            peek_data(code) $(bytes) {
                for (ch, i in bytes, count()) {
                    continue if (i < start)
                    depth += int(ch) == '(' ? 1 : (int(ch) == ')' ? -1 : 0)
                    if (depth == 0) {
                        close = i
                        break
                    }
                }
            }
            continue if (close < 0)
            let after = strip(slice(code, close + 1))
            return "" if (!starts_with(after, "\{"))
            let same_line = strip(slice(after, 1))
            return same_line if (!empty(same_line))
            for (next in range(l + 1, last + 1)) {
                let arm = code_line(at, next)
                return arm if (!empty(arm))
            }
            return ""
        }
        return ""
    }

    def check_legacy_match_arms(fn : FunctionPtr) : void {
        if (fn.flags.generated || fn.body == null || fn.at.fileInfo == null || fn.at.line == 0u) return
        let last = int(fn.body.at.last_line)
        for (ln in range(int(fn.body.at.line), last + 1)) {
            continue if (match_head_open(code_line(fn.at, ln)) < 0)
            let arm = match_arms_first_line(fn.at, ln, last)
            continue if (!starts_with(arm, "if (") && !starts_with(arm, "if("))
            var at = fn.at
            at.line = uint(ln)
            at.last_line = uint(ln)
            style_warning("STYLE044: 'if (pattern)' match arms are deprecated; write 'pattern => $ \{ body \}' arms - '$v(n)' becomes the bare name 'n', and a bare name that compared becomes 'match_expr(name)'", at)
        }
    }

    def in_match_arms(at : LineInfo; ln, first : int) : bool {
        //! Whether line ``ln`` sits directly in the arms block of a ``match``
        //! / ``static_match``: the block it is in opens on a match head.
        var depth = 0
        for (back in range(ln - first)) {
            let code = code_line(at, ln - 1 - back)
            peek_data(code) $(bytes) {
                for (ch in bytes) {
                    depth += int(ch) == '}' ? 1 : (int(ch) == '{' ? -1 : 0)
                }
            }
            return match_head_open(code) >= 0 if (depth < 0)
        }
        return false
    }

    def is_arm_expression(code : string) : bool {
        //! Whether one statement can be a match arm's expression: no
        //! declaration, control flow, ``delete``, ``unsafe``, postfix ``if``,
        //! assignment, or brace outside a string.
        return false if (empty(code) || code_find(code, " if (") >= 0 || code_find(code, "<-") >= 0
            || ends_with(code, ";"))
        for (kw in ["return", "let", "var", "if", "elif", "else", "for", "while", "break",
                    "continue", "delete",
                    "unsafe", "yield", "pass", "assume", "static_if", "try", "goto", "label"]) {
            return false if (code == kw || starts_with(code, "{kw} ") || starts_with(code, "{kw}("))
        }
        var assigns = false
        var in_string = false
        var prev = 0
        peek_data(code) $(bytes) {
            for (ch, i in bytes, count()) {
                let c = int(ch)
                if (in_string) {
                    in_string = !(c == '"' && prev != '\\')
                } elif (c == '"') {
                    in_string = true
                } elif (c == '{' || c == '}') {
                    assigns = true
                } elif (c == '=') {
                    let next = i + 1 < length(bytes) ? int(bytes[i + 1]) : 0
                    assigns ||= (prev != '=' && prev != '!' && prev != '<' && prev != '>'
                        && next != '=' && next != '>')
                }
                prev = c
            }
        }
        return !assigns
    }

    def check_block_match_arms(fn : FunctionPtr) : void {
        return if (fn.flags.generated || fn.body == null || fn.at.fileInfo == null
            || fn.at.line == 0u)
        let first = int(fn.body.at.line)
        let last = int(fn.body.at.last_line)
        for (ln in range(first, last + 1)) {
            let head = lint_source_line(fn.at, ln)
            let arrow = code_find(head, "=> $")
            let code = strip(head)
            continue if (arrow < 0 || code_line(fn.at, ln) != code
                || !starts_with(slice(head, arrow), "=> $ \{"))
            var fix = LintFix(line = ln, column = arrow, last_line = ln)
            var expr = ""
            if (ends_with(code, "=> $ \{") && ln + 2 <= last) {
                let body = lint_source_line(fn.at, ln + 1)
                let close = lint_source_line(fn.at, ln + 2)
                let indent = length(head) - length(strip_left(head))
                continue if (code_find(body, "//") >= 0 || strip(close) != "\}"
                    || length(close) - length(strip_left(close)) != indent)
                expr = strip(body)
                fix.last_line = ln + 2
                fix.last_column = length(close)
            } elif (ends_with(code, "\}")) {
                let close = length(strip_right(head)) - 1
                expr = strip(slice(head, arrow + 6, close))
                fix.last_column = close + 1
            }
            continue if (empty(expr) || arrow + 3 + length(expr) > STYLE042_MAX_LINE
                || !is_arm_expression(expr) || !in_match_arms(fn.at, ln, first))
            fix.replacement = "=> {expr}"
            var at = fn.at
            at.line = uint(ln)
            at.last_line = uint(ln)
            style_warning("STYLE046: this match arm's block holds one expression; " +
                "write it without the block - 'pattern => {expr}'", at, fix)
        }
    }

    def check_defer_pipe(fn : FunctionPtr) : void {
        if (fn.body == null || fn.at.fileInfo == null || fn.at.line == 0u) return
        let span = written_span(fn)
        for (ln in range(int(span.line), int(span.last_line) + 1)) {
            let src = lint_source_line(fn.at, ln)
            let comment_pos = code_find(src, "//")
            let defer_pos = find(src, "defer")
            if (defer_pos < 0 || (comment_pos >= 0 && comment_pos < defer_pos)) continue
            if (defer_pos > 0) {
                var preceding_ok = false
                peek_data(src) $(data) {
                    let ch = int(data[defer_pos - 1])
                    preceding_ok = ch == ' ' || ch == '\t'
                }
                if (!preceding_ok) continue
            }
            var trailing_ok = true
            peek_data(src) $(data) {
                if (defer_pos + 5 < length(data)) {
                    let ch = int(data[defer_pos + 5])
                    trailing_ok = !(is_alpha(ch) || is_number(ch) || ch == '_')
                }
            }
            if (!trailing_ok) continue
            let pipe_pos = find(src, "<|", defer_pos + 5)
            if (pipe_pos < 0) continue
            let after_pipe = strip_left(slice(src, pipe_pos + 2))
            let pipes_block = starts_with(after_pipe, "$(") || starts_with(after_pipe, "$ ") || starts_with(after_pipe, "$\{") || starts_with(after_pipe, "@") || starts_with(after_pipe, "\{")
            if (!pipes_block) continue
            var at = fn.at
            at.line = uint(ln)
            at.column = uint(defer_pos)
            at.last_line = uint(ln)
            at.last_column = uint(defer_pos + 5)
            let fix = (
                       starts_with(after_pipe, "$()") ? lint_token_fix(fn.at, ln, defer_pos + 5,
                " <| $() ", "() ")
                : starts_with(after_pipe, "\{") ? lint_token_fix(fn.at, ln, defer_pos + 5, " <| ",
                                                                 "() ") : LintFix())
            if (starts_with(after_pipe, "$()")) {
                style_warning("STYLE002: remove <| pipe and $() for parameterless block; use " +
                    "direct trailing block", at, fix)
            } else {
                style_warning("STYLE001: remove <| pipe before block argument; use direct " +
                    "trailing block syntax", at, fix)
            }
        }
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        st037_pop_and_check("function '{fn.name}'", "split it into smaller functions", fn.at)
        if (fn.body != null) {
            st038_check_length("function '{fn.name}'", "split it into smaller functions", fn.at, written_span(fn))
        }
        st041_flush()
        current_function = null
        unsafe_block_stack |> pop
        return <- fn
    }

    def st038_check_length(what : string; fix : string; at : LineInfo; decl_at : LineInfo) : void {
        //! ``decl_at`` is the declaration as parsed, before any annotation rewrote the body: its last line is the closing brace the author wrote
        if (max_function_length <= 0 || at.line == 0u || foreign_module_code()
                || (current_function != null && current_function.flags.generated)) return
        let nlines = int(decl_at.last_line) - int(at.line) + 1
        if (nlines > max_function_length) {
            style_warning("STYLE038: {what} is {nlines} lines long (max {max_function_length}); {fix}", at)
        }
    }

    def st037_bump() : void {
        let n = length(complexity_stack)
        if (n > 0) complexity_stack[n - 1]++
    }

    def foreign_module_code() : bool {
        if (current_function == null || this_module == null) return false
        if (current_function._module != this_module) return true
        let root = root_generic(current_function.fromGeneric)
        return root != null && root._module != this_module
    }

    def st037_pop_and_check(what : string; fix : string; at : LineInfo) : void {
        let n = length(complexity_stack)
        if (n == 0) return
        let cc = complexity_stack[n - 1]
        complexity_stack |> pop
        if (max_complexity > 0 && cc > max_complexity && !foreign_module_code()
                && !(current_function != null && current_function.flags.generated)) {
            style_warning("STYLE037: {what} has cyclomatic complexity {cc} (max {max_complexity}); {fix}", at)
        }
    }

    def override preVisitExprWhile(expr : ExprWhile?) : void {
        if (!expr.genFlags.generated) st037_bump()
    }

    def override preVisitExprFor(expr : ExprFor?) : void {
        if (!expr.genFlags.generated) st037_bump()
    }

    def override preVisitExprTryCatch(expr : ExprTryCatch?) : void {
        if (!expr.genFlags.generated) st037_bump()
    }

    def check_block_pipe(call_at : LineInfo; var arguments : dasvector`ptr`Expression;
                         fixable : bool) : void {
        let nargs = length(arguments)
        if (nargs == 0) return
        for (i in range(nargs)) {
            let arg = arguments[i]
            if (arg is ExprMakeBlock) {
                let mkblk = arg as ExprMakeBlock
                let blk = mkblk._block
                if (blk is ExprBlock) {
                    let eblk = blk as ExprBlock
                    if (eblk.blockFlags.isClosure) {
                        let block_arg_count = length(eblk.arguments)
                        let src = source_line_between(call_at, eblk.at)
                        let pipe_pos = find(src, "<|")
                        let has_pipe = pipe_pos >= 0
                        if (has_pipe) {
                            let after_pipe = strip_left(slice(src, pipe_pos + 2))
                            let pipes_block = starts_with(after_pipe, "$(") || starts_with(after_pipe, "$ ") || starts_with(after_pipe, "$\{") || starts_with(after_pipe, "@") || starts_with(after_pipe, "\{")
                            if (pipes_block) {
                                let arrow_pos = find(after_pipe, "=>")
                                let brace_pos = find(after_pipe, "\{")
                                let is_lambda_shorthand = arrow_pos >= 0 && (brace_pos < 0 || arrow_pos < brace_pos)
                                if (!is_lambda_shorthand) {
                                    let fix = fixable ? st001_pipe_fix(mkblk, eblk) : LintFix()
                                    if (block_arg_count == 0) {
                                        style_warning("STYLE002: remove <| pipe and $() for " +
                                            "parameterless block; use direct trailing block",
                                            call_at, fix)
                                    } else {
                                        style_warning("STYLE001: remove <| pipe before block " +
                                            "argument; use direct trailing block syntax", call_at,
                                            fix)
                                    }
                                }
                            }
                        }
                    }
                }
                break
            }
        }
    }

    def override preVisitExprCall(var expr : ExprCall?) : void {
        st029_record_func(expr.func)
        st029_record_unresolved_call(expr)
        st029_record_module_getter(expr)
        check_block_pipe(expr.at, expr.arguments, true)
        check_style019_clamp(expr)
        check_style020_from_jv(expr)
        st047_check_comparator(expr)
        if (lowered_unsafe_delete(expr)) {
            mark_unsafe_in_stack()
            return
        }
        if (expr.genFlags.generated) return
        if (call_func_needs_unsafe(expr.func, expr.flags.isForLoopSource)) {
            mark_unsafe_in_stack()
            return
        }
        if (expr.func != null && expr.func.flags.isClassMethod
                && expr.func.result != null && expr.func.result.isClass
                && expr.func.result.structType != null
                && expr.func.name == expr.func.result.structType.name) {
            mark_unsafe_in_stack()
        }
    }

    def is_supported_jv_scalar(t : TypeDecl?) : bool {
        //! True for the scalar types that have a matching `operator ??` overload in daslib/json_boost — see lines 85-140 of json_boost.das. Bitfield and enum `from_JV` overloads exist but have no `??` counterpart, so they stay silent.
        if (t == null) return false
        let bt = t.baseType
        return (bt == Type.tInt    || bt == Type.tUInt
                || bt == Type.tInt8  || bt == Type.tUInt8
                || bt == Type.tInt16 || bt == Type.tUInt16
                || bt == Type.tInt64 || bt == Type.tUInt64
                || bt == Type.tFloat || bt == Type.tDouble
                || bt == Type.tBool  || bt == Type.tString)
    }

    def check_style020_from_jv(expr : ExprCall?) : void {
        if (expr.func == null || length(expr.arguments) < 2) return
        let root = root_generic(expr.func)
        let is_from_JV = (root != null && root.name == "from_JV"
            && root._module != null && root._module.name == "json_boost")
        if (!is_from_JV) return
        if (current_function != null) {
            let in_cf = (current_function._module != null
                && current_function._module.name == "json_boost")
            let in_fg = (current_function.fromGeneric != null
                && current_function.fromGeneric._module != null
                && current_function.fromGeneric._module.name == "json_boost")
            return if (in_cf || in_fg)
        }
        if (!is_supported_jv_scalar(expr._type)) return
        style_warning("STYLE020: from_JV(v, type<T>, defV) on a supported scalar - use 'v ?? " +
            "defV' (operator ?? from json_boost)", expr.at, st020_fix(expr))
    }

    def is_math_call(expr : Expression? const; fname : string) : bool {
        //! True iff `expr` is `math::<fname>(_, _)` with exactly two arguments (peeling ExprRef2Value). Conservatively skips any non-math overload.
        let inner = peel_ref2value_const(expr)
        if (inner == null || !(inner is ExprCall)) return false
        let call = inner as ExprCall
        if (call.func == null || call.func.name != fname
            || call.func._module.name != "math") return false
        return length(call.arguments) == 2
    }

    def check_style019_clamp(expr : ExprCall?) : void {
        if (expr.func == null || expr.func._module.name != "math"
            || length(expr.arguments) != 2) return
        let outer_name = string(expr.func.name)
        if (outer_name != "min" && outer_name != "max") return
        let inner_name = outer_name == "min" ? "max" : "min"
        if (is_math_call(expr.arguments[0], inner_name)) {
            style_warning("STYLE019: nested {outer_name}({inner_name}(...)) - use 'clamp(x, lo, " +
                "hi)' from math module", expr.at, st019_fix(expr))
            return
        }
        if (is_math_call(expr.arguments[1], inner_name)) {
            style_warning("STYLE019: nested {outer_name}({inner_name}(...)) - use 'clamp(x, lo, " +
                "hi)' from math module", expr.at, st019_fix(expr))
        }
    }

    def override preVisitExprInvoke(var expr : ExprInvoke?) : void {
        check_block_pipe(expr.at, expr.arguments, true)
        check_style028_self_arrow(expr)
    }

    def check_style028_self_arrow(expr : ExprInvoke?) : void {
        //! `self->method(args)` inside a class method body lowers to ExprInvoke (isInvokeMethod=true) with arguments[0]=ExprField(value=ExprTypeDecl, name=method) — after type inference the same shape is produced by `self.method(args)` AND by the compiler's auto-promotion of bare `method(args)` to a method invoke. So the AST alone cannot distinguish the three forms — source inspection is required.
        if (expr == null || expr.genFlags.generated || !expr.isInvokeMethod ||
            current_function == null || !current_function.flags.isClassMethod ||
            current_function.fromGeneric != null ||
            empty(expr.arguments)) return
        var arg0 = expr.arguments[0]
        if (arg0 != null && arg0 is ExprRef2Value) {
            arg0 = (arg0 as ExprRef2Value).subexpr
        }
        if (arg0 == null || !(arg0 is ExprField)) return
        let f = arg0 as ExprField
        var recv = length(expr.arguments) > 1 ? peel_ref2value_const(expr.arguments[1]) : null
        if (recv != null && recv is ExprCast) {
            recv = peel_ref2value_const((recv as ExprCast).subexpr)
        }
        if (recv == null || !(recv is ExprVar) || (recv as ExprVar).name != "self"
            || recv.at.line != recv.at.last_line || f.atField.line != recv.at.line) return
        var arrow = recv.at
        arrow.column = recv.at.last_column
        arrow.last_column = f.atField.column
        if (strip(lint_span_text(arrow)) != "->") return
        arrow.column = recv.at.column
        style_warning("STYLE028: 'self->{f.name}(...)' on the class instance is optional; drop " +
            "'self->' and call '{f.name}(...)' directly (compiler auto-promotes to the same " +
            "method invoke). Use 'self.{f.name}(...)' if you prefer an explicit receiver", expr.at,
            lint_fix(arrow, ""))
    }


    def override preVisitExprMakeGenerator(var expr : ExprMakeGenerator?) : void {
        check_block_pipe(expr.at, expr.arguments, false)
    }

    def override preVisitExprMakeStruct(var expr : ExprMakeStruct?) : void {
        st029_record_func(expr.constructor)
        if (!expr.genFlags.generated && !expr.makeStructFlags.isNewClass
                && expr.makeType != null && expr.makeType.isClass) {
            mark_unsafe_in_stack()
        }
        if (expr.at.fileInfo == null || expr.at.line == 0u) return
        var src = ""
        get_file_source_line(expr.at.fileInfo, expr.at.line) $(line) {
            src := line
        }
        let col = int(expr.at.column)
        if (col >= length(src) || !starts_with(slice(src, col), "generator<")) return
        let pipe_pos = find(src, "<|", col + 10)
        if (pipe_pos < 0) return
        let after_pipe = strip_left(slice(src, pipe_pos + 2))
        let pipes_block = starts_with(after_pipe, "$(") || starts_with(after_pipe, "$ ") || starts_with(after_pipe, "$\{") || starts_with(after_pipe, "@") || starts_with(after_pipe, "\{")
        if (!pipes_block) return
        let arrow_pos = find(after_pipe, "=>")
        let brace_pos = find(after_pipe, "\{")
        let is_lambda_shorthand = arrow_pos >= 0 && (brace_pos < 0 || arrow_pos < brace_pos)
        if (is_lambda_shorthand) return
        if (starts_with(after_pipe, "$()")) {
            style_warning("STYLE002: remove <| pipe and $() for parameterless block; use direct trailing block", expr.at)
        } else {
            style_warning("STYLE001: remove <| pipe before block argument; use direct trailing block syntax", expr.at)
        }
    }

    def override preVisitExprMakeBlock(var expr : ExprMakeBlock?) : void {
        if (!empty(st041_candidates)) {
            for (cap in expr._capture) {
                for (vk, cv in keys(st041_candidates), values(st041_candidates)) {
                    if (vk != null && vk.name == cap.name) {
                        cv.poisoned = true
                    }
                }
            }
        }
        let blk = expr._block
        if (!(blk is ExprBlock)) return
        let eblk = blk as ExprBlock
        if (!eblk.blockFlags.isClosure || eblk.blockFlags.isLambdaBlock || !empty(eblk.arguments)) return
        let src = source_line_between(expr.at, eblk.at)
        let dollar_pos = find(src, "$()")
        if (dollar_pos < 0 || find(src, "<|") >= 0) return
        let after_dollar = strip_left(slice(src, dollar_pos + 3))
        if (starts_with(after_dollar, ":")) return
        style_warning("STYLE003: remove redundant $() for parameterless block; use $ \{", expr.at,
                      lint_token_fix(expr.at, int(expr.at.line), int(expr.at.column), "$() ", "$ "))
    }

    def peel_ref2value_const(e : Expression? const) : Expression? const {
        if (e != null && e is ExprRef2Value) return (e as ExprRef2Value.subexpr)
        return e
    }

    def expr_equal_struct(a : Expression? const; b : Expression? const) : bool {
        //! Structural equality via describe(). No purity filter.
        if (a == null || b == null) return false
        return describe(a) == describe(b)
    }

    def style035_numeric_cast_target(name : string) : Type {
        //! Built-in scalar numeric casts for which a character-code variable should instead use the language's native `int` character type.
        if (name == "int") return Type.tInt
        if (name == "int8") return Type.tInt8
        if (name == "int16") return Type.tInt16
        if (name == "int64") return Type.tInt64
        if (name == "uint") return Type.tUInt
        if (name == "uint8") return Type.tUInt8
        if (name == "uint16") return Type.tUInt16
        if (name == "uint64") return Type.tUInt64
        if (name == "float") return Type.tFloat
        if (name == "double") return Type.tDouble
        return Type.none
    }

    def plain_style035_variable(expr : Expression?; target : Type) : Variable const? {
        //! Return the variable behind a plain ExprVar read of the cast's numeric type. Fields, indexes, calls, and other expressions are deliberately excluded: STYLE035's fix changes the variable declaration itself.
        if (target == Type.none || target == Type.tInt || expr == null) return null
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        if (inner == null || !(inner is ExprVar)) return null
        let ev = inner as ExprVar
        let v = ev.variable
        if (v == null || v._type == null || v._type.baseType != target) return null
        return v
    }

    def numeric_char_cast_target(expr : Expression const?) : Type {
        //! T('<char>') folds to a typed ExprConst before this pass under normal compilation, but LineInfo retains the exact source span. Return T's base type, or Type.none for non-numeric/non-character casts.
        if (expr != null && expr is ExprCall) {
            let call = expr as ExprCall
            if (call.func == null || length(call.arguments) != 1
                    || call.arguments[0] == null) return Type.none
            let target = style035_numeric_cast_target(string(call.func.name))
            if (target == Type.none) return Type.none
            let arg = call.arguments[0]
            if (arg.at.fileInfo == null || arg.at.line == 0u
                    || arg.at.line != arg.at.last_line
                    || arg.at.last_column <= arg.at.column) return Type.none
            var arg_src = ""
            get_file_source_line(arg.at.fileInfo, arg.at.line) $(line) {
                let start = int(arg.at.column)
                let finish = int(arg.at.last_column)
                if (start >= 0 && finish <= length(line)) {
                    arg_src = slice(line, start, finish)
                }
            }
            let inner = strip(arg_src)
            if (length(inner) >= 3 && starts_with(inner, "'") && ends_with(inner, "'")) return target
            return Type.none
        }
        if (expr == null || expr.at.fileInfo == null
                || expr.at.line == 0u || expr.at.line != expr.at.last_line
                || expr.at.last_column <= expr.at.column) return Type.none
        var src = ""
        get_file_source_line(expr.at.fileInfo, expr.at.line) $(line) {
            let start = int(expr.at.column)
            let finish = int(expr.at.last_column)
            if (start >= 0 && finish <= length(line)) {
                src = slice(line, start, finish)
            }
        }
        let spelling = strip(src)
        let open = find(spelling, "(")
        if (open <= 0 || !ends_with(spelling, ")")) return Type.none
        let target = style035_numeric_cast_target(strip(slice(spelling, 0, open)))
        if (target == Type.none) return Type.none
        let inner = strip(slice(spelling, open + 1, length(spelling) - 1))
        if (length(inner) >= 3 && starts_with(inner, "'") && ends_with(inner, "'")) return target
        return Type.none
    }

    def check_style035_numeric_char_compare(expr : ExprOp2?) : void {
        if (expr.genFlags.generated
                || (expr.op != "==" && expr.op != "!=" && expr.op != "<"
                    && expr.op != "<=" && expr.op != ">" && expr.op != ">=")) return
        var target = numeric_char_cast_target(expr.right)
        var v = plain_style035_variable(expr.left, target)
        if (v == null) {
            target = numeric_char_cast_target(expr.left)
            v = plain_style035_variable(expr.right, target)
        }
        if (v == null) return
        style_warning("STYLE035: non-int numeric variable '{v.name}' is compared with a cast character literal; make '{v.name}' int and compare directly with the character literal", v.at)
    }

    def string_has_non_ascii(s : string const implicit; var badOffset : int&; var badByte : int&) : bool {
        var hit = false
        peek_data(s) $(arr) {
            for (i in range(length(arr))) {
                let b = int(arr[i])
                if (b >= 128) {
                    badOffset = i
                    badByte = b
                    hit = true
                    break
                }
            }
        }
        return hit
    }

    def source_span_has_non_ascii(at : LineInfo) : bool {
        if (at.fileInfo == null) return true
        var hit = false
        let l0 = int(at.line)
        let l1 = max(int(at.last_line), l0)
        for (ln in range(l0, l1 + 1)) {
            get_file_source_line(at.fileInfo, uint(ln)) $(line) {
                let len = length(line)
                let from = ln == l0 ? min(int(at.column), len) : 0
                let to = ln == l1 ? min(int(at.last_column), len) : len
                var o = 0
                var b = 0
                if (to > from && string_has_non_ascii(slice(line, from, to), o, b)) {
                    hit = true
                }
            }
            if (hit) break
        }
        return hit
    }

    def override preVisitExprConstString(expr : ExprConstString?) : void {
        var badOffset = 0
        var badByte = 0
        if (!ascii_strings || expr.genFlags.generated
                || !string_has_non_ascii(string(expr.value), badOffset, badByte)
                || !source_span_has_non_ascii(expr.at)) return
        style_warning("STYLE039: non-ASCII byte {badByte} at offset {badOffset} in string " +
            "literal; emitted strings must be ASCII - spell it out (e.g. '-' for an em-dash), or " +
            "'// nolint:STYLE039' when the character is intended", expr.at, st039_fix(expr.at))
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        st029_record_func(expr.func)
        if (!expr.genFlags.generated
                && call_func_needs_unsafe(expr.func, expr.flags.isForLoopSource)) {
            mark_unsafe_in_stack()
        }
        if (expr.op == "==") {
            if (is_string_rtti_comparison(expr.left) || is_string_rtti_comparison(expr.right)) {
                style_warning("STYLE006: string(__rtti) comparison should use `is` operator; " +
                    "e.g. expr is ExprFoo", expr.at, st006_fix(expr))
            }
        }
        if (expr.op == "==" || expr.op == "!=") {
            let lhs = peel_ref2value_const(expr.left)
            let rhs = peel_ref2value_const(expr.right)
            let lbool = lhs != null && lhs is ExprConstBool
            let rbool = rhs != null && rhs is ExprConstBool
            if (lbool != rbool) {
                let const_side = lbool ? lhs : rhs
                let cval = (const_side as ExprConstBool).value
                let positive = (expr.op == "==" && cval) || (expr.op == "!=" && !cval)
                let suggestion = positive ? "b" : "!b"
                style_warning("STYLE018: redundant boolean comparison - use '{suggestion}' " +
                    "directly (drop the '{expr.op} {cval}')", expr.at,
                    st018_fix(expr, lbool ? expr.right : expr.left, const_side, positive))
            }
        }
        if (expr.op == "|=" || expr.op == "&=") {
            check_style022_bitfield_set_clear(expr)
        }
        if (expr.op == "==" || expr.op == "!=") {
            check_style023_bitfield_test(expr)
        }
        check_style035_numeric_char_compare(expr)
    }

    def is_bitfield_value_type(t : TypeDecl?) : bool {
        //! True for plain bitfield-typed expressions (not arrays of bitfield).
        return t != null && t.baseType == Type.tBitfield
    }

    def extract_single_bit_name(expr : Expression? const; bf_type : TypeDecl?) : string {
        //! If `expr` resolves to a single named bit of `bf_type`, return its name; else "".
        if (expr == null) return ""
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        if (inner == null) return ""
        if (inner is ExprField) {
            let f = inner as ExprField
            if (f.value != null && f.value is ExprVar) return string(f.name)
            return ""
        }
        if (inner is ExprConstBitfield && bf_type != null) {
            let cb = inner as ExprConstBitfield
            let v = uint64(cb.value)
            if (v == 0ul || (v & (v - 1ul)) != 0ul) return ""
            var idx = 0
            var probe = v
            while (probe > 1ul) {
                probe = probe >> 1ul
                idx++
            }
            if (idx < 0 || idx >= length(bf_type.argNames)) return ""
            return string(bf_type.argNames[idx])
        }
        return ""
    }

    def is_zero_int_const(expr : Expression? const) : bool {
        //! True for ExprConstInt/UInt/Int64/UInt64 whose value is zero. The lint runner disables folding, so `0` / `0u` / `0l` / `0ul` literals stay as distinct typed const nodes — we accept all four.
        if (expr == null) return false
        if (expr is ExprConstInt) return (expr as ExprConstInt.value) == 0
        if (expr is ExprConstUInt) return (expr as ExprConstUInt.value) == 0u
        if (expr is ExprConstInt64) return (expr as ExprConstInt64.value) == 0l
        if (expr is ExprConstUInt64) return (expr as ExprConstUInt64.value) == 0ul
        return false
    }

    def unwrap_int_like_cast(expr : Expression? const) : Expression? const {
        //! Peel one int/uint/int64/uint64 single-arg call. Returns the inner expression, or null if not such a cast.
        if (expr == null || !(expr is ExprCall)) return null
        let call = expr as ExprCall
        if (call.func == null) return null
        let name = string(call.func.name)
        if ((name != "int" && name != "uint" && name != "int64" && name != "uint64") || length(call.arguments) != 1) return null
        return call.arguments[0]
    }

    def check_style023_bitfield_test(expr : ExprOp2?) : void {
        //! STYLE023: `int_cast(bf & BfT.bit) !=/== 0` collapses to `bf.bit` / `!bf.bit`.
        let lhs = peel_ref2value_const(expr.left)
        let rhs = peel_ref2value_const(expr.right)
        let lzero = is_zero_int_const(lhs)
        let rzero = is_zero_int_const(rhs)
        if (lzero == rzero) return
        let cast_side = lzero ? rhs : lhs
        let inner = unwrap_int_like_cast(cast_side)
        if (inner == null) return
        var and_expr = inner
        if (and_expr != null && and_expr is ExprRef2Value) {
            and_expr = (and_expr as ExprRef2Value.subexpr)
        }
        if (and_expr == null || !(and_expr is ExprOp2)) return
        let op2 = and_expr as ExprOp2
        if (op2.op != "&" || op2.left == null || !is_bitfield_value_type(op2.left._type)) return
        let bf_type = op2.left._type
        let bit_name = extract_single_bit_name(op2.right, bf_type)
        if (empty(bit_name)) return
        let positive = expr.op == "!="
        let recv = describe(op2.left)
        let suggestion = positive ? "{recv}.{bit_name}" : "!{recv}.{bit_name}"
        style_warning("STYLE023: bitfield test via int-cast can be written directly - use " +
            "'{suggestion}' instead (drop the cast and the '{expr.op} 0' compare)", expr.at,
            st023_fix(expr, op2, bit_name))
    }

    def check_style022_bitfield_set_clear(expr : ExprOp2?) : void {
        if (expr.left == null || !is_bitfield_value_type(expr.left._type)) return
        let bf_type = expr.left._type
        if (expr.op == "|=") {
            let name = extract_single_bit_name(expr.right, bf_type)
            if (empty(name)) return
            style_warning("STYLE022: 'foo |= BfT.{name}' on a bitfield is the bit-set idiom; " +
                "write 'foo.{name} = true' instead (bitfield-as-field assignment)", expr.at,
                st022_fix(expr, name))
        } elif (expr.op == "&=") {
            var inner = expr.right
            if (inner != null && inner is ExprRef2Value) {
                inner = (inner as ExprRef2Value.subexpr)
            }
            if (inner != null && inner is ExprOp1) {
                let op1 = inner as ExprOp1
                if (op1.op != "~") return
                let name = extract_single_bit_name(op1.subexpr, bf_type)
                if (empty(name)) return
                style_warning("STYLE022: 'foo &= ~BfT.{name}' on a bitfield is the bit-clear " +
                    "idiom; write 'foo.{name} = false' instead (bitfield-as-field assignment)",
                    expr.at, st022_fix(expr, name))
                return
            }
            if (inner != null && inner is ExprConstBitfield) {
                let cb = inner as ExprConstBitfield
                let v = uint64(cb.value)
                let n = uint64(length(bf_type.argNames))
                let valid_mask = n >= 64ul ? ~0ul : ((1ul << n) - 1ul)
                let complement = (~v) & valid_mask
                if (complement == 0ul || (complement & (complement - 1ul)) != 0ul) return
                var idx = 0
                var probe = complement
                while (probe > 1ul) {
                    probe = probe >> 1ul
                    idx++
                }
                if (idx < 0 || idx >= length(bf_type.argNames)) return
                let name = string(bf_type.argNames[idx])
                style_warning("STYLE022: 'foo &= ~BfT.{name}' on a bitfield is the bit-clear " +
                    "idiom; write 'foo.{name} = false' instead (bitfield-as-field assignment)",
                    expr.at, st022_fix(expr, name))
            }
        }
    }

    def single_terminator_body(body : Expression? const) : Expression? const {
        //! If `body` is a bare ExprReturn/Break/Continue, or a 1-statement ExprBlock wrapping one, return the inner terminator. Otherwise null.
        if (body == null) return null
        if (body is ExprReturn || body is ExprBreak || body is ExprContinue) return body
        if (body is ExprBlock) {
            let blk = body as ExprBlock
            if (length(blk.list) == 1 && empty(blk.finalList)) {
                let stmt = blk.list[0]
                if (stmt is ExprReturn || stmt is ExprBreak || stmt is ExprContinue) return stmt
            }
        }
        return null
    }

    def terminators_payload_equal(a : Expression? const; b : Expression? const) : bool {
        //! Two terminators have the same payload iff they are the same kind and their return-subexpr (if any) is structurally equal. Break==Break and Continue==Continue have no payload.
        if (a == null || b == null) return false
        if (a is ExprBreak) return b is ExprBreak
        if (a is ExprContinue) return b is ExprContinue
        if (a is ExprReturn) {
            if (!(b is ExprReturn)) return false
            let ra = a as ExprReturn
            let rb = b as ExprReturn
            if (ra.subexpr == null && rb.subexpr == null) return true
            if (ra.subexpr == null || rb.subexpr == null) return false
            return expr_equal_struct(ra.subexpr, rb.subexpr)
        }
        return false
    }

    def is_const_bool_return(term : Expression? const; var b_out : bool&) : bool {
        //! True if `term` is `return ExprConstBool(b)`. Writes the bool to b_out.
        if (term == null || !(term is ExprReturn)) return false
        let ret = term as ExprReturn
        let sub = peel_ref2value_const(ret.subexpr)
        if (sub == null || !(sub is ExprConstBool)) return false
        b_out = (sub as ExprConstBool).value
        return true
    }

    def unwrap_inner_ifte(if_false : Expression? const) : ExprIfThenElse? {
        //! Strip a 1-stmt ExprBlock wrapping an ExprIfThenElse, returning the inner if.
        if (if_false == null) return null
        if (if_false is ExprIfThenElse) return (if_false as ExprIfThenElse)
        if (if_false is ExprBlock) {
            let blk = if_false as ExprBlock
            if (length(blk.list) == 1 && empty(blk.finalList)) {
                let stmt = blk.list[0]
                if (stmt is ExprIfThenElse) return (stmt as ExprIfThenElse)
            }
        }
        return null
    }

    // --- STYLE045: a condition equivalent to one simpler comparison ---

    // Interval lattice over one int variable: && intersects, || unites, !
    // complements. Why subsumption misses it:
    // doc/source/reference/language/lint.rst.

    def private ivl_full() : array<Ivl> {
        var r : array<Ivl>
        r |> push(Ivl(lo = INT64_MIN_V, hi = INT64_MAX_V))
        return <- r
    }

    //! Value set for `x <op> c`, as a sorted list of disjoint inclusive
    //! intervals.
    def private ivl_from_cmp(op : string; c : int64) : array<Ivl> {
        var r : array<Ivl>
        if (op == "==") {
            r |> push(Ivl(lo = c, hi = c))
        } elif (op == "!=") {
            if (c > INT64_MIN_V) {
                r |> push(Ivl(lo = INT64_MIN_V, hi = c - 1l))
            }
            if (c < INT64_MAX_V) {
                r |> push(Ivl(lo = c + 1l, hi = INT64_MAX_V))
            }
        } elif (op == "<") {
            if (c > INT64_MIN_V) {
                r |> push(Ivl(lo = INT64_MIN_V, hi = c - 1l))
            }
        } elif (op == "<=") {
            r |> push(Ivl(lo = INT64_MIN_V, hi = c))
        } elif (op == ">") {
            if (c < INT64_MAX_V) {
                r |> push(Ivl(lo = c + 1l, hi = INT64_MAX_V))
            }
        } elif (op == ">=") {
            r |> push(Ivl(lo = c, hi = INT64_MAX_V))
        }
        return <- r
    }

    def private ivl_isect(a, b : array<Ivl>) : array<Ivl> {
        var r : array<Ivl>
        for (x in a) {
            for (y in b) {
                let lo = max(x.lo, y.lo)
                let hi = min(x.hi, y.hi)
                if (lo <= hi) {
                    r |> push(Ivl(lo = lo, hi = hi))
                }
            }
        }
        return <- ivl_normalize(r)
    }

    def private ivl_unite(a, b : array<Ivl>) : array<Ivl> {
        var r : array<Ivl> // nolint:STYLE033 — concat would pull daslib/linq into this module
        r |> push_from(a)
        r |> push_from(b)
        return <- ivl_normalize(r)
    }

    // sort by lo, then merge overlapping or adjacent runs
    def private ivl_normalize(var v : array<Ivl>) : array<Ivl> {
        var out : array<Ivl>
        if (empty(v)) return <- out
        v |> sort() <| cmp_by(_.lo)
        var cur = v[0]
        for (i in range(1, length(v))) {
            let nx = v[i]
            if (nx.lo <= cur.hi || (cur.hi < INT64_MAX_V && nx.lo == cur.hi + 1l)) {
                cur = Ivl(lo = cur.lo, hi = max(cur.hi, nx.hi))
            } else {
                out |> push(cur)
                cur = nx
            }
        }
        out |> push(cur)
        return <- out
    }

    def private ivl_invert(a : array<Ivl>) : array<Ivl> {
        var r : array<Ivl>
        var at = INT64_MIN_V
        var first = true
        for (x in a) {
            if (x.lo > at || (first && x.lo > INT64_MIN_V)) {
                if (x.lo > INT64_MIN_V && at <= x.lo - 1l) {
                    r |> push(Ivl(lo = at, hi = x.lo - 1l))
                }
            }
            first = false
            if (x.hi >= INT64_MAX_V) return <- ivl_normalize(r)
            at = x.hi + 1l
        }
        r |> push(Ivl(lo = at, hi = INT64_MAX_V))
        return <- ivl_normalize(r)
    }

    //! The simplest single comparison equal to this value set, or "" when
    //! none is.
    def private ivl_render(a : array<Ivl>; vname : string) : string {
        if (length(a) == 1) {
            let x = a[0]
            if (x.lo == INT64_MIN_V && x.hi == INT64_MAX_V) return ""       // STYLE010's job
            if (x.lo == x.hi) return "{vname} == {x.lo}"
            if (x.lo == INT64_MIN_V) return "{vname} <= {x.hi}"
            if (x.hi == INT64_MAX_V) return "{vname} >= {x.lo}"
            return ""                                                       // two-sided
        }
        // exactly one hole is `!=`
        if (length(a) == 2 && a[0].lo == INT64_MIN_V && a[1].hi == INT64_MAX_V
            && a[1].lo - a[0].hi == 2l) {
            return "{vname} != {a[0].hi + 1l}"
        }
        return ""
    }

    // A comparison leaf `v <op> const` (or the Yoda form). Returns false
    // for any other shape, or for a second distinct variable — the
    // lattice is single-variable.
    def private style045_leaf(e : Expression? const; var vr : Variable?&;
                              var vs : array<Ivl>) : bool {
        let ex = peel_ref2value_const(e)
        if (ex == null || !(ex is ExprOp2)) return false
        let o2 = ex as ExprOp2
        var op = "{o2.op}"
        var lhs = peel_ref2value_const(o2.left)
        var rhs = peel_ref2value_const(o2.right)
        if (lhs == null || rhs == null) return false
        var cval = 0l
        var v : Variable? = null
        if (lhs is ExprVar && rhs is ExprConstInt) {
            v = (lhs as ExprVar).variable
            cval = int64((rhs as ExprConstInt).value)
        } elif (rhs is ExprVar && lhs is ExprConstInt) {
            v = (rhs as ExprVar).variable
            cval = int64((lhs as ExprConstInt).value)
            if (op == "<") {
                op = ">"
            } elif (op == "<=") {
                op = ">="
            } elif (op == ">") {
                op = "<"
            } elif (op == ">=") {
                op = "<="
            }
        } else {
            return false
        }
        if (v == null || v._type == null || v._type.baseType != Type.tInt
            || (op != "==" && op != "!=" && op != "<" && op != "<=" && op != ">" && op != ">=")
            || (vr != null && vr != v)) return false
        vr = v
        vs <- ivl_from_cmp(op, cval)   // nolint:PERF030
        return true
    }

    // Folds the &&/||/! tree into one value set. leaves counts the
    // comparisons seen, so a single comparison is never "simplified" into
    // itself.
    def private style045_eval(e : Expression? const; var vr : Variable?&;
                              var leaves : int&; var vs : array<Ivl>) : bool {
        let ex = peel_ref2value_const(e)
        if (ex == null) return false
        if (ex is ExprOp1 && "{(ex as ExprOp1).op}" == "!") {
            var inner : array<Ivl>
            if (!style045_eval((ex as ExprOp1).subexpr, vr, leaves, inner)) return false
            vs <- ivl_invert(inner)   // nolint:PERF030 — out-param, empty on entry
            return true
        }
        if (ex is ExprOp2) {
            let op = "{(ex as ExprOp2).op}"
            if (op == "&&" || op == "||") {
                var l, r : array<Ivl>
                if (!style045_eval((ex as ExprOp2).left, vr, leaves, l)
                    || !style045_eval((ex as ExprOp2).right, vr, leaves, r)) return false
                vs <- op == "&&" ? ivl_isect(l, r) : ivl_unite(l, r)   // nolint:PERF030
                return true
            }
        }
        if (!style045_leaf(ex, vr, vs)) return false
        leaves++
        return true
    }

    def private check_style045(ifte : ExprIfThenElse?) : void {
        var vr : Variable? = null
        var leaves = 0
        var vs : array<Ivl>
        if (!style045_eval(ifte.cond, vr, leaves, vs) || leaves < 2 || vr == null) return
        let simpler = ivl_render(vs, "{vr.name}")
        if (!empty(simpler)) {
            style_warning("STYLE045: this condition is equivalent to '{simpler}'; " +
                "the comparisons collapse over the integers", ifte.at)
        }
    }

    def check_style048(ifte : ExprIfThenElse?) : void {
        return if (!st048_sees_match || key_exists(st048_seen, intptr(ifte)))
        let chain <- st048_collect(ifte, st048_seen)
        let folded <- [for (arm in chain.arms); [for (alt in arm.alts); st048_fold(alt)]]
        let subjects <- st048_subjects(chain, folded)
        return if (empty(subjects) || length(subjects) > ST048_MAX_SUBJECTS)
        var rows <- st048_rows(chain, folded, subjects)
        var solved <- st048_coverage(chain, rows, subjects)
        return if (solved is err)
        let cov <- solved as ok
        for (row, ri in rows, count()) {
            row.arms |> push(ri)
        }
        var dead_arms = false
        for (ai in cov.unreachable) {
            assume arm = chain.arms[rows[ai].src]
            if (!arm.synthetic && !arm.is_else) {
                dead_arms = true
                let why = "the arms before it already take every value it tests"
                style_warning("STYLE048: this if/elif arm can never run; {why}", arm.node.at)
            }
        }
        return if (dead_arms || !empty(cov.dead_alternatives) || !st048_pays(chain, rows))
        let else_dead = has_value([for (row, ai in rows, count());
                                   chain.arms[row.src].is_else && !is_reachable(cov, ai)], true)
        let unreachable = length(cov.unreachable)
        for (k in range(unreachable)) {
            rows |> erase(cov.unreachable[unreachable - 1 - k])
        }
        st048_merge_bodies(chain, rows, cov)
        for (row in rows) {
            st048_factor(row.cells)
        }
        let hint = st048_render(chain, rows, subjects, cov, else_dead)
        let what = "if/elif chain compares the same values against constants"
        style_warning("STYLE048: {what}; rewrite it as {hint}", ifte.at)
    }

    def override preVisitExprIfThenElse(ifte : ExprIfThenElse?) : void {
        if (ifte.if_flags.isStatic || ifte.genFlags.generated) return
        st037_bump()
        check_style045(ifte)
        check_style048(ifte)
        let cond = ifte.cond
        if (cond is ExprConstBool) {
            let cbool = cond as ExprConstBool
            if (cbool.value && ifte.if_false == null) {
                style_warning("STYLE010: if (true) is always taken; use a bare block instead",
                              ifte.at, st010_fix(ifte))
            }
        }
        let outer_term = single_terminator_body(ifte.if_true)
        if (ifte.if_false == null && outer_term != null && ifte.if_true is ExprBlock) {
            let blk = ifte.if_true as ExprBlock
            if (blk.at.line != outer_term.at.line || blk.at.column != outer_term.at.column) {
                style_warning("STYLE005: braces around a single-statement early exit are noise; " +
                    "use 'if (cond) <terminator>' or postfix '<terminator> if (cond)'", ifte.at,
                    st005_fix(ifte, blk, outer_term))
            }
        }
        if (outer_term != null && ifte.if_false != null) {
            let inner = unwrap_inner_ifte(ifte.if_false)
            if (inner != null && inner.if_false == null) {
                let inner_term = single_terminator_body(inner.if_true)
                if (inner_term != null && terminators_payload_equal(outer_term, inner_term)) {
                    style_warning("STYLE016: adjacent guards leading to identical early-exit can be combined with '||'", ifte.at)
                }
            }
        }
        if (outer_term != null && ifte.if_false != null) {
            let else_term = single_terminator_body(ifte.if_false)
            if (else_term != null) {
                var b1 = false
                var b2 = false
                if (is_const_bool_return(outer_term, b1) && is_const_bool_return(else_term, b2) && b1 != b2) {
                    let suggestion = b1 ? "return cond" : "return !cond"
                    style_warning("STYLE017: 'if (cond) return {b1}; else return {b2}' - use " +
                        "'{suggestion}' directly", ifte.at, st017_fix(ifte, ifte.if_false, b1))
                }
            }
        }
    }

    def override preVisitExprLetVariable(expr : ExprLet?; var v : VariablePtr; last : bool) : void {
        if (v.init == null && !v.flags.generated && !v.flags.inScope && (current_function == null || current_function.fromGeneric == null)) {
            pending_uninit_vars |> push(v)
        }
        if (v.init != null && v._type != null && v._type.flags.ref
            && v.init.genFlags.userSaidItsSafe) {
            skip_userSaidItsSafe |> push(v.init)
        }
    }

    def find_assign_var(expr : Expression?) : Variable? {
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        return null if (inner == null || !(inner is ExprVar))
        return (inner as ExprVar.variable)
    }

    def st043_decl(e : ExpressionPtr) : ExprLet const? {
        if (!(e is ExprLet) || e.genFlags.generated || code_find(lint_source_line(e.at, int(e.at.line)), "//") >= 0) return null
        let decl = e as ExprLet
        for (v in decl.variables) {
            if (v.init != null || v.flags.generated || !empty(v.annotation) || find(string(v.name), "`") >= 0) return null
        }
        return decl
    }

    def st043_same(a, b : ExprLet const?) : bool {
        let va = a.variables[0]
        let vb = b.variables[0]
        return va.flags.inScope == vb.flags.inScope && describe(va._type) == describe(vb._type)
    }

    def check_style043_declaration_runs(blk : ExprBlock?) : void {
        if (current_function == null || foreign_module_code()) return
        let n = length(blk.list)
        var i = 0
        while (i < n - 1) {
            let first = st043_decl(blk.list[i])
            var j = i + 1
            while (first != null && j < n) {
                let next = st043_decl(blk.list[j])
                break if (next == null || !st043_same(first, next) || int(blk.list[j].at.line) != int(blk.list[j - 1].at.line) + 1)
                j++
            }
            if (j - i >= 2) {
                let names = build_string() $(var w) {
                    for (k in range(i, j)) {
                        for (v in st043_decl(blk.list[k]).variables) {
                            w |> write(", {v.name}")
                        }
                    }
                }
                let first_names = build_string() $(var w) {
                    for (v in first.variables) {
                        w |> write(", {v.name}")
                    }
                }
                let head = first.variables |> back()
                let tail = st043_decl(blk.list[j - 1]).variables |> back()
                style_warning("STYLE043: consecutive declarations share one type; declare them together - '{slice(names, 2)}'", blk.list[i].at,
                    LintFix(line = int(head.at.line), column = int(head.at.column) + length(string(head.name)),
                        last_line = int(tail.at.line), last_column = int(tail.at.column) + length(string(tail.name)), replacement = slice(names, length(first_names))))
            }
            i = j
        }
    }

    def override preVisitExprBlock(var blk : ExprBlock?) : void {
        if (blk.blockFlags.isClosure) {
            unsafe_block_stack |> push(0)
            if (!blk.genFlags.generated) complexity_stack |> push(1)
        }
        check_style012_chain_runs(blk)
        check_style033_existing_runs(blk)
        check_style033_existing_chain_runs(blk)
        check_style043_declaration_runs(blk)
        let n = length(blk.list)
        for (i in range(n - 1)) {
            let cur = blk.list[i]
            let nxt = blk.list[i + 1]
            if (!(cur is ExprIfThenElse) || cur.genFlags.generated || nxt.genFlags.generated) continue
            let outer_if = cur as ExprIfThenElse
            if (outer_if.if_false != null) continue
            let outer_term = single_terminator_body(outer_if.if_true)
            if (outer_term == null) continue
            if (nxt is ExprIfThenElse) {
                let nxt_if = nxt as ExprIfThenElse
                if (nxt_if.if_false == null) {
                    let nxt_term = single_terminator_body(nxt_if.if_true)
                    if (nxt_term != null && terminators_payload_equal(outer_term, nxt_term)) {
                        style_warning("STYLE016: adjacent guards leading to identical early-exit " +
                            "can be combined with '||'", outer_if.at, st016_fix(outer_if, nxt_if))
                        continue
                    }
                }
            }
            var b1 = false
            var b2 = false
            if (is_const_bool_return(outer_term, b1) && is_const_bool_return(nxt, b2) && b1 != b2) {
                let suggestion = b1 ? "return cond" : "return !cond"
                style_warning("STYLE017: 'if (cond) return {b1}; return {b2}' - use " +
                    "'{suggestion}' directly", outer_if.at, st017_fix(outer_if, nxt, b1))
            }
        }
    }

    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (blk.blockFlags.isClosure) {
            unsafe_block_stack |> pop
            if (!blk.genFlags.generated) {
                st037_pop_and_check("closure", "extract it into a named function", blk.at)
            }
        }
        return blk
    }

    def override preVisitExprBlockExpression(var blk : ExprBlock?; expr : ExpressionPtr) : void {
        if (!empty(pending_uninit_vars)) {
            if (expr is ExprCopy) {
                let v = find_assign_var((expr as ExprCopy).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    style_warning("STYLE011: variable declaration followed by immediate " +
                        "assignment; combine into single declaration with initialization", expr.at,
                        st011_fix(blk, expr, v))
                }
            } elif (expr is ExprClone) {
                let v = find_assign_var((expr as ExprClone).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    style_warning("STYLE011: variable declaration followed by immediate " +
                        "assignment; combine into single declaration with initialization", expr.at,
                        st011_fix(blk, expr, v))
                }
            } elif (expr is ExprMove) {
                let v = find_assign_var((expr as ExprMove).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    style_warning("STYLE011: variable declaration followed by immediate " +
                        "assignment; combine into single declaration with initialization", expr.at,
                        st011_fix(blk, expr, v))
                }
            }
            pending_uninit_vars |> resize(0)
        }
        if (expr is ExprLet) {
            check_style012_array_init(blk, expr as ExprLet)
            check_style013_struct_field_init(blk, expr as ExprLet)
            check_style021_jv_table_init(blk, expr as ExprLet)
            check_style027_for_loop_comprehension(blk, expr as ExprLet)
            check_style031_table_init(blk, expr as ExprLet)
            check_style032_clone_init(blk, expr as ExprLet)
            check_style041_flag_return(blk, expr as ExprLet)
        }
    }

    def check_style041_flag_return(blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function == null || current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 2 >= n) return
        for (v in elet.variables) {
            if (v.init == null || v.flags.generated || v.flags.inScope
                    || v._type == null || v._type.baseType != Type.tBool) continue
            let ini = peel_ref2value_const(v.init)
            if (ini == null || !(ini is ExprConstBool) || (ini as ExprConstBool).value) continue
            st041_analyze_candidate(blk, idx, n, v)
        }
    }

    def st041_analyze_candidate(blk : ExprBlock?; idx, n : int; v : VariablePtr) : void {
        var inscope st = St041Scan()
        for (t in range(idx + 1, n)) {
            st041_scan_stmt(blk.list[t], blk, t, t, true, v, st)
            return if (st.rejected)
        }
        return if (st.write_count == 0 || !st.writes_terminal || st.write_top < 0
            || st.consume_count != 1 || !st.consume_top_level || st.consume_top != st.write_top + 1)
        for (pt in st.plumb_tops) {
            return if (pt != st.write_top)
        }
        return if (!st.consume_ok)
        let msg = (st.consume_moved
            ? "STYLE041: bool flag '{v.name}' is a return value in disguise; move the 'if ({v.name})' body to each '{v.name} = true' site and drop the flag"
            : "STYLE041: bool flag '{v.name}' is a return value in disguise; return directly at each '{v.name} = true' site and drop the flag")
        st041_candidates[v] = St041Candidate(
            classified_refs = st.write_count + st.plumb_count + 1,
            observed_refs = 0,
            msg = msg)
    }

    def st041_classify_write(cp : ExprCopy?; arm : ExprBlock?; pos, top : int;
                             v : VariablePtr; var st : St041Scan) : void {
        return if (find_assign_var(cp.left) != v)
        let rhs = peel_ref2value_const(cp.right)
        if (rhs == null || !(rhs is ExprConstBool) || !(rhs as ExprConstBool).value || arm == null) {
            st.rejected = true
            return
        }
        st.write_count++
        if (st.write_top == -1) {
            st.write_top = top
        } elif (st.write_top != top) {
            st.rejected = true
            return
        }
        let kind = st041_terminal_kind(arm, pos)
        var guarded = kind != St041Terminal.not_terminal
        for (i in range(length(st.guard_stack) - (kind == St041Terminal.then_break ? 1 : 0))) {
            if (!st.guard_stack[i]) {
                guarded = false
            }
        }
        if (!guarded) {
            st.writes_terminal = false
        }
    }

    def st041_scan_stmt(stmt : ExpressionPtr; arm : ExprBlock?; pos, top : int; is_top : bool;
                        v : VariablePtr; var st : St041Scan) : void {
        return if (stmt == null || st.rejected || stmt.genFlags.generated)
        if (stmt is ExprCopy) {
            st041_classify_write(stmt as ExprCopy, arm, pos, top, v, st)
            return
        }
        if (stmt is ExprIfThenElse) {
            let ifte = stmt as ExprIfThenElse
            return if (ifte.if_flags.isStatic)
            if (st041_is_flag_read(ifte.cond, v)) {
                let term = single_terminator_body(ifte.if_true)
                if (ifte.if_false == null && term != null && (term is ExprBreak || term is ExprContinue)) {
                    st.plumb_count++
                    st.plumb_tops |> push(top)
                } elif (st.consume_count == 0) {
                    st.consume_count = 1
                    st.consume_top = top
                    st.consume_top_level = is_top
                    st.consume_ok = st041_consume_body_ok(ifte, st.consume_moved)
                } else {
                    st.consume_count++
                }
                return
            }
            st041_scan_arm(ifte.if_true, top, v, st)
            st041_scan_arm(ifte.if_false, top, v, st)
            return
        }
        if (stmt is ExprFor) {
            st.guard_stack |> push(st041_loop_guarded((stmt as ExprFor).body, v))
            st041_scan_arm((stmt as ExprFor).body, top, v, st)
            st.guard_stack |> pop
            return
        }
        if (stmt is ExprWhile) {
            st.guard_stack |> push(st041_loop_guarded((stmt as ExprWhile).body, v))
            st041_scan_arm((stmt as ExprWhile).body, top, v, st)
            st.guard_stack |> pop
            return
        }
        if (stmt is ExprBlock) {
            st041_scan_arm(stmt, top, v, st)
        }
    }

    def st041_scan_arm(body : ExpressionPtr; top : int; v : VariablePtr; var st : St041Scan) : void {
        return if (body == null || st.rejected)
        if (body is ExprIfThenElse) {
            st041_scan_stmt(body, null, 0, top, false, v, st)
            return
        }
        return if (!(body is ExprBlock))
        let ab = body as ExprBlock
        let m = length(ab.list)
        for (p in range(m)) {
            st041_scan_stmt(ab.list[p], ab, p, top, false, v, st)
            return if (st.rejected)
        }
    }

    def st041_is_flag_read(e : Expression? const; v : VariablePtr) : bool {
        let pe = peel_ref2value_const(e)
        if (pe == null || !(pe is ExprVar)) return false
        return (pe as ExprVar).variable == v
    }

    def st041_terminal_kind(arm : ExprBlock?; pos : int) : St041Terminal {
        return St041Terminal.not_terminal if (!empty(arm.finalList))
        let m = length(arm.list)
        return St041Terminal.arm_end if (pos == m - 1)
        return St041Terminal.not_terminal if (pos != m - 2)
        return St041Terminal.then_break if (arm.list[m - 1] is ExprBreak)
        return St041Terminal.then_continue if (arm.list[m - 1] is ExprContinue)
        return St041Terminal.not_terminal
    }

    def st041_loop_guarded(body : Expression? const; v : VariablePtr) : bool {
        if (body == null || !(body is ExprBlock)) return false
        let ab = body as ExprBlock
        return false if (empty(ab.list))
        let stmt = ab.list[0]
        return false if (!(stmt is ExprIfThenElse))
        let ifte = stmt as ExprIfThenElse
        return false if (ifte.if_false != null || !st041_is_flag_read(ifte.cond, v))
        let term = single_terminator_body(ifte.if_true)
        return term != null && term is ExprBreak
    }

    def st041_consume_body_ok(ifte : ExprIfThenElse?; var moved_body : bool&) : bool {
        if (ifte == null || ifte.if_false != null || ifte.if_true == null) return false
        if (ifte.if_true is ExprReturn) {
            moved_body = false
            return st041_return_value_ok((ifte.if_true as ExprReturn).subexpr)
        }
        return false if (!(ifte.if_true is ExprBlock))
        let body = ifte.if_true as ExprBlock
        return false if (!empty(body.finalList))
        let m = length(body.list)
        return false if (m < 1 || !(body.list[m - 1] is ExprReturn))
        for (p in range(m - 1)) {
            let stmt = body.list[p]
            return false if (!(stmt is ExprCall))
            for (a in (stmt as ExprCall).arguments) {
                return false if (!st041_arg_repeatable(a, 0))
            }
        }
        moved_body = m > 1
        return st041_return_value_ok((body.list[m - 1] as ExprReturn).subexpr)
    }

    def st041_is_const_literal(e : Expression? const) : bool {
        return (e is ExprConstBool || e is ExprConstInt || e is ExprConstUInt
            || e is ExprConstInt64 || e is ExprConstUInt64 || e is ExprConstFloat
            || e is ExprConstDouble || e is ExprConstString || e is ExprConstEnumeration
            || e is ExprConstBitfield || e is ExprConstPtr)
    }

    def st041_return_value_ok(e0 : Expression? const) : bool {
        if (e0 == null) return true
        let e = peel_ref2value_const(e0)
        if (e == null) return false
        if (e is ExprVar) {
            let ev = e as ExprVar
            if (ev.variable == null || ev.variable._type == null) return false
            if (ev.varFlags.argument || ev.variable.flags.global) return ev.variable._type.flags.constant
            return false
        }
        if (e is ExprOp1) {
            let sub = peel_ref2value_const((e as ExprOp1).subexpr)
            return sub != null && st041_is_const_literal(sub)
        }
        return st041_is_const_literal(e)
    }

    def st041_arg_repeatable(e0 : Expression? const; depth : int) : bool {
        return false if (depth > ST041_MAX_ARG_DEPTH)
        let e = peel_ref2value_const(e0)
        if (e == null) return false
        return true if (e is ExprFakeContext || e is ExprFakeLineInfo)
        if (e is ExprVar) {
            let ev = e as ExprVar
            return ev.varFlags.argument || (ev.variable != null && ev.variable.flags.global)
        }
        return true if (st041_is_const_literal(e))
        if (e is ExprField) return st041_arg_repeatable((e as ExprField).value, depth + 1)
        if (e is ExprStringBuilder) {
            for (el in (e as ExprStringBuilder).elements) {
                return false if (!st041_arg_repeatable(el, depth + 1))
            }
            return true
        }
        if (e is ExprCall) {
            for (a in (e as ExprCall).arguments) {
                return false if (!st041_arg_repeatable(a, depth + 1))
            }
            return true
        }
        return false
    }

    def st041_flush() : void {
        for (vk, cv in keys(st041_candidates), values(st041_candidates)) {
            if (vk != null && !cv.poisoned && cv.observed_refs == cv.classified_refs) {
                style_warning(cv.msg, vk.at)
            }
        }
        st041_candidates |> clear()
    }

    def check_style012_array_init(var blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        for (v in elet.variables) {
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || v._type == null || v._type.baseType != Type.tArray) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                let stmt = blk.list[j]
                if (!(stmt is ExprCall) || !is_push_or_emplace_of(stmt as ExprCall, v)) break
                count++
            }
            if (count >= 2) {
                style_warning("STYLE012: array<T> initialized by a run of push/emplace calls; " +
                    "use an array literal `var a <- [x, <-y, z]` (or `var a <- array<T>(x, y, " +
                    "z)` when you need an explicit element type, e.g. for polymorphic upcasts)",
                    v.at,
                    st012_fix(blk, idx, count, v))
            }
        }
    }

    def check_style012_chain_runs(blk : ExprBlock?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        let n = length(blk.list)
        var i = 0
        while (i < n) {
            let recv = chain_push_receiver(blk.list[i])
            if (recv == null || chain_link_count(recv) < 1) {
                i++
                continue
            }
            var count = 1
            while (i + count < n) {
                let nrecv = chain_push_receiver(blk.list[i + count])
                if (nrecv == null || !chain_equal(recv, nrecv)) break
                count++
            }
            if (count >= 2) {
                emit_style012_chain(blk, i, recv)
            }
            i += count
        }
    }

    def chain_push_receiver(stmt : Expression const?) : Expression const? {
        //! When stmt is `<pure chain> |> push/emplace(value)` on an array<T>, returns the receiver chain expression; null otherwise. Exactly 2 args — the 3-arg `push(a, v, at_index)` positional-insert overload must not collapse into a literal.
        if (stmt == null || stmt.genFlags.generated || !(stmt is ExprCall)) return null
        let call = stmt as ExprCall
        if (call.func == null || call.func.fromGeneric == null
            || (call.func.fromGeneric.name != "push" && call.func.fromGeneric.name != "emplace")
            || length(call.arguments) != 2) return null
        let recv = call.arguments[0]
        if (recv == null || recv._type == null
            || recv._type.baseType != Type.tArray) return null
        return is_pure_chain(recv) ? recv : null
    }

    def peel_chain_adapter(e0 : Expression const?) : Expression const? {
        //! Strips typer-synthesized value adapters (Ref2Value, Ptr2Ref) that wrap chain links on pointer auto-deref — invisible to the user, safe to peel.
        var e : Expression const? = e0
        while (e != null) {
            if (e is ExprRef2Value) {
                e = (e as ExprRef2Value).subexpr
            } elif (e is ExprPtr2Ref) {
                e = (e as ExprPtr2Ref).subexpr
            } else {
                break
            }
        }
        return e
    }

    def is_pure_chain(e0 : Expression const?) : bool {
        //! Chain grammar: ExprVar root, ExprField links, ExprAt links with a const-int/uint/string or simple-var index. Anything else (calls, ?., computed indexes) is impure — receiver identity across the run can't be guaranteed, so the run is not collapsible.
        let e = peel_chain_adapter(e0)
        if (e == null) return false
        if (e is ExprVar) return true
        if (e is ExprField) return is_pure_chain((e as ExprField).value)
        if (e is ExprAt) {
            let ea = e as ExprAt
            let idx = peel_chain_adapter(ea.index)
            if (idx == null
                || (!(idx is ExprVar) && !(idx is ExprConstInt)
                    && !(idx is ExprConstUInt) && !(idx is ExprConstString))) return false
            return is_pure_chain(ea.subexpr)
        }
        return false
    }

    def chain_link_count(e0 : Expression const?) : int {
        let e = peel_chain_adapter(e0)
        if (e is ExprField) return 1 + chain_link_count((e as ExprField).value)
        if (e is ExprAt) return 1 + chain_link_count((e as ExprAt).subexpr)
        return 0
    }

    def chain_equal(a0 : Expression const?; b0 : Expression const?) : bool {
        let a = peel_chain_adapter(a0)
        let b = peel_chain_adapter(b0)
        if (a == null || b == null) return false
        if (a is ExprVar) {
            return false if (!(b is ExprVar))
            return (a as ExprVar).variable == (b as ExprVar).variable
        }
        if (a is ExprField) {
            return false if (!(b is ExprField))
            let af = a as ExprField
            let bf = b as ExprField
            return af.name == bf.name && chain_equal(af.value, bf.value)
        }
        if (a is ExprAt) {
            return false if (!(b is ExprAt))
            let aa = a as ExprAt
            let ba = b as ExprAt
            return chain_index_equal(aa.index, ba.index) && chain_equal(aa.subexpr, ba.subexpr)
        }
        return false
    }

    def chain_index_equal(a0 : Expression const?; b0 : Expression const?) : bool {
        let a = peel_chain_adapter(a0)
        let b = peel_chain_adapter(b0)
        if (a == null || b == null) return false
        if (a is ExprVar && b is ExprVar) {
            return (a as ExprVar).variable == (b as ExprVar).variable
        }
        if (a is ExprConstInt && b is ExprConstInt) {
            return (a as ExprConstInt).value == (b as ExprConstInt).value
        }
        if (a is ExprConstUInt && b is ExprConstUInt) {
            return (a as ExprConstUInt).value == (b as ExprConstUInt).value
        }
        if (a is ExprConstString && b is ExprConstString) {
            return (a as ExprConstString).value == (b as ExprConstString).value
        }
        return false
    }

    def chain_describe(e0 : Expression const?) : string {
        let e = peel_chain_adapter(e0)
        if (e is ExprVar) return string((e as ExprVar).name)
        if (e is ExprField) {
            let ef = e as ExprField
            return "{chain_describe(ef.value)}.{ef.name}"
        }
        if (e is ExprAt) {
            let ea = e as ExprAt
            let idx = peel_chain_adapter(ea.index)
            var istr = "?"
            if (idx is ExprVar) {
                istr = string((idx as ExprVar).name)
            } elif (idx is ExprConstInt) {
                istr = "{(idx as ExprConstInt).value}"
            } elif (idx is ExprConstUInt) {
                istr = "{(idx as ExprConstUInt).value}"
            } elif (idx is ExprConstString) {
                istr = "\"{(idx as ExprConstString).value}\""
            }
            return "{chain_describe(ea.subexpr)}[{istr}]"
        }
        return "?"
    }

    def ctor_field_state(v : Variable const?; fname : das_string) : CtorFieldState {
        var st : Structure const? = null
        if (v._type != null && v._type.baseType == Type.tStructure) {
            st = v._type.structType
        }
        if (st == null) return CtorFieldState.unknown
        for (fld in st.fields) {
            if (fld.name == fname) {
                if (fld.init != null) return CtorFieldState.initialized
                break
            }
        }
        var ms : ExprMakeStruct const? = null
        if (v.init != null) {
            return CtorFieldState.unknown if (!(v.init is ExprMakeStruct))
            ms = v.init as ExprMakeStruct
            return CtorFieldState.unknown if (ms._block != null)
        }
        if (ms != null) {
            for (row in ms.structs) {
                for (mf in *row) {
                    if (mf.name == fname) return CtorFieldState.initialized
                }
            }
        }
        return CtorFieldState.default_empty
    }

    def emit_style012_chain(blk : ExprBlock?; idx : int; recv : Expression const?) : void {
        let head = blk.list[idx]
        if (idx > 0 && blk.list[idx - 1] is ExprLet) {
            let elet = blk.list[idx - 1] as ExprLet
            let e = peel_chain_adapter(recv)
            if (e is ExprField) {
                let ef = e as ExprField
                let base = peel_chain_adapter(ef.value)
                if (base is ExprVar) {
                    let bvar = (base as ExprVar).variable
                    for (v in elet.variables) {
                        if (v != bvar || v.flags.generated || v.flags.inScope) continue
                        let state = ctor_field_state(v, ef.name)
                        if (state == CtorFieldState.default_empty) {
                            let tname = string(v._type.structType.name)
                            style_warning("STYLE012: array field '{ef.name}' of freshly constructed '{v.name}' filled by a run of push/emplace; fold an array literal into the constructor `var {v.name} <- {tname}({ef.name} <- [x, <-y, z], ...)`", head.at)
                            return
                        }
                        if (state == CtorFieldState.initialized) return
                        break
                    }
                }
            }
        }
        let chain = chain_describe(recv)
        style_warning("STYLE012: run of push/emplace into '{chain}'; use an array literal move-assign `{chain} <- [x, <-y, z]` (suppress with nolint when appending to existing content is intended)", head.at)
    }

    def check_style013_struct_field_init(blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        for (v in elet.variables) {
            if (v.flags.generated || v.flags.inScope
                || !is_struct_or_struct_ptr(v._type)
                || !is_default_struct_init(v)) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                if (!is_field_assign_of(blk.list[j], v)) break
                count++
            }
            if (count >= 2) {
                var inscope field_names : array<string>
                for (j in range(idx + 1, idx + 1 + count)) {
                    let name = field_name_of(blk.list[j])
                    if (!empty(name)) field_names |> push(name)
                }
                let is_new = v.init != null && v.init is ExprNew
                let struct_type = is_new ? v._type.firstType : v._type
                let type_name = (struct_type != null && struct_type.structType != null) ? string(struct_type.structType.name) : ""
                let ctor_prefix = is_new ? "new " : ""
                let init_desc = is_new ? "zero-arg `new`" : "default-init or uninitialized"
                let msg = build_string() $(var w) {
                    w |> write("STYLE013: var ({init_desc}) followed by assignments to fields \{")
                    for (k in range(length(field_names))) {
                        if (k != 0) w |> write(", ")
                        w |> write(field_names[k])
                    }
                    w |> write("\}; use a named-argument constructor `var {v.name} = {ctor_prefix}{type_name}(")
                    for (k in range(length(field_names))) {
                        if (k != 0) w |> write(", ")
                        w |> write("{field_names[k]} = ...")
                    }
                    w |> write(")`")
                }
                style_warning(msg, v.at, st013_fix(blk, idx, count, v))
            }
        }
    }

    def is_struct_or_struct_ptr(t : TypeDecl?) : bool {
        //! Match struct values AND pointers-to-struct, both flat (not array).
        if (t == null) return false
        let is_struct_val = t.baseType == Type.tStructure
        let is_struct_ptr = (t.baseType == Type.tPointer && t.firstType != null
                && t.firstType.baseType == Type.tStructure)
        return is_struct_val || is_struct_ptr
    }

    def is_default_struct_init(v : Variable?) : bool {
        //! True for `var a : Foo` (no init, struct-valued ONLY — pointer null-init is a different bug and is NOT flagged here), `var a = Foo()` (zero-arg MakeStruct), or `var a = new Foo()` / `var a = new Foo` (zero-arg ExprNew). False for any non-empty constructor, `make_foo()` call, or pipe chain — those are "modify an existing value" cases.
        if (v.init == null) return v._type != null && v._type.baseType == Type.tStructure
        if (v.init is ExprMakeStruct) {
            let ms = v.init as ExprMakeStruct
            if (ms._block != null) return false
            for (row in ms.structs) {
                for (_mf in *row) {
                    return false
                }
            }
            return true
        }
        if (v.init is ExprNew) {
            let en = v.init as ExprNew
            return empty(en.arguments)
        }
        return false
    }

    def is_field_assign_of(stmt : Expression?; target : Variable?) : bool {
        var lhs : Expression? = null
        if (stmt is ExprCopy) {
            lhs = (stmt as ExprCopy).left
        } elif (stmt is ExprClone) {
            lhs = (stmt as ExprClone).left
        } elif (stmt is ExprMove) {
            lhs = (stmt as ExprMove).left
        } else {
            return false
        }
        return false if (lhs == null || !(lhs is ExprField))
        var src = (lhs as ExprField).value
        if (src != null && src is ExprRef2Value) {
            src = (src as ExprRef2Value).subexpr
        }
        return false if (src == null || !(src is ExprVar))
        return (src as ExprVar).variable == target
    }

    def field_name_of(stmt : Expression?) : string {
        //! Return the ExprField name of stmt's LHS, or "" if stmt isn't a field assignment. Caller is expected to have already passed `is_field_assign_of`.
        var lhs : Expression? = null
        if (stmt is ExprCopy) {
            lhs = (stmt as ExprCopy).left
        } elif (stmt is ExprClone) {
            lhs = (stmt as ExprClone).left
        } elif (stmt is ExprMove) {
            lhs = (stmt as ExprMove).left
        } else {
            return ""
        }
        return "" if (lhs == null || !(lhs is ExprField))
        return string((lhs as ExprField).name)
    }

    def is_jv_pointer_type(t : TypeDecl?) : bool {
        if (t == null || t.baseType != Type.tPointer || t.firstType == null
                || t.firstType.baseType != Type.tStructure
                || t.firstType.structType == null) return false
        return (t.firstType.structType.name == "JsonValue"
            && t.firstType.structType._module != null
            && t.firstType.structType._module.name == "json")
    }

    def is_jv_table_type(t : TypeDecl?) : bool {
        //! Matches `table<string; JsonValue?>` (or any pointer-to-JsonValue value type).
        if (t == null || t.baseType != Type.tTable
                || t.firstType == null
                || t.firstType.baseType != Type.tString) return false
        return is_jv_pointer_type(t.secondType)
    }

    def is_insert_of_const_key(call : ExprCall?; target : Variable?) : bool {
        //! A computed key disqualifies the chain.
        if (call.func == null || call.func.fromGeneric == null
            || call.func.fromGeneric.name != "insert"
            || length(call.arguments) < 3) return false
        var recv = call.arguments[0]
        if (recv is ExprRef2Value) {
            recv = (recv as ExprRef2Value.subexpr)
        }
        if (recv == null || !(recv is ExprVar)
                || (recv as ExprVar).variable != target) return false
        var key = call.arguments[1]
        if (key is ExprRef2Value) {
            key = (key as ExprRef2Value.subexpr)
        }
        return key != null && key is ExprConstString
    }

    def check_style021_jv_table_init(blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        for (v in elet.variables) {
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || !is_jv_table_type(v._type)) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                let stmt = blk.list[j]
                if (!(stmt is ExprCall) || !is_insert_of_const_key(stmt as ExprCall, v)) break
                count++
            }
            if (count >= 2) {
                style_warning("STYLE021: table<string; JsonValue?> built by repeated insert calls; use 'var v = JV((k1=..., k2=..., ...))' (named-tuple JV form from json_boost)", v.at)
            }
        }
    }

    def check_style027_for_loop_comprehension(var blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        let nextStmt = blk.list[idx + 1]
        if (!(nextStmt is ExprFor)) return
        let forE = nextStmt as ExprFor
        for (v in elet.variables) {
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || v._type == null) continue
            let isArray = v._type.baseType == Type.tArray
            let isTable = v._type.baseType == Type.tTable
            if ((!isArray && !isTable)
                || !for_loop_matches_comprehension(forE, v, isArray, 1)) continue
            let kindLabel = isArray ? "array<T>" : "table<K;V>"
            let rewriteForm = (isArray
                ? "var {v.name} <- [for (x in SRC); EXPR; where COND]"
                : "var {v.name} <- \{for (x in SRC); KEY => VAL; where COND\}")
            style_warning("STYLE027: var {kindLabel} '{v.name}' followed by a for-loop that only " +
                "push/insert into it; use a comprehension `{rewriteForm}` (drop `; where COND` " +
                "when there is no filter)", v.at,
                st027_fix(blk, idx, v, isArray))
        }
    }

    def for_loop_matches_comprehension(forE : ExprFor?; target : Variable?;
                                       isArray : bool; for_budget : int) : bool {
        //! Recursively verifies forE's body is built solely from pushes/inserts/at-assigns into `target`, optionally wrapped in if-filters and nested-for (up to `for_budget` additional nested ExprFor levels). Returns false on any side-effecting statement or unrelated shape, so the all-or-nothing structure guarantees the suggested comprehension rewrite is semantically equivalent.
        if (forE == null || forE.genFlags.generated
            || forE.body == null || !(forE.body is ExprBlock)) return false
        let blk = forE.body as ExprBlock
        if (length(blk.list) != 1) return false
        return stmt_matches_in_for(blk.list[0], target, isArray, for_budget)
    }

    def stmt_matches_in_for(stmt : Expression?; target : Variable?;
                            isArray : bool; for_budget : int) : bool {
        if (stmt == null || stmt.genFlags.generated) return false
        if (stmt is ExprIfThenElse) {
            let ifte = stmt as ExprIfThenElse
            if (ifte.if_false != null || ifte.if_flags.isStatic
                || ifte.if_true == null || !(ifte.if_true is ExprBlock)) return false
            let tb = ifte.if_true as ExprBlock
            if (length(tb.list) != 1) return false
            return stmt_matches_in_for(tb.list[0], target, isArray, for_budget)
        }
        if (stmt is ExprFor) {
            if (for_budget <= 0) return false
            return for_loop_matches_comprehension(stmt as ExprFor, target, isArray, for_budget - 1)
        }
        if (isArray) {
            if (!(stmt is ExprCall)) return false
            let call = stmt as ExprCall
            return (is_named_target_call(call, target, 2, "push")
                || is_named_target_call(call, target, 2, "push_clone"))
        }
        if (stmt is ExprCall && is_named_target_call(stmt as ExprCall, target, 3, "insert")) return true
        return is_at_assign_of(stmt, target)
    }

    def is_named_target_call(call : ExprCall?; target : Variable?;
                             min_args : int; fname : string) : bool {
        //! Pipe-lowered call shape: `target |> fname(...)` resolves to fname(target, ...).
        if (call == null || call.func == null || call.func.fromGeneric == null
            || call.func.fromGeneric.name != fname
            || length(call.arguments) < min_args) return false
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (!(arg is ExprVar)) return false
        return (arg as ExprVar).variable == target
    }

    def is_at_assign_of(stmt : Expression?; target : Variable?) : bool {
        //! True when stmt is `target[<key>] = <expr>` — ExprCopy/Clone/Move whose LHS is ExprAt with subexpr resolving (after one ExprRef2Value peel) to ExprVar(target).
        var lhs : Expression? = null
        if (stmt is ExprCopy) {
            lhs = (stmt as ExprCopy).left
        } elif (stmt is ExprClone) {
            lhs = (stmt as ExprClone).left
        } elif (stmt is ExprMove) {
            lhs = (stmt as ExprMove).left
        } else {
            return false
        }
        return false if (lhs == null || !(lhs is ExprAt))
        var src = (lhs as ExprAt).subexpr
        if (src != null && src is ExprRef2Value) {
            src = (src as ExprRef2Value).subexpr
        }
        return false if (src == null || !(src is ExprVar))
        return (src as ExprVar).variable == target
    }

    def is_push_or_emplace_of(call : ExprCall?; target : Variable?) : bool {
        if (call.func == null || call.func.fromGeneric == null
            || (call.func.fromGeneric.name != "push" && call.func.fromGeneric.name != "emplace")
            || length(call.arguments) != 2) return false
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (!(arg is ExprVar)) return false
        return (arg as ExprVar).variable == target
    }

    def check_style032_clone_init(var blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        for (v in elet.variables) {
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || v._type == null || v._type.baseType != Type.tArray
                || v._type.firstType == null || !v._type.firstType.canClone) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                let stmt = blk.list[j]
                if (stmt == null || stmt.genFlags.generated || !(stmt is ExprCall)
                    || !is_array_clone_init_call(stmt as ExprCall, v)) break
                count++
            }
            if (count == 1) {
                style_warning("STYLE032: array<T> '{v.name}' default-initialized then filled by " +
                    "a single `push_from`/`push_clone_from`; that is a clone of the source - use " +
                    "`var {v.name} := SRC` (clone-assign)", v.at,
                    st032_fix(blk, idx, count, v))
            } elif (count >= 2 && count <= MAX_CONCAT_ARITY) {
                style_warning("STYLE033: array<T> '{v.name}' default-initialized then filled by " +
                    "a run of {count} `push_from`/`push_clone_from` from arrays - that is a " +
                    "concatenation; use `var {v.name} <- concat(s1, s2, ...)` (needs `require " +
                    "daslib/linq`)", v.at,
                    st032_fix(blk, idx, count, v))
            }
        }
    }

    def bulk_src_matches_elem(src : Expression? const; destElem : TypeDecl? const) : bool {
        //! A genuine bulk clone/copy: `src` is an `array<E>` whose element type E equals `destElem`. A *nested* source (`array<array<E>>` into a flat `array<E>`, or any deeper mismatch) binds the recursive-flatten push_from(Arr, src) overload (builtin.das) — that is a flatten, NOT a `:=`/concat/variadic-collapse, so it must NOT match. A C-array source (tFixedArray) is not tArray and has no clean array<T> rewrite either.
        if (src == null || src._type == null || src._type.baseType != Type.tArray
            || src._type.firstType == null || destElem == null) return false
        return describe(src._type.firstType) == describe(destElem)
    }

    def bulk_src_matches_dest(src : Expression? const; dest : Variable? const) : bool {
        if (dest == null || dest._type == null) return false
        return bulk_src_matches_elem(src, dest._type.firstType)
    }

    def is_array_clone_init_call(call : ExprCall?; target : Variable?) : bool {
        //! `target |> push_from(SRC)` / `push_clone_from(SRC)` (exactly 2 args, arg[0] == target) where SRC is an `array<E>` with E == target's element type. Into a fresh-empty target this bulk copy/clone is exactly `var target := SRC`.
        if ((!is_named_target_call(call, target, 2, "push_from")
             && !is_named_target_call(call, target, 2, "push_clone_from"))
            || length(call.arguments) != 2) return false
        return bulk_src_matches_dest(call.arguments[1], target)
    }

    def bulk_from_call_target(stmt : Expression?; var fname : string&) : Variable? {
        //! When stmt is `T |> push_from(SRC)` / `push_clone_from(SRC)` (exactly 2 args, SRC an array<E> with E == T's element type — a genuine bulk append, not a flatten), return T's variable and set fname to the call name; else null.
        if (stmt == null || stmt.genFlags.generated || !(stmt is ExprCall)) return null
        let call = stmt as ExprCall
        if (call.func == null || call.func.fromGeneric == null
            || length(call.arguments) != 2) return null
        let isFrom = call.func.fromGeneric.name == "push_from"
        let isCloneFrom = call.func.fromGeneric.name == "push_clone_from"
        if (!isFrom && !isCloneFrom) return null
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (!(arg is ExprVar)
            || !bulk_src_matches_dest(call.arguments[1], (arg as ExprVar).variable)) return null
        fname = isFrom ? "push_from" : "push_clone_from"
        return (arg as ExprVar).variable
    }

    def run_preceded_by_empty_array_decl(blk : ExprBlock?; runStart : int; v : Variable?) : bool {
        //! True when the statement just before the run is `var v : array<T>` (no init) — the fresh-empty case, owned by check_style032_clone_init (its STYLE033 concat branch).
        if (runStart <= 0) return false
        let prev = blk.list[runStart - 1]
        if (prev == null || !(prev is ExprLet)) return false
        for (lv in (prev as ExprLet).variables) {
            if (lv == v && lv.init == null
                && lv._type != null && lv._type.baseType == Type.tArray) return true
        }
        return false
    }

    def check_style033_existing_runs(var blk : ExprBlock?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        let n = length(blk.list)
        var i = 0
        while (i < n) {
            var fname = ""
            let target = bulk_from_call_target(blk.list[i], fname)
            if (target == null) {
                i++
                continue
            }
            var count = 1
            while (i + count < n) {
                var fname2 = ""
                let t2 = bulk_from_call_target(blk.list[i + count], fname2)
                if (t2 != target || fname2 != fname) break
                count++
            }
            if (count >= 2 && count <= MAX_VARIADIC_PUSH_ARITY
                && target._type != null && target._type.firstType != null
                && target._type.firstType.canCloneFromConst
                && !run_preceded_by_empty_array_decl(blk, i, target)) {
                style_warning("STYLE033: run of {count} `{fname}` calls into '{target.name}' - " +
                    "collapse to one variadic call `{target.name} |> {fname}(s1, s2, ...)`",
                    blk.list[i].at,
                    st033_fix(blk, i, count, fname))
            }
            i += count
        }
    }

    def bulk_from_chain_receiver(stmt : Expression const?; var fname : string&) : Expression const? {
        //! Like bulk_from_call_target but for a pure-CHAIN destination (>=1 field/index link, e.g. `obj.arr`): when stmt is `<chain> |> push_from/push_clone_from(SRC)` (2 args, SRC an array<E> with E == chain element type — bulk append, not flatten), return the receiver chain and set fname; else null. Plain ExprVar receivers (link_count 0) are left to bulk_from_call_target's variable form, so the two never double-fire.
        if (stmt == null || stmt.genFlags.generated || !(stmt is ExprCall)) return null
        let call = stmt as ExprCall
        if (call.func == null || call.func.fromGeneric == null
            || length(call.arguments) != 2) return null
        let isFrom = call.func.fromGeneric.name == "push_from"
        let isCloneFrom = call.func.fromGeneric.name == "push_clone_from"
        if (!isFrom && !isCloneFrom) return null
        let recv = call.arguments[0]
        if (recv == null || recv._type == null || recv._type.baseType != Type.tArray
            || !is_pure_chain(recv) || chain_link_count(recv) < 1
            || !bulk_src_matches_elem(call.arguments[1], recv._type.firstType)) return null
        fname = isFrom ? "push_from" : "push_clone_from"
        return recv
    }

    def check_style033_existing_chain_runs(var blk : ExprBlock?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        let n = length(blk.list)
        var i = 0
        while (i < n) {
            var fname = ""
            let recv = bulk_from_chain_receiver(blk.list[i], fname)
            if (recv == null) {
                i++
                continue
            }
            var count = 1
            while (i + count < n) {
                var fname2 = ""
                let nrecv = bulk_from_chain_receiver(blk.list[i + count], fname2)
                if (nrecv == null || fname2 != fname || !chain_equal(recv, nrecv)) break
                count++
            }
            if (count >= 2 && count <= MAX_VARIADIC_PUSH_ARITY
                    && recv._type != null && recv._type.firstType != null
                    && recv._type.firstType.canCloneFromConst) {
                let cd = chain_describe(recv)
                style_warning("STYLE033: run of {count} `{fname}` calls into '{cd}' - collapse " +
                    "to one variadic call `{cd} |> {fname}(s1, s2, ...)`", blk.list[i].at,
                    st033_fix(blk, i, count, fname))
            }
            i += count
        }
    }

    def is_table_insert_of(call : ExprCall?; target : Variable?; nargs : int) : bool {
        if (call.func == null || call.func.fromGeneric == null
            || call.func.fromGeneric.name != "insert"
            || length(call.arguments) != nargs) return false
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (!(arg is ExprVar)) return false
        return (arg as ExprVar).variable == target
    }

    def at_assign_key(stmt : Expression?) : Expression? {
        var lhs : Expression? = null
        if (stmt is ExprCopy) {
            lhs = (stmt as ExprCopy).left
        } elif (stmt is ExprClone) {
            lhs = (stmt as ExprClone).left
        } elif (stmt is ExprMove) {
            lhs = (stmt as ExprMove).left
        }
        return null if (lhs == null)
        return (lhs as ExprAt).index
    }

    def const_key_repr(key0 : Expression?) : string {  // nolint:STYLE037 — `is` on handled types is exact-type, so one arm per const class is irreducible
        //! Type-tagged stringification of a constant-literal key; "" for computed keys (not comparable at lint time).
        var key = key0
        if (key != null && key is ExprRef2Value) {
            key = (key as ExprRef2Value).subexpr
        }
        if (key == null) return ""
        if (key is ExprConstString) return "s:{(key as ExprConstString).value}"
        if (key is ExprConstInt) return "i:{(key as ExprConstInt).value}"
        if (key is ExprConstUInt) return "u:{(key as ExprConstUInt).value}"
        if (key is ExprConstInt64) return "i64:{(key as ExprConstInt64).value}"
        if (key is ExprConstUInt64) return "u64:{(key as ExprConstUInt64).value}"
        if (key is ExprConstFloat) return "f:{(key as ExprConstFloat).value}"
        if (key is ExprConstDouble) return "d:{(key as ExprConstDouble).value}"
        if (key is ExprConstBool) return "b:{(key as ExprConstBool).value}"
        if (key is ExprConstEnumeration) return "e:{(key as ExprConstEnumeration).value}"
        return ""
    }

    def st031_scan_insert_run(blk : ExprBlock?; idx, n : int; v : VariablePtr; isSet : bool; var dup_const_key : bool&) : int {
        var count = 0
        var seen_keys : table<string>
        for (j in range(idx + 1, n)) {
            let stmt = blk.list[j]
            var key : Expression? = null
            if (stmt is ExprCall && is_table_insert_of(stmt as ExprCall, v, isSet ? 2 : 3)) {
                key = (stmt as ExprCall).arguments[1]
            } elif (!isSet && is_at_assign_of(stmt, v)) {
                key = at_assign_key(stmt)
            } else {
                break
            }
            let repr = const_key_repr(key)
            if (repr != "") {
                if (key_exists(seen_keys, repr)) {
                    dup_const_key = true
                }
                seen_keys |> insert(repr)
            }
            count++
        }
        delete seen_keys
        return count
    }

    def st031_jv_const_key_run(blk : ExprBlock?; idx, n : int; v : VariablePtr) : int {
        var jv = 0
        for (j in range(idx + 1, n)) {
            let stmt = blk.list[j]
            if (!(stmt is ExprCall) || !is_insert_of_const_key(stmt as ExprCall, v)) break
            jv++
        }
        return jv
    }

    def check_style031_table_init(var blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        var idx = -1
        let n = length(blk.list)
        for (k in range(n)) {
            if (blk.list[k] == elet) {
                idx = k
                break
            }
        }
        if (idx < 0 || idx + 1 >= n) return
        for (v in elet.variables) {
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || v._type == null || v._type.baseType != Type.tTable) continue
            let isSet = v._type.secondType == null || v._type.secondType.baseType == Type.tVoid
            var dup_const_key = false
            let count = st031_scan_insert_run(blk, idx, n, v, isSet, dup_const_key)
            if (count < 2 || dup_const_key) continue
            if (is_jv_table_type(v._type)) {
                continue if (st031_jv_const_key_run(blk, idx, n, v) >= 2)
            }
            if (isSet) {
                style_warning("STYLE031: table<K> set initialized by a run of insert calls; use " +
                    "a set literal move-assign `var {v.name} <- \{k1, k2, ...\}`", v.at,
                    st031_fix(blk, idx, count, v))
            } else {
                style_warning("STYLE031: table<K;V> initialized by a run of insert/`[]=` " +
                    "statements; use a table literal move-assign `var {v.name} <- \{k1 => v1, k2 " +
                    "=> v2, ...\}`", v.at,
                    st031_fix(blk, idx, count, v))
            }
        }
    }

    def st029_expr_in_scope() : bool {
        return (analyze_requires
            && (current_function == null || current_function._module == this_module))
    }

    def st029_note_module(m : Module const?) : void {
        st029_note_module_name(m == null ? "" : "{m.name}")
    }

    def st029_note_module_name(n : string) : void {
        if (this_module == null || n == "" || n == "$" || n == "builtin" || n == "{this_module.name}") return
        if (!key_exists(used_modules, n)) used_modules |> insert(n)
    }

    def st029_record_module_getter(expr : ExprCall?) : void {
        if (expr.func == null || !st029_expr_in_scope() || empty(expr.arguments)) return
        let fn = expr.func.fromGeneric != null ? "{expr.func.fromGeneric.name}" : "{expr.func.name}"
        if (fn != "get_module" && fn != "find_module" && fn != "has_module" && fn != "find_compiling_module") return
        let arg0 = expr.arguments[0]
        if (arg0 != null && arg0 is ExprConstString) {
            st029_note_module_name(string((arg0 as ExprConstString).value))
        }
    }

    def st029_record_mod(m : Module const?) : void {
        if (st029_expr_in_scope()) st029_note_module(m)
    }

    def st029_record_func(fun : Function?) : void {
        return if (fun == null)
        if (fun.fromGeneric != null) st029_record_mod(fun.fromGeneric._module)
        else st029_record_mod(fun._module)
    }

    def st029_record_unresolved_call(expr : ExprCall?) : void {
        if (expr.func != null || !st029_expr_in_scope()) return
        var nm = "{expr.name}"
        let sep = nm |> rfind("::")
        if (sep >= 0) {
            nm = nm |> slice(sep + 2)
        }
        if (!empty(nm) && !key_exists(unresolved_call_names, nm)) {
            unresolved_call_names |> insert(nm)
        }
    }

    def st030_exports_unresolved_name(mod : Module?) : bool {
        if (empty(unresolved_call_names)) return false
        var seen : table<string>
        return st030_exports_unresolved_rec(mod, seen)
    }

    def st030_exports_unresolved_rec(mod : Module?; var seen : table<string>) : bool {
        var found = false
        for_each_function(mod, "") $(fn) {
            if (found) return
            if (key_exists(unresolved_call_names, "{fn.name}")) {
                found = true
            }
        }
        if (found) return true
        for_each_generic(mod, "") $(fn) {
            if (found) return
            if (key_exists(unresolved_call_names, "{fn.name}")) {
                found = true
            }
        }
        if (found) return true
        module_for_each_dependency(mod) $(dep, is_pub) {
            if (found || !is_pub || dep == null) return
            let dn = "{dep.name}"
            return if (dn == "" || dn == "$" || dn == "builtin" || key_exists(seen, dn))
            seen |> insert(dn)
            if (st030_exports_unresolved_rec(dep, seen)) {
                found = true
            }
        }
        return found
    }

    def override preVisitTypeDecl(typ : TypeDeclPtr) : void {
        if (typ.baseType == Type.tStructure && typ.structType != null) st029_record_mod(typ.structType._module)
        elif (typ.baseType == Type.tEnumeration && typ.enumType != null) st029_record_mod(typ.enumType._module)
        elif (typ.baseType == Type.tHandle && typ.annotation != null) st029_record_mod(typ.annotation._module)
    }

    def override preVisitStructure(var st : StructurePtr) : void {
        if (analyze_requires && st._module == this_module) {
            if (st.parent != null) st029_note_module(st.parent._module)
            for (a in st.annotations) {
                if (a != null && a.annotation != null) st029_note_module(a.annotation._module)
            }
        }
    }

    def override preVisitEnumeration(var en : EnumerationPtr) : void {
        if (analyze_requires && en._module == this_module) {
            for (a in en.annotations) {
                if (a != null && a.annotation != null) st029_note_module(a.annotation._module)
            }
        }
    }

    def st029_module_provides_macro(mod : Module?) : bool {
        var found = false
        for_each_call_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_reader_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_variant_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_for_loop_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_typeinfo_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_typemacro(mod) $(_) { found = true; }
        if (found) return true
        for_each_pass_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_capture_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_simulate_macro(mod) $(_) { found = true; }
        if (found) return true
        for_each_function_annotation(mod) $(_) { found = true; }
        if (found) return true
        return module_has_comment_reader(mod)
    }

    def st029_module_has_lifecycle_hook(mod : Module?) : bool {
        var found = false
        for_each_function(mod, "") $(fn) {
            if (found) return
            for (a in fn.annotations) {
                if (a != null && a.annotation != null) {
                    let an = "{a.annotation.name}"
                    if (an == "init" || an == "finalize" || an == "before_reload"
                            || an == "after_reload" || an == "live_command") {
                        found = true
                        break
                    }
                }
            }
        }
        return found
    }

    def st029_collect_public_deps(mod : Module?; var exported : table<string; bool>) : void {
        module_for_each_dependency(mod) $(dep, is_pub) {
            if (!is_pub || dep == null) return
            let dn = "{dep.name}"
            if (dn == "" || dn == "$" || dn == "builtin") return
            if (!key_exists(exported, dn)) {
                exported |> insert(dn, "{dep.fileName}" != "")
                st029_collect_public_deps(dep, exported)
            }
        }
    }

    //! every module the require reaches, public or not - what vanishes with the require
    def st029_collect_all_deps(mod : Module?; var reached : table<string; Module?>) : void {
        module_for_each_dependency(mod) $(dep, _is_pub) {
            if (dep == null) return
            let dn = "{dep.name}"
            if (dn == "" || dn == "$" || dn == "builtin") return
            if (!key_exists(reached, dn)) {
                reached |> insert(dn, dep)
                st029_collect_all_deps(dep, reached)
            }
        }
    }

    def st029_cached_has_hook(mod : Module?; var hook_cache : table<string; bool>) : bool {
        let mn = "{mod.name}"
        if (key_exists(hook_cache, mn)) return hook_cache?[mn] ?? false
        let has = st029_module_has_lifecycle_hook(mod)
        hook_cache |> insert(mn, has)
        return has
    }

    //! true when a lifecycle hook rides only through this require: a hook-bearing module in
    //! `closure` that no other require of the file reaches, and that `keep` - the module the
    //! rule would have the file require instead - does not reach either
    def st029_hook_only_through(closure : table<string; Module?>; hook_reach : table<string; int>;
                                var keep : Module?) : bool {
        var kept : table<string; Module?>
        if (keep != null) {
            kept |> insert("{keep.name}", keep)
            st029_collect_all_deps(keep, kept)
        }
        for (mn, reach in keys(hook_reach), values(hook_reach)) {
            if (reach > 1 || !key_exists(closure, mn) || key_exists(kept, mn)) continue
            return true
        }
        return false
    }

    def st029_check_requires(prog : ProgramPtr) : void {
        if (!analyze_requires || this_module == null) return
        let this_name = "{this_module.name}"
        var module_files : table<string>
        let primary = string(this_module.fileName)
        if (!empty(primary)) module_files |> insert(primary)
        for_each_function(this_module, "") $(fn) {
            if (fn.at.fileInfo != null && fn._module == this_module) {
                let fname = string(fn.at.fileInfo.name)
                if (!key_exists(module_files, fname)) module_files |> insert(fname)
            }
        }
        for_each_structure(this_module) $(st) {
            if (st.at.fileInfo != null && st._module == this_module) {
                let fname = string(st.at.fileInfo.name)
                if (!key_exists(module_files, fname)) module_files |> insert(fname)
            }
        }
        for_each_enumeration(this_module) $(en) {
            if (en.at.fileInfo != null && en._module == this_module) {
                let fname = string(en.at.fileInfo.name)
                if (!key_exists(module_files, fname)) module_files |> insert(fname)
            }
        }
        // per hook-bearing module, how many of this file's requires reach it - one means it rides only through that require
        var hook_cache : table<string; bool>
        var hook_reach : table<string; int>
        prog |> for_each_require_declaration() $ [unused_argument(req_name, file, pub)] (mod, req_name, file, pub, at) {
            if (mod == null || at.fileInfo == null || !key_exists(module_files, string(at.fileInfo.name))) return
            var reached : table<string; Module?>
            reached |> insert("{mod.name}", mod)
            st029_collect_all_deps(mod, reached)
            for (mn, m in keys(reached), values(reached)) {
                if (st029_cached_has_hook(m, hook_cache)) {
                    hook_reach[mn] = (hook_reach?[mn] ?? 0) + 1
                }
            }
        }
        st030_report_requires(prog, module_files, this_name, hook_reach)
    }

    [arch(at="ARCHITECTURE_LINT.md#style-lint-require-group")]
    def st030_report_requires(prog : ProgramPtr; module_files : table<string>; // nolint:STYLE038
                              this_name : string; hook_reach : table<string; int>) : void {
        var group_line : table<string; bool>
        prog |> for_each_require_declaration() $ [unused_argument(req_name, file)] (mod, req_name, file, pub, at) {
            if (mod == null || pub || at.fileInfo == null || !key_exists(module_files, string(at.fileInfo.name))) return
            let key = st030_decl_key(at)
            if (!key_exists(group_line, key)) {
                group_line[key] = st030_is_group_line(rtti_get_source_line(at.fileInfo, at.line))
            }
        }
        var keep : table<string; bool>
        var verdict, members, respell : table<string; string>
        prog |> for_each_require_declaration() $ [unused_argument(file)] (mod, req_name, file, pub, at) {
            if (mod == null || pub
                || at.fileInfo == null || !key_exists(module_files, string(at.fileInfo.name))) return
            let key = st030_decl_key(at)
            let mname = "{mod.name}"
            if (mname == "" || mname == this_name || mname == "$" || mname == "builtin"
                || key_exists(used_modules, mname)
                || st029_module_provides_macro(mod)
                || st029_module_has_lifecycle_hook(mod)
                || st030_exports_unresolved_name(mod)) {
                keep[key] = true
                return
            }
            var closure : table<string; bool>
            st029_collect_public_deps(mod, closure)
            var via : array<string>
            var used_count = 0
            for (cm in keys(closure)) {
                if (!key_exists(used_modules, cm)) continue
                used_count++
                if (closure?[cm] ?? false) via |> push(cm)
            }
            var reached : table<string; Module?>
            reached |> insert(mname, mod)
            st029_collect_all_deps(mod, reached)
            if (used_count == 1 && !empty(via)) {
                let mods = via[0]
                if ((group_line?[key] ?? false) || st029_hook_only_through(reached, hook_reach, reached?[mods] ?? null)) {
                    keep[key] = true
                    return
                }
                verdict["{key}|{req_name}"] = "029:{mods}"
                respell["{key}|{req_name}"] = st029_require_spelling(req_name, mod,
                    reached?[mods] ?? null)
                return
            }
            if (used_count > 0 || st029_hook_only_through(reached, hook_reach, null)) {
                keep[key] = true
                return
            }
            verdict["{key}|{req_name}"] = "030"
            let listed = members?[key] ?? ""
            members[key] = listed == "" ? "{req_name}" : "{listed}, {req_name}"
        }
        var group_reported : table<string; bool>
        prog |> for_each_require_declaration() $ [unused_argument(mod, file, pub)] (mod, req_name, file, pub, at) {
            if (at.fileInfo == null || !key_exists(module_files, string(at.fileInfo.name))) return
            let key = st030_decl_key(at)
            if (keep?[key] ?? false) return
            let v = verdict?["{key}|{req_name}"] ?? ""
            if (v == "") return
            if (starts_with(v, "029:")) {
                let mods = slice(v, 4)
                style_warning("STYLE029: require {req_name} is used only for the module it " +
                    "re-exports ({mods}); require it directly and drop {req_name}", at,
                    st029_fix(at, req_name, respell?["{key}|{req_name}"] ?? ""))
                return
            }
            if (group_line?[key] ?? false) {
                if (group_reported?[key] ?? false) return
                group_reported[key] = true
                style_warning("STYLE030: require [group] is unused - no symbol from any member " +
                    "({members?[key] ?? ""}) or a module it re-exports is referenced; drop it", at,
                    st030_fix(at, ""))
                return
            }
            style_warning("STYLE030: require {req_name} is unused - no symbol from it (or a " +
                "module it re-exports) is referenced; drop it", at, st030_fix(at, req_name))
        }
    }

    def st030_decl_key(at : LineInfo) : string => "{at.fileInfo.name}:{at.line}:{at.column}"

    def st030_is_group_line(src : string) : bool {
        //! `require [group]` or `require ?guard [group]` - the bracket right after the keyword
        //! and the optional guard, not one anywhere on the line
        var group = false
        peek_data(src) $(d) {
            let req = find(d, "require")
            if (req < 0) {
                return
            }
            var i = req + length("require")
            let n = length(d)
            while (i < n && is_tab_or_space(int(d[i]))) {
                i++
            }
            if (i < n && int(d[i]) == '?') {
                while (i < n && !is_tab_or_space(int(d[i])) && int(d[i]) != '[') {
                    i++
                }
                while (i < n && is_tab_or_space(int(d[i]))) {
                    i++
                }
            }
            group = i < n && int(d[i]) == '['
        }
        return group
    }

    def note_decl_span(var file_first_decl : table<string; uint>; fname : string; s : uint) : void {
        if (file_first_decl |> key_exists(fname)) {
            if (s < file_first_decl[fname]) {
                file_first_decl[fname] = s
            }
        } else {
            file_first_decl |> insert(fname, s)
        }
    }

    def scan_long_comment_blocks(prog : ProgramPtr) : void {
        let mod = prog.getThisModule
        if (mod == null) return
        var file_first_decl : table<string; uint>
        var fn_file : array<string>
        var fn_starts, fn_ends : array<uint>
        var fn_private : array<bool>
        for_each_function(mod, "") $(var fn) {
            if (fn.body == null || fn.at.fileInfo == null || fn.at.line == 0u
                || fn.flags.generated || fn.fromGeneric != null
                || fn._module != mod || fn.moreFlags.isTemplate) return
            let fname = string(fn.at.fileInfo.name)
            let s = fn.at.line
            let e = fn.body.at.last_line
            note_decl_span(file_first_decl, fname, s)
            fn_file |> push(fname)
            fn_starts |> push(s)
            fn_ends |> push(e)
            fn_private |> push(fn.flags.privateFunction)
        }
        for_each_structure(mod) $(var st) {
            if (st.at.fileInfo == null || st.at.line == 0u
                || st.flags.generated || st._module != mod || st.flags.isTemplate) return
            var is_template_instance = false
            peek(st.name) $(name) {
                is_template_instance = find(name, "<") >= 0
            }
            if (is_template_instance) return
            let fname = string(st.at.fileInfo.name)
            note_decl_span(file_first_decl, fname, st.at.line)
        }
        for_each_enumeration(mod) $(var en) {
            if (en.at.fileInfo == null || en.at.line == 0u) return
            let fname = string(en.at.fileInfo.name)
            note_decl_span(file_first_decl, fname, en.at.line)
        }
        for_each_global(mod) $(var v) {
            if (v.at.fileInfo == null || v.at.line == 0u || v.flags.generated) return
            let fname = string(v.at.fileInfo.name)
            note_decl_span(file_first_decl, fname, v.at.line)
        }
        for_each_typedef(mod) $(_name, value) {
            if (value.at.fileInfo == null || value.at.line == 0u) return
            let fname = string(value.at.fileInfo.name)
            note_decl_span(file_first_decl, fname, value.at.line)
        }
        scan_collected_files(mod, file_first_decl, fn_file, fn_starts, fn_ends, fn_private)
    }

    def scan_collected_files(mod : Module?;
                             var file_first_decl : table<string; uint>;
                             fn_file : array<string>; fn_starts, fn_ends : array<uint>;
                             fn_private : array<bool>) : void {
        var seen : table<string; bool>
        for_each_function(mod, "") $(var fn) {
            if (fn.at.fileInfo == null || fn._module != mod
                || fn.flags.generated || fn.fromGeneric != null || fn.moreFlags.isTemplate) return
            let fname = string(fn.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            scan_file_lines(fn.at, fname,
                file_first_decl[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_structure(mod) $(var st) {
            if (st.at.fileInfo == null || st._module != mod
                || st.flags.generated || st.flags.isTemplate) return
            var is_template_instance = false
            peek(st.name) $(name) {
                is_template_instance = find(name, "<") >= 0
            }
            if (is_template_instance) return
            let fname = string(st.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            scan_file_lines(st.at, fname,
                file_first_decl[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_enumeration(mod) $(var en) {
            if (en.at.fileInfo == null) return
            let fname = string(en.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            scan_file_lines(en.at, fname,
                file_first_decl[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_global(mod) $(var v) {
            if (v.at.fileInfo == null || v.flags.generated) return
            let fname = string(v.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            scan_file_lines(v.at, fname,
                file_first_decl[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_typedef(mod) $(_name, value) {
            if (value.at.fileInfo == null) return
            let fname = string(value.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            scan_file_lines(value.at, fname,
                file_first_decl[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
    }

    def scan_file_lines(sample_at : LineInfo; file_name : string;
                        first_decl : uint;
                        fn_file : array<string>; fn_starts, fn_ends : array<uint>;
                        fn_private : array<bool>) : void {
        var ln : uint = 1u
        var in_block = false
        var block_start : uint = 0u
        var block_count = 0
        var block_first_line = ""
        while (true) {
            var got = false
            var line_text = ""
            get_file_source_line(sample_at.fileInfo, ln) $(s) {
                got = true
                line_text = string(s)
            }
            if (!got) break
            let trimmed = strip_left(line_text)
            let is_comment = starts_with(trimmed, "//")
            if (is_comment) {
                if (!in_block) {
                    in_block = true
                    block_start = ln
                    block_count = 0
                    block_first_line = trimmed
                }
                block_count++
            } else {
                if (in_block) {
                    emit_long_comment(sample_at, file_name, first_decl, block_start, block_count, block_first_line,
                        fn_file, fn_starts, fn_ends, fn_private)
                    in_block = false
                }
            }
            ln++
        }
        if (in_block) {
            emit_long_comment(sample_at, file_name, first_decl, block_start, block_count, block_first_line,
                fn_file, fn_starts, fn_ends, fn_private)
        }
    }

    def st040_check_duplicates(prog : ProgramPtr) : void {
        return if (key_exists(disabled_codes, "STYLE040")
                   || (!empty(enabled_codes) && !key_exists(enabled_codes, "STYLE040"))
                   || !duplicate_regions_for(prog))
        var found <- find_extractable_dupes(prog)
        for (f in found) {
            if (f.anchor != null) {
                style_warning(f.message, f.anchor.at)
            }
        }
    }

    def has_nolint_directive(first_line : string) : bool {
        var rest = first_line
        if (starts_with(rest, "//!")) {
            rest = slice(rest, 3)
        } elif (starts_with(rest, "//")) {
            rest = slice(rest, 2)
        } else {
            return false
        }
        rest = strip_left(rest)
        return starts_with(rest, "@nolint")
    }

    def emit_long_comment(sample_at : LineInfo; file_name : string;
                          first_decl_line, block_start : uint; line_count : int;
                          first_line_text : string;
                          fn_file : array<string>; fn_starts, fn_ends : array<uint>;
                          fn_private : array<bool>) : void {
        if (line_count <= 1
            || block_start < first_decl_line
            || has_nolint_directive(first_line_text)) return
        var enclosing_priv = false
        var inside_function = false
        var enclosing_size : uint = 0xFFFFFFFFu
        for (f_file, s, e, f_priv in fn_file, fn_starts, fn_ends, fn_private) {
            if (f_file != file_name) continue
            if (s <= block_start && block_start <= e) {
                let size = e - s
                if (size < enclosing_size) {
                    enclosing_size = size
                    enclosing_priv = f_priv
                    inside_function = true
                }
            }
        }
        let private_ctx = inside_function && enclosing_priv
        // a '//!' block is a contract doc wherever it sits, so a private one gets the public cap
        let contract_doc = starts_with(first_line_text, "//!")
        let threshold = (private_ctx && !contract_doc) ? 1 : 3
        if (line_count <= threshold) return
        var at = sample_at
        at.line = block_start
        at.last_line = block_start
        at.column = 1u
        at.last_column = 1u
        var msg = ""
        if (private_ctx) {
            msg = "STYLE015: {line_count}-line comment inside 'def private' exceeds {threshold}-line cap; a '//!' contract doc there caps at 3 lines, a plain '//' block at one WHY line - or suppress on the first line with '//!@nolint' (//! blocks) or '// nolint:STYLE015' (// blocks)"
        } else {
            msg = "STYLE014: comment block of {line_count} lines exceeds 3-line cap; trim multi-paragraph prose to a 1-line WHY (move design notes to .md), or suppress on the first line with '//!@nolint' (//! blocks) or '// nolint:STYLE014' (// blocks)"
        }
        style_warning(msg, at)
    }

    def is_string_rtti_comparison(expr : Expression?) : bool {
        //! Returns true if expr is string(x.__rtti) pattern.
        return false if (expr == null)
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        return false if (inner == null || !(inner is ExprCall))
        var call = inner as ExprCall
        if (call.name != "string" || empty(call.arguments)) return false
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (arg is ExprField) {
            let field = arg as ExprField
            return field.name == "__rtti"
        }
        return false
    }

    def unsafe_frame_for(expr : Expression?) : UnsafeFrame {
        return UnsafeFrame() if (expr == null)
        return unsafeExprs |> key_exists(expr) ? unsafeExprs[expr] : UnsafeFrame()
    }

    def mark_unsafe_in_stack() : void {
        //! Add 1 to the current top of `unsafe_stack`.
        let n = length(unsafe_stack)
        if (n > 0) {
            unsafe_stack[n - 1].count++
        }
    }

    def mark_non_local_let_ref_in_stack() : void {
        //! Flag the current top of `unsafe_stack` as containing a non-local let-ref binding. Propagates upward through `visitExpression`.
        let n = length(unsafe_stack)
        if (n > 0) {
            unsafe_stack[n - 1].has_non_local_let_ref = true
        }
    }

    def override preVisitExpression(var expr : ExpressionPtr) : void {
        //! Push a fresh frame for this node.
        unsafe_stack |> push(UnsafeFrame(node = expr))
    }

    def override visitExpression(var expr : ExpressionPtr) : ExpressionPtr {
        //! The frame is the subtree's unsafe-requirement summary; mechanics in daslib/ARCHITECTURE_LINT.md.
        let n = length(unsafe_stack)
        var frame = unsafe_stack[n - 1]
        unsafe_stack |> pop
        if (frame.count > 0 && frame.op == null) {
            frame.op = expr
        }
        if (frame.count > 0 || frame.has_non_local_let_ref) {
            unsafeExprs |> insert(expr, frame)
            let m = length(unsafe_stack)
            if (m > 0) {
                unsafe_stack[m - 1].count = unsafe_stack[m - 1].count + frame.count
                if (unsafe_stack[m - 1].op == null) {
                    unsafe_stack[m - 1].op = frame.op
                }
                if (frame.has_non_local_let_ref) {
                    unsafe_stack[m - 1].has_non_local_let_ref = true
                }
            }
        }
        if (expr.genFlags.userSaidItsSafe && !expr.genFlags.generated
            && (current_function == null || current_function.fromGeneric == null)
            && !(skip_userSaidItsSafe |> has_value(expr))
            && frame.count == 0) {
            style_warning("STYLE024: redundant 'unsafe(...)' wrap; the inner expression has no " +
                "operation that requires unsafe - drop the wrap", expr.at,
                st024_unwrap_expr_fix(expr, fix_parent(0)))
        }
        return expr
    }

    def override preVisitExprLet(expr : ExprLet?) : void {
        //! Mirror `isLocalOrGlobal` check from ast_infer_type.cpp:4989 — `let v & = E` requires unsafe at statement level when `E` is non-local-non-temporary. Mark the let's frame so the enclosing `unsafe { ... }` block can detect it (STYLE025 must stay silent when narrowing would leave the let-ref binding unsatisfied).
        if (expr.genFlags.generated) return
        if (declaration_owns_unsafe(expr)) mark_unsafe_in_stack()
        for (v in expr.variables) {
            continue if (
                v._type == null ||
                !v._type.flags.ref ||
                v.init == null ||
                v.init.genFlags.alwaysSafe ||
                (v.init._type != null && v.init._type.flags.temporary) ||
                is_local_or_global_expr(v.init)
            )
            mark_non_local_let_ref_in_stack()
            return
        }
    }

    def call_func_needs_unsafe(func : Function const?; is_for_loop_src : bool) : bool {
        return (func == null
            || func.flags.unsafeOperation
            || func.moreFlags.unsafeWhenNotCloneArray
            || (func.moreFlags.unsafeOutsideOfFor && !is_for_loop_src))
    }

    def override preVisitExprDelete(expr : ExprDelete?) : void {
        if (expr.genFlags.generated) return
        if (expr.subexpr == null || expr.subexpr._type == null || !expr.subexpr._type.isSafeToDelete) {
            mark_unsafe_in_stack()
        }
    }

    def is_lowered_unsafe_delete(expr : ExpressionPtr) : bool =>
        expr is ExprCall && lowered_unsafe_delete(expr as ExprCall)

    def variable_owns_unsafe(v : VariablePtr) : bool {
        let vt = v._type
        return false if (v.flags.generated || vt == null)
        return ((!vt.flags.ref && (vt.hasClasses
                || (vt.isStructure && vt.structType != null && vt.structType.flags.isClass)))
            || (!vt.isLocal && !vt.isGoodBlockType)
            || ((vt.flags.smartPtr || (strict_smart_pointers && vt.needInScope)) && !v.flags.inScope)
            || (v.flags.inScope && vt.isPointer && vt.firstType != null
                && vt.firstType.isStructure && vt.firstType.structType != null
                && vt.firstType.structType.flags.isClass))
    }

    def declaration_owns_unsafe(expr : ExpressionPtr) : bool {
        return false if (!(expr is ExprLet))
        for (v in (expr as ExprLet).variables) {
            return true if (variable_owns_unsafe(v))
        }
        return false
    }

    //! `unsafe(delete p)` does not parse, so a block holding a delete anywhere in it has no narrow
    //! form to offer, whichever of its statements the one unsafe operation turns out to be.
    def block_holds_delete(blk : ExprBlock?) : bool {
        for (st in blk.list) {
            return true if (st is ExprDelete || is_lowered_unsafe_delete(st))
        }
        return false
    }

    //! Neither `unsafe(a <- b)` nor `unsafe(a := b)` parses, so a block holding a move or a clone
    //! anywhere in it has no narrow form to offer, whichever statement the one unsafe operation is.
    def block_holds_move_or_clone(blk : ExprBlock?) : bool {
        for (st in blk.list) {
            return true if (st is ExprMove || st is ExprClone || is_lowered_unsafe_clone(st))
        }
        return false
    }

    //! `unsafe(a := b)` does not parse, and a container clone reaches lint as a generated call
    //! rather than an ExprClone, so a block holding one has no narrow form to offer.
    def is_lowered_unsafe_clone(expr : ExpressionPtr) : bool {
        return false if (!(expr is ExprCall))
        let call = expr as ExprCall
        return false if (call.func == null)
        let fname = string(call.func.name)
        return (call.func.flags.generated && fname == "clone") || starts_with(fname, "builtin`clone`")
    }

    //! Infer lowers `delete` to an unflagged finalizer call, and only isSafeToDelete - which this
    //! side cannot read - knows whether the wrap was needed, so every lowered delete counts as
    //! needing it: a redundant wrap is missed rather than a needed one reported.
    def lowered_unsafe_delete(expr : ExprCall?) : bool {
        return false if (expr.func == null || length(expr.arguments) != 1)
        let fname = string(expr.func.name)
        let is_finalize = ((expr.func.flags.generated && fname == "finalize")
            || starts_with(fname, "builtin`finalize`") || starts_with(fname, "builtin`finalize_dim`"))
        return false if (!is_finalize)
        let at = expr.arguments[0]._type
        return at == null || !at.isSafeToDelete
    }

    def override preVisitExprAddr(expr : ExprAddr?) : void {
        st029_record_func(expr.func)
        if (!expr.genFlags.generated) mark_unsafe_in_stack()
    }
    def override preVisitExprCopy(expr : ExprCopy?) : void { st029_record_func(expr.func) }
    //! Mirrors infer's move rules: a class moves only under unsafe, and so does a value that needs
    //! inscope under strict_smart_pointers (a smart pointer moved by value when the option is off);
    //! an untyped operand counts too, so a needed wrap is never reported as redundant.
    def override preVisitExprMove(expr : ExprMove?) : void {
        st029_record_func(expr.func)
        return if (expr.genFlags.generated || expr.left == null || expr.right == null)
        let lt = expr.left._type
        let rt = expr.right._type
        if (lt == null || rt == null || lt.hasClasses) {
            mark_unsafe_in_stack()
        } elif (strict_smart_pointers ? rt.needInScope
                : (rt.isPointer && rt.flags.smartPtr && !rt.flags.ref && !(expr.right is ExprAscend))) {
            mark_unsafe_in_stack()
        }
    }
    def override preVisitExprClone(expr : ExprClone?) : void { st029_record_func(expr.func) }
    def override preVisitExprVar(expr : ExprVar?) : void {
        if (expr.variable != null) st029_record_mod(expr.variable._module)
        if (!empty(st041_candidates) && expr.variable != null && key_exists(st041_candidates, expr.variable)) {
            st041_candidates[expr.variable].observed_refs++
        }
    }
    def override preVisitExprConstEnumeration(expr : ExprConstEnumeration?) : void {
        if (expr.enumType != null) st029_record_mod(expr.enumType._module)
    }
    def override preVisitExprConstBitfield(expr : ExprConstBitfield?) : void {
        if (expr.bitfieldType != null && expr.bitfieldType.annotation != null) {
            st029_record_mod(expr.bitfieldType.annotation._module)
        }
    }
    def override preVisitExprRef2Ptr(expr : ExprRef2Ptr?) : void {
        if (!expr.genFlags.generated) mark_unsafe_in_stack()
    }
    def override preVisitExprAsVariant(expr : ExprAsVariant?) : void {
        if (!expr.genFlags.generated) mark_unsafe_in_stack()
    }
    def override preVisitExprSafeAsVariant(expr : ExprSafeAsVariant?) : void {
        if (!expr.genFlags.generated) mark_unsafe_in_stack()
    }

    def override preVisitExprCast(expr : ExprCast?) : void {
        if (!expr.genFlags.generated
                && (expr.castFlags.upcastCast || expr.castFlags.reinterpretCast)) {
            mark_unsafe_in_stack()
        }
        if (expr.castFlags.reinterpretCast && !expr.castFlags.fromAddrSugar
                && !expr.genFlags.generated
                && expr.castType != null && expr.castType.baseType == Type.tPointer
                && expr.subexpr != null && expr.subexpr is ExprRef2Ptr
                && !expr.subexpr.genFlags.generated) {
            style_warning("STYLE034: reinterpret<{describe(expr.castType)}>(addr(...)) collapses " +
                "to addr<{describe(expr.castType)}>(...); one unsafe() covers both halves", expr.at,
                st034_fix(expr))
        }
        if (!expr.genFlags.generated && expr.castType != null
                && (current_function == null || current_function.fromGeneric == null)) {
            let inert = style036_inert_contract(expr.castType)
            if (!empty(inert)) {
                let tail = inert == "-const" ? "; drop it, along with any 'const' it was meant to cancel" : "; drop it"
                style_warning("STYLE036: '{inert}' does nothing on the already-resolved cast " +
                    "type {describe(expr.castType)}{tail}", expr.at, st036_fix(expr, inert))
            }
        }
    }

    def style036_inert_contract(t : TypeDecl?) : string {
        if (t.isAutoOrAlias) return ""
        if (t.flags.removeConstant) return "-const"
        if (t.flags.removeRef) return "-&"
        if (t.flags.removeDim) return "-[]"
        if (t.flags.removeTemporary) return "-#"
        if (t.flags.explicitConst) return "==const"
        if (t.flags.explicitRef) return "==&"
        return ""
    }

    def override preVisitExprAt(expr : ExprAt?) : void {
        if (expr.genFlags.generated) return
        let unknown_subexpr_type = (expr.subexpr == null || expr.subexpr._type == null)
        let is_unsafe_index = (!unknown_subexpr_type
            && (expr.subexpr._type.baseType == Type.tTable
                || expr.subexpr._type.baseType == Type.tPointer))
        if (unknown_subexpr_type || is_unsafe_index) {
            mark_unsafe_in_stack()
        }
    }

    def override preVisitExprSafeAt(expr : ExprSafeAt?) : void {
        if (expr.genFlags.generated) return
        if (expr.subexpr == null || expr.subexpr._type == null) {
            mark_unsafe_in_stack()
            return
        }
        let bt = expr.subexpr._type.baseType
        if (bt == Type.tTable || bt == Type.tArray) {
            mark_unsafe_in_stack()
            return
        }
        if (bt == Type.tPointer) {
            let inner = expr.subexpr._type.firstType
            if (inner == null || !(inner.isVectorType || inner.baseType == Type.tFixedArray)) {
                mark_unsafe_in_stack()
            }
        }
    }

    def override preVisitExprField(expr : ExprField?) : void {
        if (!expr.genFlags.generated && expr.value != null
                && expr.value._type != null
                && expr.value._type.baseType == Type.tVariant) {
            mark_unsafe_in_stack()
        }
    }

    def override preVisitExprReturn(expr : ExprReturn?) : void {
        if (expr.genFlags.generated) return
        var has = expr.returnFlags.returnReference
        if (!has && expr.subexpr != null && expr.subexpr._type != null) {
            let t = expr.subexpr._type
            has = t.flags.ref || t.flags.temporary
        }
        if (!has && current_function != null && current_function.result != null) {
            let rt = current_function.result
            has = rt.flags.ref || rt.flags.temporary
        }
        if (!has && expr.returnFlags.moveSemantics && expr.subexpr != null) {
            has = (expr.subexpr._type == null
                || expr.subexpr._type.baseType == Type.autoinfer
                || expr.subexpr._type.baseType == Type.alias
                || expr.subexpr._type.flags.ref || expr.subexpr._type.flags.temporary)
        }
        if (has) mark_unsafe_in_stack()
    }

    def override preVisitExprOp1(expr : ExprOp1?) : void {
        st029_record_func(expr.func)
        if (!expr.genFlags.generated
                && call_func_needs_unsafe(expr.func, expr.flags.isForLoopSource)) {
            mark_unsafe_in_stack()
        }
    }
    def override preVisitExprOp3(expr : ExprOp3?) : void {
        st029_record_func(expr.func)
        if (!expr.genFlags.generated) st037_bump()
        if (!expr.genFlags.generated
                && call_func_needs_unsafe(expr.func, expr.flags.isForLoopSource)) {
            mark_unsafe_in_stack()
        }
    }
    def override preVisitExprNew(expr : ExprNew?) : void {
        st029_record_func(expr.func)
        if (!expr.genFlags.generated
                && (expr.func == null || expr.func.flags.unsafeOperation)) {
            mark_unsafe_in_stack()
        }
    }

    def override preVisitExprUnsafe(var expr : ExprUnsafe?) : void {
        let n = length(unsafe_block_stack)
        if (expr != null && !expr.genFlags.generated
            && (current_function == null || current_function.fromGeneric == null)
            && n > 0 && unsafe_block_stack[n - 1] > 0) {
            style_warning("STYLE026: nested 'unsafe \{ ... }' block; the outer 'unsafe' already " +
                "covers this scope - drop the inner wrap", expr.at, st024_unwrap_block_fix(expr))
        }
        if (n > 0) {
            unsafe_block_stack[n - 1]++
        }
    }

    def is_module_with(expr : ExpressionPtr) : bool =>
        expr is ExprWith && !empty((expr as ExprWith).moduleName)

    def override preVisitExprWith(var expr : ExprWith?) : void {
        if (with_module_is_unsafe && !expr.genFlags.generated && !empty(expr.moduleName)) {
            mark_unsafe_in_stack()
        }
    }

    def override visitExprUnsafe(var expr : ExprUnsafe?) : ExpressionPtr {
        let n = length(unsafe_block_stack)
        if (n > 0) {
            unsafe_block_stack[n - 1]--
        }
        if (expr == null || expr.body == null || expr.genFlags.generated
            || (current_function != null && current_function.fromGeneric != null)
            || !(expr.body is ExprBlock)) return expr
        let blk = expr.body as ExprBlock
        let frame = unsafe_frame_for(blk)
        if (frame.count == 0 && !frame.has_non_local_let_ref) {
            style_warning("STYLE024: redundant 'unsafe \{ ... }' block; no statement requires " +
                "unsafe - drop the wrap", expr.at, st024_unwrap_block_fix(expr))
        } elif (frame.count == 1 &&
                !(blk.list[0] is ExprYield ||
                  blk.list[0] is ExprDelete ||
                  blk.list[0] is ExprReturn ||
                  blk.list[0] is ExprNew ||
                  declaration_owns_unsafe(blk.list[0]) ||
                  is_module_with(blk.list[0]) ||
                  block_holds_delete(blk) ||
                  block_holds_move_or_clone(blk))
                && !frame.has_non_local_let_ref) {
            var narrowable = true
            if (blk.list[0] is ExprFor) {
                let fbody = (blk.list[0] as ExprFor).body
                if (fbody != null && fbody is ExprBlock) {
                    let fblk = fbody as ExprBlock
                    var all_deletes = !empty(fblk.list)
                    for (s in fblk.list) {
                        if (!(s is ExprDelete)) {
                            all_deletes = false
                        }
                    }
                    if (all_deletes) {
                        narrowable = false
                    }
                }
            }
            if (narrowable) {
                style_warning("STYLE025: 'unsafe \{ ... }' block scope is too broad; only one " +
                    "operation requires unsafe - narrow to 'unsafe(<sub-expr>)' wrapping that " +
                    "operation", expr.at,
                    n > 0 && unsafe_block_stack[n - 1] > 0 ? LintFix() : st025_fix(expr, frame.op))
            }
        }
        return expr
    }
}

def private require_analysis_enabled(disabled, enabled : table<string>) : bool {
    var any = false
    for (code in ["STYLE029", "STYLE030"]) {
        if (key_exists(disabled, code) || (!empty(enabled) && !key_exists(enabled, code))) continue
        any = true
    }
    return any
}

def private comment_hygiene_for(prog : ProgramPtr; requested : bool) : bool {
    //! Explicit callers (e.g. --comment-hygiene) force the checks on. Otherwise a module-local option overrides the path default, and daslib/ defaults on.
    if (requested) return true
    let configured = prog._options |> find_arg("_comment_hygiene")
    if (configured is tBool) return configured as tBool
    return is_daslib_source(prog)
}

def private ascii_strings_for(prog : ProgramPtr) : bool {
    let configured = prog._options |> find_arg("_ascii_strings")
    if (configured is tBool) return configured as tBool
    if (lint_config_forces_on("STYLE039")) return true
    return is_shipped_library_source(prog)
}

//! infer falls back to the host's policy, which lint cannot read; the policy header's default is on.
def private strict_smart_pointers_for(prog : ProgramPtr) : bool =>
    prog._options |> find_arg("strict_smart_pointers") ?as tBool ?? true

def private duplicate_regions_for(prog : ProgramPtr) : bool {
    let configured = prog._options |> find_arg("_duplicate_regions")
    if (configured is tBool) return configured as tBool
    if (lint_config_forces_on("STYLE040")) return true
    return is_core_library_source(prog)
}

def private max_complexity_for(prog : ProgramPtr) : int {
    let configured = prog._options |> find_arg("_cyclomatic_complexity") ?as tInt ?? STYLE037_DEFAULT_MAX
    return max(configured, 0)
}

def private max_function_length_for(prog : ProgramPtr) : int {
    let configured = prog._options |> find_arg("_function_length") ?as tInt ?? STYLE038_DEFAULT_MAX_LINES
    return max(configured, 0)
}

def public style_lint(prog : ProgramPtr; compile_time_errors : bool; comment_hygiene : bool = false) : int {
    //! Runs the style lint visitor on the compiled program.
    //! Returns the number of warnings found.
    //! STYLE014/STYLE015 default on for daslib sources; pass ``comment_hygiene = true`` to force them on elsewhere.
    let empty_set : table<string>
    return style_lint(prog, compile_time_errors, empty_set, [comment_hygiene = comment_hygiene])
}

def public style_lint(prog : ProgramPtr; compile_time_errors : bool; disabled_codes : table<string>; comment_hygiene : bool = false) : int {
    //! Filter-aware compile-time variant. ``disabled_codes`` suppresses
    //! the listed rule IDs (e.g. "STYLE005") alongside the usual
    //! ``// nolint:CODE`` directives.
    let hygiene = comment_hygiene_for(prog, comment_hygiene)
    var astVisitor = new StyleLintVisitor(compile_time_errors = compile_time_errors, comment_hygiene = hygiene)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.max_complexity = max_complexity_for(prog)
    astVisitor.max_function_length = max_function_length_for(prog)
    astVisitor.ascii_strings = ascii_strings_for(prog)
    astVisitor.strict_smart_pointers = strict_smart_pointers_for(prog)
    astVisitor.with_module_is_unsafe = (prog._options |> find_arg("with_module_is_unsafe")) ?as tBool ?? false
    astVisitor.this_module = prog.getThisModule
    astVisitor.st048_sees_match = st048_requires_match(prog, astVisitor.this_module)
    let empty_enabled : table<string>
    astVisitor.analyze_requires = astVisitor.this_module != null && require_analysis_enabled(disabled_codes, empty_enabled)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_with_generics(prog, astVisitorAdapter)
    }
    if (hygiene) {
        astVisitor->scan_long_comment_blocks(prog)
    }
    astVisitor->st029_check_requires(prog)
    astVisitor->st040_check_duplicates(prog)
    astVisitor->st047_report(prog)
    if (!empty(astVisitor.unsafe_stack)) {
        panic("style_lint internal: unsafe_stack not balanced after walk (size={length(astVisitor.unsafe_stack)})")
    }
    let count = astVisitor.warning_count
    unsafe { delete astVisitor; }
    return count
}

def public style_lint_collect(prog : ProgramPtr; var warnings : array<string>; comment_hygiene : bool = false) : int {
    //! Runs the style lint visitor and collects warnings as strings.
    //! Returns the number of warnings found.
    //! STYLE014/STYLE015 default on for daslib sources; pass ``comment_hygiene = true`` to force them on elsewhere.
    let empty_set : table<string>
    return style_lint_collect(prog, warnings, empty_set, empty_set, comment_hygiene)
}

def public style_lint_collect(prog : ProgramPtr; var warnings : array<string>;
                              disabled_codes, enabled_codes : table<string>;
                              comment_hygiene : bool = false) : int {
    //! Filter-aware overload. `disabled_codes` is a denylist; `enabled_codes`
    //! is a whitelist (empty == all). Codes use the bare form (e.g. "STYLE024").
    let hygiene = comment_hygiene_for(prog, comment_hygiene)
    var astVisitor = new StyleLintVisitor(compile_time_errors = false, collect_warnings = true, comment_hygiene = hygiene)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    astVisitor.max_complexity = max_complexity_for(prog)
    astVisitor.max_function_length = max_function_length_for(prog)
    astVisitor.ascii_strings = ascii_strings_for(prog)
    astVisitor.strict_smart_pointers = strict_smart_pointers_for(prog)
    astVisitor.with_module_is_unsafe = (prog._options |> find_arg("with_module_is_unsafe")) ?as tBool ?? false
    astVisitor.this_module = prog.getThisModule
    astVisitor.st048_sees_match = st048_requires_match(prog, astVisitor.this_module)
    astVisitor.analyze_requires = astVisitor.this_module != null && require_analysis_enabled(disabled_codes, enabled_codes)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_with_generics(prog, astVisitorAdapter)
    }
    if (hygiene) {
        astVisitor->scan_long_comment_blocks(prog)
    }
    astVisitor->st029_check_requires(prog)
    astVisitor->st040_check_duplicates(prog)
    astVisitor->st047_report(prog)
    if (!empty(astVisitor.unsafe_stack)) {
        panic("style_lint internal: unsafe_stack not balanced after walk (size={length(astVisitor.unsafe_stack)})")
    }
    let count = astVisitor.warning_count
    warnings |> push_from(astVisitor.warnings)
    unsafe { delete astVisitor; }
    return count
}

def public style_lint_collect_issues(prog : ProgramPtr; var issues : array<LintIssue>;
                                     disabled_codes, enabled_codes : table<string>;
                                     comment_hygiene : bool = false) : int {
    //! Structured twin of ``style_lint_collect`` — same filters, but emits
    //! ``LintIssue`` records (rule code + position) instead of display strings.
    let hygiene = comment_hygiene_for(prog, comment_hygiene)
    var astVisitor = new StyleLintVisitor(compile_time_errors = false, collect_warnings = true, comment_hygiene = hygiene)
    astVisitor.disabled_codes := disabled_codes
    astVisitor.enabled_codes := enabled_codes
    astVisitor.max_complexity = max_complexity_for(prog)
    astVisitor.max_function_length = max_function_length_for(prog)
    astVisitor.ascii_strings = ascii_strings_for(prog)
    astVisitor.strict_smart_pointers = strict_smart_pointers_for(prog)
    astVisitor.with_module_is_unsafe = (prog._options |> find_arg("with_module_is_unsafe")) ?as tBool ?? false
    astVisitor.this_module = prog.getThisModule
    astVisitor.st048_sees_match = st048_requires_match(prog, astVisitor.this_module)
    astVisitor.analyze_requires = astVisitor.this_module != null && require_analysis_enabled(disabled_codes, enabled_codes)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_with_generics(prog, astVisitorAdapter)
    }
    if (hygiene) {
        astVisitor->scan_long_comment_blocks(prog)
    }
    astVisitor->st029_check_requires(prog)
    astVisitor->st040_check_duplicates(prog)
    astVisitor->st047_report(prog)
    if (!empty(astVisitor.unsafe_stack)) {
        panic("style_lint internal: unsafe_stack not balanced after walk (size={length(astVisitor.unsafe_stack)})")
    }
    let count = astVisitor.warning_count
    issues |> push_from(astVisitor.issues)
    unsafe { delete astVisitor; }
    return count
}

[lint_macro]
class StyleLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let hygiene = prog._options |> find_arg("_comment_hygiene") ?as tBool ?? false
        let disabled <- build_lint_macro_disabled(prog)
        style_lint(prog, true, disabled, [comment_hygiene = hygiene])
        return true
    }
}
