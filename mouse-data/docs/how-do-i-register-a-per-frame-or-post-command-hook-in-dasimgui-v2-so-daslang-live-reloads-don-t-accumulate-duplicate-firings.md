---
slug: how-do-i-register-a-per-frame-or-post-command-hook-in-dasimgui-v2-so-daslang-live-reloads-don-t-accumulate-duplicate-firings
title: How do I register a per-frame or post-command hook in dasImgui v2 so daslang-live reloads don't accumulate duplicate firings?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Use a **`table<string; function<...>>`** keyed by a slot name, NOT a `array<function<...>>`. Re-registering with the same slot name replaces the prior entry, making `[init]` idempotent across reloads.

The trap: `[init]` re-runs on every daslang-live reload. The hook storage lives in `imgui_boost_runtime` (which doesn't reload when you edit a *downstream* module like `imgui_visual_aids`). Array-based storage in the upstream module accumulates one extra registration per reload, multiplying every painter/hook call. Symptom: highlights decay 2-3× too fast, mouse trails draw N copies, etc.

Canonical pattern in `modules/dasImgui/widgets/imgui_boost_runtime.das`:

```das
var private g_end_of_frame_hooks : table<string; function<() : void>>

def public register_end_of_frame_hook(name : string; hook : function<() : void>) {
    g_end_of_frame_hooks[name] = hook
}

def end_of_frame() {
    for (h in values(g_end_of_frame_hooks)) {
        invoke(h)
    }
}
```

Caller (in their own module) does:

```das
[init]
def my_overlay_init {
    register_end_of_frame_hook("my_overlay_painter", @@ paint_my_overlay)
}
```

Use function pointers (`function<...>` + `@@ paint_my_overlay`), not lambdas — painters read module globals, no capture needed, function pointers are workhorse types so storage is copy-by-value.

Same pattern in `register_post_command_hook` (also in `imgui_boost_runtime.das`) — fires after every accepted `imgui_*` command with `(command_name, resolved_target)`.

This generalizes: any hook registry in a module that survives reload needs name-keyed deduplication.

## Questions
- How do I register a per-frame or post-command hook in dasImgui v2 so daslang-live reloads don't accumulate duplicate firings?
