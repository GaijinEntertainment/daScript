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
   dasLLAMA runs **`-jit` only** — never interpreted, never AOT; nothing here registers
   with `test_aot`, and the library panics (hard stop) on a non-`-jit` run.
4. **Every moved or extracted bit ships with targeted tests for the bit itself** —
   unit-level (feed the function, check the bytes), in `modules/dasLLAMA/tests/`.
   "The LLM still runs" is not a test surface for a move.
5. **Do not add tests to cmake.** dasLLAMA tests are never registered in any
   `CMakeLists.txt` — no AOT registration, no ctest wiring. They run through
   `modules/dasLLAMA/tests/run.das` / dastest only.
6. **Run tests under `-jit` — never the interpreter, never AOT.** A test invocation
   without `-jit` is a review defect even if it happens to pass.
