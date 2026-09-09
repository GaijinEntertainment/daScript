# dasLLAMA Pocket TTS: a second TTS family, with voice cloning, on the existing block home

Goal: dasLLAMA serves Kyutai's Pocket TTS - a 110M-parameter continuous-audio language model
(6-layer transformer backbone, one-step flow head, Mimi-derived codec) with zero-shot voice
cloning from a few seconds of audio, six languages (English, French, German, Spanish,
Portuguese, Italian), no phoneme front end - as a second family behind the same `dasllama_tts`
facade, the same `txt2wav` verb, the same `/v1/audio/speech` route and the same rig that
score Kitten and Kokoro today. Every language Kokoro is blocked on by the espeak data question
(ledger row 109) except Hindi lands here with no lexicon at all.

Status: RULED 2026-09-09 (the rulings section). Base: master bebb5dd02.

## Why this model, in one paragraph

Licences are attribution-only (code MIT, weights CC BY 4.0, default voices CC0), the text path
is a 4000-piece SentencePiece tokenizer straight into the backbone, so the GPL lexicon problem
that blocks Kokoro's other languages does not exist, and every operator in it is a shape the
engine already runs: a causal transformer with a KV cache (the Whisper decoder), tap-GEMM convs
and transposed convs over rows (the StyleTTS2 generator), LayerNorm, GELU, SiLU, RoPE on
adjacent pairs (`rope` in `dasllama_math.das` is this convention exactly), a polar normal RNG.
The reference runs single-threaded batch-1 torch at 80 ms per frame and still claims 6x real
time on an M4 laptop; the kernels here have no framework overhead per frame.

Kokoro stays the higher-MOS English voice (4.44 against 4.10 on an independent CPU benchmark).
Pocket adds what Kokoro cannot: any voice, five more languages, and a first audio chunk after
one frame instead of after a whole sentence.

## Evidence base (read on 2026-09-09, verified against the source and the public weights)

Source: `github.com/kyutai-labs/pocket-tts` (clone in the session scratchpad); paper
`arxiv.org/abs/2509.06926` (Continuous Audio Language Models); public no-cloning weights
`kyutai/pocket-tts-without-voice-cloning` (downloaded: `languages/english_2026-04/model.safetensors`,
219,029,196 bytes, 109,502,146 parameters, all BF16; `tokenizer.model` 59,339 bytes).

- **Gate is real.** The public bundle carries every tensor name of the gated one, but the
  Mimi encoder convs are all-zero (`mimi.encoder.model.*`), while `speaker_proj_weight`,
  `mimi.downsample` and the decoder are populated. Voice cloning needs the gated repo
  (`kyutai/pocket-tts`, click-through: no cloning without consent, no impersonation). Boris
  accepts the terms under his own HF account; the converter pins the revision
  (`19f95fe2df36e79fbd9f10008595cc4c977a0fcc` for english_2026-04 per the config).
- **Tokenizer is SentencePiece UNIGRAM** (model_type 1), 4000 pieces, byte fallback on,
  identity normalizer, dummy prefix on, no extra-whitespace removal; ids bos 1, eos 2, unk 0,
  pad 3 (unused by the model: the LUT has 4001 rows, the padding row). The engine has the
  BPE-scored SentencePiece reader (`dasllama_spm.das`) and byte-level BPE (`dasllama_bpe.das`)
  and NO unigram (Viterbi over piece scores). That is the one missing front-end piece.
- **One bundle per language, self-contained.** Each language ships its own backbone, flow head,
  text embedding, tokenizer AND codec (the Mimi decoder tensors of the German and French
  bundles hash differently from the English ones). English, German, Italian, Spanish,
  Portuguese are 6-layer backbones (219 MB bf16); French exists only as the 24-layer variant
  (672 MB); the other four also ship a `_24l` variant. Language = model file, as Whisper sizes.
- **Default temperature 0.3** for english_2026-04 ("human evals preferred this model at 0.3");
  the package default is 0.7 for the rest. One LSD decode step (`DEFAULT_SAMPLER_DECODE_STEPS
  = 1`), eos threshold -4.0, max 50 text tokens per chunk, generation cap
  `ceil((tokens/3 + 2) * 12.5)` frames, 1-3 frames after EOS plus 2.
- **Independent CPU benchmark** (4-core Xeon, 2026): Pocket RTF 0.71 / MOS 4.10 against
  Kokoro-82M ONNX 0.64 / 4.44; flat RTF across text lengths; "no vocoder buzz". Kyutai:
  ~6x real time on an M4 Air on two cores, ~200 ms to first audio.

### The graph, with numbers (english_2026-04)

Text: SentencePiece -> ids -> LUT `conditioner.embed` [4001 x 1024].

Voice prompt (cloning): 24 kHz mono -> Mimi encoder (SEANet: conv 1->64 k7; per ratio
[4,5,6]: ResBlock(ELU, conv k3, ELU, conv k1, residual), ELU, strided conv k=2s
64->128->256->512; ELU, conv 512->512 k3; hop 120 = 200 Hz) -> 2-layer transformer (d 512,
8 heads, FFN 2048, GELU-tanh, LayerNorm, RoPE, causal with context 250, layer_scale 0.01) ->
`downsample` conv 512->32 k32 s16 replicate-pad, no bias (200 -> 12.5 Hz) -> 32-dim latents
-> `speaker_proj` [1024 x 32] -> prefix embeddings, with `bos_before_voice` [1024] prepended.

Backbone (`flow_lm.transformer`): 6 layers, d_model 1024, 16 heads of 64, FFN 4096 GELU-tanh
no bias, pre-LN LayerNorm eps 1e-5 with bias, fused `in_proj` [3072 x 1024] (q | k | v, each
[H x 64]), `out_proj` [1024 x 1024], no biases, no layer scale, RoPE max_period 10000 on
adjacent pairs, full causal (no window), linear KV cache with an absolute offset. Sequence =
[bos_before_voice, voice frames..., text tokens..., generated frames...]; every generated
frame enters through `input_linear` [1024 x 32] (a NaN latent means BOS -> `bos_emb` [32]).
Output: `out_norm` LayerNorm -> `out_eos` [1 x 1024] + bias (EOS when > threshold) and the
conditioning vector for the head.

Flow head (`SimpleMLPAdaLN`, 512 wide): x0 ~ N(0, temp) [32] -> `input_proj` 32->512;
y = `cond_embed` 1024->512 (c) + (te(0) + te(1)) / 2 where te is the sinusoidal timestep
embedder (256 freqs -> 256->512, SiLU, 512->512, a variance-based RMSNorm); 6 ResBlocks
(adaLN: SiLU, 512->1536 -> shift, scale, gate; LN eps 1e-6 affine; modulate x*(1+scale)+shift;
MLP 512->512 SiLU 512->512; x + gate*h); FinalLayer (LN no affine, adaLN 512->1024, linear
512->32). One LSD step: x1 = x0 + v(s=0, t=1, x0). Latent = x1 * emb_std + emb_mean [32].
**At one step the two timestep embeddings are constants** - bake them at load and the head is
six dense blocks of GEMVs; the variance-RMSNorm quirk never runs at inference.

Mimi decode per frame (80 ms = 1920 samples): `quantizer.output_proj` 32->512 (1x1) ->
`upsample` depthwise ConvTranspose 512 ch k32 s16 no bias (12.5 -> 200 Hz, 16 steps) ->
decoder transformer (same shape as the encoder's: 2 layers, 512, context 250, layer scale)
-> SEANet decoder: conv 512->512 k7; [ELU, ConvTranspose 512->256 k12 s6, ResBlock(256:
ELU conv k3 256->128, ELU conv k1 128->256)]; [ELU, ConvTr 256->128 k10 s5, ResBlock(128)];
[ELU, ConvTr 128->64 k8 s4, ResBlock(64)]; ELU, conv 64->1 k3. All convs causal: a conv
carries `kernel - stride` input samples of left context (zeros at start, "constant" pad
mode; the downsample conv replicates the first sample), a transposed conv carries a
`kernel - stride` partial tail it adds into the next call. Streaming and one-shot are the
same arithmetic: one-shot = left-pad zeros, run, trim the last `K - S` of every transposed
output. Sample count is exactly `frames * 1920`.

Per-frame work: backbone ~77M MAC (a GEMV over 75 MB of weights, memory-bound), head ~10M,
Mimi decoder ~260M MAC (the 16-step transformer 100M, the conv stack 160M, GEMM-shaped).
About 4.4 GMAC per second of audio.

## Kernel inventory: what exists, what adapts, what is new

| operator | reference | engine today | verdict |
|---|---|---|---|
| unigram SentencePiece | `sentencepiece` unigram + byte fallback | `dasllama_spm.das` (BPE-scored), `dasllama_bpe.das` | **NEW**: Viterbi over 4000 scored pieces, byte fallback, dummy prefix; ~150 lines |
| text LUT embedding | `nn.Embedding` 4001x1024 | token embedding reads everywhere | table read, trivial |
| causal self-attention with KV cache, RoPE, optional window | `StreamingMultiheadAttention` | `dec_self_attn` in `dasllama_whisper.das` (private, per-head packs, gemm_f32 + softmax), `attention_rows` in the block home (bidirectional, no cache) | **NEW in the block home**: `attention_causal_rows` with a cache `[L][H][hs][Tmax]` and a `context` window, channel-major twin + split-invariance cell per REVIEW_TTS; modeled on the Whisper one |
| RoPE, adjacent pairs, theta 10000 | `apply_rope` (view D/2 x 2) | `rope` / `rope_scaled` in `dasllama_math.das` - same pair convention | exists; parity at float tolerance (freq via pow vs exp) |
| LayerNorm (affine, eps 1e-5 / 1e-6; one no-affine) | `nn.LayerNorm`, custom `LayerNorm` | `layernorm_rows` (TtsNorm, eps param) | exists; no-affine = unit weights |
| GELU tanh | `F.gelu(approximate="tanh")` | `gelu` (`dasllama_math`), `gelu_tanh_lut` (tower) | exists (LUT form is a parity question: use the exact form on the reference lane) |
| SiLU | `nn.SiLU` | `silu` (`dasllama_math`) | exists |
| ELU (alpha 1) | `nn.ELU` | none | **NEW**, elementwise, `x > 0 ? x : exp(x) - 1` on the JIT vector exp |
| layer scale | `LayerScale` per-channel multiply | `scale_rows` is one scalar | **NEW** one-liner: per-channel scale over rows |
| adaLN modulate + gate | `modulate`, `x + gate * h` | `ada_layernorm_rows` (StyleTTS2's gamma/beta form) | **NEW** small: `modulate_rows(x, shift, scale)`, `gate_add_rows`; all rows = 1 at inference |
| dense linear, M=1 (decode GEMV) | `nn.Linear` | `linear_vec` (f32), `linear_rows` (f32 / q8 tap) | exists; q8 GEMV for the backbone step = perf rung, the LLM engine's q8 gemv is the target kernel |
| dense linear, M=rows (prompt, codec) | `nn.Linear` | `linear_rows` f32 / q8 | exists |
| causal conv k3 / k7 / k1, dense | `StreamingConv1d` | `conv1d_rows` dense / q8 tap-stacked | exists + a left zero pad (new `pad_zero_left_rows`, or the reflect-left helper's zero twin) |
| strided conv k = 2s (encoder, downsample) | `StreamingConv1d` stride | `conv1d_rows_dense_strided` (the noise branch is this exact shape) | exists; replicate pad for the downsample conv = `pad_edge`'s rows twin |
| transposed conv k = 2s dense | `StreamingConvTranspose1d` | `conv1d_rows_transposed` | exists + trim `K - S` |
| transposed conv depthwise k32 s16 | `ConvTrUpsample1d` groups=512 | `conv1d_rows_transposed_depthwise` | exists + trim |
| normal RNG for x0 | `torch.randn` * sqrt(temp) | `rng_normal` (polar), `TtsNoise.captured` oracle mode | exists; the oracle dumps x0 per frame for parity |
| timestep sinusoid + MLP | `TimestepEmbedder` | none needed | baked to two constants at load (one step) |
| EOS head | linear 1024->1 | dot | trivial |
| KV-state snapshot per voice | `copy.deepcopy(model_state)` | none | **NEW** plain: copy the cache rows of the voice prefix back before each chunk |

Nothing on the list needs a new GEMM backend, a new quant format or a GPU driver. The two
real pieces of work are the unigram tokenizer and the cached causal attention in the block
home; the rest is assembly.

## File charters (proposed, mirrors ARCHITECTURE_TTS.md sec.1.7c)

| file | owns |
|---|---|
| `dasllama/dasllama_ugm.das` (or a unigram arm inside `dasllama_spm.das` - ruling) | the unigram SentencePiece tokenizer: pieces + scores from GGUF metadata, Viterbi segmentation, byte fallback, dummy prefix |
| `dasllama/dasllama_tts_blocks.das` | gains ELU, per-channel layer scale, adaLN modulate/gate, the cached causal attention rows kernel (+ channel-major twin), zero/replicate left pads over rows. Operators only, per REVIEW_TTS |
| `dasllama/dasllama_pocket.das` | the Pocket family: the weight map of the converted GGUF, the assembly (voice prompt -> state; text prompt; the frame loop: backbone step, EOS, head, latent; the Mimi decoder one-shot over the chunk's latents), voice-state cache per voice name, the chunker's family rule (the reference splitter: sentence split, 50-token regroup, capitalize, terminal punctuation, semicolon policy, short-input padding), `PocketFamily` data record on the carrier |
| `dasllama/dasllama_tts_types.das` | `TtsCaps.cloning` becomes true for the first time; `TtsTimings` gains family-neutral stage slots (ruling: rename or add `prompt_us`, `backbone_us`, `head_us`, `codec_us`) |
| `dasllama/dasllama_tts.das` | facade dispatch on `general.architecture = "pocket-tts"`; a cloning verb (`tts_register_voice(m, name, pcm, rate)` -> the voice joins `caps().voices` for the session); `synthesize_stream` unchanged in shape - per chunk, per family rule |
| `harness/convert_pocket.py` | gated safetensors (pinned revision) + tokenizer.model + the voice-zero WAVs -> `pocket-tts-<lang>.gguf` (f16 weights; unigram pieces/scores, config scalars, `voice.<name>` PCM tensors as metadata/tensors) |
| `harness/pocket_oracle.py` | reference stage dumps: tokenizer ids, voice state (the prefix embeddings and the post-prompt KV cache), per-frame backbone output / x0 / latent, Mimi decoder input latents and output PCM, for N fixture sentences x M voices -> `tts_oracle/pocket/` (the TTSO container `_tts_oracle.das` already reads) |
| `tests/test_tts_pocket.das` | tokenizer parity (200-sentence fixture -> ids), voice-state parity, per-frame latent parity with captured x0, Mimi decoder parity, EOS frame parity, end-to-end waveform gate (informational, as StyleTTS2's) |
| `tests/test_tts_blocks.das` | the new rows kernels' twin cells + both split axes |
| `tests/test_tts_facade.das` / server speech test | caps (cloning true, six langs across files), a registered voice speaks, refusal of an unknown voice |

Registration: `.das_module` + `CMakeLists.txt` rows, ARCHITECTURE_TTS 1.7d, REVIEW_TTS
additions (the trim rule, the one-step bake rule), `THIRD_PARTY_NOTICES.md` (Kyutai: code
MIT, weights CC BY 4.0, `LICENSE.CC-BY-4.0` vendored, the gate's acceptable-use text quoted
on the model card), `tests/CLAUDE.md` rows, `model_specs.das` rows + `serve_tts_set`, the HF
repo `borisbat/dasllama-tts` gains the Pocket files with the card's licence table extended.

## Receipts (2026-09-09, M1 Max, f32 lane, `tests/test_tts_pocket.das`)

Every das figure in this section ran on the M1 Max (10 cores, the m1 tune manifest) under
`bin/daslang -jit`, the default tune policy, the lane the row names; the rig rows through
`harness/tts_rig.py --models pocket-tts-en:alba` (`--q8` / `--f32`, 200 sentences, parakeet WER
+ UTMOS), the parity figures through `tests/test_tts_pocket.das` over `harness/pocket_oracle.py`'s
dumps, the per-stage timings from the `timings_line` the facade logs per synthesis.

Reference arm (pip pocket-tts 3.1.0, alba, the 200-sentence fixture through `harness/tts_rig.py`):
WER 5.00 / UTMOS 4.393 / RTF 0.210 (the package pins torch to one thread); by category harvard
0.86, ljspeech 5.66, heteronym 0.25, oov 10.89, numeric 10.42 - the reference normalizes no
numbers. Parity against `harness/pocket_oracle.py` (24 cases, alba and caro_davy): codec decoder
3e-6 abs on a 0.6 peak, encoder 2e-5 on 8, voice prompt KV 2e-4 on 9, backbone conditioning
8e-6, EOS logit 1.5e-5, the head alone 4e-6, teacher-forced latents 2e-4, waveform 1.6e-4 on 0.8;
the free run lands the oracle's frame count. First das RTF, unoptimized f32: 0.15 (backbone
7.8 ms per frame, read off the `timings_line` of one `synthesize` on the f32 lane, `-jit`, M1
Max - the tiled GEMM run as a GEMV; the q8 GEMV is phase 6's first rung).

The q8 lane (same day): the transformer GEMMs and the 32-wide codec convs as Q8_0 rows, the
decode step on the q8 GEMV entry. The rig at alba, 200 sentences, the same scorer:

| lane | WER | UTMOS | RTF | numeric | oov |
|---|---|---|---|---|---|
| reference (pocket-tts 3.1.0, torch, one thread) | 5.00 | 4.393 | 0.210 | 10.42 | 10.89 |
| das f32 | 4.13 | 4.368 | 0.143 | 2.08 | 10.51 |
| das q8 | 4.09 | 4.339 | 0.056 | 2.38 | 11.67 |
| das, the published Q8_0 file | 4.13 | 4.330 | 0.057 | 2.38 | 10.51 |

The WER gain is the English normalizer in front of the tokenizer (numbers, units and
abbreviations as words); q8 holds WER and costs 0.03 MOS. The published form is the Q8_0
file (`convert_pocket.py --q8`, 152 MB for English, 56 tensors as Q8_0 in the kernels' layout,
read straight into the int8 planes); its own rig row is the number on the card.

Two facts the source reading missed: the installed package (3.1.0, what the oracle ran) and
GitHub main differ in the chunker's terminal-punctuation rule (main replaces a trailing
comma by a period; 3.1.0 appends a period only after a letter or digit) - the port follows
3.1.0; and the timestep embedder's RMSNorm epsilon is 1e-5 and load-bearing, since those
activations' variance is 1e-4: at 1e-6 every latent was off by one percent.

## Phases (checkpoints, not PRs - one PR for the arc if ruled as the TTS arc was)

0. **Access and the bar.** Boris accepts the gate; `hf download` the six gated bundles, the
   tokenizers, `kyutai/tts-voices` voice-zero at a pinned revision. A venv with pocket-tts
   (torch 2.5+, CPU) on the M1: reference RTF on the 200-sentence fixture with `alba`, WAVs
   through the existing rig (parakeet WER, UTMOS) = the reference arm E for this family. The
   oracle script lands here. Receipt: the reference row in the rig table.
1. **Converter + tokenizer.** `convert_pocket.py`; the unigram tokenizer; the ids fixture
   cell green on 200 sentences (byte-fallback cases included: an emoji, a Cyrillic word).
2. **Mimi decoder, one-shot.** Oracle latents in, PCM out, parity at the StyleTTS2 waveform
   bars. The trim rule pinned by a cell that compares one-shot to the oracle's streamed
   output frame by frame.
3. **Backbone + head.** Text-only prompt with the packaged voice state loaded from the
   oracle (so cloning is not on the path yet): per-frame latent parity with captured x0,
   EOS frame index parity, then the whole chunk through the decoder = the first das
   sentence. Timings per stage.
4. **Cloning.** The Mimi encoder + downsample + speaker_proj; voice state parity against
   the oracle's post-prompt cache; the bundled voice-zero roster encoded at load (or lazily
   - ruling); `tts_register_voice` on the facade; `caps().cloning = true`.
5. **Product.** txt2wav `--tts pocket-tts-en.gguf --voice alba`, the server route (voice by
   name; an upload route is a ruling), catalog rows, HF publish, tutorial section, the
   control page's tts card; the five other languages as five GGUFs (their front end is the
   tokenizer alone: our normalizer is English-only, the reference does no normalization in
   any language, so parity with the reference holds and a normalizer per language is a
   later quality rung).
   Landed 2026-09-09: the server serves a Pocket file standing alone (`tts_needs_packs`
   decides the file set), the `tts` block of `/v1/stats` gains `cloning`, `speed` and `lang`,
   `/v1/audio/phonemes` answers a Pocket model's chunks with empty phoneme strings, a speed on
   it is a 400, `--tts-voices-dir` / `tts_voices_dir` clones every clip of a directory at boot
   under its stem (`test_pocket_voices_dir`), the control page shows the acceptable-use terms
   beside the voice picker of a cloning model and drops the speed knob where `speed` is false,
   `caps().speed` is the facade's own word for it, the tutorial gains `--clone` and the
   sections that read the new caps, and `load_audio_mono(path, rate)` is the clip decode rail.
   The upload route is followup_general.md row 125.
6. **Perf.** The `[hot_path]` carrier (`PocketScratch`), the image rail (`.dlim`, q8 lane:
   backbone and codec GEMMs as Q8_0 tap-stacked rows; the 1->64 and 64->1 convs stay f32),
   the q8 GEMV for the decode step, then frame-block decoding for first-chunk latency.
   Rig-gated: WER/UTMOS must hold at the phase-3 numbers.

Prediction (the game): on the M1 Max, JIT, q8 lane, warm - RTF <= 0.05 for a 10-second
sentence (the reference's torch path is framework-bound per 80 ms frame; ours is a 75 MB
GEMV plus 260M MAC of GEMM per frame), first audio chunk under 100 ms after the text prompt.
Kokoro sits at 0.069-0.075 on the same box (`harness/tts_rig.py --models kokoro-82m --q8`,
`-jit`, the rig table of `plans/dasllama-tts.md`).

## Design rulings (Boris, 2026-09-09)

1. ONE PR, long arc, on the M1 Max; the phases are checkpoints inside it.
2. One family file, `dasllama_pocket.das`, holding the family rules and the assembly; the
   StyleTTS2 split is not repeated until a second model shares this lineage.
3. The unigram tokenizer is an arm inside `dasllama_spm.das`, keyed on the GGUF value `t5`
   (upstream's name for a unigram SentencePiece model) so the reader also serves any
   upstream-converted T5-class model; piece table, scores and byte fallback shared with the
   BPE arm, only the segmentation differs.
4. The bundled voices ride the GGUF as `voice.<name>` PCM tensors (Kokoro's pack pattern),
   encoded into a voice state on first use and cached for the session; the file stays
   self-contained and the image serves without its GGUF. CORRECTION after the ruling: the
   package's 21-name English roster is NOT all voice-zero - it draws on six folders of
   `kyutai/tts-voices` (`_ORIGINS_OF_PREDEFINED_VOICES` in `pocket_tts/utils/utils.py`):
   voice-zero (4, CC0), voice-donations (2, CC0), vctk (12, CC BY 4.0), alba-mackenna (alba,
   CC BY 4.0), and two from CC BY-NC folders - `cosette` (expresso) and `jean` (ears). The
   shipped roster is the 19 with attribution on the card; the two non-commercial ones are
   left out. The five non-English defaults (giovanni, lola, juergen, rafael, estelle) are
   files in the `kyutai/pocket-tts` repo itself (Common Voice clips, CC0) and one
   unmute-prod-website clip; each is checked at conversion.
5. Server cloning by NAMED voices only in this arc: the GGUF roster plus a `tts_voices_dir`
   config key whose WAVs join `caps().voices` at boot; an upload route is a ledger row. The
   gate's acceptable-use text goes on the model card, the server README and the control
   page's tts card beside the voice picker; legal sees the wording once.
6. `TtsTimings` keeps its fields (the server stats and the studio read them) and gains four:
   `prompt_us`, `backbone_us`, `head_us`, `codec_us`; a family leaves the stages it lacks at
   zero and `timings_line` prints the non-zero ones.
7. All six languages ship in the PR: English carries the parity rails and the rig; the five
   others get the converter run, a tokenizer-parity cell and a waveform check against the
   reference. A per-language WER arm through the engine's own multilingual Whisper is
   possible without new tooling and does not gate the PR.
8. q8 after parity on the f32 lane, never before.
9. No watermark (CC BY 4.0 carries no duty; Kyutai ships none); legal confirms.
10. Each language's default temperature (0.3 English, 0.7 the rest) rides as GGUF metadata
    and is `synthesize`'s default.

## Licensing (verified 2026-09-09)

Code MIT (`LICENSE`, Resemble-style plain MIT, Kyutai). Weights CC BY 4.0 on the HF card,
behind a click-through acceptable-use agreement (quoted in `THIRD_PARTY_NOTICES.md` when it
lands). Default voices `voice-zero/` CC0; the wider `kyutai/tts-voices` bank mixes CC BY 4.0,
CC0 and CC BY-NC 4.0 (`expresso/`, `ears/`) - never take a roster from the mixed folders.
Training data includes Emilia (original release CC BY-NC 4.0); Kyutai declared CC BY 4.0
regardless, which is their call and the field's norm - noted, not actionable. SentencePiece
the library is Apache-2.0; the tokenizer model file is part of the weights.

## Part 2 (ruled 2026-09-09): the 4-bit lane

The Hugging Face upload of the Q8_0 files happens at the END of the arc, once every language is
converted, not per milestone. Part 2 is followup_general.md row 124: the engine's 4-bit weight
formats on the same GEMMs the q8 lane quantizes, one format at a time, each a rig row on both
lanes, the flow head f32 throughout; the winning format becomes the published file (about 80 MB
for English against the Q8_0 file's 152).

## Not this arc

Hindi (not a Pocket language; stays on the Kokoro ledger row), the Kyutai training rail
(adding a language needs a GPU and speech data), per-language normalizers, Metal/Vulkan
drivers for the backbone (the sec.2.14 hook slot pattern applies later, decode-step first),
Chatterbox.
