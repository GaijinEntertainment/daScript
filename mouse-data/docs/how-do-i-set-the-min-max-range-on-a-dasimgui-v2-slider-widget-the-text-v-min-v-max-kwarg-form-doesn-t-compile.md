---
slug: how-do-i-set-the-min-max-range-on-a-dasimgui-v2-slider-widget-the-text-v-min-v-max-kwarg-form-doesn-t-compile
title: How do I set the min/max range on a dasImgui v2 slider widget? The `(text=..., v_min=..., v_max=...)` kwarg form doesn't compile.
created: 2026-05-11
last_verified: 2026-05-12
links: []
---

**Single-global form (recommended since 2026-05-12): `bounds=` / `init=` are first-touch sugar on the call itself.**

```das
slider_float(VOLUME, (text="Volume", bounds=(0.0f, 100.0f), init=50.0f))
// equivalent to: VOLUME = SliderStateFloat(value=50.0f, bounds=(0,100)); slider_float(VOLUME, (text="Volume"))
```

`bounds=` matches the state field name 1:1; `init=` is a rename to `value`. The macro pumps both into the auto-emitted global's `ExprMakeStruct(useInitializer)` at module init and strips them from the call. Fires once at global creation — for reload-time rebinding, fall back to imperative (or wait for the future `bounds`-rebinds-on-reload work, not in V1). See [`dasimgui-widget-init-bounds-sugar-single-global-only`](dasimgui-widget-init-bounds-sugar-single-global-only.md).

**Indexed form (`slider_float(CHANNEL_VOL[i], …)`) rejects `init=`/`bounds=`** — pre-populate the table in an `[init]` function: `CHANNEL_VOL[i].bounds = (0,1); CHANNEL_VOL[i].value = 0.5f`. The macro errors with a clear fixit if you try the sugar there.

**Imperative pattern (still works for single-globals — required for indexed):**

```das
VOLUME.bounds = (0.0f, 100.0f)
slider_float(VOLUME, (text = "Volume"))
```

The state struct (`SliderStateFloat`, `SliderStateInt`, etc. in `widgets/imgui_boost_runtime.das`) carries a `bounds : tuple<T; T>` field with `@live` so the value survives reload. The widget reads `state.bounds` at render time.

Applies to every variant — `slider_int.bounds = (0, 9)`, `slider_float2.bounds = (-1.0f, 1.0f)`, `slider_int4.bounds = (0, 512)`, etc. The scalar widgets' tuple elements are the same scalar type (just min/max); the vector variants reuse the same `(min, max)` pair for all components.

Canonical example: `modules/dasImgui/examples/features/inputs_slider.das:40-110` walks through every slider variant with imperative bounds assignment. The `(text = "...")` form in the call only carries display-time args (label, format string, ImGuiSliderFlags).

This is intentionally different from raw `imgui::SliderFloat(label, value, v_min, v_max)` — the v2 wrapper keeps the bounds in the state struct so they're @live across reload and visible to the snapshot/telemetry layer.

## Questions
- How do I set the min/max range on a dasImgui v2 slider widget? The `(text=..., v_min=..., v_max=...)` kwarg form doesn't compile.
