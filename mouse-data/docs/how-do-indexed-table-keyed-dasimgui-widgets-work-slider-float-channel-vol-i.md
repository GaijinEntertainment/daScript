---
slug: how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i
title: How do indexed (table-keyed) dasImgui widgets work — `slider_float(CHANNEL_VOL[i], ...)`?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**User pre-declares a `table<K; StateT>` global at module scope. The `[widget]`/`[container]` macro detects `ExprAt` on the first arg, validates the table, and lazily emits a per-(kind, IDENT) wrapper function. Each rendered `[k]` gets its own `g_widgets` entry keyed as `<container>/<IDENT>[<k>]`.**

```das
// User code
var private CHANNEL_VOL : table<int; SliderStateFloat>
var private TRACK_VOL   : table<string; SliderStateFloat>

[init]
def setup() {
    for (i in range(8)) {
        CHANNEL_VOL[i].bounds = (0.0f, 1.0f)
        CHANNEL_VOL[i].value  = 0.5f
    }
}

def render() {
    window(MIX_WIN, (text="MIX_WIN", closable=false, flags=ImGuiWindowFlags.None)) {
        for (i in range(8)) {
            slider_float(CHANNEL_VOL[i], (text="ch{i}"))
        }
    }
}
```

Snapshot keys: `MIX_WIN/CHANNEL_VOL[0]`, `MIX_WIN/CHANNEL_VOL[1]`, …, `MIX_WIN/TRACK_VOL[bass]`, … Dispatch (`imgui_set`, `imgui_click`) uses those literal strings — square brackets are part of the JSON key.

**What the macro emits (one wrapper per (kind, IDENT)):**

```das
def private $imgui$indexed$slider_float$CHANNEL_VOL(
    k : int; text : string; format : string = "%.3f";
    flags : ImGuiSliderFlags = ImGuiSliderFlags.None
) : bool {
    let bare = "CHANNEL_VOL[{k}]"
    let path = imgui_boost_runtime::widget_path_key(bare)
    if (!imgui_boost_runtime::widget_registered(path)) {
        unsafe {
            let ser <- @ capture(= k) () : JsonValue? { return JV(CHANNEL_VOL[k]) }
            imgui_boost_runtime::register_widget("", path, "slider_float",
                                                  addr(CHANNEL_VOL[k]), ser)
        }
    }
    return slider_float(CHANNEL_VOL[k], bare, text, format, flags)
}
```

The call site `slider_float(CHANNEL_VOL[i], (text="ch{i}"))` rewrites to `$imgui$indexed$slider_float$CHANNEL_VOL(i, "ch{i}")`. Wrapper emission is idempotent — gated by `find_unique_function`.

**Hot-reload story:** `clear_module_widgets(module)` wipes `g_widgets` on reload but `@live` table entries survive. The wrapper's `widget_registered(path)` check fails post-reload, so register_widget re-fires (rebinding the serializer's `addr(CHANNEL_VOL[k])` capture); the table value itself isn't reinitialized. That's what users want — they get fresh telemetry plumbing with preserved values.

**Snapshot expiration for removed keys:** when the user `erase`s a key from the table, the wrapper won't re-render it next frame. The old `g_widgets` entry still exists but `last_seen_frame` doesn't update. `imgui_snapshot` filters: if path contains `[` AND `last_seen_frame < g_frame - 1`, the entry is omitted. Single-globals (no `[`) stay visible even when unrendered — closed windows should still appear in snapshot.

**Restrictions:**

- Indexed-form **rejects `init=`/`bounds=`** sugar (V1) — pre-populate the table in an `[init]` function instead. The wrapper is emitted once per IDENT but called from N sites; silently picking-first-call's-value is worse UX than the explicit imperative init.
- Indexed-form **rejects dotted flags** (`MIXER.PUBLIC[i]`) — flags belong on the user's table declaration, not the reference.
- **Iteration gotcha**: `for (k in keys(t)) { slider_float(t[k], ...) }` panics with "can't insert to a locked table" because the wrapper's internal `t[k]` is auto-insert and the iterator holds a lock. Snapshot keys to a local array first (see `daslang-for-keys-table-locked-cant-insert`).
- **`radio_button_int` is exempt** from per-frame "render exactly once" panic because it's the canonical shared-state-across-N-calls widget. The exempt-list is in `check_unique_render` (imgui_boost_runtime.das).

Landed 2026-05-12. See `modules/dasImgui/examples/features/inputs_indexed.das` (static), `indexed_dynamic.das` (add/remove/move with telemetry assertions), and the matching `tests/integration/test_inputs_indexed.das` + `test_indexed_dynamic.das`.</body>
<parameter name="slug">dasimgui-indexed-form-table-keyed-widgets-table-state-wrapper-per-ident

## Questions
- How do indexed (table-keyed) dasImgui widgets work — `slider_float(CHANNEL_VOL[i], ...)`?
