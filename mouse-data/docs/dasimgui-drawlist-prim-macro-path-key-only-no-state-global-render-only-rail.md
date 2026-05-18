---
slug: dasimgui-drawlist-prim-macro-path-key-only-no-state-global-render-only-rail
title: How do I design a dasImgui macro annotation that emits path-key telemetry per call site but no state-global, for render-only operations?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

For render-only call sites (drawlist primitives, frame markers, etc.) where every invocation is purely transient — no hover, no active, no edit state worth preserving across `@live` reloads — emit a stable `<mod>:<line>:<col>` path-key per call site for mouse-cards / playwright targeting BUT skip the `__widget_<line>_<col>` state-global emission that `[widget]` does.

The pattern is a sibling to `[widget]` / `[container]`, sharing the same surface area in `widgets/imgui_boost.das`. Mirror this template (`[drawlist_prim]` is the reference implementation from dasImgui PR #48):

```das
// widgets/imgui_boost.das — same module as [widget] / [container] annotations.
// Annotation MUST live in a different module from the call sites — defining
// the annotation class and immediately using [drawlist_prim] in the same file
// errors out because the parser hasn't completed annotation registration
// when the sibling annotation gets resolved.

class DrawlistPrimCallMacro : AstCallMacro {
    kind_name : string

    def override visit(prog : Program?; mod : Module?; var expr : smart_ptr<ExprCallMacro>) : ExpressionPtr {
        let modName = mod == null ? "anon" : string(mod.name)
        let path_key = "{modName}:{expr.at.line:d}:{expr.at.column:d}"
        var rewritten = new ExprCall(at = expr.at, name := kind_name)
        var pathArg = new ExprConstString(at = expr.at, value := path_key)
        rewritten.arguments |> push(pathArg)
        for (a in expr.arguments) {
            rewritten.arguments |> push(clone_expression(a))
        }
        return <- rewritten
    }
}

[function_macro(name = "drawlist_prim")]
class DrawlistPrimFunctionMacro : AstFunctionAnnotation {
    def override transform(var func : FunctionPtr; var args : AnnotationArgumentList) : ExpressionPtr {
        let kind = string(find_arg(args, "kind"))
        // Inject a leading `widget_ident : string` parameter so the rewritten
        // call carries the path key into the runtime register helper.
        var widthIdentVar = new Variable(at = func.at,
            name := "widget_ident",
            _type <- new TypeDecl(baseType = Type.tString, at = func.at))
        widthIdentVar.flags.generated = true
        func.arguments |> emplace(widthIdentVar, 0)
        // Register the call-site rewriter under the function's own name —
        // call sites write `add_rect(dl, ...)` and the macro slots in
        // the path-key argument transparently.
        var inst = new DrawlistPrimCallMacro(kind_name = kind)
        compiling_module() |> add_call_macro(make_call_macro(kind, inst))
        return [[ExpressionPtr]]
    }
}
```

Then at the impl side (`widgets/imgui_drawlist_builtin.das` or similar):

```das
[drawlist_prim(kind = "add_rect")]
def add_rect(widget_ident : string; var dl : ImDrawList?; p_min, p_max : float2; col : uint; rounding : float; flags : ImDrawFlags; thickness : float) {
    drawlist_register(widget_ident, "add_rect", make_bbox(p_min, p_max))
    *dl |> AddRect(p_min, p_max, col, rounding, flags, thickness)
}
```

Where `drawlist_register(widget_ident, kind, bbox)` writes a lightweight `WidgetEntry` (kind + bbox, no hover/active/focus fields) into `g_registry`. No `__widget_<line>_<col>` global declared — multiple primitives at the same source coordinates never collide because nothing's stored.

**Key correctness points**:

1. **Annotation lives in a different file from the call sites**. Same-file definition + use fails — parser order issue. Put `[function_macro(name="...")] class` in `imgui_boost.das` next to `[widget]`/`[container]`; primitives `require imgui/imgui_boost_v2 public` then tag themselves with `[drawlist_prim(kind="add_rect")]`.

2. **Path-key shape is `<mod>:<line>:<col>`** — leaf format that `widget_path_key(widget_ident)` recognises and slots under the current container (if any). Tests should assert leaves match a `:\d+:\d+$` regex.

3. **No state global**. `[widget]` declares `__widget_<line>_<col>` to persist hover/click state across `@live`. Drawlist primitives skip that step — they're stateless, so collisions are impossible-by-design, NOT impossible-by-accident.

4. **Container nesting is lexical**, NOT visual. `with_window_drawlist` painted INSIDE `window(W, ...)` produces nested path keys (`W/mod:line:col`); same call OUTSIDE the window block produces unnested keys. Visual drawlist scope (`with_foreground_drawlist` paints to the viewport drawlist regardless of where called) is independent of telemetry nesting.

Verified by `tests/integration/test_drawlist_path_key.das` and `test_with_window_drawlist.das` in dasImgui PR #48.

## Questions
- How do I design a dasImgui macro annotation that emits path-key telemetry per call site but no state-global, for render-only operations?
