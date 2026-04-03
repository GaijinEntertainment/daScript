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
//! warnings as ``CompilationError::style_lint`` (error code 40218).
//!
//! Rules:
//!   STYLE001 — unnecessary <| pipe before block argument
//!   STYLE002 — <| pipe before parameterless block (remove pipe and $())
//!   STYLE003 — redundant $() on parameterless block argument
//!   STYLE005 — single-statement if can use postfix conditional (configurable, off by default)
//!   STYLE006 — string(__rtti) comparison should use `is` operator
//!   STYLE010 — if (true) should be a bare block

require daslib/ast_boost
require strings

// ---------------------------------------------------------------------------
// Visitor
// ---------------------------------------------------------------------------

class StyleLintVisitor : AstVisitor {
    compile_time_errors : bool
    postfix_conditionals : bool = false
    // counter
    warning_count : int = 0
    // collection mode
    collect_warnings : bool = false
    warnings : array<string>
    // dedup: suppress duplicate warnings from generic instantiations
    reported_locations : array<uint64>
    // current function (for inferStack reporting)
    @do_not_delete current_function : Function?

    def StyleLintVisitor() {
        pass
    }

    def location_key(at : LineInfo) : uint64 {
        return uint64(at.line) | (uint64(at.column) << uint64(20))
    }

    def is_suppressed(text : string; at : LineInfo) : bool {
        if (at.fileInfo == null || at.line == 0u) {
            return false
        }
        let colon = find(text, ":")
        if (colon < 0) {
            return false
        }
        let code = slice(text, 0, colon)
        var found = false
        get_file_source_line(at.fileInfo, at.line) $(line) {
            let nolint_pos = find(line, "nolint")
            if (nolint_pos >= 0) {
                found = find(line, code, nolint_pos) >= 0
            }
        }
        return found
    }

    def style_warning(text : string; at : LineInfo) : void {
        let key = self->location_key(at)
        if (reported_locations |> has_value(key)) {
            return
        }
        reported_locations |> push(key)
        if (self->is_suppressed(text, at)) {
            return
        }
        warning_count++
        var msg = text
        if (current_function != null && current_function.fromGeneric != null && length(current_function.inferStack) > 0) {
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
        if (at_start.fileInfo == null) {
            return result
        }
        for (ln in range(int(at_start.line), int(at_end.line) + 1)) {
            get_file_source_line(at_start.fileInfo, uint(ln)) $(line) {
                result = "{result}{line}\n"
            }
        }
        return result
    }

    // --- function tracking ---

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = get_ptr(fn)
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        return <- fn
    }

    // --- STYLE001/002/003/011: pipe and redundant $() on block arguments ---

    def check_block_pipe(call_at : LineInfo; var arguments : dasvector`smart_ptr`Expression) : void {
        let nargs = length(arguments)
        if (nargs == 0) {
            return
        }
        // Find ExprMakeBlock among arguments (may not be last due to ExprFakeContext/ExprFakeLineInfo)
        for (i in range(nargs)) {
            var arg = get_ptr(arguments[i])
            if (arg is ExprMakeBlock) {
                var mkblk = arg as ExprMakeBlock
                var blk = get_ptr(mkblk._block)
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

    def override preVisitExprCall(var expr : smart_ptr<ExprCall>) : void {
        self->check_block_pipe(expr.at, expr.arguments)
    }

    def override preVisitExprMakeGenerator(var expr : smart_ptr<ExprMakeGenerator>) : void {
        self->check_block_pipe(expr.at, expr.arguments)
    }

    // --- STYLE003: redundant $() on parameterless blocks (all contexts) ---

    def override preVisitExprMakeBlock(var expr : smart_ptr<ExprMakeBlock>) : void {
        var blk = get_ptr(expr._block)
        if (!(blk is ExprBlock)) {
            return
        }
        var eblk = blk as ExprBlock
        if (!eblk.blockFlags.isClosure || eblk.blockFlags.isLambdaBlock || length(eblk.arguments) != 0) {
            return
        }
        let src = self->source_line_between(expr.at, eblk.at)
        let dollar_pos = find(src, "$()")
        if (dollar_pos < 0) {
            return
        }
        // Skip if <| pipe is present — STYLE002 already covers that case
        if (find(src, "<|") >= 0) {
            return
        }
        // Skip if explicit return type follows: $() : Type { ... }
        let after_dollar = strip_left(slice(src, dollar_pos + 3))
        if (starts_with(after_dollar, ":")) {
            return
        }
        self->style_warning("STYLE003: remove redundant $() for parameterless block; use $ \{", expr.at)
    }

    // --- STYLE006: string(__rtti) == "..." should use `is` ---

    def override preVisitExprOp2(expr : smart_ptr<ExprOp2>) : void {
        if (expr.op != "==") {
            return
        }
        if (self->is_string_rtti_comparison(get_ptr(expr.left)) || self->is_string_rtti_comparison(get_ptr(expr.right))) {
            self->style_warning("STYLE006: string(__rtti) comparison should use `is` operator; e.g. expr is ExprFoo", expr.at)
        }
    }

    // --- STYLE005: postfix conditionals (configurable) ---
    // --- STYLE010: if (true) should be bare block ---

    def override preVisitExprIfThenElse(ifte : smart_ptr<ExprIfThenElse>) : void {
        if (ifte.if_flags.isStatic) {
            return
        }
        // STYLE010: if (true) { ... }
        var cond = get_ptr(ifte.cond)
        if (cond is ExprConstBool) {
            var cbool = cond as ExprConstBool
            if (cbool.value && ifte.if_false == null) {
                self->style_warning("STYLE010: if (true) is always taken; use a bare block instead", ifte.at)
            }
        }
        if (postfix_conditionals && ifte.if_false == null) {
            var then_expr = get_ptr(ifte.if_true)
            if (then_expr is ExprBlock) {
                var blk = then_expr as ExprBlock
                if (length(blk.list) == 1 && length(blk.finalList) == 0) {
                    var stmt = get_ptr(blk.list[0])
                    if (stmt is ExprReturn || stmt is ExprBreak || stmt is ExprContinue) {
                        // Skip if already on one line (already postfix or one-liner)
                        if (ifte.at.line != stmt.at.line) {
                            self->style_warning("STYLE005: single-statement if can use postfix form; e.g. return val if (cond)", ifte.at)
                        }
                    }
                }
            }
        }
    }

    // --- helpers ---

    def is_string_rtti_comparison(expr : Expression?) : bool {
        //! Returns true if expr is string(x.__rtti) pattern.
        return false if (expr == null)
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = get_ptr((inner as ExprRef2Value).subexpr)
        }
        return false if (inner == null || !(inner is ExprCall))
        var call = inner as ExprCall
        if (call.name != "string" || length(call.arguments) < 1) {
            return false
        }
        var arg = get_ptr(call.arguments[0])
        if (arg is ExprRef2Value) {
            arg = get_ptr((arg as ExprRef2Value).subexpr)
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

def public style_lint(prog : ProgramPtr; compile_time_errors : bool; postfix_conditionals : bool = false) : int {
    //! Runs the style lint visitor on the compiled program.
    //! Returns the number of warnings found.
    var astVisitor = new StyleLintVisitor(compile_time_errors = compile_time_errors, postfix_conditionals = postfix_conditionals)
    var inscope astVisitorAdapter <- make_visitor(*astVisitor)
    visit_with_generics(prog, astVisitorAdapter)
    let count = astVisitor.warning_count
    unsafe { delete astVisitor; }
    return count
}

def public style_lint_collect(prog : ProgramPtr; var warnings : array<string>; postfix_conditionals : bool = false) : int {
    //! Runs the style lint visitor and collects warnings as strings.
    //! Returns the number of warnings found.
    var astVisitor = new StyleLintVisitor(compile_time_errors = false, collect_warnings = true, postfix_conditionals = postfix_conditionals)
    var inscope astVisitorAdapter <- make_visitor(*astVisitor)
    visit_with_generics(prog, astVisitorAdapter)
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
        style_lint(prog, true)
        return true
    }
}
