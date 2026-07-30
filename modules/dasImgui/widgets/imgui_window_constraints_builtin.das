options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_window_constraints_builtin shared

require imgui public
require daslib/rtti public

//! Lambda-bearing `SetNextWindowSizeConstraints` overload — escape hatch for
//! aspect-ratio / fixed-step / "always square" resize callbacks. The no-callback
//! 2-arg overload `SetNextWindowSizeConstraints(min, max)` is on ALLOWED_IMGUI
//! and stays raw at call sites; require this module only when you need the
//! 3-arg callback form.
//!
//! Struct layout MUST stay 1:1 with `DasImGuiSizeConstraints` in
//! `src/dasIMGUI.main.cpp` — the C++ trampoline reads the field offsets
//! directly to dispatch the lambda via `das_invoke_lambda<void>::invoke`.

struct ImGuiSizeConstraints {
    //! Wraps a daslang lambda for ImGui's `ImGuiSizeCallback` slot.
    //! `context` and `at` are populated by `das_invoke_lambda`'s capture
    //! machinery — leave them null; the C++ side stamps them at call time.
    context : Context?
    callback : lambda<(var data : ImGuiSizeCallbackData) : void>
    at : LineInfo?
}

def ImGuiSizeConstraints(var lmb : lambda<(var data : ImGuiSizeCallbackData) : void>) {
    //! Constructor: wrap ``lmb`` for a SetNextWindowSizeConstraints callback.
    var self : ImGuiSizeConstraints
    self.callback <- lmb
    return <- self
}

def SetNextWindowSizeConstraints(size_min, size_max : float2;
                                 var cn : ImGuiSizeConstraints) {
    //! 3-arg overload: `SetNextWindowSizeConstraints(min, max, callback)` where
    //! ``callback`` is an `ImGuiSizeConstraints` wrapping a daslang lambda.
    _builtin_SetNextWindowSizeConstraints(cn, size_min, size_max)
}
