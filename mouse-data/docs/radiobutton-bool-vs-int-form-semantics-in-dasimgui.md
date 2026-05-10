---
slug: radiobutton-bool-vs-int-form-semantics-in-dasimgui
title: RadioButton bool form vs int form in dasImgui — what's the right semantics for a single radio (acts as toggle) vs grouped radios (exclusive)?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

Two distinct ImGui signatures, two distinct dasImgui boost widgets:

**Bool form — `RadioButton(label : string, active : bool) : bool`**
- Returns true on click. Does NOT mutate. ImGui only paints the dot per the `active` flag you pass.
- For "single radio that toggles like a checkbox": flip `state.value` yourself on click.
- dasImgui boost: `radio_button(state : ToggleState)`, body:
  ```das
  let im_clicked = RadioButton(text, state.value)
  if (im_clicked) { state.value = !state.value }
  ```

**Int form — `RadioButton(label : string, v : int?, v_button : int) : bool`**
- Pointer form. Returns true on click AND mutates `*v = v_button`. This is the canonical grouped-radio pattern in ImGui — N call sites, all sharing the same `int* v`, each with a distinct `v_button` value.
- dasImgui boost: `radio_button_int(state : RadioIntState; v_button : int)`, body:
  ```das
  let changed = RadioButton(text, unsafe(addr(state.value)), v_button)
  ```
- Three calls sharing the same identifier (`DIFFICULTY`) form one logical group. The auto-emit gate ensures `DIFFICULTY` is allocated once; `widget_finalize`'s last-write-wins on `g_registry[ident]` is fine because state.value is the same across all calls.

**State struct picks:** `ToggleState` (bool value + pending) for the bool form; `RadioIntState` (int value + pending) for grouped radios. Defined in `modules/dasImgui/widgets/imgui_boost_runtime.das`.

## Questions
- RadioButton bool form vs int form in dasImgui — what's the right semantics for a single radio (acts as toggle) vs grouped radios (exclusive)?
