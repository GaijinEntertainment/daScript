---
slug: imgui-closable-window-skip-begin-when-closed-not-call-with-pfalse
title: For ImGui closable Begin/End windows — should I keep calling Begin with `&state.open=false`, or skip Begin/End entirely once the window is closed?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Skip Begin/End entirely.** ImGui's contract is "the application stops calling Begin once `*p_open` flips to false from a prior frame." Calling Begin with `*p_open=false` doesn't reliably suppress the body — the leaves inside continue to render at least one extra frame. Confirmed empirically 2026-05-10 in dasImgui Phase 0b.3: the test for `imgui_close DETAIL_WIN hides nested leaves` failed when we always called Begin/End; passed once we gated the entire Begin/End on `state.open` when `closable=true`.

**Pattern for `[container] def window`** (`modules/dasImgui/widgets/imgui_containers_builtin.das`):
```das
if (state.pending_open)  { state.open = true;  state.pending_open  = false }
if (state.pending_close) { state.open = false; state.pending_close = false }
state.flags = flags
if (!closable || state.open) {                  // gate Begin on state.open when closable
    var p_open : bool? = null
    if (closable) { unsafe { p_open = addr(state.open) } }
    let im_open = Begin(text, p_open, flags)
    if (im_open) { ... invoke(blk) }
    End()                                       // always-pair Begin
}
open_close_finalize(widget_ident, "window", state)
```

When `closable=false`: always call Begin/End. State.open is meaningless for visibility — window always renders.
When `closable=true` and `state.open=false`: skip Begin/End. Body never runs. ImGui never sees the window this frame. Leaves inside don't register in `g_registry`. ✓

container_finalize still runs (outside the gate) so the dispatcher stays registered — `imgui_open` later flips state.pending_open and the next frame's body re-enters the Begin/End branch.

**Same applies to `tab_item`** — gate `BeginTabItem` on `state.open` when `closable=true`. Fixed in the same commit.

**Does NOT apply to `popup` / `popup_modal`** — those use `OpenPopup` / `BeginPopup` which has different lifecycle semantics: the popup never renders unless OpenPopup was called, so no gating needed.

## Questions
- For ImGui closable Begin/End windows — should I keep calling Begin with `&state.open=false`, or skip Begin/End entirely once the window is closed?
