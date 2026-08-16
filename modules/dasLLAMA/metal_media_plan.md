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

# Chunk 2 — towers and embedders on Metal

## Ground truth (three sweeps, 2026-08-16)

**Two encoder implementations, not seven.** One config-driven Whisper-class core
(`audio_encode_blocks` + `encoder_blocks` + `enc_attention`, `dasllama_audio.das`) serves
whisper, qwen2audio, ultravox, voxtral, AND qwen3a (its conv2d frontend is family-local, the
block loop is shared). Canary, parakeet, gemma4a are hand-written Conformer forwards sharing
only leaf kernels — with scalar conv/GLU modules and (gemma4a) fully scalar attention.

**The hard pieces are free; the cheap pieces are missing.** The mul_mm GEMM family serves q8
and bf16 planes today (zero-copy off a mapped `.dlim` — the format guarantees 16 KB section
alignment), and non-causal attention is chunk 1's own `uend` with
`AttnArgs(uend = npos, qoff = 0, window = 0, kv_mul = 1)` — no new attention kernel. Missing:
LayerNorm w+b (clone `MetalRmsNorm`, ~40 lines), a dense f32-weight mul_mm stamp (strip
`MetalAttnQKMm`'s head indexing + causal skip — it already stages f32×f32), a 1-operand GELU
(the tanh body exists verbatim in `MetalGeglu`), im2col, avg-pool, stack-N, the gemma4uv
2-axis pos add. Activations are f32 everywhere; no requant step exists on the metal GEMM path.

**The prize, from the ledger:** gemma4a das encode is 16x mtmd's (1888 vs 117 ms); the
Omni/qwen3a towers carry a ~4.8x encode gap; whisper-large-v3-turbo serves at xRT 5.6-11.8 —
"fp32 scalar tower, no perf pass". The das rows carry no `encode_ms` (field postdates the
sweep), so slice G re-instruments before predicting per-family wins.

**Precision doctrine, declared up front:** metal GEMMs stage f16 (the 2e-2 mm envelope);
the CPU tower gates are 1e-4/5e-3 vs external dumps and CANNOT be reused as GPU bars. GPU
tower gates are (a) transcript/text equality — backend-portable by construction (whisper +
parakeet transcript-exact, gemma4a/qwen3a q8-vs-f32 text cells are the shape), and (b)
envelope-relative numeric arms (`buf_mismatch_env` style) with the tolerance stated from the
f16-stage model, not borrowed. The gemma4uv tier-1 gate (2e-4 abs vs mtmd dumps) is NOT
assumed to hold under f16 staging — the bf16 tensor lane (`tmm2d`, no f16 stage) is the
first thing to try if it reds, and the caption-level gate is the floor.

## Slices

**G — the stage split first.** `asr_stage_probe` on whisper-large-v3-turbo and qwen3a on the
M1: encoder share of transcription wall (`wh.encode_total` / `q3a.encode` buckets).
Predictions on record before the probe: encoder >= 70% of whisper-large wall; blocks (not
frontend/tail) >= 85% of encoder time.

**H — three primitive kernels + unit gates.** LayerNorm (weight+bias, two-pass mean/var,
`MetalRmsNorm` skeleton), the dense f32 mul_mm stamp, the 1-operand GELU (tanh form; the
whisper f16-LUT flavor stays CPU-bit-exact territory — the GPU lane is tolerance-gated).
Unit arms in the kernels suite with negative controls before first commit.

**I — the gemma4uv Metal embedder: the smallest end-to-end proof.** CPU im2col (serial, cheap,
input prep) -> upload -> LN -> f32 GEMM (patch) -> bias -> LN -> pos-add kernel -> LN ->
weightless rms -> bf16 GEMM (proj, zero-copy plane) -> readback. New module
`dasllama_metal_tower.das` on the lens/kn_ rail (no census opt-out — the gemm driver's cycle
does not apply), one command buffer, decline enum in `dasllama_metal_shapes.das`,
`DASLLAMA_METAL_TOWER` env bool. Gates: CPU-vs-GPU envelope arm + tier-1 attempt (bf16
tensor lane if f16 staging reds it) + the img:enc metal cell.

**J — the Whisper-class block loop on Metal (five families at once).** Blocks-on-GPU,
seams-on-CPU: frontend (conv im2col + GEMMs) and family tails stay CPU initially; the 32-layer
block loop (LN -> qkv GEMMs -> non-causal trio -> o-proj -> LN -> fc1 -> GELU -> fc2, T<=1500,
hs=64 throughout) runs as one command buffer. q8 towers need the tower blob transform
(interleaved 34B blocks + the off%256 rule) — f32 lane first, q8 lane after parity. Gates:
transcript-exact (whisper tiny + parakeet excluded — Conformer), qwen3a/ultravox/voxtral
text-equality cells, one support-matrix engage cell (in a RUN arm, on the right model — the
chunk-1 lesson), stage-probe A/B.

**K — the measurement legs.** `lcpp_bench --asr --ngl`: `backend = "metal"` rows with the
tripwire ARMED (the CPU-by-protocol line and `backend="cpu"` stamp both flip on the gpu arm),
`encode_ms` populated on das rows. A/B whisper-large + qwen3a + E2B audio-chat on the M1.

**L — docs + the serving note.** PROFILE.md asr-cell paragraph, ENVIRONMENT.md regen, the
ledger: server `asr_workers` GPU wiring is a DESIGN decision (metal state is per-context —
N workers cannot each upload; map-and-wrap or a device-owning worker), deferred with the
audio-chat HTTP surface (#25).

## Non-goals (chunk 2)

- **Conformer families** (canary, parakeet, gemma4a) — per-family forwards with scalar
  conv/GLU modules; gemma4a's attention is scalar CPU with a 12-wide window. Their gap
  (incl. the 16x) is part scalar-CPU, part GPU-shape — a separate chunk with its own
  CPU-vectorize-vs-offload call per family.
- Server ASR-worker GPU wiring; audio-chat HTTP surfaces (#25); the whisper f16-LUT GELU
  bit-exactness on GPU (tolerance lane instead); mel/FFT on GPU (frontend stays CPU).
