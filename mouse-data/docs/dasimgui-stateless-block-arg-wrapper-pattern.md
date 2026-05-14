---
slug: dasimgui-stateless-block-arg-wrapper-pattern
title: How do I write a dasImgui stateless block-arg helper like with_id that wraps a Push/Pop pair?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Plain `def` with a trailing `blk : block` arg — **no annotation, no state struct**. Pattern lives in `modules/dasImgui/widgets/imgui_id_builtin.das` and `imgui_scope_builtin.das`.

Canonical shape:

    def public with_indent(amount : float; blk : block) {
        //! 1-2 line docstring describing the Push/Pop pair being scoped.
        Indent(amount)
        invoke(blk)
        Unindent(amount)
    }

Key constraints:

- **No `[container]` annotation** — stateless wrappers don't ID anything, don't need `container_path_push`/`pop`, and don't register a snapshot entry. `[container]` is for Begin/End scopes that gate the body and own path-tree position.
- **No `[widget]` annotation either** — those auto-emit a state global and dispatch named-tuple args; way too much machinery for a pure Push/Pop bracket.
- **No default on the leading param when followed by a block** — daslang's trailing-block sugar parses `with_foo() { ... }` as "block in the first arg slot" and binds the block to `amount` (wrong type). Drop the default; callers pass `0.0f` explicitly. See `dasimgui-scope-wrapper-default-arg-trailing-block-conflict.md`.
- **`finally` is unreliable on panic** (Issue #2532) — don't bother with `{ ... } finally { Pop... }`. On ImGui panic mid-frame the state machine is hosed regardless.

Companion: dotted-flag wrappers like `with_id(s) { ... }` push *both* ImGui's ID stack and the boost container-path:

    def public with_id(s : string; blk : block) {
        container_path_push(s)
        PushID(s)
        invoke(blk)
        PopID()
        container_path_pop()
    }

Use this shape when the scope contributes to widget identity. Bare Push/Pop wrappers (indent, item width, text wrap pos, style overrides) skip the path push since they don't ID anything.

**Where to register the new module**: add a `register_native_path("imgui", "imgui_<name>_builtin", "{project_path}/widgets/imgui_<name>_builtin.das")` line to `modules/dasImgui/.das_module`. Existing entries cluster the boost surface under `imgui_*_builtin` names.

Shipped this pattern in PR #28 (2026-05-14): `widgets/imgui_scope_builtin.das` with `with_indent`, `with_item_width`, `with_text_wrap_pos`.

## Questions
- How do I write a dasImgui stateless block-arg helper like with_id that wraps a Push/Pop pair?
