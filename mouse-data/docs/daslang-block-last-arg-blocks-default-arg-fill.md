---
slug: daslang-block-last-arg-blocks-default-arg-fill
title: In daslang gen2, why do default args after a positional `; blk : block` not work with the block-as-last-arg sugar?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Pitfall confirmed 2026-05-10 (dasImgui Phase 0b.3).** A function with shape `def foo(a, b=def, c=def, blk : block)` called via the trailing-block sugar `foo(arg) <| ${body}` does **NOT** auto-fill the b/c defaults so blk lands at param position 3. Instead daslang's positional resolution goes left-to-right: arg 0=a, arg 1=BLOCK → mismatched against b's type. Compile error of the shape `invalid argument 'b' (1). expecting 'bool const', passing 'block<void> const'`.

**Why**: the block-as-last-arg sugar appends BLOCK at the trailing positional slot of the *call*, after macro expansion. Daslang then matches positionally to the function's params left-to-right. There's no "shift defaults backwards to align a trailing block" logic.

**Fixes** (in priority order):

1. **Drop the defaults — make all per-call config args required.** User specifies them all via the named-tuple. This is the convention dasImgui's 0b.3 [container] defs use:
   ```das
   [container]
   def window(var state : WindowState; text : string; closable : bool;
              flags : ImGuiWindowFlags; blk : block) { ... }
   // Caller:
   window(MAIN, (text="Main", closable=false, flags=ImGuiWindowFlags.None)) { ... }
   ```
   The CallMacro destructures the named-tuple positionally (per `dasimgui-widget-named-tuple-args-positional-not-named`), so the call ends up `window(state, "MAIN", "Main", false, None) <| BLOCK` → 6 args total → matches the 6-param signature.

2. **Move "optional" config to state struct fields**, function takes only required args. User sets `MY_WIN.flags = ...` once in init, function reads `state.flags`. Loses ergonomics for one-shot configs.

3. **Two function definitions with different arities** — does NOT work for `[widget]`/`[container]`: the macro registers a CallMacro under the kind name once, and a duplicate registration panics in `add_call_macro` (C++ side).

4. **ExprNamedCall emission from the macro** — could let daslang's named-arg dispatch handle defaults. Confirmed unused in dasImgui (Boris explicitly preferred named-tuple); not the right tool here.

**`[widget]` (no block)** is unaffected — defaults at the end fill normally because there's no block to displace them. Only `[container]` (block-required) hits this.

## Questions
- In daslang gen2, why do default args after a positional `; blk : block` not work with the block-as-last-arg sugar?
