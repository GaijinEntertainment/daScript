options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options strict_smart_pointers = true

module perf_lint shared private

//! Performance lint module.
//!
//! Detects common performance anti-patterns in daslang code at compile time.
//! When this module is required, a lint pass runs after compilation and reports
//! warnings as ``CompilationError::runtime_macro_performance`` (error code 31208).
//!
//! Rules:
//!   PERF001 — string += in loop (O(n²))
//!   PERF002 — character_at in loop with loop variable index (O(n) per call)
//!   PERF003 — character_at anywhere (info: O(n) per call)
//!   PERF004 — string interpolation reassignment in loop (O(n²))
//!   PERF005 — length(string) in while condition (strlen each iteration)
//!   PERF006 — push/emplace in loop without prior reserve()
//!   PERF007 — unnecessary string(das_string) in comparison
//!   PERF008 — unnecessary get_ptr() for is/as type checks
//!   PERF009 — redundant move-init variable immediately returned
//!   PERF010 — unnecessary get_ptr() for null comparison
//!   PERF011 — unnecessary get_ptr() for field access
//!   PERF012 — string(das_string) passed to strings module function
//!   PERF013 — a += 1 / a -= 1 — use a++ / a--
//!   PERF014 — closed-interval char-class range — see is_alpha / is_alnum / is_number etc.
//!   PERF015 — ternary min/max — use min(a, b) / max(a, b)
//!   PERF016 — ternary abs — use abs(x)
//!   PERF017 — length(s) == 0 / != 0 — use empty(s) / !empty(s)
//!   PERF018 — for (i in range(length(arr))) where i only indexes arr — use 'for (c in arr)'

require daslib/ast_boost
require strings

// ---------------------------------------------------------------------------
// Visitor
// ---------------------------------------------------------------------------

struct VarStackEntry {
    @do_not_delete v : Variable?
    depth : int
    is_iter : bool
}

struct Perf018State {
    //! PERF018 per-for-loop candidate. Each enclosing `for` pushes one; `matched`
    //! flips true once we observe `for (i in range(length(V)))` with exactly one
    //! source and one iter var, where V is a bare Variable. We push a sentinel
    //! for non-matching loops too so visitExprFor can pop unconditionally and
    //! iter_var refs across nesting attribute to the right frame.
    //! `array<T>` requires T trivially-storable, so the owning ExprFor pointer
    //! lives in a parallel `perf018_for_exprs` array.
    @do_not_delete iter_var : Variable?
    @do_not_delete arr_var : Variable?
    qualified_count : int
    sources_seen : int
    vars_seen : int
    disqualified : bool
    matched : bool
}

class PerfLintVisitor : AstVisitor {
    compile_time_errors : bool
    // variable + loop tracking
    loop_depth : int = 0
    in_closure : int = 0
    var_stack : array<VarStackEntry>
    scope_stack : array<int>
    // while tracking
    in_while_cond : bool = false
    // counter-based detection state
    in_character_at_call : int = 0
    @do_not_delete current_character_at : ExprCall?
    in_length_while_call : int = 0
    in_string_builder_check : int = 0
    @do_not_delete string_builder_target_var : Variable?
    @do_not_delete string_builder_save_stack : array<Variable?>
    // reported character_at locations (to avoid duplicate PERF002+PERF003)
    @do_not_delete reported_character_at : array<ExprCall?>
    // PERF006: path keys for arrays that had reserve() called
    reserved_paths : array<string>
    // PERF006: if-depth per scope — push(0) on scope entry, pop on exit
    if_depth_stack : array<int>
    // current function being visited (for inferStack reporting)
    @do_not_delete current_function : Function?
    in_template : bool = false
    // PERF006: known-length loop tracking
    known_length_loop_depth : int = 0
    current_for_known_length : bool = true
    // PERF006: suppress when loop has break/continue (unpredictable iteration count)
    loop_has_early_exit : array<bool>
    // PERF009: track last move-initialized variable for redundant return detection
    @do_not_delete pending_move_var : Variable?
    @safe_when_uninitialized pending_move_at : LineInfo
    // PERF018: per-for-loop index-only candidates; arr_path (string) and the
    // owning ExprFor (for `.at` at warn time) live in parallel arrays kept in
    // lockstep with perf018_stack. at_qualifying_pushes records how many
    // increments each in-flight ExprAt added to in_qualifying_idx so visitExprAt
    // can undo them.
    perf018_stack : array<Perf018State>
    perf018_paths : array<string>
    @do_not_delete perf018_for_exprs : array<ExprFor?>
    in_qualifying_idx : int = 0
    at_qualifying_pushes : array<int>
    warning_count : int = 0
    // collection mode — when true, warnings are appended to `warnings` instead of to_log
    collect_warnings : bool = false
    warnings : array<string>
    // dedup: reported (file_hash, line, column) to suppress duplicate warnings from generic instantiations
    reported_locations : array<uint64>
    def PerfLintVisitor() {
        pass
    }

    def location_key(at : LineInfo) : uint64 {
        return uint64(at.line) | (uint64(at.column) << uint64(20))
    }

    def is_suppressed(text : string; at : LineInfo) : bool {
        if (at.fileInfo == null || at.line == 0u) {  //
            return false
        }
        // extract PERFxxx code from warning text (e.g. "PERF003: ...")
        let colon = find(text, ":")
        if (colon < 0) {  //
            return false
        }
        let code = slice(text, 0, colon)
        var found = false
        get_file_source_line(at.fileInfo, at.line) $(line) {
            // look for "// nolint:PERFxxx" or "// nolint(PERFxxx)" in a comment
            let nolint_pos = find(line, "nolint")
            if (nolint_pos >= 0) {
                found = find(line, code, nolint_pos) >= 0
            }
        }
        return found
    }

    def perf_warning(text : string; at : LineInfo) : void {
        // Suppress in templates and inside macro-generated functions —
        // the user didn't write that code.
        if (in_template  //
            || (current_function != null && current_function.flags.generated)) return
        // Deduplicate warnings — same source location reported once, regardless of generic instantiations
        let key = self->location_key(at)
        if (reported_locations |> has_value(key)) {  //
            return
        }
        reported_locations |> push(key)
        // Check inline suppression: // PERFxxx on the same line
        if (self->is_suppressed(text, at)) {  //
            return
        }
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
            compiling_program() |> macro_performance_warning(at, msg)
        } elif (collect_warnings) {
            warnings |> push("performance warning: {msg} at {describe(at)}")
        } else {
            to_log(LOG_WARNING, "performance warning: {msg} at {describe(at)}\n")
        }
    }

    // --- variable scope helpers ---

    def is_loop_variable(v : Variable?) : bool {
        if (v == null) {  //
            return false
        }
        for (entry in var_stack) {
            if (entry.v == v && entry.is_iter) {  //
                return true
            }
        }
        return false
    }

    def is_defined_outside_loop(v : Variable?) : bool {
        if (v == null || loop_depth == 0 || self->is_loop_variable(v)) {  //
            return false
        }
        for (entry in var_stack) {
            if (entry.v == v) {  //
                return entry.depth < loop_depth
            }
        }
        // not on stack — function argument or global — outside any loop
        return true
    }

    // --- expression helpers ---

    def is_character_at_zero(expr : ExprCall?) : bool {
        if (length(expr.arguments) >= 2) {
            var idx = expr.arguments[1]
            if (idx is ExprConstInt && (idx as ExprConstInt).value == 0) {  //
                return true
            }
        }
        return false
    }

    def is_array_func(expr : ExprCall?; fname : string) : bool {
        // Builtin generics (push, reserve, etc.) have fromGeneric set.
        // Verify first argument is an array type.
        if (expr.func == null || expr.func.fromGeneric == null  //
            || expr.func.fromGeneric.name != fname || empty(expr.arguments)) return false
        let arg = expr.arguments[0]
        return arg._type != null && arg._type.baseType == Type.tArray
    }

    def find_expr_path(var expr : Expression?; var path : string&) : Variable? {
        //! Walks expression chains (field access, deref, index, etc.) to find the
        //! root variable, building a path string to distinguish foo.a from foo.b.
        //! Returns null if the chain can't be resolved (call results, Op3, etc.).
        return null if (expr == null)
        if (expr is ExprVar) {
            var evar = expr as ExprVar
            return null if (evar.variable == null)
            return evar.variable
        }
        if (expr is ExprRef2Value) {  //
            return self->find_expr_path((expr as ExprRef2Value.subexpr), path)
        }
        if (expr is ExprField) {
            var field = expr as ExprField
            path := ".{field.name}{path}"
            return self->find_expr_path(field.value, path)
        }
        if (expr is ExprSafeField) {
            var field = expr as ExprSafeField
            path := ".{field.name}{path}"
            return self->find_expr_path(field.value, path)
        }
        if (expr is ExprAt) {
            var at = expr as ExprAt
            path := "[*]{path}"
            return self->find_expr_path(at.subexpr, path)
        }
        if (expr is ExprSafeAt) {
            var at = expr as ExprSafeAt
            path := "[*]{path}"
            return self->find_expr_path(at.subexpr, path)
        }
        if (expr is ExprSwizzle) {
            var swiz = expr as ExprSwizzle
            return self->find_expr_path(swiz.value, path)
        }
        if (expr is ExprCast) {
            var ca = expr as ExprCast
            return self->find_expr_path(ca.subexpr, path)
        }
        if (expr is ExprRef2Ptr) {
            var rr = expr as ExprRef2Ptr
            return self->find_expr_path(rr.subexpr, path)
        }
        if (expr is ExprPtr2Ref) {
            var rr = expr as ExprPtr2Ref
            return self->find_expr_path(rr.subexpr, path)
        }
        // Bail on everything else (Op3, NullCoalescing, calls, etc.)
        return null
    }

    def make_path_key(v : Variable?; path : string) : string {
        return "{intptr(v)}{path}"
    }

    def find_string_var_from_expr(expr : Expression?) : Variable? {
        return null if (expr == null)
        // Unwrap ExprRef2Value (compiler inserts these for value-type reads)
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        return null if (inner == null || !(inner is ExprVar))
        var evar = inner as ExprVar
        return null if (evar.variable == null || evar.variable._type == null)
        if (evar.variable._type.baseType == Type.tString) {  //
            return evar.variable
        }
        return null
    }

    // --- PERF007/PERF008 helpers ---

    def is_string_of_das_string(expr : Expression?) : bool {
        //! Returns true if expr is a string(X) call where X is das_string.
        return false if (expr == null)
        var inner = expr
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        return false if (inner == null || !(inner is ExprCall))
        var call = inner as ExprCall
        // After inference, string() calls may have FakeContext/FakeLineInfo extra args.
        // Check the function name and first argument type.
        if (empty(call.arguments) || call.name != "string") {  //
            return false
        }
        var arg = call.arguments[0]
        if (arg._type == null || arg._type.baseType != Type.tHandle) {  //
            return false
        }
        return arg._type.annotation != null && arg._type.annotation.name == "das_string"
    }

    def is_get_ptr_of_smart_ptr(expr : Expression?) : bool {
        //! Returns true if expr is a X call where X is a smart_ptr.
        return false if (expr == null || !(expr is ExprCall))
        var call = expr as ExprCall
        if (call.func == null || call.func.fromGeneric == null) {  //
            return false
        }
        return call.func.fromGeneric.name == "get_ptr"
    }

    def is_get_ptr_vs_null(maybe_get_ptr : Expression?; maybe_null : Expression?) : bool {
        //! Returns true if maybe_get_ptr is get_ptr() and maybe_null is null.
        if (!self->is_get_ptr_of_smart_ptr(maybe_get_ptr) || maybe_null == null) {  //
            return false
        }
        if (maybe_null is ExprConstPtr) {
            var cptr = maybe_null as ExprConstPtr
            return cptr.value == null
        }
        return false
    }

    // --- generic constant / structural helpers (PERF013-017) ---

    def is_const_zero(expr : Expression? const) : bool {
        if (expr == null) {  //
            return false
        }
        if (expr is ExprConstInt) {  //
            return (expr as ExprConstInt).value == 0
        }
        if (expr is ExprConstUInt) {  //
            return (expr as ExprConstUInt).value == 0u
        }
        if (expr is ExprConstInt64) {  //
            return (expr as ExprConstInt64).value == 0l
        }
        if (expr is ExprConstUInt64) {  //
            return (expr as ExprConstUInt64).value == 0ul
        }
        if (expr is ExprConstFloat) {  //
            return (expr as ExprConstFloat).value == 0.0f
        }
        if (expr is ExprConstDouble) {  //
            return (expr as ExprConstDouble).value == 0.0lf
        }
        return false
    }

    def is_const_one(expr : Expression? const) : bool {
        if (expr == null) {  //
            return false
        }
        if (expr is ExprConstInt) {  //
            return (expr as ExprConstInt).value == 1
        }
        if (expr is ExprConstUInt) {  //
            return (expr as ExprConstUInt).value == 1u
        }
        if (expr is ExprConstInt64) {  //
            return (expr as ExprConstInt64).value == 1l
        }
        if (expr is ExprConstUInt64) {  //
            return (expr as ExprConstUInt64).value == 1ul
        }
        if (expr is ExprConstFloat) {  //
            return (expr as ExprConstFloat).value == 1.0f
        }
        if (expr is ExprConstDouble) {  //
            return (expr as ExprConstDouble).value == 1.0lf
        }
        return false
    }

    def is_const_neg_one(expr : Expression? const) : bool {
        if (expr == null) {  //
            return false
        }
        if (expr is ExprConstInt) {  //
            return (expr as ExprConstInt).value == -1
        }
        if (expr is ExprConstInt64) {  //
            return (expr as ExprConstInt64).value == -1l
        }
        if (expr is ExprConstFloat) {  //
            return (expr as ExprConstFloat).value == -1.0f
        }
        if (expr is ExprConstDouble) {  //
            return (expr as ExprConstDouble).value == -1.0lf
        }
        return false
    }

    def expr_equal_struct(a : Expression? const; b : Expression? const; require_pure : bool = true) : bool {
        //! Structural equality via describe(). With `require_pure=true` (default),
        //! returns false when either side has side effects — protects rules that
        //! suggest collapsing duplicated subexpressions (PERF014/015/016) from
        //! silently changing evaluation count.
        if ((a == null || b == null)  //
            || (require_pure && (!a.flags.noSideEffects || !b.flags.noSideEffects))) return false
        return describe(a) == describe(b)
    }

    def is_workhorse_numeric(t : TypeDecl?) : bool {
        //! True if the type is one of the six numeric workhorse scalars (int, uint,
        //! int64, uint64, float, double). Vectors/bitfields/enums do NOT qualify.
        if (t == null || !empty(t.dim)) {  //
            return false
        }
        let bt = t.baseType
        return (bt == Type.tInt || bt == Type.tUInt
                || bt == Type.tInt64 || bt == Type.tUInt64
                || bt == Type.tFloat || bt == Type.tDouble)
    }

    def is_collection_length_call(expr : Expression? const) : bool {
        //! True for length(string) / length(das_string) / length(array) / length(table).
        //! Excludes math::length(float2/3/4) which returns vector magnitude.
        if (expr == null || !(expr is ExprCall)) {  //
            return false
        }
        let call = expr as ExprCall
        if (call.func == null || call.func.name != "length") {  //
            return false
        }
        let modname = string(call.func._module.name)
        return modname == "strings" || modname == "$" || modname == "builtin"
    }

    // --- function tracking ---

    def override preVisitFunction(var fn : FunctionPtr) : void {
        current_function = fn
        in_template = fn.moreFlags.isTemplate
    }

    def override visitFunction(var fn : FunctionPtr) : FunctionPtr {
        current_function = null
        in_template = false
        return <- fn
    }

    // --- scope tracking ---

    def override preVisitExprBlock(blk : ExprBlock?) : void {
        if (blk.blockFlags.isClosure) {
            in_closure++
            string_builder_save_stack |> push(string_builder_target_var)
            string_builder_target_var = null
        }
        scope_stack |> push(length(var_stack))
    }

    def override visitExprBlock(var blk : ExprBlock?) : ExpressionPtr {
        if (blk.blockFlags.isClosure) {
            in_closure--
            if (!empty(string_builder_save_stack)) {
                string_builder_target_var = string_builder_save_stack |> back()
                string_builder_save_stack |> pop()
            }
        }
        if (!empty(scope_stack)) {
            var_stack |> resize(scope_stack |> back())
            scope_stack |> pop()
        }
        return <- blk
    }

    // --- variable declaration tracking ---

    def override preVisitExprLetVariable(expr : ExprLet?; var v : VariablePtr; last : bool) : void {
        if (in_closure == 0) {
            var_stack |> push(VarStackEntry(v = v, depth = loop_depth, is_iter = false))
        }
        // PERF009: track last move-initialized variable (only explicit `var x <- expr`)
        if (last && v.flags.init_via_move && v.init != null) {
            var dominated_by_clone = false
            // Exclude compiler-lowered `:=` (clone_to_move) on non-copyable types
            var ini = v.init
            if (ini is ExprCall) {
                let ecall = ini as ExprCall
                if (ecall.func != null && ecall.func.fromGeneric != null) {
                    dominated_by_clone = ecall.func.fromGeneric.name == "clone_to_move"
                }
            }
            if (!dominated_by_clone) {
                pending_move_var = v
                pending_move_at = expr.at
            }
        }
    }

    // --- PERF009: redundant move-init variable before return ---

    def override preVisitExprBlockExpression(blk : ExprBlock?; expr : ExpressionPtr) : void {
        if (pending_move_var != null) {
            var e = expr
            if (e is ExprReturn) {
                var ret = e as ExprReturn
                if (ret.returnFlags.moveSemantics && ret.subexpr != null) {
                    var sub = ret.subexpr
                    if (sub is ExprRef2Value) {
                        sub = (sub as ExprRef2Value.subexpr)
                    }
                    if (sub is ExprVar && (sub as ExprVar.variable) == pending_move_var) {
                        self->perf_warning("PERF009: redundant move into variable immediately returned; use return <- expr directly", pending_move_at)
                    }
                }
            }
            pending_move_var = null
        }
    }

    // --- loop tracking ---

    def override preVisitExprFor(expr : ExprFor?) : void {
        if (in_closure == 0) {
            scope_stack |> push(length(var_stack))
            loop_depth++
            if_depth_stack |> push(0)
            current_for_known_length = true
            loop_has_early_exit |> push(false)
            self->perf018_push_sentinel(expr)
        }
    }

    def is_known_length_source(src : Expression?) : bool {
        if (src == null || src._type == null) {  //
            return false
        }
        let bt = src._type.baseType
        // fixed arrays have dim > 0; otherwise we accept any baseType whose
        // length is determined at the start of the loop.
        return (bt == Type.tArray || bt == Type.tString
                || bt == Type.tRange || bt == Type.tRange64
                || bt == Type.tURange || bt == Type.tURange64
                || !empty(src._type.dim))
    }

    def override preVisitExprForSource(expr : ExprFor?; src : ExpressionPtr; last : bool) : void {
        if (in_closure == 0 && !self->is_known_length_source(src)) {
            current_for_known_length = false
        }
        if (in_closure == 0) {
            self->perf018_record_source(src)
        }
    }

    def override preVisitExprForBody(expr : ExprFor?) : void {
        if (in_closure == 0 && current_for_known_length) {
            known_length_loop_depth++
        }
        if (in_closure == 0) {
            self->perf018_finalize_on_body()
        }
    }

    def override preVisitExprForVariable(expr : ExprFor?; var v : VariablePtr; last : bool) : void {
        if (in_closure == 0) {
            var_stack |> push(VarStackEntry(v = v, depth = loop_depth - 1, is_iter = true))
            self->perf018_record_var(v)
        }
    }

    def override visitExprFor(var expr : ExprFor?) : ExpressionPtr {
        if (in_closure == 0) {
            self->perf018_pop_and_check()
            loop_depth--
            if (current_for_known_length) {
                known_length_loop_depth--
            }
            if (!empty(scope_stack)) {
                var_stack |> resize(scope_stack |> back())
                scope_stack |> pop()
            }
            if (!empty(if_depth_stack)) {
                if_depth_stack |> pop()
            }
            if (!empty(loop_has_early_exit)) {
                loop_has_early_exit |> pop()
            }
        }
        return <- expr
    }

    // --- PERF018: for (i in range(length(arr))) where i only indexes arr ---

    def perf018_push_sentinel(expr : ExprFor?) : void {
        //! Always pushed in preVisitExprFor — keeps the stack in lockstep with
        //! for-loop nesting so visitExprFor can pop unconditionally. Matched/
        //! disqualified status is filled in by the per-source/per-variable hooks.
        var state = Perf018State()  //
        state.disqualified = true   // flips false only when source+iter both match
        state.matched = false
        perf018_stack    |> push(state)
        unsafe {
            perf018_for_exprs |> push(reinterpret<ExprFor?>(expr))
        }
    }

    def perf018_pop_and_check() : void {
        if (empty(perf018_stack)) {  //
            return
        }
        let top_idx = length(perf018_stack) - 1
        let state = perf018_stack[top_idx]
        let owning = perf018_for_exprs[top_idx]
        perf018_stack    |> pop()
        perf018_for_exprs |> pop()
        if (state.matched && !state.disqualified && state.qualified_count > 0 && owning != null) {
            self->perf_warning("PERF018: for (i in range(length(X))) where i only indexes X; use 'for (c in X)' for direct iteration", owning.at)
        }
    }

    def perf018_bare_var_of(var e : ExpressionPtr) : Variable? {
        //! Returns the underlying Variable if `e` (after one optional Ref2Value
        //! peel) is a bare ExprVar. Null otherwise — field paths, calls, etc. all
        //! intentionally disqualify PERF018's arr resolution.
        var inner = e
        if (inner is ExprRef2Value) {
            inner = (inner as ExprRef2Value.subexpr)
        }
        return null if (inner == null || !(inner is ExprVar))
        var evar = inner as ExprVar
        return evar.variable
    }

    def perf018_record_source(src : ExpressionPtr) : void {
        //! Called from preVisitExprForSource. Peels `range(length(<bareVar>))`
        //! (with one optional ExprCast on either layer) and, when matched, stamps
        //! the candidate's arr_var. Iter var arrives via perf018_record_var.
        if (empty(perf018_stack)) {  //
            return
        }
        let top_idx = length(perf018_stack) - 1
        var state = perf018_stack[top_idx]
        state.sources_seen++
        if (state.sources_seen > 1) {
            // Multi-source for loops can't be turned into a single for-each.
            state.disqualified = true
            perf018_stack[top_idx] = state
            return
        }
        var s = src
        if (s is ExprCast) {
            s = (s as ExprCast.subexpr)
        }
        if (s == null || !(s is ExprCall)) {
            perf018_stack[top_idx] = state
            return
        }
        var rng = s as ExprCall
        if (rng.func == null || string(rng.func.name) != "range"  //
            || length(rng.arguments) != 1) {
            perf018_stack[top_idx] = state
            return
        }
        var inner = rng.arguments[0]
        if (inner is ExprCast) {
            inner = (inner as ExprCast.subexpr)
        }
        if (inner == null || !(inner is ExprCall)) {
            perf018_stack[top_idx] = state
            return
        }
        var len = inner as ExprCall
        if (len.func == null || string(len.func.name) != "length"  //
            || length(len.arguments) != 1) {
            perf018_stack[top_idx] = state
            return
        }
        var av = self->perf018_bare_var_of(len.arguments[0])
        if (av == null) {
            perf018_stack[top_idx] = state
            return
        }
        state.arr_var = av
        perf018_stack[top_idx] = state
    }

    def perf018_record_var(var v : Variable?) : void {
        //! Called from preVisitExprForVariable. Stamps iter_var on the top frame.
        //! When a single iter var pairs with a single recognized source, marks
        //! the candidate matched + un-disqualified.
        if (empty(perf018_stack) || v == null) {  //
            return
        }
        let top_idx = length(perf018_stack) - 1
        var state = perf018_stack[top_idx]
        state.vars_seen++
        if (state.vars_seen > 1) {
            state.disqualified = true
            perf018_stack[top_idx] = state
            return
        }
        state.iter_var = v
        perf018_stack[top_idx] = state
    }

    def perf018_finalize_on_body() : void {
        //! Called from preVisitExprForBody — fires once per for, after every
        //! preVisitExprForSource and preVisitExprForVariable. Flips matched=true
        //! iff the loop had exactly one source we recognized (arr_var set) and
        //! exactly one iter var (iter_var set).
        if (empty(perf018_stack)) {  //
            return
        }
        let top_idx = length(perf018_stack) - 1
        var state = perf018_stack[top_idx]
        if (state.sources_seen == 1 && state.vars_seen == 1
            && state.arr_var != null && state.iter_var != null) {
            state.matched = true
            state.disqualified = false
        }
        perf018_stack[top_idx] = state
    }

    def perf018_at_matches(var at : ExprAt?; frame_idx : int) : bool {
        //! True when `at.subexpr[at.index]` accesses the frame's arr_var (bare)
        //! and indexes by the frame's iter_var with no arithmetic. Conservative
        //! by design — field paths, casts on the subexpr, etc. don't qualify.
        if (frame_idx < 0 || frame_idx >= length(perf018_stack)) {  //
            return false
        }
        let state = perf018_stack[frame_idx]
        if (state.disqualified || !state.matched  //
            || state.arr_var == null || state.iter_var == null) return false
        let av = self->perf018_bare_var_of(at.subexpr)
        if (av != state.arr_var) {  //
            return false
        }
        var idx = at.index
        if (idx is ExprRef2Value) {
            idx = (idx as ExprRef2Value.subexpr)
        }
        return idx != null && idx is ExprVar && (idx as ExprVar).variable == state.iter_var
    }

    def perf018_on_at(var expr : ExprAt?) : void {
        var pushes = 0
        let n = length(perf018_stack)
        for (i in range(n)) {
            if (self->perf018_at_matches(expr, i)) {
                var st = perf018_stack[i]
                st.qualified_count++
                perf018_stack[i] = st
                pushes++
            }
        }
        if (pushes > 0) {
            in_qualifying_idx += pushes
        }
        at_qualifying_pushes |> push(pushes)
    }

    def perf018_off_at() : void {
        if (empty(at_qualifying_pushes)) {  //
            return
        }
        let n = at_qualifying_pushes |> back()
        at_qualifying_pushes |> pop()
        in_qualifying_idx -= n
    }

    def perf018_on_var(v : Variable?) : void {
        //! Called from preVisitExprVar: a bare iter_var reference outside any
        //! qualifying ExprAt index slot disqualifies the matching frame.
        if (v == null || empty(perf018_stack) || in_qualifying_idx > 0) {  //
            return
        }
        let n = length(perf018_stack)
        for (i in range(n)) {
            var st = perf018_stack[i]
            if (st.matched && !st.disqualified && st.iter_var == v) {
                st.disqualified = true
                perf018_stack[i] = st
            }
        }
    }

    def override preVisitExprAt(var expr : ExprAt?) : void {
        if (in_closure == 0 && expr != null) {
            self->perf018_on_at(expr)
        }
    }

    def override visitExprAt(var expr : ExprAt?) : ExpressionPtr {
        if (in_closure == 0) {
            self->perf018_off_at()
        }
        return <- expr
    }

    def override preVisitExprWhile(expr : ExprWhile?) : void {
        if (in_closure == 0) {
            scope_stack |> push(length(var_stack))
            loop_depth++
            if_depth_stack |> push(0)
            loop_has_early_exit |> push(false)
            in_while_cond = true
        }
    }

    def override preVisitExprWhileBody(expr : ExprWhile?; body : ExpressionPtr) : void {
        in_while_cond = false
    }

    def override visitExprWhile(var expr : ExprWhile?) : ExpressionPtr {
        if (in_closure == 0) {
            loop_depth--
            if (!empty(scope_stack)) {
                var_stack |> resize(scope_stack |> back())
                scope_stack |> pop()
            }
            if (!empty(if_depth_stack)) {
                if_depth_stack |> pop()
            }
            if (!empty(loop_has_early_exit)) {
                loop_has_early_exit |> pop()
            }
        }
        in_while_cond = false
        return <- expr
    }

    // --- break/continue tracking (for PERF006 early-exit suppression) ---

    def override preVisitExprBreak(expr : ExprBreak?) : void {
        if (in_closure == 0 && !empty(loop_has_early_exit)) {
            loop_has_early_exit[length(loop_has_early_exit) - 1] = true
        }
    }

    def override preVisitExprContinue(expr : ExprContinue?) : void {
        if (in_closure == 0 && !empty(loop_has_early_exit)) {
            loop_has_early_exit[length(loop_has_early_exit) - 1] = true
        }
    }

    // --- if-depth tracking (for PERF006 conditional suppression) ---

    def override preVisitExprIfThenElse(ifte : ExprIfThenElse?) : void {
        if (!empty(if_depth_stack)) {
            if_depth_stack[length(if_depth_stack) - 1]++
        }
    }

    def override visitExprIfThenElse(var ifte : ExprIfThenElse?) : ExpressionPtr {
        if (!empty(if_depth_stack)) {
            if_depth_stack[length(if_depth_stack) - 1]--
        }
        return <- ifte
    }

    // --- PERF001: string += in loop ---
    // --- PERF007: unnecessary string(das_string) in comparison ---
    // --- PERF008: unnecessary get_ptr() for is/as ---

    def override preVisitExprOp2(expr : ExprOp2?) : void {
        // No in_closure check needed — loop_depth doesn't increment inside
        // closure bodies (see preVisitExprFor/While), so PERF001's loop check
        // is already correctly scoped. The other rules below are syntactic
        // patterns that misbehave identically inside or outside closures.
        if (loop_depth > 0 && expr.op == "+=") {
            let v = self->find_string_var_from_expr(expr.left)
            if (v != null && self->is_defined_outside_loop(v)) {
                self->perf_warning("PERF001: string += in loop creates O(n^2) allocations; use build_string() instead", expr.at)
            }
        }
        // PERF007: string(das_string) == or != comparison
        if (expr.op == "==" || expr.op == "!=") {
            let left_is = self->is_string_of_das_string(expr.left)
            let right_is = self->is_string_of_das_string(expr.right)
            if (left_is || right_is) {
                self->perf_warning("PERF007: string(das_string) in comparison is unnecessary; das_string supports == and != directly", expr.at)
            }
        }
        // PERF008: x.__rtti == "Type" is expanded from `x is Type`
        if (expr.op == "==") {
            var left = expr.left
            // Unwrap ExprRef2Value — compiler inserts these for value-type reads
            if (left is ExprRef2Value) {
                left = (left as ExprRef2Value.subexpr)
            }
            if (left is ExprField) {
                var field = left as ExprField
                if (field.name == "__rtti" && self->is_get_ptr_of_smart_ptr(field.value)) {
                    self->perf_warning("PERF008: get_ptr() is unnecessary for is/as; smart_ptr supports type checks directly", expr.at)
                }
            }
        }
        // PERF010: x == null or x != null
        if (self->is_get_ptr_vs_null(expr.left, expr.right) || self->is_get_ptr_vs_null(expr.right, expr.left)) {
            self->perf_warning("PERF010: get_ptr() is unnecessary for null comparison; smart_ptr supports == and != null directly", expr.at)
        }
        // PERF013: a += ±1 / a -= ±1 → a++ / a--
        // Derive the actual delta from (op, RHS) so each branch can suggest the
        // correct postfix operator. RHS is one of {+1, -1}; op flips the sign.
        if (expr.op == "+=" || expr.op == "-=") {
            if (expr.left != null && self->is_workhorse_numeric(expr.left._type)) {
                let rhs_one = self->is_const_one(expr.right)
                let rhs_neg = self->is_const_neg_one(expr.right)
                var delta = 0
                if (rhs_one) {
                    delta = expr.op == "+=" ? 1 : -1
                } elif (rhs_neg) {
                    delta = expr.op == "+=" ? -1 : 1
                }
                if (delta == 1) {
                    self->perf_warning("PERF013: '{expr.op} {rhs_one ? 1 : -1}' on a numeric scalar; use postfix '++' for a faster, idiomatic increment", expr.at)
                } elif (delta == -1) {
                    self->perf_warning("PERF013: '{expr.op} {rhs_one ? 1 : -1}' on a numeric scalar; use postfix '--' for a faster, idiomatic decrement", expr.at)
                }
            }
        }
        // PERF014: closed-interval char-class range check
        if (expr.op == "&&") {
            self->check_perf014_char_class(expr)
        }
        // PERF017: length(s) <op> 0 / 1 → empty(s) / !empty(s)
        if (expr.op == "==" || expr.op == "!=" || expr.op == "<" || expr.op == "<=" || expr.op == ">" || expr.op == ">=") {
            self->check_perf017_length_zero(expr)
        }
    }

    // --- PERF014: closed-interval char-class range checks ---

    def parse_range_leg(leg : Expression?; var v_out : Expression?&; var bound_out : int&; var is_hi_out : bool&) : bool {
        //! Parse `var <op> const` or `const <op> var` where op is `<=` or `>=`.
        //! Returns true and fills outputs with: the variable expression,
        //! the int bound, and whether this leg expresses an upper bound on var.
        if (leg == null || !(leg is ExprOp2)) {  //
            return false
        }
        var op2 = leg as ExprOp2
        let opname = string(op2.op)
        if (opname != "<=" && opname != ">=") {  //
            return false
        }
        if (op2.right is ExprConstInt && op2.left != null) {
            v_out = op2.left
            bound_out = (op2.right as ExprConstInt).value
            is_hi_out = opname == "<="
            return true
        }
        if (op2.left is ExprConstInt && op2.right != null) {
            v_out = op2.right
            bound_out = (op2.left as ExprConstInt).value
            is_hi_out = opname == ">="
            return true
        }
        return false
    }

    def is_known_char_class_range(lo : int; hi : int) : bool {
        //! Three closed ranges hint at the strings is_*() helpers.
        //! Hex extras ('a'..'f', 'A'..'F') are excluded — is_hex is broader.
        return ((lo == 48 && hi == 57)         // '0'..'9' — is_number
                || (lo == 97 && hi == 122)     // 'a'..'z' — is_alpha (lower half)
                || (lo == 65 && hi == 90))     // 'A'..'Z' — is_alpha (upper half)
    }

    def check_perf014_char_class(expr : ExprOp2?) : void {
        var va : Expression? = null
        var vb : Expression? = null
        var bound_a = 0
        var bound_b = 0
        var hi_a = false
        var hi_b = false
        // Both legs must parse, constrain the SAME expression, and split into
        // one upper / one lower bound.
        if (!self->parse_range_leg(expr.left, va, bound_a, hi_a)  //
            || !self->parse_range_leg(expr.right, vb, bound_b, hi_b)
            || !self->expr_equal_struct(va, vb)
            || hi_a == hi_b) return
        let lo = hi_a ? bound_b : bound_a
        let hi = hi_a ? bound_a : bound_b
        if (lo >= hi || !self->is_known_char_class_range(lo, hi)) {  //
            return
        }
        // Pick the helper that matches the detected range. Note: is_alpha covers
        // BOTH cases, so for a single-case range ('a'..'z' or 'A'..'Z') it's a
        // broader replacement, not an exact one — call that out so the user
        // doesn't replace lower-only with a both-cases check.
        if (lo == 48 && hi == 57) {
            self->perf_warning("PERF014: char-class range check '0'..'9' — use 'is_number(c)' from strings module", expr.at)
        } else {
            // 'a'..'z' or 'A'..'Z'
            let case_kind = (lo == 97) ? "lowercase" : "uppercase"
            self->perf_warning("PERF014: char-class range check ({case_kind} only) — consider 'is_alpha(c)' from strings module (note: is_alpha matches both cases, broader than this range)", expr.at)
        }
    }

    // --- PERF017: length(collection) <op> 0/1 → empty / !empty ---

    def peel_ref2value(e : Expression? const) : Expression? const {
        if (e != null && e is ExprRef2Value) {  //
            return (e as ExprRef2Value.subexpr)
        }
        return e
    }

    def const_int_value(e : Expression? const; var v : int&) : bool {
        let inner = self->peel_ref2value(e)
        if (inner == null) {  //
            return false
        }
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

    def check_perf017_length_zero(expr : ExprOp2?) : void {
        // Skip the canonical implementation of empty() in daslib/builtin.das —
        // empty(x) is literally `return length(x) == 0`, and the rule's whole job
        // is to suggest `empty(x)`. Self-flagging the suggestion's own body is
        // noise. Generic instantiations of empty<T> hit this path with
        // current_function.fromGeneric pointing at the source `empty`. Both arms
        // also gate on _module.name == "builtin" so unrelated user methods named
        // empty() (e.g. delegate.empty(), AOT cache empty()) keep getting linted.
        if (current_function != null) {
            let direct_match = (current_function.name == "empty"
                && current_function._module != null
                && current_function._module.name == "builtin")
            let generic_match = (current_function.fromGeneric != null
                && current_function.fromGeneric.name == "empty"
                && current_function.fromGeneric._module != null
                && current_function.fromGeneric._module.name == "builtin")
            if (direct_match || generic_match) {  //
                return
            }
        }
        let lhs = self->peel_ref2value(expr.left)
        let rhs = self->peel_ref2value(expr.right)
        var len_on_left = true
        if (self->is_collection_length_call(lhs)) {
            len_on_left = true
        } elif (self->is_collection_length_call(rhs)) {
            len_on_left = false
        } else {
            return
        }
        let const_side = len_on_left ? rhs : lhs
        var k = 0
        if (!self->const_int_value(const_side, k) || (k != 0 && k != 1)) {  //
            return
        }
        let opname = string(expr.op)
        // Canonicalize so that length is on the left; flip op if needed.
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
        // Map (canon_op, k) → empty / !empty
        //   k=0: ==, <=             → empty
        //        !=, >              → !empty
        //   k=1: <                  → empty   (length < 1 means length == 0)
        //        >=                 → !empty  (length >= 1 means length > 0)
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
        // Diagnostic uses canon_op so Yoda forms (e.g. `1 <= length(x)`) print
        // the canonical equivalent (`length(x) >= 1`), matching what we matched.
        if (is_empty_pattern) {
            self->perf_warning("PERF017: 'length(x) {canon_op} {k}' — use 'empty(x)' (avoids unnecessary strlen on strings)", expr.at)
        } elif (is_nonempty_pattern) {
            self->perf_warning("PERF017: 'length(x) {canon_op} {k}' — use '!empty(x)' (avoids unnecessary strlen on strings)", expr.at)
        }
    }

    // --- PERF015 / PERF016: ternary min/max/abs ---

    def override preVisitExprOp3(expr : ExprOp3?) : void {
        if (in_closure > 0 || expr.op != "?"  //
            || expr.subexpr == null || !(expr.subexpr is ExprOp2)) return
        var cmp = expr.subexpr as ExprOp2
        let cop = string(cmp.op)
        if ((cop != "<" && cop != "<=" && cop != ">" && cop != ">=")  //
            || expr.left == null || expr.right == null) return
        let t_branch = expr.left
        let f_branch = expr.right
        // PERF016 first — a ternary that matches abs may also superficially match
        // min/max if x and -x ever describe-equal, but they can't. So order is fine.
        if (self->try_perf016_abs(expr, cmp, t_branch, f_branch, cop)) {  //
            return
        }
        self->try_perf015_minmax(expr, cmp, t_branch, f_branch, cop)
    }

    def try_perf015_minmax(expr : ExprOp3?; cmp : ExprOp2?; t_branch, f_branch : Expression? const; cop : string) : bool {
        let cl = cmp.left
        let cr = cmp.right
        if (cl == null || cr == null) {  //
            return false
        }
        // ternary branches must be a permutation of (cl, cr)
        let tcl = self->expr_equal_struct(t_branch, cl)
        let tcr = self->expr_equal_struct(t_branch, cr)
        let fcl = self->expr_equal_struct(f_branch, cl)
        let fcr = self->expr_equal_struct(f_branch, cr)
        let same_order = tcl && fcr   // T==L, F==R
        let swap_order = tcr && fcl   // T==R, F==L
        if (!same_order && !swap_order) {  //
            return false
        }
        // Mapping
        //   <  / <=  : T==L,F==R → min;  T==R,F==L → max
        //   >  / >=  : T==L,F==R → max;  T==R,F==L → min
        var is_min = false
        if (cop == "<" || cop == "<=") {
            is_min = same_order
        } else {
            is_min = swap_order
        }
        let which = is_min ? "min" : "max"
        self->perf_warning("PERF015: ternary {which} — use '{which}(a, b)' from math module", expr.at)
        return true
    }

    def try_perf016_abs(expr : ExprOp3?; cmp : ExprOp2?; t_branch, f_branch : Expression? const; cop : string) : bool {
        // One side of the comparison must be const-zero
        var x_on_left = false
        if (self->is_const_zero(cmp.right)) {
            x_on_left = true
        } elif (self->is_const_zero(cmp.left)) {
            x_on_left = false
        } else {
            return false
        }
        let x_expr = x_on_left ? cmp.left : cmp.right
        if (x_expr == null) {  //
            return false
        }
        // Identify negated branch (-x) and plain branch (x)
        var neg_then = false
        var ok = false
        if (self->is_neg_of(t_branch, x_expr) && self->expr_equal_struct(f_branch, x_expr)) {
            neg_then = true
            ok = true
        } elif (self->expr_equal_struct(t_branch, x_expr) && self->is_neg_of(f_branch, x_expr)) {
            neg_then = false
            ok = true
        }
        if (!ok) {  //
            return false
        }
        // Determine canonical sign of x in comparison: x < 0 or x > 0 etc.
        // Reduce to: "is x's value-relation `negative` or `positive`" in the true branch
        // Forms that produce abs:
        //   x < 0 → -x  (T branch is -x, x is on left)        — neg_then=true , x_on_left=true , cop in {<, <=}
        //   x > 0 → x   (T branch is x,  x is on left)        — neg_then=false, x_on_left=true , cop in {>, >=}
        //   0 > x → -x                                         — neg_then=true , x_on_left=false, cop in {>, >=}
        //   0 < x → x                                          — neg_then=false, x_on_left=false, cop in {<, <=}
        var is_abs = false
        if (x_on_left) {
            if ((cop == "<" || cop == "<=") && neg_then) {
                is_abs = true
            } elif ((cop == ">" || cop == ">=") && !neg_then) {
                is_abs = true
            }
        } else {
            if ((cop == ">" || cop == ">=") && neg_then) {
                is_abs = true
            } elif ((cop == "<" || cop == "<=") && !neg_then) {
                is_abs = true
            }
        }
        if (!is_abs) {  //
            return false
        }
        self->perf_warning("PERF016: ternary abs — use 'abs(x)' from math module", expr.at)
        return true
    }

    def is_neg_of(maybe_neg : Expression? const; ref : Expression? const) : bool {
        //! True when `maybe_neg` is `-ref` structurally.
        if (maybe_neg == null || !(maybe_neg is ExprOp1)) {  //
            return false
        }
        let op1 = maybe_neg as ExprOp1
        if (op1.op != "-") {  //
            return false
        }
        return self->expr_equal_struct(op1.subexpr, ref)
    }

    // --- PERF011: unnecessary get_ptr() for field access ---

    def override preVisitExprField(expr : ExprField?) : void {
        if (in_closure > 0) {  //
            return
        }
        if (self->is_get_ptr_of_smart_ptr(expr.value)) {
            self->perf_warning("PERF011: get_ptr() is unnecessary for field access; smart_ptr auto-dereferences", expr.at)
        }
    }

    def override preVisitExprSafeField(expr : ExprSafeField?) : void {
        if (in_closure > 0) {  //
            return
        }
        if (self->is_get_ptr_of_smart_ptr(expr.value)) {
            self->perf_warning("PERF011: get_ptr() is unnecessary for field access; smart_ptr auto-dereferences", expr.at)
        }
    }

    // --- PERF002 + PERF003: character_at (counter-based) ---

    def override preVisitExprCall(var expr : ExprCall?) : void {
        if (in_closure > 0 || expr.func == null) {  //
            return
        }
        if (expr.func.name == "character_at" && expr.func._module.name == "strings") {
            var ecall = expr
            // PERF002: start tracking — any loop var inside this call triggers warning
            if (loop_depth > 0) {
                in_character_at_call++
                current_character_at = ecall
            }
        }
        // PERF005: length(string) in while condition — start tracking
        if (in_while_cond && expr.func.name == "length" && expr.func._module.name == "strings") {
            in_length_while_call++
        }
    }

    def override visitExprCall(var expr : ExprCall?) : ExpressionPtr {
        if (expr.func == null) {  //
            return <- expr
        }
        // PERF003: character_at anywhere — fires even inside closures
        if (expr.func.name == "character_at" && expr.func._module.name == "strings") {
            var ecall = expr
            if (in_closure == 0 && loop_depth > 0 && in_character_at_call > 0) {
                in_character_at_call--
                current_character_at = null
            }
            // deferred so PERF002 can claim it first
            if (!reported_character_at |> has_value(ecall)) {
                if (self->is_character_at_zero(expr)) {
                    self->perf_warning("PERF003: character_at(s, 0) can be replaced with first_character(s)", expr.at)
                } else {
                    self->perf_warning("PERF003: character_at is O(n) due to strlen; consider peek_data() for hot paths", expr.at)
                }
            }
        }
        // PERF006: track reserve() calls and detect push/emplace without reserve
        if (self->is_array_func(expr, "reserve")) {
            var path = ""
            let rv = self->find_expr_path(expr.arguments[0], path)
            if (rv != null) {
                let key = self->make_path_key(rv, path)
                if (!reserved_paths |> has_value(key)) {
                    reserved_paths |> push(key)
                }
            }
        }
        let in_conditional = !empty(if_depth_stack) && if_depth_stack[length(if_depth_stack) - 1] > 0
        let has_early_exit = !empty(loop_has_early_exit) && loop_has_early_exit[length(loop_has_early_exit) - 1]
        if (in_closure == 0 && known_length_loop_depth > 0 && !in_conditional && !has_early_exit) {
            if (self->is_array_func(expr, "push") || self->is_array_func(expr, "push_clone") || self->is_array_func(expr, "emplace")) {
                var path = ""
                let rv = self->find_expr_path(expr.arguments[0], path)
                if (rv != null && self->is_defined_outside_loop(rv) && !reserved_paths |> has_value(self->make_path_key(rv, path))) {
                    let fname = string(expr.func.fromGeneric.name)
                    self->perf_warning("PERF006: {fname} in loop without prior reserve() may cause repeated reallocations; consider reserve() before the loop", expr.at)
                }
            }
        }
        // PERF012: string(das_string) passed to strings module function
        if (expr.func._module.name == "strings" && expr.func.name != "string") {
            for (arg in expr.arguments) {
                if (self->is_string_of_das_string(arg)) {
                    self->perf_warning("PERF012: string(das_string) passed to strings function; use peek(das_string) instead", expr.at)
                    break
                }
            }
        }
        return <- expr if (in_closure > 0)
        if (expr.func.name == "length" && expr.func._module.name == "strings") {
            if (in_length_while_call > 0) {
                in_length_while_call--
            }
        }
        return <- expr
    }

    // --- PERF004: string builder reassignment in loop (counter-based) ---

    def override preVisitExprCopy(expr : ExprCopy?) : void {
        if (in_closure > 0 || loop_depth == 0) {  //
            return
        }
        var v = self->find_string_var_from_expr(expr.left)
        if (v != null && self->is_defined_outside_loop(v) && expr.right is ExprStringBuilder) {
            string_builder_target_var = v
        }
    }

    def override visitExprCopy(var expr : ExprCopy?) : ExpressionPtr {
        string_builder_target_var = null
        return <- expr
    }

    def override preVisitExprMove(expr : ExprMove?) : void {
        if (in_closure > 0 || loop_depth == 0) {  //
            return
        }
        var v = self->find_string_var_from_expr(expr.left)
        if (v != null && self->is_defined_outside_loop(v) && expr.right is ExprStringBuilder) {
            string_builder_target_var = v
        }
    }

    def override visitExprMove(var expr : ExprMove?) : ExpressionPtr {
        string_builder_target_var = null
        return <- expr
    }

    def override preVisitExprStringBuilder(expr : ExprStringBuilder?) : void {
        if (string_builder_target_var != null) {
            in_string_builder_check++
        }
    }

    def override visitExprStringBuilder(var expr : ExprStringBuilder?) : ExpressionPtr {
        if (in_string_builder_check > 0) {
            in_string_builder_check--
        }
        return <- expr
    }

    // --- Central ExprVar handler (PERF002, PERF004, PERF005) ---

    def override preVisitExprVar(expr : ExprVar?) : void {
        if (in_closure > 0) {  //
            return
        }
        let v = expr.variable
        // PERF018: a bare iter_var reference outside a qualifying ExprAt index slot disqualifies the loop.
        self->perf018_on_var(v)
        // PERF002: loop variable inside character_at call
        if (in_character_at_call > 0 && self->is_loop_variable(v)) {
            if (current_character_at != null && !reported_character_at |> has_value(current_character_at)) {
                self->perf_warning("PERF002: character_at(s, i) in loop is O(n) per call (strlen + bounds check each time); use peek_data(s) to access characters as array<uint8>", expr.at)
                reported_character_at |> push(current_character_at)
            }
        }
        // PERF004: target var inside string builder
        if (in_string_builder_check > 0 && string_builder_target_var != null && v == string_builder_target_var) {
            self->perf_warning("PERF004: rebuilding string via interpolation in loop creates O(n^2) allocations; use build_string() instead", expr.at)
        }
        // PERF005: string var inside length() in while condition
        if (in_length_while_call > 0 && v != null) {
            if (expr.variable._type != null && expr.variable._type.baseType == Type.tString) {
                if (!expr.variable.access_flags.access_ref) {
                    self->perf_warning("PERF005: length(string) in while condition recomputes strlen each iteration; cache in a local variable", expr.at)
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

def public perf_lint(prog : ProgramPtr; compile_time_errors : bool) : int {
    //! Runs the performance lint visitor on the compiled program.
    //! Returns the number of warnings found.
    var astVisitor = new PerfLintVisitor(compile_time_errors = compile_time_errors)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit(prog, astVisitorAdapter)
    }
    let count = astVisitor.warning_count
    unsafe { delete astVisitor; }
    return count
}

def public perf_lint_collect(prog : ProgramPtr; var warnings : array<string>) : int {
    //! Runs the performance lint visitor and collects warnings as strings.
    //! Returns the number of warnings found.
    var astVisitor = new PerfLintVisitor(compile_time_errors = false, collect_warnings = true)
    make_visitor(*astVisitor) $(astVisitorAdapter) {
        visit(prog, astVisitorAdapter)
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
class PerfLintMacro : AstPassMacro {
    def override apply(prog : ProgramPtr; mod : Module?) : bool {
        perf_lint(prog, true)
        return true
    }
}
