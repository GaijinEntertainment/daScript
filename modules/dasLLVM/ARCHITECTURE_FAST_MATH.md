# dasLLVM architecture - the fast-math stamp

Companion to `ARCHITECTURE.md`: what `options fast_math` does to the emitted IR, and the one
annotation that takes a function out of it.

## 1. The stamp is per instruction, and `[never_fast_math]` withholds it {#never-fast-math}

Under `options fast_math` - the default for a program compiled by a host built with
`-ffast-math` or `/fp:fast` (`HOST_FAST_MATH`) - `apply_fast_math_to_module`
(`daslib/llvm_jit_run.das`) stamps `reassoc|nsz|contract` on every floating-point instruction that
carries no fast-math flags of its own, before the optimizer runs. The flags license each
instruction on its own, so two copies of one value may round differently, and the optimizer does
make copies: the SLP vectorizer can carry one loop-carried variable in two vector phis at once. A
recurrence whose feedback gain is above one amplifies the difference between those copies every
step. The strudel formant biquad, where `|b1|` is about 1.98, doubles it per sample and reaches
NaN inside 130 samples wherever the backend rounds the two copies apart, as it does for an
AVX-512 target. An AVX2 target lowers both copies the same way and stays exact, so the target
decides which machines break, not the source.

`[never_fast_math]` on a das function withholds the stamp. The emitter tags the function's impl,
and the impl of every block literal written in its body, with the string attribute
`das-never-fast-math` (`JIT_NEVER_FAST_MATH_ATTR`, `daslib/llvm_jit_plan.das`), and the stamp
pass skips a tagged function whole. The flags live on instructions, so the exemption survives
inlining: the function's arithmetic keeps no flags inside whatever loop LLVM inlines it into. The
AST inliner is the one splice that runs before the tag exists - it would move the arithmetic into
an unmarked caller - so the annotation implies `[never_inline]` and refuses `[inline]`. A
lambda or a generator in the body is a function of its own and takes its own annotation. The IR
the annotation changes is invisible to the AOT hash, so `fold_function_hints` folds it into both
cache keys (`ARCHITECTURE.md` sec.2).

The annotation acts on the LLVM tiers only. The interpreter never reassociates, and a C++ AOT
build under `DAS_FAST_MATH` compiles every translation unit with the host compiler's fast-math
flag, which no das annotation reaches.
