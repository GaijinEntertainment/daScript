# Vulkan class kernels arc — port the Metal kernel model to the SPIR-V/Vulkan tier

## Context

`modules/dasLLAMA/followup_vulkan.md` item 9, promoted to its own arc (Boris ruling: separate from
the reorg, one longish PR). Today's Vulkan kernels are zero-parameter free functions over module
globals: 15 `@ssbo` globals aliased into 6 shared binding slots across all 43 kernels, params as
raw indices into a `vk_meta` word buffer, one shared `VkDescriptorSetLayout`/pool, and 74
hand-written 18-argument `vk_set6`/`pf_bind` call sites with dummy-buffer filler. Nothing has a
per-kernel interface, so nothing can be generated, checked, or unit-tested; the duplication
INVENTORY measured (meta unpack ×12, subgroup reduce ×5, kq tiles ~91% identical ×4, GEMV shells
×6, `ensure_*` ×18) follows directly.

Metal solved this shape in-tree and it's proven: **a kernel is a class** — resources are annotated
members, kernels are methods, the body is ordinary daslang (CPU-runnable = the oracle), and a
second macro (`[metal_dispatch]`) generates the whole host encoder from the same members. The
masterplan explicitly deferred "dasSpirv gains the same class-member authoring" as an in-tree
follow-up, ours to make. This arc makes it.

**End state:** everything Vulkan in dasLLAMA runs on the class machinery. The module-globals
`[compute_shader]` frontend stays supported in dasSpirv (shipped feature, escape hatch), but the
dasLLAMA tier uses none of it. One way to write a kernel in daslang; the backend is a target, not
a dialect.

## Settled decisions (from the discussion, 2026-08-02)

1. **Full port, one PR.** Refactor-arc style: the daScript PR lands when the dust settles —
   kernels, both drivers, seams, tests. Internal sequencing exists (below) but no intermediate PRs.
   One corresponding **dasVulkan PR** carries that repo's fixes. Branch policy: everything on one
   branch (`bbatkin/vulkan-class-kernels`), no side branches.
2. **Params ride push constants.** `@push_constant ka : Args` struct member — named fields
   end-to-end, recorded inline in the command buffer: no meta write+copy+barrier per dispatch chain
   (decode is launch-bound; this is a strict improvement). Fixed blocks fit easily (fattest today =
   dn's 80 B vs 128 B guaranteed / 256 B on NVIDIA). Variable-length region-pair tails stay SSBO
   (bulk data, not params). Escape hatch: a params block can be an `@ssbo` member if one ever
   outgrows the limit. Precondition: fix the dasVulkan generated pusher (drops small trailing
   scalars — the reason `vk_meta` exists at all; see `dasllama_vulkan_kernels.das:25-29`).
   **Phase-0 refinement:** the decode submission model v2 PRE-RECORDS command buffers per stack
   and refreshes only mapped memory per token (`dasllama_vulkan_common.das:29-33`) — push values
   are frozen at record time, so per-token-varying params cannot ride push constants on those
   rails without per-token re-record. The `[vk_dispatch]` generator therefore supports BOTH
   carriers per class — push constants (re-recorded rails: prefill, batch, seams) and a mapped
   params buffer (pre-recorded decode sets) — with identical named-struct authoring either way.
   Whether decode moves to per-token re-record (push everywhere) or keeps mapped params is a
   phase-5 measurement, not an up-front bet.
3. **One descriptor set per class.** `@set` parsed but must be 0 (clean error — Metal-consistent
   restraint). Per-class `VkDescriptorSetLayout` generated from members replaces the shared 6-slot
   layout. Generated set-builder caches sets keyed by (class, bound resources) — preserves today's
   build-once-per-layer/role, reuse-across-tokens pattern. Pool sizing becomes derived from the
   plan instead of the guessed `MAX_STACKS` constant; set-alloc failure is fail-closed (the silent
   −14% tg DN-layer drop class dies here).
4. **Workgroup width stays declared on the kernel** (`local_size_x=` as today — Vulkan bakes width
   at pipeline build; there is no dispatch-time choice). The generated dispatch derives group
   counts from `grid=` + the declared width — the hand-computed ceil-divides against a
   hand-mirrored `WG_X` disappear. Spec-constant wiring (`local_size_x_id`, declared today but
   never fed) becomes an **opt-in tune hatch** per kernel: declared-tunable vs structural (a body
   whose logic bakes the width says so and is never retuned).
5. **Hazards keep the region-bit rail** (`uint[6]`-style masks → per-class derived bits). The
   encoder declares bits derived by the access lens; the byte-range tracker (Metal's model) is a
   measured-need follow-up, noted because arena aliasing is where coarse bits over-barrier — only
   if `DASLLAMA_GPU_PROF` ever shows barrier stalls.
6. **Ownership split copies Metal:** class-member frontend in **dasSpirv** (the emitter's scan);
   `[vk_dispatch]` — encoder generation, set cache, hazard bits, census — in **dasLLAMA** next to
   the vulkan lens. Copy Metal's machinery wholesale where it fits (member scan, write-set,
   flatten+devirtualize, fail-closed suite shape, companion-capture); it's transplant, not
   invention.
7. **Multiple kernel methods per class** over one binding set + one params struct — the family
   pattern (kq GEMV ×4, `ar_*` fused family). One set layout serves several pipelines; the decode
   ladder's same-set-different-pipeline usage falls out for free.
8. **M1/MoltenVK is a late phase** with the ssh M1 as gate. Acceptance = tier suites green on the
   sdot4 rail with capability-gated *declines* (no coopmat under MoltenVK; `dn_shared_ok` false on
   the 32 KB shared-mem cap), correctness only — Metal remains the fast path on that box by
   design. Clear-handoff exit allowed if MoltenVK fights structurally after per-kernel bisection.
9. **Audit fixes ride phase 0** (standalone, correct regardless of the arc): `stream_reserve`
   never written (sizing always takes the legacy constant — wire it from the real carve or delete
   the field), `batch_y2` 64 MB idle **host** staging buffer (device twin is live; drop or
   lazy-alloc the host one), loud panic on desc-set alloc failure as interim protection.

## Phases

### Phase 0 — foundations + corresponding-repo fixes
- **dasVulkan branch** (`bbatkin/vulkan-class-arc` or continue `bbatkin/molten-vk-portability`):
  - Fix the generated push-constant pusher (trailing-small-scalars bug, ROADMAP item).
  - Land the portability_subset fix (9e2b8ec, already pushed to borisbat/dasVulkan) + its declared
    follow-up: sweep the other 12 `vkCreateDevice` sites for the same transparent extension append.
- **dasLLAMA small fixes** (audit items): `stream_reserve` (`dasllama_vulkan_common.das:489`, read
  at `dasllama_vulkan_prefill.das:1231`), `batch_y2` host buffer
  (`dasllama_vulkan_common.das:461,2710`), fail-closed `alloc_desc_set`.
- Commit this plan as `plans/vulkan_class_kernels.md` (retire to `history/` when the PR lands).
- Gate: tier suites green on zen2, unchanged numbers (nothing perf-visible yet); pusher fix proven
  by a dasVulkan-side test with a trailing-scalar params struct.

### Phase 1 — dasSpirv class-member frontend
- New function annotation **`[spirv_kernel]`** for the class-method form (mirrors
  `[metal_kernel]`; an annotation name cannot serve both a function and a class, and the
  class-level name is `[vk_dispatch]` — distinct names throughout). The shipped module-globals
  `[compute_shader]` frontend is **untouched** — zero regression surface on existing users.
- `[spirv_kernel]` on a **class method**: member scan for
  `@ssbo`/`@uniform`/`@push_constant`/`@workgroup` `@binding` fields (analog of `msl_emit.das`
  `scan_members`), write-set derivation, `exports=true` in apply / `neverInline` patch / emit in
  fixup — the apply/patch/fixup division dasMetal already probe-proved on methods
  (`msl_shader.das:52-116`). Method calls, free functions, inheritance: flatten + static
  devirtualization per `msl_emit.das:451-476, 2570-2619`, adapted to SPIR-V function emission
  (no struct-with-methods needed — SPIR-V has functions; members lower to the same per-kernel
  `OpVariable`s the globals frontend produces today).
- Emitter back half unchanged: SPIR-V words + reflection capture as today (`spirv_shader.das`).
  Builtins unchanged (lingua franca). `@set != 0` = clean error.
- **Fail-closed suite**: `tests/spirv/_fail_closed/` grows the class-frontend fixtures mirroring
  `tests/msl/_fail_closed/` (recursion, abstract method, written uniform/push, `@workgroup`
  dynamic array, address-space mismatches, untraceable pointer provenance…). Census discipline per
  dasSpirv's existing gates; spirv-val on every fixture.
- Gate: an a*b class kernel through the real emitter, byte-compared against the same body written
  module-globals style (both frontends must produce equivalent modules); `tests/spirv` green.

### Phase 2 — `[vk_dispatch]` in dasLLAMA
- Structure macro (sibling of `[metal_dispatch]`, `dasllama_metal_lens.das:288-530`): generates
  per-class layout creation, the cached set-builder, `enc_*` functions — get-or-build set → bind →
  push params → declare hazard bits → dispatch with derived groups. `grid=`/`params=` micro-grammar
  copied from Metal's.
- Access derivation: `dasllama_kernel_access.das` `fieldMode` already handles `self.<name>` roots —
  wire it as the vulkan lens's classifier for classes; `@role` stays a cross-checked assertion.
- Per-class pipeline creation (+ spec-constant feed for declared-tunable kernels); pipelines still
  register with the hazard rail (`hz_pipe_register` analog keyed per class/method).
- Compile-time **dispatch census** (Metal's `MetalManualDispatchCensus` analog): manual
  `vkCmdDispatch`/hand-built sets forbidden in `dasllama*vulkan*` modules — armed at the END of the
  port (phase 5), authored here.
- Gate: one toy class dispatched through the generated encoder against a CPU run of the same
  method, on zen2.

### Phase 3 — bring-up pilot: the `ar_*` fused elementwise family
- First real port: add_rms / requant fusions as one class (small bodies, few bindings). Validated
  against the **existing** single-op seams and their tests (`vk_add_rms` et al.,
  `test_vulkan_tier.das:2401-2743` CPU references).
- Bootstrap the **model-less kernel unit suite**: `modules/dasLLAMA/tests/test_vulkan_kernels.das`
  (analog of `test_metal_decode_kernels.das`) — bind, push, dispatch one kernel vs the CPU run of
  the same method. This suite is also the M1 bisection tool (phase 6).
- Gate: bit-identity vs the old kernels where the math is unchanged (same-probe master-vs-branch,
  `_vk_prefill_parity.das` pattern); tier suites green.

### Phase 4 — kernel families port (the bulk)
- Family by family, each becoming one class with N entry methods + one params struct: kq decode
  GEMVs (4→1), batch GEMM tiles (4 near-identical ×~120 lines), attention (at/da pair), rope+KV
  store, quant/cvt/requant, deltanet set, MoE kernels, embed/argmax. The ×12 region-list unpack and
  ×5 subgroup reduce become shared methods/free functions.
- Every family gates before the next starts: kernel-unit tests + the relevant tier arms +
  spirv-val; bit-identity where the port is mechanical, IDS-parity where float order legitimately
  moves (documented per family, drift-class only with reason).
- Note: `_vk_prefill_parity.das` is a degenerate fixture on Q4_K_M (banked finding) — don't trust
  it as a kq gate; the tier's kq/q40/mixed arms are the real gate there.

### Phase 5 — drivers rewire + old machinery deletion
- Decode (`dasllama_vulkan_decode.das` ladders at :147-266 and friends), prefill (`pf_bind` ×19,
  `PF_ROLES` loop), common seams: move to generated encoders. Delete `vk_set6`/`vk_write6`/
  `pf_bind`, the dummy filler buffers, the `fill_*_meta` family, hand group-count math; META
  shrinks to region-list tails. Descriptor pool sizing derived from the plan; the census (phase 2)
  arms.
- Gate: full zen2 re-gate — vulkan_tier 34/34, cm2 6/6, gpu_tier, slot_swap, image_vulkan,
  paranoid==elided, lint+format. Then the **perf gate** on the quiet box, one row at a time vs
  banked records (lcpp_bench, 16 threads, DASLLAMA_BOX=zen2): bar = no regression on the canonical
  rows; push-constant removal of the meta copy+barrier may show as a small tg win — measure,
  don't promise. Boris flags before any judged measurement.

### Phase 6 — M1 / MoltenVK
- Rig: ssh M1. Sequence deliberately after phase 3's kernel-unit suite exists: bisect the known
  `ffn_vs_ref` parity red + `add_rms` flake per-kernel instead of whole-forward.
- Capability declines become test-visible skips (cm2 suite gates on the coopmat extension;
  `dn_conv` declined by `dn_shared_ok` — both working as designed today, just reported as red).
- Acceptance: tier suites green on the sdot4 rail with declines; correctness only, no perf chase.
  Exit hatch: if MoltenVK fights structurally, write the clear handoff instead.

### Phase 7 — closing
- Docs: ARCHITECTURE.md + INVENTORY.md deltas, `followup_vulkan.md` item 9 marked done,
  `skills/*` factual updates if any emitter claims changed. Plan doc → `history/dasLLAMA/` per
  `skills/doc_archiving.md`.
- PRs: the daScript arc PR + the dasVulkan PR (externals-merge-first ordering per
  `skills/abi_break_sweep.md` if any dasVulkan API surface moved). Standard `skills/make_pr.md` +
  `skills/preflight.md` checklists; babysit per `skills/babysit.md`, one push per review round.

## Key files

- dasSpirv frontend: `modules/dasSpirv/spirv/spirv_shader.das`, `spirv_emit.das`
  (`classify_global` → member classification), `spirv_reflect.das` (unchanged).
- Metal donors (read-only models): `modules/dasMetal/metal/msl_emit.das` (scan_members :191-325,
  devirtualize :451-476, flatten :2570-2619), `msl_shader.das` (:52-116),
  `modules/dasLLAMA/dasllama/dasllama_metal_lens.das` (`[metal_dispatch]` :288-530, census
  :556-578), `dasllama_metal_common.das` (kn_* rail, hazard ranges).
- Vulkan tier (rewritten): `dasllama_vulkan_kernels.das`, `_common.das`, `_decode.das`,
  `_prefill.das`, `dasllama_vulkan_lens.das`, `dasllama_kernel_access.das` (fieldMode reuse).
- Tests: `tests/spirv/` (+`_fail_closed/`), `modules/dasLLAMA/tests/test_vulkan_tier.das`,
  new `test_vulkan_kernels.das`, `test_vulkan_tier_cm2.das` (capability gating).
- dasVulkan (separate repo, own PR): generated pusher, `create_device` portability sweep.

## Risks / open items

- **Pusher fix shape** unknown until we're in the dasVulkan generator — phase 0 sizes it.
- **AOT**: macro-only emitter modules stay un-AOT'd (spirv_emit/msl precedent); the dasLLAMA
  vulkan modules keep their existing `.das_module` registration — verify `test_aot_subset` early,
  not at the end.
- **Master churn**: long-lived branch over 4 big files — use the banked merge-not-rebase recipe +
  the difflib opcode scan on extract-vs-edit conflicts; C++ rebuild after merges when master moves
  externs.
- **JIT**: no `-jit` runs while other daslang processes run (DLL cache collision); never TaskStop a
  GPU-armed run (bus-loss history on this box).
- **Perf honesty**: quiet-box only for judged rows; tune-relaunch first runs are parrots; one row
  at a time vs banked records.

## Verification (end to end)

1. Per-phase gates as listed above (each phase closes green before the next).
2. Kernel level: `test_vulkan_kernels.das` (new) — every ported kernel vs CPU run of the same
   method; spirv-val VALID on every emitted module.
3. Tier level: vulkan_tier / cm2 / gpu_tier / slot_swap / image_vulkan on zen2; bit-parity probe
   master-vs-branch on tinyllama (the extraction-gate pattern).
4. Perf: lcpp_bench canonical rows (Qwen3-4B Q8, tinyllama, 35B MoE @13000) vs banked records —
   no-regression bar, quiet box, Boris flags first.
5. M1: tier suites over ssh with capability declines; per-kernel bisection results recorded either
   as fixes or as the handoff doc.
6. Repo hygiene: MCP lint + format on every touched `.das`; comment-hygiene rules; census +
   fail-closed suites green on all platforms (emitters are pure das, run everywhere).
