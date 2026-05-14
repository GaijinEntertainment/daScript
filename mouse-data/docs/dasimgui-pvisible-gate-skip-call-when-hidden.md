---
slug: dasimgui-pvisible-gate-skip-call-when-hidden
title: ImGui `Begin*(label, bool* p_visible, ...)` form — when *p_visible is false, do I skip the call entirely or pass null/some other gate?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Skip the call entirely.** ImGui's `bool* p_visible` overloads (`CollapsingHeader(label, bool* p_visible, flags)`, `BeginPopupModal(label, bool* p_open, flags)`, `Begin(label, bool* p_open, flags)`, `BeginTabItem(label, bool* p_open, flags)`) contract is: while `*p_visible` is `false`, **do not issue the call**. Once the X-button or `imgui_close` flips it false, the application is expected to stop calling Begin entirely — NOT to pass a "now false" pointer and let ImGui no-op.

dasImgui boost containers all mirror this pattern. The shape (from `widgets/imgui_containers_builtin.das`):

    if (state.pending_close) {
        state.open = false
        state.pending_close = false
    }
    if (!closable || state.open) {            // <-- the gate
        var p_open : bool? = null
        if (closable) {
            unsafe { p_open = addr(state.open) }
        }
        let im_open = Begin(text, p_open, flags)
        // ... body
        End()
    }

`window`, `collapsing_header(closable=true)` (added 2026-05-15 in the v2 boost mega-detour), `tab_item`, and `popup_modal` all use this exact gate. The `if (!closable || state.open)` short-circuit means non-closable variants never look at `state.open` and closable variants stop emitting the Begin call once they're hidden.

Why not "always call Begin and let ImGui no-op"? Because for some of these (`collapsing_header(p_visible=false)`) the call still draws state — even if the body skips, the section header would render. The gate keeps the chrome from showing when the user closed it.

**For `imgui_open` to restore visibility**: the dispatcher sets `state.pending_open=true`; the renderer then flips `state.open=true` and the gate opens, so the next frame's Begin call fires again. End-to-end: an imgui_open → next frame the container re-renders.

Same gate logic applies if you're hand-rolling a new closable container — mirror `window` lines ~95-137 in `widgets/imgui_containers_builtin.das`.

## Questions
- ImGui `Begin*(label, bool* p_visible, ...)` form — when *p_visible is false, do I skip the call entirely or pass null/some other gate?
