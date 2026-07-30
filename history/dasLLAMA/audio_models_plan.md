# Audio-models expansion wave — audit action items

Extends the audio arc (qwen2_audio_plan.md + whisper_plan.md) per the 2026-07-06 audit:
add Ultravox, Voxtral, Qwen2.5-Omni (audio), Qwen3-ASR, and Parakeet-TDT, each to the same
bar as everything else in dasLLAMA — **token-for-token vs its reference implementation** —
with README model-matrix rows as each lands. Same branch, same one big PR.

The structural shortcut (from the audit): llama.cpp mtmd serves Ultravox, Voxtral, and
Qwen2-Audio from ONE whisper-encoder graph — the tower we already validated. For those,
"support" = a projector tail + chat wiring, not a new encoder.

## Session A — audio-LLM wave (Ultravox + Voxtral + Qwen2.5-Omni audio)

All three ride existing rails: whisper tower (validated) + decoders we already run.

- **Ultravox v0.5** (llama-3.2-1b dev, llama-3.1-8b validate): the decoder is STOCK
  Llama-3.2-1B / 3.1-8B-Instruct — the Q8 GGUFs are already in models/; only the mmprojs
  download (~1.2 GB each, whisper tower inside). New work: the `ultravox` projector tail —
  stack(proj_stack_factor) frame-concat → mm_1 → swiglu_swapped → mm_2 (mtmd
  whisper-enc.cpp:51-73) — plus loader keys and llama-3 chat splice.
- **Voxtral-Mini-3B** (Mistral): same whisper tower, same stack+SwiGLU tail family
  (`voxtral` projector), llama-arch decoder, [INST] template splice. ggml-org publishes
  only Q4_K_M (K-quants — we don't read them): **self-convert** the HF original with the
  qwen2audio venv recipe (convert_hf_to_gguf.py → decoder Q8_0 + mmproj).
- **Qwen2.5-Omni 3B** (7B validate): audio side is the QWEN2A projector VERBATIM (mtmd
  aliases it); decoder is qwen2. Work ≈ conversion-free download + loader tolerance for the
  omni mmproj (it also carries v.* vision tensors — name-driven loader skips them) + the
  omni chat template + parity runs. Vision input explicitly out of scope.
- Gates: `llama-mtmd-cli --temp 0` token-for-token per model on jfk.wav + test-2.mp3
  (the qwen2audio harness recipe); suite tests model-gated like test_audio.das.
- Possible follow-on freebies to check while here: SeaLLM-Audio (qwen2a family),
  MERaLiON (gemma2 + whisper tower) — audit says small; only if the wave goes smoothly.

## Session B — user-facing API (Boris discussion first)

Shape the transcribe/audio-chat surface knowing all backends: whisper (30 s windows,
timestamps), Qwen3-ASR (LLM-style ASR), Parakeet (streaming transducer), and the
audio-chat family (soft-token splice into chat sessions). Facade placement, segment/result
shapes, naming. Then implement. Tutorials + mic example follow this.

**ALL DECISIONS SETTLED (Boris, 2026-07-06) — session B is now pure implementation:**
- Uniform `load_asr_model(path[, mmproj])` — format sniffed (GGUF magic vs ggml-bin;
  whisper-vs-parakeet bins share the magic, disambiguate on n_vocab ≥ 51864 vs ~1k),
  per-family loaders demote to plumbing, one verb set with internal dispatch. NO family
  names in the public API.
- `caps(m : AsrModel) : AsrCaps` (languages / translate / timestamps none-segment-word /
  streaming / prompt) — advisory; invalid options still panic loudly, never silently ignore.
- Chat-audio verbs live on the facade (`dasllama.das`): `create_chat(m, tower)` (tower
  attached at creation), `add_user_audio(chat, samples)` composing with `add_user(text)`
  in one turn; splice inside render/respond, per-model wiring in the arch registry.
- `WhisperSegment` → shared `TranscribeSegment` + `avg_logprob : float`.
- `lang = "auto"` (whisper language detect, one extra prompt decode) implemented here.
- `create_whisper_session` → `create_session` overload.

**Also in this session: `caps` for LLMs** (Boris: "it's on us regardless — buck stops
here"). The known silent gap: gemma has no system role and the chat layer absorbs the
system prompt without telling anyone. `caps(m : Model) : LlmCaps` declared by each arch
registration (the registry already owns templates/stop tokens — caps live beside them):
system_prompt support at minimum; grow honestly as gaps surface (audio-tower attach,
thinking variants) rather than speculatively.

## Session C — Qwen3-ASR (0.6B dev, 1.7B validate)

qwen3 decoder (registered) + a NEW audio encoder: mtmd's `qwen3a` graph — 128-mel input
processed in 100-frame chunks (chunked windowed attention; ref modeling_qwen3_asr.py).
GGUFs from ggml-org. Oracle = mtmd-cli. Hooks into the session-B API surface.

## Session D — Parakeet-TDT (0.6b-v2)

FastConformer encoder (24L, d=1024, 8h): conv subsampling, macaron dual half-FFN,
depthwise-conv module w/ inference batch-norm, Transformer-XL-style rel-pos attention
(pos_bias u/v — the one genuinely new attention piece); tiny LSTM prediction network +
joint net + TDT greedy loop (token+duration, frame-skipping). Oracle: whisper.cpp checkout
has `parakeet-cli` BUILT in-tree (+ convert-parakeet-to-ggml.py, .nemo downloaded,
test-parakeet-full-jfk as reference). Streaming-native — the natural mic-example backend.

## Session E — tutorials

NOT blocked on the mic demo: tutorials document the settled session-B surface across ASR
+ audio-chat as the model sessions land. Do these, then wait.

## Session F — mic demo (Parakeet)

**Runs on Parakeet** (Boris's call — streaming-native, no 30 s window latency). Separate
section, gated: needs session D AND dasAudio capture (PR #3388) MERGED — do tutorials
first, wait for the merge, then this.

## Status

- [x] A. Ultravox (1b + 8b) — ✅ token-for-token: 1b on jfk.wav + test-2.mp3, 8b on jfk.wav
- [x] A. Voxtral-Mini-3B — ✅ transcription prompt byte-identical (jfk); tokenizer id-for-id; decoder 40/40; encoder ≤5e-4; the two open-caption prompts each flip one 0.079/0.195-logit coin-flip (whisper-tiny/base precedent, not chased)
- [x] A. Qwen2.5-Omni 3B/7B — ✅ token-for-token: 3B on jfk.wav + test-2.mp3, 7B on jfk.wav
- [x] B. API implementation — `load_asr_model` (sniffed) + `caps(AsrModel)` + `TranscribeSegment`
  (+`avg_logprob`) + `lang="auto"` + `create_session` overload + facade chat-audio verbs
  (`create_chat(m, tower)` / `add_user_audio`, per-template audio wraps incl. the NEW
  mistral-v7-tekken template detected BY NAME) + `caps(Model) : LlmCaps` (gemma
  `system_prompt=false`). audio.das demo now runs through the facade — all three template
  families re-gated token-identical through the new path.
- [x] C. Qwen3-ASR 0.6B/1.7B — ✅ transcripts byte-identical vs mtmd-cli: 0.6B on jfk.wav +
  test-2.mp3, 1.7B on jfk.wav; encoder ≤1e-4 vs MTMD_DEBUG_EMBEDDINGS per-window token-0.
  New: bf16 GGUF reads (exact), qwen3vl→qwen3 alias, `dasllama_qwen3a.das` encoder, ASR
  dispatch (`AsrSession`, GGUF pair route `load_asr_model(path, mmproj)`, `detected_lang`,
  `context` biasing → caps.prompt=true); transcribe.das takes the pair.
- [x] D. Parakeet-TDT 0.6b-v2 — ✅ token-for-token vs parakeet-cli: jfk 33/33 + gb1 786/786
  (ids/frames/durations); full spec + the fp32-underflow duration-argmax finding in
  parakeet_plan.md; wired into load_asr_model (small-vocab ggml-bin route) + transcribe.das
- [x] E. tutorials — 07_speech_to_text.das (ASR surface: sniffed loader, caps, segments,
  auto-lang, feed/drain/flush; verified through whisper-tiny AND parakeet) +
  08_audio_chat.das (create_chat(m,tower)/add_user_audio/respond + LlmCaps; verified
  through ultravox-1b with a multi-turn follow-up); RST pages + toctree; sphinx -W green
  (needed the master merge for #3388's audio_record stdlib page + a daslang rebuild)
- [x] F. mic demo — examples/dasLLAMA/dictate.das: sound_record_* ring-buffer capture →
  Parakeet with 2 s incremental re-transcription + final pass; verified END TO END with the
  real microphone (live ambient audio transcribed). Any load_asr_model file works; Parakeet
  is the latency pick.

## Findings

- **ggml's build_stack "padding" never materializes** — `GGML_PAD(total, stride)` is a
  power-of-2 bitmask macro but the ultravox stride (n_embd·stack = 10240) isn't a power of
  two, so `GGML_PAD(1920000, 10240) = 1921024`, and `1921024 / 10240 = 187` in the view's
  int division. Net semantics: floor(n_pos/stack) full rows, ragged tail positions DROPPED
  (4 positions ≈ 80 ms for ultravox, 2 for voxtral), zero-pad row never reaches the
  projector. Matches clip_n_output_tokens' `ALIGN(3000,8)/8/2 = 187` only by this accident;
  we implement the floor directly (`audio_tokens_per_chunk`).
- **`clip.audio.projection_dim` lies for ultravox-1b** (says 4096; mm.a.mlp.2 projects into
  the 2048-wide llama-3.2-1b decoder). Loader derives projector dims from the mm tensors.
- **f16 mmproj vs fp32 parity**: published ultravox/omni mmprojs are f16; ggml's f16
  mul_mat path (fp16 accumulate on ARM) drifts vs our fp32 → oracle runs use a local
  f16→f32 re-encode of the mmproj (scratch script; conv weights MUST stay f16 — ggml's
  clip conv/im2col path aborts on F32 kernels, which is also why the upstream converter
  pins convs to f16 even at `--outtype f32`). Our side reads either dtype identically.
- **Qwen2.5-Omni decoder is arch `qwen2vl` (M-RoPE)** — for text+audio input mtmd assigns
  every M-RoPE coordinate the same linear position (`set_position_mrope_1d`), each rope
  section rotates by exactly the standard angle, so plain qwen2 forward is bit-identical.
  Registered `qwen2vl` as a qwen2 alias; vision input stays out of scope. Omni GGUFs also
  omit `tokenizer.ggml.bos_token_id` entirely (BPE loader now tolerates that).
- **tekken pre-tokenizer goes through llama.cpp's GENERIC wregex** (no custom splitter), so
  the parity target is true ECMAScript alternation semantics of the case-structured
  approximation — a non-cased run followed by ASCII uppercase ("你A") splits "你"|"A",
  unlike gpt-4o's merged-run reduction which would take the whole run. No contractions,
  single-digit \p{N}, '/' in the punct tail. `ignore_merges` (whole-word-first) was already
  unconditional in our BPE.
- **Voxtral upstream converter bug (patched locally in ~/Work/llama.cpp)**: conversion/llama.py's
  tekken branch calls `_set_vocab_mistral()` without `return`, falling through to the
  gpt2 vocab path which crashes on MistralCommonTokenizer. Upstream alongside the mtmd
  `is_audio` one-liner.
- mtmd-cli prompt shapes (all: audio marker PREPENDED when absent, add_special BOS rules):
  ultravox = llama-3 render, NO audio delimiters, embeds inline; voxtral = `<s>[INST]`
  `[BEGIN_AUDIO]` + embeds + text + `[/INST]` (v7-tekken, no spaces); omni = qwen2a's
  chatml + `<|audio_bos|>/<|audio_eos|>` verbatim, no system message.
- **Voxtral near-tie verdict method** (reusable): when a temp-0 gate diverges, walk our
  greedy steps printing top-2 logit gaps. Voxtral jfk flipped at step 0 (gap 0.079 of ~24,
  p≈0.52) and test-2 at step 30 (gap 0.195; ' headline' vs ' speaker'), while every
  MATCHED step cleared gaps up to 12 logits and tokenizer/decoder/encoder each verified
  independently (id-for-id / 40-40 / ≤5e-4). A prompt without a tie ("Transcribe the
  speech.") matched byte-identical to the natural stop — that's the positive gate.
- **Session B notes**: `register_arch` MOVES the ArchDesc — clone the template before
  registering an alias. ChatTemplate gained `assistant_close` (v7-tekken assistant turns
  close with `</s>`, not the user-tail heuristic). ⚠️ pre-existing: the v0.3 Mistral
  `[INST]` template's derived close is ` [/INST]` — wrong for multi-turn (should be
  `</s>`); untouched here, flagged for Boris. New module files must ALSO be listed in
  `modules/dasLLAMA/.das_module` (require-root registration is manifest-driven).
- **Session C notes (qwen3a)**: the mmproj spells the projector key
  `clip.audio.projector_type` (not `clip.projector_type`); `a.conv_out.bias` doesn't exist
  (graph guards it); positions reset per chunk — only rows 0..12 of the stored 1500-row
  table are ever used; the qwen3a mel flavor skips the 30 s tail entirely (manual 200-sample
  reflection both ends, n_len = effective frames, windows zero-padded to 100-multiples with
  RAW 0.0, and the max−8 clamp stays in double — no float round-trip like the mtmd whisper
  flavor). Encoder matched the oracle first try at ≤7e-5. Qwen3-ASR responses are
  `language {L}<asr_text>{text}` — parse, don't strip. Deep AsrModel/AsrSession struct
  returns need `options stack = 65536` in user programs (transcribe.das hit the default).
