---
slug: my-dasimgui-recording-driver-calls-move-to-app-p-widget-but-the-cursor-sprite-shows-up-at-the-framebuffer-corner-instead-of-the
title: My dasImgui recording driver calls move_to(app, p_widget) but the cursor sprite shows up at the framebuffer corner instead of the widget. Snapshot has the widget. What's wrong?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**The widget path you passed to `widget_center` doesn't exist in the snapshot. `widget_center` returns `(0, 0)` on missing paths (it `??` defaults each bbox field to `0.0f`), and `(0, 0)` is the framebuffer corner.**

Most common cause: the feature file omits an explicit ident on the widget, so the boost `[widget]` macro falls back to a source-line:col path. Example — `narrative_layout_tour.das` had:

    text("Plain text() — one line, telemetry-visible.")
    text_wrapped("text_wrapped() reflows ...")

These register at `TOUR_WIN/:66:8` and `TOUR_WIN/:69:8`. The driver asked for `TOUR_WIN/INTRO` / `TOUR_WIN/WRAP` (the names from the file's docstring "All idents are intentionally short and stable") — got `(0, 0)` back, `move_to((0, 0))` parked the cursor at the corner. The narrate callout also resolved its `target` to the missing path and anchored near the corner with a connector line dangling to the actual widget rect.

## Fix: explicit ident state + named-tuple value

For `[widget]` macros, the canonical "stable ident" form is:

    text(INTRO, (text = "Plain text() — one line, telemetry-visible."))
    text_wrapped(WRAP, (text = "text_wrapped() reflows ..."))
    text_colored(COLORED, (color = float4(...), text = "..."))
    label_text(LABEL_VERSION, (key = "Version", value = "v2.0-detour"))
    separator_text(SEP_TITLE, (text = "Display + tooltips"))
    progress_bar(PROG_BAR, (fraction = g_progress, size = ..., overlay = ""))

The first positional arg is the state ident (auto-emitted as a `var private` at module scope by the macro). The second is a NAMED-TUPLE wrapping all the actual widget params — named-tuple destructure is positional-by-tuple-field-index against the function signature (see mouse card `dasimgui-widget-named-tuple-args-positional-not-named`).

For `[widget]`s that take a single positional arg like `text(s : string)`, you'd think `text(INTRO, "hello")` would work; in practice the macro wants the named-tuple form for consistency with multi-arg widgets — and reading `(text = "hello")` makes the call self-documenting.

## How to detect this trap before recording

`mcp__daslang__live_command name="imgui_snapshot"`, then grep the JSON for your driver's target paths. If you see `TOUR_WIN/:66:8`-style entries instead of `TOUR_WIN/INTRO`, the feature file needs idents.

## Related

- `examples/tutorial/narrative_layout_tour.das` — fixed 2026-05-17 to use explicit idents on all 8 narrative+layout widgets.
- mouse card `dasimgui-widget-named-tuple-args-positional-not-named` — why field NAMES inside `(...)` don't dispatch.
- mouse card `my-dasimgui-single-scene-harness-driver-has-no-cursor-sprite-in-the-recorded-apng-and-clicks-don-t-open-menus-what-am-i-missing` — different cause (synth IO not draining); same symptom (no visible cursor).

## Questions
- My dasImgui recording driver calls move_to(app, p_widget) but the cursor sprite shows up at the framebuffer corner instead of the widget. Snapshot has the widget. What's wrong?
