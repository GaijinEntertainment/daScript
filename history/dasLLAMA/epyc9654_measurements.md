# EPYC 9654 — dasLLAMA x64 AVX-matrix measurement log

Rented-box campaign for the AVX kernel matrix (VNNI / AVX-512 tiers) + the #3370 dispatch-lanes
fix. **This file is the durable record on the Mac** — the rented box is ephemeral. Companion to
`avx_kernel_matrix.md` (design), `x64_arch.md` (backend registry), `tune_for_this_box.md` (method).

Branch: `bbatkin/dasllama-x64-dispatch-lanes` @ `36d1b5f07`. Started 2026-07-03.

---

## Box

| | |
|---|---|
| Provider | Cherry Servers, bare-metal hourly ($2.00/hr) |
| CPU | AMD EPYC 9654 (Zen 4 Genoa), single socket |
| Cores / threads | 96 physical / 192 logical (2-way SMT) |
| RAM | 755 GB |
| OS | Ubuntu 24.04.4 LTS, kernel 6.8 |
| Storage | 2×3.5 TB NVMe; root was a tiny 10 GB RAID1, **claimed the free 3.83 TB into `/data`** |
| Access | SSH host alias `dasbox` (88.216.197.35), passwordless; `dasbox` SSH MCP installed |

### ISA — full matrix coverage (via EVEX)
Present: `avx2 fma avx512f avx512bw avx512vl avx512dq avx512_vnni`.
**Absent**: `avx_vnni` (VEX-256 encoding — Genoa only advertises EVEX) and `avx512bf16`.
Consequence: every matrix gate is satisfied (`x64-vnni256` via the `avx512vnni && avx512vl`
arm, i.e. EVEX `VPDPBUSD`; `avx512bw`; `avx512vnni`). We **cannot** validate the *VEX*
`vpdpbusd ymm` encoding here (needs an Intel Alder-Lake-class client chip). `bf16` absent →
irrelevant to the matrix (only the deferred fp16-scale idea).

### daslang build
0.6.3, Release + dasLLVM, LLVM **22.1.5** prebuilt (auto-downloaded, `linux_64`). Clean build
**2m10s** on 96 cores. Configure needed `-DDAS_LLVM_DISABLED=OFF` (JIT is OFF by default!) and
`-DDAS_GLFW_DISABLED=ON -DDAS_STDDLG_DISABLED=ON -DDAS_HV_DISABLED=ON` (headless, no GL/X11).
Binary: `/data/daScript/bin/daslang`. `-jit` verified.

### Threads — what daslang sees
`nproc = 192`, physical = 96. daslang: `get_total_hw_jobs()` = `min(1024, hardware_concurrency−1)`
and `hardware_concurrency()` returns **logical** → **191 workers, + main-steal = 192 compute
threads (every SMT lane)** by default. `DAS_JOBQUE_THREADS=N` overrides. The `−1` was tuned for
Apple P-cores; on x86 SMT it just yields logical−1. `get_dispatch_lanes()` = workers + caller lane
= 192.

---

## Correctness — ALL GREEN (this box is the first silicon the VNNI/AVX-512 emitters ever ran on)

On the 3990X (Zen2) none of the VNNI/AVX-512 tiers ever executed a real instruction — only their
delegating AVX2 fallbacks under `_FORCE`. Only `acc8` (plain AVX2) had run. **First real execution
of `VPDPBUSD` / AVX-512 kernels = here, and they're correct.**

| Check | Result |
|---|---|
| `modules/dasLLAMA/tests` suite `-jit` (default backend) | **171/171**, 0 skipped |
| `avx_matrix_probe` interp (fallback bodies) | ALL OK |
| `avx_matrix_probe` **-jit** (real VNNI/AVX-512), `AVX_MATRIX=1` no force | ALL OK — every tier bit-exact vs donor over n=64/96/2880 (incl. threaded); intrinsic identity 64/64 blocks; z16 vs portable rel<1e-5 |
| `mx4x64_probe` -jit (shipped mxfp4 tiers) | ALL OK |
| `test_mxfp4`/`test_groupn` internal backend sweep w/ `AVX_MATRIX=1` | matrix backends vs scalar leaf ✓ (this is what actually covers them end-to-end in the suite) |
| **gpt-oss-20B full MoE forward, token-for-token vs golden**, pinned per backend | portable ✓ · x64-avx2 ✓ · **x64-vnni256 ✓** · **x64-avx512vnni ✓** — all MATCH (40 tok, short + window-engaged) |

**Gotcha logged:** `DASLLAMA_PIN_BACKEND` is read only by specific benches/probes
(`bench_gemm_iso`, `llama3_perf`, `gptoss_parity_probe`), **NOT by `dastest`**. Pinning the suite
via the env is a no-op — it silently runs the default backend. Matrix coverage in the suite comes
from `AVX_MATRIX=1` enabling the tests' *internal* `for name in kernel_backend_names()` sweeps.

---

## llama.cpp baseline (`llama-bench`, CPU build `-march=native`, **-t 96**)

The yardstick. Note the thread-count fairness gap: llama-bench here is 96 (physical); dasLLAMA
defaults to 192 (all SMT). Comparisons must pin both sides to the same count.

| model | size | pp512 (t/s) | tg128 (t/s) |
|---|---|--:|--:|
| Llama-3.1-8B Q8_0 | 7.95 GiB | 426.6 | 25.4 |
| Llama-3.2-1B Q8_0 | 1.22 GiB | 2605.0 | 139.1 |
| gpt-oss-20B MXFP4 MoE | 11.27 GiB | 509.9 | 54.5 |

---

## Thread-count sweep — isolated GEMM (`bench_gemm_iso`, `matmul_q8q8_batch`, ntok=512)

Real Llama prefill shapes, **GMAC/s total** (single run each; internal warmup+reps).

### Repack tier — `x64-avx2-repack` (the shipped prefill GEMM)
| shape | T=48 | T=96 | T=144 | T=192 (default) | best |
|---|--:|--:|--:|--:|:--|
| kv 2048×512 | **693** | 460 | 357 | 277 | 48 |
| qo 2048×2048 | 846 | **965** | 921 | 743 | 96 |
| w13 2048×5632 | 1000 | 1042 | **1255** | 943 | 144 |
| w2 5632×2048 | 888 | **1211** | 1199 | 977 | 96 |
| cls 2048×32000 | 1065 | 1379 | 1553 | **1620** | 192 |

### Row-major tier — `x64-avx2` (per-core at T=96 in parens)
| shape | T=48 | T=96 | T=144 | T=192 |
|---|--:|--:|--:|--:|
| kv 2048×512 | 478 | 308 (3.21/c) | 321 | 222 |
| qo 2048×2048 | — | 671 (6.99/c) | — | — |
| w13 2048×5632 | 794 | 781 (8.13/c) | 1050 | 743 |
| w2 5632×2048 | — | 783 (8.15/c) | — | — |
| cls 2048×32000 | 813 | 836 (8.71/c) | 1215 | 963 |

Repack tier ≫ row-major (e.g. cls@144: 1553 vs 1215; w2@96: 1211 vs 783) — confirms the tiled
repack kernel is the one to A/B the VNNI tiers against.

### Finding: the sweet spot is **shape-dependent**, and 192 (daslang default) is the *worst*
choice for 4 of 5 shapes.
- Small `kv` (d=512) peaks at 48 and **craters 2.5× at 192** (693→277).
- Mid `qo`/`w2` peak at 96; big dense `w13` at 144; only the giant `cls` (d=32000) wants 192.

---

## Root cause — no minimum chunk size (the real lever)

The quantized matmul dispatch (`dasllama_math.das`):
```
maybe_parallel_for(n*d >= g_matmul_par_threshold, 0, int(d), <num_chunks>) $(rb, re) { ... }
```
- There is a par-**threshold** (thread or not: `n*d >= g_matmul_par_threshold`) but **no
  min-chunk-*size***.
- `num_chunks` = `get_dispatch_lanes()` (≈192) for GEMV, `chunks_per_job × get_dispatch_lanes()`
  for quantized/batch: **q8 batch = 4 × 192 = 768 chunks over `d` rows**, fixed, independent of `d`.
- The ×4 oversplit (`g_q8_batch_chunks_per_job`, math.das:154-158) was **deliberate** (idle
  workers steal a straggler's remaining chunks) and tuned on 3990X @32 workers → 128 chunks. At
  192 lanes it becomes 768.

Effect: for `kv` d=512, 768 chunks ⇒ **more chunks than rows** (~0.7 row/chunk after clamp = 1
row/chunk) → the GEMM is shattered into ~512 one-row work items; dispatch overhead + SMT port
contention dominate. For `cls` d=32000, 768 chunks ⇒ ~42 rows/chunk → healthy, scales to 192.

There IS a min-size clamp already — but only for **tokens** (`effective_token_block`, L2-budget
arithmetic in the repack kernel), **not for the row-chunk count**.

## How ggml does it (teammate research, `~/Work/llama.cpp/ggml/src/ggml-cpu/ggml-cpu.c`)

ggml's `mul_mat` chunker (`:1388-1442`, MoE mirror `:1652-1666`) is a **two-stage clamp**:
1. **Min grain floor** (`:1388-1393`): `chunk_size = 16` rows, bumped to **64** for a GEMV
   (`nr0==1 || nr1==1`). Grid = `ceil(nr0/16) × ceil(nr1/16)`. No chunk is ever finer than 16 rows.
2. **Anti-fragmentation fallback** (`:1404-1408`): if that grid yields `< nth*4` chunks (or NUMA),
   scrap it and do a plain **1D split into exactly `nth` chunks** along the longer axis.
3. **Emergent thread cap** (`:1415-1442`): all `nth` threads are dispatched (`n_tasks=n_threads`,
   `:2302`), but the loop `while (current_chunk < nchunk0*nchunk1)` + `if (nth >= chunks) break`
   means **only `min(nth, chunkCount)` threads ever touch data** — surplus fall through the barrier
   idle. Remaining chunks work-steal via one cache-aligned atomic `current_chunk` (`atomic_fetch_add`).
4. **Default threads = physical *math* cores, SMT + E-cores excluded** (`common/common.cpp:175-207`,
   `common_cpu_get_num_math` → `common_cpu_get_num_physical_cores`). Comments: *"efficiency cores
   harm lockstep threading"*, *"hyperthreading isn't useful for linear algebra."* So ggml sidesteps
   SMT contention *at the thread-count level*, before chunking runs. (This is why `llama-bench` here
   defaulted to ~96, not 192.)

### dasLLAMA vs ggml
| aspect | ggml | dasLLAMA (current) |
|---|---|---|
| min chunk grain | **16 rows** (64 for GEMV) | **none** — chunk = `d / (cpj×lanes)`, can be < 1 row |
| chunk count for small op | capped: `min(nth, ⌈d/16⌉)`, fallback ≤ `nth` | fixed `cpj × lanes` (q8 batch = 4×192 = **768**) |
| the `×4` factor | a **guard ceiling** (`< nth*4` ⇒ simplify) | a **multiplier floor** (`g_q8_batch_chunks_per_job`, always ×4) |
| surplus threads on small op | idle (fall through barrier) | each gets a sub-1-row chunk → dispatch + SMT thrash |
| default threads | physical cores, **no SMT** | all logical (**192, SMT included**) |

Two independent divergences: **(a) no min grain**, and **(b) SMT-inclusive default**. Either alone
hurts; together they produce the `kv` 2.5× cratering.

### Proposed fix — mirror ggml's grain floor (addresses thread count *via* min-chunk-size)
Per Boris: fix the **min-chunk-size**, don't change the jobque thread default. With a grain floor,
daslang's 192 default becomes *harmless* — exactly ggml's emergent capping (surplus threads idle
on small ops). Concretely, at each dispatch site clamp the requested chunk count:
```
let want   = chunks_per_job * get_dispatch_lanes()
let capped = min(want, max(1, int(d) / MIN_GRAIN))   // MIN_GRAIN ≈ 16 rows (ggml), tune here
maybe_parallel_for(cond, 0, int(d), capped) $(rb, re) { ... }
```
- `MIN_GRAIN = 16` mirrors ggml (start there; sweep 8/16/32/64 on this box). GEMV/decode may want
  the 64 bump.
- Keeps the ×4 straggler-steal oversplit for *large* `d`, kills it for *small* `d`.
- Row-count grain is a first cut; a work-based grain (`d*n` MACs, or `d*n*ntok` for batch) is more
  principled since a row's cost scales with `n`/`ntok` — decide after the row-grain sweep.
- Whether `team_parallel_for`/`parallel_for` already clamp `num_jobs` to the range (no empty
  chunks) is a C++ detail to confirm; irrelevant to the fix (we clamp at the call site regardless).

---

## Min-grain clamp — IMPLEMENTED + measured (2026-07-03)

**Change:** `matmul_chunks(rows, want)` in `dasllama_math.das` (next to `get_dispatch_lanes`):
`clamp(rows / g_matmul_min_chunk_rows, 1, want)`, default grain **16** (ggml parity), 0 disables,
knob `set_matmul_min_chunk_rows`. Applied at **all 72 matmul dispatch sites** across
`dasllama_math{,_default,_x64_avx,_aarch64_neon}.das` — maybe_parallel_for row/group/region
splits (group ranges pass `groups*4` as rows), the 10 raw group3 `parallel_for` sites
(`count_parallel_dispatch` kept truthful), and the precomputed `nchunks`/`njobs` strip kernels
(whose old `clamp(ng/4, lanes, lanes*cpj)` used **lanes as the lower bound** — the exact bug:
the 16-row grain the author designed was defeated whenever `ng/4 < lanes`).
Env knob `DASLLAMA_MIN_CHUNK_ROWS` wired into `bench_gemm_iso` + `prefill_perf`.
Post-change correctness: `avx_matrix_probe` ALL OK, suite 171/171. (Note: editing the core math
.das files re-keys EVERY dasLLAMA JIT DLL → first run after = full recompile, ~270 s for the
suite. Expected, not a hang.)

### Grain sweep, repack tier @ T=192 (the clean control — GMAC/s total)
| shape | g=0 (old) | g=8 | g=16 | g=32 | g=64 |
|---|--:|--:|--:|--:|--:|
| kv 2048×512 | 202 | **318** | 258 | 194 | 176 |
| qo 2048×2048 | 299 | **707** | 618 | 458 | 311 |
| w13 2048×5632 | 571 | 984 | **1007** | 796 | 668 |
| w2 5632×2048 | 627 | **962** | 914 | 647 | 370 |
| cls 2048×32000 | 1262 | **1614** | 1533 | 1593 | 1241 |

**At the 192-lane default the grain floor is a 1.3–2.4× win on every shape** (qo 299→707).
cls@192 g=8 (1614) matches the old global best (1620). **g=8 > g=16 on this box** everywhere
except w13 (~2% ≈ noise). Grain is box-dependent: the 3990X kernel-comment measurements favored
16 (4 groups/chunk); EPYC favors 8 → `box_profile` runtime knob, default stays 16.

### Thread sweep with grain=16 (repack tier)
| shape | T=48 | T=96 | T=144 | T=192 |
|---|--:|--:|--:|--:|
| kv | **479** | 319 | 238 | 258 |
| qo | 850 | **984** | 749 | 618 |
| w13 | 1004 | 1129 | **1145** | 1007 |
| w2 | 892 | **1231** | 984 | 914 |
| cls | 1068 | 1390 | **1539** | 1533 |

### Measurement-validity caveat (cross-run absolutes are polluted)
The g=0 control (202/299/571/627/1262) sits far BELOW the pre-change run of the *identical*
config (277/743/943/977/1620): the grain sweep ran 5th–8th after four back-to-back bench runs
(hot box), the pre-change sweep ran cold — the sequential-run thermal confound the tuning doc
warns about, reproduced live. What survives it: **g=8 ran AFTER g=0 on an even hotter box and
still won 1.6–2.4×** — the grain win is real and under-, not over-stated. What does NOT survive:
cross-session absolute claims (e.g. "old kv T=48 = 693 vs new 479" — unsettled). A proper
interleaved same-process grain A/B is the follow-up if we need exact ratios.

### Caveats the sweep exposed (honest)
1. **kv (small-d) likely still short of its low-thread optimum**: row-only splitting can't feed
   many lanes for small d at any grain — **ggml chunks the token dimension too** (2D grid:
   kv-like shapes get 32×32 = 1024 tiles of 16×16). 2D chunking = the structural fix →
   **perf ledger, not chased now**.
2. Optimal grain differs per shape and box → runtime knob + per-box tune, not a constant.
3. Bench runs are sequential per config (no in-process grain interleave yet) — treat all
   cross-config ratios ≳noise-floor accordingly; the 1.3–2.4× T=192 wins are far above it.

### In-model prefill A/B (Llama-1B, T=192, sequential runs) — the deciding numbers
prefill tok/s (batched `forward_prefill`) and per-token tok/s (decode-style):
| N | g=0 | g=16 | g=8 |
|---|--:|--:|--:|
| 64 | 134.0 | **194.6 (+45%)** | 161.3 |
| 256 | 297.7 | **326.9 (+10%)** | 320.8 |
| 512 | 493.8 | 495.4 | **501.2** (~flat) |
| per-token (decode-style) | 7.12–7.65 | **7.40–8.05 (+4-5%)** | ≈ g=0 |

**In-model, g=16 wins** (biggest at small batch, where dispatch dominates) — opposite of the
iso-GEMM bench's g=8 preference (iso = batch kernel only; in-model mixes GEMV+groupn+batch).
In-model is the truth → **default grain stays 16**.

## Physical-core jobque cap (Boris's design, implemented 2026-07-03)

dasLLAMA-scoped limiter, core daslang default untouched: `JobQue::set_default_threads_cap(n)`
(C++ — min()'d with the stock rule at queue creation, so it can only lower; env
`DAS_JOBQUE_THREADS` still beats everything) + `get_total_hw_cores()` physical-core builtin
(Linux `smt/active`, Windows `RelationProcessorCore` records, mac `hw.physicalcpu`) +
`[init] dasllama_jobque_threads_cap` in `dasllama_math.das` setting cap = phys−1 → workers
phys−1 + computing main = **physical cores, ggml's exact `-t` default**. min() semantics keep
the Apple P−1 rule (cap 9 doesn't raise M1 Max's 7). Files: `job_que.{h,cpp}`, `sysos.cpp`,
`module_builtin_jobque.cpp`, `aot_builtin_jobque.h`, `dasllama_math.das`.
PR-time TODO: handmade RST stubs for the two new builtins (doc gate).

## Planned centerpiece — the ISA-yield ladder (Boris, 2026-07-03)

Once grain + threads + tune are settled, pin down the ladder and measure the MARGINAL yield of
each extension on identical shapes (the matrix cells are same-kernel-one-ISA-step twins by
construction, bit-exact-verified):
`portable → x64-avx2 (pmaddubsw+pmaddwd) → x64-vnni256 (vpdpbusd ymm) → x64-avx512bw (zmm pair)
→ x64-avx512vnni (vpdpbusd zmm)`. In-process interleaved via `DASLLAMA_PIN_BACKEND`; promote/
delete falls out of the same data. Second axis: whole-program compiler-target downgrade via
`DAS_JIT_X64_FORCE_FEATURES` minus-tokens (verify the parse accepts `-feat`) → autovectorizer
width yield on the float kernels. Writeup caveat: Zen 4 double-pumps 512-bit, so width rungs
underestimate native-512 silicon (SPR / Zen 5); VNNI rungs are clean.

## The dispatch hunt (Boris's "something is very wrong" — confirmed and fixed, 2026-07-03)

**Floor check first** (new standing rule — see memory `sanity-floor-before-sweeps`): SmolLM2-135M
@ T=1 = 116 t/s decode / 218 prefill — engine + JIT fundamentally sane on this box.

**The signature** (SmolLM ladder, fifo dispatch): decode FLAT 116-128 at any T (below par
threshold → inline); prefill COLLAPSES 218→190→100→49 t/s as T grows 1→96 (above threshold →
dispatches). ttft 18→80ms. → the fork/join dispatch itself is the cost.

**Quantified** (`benchmarks/core/jobque/dispatch.das` + new team arm): fifo fork/join ≈ **10 µs
PER JOB** (alloc+fifo+wake+waitgroup) → 96-chunk dispatch ≈ 1 ms; × ~160 dispatches/decoded
token ≈ the entire budget → the observed Llama-1B 7.6 t/s vs llama.cpp 139 (18×). On M1 (9
lanes) this was ~90 µs/dispatch — invisible. ggml pays ~µs spin barriers instead (persistent
graph lockstep, no per-op posting). Team dispatch (#3368) measures 0.5-5.7 ns/op in the empty
probe (flattered — caller self-serves; real-work cost higher but orders cheaper than fifo).

**Team mode as runner default (box experiment — sed in setup_dasllama_jobque_):**
| metric | fifo | team |
|---|--:|--:|
| SmolLM prefill @96w | 49 | **250** (ttft 80→16ms) |
| Llama-1B decode @95w default | 7.6 | **43** (5.7×) |
| Llama-1B decode knee | — | **97 @ T=24** (70% of lcpp 139) |
| Llama-1B prefill-512 @default | 495 | **969** (37% of lcpp 2605; N=256: 1120) |

**Decode residual:** ~~lcpp holds 139 via GEMV grain 64 ⇒ ~32 effective threads, emergent~~ —
WRONG inference, corrected session 2 against the source: a decode GEMV lands in ggml's
anti-fragmentation FALLBACK (grid < nth*4 ⇒ exactly-nth equal chunks, `ggml-cpu.c:1403`), so
ALL 96 threads work ~22-row slices; grain 64 is a strategy *selector*, not a lane cap, and
their flat curve is carried by the ~µs barrier alone. Our grain-64 env test: decode
43.9→**53.8** t/s at default (+13-15%). Full fix = cheaper rendezvous (barrier-cheap epoch),
NOT lane limiting — see session 2.

**Day total: Llama-1B decode 7.6 → 97 t/s (12.7×) with knee-threads, 43-54 at defaults;
prefill-512 2×.** Fixes: min-grain clamp + phys-core cap (committed) + team-default (pending
decision) + decode-lane knob (ledger).

### llama.cpp Llama-1B thread curve (knee-vs-knee, same box)
| threads | lcpp pp512 | lcpp tg128 | us decode (team) |
|--:|--:|--:|--:|
| 6 | 211.5 | 52.1 | — |
| 8 | 282.3 | 67.6 | **76 (1.12× — we win)** |
| 24 | 790.4 | 137.6 | 97 (0.70×) |
| 96 | 2605.0 | 139.1 | 43 (0.31×) |

**The decomposition that matters:** lcpp decode PLATEAUS 24→96 (surplus threads cost nothing —
spin-barrier lockstep); ours PEAKS at 24 and FALLS (every thread costs a rendezvous share).
Below the knee we are at parity or better → kernels/memory path fine; the whole remaining gap
is scheduler-overhead growth. Next levers: decode-lane capping / GEMV grain 64 (+13-15%
measured), and the non-GEMM loop audit — `dasllama_common.das:2012` forces decode attn parallel
unconditionally under team mode (M1-tuned), plus M1-tuned per-pass thresholds; instrument =
`decode_prof.das` per-op attribution at T=24 vs default.

## Session 2 (2026-07-03 evening) — GEMV grain, the settled 32-anomaly, the dispatch-tax decomposition

**Box continuity break:** the box REBOOTED ~19:03 (uptime reset; `/data` intact) and was re-set-up:
cmake reconfigure 19:14, models re-copied 19:25-19:29, full-tree rsync 20:35, `bin/` rebuilt 20:52
(now the shared-lib layout — `liblibDaScriptDyn.so`, 97KB launcher). Engine revalidated green after
(floor + suite). Consequence: cross-reboot absolute comparisons join the thermal caveat — in-batch
ratios only.

**Harness gotcha (cost 20 min):** `llama3_perf.das` drives HARDCODED Llama-3 token ids (128000 =
begin_of_text). A smaller-vocab model (SmolLM, 49152) reads past the embedding table →
"array index out of range" at `dequant_q8_row` — looks exactly like an engine bug and isn't.
Guard added (vocab check + exit). Shape-agnostic harness = `prefill_perf` (tokens 1000..5999).

### GEMV-specific min grain — implemented (the decode half of the grain fix)
ggml uses grain 16 for GEMM but **64 for GEMV** (`nr0==1||nr1==1`). Mirrored: `matmul_chunks_gemv`
+ `g_matmul_min_chunk_rows_gemv` (default 64, env `DASLLAMA_MIN_CHUNK_ROWS_GEMV` in
llama3_perf/prefill_perf). All 50 GEMV-shaped dispatch sites (mm / group3 / groupn kernels, the
fp32+q8+q4 matvecs) across the 4 math files switched; batch/prefill sites keep grain 16. Both
grain knobs wired into the `box_profile` runtime rail (`matmul_min_chunk_rows{,_gemv}`).

### Grain ladder — Llama-1B decode @ defaults (95 workers), sequential runs
| gemv grain | 0 | 16 (old) | 32 | 64 | 128 | 256 | 512 |
|---|--:|--:|--:|--:|--:|--:|--:|
| tok/s | 42.5 | 44.4 | 43.3 | 49.2–52.2 | **57.3** | 53.1 | 44.4 |

@ T=24 knee: g64 94.9 ≈ g128 94.3 (knee is rendezvous-bound, not chunk-bound); g256 74.5 (starves
25 lanes). **The grain-32 anomaly is SETTLED: real, not noise** — 32 ≈ 16 ≈ 0; the win only starts
at 64. **Default stays 64** (ggml parity; M1's 9 lanes would starve small-d GEMVs at 128); EPYC's
box_profile should carry **128**.

### Per-op attribution (llama3_perf built-in profile, µs/token, 64-tok window)
| bucket | defaults g16 | defaults g128 | T=24 g64 |
|---|--:|--:|--:|
| mm_qkv (group3, 3072 rows) | 10752 | 4178 | 2032 |
| mm_ffn (w13 16384 + w2 2048) | 6924 | 8212 | 5220 |
| mm_wo | 1220 | 1568 | 681 |
| attn (forced-par under team) | 1363 | 1094 | 455 |
| final (cls) | 1539 | 1624 | 1468 |
| **TOTAL** | **22196** | **17168** | **10222** |

Three structural readings:
1. **Per-shape grain conflict:** mm_qkv wants coarse (2.6× win at 128); mm_ffn wants fine — its
   w2 (d=2048) drops to 16 chunks at g128 and loses 19% (bandwidth-starved at 16 active lanes).
   One global GEMV grain is a compromise → per-shape grain (or a work-based grain) is the next
   rung — ledger.
2. **The dispatch tax is ~flat per dispatch and scales with lane count:** every bucket except
   `final` costs ~2× more at 96 lanes than at 24 at comparable chunk counts; the defaults-vs-T24
   delta ≈ 86 µs/dispatch × 81 dispatches/token. Chunking can't remove it (team rendezvous
   involves every lane regardless of who gets work) → the structural lever is a **decode lane
   cap** (dispatch on a subset of the team) or cheaper rendezvous. This is the whole remaining
   defaults-vs-knee gap (57 → 95 t/s).
3. **Boris's forced-attn hunch: confirmed, secondary.** The team-mode-unconditional decode attn
   dispatch costs ~0.9 ms/tok extra at 96 lanes vs 24 (6% share) — real, but the same tax hits
   every dispatch; fixing attn alone buys ~5%.

Floor (post-reboot revalidation): SmolLM T=1 per-token 105-122 t/s, prefill ~330 t/s — sane.

### Team-slot cache-line surgery — NEGATIVE RESULT (do not retry)
Hypothesis: the ~86µs/dispatch tax = claim-line refetch storm (spinners acquire-poll `mTeamSeq`
on the same line `mTeamOp` claims hammer). Tested 4 layouts, bracketed with controls, Llama-1B
decode @ defaults:
| variant | g128 | g16 |
|---|--:|--:|
| ctrl / ctrl2 / ctrl3 (unpadded, shipped) | 58.7 / 61.3 / **61.4** | 47.5 / 45.6 / 34.1 |
| alignas(64) op, remaining, parked each own line | 50.9 | 33.1 |
| + per-iteration test-before-claim peek | 52.4 | 34.2 |
| seq isolated, op+remaining paired | 50.0 | 31.6 |
| ditto + entry-only peek | 56.4 | 33.9 |
**Every padded layout LOSES to the unpadded original** (−8-18% @ g128). The co-located slot is
load-bearing, not accidental. Also: the g16 CONTROL itself swung 47.5→34.1 across brackets —
fine-grain (192-chunk) decode is chaotically sensitive to box state after hours of load; g128 is
stable (58.7-61.4). Coarse grain buys *stability*, not just speed. Always bracket controls.

**Why ggml is still cheaper per op (the real mechanical difference, `ggml-cpu.c:1414-1441`):**
in the decode (equal-split fallback) regime ggml has NO shared claim atomic at all — thread i
takes chunk i (`current_chunk = ith`), and `nth >= nchunk` short-circuits the `atomic_fetch_add`
path entirely. Per-op sync = static slice + one barrier arrival. Ours = seq poll + dynamic
fetch_add claims + remaining decrements on every op. The residual tax is the *protocol*, not the
line layout. Candidate designs (undecided — instrument first): (A) static partition when
`numChunks <= lanes` (ggml-exact; loses robustness to a busy/parked worker — needs a caller
sweep for unserved chunks), (B) sharded claim counters, (C) neither — measure the actual
anatomy (publish→first-claim, →last-claim, →join-done, caller-vs-worker claim share) with rdtsc
instrumentation in teamParallelFor at T=24 vs 96 before any further surgery.

### Team dispatch anatomy (rdtsc instrument, `DAS_TEAM_PROF=1`) — the tax theory FALSIFIED
`teamParallelFor` phase averages, Llama-1B decode (4485 ops ≈ 70/token):
| avg per op | T=96 g128 | T=24 g64 |
|---|--:|--:|
| publish (stores + seq + wake check) | 789 ns | 359 ns |
| first worker claim after publish | **+206 ns** | +295 ns |
| last worker claim starts | +9.2 µs | +3.3 µs |
| caller-serve (its ~1 chunk of ~40) | 107 µs | 68 µs |
| join-tail after caller done | 24.5 µs | **41.1 µs (37% of op)** |
| total | 133 µs | 110 µs |
**The rendezvous is ALREADY barrier-cheap** — sub-µs publish, 200 ns worker reaction, 0% solo
ops. The "~86 µs/dispatch tax" was a confound: per-chunk COMPUTE inflates with active-lane count
(same-shape chunk 68→107 µs from T24→T96 — bandwidth/frequency contention), not sync. Protocol
surgery (static partition, sharded counters) is pointless — closed.

**Equal-split GEMV cap (ggml fallback shape, `min(want, lanes)` in matmul_chunks_gemv): NEUTRAL,
kept.** T24 92.2-96.0 (was 94.3-94.9), T96 g64 51.3 (was 49-52). Join-tail 41→31 µs, but larger
per-chunk time absorbed it. Kept for ggml parity + smaller tail + no-oversplit simplicity.

### The remaining decode gap, decomposed (the campaign's new frame)
- Defaults (96 lanes): per-chunk inflation — more active lanes = slower every stream. Coarse
  grain (128) mitigates; structural fix unknown (lcpp plateaus by equal-splitting tiny slices
  across all 96 — their per-thread streams shrink but wall holds).
- **Knee (T=24): pure memory-path efficiency.** Per decoded token both engines stream the same
  1.2 GB of weights; we do it at ~113 GB/s (10.6 ms/tok), llama-bench at ~164 GB/s (7.3 ms/tok),
  both against the same ~180 GB/s 24-core-DRAM ceiling. Sync µs, chunking exhausted → the 1.45×
  is kernel/memory-path (prefetch, requant overhead, repack layout, NT loads). This is
  `tune_kernels` + ISA-ladder territory — the original mission, now with the target metric:
  **GB/s per decoded token at T=24**.

### Session 3 (2026-07-03 night): `tune_kernels` ran → box_profile.json exists (the original mission artifact)
Sweep is fast (~1 min wall, 15 kernels × 20 perms, interleaved best-of, all correctness gates
green). Real winners vs shipped fallbacks (rest ≤2-3% = noise floor, honest shape):
| kernel | shipped | winner | isolated Δ |
|---|---|---|--:|
| dot_q8q8 | vec16 | u2 (any unroll ≥2; bare vec16 = worst) | **−22.5%** |
| dot_q4 | vec4_u4 | vec8_u8 | −16% |
| rope_scaled_neox_tab | vec8_u2 | plain | −21% |
| rmsnorm | plain | vec8_u2 | **−87% (7.7×)** |
Zen4 reading: plain `vec8` rules the float elementwise set (double-pumped 512 — 256-bit-shaped
loops already saturate); vec16/vec32 only ever win on don't-care bandwidth kernels.
**End-to-end: NEUTRAL.** ABBA brackets, 1B decode, gemv grain 128 pinned via env both sides:
T=24 base 89.8/88.7 vs prof 90.2/88.4; defaults base 60.2/57.6 vs prof 64.4/56.3 (defaults
swing 56-64 — chaotic, as usual). Why: decode's hot GEMVs run the repack-backend matrix
kernels (hand intrinsics, not `[tuned]` templates), and the real wins live in ~1%-of-token
buckets (rmsnorm, rope). The profile is still right: no-harm + big wins for portable-tier
consumers. Runtime section hand-carries `matmul_min_chunk_rows_gemv=128`; the tuner writer
now snapshots both grain knobs (was missing — predates session 2).
**Note absolute drift again:** T=24 ~89 t/s tonight vs 94-96 this afternoon, same code —
cross-session absolutes remain polluted; only in-bracket deltas count.

### Session 3b: per-op breakdown vs instrumented llama.cpp + THE ISA LADDER — portable+tuned WINS
**Instrumented lcpp on box:** `ggml_op_profile.patch` applied to `/data/llama.cpp` (git tree
left patched, runtime-gated on `GGML_OP_PROFILE=1`), rebuilt. Decode table = `llama-bench
-p 0 -n 64 -t 24`, divide bucket totals by mm_cls nodes (=tokens, 321).

Per-token µs @ T=24, Llama-1B Q8 (lcpp 138.7 t/s; ours under each backend):
| bucket | lcpp | auto=avx2-repack | portable+tuned | port/lcpp |
|---|--:|--:|--:|--:|
| mm_ffn | 3884 | 5873 | 5535 | 1.42× |
| mm_qkv | 664 | 1969 | **877** | 1.32× (was 2.96×) |
| final/cls | 1243 | 1563 | 1490 | 1.20× |
| mm_wo | 348 | 739 | **736 (unmoved)** | 2.11× |
| attn | 173 | 464 | 333 | 1.93× |
| rope+kv | 181 | 82 | 99 | we win |
| TOTAL | 7083 | 10970 | 9337 | 1.32× |

**ISA ladder** (T=24, gemv 128, box profile ON, one process per rung, 3 auto controls):
| rung | t/s |
|---|--:|
| **portable (tuned perms)** | **103.7** |
| x64-avx2-acc8 | 98.0 |
| x64-vnni256-acc8 | 94.8 |
| auto = x64-avx2-repack (controls) | 88.3 / 92.7 / 90.4 |
| x64-avx2 | 91.1 |
| x64-vnni256 | 90.9 |
| x64-avx512bw | 90.0 |
| x64-vnni256-repack | 89.8 |
| x64-avx512vnni | 84.2 |
| portable UNTUNED (attribution follow-up) | 77.1 |

Four readings:
1. **The tuner was NOT end-to-end neutral — it flipped the backend ranking.** Profile is worth
   +34% on portable (77.1→103.7); tuned-portable beats the shipped auto default by ~+15%.
   The session-3a "neutral" verdict was an artifact: `[tuned]` hints only bite on the
   portable-tier kernels; the intrinsic auto-backend masks the profile entirely. **Lesson:
   run the ISA ladder WITH the profile on before calling a profile neutral.**
2. acc8 (lcpp's per-block accumulator shape) beats its dot4x4 sibling on Zen4 GEMV decode
   (98.0 vs 91.1 avx2; 94.8 vs 90.9 vnni256) — best intrinsic family, still below tuned-portable.
3. VNNI buys ~nothing at decode (bandwidth-bound; 90.9≈91.1, 94.8<98.0); zmm tiers LOSE
   (84-90 — double-pumped 512 + z16 shape overhead). The matrix's decode promotion answer on
   Genoa: none — delete-side evidence for the promote/delete question (prefill still open).
4. Remaining vs lcpp at tuned-portable: ffn 1.42×, wo 2.11× (the one unmoved bucket — small-d
   GEMV, suspect per-op fixed cost: requant + entry), cls 1.20×, attn 1.93× (team-gated
   dispatch, known). 103.7/138.7 = 75% of lcpp (was 64%).

**Promotion (the designed rail):** `box_profile.json` runtime now pins `"backend": "portable"`
— verified end-to-end via `load_model`: logs `backend = portable (profile pin 'portable')`,
`active_kernel_backend()` == portable, grain knobs applied. prefill_perf gained
`DASLLAMA_PIN_BACKEND` support (mirrors llama3_perf) for the prefill-side validation of the pin.

### Session 3c: the pin's prefill cost + the strictly-dominant hybrid candidate
prefill_perf ABBA @512 (defaults threads): auto=repack 1009/1003 vs portable 871/865 —
**portable pays −14% prefill** (repack's batch tier earns its keep... or so it seemed).
Follow-up point: **x64-avx2-acc8 prefill 1047 vs auto ctrl 1018** — the ROW-MAJOR acc8 batch
matches/beats the grp4 repack tier on Genoa (repack = no value on this box, delete-side
evidence #2). Bonus from the same log: at 96-lane defaults portable decode = 75.2 t/s
per-token vs auto 49-53 (**+45%** — the tuned-portable defaults win is bigger than the knee win).
Standing decision fork (Boris): backend pin = portable (decode-max, campaign metric) vs acc8
(strictly dominates auto both axes) vs a **hybrid** — portable mm/group3/groupn + acc8 batch,
both row-major so trivially registrable per the matrix slot-mixing design; proper rail would
be per-slot pins in box_profile runtime (e.g. `"batch_backend"`), not a hardcoded mix.

### Session 3d: the M1 cross-check — the picture is coherent across µarchs
Same ladder-lite on the M1 Max dev box (worktree binary rebuilt, tuner re-run, defaults =
9 workers, 1B decode): auto=arm64-laneq controls 79.7/75.5, arm64-sdot 78.0, portable-tuned
77.8, portable-untuned 76.9 — **all inside the control bracket; decode is backend-invariant
on M1** (bandwidth saturates; machine was in interactive use, hence the 5% control swing —
deltas only). Tuned≈untuned on M1 (~1%): the shipped fallbacks WERE the M1 hand-tune — except
rmsnorm, where `plain` was wrong on both platforms (M1 sweep: vec8 +89%, mirroring EPYC).
M1 tuner's other finds: dot_mx4q8 u8 +10.3%, rope vec4_u4 +5.3%; dot_q8q8 HOLDS vec16
(the u2 win was Zen4-specific).

**The unified story:** decode = bandwidth-bound GEMV → tuned-portable ≥ hand kernels
everywhere (equal on M1, +15% on Genoa where the intrinsic shapes were tuned for other
µarchs); batch/prefill = compute-bound GEMM → hand/batch tiers win (laneq on M1; row-major
acc8 suffices on Genoa, grp4 repack worthless there). The hybrid (portable GEMV slots +
best batch slot) is x64-only in practice today — on M1 the GEMV delta is ~0, keep laneq whole.
NB portable-GEMV + repack-batch is NOT directly composable: repack interleaves planes
in place (see the tied-cls dual-copy precedent); the row-major hybrid avoids this entirely.

### Session 3e: HYBRID SHIPPED (Boris call) — per-slot batch pin, box profile complete
`select_batch_backend(name)` / profile `runtime.batch_backend` / env `DASLLAMA_PIN_BATCH_BACKEND`:
overrides only the batch-shaped slots (batch, mx4_batch, mx4_batch_groupn, mx4_native_batch flag)
from a layout-compatible donor; sticky across re-activation (activate() re-applies the pin, so
the load-time repack select can't clobber it); layout guard rejects repack↔row-major mixes and
different repack layouts. Covered by test_kernel_backend (6/6).
**Measured (1B):** prefill @512 hybrid **1044** ≈ acc8 1047 (portable-alone 871, auto 1016);
decode T=24 hybrid 98.0 ≈ portable ctrl 102.2 (bracket noise — decode never calls batch slots);
defaults per-token 71.0 vs auto 60.2. EPYC box_profile.json final: backend=portable +
batch_backend=x64-avx2-acc8 + gemv grain 128 + tuned perms = best measured decode AND prefill.
**The "25 chunks on 24 cores" question (Boris): by design, not a bug** — get_dispatch_lanes() =
workers + the caller, which serves a chunk too (workers-only split measured −8% on M1);
matmul_chunks_gemv already is `min(lanes, rows/N)` with balanced sizes = the asked-for rule;
the open knob is only N per shape.

### AVX_MATRIX master switch REMOVED (Boris, post-validation)
Correctness held on real silicon (probe + gpt-oss token-for-token, 4 backends) → the
`DASLLAMA_AVX_MATRIX=1` env gate is redundant. Matrix tiers now register unconditionally
per-tier (`cpu_supports` gated); priorities unchanged (3-7, below shipped) so auto-selection
is untouched — still pin-only until the ISA ladder promotes. `DASLLAMA_AVX_MATRIX_FORCE=1`
kept (probe/validation on tier-less boxes). Suite sweeps now cover matrix backends by default
on capable hardware.

## Open items / next
- [x] ggml min-chunk-size comparison (teammate) → pick MIN_ROWS_PER_CHUNK / work metric.
- [x] Implement + measure the min-chunk-size clamp (batch grain day-1; GEMV grain session 2).
- [x] GEMV grain ladder → 128 on this box; grain-32 anomaly settled (real, mechanism understood).
- [x] ~~Decode lane cap~~ CLOSED session 2c — the "86 µs tax" was per-chunk compute inflation
      (bandwidth/frequency contention), not rendezvous; equal-split cap landed neutral, kept.
- [ ] Per-shape / work-based GEMV grain (mm_qkv vs mm_ffn pull opposite) — low prio.
- [ ] Per-cell VNNI-vs-donor A/B at a fixed thread count (the promote/delete question). Pin 96 baseline.
- [ ] #3370 dispatch-lanes utilization delta on 96 cores.
- [ ] In-model dasLLAMA vs llama.cpp (same thread count both sides), Llama-1B + Llama-8B + gpt-oss.
- [x] `tune_kernels` → box_profile.json for the EPYC (gemv grain 128 carried; session 3).
- [ ] **ISA-yield ladder** (portable→avx2→vnni256→avx512bw→avx512vnni, pin-interleaved) with the
      target metric GB/s per decoded token at T=24 (113 now, lcpp 164) — THE open perf lever.

### DAS_JOBQUE_THREADS semantics — the off-by-one Boris caught (session 3e)
`DAS_JOBQUE_THREADS=N` spawns **N WORKERS**; the caller computes too ⇒ **N+1 execution lanes**
(job_que.cpp:85 raw override; getTotalHwJobs()==worker count; get_dispatch_lanes()=workers+1).
Only the DEFAULT path is total-lane-shaped (cores−1 workers + caller = cores). ggml `-t N` = N
TOTAL threads. So every "T=24" compare above ran ours on 25 lanes vs lcpp's 24. Re-measured
fair: 23w+caller (24 lanes) = **100.6** vs 24w+caller (25 lanes) = 95.5, same bracket — knee is
bandwidth-bound, the extra lane is worthless; the 75%-of-lcpp headline HOLDS at true parity.
**RULE: fair vs `llama-bench -t N` = `DAS_JOBQUE_THREADS=N−1`.** (Env semantics left as-is —
too much measurement history keyed to it; flag if we ever want env = total lanes.)

### Session 3f: WAVE FILL (Boris's rule) — +14% at the knee
Rule: chunk counts land on whole waves of the lane count. Grain N is halvable, so a
grain-derived count in `[lanes/2, lanes)` rounds UP to one full wave (chunks shrink, stay
≥ N/2, no lane idles); below lanes/2 stays grain-limited (won't shred). Implemented in
`matmul_chunks_gemv` (knob `set_gemv_wave_fill`, default ON; env `DASLLAMA_GEMV_WAVE_FILL`,
profile `gemv_wave_fill`). Kept the 1-wave cap for want > lanes (dynamic claims self-balance
multi-wave; ggml parity).
ABBA @24 lanes (23w, portable-hybrid, N=128): OFF 93.6/103.0 vs **ON 106.4/118.3** — ON wins
both ordered pairs; mm_wo 16→24 chunks/disp (bucket −12%), w2 filled (mm_ffn −15%). Defaults
inert as designed (66.9 vs 68.0; 2×16 < 97 lanes). **Peak 118.3 = 85% of lcpp 138.7; bracket
avg ~112 = 81% (campaign start: 64%).** Suite 173/173, lint 0.
NB tonight's drift is large (±10% across brackets after hours of load) — the ON-vs-OFF margin
exceeds it in both orderings, but absolutes need a fresh-boot confirm.

### Session 4 (2026-07-04): FUSED DISPATCH CHAINS — 81 → 33 disp/tok, +7.6% at the knee (all pairs)
The join-tail attack Boris chose (session 3f next-steps #1), built as general machinery:
- **jobque `teamParallelForStages`** (+ das `team_parallel_stages` in jobque_boost): one team
  rendezvous runs N dependent stages with worker-side barriers between them. Seqlock publish
  (odd seq = "publishing") + an in-flight entrant gate close the cross-chain stage-order race by
  construction (a straggler can never hold a stage claim word across a republish); per-stage
  claim words keep the packed {K|ctr} epoch-race fix. Single-op teamParallelFor is now
  stages(1) — one protocol. tests/jobque 200/200 on both boxes; TSAN 25x0 on the new storm test.
- **dasLLAMA fused decode chains** (knob `set_fused_decode`, default ON; env
  `DASLLAMA_FUSED_DECODE`, profile `fused_decode`): attention block = ONE 3-stage chain
  [qkv rows + bias/qk-norm/rope/kv-store per head slot] -> [attn heads + per-head wo-input
  requant] -> [wo rows + bias + residual add]; dense FFN = ONE 2-stage chain [paired w1/w3 rows +
  act + requant into new xq2/xs2 scratch] -> [w2 rows + residual add]. All the old serial glue
  rides inside chunk epilogues. Backends need the new `mm_rows` row-range core (portable + both
  arm64; x64 intrinsic tiers ledgered) — `kernel_backend_has_rows()` gates, per-layer fallback
  to the per-op path (also: non-q8, pre_post_norm arches, head_size % 32 != 0, MoE FFN).
- **Bit-exactness proven, not assumed:** test_fused_decode (new) compares 24 greedy steps
  fused-vs-per-op — ids AND logits EXACTLY equal — on stories15M (FFN chain) and SmolLM2-135M
  (both chains), under the auto backend on M1 (laneq rows core) AND under a pinned-portable
  fresh load (the EPYC decode config, verified on the box). The hs=48 requant mis-slice the
  first run caught -> the head_size % 32 gate. Suite 177/177 local AND box (NB the box suite's
  gguf tests auto-select an x64 tier with no rows core, so suite-on-box exercises the FALLBACK;
  the pinned-portable test run is the box's fused proof).
- **Measured (1B, 24 lanes = 23w, portable-hybrid, gemv 128):** dispatches 81 -> **33/tok**
  (16 attn_chain + 16 ffn_chain + cls). In-process interleaved A/B (new `DASLLAMA_FUSED_AB=N`
  rail in llama3_perf — ABBA window order cancels KV-depth drift): **fused wins ALL 6 pairs,
  mean +7.6%** (per-pair +1.8..+24%, median ~+5). Fresh-process bracket peak **124.0 t/s =
  89% of lcpp 138.7** (campaign start 64%, session 3f 85%).
- **WARNING: 96-lane defaults REGRESS under full fused: -9.2%** (4-pair in-process A/B, ON 63.0
  vs OFF 69.4). Suspected: the attn chain's narrow stages (48 slots / 32 heads) leave most of 96
  lanes at stage barriers where the per-op path lets them idle differently; ffn-only probe was
  confounded (the 10M threshold un-threaded small ops in both arms; +1.8% under that config).
  Default stays ON (knee = the campaign metric + every <=32-lane box wins); the defaults auto-
  gate (e.g. lanes <= 2 x n_heads — the whole-wave rule extended to chains) is Boris's call.
- Fresh-boot confirm of absolutes still pending (drift tonight as usual; in-process pairs are
  the trustworthy numbers).

## Session 5 (2026-07-04): fleet sweep vs lcpp begins + the prefill "regression" resolved + zvnni batch pin (+20%)

**Fleet-sweep method (Boris's spec):** per (model, T): ours = `DAS_JOBQUE_THREADS=T emission_bench
-p 512 -n 64 --nprompts 4` (ttft = direct batched prefill-512 + 1 token; derived pp512 validated
vs prefill_perf direct at ±1% on 1B/8B/gpt-oss); lcpp = clean `llama-bench -p 512 -n 64 -t T`
(op-profile patch saved to `/data/ggml_op_profile.saved.patch`, tree restored). Same T both sides.
Driver `/data/sweep/run_bracket.sh`; ~30 min per T bracket, 14 models. **Harness fix that
mattered:** emission_bench/prefill_perf used raw `load_gguf` → never applied box_profile runtime
(would have swept the UNTUNED auto stack); both now call `apply_box_profile_runtime()` first
(commit `eb43dccef`).

**T=8 + T=16 fleet result (acc8-pin config):** prefill uniformly 0.60-0.67× lcpp on every dense
model; emit scales with size — 0.24× (135M, FLAT across T = the 2M par threshold leaves all
non-cls matmuls single-lane) up to 0.90-0.97× (4-8B); gpt-oss outlier 0.39×/0.23×. Full tables
in the session transcript; raw logs `/data/sweep/T{8,16}_*`.

**The "we lost repack" scare — resolved, no regression anywhere:**
- Boris desktop (3990X Zen2, master tip): 1B pp 361 vs lcpp 409 at 16C (0.88×) — but **+20% WIN
  at 32C**, reproduced on current code. #3365's 121% claim was @32T; 16T was never a measured
  point. lcpp scales flat (409→~500), ours steep (361→~600) — the curves cross between 16 and 32.
  #3368/#3369/#3370 cost nothing on Zen2.
- Genoa box, same code, same model, T=32: ours 565/571 pp vs lcpp 1072 = **0.53×**. The flip is
  the opponent, not us: lcpp's `arch/x86/repack.cpp` compiles AVX512VNNI **zmm** GEMM paths on
  Genoa (absent on Zen2) ≈ 2× per-core int8 throughput. We out-kerneled AVX2-lcpp; VNNI-lcpp is
  a different engine.
- Batch-donor A/B (prefill_perf 1B, profile-variant via DASLLAMA_BOX_PROFILE, mirrored brackets):
  acc8→repack-whole is only **+3%** (the hybrid's acc8 pick cost ~nothing); ladder @T=8:
  acc8 182 → zbw 198 → vnni256-acc8 201 → vnni256-repack 208 → **zvnni 219 (+20%, wins every
  pair at T=8/16/32; decode identical — slot isolation confirmed)**.

**Profile updated: `runtime.batch_backend = x64-avx512vnni`** (backup `box_profile.json.pre-zvnni`).
The zmm acc16 GEMM loses decode (84.2 in the s3b ladder) but wins batch — compute-vs-bandwidth
split, exactly the hybrid design's use case. Gap after pin: 0.77×/0.71×/0.64× @T=8/16/32.
Fleet sweep restarted from T=8 under this config.

### Final fleet tables (zvnni pin, lcpp -r 1; ratio = ours/lcpp; pp derived from emission ttft, spot-validated)
| model | T8 pp | T8 emit | T48 pp | T48 emit |
|---|--:|--:|--:|--:|
| SmolLM2-135M | 0.86 | 0.25 | 0.47 | 0.28 |
| Qwen2.5-0.5B | 0.78 | 0.58 | 0.44 | 0.36 |
| Qwen3-0.6B | 0.85 | 0.66 | 0.57 | 0.40 |
| Gemma-3-1B | 0.73 | 0.70 | 0.53 | 0.46 |
| Llama-3.2-1B | 0.77 | 0.92 | 0.58 | 0.50 |
| Qwen2.5-1.5B | 0.77 | 0.76 | 0.54 | 0.49 |
| SmolLM2-1.7B | 0.81 | 0.88 | 0.62 | 0.54 |
| Gemma-2-2B | 0.81 | 0.87 | 0.62 | 0.61 |
| Phi-3.5-mini | 0.83 | 0.99 | 0.67 | 0.64 |
| Gemma-3-4B | 0.81 | 0.89 | 0.61 | 0.62 |
| Qwen3-4B | 0.81 | 0.97 | 0.66 | 0.60 |
| Mistral-7B | 0.82 | 0.90 | 0.69 | 0.75 |
| Llama-3.1-8B | 0.80 | 0.95 | 0.70 | 0.75 |
| gpt-oss-20B-MoE | 0.61 | 0.23 | 0.27 | 0.40 |

zvnni-pin before/after @T8 pp (acc8→zvnni): dense +15-27% (mean ~+21%), gptoss +56% (0.39→0.61).
Absolutes: 1B T48 pp 857 (lcpp 1472), 8B T48 emit 19.8 (lcpp 26.4) — ours still climbs 8→48
everywhere ≥4B; lcpp climbs harder (zmm GEMM + bandwidth on pp; decode plateau vs our falloff).
Boris's verdict: box work complete — parked. EPYC profile preserved as
`modules/dasLLAMA/box_profile.epyc9654.json`; raw logs were /data/sweep (ephemeral).

**Open questions sharpened by the sweep:** (1) portable GEMV pin costs at low T (1B emit 61 @T8
vs day-1 auto 76; SmolLM T=1 floor 73 vs 116) — per-lane-regime backend pin candidate; (2) the
fifo-era par thresholds (2M) strangle ≤1B models at every T (135M emit flat 84 vs lcpp ~350);
(3) remaining prefill gap = Genoa-tuned zmm GEMM arc (tiling/blocking vs lcpp's repack kernels).

## Campaign method reminders
- Correctness before speed; real-model tests = **small model, one run at a time** (the oracle
  path in `gptoss_parity_probe` is deliberately un-vectorized → slow; not a perf tool).
- Interleaved A/B in one process for cross-backend; absolute numbers need a quiet box.
- Pin thread count explicitly (`DAS_JOBQUE_THREADS`), same on both sides of any llama.cpp compare.
