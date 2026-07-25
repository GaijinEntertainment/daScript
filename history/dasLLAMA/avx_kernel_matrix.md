# AVX kernel matrix — VNNI / AVX-512 tiers (default OFF, unmeasured)

> **HISTORICAL (superseded 2026-07-04).** The whole matrix — and its host file
> `dasllama_math_x64_avx.das`, plus `harness/avx_matrix_probe.das` / `mx4x64_probe.das` —
> was DELETED after the generated GEMM family (`x64-gen`, per-ISA fallback chains incl.
> VNNI/AVX-512 bias128 tiles) out-performed every hand tier on EPYC and SPR silicon.
> Kept as the design record of the hand-tier exploration.

Session notes + runbook for the AVX kernel matrix (built 2026-07-03, commit `cca44fc30` on
`bbatkin/dasllama-mxfp4-x64`). Companion to `x64_arch.md` (which carries the backend registry
architecture and the shipped-backend story; its backend table includes the matrix rows).

**What this is.** VNNI / AVX-512 twins of the shipped x64 kernels — one backend per
(ISA tier × kernel shape) cell — so a bare-metal box that HAS the tiers can A/B every cell
in-process against its shipped donor. Correctness was proven on real VNNI/AVX-512 silicon
(EPYC 9654, 2026-07-03: probe + gpt-oss token-for-token across 4 backends), so registration
is now unconditional per-tier (`cpu_supports` gated, no master switch); every matrix priority
still sits below every shipped backend — pin-only until the ISA ladder promotes a tier.

## Switches

| Switch | Effect |
|---|---|
| `DASLLAMA_AVX_MATRIX_FORCE=1` | Register tiers the box LACKS. Safe everywhere: kernels run the delegating fallbacks (correct, ymm-speed on an AVX2 box). Probe/validation only |
| `DAS_JIT_X64_FORCE_FEATURES=avxvnni,avx512f,avx512bw,avx512vl,avx512vnni` | JIT-side: flips the emitter gates AND appends `+feature` to the TargetMachine — **emission-only** (compile + disasm the DLL; executing forced sequences on a box without them is an illegal instruction). Folded into the DLL cache key, so forced artifacts never cache-hit normal runs. DLL path only (in-memory MCJIT mode is not covered) |
| `DASLLAMA_PIN_BACKEND=<name>` | The measurement instrument (existing) — pin BEFORE `load_gguf` for repack backends |

## Intrinsics (`modules/dasLLVM/daslib/x64_avx.das`)

All fallback bodies **delegate to the established intrinsic of the same math** — exactness by
construction, and on a box without a tier the delegate is itself recognized, so a matrix kernel
degrades to the proven ymm sign-trick path, never to scalar.

| Intrinsic | Emits (when gated in) | Fallback | Exactness class |
|---|---|---|---|
| `dot32_vnni(acc:int4, w, x)` | abs/psign ymm + ONE `VPDPBUSD` + hi/lo fold (replaces `VPMADDUBSW`+`VPMADDWD`) | `dot32` | integer identity — bit-exact vs dot32 |
| `dot32_acc8_vnni(acc:float?, w, x, s)` | VNNI quads + acc8 float tail | `dot32_acc8` | bit-exact vs dot32_acc8 (strict FP) |
| `dot32_mx4_vnni(acc:int4, lut, wn, x)` | ymm PSHUFB dequant + VNNI dot | `dot32_mx4` | integer identity |
| `dot64_acc16(acc:float?, w, x, s0, s1)` | zmm: `VPABSB` + cmp/neg/select sign + `VPMADDUBSW`/`VPMADDWD` + `VCVTDQ2PS` + FMA into float[16] mem acc (scale vector `[s0×8 \| s1×8]`) | 2× `dot32_acc8` (lanes 0-7 / 8-15) | bit-exact vs fallback per lane (strict FP) |
| `dot64_acc16_vnni(...)` | same, dot = ONE `VPDPBUSD` zmm | `dot64_acc16` | integer identity on quads |
| `mx4_dequant64(lut, wn, dst)` | 2-block nibble select + ONE `VPSHUFB` zmm (LUT `vbroadcasti32x4`) + 64-byte store | 2× `mx4_dequant32` | byte identity |

Preconditions unchanged from dot32: `x ∈ [-127,127]` (the Q8 quantizer guarantees it); `w` may
be any int8 including −128.

**Emitter facts (llvm_jit_intrin.das):**
- LLVM's vpdpbusd intrinsics take **byte vectors** — `<8 x i32> @llvm.x86.avx512.vpdpbusd.256(<8 x i32>, <32 x i8>, <32 x i8>)`
  and the `<16 x i32>(…, <64 x i8>, <64 x i8>)` 512 form — NOT the i32 vectors clang's builtin
  casts suggest. `uw`/`sx` feed directly, no bitcast. (Caught by the LLVMLookupIntrinsicID +
  `LLVMPrintValueToString` signature probe BEFORE writing the emitters — keep that rail.)
- The same `llvm.x86.avx512.vpdpbusd.256` ID selects the VEX AVX-VNNI encoding under
  `+avxvnni` and the EVEX form under `+avx512vnni,+avx512vl` — both observed in one disasm.
- **`VPSIGNB` has no 512-bit form.** The zmm sign trick is `icmp slt` + `sub(0,x)` + `select`,
  which LLVM lowers to `vpmovb2m k1` + `vpsubb zmm{k1}`. w==0 lanes keep `x` instead of zeroing
  — exact anyway, they multiply `|w| = 0`.
- 512-bit intrinsic names verified present: `llvm.x86.avx512.pshuf.b.512`,
  `.pmaddubs.w.512` (`(<64 x i8>, <64 x i8>) → <32 x i16>`), `.pmaddw.d.512`.
- Shared cores: `emit_dot32_quads_v(..., use_vnni)` (branch after uw/sx), `emit_dot32_acc8`,
  `emit_dot32_mx4`, `emit_dot64_acc16(..., use_vnni)` — per-name wrappers keep the lookup-table
  signature uniform.

**JIT gates (llvm_jit_common.das):** `g_target_x64_vnni256` = avx2 gate && (avxvnni ||
(avx512vnni && avx512vl)); `g_target_x64_avx512bw` = avx2 gate && avx512f && avx512bw;
`g_target_x64_avx512vnni` = bw gate && avx512vnni. Each guards its own lookup table in
llvm_jit_intrin; a false gate compiles the daslang body. Cache key (llvm_jit_run.das) folds the
five cpuid bits + the force string; `LLVM_JIT_CODEGEN_VERSION = 0x37`.

**cpu_supports additions (module_builtin_runtime.cpp):** `avx512bw` (leaf-7 EBX bit 30),
`avx512vl` (bit 31), both os_zmm-gated like avx512f. Implication rows added to
`tests/language/cpu_supports.das`.

## Backends (registered in `dasllama_math_x64_avx.das` [init], flag-gated)

| name | prio | shape | dot | A/B donor | slots shared with |
|---|---|---|---|---|---|
| `x64-vnni256` | 7 | row-major dot4x4 | `dot32_vnni` | `x64-avx2` | — (full twin incl. mx4 strips + groupn) |
| `x64-vnni256-acc8` | 6 | row-major acc8 | `dot32_acc8_vnni` | `x64-avx2-acc8` | mx4/groupn slots = the vnni256 row-major ones |
| `x64-vnni256-repack` | 5 (needs_repack) | grp4 interleave + grp4x2 tiles | `dot32_vnni` | `x64-avx2-repack` | mm_mx4/groupn_mx4 = vnni256 row-major (same convention as the donor); same `repack_q8q8_x64` |
| `x64-avx512bw` | 4 | row-major z16 (4× float[16] accs, 8 blocks/iter, odd-block tail via `dot32_acc8`) | `dot64_acc16` | `portable` (tolerance) | mx4 slots = the x64-avx2 natives (no zmm mx4 twin — the mx4 delta belongs to the vnni/z16vnni cells) |
| `x64-avx512vnni` | 3 | row-major z16 + mx4 strips via `mx4_dequant_row_x64z` (`mx4_dequant64`) | `dot64_acc16_vnni` | `x64-avx512bw` (bit-exact) | mm_mx4/groupn_mx4 = vnni256 |

7 twin row dots (`dot_q8q8_x64vnni`, `dot_q8q8_x64acc8vnni`, `dot_q8q8_x64grp4_vnni`,
`dot_q8q8_x64grp4x2_vnni`, `dot_mx4q8_x64vnni`, `dot_q8q8_x64z16`, `dot_q8q8_x64z16vnni`) +
`mx4_dequant_row_x64z` + 25 kernel wrappers — verbatim copies of the shipped kernels with only
the dot swapped (house per-variant pattern; scheduling/oversplit/TB mirror each wrapper's
source so a pin-A/B isolates the dot alone).

## Validation status (3990X, 2026-07-03 — all green)

- `harness/avx_matrix_probe.das`, plain AND `-jit`, with `DASLLAMA_AVX_MATRIX_FORCE=1`
  (registration itself is unconditional since the EPYC validation):
  - intrinsic identity gates exact over 64 blocks incl. extremes (w rolls all 256 bytes incl.
    −128; x block 0 pinned ±127);
  - every VNNI backend **bit-exact vs its donor** across all facades — mm, batch, groupn
    (biased), mm_mx4, groupn_mx4, mx4_batch, mx4_batch_groupn (varying counts + a zero-count
    region), repacked forms for the repack pair;
  - z16 tiers: `x64-avx512vnni` ≡ `x64-avx512bw` bit-exact; `x64-avx512bw` vs portable
    rel < 1e-5 (the acc8/ps float-shape class); plus the bit-exact strip-expansion contract
    (same-backend mx4-batch vs Q8-batch-on-exact-expansion).
  - Shapes: (n=64,d=8,ntok=4,R=5), (n=96 — odd nb, exercises z16/dequant64 odd-block tails —
    d=10 → d%4 row tails, ntok=3, R=4), (n=2880,d=20,ntok=5,R=3 — grp4x2 + TB + threaded).
- Default-off contract: no env → `backends: [portable, x64-avx2, x64-avx2-ps, x64-avx2-acc8,
  x64-avx2-repack]` — matrix invisible. Suite `modules/dasLLAMA/tests` 171/171 under `-jit`.
- Forced-feature disasm (dumpbin on the force-keyed DLL): VEX `vpdpbusd ymm` + EVEX ymm16 form,
  `vpdpbusd zmm`, `vpmaddubsw/vpmaddwd zmm`, `vpshufb zmm` + `vbroadcasti32x4` LUT broadcast,
  `vpmovb2m`+`vpsubb zmm{k1}` sign trick, zmm float tail (`vcvtdq2ps`/`vmulps`/`vaddps`) with
  the scale splat as `vpermi2ps`. Zero "Cannot select".

**Probe gate lesson:** the avx512vnni-vs-avx512bw **mx4_batch** slots are tolerance-class, not
bit-exact — the z-strip kernels dot with the z16 float[16] shape while the donor's strips dot
with int-flush dot4x4; only the dot-identical pairs gate bit-exact. The strong gate for the
z-strip cells is the expansion contract.

## Rented-box runbook

Hardware want-list: `avxvnni` (Alder Lake / Sapphire Rapids+) **or** `avx512vnni`+`avx512vl`
(Ice Lake SP+ — the vnni256 gate reaches it via the vl arm), plus `avx512f/bw` for the z16
tiers. Verify first: `cpu_supports` probe (all five names + `avx2`/`fma`).

1. **Correctness first, real hardware paths:** (NO force) →
   `daslang modules/dasLLAMA/harness/avx_matrix_probe.das` plain AND `-jit`. Also run
   `harness/mx4x64_probe.das -jit` (shipped tiers must still hold on the new silicon).
2. **Per-cell measurement, pin-A/B vs donor** (interleaved best-of; A-B-A brackets for any
   cross-binary ratio; the box-measurement discipline in `x64_arch.md`):
   - 1-core kernel grid: `harness/gemm_1core_probe.das` focus mode
     (`DASLLAMA_FOCUS_BACKEND` ∈ matrix ∪ donors).
   - Batched GEMM: `benchmarks/matmul/bench_gemm_iso.das` with `DASLLAMA_PIN_BACKEND`.
   - In-model: `llama3_perf` / `prefill_perf` / `decode_prof` with `DASLLAMA_PIN_BACKEND`
     (pin before load; the repack pair needs it for `select_matmul_backend_for_load`).
   - Run `tune_kernels` on the box first (`box_profile.json` is per-box; `[tuned]` perms move
     the portable reference).
3. **Cells and hypotheses:** dot32-VNNI saves ~1 fused op/block and moves the dot off the
   FMA-port pair (Zen2's int-flush-rides-free story may invert on Intel port layouts);
   z16 halves blocks/instr but doubles per-instr latency exposure; acc8 was the 1-core winner
   on Zen2 (+50% over ggml) — VNNI may re-balance it; grp4x2-vnni is the prefill GEMM cell;
   `mx4_dequant64` halves dequant ops in the z-strip cell. All priorities are placeholders —
   set by measurement, then promote winners into the shipped set / delete losers.

## Gotchas hit this session

- `require fio` is `error[20605]` — the env-var functions live in **`daslib/fio`**.
- The vpdpbusd operand-type surprise (byte vectors) — see emitter facts above; always probe
  intrinsic signatures, not just IDs.
- dastest CLI: `-jit` is a **daslang** flag (`daslang -jit dastest/dastest.das -- --test …`),
  not a dastest arg.
- The probe's group3/threaded paths need `with_job_que() { setup_dasllama_jobque_() … }`.
- Forced-feature runs may legally vectorize unrelated code with forced ISA — keep forced
  artifacts compile/disasm-only and never reuse them (the cache key enforces the latter).

## PR-time TODO (folds into the arc PR)

- AOT sweep: `KernelBackend` consumers + `x64_avx.das` grew (matrix intrinsics must
  AOT-compile; suite here was interp+JIT only).
- `README` module-tree line if the doc set changed; `x64_arch.md` matrix rows/section already
  landed (this doc is the detail).
- Probe not CI-wired (same status as the other harness probes; `modules/dasLLAMA/tests` covers the
  default-off contract implicitly).
- Standard arc-PR checklist: rebase onto origin/master (make_pr step 0), full preflight token,
  lint exact CI changed-set with absolute paths, dupe check.
