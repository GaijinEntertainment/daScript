---
slug: daslang-label-reserved-word-gen2
title: Is `label` a reserved word in daslang gen2?
created: 2026-05-14
last_verified: 2026-05-15
links: []
---

Yes — `label` is a daslang keyword in gen2 (confirmed by Boris 2026-05-14). The reservation is **lexical**, not contextual — it bites in every identifier position: `let`/`var`, **struct fields**, **function parameters**, **named-tuple member names**.

Same error from all four shapes:

    let label = "..."                                   // ← error[30151]
    var label : string                                  // ← error[30151]
    struct LabelTextState { label : string }            // ← error[30151] (hit during v2 boost mega-detour 2026-05-14)
    def label_text(label : string; value : string)      // ← error[30151] (same)

Error reads: `error[30151]: syntax error, unexpected label, expecting $i or name`.

Workaround: rename. The mega-detour's `LabelTextState`/`label_text` adopted `key : string` for the left-side string (and the `[widget]` body calls `LabelText(key, value)` — note ImGui's C++ API param IS named `label`, so the C-side binding is unaffected; only the daslang identifier breaks).

The reserved-word list also includes the obvious ones (`if`, `for`, etc.) and a handful that read as ordinary identifiers in other languages — `label` is the one that catches you out because `label` reads as a perfectly reasonable variable / field / param name for UI text content.

Don't infer the full reserved-word list from this card — when in doubt, grep `ds2_parser.ypp` (the grammar is the source of truth) or just try compiling.

## Questions
- Is `label` a reserved word in daslang gen2?
