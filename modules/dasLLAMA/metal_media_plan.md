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

**A — measure the cliff first.** `lcpp_bench --image` on the M1 (12B Q4_K_M, cats): the
CPU baseline row exists; add the timing probe run (`--prof` pp window) splitting head/span/
tail eval wall. Text-only control at the same knobs. No code changes; predictions above
judged against this.

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

**D — routing and parity surfaces.** New `span` arm in the prefill parity suite
(embd-driven — no existing arm feeds embeddings; CPU control via `eval_embd`, per KV codec
f16/q8_0/tq4, plus a continuation-shaped variant with the span at `start_pos > 0`);
`test_attn_span` gains a fourth mode row (Metal chain vs classic, blocked's tolerance
family); extend `arm14-poison` (a GPU media prefill writes the shared region it guards);
coverage census gains the non-causal dispatch leg (synthetic embd rows in `cov_model` —
model-free, no mmproj download).

**E — server + stats + demo.** Engine knob `DASLLAMA_METAL_SPAN` (MetalEnv bool, default
true, doc names the CPU A/B fallback — `attn`/`mulmm` precedent; declared in
`dasllama_env.das` or `test_env_registry` reds; ENVIRONMENT.md regen). No new server CLI
knob — rail selection rides the existing `--gpu` surface and `GET /config`. `/v1/stats`
gains a media-prefill-served counter beside `vision_pending`; the page needs zero new UI —
the demo is a metal-blob model with `--image-mmproj`: the vision button appears where it
was previously refused, and the prefill tok/s chart + ttft tile move.

**F — the bench row + docs.** `lcpp_bench --image` gains the `backend = "metal"` row (same
three keys `img:enc/img:pp/img:tg`; the cell's `allow_cpu_prefill()` protocol line and
`PROFILE.md:144-149`'s CPU-by-design paragraph are the doc edits). REVIEW rules already
demand the cell in the same change as the capability.

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
