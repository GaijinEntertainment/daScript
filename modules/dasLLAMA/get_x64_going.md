# Getting dasLLAMA going on x64 — bring-up runbook

Step-by-step for the x64 desktop. Read `x64_arch.md` first (the map — registry contract,
intrinsic pattern, the u8×s8 gotcha); this doc is the order of operations with acceptance
gates. Do the steps in order — each one's gate is the next one's foundation. Perf tuning comes
last and has its own doc (`tune_for_this_box.md`).

Path convention below is the single-config layout (`build/daslang`); on Windows MSVC it's
`bin/Release/daslang.exe` — substitute throughout.

---

## Step 0 — toolchain sanity (portable path, zero code changes)

1. **Build with a real thread pool.** Non-Apple builds cap jobque workers at
   `min(DAS_MAX_HW_JOBS, cores-1)` and `DAS_MAX_HW_JOBS` **defaults to 4**
   (`include/daScript/misc/platform.h:585`, `src/misc/job_que.cpp:42-48`). On a many-core x64
   box configure with `-DDAS_MAX_HW_JOBS=<physical cores>` (or export
   `DAS_JOBQUE_THREADS=<n>` per run — it overrides everything). Forgetting this makes every
   threaded number in later steps a 4-thread number.
2. Build (Release, dasLLVM enabled so `-jit` exists). Builds are slow — don't treat silence
   as failure.
3. **JIT smoke:** run any small script with `-jit`; then the dasLLAMA suite on the portable
   backend:
   ```
   bin/daslang -jit dastest/dastest.das -- --test tests/dasLLAMA/
   ```
   Model-gated tests self-skip without local GGUFs. **Gate: all green.** This proves the
   three x64-safety layers (portable backend active, scalar fallbacks, wrapper-driven tests)
   before you touch anything.
4. Optional but recommended: bootstrap the MCP tools for the session
   (`daslang utils/mcp/setup.das -- --root <checkout>` from any built daslang binary).

## Step 1 — models + oracles on this box

1. Fetch models (gitignored; the Mac keeps them in `~/Work/llama.cpp/models/` — any path
   works, demos take explicit paths):
   `hf download <repo> <file> --local-dir <models-dir>`. Minimum useful set:
   `Llama-3.2-1B-Instruct-Q8_0.gguf` (the perf workhorse) + `stories15M.bin` from llama2.c
   (the tiny correctness baseline).
2. Build llama.cpp **CPU-only** on this box (no CUDA/Vulkan backends — they'd grab the graph)
   plus the instrumented oracle `harness/oracle/simple_ids.cpp` and `llama-bench`. The oracle
   method is documented in `README.md` ("How to verify a new model"); `harness/parity.sh`
   wires both sides.
3. **Gate:** `bin/daslang -jit examples/dasLLAMA/run.das -- <model>` produces coherent text,
   and a parity run matches the oracle token-for-token on the portable backend. Note the
   `DEFAULT_MODEL` constants in benches/tests are Mac paths — pass yours explicitly.

## Step 2 — record the portable baseline

Before any x64 code exists, record (a) isolated GEMM, (b) end-to-end prefill, (c) llama.cpp
on the same box:

```
bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/matmul/bench_gemm_iso.das
bin/daslang -jit modules/dasLLAMA/benchmarks/prefill_perf.das -- <model.gguf>
llama-bench -m <model> -p 512,1024,2048 -n 0 -t <threads>    # CPU-only build
```

These numbers are the denominator for every later claim. Bench discipline (interleaved A/B,
remote-session CPU theft, thermals) is in `tune_for_this_box.md` — skim its "measurement
discipline" section now, before you record anything.

## Step 3 — discover the ISA

Ask LLVM, not the spec sheet — a probe script (verified to run as-is):

```das
options gen2
require llvm/daslib/llvm_boost

[export]
def main {
    let cpu = LLVMGetHostCPUName()
    let features = LLVMGetHostCPUFeatures()
    print("cpu: {cpu}\nfeatures: {features}\n")
    LLVMDisposeMessage(cpu)
    LLVMDisposeMessage(features)
}
```

(On macOS `features` prints empty — implied by the CPU name; on x64 it's the full cpuid
list.)

Look for `+avx2`, `+avx512vnni` (+`avx512vl`), `+avxvnni` (VEX 256-bit VNNI on client cores),
`+avxvnniint8` (`vpdpbssd`, natively s8×s8 — no sign trick needed). Decide the target
instruction now; everything in step 4 keys off it. If only AVX2 exists (no VNNI anywhere),
the fallback plan is `pmaddubsw`+`pmaddwd` (ggml's pre-VNNI path) — same sign-trick, two
instructions per dot.

## Step 4 — the JIT intrinsic (dasLLVM side)

Mirror `aarch64_neon.das` exactly (the full pattern with line refs is in `x64_arch.md` seam 2):

1. `modules/dasLLVM/daslib/x64_avx.das` — module `x64_avx`; `vnni_dot4(acc : int4;
   w : int8 const?; x : int8 const?) : int4` with the *same portable fallback body* as
   `sdot4`, plus a laneq analog if/when the repack kernel needs it.
2. `g_target_is_x64` in `llvm_jit_common.das` — mirror the `g_target_is_aarch64` assignment
   (:484-486): `get_architecture_name() == "x86_64"` on the host path, triple prefix on the
   cross path.
3. `g_x64_intrin_lookup` table + gate checks in `llvm_jit_intrin.das` `has_intrinsic`
   (:150-177) and `lookup_intinsic` (:179-193), keyed `"x64_avx::vnni_dot4"`.
4. The emitter: loads → (sign-trick unless `vpdpbssd`) → the VNNI intrinsic call. Verify the
   exact intrinsic name against your LLVM with an `llc` probe first.
5. Host features: on x64 `LLVMGetHostCPUFeatures()` is populated, so the forced-feature branch
   (`llvm_jit_common.das:999-1004`, the macOS `+dotprod` workaround) should be unnecessary —
   add an x64 branch only if instruction selection fails.
6. **Bump `LLVM_JIT_CODEGEN_VERSION`** (`llvm_jit_run.das:36`).

**Gate:** a probe script that fills random int8 buffers and asserts
`vnni_dot4(...) == <inline scalar reference>` across many sizes/values — under `-jit` (emitter
path) *and* interpreted (fallback path). Integer dot is exact: use `==`, not tolerance.
Include the s8 extremes (`-128`, `127`) — that's where a botched sign trick breaks. Then dump
the IR (`bin/daslang -jit probe.das -- --jit-dump`) and confirm the VNNI instruction is
actually emitted, not the fallback loop.

## Step 5 — the kernel backend (dasLLAMA side), row-major first

1. `modules/dasLLAMA/dasllama/dasllama_math_x64_avx.das`: port the three NEON *row-major*
   kernels (`q8q8_kernel_neon_sdot4x4` mm/batch/group3 family) swapping `sdot4` →
   `vnni_dot4`. Keep the per-block float flush order identical to portable so the A/B diff
   stays tiny (integer part is exact; only float accumulation order can differ).
2. Register at `[init]`:
   ```das
   if (get_architecture_name() == "x86_64" && jit_enabled()) {
       register_kernel_backend(KernelBackend(name = "x64-vnni", mm = @@..., batch = @@...,
           group3 = @@..., repack = @@repack_q8q8_identity, needs_repack = false, priority = 10))
   }
   ```
3. Wire the file in (5 spots, listed in `x64_arch.md` seam 1): the side-effect `require` in
   `dasllama_common.das:8-9`, `.das_module:7`, `modules/dasLLAMA/CMakeLists.txt`,
   `tests/aot/CMakeLists.txt` `AOT_DASLLAMA_MODULE_FILES`, `README.md` tree.

**Gates, in order:**
- Full suite `-jit` green with `x64-vnni` auto-active (it out-ranks portable at priority 10;
  `test_kernel_backend.das` asserts portable-registered + *a* row-major default — it never
  pins arch names, so it stays green).
- A/B max-abs-diff vs portable on real shapes (`bench_matmul_laneq.das:53-57` uses `1e-3`;
  expect far tighter if the flush order matches).
- Token-for-token oracle (step 1's parity run) with the backend active.
- AOT regression: `bin/test_aot -use-aot dastest/dastest.das -- --test tests/dasLLAMA/`
  (compiles the fallback bodies — correctness only, untuned by design).

## Step 6 — repack backend (only if measured short)

Re-run step 2's benches with `x64-vnni` active. If isolated GEMM still trails llama.cpp
meaningfully (their x64 CPU path *does* repack — `cpu_repack_buffer_type`), mirror the laneq
tier: a 4×4-interleave `repack_q8q8_x64` + a token-blocked batch kernel reading
`effective_token_block(g_q8_token_block, n)` (the shared L2 cliff-guard —
`dasllama_math.das:301`), registered as `"x64-vnni-repack"`, priority 20,
`needs_repack = true`. The loader handshake (`select_matmul_backend_for_load`) then picks it
up with **zero loader edits**. The M1 history says the big win here was the token-blocking
(L2 residency, +45% on the DRAM-bound shapes), not the interleave per se — measure before
building. Gate everything the same way as step 5.

## Step 7 — perf validation, then tuning

- Interleaved A/B between backends via `pin_kernel_backend("portable" | "x64-vnni" | ...)`
  in one process — never sequential runs (see the tune doc for why).
- Head-to-head vs `llama-bench` at `-p 512,1024,2048`. Use `options _jit_fast_math = true`
  for the end-to-end comparison (it's already on the runners/benches; ggml hand-codes the
  equivalent FP laxity — this is the fair comparison). **Never on tests.**
- Then work through `tune_for_this_box.md` — kernel loop-hint sweep, TB/L2 sweep, thread
  sweep — and only bake in repeatable, correctness-gated wins.

---

## Gotcha ledger (things that already bit us once)

- **JIT DLL cache staleness:** the cache key folds the codegen version, function hashes, loop
  hints, and fast-math — your version bump (step 4.6) invalidates everything correctly, and
  `box_profile.json` changes re-key automatically. Blunt fallback if something ever looks
  stale: delete `.jitted_scripts/`.
- **Never benchmark interpreted.** If a number looks 100× off, check you passed `-jit`.
  Benches self-skip off-JIT and say so; heed the message.
- **ISA-pinned harnesses skip off their arch** (`tune_tb.das:173-175` pattern). When you add
  x64-pinned benches, gate them the same way so the ARM box skips them cleanly, and vice versa.
- **Remote-session CPU theft:** a streaming/remote-desktop agent (Parsec etc.) on the box
  silently wrecks absolute throughput numbers. Interleaved A/B in one process is the defense;
  absolute numbers need a quiet machine.
- **`assert` messages must be string constants** — `if (!cond) panic("...{x}...")` for
  interpolated diagnostics (daslang `error[30117]`).
- **Empty-array forwarding:** the `<kernel>(unsafe(addr(x[0])), size)` array-overload pattern
  index-faults on empty arrays — any new overload needs a `size <= 0` early-return (this was
  a real Copilot-caught regression).
- **Don't hand-strip `const` to write through a pointer** — declare the parameter `var T?`.
  The optimizer trusts the type; a laundered write gets elided silently (the root of two past
  miscompile hunts). See CLAUDE.md "The const model".
