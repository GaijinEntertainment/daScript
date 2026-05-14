---
slug: dasimgui-new-state-struct-widget-auto-emit-just-works
title: dasImgui [widget] auto-emit with a new state struct — do I need to register it anywhere besides defining the struct?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**No registration step.** The macro infrastructure (`apply_widget_or_container` in `widgets/imgui_boost.das`, `WidgetCallMacro` register/dispatch) is generic over the state struct type. Define the struct, write a `[widget]` def that takes `var state : MyState` as its first arg, and the auto-emit + serializer + dispatcher all just work — the generic JV/serialize machinery in `daslib/json_boost` handles new struct types transparently.

Confirmed during the v2 boost mega-detour (2026-05-15): six new state struct types — `NarrativeState`, `LabelTextState`, `EmptyMarkerState`, `ImageState`, `ProgressBarState`, `MainMenuBarState` — landed in `widgets/imgui_boost_runtime.das`. Each shows up in `program_log` output (post-macro-expansion) with auto-generated `serialize` / `JV` generic instances. The first call site in user code triggers auto-emit of a module-scope `var private IDENT : MyState` global, identical to how `var private SAVE_BTN : ClickState` was emitted on first call to `button(SAVE_BTN, ...)`.

The contract for a new state struct:
- Mark fields with `@live` (preserved across hot-reload) or `@optional` (transient, dropped from JV when zero-valued). Don't leave fields un-annotated unless you want them included unconditionally.
- Default values (`@optional value : string = ""`) fire on the auto-emitted global — confirmed by mouse card `dasimgui-widget-state-struct-field-defaults-fire`.
- For stateless decoration widgets (separator, spacing, etc.), use the `@optional _placeholder : bool` shape so the struct stays non-empty for the auto-emit gate — mirrors `GroupState`/`MenuBarState`/`TooltipState` precedent. JV emits `{}` for these.

Custom finalize helper: each widget kind that needs unique L2/L3 dispatch (`"set"`, `"click"`, etc.) gets a per-kind `def private xxx_finalize(widget_ident; kind; var state : MyState)` — see `click_finalize` / `toggle_finalize` / `text_value_finalize` for the lambda+dispatcher pattern. For read-only display widgets, dispatcher closure is a no-op and `register_focusable` is skipped (see `narrative_finalize` / `marker_finalize` / `plot_finalize` from the v2 boost commits).

**What this means in practice**: extending the dasImgui surface with a new widget family is mechanical — a state struct in `imgui_boost_runtime.das`, a finalize helper + `[widget]` def in `imgui_widgets_builtin.das`, and that's it. No registry tables to update, no codegen step, no JSON schema to extend. The smoke verification under daslang-live in the v2 mega-detour ran `imgui_snapshot` and every new kind showed up with the right payload — that's load-bearing evidence the auto-emit chain is fully generic.

## Questions
- dasImgui [widget] auto-emit with a new state struct — do I need to register it anywhere besides defining the struct?
- How do I add a new widget family to dasImgui — what's the minimum set of files to touch?
- Does the [widget] macro work with arbitrary state struct types, or do I need to teach it about each one?
