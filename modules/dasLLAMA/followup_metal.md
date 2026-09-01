# Metal / mac-session follow-up - the M5 catch-up ledger

**Status: OPEN (2026-08-31, unquirk pass).** The ledger for the parallel mac session that
forks after PR-1 of `plans/unquirk_pass.md`. Metal-tier perf items move here from the
per-format notes during the Phase E doc split; the CPU items below are mac-session work too -
the M-series CPU tiers are minted and raced from that box.

## 1. The smmla (i8mm) leg - RACED on M5, NEON keeps the crown

The five `dot = "smmla"` seats of `q8q8_tile_gen` (mr4/mr8 x kstep/nrsplit/gkstep,
`requires = "i8mm"`) raced for the first time in the 2026-09-01 M5 box mint, post the
PR-1 detection fix (`g_target_arm64_i8mm` consults `cpu_supports`, `+i8mm` appended to the
target machine). The `harness/smmla_probe.das` gate passed first: correctness OK, 2.02x
register-resident MAC throughput over sdot4 - the nominal ceiling. In the tile race that
ceiling does not survive the memory traffic plus the kg8 re-layout: best smmla seat
`mr8_kstep2_nrsplit2` at 25893 us vs NEON `mr8_budget` at 21349 us (~21% behind; full table
in the m5 sidecar's race section and `~/.tune-history/m5/`). No `arm-i8mm` defaults export -
the export was conditional on a win.

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

Placeholder - the per-format Metal notes (tg 0.78-0.93x tails, the IQ4_XS lane-map gap of
followup_general #58, the Q22 dispatch-loop probe method) consolidate here in Phase E of
`plans/unquirk_pass.md`; until then they live in `HOW_TO_ADD_A_FORMAT.md`'s per-format notes.

## 4. The elementwise / activation-precision lane (the last M5 pp residual)

Attribution (M5, 1B iq2xxs, pp512 = 31 ms encode): mm 27.7 ms at measured tensor-twin rates,
attention 0.16 ms - the ~3 ms remainder is every non-matmul pass over the activation planes
(norms, residual adds, rope, swiglu, activation converts, glue). llama.cpp's slice: ~1.7 ms.

**What already exists** - the producer-fused f16 twin family (`_hx`): `pf_enc_rms_hx`,
`pf_enc_add_rms_bhx` (add+norm+half-emit in one), `enc_swiglu_hx`/`enc_geglu_hx`,
`enc_qk_rope_hx`. On the dense path these cover the norm and activation producers; the
standalone `enc_cvt_half` fires only through `pf_cvt_panel` fallbacks and at the sites below.
Measured ceiling of ALL remaining converts (`DASLLAMA_METAL_PREFILL_SKIP=act_cvt` knockout):
**+0.65% pp512 on M5** (15334 vs 15235 tok/s) - the fusion rung is mostly banked already.

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
