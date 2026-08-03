# Whisper-proper plan — speech-to-text on dasLLAMA

## Direction

Full Whisper ASR: the validated encoder tower (qwen2_audio_plan.md) + a NEW text decoder with
cross-attention, reading ggerganov/whisper.cpp `ggml-*.bin` models directly (drop-in HF models,
no converter). Flagship = large-v3-turbo (encoder identical to the qwen2a tower we validated);
dev model = tiny. Oracle = whisper.cpp CPU greedy (`whisper-cli -bs 1 -bo 1 -nf -ng`).
Public API = block idiom: `transcribe(w, samples) $(seg) { ... }` + one-shot + push-chunks
feed/drain session. v1 = no mic (dasAudio capture gap), no live streaming; `transcribe()` is
whisper-only (Qwen2-Audio stays on the chat rail). Perf: `-jit` only, AOT = compile-gate.

**This arc ships as ONE PR together with the qwen2-audio commits already on the branch.**

## The ggml bin container (whisper.cpp `whisper_model_load`, verified against checkout)

1. magic u32 `0x67676d6c`.
2. 11 × i32 hparams: n_vocab, n_audio_ctx, n_audio_state, n_audio_head, n_audio_layer,
   n_text_ctx, n_text_state, n_text_head, n_text_layer, n_mels, ftype.
   `qntvr = ftype/1000; ftype %= 1000`; ftype 0 = f32, 1 = f16 (shipped models are f16).
3. mel filters: i32 n_mel, i32 n_fft(=201 bins), then n_mel×n_fft f32 — filterbank comes FROM
   THE FILE (no slaney generation on this path).
4. vocab: i32 n_vocab_file, then per token u32 len + raw bytes (already byte-decoded — decode =
   plain concat; id 50256 is an empty string in multilingual files). If hparams.n_vocab >
   n_vocab_file, the tail ids are synthesized specials/timestamps (no bytes to emit).
5. tensor records until EOF: i32 n_dims, i32 name_len, i32 ttype; i32 dims[n_dims]
   (ne[0] innermost); name bytes; RAW tensor data immediately after — **NO alignment padding**.
   Biases/LN/pos-emb are f32; weight matrices f16 in shipped models (→ f16_to_f32 on load,
   helper already in dasllama_gguf.das).

Special-token id math (`whisper_vocab`): defaults eot 50256, sot 50257, translate 50357,
transcribe 50358, solm 50359, prev 50360, nosp 50361, not 50362, beg 50363. If multilingual
(n_vocab ≥ 51865): eot++, sot++, and the rest += `num_languages() − 98` where
`num_languages = n_vocab − 51765 − 1`. So tiny (51865, 99 langs): eot 50257, sot 50258,
lang_en 50259, transcribe 50359, not 50363, beg 50364. large-v3/turbo (51866, 100 langs incl.
yue): transcribe 50360, not 50364, beg 50365. lang token = sot + 1 + lang_id (g_lang table,
en=0, 100 entries).

Tensor names: `encoder.positional_embedding`, `encoder.conv{1,2}.{weight,bias}`,
`encoder.ln_post.{weight,bias}`, `encoder.blocks.{i}.{attn_ln,mlp_ln}.{weight,bias}`,
`encoder.blocks.{i}.attn.{query,key,value,out}.{weight,bias}` (key has NO bias),
`encoder.blocks.{i}.mlp.{0,2}.{weight,bias}`; decoder mirrors with `decoder.` prefix plus
`decoder.token_embedding.weight`, `decoder.positional_embedding`, `decoder.ln.{weight,bias}`,
and per block the `cross_attn_ln` / `cross_attn.{query,key,value,out}` family (cross key no
bias). Decoder = 24 tensors/layer, encoder = 15.

## Mel — whisper.cpp `log_mel_spectrogram` (DIVERGES from the mtmd variant we shipped)

- Padding: reflect-200 head taken from RAW samples[1..200] reversed (no 31 s zero-extend
  first!), then 30 s of zeros + 200 zeros tail (comment says reflective, code fills zeros).
- `n_len = (n_padded − 400)/160` (NO +1 — "remove the last frame");
  `n_len_org = 1 + (n_samples + 200 − 400)/160` (frame count of real audio; = seek_end).
- Frames at offset ≥ n_samples+200 skip the FFT entirely: every mel bin = `log10(1e-10)` = −10.
- Floor is `1e-10` (NOT mtmd's 2⁻²⁴); accumulation = float products in groups of 4 summed in
  float, each group added to a double accumulator (mirror exactly); log10 in double.
- Global max−8 clamp + (x+4)/4 in double over the WHOLE spectrogram (audio + zero tail).
- Filterbank loaded from the model file. DFT stays our twiddle-GEMM (their radix-2/naive-25
  FFT differs at ~1e-5 float order, same as the mtmd gate).
- Encoder window: 3000 frames sliced from the global mel at `seek`, zero-filled (0.0f) past
  n_len. Mel layout mel-major [n_mel][n_len], same as MelChunk.

## Encoder = the qwen2a tower minus pool/projector, plus ln_post, DIFFERENT gelu

Same conv k3s1 + k3s2 frontend, +pos rows, pre-LN blocks (q/v/out bias, k none, eps 1e-5,
non-causal). After blocks: ln_post directly (whisper) vs pool→ln_post→projector (qwen2a).
Refactor `audio_encode` so both share the conv+pos+blocks core; tails diverge.

**GELU flavor differs per oracle**: mtmd path (qwen2a) = gelu_erf; whisper.cpp = `ggml_gelu`
tanh approximation THROUGH AN FP16 LUT (`GGML_GELU_FP16`): x≤−10 → 0, x≥10 → x, else
`f16_to_f32(f16(0.5·xr·(1+tanhf(0.79788456·xr·(1+0.044715·xr²)))))` with `xr = f16_to_f32(f16(x))`.
Mirror exactly (needs f32→f16 RTE helper). Same flavor in the decoder MLP.

Known noise we do NOT mirror (tolerance-gated, token parity is the real gate): whisper.cpp
casts K/V to f16 in attention (`itype`), keeps KV caches f16, and its f16-weight mul_mats
convert activations to f16 (f16×f16 dot, f32 accum). We compute f32 throughout on
f16→f32-converted weights.

## Decoder (whisper_build_graph_decoder, exact op order)

- x = token_embd[id] + pos_embd[pos] (learned, absolute position = n_past+i, ≤448).
- Per layer: LN(w,b) → self-attn: q = W_q·x+b **scaled hs^−0.25**, k = W_k·x **scaled
  hs^−0.25** (cached), v = W_v·x+b (cached); scores softmax at scale 1.0 with causal mask;
  out W+b; +residual → inpCA. LN(w,b) → cross-attn: q = W_q·x+b (UNscaled); cached cross
  K (pre-scaled hs^−0.25 at cross-compute, no bias) / V (w+b); softmax at scale hs^−0.25
  (net hs^−0.5); out W+b; +residual. LN(w,b) → mlp fc1+b → gelu(LUT) → fc2+b; +residual.
- Final LN(w,b); logits = token_embd · x (tied), last row only for greedy.
- Cross K/V precomputed ONCE per encoder window for all layers (from ln_post output).
- KV cache: contiguous, single sequence; prompt prefill = one batched pass (causal mask),
  generation = n=1 steps. One code path, f32 cache.

## Greedy driver (whisper_full, single decoder, temp 0, fallback disabled)

- prompt_init = [sot, lang, transcribe] (+ not when no_timestamps). Multilingual only; v1
  requires explicit language (default "en"), auto-detect punted.
- Optional rolling context: prompt = [prev] + tail of prior decoded tokens (budget
  n_text_ctx/2 − 1, "no_context" default in CLI to verify at driver time) + prompt_init.
- Logit filters each step (order matters): initial-step suppress eot + " " (suppress_blank);
  always −INF: not, sot, nosp, solm, translate, transcribe, prev, lang tokens (sot+1+i,
  i<100); no_timestamps → −INF all ids ≥ beg. Timestamp mode adds: pairs rule (last-was-ts →
  ban ts / ban text depending on penultimate), max_initial_ts 1.0 (initial step: ban >
  beg+50), non-decreasing rule (has_ts → ban < beg+seek_delta/2), and the sum-vs-max rule
  (if logsumexp(ts logprobs) > max text logprob → ban all text). suppress_nst list = CLI
  default false, skip in v1.
- Sample: argmax (probs after log_softmax — order-preserving, plain argmax over filtered
  logits). Loop cap n_text_ctx/2 − 4 = 220 steps. Stop on eot.
- no_timestamps: seek_delta = 3000 frames (30 s); result_len = all tokens.
- Timestamps (slice 2): ts token > beg updates seek_delta = 2·(id−beg), result_len; segments
  split on second ts of a pair; seek += seek_delta; loop `while (seek + 10 < seek_end)`,
  seek_end = n_len_org.
- Text = concat vocab bytes of ids < eot per segment.

## Oracle strategy

- `whisper-cli -m ggml-tiny.bin -f samples/jfk.wav -l en -nt -bs 1 -bo 1 -nf -ng -ojf` →
  token ids in the full JSON. Reference (tiny, jfk.wav, no-timestamps):
  `[400, 370, 452, 7177, 6280, 1029, 406, 437, 428, 1941, 393, 360, 337, 291, 11, 1029, 437,
  291, 393, 360, 337, 428, 1941, 13, 50257]`
  = " And so my fellow Americans ask not what your country can do for you, ask what you can
  do for your country."
- Prompt for that run: [50258, 50259, 50359, 50363]. `batchd 4 runs / decode 24 runs` in cli
  timings confirms prompt-batch + 24 greedy steps (eot sampled on step 25).
- Stage gates as needed: mel via `log_mel_spectrogram.json` (whisper_full debug_mode dump) or
  numpy; encoder via the existing `set_audio_encode_ref_dir` witness rail pointed at numpy
  dumps off the f16 bin (np_encoder.py pattern, harness/).
- Slice-2 fixture: a >30 s clip (concatenate jfk or use a LibriSpeech chapter) with
  timestamps ON, vs cli token stream.
- rig: ~/Work/whisper.cpp built Release (CPU via -ng); models/ggml-tiny.bin,
  ggml-large-v3-turbo.bin.

## Slices

- **W-A. Loader**: bin container → WhisperModel (enc AudioTower-shaped blob reuse + dec blob
  + vocab + filters + special-id math). Probe: hparams/vocab spot-checks vs cli log lines.
- **W-B. Mel variant + encoder tail**: whisper mel rules; shared conv+blocks core refactor;
  gelu LUT; ln_post tail. Gate: encoder output maxdiff vs numpy on jfk.
- **W-C. Decoder + cross + greedy**: token-for-token vs the tiny reference above.
- **W-D. API**: `transcribe` block/one-shot/session forms; language param; timestamps ON by
  default (slice 2 first if needed for API shape).
- **W-E. Timestamps + long-form seek**: slice-2 rules above; >30 s fixture parity.
- **W-F. Example app** (any-file→text via dasAudio) + turbo flagship gate + tests + docs.

## Findings

- **W-A loader**: container parsed exactly as recon'd (probe vs python byte-level reference:
  hparams, special ids, vocab, filterbank, permuted convs, deep-layer offsets). f32_to_f16
  round-trips all 65536 f16 patterns and matches numpy RNE on every boundary case (65519.9 →
  max, 65520 → inf, subnormal ties-to-even).
- **W-B encoder**: vs the numpy stage oracle on jfk/tiny — mel maxdiff 4e-5 (DFT-GEMM float
  order, same as the mtmd gate), ln_post output 7e-3 max over 576k elements. The fp16-LUT
  gelu turns infinitesimal input diffs into one-fp16-ulp output flips at rounding boundaries
  (e.g. 1.95e-3 at |x|≈3.4) — expected, benign, and why stage diffs are larger than qwen2a's.
- **W-C decoder + greedy**: **large-v3-turbo (flagship): TOKEN-FOR-TOKEN with whisper-cli
  greedy on jfk.wav** (26 text tokens + eot). tiny: 23/24 — the sole divergence is the first
  comma, a genuine knife-edge (oracle p(",")=0.3465 vs runner-up " ask"; our logit gap
  −0.050 the other way). whisper.cpp's own ARM CPU path computes every mul_mat as f16×f16
  with fp16 vector accumulation (GGML_SIMD vfmaq_f16) and keeps K/V caches f16 (`itype`) —
  its logits carry ~1e-2-scale noise vs exact f32 math, so sub-0.05-logit ties can land
  either way per implementation. Not chased: mirroring fp16 accumulation would make our
  compute strictly WORSE to match noise; the flagship gate is the arc's claim.
- whisper-cli's `--debug-mode` mel dump is unreachable via whisper_full (the mel call
  hardcodes debug=false) — numpy is the mel oracle.
- `no_context` defaults to TRUE (whisper.h 5939) — stock cli does NOT carry rolling context
  between windows; multi-window in notimestamps mode = independent windows.
- **Family sweep (jfk, +timestamps, greedy)**: tiny, base.en, small, medium, large-v3,
  large-v3-turbo all TOKEN-FOR-TOKEN (base.en also validates the non-multilingual path —
  no lang/task prompt tokens, unshifted special ids). base diverges at ONE step: the
  force-timestamp rule's logsumexp-vs-max-text comparison lands 2e-4 logits apart
  (35.469856 vs 35.469654) while the oracle's own p(" ask") = 0.495 — both engines sit
  exactly on that rule's 50/50 threshold and fp16 noise across the 1500-token logsumexp
  decides it; base is exact in `-nt` mode. Also: whisper.cpp's `-nt` segment t0 is garbage
  (uninitialized tid=0 → t0 = 2·(0−beg) = −100728 cs) — ours reports 0; ignore t0 when
  diffing `-nt` runs.
- **>2 GB bins (large-v3)**: `length(bytes)` on the fmap'd array is int32 and wraps
  negative at 3.1 GB, silently emptying the until-EOF record walk ("missing tensor" panic).
  Fix: byte count from `stat(path).size` (64-bit st_size). Offset reads past 2^31 were
  always fine (the 8.5 GB llama GGUF proved that rail).
- **W-E timestamps + long-form**: turbo EXACT on jfk (+ts, [0→1040] one segment, identical
  tokens) and on jfk3 33 s long-form (3 windows/segments, identical offsets and token lists).
  tiny +ts on jfk also exact. Two rules beyond the filter suite were load-bearing:
  - **single_timestamp_ending** (whisper.cpp PR #2629): kept tokens ending text-then-SINGLE
    ts (no closing pair) skip the entire remaining chunk (`seek_delta = min(seek_end−seek,
    3000)`) — without it a second phantom window transcribes the trailing applause.
  - **closing pairs may be unequal** (segment-end ts, then next-segment-start ts, e.g.
    TT_1069/TT_1092): every ts token updates seek_delta, the emission skip-loop re-bases t0
    per consumed ts, and the window advances by the SECOND of the pair.
  - the no-speech gate (nosp prob off raw prompt logits > 0.6 AND avg_logprobs over kept
    tokens < −1.0) suppresses a window's segments; plog's normalizer is captured between the
    main filter suite and the force-timestamp rule (their logprob order).
  - v1 simplifications vs whisper_full (documented, all off the greedy-happy-path): the
    "don't go back in time" and repetition-loop failures end the window instead of triggering
    temperature fallback (fallback is disabled in the oracle runs anyway); entropy-based
    failure (result_len > 32 && entropy < 2.4) not implemented.

## Performance ledger

Folded into `PERF_LEDGER.md` (2026-07-29 doc reorg) — arc-local perf notes live there now.
