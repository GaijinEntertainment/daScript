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
18. **Every logits-checking test also logs decoded TEXT.** Any cell that compares logits
    (tolerance compares, teacher-forced feeds) `to_log`s a decoded-text form of both sides
    (the forced stream plus the GPU's greedy would-be picks, or at minimum both next-token
    pieces), and every token-for-token generate cell logs both decoded streams
    (`log_gen_texts` in `tests/_model_tier.das`) — a red, or a suspicious green, must be
    eyeball-able as text in the log, not just an id/float diff. A numeric-only parity cell
    is a review defect.
19. **Never key a GPU-resident cache by host address alone.** An address-keyed entry carries
    its SPAN, a hit must cover the request (a shorter first upload must never serve a wider
    later one), and different upload FORMS (plain span vs concat) live in separate tables so
    they can never alias — the metal `RegionEntry` rail is the model. Buffers grown out of an
    entry retire to a list released only at quiesce boundaries: unretained command buffers may
    still bind them. A new address-keyed cache without span+form in its identity is a review
    defect.
20. **Test suites load models with `load_model_` — never the image rail.** `load_model` /
    `load_model_cached` mint identity-stamped `.dlim` flavors and GC-purge siblings; a suite
    child's pinned identity (backend pin, wscale, tune manifest) differs from the serving
    rig's, so a suite on the rail both re-mints multi-GB images the rig cannot use and purges
    the flavors the rig depends on. Image-rail coverage (mint, map, GC, flavors) belongs to
    the image suites alone (`test_model_image`, `test_model_image_vulkan`). A non-image suite
    calling the image rail is a review defect.

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
    vulkan's probe/arm/routers/`[init]` → `dasllama_math_vulkan` — whose NAME is common's
    `?vulkan` require contract, never rename it (metal has no math_ entry: transformer +
    shapes are its doors, and `dasllama_metal_gemm` is the below-common batch-GEMM donor,
    not a facade). Matching responsibilities get MATCHING file names across
    backends (kernels/common/decode/prefill/shapes/lens); a backend-only capability lives in
    its matching role file, never a new grab-bag. Metal specifics: the `[metal_dispatch]`
    lens generates enc_* builders and MSL globals into the module the class COMPILES in, so
    a kernel class carries its whole dispatch surface wherever it's placed — "the builder
    needs the driver module" is never a reason to put a kernel in prefill/decode. A NEW
    `[metal_kernel]` class goes in `dasllama_metal_kernels`; the 33 prefill-only classes
    still sitting in `dasllama_metal_prefill` are convergence debt, not precedent — don't
    add beside them.

## Implementation

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
17. **A new GPU kernel ships with a small model in the kernel coverage oracle.** When a
    change adds a kernel (a `[metal_kernel]` class, a vulkan `[compute_shader]`), the kernel
    coverage suite (small-model dispatch census, metal + vulkan) gains a run that actually
    DISPATCHES it — the smallest model/shape that reaches the kernel. A kernel no covered
    run dispatches surfaces as a LOUD warning, never an auto-dead verdict; adding a kernel
    without extending the coverage oracle is a review defect.
21. **No dynamic dispatch in a kernel: SHAPE is compile-time, only DATA is runtime.** The test
    is one question — *for a given compiled kernel, can this value change between dispatches?*
    If yes it is DATA (context depth, row counts, buffer offsets, `kv_dim`, scales, head
    counts) and belongs in a uniform or a kargs struct. If no, it is SHAPE (a codec's block
    stride, a scale-plane stride, a lane width, an unroll factor, a format selector) and must
    NOT reach the kernel as a uniform, a kargs field, or a helper parameter. Shape belongs to
    the specialization: a separate kernel class and PSO, a per-codec overload, a monomorphized
    generic, or a `static_if` on a compile-time witness. Handing a shape constant over as a
    value and trusting the shader compiler to inline-and-fold it back is an assumption, not a
    guarantee, and it is worth nothing in the kernels that matter. The same rule bans
    indirection in a kernel body — no function pointers, no vtables; class methods devirtualize
    statically or the emitter refuses them.

    **Verify against the EMITTED shader, never the das source.** Read the `*_msl` global (or
    the SPIR-V dump) and confirm the constant is literal there: `blk * 34u`, not `blk * bstr`.
    A helper that looks specialized in das can still lower to a runtime multiply.

22. **Complexity/length lint: suppress an honest shape, never force a split.** STYLE037
    (cyclomatic) and STYLE038 (line count) are prompts to look, not orders to refactor. This
    module has shapes that are irreducible by design and they take `// nolint:STYLE03x` on the
    `def` line with a one-line reason: the flat one-call-per-item runs (`metal_decode_init`'s
    per-kernel `compile_pso` list, `metal_kernels_release`'s `release_pso` list), and GPU kernel
    bodies whose phases are coupled by `barrier()`, simdgroup ops or register residency and so
    cannot cross a function boundary without changing the shader. Split only where a real seam
    exists — genuine duplication (two near-identical kernel bodies that should share stages, the
    way the split-K family shares `sqd_*`), a distinct phase, a self-contained arm — and only
    when the extracted helper stands on its own as a function.

    Two corollaries this module keeps tripping over. **A kargs fold that grows an already-over-cap
    kernel body is not a reason to abandon the fold**: unpacking N fields adds N lines, and if
    each field is read several times, inlining `ka.field` at every use is noisier rather than
    shorter — take the growth and either suppress or ledger the real seam. And **never suppress a
    function you have just argued is reducible** — if it is on the follow-up ledger as wanting a
    dedup, it keeps its warning until the dedup lands.

23. **THERE IS ONE WAY TO LOAD A MODEL. Any other is a review FAIL.** A weight carrier becomes
    a live struct through exactly two functions in `dasllama_image.das`, and nothing else may
    read weights into one:

    - **`build_image`** walks a carrier's planes into a sink — a `.dlim` file or a page-aligned
      memory chunk. Every prepared image in the process comes out of this walk. A second walk
      that emits image bytes is a review defect; the streaming form is a plane HOOK on this
      walk, not a copy of it (that is precisely the drift this rule exists to prevent — the
      streaming twin had already lost the nested-weight-carrier arm before they were merged).
    - **`parse_image`** turns `(base, bytes)` into borrowed-plane fields. It does not know or
      care where the bytes came from. `load_image` (mmap a file) and `adopt_image` (a chunk this
      process built) are its only two wrappers, and both exist so the release side stays honest.

    So **a cold load and a warm load produce the same struct**: planes borrowed over a prepared
    image, `image_map` non-null, released through `image_backing_release`. Cold reaches it by
    building the image and handing off through the file (write, drop the model, map); warm by
    mapping the file it finds. `cache_via_image` is that handoff for every carrier — audio,
    whisper, parakeet, canary, qwen3a, gemma4a and Model all call it, and a new carrier that
    hand-rolls "load eagerly, save, return the eager struct" is a review FAIL.

    Why the rule is this strict: that hand-rolled tail is what the module had, six times over,
    and it meant the mapped path only ever ran on a SECOND load — so on Metal it had never run
    at all. A forked loader does not fail loudly; it silently halves your coverage.

    Two contracts the rule carries:

    - **Never hold the model and its image at once.** The file is the handoff for that reason.
      Building into a chunk keeps the largest plane live in both places while it is copied
      between them; measured on an 8 GB gguf that is 12.3 GB → 20.5 GB of peak footprint. Only
      take the chunk when there is nowhere to write (`DASLLAMA_IMAGE_SAVE=0`).
    - **Prefer the streamed source.** `save_model_image_streaming` /
      `image_from_model_streaming` transcode planes from the gguf mapping straight into the
      image, so the planes never materialize — 12.3 GB → 4.0 GB on the same model. Only the
      blob flavors take the eager rail, and only because their transforms need whole planes.

    Test-side: the image suites assert a cold load is image-backed. That assertion IS the rule —
    if a change makes it fail, a second rail has grown back. Since cold and warm can no longer
    check each other, every round-trip cell also loads OFF the rail (`load_model_`,
    `load_whisper_model_`, …) for the gguf-vs-image compare; a cell that drops that control is
    comparing an image to itself.
