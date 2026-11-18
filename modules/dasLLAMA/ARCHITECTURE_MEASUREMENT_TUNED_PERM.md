# dasLLAMA Architecture - a `[tuned]` kernel's perm

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document carries
one section, `ARCHITECTURE_MEASUREMENT_TUNED_PERM.md#tuned-perm-precedence`: the order a `[tuned]`
kernel's perm is picked in at its own compile, and what a pick the grid does not carry does.

### A `[tuned]` kernel's perm is decided at its own compile {#tuned-perm-precedence}

`dasllama_tune.das` picks one perm per `[tuned]` kernel, first match wins: the `reference` tune
policy (the kill switch - the template's own loops, no hints stamped), a `perm=` pin on the
annotation, this box's sidecar `"kernels"` entry, this box's class entry in the shipped defaults
profile (`performance/defaults`), the annotation's `fallback` `;`-chain, then `DEFAULT_PERM`.
`tune_kernel_pick` (llvm_tune) reads the sidecar and the profile in that order and hands back the
FILE its answer came from, so a verbose compile names which of the two stamped each kernel. A box
the shipped profile covers therefore compiles tuned kernels without racing anything, and a box it
does not cover falls to the fallback chain - never to another box's winners. With no tune
framework in the build the first three steps do not exist - no policy env, no sidecar, no
profile - so the ladder is the `perm=` pin, then the `fallback` chain's first UNCONDITIONAL
entry (a `suffix:requires` seat cannot be judged with no feature probe), then `DEFAULT_PERM`,
and the compile reports nothing. `[dasllama_fallbacks]` reads each kernel's fallback off its `[tuned]`
annotation, so `dasllama_tuned_fallbacks()` is whole whether the kernel's module compiled in this
process or the front-end module cache served it. The picked perm is then looked up in the grid,
and a miss is handled by where the pick came from. A `perm=` pin names the row its author wants,
so a pin the grid does not carry fails the compile as an authoring error. Any other pick - a
sidecar or profile entry - is measured data, not law: one the grid no longer carries (a stale mint)
prints a line and stamps the annotation's fallback instead, so the compile that the tuner needs to
re-race the kernel still succeeds; a fallback the grid does not carry fails the compile.
