options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
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
//!   STYLE013 — struct var with default/empty init followed by a run of field assignments
//!   STYLE014 — comment block exceeds 3 lines at module/public scope (opt-in via ``options _comment_hygiene = true``, suppress with '//!@nolint' or '// nolint:STYLE014')
//!   STYLE015 — comment block exceeds 1 line inside a 'def private' (opt-in via ``options _comment_hygiene = true``, suppress with '// nolint:STYLE015')
//!   STYLE016 — adjacent guards leading to identical early-exit can be combined with '||'
//!   STYLE017 — 'if (cond) return true; else return false' — use 'return cond' (or 'return !cond')
//!   STYLE018 — redundant boolean comparison ('b == true' / 'b != false') — use 'b' / '!b'
//!   STYLE019 — nested min/max — use 'clamp(x, lo, hi)' from math module
//!   STYLE020 — from_JV(v, type<T>, defV) on a scalar — use 'v ?? defV' (operator ?? from json_boost)
//!   STYLE021 — table<string; JsonValue?> built by repeated insert — use 'var v = JV((k1=..., k2=...))'
//!   STYLE022 — bitfield compound-bit-set/clear ('foo |= BfT.m' / 'foo &= ~BfT.m') — use field-assign 'foo.m = true' / 'foo.m = false'

require daslib/ast_boost
require strings

// ---------------------------------------------------------------------------
// Visitor
// ---------------------------------------------------------------------------

class StyleLintVisitor : AstVisitor {
    compile_time_errors : bool
    comment_hygiene : bool = false
    // counter
    warning_count : int = 0
    // collection mode
    collect_warnings : bool = false
    warnings : array<string>
    // dedup: suppress duplicate warnings from generic instantiations
    reported_locations : table<uint64>
    // current function (for inferStack reporting)
    @do_not_delete current_function : Function?
    // STYLE011: track uninitialized variables from most recent ExprLet
    @do_not_delete pending_uninit_vars : array<Variable?>

    def StyleLintVisitor() {
        pass
    }

    def location_key(at : LineInfo) : uint64 {
        return uint64(at.line) | (uint64(at.column) << uint64(20))
    }

    def is_suppressed(text : string; at : LineInfo) : bool {
        return is_lint_suppressed(at, extract_lint_code(text))
    }

    def style_warning(text : string; at : LineInfo) : void {
        // Suppress warnings for macro-generated functions — the user didn't write that code.
        if (current_function != null && current_function.flags.generated) return
        let key = self->location_key(at)
        if (key_exists(reported_locations, key)) return
        reported_locations |> insert(key)
        if (self->is_suppressed(text, at)) return
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
            compiling_program() |> macro_style_warning(at, msg)
        } elif (collect_warnings) {
            warnings |> push("style warning: {msg} at {describe(at)}")
        } else {
            to_log(LOG_WARNING, "style warning: {msg} at {describe(at)}\n")
        }
    }

    // --- source line helpers ---

    def source_line_between(at_start : LineInfo; at_end : LineInfo) : string {
        //! Reads the source text between two line infos (potentially multi-line).
        //! Returns the concatenated lines from at_start.line to at_end.line.
        var result = ""
        if (at_start.fileInfo == null) return result
        for (ln in range(int(at_start.line), int(at_end.line) + 1)) {
            get_file_source_line(at_start.fileInfo, uint(ln)) $(line) {
                result = "{result}{line}\n"
            }
        }
        return result
    }

    // --- function tracking ---

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
        self->check_defer_pipe(fn)
    }

    // --- STYLE001/002 for defer <| (macro erases AST, scan source lines) ---

    def check_defer_pipe(fn : FunctionPtr) : void {
        if (fn.body == null || fn.at.fileInfo == null || fn.at.line == 0u) return
        let body_at = fn.body.at
        for (ln in range(int(body_at.line), int(body_at.last_line) + 1)) {
            var src = ""
            get_file_source_line(fn.at.fileInfo, uint(ln)) $(line) {
                src := line
            }
            let comment_pos = find(src, "//")
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
            if (starts_with(after_pipe, "$()")) {
                self->style_warning("STYLE002: remove <| pipe and $() for parameterless block; use direct trailing block", at)
            } else {
                self->style_warning("STYLE001: remove <| pipe before block argument; use direct trailing block syntax", at)
            }
        }
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        return <- fn
    }

    // --- STYLE001/002/003/011: pipe and redundant $() on block arguments ---

    def check_block_pipe(call_at : LineInfo; var arguments : dasvector`ptr`Expression) : void {
        let nargs = length(arguments)
        if (nargs == 0) return
        // Find ExprMakeBlock among arguments (may not be last due to ExprFakeContext/ExprFakeLineInfo)
        for (i in range(nargs)) {
            var arg = arguments[i]
            if (arg is ExprMakeBlock) {
                var mkblk = arg as ExprMakeBlock
                var blk = mkblk._block
                if (blk is ExprBlock) {
                    var eblk = blk as ExprBlock
                    if (eblk.blockFlags.isClosure) {
                        let block_arg_count = length(eblk.arguments)
                        // Read source around the call to check for <| and $()
                        let src = self->source_line_between(call_at, eblk.at)
                        let pipe_pos = find(src, "<|")
                        let has_pipe = pipe_pos >= 0
                        if (has_pipe) {
                            // Only warn when <| pipes a block/lambda ($, @, @@, or {).
                            // Value pipes like <| qmacro(...) are fine style.
                            let after_pipe = strip_left(slice(src, pipe_pos + 2))
                            let pipes_block = starts_with(after_pipe, "$(") || starts_with(after_pipe, "$ ") || starts_with(after_pipe, "$\{") || starts_with(after_pipe, "@") || starts_with(after_pipe, "\{")
                            if (pipes_block) {
                                // Check for lambda shorthand: <| $(params) => expr
                                // The => syntax requires <|, so don't flag
                                let arrow_pos = find(after_pipe, "=>")
                                let brace_pos = find(after_pipe, "\{")
                                let is_lambda_shorthand = arrow_pos >= 0 && (brace_pos < 0 || arrow_pos < brace_pos)
                                if (!is_lambda_shorthand) {
                                    if (block_arg_count == 0) {
                                        self->style_warning("STYLE002: remove <| pipe and $() for parameterless block; use direct trailing block", call_at)
                                    } else {
                                        self->style_warning("STYLE001: remove <| pipe before block argument; use direct trailing block syntax", call_at)
                                    }
                                }
                            }
                        }
                        // STYLE003 for $() without pipe is handled in preVisitExprMakeBlock
                    }
                }
                break
            }
        }
    }

    def override preVisitExprCall(var expr : ExprCall?) : void {
        self->check_block_pipe(expr.at, expr.arguments)
        self->check_style019_clamp(expr)
        self->check_style020_from_jv(expr)
    }

    // --- STYLE020: from_JV scalar form → v ?? defV ---

    def is_supported_jv_scalar(t : TypeDecl?) : bool {
        //! True for the scalar types that have a matching `operator ??` overload in
        //! daslib/json_boost — see lines 85-140 of json_boost.das. Bitfield and enum
        //! `from_JV` overloads exist but have no `??` counterpart, so they stay silent.
        if (t == null || !empty(t.dim)) return false
        let bt = t.baseType
        return (bt == Type.tInt    || bt == Type.tUInt
                || bt == Type.tInt8  || bt == Type.tUInt8
                || bt == Type.tInt16 || bt == Type.tUInt16
                || bt == Type.tInt64 || bt == Type.tUInt64
                || bt == Type.tFloat || bt == Type.tDouble
                || bt == Type.tBool  || bt == Type.tString)
    }

    def root_generic(fn : Function const?) : Function const? {
        //! Walk fromGeneric chain to its root. Template instantiations of overloaded
        //! generics produce a 2-level chain: instantiated → mangled-overload → root.
        var cur = fn
        while (cur != null && cur.fromGeneric != null) {
            cur = cur.fromGeneric
        }
        return cur
    }

    def check_style020_from_jv(expr : ExprCall?) : void {
        if (expr.func == null || length(expr.arguments) < 2) return
        // The user writes `from_JV(jv, type<int>, 13)`. After template
        // instantiation (the json_boost overloads are `[template(ent)]`), expr.func
        // is the mangled `template`0x..`json_boost`from_JV`..` and the `ent` witness
        // is stripped — arguments shrink to `(jv, defV)`. The instantiation chain is
        // two levels deep: instantiated → mangled-overload → original `from_JV`.
        let root = self->root_generic(expr.func)
        let is_from_JV = (root != null && root.name == "from_JV"
            && root._module != null && root._module.name == "json_boost")
        if (!is_from_JV) return
        // Skip lint when current_function is itself a json_boost overload or a generic
        // instantiation of one — the dispatch chain inside JV/from_JV templates calls
        // from_JV recursively on struct fields, which would self-trigger otherwise.
        if (current_function != null) {
            let in_cf = (current_function._module != null
                && current_function._module.name == "json_boost")
            let in_fg = (current_function.fromGeneric != null
                && current_function.fromGeneric._module != null
                && current_function.fromGeneric._module.name == "json_boost")
            return if (in_cf || in_fg)
        }
        // Use the call's result type — it's the scalar (or non-scalar) the user
        // would write on the RHS of `??`. Robust under both pre- and post-instantiation
        // argument shapes.
        if (!self->is_supported_jv_scalar(expr._type)) return
        self->style_warning("STYLE020: from_JV(v, type<T>, defV) on a supported scalar — use 'v ?? defV' (operator ?? from json_boost)", expr.at)
    }

    // --- STYLE019: nested min/max → clamp ---

    def is_math_call(expr : Expression? const; fname : string) : bool {
        //! True iff `expr` is `math::<fname>(_, _)` with exactly two arguments
        //! (peeling ExprRef2Value). Conservatively skips any non-math overload.
        let inner = self->peel_ref2value_const(expr)
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
        // min(max(x, lo), hi) — inner is arg[0]
        // max(min(x, hi), lo) — inner is arg[0] (mirror)
        if (self->is_math_call(expr.arguments[0], inner_name)) {
            self->style_warning("STYLE019: nested {outer_name}({inner_name}(...)) — use 'clamp(x, lo, hi)' from math module", expr.at)
            return
        }
        if (self->is_math_call(expr.arguments[1], inner_name)) {
            self->style_warning("STYLE019: nested {outer_name}({inner_name}(...)) — use 'clamp(x, lo, hi)' from math module", expr.at)
        }
    }

    def override preVisitExprInvoke(var expr : ExprInvoke?) : void {
        self->check_block_pipe(expr.at, expr.arguments)
    }


    def override preVisitExprMakeGenerator(var expr : ExprMakeGenerator?) : void {
        self->check_block_pipe(expr.at, expr.arguments)
    }

    // --- STYLE001/002 for desugared generators ---

    def override preVisitExprMakeStruct(var expr : ExprMakeStruct?) : void {
        // generator<T>() <| $ { ... } is lowered before lint runs — ExprMakeGenerator
        // is replaced by ExprMakeStruct. Detect via source line inspection.
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
            self->style_warning("STYLE002: remove <| pipe and $() for parameterless block; use direct trailing block", expr.at)
        } else {
            self->style_warning("STYLE001: remove <| pipe before block argument; use direct trailing block syntax", expr.at)
        }
    }

    // --- STYLE003: redundant $() on parameterless blocks (all contexts) ---

    def override preVisitExprMakeBlock(var expr : ExprMakeBlock?) : void {
        var blk = expr._block
        if (!(blk is ExprBlock)) return
        var eblk = blk as ExprBlock
        if (!eblk.blockFlags.isClosure || eblk.blockFlags.isLambdaBlock || !empty(eblk.arguments)) return
        let src = self->source_line_between(expr.at, eblk.at)
        let dollar_pos = find(src, "$()")
        // Skip when there is no $() at all, or when <| pipe is present
        // (STYLE002 already covers the piped form).
        if (dollar_pos < 0 || find(src, "<|") >= 0) return
        // Skip if explicit return type follows: $() : Type { ... }
        let after_dollar = strip_left(slice(src, dollar_pos + 3))
        if (starts_with(after_dollar, ":")) return
        self->style_warning("STYLE003: remove redundant $() for parameterless block; use $ \{", expr.at)
    }

    // --- STYLE006: string(__rtti) == "..." should use `is` ---
    // --- STYLE018: b == true / b == false / b != true / b != false ---

    def peel_ref2value_const(e : Expression? const) : Expression? const {
        if (e != null && e is ExprRef2Value) return (e as ExprRef2Value.subexpr)
        return e
    }

    def expr_equal_struct(a : Expression? const; b : Expression? const) : bool {
        //! Structural equality via describe(). No purity filter.
        if (a == null || b == null) return false
        return describe(a) == describe(b)
    }

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        if (expr.op == "==") {
            if (self->is_string_rtti_comparison(expr.left) || self->is_string_rtti_comparison(expr.right)) {
                self->style_warning("STYLE006: string(__rtti) comparison should use `is` operator; e.g. expr is ExprFoo", expr.at)
            }
        }
        // STYLE018: b == true / b == false / b != true / b != false
        if (expr.op == "==" || expr.op == "!=") {
            let lhs = self->peel_ref2value_const(expr.left)
            let rhs = self->peel_ref2value_const(expr.right)
            let lbool = lhs != null && lhs is ExprConstBool
            let rbool = rhs != null && rhs is ExprConstBool
            // Avoid double-flagging `true == false` on both sides — flag only when
            // exactly one side is a bool literal (the other side is the bool var/expr).
            if (lbool != rbool) {
                let const_side = lbool ? lhs : rhs
                let cval = (const_side as ExprConstBool).value
                let positive = (expr.op == "==" && cval) || (expr.op == "!=" && !cval)
                let suggestion = positive ? "b" : "!b"
                self->style_warning("STYLE018: redundant boolean comparison — use '{suggestion}' directly (drop the '{expr.op} {cval}')", expr.at)
            }
        }
        // STYLE022: foo |= BfT.m  /  foo &= ~BfT.m  on a bitfield variable.
        if (expr.op == "|=" || expr.op == "&=") {
            self->check_style022_bitfield_set_clear(expr)
        }
        // STYLE023: int_cast(bf & BfT.m) !=/== 0  →  [!]bf.m
        if (expr.op == "==" || expr.op == "!=") {
            self->check_style023_bitfield_test(expr)
        }
    }

    // --- STYLE022: bitfield-as-field compound-set/clear ---

    def is_bitfield_value_type(t : TypeDecl?) : bool {
        //! True for plain bitfield-typed expressions (not arrays of bitfield).
        return t != null && t.baseType == Type.tBitfield && empty(t.dim)
    }

    def extract_single_bit_name(expr : Expression? const; bf_type : TypeDecl?) : string {
        //! If `expr` resolves to a single named bit of `bf_type`, return its name; else "".
        //! Accepts two shapes:
        //!   - ExprField(value=ExprVar(BfTypeName), name="m") — common when running under
        //!     lint policies (no_optimizations + no_infer_time_folding), which keep the
        //!     source-level field access intact.
        //!   - ExprConstBitfield(value=<single-bit mask>) — what regular compilation
        //!     leaves after folding; map the mask back to the bit name via argNames.
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
            // Single-bit mask: power of two, exactly one bit set.
            if (v == 0ul || (v & (v - 1ul)) != 0ul) return ""
            // Map bit index → argNames[index].
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
        //! True for ExprConstInt/UInt/Int64/UInt64 whose value is zero. The lint
        //! runner disables folding, so `0` / `0u` / `0l` / `0ul` literals stay as
        //! distinct typed const nodes — we accept all four.
        if (expr == null) return false
        if (expr is ExprConstInt) return (expr as ExprConstInt.value) == 0
        if (expr is ExprConstUInt) return (expr as ExprConstUInt.value) == 0u
        if (expr is ExprConstInt64) return (expr as ExprConstInt64.value) == 0l
        if (expr is ExprConstUInt64) return (expr as ExprConstUInt64.value) == 0ul
        return false
    }

    def unwrap_int_like_cast(expr : Expression? const) : Expression? const {
        //! Peel one int/uint/int64/uint64 single-arg call. Returns the inner
        //! expression, or null if not such a cast.
        if (expr == null || !(expr is ExprCall)) return null
        let call = expr as ExprCall
        if (call.func == null) return null
        let name = string(call.func.name)
        if ((name != "int" && name != "uint" && name != "int64" && name != "uint64") || length(call.arguments) != 1) return null
        return call.arguments[0]
    }

    def check_style023_bitfield_test(expr : ExprOp2?) : void {
        //! STYLE023: `int_cast(bf & BfT.bit) !=/== 0` collapses to `bf.bit` / `!bf.bit`.
        //! Detect: one side is a typed zero constant; the other side is
        //! `int|uint|int64|uint64(bf & SingleBit)` where `bf` is bitfield-typed
        //! and `SingleBit` is a single named bit of the same bitfield.
        let lhs = self->peel_ref2value_const(expr.left)
        let rhs = self->peel_ref2value_const(expr.right)
        let lzero = self->is_zero_int_const(lhs)
        let rzero = self->is_zero_int_const(rhs)
        if (lzero == rzero) return    // 0 == 0 and cast == cast both skipped
        let cast_side = lzero ? rhs : lhs
        let inner = self->unwrap_int_like_cast(cast_side)
        if (inner == null) return
        // inner must be `bf & SingleBit`. Peel ref2value, require ExprOp2 op="&".
        var and_expr = inner
        if (and_expr != null && and_expr is ExprRef2Value) {
            and_expr = (and_expr as ExprRef2Value.subexpr)
        }
        if (and_expr == null || !(and_expr is ExprOp2)) return
        let op2 = and_expr as ExprOp2
        if (op2.op != "&" || op2.left == null || !self->is_bitfield_value_type(op2.left._type)) return
        let bf_type = op2.left._type
        let bit_name = self->extract_single_bit_name(op2.right, bf_type)
        if (empty(bit_name)) return
        let positive = expr.op == "!="    // !=0 means "the bit is set"
        let suggestion = positive ? "bf.{bit_name}" : "!bf.{bit_name}"
        self->style_warning("STYLE023: bitfield test via int-cast can be written directly — use '{suggestion}' instead (drop the cast and the '{expr.op} 0' compare)", expr.at)
    }

    def check_style022_bitfield_set_clear(expr : ExprOp2?) : void {
        if (expr.left == null || !self->is_bitfield_value_type(expr.left._type)) return
        let bf_type = expr.left._type
        if (expr.op == "|=") {
            // foo |= BfT.m  →  foo.m = true
            let name = self->extract_single_bit_name(expr.right, bf_type)
            if (empty(name)) return
            self->style_warning("STYLE022: 'foo |= BfT.{name}' on a bitfield is the bit-set idiom; write 'foo.{name} = true' instead (bitfield-as-field assignment)", expr.at)
        } elif (expr.op == "&=") {
            // foo &= ~BfT.m  →  foo.m = false
            // The folded form is ExprConstBitfield with the complement mask; the
            // un-folded form is ExprOp1(~, ExprField). Cover both.
            var inner = expr.right
            if (inner != null && inner is ExprRef2Value) {
                inner = (inner as ExprRef2Value.subexpr)
            }
            // ExprOp1(~, X): peel the operator, match a single bit on X.
            if (inner != null && inner is ExprOp1) {
                let op1 = inner as ExprOp1
                if (op1.op != "~") return
                let name = self->extract_single_bit_name(op1.subexpr, bf_type)
                if (empty(name)) return
                self->style_warning("STYLE022: 'foo &= ~BfT.{name}' on a bitfield is the bit-clear idiom; write 'foo.{name} = false' instead (bitfield-as-field assignment)", expr.at)
                return
            }
            // ExprConstBitfield: must be the complement of a single bit.
            if (inner != null && inner is ExprConstBitfield) {
                let cb = inner as ExprConstBitfield
                let v = uint64(cb.value)
                // Build the valid-bit mask for the bitfield width. Shifting by ≥ 64 is UB,
                // so special-case bitfield64 (length(argNames) == 64) to ~0ul.
                let n = uint64(length(bf_type.argNames))
                let valid_mask = n >= 64ul ? ~0ul : ((1ul << n) - 1ul)
                let complement = (~v) & valid_mask
                // Single-bit complement: complement is a power of two.
                if (complement == 0ul || (complement & (complement - 1ul)) != 0ul) return
                var idx = 0
                var probe = complement
                while (probe > 1ul) {
                    probe = probe >> 1ul
                    idx++
                }
                if (idx < 0 || idx >= length(bf_type.argNames)) return
                let name = string(bf_type.argNames[idx])
                self->style_warning("STYLE022: 'foo &= ~BfT.{name}' on a bitfield is the bit-clear idiom; write 'foo.{name} = false' instead (bitfield-as-field assignment)", expr.at)
            }
        }
    }

    // --- STYLE005: braces around a single-statement early exit ---
    // --- STYLE010: if (true) should be bare block ---
    // --- STYLE016(b): if (a) { return X } else if (b) { return X } ---
    // --- STYLE017(a): if (cond) return true else return false ---

    def single_terminator_body(body : Expression? const) : Expression? const {
        //! If `body` is a bare ExprReturn/Break/Continue, or a 1-statement ExprBlock
        //! wrapping one, return the inner terminator. Otherwise null.
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
        //! Two terminators have the same payload iff they are the same kind and
        //! their return-subexpr (if any) is structurally equal. Break==Break and
        //! Continue==Continue have no payload.
        if (a == null || b == null) return false
        if (a is ExprBreak) return b is ExprBreak
        if (a is ExprContinue) return b is ExprContinue
        if (a is ExprReturn) {
            if (!(b is ExprReturn)) return false
            let ra = a as ExprReturn
            let rb = b as ExprReturn
            if (ra.subexpr == null && rb.subexpr == null) return true
            if (ra.subexpr == null || rb.subexpr == null) return false
            return self->expr_equal_struct(ra.subexpr, rb.subexpr)
        }
        return false
    }

    def is_const_bool_return(term : Expression? const; var b_out : bool&) : bool {
        //! True if `term` is `return ExprConstBool(b)`. Writes the bool to b_out.
        if (term == null || !(term is ExprReturn)) return false
        let ret = term as ExprReturn
        let sub = self->peel_ref2value_const(ret.subexpr)
        if (sub == null || !(sub is ExprConstBool)) return false
        b_out = (sub as ExprConstBool).value
        return true
    }

    def unwrap_inner_ifte(if_false : Expression? const) : ExprIfThenElse? {
        //! Strip a 1-stmt ExprBlock wrapping an ExprIfThenElse, returning the inner if.
        //! Otherwise return the if_false directly cast to ExprIfThenElse?, or null.
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

    def override preVisitExprIfThenElse(ifte : ExprIfThenElse?) : void {
        if (ifte.if_flags.isStatic) return
        // STYLE010: if (true) { ... }
        var cond = ifte.cond
        if (cond is ExprConstBool) {
            var cbool = cond as ExprConstBool
            if (cbool.value && ifte.if_false == null) {
                self->style_warning("STYLE010: if (true) is always taken; use a bare block instead", ifte.at)
            }
        }
        // Detect terminator in the `then` branch — used by STYLE005, STYLE016(b), STYLE017(a)
        let outer_term = self->single_terminator_body(ifte.if_true)
        // STYLE005: braces around a single-statement early exit. The parser builds a
        // synthetic ExprBlock for both braceless `if (c) return` and postfix-desugared
        // `return X if (c)`, sharing its LineInfo with the inner terminator. A source-
        // written `{...}` has its own LineInfo (starts at the `{`). So `blk.at != inner.at`
        // ⇔ user wrote braces.
        if (ifte.if_false == null && outer_term != null && ifte.if_true is ExprBlock) {
            let blk = ifte.if_true as ExprBlock
            if (blk.at.line != outer_term.at.line || blk.at.column != outer_term.at.column) {
                self->style_warning("STYLE005: braces around a single-statement early exit are noise; use 'if (cond) <terminator>' or postfix '<terminator> if (cond)'", ifte.at)
            }
        }
        // STYLE016(b): if (a) { return X } else if (b) { return X }
        if (outer_term != null && ifte.if_false != null) {
            let inner = self->unwrap_inner_ifte(ifte.if_false)
            if (inner != null && inner.if_false == null) {
                let inner_term = self->single_terminator_body(inner.if_true)
                if (inner_term != null && self->terminators_payload_equal(outer_term, inner_term)) {
                    self->style_warning("STYLE016: adjacent guards leading to identical early-exit can be combined with '||'", ifte.at)
                }
            }
        }
        // STYLE017(a): if (cond) return true else return false (or vice versa)
        if (outer_term != null && ifte.if_false != null) {
            let else_term = self->single_terminator_body(ifte.if_false)
            if (else_term != null) {
                var b1 = false
                var b2 = false
                if (self->is_const_bool_return(outer_term, b1) && self->is_const_bool_return(else_term, b2) && b1 != b2) {
                    let suggestion = b1 ? "return cond" : "return !cond"
                    self->style_warning("STYLE017: 'if (cond) return {b1}; else return {b2}' — use '{suggestion}' directly", ifte.at)
                }
            }
        }
    }

    // --- STYLE011: variable declaration followed by immediate assignment ---

    def override preVisitExprLetVariable(expr : ExprLet?; var v : VariablePtr; last : bool) : void {
        if (v.init == null && !v.flags.generated && !v.flags.inScope && (current_function == null || current_function.fromGeneric == null)) {
            pending_uninit_vars |> push(v)
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

    // --- STYLE016(a): adjacent guards `if (a) { return X }; if (b) { return X }` ---
    // --- STYLE017(b): adjacent `if (cond) { return b1 }; return b2` (b1 != b2) ---

    def override preVisitExprBlock(blk : ExprBlock?) : void {
        let n = length(blk.list)
        for (i in range(n - 1)) {
            let cur = blk.list[i]
            let nxt = blk.list[i + 1]
            if (!(cur is ExprIfThenElse)) continue
            let outer_if = cur as ExprIfThenElse
            if (outer_if.if_false != null) continue
            let outer_term = self->single_terminator_body(outer_if.if_true)
            if (outer_term == null) continue
            // STYLE016(a): adjacent guard with same payload
            if (nxt is ExprIfThenElse) {
                let nxt_if = nxt as ExprIfThenElse
                if (nxt_if.if_false == null) {
                    let nxt_term = self->single_terminator_body(nxt_if.if_true)
                    if (nxt_term != null && self->terminators_payload_equal(outer_term, nxt_term)) {
                        self->style_warning("STYLE016: adjacent guards leading to identical early-exit can be combined with '||'", outer_if.at)
                        continue
                    }
                }
            }
            // STYLE017(b): adjacent return-true-then-return-false
            var b1 = false
            var b2 = false
            if (self->is_const_bool_return(outer_term, b1) && self->is_const_bool_return(nxt, b2) && b1 != b2) {
                let suggestion = b1 ? "return cond" : "return !cond"
                self->style_warning("STYLE017: 'if (cond) return {b1}; return {b2}' — use '{suggestion}' directly", outer_if.at)
            }
        }
    }

    def override preVisitExprBlockExpression(blk : ExprBlock?; expr : ExpressionPtr) : void {
        if (!empty(pending_uninit_vars)) {
            if (expr is ExprCopy) {
                let v = self->find_assign_var((expr as ExprCopy).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    self->style_warning("STYLE011: variable declaration followed by immediate assignment; combine into single declaration with initialization", expr.at)
                }
            } elif (expr is ExprClone) {
                let v = self->find_assign_var((expr as ExprClone).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    self->style_warning("STYLE011: variable declaration followed by immediate assignment; combine into single declaration with initialization", expr.at)
                }
            } elif (expr is ExprMove) {
                let v = self->find_assign_var((expr as ExprMove).left)
                if (v != null && pending_uninit_vars |> has_value(v)) {
                    self->style_warning("STYLE011: variable declaration followed by immediate assignment; combine into single declaration with initialization", expr.at)
                }
            }
            pending_uninit_vars |> resize(0)
        }
        if (expr is ExprLet) {
            self->check_style012_array_init(blk, expr as ExprLet)
            self->check_style013_struct_field_init(blk, expr as ExprLet)
            self->check_style021_jv_table_init(blk, expr as ExprLet)
        }
    }

    // --- STYLE012: array<T> var then contiguous push/emplace instead of literal ---

    def check_style012_array_init(blk : ExprBlock?; elet : ExprLet?) : void {
        if (current_function != null && current_function.fromGeneric != null) return
        // Find this ExprLet's index in the enclosing block's statement list.
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
            // Only flag uninitialized array<T> locals from user code (skip generic
            // host instantiations and compiler-synthesized vars).
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || v._type == null || v._type.baseType != Type.tArray
                || !empty(v._type.dim)) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                let stmt = blk.list[j]
                if (!(stmt is ExprCall) || !self->is_push_or_emplace_of(stmt as ExprCall, v)) break
                count++
            }
            if (count >= 2) {
                self->style_warning("STYLE012: array<T> initialized by a run of push/emplace calls; use an array literal `var a <- [x, <-y, z]` (or `var a <- array<T>(x, y, z)` when you need an explicit element type, e.g. for polymorphic upcasts)", v.at)
            }
        }
    }

    // --- STYLE013: struct var with default/empty init then a run of field assignments ---

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
            // Skip generic-host instantiations and any non-struct or non-default-init local
            if (v.flags.generated || v.flags.inScope
                || v._type == null || v._type.baseType != Type.tStructure
                || !empty(v._type.dim) || !self->is_default_struct_init(v)) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                if (!self->is_field_assign_of(blk.list[j], v)) break
                count++
            }
            if (count >= 2) {
                self->style_warning("STYLE013: struct var (default-init or uninitialized) followed by a run of field assignments; use a named-argument constructor `var a = Foo(bar = ..., far = ...)`", v.at)
            }
        }
    }

    def is_default_struct_init(v : Variable?) : bool {
        //! True for `var a : Foo` (no init) or `var a = Foo()` (zero-arg MakeStruct).
        //! False for any non-empty constructor or non-MakeStruct init — those are
        //! "modify an existing value" cases, not the bug we want to flag.
        return true if (v.init == null)
        return false if (!(v.init is ExprMakeStruct))
        var ms = v.init as ExprMakeStruct
        if (ms._block != null) return false
        for (row in ms.structs) {
            for (_mf in *row) {
                // any field present → not the zero-arg constructor
                return false
            }
        }
        return true
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
        let src = (lhs as ExprField).value
        return false if (src == null || !(src is ExprVar))
        return (src as ExprVar).variable == target
    }

    // --- STYLE021: table<string; JsonValue?> built via repeated insert → JV(named-tuple) ---

    def is_jv_pointer_type(t : TypeDecl?) : bool {
        if (t == null || t.baseType != Type.tPointer || t.firstType == null
                || t.firstType.baseType != Type.tHandle
                || t.firstType.annotation == null) return false
        return t.firstType.annotation.name == "JsonValue"
    }

    def is_jv_table_type(t : TypeDecl?) : bool {
        //! Matches `table<string; JsonValue?>` (or any pointer-to-JsonValue value type).
        if (t == null || t.baseType != Type.tTable || !empty(t.dim)
                || t.firstType == null
                || t.firstType.baseType != Type.tString) return false
        return self->is_jv_pointer_type(t.secondType)
    }

    def is_insert_of_const_key(call : ExprCall?; target : Variable?) : bool {
        //! True when `call` is `target |> insert(<const string>, <any>)` — i.e.
        //! pipe-lowered to a 3-arg insert call on this exact variable, with a
        //! constant string key. Computed keys disqualify the chain.
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
        // Find this ExprLet's index in the enclosing block's statement list.
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
            // Only flag uninitialized table<string; JsonValue?> locals from user code.
            if (v.init != null
                || v.flags.generated || v.flags.inScope
                || !self->is_jv_table_type(v._type)) continue
            var count = 0
            for (j in range(idx + 1, n)) {
                let stmt = blk.list[j]
                if (!(stmt is ExprCall) || !self->is_insert_of_const_key(stmt as ExprCall, v)) break
                count++
            }
            if (count >= 2) {
                self->style_warning("STYLE021: table<string; JsonValue?> built by repeated insert calls; use 'var v = JV((k1=..., k2=..., ...))' (named-tuple JV form from json_boost)", v.at)
            }
        }
    }

    def is_push_or_emplace_of(call : ExprCall?; target : Variable?) : bool {
        if (call.func == null || call.func.fromGeneric == null
            || (call.func.fromGeneric.name != "push" && call.func.fromGeneric.name != "emplace")
            || length(call.arguments) < 2) return false
        var arg = call.arguments[0]
        if (arg is ExprRef2Value) {
            arg = (arg as ExprRef2Value.subexpr)
        }
        if (!(arg is ExprVar)) return false
        return (arg as ExprVar).variable == target
    }

    // --- STYLE014/STYLE015: large comment blocks (post-visit pass) ---

    def scan_long_comment_blocks(prog : ProgramPtr) : void {
        let mod = prog.getThisModule
        if (mod == null) return
        var file_first_decl : table<string; uint>
        var file_max_line : table<string; uint>
        var fn_file : array<string>
        var fn_starts : array<uint>
        var fn_ends : array<uint>
        var fn_private : array<bool>
        for_each_function(mod, "") $(var fn) {
            if (fn.body == null || fn.at.fileInfo == null || fn.at.line == 0u
                || fn.flags.generated || fn.fromGeneric != null
                || fn._module != mod || fn.moreFlags.isTemplate) return
            let fname = string(fn.at.fileInfo.name)
            let s = fn.at.line
            let e = fn.body.at.last_line
            if (file_first_decl |> key_exists(fname)) {
                if (s < file_first_decl[fname]) {
                    file_first_decl[fname] = s
                }
                if (e > file_max_line[fname]) {
                    file_max_line[fname] = e
                }
            } else {
                file_first_decl |> insert(fname, s)
                file_max_line |> insert(fname, e)
            }
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
            let s = st.at.line
            if (file_first_decl |> key_exists(fname)) {
                if (s < file_first_decl[fname]) {
                    file_first_decl[fname] = s
                }
                if (s > file_max_line[fname]) {
                    file_max_line[fname] = s
                }
            } else {
                file_first_decl |> insert(fname, s)
                file_max_line |> insert(fname, s)
            }
        }
        for_each_enumeration(mod) $(var en) {
            if (en.at.fileInfo == null || en.at.line == 0u) return
            let fname = string(en.at.fileInfo.name)
            let s = en.at.line
            if (file_first_decl |> key_exists(fname)) {
                if (s < file_first_decl[fname]) {
                    file_first_decl[fname] = s
                }
                if (s > file_max_line[fname]) {
                    file_max_line[fname] = s
                }
            } else {
                file_first_decl |> insert(fname, s)
                file_max_line |> insert(fname, s)
            }
        }
        for_each_global(mod) $(var v) {
            if (v.at.fileInfo == null || v.at.line == 0u || v.flags.generated) return
            let fname = string(v.at.fileInfo.name)
            let s = v.at.line
            if (file_first_decl |> key_exists(fname)) {
                if (s < file_first_decl[fname]) {
                    file_first_decl[fname] = s
                }
                if (s > file_max_line[fname]) {
                    file_max_line[fname] = s
                }
            } else {
                file_first_decl |> insert(fname, s)
                file_max_line |> insert(fname, s)
            }
        }
        for_each_typedef(mod) $(name, value) {
            if (value.at.fileInfo == null || value.at.line == 0u) return
            let fname = string(value.at.fileInfo.name)
            let s = value.at.line
            if (file_first_decl |> key_exists(fname)) {
                if (s < file_first_decl[fname]) {
                    file_first_decl[fname] = s
                }
                if (s > file_max_line[fname]) {
                    file_max_line[fname] = s
                }
            } else {
                file_first_decl |> insert(fname, s)
                file_max_line |> insert(fname, s)
            }
        }
        var seen : table<string; bool>
        for_each_function(mod, "") $(var fn) {
            if (fn.at.fileInfo == null || fn._module != mod
                || fn.flags.generated || fn.fromGeneric != null || fn.moreFlags.isTemplate) return
            let fname = string(fn.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            self->scan_file_lines(fn.at, fname,
                file_first_decl[fname], file_max_line[fname],
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
            self->scan_file_lines(st.at, fname,
                file_first_decl[fname], file_max_line[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_enumeration(mod) $(var en) {
            if (en.at.fileInfo == null) return
            let fname = string(en.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            self->scan_file_lines(en.at, fname,
                file_first_decl[fname], file_max_line[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_global(mod) $(var v) {
            if (v.at.fileInfo == null || v.flags.generated) return
            let fname = string(v.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            self->scan_file_lines(v.at, fname,
                file_first_decl[fname], file_max_line[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
        for_each_typedef(mod) $(name, value) {
            if (value.at.fileInfo == null) return
            let fname = string(value.at.fileInfo.name)
            if ((seen |> key_exists(fname)) || !file_first_decl |> key_exists(fname)) return
            seen |> insert(fname, true)
            self->scan_file_lines(value.at, fname,
                file_first_decl[fname], file_max_line[fname],
                fn_file, fn_starts, fn_ends, fn_private)
        }
    }

    def scan_file_lines(sample_at : LineInfo; file_name : string;
                        first_decl, max_line : uint;
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
                    self->emit_long_comment(sample_at, file_name, first_decl, block_start, block_count, block_first_line,
                        fn_file, fn_starts, fn_ends, fn_private)
                    in_block = false
                }
            }
            ln++
        }
        if (in_block) {
            self->emit_long_comment(sample_at, file_name, first_decl, block_start, block_count, block_first_line,
                fn_file, fn_starts, fn_ends, fn_private)
        }
    }

    def has_nolint_directive(first_line : string) : bool {
        // Positional check — `@nolint` MUST be the first token after `//`/`//!`
        // (and optional whitespace) so we don't mis-match URLs/prose that
        // happen to contain the literal `@nolint`.
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
        // Skip when the block is too short, lives before the first AST decl
        // (module-leading docstring), or carries an explicit @nolint marker.
        if (line_count <= 1
            || block_start < first_decl_line
            || self->has_nolint_directive(first_line_text)) return
        var enclosing_priv = false
        var inside_function = false
        var enclosing_size : uint = 0xFFFFFFFFu
        for (i in range(length(fn_file))) {
            if (fn_file[i] != file_name) continue
            let s = fn_starts[i]
            let e = fn_ends[i]
            if (s <= block_start && block_start <= e) {
                let size = e - s
                if (size < enclosing_size) {
                    enclosing_size = size
                    enclosing_priv = fn_private[i]
                    inside_function = true
                }
            }
        }
        let private_ctx = inside_function && enclosing_priv
        let threshold = private_ctx ? 1 : 3
        if (line_count <= threshold) return
        var at = sample_at
        at.line = block_start
        at.last_line = block_start
        at.column = 1u
        at.last_column = 1u
        var msg = ""
        if (private_ctx) {
            msg = "STYLE015: {line_count}-line comment inside 'def private' exceeds 1-line cap; private symbols don't get public docs — keep at most one WHY line, or suppress on the first line with '// nolint:STYLE015'"
        } else {
            msg = "STYLE014: comment block of {line_count} lines exceeds 3-line cap; trim multi-paragraph prose to a 1-line WHY (move design notes to .md), or suppress on the first line with '//!@nolint' (//! blocks) or '// nolint:STYLE014' (// blocks)"
        }
        self->style_warning(msg, at)
    }

    // --- helpers ---

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
            var field = arg as ExprField
            return field.name == "__rtti"
        }
        return false
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

def public style_lint(prog : ProgramPtr; compile_time_errors : bool; comment_hygiene : bool = false) : int {
    //! Runs the style lint visitor on the compiled program.
    //! Returns the number of warnings found.
    //! Pass ``comment_hygiene = true`` to enable STYLE014/STYLE015 checks.
    var astVisitor = new StyleLintVisitor(compile_time_errors = compile_time_errors, comment_hygiene = comment_hygiene)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_with_generics(prog, astVisitorAdapter)
    }
    if (comment_hygiene) {
        astVisitor->scan_long_comment_blocks(prog)
    }
    let count = astVisitor.warning_count
    unsafe { delete astVisitor; }
    return count
}

def public style_lint_collect(prog : ProgramPtr; var warnings : array<string>; comment_hygiene : bool = false) : int {
    //! Runs the style lint visitor and collects warnings as strings.
    //! Returns the number of warnings found.
    //! Pass ``comment_hygiene = true`` to enable STYLE014/STYLE015 checks.
    var astVisitor = new StyleLintVisitor(compile_time_errors = false, collect_warnings = true, comment_hygiene = comment_hygiene)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit_with_generics(prog, astVisitorAdapter)
    }
    if (comment_hygiene) {
        astVisitor->scan_long_comment_blocks(prog)
    }
    let count = astVisitor.warning_count
    warnings |> reserve(length(astVisitor.warnings))
    for (w in astVisitor.warnings) {
        warnings |> push(w)
    }
    unsafe { delete astVisitor; }
    return count
}

// ---------------------------------------------------------------------------
// Lint macro (auto-runs when module is required)
// ---------------------------------------------------------------------------

[lint_macro]
class StyleLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        let hygiene = prog._options |> find_arg("_comment_hygiene") ?as tBool ?? false
        style_lint(prog, true, [comment_hygiene = hygiene])
        return true
    }
}
