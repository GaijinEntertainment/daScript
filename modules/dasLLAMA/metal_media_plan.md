# Metal media prefill — chunk 1 of the metal-media arc

The non-causal media span (image today, audio-span-capable by construction) serves on the
Metal prefill rail instead of declining to the CPU loop. Closes `followup_general.md` #23's
Metal half and deletes the blob-model vision refusals — a metal-blob decoder gains image
chat outright, since it has no CPU planes to fall back to.

## Ground truth (explored 2026-08-16, three sweeps; citations are current tree)

**Two GPU prefill rails exist; only one is Metal's.** `attention_gpu_prefill`
(`dasllama_blocks.das:691`) is the VULKAN tier chain — no Metal file registers a
`MoeGpuAttnFn`, so `attn_gpu_prefill_ready`'s `start_pos == 0` never binds a Metal build.
Metal's rail is the whole-step prefill override (`metal_prefill_forward`,
`dasllama_metal_prefill.das:2271`), and **continuation is already served there** (M2.5:
`MetalPrefillDecline.start_pos` retired; stored rows `[0, start_pos)` gather host-side into
f32 panels, `:2472-2503`; the parity suite's `cont` arm pins two-chunk GPU prefill per KV
codec). What declines is exactly the span: `prefill_decline`'s FIRST clause
(`s.attn_uniform_end != 0l → non_causal_span`, `:1983-1987`).

**The mask is arithmetic on uniforms, not baked.** All six attention kernels (QK,
QK-mm-t, QK-mm, AV, AV-mm-t, AV-mm) plus softmax share one `AttnArgs` (`:1098-1111`) that
already carries `qoff`, `window` (SWA lower bound), `softcap`, `hass`+sinks. The causal
UPPER bound is the one hard-wired expression — `qoff + qi + 1` — in three places per rail:
QK block-skip (`:1131-1136`, mm twins), softmax `cnt` (`:1204`), AV `jlimit`/`klimit`
(`:1302`, `:1564`, mm twin). AV's bound is correctness, not optimization.

**The CPU contract the GPU must mirror** (the 21-clause list, key ones):
- Upper bound lifts to `uniform_end`; the SWA LOWER bound stays query-based —
  `[sliding floor, uniform_end)`, never `[0, end)` (`dasllama_attn_prefill.das:243-245`,
  `vision_plan.md` doctrine, matches llama.cpp `is_masked_swa`).
- `uniform_end` is always exactly `start_pos + npos` (foreign values panic,
  `dasllama_attn_prefill.das:543-546`) — the kernel-side `uend` needs no independent range.
- One un-chunked call per span; flag set/cleared at `dasllama_blocks.das:1531/:1533`;
  causal-after-span bit-identical to fresh causal (`test_attn_span.das:116-121`).
- Bit-exactness reference is classic/blocked; flash is off-reference by design
  (parity bars 1e-3 / 1e-2, `test_attn_span.das:100-112`).
- KV writeback byte-identical to the CPU store (`:3178-3196` vs `dasllama_blocks.das:807-815`);
  recurrent families have no uniform-bound form and panic (`:1508-1510`) — non-goal.

**Guards that invert, in one change:** the tripwire's exemption conjunct
(`&& s.attn_uniform_end == 0l`, `dasllama_common.das:4760`), the Metal decline
(`dasllama_metal_prefill.das:1985`), the blob vision refusals (`dasllama_chat.das:682-684`,
`openai_server.das:339`), and their tests (`test_cpu_prefill_tripwire.das:42-52` inverts;
`test_metal_support_matrix.das:340-364` flips to the retired-clause pattern of `:335-336`).
The Vulkan decline (`dasllama_gpu_resident.das:728`) and `attn_gpu_prefill_ready`
(`dasllama_blocks.das:762`) are the VULKAN half — see Non-goals.

## Predictions (on record before any measurement)

- P1: a metal-blob 12B + mmproj serves an image turn end to end (today: hard refusal).
- P2: on the planar+metal path, the 130-row cats span evals CPU-side today at roughly the
  CPU pp rate (~64 t/s on M1 → ~2.0 s of the turn); served on Metal it lands near the blob
  mul_mm pp rate — the media-turn TTFT drops by >1 s at 130 tokens, more at 280.
- P3: kernel-unit span arms hold the existing tolerance families (1e-4 non-mm, 2e-2 mm)
  against the one-line-modified CPU oracle.
- P4: losing the QK block-skip over the span region roughly doubles span score work but is
  invisible in wall time at ≤280 rows (GEMMs dominate; score slab `n_heads*mp*nk64*4`).

## Slices

**A — measure the cliff first. DONE 2026-08-16.** The checked-in m1 record rows carry the
rates (12B Q4_K_M: das CPU pp 67.1 / tg 14.8; das metal-blob pp 326.5 / tg 33.4), so the
span arithmetic needed no fresh mint: 130 cats rows = 1.94 s at CPU pp vs 0.40 s at metal
pp; the 280-row ceiling = 4.2 s vs 0.9 s. P2 HELD (predicted ~2.0 s / >1 s saving). The
today-side of P1 reproduced live: `--gpu metal --image-mmproj` dies at boot with the
`openai_server.das:339` refusal ("a metal-blob model … serve it without the metal blob to
take a vision arm") — on Metal there is no slow image path, there is none. The live
before/after turn (same binary both sides) rides slice F, where the one re-mint is
amortized across the before and after cells.

**B — `uend` through the six kernels.** Add `uend : uint` (0 = causal) to `AttnArgs`; edit
the three QK skips, softmax `cnt`, the three AV limits; driver passes
`s.attn_uniform_end` at `:2870-2876` (geometry already right: `npos = start_pos + npos`,
`qoff = start_pos`). Kernel-unit gates FIRST in `test_metal_prefill_kernels.das` — extend
the `attn_trio_gate` shape with a span arm (oracle change is one line: `cnt = qi+1` → the
uniform bound); every hand-bound `metal_set_bytes` site takes the new field; negative
control per compare (poison sized to beat rel·env at the longest dot); tgmem length set
exactly as production. Method per the kernel-lessons checklist: disasm before timing,
lane-owned stores, no divergent guards.

**C — the inversions.** Delete the Metal decline (retire `non_causal_span` the way
`start_pos`/`depth` retired); delete the tripwire exemption conjunct; delete both blob
vision refusals; invert the tripwire test; flip the support-matrix cell. One commit — the
guards and their tests move together.

**D — routing and parity surfaces. DONE.** The `span` arm landed in the prefill parity
suite: the media eval shape (causal head chunk, non-causal embd span at `start_pos > 0`
built from the tail tokens' own embedding rows, greedy decode) — token-exact vs the all-CPU
control per KV codec f16/q8_0/tq4, with both prefill calls asserted GPU-served. Run green.
Three planned sub-items dissolved on inspection, deliberately: the `test_attn_span` GPU
mode row is redundant across the three tiers that now gate (kernel-unit oracle, support-
matrix engage cell, model-level token-exact parity); `arm14-poison` needs no extension (a
span rides the identical panels and driver — there is no new region to collide); and the
census has no new kernel to cover — the same six kernels serve spans via the uniform, which
is the design #23's "dispatches the non-causal arm" criterion predates.

**E — server + stats + demo. DONE (code).** `DASLLAMA_METAL_SPAN` landed (MetalEnv bool,
default true; snapshot into `g_pf_env_span` at the prefill [init]; the A/B decline is the
retired clause's seat, knob-gated; ENVIRONMENT.md regenerated, registry green). On a BLOB
model span=0 is a loud `blob_only_panic` — there is no CPU arm to A/B to; the knob's CPU
arm exists only on planar loads with declared intent. `/v1/stats` gains `media_rows`
(admitted span rows since boot). ⚠ VALIDATION LESSON, recorded: the support-matrix span
cell lives in the `window` ARM, not `cells-q8` — two earlier "greens" were vacuous arm-
filter skips (the loud SKIPPED verdict said so), and the cell also had to move to the BLOB
twin to engage at all. Real proof now: `--arm window` green with an `npos=12` driver line,
and `DASLLAMA_METAL_SPAN=0` reds the same arm (the knob control).

**F — the bench row + docs. DONE, measured.** `lcpp_bench --image` takes `--ngl`: the metal
arm loads the blob flavor and serves the whole turn on the driver, tripwire ARMED. Measured
on the M1 (12B Q4_K_M, cats, same binary/sidecar/fixture, debug-grade cells, `-r 3`):
CPU 53.3 enc ms / 63.6 pp / 14.0 tg → METAL 53.3 / **135.8** / **34.3** — prefill 2.14x,
decode 2.45x, encoder identical (the CPU embedder is chunk 2's subject), captions correct
both arms. Turn-level: a 158-position + ~60-token reply goes ~6.8 s → ~2.9 s. Demo smoke:
the `--gpu metal --image-mmproj` boot that threw the `openai_server.das:339` EXCEPTION in
slice A now boots with `vision = true` and `media_rows` in `/v1/stats`. Mint note: the
first settling mint's kernel half died on a noise refusal (the gate doing its job); the
second completed and both cells gated through it.

## Non-goals (this chunk)

- **Vulkan** (#23's other half): the tier chain needs signature surgery (`MoeGpuAttnFn`
  takes no `start_pos`/span end), device KV pre-seeding, and an `AT_CTX` decision — and
  real-hardware validation lives on zen2 (correctness-only box). Its own chunk.
- **Towers/embedders on Metal** (ASR RTF, the audio-heavy leg): chunk 2 of the arc.
- **Recurrent (deltanet) span** — no uniform-bound form exists CPU-side; the panic is the
  contract.
- **Media prefix-cache / batched image rail** — ledgered behind triggers that haven't fired.
- **Audio-in-chat surfaces** (`input_audio` parts etc.) — ledger #25's conformance tier,
  separate from the kernel work.

## Exit criteria

Parity suite green including the new `span` arms per codec; kernel gates green with
negative controls recorded; tripwire inverted and green; support matrix flipped; census
dispatches the non-causal arm; blob-model image chat demo over HTTP with the measured
before/after in the PR body (cells behind every figure); `img:*` metal row in the records.

---


# Chunk 2 — towers and embedders on Metal (proper plan)

## Ground truth (three exploration sweeps + battery design, 2026-08-16)

**Two encoder implementations, not seven.** One config-driven Whisper-class core
(`audio_encode_blocks` + `encoder_blocks` + `enc_attention` in `dasllama_audio.das`) serves
whisper, qwen2audio, ultravox, voxtral, and qwen3a (family-local conv frontend, shared block
loop). Canary, parakeet, gemma4a are hand-written Conformer forwards sharing only leaf
kernels — scalar conv/GLU modules, and gemma4a's attention is fully scalar with a 12-wide
window. Chunk 2 takes the Whisper-class core; the Conformers are a later chunk with their
own CPU-vectorize-vs-offload call.

**The hard kernels are free.** The mul_mm family serves q8 and bf16 planes zero-copy off a
mapped `.dlim` (the format guarantees 16 KB section alignment; `plane_buffer` +
`bytesNoCopy`), and chunk 1's `uend` IS the bidirectional mask:
`AttnArgs(uend = npos, qoff = 0, window = 0, kv_mul = 1)` — no new attention kernel, and no
head packing either: the tower's `[rows x qd]` activations are exactly the strided layout
the trio reads. Missing, all cheap: LayerNorm w+b (clone `MetalRmsNorm`, two-pass), a dense
f32-weight mul_mm stamp (strip `MetalAttnQKMm`'s head indexing + causal skip — it already
stages f32 x f32), a 1-operand tanh-GELU (the body exists in `MetalGeglu`; Metal `tanh`
NaNs past ~44, the exp-identity form saturates — keep it), im2col/pool/stack (CPU-side
initially), and the gemma4uv 2-axis pos add (~10 lines).

**The prize, priced by the ledger:** gemma4a das encode 16x mtmd (1888 vs 117 ms — deferred
with the Conformers), Omni/qwen3a ~4.8x encode gap, whisper-large-v3-turbo xRT 5.6-11.8 —
all "fp32 scalar tower, no perf pass". The das record rows carry no `encode_ms` (field
postdates the sweep) — slice G re-instruments the split before per-family predictions.

**Precision doctrine.** Metal GEMMs stage f16; the CPU dump oracles (1e-4/5e-3 abs;
gemma4uv tier-1 2e-4) are NOT GPU bars. GPU gates: transcript/text equality
(backend-portable) + envelope-relative numeric arms with DERIVED constants (below). bf16
weights cost nothing extra (bf16 -> f16 is exact in f16's normal range); only the
activation staging rounds (~4.88e-4/element, ~8e-4 per GEMM against row RMS).

**The serving-default caveat (product honesty, stated up front):** `load_asr_model`
defaults to q8 towers. Chunk 2 ships the f32 lane first, so the measured win is available
on `set_asr_fp32(true)` / f32-mmproj loads; the DEFAULT ASR path stays CPU until the q8
tower lane lands (either the interleaved-34B tower blob transform matching `MetalQ8MulMm` +
the off%256 rule, or upload-time dequant to device f16 — decided by measurement in its own
slice, possibly post-PR).

## The driver — `dasllama_metal_tower.das`

- Entry points `metal_tower_encode(t, s, ...) : bool` (Whisper-class blocks) and
  `metal_gemma4uv_encode(...) : bool`, decline-and-return-false contract like
  `metal_prefill_forward`; CPU fallback always correct.
- `MetalTowerDecline` in `dasllama_metal_shapes.das` (portable, PC-answerable): `none`,
  `knob`, `quant_mode` (q8 tower — f32 lane only for now), `graph` (Conformer family),
  `shape` (m%32 / ndim%64 / kdim%32 — no current family violates; gate anyway), `device`,
  `gpu_error`. Declines route through the required-mode panic rail — the tower's tripwire.
- Rides the lens/kn_ rail (no census opt-out — the gemm driver's require-cycle does not
  apply here). PSOs compiled in one `*_init` with a `g_failed` latch; census auto-registers.
- Weights: one `plane_buffer` per plane (zero-copy when `image_map != null`), tensors bound
  at byte offsets; `upload_region` for LN weight/bias rows. Uploaded once, resident.
- Buffers: `g_tw_pool` (tracked activations, sized to ceil64 rows) + `g_tw_upool`
  (untracked uniforms). ONE command buffer, one commit/wait (no KV readback to interleave —
  `ncb` chunking only if encode-CPU shows on 32-block towers); capture rail armed so the hz
  oracle derives barriers.
- Stats: `metal_tower_stats() : (encodes, blocks, rows, declines)` +
  `metal_tower_declines() : table` — every model-level gate asserts COUNTER DELTAS derived
  from geometry (`encodes == N chunks`, `blocks == n_layer * N`), never "the model ran".
- Knob: `DASLLAMA_METAL_TOWER` (MetalEnv bool, default on, [init] snapshot — the SPAN seat)
  + in-process `set_metal_tower(on)` for tests.

## Kernel specs (slice H) — DONE 2026-08-16

Four kernels + four PSO globals landed; all gates green in the 7-file kernels suite.
Controls discriminated: LN drop-bias RED, posadd axis-swap RED (non-square only, as
designed), f32mm weight-row shift RED, gelu constant RED at 1e-2 granularity (finer is
below the gate's 1e-4 bar — stated; the form-delta cell owns the form). Two measured
corrections: the LUT-substitution bound is the f16 HALF-ULP at the range edge (3.90625e-3
deterministic; pinned 4.5e-3 with the measurement in the comment), and the #3724 guard
caught the gate's own 106 MB fixture (reserve added).

1. **`MetalLayerNorm`** — `MetalRmsNorm` skeleton (one tg/row, `simd_sum` + partial[32] +
   broadcast), two-pass mean then population variance (f32 tree-sum; CPU doubles — the
   1e-4 bar has ~100x headroom over the reduction delta), affine `(x-mean)*rsqrt(var+eps)*w+b`,
   in-place-safe (barrier between read and write passes — gemma4uv calls it in place).
2. **`MetalF32MulMm`** — the dense f32-weight stamp from `MetalAttnQKMm`: strip head
   indexing + the causal/uend skip; weight rows `[out][k]`, activations `[m][k]`, both
   staged f32 -> f16 tiles, f32 accumulate; grid m/32 x out/64.
3. **`MetalGelu1`** — 1-operand `MetalEw1T` template beside `MetalEw2T`; the exp-identity
   tanh-GELU body from `MetalGeglu`, in-place, saturation-exact at |x| >= 50.
   The whisper f16-LUT flavor is NOT ported (bit-exactness stays a CPU property; the GPU
   lane is tolerance-gated, with the substitution SIZE pinned by a `gelu_form_delta` cell).
4. Reuse arms: weightless rms = `MetalRmsNorm` + ones vector + meta eps; pos-add = new
   ~10-line 2-axis broadcast kernel; bias/residual = `enc_add_bias_rows` / `enc_add_c`.

## The battery (designed 2026-08-16; the full text is the design-agent record)

- **Unit gates** (kernels suite, arm-less): `layernorm_gate` (9 arms incl. in-place,
  zero-variance, dim 100/384/6912; bar 1e-4 derived; 3 controls incl. drop-bias and
  sample-variance), `f32_mulmm_gate` (f16-exact-stage arm at 2e-3 + rounded-stage arm at
  4e-3 derived from 2xRNE; transpose + sized-poison + sentinel controls; the 2e-2 envelope
  deliberately NOT used at unit level), `gelu1_gate` (+ `gelu_form_delta` pinning the
  LUT/erf substitution at <=1.5e-3/<=1e-3 over [-10,10], logged on green),
  `rms_weightless_gate`, `pos_add_gate` (bit-EXACT, split-exponent fixture, axis-swap
  control on the non-square shape).
- **Embedder gates**: arm `gemma4uv-metal` in test_model_image (5 canvases incl. 672x336
  non-square and a no-pad 384x384; four-statistic helper in _model_tier — rel_elem <= 8e-3,
  rel_l2 <= 4e-3, cos >= 1-1e-5, norm_dev <= 4e-3, all derived from the 2-GEMM f16-stage
  model at 5x headroom, all logged on green); tier-1 ATTEMPT in test_gemma4uv (ladder:
  hold 2e-4 -> else bf16 tensor lane -> else scale-relative 2e-4 + 4e-3*rms with the CPU
  constant untouched); the caption floor in test_vision_chat (forced-feed logits form —
  freeform token parity is banned).
- **Tower gates**: ONE new arm token `mtower`, five family blocks in test_model_image, all
  on the f32 rail both sides; depth-scaled numeric bar `3e-3*sqrt(L)` (whisper tiny at
  6e-3 is the sharp instrument; 32-layer towers at 1.7e-2 are the structural check);
  whisper cells are transcript-exact AND pinned-id-exact vs the whisper-cli list (in-repo
  pins are not freeform caches); qwen3a/ultravox text-equality cells with the
  forced-feed-conversion escape recorded for near-ties.
- **Matrix arm `tower`**: engage / q8-declines (THE consequential cell — the default path
  is untouched) / Conformer-graph-declines / knob / required-mode panic / feint.
- **Knob controls**: ON and OFF legs inside the same arm (the chunk-1 lesson made
  structural); PR-body evidence = `--arm mtower` green with counter lines + the same arm
  red under `DASLLAMA_METAL_TOWER=0`.
- **Census**: `cov_tower` row (whisper tiny f32 + a small gemma4uv canvas) — the new
  kernels appear non-zero.
- **Bench leg** (slice K): `lcpp_bench --asr --ngl` — required metal mode instead of
  `allow_cpu_prefill`, `backend="metal"`, `encode_ms` populated, LOUD non-zero-exit if
  `encodes == 0` or `encode_ms == 0` on a gpu row (anti-sandbag).
- **Stated claims** (9, in the battery record): LUT bit-exactness not claimed; transcript
  equality is corpus-scoped; the win is f32-rail-scoped until the q8 lane; tier-1 2e-4 is
  CPU-only; bars are M1-measured; frontend/tails stay CPU; Conformers out of scope
  (decline-tested only); worker wiring is a design decision; cross-shape determinism not
  contractual.

## Predictions (before slice G ran) and slice G verdicts (measured 2026-08-16, gb1 198.7 s)

- P5: encoder >= 70% of whisper-large wall; blocks >= 85% of encoder. **BOTH HELD, with
  room**: f32 rail encode 102.1 s vs decode 3.5 s (~97% of engine time); FFN GEMMs alone
  68.6 s, conv 1.2%.
- **The q8 CPU context (the product default)**: whisper-large q8 encode 15.7 s — and 85%
  of it (13.3 s) is the f32 attention micro-GEMMs (`gemm_f32` per head-block), which the
  Metal trio serves directly. The q8-CPU tower's bottleneck is attention, not weights.
- P6: Metal block loop >= 4x CPU f32; whisper-large xRT -> >= 12 on the f32 rail. OPEN
  (slice K judges). The bar that matters: beat the q8-CPU xRT 11.8.
- P7 **REVISED by measurement**: qwen3a's encode (~10 s on gb1, f32 AND q8 — identical) is
  ~89% its family conv2d FRONTEND (serial im2col + per-chunk convs), only ~1.1 s in the
  shared blocks. Blocks-only offload buys qwen3a ~11%. The family needs its frontend
  addressed — GPU im2col+mm, or at minimum threading the serial im2col — added to slice J
  as J-qwen3a, judged separately from the whisper-class win.
- P8: the gemma4uv Metal embedder holds the derived envelope (rel_elem <= 8e-3) but NOT
  tier-1 2e-4 on the f16-staged lane; the bf16 tensor lane gets within 2e-4 where crowned.
- P9: the f32-lane tower serve costs no correctness: transcript-exact on the pinned corpus
  for whisper tiny AND large.

Slice G instrument fixes that rode along: the stage probe gained `--lang` (Qwen3-ASR needs
`auto`) and its missing `allow_cpu_prefill()` declaration — chunk 1's tripwire caught the
probe itself on qwen3a's 2597-row decoder prefill.

## Slice I — DONE 2026-08-16 (driver + gates, controls recorded)

`dasllama_metal_tower.das` on the prefill contract (decline/false, counter-delta engage
evidence); one command buffer runs ln1 -> patch GEMM (per-plane f32/bf16) -> bias -> ln2 ->
posadd -> ln3 -> weightless rms -> proj GEMM, pad rows zeroed. Hook = register_gemma4uv_gpu
seam after uv_im2col; umbrella-required under ?das_metal. Borrowed prefill PSOs come up via
the new public metal_prefill_pso_init (per-encode, re-arms after either shutdown seat);
enc_add_bias_rows grew a bias offset (tower biases live inside the model blob).
- Gates green: `gemma4uv-metal` arm (5 canvases; rel_elem 2.7-4.0e-3 vs bar 8e-3, rel_l2
  <= 1.9e-4, cos 1-2e-8, norm_dev <= 8.3e-5, counters exact both legs); tier-1 GPU attempt
  (all fixtures held the SCALE-RELATIVE rung, maxdiff ~4.4e-3 — P8 CONFIRMED: strict CPU
  2e-4 does not hold on the f16-staged lane, so slice J inherits the staged GEMM with
  tolerance bars, not element-exactness); vision-chat tower legs (engage + knob-off floor).
- Controls: rms-stage drop -> parity arm red (rel_elem ~144, cos-cell red too) AND tier-1
  rung red (excess 276+) while the pinned CPU gate stayed green (the pin is real); knob
  no-op -> exactly the two OFF-leg cells red. All reverted.
- The CPU tier-1 gate now pins the tower off explicitly — the facade's require graph
  registers the hook, so an unpinned "CPU" gate would silently measure the GPU.

## Slice J — blocks driver + battery DONE 2026-08-16 (bench legs = slice K, docs = L)

metal_tower_blocks: encoder_blocks' loop as one command buffer; the attention trio rides
uend = npos (chunk 1's span machinery IS the tower attention); q8 towers decline
(quant_mode — the serving default untouched); capability declines honor MetalMode.required
(knob = policy). Pad-row garbage contained by kernel geometry (comment in the driver).
- **The form-matched-GELU correction (measured, replaces H's substitution design)**: the
  single tanh-form GPU GELU redded qwen2audio's 32-layer parity at 4e-2 rel_l2 /
  cos 0.99921 — a ~1e-3/element form delta amplifies COHERENTLY through a deep residual
  stream. MetalGelu1 is retired for two form-matched flavors picked per t.gelu_tanh:
  MetalGeluErf (A&S 7.1.26; measured 2.5e-7 vs the CPU's double erf) and MetalGeluLut
  (clamps + f16 round-trips in kernel; measured 1.2e-7 = one f32 ulp). Post-fix:
  qwen2audio 32L rel_l2 2.2e-3, voxtral 32L 1.9e-3 (depth bar 1.7e-2, ~8x headroom).
- tower_parity gained the depth arm (rel_l2/norm <= 3e-3*sqrt(L), cos 1e-4; rel_elem
  LOGGED not asserted — attention near-ties diverge O(1)); the parity mel fixture is
  varied (all-ones is degenerate: every attention weight near-tied).
- Gates green: mtower arm — whisper tiny (4L) AND large-v3-turbo (32L) transcript-EXACT
  CPU vs GPU on jfk, qwen3a f32-rail transcript equality (conv2d frontend CPU + blocks
  GPU), q8-decline cell (zero dispatches, quant_mode counted, CPU still serves),
  required-mode panic cell, parakeet Conformer-absence cell (counters frozen — no seam);
  test_encoder_blocks_gpu (qwen2audio + voxtral 32L parity + counters); census cov_tower
  row added (whisper tiny f32 chunk + gemma4uv 96px canvas). P9 CONFIRMED (3 families
  transcript-identical). Controls: ln2-reads-ln1 poison -> transcript degrades to garbage,
  exactly the transcript cell reds; the gelu-form red-then-green is the parity cell's
  recorded control.
- Deviations from the battery text: no separate matrix `tower` arm — the tower's
  engage/decline axes are family-shaped, so all cells live in the mtower arm + test_audio
  (where the family modules already are); ultravox text-equality is served by the
  qwen2audio-core parity + voxtral cells (the ultravox chat surface would add a decoder
  load for no new tower coverage); the whisper-cli pinned-id list is DEFERRED until minted
  from whisper-cli itself (never invented) — transcript equality CPU-vs-GPU is the bar.
- Existing-test pins that rode along: test_audio's three mtmd oracles + test_gemma4uv's
  CPU tier-1 pin the knob OFF (the umbrella hook would silently flip them to GPU).
- J-qwen3a (the conv2d frontend, ~89% of that family's encode) — OPEN, judged separately.
- Lens finding (reported): the kernel-access classifier's intrinsic table lacks scalar
  converts (float16(x) on a buffer-derived scalar mis-banks as whole-buffer); the
  [never_inline] escape on MSL helper bodies is the workaround, and the block-annotation
  inline asymmetry is filed as daScript #3752.

## Slice K — bench legs + the A/B numbers DONE 2026-08-16 (M1, debug-grade -jit cells, -r 3)

`lcpp_bench --asr --ngl`: the f32 tower rail under set_asr_fp32, backend stamped "metal",
per-row anti-sandbag (a measured row with zero tower encodes or zero encode_ms exits
non-zero — a Conformer row under --ngl is a refused measurement, not a silent CPU row);
decoders/Conformer paths keep declared CPU intent.

Whisper large-v3-turbo, best-of-3 per clip, same binary/box (q8-CPU default vs f32-GPU):
jfk 1969 -> 910 ms; jfk3 4031 -> 2019; gb1 16982 -> 9319; hp0 24503 -> 12851;
hp0x2 47266 -> 25187. GPU xRT ~21.3 on the long clips vs the q8-CPU 11.7.
- P6 HELD, all three bars: block loop 3.03 s GPU vs ~95 s CPU f32 on gb1's 8 chunks
  (~31x; bar 4x); whisper-large f32-rail xRT 21.3 (bar 12); the q8-CPU serving default
  (xRT 11.7) beaten 1.82x END TO END — the f32-lane win now reaches the product default
  path, closing the "doesn't reach q8 serving" caveat for whisper-class.
- Post-fix split (gb1): encode 4.35 s (blocks.gpu 3.03, conv 1.32) vs decode 3.50 +
  cross_kv 1.65 — the decoder is now the larger half (out of scope, ledgered with
  J-qwen3a as the follow-up perf items).
- Whisper tiny f32-GPU: gb1 1.27 s (xRT 157), hp0x2 3.12 s (xRT 175).

## Record-grade re-profile (released exe, winners baked; M1, best-of-3, 2026-08-16)

All four legs exit 0 (the gpu legs' anti-sandbag live). ASR whisper large-v3-turbo:
q8-CPU gb1 16.97 s / hp0x2 47.3 s; f32-GPU 9.38 s / 25.2 s — the debug-grade pair confirmed
within noise. Image turn (12B Q4_K_M + cats): CPU enc 11.6 ms / pp 60.1 / tg 14.2 -> GPU
enc 10.2 ms / pp 139.1 / tg 34.4 — pp 2.31x, tg 2.43x, and the EMBEDDER encode itself is
now 10.2 ms vs chunk 1's 53.3 ms CPU figure (the tower serves it, ~5.2x).

## Sequencing (each gate green before the next)

G stage-probe baselines (whisper-large + qwen3a; predictions P5 judged) ->
H kernels + unit gates (controls recorded) -> statistic helper ->
I gemma4uv driver + gates (tier-1 ladder decides the GEMM lane J inherits) — DONE ->
J tower driver, whisper tiny first, then large + the other four — DONE (cells in mtower,
not a matrix arm; see the slice J deviations note) ->
census row -> K bench legs + the A/B numbers (only after every gate is green) — DONE ->
L docs, ENVIRONMENT.md regen, CLAUDE.md arm lists (same commit as the arms) — DONE
incrementally: ENVIRONMENT.md regenerated with the tower knob (slice I), the arm lists and
suite-less ledger entries rode each battery commit, the plan carries every slice record;
website perf figures stay embargoed per the standing rule.

## Non-goals (chunk 2)

Conformer families (canary/parakeet/gemma4a — decline-tested, gap split unmeasured);
the q8 tower lane (own slice, possibly post-PR — the transform-vs-upload-dequant call is
measurement-driven); server `asr_workers` GPU wiring (per-context metal state — map-and-wrap
or a device-owning worker, a design decision deferred with #25's audio-chat surfaces);
mel/FFT + im2col/pool/stack on GPU (CPU seams); whisper LUT-GELU bit-exactness on GPU.

# Chunk 3 — the ASR path GPU-complete (Boris 2026-08-16: "the idea is to have entire
# thing on metal" — kill the CPU-GPU-CPU sandwich; decoder first, then the conv frontends;
# the q8/f16 GPU lanes stay deferred)

Wall today (gb1, record-grade f32-GPU 9.38 s): encode 4.35 s (blocks.gpu 3.03 + conv 1.32)
+ decode 3.50 s + cross_kv 1.65 s. whisper-cli Metal q8 runs the same clip in 5.80 s — the
remaining gap IS the CPU decoder + frontends (encoder blocks are at parity).

Ground facts the design leans on (scouted 2026-08-16):
- The whisper decoder is a BESPOKE loop (`whisper_decode_batch`, dasllama_whisper.das) with
  no Session/Model — the TOWER hook pattern fits, not `DecodeOverrideFn`. qwen3a and canary
  decode through the LLM path and already inherit the metal decode override.
- The decoder serves Q8-NATIVE on GPU (Boris ruling 2026-08-16, after the whisper.cpp scout:
  whisper.cpp serves q8_0 decoder mms on Metal INCLUDING the tied-embedding logits GEMV —
  decode is GEMV/weight-bandwidth-bound, so an f32 lane concedes ~4x on the dominant
  per-token term). The dense compact q8 planes (`dqblob`/`dqscales` from
  `whisper_dec_quantize`) are the upload; f32 stays the numeric-oracle rail. Numerics note:
  CPU q8 decode is q8-weight × q8-ACTIVATION integer dots (`matmul_q8q8_batch`), the GPU
  lane is q8-weight × f32-activation (whisper.cpp's regime) — transcripts vs the CPU q8
  serving default are the gate, the f32 rail is the logits rung; a near-tie flip is judged
  with fixtures, not papered over.
- `whisper_cross_kv` is once per 30 s window: per layer two `[1500×d]×[d×d]` GEMMs + the kx
  transpose (pre-scaled hs^-0.25) + the vx t-major copy — tower-shaped work.
- Logits MUST return to CPU each token: the filter suite (suppress/timestamp rules) needs
  2-token history + `has_ts`/`seek_delta`, then double argmax. One n_vocab f32 readback per
  token is the design; GPU-side filtering is a non-goal.
- The decoder MLP is `gelu_tanh_lut` — the SAME LUT form chunk 2's MetalGeluLut form-matched
  (1.2e-7). The form-matched-GELU law is already satisfied by an existing kernel.
- Every kernel a decoder step needs is public (`enc_ln` with bias, `enc_f32_mm`, `enc_add`,
  `enc_add_bias_rows`, `enc_gelu_lut`, `enc_softmax`) EXCEPT attention against whisper's
  pre-transposed K layouts (kcache `[H][hs][Tmax]`, kx likewise) — new small kernels or a
  transposed-K variant of the qk/av pair; decided at the prototype, measured not argued.

## Predictions (chunk 3 — on record before slice M's probe)

- P10 (revised with the q8-native ruling, still pre-measurement): gb1 9.38 s → ≤ 5.7 s
  after decoder+cross_kv (N+O), ≤ 4.8 s after the conv slices — das (f32 tower + q8
  decoder) PASSES whisper-cli's q8-Metal 5.80 s before the q8 tower lane even lands.
- P11: cross_kv 1.65 s → ≤ 0.15 s (the GEMMs are tower-shaped; the transposes ride along).
- P12 (revised for q8 weights): decode 3.50 s → 0.9–1.4 s (per-token command-buffer
  round-trip keeps it off the pure-bandwidth floor; our fixed-sequence driver skips the
  per-token graph-rebuild cost whisper.cpp pays, so the overhead share should come in
  UNDER theirs; encode-ahead is the known escape, ledgered not built).
- P13: qwen3a encode ~10 s (gb1) → ≤ 2 s (the frontend ~8.9 s → under 1 s; blocks stay 1.1).
- P14: wh.mel is < 5% of the remaining wall on gb1 — if wrong it becomes a ledger item,
  never silent scope creep.

## Slices (each gate green before the next)

- **M — buckets + baselines.** qwen3a's `q3a.encode` is ONE bucket (the ~89% was an ad-hoc
  slice-G measurement) — split conv/blocks/projector. Whisper decode: verify/extend the
  step splits (self-attn / cross-attn / mlp / vocab GEMV vs the existing `wh.dec.logits`),
  and get `wh.mel` a number. Fresh stage-probe baselines: large-v3-turbo (gb1 + hp0x2),
  tiny, qwen3a; token counts per clip recorded (the per-token denominators). P-judgments.

## Slice M record — DONE 2026-08-16 (M1, debug-grade -jit probe, ONE profiled rep; ran on
## fallback stamps — the sidecar predates the rebuilt binary — shares are the product and
## the absolutes cross-check slice K within noise)

Buckets added: `q3a.mel`/`q3a.conv`/`q3a.blocks`/`q3a.proj` + the `q3a.conv.im2col`/`.mm`
split inside `conv2d_s2`. The whisper decode splits already existed
(`wh.dec.self`/`.cross`/`.ffn`/`.logits`).

gb1, large-v3-turbo, **f32 arm (the GPU serving mode)**: encode 4243 ms (blocks.gpu 3037,
conv 1201) / decode 3551 (logits 1166 over 531 calls = 2.2 ms/call, ffn 1063, self 665,
cross 623) / cross_kv 1599 / mel 56. Filter+sample+embed residue ≈ 34 ms total =
0.06 ms/token — the CPU sampler is FREE, the readback design confirmed. 523 sampled tokens
(531 decode calls − 8 window prompts).

gb1, large-v3-turbo, **q8 arm (the CPU serving default)**: decode 1537 (cross 431, ffn 404,
logits 349, self 318) / cross_kv 191 / conv 267.

**FINDING F1 — the GPU serve's decoder cost is 2.3x self-inflicted.** `set_asr_fp32` is
model-wide, so `--ngl` serving forces the CPU decoder AND cross_kv to f32: 3.55 + 1.60 =
5.15 s where the q8-CPU default pays 1.54 + 0.19 = 1.73 s — ~3.4 s of the 9.38 s GPU serve
is the enc/dec coupling artifact, not GPU-vs-CPU. Consequences: (a) slices N+O must beat
the q8-CPU 1.73 s, not the 5.15 s in the serve; (b) decoupling the halves (f32 tower +
q8 CPU decoder) takes the serve to ~6.0 s with ZERO new kernels — folded in as the natural
first step of N/O, since the hooks land on the q8 decoder anyway; (c) the GPU decoder's
marginal win over q8-CPU decode is ~0.5–0.7 s on gb1 — the knob split is half the chunk's
e2e win, stated honestly. GPU decode still carries the arc's point (all-Metal, frees the
CPU, and non-turbo large-v3 has 8x the decoder layers; tiny's decode is 36% of ITS wall —
logits alone 42% of tiny's decode).

hp0x2, large-v3-turbo, f32 arm: encode 12107 (blocks.gpu 8683, conv 3409) / decode 8263
(logits 2766, ffn 2454, self 1547, cross 1417; 1189 tokens over 23 windows) / cross_kv 4705
/ mel 145. Everything scales with the window count (23 vs gb1's 8): per-token decode
6.8 ms both clips, bucket sums match slice K's e2e wall (25.25 vs 25.2 s; gb1 9.45 vs
9.38) — the rail accounts for the whole transcribe.

## Slice N0 record — the per-half fp32 knob DONE 2026-08-16 (F1's fix)

`set_asr_tower_fp32` (encoder/tower half only: whisper enc, the parakeet bin, the canary
encoder; `set_asr_fp32` = both halves, wins when both set; GGUF facade towers stay q8) +
per-half `load/stage/mint/stream_whisper_model` overloads (one-flag forms forward) + the
third family tag `whisper-f32-q8` (enc-q8/dec-f32 panics — no serving mode). The `--ngl`
bench leg now serves the mixed mode; the probe gained `--fp32-tower`. Measured (probe, gb1,
turbo, debug-grade): the mixed serve sums to **6.01 s vs 9.38 all-f32** — decode 1.59
(was 3.55), cross_kv 0.19 (was 1.60), encode unchanged — F1's ~6.0 s call EXACT, and das is
at whisper-cli parity (5.80) before any new kernel. Tiny decode 476 ms (was 826). Gates
green: the knob cell (test_whisper — mixed vs q8-default token-identical on jfk, exec_fmt
"f32/q8", 35/0) and the mixed-flavor roundtrip leg (image suite `--arm whisper`, 24/0 —
dq planes element-exact streamed-vs-staged, enc.fblob full-layout exact, facade .dlim tag
route).

## Slice N record — cross-KV on the GPU DONE 2026-08-16

`dasllama_metal_asr_dec.das` (new module; umbrella `?das_metal`, .das_module registered):
the whisper cross-KV per-window computation on the tower contract — per layer the ck/cv q8
GEMMs over encoder rows through `enc_gemm_mm_b`, then two NEW shuffle kernels
(`MetalCrossKx` transpose+prescale, `MetalCrossVx` re-tile; PSOs in the registry pair,
bit-exact unit gates in the kernels suite with NEGATIVE CONTROLS recorded — the poisoned kx
oracle's mismatch ratio was exactly kscale). The decoder's dq planes upload ONCE through
`q8_region_to_metal_blob` (dasllama_layout: un-repacks the CPU backend's grp<mr> interleave
+ interleaves to the 34B blob, so the TUNED q8 GEMM/GEMV families serve every decoder-side
matmul — slice O rides the same attach). Knob `DASLLAMA_METAL_WDEC` + `set_metal_wdec` +
`MetalWdecDecline` (q8-NATIVE: the f32 oracle rail declines quant_mode); counters in
metal_common (windows + declines-by-reason).

**Measured (probe, gb1, turbo, debug-grade): cross_kv 191 → 63 ms (f32-CPU paid 1600) —
P11 HELD; the mixed serve bucket sum lands at 5.86 s ≈ whisper-cli's 5.80 before slice O.**

FINDING F2 — the alignment trap the gates caught: whisper tiny's ODD vocab (51865) leaves
`dqlayers_off` only 128-byte-aligned, so binding at CPU element offsets fails the 16B rule;
the first driver declined `shape` on tiny and the mtower window-delta assert caught it while
transcript equality stayed vacuously green (both legs CPU). Fix: the attach lays regions out
at DRIVER-CHOSEN 16B-aligned destinations (`g_wd_boffs`) — the GPU blob owns its layout, CPU
element offsets stay CPU-only. Gates green after: mtower 28/0 (the new
`test_whisper_metal_cross_kv`: transcript equality GPU-vs-CPU cross-KV, window deltas,
knob/quant_mode declines, required-mode both classes, shutdown re-arm), test_whisper 35/0
(wdec pins on the CPU-claim cells — the hook-flip audit), kernels suite 2/0. The cov_tower
census chunk went MIXED (f32 enc + q8 dec) so one transcribe covers both drivers.
Hook-flip pins landed: q8_gate + tower_fp32_knob (test_whisper), the tower cell
(test_model_image — its required-mode policy leg transcribes an f32 decoder, which wdec
would panic).

## Slice O record — the decode step serves on the GPU DONE 2026-08-16 (perf verdict OPEN)

The step driver landed q8-native and CORRECT on the first live run (predicted 60% first-run
failure — happy miss): `register_whisper_decode_gpu` at the `whisper_decode_batch` seam, one
command buffer per batch (CPU embed in, full-vocab logits readback out), window-granular
ownership (a decline at cross_kv leaves the whole window to the CPU chain, which reads the
ds.kx/vx readback; mid-window GPU failure PANICS — a silent fallback would decode blind on
the GPU-only caches). Transcripts byte-identical CPU-vs-GPU on tiny and turbo (gb1 + jfk);
engage evidence = steps/tokens counter deltas, strict-hazard runs clean.

New machinery: the fused single-query attention `MetalWdecAttn` (tgmem scores, two-pass
softmax, per-j coalesced V sum; q bias folded at load; one kernel serves self (causal,
len0+r) and cross (fixed len)); `MetalCrossVx` grew the KV-APPEND form (t0/tstride/soff/
folded bias — the same shuffle serves cross-KV residency and the per-step append, and the
fused-QKV column pick); `MetalBiasAddRes` + `MetalBiasGeluLut` (form-matched LUT flavor);
the attach lays q/k/v contiguous so n=1 QKV rides ONE row-parallel GEMV over 3d rows; the
tied-embedding logits ride the tuned `enc_gemv` (te region padded to 64 rows — the row-group
grid truncates odd-vocab tails). Unit gates for all five with negative controls recorded.

**The optimization ledger (turbo gb1, 517 steps, GPU-busy via the metal_wdec_times lens):**
M-pad-32 GEMM tiles 2593 ms → concurrent encoder + hz oracle 2523 (−3%: the chain's
barriers are ~93% REAL — RAW-dependency floor) → row-parallel GEMVs 1647 (−35%) →
dispatch-count fusion batch 1612 (flat: the removed elementwise ops were not the cost).
Host encode is 22–35 ms TOTAL — everything is GPU-timeline latency: ~65 serialized
barrier-segments/step × ~42 µs.

**FINDING F3 — the calibration that frames the verdict:** whisper-cli (same box, turbo q8
Metal, gb1) prints decode 1.63 ms/single-step and 0.50 ms/beam-5-batched-step at the SAME
graph node count — sub-2 ms/step is PROVEN reachable, so our 3.1 ms is kernel-level slack,
not an architecture wall. Today: our GPU decode ≈ 1.9 s wall vs the CPU q8 decoder's 1.54 s
(tiny: 1.5 s vs 0.48 — small decoders lose hard to the latency floor). Their encode is
3.38 s vs our 3.0 s GPU blocks — we already win encode; their conv rides the GPU (slice P
closes exactly that gap). Their default is beam-5 (2457 sampled tokens vs our greedy 523) —
quality regimes differ in their favor on the same wall.

## Slice O2 record — the tuning round DONE 2026-08-16 (format parity, then the real term)

Boris's method call: verify FORMATS and SHAPES against whisper.cpp before calling anything
"tuning". The comparison: shapes identical (same model), weights identical (Q8_0 34B blocks
both sides); ONE format difference — their KV/cross memory is f16, ours was f32. The round,
each step measured on turbo gb1 (GPU-busy, 517 steps):
- f16 resident K/V (format parity; the f32 ds.kx/vx readbacks stay for the CPU fallback):
  1612 → 1470 ms (−9%), transcript intact.
- `DASLLAMA_METAL_GEMV_TG` sweep 2/4/8/16 → 1470/1470/1480/1498 — the default 4 is right,
  the row-group was NOT the term.
- Knockout attribution: the chain minus attention runs 1.68 ms/step — attention was ~41%
  at ~145 µs/dispatch (20 threadgroups = the occupancy wall).
- **The part/comb chunked attention (256-t online-softmax partials + log-sum-exp merge, the
  sq_attn house pattern): 1470 → 709 ms — 1.37 ms/step, UNDER whisper-cli's single-row
  1.63 ms.** e2e stage probe: decode_total 966 ms (CPU q8 was 1537) — the GPU decoder now
  WINS 1.6x, and the gb1 bucket sum is **5.33 s vs whisper-cli's 5.61** (greedy vs their
  beam-5; our conv is still CPU — slice P).

SHIPPED POSTURE: `DASLLAMA_METAL_WDEC_STEP` default ON with a serving floor
`n_text_state >= 1024` (measured: d=1280 wins 1.6x, d=384 tiny still loses to its CPU —
the `small` decline is policy-class, cross-KV still serves; `set_metal_wdec_step_min_d` is
the test seat). The lcpp `--asr` CPU rows pin the driver OFF (a CPU row means CPU — the
bench-level hook-flip trap). Gates: the pc pair's unit cells with a recorded control, the
floor legs in the wdec model gate, f16 fixtures in the attention gates.

## Slice P record — the whisper conv frontend on the GPU DONE 2026-08-16

`MetalIm2col3` (k=3 "same"-pad, one kernel for both layouts: chmajor mel for conv1, row-major
y1 for conv2, stride 1/2) + the existing `enc_f32_mm`/bias/GELU-flavor/`enc_add` chain as
one command buffer in the tower driver; `register_tower_conv_gpu` at the top of
`audio_encode_blocks` (BEST-EFFORT inside the tower knob: a shape misfit — tiny's 3x80
im2col width fails kdim%32 — silently keeps the CPU conv while the blocks still ride the
GPU; no decline-counter noise, `convs` in metal_tower_stats is the engage evidence).

**Measured (turbo gb1, stage probe): enc.conv 1229 → 33 ms (37x). The gb1 bucket sum lands
at 4.11 s — encode 2.99 (vs whisper.cpp's own 3.38), decode 1.01, cross_kv 0.06, mel 0.055.
Everything except the 55 ms mel is on Metal.** P10's post-conv bar (≤4.8) beaten.

Gates: im2col unit cells (both layouts, both strides, odd tail + pad edges; control: the
dropped pad shift redded 18 cells), the tower cell's convs-delta asserts (large = 1 serve,
tiny = 0 with the carve-out named), mtower 28/0 transcript-exact with the GPU conv in the
chain. qwen2audio/voxtral (nm=128, d fits) now conv-serve through the same hook — the
blocks-parity suite re-run gates them.

## Slice Q record — the qwen3a conv2d frontend on the GPU DONE 2026-08-16

`MetalIm2col2d` (k=3 s=2 p=1, conv2d_s2's exact walk, K-padded rows + strided source) + the
f32 TILE GEMM via a driver-owned PADDED weight buffer (`q3a_wpad_attach`: out-channels
480 → ncp 512 for the ndim%64 grid, conv1's K 9 → 32 for kdim%32, biases zero-padded —
minted once per tower, ~18 MB). One command buffer per WINDOW (all chunks × three stages);
the shuffle/conv_out tail stays CPU; strided readback slices nch of ncp per row. Hook
`register_qwen3a_conv_gpu` — and because the conv planes are f32 regardless of the q8 tower,
this serves the **q8 SERVING DEFAULT** with no mode change.

FIRST-CUT LESSON (measured, kept as the record): a naive one-thread-per-output conv GEMM
only bought 24% — O(rows·cout·kdim) traffic with zero weight reuse; the fix was never a
custom kernel but PADDING the shapes into the existing tile GEMM's grid.

**Measured (qwen3a gb1, q8 default): conv 8617 → 347 ms (25x); encode 9838 → 1559 ms
(6.3x) — now blocks 794 (q8-CPU) + mel 382 + conv 347 + proj 20. P13 (≤2 s) beaten.**
followup #26's J-qwen3a closes.

Gates: im2col2d unit cells (K pad, strided source, odd grid; control: the dropped pad shift
redded 18 cells); the mtmd-oracle cell (`test_qwen3a_tower`) pins the CPU conv — its ~1e-4
reorder bars are CPU claims (the hook-flip discipline); the q8-vs-f32 transcript A/B and the
mtower qwen3a cell ride the knob symmetrically.

## Slice R record — the f32-fallback compliance audit DONE 2026-08-16 (zero fixes)

Every call site of the slow-f32 GEMM family classified per the REVIEW rule; the tree is
ALREADY compliant — the q8 enrollment arcs wrapper-ized the weights-GEMMs, and this chunk
GPU-served the two real offenders the slice-M probe named (the qwen3a conv GEMMs, the
whisper conv). The classification:
- **q8-wrapper fallback arms** (the rule's sanctioned shape): `cn_mm`/`cn_mm_rq` (canary),
  the parakeet enc wrapper, `g4a_mm` x2 (gemma4a), `q3a_mm_rq` (qwen3a), `tw_mm`/`tw_mm_rq`
  (towers), `wm_mm` (whisper dec), the `wblob` arm in dasllama_common.
- **No faster twin exists** (unquantized planes / K under 32): the gemma4uv embedder's CPU
  fallback, whisper conv1 (its fill job carries NO `gemm_n` — deliberately not a quantizable
  region), the qwen3a conv2d CPU fallback (GPU-served now), canary's c0 conv (K=9), the
  parakeet joint_net (its WRITTEN carve-out: "joint_net stays fp32, a few % of encode"),
  the MoE router against the f32 aux blob (bit-exact-routing comment in place).
- **Activation math — no quant twin as a class**: every `gemm_f32` site (the whisper CPU
  decoder attention — now the GPU step's fallback; the q8-tower CPU attention micro-GEMMs;
  prefill attention; the deltanet chain; the FFT twiddle GEMMs).
- **Oracle rails**: the QuantMode.fp32 / `set_asr_fp32` paths, by definition.
The q8-tower CPU attention (85% of a q8-CPU encode) stays the one big LEDGERED f32 cost —
rule-compliant (no twin exists), GPU-served under `--ngl`, a PERF ledger item for the CPU rail.

**qwen3a, gb1, q8 default**: encode 9838 = conv 8617 (88% — the slice-G ad-hoc figure
confirmed) + blocks 798 + proj 21 + mel 384. Conv split: **mm 7914 (92%) vs im2col 650
(7.5%) — PREDICTION MISS** (called im2col ≥ 60%): the wall is the three f32 `mm_blob_b`
conv GEMMs (K = 9·n_ch shapes), the exact f32-fallback pattern the new REVIEW rule names.
Slice Q design update: the GEMM offload is the win; im2col threading is a rider.

P-judgments so far: P14 HELD (wh.mel 0.32% of the gb1 wall; q3a.mel 3.9% of its encode —
under the bar). P11's bar RESTATED vs the q8-CPU 191 ms — the raw win is marginal; GPU
cross_kv's real point is kx/vx residency for the GPU decoder. P12's bar restated vs
1.54 s q8-CPU.
- **N — cross_kv on GPU.** `register_whisper_cross_kv_gpu` (tower contract: decline/false,
  counter deltas). Per window: upload enc_out once, per layer two q8-weight × f32-act GEMMs
  (the ck/cv planes are decoder q8) + bias + the kx transpose-scale kernel + the vx t-major
  copy, outputs GPU-resident AND read back (the CPU decoder still runs until O; after O the
  readback drops). enc_out re-upload residency (tower writes s.x back to CPU) is a ledger
  item, measured not assumed.
- **O — the decoder step driver, q8-native.** `register_whisper_decode_gpu` at the
  `whisper_decode_batch` seam; own knob + `set_` twin (decline policy differs from the
  tower's). GPU-resident mirrors: the dense compact q8 planes uploaded once (repack-at-
  upload into an existing q8 kernel format vs a new GEMV over the compact layout — an open
  decision, prototyped not argued), kcache/vcache on GPU (Tmax=448, appended by kernel),
  kx/vx resident from N. Per token: CPU embed (two-row add) + d-float upload, ONE command
  buffer through the layer stack (LN → QKV q8 GEMV → KV append → self-attn over npos →
  out proj → cross-attn vs kx/vx → MLP with enc_gelu_lut → final LN → q8 vocab GEMV against
  the tied embedding), logits readback (~n_vocab f32; the sampled id is the only HARD
  per-token dependency — the full-logits read is a unified-memory memcpy and keeps the
  CPU filter/sampler as the parity anchor, whisper.cpp's own regime). Prompt rows (3–4,
  once per window) loop the same driver. Gates: transcripts vs the CPU q8 serving default
  (tiny + large, jfk + gb1), logits rung vs the f32 oracle rail, engage census, hook-flip
  audit on every test reaching the seams (the chunk 2 trap), decline battery
  (required-panic, absence, f32-forced serve declines to CPU or serves — decided at O).
- **P — whisper conv frontend.** GPU im2col (k=3, s=1 and s=2) + `enc_f32_mm` + bias +
  GELU + posadd (`enc_posadd2d` exists); kills the 1.32 s conv bucket. Mel stays CPU (P14).
- **Q — qwen3a conv2d frontend.** GPU im2col for `conv2d_s2` (stride-2 pad-1, c-fastest
  rows) ×3 convs + the feature shuffle + conv_out GEMM + bias + `enc_gelu_erf` + pos-add;
  batch the per-100-frame chunk loop into few command buffers. The projector/blocks path
  is untouched (already GPU).
- **R — the f32-fallback compliance audit (Boris 2026-08-16: "audit where we do similar
  silly stuff and make compliant with new ruling").** Every call site of the slow-f32 GEMM
  family (`matmul_batch`, `mm_blob_b`, per-head `gemm_f32`) across the module, classified
  three ways per the REVIEW rule: parity/oracle rail (stays), faster twin exists for the
  same weights (FIX — route it), no faster twin (comment or out of scope). The audit table
  lands in this plan; fixes land before S so the bench measures the compliant tree.
- **S — bench + docs.** `lcpp_bench --asr --ngl` re-run (anti-sandbag live), record-grade
  released-exe numbers, the whisper-cli comparison re-quoted, followup #26 updated,
  ENVIRONMENT.md regen (new knob), arm lists, REVIEW companions.

## Open design decisions (named before O starts)

- Driver home: a new `dasllama_metal_asr_dec.das` (registered in BOTH .das_module and
  CMakeLists) vs growing dasllama_metal_tower.das — decided by size; either way new kernel
  classes compile/release in the registry pair (the chunk 2 PSO-ownership rule).
- The attention kernel shape: transposed-K variants of enc_qk_mm/enc_av_mm vs dedicated
  n=1 GEMV-shaped kernels — prototype both ways only if the first measures short.

## Non-goals (chunk 3)

The q8 TOWER lane (deferred by ruling — "2 can wait"; the DECODER is q8-native in scope
per the later ruling); f16 KV mirrors (whisper.cpp precedent says transcript-safe — a
measured flip, ledgered); Conformer families (canary/parakeet decode through NeMo-ish/LLM
paths); beam/temperature fallback (greedy stays greedy); GPU-side logit filtering (the
sampled id is the only hard per-token readback; full logits are free on unified memory and
keep the CPU sampler as the parity anchor); mel/FFT on GPU (whisper.cpp REMOVED its GPU
mel); server asr_workers GPU wiring (deferred with #25); enc_out GPU residency across the
tower→cross_kv seam (ledger, measure first).
