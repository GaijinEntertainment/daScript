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

## Kernel specs (slice H)

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

## Predictions (before slice G runs)

- P5: encoder >= 70% of whisper-large-v3-turbo transcription wall on M1; blocks >= 85% of
  encoder time.
- P6: the Metal block loop is >= 4x the CPU f32 block loop at T=1500/d=1280 (mulmm-class
  GEMMs dominate); end-to-end whisper-large xRT 5.6 -> >= 12 on the f32 rail.
- P7: qwen3a (18 layers, hs 64) closes its 4.8x encode gap to <= 2x on the f32 rail.
- P8: the gemma4uv Metal embedder holds the derived envelope (rel_elem <= 8e-3) but NOT
  tier-1 2e-4 on the f16-staged lane; the bf16 tensor lane gets within 2e-4 where crowned.
- P9: the f32-lane tower serve costs no correctness: transcript-exact on the pinned corpus
  for whisper tiny AND large.

## Sequencing (each gate green before the next)

G stage-probe baselines (whisper-large + qwen3a; predictions P5 judged) ->
H kernels + unit gates (controls recorded) -> statistic helper ->
I gemma4uv driver + gates (tier-1 ladder decides the GEMM lane J inherits) ->
J tower driver, whisper tiny first, then large + the other four; matrix `tower` arm ->
census row -> K bench legs + the A/B numbers (only after every gate is green) ->
L docs, ENVIRONMENT.md regen, CLAUDE.md arm lists (same commit as the arms).

## Non-goals (chunk 2)

Conformer families (canary/parakeet/gemma4a — decline-tested, gap split unmeasured);
the q8 tower lane (own slice, possibly post-PR — the transform-vs-upload-dequant call is
measurement-driven); server `asr_workers` GPU wiring (per-context metal state — map-and-wrap
or a device-owning worker, a design decision deferred with #25's audio-chat surfaces);
mel/FFT + im2col/pool/stack on GPU (CPU seams); whisper LUT-GELU bit-exactness on GPU.
