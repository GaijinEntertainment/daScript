options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_style_builtin shared

require imgui public
require daslib/ast
require daslib/ast_boost
require daslib/templates_boost
require daslib/macro_boost

//! `with_style((key, val), …) { … }` — one push/pop pair per style change,
//! all bracketed around a single trailing block.
//!
//! Each `(key, val)` tuple is dispatched at call-site by overload
//! resolution: `ImGuiCol` keys take `ImVec4` or `uint`-packed colors and
//! route to `PushStyleColor`; `ImGuiStyleVar` keys take `float` or
//! `ImVec2` and route to `PushStyleVar`. Tuple types are heterogeneous,
//! so this is a `[call_macro]` (not a plain variadic def — daslang
//! varargs require a uniform element type).
//!
//! The macro emits N `push_style_one(...)` calls in source order, then
//! `invoke(blk)`, then a single `pop_style_n(N)` that pops the matching
//! number of colors and vars in one shot. Tracking is per-thread via a
//! private kind stack; no cross-context state. Linear (no `finally`):
//! `invoke(blk)` doesn't let `return`/`break`/`continue` escape the
//! enclosing function, and a panic kills the frame and resets ImGui's
//! style stack on the next frame, so leaks are non-observable.

// ===== Push/pop primitives + kind stack =====

// Kind tag stored per push: 0 = color (PopStyleColor pops it), 1 = var
// (PopStyleVar pops it). Two parallel running counts let the bulk pop in
// pop_style_n issue at most one PopStyleColor + one PopStyleVar call.
var private g_style_pop_stack : array<int>

def public push_style_one(idx : ImGuiCol; col : ImVec4) {
    //! Push one color style. Routes to `imgui::PushStyleColor(int, ImVec4)`.
    PushStyleColor(idx, col)
    g_style_pop_stack |> push(0)
}

def public push_style_one(idx : ImGuiCol; col : uint) {
    //! Push one color style with a packed `ImU32` color value.
    PushStyleColor(idx, col)
    g_style_pop_stack |> push(0)
}

def public push_style_one(idx : ImGuiStyleVar; val : float) {
    //! Push one scalar style var.
    PushStyleVar(idx, val)
    g_style_pop_stack |> push(1)
}

def public push_style_one(idx : ImGuiStyleVar; val : ImVec2) {
    //! Push one 2-vector style var (`FramePadding`, `WindowPadding`, etc.).
    PushStyleVar(idx, val)
    g_style_pop_stack |> push(1)
}

def public pop_style_n(n : int) {
    //! Pop the last `n` style entries pushed via `push_style_one`, bulk-
    //! issuing one `PopStyleColor(colors)` and one `PopStyleVar(vars)`.
    //! Reads kind tags from `g_style_pop_stack` so unbalanced calls (e.g.
    //! `n` larger than the current stack depth) are caught implicitly via
    //! array-bounds panic — keeps the contract loud.
    return if (n <= 0)
    var colors = 0
    var vars = 0
    let start = length(g_style_pop_stack) - n
    for (i in range(n)) {
        let kind = g_style_pop_stack[start + i]
        if (kind == 0) {
            colors++
        } else {
            vars++
        }
    }
    for (_i in range(n)) {
        g_style_pop_stack |> pop()
    }
    if (colors > 0) {
        PopStyleColor(colors)
    }
    if (vars > 0) {
        PopStyleVar(vars)
    }
}

// ===== `with_style` call_macro =====

[call_macro(name = "with_style")]
class private WithStyleCallMacro : AstCallMacro {
    def override visit(prog : ProgramPtr; mod : Module?; var call : ExprCallMacro?) : Expression? {
        let nargs = length(call.arguments)
        macro_verify(nargs >= 2, prog, call.at,
                     "with_style((key, val), ...) requires at least one (key, val) tuple plus a trailing block")
        return null if (nargs < 2)
        let pushCount = nargs - 1
        let blkArg = call.arguments[nargs - 1]
        var bodyStmts : array<ExpressionPtr>
        bodyStmts |> reserve(pushCount + 2)
        for (i in range(pushCount)) {
            let a = call.arguments[i]
            macro_verify(a is ExprMakeTuple, prog, a.at,
                         "with_style: each argument must be a `(key, value)` tuple")
            return null if (!(a is ExprMakeTuple))
            let mt = a as ExprMakeTuple
            macro_verify(length(mt.values) == 2, prog, a.at,
                         "with_style: each tuple must have exactly two elements (enum key, value)")
            return null if (length(mt.values) != 2)
            let key = clone_expression(mt.values[0])
            let val = clone_expression(mt.values[1])
            bodyStmts |> push <| qmacro_expr(${
                push_style_one($e(key), $e(val))
            })
        }
        bodyStmts |> push <| qmacro_expr(${
            invoke($e(blkArg))
        })
        bodyStmts |> push <| qmacro_expr(${
            pop_style_n($v(pushCount))
        })
        return qmacro_block() {
            $b(bodyStmts)
        }
    }
}
