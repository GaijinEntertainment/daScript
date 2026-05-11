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
// Rules supported:
//   STYLE005  if (cond) { <terminator> }   ->  if (cond) <terminator>
//   STYLE018  b == true / b != false       ->  b ; b == false / b != true -> !b
//   PERF013   a += 1 / a -= 1              ->  a++ / a--
//   PERF017   length(x) == 0 / != 0        ->  empty(x) / !empty(x)
//
// Future rules (STYLE017, PERF014, PERF015, PERF016) layer on the same Edit
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
        // STYLE017(b): adjacent `if (cond) return b1` / `return b2` (b1 != b2).
        self->check_style017b(blk)
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

    def extract_source(at : LineInfo) : string {
        //! Extracts source bytes covered by `at`. Multi-line aware. Columns
        //! 0-based, last_column exclusive.
        return "" if (at.fileInfo == null || at.line == 0u)
        let start_line = int(at.line)
        let end_line = int(at.last_line)
        let s_col = int(at.column)
        let e_col = int(at.last_column)
        var result = ""
        if (start_line == end_line) {
            get_file_source_line(at.fileInfo, at.line) $(line) {
                let lo = max(0, s_col)
                let hi = min(length(line), e_col)
                if (hi > lo) {
                    result := slice(line, lo, hi)
                }
            }
            return result
        }
        for (ln in range(start_line, end_line + 1)) {
            get_file_source_line(at.fileInfo, uint(ln)) $(line) {
                if (ln == start_line) {
                    let lo = max(0, s_col)
                    if (lo < length(line)) {
                        result := slice(line, lo)
                    }
                } elif (ln == end_line) {
                    let hi = min(length(line), e_col)
                    if (hi > 0) {
                        result := "{result}\n{slice(line, 0, hi)}"
                    }
                } else {
                    result := "{result}\n{line}"
                }
            }
        }
        return result
    }

    def peel_ref2value(e : Expression? const) : Expression? const {
        //! Peels a single ExprRef2Value wrapper (typer artifact). Safe — does not
        //! peel ExprPtr2Ref (user-visible `*foo` deref).
        if (e != null && e is ExprRef2Value) {
            return (e as ExprRef2Value).subexpr
        }
        return e
    }

    def is_length_call(e : Expression? const) : bool {
        //! True for length(string) / length(das_string) / length(array) /
        //! length(table). Excludes math::length(float2/3/4).
        return false if (e == null || !(e is ExprCall))
        let call = e as ExprCall
        return false if (call.func == null || call.func.name != "length")
        return false if (call.func._module == null)
        let modname = string(call.func._module.name)
        return modname == "strings" || modname == "$" || modname == "builtin"
    }

    def source_span_of(e : Expression? const;
                       var sl : int&; var sc : int&;
                       var el : int&; var ec : int&) : void {
        //! Returns the source bounds for an expression. For ExprCall, this is
        //! `atEnclosure` (covers `name(args)`); for everything else, `at`. Note
        //! that the typer may have canonicalized operand order so the AST left
        //! is not necessarily the source left — callers must take min/max
        //! across operands.
        if (e == null) {
            sl = 0; sc = 0; el = 0; ec = 0
            return
        }
        if (e is ExprCall) {
            let c = e as ExprCall
            sl = int(c.atEnclosure.line)
            sc = int(c.atEnclosure.column)
            el = int(c.atEnclosure.last_line)
            ec = int(c.atEnclosure.last_column)
            return
        }
        sl = int(e.at.line)
        sc = int(e.at.column)
        el = int(e.at.last_line)
        ec = int(e.at.last_column)
    }

    def rightmost_pos_of(e : Expression? const; var hi_line : int&; var hi_col : int&) : void {
        //! Recursive rightmost source position. ExprField.at covers only the
        //! `.` separator (the trailing name is implicit), ExprAt.at covers only
        //! `[` (the closing `]` follows index). Heuristic — assumes no whitespace
        //! around `.` or before `]`.
        if (e == null) {
            hi_line = 0; hi_col = 0
            return
        }
        if (e is ExprField) {
            let fld = e as ExprField
            hi_line = int(fld.at.last_line)
            hi_col = int(fld.at.last_column) + length(string(fld.name))
            return
        }
        if (e is ExprSafeField) {
            let fld = e as ExprSafeField
            hi_line = int(fld.at.last_line)
            hi_col = int(fld.at.last_column) + length(string(fld.name))
            return
        }
        if (e is ExprAt) {
            let ea = e as ExprAt
            if (ea.index != null) {
                hi_line = int(ea.index.at.last_line)
                hi_col = int(ea.index.at.last_column) + 1
                return
            }
        }
        if (e is ExprSafeAt) {
            let ea = e as ExprSafeAt
            if (ea.index != null) {
                hi_line = int(ea.index.at.last_line)
                hi_col = int(ea.index.at.last_column) + 1
                return
            }
        }
        if (e is ExprCall) {
            let c = e as ExprCall
            hi_line = int(c.atEnclosure.last_line)
            hi_col = int(c.atEnclosure.last_column)
            return
        }
        hi_line = int(e.at.last_line)
        hi_col = int(e.at.last_column)
    }

    def is_simple_subject(e : Expression? const) : bool {
        //! True if `e` has a clean, known source span via leftmost_pos_of /
        //! rightmost_pos_of. Compound operator expressions and parenthesized
        //! forms don't qualify — STYLE018 / similar rules bail on them.
        return false if (e == null)
        return (e is ExprVar || e is ExprField || e is ExprSafeField
                || e is ExprAt || e is ExprSafeAt || e is ExprCall)
    }

    def leftmost_pos_of(e : Expression? const; var lo_line : int&; var lo_col : int&) : void {
        //! Recursive leftmost source position. ExprField.at points at the `.`
        //! separator and ExprAt.at at the `[`, so the user-source start lives
        //! in the chain root.
        if (e == null) {
            lo_line = 0; lo_col = 0
            return
        }
        if (e is ExprField) {
            self->leftmost_pos_of((e as ExprField).value, lo_line, lo_col)
            return
        }
        if (e is ExprSafeField) {
            self->leftmost_pos_of((e as ExprSafeField).value, lo_line, lo_col)
            return
        }
        if (e is ExprAt) {
            self->leftmost_pos_of((e as ExprAt).subexpr, lo_line, lo_col)
            return
        }
        if (e is ExprSafeAt) {
            self->leftmost_pos_of((e as ExprSafeAt).subexpr, lo_line, lo_col)
            return
        }
        lo_line = int(e.at.line)
        lo_col = int(e.at.column)
    }

    def const_int_value(e : Expression? const; var v : int&) : bool {
        let inner = self->peel_ref2value(e)
        return false if (inner == null)
        if (inner is ExprConstInt) {
            v = (inner as ExprConstInt).value
            return true
        }
        if (inner is ExprConstUInt) {
            v = int((inner as ExprConstUInt).value)
            return true
        }
        return false
    }

    def is_const_pm_one(e : Expression? const; var sign_out : int&) : bool {
        //! True if `e` is the integer/float constant +1 or -1. Sign written to
        //! `sign_out`. Mirrors PERF013's `is_const_one` + `is_const_neg_one`.
        let inner = self->peel_ref2value(e)
        return false if (inner == null)
        if (inner is ExprConstInt) {
            let v = (inner as ExprConstInt).value
            if (v == 1) {
                sign_out = 1
                return true
            }
            if (v == -1) {
                sign_out = -1
                return true
            }
            return false
        }
        if (inner is ExprConstUInt) {
            if ((inner as ExprConstUInt).value == 1u) {
                sign_out = 1
                return true
            }
            return false
        }
        if (inner is ExprConstInt64) {
            let v = (inner as ExprConstInt64).value
            if (v == 1l) {
                sign_out = 1
                return true
            }
            if (v == -1l) {
                sign_out = -1
                return true
            }
            return false
        }
        if (inner is ExprConstUInt64) {
            if ((inner as ExprConstUInt64).value == 1ul) {
                sign_out = 1
                return true
            }
            return false
        }
        if (inner is ExprConstFloat) {
            let v = (inner as ExprConstFloat).value
            if (v == 1.0f) {
                sign_out = 1
                return true
            }
            if (v == -1.0f) {
                sign_out = -1
                return true
            }
            return false
        }
        if (inner is ExprConstDouble) {
            let v = (inner as ExprConstDouble).value
            if (v == 1.0lf) {
                sign_out = 1
                return true
            }
            if (v == -1.0lf) {
                sign_out = -1
                return true
            }
            return false
        }
        return false
    }

    def is_workhorse_numeric(t : TypeDecl? const) : bool {
        //! Six numeric workhorse scalars (int, uint, int64, uint64, float, double).
        //! Mirrors perf_lint's gate so the fixer matches what the lint flags.
        return false if (t == null || !empty(t.dim))
        let bt = t.baseType
        return (bt == Type.tInt || bt == Type.tUInt
                || bt == Type.tInt64 || bt == Type.tUInt64
                || bt == Type.tFloat || bt == Type.tDouble)
    }

    def push_edit(at : LineInfo; replacement : string; rule : string) : void {
        self->push_edit_span(int(at.line), int(at.column),
                             int(at.last_line), int(at.last_column),
                             replacement, rule)
    }

    def push_edit_span(start_line : int; start_col : int;
                       end_line : int; end_col : int;
                       replacement : string; rule : string) : void {
        var e : SourceEdit
        e.start_line = start_line
        e.start_col = start_col
        e.end_line = end_line
        e.end_col = end_col
        e.rule = rule
        edits |> push(e)
        replacements |> push(replacement)
    }

    // --- STYLE005 / STYLE017: ExprIfThenElse ---

    def override preVisitExprIfThenElse(ifte : ExprIfThenElse?) : void {
        return if (in_template || in_closure > 0)
        // STYLE017(a): if (cond) return true else return false  →  return cond
        self->check_style017a(ifte)
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

    // --- PERF013 / PERF017 / STYLE018 ---

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        return if (in_template || in_closure > 0)
        let opname = string(expr.op)
        // PERF013: a += ±1 / a -= ±1 → a++ / a--
        if (opname == "+=" || opname == "-=") {
            self->check_perf013(expr, opname)
            return
        }
        // STYLE018: b == true / != false → b ; b == false / != true → !b
        if (opname == "==" || opname == "!=") {
            self->check_style018(expr, opname)
        }
        return if (opname != "==" && opname != "!=" && opname != "<"
                && opname != "<=" && opname != ">" && opname != ">=")
        let lhs = self->peel_ref2value(expr.left)
        let rhs = self->peel_ref2value(expr.right)
        var len_on_left = true
        if (self->is_length_call(lhs)) {
            len_on_left = true
        } elif (self->is_length_call(rhs)) {
            len_on_left = false
        } else {
            return
        }
        let len_call = len_on_left ? (lhs as ExprCall) : (rhs as ExprCall)
        let const_side = len_on_left ? rhs : lhs
        var k = 0
        return if (!self->const_int_value(const_side, k) || (k != 0 && k != 1))
        // Canonicalize so length is on the left; flip op if needed.
        var canon_op = opname
        if (!len_on_left) {
            if (opname == "<") {
                canon_op = ">"
            } elif (opname == "<=") {
                canon_op = ">="
            } elif (opname == ">") {
                canon_op = "<"
            } elif (opname == ">=") {
                canon_op = "<="
            }
        }
        var is_empty_pattern = false
        var is_nonempty_pattern = false
        if (k == 0) {
            if (canon_op == "==" || canon_op == "<=") {
                is_empty_pattern = true
            } elif (canon_op == "!=" || canon_op == ">") {
                is_nonempty_pattern = true
            }
        } else {
            if (canon_op == "<") {
                is_empty_pattern = true
            } elif (canon_op == ">=") {
                is_nonempty_pattern = true
            }
        }
        return if (!is_empty_pattern && !is_nonempty_pattern)
        // Some C++-bound `length` overloads (e.g. strings::length) have a hidden
        // `context` arg; we only need arg[0], the user-passed collection.
        return if (empty(len_call.arguments))
        let arg_at = len_call.arguments[0].at
        let arg_text = self->extract_source(arg_at)
        return if (empty(arg_text))
        let replacement = is_empty_pattern ? "empty({arg_text})" : "!empty({arg_text})"
        // ExprOp2.at covers only the operator token, not the full expression.
        // Worse, the typer may canonicalize Yoda comparisons so the AST left is
        // not the source left. Compute the actual source bounds via min/max of
        // each operand's source span (which for calls means atEnclosure).
        var l_sl = 0; var l_sc = 0; var l_el = 0; var l_ec = 0
        var r_sl = 0; var r_sc = 0; var r_el = 0; var r_ec = 0
        self->source_span_of(lhs, l_sl, l_sc, l_el, l_ec)
        self->source_span_of(rhs, r_sl, r_sc, r_el, r_ec)
        var start_line = l_sl
        var start_col = l_sc
        if (r_sl < start_line || (r_sl == start_line && r_sc < start_col)) {
            start_line = r_sl
            start_col = r_sc
        }
        var end_line = l_el
        var end_col = l_ec
        if (r_el > end_line || (r_el == end_line && r_ec > end_col)) {
            end_line = r_el
            end_col = r_ec
        }
        self->push_edit_span(start_line, start_col, end_line, end_col, replacement, "PERF017")
    }

    def check_perf013(expr : ExprOp2?; opname : string) : void {
        //! a += 1 / a -= 1 / a += -1 / a -= -1 → a++ / a--
        //! Net delta: '+=' with +N has sign of N; '-=' flips the sign.
        return if (expr.left == null || !self->is_workhorse_numeric(expr.left._type))
        var rhs_sign = 0
        return if (!self->is_const_pm_one(expr.right, rhs_sign))
        let delta = opname == "+=" ? rhs_sign : -rhs_sign
        return if (delta != 1 && delta != -1)
        // Find the leftmost source position of the LHS (which may be a field
        // chain like `s.count` or an index `arr[i]` whose `.at` is the inner
        // separator, not the root). The LHS source extends from there to the
        // start of the operator (expr.at.column).
        var lhs_lo_line = 0
        var lhs_lo_col = 0
        self->leftmost_pos_of(expr.left, lhs_lo_line, lhs_lo_col)
        return if (lhs_lo_line == 0)
        let op_line = int(expr.at.line)
        let op_col = int(expr.at.column)
        let lhs_text = strip(self->extract_source_at(
            lhs_lo_line, lhs_lo_col, op_line, op_col, expr.at.fileInfo))
        return if (empty(lhs_text))
        let suffix = delta == 1 ? "++" : "--"
        // Replacement spans from LHS leftmost to RHS last_column (the const).
        let r_end_line = int(expr.right.at.last_line)
        let r_end_col = int(expr.right.at.last_column)
        self->push_edit_span(lhs_lo_line, lhs_lo_col,
                             r_end_line, r_end_col,
                             "{lhs_text}{suffix}", "PERF013")
    }

    def is_const_bool_return(e : Expression? const; var bv : bool&) : bool {
        //! True if `e` is `return <ConstBool>` (or 1-stmt block wrapping one).
        //! Writes the bool value to `bv`.
        return false if (e == null)
        if (e is ExprBlock) {
            let blk = e as ExprBlock
            return false if (length(blk.list) != 1 || !empty(blk.finalList))
            return self->is_const_bool_return(blk.list[0], bv)
        }
        return false if (!(e is ExprReturn))
        let ret = e as ExprReturn
        return false if (ret.subexpr == null)
        let sub = self->peel_ref2value(ret.subexpr)
        return false if (sub == null || !(sub is ExprConstBool))
        bv = (sub as ExprConstBool).value
        return true
    }

    def check_style017b(blk : ExprBlock?) : void {
        //! Form (b): adjacent `if (cond) return b1` / `return b2` (b1 != b2) →
        //! `return cond` or `return !cond`. Spans both statements.
        return if (in_template || in_closure > 0)
        let n = length(blk.list)
        return if (n < 2)
        for (i in range(n - 1)) {
            let cur = blk.list[i]
            let nxt = blk.list[i + 1]
            continue if (!(cur is ExprIfThenElse))
            let outer_if = cur as ExprIfThenElse
            continue if (outer_if.if_false != null || outer_if.if_flags.isStatic)
            var b1 = false
            var b2 = false
            continue if (!self->is_const_bool_return(outer_if.if_true, b1))
            continue if (!self->is_const_bool_return(nxt, b2))
            continue if (b1 == b2)
            continue if (outer_if.cond == null)
            let cond = self->peel_ref2value(outer_if.cond)
            continue if (cond == null)
            var c_lo_l = 0; var c_lo_c = 0
            var c_hi_l = 0; var c_hi_c = 0
            self->leftmost_pos_of(cond, c_lo_l, c_lo_c)
            self->rightmost_pos_of(cond, c_hi_l, c_hi_c)
            if (cond is ExprOp2) {
                let op = cond as ExprOp2
                continue if (op.left == null || op.right == null)
                self->leftmost_pos_of(op.left, c_lo_l, c_lo_c)
                self->rightmost_pos_of(op.right, c_hi_l, c_hi_c)
            }
            continue if (c_lo_l == 0 || c_hi_l == 0)
            let cond_text = self->extract_source_at(c_lo_l, c_lo_c, c_hi_l, c_hi_c, outer_if.at.fileInfo)
            continue if (empty(cond_text))
            let replacement = (b1
                ? "return {cond_text}"
                : (cond is ExprOp2 ? "return !({cond_text})" : "return !{cond_text}"))
            // Span: outer_if.at start to nxt.at end. nxt is ExprReturn whose
            // .at covers `return`; the source end is past the const bool.
            let start_line = int(outer_if.at.line)
            let start_col = int(outer_if.at.column)
            let end_line = int(nxt.at.last_line)
            var end_col = int(nxt.at.last_column)
            // Walk into the return to extend end past the const bool.
            if (nxt is ExprReturn) {
                let ret = nxt as ExprReturn
                if (ret.subexpr != null) {
                    let sub = self->peel_ref2value(ret.subexpr)
                    if (sub != null && sub is ExprConstBool) {
                        end_col = int(sub.at.last_column)
                    }
                }
            }
            self->push_edit_span(start_line, start_col, end_line, end_col, replacement, "STYLE017")
        }
    }

    def check_style017a(ifte : ExprIfThenElse?) : void {
        //! Form (a): `if (cond) return b1 else return b2` (b1 != b2) →
        //! `return cond` or `return !cond`.
        return if (in_template || in_closure > 0 || ifte.if_false == null)
        var b1 = false
        var b2 = false
        return if (!self->is_const_bool_return(ifte.if_true, b1))
        return if (!self->is_const_bool_return(ifte.if_false, b2))
        return if (b1 == b2)
        // Extract `cond` source. Bail if non-simple.
        return if (ifte.cond == null)
        let cond = self->peel_ref2value(ifte.cond)
        return if (cond == null)
        // For the `cond` source, find its leftmost/rightmost bounds.
        var c_lo_l = 0; var c_lo_c = 0
        var c_hi_l = 0; var c_hi_c = 0
        self->leftmost_pos_of(cond, c_lo_l, c_lo_c)
        self->rightmost_pos_of(cond, c_hi_l, c_hi_c)
        // `cond` may be a compound ExprOp2 (e.g. `x > 0`). Its `.at` covers only
        // the operator, so use leftmost/rightmost of operands. Skip the rule if
        // we can't reliably bound the cond — leave for hand-fix.
        if (cond is ExprOp2) {
            let op = cond as ExprOp2
            return if (op.left == null || op.right == null)
            self->leftmost_pos_of(op.left, c_lo_l, c_lo_c)
            self->rightmost_pos_of(op.right, c_hi_l, c_hi_c)
        }
        return if (c_lo_l == 0 || c_hi_l == 0)
        let cond_text = self->extract_source_at(c_lo_l, c_lo_c, c_hi_l, c_hi_c, ifte.at.fileInfo)
        return if (empty(cond_text))
        let replacement = (b1
            ? "return {cond_text}"
            : (cond is ExprOp2 ? "return !({cond_text})" : "return !{cond_text}"))
        // Span: from ifte.at start to the end of if_false (entire if/else
        // statement). if_false is an ExprBlock (braces) or bare ExprReturn.
        let start_line = int(ifte.at.line)
        let start_col = int(ifte.at.column)
        var end_line = int(ifte.if_false.at.last_line)
        var end_col = int(ifte.if_false.at.last_column)
        self->push_edit_span(start_line, start_col, end_line, end_col, replacement, "STYLE017")
    }

    def check_style018(expr : ExprOp2?; opname : string) : void {
        //! `<bool_expr> == true/false` / `<bool_expr> != true/false` → drop the
        //! literal. Only fires when the non-bool side is a simple subject (var /
        //! field / index / call) — compound or parenthesized expressions are
        //! left for hand-cleanup since their source span isn't reliable.
        let lhs = self->peel_ref2value(expr.left)
        let rhs = self->peel_ref2value(expr.right)
        let lbool = lhs != null && lhs is ExprConstBool
        let rbool = rhs != null && rhs is ExprConstBool
        return if (lbool == rbool)
        let const_side = lbool ? lhs : rhs
        let other_side = lbool ? rhs : lhs
        return if (!self->is_simple_subject(other_side))
        let cval = (const_side as ExprConstBool).value
        let positive = (opname == "==" && cval) || (opname == "!=" && !cval)
        // Source span of the non-bool subject (leftmost ... rightmost).
        var sub_lo_l = 0; var sub_lo_c = 0
        var sub_hi_l = 0; var sub_hi_c = 0
        self->leftmost_pos_of(other_side, sub_lo_l, sub_lo_c)
        self->rightmost_pos_of(other_side, sub_hi_l, sub_hi_c)
        return if (sub_lo_l == 0 || sub_hi_l == 0)
        let other_text = self->extract_source_at(sub_lo_l, sub_lo_c, sub_hi_l, sub_hi_c, expr.at.fileInfo)
        return if (empty(other_text))
        let replacement = (positive
            ? other_text
            : (self->needs_parens_for_negation(other_side)
                ? "!({other_text})"
                : "!{other_text}"))
        // Replacement span: subject's full extent vs the bool literal's. The
        // typer may have swapped operands, so take min/max of each side's
        // (leftmost, rightmost).
        var c_lo_l = 0; var c_lo_c = 0
        var c_hi_l = 0; var c_hi_c = 0
        self->leftmost_pos_of(const_side, c_lo_l, c_lo_c)
        self->rightmost_pos_of(const_side, c_hi_l, c_hi_c)
        var start_line = sub_lo_l
        var start_col = sub_lo_c
        if (c_lo_l < start_line || (c_lo_l == start_line && c_lo_c < start_col)) {
            start_line = c_lo_l
            start_col = c_lo_c
        }
        var end_line = sub_hi_l
        var end_col = sub_hi_c
        if (c_hi_l > end_line || (c_hi_l == end_line && c_hi_c > end_col)) {
            end_line = c_hi_l
            end_col = c_hi_c
        }
        self->push_edit_span(start_line, start_col, end_line, end_col, replacement, "STYLE018")
    }

    def needs_parens_for_negation(e : Expression? const) : bool {
        //! Determines whether `!<src(e)>` could re-parse with the wrong precedence.
        //! Safe atoms (no parens): vars, fields, indices, calls, parenthesized exprs,
        //! existing unary ops, constants. Operator-level expressions need parens.
        return false if (e == null)
        if (e is ExprVar || e is ExprField || e is ExprSafeField
                || e is ExprAt || e is ExprSafeAt || e is ExprCall
                || e is ExprConstBool || e is ExprConstInt || e is ExprConstUInt
                || e is ExprConstFloat || e is ExprConstDouble || e is ExprConstString
                || e is ExprOp1) {
            return false
        }
        return true
    }

    def extract_source_at(sl : int; sc : int; el : int; ec : int;
                          fi : FileInfo? const) : string {
        //! Like extract_source(LineInfo) but takes ints — for spans we build by
        //! hand from operand source bounds.
        return "" if (fi == null || sl == 0)
        var result = ""
        if (sl == el) {
            get_file_source_line(fi, uint(sl)) $(line) {
                let lo = max(0, sc)
                let hi = min(length(line), ec)
                if (hi > lo) {
                    result := slice(line, lo, hi)
                }
            }
            return result
        }
        for (ln in range(sl, el + 1)) {
            get_file_source_line(fi, uint(ln)) $(line) {
                if (ln == sl) {
                    let lo = max(0, sc)
                    if (lo < length(line)) {
                        result := slice(line, lo)
                    }
                } elif (ln == el) {
                    let hi = min(length(line), ec)
                    if (hi > 0) {
                        result := "{result}\n{slice(line, 0, hi)}"
                    }
                } else {
                    result := "{result}\n{line}"
                }
            }
        }
        return result
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

def edits_overlap(a : EditRecord; b : EditRecord) : bool {
    //! True if [a.start, a.end) and [b.start, b.end) share any column.
    // a ends before b starts → no overlap
    let a_before_b = (a.end_line < b.start_line
        || (a.end_line == b.start_line && a.end_col <= b.start_col))
    let b_before_a = (b.end_line < a.start_line
        || (b.end_line == a.start_line && b.end_col <= a.start_col))
    return !(a_before_b || b_before_a)
}

def apply_edits_to_file(file : string; var records : array<EditRecord>; dry_run : bool) : int {
    //! Applies all edits to a file's source, returning the number written.
    //! Sorts edits by descending end position so earlier-position edits stay
    //! valid through later applications. Drops edits subsumed by a larger one
    //! (e.g. STYLE005 inside STYLE017(b)) — outer wins.
    return 0 if (empty(records))
    let text = fread(file)
    return 0 if (empty(text))
    var lines <- text |> split("\n")
    // Sort by (end_line desc, end_col desc) — apply from end of file backwards.
    // Daslang has `sort` from daslib/sort_boost; here we just bubble for small N.
    let n0 = length(records)
    for (i in range(n0)) {
        for (j in range(i + 1, n0)) {
            let a = records[i]
            let b = records[j]
            let a_after_b = (a.end_line > b.end_line
                || (a.end_line == b.end_line && a.end_col > b.end_col))
            if (!a_after_b) {
                var tmp <- records[i]
                records[i] <- records[j]
                records[j] <- tmp
            }
        }
    }
    // Drop subsumed/overlapping edits: walk in order (now end-desc) and keep an
    // edit only if it doesn't overlap any earlier (larger-end) kept edit.
    var keep : array<EditRecord>
    for (r in records) {
        var skip = false
        for (k in keep) {
            if (edits_overlap(k, r)) {
                skip = true
                break
            }
        }
        if (!skip) {
            var clone <- r
            keep |> push(<- clone)
        }
    }
    records |> clear
    for (r in keep) {
        var clone <- r
        records |> push(<- clone)
    }
    let n = length(records)
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
