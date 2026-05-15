---
slug: imgui-text-vs-textunformatted-format-string-footgun
title: imgui Text vs TextUnformatted — when does the format-string interpretation kick in, and what's the safe wrapper pattern?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

# `ImGui::Text` interprets `%`; `TextUnformatted` doesn't

`ImGui::Text(const char* fmt, ...)` is a printf-format wrapper — a literal `%` in the runtime string is a memory-corruption / crash footgun. Any user string that flows into `Text()` (or `TextColored`, `TextWrapped`, `TextDisabled`, `BulletText`) needs either escaping (`%%`) or routing through `TextUnformatted`. daslang has no native printf, so the printf semantics aren't *useful* — they're pure risk.

## The dasImgui v2 fix (PR #33)

All 5 narrative-text wrappers decompose to `TextUnformatted`-based primitives so a `%` from consumer code can never reach the printf parser regardless of what's passed in:

| Wrapper | Before | After |
|---|---|---|
| `text` | `Text(text)` | `TextUnformatted(text)` |
| `text_wrapped` | `TextWrapped(text)` | `PushTextWrapPos(0.0f); TextUnformatted(text); PopTextWrapPos()` |
| `text_colored` | `TextColored(color, text)` | `PushStyleColor(Text, color); TextUnformatted(text); PopStyleColor(1)` |
| `text_disabled` | `TextDisabled(text)` | `PushStyleColor(Text, GetStyleColorVec4(TextDisabled)); TextUnformatted(text); PopStyleColor(1)` |
| `bullet_text` | `BulletText(text)` | `Bullet(); TextUnformatted(text)` (no manual `SameLine` — `Bullet()` advances the cursor itself) |

`text_unformatted` is the canonical entry point; `text` is now an alias for clarity.

## Why decomposition not a single binding

ImGui's C++ API has no `TextWrappedUnformatted` / `TextColoredUnformatted` / `BulletTextUnformatted`. The wrap, color, and bullet behaviors live in different stacks (text-wrap-pos stack, style-color stack, item-spacing math) — open the right stack, call `TextUnformatted`, close it. Same pixel output, no printf path.

## Lint angle

With the wrappers fixed at the source, the `%`-in-literal lint rule (LINT001 in the original plan) has nothing to catch. Dropped from PR #33. The remaining risk surface is direct `ImGui::Text` calls — those are already blocked by IMGUI002 (`raw imgui::Text is forbidden`).

## Questions
- imgui Text vs TextUnformatted — when does the format-string interpretation kick in?
- How do I write a text widget that's safe against `%` in user input?
- Why does the dasImgui `text` wrapper route through TextUnformatted instead of Text?
- How do I decompose TextWrapped / TextColored / BulletText without their format-string variants?

## Questions
- imgui Text vs TextUnformatted — when does the format-string interpretation kick in, and what's the safe wrapper pattern?
