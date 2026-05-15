---
slug: how-do-i-embed-a-bound-cpp-struct-as-a-daslang-state-field-and-what-binding-overrides-do-i-need
title: How do I embed a bound C++ struct (e.g. ImGuiTextFilter) as an inline field of a daslang state struct so [widget] auto-emit can hold one in a module-scope global?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

By default a `ManagedStructureAnnotation<T>` rejects most embed paths daslang needs for module-scope `var private` state globals — you'll hit one error at a time until **four** virtual overrides are flipped on the C++ side, then **two** more on the daslang side.

## C++ overrides (e.g. `src/dasIMGUI.struct.class.inc`)

```cpp
struct ImGuiTextFilter_GeneratedAnnotation : ManagedStructureAnnotation<ImGuiTextFilter> {
    virtual bool isLocal() const override { return true; }
    virtual bool canBePlacedInContainer() const override { return true; }
    virtual bool hasNonTrivialCtor() const override { return false; }
    virtual bool canCopy() const override { return true; }
    ...
};
```

Why each one:
- **`isLocal()=true`** — daslang structs can embed this as a field. Default is false; without it, error 30239 *"contains Handled type, where isLocal() returned false"* on first field declaration.
- **`canBePlacedInContainer()=true`** — daslang containers (tables, arrays) can hold this. `isLocal` alone isn't enough.
- **`hasNonTrivialCtor()=false`** — daslang trusts zero-init bytes as a valid default. Default is true; without it, error 30175 *"global variable X can't be initialized at all"* once the containing struct goes module-scope. Only safe to flip when the C++ struct's zero-bytes form is a valid object (e.g. `ImGuiTextFilter` zeros → empty `InputBuf` + empty `ImVector Filters` + `CountGrep=0`).
- **`canCopy()=true`** — needed whenever the type is bound with `addCtorAndUsing<T, ...>` (i.e. has a daslang-visible factory). Without it, daslang's `BuiltInFn` registration refuses with *"can't be bound. It returns values which can't be copied or moved"* because the factory returns `T` by value.

## Binder side (the `bind/*.das` policy file)

`cbind_boost` only emits **the first two** overrides for entries in `local_type_names <- { ... }`. The trivial-ctor + canCopy overrides have to be re-applied manually after every regen. Add a comment block above the struct flagging this — `cbind_boost.das:1131` is the only emitter.

## Daslang overrides (where the state struct lives, e.g. a `shared` boost module)

Even with the C++ side correct, two daslang-side generic walkers fail on bound C++ fields:

1. **`daslib/json_boost`'s `JV<T>(value : T) : JsonValue?`** walks every field and calls `_::JV(field)`. The v1 `register_widgets` chain auto-instantiates this for every state struct, so JV-failing field = whole-module compile fail. Add an explicit override:
   ```das
   def public JV(value : MyState) : JsonValue? {
       return JV((field1 = value.field1, ...))   // serialize what makes sense
   }
   ```

2. **`daslib/archive`'s generic `serialize<T>(arch, value : T)`** walks every field and calls `_::serialize(arch, field)`. The `LiveVarsPass` (in `modules/dasLiveHost/live/live_vars.das`) auto-discovers every module-scope global marked `@live` (which **includes every [widget]-auto-emitted state global** in dasImgui) and emits archive calls into `__before_reload_live_vars`. Same opt-out — add an explicit override:
   ```das
   def public serialize(var arch : Archive; var value : MyState) {
       arch |> _::serialize(value.field_to_persist)
       // skip the bound C++ field; it regenerates on first frame after reload
   }
   ```

Both overrides go next to the state struct in the boost rail (`widgets/imgui_boost_runtime.das` is the dasImgui slot). Make them `def public` so transitive consumers see them.

## Reality check

If the bound C++ struct's **fields** are also walked by some downstream generic (rare, but a hand-bound nested field that itself contains another bound type might trigger it), you may need a per-field override too. Surface as the error appears — there's no exhaustive list. The four C++ + two daslang overrides above cover dasImgui's [widget]/[container] auto-emit chain (verified 2026-05-15 on `ImGuiTextFilter` in dasImgui PR landing the `app_console.das` port).

## Pure-daslang alternative

If you don't actually need the C++ struct's methods (e.g. `ImGuiTextFilter::Draw` rendering an InputText editor), and you're just consuming a predicate, reimplement in daslang. ~30 LOC for the comma-split include/exclude logic; no C++ rebuild, no generic-walker overrides. Pick this when the C++ struct's only value is convenience.

## See also
- `dasimgui-widget-state-struct-field-defaults-fire` — companion: how state struct field defaults map to the auto-emitted global
- `how-do-i-preserve-a-c-owned-resource-across-a-daslang-live-reload` — the live-reload context for why archive::serialize gets walked at all

## Questions
- How do I embed a bound C++ struct (e.g. ImGuiTextFilter) as an inline field of a daslang state struct so [widget] auto-emit can hold one in a module-scope global?
