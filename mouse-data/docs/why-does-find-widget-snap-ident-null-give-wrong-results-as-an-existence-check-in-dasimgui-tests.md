---
slug: why-does-find-widget-snap-ident-null-give-wrong-results-as-an-existence-check-in-dasimgui-tests
title: Why does `find_widget(snap, ident) != null` give wrong results as an existence check in dasImgui tests?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**`find_widget` returns a JV(null) placeholder for missing keys — never literal null. Use `widget_exists(snap, ident)` for existence checks.**

daslib/json's `?[]` operator has a documented quirk: looking up a missing key returns `JV(null)` (a non-null `JsonValue?` pointer wrapping a null JsonValue), not literal `null`. `find_widget` is a thin wrapper:

```das
def public find_widget(var snap : JsonValue?; ident : string) : JsonValue? {
    return snap?["globals"]?[ident]   // chain inherits the quirk
}
```

So `find_widget(snap, "MISSING_KEY") != null` evaluates **true** even when the key is missing — the pointer is non-null; only its contents are JsonValue-null.

For existence checks use `widget_exists` (imgui_playwright.das) — it strips the JV(null) case and returns a bare `bool`:

```das
// Wrong:
t |> success(find_widget(snap, "DYN_WIN/CHANNEL_VOL[0]") == null, "empty initially")

// Right:
t |> success(!widget_exists(snap, "DYN_WIN/CHANNEL_VOL[0]"), "empty initially")
```

Similarly when waiting for a key to disappear via `wait_until $(snap)`, the predicate must use `widget_exists`:

```das
let dropped = wait_until(d, 5.0f) $(var snap : JsonValue?) : bool {
    return !widget_exists(snap, "DYN_WIN/CHANNEL_VOL[1]")
}
```

The comment on `find_widget` itself spells this out — but the trap is easy to fall into because the function name *sounds* like it should return null for missing.

`widget_payload_field` (`globals[ident].payload[field]`) has the same chain and the same caveat, but its callers usually `?? defaultValue`, which folds JV(null) into the default. The bare `find_widget(...) != null` form is the one that fails silently.</body>
<parameter name="slug">dasimgui-find-widget-not-null-wrong-existence-check-use-widget-exists

## Questions
- Why does `find_widget(snap, ident) != null` give wrong results as an existence check in dasImgui tests?
