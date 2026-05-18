---
slug: dasimgui-imguisizeconstraints-lambda-needs-module-scope-lifetime-not-stack-local
title: Why does `SetNextWindowSizeConstraints(min, max, ImGuiSizeConstraints(@(var d) { ... }))` crash with an access violation inside `CalcWindowNextAutoFitSize` on the next frame?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**Because ImGui stores the callback pointer + user_data on `g.NextWindowData.SizeCallback` / `.SizeCallbackUserData` and consumes them DEFERRED — at the next `Begin()` AND on later `CalcWindowNextAutoFitSize` passes — but your daslang `ImGuiSizeConstraints` struct (which holds the `lambda<...>` the C++ trampoline invokes) was a stack local in the function that called `SetNextWindowSizeConstraints`.** That local dies when the function returns. The lambda's captured `Context*` + closure body is GC'd before ImGui dereferences `SizeCallbackUserData`, so `_builtin_SetNextWindowSizeConstraints`'s trampoline reads freed memory → AV in the auto-fit pass.

**The fix:** make the `ImGuiSizeConstraints` holder live at least as long as the window. Module-scope `var private` + `[init]` seeding is the canonical pattern:

```das
// app_small.das — ShowExampleAppConstrainedResize
var private SMALL_CONSTR_SQUARE_CN : ImGuiSizeConstraints
var private SMALL_CONSTR_ASPECT_CN : ImGuiSizeConstraints
var private SMALL_CONSTR_STEP_CN   : ImGuiSizeConstraints

[init]
def small_constr_init() {
    SMALL_CONSTR_SQUARE_CN <- ImGuiSizeConstraints(@(var d : ImGuiSizeCallbackData) {
        let m = max(d.DesiredSize.x, d.DesiredSize.y); d.DesiredSize = float2(m, m)
    })
    SMALL_CONSTR_ASPECT_CN <- ImGuiSizeConstraints(@(var d : ImGuiSizeCallbackData) {
        d.DesiredSize.y = d.DesiredSize.x / (16.0f / 9.0f)
    })
    SMALL_CONSTR_STEP_CN <- ImGuiSizeConstraints(@(var d : ImGuiSizeCallbackData) {
        let s = 100.0f
        d.DesiredSize = floor(d.DesiredSize / float2(s, s)) * float2(s, s)
    })
}

// In ShowExampleAppConstrainedResize, per case:
SetNextWindowSizeConstraints(float2(0,0), float2(FLT_MAX, FLT_MAX), SMALL_CONSTR_SQUARE_CN)
```

**Why this is non-obvious from the daslang API surface:** `ImGuiSizeConstraints(...)` looks like a value constructor that returns by value, and `SetNextWindowSizeConstraints(min, max, var cn : ImGuiSizeConstraints)` takes `var cn` so it looks borrow-style. But the C++ `_builtin_SetNextWindowSizeConstraints` at `src/dasIMGUI.main.cpp:361-384` reinterprets the daslang struct's bytes (Context*, Lambda, LineInfo*) and stashes them in ImGui's `SizeCallback`/`SizeCallbackUserData` slots — pointer escape ImGui consumes later in the frame and on subsequent auto-fit passes. No daslang side-channel reflects this lifetime extension.

The 2-arg no-callback form `SetNextWindowSizeConstraints(min, max)` (in `ALLOWED_IMGUI`) doesn't have this trap — no callback to keep alive.

**Tutorial parity:** the `examples/tutorial/window_size_constraints.das` example uses the same module-scope `SQUARE_CN` / `ASPECT_CN` / `STEP_CN` pattern. If you copy a `SetNextWindowSizeConstraints` lambda call from your own code into a function body, you've reintroduced the bug.

## Questions
- Why does `SetNextWindowSizeConstraints(min, max, ImGuiSizeConstraints(@(var d) { ... }))` crash with an access violation inside `CalcWindowNextAutoFitSize` on the next frame?
- How do I correctly extend the lifetime of an `ImGuiSizeConstraints` callback lambda in dasImgui so ImGui's deferred SizeCallback consumer doesn't dereference freed memory?
- Can `ImGuiSizeConstraints` be a function-local in dasImgui, or does it need module-scope lifetime?
