---
slug: sphinx-w-fails-on-my-pr-branch-with-undefined-label-struct-module-x-but-master-ci-is-green-what-causes-this-hash-order-flip-and
title: Sphinx -W fails on my PR branch with "undefined label: 'struct-MODULE-X'" but master CI is green — what causes this hash-order flip and how do I fix it?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Symptom:** CI doc workflow's `sphinx-build -W` fails on your branch only. Error: `doc/source/stdlib/generated/MODULE.rst:N: WARNING: undefined label: 'struct-MODULE-StructName'`. Master CI is green for the exact same `daslib/*.das` source.

**Diagnosis:** `doc/reflections/das2rst.das` emits the **full** RST detail (description + `:Arguments:` block with `:ref:` to param types) only for the **first** function of a same-named overload group; the rest get a stub `.. das:function::` label + signature only. The iteration order over functions is **hash-bucket-dependent** — binary layout changes (new templates, added install paths, anything that shifts pointer/address-based hashing) can flip which overload wins the "first detailed" slot.

If the winning overload references a **private struct** as a parameter type, das2rst still emits `:ref:`StructName <struct-MODULE-StructName>``, but the struct label is never defined (das2rst only emits Structures-section entries for public structs). Sphinx -W then fails with the undefined-label warning.

**Verification pattern:**
1. Build master's daslang in a worktree (`git worktree add /tmp/repo-master master`)
2. Run `./bin/daslang doc/reflections/das2rst.das` on each
3. `diff` the affected `.rst` — if the broken `:ref:` appears in a *different* overload's `:Arguments:` block on master, you've confirmed the order-flip pattern.

**Durable fix:** Mark the struct `public` in the `.das` source. Since the function exposing it is `public`, the struct already needs to be reachable from caller code anyway — the private qualifier was the latent bug. Example from `daslib/quote.das` (PR #2707):
```das
struct public CaptureEntryInitData {
    //! Initialization data for a captured variable entry.
    name : string       //! Variable name being captured.
    mode : CaptureMode  //! Capture mode (copy, reference, move, or clone).
}
```

**Caveat — public structs need per-field `//!` docstrings.** Otherwise `daslang doc/reflections/das2rst.das` PANICs with `"... has less documentation than values. Expected at least N lines, got 1"`. The struct's main `//!` + one `//!` per field is the convention (see `LineInfoInitData` in `daslib/quote.das`).

**Don't:** Add a handmade RST stub under `doc/source/stdlib/handmade/` for the struct — handmade files are only consulted for C++ builtin modules. For daslang modules das2rst regenerates from the source.

**Reference:** PR #2707, commit `dd9b250ca`; my added template instantiations in `include/daScript/simulate/das_qsort_r.h` shifted hash buckets so `CaptureEntryInitData` won the detailed slot over `LineInfoInitData`.</body>
<slug>sphinx-w-undefined-label-private-struct-public-fn-hash-order-flip</slug>
</invoke>

## Questions
- Sphinx -W fails on my PR branch with "undefined label: 'struct-MODULE-X'" but master CI is green — what causes this hash-order flip and how do I fix it?
