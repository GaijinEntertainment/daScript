---
slug: in-a-container-body-when-do-i-call-getscrollmaxx-getscrollmaxy-before-or-after-invoke-blk
title: In a `[container]` body, when do I call `GetScrollMaxX()` / `GetScrollMaxY()` — before or after `invoke(blk)`?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**AFTER `invoke(blk)`, BEFORE `End()` / `EndChild()`.** `GetScrollMaxX/Y` derives from the content extent — it's only computed once items have been emitted into the window's draw list. Reading it before `invoke(blk)` returns 0 (no content yet); reading after `End()` errors (no current window).

`GetScrollX/Y` (the CURRENT scroll position) can be read anywhere in the body — it's persistent state from the previous frame.

**Pattern in `widgets/imgui_containers_builtin.das` (Phase 1.2):**
```das
let im_open = Begin(text, p_open, flags)
if (im_open) {
    // Per-frame observed state read before invoke (these are valid immediately).
    let pos = GetWindowPos()
    state.pos = float2(pos.x, pos.y)
    state.scroll = float2(GetScrollX(), GetScrollY())   // current position — fine
    invoke(blk)
    // scroll_max derives from content extent — must read AFTER invoke.
    state.scroll_max = float2(GetScrollMaxX(), GetScrollMaxY())
}
End()
```

I shipped this with scroll_max BEFORE invoke first and the integration test showed `scroll_max.y = 0` even though the content overflowed by ~600px. Moving the read to after invoke fixed it.

## Questions
- In a `[container]` body, when do I call `GetScrollMaxX()` / `GetScrollMaxY()` — before or after `invoke(blk)`?
