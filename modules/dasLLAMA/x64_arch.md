# dasLLAMA on x64 — architecture handoff

**Audience:** the instance bringing up x64 kernels on an x64 desktop. This doc is the map;
`get_x64_going.md` is the step-by-step runbook; `tune_for_this_box.md` is the per-box tuning
guide (read it *after* the kernels are correct). Line numbers are as of the PR #3340 tip —
symbols are the durable reference, lines will drift.

**The one-sentence brief:** everything platform-specific already sits behind two registries —
a **kernel-backend registry** in `dasllama_math.das` and a **name-keyed JIT-intrinsic table**
in dasLLVM — and the ARM NEON backend is the worked example of both. x64 bring-up = mirror the
NEON files, register at `[init]`, never edit the core.

---

## Ground rules (non-negotiable)

- **Always `-jit`.** dasLLAMA perf work is JIT-tier only; interpreted runs are orders of
  magnitude too slow and loop hints/intrinsics don't exist there. Test command:
  `bin/daslang -jit dastest/dastest.das -- --test tests/dasLLAMA/`. (The MCP `run_test` tool is
  interpreted — don't use it for model runs.)
- **Correctness before speed, token-for-token.** The engine is validated against external
  oracles (llama2.c + llama.cpp `simple_ids`, see `README.md` "How to verify a new model").
  Every new kernel must pass the full suite + the oracles *with the new backend active* before
  any perf claim.
- **Token-exact oracle tests pin the bit-exact path** (classic attention, scalar activation);
  approximate/fast paths get separate tolerance tests. Don't reroute an oracle test through a
  non-bit-exact default — it will pass on the machine it was frozen on and flip elsewhere.
- **AOT is correct-but-untuned by design.** The AOT C++ emitter (`daslib/aot_cpp.das`) drops
  loop hints entirely, and intrinsics compile as their portable fallback bodies. The AOT gate
  (`bin/test_aot -use-aot dastest/dastest.das -- --test tests/dasLLAMA/`) is a compile+link+run
  regression check, never a tuning test.

---

## The engine in one page

```
modules/dasLLAMA/dasllama/
  dasllama_math.das               float/Q8/Q4 primitives + the Q8·Q8 KernelBackend registry (the seam)
  dasllama_math_default.das       "portable" backend — the fallback everywhere, priority 0
  dasllama_math_aarch64_neon.das  "arm64-sdot" + "arm64-laneq" backends — THE FILE TO MIRROR
  dasllama_tune.das               per-box loop-hint tuner macros ([tuned], [dasllama_grid])
  dasllama_par.das                maybe_parallel_for (jobque fork threading)
  dasllama_quant.das              Q8_0 / Q4_0 (de)quantization
  dasllama_common.das             engine core — Config/Model/Session, loader, forward, sample
  dasllama_arch_*.das             per-arch config + [init] registration (llama/qwen2/phi3/gemma2)
  dasllama_transformer.das        thin umbrella — what consumers require
modules/dasLLVM/daslib/
  aarch64_neon.das                sdot4/sdot4_laneq intrinsics — THE OTHER FILE TO MIRROR
  llvm_jit_intrin.das             intrinsic emitters + the per-target lookup tables
  llvm_jit_common.das             target machine, host features, g_target_is_aarch64
  llvm_jit_run.das                JIT DLL cache key, LLVM_JIT_CODEGEN_VERSION
```

**The Q8·Q8 path** (where all the prefill GEMM time goes): Q8_0 = blocks of 32 int8 + one
float scale. The loader (QuantMode.q8) quantizes weights once; each forward quantizes the
activations; the kernel does exact int8·int8 integer MACs per 32-block, then accumulates
`float(block_sum) * wscale * xscale`. Prefill uses the *batch* kernel (token-major Y,
`[ntok × d]`); decode uses the single-token kernel; QKV uses a fused 3-output *group3* kernel.

**Threading:** kernels own their threading via `maybe_parallel_for` (`dasllama_par.das`);
dispatch through the backend function pointers happens at whole-matmul level (the pointer is
never read inside a worker). Worker count (`src/misc/job_que.cpp:42-48`): the
`DAS_JOBQUE_THREADS` env var overrides everything; Apple Silicon gets one worker per P-core;
**everything else gets `min(DAS_MAX_HW_JOBS, cores-1)` and `DAS_MAX_HW_JOBS` defaults to 4**
(`include/daScript/misc/platform.h:585`, a CMake `-DDAS_MAX_HW_JOBS=N` define). An x64 box
must raise it or every "threaded" number is a 4-thread number — see `get_x64_going.md` step 0.

---

## Seam 1 — the KernelBackend registry (`dasllama_math.das`)

`dasllama_math.das` owns the abstraction; ISA modules self-register at `[init]`; **nothing in
the core dispatch changes when a backend is added.**

**The contract** (all in `dasllama_math.das`):

- Function-pointer typedefs (:321-326): `MatmulQ8Q8Fn` (single token), `MatmulQ8Q8BatchFn`
  (`... ntok`), `MatmulQ8Q8Group3Fn` (fused Q/K/V, three outputs, one activation),
  `RepackQ8Q8Fn` (in-place repack of one `[d × n]` Q8 region; `n % 32 == 0`).
- `struct KernelBackend` (:331): `name, mm, batch, group3, repack, needs_repack, priority` —
  copyable, lives in `g_kernel_backends : table<string; KernelBackend>`.
- Active pointers `g_mm_q8q8 / g_mm_q8q8_batch / g_mm_q8q8_group3 / g_repack_q8q8` (:343-346)
  start as **panic stubs** ("no Q8·Q8 kernel backend registered — require
  dasllama/dasllama_math_default") — any q8·q8 consumer must require the default backend
  (`dasllama_common.das:8` does).
- Public wrappers `matmul_q8q8 / matmul_q8q8_batch / matmul_q8q8_group3` (:455-489) `invoke`
  the active pointers; `repack_q8q8_weight` (:446) forwards to the active repack (scale offset
  is `woff/32`).

**Two selection tiers** (this is the part to get right):

1. **`register_kernel_backend(be)`** (:380) — auto-activates the registrant only if it is the
   highest-priority **no-repack** backend so far (and no pin is set). Direct callers — tests,
   benches, row-major weights — always get the best kernel that works on unrepacked data, with
   zero load step.
2. **`select_matmul_backend_for_load()`** (:415) — called by the loader on the Q8 path. Picks
   the best backend **overall**, including `needs_repack` ones, and returns whether the loader
   must repack every weight region. This is the *only* path that activates a repack backend —
   activating one eagerly at registration would run the interleaved kernel on row-major data
   (garbage), which is why registration deliberately skips them.

`pin_kernel_backend(name)` (:397) forces a named backend for A/B (unknown name = warn + keep
current, never wedge); `clear_kernel_backend_pin()`, `active_kernel_backend()`,
`kernel_backend_names()` complete the API. `tests/dasLLAMA/test_kernel_backend.das` covers all
of it.

**Backends today:**

| name | file | priority | needs_repack | registration gate |
|---|---|---|---|---|
| `portable` | `dasllama_math_default.das:170` | 0 | no | none (always) |
| `arm64-sdot` | `dasllama_math_aarch64_neon.das:415` | 10 | no | `get_architecture_name() == "arm64" && jit_enabled()` (:412) |
| `arm64-laneq` | `dasllama_math_aarch64_neon.das:420` | 20 | **yes** | same |

The `jit_enabled()` half of the gate matters: off-JIT the intrinsic functions run their scalar
fallback bodies, which are *slower* than the vectorized portable kernels — so interp/AOT stay
on `portable` on every platform.

**What x64 adds:** one new file `dasllama_math_x64_avx.das`, registering (suggested)
`"x64-vnni"` (row-major, priority 10, `needs_repack = false`) and — only if measurement
justifies it — `"x64-vnni-repack"` (priority 20, `needs_repack = true`). Gate:
`get_architecture_name() == "x86_64" && jit_enabled()` — the builtin returns `"x86_64"` on
x64 (`src/builtin/module_builtin_runtime.cpp:1860`). Wiring checklist (all mechanical):

1. `require dasllama/dasllama_math_x64_avx` in `dasllama_common.das` (next to :8-9; needs
   `// nolint:STYLE030` like its siblings — it's a side-effect require).
2. `.das_module` `register_native_path` list (:7).
3. `modules/dasLLAMA/CMakeLists.txt` — one `ADD_MODULE_DAS` line (:6-7 pattern).
4. `tests/aot/CMakeLists.txt` — `AOT_DASLLAMA_MODULE_FILES` (:106), leaf-order before the
   umbrella.
5. `README.md` module-layout tree.

---

## Seam 2 — JIT intrinsics (dasLLVM)

**The pattern** (`modules/dasLLVM/daslib/aarch64_neon.das` is 57 lines — read it whole):

- The daslang side is a plain function with a **portable scalar fallback body** and *no*
  annotation: `def sdot4(acc : int4; w : int8 const?; x : int8 const?) : int4` (:20-34) —
  each of the 4 dword lanes accumulates a dot of 4 int8s (exactly one `v16i8 × v16i8 → v4i32`
  dot-product instruction). `sdot4_laneq(acc, w, x, lane)` (:43-57) reuses one 4-byte group of
  `x` across all rows — `lane` must be a compile-time constant.
- The emitter recognizes calls **by qualified name** at JIT codegen
  (`llvm_jit_intrin.das:145-148`): `g_aarch64_intrin_lookup <- { "aarch64_neon::sdot4" => @@intrinsic_sdot4, ... }`.
  The table is consulted only when `g_target_is_aarch64` — two gate sites: `has_intrinsic`
  (:150-177) and `lookup_intinsic` (:179-193; yes, the typo is the real symbol name).
- `intrinsic_sdot4` (:1431-1451): cast args to `v16i8*`, unaligned loads,
  `LLVMLookupIntrinsicID("llvm.aarch64.neon.sdot")` + `LLVMGetIntrinsicDeclaration` overloaded
  on `[v4i32, v16i8]`, one `LLVMBuildCall2`. The laneq variant (:1460-1496) additionally
  validates the constant lane and splats it with `LLVMBuildShuffleVector`.
- **Off-target the call is just a normal function** — the fallback body compiles verbatim
  (interpreter, AOT, non-matching JIT). Correct everywhere, fast only where the emitter fires.

**Target features:** `with_default_target_machine` (`llvm_jit_common.das:966`) plumbs
`LLVMGetHostCPUName()` + `LLVMGetHostCPUFeatures()` into the TargetMachine for JIT artifacts.
The aarch64 branch at :999-1004 force-appends `+dotprod` because **macOS returns an empty host
feature string** (features are implied by the CPU name, and an unrecognized-CPU fallback to
"generic" would abort SDOT selection). On Linux/Windows x64 `LLVMGetHostCPUFeatures()` is
populated (cpuid-derived), so VNNI should arrive automatically — only add a forced-feature
branch if instruction selection actually fails.

**Arch detection:** `g_target_is_aarch64` is set in `init_jit`
(`llvm_jit_common.das:484-486`) from `get_architecture_name() == "arm64"` (host path) or the
triple prefix (cross path). The x64 mirror is a `g_target_is_x64` set from `"x86_64"` /
`starts_with(triple, "x86_64")`.

**Cache invalidation:** `LLVM_JIT_CODEGEN_VERSION` (`llvm_jit_run.das:36`, currently `0x2e`)
seeds the JIT DLL cache key — **bump it** when the port changes emitted IR for existing
programs (new intrinsic table, feature-string change, triple change). The key also folds each
function's AOT hash, all loop-hint annotations (`fold_loop_hints`, :99-118 — this is why
re-tuning `box_profile.json` re-keys the DLL automatically), opt/size levels, and
`g_jit_fast_math`.

**What x64 adds:** `modules/dasLLVM/daslib/x64_avx.das` (module `x64_avx`) mirroring
`aarch64_neon.das` — e.g. `vnni_dot4` with the *identical* signature and fallback body as
`sdot4` — plus a `g_x64_intrin_lookup` table and `g_target_is_x64` checks at the two gate
sites, an `intrinsic_vnni_dot4` emitter, and the version bump.

---

## Why x64 is already correct with zero work

Three deliberate layers make the tree x64-safe *today* — verify them before changing anything:

1. **Registration gate** — the NEON `[init]` never fires off-arm, so `portable` stays active
   and every model run on x64 is correct (just unoptimized).
2. **Scalar fallbacks** — even a direct call to `sdot4` off-arm computes the right answer via
   its fallback body; only the JIT emitter lowers it to hardware.
3. **Tests drive public wrappers** — the suite dispatches through the active backend, and
   `test_kernel_backend.das` asserts only that `portable` is registered and *a* row-major
   default is active (never an arch-specific name), so the suite is green on any ISA.

**The known gap:** harnesses that *pin* a specific ISA backend (`harness/tune_tb.das`,
`benchmarks/matmul/bench_matmul_laneq.das`) are JIT-gated but measure the portable path off
their arch — meaningless numbers, not wrong ones. `tune_tb.das:173-175` shows the fix pattern:
skip cleanly when `active_kernel_backend()` isn't the pinned one.

---

## Universal vs per-box — what to keep, what to re-derive

| | What | Where |
|---|---|---|
| **Universal (keep)** | token-blocked batch loop nest (token-block → group → tokens-in-block); the 4×4 repack *concept*; the batch/group3 kernel shapes; `effective_token_block` clamp; all float4 elementwise/attention kernels; loop-hint machinery (target-agnostic, `llvm_jit.das:6993-7009`; only wasm skips hints) | shared files |
| **Per-box (re-derive on x64)** | ISA backend choice; `vectorize_width × unroll_count` per kernel (`box_profile.json`); `TB` (`set_q8_token_block`, default 128); L2 budget (`set_q8_l2_budget`, default 4 MB — **provisional, measured on one M1 Max**); thread count | `tune_for_this_box.md` |

The M1-stamped defaults are *good defaults*, not deletions — `vec8_u2`
(`[vectorize, vectorize_width = 8, unroll_count = 2]`) is the shipped permutation everywhere,
and the grid already carries `vec16`/`vec32` rows specifically because 512-bit AVX-512 may
prefer them (M1's NEON is 128-bit; width-8 = 2 registers is its FMA sweet spot).

---

## The kernel that actually needs porting

Only the **Q8·Q8 integer dot** is ISA-specific. Everything float is portable `float4` SIMD
(maps to SSE on x64 automatically) or loop-hint-driven autovectorization.

**SDOT → VNNI, and the one real x64 gotcha:** ARM `sdot` is natively signed×signed.
The mainstream x64 analog `vpdpbusd` (AVX512-VNNI, and VEX-encoded AVX-VNNI on client cores)
is **unsigned×signed** (u8 × s8). The standard exact fix (ggml uses it):

```
dot(a, b) == dot(|a| as u8, sign(a) applied to b)      // integer identity, exact
```

`|a|` goes on the *unsigned* operand because `|-128| = 128` fits u8 but not s8. Products fit
s16, four-product sums fit s32, `vpdpbusd` accumulates without saturation → the kernel stays
exact. Newest cores also have AVX-VNNI-INT8 (`vpdpbssd`, natively s8×s8 — no trick needed);
check `LLVMGetHostCPUFeatures()` on the actual box before choosing. Verify the exact LLVM
intrinsic name for your LLVM version with an `llc` probe (the method that validated the exp
intrinsic) rather than trusting docs.

**The laneq blueprint** (mirror only if row-major VNNI measures short of llama.cpp — that's
what closed the last 18% on M1): `repack_q8q8_neon` (`dasllama_math_aarch64_neon.das:242-281`)
interleaves 4 weight rows per group so one k-quad of 4 rows is 16 contiguous bytes, with
float4-per-block interleaved scales; `q8q8_batch_kernel_neon_laneq` (:306-353) then runs
`parallel_for over groups → while token-block (TB = effective_token_block(g_q8_token_block, n),
:315) → group loop → 4-token tiles` (`dot_q8q8_laneq4x4`, 4 independent accumulators,
`[unroll_count = 2]`, weight loads CSE'd across tokens). The TB slice of activations staying
L2-resident across a thread's groups is the whole point — that structure is universal; only
the numbers are M1's.

**Not a target:** AMX tiles (the true `smmla` analog, Sapphire Rapids+) — Tier-3/deferred.
The ARM SMMLA experiment is **not in the tree**; it's preserved on branch
`bbatkin/dasllama-smmla-i8mm` for future i8mm hardware.

**A queued decision that unblocks on x64:** the `exp(floatN)` vector intrinsic
(`build_vector_expf`, `llvm_jit_intrin.das:885-919`) emits generic vector IR proven via `llc`
to vectorize on SSE2/AVX2/AVX-512 with zero scalarization, but is **gated to aarch64**
(:928) pending real-hardware validation. The recorded plan: validate on x64 → widen the gate →
add the fast-path branch (recover ~7× from ~2.9×) → retire the local `exp4` fast-path helper
in `dasllama_math.das`. Until then dasLLAMA's `exp4`/`swiglu4`/`geglu4`/`softcap4`/`hmax` are
plain portable float4 and work on x64 as-is.

---

## What to measure against

llama.cpp CPU-only on the same box is the yardstick (`llama-bench -p 512,1024,2048 -n 0 -t N`,
plus the token-for-token oracles from `README.md`). Two fairness notes from the M1 campaign:
ggml is not `-ffast-math` but hand-codes equivalent FP laxity (FMA intrinsics, ~2ulp exp poly,
reassociated reductions) — our `options _jit_fast_math = true` (already on the runners and
end-to-end benches, **never on tests**) is the apples-to-apples configuration; and llama.cpp's
x64 CPU path uses repacked weight layouts (`cpu_repack_buffer_type`), so compare row-major
VNNI against it knowing that. Measurement methodology — interleaved A/B, correctness gates,
the ggml per-op-timing patch — lives in `tune_for_this_box.md`.
