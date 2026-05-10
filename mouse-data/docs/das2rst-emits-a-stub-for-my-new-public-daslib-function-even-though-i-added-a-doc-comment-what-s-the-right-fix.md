---
slug: das2rst-emits-a-stub-for-my-new-public-daslib-function-even-though-i-added-a-doc-comment-what-s-the-right-fix
title: das2rst emits a // stub for my new public daslib function even though I added a //! doc-comment — what's the right fix?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

Two things are going on; the `documentation_rst.md` skill rolls them together as "add `//!` instead of filling the stub" but the real story has a wrinkle.

**1. `//!` placement — must be INSIDE the function body, not before `def`.**
For pure-daslang functions in `daslib/*.das`, `rst_comment.das` extracts `//!` comments only when they appear as the first line(s) of the function body:

```daslang
def public foo(x : int) : int {
    //! Docs go HERE — first lines of the body.
    //! Multi-line continues like this.
    return x + 1
}
```

`//!` placed *before* `def` is silently ignored. Symptom: regen still produces a `// stub` placeholder under `doc/source/stdlib/handmade/function-<module>-<name>-<hash>.rst`. The fix is to move the doc-comment inside the body and re-run `das2rst`.

**2. Some daslib modules expect BOTH a `//!` body comment AND a per-symbol `handmade/*.rst`.**
Modules like `strings_boost`, `fio`, and other long-established ones have a per-symbol `handmade/*.rst` for *every* function — see e.g. `function-strings_boost-levenshtein_distance-0xbb5a4a3017b240a5.rst`. When you add a new public function to one of those modules, `das2rst` will emit a fresh `// stub` even with correctly-placed `//!`. The convention there is: keep the `//!` (it lands in `detail/`) **and** fill the stub with a 1-2 sentence handmade description. Don't delete the stub — re-running `das2rst` recreates it.

Newer modules like `archive`, `json_boost`, `command_line` don't have per-symbol handmade entries; for those, `//!` alone is enough.

**How to apply:**
- New daslib public function → add `//!` inside body first.
- Run `bin/daslang doc/reflections/das2rst.das`.
- `grep -rln "// stub" doc/source/stdlib/handmade/` — if your function appears, fill that file with a short description (plain text, no RST directives). If your function doesn't appear, you're done.
- Re-run `das2rst` to confirm clean.
- Verify `grep -c Uncategorized doc/source/stdlib/generated/*.rst | grep -v ':0$'` is empty (means the function is in a `group_by_regex` group in `das2rst.das`).

## Questions
- das2rst emits a // stub for my new public daslib function even though I added a //! doc-comment — what's the right fix?
