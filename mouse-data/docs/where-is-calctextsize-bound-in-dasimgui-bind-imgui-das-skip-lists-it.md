---
slug: where-is-calctextsize-bound-in-dasimgui-bind-imgui-das-skip-lists-it
title: Where is CalcTextSize bound in dasImgui — `bind_imgui.das` skip-lists it?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

It IS callable from daslang despite the skip-list. The autogen skip in `modules/dasImgui/bind/bind_imgui.das:106` only suppresses the auto-bound `ImGui::CalcTextSize`; a **manual shim** in `modules/dasImgui/src/dasIMGUI.main.cpp:342` publishes a wrapper that's bound at module init (registered around `:511`).

Daslang signature:

```das
CalcTextSize(text : string; hide_text_after_double_hash : bool; wrap_width : float) : float2
```

Call site:

```das
let sz = CalcTextSize(my_text, false, -1.0f)
let box = float2(anchor.x + sz.x + pad * 2.0f, anchor.y + sz.y + pad * 2.0f)
```

Picks up `FontGlobalScale` and any active font, so the returned size matches what `AddText` will actually paint. Useful for sizing overlay boxes (narrate callouts in `widgets/imgui_visual_aids.das`).

This pattern — autogen skip + manual shim with a slightly different signature — applies to other functions in the skip table too (`AddText` on `ImDrawList`, `TextUnformatted`, `SetNextWindowSizeConstraints`, etc.). Grep `modules/dasImgui/src/dasIMGUI.main.cpp` for `addExtern<DAS_BIND_FUN(das::...)>` to find the manually-bound surface.

## Questions
- Where is CalcTextSize bound in dasImgui — `bind_imgui.das` skip-lists it?
