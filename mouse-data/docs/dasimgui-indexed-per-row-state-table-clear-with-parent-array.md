---
slug: dasimgui-indexed-per-row-state-table-clear-with-parent-array
title: My loop renders one widget per item using an indexed `table<int; State>` for per-row state. When my "Clear" action clears the parent items array, does the indexed table also need to be cleared, or does it self-prune?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Clear the indexed `table<int; State>` explicitly alongside the parent array.** Daslang tables don't self-prune by parent-reference — once you `insert` an entry for key `i`, it stays until you `erase(i)` or `clear()` the whole table.

The lifecycle pattern in `examples/imgui_demo/app_console.das`:

```das
var private CONSOLE_ITEMS : array<string>
var private CONSOLE_ITEM_TEXT : table<int; NarrativeState>   // indexed per-row widget state

def private clear_log() {
    CONSOLE_ITEMS |> clear()
    CONSOLE_ITEM_TEXT |> clear()    // <-- required; without this the
                                    // table retains all prior entries
}
```

**What you observe without the table-clear:**

- The table grows to the high-water mark of all past `CONSOLE_ITEMS` lengths and stays there. After "flood with 1000 lines, then Clear", the table holds 1000 stale entries until the next flood overwrites them by index.
- It's *bounded* (max key < max-past-length) — not strictly an unbounded leak — so the issue is less "memory leak" and more "stale state visible in `/snapshot` after the user thinks they cleared everything" and "unnecessarily retained per-row widget state structs".
- PR #32 round 3 caught exactly this gap on `clear_log()` — Copilot flagged it as a hygiene contract violation.

**Why `[widget]` auto-emit doesn't help here:** the auto-emitted state globals are for *scalar* state structs (one per call site). When the call site is inside a `for (i in range(N))` loop, you have to declare the indexed `table<int; State>` at module scope yourself (as the [container] / [widget] macros don't synthesize per-iteration globals). That means lifecycle is also your responsibility — see the indexed-widget how-to: `how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i`.

**Rule of thumb:** every place you have a "clear the parent collection" action (`clear_log`, "remove all tabs", "reset all entries"), look for sibling indexed widget-state tables tied to that collection and `|> clear()` them in the same operation.

## Questions
- My loop renders one widget per item using an indexed `table<int; State>` for per-row state. When my "Clear" action clears the parent items array, does the indexed table also need to be cleared, or does it self-prune?
