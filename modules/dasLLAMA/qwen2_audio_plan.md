# Qwen2-Audio plan — audio input for dasLLAMA

## Direction

Speech/audio→text on the existing engine: a Whisper-large-v3 encoder tower + Qwen2-Audio's
linear projector produce soft tokens that splice into the EXISTING qwen2 decoder at the
`<|AUDIO|>` placeholder. No cross-attention anywhere; the decoder is untouched. The encoder
is shared infrastructure — the same tower unlocks Ultravox (llama decoder, have it) and most
of a Whisper-proper port later.

Oracle = llama.cpp mtmd. ggml-org ships NO pre-quantized GGUF for Qwen2-Audio (their note:
"very poor result", ref PR #13760) — we convert `Qwen/Qwen2-Audio-7B-Instruct` ourselves with
the in-tree `convert_hf_to_gguf.py` (guaranteed tensor-name compat with the checkout's mtmd).
Model quality is the model's problem; parity with mtmd is ours. Perf: `-jit` only, AOT =
compile-gate, same as the rest of dasLLAMA.

Audio input: dasAudio already decodes MP3/WAV/FLAC/Vorbis (`decode_audio`, miniaudio) and
resamples at decode time (`make_decoder(filename, rate, channels)` → 16 kHz mono directly).

## The pipeline (exact ops, from mtmd `models/whisper-enc.cpp` + `clip.cpp build_vit` + `mtmd-audio.cpp`)

0. **Load**: file → 16 kHz mono f32 PCM (dasAudio).
1. **Mel**: STFT n_fft=400, hop=160, Hann; power spectrum → 201 bins; 128-mel filterbank;
   `log10(max(x,1e-10))`; clamp to global `max−8`; `(x+4)/4`. 30 s chunks → `[3000×128]`.
   Exact padding/edge rules + filterbank source = mtmd-audio.cpp (verify at implementation).
   - **dasMinfft is pow2-only; Whisper's DFT is 400-point** → the ledger's "via dasMinfft"
     doesn't hold. Instead the DFT is two GEMMs with precomputed twiddles
     (`Re = frames·cos[400×201]`, `Im = frames·sin`), power = Re²+Im², mel = one more GEMM
     `[201→128]`. The whole frontend is existing matmul kernels; ~0.5 GMAC per 30 s chunk.
2. **Conv frontend**: conv1d k=3 s=1 "same" + bias → gelu_erf; conv1d k=3 s=2 "same" + bias
   → gelu_erf; transpose → `[1500×1280]`. Convs = im2col gather + existing batched GEMM.
3. **+pos-emb**: rows 0..n_pos−1 of `position_embd` — LOADED from the mmproj (no sinusoid
   generation).
4. **32 pre-LN blocks** (d=1280, 20 heads, hs=64, ffn=5120): LayerNorm(w,b) → attention
   (q has bias, k has NO bias, v bias, out bias; non-causal, no cache, scale 1/√64) →
   residual; LayerNorm(w,b) → fc1+b → gelu_erf → fc2+b → residual.
5. **avg_pool1d(k=2,s=2)** over time → `[750×1280]`, THEN post-LN (order matters — pool sits
   between the last block and ln_post, inside mtmd's build_vit).
6. **Projector**: single linear `mm_fc_w [1280→3584] + mm_fc_b` → 750 soft tokens / 30 s.
7. **Splice**: ChatML prompt with the audio segment rendered as
   `<|audio_bos|>` + 750 embeddings + `<|audio_eos|>`; prefill consumes a mixed
   token/embedding stream; generation after prefill is ordinary tokens.

## Integration shape (das side)

- **AudioTower struct** embedding a blob-container `Model` (fblob for conv/norm/bias/pos-emb,
  qblob+qscales for the 7 GEMM weight families: wq/wk/wv/wo, fc1/fc2, projector). All batched
  mm entry points take explicit `(t, woff, n, d, npos)` — the tower stores its own offsets and
  calls the same kernels; zero kernel-signature changes. Q8 repack per-tensor with explicit
  offsets (same primitive `repack_q8_weights` uses).
- **New kernels**: `layernorm_batch` (mean/var — decoder only has RMS), `gelu_erf_batch`
  (erf — bind/verify libm parity with ggml's erff), time-axis avg-pool, im2col gather,
  non-causal batched attention (flash-prefill is causal+cache-coupled; encoder gets its own
  loop over the same `gemm_f32` tiles).
- **forward_prefill split**: today tokens are consumed only in the `embed_row` loop filling
  `s.x_b` (dasllama_common.das:4184). Factor the body into `forward_prefill_x` (assumes
  `s.x_b` pre-filled) + keep `forward_prefill(tokens)` as the filler wrapper + add
  `forward_prefill_embd(embd)`. Audio path requires q8 mode initially (the q4/npos≤0 fallback
  loops per-token `forward()` which embeds internally — panic with a clear message).

## Oracle strategy

- `llama-mtmd-cli -m decoder.gguf --mmproj mmproj.gguf --audio f.wav -p ...` = end-to-end
  token stream (temp 0) → GEN_IDS-style token-for-token parity, the arc's acceptance gate.
- `MTMD_DEBUG_EMBEDDINGS=1` (clip.cpp:4484) dumps the projected soft tokens → stage-level
  gate for mel+conv+encoder+projector before the decoder is even wired.
- Fixtures: `llama.cpp/tools/mtmd/test-2.mp3` (in-tree, what mtmd's own tests use) + jfk.wav
  (the canonical whisper 11 s clip) + one LibriSpeech test-clean utterance + one non-speech
  sound (Qwen2-Audio does general audio understanding, not just ASR).

## Slices

- **A. Oracle rig**: venv convert HF→GGUF (decoder Q8_0 via llama-quantize; mmproj f16),
  mtmd-cli reference runs on the fixtures, MTMD_DEBUG_EMBEDDINGS reference dumps. Verify:
  decoder GGUF arch string (expect plain `qwen2`; alias if `qwen2audio`), audio special-token
  ids, chat template.
- **B. mmproj loader + tower**: GGUFMeta reuse; tensor table → blobs + offsets + q8 repack.
- **C. Mel frontend** + parity vs mtmd's mel (patch-dump or python reference on the fixture).
- **D. Conv + encoder + projector forward** → embedding parity vs MTMD_DEBUG_EMBEDDINGS.
- **E. Splice + prefill + generate** → token-for-token parity; audio-chat demo.
- **F. Tests + docs**: suite tests (model-gated like existing dasLLAMA tests), README, ledger
  sweep.

## Findings (all slices landed; end-to-end token parity 2026-07-05)

- **Token-for-token parity** with `llama-mtmd-cli --temp 0` on both fixtures: jfk.wav
  ("The spoken content in the audio is '而我最亲爱的美国同胞们' in Mandarin.") and
  tools/mtmd/test-2.mp3 ("I hear a slow-tempo, ambient electronic track in Bb minor …") —
  through miniaudio decode, mel, encoder, projector, splice, and the 7B q8 decoder.
- mel filterbank is GENERATED in mtmd (librosa slaney formula, area-normed, fmax=sr/2) —
  ported directly; mel gate vs `llama-mtmd-debug -p preproc --audio 440`: chunk 0 maxdiff
  3.1e-05 (their radix-2 FFT vs our DFT-GEMM float order), silence chunk 3.6e-07.
- padding pinned: zero-extend input to 31 s FIRST, then reflect-200 head + 30 s zeros + 200
  tail; Hann periodic-400; mel_floor 2⁻²⁴ (`log10(1e-10)` tail branch is unreachable in the
  whisper path); global max−8 clamp with the clamp value float-rounded before (x+4)/4.
  EVERY ≤30 s clip yields TWO chunks (real + all-silence) — mtmd feeds both to the encoder
  and the decoder sees 1500 audio positions; confirmed in mtmd-cli logs.
- encoder gate: all-ones mel via `llama-mtmd-debug -p encode --audio one -n 3000` (`-n` is
  MEL FRAMES there, raw samples are preproc-mode only). Upstream BUG found: the debug
  audio path crashes (SIGBUS) — `clip_image_encode` drops `is_audio` when wrapping the
  batch, sending the mel down the 3-channel vision copy. Local patch in the checkout
  (clip.cpp, infer from single-channel buffer size); consider upstreaming.
- mtmd/clip library logs are LOG_INF via their own logger — invisible without `--verbose`
  (both dumps "silently missing" cost a debug cycle each).
- numpy stage oracle (scratchpad np_encoder.py, ~60 lines off the f32 mmproj) reproduces
  mtmd stats to ~1e-3 (their encoder runs flash-attn) — its stage dumps + the module's
  `set_audio_encode_ref_dir` witness rail bisected the one real bug in minutes: layer
  stride counted 7 d-vectors instead of 8, so each layer's fc2_b was clobbered by the next
  layer's ln1_w (attention perfect, FFN tail wrong).
- prompt shape (from mtmd-cli debug logs): NO system message — llama.cpp's C++ chatml
  render does not inject the Jinja default. Exact stream:
  `<|im_start|>user\n<|audio_bos|>` + 750×n_chunks embeddings + `<|audio_eos|>{text}<|im_end|>\n<|im_start|>assistant\n`,
  ONE bos/eos pair around ALL chunks. ids: im_start 151644, im_end 151645, audio_bos
  151647, audio_eos 151648, `<|AUDIO|>` 151646 (unused by mtmd's splice).
- **tokenizer gap**: `encode(parse_special=true)` is still documented-unhonored, and BPE
  add_special prepends BOS where llama.cpp's qwen2 adds none. The demo assembles specials
  by id + per-segment text encodes (the chat layer's pattern). Follow-up: honor
  parse_special in the tokenizer so multimodal drivers can tokenize rendered templates
  directly.
- decoder GGUF converts to plain `general.architecture = qwen2` (dim 4096, MHA 32/32) —
  existing arch registration, zero decoder changes. `clip.audio.projection_dim` = 4096
  read from the mmproj (do not assume 3584).

## Performance ledger

Folded into `PERF_LEDGER.md` (2026-07-29 doc reorg; the two arc-local sections deduped there).
