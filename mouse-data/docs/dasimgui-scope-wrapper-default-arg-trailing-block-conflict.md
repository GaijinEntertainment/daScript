---
slug: dasimgui-scope-wrapper-default-arg-trailing-block-conflict
title: Why does my dasImgui scope wrapper `def with_foo(amount = 0.0f; blk : block) { ... }` fail to parse `with_foo() { ... }`?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

Trailing-block sugar (`fn() { body }`) doesn't compose with a default-value preceding parameter. When you write:

    def with_indent(amount : float = 0.0f; blk : block) {
        Indent(amount); invoke(blk); Unindent(amount)
    }

…and call `with_indent() { … }`, daslang sees the trailing block as the FIRST argument (slot for `amount`) and errors with:

    invalid argument 'amount' (0). expecting 'float const', passing 'block<void> const'
    missing argument blk

The trailing-block sugar treats the block as the sole arg in source-position-1, not as the last arg. So a default on the first param can't be "skipped" to leave it bound while the block lands in the last slot.

Fix: drop the default. Require callers to pass the value explicitly:

    def with_indent(amount : float; blk : block) { ... }
    // call site:
    with_indent(0.0f) { ... }   // "default" case
    with_indent(40.0f) { ... }  // custom

This is the pattern `imgui/imgui_id_builtin.das`'s `with_id(s : string; blk : block)` uses — no defaults, explicit args.

The wip.das example `def foo(a : int; blk : block = ${})` works ONLY because the block has the default — caller picks `foo(42)` (no block) OR `foo(42) { ... }` (with block). But `foo()` alone (no args) doesn't work — there's no default on `a`.

Bit me 2026-05-14 building `with_indent`/`with_text_wrap_pos` in dasImgui boost scope wrappers.

## Questions
- Why does my dasImgui scope wrapper `def with_foo(amount = 0.0f; blk : block) { ... }` fail to parse `with_foo() { ... }`?
