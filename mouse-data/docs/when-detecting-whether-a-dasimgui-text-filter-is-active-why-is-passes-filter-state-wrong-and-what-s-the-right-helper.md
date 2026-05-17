---
slug: when-detecting-whether-a-dasimgui-text-filter-is-active-why-is-passes-filter-state-wrong-and-what-s-the-right-helper
title: When detecting whether a dasImgui text_filter is active, why is `passes_filter(state, "")` wrong and what's the right helper?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

**TL;DR — use `is_active(state)`, not `passes_filter(state, "")`.**

Wrong:
```daslang
if (passes_filter(LOG_FILTER, "")) {
    // assume filter inactive
}
```

Right:
```daslang
if (!is_active(LOG_FILTER)) {
    // filter inactive
}
```

## Why `passes_filter(state, "")` is broken

`passes_filter` bottoms out to `ImGuiTextFilter::PassFilter(text, nullptr)`. Per the C++ semantics:

- Empty filter (no terms) ⇒ `PassFilter(anything)` returns `true`.
- Filter with INCLUDE terms only (e.g. `info,warn`) ⇒ `PassFilter("")` returns `false` (empty string contains no include term).
- Filter with EXCLUDE terms only (e.g. `-error`) ⇒ `PassFilter("")` returns `true` (empty string isn't `-error`-matched).
- Mixed include + exclude ⇒ exclude-first short-circuit, then include check.

The "exclude-only" case is the trap. `passes_filter(state, "")` returns `true` for both "filter inactive" AND "filter has only exclude terms". Code that uses this as the inactivity gate (e.g. to skip filter checks during clipper-cull) will render lines the user explicitly excluded.

## The right primitive

`def public is_active(var state : TextFilterState) : bool` in `widgets/imgui_widgets_builtin.das` (added in PR #40, 2026-05-16). Delegates directly to `ImGuiTextFilter::IsActive()`:

```cpp
bool ImGuiTextFilter::IsActive() const { return !Filters.empty(); }
```

True iff parsing produced ≥1 include OR exclude term — exactly what "is the filter active" means in C++.

## Reference

- Caught by Copilot review on PR #40 round 1 ([discussion](https://github.com/borisbat/dasImgui/pull/40#discussion_r3253962411)); fix in `f020f25`.
- Helper definition: `widgets/imgui_widgets_builtin.das:is_active`.
- `passes_filter` docstring carries an inline warning against using it as an active-state proxy.
- Original pattern lives in `examples/imgui_demo/app_log.das` ShowExampleAppLog scroll-region clipper gate. Same C++ idiom is at `imgui_demo.cpp:7554` (`if (Filter.IsActive()) { …non-clipper… } else { …clipper… }`).

## Questions
- When detecting whether a dasImgui text_filter is active, why is `passes_filter(state, "")` wrong and what's the right helper?
