# dasLLAMA Performance Ledger

**Living document.** The perf backlog for the whole dasLLAMA engine — CPU, Metal, and the
audio/ASR towers: what each finding costs today, what the fix would change, and which
hypotheses were measured and refuted. (Extracted 2026-07-29 from `API_REWORK.md`, whose plan
half is archived under `history/dasLLAMA/`.)

Standing rule (Boris, 2026-07-01): any performance possibility spotted while doing wave work
gets a note HERE instead of being acted on mid-wave — the model waves optimize for correctness
and coverage; this ledger is the backlog for the perf pass that follows them. Every entry says
what it costs today and what the fix would change.

## Entries

- **gemma4v ViT tower (E-series): the CPU encode is MOST of the image turn — the q8 lane and
  the Metal leg are both owed (measured 2026-08-19, M1 Max, the gemma4v arc's slice F).** The
  tower is 16 blocks × 7 GEMMs (112 bf16 GEMMs, ≈151 MMAC per patch) over 1170 patches for a
  640×480 photo (→ 130 soft tokens). The image cell (`lcpp_bench --image`, gemma-4-E2B Q8_0
  off the image rail, debug-jit): **`img:enc` 1.93 s**, `img:pp` 337 tok/s over the 155-token
  spliced prompt (0.46 s), `img:tg` 42 tok/s — the encode is **≈61 % of a 32-token turn** on the
  CPU tier, against 0.7 % for gemma4uv. Why: the 177 GMAC run through `matmul_bf16_batch`'s
  per-row `dot_bf16` loop (≈92 GMAC/s aggregate, ~10× under the q8×q8 prefill kernels); the
  accelerate tier (`--accel`) did not move it (1.93 s → 1.93 s — the bf16 override is not
  engaging for these shapes, a separate finding). Prediction P2 (0.9–1.8 s, ≥12 % of the
  turn): the share held by a wide margin, the absolute landed just above the band. **Ruled:**
  the q8 tower lane (the gemma4a / parakeet recipe — Q8_0 GEMM planes + per-row requant,
  `tower_quantize`) is no longer declined for this tower, and the Metal leg
  (`register_gemma4v_gpu`) is on; each lands with its own parity gate. The E2B decoder's own
  Metal prefill is declined in `required` mode on this tree ("layers") independent of the
  image span — the gemma4v Metal leg would be the only GPU piece of an E2B image turn until
  that is served.
- **gemma4uv vision embedder: no quantized lane; the dlim rail is owed anyway (measured
  2026-08-14, M1 Max, the vision arc's slice G).** The embedder is two GEMMs (6912→3840 patch
  projection, 3840×3840 output projection). In a product-shaped turn — gemma-4-12B Q4_K_M
  served q8 off the image rail, a 640×480 photo → 130 soft tokens — the whole image side
  (geometry + letterbox + encode) is **under one percent of the turn** against the
  time-to-first-token and decode the cell reproduces (`img:enc` vs `img:pp`/`img:tg`). Encode alone is ~54 ms at 130 tokens
  (`img:enc`, the cell) and scales with the row count, so the 280-row geometry ceiling bounds
  what any image can cost — re-measure via the cell with a max-geometry input. **A quantized plane pair is DECLINED:** a second
  plane format cannot be worth it against 0.7%.
  ⚠ **Measurement provenance, because the first pass got it wrong twice:** the original
  numbers came from a one-off script that (1) took `load_model_`'s `QuantMode.fp32` default,
  dequantizing a K-quant 12B and reading 2 t/s instead of 14 — a 7× error — and (2) skipped
  `tune_gate()`, worth a further ~9%. Both defaults are stated automatically by the profiling
  rig, which is why model-level timing belongs there and not in a bench somebody writes. **The `.dlim`
  rail is NOT declined, and the timing above is the wrong instrument for it** — the rail is an
  ownership question, not a speed one. Every other model artifact loads mapped, with no
  allocation and no processing at load; the embedder's `blob : array<float>` is the second,
  array-owning shape `dasllama_audio.das` explicitly refuses to maintain. What it buys is RSS
  (mapped pages are shared and evictable across processes; a 190 MB owned bf16→f32 widening is
  neither) and zero load-time work — not the load wall. **Both are DONE (2026-08-14, the same arc's slices I and J).** The rail
  landed: the embedder stages, mints and maps like the audio towers, and the prepared image maps
  with no load-time work (mmap). Plane format settled as **per-GEMM, following each source tensor's on-disk type** —
  and the premise that the mmproj is uniformly bf16 was HALF WRONG: gemma-4's shipped "BF16"
  mmproj carries an **F32** patch embedder (26.5M params) and a **BF16** projection (14.7M), so a
  both-or-neither rule bought nothing. Per-GEMM gives 200 MB → 170 MB with tier-1 unmoved to the
  ulp (2.098e-05 before and after) — the bit-for-bit claim about `dot_bf16`'s widen, confirmed.
  Rounding the F32 half down would cost ~0.2% relative, two orders over the tier-1 gate, so it
  stays fp32. The turn is also a rig cell now (`lcpp_bench --image`: `img:enc` 53.8 ms,
  `img:pp` 63.5 tok/s, `img:tg` 14.3 tok/s on the M1 Max), so these numbers are re-measurable
  rather than quoted. **What reopens the quantized half:** a family whose vision side is a real
  ViT (the E-series `gemma4v` tower — 16 attention blocks, not two GEMMs), or batch image serving,
  where per-image cost stops hiding behind one prefill.

- **CPU attention: promote it from the loop-crown tier to the EMITTED tier (named 2026-08-05,
  the E2B deep-clip fade probes; corrected same day after a double-check).** Attention is NOT
  kernel-less: flash prefill (the default mode) tiles with online softmax and rides the
  per-box-tuned `gemm_f32_uk_4x16` for QK^T and A·V, with full f16/q8/tq4 KV codec arms.
  What it lacks is the EMITTED tier the weight GEMMs got: with f16 KV the flash path
  packs-and-converts K/V to f32 first and then runs the fp32 GEMM — no f16-native dots
  (F16C / fp16 hw-convert loads fused into the tile), no int8 QK option, softmax outside
  the tile. The measured tier gap IS the finding: on a gb1-class ~5k-token ASR prefill the
  emitted-tier blk_ffn crosses zen2/M1 at 1.13x while the loop-crown attn crosses at 2.9x
  (16.4s vs 5.6s); the f16-KV default bought only -3% (read bytes halve, the fp32 compute
  stays). It reads fine at pp512 (small share) and hurts at depth — deep prompts and batch
  serving pay the same tax audio does, text board included. **What a fix looks like:** an
  emitted, per-box-tuned attention family — convert-in-tile f16 dots, optional int8 QK
  (requant Q once, vnni/sdot straight against q8 KV — q8 flips from slowest cache format to
  likely fastest), softmax fused into the flash walk. **Why it's a WIN, not catch-up (Boris
  2026-08-05): llama.cpp runs the same dequant-to-f32-scratch seam — an emitted tile is
  'potentially faster' than the ref, and the deep-clip audio cards on the site are the
  ready-made scoreboard.** NOT the quant-lane arc; a kernel arc of its own, eventually.

- **Batched-tg bench row: tg at B=16 sequences is the kernel story decode can't tell at B=1
  (spotted 2026-08-04, zen2 thread ladder).** Single-sequence decode is GEMV — bandwidth-bound
  at any thread count, so das:ref tg reads as parity (t=4 zen2: 13.10 vs 12.59) while pp shows
  +45% on the same box. At B=16 every decode step is a 16-row GEMM: arithmetic intensity ×16,
  weight stream amortized across the batch — the same regime pp wins in, and the serving shape
  dasllama.io actually runs. The engine side EXISTS (`eval_batch_` in `dasllama_batch.das`,
  synchronous B-row step; Metal has `metal_batch_decode_forward`); what's missing is only the
  bench row: a `--tg-batch N` mode in `lcpp_bench.das` driving B sessions per step, ref via
  `llama-batched-bench` (separate binary from `llama-bench` — check the ref checkouts build it).
  **What a fix changes:** a board column where the kernel advantage shows in decode, and the
  first honest throughput-serving number for the server story.

- **Decode work-splitting vs thread count: tg on the 3990X ladder read flat 13.1–13.3 at t=4–8
  then jumped +59% to 21.2 at t=12 (2026-08-04).** Read: the per-op split count is mismatched to
  the lane count at some t (undersplit or oversplit, depending), with CCD-spread-vs-pack pin
  placement a secondary suspect (unpinned llama-bench led das tg at t=8, 14.1 vs 13.3). Cost
  today: optics only — single-sequence tg absolutes don't drive engine choices. Why it stays
  ledgered: batched decode (see the batched-tg entry) turns each step into a B-row GEMM where the
  same split/placement question prices real serving throughput; revisit there, not as a B=1 hunt.

- **`g_lp_cbs` per-step buffer churn: the `@scratch` recycling never happens on the batch-decode
  pipeline path (spotted 2026-08-02, lint uplift).** `metal_batch_decode_forward` builds each
  step's command-buffer list in a LOCAL `cbs` and hands it over with `g_lp_cbs <- cbs`
  (`dasllama_metal_decode.das:3002`) — the move drops the global's retained buffer every step,
  so `finish_pending_step`'s capacity-retaining `clear()` recycles nothing, and on a persistent
  heap the dropped buffer is ~ncb pointers per step. **What a fix changes:** prefill's
  `g_pf_cbs` ping-pong pattern (build into the global, or `push_from` a reference) recycles both
  buffers, deletes the per-step allocation, and makes the PERF026/PERF030 nolints at that site
  unnecessary. Cost today is small (one small alloc + drop per step); the reason to do it is the
  allocation sits on the measured hot orchestrator, so it belongs to a perf pass with A/B cells,
  not a lint sweep.

- **ASR peak-memory census (2026-08-03, m1, /usr/bin/time -l around asr_bench — the
  reproducible ladder is `benchmarks/asr/mem_census.sh`): canary warm footprint
  jfk 2.71 / gb1 4.29 / hp0x2 8.35 GB; canary MINT gb1 4.62 (+0.33 over warm);
  parakeet-v3 warm hp0x2 5.61 GB.** The canary-apples arc closed the 2026-08-02 baseline's
  two findings: the 13.1 GB canary serve peak (fp32 weights + quadratic scratch) is now
  ~4.3 GB on gb1 (q8 end-to-end + per-job attention slabs), the mint spike is +0.3 GB
  (read-time tensor-at-a-time transcode), and hp0x2-length canary — previously one clip from
  swap on a 16 GB box — runs at 8.35 GB. Parakeet's hp0x2 quadratic scratch died with the
  same slab change (−0.6 GB vs its pre-slab control). REMAINING: canary's hp0x2 tail is
  ~4 GB of decoder-side prefill/KV at ~7k soft tokens — map it (KV codec residency, prefill
  panel sizing) before chasing; and whisper's mint spike (1.48/5.29) predates this arc's
  machinery, already ledgered via the streamed mint.

- **Try the borrowed plane WITHOUT its bounds check, at profiling time (spotted 2026-08-01, audio
  `.dlim` arc).** `PlaneF`/`PlaneI8` `operator []` in `dasllama_plane.das` guards every read with
  one unsigned compare. **Costs nothing today, measured:** M1, `-jit`, cache-resident
  4-accumulator sum, 67.1 M elements, 7 interleaved reps — raw pointer 15.608 ms, checked plane
  15.611 ms, and the `array<float>` indexing it replaces 20.856 ms (cv ≤ 0.6% on every arm). So
  the check ships. **What the removal would change:** in that microbenchmark, nothing — LLVM
  folds the compare out of a loop whose bound it can prove. Real audio kernels index with computed
  offsets (`blob[woff + i]`, layer strides, conv taps) where the bound is not provable, so the
  compare may survive there and the microbenchmark does not answer it. The A/B to run once the
  audio carriers are converted: an `unsafe`/unchecked accessor variant behind a build knob, A/B'd
  on real encode/decode cells per family, interleaved, not on a synthetic sum. **Do not remove it
  on the strength of the number above** — that number measures a loop the real kernels are not.

- **gemma-4-26B-A4B tg 0.84x — routed k4 GEMV was load-issue-bound; float4-x fix SHIPPED, +8.7% tg
  (2026-07-23).** Chapter 1, knockout (new moe_rt/moe_sh arms, decode_metal_chase nomoert/nomoesh,
  Q4_K_M @512, best-of-3, clean window): full 18.80ms; routed k4/q51 expert GEMVs 5.48ms (29%) at
  ~163 GB/s; other GEMVs (QKV/WO/router) 5.30ms; dense-shared 1.89ms; attention 1.93ms; ew 1.96ms;
  non-gemv floor 6.13ms. Classifier ≈ ZERO time despite 21% byte share — the greedy spec chain
  covers it (1342 hits / 3 misses). Chapter 2, kernel-lab (benchmarks/matmul/bench_metal_moe_lab
  — exact 26B expert shapes E=128 nfe=704 dim=2816 k=8, exact-arith bit-exact contract, sel
  rotation for DRAM honesty): the barrier/bubble theory REFUTED (pipelined-no-barriers ==
  in-graph rate, ±7%; scatter-vs-contiguous sel: nil) — the production MoeGemvK4 kernel itself
  ran **142 wGB/s** at expert shapes, load-issue-bound on 32 scalar x loads per 256-block
  (the AGX checklist's vector-width-views item, missing in the MoE twins). float4-x view =
  **321 wGB/s (2.25x)**; W13-pair fusion ties it (327) so NO driver surgery; 4-rows/sg partial
  (202). FIX SHIPPED in MetalMoeGemvK4 (kernel-only; dispatch/driver/blob unchanged): in-graph
  full 53.06 → **57.65 t/s (+8.7%)**, gpu 18.80 → 17.31ms/step; kernel units + fam tolerance
  cells green. Chapter 3, the "other gemv" anomaly RESOLVED as a phantom (finer noqkv/nowo
  knockout arms, post-fix @512, full 17.27ms): QKV 2.56ms is AT ceiling — the 26B is HETERO
  (global heads 512-wide: wq 8192x2816 on global layers), so real QKV bytes are ~857MB/step,
  ideal 2.60ms; the old "3x" was a byte-estimate error. WO 1.52ms vs 1.30 ideal (~85%, ~0.2ms
  slack). The residual gemv-class remainder 1.23ms = router (~0.15) + the greedy spec chain's
  classifier dispatches (~1.1ms, gemv-gated, useful work). Routed experts post-fix: 3.84ms.
  Remaining quantified levers, by size: (1) **the dispatch model — MEASURED from both sides
  (2026-07-23 evening, same-window)**: das encodes **780 dispatches/step @8 / 811 @512** (new
  metal_dispatch_call_count instrument), every one implicitly barriered on the serial encoder.
  llama.cpp's own knobs price the structure on this exact model (tg128, ±0.4% reps): stock 58.50
  t/s; GGML_METAL_CONCURRENCY_DISABLE **52.17 (+2.08ms/tok)**; FUSION_DISABLE 53.90 (+1.46);
  GRAPH_OPTIMIZE_DISABLE 53.96 (+1.44) — lcpp-with-serial-encoder lands in OUR class, i.e. no
  kernel section loses; the whole gap is dispatch structure. Same-window das: chase full @8
  **62.3 t/s (16.05ms) — AHEAD of lcpp stock 58.50** under real greedy decode; the llama-bench-
  protocol rail reads 55.75 ±3.20 (synthetic-id feed perturbs the spec chain + the rail's tg is
  intrinsically noisy — the recorded 50.31 ±2.2 board cell is protocol-dragged and/or tinted).
  The port blueprint is llama.cpp's ggml_mem_ranges (ggml-metal-common.h): concurrent encoder +
  per-op src/dst range tracking — barrier + reset only when a new op's ranges conflict with the
  live set — plus ew-chain fusion and a graph-reorder pass that grows concurrent sets. Our shape:
  an enc_dispatch wrapper in dasllama_metal_common taking declared read/write (buffer, off, len)
  ranges per dispatch (every enc_* helper already knows its buffers), auto-barriering on conflict;
  g_skip runs must stay serial (the knockout-unbarriered caveat at dasllama_metal_decode.das:2511).
  **SESSION 1 SHIPPED (2026-07-23): the hazard-tracked concurrent encoder is in** — range tracker
  in dasllama_metal_common (exact (buffer, off, len) ranges, mutable frame buffers only; weights/
  uniforms untracked), hz_gate in every decode-path enc_* helper, undeclared dispatches take a
  conservative barrier (or PANIC under DASLLAMA_METAL_HAZARD_STRICT — the parity suite's arm13
  runs strict, so declaration coverage is test-enforced), a dispatch-counter check makes a gate
  bypass loud, DASLLAMA_METAL_HAZARD_PARANOID isolates the encoder flip. Measured on the 26B
  Q4_K_M chase (full arm, best of 3): @8 63.08 -> 65.78 t/s (+4.3%), gpu 15.81 -> 15.11ms; @512
  58.42 -> 60.84 t/s (+4.1%), gpu 17.07 -> 16.40ms; barriers 780/811 implicit -> 634/665 real.
  DEFAULT flipped ON after the family gates (llama arm13 strict, arm7 q8/tq4, gemma4moe both
  rows, qwen35/qwen35moe, gptoss, gemma4e — all green under CONCURRENT+STRICT); 12B Q4_K_M spot
  pair confirms: @8 35.05 -> 36.31 t/s (+3.6%, gpu -0.83ms), @512 32.37 -> 33.34 (+3.0%, -0.88ms).
  The barrier floor is the PROGRAM-ORDER chain depth (~19 genuine links/layer on the g4 graph:
  qkv triple, we1||we3, pre_ffn2||router_norm, post_ffn2||shared-w13sw group — everything else
  chains). **SESSION 2 PHASES A-C SHIPPED (2026-07-23 night): step-graph capture (kn_* twins
  record KNodes; graph_flush replays with the hz oracle deriving barriers, so a schedule can
  only change speed) + a shape-class schedule cache (FNV over the pso sequence, compiled once)
  with ASAP leveling over the conflict DAG + the g4 dense-shared/qwen35moe-shexp scratch split
  (own bh12s panel).** Measured 26B: @8 69.23 t/s gpu 14.40ms, @512 62.90 gpu 15.87 — cumulative
  vs serial @8 +9.7%/-1.41ms (das now well ahead of lcpp stock 58.50 same-window); barriers
  634->574 (the split's 2 levels/layer; leveling found nothing else — depth 574 IS the true
  chain). 12B (dense, no branch): unchanged, as expected. 26B/qwen35moe tolerance cells
  byte-identical maxd under STRICT; llama arm1/7/13 token-exact.
  **R1 ew-FUSION MEASURED (2026-07-23): one kernel ships, one refuted — the barrier lever has a
  sub-1% ceiling on the MoE-bound 26B.** MetalPreAddRms collapses the pre_post_norm epilogue
  post_attn_rms + add_rms into one dispatch (enc_pre_add_rms_site: two register reductions, x
  staged for the trailing norm; bit-identical to the pair — reduction structure copied from
  metal_rmsnorm/add_rms, the one reordered product is IEEE-multiply-commutative). DEFAULT ON via
  DASLLAMA_METAL_FUSE; chase `nofuse` A/B arm. Correctness: gemma3 token-exact (pre_post_norm
  fires it), gemma4moe-26b tolerance maxd byte-identical to baseline. 26B Q4_K_M 3-arm interleaved
  chase (full/fuse1/nofuse — Parsec-active, read the DELTA): **fuse1 vs nofuse +0.35% @8 / +0.37%
  @512, gpu -0.05ms, barriers 574->544 (-30 = one collapsed level/layer x 30 layers)** — small but
  consistent + reusable capability (larger on dense gemma: shorter chain, 30 barriers a bigger
  fraction). The SECOND fusion, MetalRmsAdd on the g4 dense-shared post_ffn1_rms + add, was
  authored, correctness-proven, and **REFUTED** by the same run: **-1.0% at both depths and
  DETERMINISTICALLY 0 barrier reduction** (720 vs 750 disp but 544 = 544 bar) — that tail is on the
  SHARED branch, which the leveler already overlaps with the longer routed branch, so fusing rms
  (inputs ready early) with the add (needs bmoe_rt, ready late) SERIALIZES the reduction behind the
  routed branch it used to overlap. Reverted; kept as this ledger's negative control. **LESSON:
  barrier COUNT isn't the whole story — a fusion OFF the critical path can't help and can hurt by
  moving a bigger node into a later, tighter level; and the 26B's barrier lever is bounded (~sub-1%)
  because the GPU is routed-GEMV-bandwidth-bound (round-1 dig), not barrier-bound.** Implication
  for the plan's R2-R5: the [metal_dispatch] macro lens stays worth it as CAPABILITY/eDSL, but the
  fusion+reorder PERF upside on this MoE model is small — the candidate deeper fusions
  (router_norm+router, swiglu+we2) hit the same norm-into-GEMV grid-wide-dep wall or need the GEMV
  kernel itself to fuse the activation (ledger-class). Still open from the plan: [tune]
  schedule axes (thin until fusion adds real choices), batch-rail unification (R4), dasMetal
  promotion of the graph/lens machinery;
  (2) the q51 w2 MoE kernel at 224 wGB/s in-lab —
  the integer-compose form (q | hbit<<4 pre-convert, replacing the select chain) TESTED + REFUTED
  2026-07-23: 226 vs 224 wGB/s (+1%), bit-exact but the dot stays issue-bound in the shift/mask
  decode regardless of compose shape; kept as the lab's w2_ic negative control, do not re-chase —
  a real q51 win needs a different decode strategy (per-thread multi-block amortization or an
  upload-time qh transpose, both ledger-class); (3) WO ~0.2ms slack; (4) MetalMoeGemvK5 carries the same
  scalar-x block — mechanical sibling of the shipped fix, prove via a lab arm first. Lab kept as
  the standing rig: variants stay as arms/negative controls.

- **[metal_dispatch] lens ROLLED OUT across the decode path (2026-07-23): 34 kernels generate
  their dispatch builders from the class; @role ratchet live.** The structure macro
  (dasllama_metal_lens.das) reads a [metal_kernel] class's @ssbo/@uniform fields and generates
  the enc_* builder — binds in @binding order, hz from @role (read/write/readwrite; weight =
  untracked static upload; alias = second view of a dual-bound buffer), @off = caller-passed
  bind offset (param interleaved after its buffer; shared names emit once — dual views ride
  two fields sharing one @off), @span = exact hz byte length (product micro-grammar), grid =
  per-WorkGroupID-dim `1|param|param/coverage` (literal coverage folds the ceil bias to the
  hand-written spelling; named coverage like g_gemv_rows keeps the +cov-1 form), tg = int
  const | "param" | "name*int". Every conversion was proven ast_dump byte-identical against
  its hand-written twin before deletion (only sanctioned diffs: dn_scan's exact-div grid
  emitted as value-identical ceil; order-free hz/bind permutations). Coverage: the simple
  single-PSO set + moe routing chain + deltanet chain + embed/gemv/w13sw/qkv_rs cores + the
  whole attn and rope-store families (14 cores) — variation points stay thin hand-written
  wrappers (fmt dispatchers kq_*/moe_gemv*, pso-parameterized ew2, dummy binds, f16 picks);
  batch rail untouched (R4). Net ≈ −690 lines of hand-maintained bind/hz code. RATCHET: a
  role-less @ssbo on a lensed class is now a COMPILE ERROR (the flip immediately caught
  RopeStoreQ8's undeclared bias plane); un-lensed classes stay covered by runtime STRICT.
  Gates: decode-parity suite green under CONCURRENT+STRICT (arm13 coverage proof); six
  fam matrix gates green — 26B q8/k4 tolerance cells BYTE-IDENTICAL to the pre-refactor
  baseline (2.498179/5.1424093, 2.5177252/13.825883), gptoss/qwen35/gemma2/gemma4/gemma4e
  clean; the only reds are the tracked pre-existing metal object leak (30/36/15 by filter,
  unchanged counts). `range` is a lexer type token — the hz-length tag is @span, not @range.

- **.dlim mint abort past ~11GB: root-caused as DISK-FULL, writer fixed, red cleared (2026-07-23).**
  Not an int-width bug — the write rail is 64-bit clean end-to-end (ftello, long_fwrite → size_t
  fwrite, long_length, uint64 offsets; the Jul-21 29GB mint of this same model and the image
  suite's 5.4GB voxtral arm are standing proof). The disk had filled: 804 GiB of .dlim images
  across ~27 identity generations back to Jul 16 — nothing ever deletes a superseded image, and
  the Jul-22 sweep's ~92GB of MTP mints ate the last headroom. ENOSPC then surfaced as the
  per-plane "offset-accounting" cascade (the quoted `0xf0` was the nbytes field — 240 bytes, the
  30-layer offs plane), and the abort path removed the tmp, hiding the space pressure. Fixed in
  the writer: `write_plane` now reports short writes honestly ("disk full?"), bails the walk on
  the first failure instead of pushing the remaining planes at a full disk, and `save_image`
  verifies the on-disk byte count after close (a buffered small-tail ENOSPC at fclose was
  silent). Swept the 115 dead pre-v5 images (659 GiB; every pre-Jul-22-13:18 file predates the
  IMAGE_VERSION=5 bump and is unloadable by construction); fam-gemma4moe re-mint verified green
  (both 26B rows' engage + tolerance cells pass; the arm's remaining red is the tracked
  pre-existing metal-object leak, 30 objects on this filter). OPEN (design, propose-first):
  .dlim GC — every IMAGE_VERSION/knob change silently orphans the previous generation (~20GB per
  big model). Candidate shapes: fold the tag into the filename and keep-one-per-(gguf, tag) on
  successful save; or utime-touch images on load and age out cold siblings at save time.

- **Q6-greedy spec-chain inversion on big pure-k6 files (2026-07-22 re-pair).** `spec_cls_capable`
  (dasllama_metal_decode.das:164) is a pure CAPABILITY test — it engages the greedy spec chain for
  any tied-k6 classifier with no BENEFICIAL condition, so a big pure-k6 file eats the spec-chain
  work where it is a net loss. Cost today: gemma4-12B Q6_K B=1 greedy runs 26.81 t/s with spec on
  vs 27.11 spec-off (~+1%; `DASLLAMA_METAL_SPEC=0` recovers). This was the board's "0.81x" cell —
  RE-PAIRED to 0.977 on / 0.988 off in a clean quiet window (the 0.81 was window-skew; spec-on had
  drifted 22.4→26.8). Fix: a size/format beneficial-gate (decline spec on pure-k6 above a
  param/layer threshold, picked at the 4B-wins / 12B-loses crossover) in spec_cls_capable or the
  :1888 engagement gate. Small inch (~+1%); deferred (Boris, 2026-07-22).

- **Small-model q8 single-stream GEMV vector-load: TESTED + REFUTED (2026-07-22), do not
  re-chase.** The hypothesis (the blob's +2 quant phase forces 4 scalar int8 loads instead of
  one byte4 vector load, costing ~7-8% at B=1) was IMPLEMENTED as Fix A (a uint16 blob view +
  2 aligned ushort loads + sign-extend unpack in MetalQ8Gemv), proven bit-exact on GPU, and
  measured in a clean A/B (distinct dll hashes): PERF-NEUTRAL-to-slightly-NEGATIVE. gemma3-1b
  B=1 master 192.1 vs Fix A 188.7 (the unpack ADDS ALU where the GEMV is not load-issue-bound);
  qwen3-4B B=1 63.40 vs 63.45 (noise). Root cause of the original "regression": the ledgered
  64.5→59.5 was WINDOW-SKEW, not a real issue-bound cost — the q8 B=1 GEMV is
  bandwidth-bound at the M1 ceiling (qwen3-4B 349 GB/s, gemma4-12B 327 GB/s, both near peak),
  so cutting weight-load issues does nothing. Contrast the gpt-oss expert-GEMV win, which cut
  X-RELOAD issues (a different axis). Reverted. The real q8 B=1 lever is the non-GEMV overhead
  (elementwise dispatch fusion + attention), not the weight-load path — see the ew-fusion
  ledger items below. Kept a byproduct: the gemv kernel-unit now covers the main vectorized
  loop (n=1120), which it never did before (committed).

- **Per-config .dlim: map-only load, BLOB-ONLY metal flavor — SHIPPED (2026-07-18).** The
  contract "no processing on load FOR THE CONFIG IT WAS BUILT FOR" holds: image v3 +
  METAL_IMAGE_TAG identity flavor; the 34B block_q8_0 blob REPLACES the planar q8 planes
  (one zero-copy MTLBuffer per plane via `metal_new_buffer_no_copy_untracked`, region byte
  offsets at bind — kernel indices stay uint32-safe); k4s/k5s ride the 16B strips and k6s
  the GPU split form; every q8 kernel is blob-addressed (the S16 scale twins collapsed),
  the fused QKV/W13 kernels bind per-segment views (kind-major layout), the fixed-B
  kernels grew the kq twins' `ys` y-row-stride uniform for fused-buffer writes, and the
  cat-blob caches + the `metal_blob_region` repack are DELETED. CPU inference on a blob
  model panics; the gathers (embed_row, dequant_q8_row, split-k6) read the blob directly;
  `load_model` picks the flavor via the registered `metal_model_servable` hook. Measured:
  1B transform 133ms / map 24ms; kq-pure transform 21ms / map 13ms; all three GPU paths
  serve with 0 declines. Rewiring residue for the ledger (re-measure the q8 cells first):
  the batch qkv site's split-K stands down (its reduce writes contiguous y — plain GemmB
  serves per-segment), the B<=4 unfused single-decode qkv/w13 cats became per-tensor
  dispatches (~2 extra dispatches/layer on those rails), and the legacy quantized-X
  prefill rail is DELETED (the `!mm` serving arms, the fused add+rms+quant/swiglu+quant/
  rope_qk kernels + PSOs, enc_gemm, and the X-quant pools — ~350 lines; the mulmm_legacy
  knob survives as the required-mode forced-decline test switch, and dasllama_metal_gemm's
  planar GEMM donor is untouched — it serves CPU-flavor models' batch offload).

- **QK-norm rope-store fusion — the f16 single-stream H-form SHIPPED (wave A chase round 2);
  the rest of the family is the residual (2026-07-17).** MetalRopeStoreHF16 folds bias +
  per-head RMS + rope + store into one threadgroup-per-head dispatch on QK-norm x f16-mirror
  single-stream decode (+~1% on the 4B board; Q6_K B=1 tied lcpp exactly). Residual scope,
  build when a board shows the gap: the f32/q8_0/tq4 codec twins and the BATCH H twins
  (those paths keep the MetalQkNorm prepass + flat rope-store — batch amortizes the extra
  dispatch over B rows, so the gap is smaller there), and a norm-capable fused qkv_rs
  two-pass form for the s16 path (fused qkv_rs still stands down under qk_norm).

- **Gemma stage-1 Metal deferrals (wave B, 2026-07-17).** The gemma2 enablement chose
  correctness-first shapes; each entry names today's cost on gemma-class models only (llama/qwen
  paths untouched):
  (1) *GeGLU fused-w13 stand-down* — SINGLE-STREAM HALF SHIPPED (2026-07-22): the decode
  MetalQ8GemvW13Sw grew an `act` uniform (0 = silu, 1 = gelu using the CPU geglu4 tanh-via-exp
  identity), and the dense-FFN gate fires the fused path for `w13_q8` regardless of activation
  (gemma's geglu no longer stands it down), skipping the separate geglu ew pass — -2
  dispatches/layer. Measured: gemma4-12B Q8 B=1 20.98→22.31 (+6.3%, 0.85→0.90x), gemma3-4b
  0.92→0.95, gemma2-2b 0.93→0.96, gemma3-1b 1.22→1.24; parity green (fam-gemma3/gemma4 +
  arm1-basic swiglu regression). STILL OPEN: the batch fuse13 rail (b2/b4 PSOs) keeps the
  hardcoded silu epilogue, and prefill's legacy fused rail (`enc_swiglu_quant` has no gelu twin,
  and the deferred-W2-add trick has no post-ffn-norm slot, so `fused` stands down entirely —
  mm-path prefill, the default, is unaffected).
  (2) *pre_post_norm sites are composed, not fused* — each post-norm is a separate in-place rms
  dispatch before the residual add (+2 dispatches/layer on every path). Fix: a `post_add_rms`
  kernel (rms(branch)·w_post + x, then the next pre-norm in the same tg — the row is already
  staged for the add_rms reduction).
  (3) *Sliding chunked dispatch is not compacted* — the single-stream part dispatch still grids
  ALL context chunks; below-window chunks exit whole-tg on entry (the comb skips them via chlo).
  At gemma2's 4096 window this only bites past 4K depth; fix = dispatch chunks [chlo, nchunks)
  with a ch0 uniform. Batch shares the early-exit shape (per-row windows preclude one compact
  range).
  (4) *Spec-chain stands down on embed-scale models* — the greedy GPU-argmax chain requires
  `embed_scale == 1.0`, so every gemma (sqrt-dim scale) eats the CPU next-token poke per step.
  Fix: a scale uniform on the embed-gather kernels (enc_embed / enc_embed_k6). Revisit at the
  gemma4-12B B=1 board — this was the 4B chase's biggest single lever.
  (5) *Batch window-crossing parity has no dedicated test* — the fam-gemma2 masking row proves
  single-decode + prefill past the window; the batch part/comb twins share the masked-kernel
  code but no batch test drives depth > window. Add one when a batch harness with deep
  per-row contexts exists.
  (6) *Prefill V-from-K is two dispatches* (stage 3c, 2026-07-18) — the no-wv layers run a flat
  panel copy (MetalPfCopy) then the ones-plane MetalQkNorm in place (+2 dispatches on gemma4's
  8 global layers). Fix: a read-K-write-V weightless-RMS variant (the CPU fuses the copy into
  rms_batch). Suppress adds one tiny classifier-tail dispatch — not worth fusing.
  (7) *Batch mv + split-K rails stand down under hetero* (stage 3d, 2026-07-18) — the mul_mv
  x-staging strides (u_xs4_*) and the split-K totals (u_skt_*, and the sk sites' u_qd k-dims)
  bake ONE attention class, so hetero (gemma4) batch rides the cat-GEMV forms at B <= 4 and
  the planar GEMM without sk at B >= 5. Fix when the gemma4 batch ladder says it matters:
  per-class xs4/skt twins bound per layer, same shape as the attention uniform picks. The
  batch V-from-K copy is also nrows tiny enc_copy_row dispatches — a strided-seg copy kernel
  collapses them to one.

- **Embeddings path (spotted building `/v1/embeddings`, 2026-07-06).** Two small items, neither
  chased: (1) `embed_forward` takes approach A — reuse `forward_prefill` then re-norm every
  position — which pays **one wasted last-position classifier GEMM** (vocab×dim) per embed call,
  because `forward_prefill` always runs the final norm+classifier on the last token. Fix would be
  to split the transformer stack out of `forward_prefill_body` (a `forward_prefill_stack` helper)
  and call it directly, skipping the classifier; cost is one GEMM against a whole forward, so
  negligible until embeddings are hot. (2) The server's `/v1/embeddings` calls facade `embed`
  per input, which **creates + deletes a full KV-cache session per input** — for a batch of N
  strings that's N session allocations. Fix: a dedicated reused embed session in the server (the
  facade would need a session-taking `embed` overload, or the server reaches the public
  `embed_forward` primitive — but that breaks the facade-only invariant, so the overload is the
  clean path). Both are backlog; the server is serial and embeddings are low-frequency.
- **q8 GEMV loses to fp32 on cache-resident weights without VNNI (zen2 whisper decoder,
  2026-07-08).** Decoder-q8 stage A/B on zen2 tiny/jfk: logits GEMV 3.8x faster (76 MB —
  bandwidth-bound, the q8 win) but per-layer decoder GEMVs +7-21% SLOWER q8 (~2.3 MB mats,
  L2/L3-hot across the serial decode steps; AVX2 int8-dot + per-step requant loses to plain
  FMA when there's no memory traffic to save). Cost today: whisper-tiny zen2 decode leaves
  ~10-15 ms/clip on the table; net end-to-end neutral so nothing urgent. Possible fix: a
  per-region "stay-fp32 when the mat fits cache and the box lacks int8-dot silicon" load
  heuristic (or a box_profile knob like batch_grid_2d) — DON'T build until the M1 decoder-q8
  re-sweep says whether sdot boxes want q8 everywhere (likely yes → knob would be x64-only).
- **Gemma-4 E-series DENSE prefill trails lcpp Accelerate-BLAS on M1 (Wave G3 A/B, 2026-07-07).**
  E4B pp512 das **178.7** vs lcpp **192.9 = 0.93×**; E2B pp512 das **376** vs lcpp **382 = 0.98×**
  (E2B ties, E4B ~7% back). Decode tied both (bandwidth-bound: E4B 18.7/19.2, E2B 36.0/36.9).
  Root: dense prefill has no sparsity/grouping lever (unlike the MoE waves that LED lcpp), so it's
  das NEON-SDOT vs lcpp Accelerate-BLAS (AMX-backed) on the projection/FFN GEMMs — the larger E4B
  dim (2560 × ff 10240) favors AMX more, hence 0.93× vs E2B's 0.98×. Cost today: ~7% E4B prefill
  on M1 only; boxes without AMX exposure (zen2/SPR VNNI) already close it. Possible lever: a
  gemm-gen tuned Q8 kernel for the E-series shapes (the tune framework already exists) — don't
  build until an AMX-less box A/B says the shape actually leaves das-kernel headroom on the table.

- **Canary-Qwen ASR runs fp32 for parity; q8 decoder+encoder is the follow-up (Wave A1,
  2026-07-08).** The token-for-token gate loads the LoRA-merged Qwen3-1.7B decoder + FastConformer
  encoder at fp32. Perf A/B (M1 8T, das vs NeMo SALM greedy; `benchmarks/asr/results.md`, now git history): das
  LEADS every short/dictation clip 1.4–3× (jfk das/nemo 0.61×, LibriSpeech 0.34–0.49×) — the
  Canary-Qwen use case — but TRAILS 3.7× on the 3-min gb1, where the fp32 1.7B decoder is
  bandwidth-bound over gb1's ~2500 audio soft tokens. Fix: a q8 decoder (the existing q8 GEMV path,
  ~2× decode on the bandwidth rail) + q8 encoder, both straight ports of the parakeet/whisper q8
  machinery gated behind the fp32 parity default. Don't chase until the ASR-perf pass — but
  gb1-class long-audio is where it pays.

- **Gemma-4 E-series audio (gemma4a) encoder is fp32 SCALAR — big A/B gap (Wave A2, 2026-07-08).**
  The parity gate is fp32 encoder correctness, so the gemma4a Conformer runs a plain fp32 scalar
  forward. A/B (M1 Max 8T, das vs llama-mtmd-cli; `benchmarks/asr/results.md`, now git history): das transcribe
  6028 ms / xRT 2.89 vs mtmd-cli 1547 ms / xRT 11.3 → **das TRAILS 3.9×**, dominated by the encoder:
  das encode 1888 ms vs mtmd 117 ms = **16×** (fp32 scalar Conformer vs ggml's bf16-weight SIMD
  GEMMs); long-context decode 21.7 vs 78 tok/s also lags. Unlike A1/parakeet/whisper (which lead or
  tie), this tower has had NO perf pass. Fix: route the gemma4a tower through the gemm-gen Q8 audio
  kernel (the same SIMD/threaded machinery parakeet/whisper towers already use) — likely the single
  biggest audio-side win on the shelf — plus the long-context decode path. Not chased mid-wave.

- **Qwen3-Omni AuT tower is fp32 scalar too; perf numbers are SOFT (Wave A3, 2026-07-08).**
  Same shape as A2: parity gate is fp32, so the shared qwen3a AuT encoder runs scalar. A/B (M1 Max
  8T, das vs llama-mtmd-cli): jfk das 3625 ms / xRT 3.03 vs mtmd 1173 / 9.4 = **das trails 3.09×**;
  jfk3 das 8263 / 3.99 vs mtmd 2079 / 15.9 = **3.97×**. Dominant gap = the fp32 scalar qwen3a tower
  (~4.8× encode), same lever as the gemma4a entry above — SIMD/Q8 the shared AuT/qwen3a encoder
  covers BOTH A2 and A3. The q8 MoE thinker (grouped prefill ~207 t/s + q8 decode) also trails ggml.
  ⚠️ These A/B numbers are SOFT: measured with a dormant Parsec host daemon (1.6% CPU) + Spotlight
  indexing the freshly-downloaded 34 GB — a clean announced Parsec-off re-sweep would firm them (parity
  is unaffected). Not chased mid-wave.

- **ASR short-clip fixed costs (parakeet, M1 — NEXT ROUND, Boris 2026-07-06; whisper tower
  q8 postponed one session behind it).** Cost today at matched 8T: jfk das 703 ms vs cli 352
  (2.0x), LibriSpeech dictation p50 651 vs 324; long clips already 1.07-1.10x, so the short
  end is where the M1 gap lives. jfk stage profile (q8): ffn 215 / conv_module 126 /
  attn_heads 120 / attn_proj 119 / conv_sub 55 / mel 33 / decode 27 ms. Levers: mel is
  single-threaded (thread FFT/mel-dot over frames); conv_sub GEMMs stay fp32 — check their
  threading; audit every `lanes_for_work` gate at jfk-size tt (small clips may run stages
  inline that would profitably thread at 8 lanes); small-tt q8 tile shapes /
  `effective_token_block` at tt≈140; per-layer requant overhead at small tt; v3 decode GEMV
  is 8198×640 per step (8× v2's rows) — q8 the joint if it shows. Gate per lever: token
  parity + jfk best-of-3 + LS p50 before/after, both boxes.

- **✅ SHIPPED + SILICON-ADJUDICATED (zen2, 2026-07-05): 2-D batch chunk space (row-units ×
  token-blocks).** Landed as `batch_grid_2d` (0 = 1-D / 1 = fine grid, ggml's 16-token cells /
  2 = wave-aligned, rc·ntc = whole L-waves); the knob arms a per-dispatch auto-gate (engages only
  when the 1-D grain cap starves the admitted lanes), gen ts=4 walk factored into a shared
  body_cell so the off-path is the old walk verbatim; bit-exact all three ways (op-level test +
  512-tok GEN_IDS on both boxes). zen2 A/B (T=48, 3 interleaved reps): 135M pp512 knob2 +7.9%
  (every rep; +15% in the post-pin interleaved confirm), knob1 only +2.8% — **wave alignment
  beats ggml's amortize-the-tail at one-claim-per-lane granularity, the same physics as the 1-D
  wave invariant**; 1B +3-4% (kv d=512 starves); Qwen3-0.6B null as geometry predicts (nothing
  starves at 48 lanes); T=24 control identical (gate can't engage). zen2 profile pinned
  `batch_grid_2d: 2`; M1 pin stays off (≤11 lanes can never starve — verified no-regress).
  OPEN: the SPR respin should A/B the pin at T≥48 (more shapes starve at higher lane counts;
  fine-vs-aligned may flip where claim overhead differs). Original scoping kept below.
  Our batch dispatch chunks over out-row units only; tokens loop inside each chunk. Shapes
  with few row-units starve high lane counts (135M d=576 → ~5-36 chunks for 48 lanes; the
  Qwen3-0.6B attn_chain "deep-thin" 50% lead is the same geometry). llama.cpp's GENERIC path
  chunks 2-D — (out-rows × tokens) grid, chunk 16, one atomic counter, all architectures
  (ggml-cpu.c:1388-1442) — which is exactly why their mid/high-lane scaling holds on tiny
  models where our gated pool tops out. Our batch walk already loops tokens inside units, so
  the chunk space generalizes mechanically (matmul_chunks gains a token axis; worker chunk =
  (unit range, token-block range)); y-slices stay disjoint by construction. Ordering: linearize
  ROWS-FASTEST (concurrent workers then share the token block's activation slice in LLC and
  stream disjoint weight rows — lcpp's choice; their counter-starts-at-nth first-wave trick is
  the rank gate's natural analog on our side). **Delivery shape (Boris, 2026-07-05): a PIN per
  architecture, not a default rewrite** — the 1-D scheme already wins 2 of 3 ladder rungs
  head-to-head (avx2, vnni) and the M1/zen2 boxes; 2-D lands as a box_profile knob (default
  off, current behavior byte-identical), enabled where the per-box tune/fleet A/B proves it
  (SPR-class high-T tiny models, deep-thin attn shapes). Candidate refinement to discuss at
  implementation: an in-code auto-gate that only engages 2-D when the 1-D unit count starves
  the admitted lanes (units < k·lanes) — the knob then pins the gate rather than the mode.
  Sized: the 135M-class T≥24 residual vs lcpp (their 0.90-1.0 cells) + the attn_chain lead.
  (Spotted reading their amx/generic drivers during SPR session 3, 2026-07-05.)
- **✅ SHIPPED (audio arc, 2026-07-06): the Qwen2-Audio arc (speech→text, Boris 2026-07-05).**
  Landed token-for-token vs mtmd, then grew into the whole audio wave — whisper-proper ASR
  (6-model family sweep), Ultravox/Qwen2.5-Omni/Voxtral, Qwen3-ASR (new qwen3a chunked
  encoder), Parakeet-TDT, live-mic dictation, uniform `load_asr_model`/`transcribe` surface +
  chat-audio verbs on the facade. Arc-local perf ledgers folded in below. Original scoping kept:
  The cheapest audio-input path: Whisper-large-v3 encoder (~640M — mel frontend via the
  ALREADY-BOUND dasMinfft real FFT, the same per-frame-FFT pattern dasAudio's partitioned
  convolution reverb production-tests; 2× conv1d+GELU subsample = kernel-3 neural convs →
  im2col over EXISTING matmuls, no new compute kernel — FFT convolution only pays at long
  kernels; N PLAIN encoder blocks = existing matmul/norm/softmax with non-causal no-cache
  attention) → avg-pool → linear projector → soft tokens spliced at the `<|audio|>`
  placeholder; the decoder is our EXISTING qwen2 arch untouched (no cross-attention anywhere).
  New pieces: im2col gather, encoder forward, embedding-span prefill (driver), mmproj GGUF
  loader; oracle = llama.cpp mtmd (GGUF pairs ship). ~1 modest arc; the encoder is SHARED
  infrastructure — the same implementation unlocks Ultravox (llama-3 decoder ✓ have it) and
  ~80% of a Whisper-proper port later.
- **✅ SHIPPED (kq chain regrain, 2026-07-12 PM²): kq layers re-admitted to both fused decode
  chains.** The chains run one activation image per consumed form (the per-op mm_pre_f
  contract): q8 projections read the Q8_0 image, kq projections the Q8_K image (kxq/kxs/kxbs);
  the w2-input requant moves to 256-row stage-0 groups and the wo-input requant to HEAD GROUPS
  of 256/head_size heads (Qwen3's 128 = head pairs) so every Q8_K quantize covers whole
  superblocks. Gates: dim % 256 for any kq weight, hidden % 256 for a kq down-proj, qd % 256 +
  head-size divisibility for a kq wo (all real kq models pass). Bit-exact vs the per-op path —
  test_fused_decode grew a kq arm (Qwen3-4B Q5_K_M = k5+k6+head-pairs, gemma-2-2b Q4_K_M =
  k4+k6+softcap/post-norm; ids + full logits EXACT). Measured (M1 Max steady-state): Q6 tg
  30.5 → 31.1 (+2.1%), Q5 ~flat (33.8, within run wobble), Q4 control flat; decode_prof
  confirms attn_chain 26% + ffn_chain 63% carry the whole kq decode. Dead requant_rows_q8_bs
  deleted with the last Q8_0-bs chain arms.
- **✅ SHIPPED (kq v3 panel-scratch, 2026-07-12 PM): the k5/k6 kernel unpack lever.** The
  tile now reads a BYTE-EXPANDED panel (one byte per weight — zero unpack ALU) that the
  batch cell unpacks from the packed grp planes once per (group, token-block)
  (`unpack_kq_panel_grp`, SWAR uint64 deposit), amortizing the 5/6-bit deposit over TB=128
  tokens instead of the tile's 4; the DRAM planes KEEP the packed 160/192 B/superblock form
  the decode path streams. M1 Max e2e (Qwen3-4B, warm/quiet, 3 alternated reps): Q5 pp512
  110 → 150–168 (lcpp 131 → das 1.14–1.28×), Q6 111 → 140–143 (lcpp 138.5 → 1.01–1.03×),
  tg64 at v2 parity (Q5 ~33.7, Q6 ~30.5, Q4 control untouched). Iso tile: k5 65→89, k6
  58→76 GMAC/s at the probe's ×16 amortization (~94/~79 effective at production ×32).
  **THE LESSON (measured, do not re-learn):** pure byte-expanded DRAM planes (no scratch)
  win the same pp but cost tg −30%/−20% on Q5/Q6 — M1 Max decode is DRAM-bound at the
  model level (~90 GB/s effective), so plane bytes ARE decode time; k4 stays nibble-packed
  everywhere for the same reason. Residual headroom, ledgered: (a) k6 tile 76 vs k4 95 —
  the per-16 SIGNED sub-scale fold pays 2 scale-row sexts + 4 muls per block vs k4/k5's 1;
  (b) k5 unpack still ~6% of production tile (the broadcast+carry deposit — a generated
  NEON tbl/cmtst unpack kernel would close it).
- **✅ RESOLVED (2026-07-12 PM², trace-diagnosed): the das prefill "bimodality" is M1
  package DVFS, not code.** Lane-timeline traces of a fast (168 t/s) vs slow (150) Q5
  pp512 run: all 8 lanes 98–99% utilized in BOTH, identical chunk counts, and a UNIFORM
  ×1.113 per-chunk slowdown flat across run-deciles — one P-cluster clock step
  (3228→2904-class), run-scoped. First-run-after-idle rides a ~3 s boost window (168);
  back-to-back runs sit at the sustained clock (150–152, ±0.6% — exactly llama-bench's
  stability, because llama-bench's reps are always steady-state). 45 s cool-downs recover
  only partially (155–162); pmset shows no thermal warnings (ordinary sustained-load DVFS).
  E-core lane placement is EXONERATED — nothing to pin. **METHOD RULE: report the
  steady-state MEDIAN of ≥3 back-to-back reps and discard the first-after-idle rep;
  best-of-N systematically picks the boost outlier.** Steady-state scoreboard (M1 Max,
  Qwen3-4B vs lcpp steady): Q5 ~150 vs 131 = 1.15×, Q6 ~141 vs 138.5 = 1.02×, Q4_K_M
  ~155-158 vs 172 = 0.90×.
- **✅ RACED (zen2, 2026-07-12 PM², 16 affinity-pinned cores both sides, ABBA before=52a22a39b
  after=3d78ca8ef, Qwen3-4B): das WINS Q5 pp 1.84× / Q6 1.32× vs lcpp; the kq v2+v3 arc itself
  is ~NEUTRAL on the maddubs lattice.** pp512 das-after/lcpp: Q4 161–180 vs 168.3 (~parity —
  their one AVX2 K-quant repack), Q5 158–161 vs 87.0 (**1.84×**), Q6 128–130 vs 98.0
  (**1.32×**); tg64 ≈ lcpp parity all three (19.3/16.6/14.5 vs 19.25/16.87/14.69). ABBA
  before→after: Q4/Q5 pp par-to-+3%, **Q6 pp −4.6%** (135.2→129.0 median), tg within noise
  (Q4 −4%, Q5 +2%, Q6 −3%). Reading: the M1 unpack win was an sdot-lattice property — on
  Zen2's maddubs lattice the v1 unpack was never the bottleneck, and the Q5/Q6 lcpp wins
  pre-date the arc (their vec_dot rail is that slow). The Q6 −4.6% is the arc's one x64 cost;
  suspects (not yet attributed — needs a mid-arc leg): the v2 signed per-16 fold's separate
  lo/hi i16 chains (flush every 2 madds) vs v1's, or v3's second verbatim load per weight
  vector on a load-port-bound lattice. Candidate fix if chased: gate kqBytes per-ISA (byte
  panels NEON-only — needs a per-format panel-flag companion so the batch cell knows).
  Sized: ~5% Q6 pp on zen2 only; zen2 stays 1.32× ahead of lcpp regardless. Also not yet run:
  the zen2 kq tune sweep (families ran the maddubs-mr8 fallback rows; crowns could shift a
  few %). SPR when a box respins — per Boris (2026-07-12): future profiling moves to an
  AWS box, local boxes are a bottleneck.
- **kq tune bench lacks a MoE-shaped cell row (spotted validating the mr4 crowns, 2026-07-12).**
  The mr4 tile crown gave dense pp +10-26% but gave back ~3% MoE prefill on qwen3moe-30B
  (fused expert cells average ~32 tokens per expert with d=768-class group spans — a regime
  the d=512/ntok=64 batch fixture doesn't represent; mr8's halved group count wins there).
  Adding a MoE-cell-shaped fixture to kq_tune_family and weighting the decision (or a
  per-model-class entry) recovers it. Sized: ~3% MoE pp on M1; re-check on zen2/SPR grids.
- **AMX fold pipelining (double-buffered C spill) — only if amx silicon verdict ever flips.**
  lcpp's tinygemm_kernel_amx interleaves block i−1's AVX-512 scale-fold between block i's
  TMUL ops (double-buffered thread-local C scratch, mmq.cpp:2015-2105) — the fold hides under
  tile latency; our emit_amx_tile serializes them, likely most of the T-independent ~1.6×
  end-to-end amx loss on SPR. Pure emission-order change (second spill alloca + reordered
  fold). NOT worth doing while the amx leg loses on frequency/bandwidth grounds anyway
  (SPR session-3 verdict: grid-resident, biased busd512 keeps the manifest); revisit on
  Granite-Rapids-class silicon or a cache-resident serving regime. Their thread-local
  once-per-thread ldtilecfg (vs our per-call config+release) rides the same follow-up.
  (Spotted reading mmq.cpp during SPR session 3, 2026-07-05.)
- **x64 intrinsic backends lack the `mm_rows` row-range GEMV core (fused-chain fallback).** The
  fused decode chains (team_parallel_stages, 2026-07-03) gate on `kernel_backend_has_rows()`;
  portable + both arm64 backends carry the core, so the EPYC (profile-pinned portable) and M1
  get the fused path — but an x64 box on auto-select (avx2-repack / acc8 / vnni tiers) falls
  back to per-op dispatches. Fix = extract each x64 family's GEMV inner loop into a
  `q8q8_rows_kernel_*` (mechanical, mirrors the arm64 extraction); worth ~the same join-tail
  savings wherever an intrinsic tier ever beats tuned-portable at decode. Cost today: none on
  the campaign boxes (both run backends that carry the core). (Spotted during the fused-dispatch
  work, 2026-07-03.)
- **Metal batched decode's skinny-M kernel valley — the P4 chase (emission arc, 2026-07-14).**
  The 3B d512 engineering curve (Parsec-on, fixed-token protocol) is 101/103/148/270 t/s at
  B=2/4/8/16 vs llama-batched-bench Metal's 134/219/250/363 — steps are GPU-bound (CPU side
  < 1.5ms), so the whole gap is skinny-M GEMM efficiency: the fixed-B batched GEMV forms
  ALU-saturate (~340-540 GMAC/s — the B=4 valley's 35.9ms step), and gemm32's M-pad-32 staging
  floor is ~51-54ms/step (~2900 GMAC/s padded) regardless of B, while lcpp's mul_mv sustains
  near-weight-stream rates to ne11~8. Chase candidates, LAB ROWS FIRST (bench_metal_gemv_kernels
  grows variants): (a) x-staged batched GEMV — stage the B-row X panel slice in threadgroup
  memory per k-chunk so per-lane device x loads (the ALU-bound form's limiter) become tgmem
  reads; (b) a gemm16 twin (16-row M tile — halves the pad waste and A-staging at B <= 16);
  (c) the production 34B-blob mul_mm at M-pad-64 (f32-X, no quant dispatches — costs blob
  residency next to the decode driver's planar regions). Smaller shavings on the same row: a
  cls-specific GEMM crossover (the lab says the 128256-row classifier flips at B~4, the uniform
  policy switches at 5); per-row KV writeback + logits scatter memcpys (~2.3ms at B=16) could
  thread; llama-8B (dim 4096) declines batch at B >= 5 (the add+rms+quant row slab caps at
  3072 — an unfused rms+quant pair would re-admit it); GPU-side per-row greedy argmax for the
  batch (the single-stream spec chain already carries the kernel) if greedy batch serving ever
  matters. Sized honestly: closing to lcpp's curve is worth up to ~2.1x at B=4, ~1.7x at B=8,
  ~1.34x at B=16 on M1. UPDATE (2026-07-14 late): (a) LANDED as the x-staged fixed-B forms —
  lab b2x 326-363 wGB/s (+40-88%), b4x 188-217 (~2x, above lcpp's ~186 at ne11=4); driver
  same-window A/B B=4 +19.2%, B=2 +1.8% (B=2's step is residual/dispatch-bound: knockout says
  GEMV+cls is 11.7ms of the 16.7ms gpu step at B=2, 21.4 of 27.6 at B=4 — the rest is the
  per-step kernel residual + ~141 serialized dispatch tails, the encode-ahead/fusion rounds'
  territory). (b)/(c) were settled by the GemmB v2 + per-site split-K rounds (32x32 occupancy
  beats 64-wide at M-pad-32; mul_mm rail kept opt-in). NEW smaller shaving measured en route:
  the in-loop s16 scale select costs ~3% at B=2-4 (wscale-f16 0 vs 1 driver A/B) — the per-PSO
  s16 bake stays on the ranked list.
- **Fused-chain follow-ups: MoE FFN chain + norm/quantize as a stage.** The 2026-07-03 fused
  decode covers the attention block (all q8 arches, head_size % 32 == 0) and the DENSE FFN;
  gpt-oss's routed-expert FFN keeps its per-op groupn dispatches (3/layer) — a 2-stage MoE chain
  (router serial, [experts' gate+up + act + requant] -> [downs + weighted reduce]) is the same
  shape one level up. And each chain still has a serial ~3-6us norm+quantize prologue per block;
  folding it in as a tiny stage-0 (chunked scale+quantize after a serial sum-of-squares) or
  fusing norm INTO the quantize pass is the last serial glue. Sized: MoE chain ~= the dense win
  for gpt-oss decode; norm-stage ~1% at 1B. (Spotted during the fused-dispatch work, 2026-07-03.)
- **DONE (perf pass, 2026-07-02): tied classifier matmuls the Q8 disk quants (`Model.cls_q8`).**
  Tied Q8 loads of a Q8_0 embedding (every tied model we run — probed all 11) transcode
  `token_embd` twice into qblob — a classifier copy at wcls_off (repacked with the other 2D
  weights) and a LINEAR copy at emb_q8_off that embedding rows dequant from on demand (the laneq
  repack interleaves wcls in place, so row reads need their own un-repacked copy; on a no-repack
  box the two could alias — noted x64 follow-up) — and drop the fp32 table: on gemma-4-12B,
  classifier traffic 4.03GB → 1.13GB/token and resident 4.03GB → 2.26GB. Rows are bit-identical
  (same Q8_0 data the fblob decode used; gated by test_parity_tied_cls_q8_rows); the classifier
  quants are exactly what llama.cpp matmuls. 8 of 9 tied-model fixtures held token-for-token
  unchanged; gemma2's "Once upon a time" flipped a near-tie under the PINNED classic+libm test
  kernels only (default kernels still matched the oracle 24/24) → moved to the counting prompt
  like Qwen2.5/Phi, oracle-refrozen. fp32/q4 loads and non-Q8_0 embeddings keep the exact old
  path. (Spotted wave 3.)
- **DONE (perf pass, 2026-07-02): V-from-K layers fuse the K→V copy with the weightless V-norm.**
  Decode (mm_qkv) and prefill both rmsnorm k→v out-of-place when v_norm is on (bit-identical to
  copy + in-place norm; the block's v_norm step skips those layers). (Spotted wave 3.)
- **DONE (perf pass, 2026-07-02): llama.cpp A/Bs run (quiet box, CPU `-ngl 0`, llama-bench
  pp512/tg64 vs our matched driver, ggml-parity fast-math).** gemma-4-12B: prefill us 75-80 t/s
  vs llama.cpp 74.4±0.5 (parity to +5%); decode us ~7.3 vs 8.74 (~84% — the remaining decode gap
  is the next lever). gpt-oss-20b: prefill us ~219 vs 117 (~1.9× FASTER — the grouped MoE GEMM);
  decode us ~19 vs ~39-42 (~0.47× — exactly the MXFP4→Q8 doubled expert-weight-traffic asymmetry
  quantified: the native-MXFP4/Q4_0 entry below is now the headline gpt-oss decode lever).
  POST-JOBQUE (#3361 wake propagation + batch dispatch + worker spin, same-window anchors):
  12B decode 7.3 → 7.9 t/s vs 8.63 (gap 84% → ~92%); gpt-oss decode 19.2 → 22.0 vs 39.9
  (0.47× → 0.55×) — the dispatch-latency share of the decode gap is banked; what remains on
  gpt-oss is the weight-format asymmetry. (Spotted waves 3/5.)
- **DONE (perf pass, 2026-07-02): MoE prefill runs expert-bucketed grouped GEMMs — bit-exact.**
  `ffn_moe_prefill_grouped` routes every position (one batched router GEMM + the shared
  `moe_select`), CSR-buckets the (position, slot) pairs by expert, runs one batched GEMM chain
  per touched expert off a single whole-batch requant, and reduces the parked outputs in exactly
  the decode accumulation order (k slots then shared expert) — so it is bit-identical to the
  per-position path: the batch GEMM/requant/gate kernels are bit-for-bit their single-token
  forms per row. Proven on both MoE models (all logits identical after a 300-token prefill,
  grouped vs reference) and pinned by the qwen2moe fixture running through BOTH paths
  (`set_moe_grouped_prefill` A/B). Decode unchanged (one token = no bucketing win).
  Measured (M1 Max, interleaved in-process A/B): Qwen1.5-MoE 512-tok prefill 31 → ~270 t/s
  (~8.7×); gpt-oss-20b 256-tok prefill 27 → ~186 t/s (~6.8×). (Spotted wave 4.)
- **DONE (perf pass, 2026-07-02): MoE decode re-quantized the same activation per expert.**
  `moe_ffn_core` now quantizes xb once per layer into dedicated `moe_xq/moe_xs` (the
  down-projections quantize s.hb into the shared xq/xs, which would clobber a hoisted image
  there) and routes every gate/up matmul through `mm_at_q8_pre`. Bit-identical (same quants).
  (Spotted wave 4.)
- **DONE (MXFP4 arc, 2026-07-02): native-MXFP4 expert stacks + repacked TBL/SDOT kernels + the
  MoE dispatch fuse.** Was: gpt-oss-20b's 4.25-bit expert stacks ran as Q8 (2× resident, 2×
  decode traffic). Now: the stacks stay as raw nibble + E8M0 planes (mxq/mxs, exact disk bits —
  the old dequant→requant amax error is gone), decoded in-register by new aarch64_neon
  intrinsics (tbl16_lo/tbl16_hi = vqtbl1q_s8 of the doubled-e2m1 LUT; sdot4_w / sdot4_laneq_w
  take the tbl result as a VALUE) through dot_mx4q8 ([tuned], row-major) and dot_mx4q8_laneq4
  (interleaved 4-row repack, the block_mxfp4x4 twin). Grouped prefill expands each touched
  expert to EXACT Q8 (lossless: q = LUT int, scale = e8m0_half), writing the interleaved form
  directly on a repack backend; short prompts route per-position (npos·k ≥ 8·n_expert guard).
  On top, the MoE decode dispatch fuse: region-list groupn/groupn_mx4 kernels run all k
  experts' gates (ups, downs) in ONE fork/join — 288 → ~72 mm dispatches/token, bit-exact.
  Measured (QUIET box, 2026-07-02, same-window anchors): decode 22.0 → **31.8 t/s** @ ctx 8
  (30.7 @ ctx 512) vs llama-bench tg64 41.1 — **0.55× → 0.77×**; per-op profile: mm_moe 50%
  measured vs 47.9% theoretical share (the 66/66 format asymmetry is GONE), MoE mms sustain
  ~77GB/s vs the dense mms' ~99 (the remaining MoE-efficiency gap = the next lever); 12B decode
  7.98 vs anchor 8.67 (92%, unchanged — dense path untouched); resident 26 → 13.2GB; every
  fixture token-for-token unchanged (no refreeze — the counting fixtures absorbed all
  kernel-order changes). Cost paid: gpt-oss pp512 ~186 → ~121-149 t/s (the per-expert
  expansion) vs llama.cpp's 119.9 — still ≥ parity; the native mx4 batch GEMM in the expansion
  entry below reclaims it. (Spotted wave 5; the Q4_0 halfway house was skipped — native landed
  directly.)
- **q4 has no batched prefill kernel — prefill collapses to decode rate.** The q4 path serves
  everything through the scalar fp32-activation `dot_q4`/`matmul_q4` (no q8-style token-blocked
  batch GEMM, no NEON arm, no repack backend), so a q4 prefill runs at generation speed:
  measured on SmolLM2-135M, q8 prefill 1391 t/s vs q4 prefill 70 t/s ≈ its own 69 t/s decode.
  A q4 batch kernel (or the load-time q4→q8 transcode as the cheap fix) closes it.
  (Spotted tutorials wave.) **LOW PRIORITY (2026-07-02):** the path is cold — every model we
  test/ship parity for is Q8_0 on disk (plus gpt-oss MXFP4→Q8); no Q4_0 GGUF anywhere in the
  fixture set. q4 only fires when a user opts into `QuantMode q4` for footprint. Priority rises
  only if the MXFP4→Q4_0 halfway house above lands (q4 becomes the resident format of a real
  20B model). **RESOLVED DIFFERENTLY (2026-07-16):** Q4_0 *files* now serve natively on the kq
  rails under q8 mode (KqFmt.q40, #3481) — batched prefill included; the legacy tier was renamed
  `QuantMode.q4_0` and remains the requant-from-f32 footprint option, per-row kernels unchanged.
- **LOW PRIORITY: f32 projection GEMM is untiled — dot-per-token, no token block.**
  `matmul_batch` (dasllama_math.das) is the exact pre-#3315 shape the Q8 path had: weight-
  stationary nest with one horizontal-reduce `dot()` per (row, token), zero register reuse
  across rows/tokens, and no L2 token-blocking (long-prefill X re-streams from DRAM per weight
  row). The SDOT-era fix transfers verbatim since it's dtype-agnostic: a 4-row × 4-token
  register tile with float4 `mad` chains and per-tile reduces (the fp32 twin of
  `dot_q8q8_sdot4x4` — keeps W row-major, no repack, decode GEMV untouched; do NOT reuse the
  broadcast-A `gemm_f32` form, it needs a transposed W copy) plus an `effective_token_block`
  at ~¼ the Q8 block (fp32 activations are 4× fatter). Expected kernel win ~2-3.5× (what the
  attention tile measured), ceiling below Q8 (fmla = 4 MACs/instr vs SDOT 16; 4B/weight vs
  ~1.06B). LOW because the f32 arm only fires for f32 GGUF tensors — in practice the tiny
  teaching models; attention's fp32 GEMMs already have the register tile (`gemm_f32_uk_4x16`).
  (Spotted post-#3354, 2026-07-02.)
- **DONE (perf pass, 2026-07-02): `kv_cache_off` prefix-summed per call.** `Model.kv_row_prefix`
  (filled by layout_offsets, seq_len-independent) × the LIVE seq_len at call time — the O(1)
  Model overload serves both hot call sites; the Config walking form stays as the definitional
  reference. (Spotted post-wave-3 review, per Copilot on #3346.)
- **DONE (perf pass, 2026-07-02): decode attention threads over heads — crossover measured,
  default re-set.** `attention_std_decode` maybe_parallel_fors the head loop (disjoint per-head
  rows ⇒ bit-exact vs inline, gated by test_forward), behind `g_decode_attn_par_threshold`
  (profile `runtime.decode_attn_par_threshold`). Quiet-box sweep (M1 Max, inline-vs-threaded
  interleaved at 32..2048 ctx on Llama-3.2-1B and gemma-4-12B): crossover at ~200-260K work on
  BOTH; below it threading costs ≤2%, above it wins reach +74% (1B) / +89% (12B) at 2048 ctx —
  the derived 4M default was ~15× too conservative (the 12B ran inline below ctx 512). Default
  is now the measured 262144. (Spotted tune audit, 2026-07-02.)
- **LOW PRIORITY: `sample_` top-k is O(top_k × vocab) scalar selection.** Each of the top_k
  rounds rescans the whole vocab (dasllama_common.das sample_) — top_k=40 on gemma-4's 262144
  vocab is ~10M compares per sampled token. Cold today (SamplingParams defaults are greedy /
  top_k=0, and all parity fixtures are greedy), but it's the sampling path the tutorials teach.
  Fix = single-pass partial selection (bounded min-heap of size top_k, or threshold-and-count).
  (Spotted tune audit, 2026-07-02.)
- **DONE (MXFP4 arc follow-up, 2026-07-02): the mm_moe bandwidth-gap profile + the bias fold.**
  Iso-benched the exact decode dispatch shape (4× [2880 x 2880] regions, DRAM-rotating):
  the fused mx4 groupn GEMV sustains **~101 GB/s — bandwidth parity with the q8 dense kernels**
  (the "77 GB/s" in-decode reading was largely single-window wobble: same build re-measured
  90 GB/s an hour later; METHOD: only round-robin interleaved cells within one process are
  trustworthy on this box, single-window absolutes swing ±10-15%). The pre-fuse 4×1 dispatch
  shape measures 63-72 GB/s — the dispatch fuse was worth ~30% and is confirmed load-bearing.
  Follow-up landed: **expert bias vectors fold into the groupn workers' stores** (bp/boffs on the
  groupn contract; bit-identical to the post-pass add_bias, minus its serial ~36us/layer) —
  decode 34.7 → **35.2 t/s @ ctx 8 / 33.8 @ ctx 512** (llama.cpp same-window anchor 41.1 →
  0.86×). Also swept: decode-attn threshold 0-vs-262144 under the spinner at ctx 8/512 —
  a WASH at both depths (the low-ctx attention is memory-latency-bound; threading's dispatch
  cost ≈ its serial cost), so the measured default stands; moe_reduce/rope threading rejected
  (~8us/layer each, below dispatch cost). What remains vs llama.cpp is their continuous-polling
  threadpool (the bus never idles between ops) — picked up by the x64 arc's jobque work, not
  patchable here. (Profiling session, 2026-07-02.)
- **MXFP4 grouped prefill pays a per-touched-expert Q8 expansion (~120MB of traffic each, half of
  it the repack scratch copy).** `expand_mx4_region_q8` writes exact row-major Q8 then runs the
  load-time `repack_q8q8_weight` (temp copy + interleave) so the laneq batch GEMM applies. Levers,
  in effort order: (a) expand DIRECTLY into the interleaved layout (folds the repack's copy away —
  needs a backend-provided expand-repack, not a layout hardcode in common); (b) a native MXFP4
  batch GEMM (mx4 twin of the laneq 4x4 tile — halves the GEMM's weight streaming too, likely wins
  outright); (c) the `npos * k >= 8 * n_expert` tiny-batch guard is an ESTIMATED breakeven
  (4-tok-prompt ttft 1895ms -> 114ms) — sweep it when the mx4 A/B rig exists. (Spotted MXFP4 arc,
  2026-07-02.)
- **Flash-attention tile shape is a frozen compile-time constant — deferred x64 tuning axis.**
  `ATTN_FLASH_QT/KV = 64×64` (dasllama_common.das) was chosen on M1 and never swept; tile shape
  is the classic per-box cache parameter, and x64's small private L2 differs in kind from M1's
  big shared L2. QT is compile-time-coupled to the `float[64]` running max/sum fixed arrays and
  the fa_* scratch sizing, so this is a compile-time axis à la `[tuned]` (profile-keyed), not a
  runtime setter. DEFERRED until an x64 box exists to measure on — do not solve the coupling
  speculatively. (Spotted tune audit, 2026-07-02.)
- **DONE (GEMV hunt, 2026-07-03): the decode "kernel top-end gap" was chunk-count misalignment,
  not the kernel.** A 12-variant GEMV race (`bench_gemv_decode.das` + the matmul_variants decode
  cells: unroll/ILP/fma-flush/dual-group/inline-scale) proved every kernel micro-opt a wash —
  llama.cpp's live M1 kernel (`ggml_gemv_q8_0_4x4_q8_0`; their 4x8 tier needs i8mm) is the same
  shape as `dot_q8q8_laneq4`. The real delta: njobs = `get_total_hw_jobs()*k` sized chunks to the
   7 WORKERS, but 8 lanes serve them (workers + caller — team by design, fifo via main-steal), so
  the last wave ran half-empty (28 chunks / 8 lanes = 87.5% utilization). Fix:
  `get_dispatch_lanes()` (workers + caller; knob `set_dispatch_caller_lane`, decode_prof
  `--legacy-lanes`) at all 54 dispatch sites incl. x64 (same effect there, smaller: ~2-3% at 32+
  workers). Measured: GEMV cls 109 → 118-120 GB/s (their exact rate); decode e2e 74.3 → 77.6 t/s
  @ ctx 8 (+4.4%, ~0.95× lcpp) / 66.3 → 67.8 @ ctx 512; mm_ffn 1.10× → 1.04×, cls 1.19× → 1.12×
  of lcpp per-op. Suite 171/171 token-for-token (chunk splits don't move per-row math). Oversplit
  re-sweep under aligned lanes: decode e2e is a WASH across ×1/×2/×4 (×8 past the knee) — the
  straggler-mitigation rationale dissolved once the split became 8-on-8, ×4 default stands on the
  prefill iso numbers. (GEMV hunt session, 2026-07-03.)
- **Q8 scales stream as a separate fp32 plane — llama.cpp's inline-fp16 layout moves ~6% less
  weight traffic.** Their `block_q8_0x4` packs 4 fp16 scales WITH the 128 quant bytes (136 B per
  4-row block-group, one stream); our laneq layout reads a second fp32-plane stream (144 B total
  per group). At the now-aligned ~118-120 GB/s both sides saturate equally, so the remaining
  cls/ffn per-op gap (1.04-1.12× of lcpp) is almost exactly this byte ratio (144/136 = 5.9%;
  bench: our best cls call 2448us vs their 2307 = 6.1%). The bench's `inline`/`inline_u2m` cells
  already prove the single-stream kernel shape works (fp32 inline = parity, occasionally +1%).
  The win requires fp16: (a) an f16→f32 convert intrinsic in dasLLVM aarch64_neon (vcvt_f32_f16
  via LLVM fpext, x64 twin F16C `vcvtph2ps` — gate on `cpu_supports`); (b) a repack variant
  writing [4×fp16 d | 128 qs] blocks + the GEMV/batch/group3/groupn kernels reading it; (c) the
  element-offset plumbing moves from plane offsets to block strides (loader `w*_off` math, groupn
  offs contracts). Sized: dasLLVM intrinsic + codegen bump, ~4 kernel twins, loader/offset sweep,
  fixture risk LOW (same math, scales exact fp16 round-trip of what the loader already computes —
  but the quantizer would store fp16-rounded scales, so oracle fixtures need a refreeze check).
  Expected: cls 1.12× → ~1.06×, ffn 1.04× → ~parity; decode e2e ~+2-3%. NOT this PR — needs its
  own arc. (Spotted GEMV hunt, 2026-07-03.)
- **Audio arc: the fp32 encoder is the whole ASR cost — q8 the encoder GEMMs.** The whisper-family
  encoder runs fp32 ≈ 18–19 s per 30 s chunk on M1 (~37 s for qwen2a's standard 2 chunks;
  large-v3-turbo transcribe = 0.49× realtime, almost all of it the encoder window). Fast path:
  q8 the 6 encoder GEMM families + projector at load (`quantize_weights` pattern) onto the
  generated q8q8 kernels — expect the usual ~4× (turbo → ~2× realtime) — plus threading the
  im2col/pack loops. Tolerance-gate like flash-decode (stage witness vs fp32), token-parity
  revalidate. (Audio arc, 2026-07-05/06.)
- **Audio arc: the all-silence second chunk's soft tokens are input-independent.** Every ≤30 s
  clip pads to 2 chunks and the second is all mel-floor — its 750 soft tokens are the same for
  every clip, cacheable per tower; halves qwen2a encoder cost for short clips. (Audio arc,
  2026-07-05.)
- **Audio arc: whisper decoder logits GEMV, only if it shows.** The ASR decoders are small
  (tiny ~35 MMAC/token, turbo ~140 incl. the 66 MMAC tied logits) — fp32 is fine; q8 the
  token_embd logits GEMV if profiling ever surfaces it. (Audio arc, 2026-07-06.)
- **Audio arc (correctness follow-up, not perf): honor `encode(parse_special=true)`.** The flag
  is documented-unhonored; every call site assembles specials by id + per-segment text encode
  (the chat-layer pattern). Fix belongs in the tokenizer proper; the workaround callers migrate
  after. (Audio arc, 2026-07-05.)

## Transferred entries (folded in 2026-07-29, dasLLAMA doc reorg)

Arc-local ledgers and standing verdicts that were living inside per-arc plan docs. Two of them
(`whisper_plan.md`, `qwen2_audio_plan.md`) carried their own header saying "fold into
API_REWORK.md at PR time" — this is that fold, into the ledger's new home. Source doc named per
group; wording kept.

### From `gemm_generator_plan.md`

- **AMX tile family — arc CLOSED (slice I built it, slice K's SPR session 4 adjudicated it,
  2026-07-05).** amx is 0.36–0.55× the biased-busd512 champion across all four ladder models and
  the best cell ever measured sits below the plain AVX2 maddubs tier; the bill is the kstep=1
  Q8-scale-boundary fold (~256KB of C-spill per 32-row × 32-token unit vs 64KB of weight traffic —
  codegen, frequency license, and dispatch all separately exonerated). Family stays grid-resident,
  `spr_manifest` unchanged; revisit only on Granite-Rapids-class silicon or a cache-resident
  serving regime. Conditional-only tail: an **amx kstep=2 fold** (K=64 tiles, two scale segments
  per spill) halves the C traffic but its honest ceiling is ~1.6× behind the champion.
- **smmla leg (slice J) — no fallback-chain change (M3 Air silicon, 2026-07-05).** The smmla tile
  sits at **100% of the M3's ONE-NEON-pipe MMA ceiling** (130.6 GMAC/s = 4.08 G MMA/s ≈ 1/cyc,
  marshalling fully hidden) and still loses to sdot's ~2.45 issues/cyc across 4 pipes — 1 pipe × 32
  MACs < 4 pipes × 16 MACs, structurally. The leg stays grid-resident + `requires="i8mm"`-gated for
  server arm64 (Neoverse V1/V2 / Graviton3+ issue smmla 2–4/cyc, where the same stamp should flip
  the verdict — the tune framework decides per box, which is the whole point).
- **PARTLY SHIPPED — loop-hint manifest kind.** The *storage* unified: the per-app tune sidecar's
  `"kernels"` section is documented to carry "this module's stamps and the loop-hint perms"
  (`modules/dasLLVM/daslib/llvm_tune.das:40`), and `dasllama_tune.das` now reads it through
  llvm_tune (`box_profile_path` → `tune_manifest_path`, `box_profile_stale` →
  `tune_sidecar_stale`, the perm read → `tune_manifest_get`). Still OPEN: llvm_tune has **no
  loop-hint value kind of its own** (no loop-stamping code anywhere in it) — `dasllama_tune.das`
  still owns the `[tuned]` grid and does the hint stamping, with 18 live `[tuned]` sites, so the
  plan's "subsume, then delete dasllama_tune's `[tuned]`/box_profile rail" is not done.
- **OPEN — per-slot perms.** Unimplemented: no per-slot perm plumbing exists in llvm_tune or
  `dasllama_math_gen.das`; the phrase appears only in plan prose and in the unrelated per-slot
  *backend* pin (`x64_arch.md`, `dasllama_math.das`). What landed instead is per-FORMAT families —
  separate `[tune]` brackets for k4 / k5 / k6 / q40 / q8q8 / q51 in `dasllama_math_gen.das`, each
  its own manifest entry. Per-slot perms remains the named escalation if a box ever flips the amx
  end-to-end verdict, or if biased-vs-plain gemv turns out to matter under an amx stamp.

### From `kv_cache_compression.md` §9

- **BLAS / Accelerate on macOS — RULED OUT (measured).** dasLLAMA's 8-bit `sdot` kernel beats the
  BLAS (Accelerate) path, and it is structural, not incidental: BLAS is f32-only (no `hgemm`, no
  int8 — ggml's whole BLAS backend is one `cblas_sgemm` that dequantizes any non-f32 operand into
  scratch first), while the weight matmul is memory-bound on the weight stream, so a 4× weight
  expansion to feed AMX/SME f32 throughput can never win. Decision: don't route dasLLAMA matmuls
  through BLAS. **Its "for Apple lower-precision matmul the framework is BNNS … not pursued" tail
  is SUPERSEDED** — the AMX arc (#3562) shipped a BNNS-f16 lane in
  `dasllama/dasllama_math_accelerate.das` (bf16 planes via BNNS f16 matmul under
  `DASLLAMA_ACCEL_F16=1`, jobque strip-dispatched).

- **OPEN — k4-native deltanet planes (the "dn planes are ALWAYS q8" loader invariant aged).**
  Minted on 0.8B–4B where the dn projections were a rounding error; on Qwen3.8-27B (dense
  hybrid, 48 recurrent layers) they are ~5.6 B of the 6.7 B q8-region weights, so the
  q4k→q8 load transcode costs **+~2.8 GB of image (22.3 vs ~19.5 GB metal blob) and ~20% of
  decode's per-token weight reads** (measured via the 2026-08-17 27B bring-up: dlim plane
  sizes + the q8-region scale count). The dn shapes are why the invariant exists — β/α are
  dim × dt_rank (16–64, under every kq GEMM tile's N) — so the fix is a narrow k4 GEMV
  family (`MetalDnBa`-class + the CPU dot), not a kq-tile retrofit. Pays on every lane, and
  it is step one of **the 16 GB-card game** (Boris, 2026-08-17): 27B q4k on a discrete
  16 GB GPU needs the image at ~19.5 GB *plus* partial residency (the vulkan tier's
  offload/stream lane) or a ~3.5 bpw format family (k3/IQ — unsupported today).

### From `audio_models_plan.md` (Findings; correctness, not perf)

- **OPEN — the v0.3 Mistral `[INST]` template's derived close is ` [/INST]` — wrong for multi-turn
  (should be `</s>`); untouched, flagged for Boris.** Pre-existing; spotted in audio session B while
  adding `ChatTemplate.assistant_close` for the v7-tekken template (which does close with `</s>`).

### From `whisper_plan.md` ("Performance ledger — fold into API_REWORK.md at PR time")

- Measured (M1, `examples/dasLLAMA/transcribe.das`, jfk 11 s, fp32): tiny 22×, base 10×, small
  2.6×, medium 0.85×, large-v3 0.42×, **large-v3-turbo 0.49× realtime** — almost all of it the fp32
  encoder window.
- Turbo encoder = the qwen2a tower cost (fp32 ≈ 18–19 s per 30 s chunk on M1) — the q8
  encoder-GEMM path from the qwen2-audio ledger applies verbatim and is the headline item
  (expected ~4× → ~2× realtime turbo). Same lever as the "Audio arc: the fp32 encoder is the whole
  ASR cost" entry above.
- Decoder is small (tiny ~35 MMAC/token, turbo ~140 MMAC/token incl. 66 MMAC tied logits) — fp32
  fine; q8 the token_embd logits GEMV if it shows up.
- Model load: byte-wise f16→f32 tensor reads (the bin stores tensors unaligned) — turbo ~1.6 GB f16
  loads in a few seconds; an aligned-fast-path only if load time starts to bite.
- gelu LUT round-trip is per-element float work — noise next to GEMMs, don't optimize.
- Long audio: encoder windows are sequential per seek; pipelining only if long-form becomes a real
  use case.
- One WhisperSession per stream shares one loaded model; sessions are cheap (scratch + caches),
  models are not — same economics as the LLM side.

### From `qwen2_audio_plan.md` (its two duplicate "Performance ledger" sections, deduped)

- Encoder GEMMs ride the existing generated-kernel family as-is (n=1280/5120 batch shapes). A
  dedicated tune pass over encoder shapes (1500-token batch, always-prefill regime) only if the
  encoder becomes a measured bottleneck vs mtmd. *(This is the later of the doc's two takes; the
  earlier "q8 the 6 GEMM families + projector at load" phrasing is the same lever, already carried
  by the "Audio arc: the fp32 encoder is the whole ASR cost" entry above.)*
- prefill 1518 pos (1500 audio) ≈ 16.4 s on M1 for the 7B q8 decoder (~93 t/s); decode ~11 t/s —
  normal 7B-on-M1 numbers, no audio-specific cost.
- mel/DFT-GEMM is noise, leave it: single-threaded `gemm_f32`, ~0.13 s per clip, and the
  Mel-as-GEMM twiddle matrices are f32 fblob consts (~410 KB) — quantizing them is noise, skip.
- 30 s chunking processes chunks serially; multi-chunk audio could pipeline encoder chunks — only
  relevant for long-audio use, not scoped.
- *(Deduped away: the all-silence-second-chunk soft-token cache appears in both sections and is
  already its own entry above — "Audio arc: the all-silence second chunk's soft tokens are
  input-independent", 2026-07-05.)*

### From `PUBLIC_BENCH_PLAN.md` (v2 backlog)

- **OPEN — v2: hosted submission server**, which also hosts / auto-fetches the pinned llama.cpp ref
  binaries. v1 accepts community records via reviewed PR plus the result panel's "copy record"
  paste funnel; the server replaces that funnel.
- **OPEN — v2: select-two compare widget (maybe)** on the leaderboard, and publishing the
  depth/batch test variants (`pp512@d4096`, `tg128@b4`, …) that stay internal-only under v1.
