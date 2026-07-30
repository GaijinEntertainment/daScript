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

0. **THE PATTERN: a new module lands with its records, from the get-go.** Every new
   `dasllama_*.das` ships in the same change with (a) an ARCHITECTURE.md placement rule
   saying what belongs there, (b) a CODEREVIEW.md rule below so review catches strays,
   and (c) targeted tests. A module without its records is a review defect — that is how
   "things go where they belong" stays true after the reorg.

8. **All new repacks go into `dasllama_repack.das`.** Any disk-order → compute-order
   kernel-layout transform (grp interleave, extractor, panel unpack — any format, any
   platform) lands there; a repack implemented anywhere else is a review defect.
9. **All new conversions go into `dasllama_convert.das`.** Any tensor format conversion —
   quantize/dequantize/transcode/encode, byte readers for a codec, numeric widen/narrow —
   lands there, regardless of platform or which loader wants it; a conversion implemented
   anywhere else is a review defect. ONE carve-out: a conversion that is a KV-cache format's
   store/read half belongs to its codec family — rule 13.
10. **All RoPE angle/table generation goes into `dasllama_rope.das`.** The theta schedule,
    the `rope_freqs` divisor, fscale/mscale, and every materialized layout (two-tab tables,
    row tables, the packed device row). A fresh `1/pow(theta, 2j/hs)` loop anywhere else is
    a review defect. APPLICATION kernels stay with their backends (dasllama_math's
    `rope_scaled_*` leaves, the Metal/Vulkan fused rope-store kernels) — their per-shape
    specialization is deliberate hot-loop design; do NOT hand-merge them. Float multiply
    order in the builders is contractual (parity-pinned) — never "unify" it.
11. **GPU cooperation goes through `dasllama_gpu_tier.das`.** A GPU backend that cooperates
    with the CPU forward (per-op offload, resident-driver plumbing) implements THAT seam —
    hook types, install slots, want/status — never fresh function-pointer globals elsewhere.
    Whole-forward ownership goes through common's override registries (the contract Metal
    uses; Vulkan's resident driver registers there too). A backend reaching into the engine
    around these two seams — or the engine reaching into a backend — is a review defect.
    Direction between the styles: `followup_vulkan.md`.
12. **All format identity lives in `dasllama_kqformat.das`.** The `KqFmt` enum, the
    per-format descriptor table (strides, block geometry, stream codes), and format
    predicates. A new weight format = a new enum member + descriptor row THERE — never a
    fresh `if (fmt == ...)` ladder, never a second format-id space, never a local stride
    constant. An `int` carrying a format id crossing a module boundary is a review defect
    (pass the enum; cast at the IR/kernel-param boundary only).
13. **The KV-cache runtime codec lives in `dasllama_kv_codec.das` — families stay WHOLE.**
    A new cache format lands there as a complete family: store (quantize), read (dequant),
    score dot, V-accumulate axpy — its block geometry constants shared inside the module. A
    cache-format kernel implemented elsewhere, or a family member split across modules (an
    encode in one module writing block bytes a dot in another reads), is a review defect.
    Dispatch (`KVDtype`) stays at common's `kv_store_row`/`kv_load_row`/`kv_dot`/`kv_axpy`
    seam. Load-time tensor conversion is rule 9's territory; per-token cache codecs are
    this rule's.
14. **A module gaining its first `[tuned]`/`[tune]` kernel joins the `[tune_scope]`
    `covers=` list** (`dasllama_math_gen.das`) in the same change — otherwise the scope's
    completeness check silently stops demanding sidecar entries for those kernels and tune
    drift goes dark. Sidecars key kernels by BARE name, so MOVING a kernel between covered
    modules needs no re-tune and no sidecar edit.
15. **GPU backend code goes in its ROLE file.** Kernel source → `dasllama_<gpu>_kernels` (a
    `[compute_shader]`/MSL kernel in a driver file is a review defect); device state, buffer/
    command plumbing, rails, shared lazy-state builders → `dasllama_<gpu>_common`; the resident
    token-step driver + decode arms → `dasllama_<gpu>_decode`; the batched prefill driver +
    batch arms → `dasllama_<gpu>_prefill`; portable servability gates → `dasllama_<gpu>_shapes`;
    probe/arm/routers/`[init]` → `dasllama_math_<gpu>` — whose NAME is common's `?<gpu>` require
    contract, never rename it. Matching responsibilities get MATCHING file names across
    backends (kernels/common/decode/prefill/shapes/lens); a backend-only capability lives in
    its matching role file, never a new grab-bag.
16. **Vulkan descriptor sets build through `vk_set6`/`vk_write6`; kernel stages through the
    shared helpers.** A hand-rolled `write_buf_desc` six-pack (+ `update_descriptor_sets` +
    `hz_set_bits`) is a review defect — `vk_set6` allocates-and-writes, `vk_write6` rewrites in
    place. Likewise a new kernel that re-pastes a stage an existing helper covers (`wg_rms_inv`,
    `q8_blk_pack`/`q8k_*`, `kq_bt_*`, `kq_gemv_dm`/`q40_blk_d`, `fa_*`, `vk_region_rec`) is a
    review defect — extend the helper family instead. Two hard constraints on such helpers: the
    shader-function ABI takes only scalars/vectors/matrices/plain structs (fixed-array params
    are error 50501 — restructure per-element), and float op ORDER is contractual (keep the
    caller's `+=` granularity; never fold a sum before the accumulate). Deliberate variants
    (coopmat pair, h128 flash twin, per-format scale folds) stay separate — don't "unify" them.
