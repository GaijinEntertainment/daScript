# Metal / mac-session follow-up - the M5 catch-up ledger

**Status: OPEN (2026-08-31, unquirk pass).** The ledger for the parallel mac session that
forks after the unquirk pass's PR-1. Metal-tier perf items land here; the CPU items below are
mac-session work too -
the M-series CPU tiers are minted and raced from that box.

## 1. The smmla (i8mm) leg - RACED on M5, NEON keeps the crown

The five `dot = "smmla"` seats of `q8q8_tile_gen` (mr4/mr8 x kstep/nrsplit/gkstep,
`requires = "i8mm"`) raced for the first time in the 2026-09-01 M5 box mint, post the
PR-1 detection fix (`g_target_arm64_i8mm` consults `cpu_supports`, `+i8mm` appended to the
target machine). The `harness/smmla_probe.das` gate passed first: correctness OK, 2.02x
register-resident MAC throughput over sdot4 - the nominal ceiling. In the tile race that
ceiling does not survive the memory traffic plus the kg8 re-layout (the tile race:
`DAS_TUNE_MODE=tune harness/dasllama_tuner.das`'s q8q8_tile_gen family bench, m5): best smmla seat
`mr8_kstep2_nrsplit2` at 25893 us vs NEON `mr8_budget` at 21349 us (~21% behind; full table
in the m5 sidecar's race section and `~/.tune-history/m5/`). The `arm-i8mm` defaults profile
SHIPS regardless (`performance/defaults/arm-i8mm.tune-defaults.json`, exported from the full
m5 mint): its winners are the NEON ones, and its `i8mm` features fingerprint marks the smmla
seats raced-and-covered, so no M2+ box re-races them at adoption.

Consequences: an smmla kq tile emitter arm is NOT mac-leverage (the q8q8 verdict transfers -
the kq tiles are more memory-bound, not less); it remains a Graviton3+/c8g candidate raced on
that silicon, not built speculatively. SME/SME2 stays the researched-first tier: no seats, no
emitter, new kernel design (streaming mode + ZA tiles).

## 2. The fixtures - which GGUF per format, and where to get it

Every vehicle is a Llama-3.2-1B-Instruct requant unless named otherwise. zen2 holds them at
`D:\Work\llama.cpp\models\`; the M1 worktree box holds the same set at
`~/Work/llama.cpp/models/` together with the baked `.dlim` images beside each; the M5 starts
empty - `scp m1:~/Work/llama.cpp/models/<file> ~/Work/llama.cpp/models/` (or from zen2) per row.
The `-local` files are our own `llama-quantize --allow-requantize` products (the IQ ones with
`--imatrix`), the rest are bartowski / mradermacher downloads.

| format | vehicle GGUF | notes |
|---|---|---|
| IQ4_XS | `Llama-3.2-1B-Instruct-IQ4_XS.gguf` | the pilot (bartowski) |
| Q3_K | `Llama-3.2-1B-Instruct-Q3_K_L.gguf` | a third of the bytes are k5/k6 |
| IQ3_S | `Qwen2.5-1.5B-Instruct-IQ3_XS.gguf` (kernels) + `Llama-3.2-1B-Instruct-IQ3_M.gguf` (e2e/bench) | the Qwen file proves kernels, the local IQ3_M carries iq3_s on the bench rows |
| IQ3_XXS | `Llama-3.2-1B-Instruct-IQ3_XXS-local.gguf` | `Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf` (mradermacher) mixes in IQ2_S attn - the IQ2_S vehicle below |
| IQ4_NL | `Llama-3.2-1B-Instruct-IQ4_NL-local.gguf` | no imatrix needed |
| Q2_K | `Llama-3.2-1B-Instruct-Q2_K-local.gguf` | |
| IQ2_S | `Llama-3.2-1B-Instruct.i1-IQ3_XXS.gguf` | IQ2_S attn x32 + IQ3_XXS/IQ3_S/Q4_K/Q5_K - a MIXED vehicle by construction |
| IQ2_XS | `Llama-3.2-1B-Instruct-IQ2_XS-local.gguf` | carries q2_K on blk.0/1 ffn_down - the mixed-vehicle e2e of QUIRK 26 |
| IQ2_XXS | `Llama-3.2-1B-Instruct-IQ2_XXS-local.gguf` | prompt ids `128000,12805,5304,264,892`; the step-3 0.0267 top-2 tie |

The reference build for the ratios is llama.cpp b10660 (`~/Work/llama.cpp/build/bin/llama-bench`
/ `llama-simple` on the M1; the same tree serves the `simple_ids` oracle). Parity prompt for
every format: `harness/parity.das -- -m <gguf> -n 40 --ids 128000,12805,5304,264,892`.

## 3. Metal tier perf items

The per-format Metal items: the IQ4_XS lane-map gap is `followup_general.md` item 58, the
dispatch-loop probe method is `HOW_TO_ADD_A_FORMAT.md`'s Metal step, and the M1 tg tails of the
formats' bring-ups predate the ARM row-group decode - re-stamp a vehicle before quoting one.

## 4. The elementwise / activation-precision lane (the last M5 pp residual)

Attribution (M5, 1B iq2xxs, pp512 = 31 ms encode; `benchmarks/lcpp_bench.das
--for-debug-purposes --ngl 99 -p 512`, the prefill stage log line + per-kernel lab rates from
`benchmarks/matmul/bench_metal_kq_race.das`; the llama.cpp slice from its `llama-bench -p 512`
wall minus the same mm/attention accounting at test-backend-ops rates): mm 27.7 ms at measured
tensor-twin rates, attention 0.16 ms - the ~3 ms remainder is every non-matmul pass over the
activation planes (norms, residual adds, rope, swiglu, activation converts, glue). llama.cpp's
slice: ~1.7 ms.

**What already exists** - the producer-fused f16 twin family (`_hx`): `pf_enc_rms_hx`,
`pf_enc_add_rms_bhx` (add+norm+half-emit in one), `enc_swiglu_hx`/`enc_geglu_hx`,
`enc_qk_rope_hx`. On the dense path these cover the norm and activation producers; the
standalone `enc_cvt_half` fires only through `pf_cvt_panel` fallbacks and at the sites below.
Measured ceiling of ALL remaining converts (`DASLLAMA_METAL_PREFILL_SKIP=act_cvt` knockout on
`benchmarks/lcpp_bench.das --for-debug-purposes --ngl 99 -p 512 -n 16 -r 3`, m5):
**+0.65% pp512** (15334 vs 15235 tok/s) - the fusion rung is mostly banked already.

- **4a. The attention-out `_hx`** (the one live dense-path cvt): the AV kernels
  (`MetalAttnAV`, `MetalAttnAVMm`, the tensor `MetalAttnAVMmTensorT`) write only f32 `xb`;
  wo's X half twin comes from `pf_cvt_panel` at the `bxh_av` site. An HX store beside the
  simdgroup/tensor stores kills that pass. Caveat: `q_gated` models rewrite `bxb` through
  `enc_sigmul` AFTER attention - there the twin must come from sigmul (give it an `_hx`),
  or the site keeps the cvt. Prize: ~0.3% pp on M5, larger on M1/M4-class (same bytes,
  a third of the bandwidth). Three kernel variants + the gated ordering = half a day.
- **4b. Per-model cvt arms** still on `pf_cvt_panel`: the deltanet out (`bdno`), the MoE
  hidden (`bxh_mg`) and shexp, PLE gather/project (gemma4e), the embedder/cat legs. Same
  `_hx` pattern where the producer is ours; size per model class before building.
- **4c. THE BIG RUNG - f16 activation planes end-to-end for prefill.** The remaining ~1 ms/
  prefill vs llama.cpp is the f32 elementwise traffic itself (their graph moves half the
  bytes through every norm/add/act pass and needs no cvt at all). Touches plane formats,
  every elementwise kernel, attention, the CPU-fallback paths, logits/readback - and
  re-opens the numerics bars across the parity and prefill suites. ITS OWN ARC, planned;
  the acceptance bar is the existing parity suites plus a pp/tg board A/B per class.

## 6. The Metal kernel twins - the census for the Mac session (Boris, 2026-09-07)

Every `[metal_dispatch]` class has been read and the suspected twins compared line by line;
this entry is the map so the research is not done twice.
Boris: the Mac kernels are a follow-up done on the Mac, together with the Metal side of the Qwen
zoo. Facts that decide the order:

- 296 dispatch classes in `dasllama_metal_kernels.das` (175) + `dasllama_metal_prefill.das`
  (121): 173 are template stamps, 14 ride a `def abstract` base, ~44 are hand-written twins
  (~3000 lines addressable), ~65 singletons. The near-miss pairs that stay apart are the ones
  a document rules: the float/quant attention pairs (two binding layouts,
  `REVIEW_GPU_KERNEL_CLASSES.md`); the f4-slab GEMV crowns `MetalKqGemvIq3sF4` / `Iq3xxsF4` /
  `Iq2xxsF4` and `MetalKqGemvK5C` beside `K5T` (per-box crowns with their numbers,
  `ARCHITECTURE_GPU_QUANT_PLANES.md` sec.2.2z); the `*Db` double-buffered shells beside the
  single-tile shells (`ARCHITECTURE_GPU_PREFILL.md` sec.2.2c carries the measurement); the
  compact-kargs and unread-bind asymmetries (`ARCHITECTURE_GPU.md` sec.1.5); the codebook grid
  tables twinned in the Vulkan home (sec.1.5 role table); and the scalar attention trio beside
  the tensor QK/AV pair (`ARCHITECTURE_GPU_PREFILL.md` sec.2.2f).
- Two thirds of the debt is one family: the per-format GEMV, MvB2/B4 and MvB8 copies - 36
  classes over 12 formats (`MetalKqGemv*` 7985-9319, `MetalKqMv*T` 8368-10970, `MetalKqMvB8*`
  8477-11102) where iq4xs and iq4nl differ in ONE line and iq2s and iq2xs in eight; the shells
  are byte-identical. One format-abstract shell per family with one `def abstract` decode per
  format (`stage_w16` / `dot_block`) closes ~1930 lines; `MetalKqGemvIq2xxsFam` and
  `MetalKqMulMmSplitTensorBase.stage16` already prove the shape in-tree. Keep the per-box crowns
  (`K5C`, `Iq3sF4`, `Iq3xxsF4`) as separate stamps; the F4 forks bundled a second change
  (`first_row * 2u` -> `* 4u`, `sumf[2]` -> `[4]`, `ib += 4u` -> `2u`) = two constants, F4 + ROWS;
  the `TILED` arm duplicates the b-loop for a measured +2% (k4) - prove the generated MSL
  byte-identical per stamp before and after.
- Then: the SqAttn `BATCHED` axis (10 templates, ~250 lines; `MetalSqAttnCombT` ships the exact
  pattern); the `MetalKqMulMmK45T` 12-bool `static_if` ladder into the `stage16` scaffold plus
  the tensor K45/K6 x Db pair (~285; removes the coupled-bool trap where `MetalKqMulMmIq4nl`
  must set `IQ4XS` and `IQ4NL`; the Db forms sit on the sanctioned float-A list); the four dense
  mul_mm shells onto a `MetalMoeMulMmBase` twin (~145); the MoE GEMV `GATHERED` axis (~230, the
  `float4` x view stays its own axis - a measured 2.25x); the singles (DequantK6H; the bias
  pair that folds is `MetalAddBiasRows` with `MetalBiasGeluLut` - field for field at 0-3, the
  map and `x`'s offset apart - while `MetalBiasAddRes`'s residual plane at 1 keeps it out). Two
  classes fold only when their (binding number -> field type, `@off`) maps agree: the
  SqAttn single/batched pairs (the layer slab through `@off`, the kargs at 4 vs 5 under `rt`),
  the rope-store single/batched pairs (the single form's raw-V buffer at 1 shifts every later
  binding) and the RmsNorm/AddRms pair (the residual at 1 shifts five) stay apart on that rule;
  `MetalSqAttnCombT` folds because both its stamps share one kargs struct at one binding. The
  Q8MvB2/B4 pair stays apart on the emitter: an NR-wide stamp needs a local fixed array of
  pointers, which has no MSL form.
- Rules for every conversion: a stamp's `tgmem=` string is `<LeafClass>_<method>_msl_tgmem`, so a
  hand class becoming a stamp keeps its names by keeping its named `[metal_kernel(name=..)]`
  def on the leaf and moving only the shared body to the template; a leaf that inherits a
  `[metal_kernel]` method AND declares its own emits BOTH kernels (a second
  `<Leaf>_<method>_msl` global), so a chain splits the shared decode from the kernel-carrying
  levels; an instance may add fields (the `MetalKqDequant<Fmt>` stamps bind their own `@ssbo`);
  a `@template_gate`d field may be named only inside a `static_if` arm on its own axis (a ternary
  infers both arms), and on a class template it declares no `@role = "read"`/`"write"` - the
  census cross-checks declared roles against the unreified body and reads the gated field as
  unused, while an undeclared role derives the same value; a method used in value position is
  an arrow-form single return, so a multi-statement format hook hands its value back through
  a `var T&` parameter (the emitter's carve-out to the return-the-result rule); a per-stamp `@off`
  asymmetry on a shared binding rides a `@template_gate`d field pair at that binding plus one
  `static_if` accessor arm; a ROWS-style axis that stages per-row values in fixed arrays is not
  AIR-neutral (Metal's -O2 does not scalar-replace them once a stamp has two rows - the k4 tile,
  `followup_general.md` item 96); the kernel-unit gates (`tests/test_metal_gemv_kernels.das`,
  `test_metal_gemm_kernels.das`) are the parity lock per format - green before and after, on the
  M1 first, the M5 pass after.
- Detect-dupe (`utils/detect-dupe`) over the two files finds the exact-clone shells and the
  copied helpers; it does not see a twin whose bodies differ by a guard or an accumulator
  count, which is why the census reads the bodies.

## 5. The dense-KQ tensor twins' missing stamps

The nine iquant/split-scale tensor mul_mm twins stamp `T` and `TH` only. k4/k5/k6 additionally
carry the tall (`TH128`) and double-buffered (`THDb`, `THDb128`) stamps, and that is where the
tall in-kernel-dequant win lives (`ARCHITECTURE_GPU_PREFILL.md` sec.2.2c form 1). Race a tall
stamp for the iquant scaffold before assuming the k6 result transfers; the arc-end matrix's
soft spot is the deep-K w2 column (k6 0.91x / iq3s 0.88x / iq3xxs 0.92x vs llama.cpp on m5),
which is exactly the column a tall stamp serves.

## 7. The Qwen Metal sweep - every Qwen text file the M5 stocks, one at a time

The Vulkan tier went through every Qwen the zen2 box stocks; Metal never had that pass. This
is it: smallest file first, one file per run, each file gating on two things before the next
starts - the counting fixture matches (the parity pregate, 8/8 opening tokens) and both pp512
and tg128 read at or above 0.9 of llama.cpp. A file under either bar stops the sweep until it
is fixed; the fix lands as its own row above or its own PR, and the file re-runs. A Vulkan fix
for the same model or format is the first place to look.

Provenance, every row: the M5 Max, `performance/gen_bench_records.das -- -w llm --legs metal
--catalog all --jit --das-settle 10 --store <scratch>` with `DASLLAMA_ALLOW_UNTUNED=1` - the
das cell is the bench script under `-jit` with `--for-debug-purposes` (debug-jit: the Metal
kernels are the emitted stamps with the box profile's crowns, the CPU tune kernels run the
arm-i8mm class profile), 5 reps, `-p 512 -n 128`, `--ngl 99`; the reference is the stock
llama-bench at the ref pin (b10659), `-ngl 99`, the same shapes and reps, from a 10 s cool
slot. Rates are tok/s, mean over the reps with the rig's own stddev; the ratio is das over
llama.cpp. The scratch store is not in the tree; the rows here are the record.

| file | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| Qwen2.5-0.5B-Instruct-Q8_0 | 31069 +- 169 | 40459 +- 362 | 1.30 | 358 +- 47 | 490 +- 18 | 1.37 | 8/8 |
| Qwen3.5-0.8B-Q4_K_M | 16590 +- 35 | 23025 +- 60 | 1.39 | 337 +- 21 | 443 +- 3 | 1.31 | 8/8 |
| Qwen3.5-0.8B-Q4_K_M-q8out | 16653 +- 29 | 23049 +- 66 | 1.38 | 273.7 +- 8.2 | 443.5 +- 3.4 | 1.62 | 40/40 |
| Qwen3-0.6B-Q8_0 | 24669 +- 87 | 32194 +- 692 | 1.31 | 317.2 +- 41.0 | 441.2 +- 17.3 | 1.39 | 40/40 |
| Qwen2.5-1.5B-Instruct-IQ3_XS | 11559 | 15820 | 1.37 | 280.8 | 354.5 | 1.26 | 40/40 |
| Qwen3.5-0.8B-Q8_0 | 16921 +- 27 | 23060 +- 57 | 1.36 | 259.7 +- 13.9 | 396.6 +- 1.5 | 1.53 | 40/40 |
| Qwen2.5-1.5B-Instruct-Q8_0 | 11844 +- 28 | 14958 +- 24 | 1.26 | 209.0 +- 3.9 | 249.8 +- 10.2 | 1.20 | 40/40 |
| Qwen3-4B-Instruct-2507-Q4_K_M | 5067 +- 2 | 6643 +- 30 | 1.31 | 159.6 +- 2.4 | 164.0 +- 6.9 | 1.03 | 40/40 |
| Qwen3.5-4B-Q4_K_M | 4157 +- 2 | 5504 +- 8 | 1.32 | 127.9 +- 0.4 | 138.2 +- 0.3 | 1.08 | 40/40 |
| Qwen3-4B-Instruct-2507-Q5_K_M | 4899 +- 5 | 6636 +- 32 | 1.35 | 143.0 +- 0.5 | 138.8 +- 5.8 | 0.97 | 40/40 |
| Qwen3.5-4B-Q5_K_M | 4005 +- 5 | 5505 +- 7 | 1.37 | 117.5 +- 0.7 | 126.1 +- 0.2 | 1.07 | 40/40 |
| Qwen3-4B-Instruct-2507-Q6_K | 5091 +- 8 | 6648 +- 27 | 1.31 | 130.3 +- 0.4 | 133.2 +- 5.7 | 1.02 | 40/40 |
| Qwen3.5-4B-Q6_K | 4114 +- 8 | 5493 +- 35 | 1.34 | 108.4 +- 0.4 | 122.3 +- 0.2 | 1.13 | 40/40 |
| Qwen3-4B-Instruct-2507-Q8_0 | 5192 +- 10 | 6262 +- 27 | 1.21 | 108.5 +- 0.4 | 111.4 +- 4.6 | 1.03 | 40/40 |
| Qwen3.5-4B-Q8_0 | 4338 +- 7 | 5175 +- 41 | 1.19 | 93.0 +- 0.1 | 107.3 +- 0.1 | 1.15 | 40/40 |
| Qwen3.5-9B-IQ4_XS | 2496 +- 1 | 2490 +- 6 | 1.00 | 86.1 +- 0.1 | 85.4 +- 0.1 | 0.99 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local | 5486 +- 21 | 6328 +- 51 | 1.15 | 200.7 +- 1.3 | 207.6 +- 0.3 | 1.03 | 40/40 |
| Qwen3.5-9B-Q8_0 | 2565 +- 4 | 3087 +- 18 | 1.20 | 57.0 +- 0.4 | 62.3 +- 0.1 | 1.09 | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS | 3695 | 3934 | 1.06 | 139.0 | 168.9 | 1.22 | 40/40 |
| Qwen3.5-35B-A3B-UD-IQ2_XXS | 3307 | 3692 | 1.12 | 110.9 | 144.4 | 1.30 | 40/40 |
| Qwen3.6-35B-A3B-UD-IQ2_XXS | 3299 | 3681 | 1.12 | 109.2 | 141.3 | 1.29 | 40/40 |
| Qwen3.8-27B.i1-IQ3_S | 699 +- 14 | 882 +- 6 | 1.26 | 28.2 +- 3.3 | 28.9 +- 0.8 | 1.02 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat.Q6_K | 5069 +- 40 | 5936 +- 355 | 1.17 | 164.0 +- 2.3 | 163.5 +- 4.1 | 1.00 | 40/40 |
| Qwen3.8-27B-UD-Q3_K_XL | 756 | 782 | 1.03 | 23.6 +- 1.3 | 28.8 +- 0.9 | 1.22 | 40/40 |
| Qwen3.5-35B-A3B-UD-IQ3_S | 3270 | 3269 | 1.00 | 105.0 | 131.0 | 1.25 | 40/40 |
| Qwen3.6-35B-A3B-UD-IQ3_S | 3262 | 3285 | 1.01 | 104.7 | 129.8 | 1.24 | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-UD-Q3_K_XL | 3187 | 3602 | 1.13 | 139.0 | 166.1 | 1.19 | 40/40 |
| Qwen3-30B-A3B-Thinking-2507-UD-Q3_K_XL | 3227 | 4027 | 1.25 | 137.7 | 165.8 | 1.20 | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-UD-Q3_K_XL | 3205 | 4009 | 1.25 | 138.1 | 166.0 | 1.20 | 40/40 |
| Qwen3.8-27B-UD-IQ4_XS | 493 +- 19 | 754 +- 7 | 1.53 | 22.1 +- 0.7 | 27.8 +- 1.0 | 1.26 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat.Q8_0 | 5348 +- 44 | 6004 +- 34 | 1.12 | 157.7 +- 2.0 | 179.7 +- 0.2 | 1.14 | 40/40 |
| Qwen3-8B-BF16 (llama.cpp bf16, das q8-requant: no ratio) | 2734 +- 10 | 3496 +- 10 | - | 34.9 +- 0.1 | 65.9 +- 0.1 | - | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-Q4_K_M (board row: `performance/records/m5.json`) | 3414 +- 13 | 3761 +- 13 | 1.10 | 136.3 +- 1.2 | 164.6 +- 0.2 | 1.21 | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-Q4_K_M | 3387 | 3669 | 1.08 | 137.1 | 165.3 | 1.21 | 40/40 |
| Qwen3.8-27B-Q4_K_M (board row: `performance/records/m5.json`) | 752 | 906 +- 6 | 1.20 | 25.3 +- 0.4 | 28.0 | 1.11 | 40/40 |
| Qwen3.6-35B-A3B-UD-Q4_K_M | 3173 +- 15 | 3426 +- 9 | 1.08 | 95.7 +- 0.8 | 127.5 +- 0.2 | 1.33 | 40/40 |
| Qwen3-30B-A3B-Q8_0 | 3392 +- 23 | 3515 +- 21 | 1.04 | 104.8 +- 0.5 | 122.1 | 1.16 | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-Q8_0 | 3334 | 3381 | 1.01 | 103.5 | 122.3 | 1.18 | 40/40 |
| Qwen3.6-35B-A3B-Q8_0 | 3081 +- 15 | 3225 +- 12 | 1.05 | 89.1 +- 0.3 | 121.1 +- 0.3 | 1.36 | 40/40 |
| Qwen3-Coder-Next-Q4_K_M | 2091 | 2160 | 1.03 | 82.6 | 112.5 | 1.36 | 40/40 |

The fixture column past the first two rows is the coherence gate: 40 greedy tokens of the
counting fixture on Metal, token-for-token against llama.cpp's own greedy on the same file
(`harness/parity.sh` with `NGL=99` and the box manifest, so the crowned route serves). The
nine routed rows - the six iquant / q3_K expert files and Coder-Next - first declined
(no expert kernel for their formats; the 256-expert select cap); their rows are the re-times
after the split-format expert twins and the 16-deep select landed. The IQ2_XXS and Q3_K_XL
files first re-timed 0.59-0.95 with the twins' grid taking the padded panel's rows as tiles.
The 27B i1-IQ3_S row is the re-time after its tall stamp landed: before it the file read 644
against 743 (0.87), the sweep's first stop. The 27B UD-IQ4_XS first read 349 / 16.6 behind a
run of hot 30B cells (the rig kept a throttled warm re-run, 0.47x of its cold first run); the row
is its cool re-time, and the rig now keeps the cold run when the warm one reads slower.

The Llama-3.2-1B format matrix, the same rig and gate on the one small dense model (the
`-local` files are requants of the bartowski Q8_0; the i1-IQ3_XXS file mixes IQ2_S attention
with IQ3_XXS, IQ3_S, Q4_K and Q5_K planes, so its row exercises four passes at once):

| file | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| Llama-3.2-1B-Instruct-IQ2_XXS-local | 17683 | 21723 | 1.23 | 479.4 | 520.8 | 1.09 | 40/40 |
| Llama-3.2-1B-Instruct-IQ2_XS-local | 17636 | 21708 | 1.23 | 380.2 | 514.3 | 1.35 | 40/40 |
| Llama-3.2-1B-Instruct.i1-IQ3_XXS | 17518 | 21704 | 1.24 | 373.0 | 493.3 | 1.32 | 40/40 |
| Llama-3.2-1B-Instruct-IQ3_XXS-local | 17714 | 21737 | 1.23 | 405.9 | 498.3 | 1.23 | 40/40 |
| Llama-3.2-1B-Instruct-Q2_K-local | 17832 | 21644 | 1.21 | 395.7 | 484.4 | 1.22 | 40/40 |
| Llama-3.2-1B-Instruct-IQ3_M | 16644 | 21626 | 1.30 | 252.2 | 461.8 | 1.83 | 40/40 |
| Llama-3.2-1B-Instruct-Q3_K_L | 16545 | 19693 | 1.19 | 350.1 | 369.0 | 1.05 | 40/40 |
| Llama-3.2-1B-Instruct-IQ4_XS | 17299 | 20939 | 1.21 | 407.0 | 398.6 | 0.98 | 40/40 |
| Llama-3.2-1B-Instruct-Q4_0-local | 18352 | 21623 | 1.18 | 370.0 | 449.0 | 1.21 | 40/40 |
| Llama-3.2-1B-Instruct-IQ4_NL-local | 17466 | 20949 | 1.20 | 398.4 | 362.6 | 0.91 | 40/40 |
| Llama-3.2-1B-Instruct-Q4_K_M | 17000 | 21194 | 1.25 | 418.1 | 399.4 | 0.96 | 40/40 |
| Llama-3.2-1B-Instruct-Q5_K_M | 16570 | 21640 | 1.31 | 339.2 | 384.6 | 1.13 | 40/40 |
| Llama-3.2-1B-Instruct-Q8_0 | 17729 | 21723 | 1.23 | 299.6 | 319.4 | 1.07 | 40/40 |

The three iquant rows are re-times: the sweep first read them 0.91, 0.93 and 0.85 (the
IQ3_XXS stop), every split-scale site on the staged mul_mm stamp that re-dequantizes W per
32-row tile, while the K-quant twin rode the image's baked dev-W panels. The nine split-scale
formats now carry a dev-W dequant pass on their tensor class and bake into the image's
`devwf16` plane (IMAGE_VERSION 37, `ARCHITECTURE_GPU_PREFILL.md` sec.2.2aa): 1.05 on the
scratch route, the rows above resident. The IQ2_XS file skips "19" on llama.cpp too; its
fixture is frozen off that file. The 40-token tg128 reads on a 1B move +-10% between cool
slots (the IQ2_XXS 1.09 / 1.26 / 1.30 across three re-times); the pp512 reads hold to 1%.

Gaps the sweep found, in order of what they block:
- Q4_0 had no Metal kernel at all, so a Q4_0 file served nothing on Metal: closed in this arc
  as the `Q40` flag on iq4nl's five stamps (the linear nibble map in the codebook slot), its
  dev-W pass and bake riding the class; the row above is the file's first Metal reading. The
  first read was 0.36 on prefill: the `-jit` rig child reads the bench sidecar beside the
  script, and the new crown was in the performance manifest only - both carry it now.
- The 1B decode sits at parity on the 4-bit dense formats (IQ4_NL 0.91, Q4_K_M 0.96, IQ4_XS
  0.98) while the 2- and 3-bit files decode 1.2-1.8 ahead and the Qwen 0.5B/0.6B decode 1.3-1.4:
  the 4-bit GEMVs against the 1B's widths (2048 / 8192, 32 heads of 64, 8 KV), the GEMV lab.
- The Metal routed block had no expert kernel for the iquant formats or q3_K (eight files
  declined): closed in this arc as the split-format expert twins on the dense split classes
  and the decode GEMVs on the site template; every stocked MoE file serves at 1.00-1.25 on
  prefill and 1.19-1.30 on decode. The 35B UD-IQ3_S pair sits on the line (1.00 / 1.01): the
  iq3s stage16 is the slowest of the set in the isolated expert-stamp race (1.03 of k4's).
- The iquant and q3_K tensor twins other than iq3s and iq3xxs still carry no tall stamp: q3_K
  prefill reads 1.03 on the 27B, IQ4_XS 1.00 on the 9B. The same one-stamp fold as the iq3s
  one, per format, raced on the file that reads lowest. The 1.5B IQ3_XS first read 0.90 on
  the staged stamps; its row is the re-time on the baked dev-W panels.
- The dense 4B decode sits at parity across Q4_K, Q5_K, Q6_K and Q8_0 (0.97-1.03): a GEMV
  shape gap on the 4B's widths, not a format's - the GEMV lab against the 4B's shapes.
- Qwen3-Coder-Next (qwen3next: 512 experts, top-10, expert width 512) declined on the select
  kernel's 256-expert cap: closed with the 16-deep select stamp. Its first read was 0.98 on
  prefill: at ten rows per expert the padded bucket panel is three quarters padding, and the
  routed block's activation and the down site's convert ran over the CPU's padded bound. The
  activation now writes the f16 twin itself (the dense HX form; no convert pass) and guards on
  the bucket rail's own padded total (basep[ne]); the row is the re-time, 1.03.
- Both Qwen3-Coder-30B files declined on a dense FFN width no layer dispatches (the file's
  `feed_forward_length` 5472, off the 64 lattice, on a fully routed model): fixed in this arc
  (`has_dense_ffn` / `mm_tile_widths_64_ok`), the rows above are their re-time.
- The q8 MoE expert prefill sits at parity on every q8 MoE file (30B 1.04, Coder 1.01, 35B
  1.05) while the same files decode 1.16-1.36 ahead and their q4_K twins prefill 1.08-1.10:
  the q8 expert mul_mm stamps (`MetalMoeMulMmQ8TensorT`), the family whose double-buffer
  race measured a loss - the GEMM lab against the expert shapes. On the M4 Pro, where the
  `moe_mulmm_q8` crown lost and the simdgroup base serves, Qwen1.5-MoE-A2.7B Q8 reads 0.96
  (1222 against 1273 t/s; decode 1.09) while its Q6_K sibling reads 1.00 and the k4 routed
  files 1.02-1.05 - the same q8 expert family, the base form this time.

### 7.1 The M4 Pro pass - the older-hardware check

The same driver on the M4 Pro (14-core, 64 GB, macOS 26.5; the same llama.cpp ref pin, its own
mint `performance/m4.tune.json`, the parity oracle built beside `build-stock`; the rig, flags and
debug-jit provenance are sec.7's). The mint raced
every mul_mm tensor twin and all thirteen kq formats, q8, the MoE and attention pairs LOST to
the simdgroup base by 4-10% (the M4 GPU has no neural accelerators; llama.cpp disables its own
tensor API below M5): only the kq_rows, kq_mvb2_k4_r2 and mulmm_bf16 crowns stand. Bar for the
box: 0.99 on both axes. The Llama-3.2-1B matrix first read pp 0.93 on every iquant file and
0.94-0.98 on the rest, tg 0.92-0.97 on every format but Q2_K / Q3_K_L / IQ3_M; the Qwen files
0.73 (0.5B Q8) and 0.80 (0.6B Q8) on decode. Two mechanisms, both landed:

- **Decode: the q8 GEMV starves at short reductions.** The reduction-split MetalQ8Gemv (2 rows
  per threadgroup, the n-walk across 4 simdgroups) at n=896 gives each thread one block and pays
  two barriers per row pair: 34-74 GB/s on the qkv / wo / w13 sites, 147 on the classifier,
  against the part's 267 GB/s. The row-per-simdgroup form (`MetalQ8GemvSg`, `MetalQ8GemvW13SwSg`)
  streams 267 on the classifier and 1.5-1.7x the split on the narrow sites; the split form is back
  ahead from n=2048 at classifier width on both boxes, so `GEMV_SG_MAX_N = 1536` picks per site.
  0.5B Q8 decode 4.2 -> 3.2 ms per token in the step trace; the rig rows below.
- **Prefill: dev-W compiled only behind the tensor crown.** The dequant-once panel path's win is
  the one-time dequant, not the tensor lane, yet its passes compiled only where the format's
  tensor stamp had won the dense race - never on this box. They now compile behind a toolchain
  probe (`g_pf_tensor_ok`), as do the split-format MoE twins, which are the only Metal path for
  the iquant / q3_K expert files (0.95 / 1.09 here against a CPU decline before).

| file (M4 Pro, every mechanism below landed) | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| Llama-3.2-1B-Instruct-IQ2_XXS-local | 3264 | 3451 | 1.06 | 252.8 | 256.2 | 1.01 | 40/40 |
| Llama-3.2-1B-Instruct-IQ2_XS-local | 3260 | 3452 | 1.06 | 245.1 | 255.8 | 1.04 | 40/40 |
| Llama-3.2-1B-Instruct.i1-IQ3_XXS | 3251 | 3453 | 1.06 | 238.6 | 245.4 | 1.03 | 40/40 |
| Llama-3.2-1B-Instruct-IQ3_XXS-local | 3256 | 3451 | 1.06 | 239.8 | 246.1 | 1.03 | 40/40 |
| Llama-3.2-1B-Instruct-Q2_K-local | 3132 | 3452 | 1.10 | 227.0 | 255.6 | 1.13 | 40/40 |
| Llama-3.2-1B-Instruct-IQ3_M | 3214 | 3450 | 1.07 | 232.5 | 252.2 | 1.08 | 40/40 |
| Llama-3.2-1B-Instruct-Q3_K_L | 2978 | 3452 | 1.16 | 176.2 | 194.4 | 1.10 | 40/40 |
| Llama-3.2-1B-Instruct-IQ4_XS | 3265 | 3451 | 1.06 | 246.2 | 249.6 | 1.01 | 40/40 |
| Llama-3.2-1B-Instruct-Q4_0-local | 3363 | 3452 | 1.03 | 243.8 | 253.9 | 1.04 | 40/40 |
| Llama-3.2-1B-Instruct-IQ4_NL-local | 3296 | 3452 | 1.05 | 243.2 | 247.8 | 1.02 | 40/40 |
| Llama-3.2-1B-Instruct-Q4_K_M | 3195 | 3451 | 1.08 | 243.4 | 242.5 | 1.00 | 40/40 |
| Llama-3.2-1B-Instruct-Q5_K_M | 2954 | 3452 | 1.17 | 192.8 | 190.8 | 0.99 | 40/40 |
| Llama-3.2-1B-Instruct-Q8_0 | 3326 | 3500 | 1.05 | 167.3 | 166.7 | 1.00 | 40/40 |

Before the mechanisms the same thirteen read pp 0.93-1.02 and tg 0.92-1.05 (IQ2_XXS 0.93 /
0.94, Q4_K_M 0.97 / 0.93, Q8 0.98 / 0.95). The Qwen text files the box held, the same rig and
gate, every mechanism landed; two rows sit under the 0.98 bar and are ledgered (sec.10 and
sec.8), the sweep continued past them:

| file (M4 Pro; board row = `performance/records/m4.json`) | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| Qwen2.5-0.5B-Instruct-Q8_0 | 7798 | 7923 | 1.02 | 296.6 | 301.0 | 1.02 | 40/40 |
| Qwen3.5-0.8B-Q4_K_M | 4313 | 5066 | 1.17 | 201.9 | 270.5 | 1.34 | 40/40 |
| Qwen3.5-0.8B-Q4_K_M-q8out | 4326 | 5067 | 1.17 | 201.1 | 270.5 | 1.35 | 40/40 |
| Qwen3-0.6B-Q8_0 | 6029 | 6638 | 1.10 | 246.1 | 267.0 | 1.08 | 40/40 |
| Qwen2.5-1.5B-Instruct-IQ3_XS | 2315 | 2490 | 1.08 | 163.8 | 184.2 | 1.12 | 40/40 |
| Qwen3.5-0.8B-Q8_0 | 4463 | 5030 | 1.13 | 170.9 | 227.2 | 1.33 | 40/40 |
| Qwen2.5-1.5B-Instruct-Q8_0 | 2381 | 2457 | 1.03 | 125.7 | 129.3 | 1.03 | 40/40 |
| Qwen3-4B-Instruct-2507-Q4_K_M | 815 | 939 | 1.15 | 81.8 | 84.7 | 1.04 | 40/40 |
| Qwen3.5-4B-Q4_K_M | 756 | 851 | 1.13 | 61.1 | 69.3 | 1.13 | 40/40 |
| Qwen3-4B-Instruct-2507-Q5_K_M (sec.10) | 776 | 938 | 1.21 | 59.2 | 57.1 | 0.96 | 40/40 |
| Qwen3.5-4B-Q5_K_M | 727 | 872 | 1.20 | 51.4 | 55.0 | 1.07 | 40/40 |
| Qwen3-4B-Instruct-2507-Q6_K | 827 | 939 | 1.14 | 65.6 | 67.5 | 1.03 | 40/40 |
| Qwen3.5-4B-Q6_K | 745 | 866 | 1.16 | 52.3 | 60.5 | 1.16 | 40/40 |
| Qwen3-4B-Instruct-2507-Q8_0 | 880 | 883 | 1.00 | 53.5 | 54.2 | 1.01 | 40/40 |
| Qwen3.5-4B-Q8_0 | 808 | 812 | 1.01 | 43.7 | 52.1 | 1.19 | 40/40 |
| Qwen3.5-9B-IQ4_XS | 428 | 442 | 1.03 | 38.7 | 41.4 | 1.07 | 40/40 |
| Qwen3.5-9B-Q8_0 | 447 | 442 | 0.99 | 26.4 | 29.0 | 1.10 | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS (sec.8) | 792 | 767 | 0.97 | 76.3 | 90.1 | 1.18 | 40/40 |
| Qwen3.5-35B-A3B-UD-IQ2_XXS | 735 | 743 | 1.01 | 55.3 | 76.4 | 1.38 | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-Q4_K_M (board row: `performance/records/m4.json`) | 780 | 799 | 1.02 | 76.3 | 93.9 | 1.23 | 40/40 |
| Qwen3.8-27B-Q4_K_M (board row: `performance/records/m4.json`) | 125 | 125 | 1.00 | 11.5 | 13.1 | 1.13 | 40/40 |
| Qwen3.6-35B-A3B-UD-Q4_K_M | 747 | 788 | 1.05 | 48.5 | 66.4 | 1.37 | 40/40 |

The eighteen files the box could not hold beside these ran staged - fetched, timed on the same
rig and gate, the staged copy deleted (three of the day's own fetches went the same way to make
Coder-Next's room); the two under the bar are the q8 expert prefill and the k4 expert GEMV at
Qwen1.5-MoE's 1408-wide experts (the q8 bullet above, sec.11), and the BF16 row has no
like-for-like pair (`followup_general.md` row 144):

| file (M4 Pro, staged) | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| Qwen3.6-35B-A3B-UD-IQ2_XXS | 733 | 747 | 1.02 | 55.1 | 75.0 | 1.36 | 40/40 |
| Qwen3.8-27B.i1-IQ3_S | 124 | 126 | 1.02 | 12.4 | 13.9 | 1.12 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat.Q6_K | 1209 | 1205 | 1.00 | 95.4 | 100.1 | 1.05 | 40/40 |
| Qwen3.8-27B-UD-Q3_K_XL | 126 | 128 | 1.01 | 12.9 | 13.5 | 1.05 | 40/40 |
| Qwen3.5-35B-A3B-UD-IQ3_S | 745 | 730 | 0.98 | 52.5 | 69.1 | 1.31 | 40/40 |
| Qwen3.6-35B-A3B-UD-IQ3_S | 741 | 739 | 1.00 | 52.6 | 68.8 | 1.31 | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-UD-Q3_K_XL | 738 | 773 | 1.05 | 68.4 | 81.2 | 1.19 | 40/40 |
| Qwen3-30B-A3B-Instruct-2507-UD-Q3_K_XL | 747 | 790 | 1.06 | 68.2 | 81.3 | 1.19 | 40/40 |
| Qwen3-30B-A3B-Thinking-2507-UD-Q3_K_XL | 749 | 794 | 1.06 | 68.2 | 81.3 | 1.19 | 40/40 |
| Qwen3.8-27B-UD-IQ4_XS | 125 | 127 | 1.01 | 12.9 | 13.1 | 1.02 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat.Q8_0 (q8 expert bullet) | 1273 | 1222 | 0.96 | 83.2 | 91.0 | 1.09 | 40/40 |
| Qwen1.5-MoE-A2.7B-Chat-Q4_K_M-local (sec.11) | 1227 | 1211 | 0.99 | 113.4 | 109.6 | 0.97 | 40/40 |
| Qwen3-8B-BF16 (llama.cpp bf16, das q8-requant: no ratio) | 493 | 471 | - | 16.8 | 30.8 | - | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-Q4_K_M | 770 | 791 | 1.03 | 77.3 | 94.2 | 1.22 | 40/40 |
| Qwen3-30B-A3B-Q8_0 | 809 | 815 | 1.01 | 55.9 | 65.6 | 1.17 | 40/40 |
| Qwen3-Coder-30B-A3B-Instruct-Q8_0 | 799 | 792 | 0.99 | 55.9 | 65.6 | 1.17 | 40/40 |
| Qwen3.6-35B-A3B-Q8_0 | 770 | 789 | 1.03 | 44.4 | 64.1 | 1.44 | 40/40 |
| Qwen3-Coder-Next-Q4_K_M | 491 | 502 | 1.02 | 42.6 | 62.5 | 1.47 | 40/40 |

The whole M4 pass: 53 files, 53 coherent, 47 at or above 0.98 on both axes, four under it and
ledgered (the k5 GEMV at the 4B widths, the routed IQ2_XXS 30B's tensor twins, the two
Qwen1.5-MoE expert kernels), one on the bar, one without a pair.

The third mechanism, found after the table's first fill: **the step wait wakes late.** On
Llama-1B Q8 the per-step trace put the GPU at 5.73 ms per token, under llama.cpp's 6.0, and
the wall at 6.4-6.7 - the GPU idled 3.3-3.6 ms before every third step with the next command
buffer already queued, the blocking `waitUntilCompleted` returning about a step late, a mode
that locks per process and that the M5 never enters; the bench process pays it on every
unchained step too (IQ2_XXS 235.7 against 256.5 t/s). A spin on the command buffer's GPU end
time removes it (5.66 ms per token, three runs alike; the M5 unchanged), shipped on by default
as `DASLLAMA_METAL_WAIT_SPIN` (`ARCHITECTURE_GPU_MTP.md` 2.38): decode 0.94 -> 1.03 (0.5B),
0.96 -> 1.03 (1.5B), 0.95 -> 1.00 (Llama Q8), 0.93 -> 1.00 (Llama Q4_K_M).

The fourth: **the 128-row tall stamps lose on this GPU** (Llama-1B 5%, the 30B 1%, the 9B
even) and open only behind the `hmm_tall` crown the mint races (`ARCHITECTURE_GPU_PREFILL.md`,
the occupancy-floor paragraph); the M5's sidecars carry the crown by hand until its next mint.
The table above is the family under all four.

Open on this box, to the 0.99 bar:
- The fused QKV GEMV (`MetalQ8GemvQkvRsT`) keeps the split reduce at every dim (~1% on the
  0.5B), and the n=2048 narrow-d band where the M4 still wants the row form and the M5 the
  split: sec.8 and sec.9.
- Dense q8 prefill 0.95-0.98 below dim 1536 (the mulmm base at these widths), the MoE
  split twins' prefill 0.95, and the remaining 20 Qwen files never timed here.

### 7.2 The gemma pass - both boxes

The same driver over every gemma file the two boxes stock (gemma-2, gemma-3, the gemma-4 dense
and E-series, the 26B-A4B MoE in five formats), sec.7's rig and bar (0.98 on both axes, the
40-token instructed counting fixture token-for-token against llama.cpp's own greedy). Four
mechanisms landed on the way:

- **The E-series decode built its PLE side input from nothing.** The hub's decode path consulted
  the prefill's device pre-step gate at one position; the Metal gate answered yes, the CPU pre-step
  was skipped, and the Metal decode copied the unfilled host rows - every E-series q8 file served
  garbage from the second token on both boxes. The gate is now two seats, one per direction
  (`register_ple_gpu_decode_gate`), and Metal registers the prefill one alone (sec.13 is the
  decode-side gather).
- **The k4/k5/q40 PLE token tables gather on device.** The prefill's GPU pre-step took q8 tables
  only; every other table fell to the serial CPU gather, and the E2B Q4_0 and Q4_K_M files
  prefilled at 0.49 and 0.51 of llama.cpp on the M5. `MetalPleGatherKqT` stamps the three formats
  off their native planes; the rows below read 1.11 and 1.18.
- **The dev-W bake left the expert stacks in.** gemma-4 splits its fused expert stacks per expert
  at load, every slice passed the bake's format-and-size predicate, and a 26B image carried 32 GB of
  panels the routed block never reads (47 GB, an eight-minute bake). The bake refuses expert
  slices (IMAGE_VERSION 38): the same image is 18 GB, its dev-W plane 3.1 GB over 205 dense sites,
  baked in under a second. Both readings are the metal-flavor mint of
  `gemma-4-26B-A4B-it-UD-IQ3_XXS.gguf` as the rig's cell logs it (`performance/gen_bench_records.das
  --legs metal --catalog all --jit`, the `dasLLAMA image:` lines): the 47 GB / 493 s mint on the
  M5 Max before the change, the 18 GB / 0.8 s mint on the M4 Pro after it.
- **The routed block serves q40 expert planes.** A Q4_0 26B declined `graph` on both boxes and ran
  the whole model on the CPU rails, though the dense q40 kernels had shipped: the routed ladders
  carried no q40 arm. q40 joins the split-format expert twins off the iq4xs template
  (`MetalMoeMulMmQ40T/TH/TH128/THR`) with its own expert GEMV (`MetalMoeGemvQ40`); the M5 row below
  reads 1.05 and 1.11.

| file (M5 Max; a board row is a file `performance/records/m5.json` covers, its cell an older commit's) | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| gemma-3-1b-it-Q8_0 | 17866 | 24256 | 1.36 | 288.4 | 327.3 | 1.13 | 40/40 |
| gemma-2-2b-it-Q4_K_M | 8578 | 11378 | 1.33 | 211.7 | 228.4 | 1.08 | 40/40 |
| gemma-2-2b-it-Q8_0 | 8757 | 11383 | 1.30 | 154.7 | 165.9 | 1.07 | 40/40 |
| gemma-4-E2B-it-Q4_0 | 7629 | 8452 | 1.11 | 177.4 | 215.0 | 1.21 | 40/40 |
| gemma-4-E2B-it-Q4_K_M | 7209 | 8516 | 1.18 | 175.7 | 219.1 | 1.25 | 40/40 |
| gemma-3-4b-it-Q8_0 | 5923 | 7103 | 1.20 | 107.9 | 114.5 | 1.06 | 40/40 |
| gemma-4-E2B-it-Q8_0 (board row) | 7545 | 9100 | 1.21 | 137.5 | 160.5 | 1.17 | 40/40 |
| gemma-4-E4B-it-Q8_0 (board row) | 4411 | 5330 | 1.21 | 81.7 | 90.4 | 1.11 | 40/40 |
| gemma-3-12b-it-Q4_K_M | 1762 | 2163 | 1.23 | 62.2 | 65.2 | 1.05 | 40/40 |
| gemma-4-12B-it-Q4_K_M (board row) | 1664 | 2086 | 1.25 | 59.4 | 64.0 | 1.08 | 40/40 |
| gemma-4-12b-it-Q5_K_M | 1635 | 2117 | 1.30 | 53.1 | 51.0 | 0.96 | 31/40 (a) |
| gemma-4-12b-it-Q6_K | 1690 | 2072 | 1.23 | 48.2 | 50.3 | 1.04 | 40/40 |
| gemma-4-12B-it-Q8_0 | 1763 | 2136 | 1.21 | 39.9 | 41.0 | 1.03 | 40/40 |
| gemma-4-26B-A4B-it-UD-IQ3_XXS | 3461 | 4009 | 1.16 | 108.7 | 129.4 | 1.19 | 40/40 |
| gemma-4-26B-A4B-it-UD-IQ4_XS | 3429 | 3856 | 1.12 | 99.4 | 110.8 | 1.12 | 40/40 |
| gemma-4-26B-A4B-it-Q4_K_M (board row) | 3467 | 3943 | 1.14 | 99.5 | 118.8 | 1.19 | 40/40 |
| gemma-4-26B-A4B-it-Q8_0 | 3344 | 3767 | 1.13 | 91.5 | 106.4 | 1.16 | 40/40 |
| gemma-4-26B-A4B-it-Q4_0 | 3796 | 3979 | 1.05 | 120.1 | 132.7 | 1.11 | 40/40 (b) |
| gemma-4-26B_q4_0-it (QAT) | - | - | - | - | - | - | CPU pregate red (c) |

| file (M4 Pro; board row = `performance/records/m4.json`) | pp512 llama.cpp | pp512 das | ratio | tg128 llama.cpp | tg128 das | ratio | fixture |
|---|---|---|---|---|---|---|---|
| gemma-3-1b-it-Q8_0 | 4248 | 4425 | 1.04 | 171.5 | 182.0 | 1.06 | 40/40 |
| gemma-2-2b-it-Q4_K_M | 1521 | 1691 | 1.11 | 114.9 | 118.8 | 1.03 | 40/40 |
| gemma-2-2b-it-Q8_0 | 1585 | 1688 | 1.06 | 80.4 | 81.6 | 1.01 | 40/40 |
| gemma-4-E2B-it-Q4_0 | 1572 | 1584 | 1.01 | 108.4 | 121.6 | 1.12 | 40/40 |
| gemma-4-E2B-it-Q4_K_M | 1494 | 1593 | 1.07 | 106.5 | 129.6 | 1.22 | 40/40 |
| gemma-3-4b-it-Q8_0 | 982 | 983 | 1.00 | 54.7 | 55.5 | 1.01 | 40/40 |
| gemma-4-E2B-it-Q8_0 (board row) | 1558 | 1541 | 0.99 | 78.6 | 86.3 | 1.10 | 40/40 |
| gemma-4-E4B-it-Q8_0 (board row) | 786 | 772 | 0.98 | 42.3 | 45.7 | 1.08 | 40/40 |
| gemma-4-12B-it-Q4_K_M (board row) | 276 | 290 | 1.05 | 29.4 | 30.7 | 1.04 | 40/40 |
| gemma-4-12b-it-Q5_K_M | 263 | 297 | 1.13 | 21.2 | 21.6 | 1.02 | 40/40 |
| gemma-4-12b-it-Q6_K | 279 | 297 | 1.07 | 22.4 | 23.8 | 1.06 | 40/40 |
| gemma-4-12B-it-Q8_0 | 298 | 297 | 1.00 | 18.8 | 19.2 | 1.02 | 40/40 |
| gemma-4-26B-A4B-it-UD-IQ3_XXS | 729 | 797 | 1.09 | 59.5 | 66.8 | 1.12 | 40/40 |
| gemma-4-26B-A4B-it-UD-IQ4_XS | 741 | 778 | 1.05 | 53.8 | 54.8 | 1.02 | 40/40 |
| gemma-4-26B-A4B-it-Q4_K_M (board row) | 731 | 818 | 1.12 | 53.9 | 60.4 | 1.12 | 40/40 |
| gemma-4-26B-A4B-it-Q4_0 | 758 | 790 | 1.04 | 68.9 | 70.3 | 1.02 | 40/40 (b) |
| gemma-4-26B_q4_0-it (QAT) | - | - | - | - | - | - | CPU pregate red (c) |

(a) The M5's decode sits at 0.96 on the k5 file - sec.10's gap at a third width (n=3840). Its
fixture flips at token 21 on a 0.03-logit near-tie the Metal chain lands on with no crown armed
(the k5 tensor twin holds the double-precision oracle at the production width - the kernel cells
now run kdim 3840); the token gate is no instrument for that row (`followup_general.md` row 151,
the fixture margin floor). (b) The routed block serves thirteen expert-plane formats, q40 among
them: the ggml-org Q4_0 26B carries q40 fused gate_up stacks beside q8 down stacks (a 704-wide
down row is no multiple of 256, so the loader demotes that plane) and runs the routed block on
both boxes through the q40 split twins and the q40 expert GEMV. (c) The Google QAT file is the ggml-org checkpoint's twin (its scale tensors are
byte-identical; only the token table's format and the Q4_0 rounding differ) and diverges on the
CPU kq-native rails at one token: a top-8 router pick at layer 29 on an 8e-5 margin, which the
arm64-sdot backend lands the other way - the CPU pregate refuses the file, so its rows stay
unmeasured (`followup_general.md` row 152 has the activation form behind the margin); on Metal,
whose GEMVs read the f32 activation, the same fixture holds 40/40 through the q40 expert twins.

## 8. The M4 Pro's routed iquant files prefill at 0.97 of llama.cpp

Qwen3-30B-A3B IQ2_XXS reads pp 0.97 (767 against 792 t/s) with decode 1.18, the M5 1.06 on the
same file (the sec.7.1 and sec.7 rows). The iquant and q3_K expert planes have no gathered
simdgroup kernel: their prefill is the split-format tensor twins alone
(`ARCHITECTURE_GPU_PREFILL_MOE.md`), and on this GPU every tensor stamp runs a few percent under
its simdgroup form (the dense race's verdict on all thirteen formats), so the routed file
inherits that few percent with nothing to fall back on. Unquirked: a simdgroup twin of the
split-format expert mul_mm for the routed sites, raced per box like the dense ones; the fused QKV
decode GEMV (`MetalQ8GemvQkvRsT`) keeping the reduction-split form at every dim is the remaining
decode item, worth ~1% on the 0.5B. The dense q8 prefill this row first carried closed with the
uncrowned q8 dev-W arm and the tall crown (0.5B 1.02, 1.5B 1.03, Llama Q8 1.05).

## 9. The n=2048 narrow-width GEMV band splits by box

At n=2048 and d=4096 the row form reads +28% on the M4 Pro and -9% on the M5 Max; at classifier
width the split form is ahead on both, so `GEMV_SG_MAX_N` stops at 1536 and Llama-1B's qkv / wo
sites (dim 2048) keep the split form on the M4. Unquirked: a `q8_gemv_sg_2k` decode crown raced
at kdim 2048 in `metal_tensor_race_decode`, extending the row form to 2048 where it wins.

## 10. The k5 decode GEMV sits 3-4% under llama.cpp at the Qwen3-4B widths on both boxes

Qwen3-4B-Instruct-2507-Q5_K_M reads tg 0.96 on the M4 Pro (57.1 against 59.2 t/s, the sec.7.1
row) and 0.97 on the M5 Max while its prefill reads 1.21 / 1.35; the Llama-1B Q5_K_M (dim 2048)
reads 0.99 and 1.05. The GEMV lab at the 4B shapes (dim 2560, qd 4096, hidden 9728, the vocab
plane) puts every k5 form - the production compact walk, the split-d walk, the two-row form and
the llama.cpp port - at 160-177 GB/s of the M4's 267, the compact form within 4% of the best on
every shape, so the k5 dot is at its ALU wall (`benchmarks/matmul/bench_metal_gemv_kernels.das`,
M4 Pro), and the kq_rows_k5 twin lost its race (0.86 against 0.99 ms). Unquirked: a new k5
spelling in the lab against llama.cpp's `mul_mv_q5_K` at these shapes (the unpack: the low
nibbles and the high-bit plane per 32-block), promoted through the kq_rows race; the sweep
continues past the file under this row. The gemma-4-12b Q5_K_M (dim 3840, hidden 15360) reads
0.96 on the M5 Max and 1.02 on the M4 Pro (sec.7.2): at that width the gap is the M5's alone.

## 11. The k4 expert GEMV reads 0.97 of llama.cpp on the M4 Pro at Qwen1.5-MoE's expert width

Qwen1.5-MoE-A2.7B-Chat Q4_K_M (60 experts, 4 routed, 1408-wide experts over dim 2048) decodes at
109.6 against 113.4 t/s on the M4 (the sec.7.1 staged row) while the M5 reads 1.03, and its
prefill reads 0.99; the 30B and 35B k4 routed files (ff 768 per expert, 8 of 128 routed) read
1.23-1.37 on decode there. The expert-indexed k4 GEMV (`MetalMoeGemvK4`,
`DASLLAMA_METAL_GEMV_TG` rows per threadgroup) at a 1408-wide expert is the shape the kq race lab
never carried. Unquirked: the MoE k4 GEMV in the lab at Qwen1.5-MoE's expert shape on the M4
against the dense k4 form and llama.cpp's `mul_mv_id`, the rows-per-threadgroup knob raced there.

## 12. The q8 GEMV form pick is one-dimensional and the crossover is not

`enc_q8_site_gemv` picks the row-per-simdgroup form on the reduction width alone
(`GEMV_SG_MAX_N`, 1536), while the measured crossover moves on two axes - the reduction width n
and the output width d. At n=896 the row form wins every projection shape on both boxes and loses
the M5 Max classifier (d=151936) by 6%, so that site takes the slower form there; the M4 Pro
wants the row form at n=2048 and d=4096 where the M5 wants the split (sec.9). Unquirked: a
two-axis pick over n and d, raced per box in `metal_tensor_race_decode` at both the projection
and the classifier widths. The bar is no loss on either box.

## 13. The Metal decode builds the E-series PLE side input on the CPU

The Metal decode reads the side input `ple_pre_decode` builds on the host - the token row's
gather plus the model_proj GEMV (E2B 1536x8960 bf16, E4B 27 MB) and its norm - between the step
wait and the next commit, while the Vulkan token command gathers the row and projects on device
(`register_ple_gpu_decode_gate`). Unquirked: a decode-side twin of the prefill's `MetalPleGatherQ8`
+ `pf_enc_bf16_mm` + `MetalPleFinish` chain at one position, registered as the Metal decode gate.
The bar is our own E-series tg128 rows of sec.7.2 not regressing on either box (no reference-build
run: the ratio to llama.cpp is sec.7.2's, re-taken through its rig), the served-pipeline fixture green.

## 14. The MoE expert mul_mm f32-X (`t`) form has no model-less cell in any format

`moe_split_twin` in `tests/test_metal_gemm_kernels.das` stamps `th`, `th128` and `thr` only, so
`moe_mulmm_split_gate` never dispatches the `t` stamp of any split format. That stamp is the arm
`pf_enc_moe_mm` takes when the prefill minted no half panel, which no stocked routed model on
either box reaches - leaving `MetalMoeMulMm<Fmt>T` covered by nothing. Unquirked: a
`MoeSplitForm.t` arm in `moe_split_twin` over every split format, dispatched by
`moe_mulmm_split_gate` off the f32 X panel against the same per-expert oracle its half-X sibling
uses. The bar is the eight `*T` entries of `CENSUS_NEVER_DISPATCHED` in
`tests/test_kernel_coverage.das` naming that arm as their coverage, the no-coverage note gone.

## 15. Two Metal review gates the M4 pass found the shape of

(a) A `REVIEW.das` check that reads every per-format dispatch ladder in
`dasllama/dasllama_metal_kernels.das` and `dasllama/dasllama_metal_prefill.das` (eleven today -
`enc_kq_gemv`, `enc_kq_mvb`, `enc_kq_gemm_mm_b`, `enc_moe_gemv`, `pf_kq_dq_pso`,
`pf_moe_split_pso`, `pf_moe_th_pso`, `pf_enc_kq_dq`, `pf_moe_split_enc`, plus the PLE pre-step's
pair `ple_gather_pso_of` and `pf_enc_ple_gather_fmt`, whose format sets must agree with each
other and with the gate's alignment arm) against the served-format predicates
(`kq_fmt_gpu_supported`, `moe_fmt_metal_served`, `moe_site_ok` in
`dasllama/dasllama_metal_shapes.das`; `pf_kq_split_fmt`, `pf_moe_split_fmt` in the prefill) and
reports an arm whose format no predicate serves - today the ladders' `panic` default arms are the
only catch, at serve time; the gate retires `REVIEW_GPU.md`'s two dispatch-ladder rules to
"weakening it is a defect". (b) A `tests/REVIEW.das` check over the hand-bound kernel-cell sites
(38 today) that pass a `*_tgmem` global to `metal_set_threadgroup_memory_length` with no
non-zero test: a stamp that gates its `@workgroup` state off makes the global read 0 and the
call throw, which the gate in `test_metal_gemv_kernels.das`'s `w13sw_gate` now guards by hand.
(c) A `REVIEW.das` check that `BLOB_SCALE_PLANES` (`dasllama/dasllama_load.das`) and the
`name == "..."` arms of `metal_blob_scale_plane` (`dasllama/dasllama_layout.das`) name one
roster: a format added to the ladder and not the roster loses the split-transform memo and is
never committed by `metal_blob_commit`.

## 16. Three model classes have no batched decode arm and step per row under the server

`batch_decode_decline` (`dasllama/dasllama_metal_decode.das`) declines `graph` for a
non-standard attention block - the deltanet hybrids, Qwen3.5 / 3.6 / 3.8 / Coder-Next - and for
a MoE with a shared expert - Qwen1.5-MoE, Qwen3.5-35B-A3B, Qwen3.6-35B-A3B, GLM-4.5-Air - and
`feature` for the per-layer-embedding E-series (gemma-4 E2B, E4B). Each such step falls to the
per-row single decode: every stream reads the weights once per token, so N streams cost N
weight passes where one batched step costs one. `REVIEW_GPU.md` rules a missing batched arm a
defect. The work, one arm per class: the deltanet step and gated Q batched over rows (the
recurrent state is per session, the GEMMs are not), the shared expert's triple as one batched
site beside the routed experts (the CPU batch stack already runs it), and the PLE side input
gathered per row into the batch step (item 13 is its CPU half).

## 17. `ksign7m` and Vulkan's `ksign7` are one function under two homes

`ksign7m` (`dasllama/dasllama_metal_kernels.das`) and `ksign7` (`dasllama/dasllama_vulkan_classes.das`)
are five identical lines of pure ALU - no table, no backend lowering. `ARCHITECTURE_GPU.md`
sec.1.5's role table keeps the codebook TABLES per kernel home; it does not reach a helper with
no table in it. The shared-grammar precedent is `dasllama_kernel_access.das`, one owner after two
private copies drifted. One home for the sign helper, both backends calling it.

## 18. `enc_kq_mvb` is a table wearing thirty-nine `elif`s

`enc_kq_mvb` (`dasllama/dasllama_metal_kernels.das`) walks thirteen formats by three arms each to
pick a builder triple, while the per-format doff/soff choice it repeats is already a table
(`KQ_ROWS_RACE_FORMATS` beside it). A `(fmt -> builder triple)` table with one arm body closes
the ladder; the sec.15(a) gate then reads one table instead of one ladder.

## 19. The batched and rows w13sw kernels have no activation axis

`MetalQ8GemvW13SwT.put_act` branches on the `act` uniform between `msl_silu_mul` and
`msl_geglu_mul`; the batched family (`enc_gemv_w13sw_b`, `enc_gemv_w13sw_rows`) binds no `act`
and computes SiLU only. Every live caller is SiLU today: the dense batch gates `fuse13` off
under geglu, the MTP verify declines non-silu, and gemma's shared expert is the
`moe_dense_shexp` form through `enc_geglu`, so no gelu architecture reaches the `u_moe_nsh`
path. A gelu MoE with a loaded `n_ff_shexp` would compute its shared expert with the wrong
activation; the fix is the `act` binding on the batched family, the same uniform the single-row
form takes.

## 20. The mv shells' per-format decode cannot cross a free-function boundary

The B2/B4 and B8 mv shells (`MetalKqMvShellT`, `MetalKqMvB8ShellT`) each carry a per-format
weight decode; for the five iquant formats the two copies are verbatim. One decode per format
needs either a common class ancestor that declares the format's planes - single inheritance
puts every format class under exactly one shell - or a free function taking the thread-local
`float4[8]` stage, and `msl_emit`'s `ufn_param` lowers every fixed-array parameter to
`threadgroup T*` (the fixed-array contract: only a `@workgroup` member). Two closes: the emitter
lowers a fixed-array parameter to `thread T*` when every argument at every call site is
thread-local (the cheaper path, and a lens diagnostic at the call site for the mixed case), or
the two shells merge into one template on a `B8` axis with the panel gated, so a format class
becomes the ancestor of both its stamps. ~200 lines behind either.
