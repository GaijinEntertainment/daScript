# dasLLAMA Code Review Checklist

> Run this list on EVERY dasLLAMA change before it ships. It grows as the reorg settles
> more rules — when a review argument gets settled, the outcome lands here as a rule.

## Tests

1. **Run `modules/dasLLAMA/tests` before any PR.** Model/Metal suites go through the
   sanctioned scoped runner (`modules/dasLLAMA/tests/run.das` — invocation discipline in
   `modules/dasLLAMA/tests/CLAUDE.md`); never invoke dastest directly on the metal suites.
2. **Keep the tests multiplatform.** A test must pass — or skip *explicitly* through a
   capability/model gate (`_model_tier.das`, `builtin_module_exists`, format-support
   predicates) — on both the mac boxes (arm64 / Metal) and the PC (x64 / Vulkan). A test
   that silently vanishes on one platform is a review defect.
3. **All dasLLAMA tests live under `modules/dasLLAMA/tests/`.** `/tests/dasLLAMA` must
   not exist; anything and everything intended there goes under `modules/dasLLAMA/tests`.
   dasLLAMA inference runs **`-jit` only** — never interpreted, never AOT; nothing here
   registers with `test_aot`. The distinction is LIBRARY vs SCAFFOLDING: the library
   panics (hard stop) on a non-`-jit` model run; tools that only convert, drive, or
   debug (`.dlim` bake, batch runners spawning the real runner as a child) may run
   interpreted.
4. **Every moved or extracted bit ships with targeted tests for the bit itself** —
   unit-level (feed the function, check the bytes), in `modules/dasLLAMA/tests/`.
   "The LLM still runs" is not a test surface for a move.
5. **Do not add tests to cmake.** dasLLAMA tests are never registered in any
   `CMakeLists.txt` — no AOT registration, no ctest wiring. They run through
   `modules/dasLLAMA/tests/run.das` / dastest only.
6. **Run tests under `-jit` — never the interpreter, never AOT.** A test invocation
   without `-jit` is a review defect even if it happens to pass.
7. **Review test COST before running.** Know what a test loads before launching it: a
   test that loads a large GGUF (the >6 GiB tier) must sit behind the large-model gate
   (`model_available` + `DASLLAMA_PARITY_FULL=1` — see `tests/_model_tier.das` and
   `tests/CLAUDE.md`) and runs only as the FINAL pre-PR gate, never in the iteration
   loop. A new test that loads a big model ungated is a review defect.

## Structure

8. **All new repacks go into `dasllama_repack.das`.** Any disk-order → compute-order
   kernel-layout transform (grp interleave, extractor, panel unpack — any format, any
   platform) lands there; a repack implemented anywhere else is a review defect.
