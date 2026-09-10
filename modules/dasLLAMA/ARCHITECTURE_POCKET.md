# dasLLAMA architecture - the Pocket TTS family

The companion `ARCHITECTURE.md` indexes: the Pocket TTS charter (sec.1.7d) and the mechanisms
the family implements (sec.2.46-2.50). `ARCHITECTURE_COMMON.md` (repo root) is the contract.
The TTS block home, facade and phoneme families are `ARCHITECTURE_TTS.md`.

## 1. File charters

### 1.7d Pocket TTS

- **`dasllama_pocket.das`** - the Pocket TTS family (Kyutai; the reference is the `pocket-tts`
  package at 3.1.0, the weights `kyutai/pocket-tts`): a continuous-audio
  language model, not a StyleTTS2 assembly, so it holds both the family's rules and its own
  assembly. The weight map of the converted GGUF (`harness/convert_pocket.py`: the canonical
  tensor names `backbone.N.*`, `head.*`, `mimi.enc_tf.N.*` / `mimi.dec_tf.N.*`, the rest as the
  bundle names them; the `pocket.*` scalars from the package's per-language config; the
  unigram tokenizer under `tokenizer.ggml.model = "t5"`; the roster as `voice_latents.<name>`
  latent frames - an older file carries `voice.<name>` PCM instead, encoded on first use;
  `pocket.cloning` says whether the codec encoder is inside), the model (`PocketModel`: the
  causal backbone, the one-step flow head, the Mimi-derived codec with or without its encoder,
  the roster and its voice states built from the stored frames), the activation carrier
  (`PocketScratch`), and the assembly - the voice prompt (sec.2.47), the text prompt, the frame
  loop (sec.2.48), the codec decoder over a chunk's latents (sec.2.46) - plus the reference
  driver's text preparation and chunker (sec.2.49). `pocket_speak` is the facade's entry; the
  seams `pocket_encode_latents`, `pocket_decode_latents`, `pocket_voice_state`, `pocket_head`
  and `pocket_synthesize` (with the oracle's noise draws and teacher-forced frames) are what
  `tests/test_tts_pocket.das` holds against `harness/pocket_oracle.py`. Every operator is the
  block home's: the transformer layer runs on `linear_rows`, `layernorm_rows`, `rope_rows`,
  `attention_causal_rows` over a `TtsKvCache`, `gelu`, `layer_scale_rows`; the codec on
  `conv1d_rows`, `conv1d_rows_transposed_depthwise` and `elu_rows`. One language per file:
  `pocket.language` names the package config the GGUF came from, and `lang` the code `caps`
  reports; every roster voice speaks that language.

## 2. Mechanisms

### 2.46 The codec runs a chunk in one shot {#pocket-one-shot-codec}

The reference streams the codec frame by frame through convolutions that carry state: a forward
conv keeps its last `kernel - stride` input samples, a transposed conv a `kernel - stride`
partial tail it adds into the next call. Both are exactly a causal conv over the whole sequence:
the forward conv's state is a left zero pad of `kernel - stride` (the downsampling conv
replicates the first row instead), the transposed conv's tail is the last `kernel - stride`
outputs the stream never emits. The converted geometry says so - `pad_l = k - stride` on a
forward conv, `pad_r = k - stride` on a transposed one - and the rows kernels apply it, so a
chunk's latents decode in one pass to exactly `frames * frame_samples` samples, and the oracle
script checks the claim against the package's own frame-by-frame output (1e-6 apart).
Nothing in the family carries conv state.

### 2.47 A voice is the backbone's memory of a clip {#pocket-voice-state}

There is no speaker vector. A clip at 24 kHz runs through the codec encoder to 32-float latent
frames at 12.5 Hz, through `speaker_proj` into the backbone's width, and - with the BOS row in
front - through the backbone at positions 0.., filling every layer's key-value cache. That cache
(`PocketVoiceState`, `len` positions) is the voice. A synthesis appends its text and frames after
`len` and a later one forgets them by resetting each cache's fill to `len`; nothing is copied.
The caches are sized for the clip plus 1024 rows and grow, the voice's rows kept, when a chunk's
text plus every frame its cap allows needs more - one unsplittable run of two hundred tokens is
such a chunk. A clip is at most 60 s (`POCKET_MAX_VOICE_SECONDS`): the state is the clip's frames
per layer, and the codec encoder's attention is a query block by the 250-key window it sees.
The roster rides the GGUF as each clip's latent frames (`voice_latents.<name>`, the package's
own codec encoder over the clip at conversion), and a voice's state is built from them on first
use - the second half of the clip path, no encoder needed; a file of the older form carries the
clips themselves (`voice.<name>`) and encodes them on first use. A cloned voice is the whole clip
path over a caller's clip (`tts_register_voice`), so it needs the encoder: a file converted
`--no-cloning` leaves the encoder out, says so in `pocket.cloning`, reports `cloning = false` in
`caps()` and refuses a clip by name. The package's precomputed states differ from the clip path
by 1.5e-2 (they come from another checkpoint revision; `harness/pocket_oracle.py` dumps both and
`test_pocket_parity`'s voice cell compares the clip path); the clip path is the reference.

### 2.48 One flow step makes the timestep embeddings constants {#pocket-one-step-head}

The head samples a frame as `x1 = x0 + v(0, 1, x0 | cond)`: one Lagrangian self-distillation
step from noise `x0 ~ N(0, temperature)`. Its conditioning is `cond_embed(cond)` plus the mean of
two sinusoidal timestep embeddings at the fixed times 0 and 1, so the loader evaluates both
embedders once and bakes their mean (`time_const`); at inference the head is dense GEMVs alone.
The embedders end in a variance-scaled norm (`x * alpha / sqrt(var + 1e-5)`, the variance with N
- 1) whose activations have a variance near 1e-4, so its epsilon of 1e-5 is load-bearing - at
1e-6 every latent lands one percent off (`test_pocket_frames`'s head cell against the oracle's
flow output is what moved). The backbone's input for the first frame is the
learned BOS latent; for the next, the previous frame's normalized `x1`; the codec reads
`x1 * emb_std + emb_mean`. Generation runs to EOS (the logit over `out_norm`'s row against
-4.0) plus `frames_after_eos` (the config's, else the text-length guess plus 2), capped at
`ceil((tokens / 3 + 2) * 12.5)`.

### 2.49 The chunker is the package's, at its released version {#pocket-chunker}

`pocket_chunks` reproduces `split_into_best_sentences` of pocket-tts 3.1.0 on the tokenizer's
ids: the whole text prepared (whitespace folded, a capital first letter, a period after a
trailing letter or digit, the short-input padding), split after every run of the sentence-mark
ids `.`, `!`, `...`, `?` except a decimal point, an over-long sentence re-split after its `,`,
`;`, `:` ids, the pieces decoded and regrouped under the 50-token budget. GitHub main has since
changed the terminal-punctuation rule; the port follows the release the oracle ran. The
reference splits abbreviations ("Dr." ends a sentence) and joins segments with spaces
("p.m.;" reads back "p. m. ;"); a chunker of our own is a quality rung for the rig, not a
parity matter. The English facade path normalizes the text first (numbers, units, abbreviations
become words) - the reference does not, which is where its numeric WER comes from; a model in
another language takes the text as it is, since the normalizer reads English.

### 2.50 The published file carries the served quants {#pocket-q8-file}

A file has three lanes and its formats decide which it can take. A K-quant dense layer
(`convert_pocket.py --kq`: the backbone's and the codec transformers' matrices as Q4_K, the flow
head as Q8_0, the rest as the q8 form writes it) serves as its own planes unless a lane is
pinned - `TtsLinear` holds the plane pair the GGUF transcoder wrote, repacked where the backend
the load selected carries kq kernels (the load line says which arm), and the frame loop's GEMV
and the prompt's GEMM take the engine's own K-quant entries (`linear_rows_decode`,
`linear_rows_kq`), the rows requantized to the Q8_K form the way the engine's own decode does.
The text embedding table is Q4_K on disk only: it is a lookup, and dequantizes at load on every
lane. Pinned q8 or f32, a K-quant tensor dequantizes into that lane, so one file serves every
lane and the rig compares them on the same sentences. A vector layer the file stores Q8_0 (the
head) runs its GEMV on the q8 lane.

The two lanes every Pocket file has, as the StyleTTS2 families have them: f32, the parity
rail's reference, and q8, the served default - the transformer layers' four matrices, the frame
input projection and every
dense stride-1 codec conv on 32-wide channels as Q8_0 rows (`linear_prepare`,
`conv1d_q8_eligible`), the decode step on the q8 GEMV entry. The published GGUF
(`convert_pocket.py --q8`) stores exactly those tensors as Q8_0 in the layout the kernels read
- a linear as [nout][nin], a conv as the tap-stacked slab [cout][k][cin] with the 32-blocks
along the input channels - so `read_linear` and `read_conv_q8` take the blocks into the int8
planes and repack for the backend; every other tensor stays f16, and the f32 lane of such a
file serves the same weights dequantized. The converter's `q8_linear` / `q8_conv` are the
engine's eligibility rule written a second time; `test_pocket_q8_file` holds the two files to
each other and the rig holds the published file to the reference. The file's block scales
are f16, the load-time quantizer's f32, and the flow head amplifies that half a thousandth per
block into about one percent of a frame (`test_pocket_q8_file`'s teacher-forced compare of the
two loads) - the same picture as the lane itself against the f32
oracle, and why the rig, not a per-element bar, is the q8 gate.
