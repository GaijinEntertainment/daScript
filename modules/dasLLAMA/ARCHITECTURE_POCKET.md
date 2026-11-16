# dasLLAMA architecture - the Pocket TTS family

The companion `ARCHITECTURE.md` indexes: the Pocket TTS charter (`ARCHITECTURE_POCKET.md#pocket-tts`)
and the mechanisms the family implements. `ARCHITECTURE_COMMON.md` (repo root) is the contract.
The TTS block home, facade and phoneme families are `ARCHITECTURE_TTS.md`.

## File charters

### Pocket TTS {#pocket-tts}

- **`dasllama_pocket.das`** - the Pocket TTS family (the reference build is the family's own
  package at its 4.0.1 release, whose weights the converter reads): a continuous-audio
  language model, not a StyleTTS2 assembly, so it holds both the family's rules and its own
  assembly. The weight map of the converted GGUF (`harness/convert_pocket.py`: the canonical
  tensor names `backbone.N.*`, `head.*`, `mimi.enc_tf.N.*` / `mimi.dec_tf.N.*`, the rest as the
  bundle names them; the `pocket.*` scalars from the package's per-config YAML - the backbone's
  `attn_dim`, `kv_heads` and `ffn`, the `z_dim` bottleneck, the `text_rank`, the head's
  `flow_type`, the decoder side's `pocket.mimi.dec.*`, and the driver rules of
  `ARCHITECTURE_POCKET.md#pocket-one-step-head` and `ARCHITECTURE_POCKET.md#pocket-chunker`; the
  unigram tokenizer under `tokenizer.ggml.model = "t5"`; the roster as `voice_latents.<name>`
  latent frames - an older file carries `voice.<name>` PCM instead, encoded on first use;
  `pocket.cloning` says whether the codec encoder is inside), the model (`PocketModel`: the
  causal backbone - `PocketTransformer` carries `heads` query heads over `kv_heads` key-value
  heads of `dh = attn_dim / heads`, one each on the 100M models, eight over two (7m) or three over
  one (2m) on the 2026-10 small ones, which also project the queries wider than the residual; a
  low-rank text table stays low-rank, its projection (`flow_lm.conditioner.proj`) applied to the
  rows a prompt gathers; a `z_proj` bottleneck, where the file has one, narrows `out_norm`'s row to the
  `cond_dim` the head and the EOS logit read - the one-step flow head, the Mimi-derived codec
  with or without its encoder - the decoder side its own SEANet and transformer where the config
  names them (the small models' 128-wide, 12-layer decoder behind the 100M models' encoder) -
  the roster and its voice states built from the stored frames), the activation carrier
  (`PocketScratch`), and the assembly - the voice prompt (`ARCHITECTURE_POCKET.md#pocket-voice-state`), the text prompt, the frame
  loop (`ARCHITECTURE_POCKET.md#pocket-one-step-head`), the codec stream over a chunk's latents or a clip (`ARCHITECTURE_POCKET.md#pocket-codec-stream`) - plus the reference
  driver's text preparation and chunker (`ARCHITECTURE_POCKET.md#pocket-chunker`). `pocket_speak` is the facade's entry; the
  seams `pocket_encode_latents`, `pocket_decode_latents`, `pocket_voice_state`, `pocket_head`
  and `pocket_synthesize` (with the oracle's noise draws and teacher-forced frames) are what
  `tests/test_tts_pocket.das` holds against `harness/pocket_oracle.py`. Every operator is the
  block home's but the residual add, the towers' `add_inplace_rows`: the transformer layer runs on
  `linear_rows`, `layernorm_rows_into`, `rope_rows`, `attention_causal_rows` over a `TtsKvCache`,
  `gelu`, `layer_scale_rows`; the codec on `conv1d_rows`, `conv1d_rows_transposed_depthwise` and
  `elu_rows`. The codec decoder, the
  frame loop and the text prompt are the three seats of the family's hook record (`PocketGpuDriver`,
  `register_pocket_gpu`, `pocket_gpu_stats`; `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec` and
  `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`): a registered driver gets the first refusal of `pocket_decode_latents`, of the
  prompt's rows into the caches (`prompt_rows`, over the embedding rows `pocket_prompt_embed` gathers on
  either rail) and of the loop inside `pocket_synthesize` once those rows sit there, and the CPU form
  serves a decline; a served loop's wall reads as the backbone's timing, its head timing zero. A prompt's
  residual is read by nothing after its last layer's keys and values, so the CPU chain runs the prompt and
  the voice state through `transformer_rows` under `kv_only_last` - every layer whole but the last, which ends at its cache append.
  Both drivers run the served loop's host side through `pocket_frames_batched` (the batch of
  frames a submit carries, the noise draws, the end-of-speech check) at the one batch knob
  `set_pocket_frame_batch` / `pocket_frame_batch()`.
  One language per file:
  `pocket.language` names the package config the GGUF came from, and `lang` the code `caps`
  reports; every roster voice speaks that language. The 2026-10 small English models
  (`english_18m_2026-10`, `english_7m_2026-10`, `english_2m_2026-10` and their reward-tuned
  `_refl_` twins) are files of this family like any other; what sets them apart is metadata the
  reader folds into the same assembly. Every GPU seat serves them on both homes - the codec
  (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`), the frames and the prompt
  (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`).

## Mechanisms

### The codec runs a chunk in windows, every conv carrying its rows {#pocket-codec-stream}

The codec's activations sit at audio rate - about 20 MB per second of audio across the chain's
ping-pong rows, on the encoder and the decoder alike - so a chunk or a clip run in one pass is
the say's working set, and on a phone's browser tab the one that runs it out of memory. The
decoder runs a chunk `POCKET_CODEC_WINDOW_DEFAULT` latent frames per step (`set_pocket_codec_window`,
0 for the whole run), the encoder a clip the same count of frames in samples, and the
activations scale with the window instead of the run. Each conv is causal, so a window's
output depends on the window and on the rows before it that the conv's taps reach: a forward conv
its last `k - stride` inputs, a transposed one (every one in the file is `k = 2 * stride`) its
last input row. `PocketConvCarry` holds those rows per conv, `stream_conv` runs the conv over
the carried rows followed by the window with no pad at all (`read_conv` zeroes both pads), takes
the window's own output rows - a transposed conv's are the ones after the carried rows' stride
and before the tail its taps leave unfinished - and keeps the window's tail as the next carry.
The first window's carry is the reference's pad: `k - stride` zero rows on a forward conv
(`carry_init`), `frame_steps` copies of the first row on the encoder's downsampler
(`carry_replicate`), nothing on a transposed conv. The two codec transformers stream through
their KV caches at the window's position, as the backbone already does. A window is one output
row's worth of sums in the same order the whole run computes them, so on the f32 lane the
windowed output equals the one-shot output to float noise at every window, one frame included
(`test_pocket_codec_stream`); on the q8 lane the served GEMMs' result depends on how many rows
they batch, so a window's output differs from the one-shot's the way two chunk lengths already
do, and the rig, not a per-element bar, holds that lane. The oracle script's own check - the
package's whole-chunk decode against its frame-by-frame stream, 1e-6 apart - is the reference's
statement of the same fact. What a run leaves behind is the carrier (`PocketScratch`, sized to
the largest window it saw) and the block home's scratch globals - the transposed conv's tap
lift, a cache-sized block of input rows at a time, the attention head rows, the requant images;
`pocket_release_scratch` (the facade's `tts_release_scratch`) frees them all and the next run
grows them back, for a platform where idle memory matters more than the allocation a run then
pays.

### A voice is the backbone's memory of a clip {#pocket-voice-state}

There is no speaker vector. A clip at 24 kHz - trimmed first to end on exactly 80 ms of silence
where the file's driver trimmed its prompts (`pocket.prompt_end_on_pause`, the block home's
`end_on_pause`: the trailing 20 ms frames more than 35 dB under the loudest are cut, the last
20 ms fade out, the pause follows; `pocket_prompt_pcm` is that step, and `tts_prepare_voice_prompt`
takes it too) - runs through the codec encoder to 32-float latent
frames at 12.5 Hz, through `speaker_proj` into the backbone's width, and - with the BOS row in
front - through the backbone at positions 0.., filling every layer's key-value cache. That cache
(`PocketVoiceState`, `len` positions) is the voice. A synthesis appends its text and frames after
`len` and a later one forgets them by resetting each cache's fill to `len`; nothing is copied.
A model keeps the `POCKET_VOICE_STATES` (2) states used last and frees the least recently used
past them: a voice's stored form is its frames, 15 KB against the state's megabytes, and a freed
voice rebuilds from them on its next use - one backbone pass over the clip's frames
(`PERF_LEDGER.md`'s "TTS weight formats" entry times it) - where a voice of the older file form,
stored as its clip, runs the codec encoder again. One synthesis runs on a model at a time, so the
state a chunk continues from is the one stamped last.
The caches are sized for the clip plus the rows a chunk of the reference's budget can take -
`POCKET_MAX_TOKENS` text rows and every frame its cap allows, about 285 - and grow by a small
slack, the voice's rows kept, when a chunk needs more - one unsplittable run of two hundred
tokens is such a chunk; a voice is about 20 MB of keys and values, not the 54 MB a flat 1024-row
slack cost. A clip is at most 60 s (`POCKET_MAX_VOICE_SECONDS`): the state is the clip's frames
per layer, and the codec encoder's attention is a query block by the 250-key window it sees.
The roster rides the GGUF as each clip's latent frames (`voice_latents.<name>`, the package's
own prompt path - the trim, then its codec encoder - over the clip at conversion), and a voice's
state is built from them on first
use - the second half of the clip path, no encoder needed; a file of the older form carries the
clips themselves (`voice.<name>`) and encodes them on first use. A cloned voice is the whole clip
path over a caller's clip (`tts_register_voice`), and it keeps the clip's frames as a roster voice
does - 15 KB for a ten-second clip against the clip's megabyte - so it needs the encoder: a file converted
`--no-cloning` leaves the encoder out, says so in `pocket.cloning`, reports `cloning = false` in
`caps()` and refuses a clip by name. A prepared prompt (`TtsVoicePrompt`, the frames of one
clip path) is portable across files that read the same latent space, and that space is named
by a fingerprint every file carries with or without its encoder - the codec geometry and the
backbone's latent statistics (`emb_mean`, `emb_std`), hashed at load (`pocket_latent_space_id`) -
rather than by `pocket.language`, which names a package config: the 2026-04 and the 2026-10
English files share one codec, so a prompt prepared on the 100M model conditions the small
ones, and a file of another codec refuses it at registration whatever its config name says. The reference build's precomputed
voice states for the 2026-10 models match the clip path (`harness/pocket_oracle.py` compares them;
the distances are the small-model leg's `PERF_LEDGER.md` entry); the 2026-04 model's come from
another checkpoint revision and differ from it, and the clip path is the reference.

### One flow step makes the timestep embeddings constants {#pocket-one-step-head}

The head samples a frame in one step from noise `x0 ~ N(0, temperature)` by the file's
`pocket.head.flow_type` (`PocketFlow`): `lsd` - the 100M models - as `x1 = x0 + v(0, 1, x0 |
cond)`, one Lagrangian self-distillation step whose conditioning is `cond_embed(cond)` plus the
mean of two sinusoidal timestep embeddings at the fixed times 0 and 1, so the loader evaluates
both embedders once and bakes their mean (`time_const`); `drifting` (the 18m and 7m models) as
the head's output alone, `pointwise` (the 2m) as the noise plus it, both with no timestep at all.
At inference every head is dense GEMVs alone. The `lsd` embedders end in a variance-scaled norm
(`x * alpha / sqrt(var + 1e-5)`, the variance with N - 1) whose activations have a variance near
1e-4, so its epsilon of 1e-5 is load-bearing - at 1e-6 every latent lands one percent off
(`test_pocket_frames`'s head cell against the oracle's flow output is what moved). The
conditioning row is `out_norm`'s row, or `z_proj` of it where the file has the bottleneck
(`cond_dim`); the EOS logit reads the same row. The backbone's input for the first frame is the
learned BOS latent; for the next, the previous frame's normalized `x1`; the codec reads
`x1 * emb_std + emb_mean`. Generation runs to EOS (the logit against `pocket.eos_threshold` - the
config's EOS threshold, -4.0 on a file minted before the key, -1.0 on the small models -
ignored on a chunk's first `pocket.min_frames_before_eos` frames, six under the 4.0.1 rules, so a
voice whose logit crosses before speech starts does not end a short text unspoken) plus
`frames_after_eos` (the config's, else the text-length guess plus 2), capped at
`ceil((tokens / 3 + 2) * 12.5)`. The decoded chunk's first `pocket.fade_in_samples` ramp in
linearly (5 ms under that driver: a fresh decoder state puts a small step in its first samples,
heard as a click at every chunk's start), on the CPU chain and after a served codec seat alike.
A file minted before the driver keys existed reads with no EOS guard, no fade and no prompt trim -
what its own oracle ran.

### The chunker is the package's, at its released version {#pocket-chunker}

`pocket_chunks` reproduces the reference build's sentence splitter at its 4.0.1 release on the tokenizer's
ids: the whole text prepared (`prepare_pocket_text`: the file's per-character replacements
(`pocket.replace_from` / `pocket.replace_to`, the config's character table - a config's
curly apostrophes to straight ones, the small models' dashes to commas, a non-English config's
quotes and brackets deleted) with a whitespace fold and the weak mark a deleted quote leaves
after a sentence mark dropped, line breaks to spaces, a capital first letter where
`pocket.capitalize_first_letter` says so, the terminal-punctuation rule - a prompt ending in a
sentence mark, closing quotes or brackets after it allowed, is left alone, one ending in a weak
mark (`, ; : -` and the dashes) has it replaced by a period before the closers, anything else
takes a period after them - and the short-input padding), split after every run of the
sentence-mark ids `.`, `!`, `...`, `?` except a decimal point (the two ids before the boundary
decode to a digit and the period, the period decoded with the id after it keeps a digit right
behind it - "3. 14 people" is a sentence end), an over-long sentence re-split after its `,`,
`;`, `:` ids, the pieces decoded and regrouped under the 50-token budget. A file minted before
the converter wrote the replacement keys reads these rules with its table empty, and its
tokens fixture is minted `--no-replace` to say so. The
reference splits abbreviations ("Dr." ends a sentence) and joins segments with spaces
("p.m." reads back "p. m."); a chunker of our own is a quality rung for the rig, not a
parity matter. The English facade path normalizes the text first (numbers, units, abbreviations
become words) - the reference does not, which is where its numeric WER comes from; a model in
another language takes the text as it is, which is what its reference does in every language
(the facade's Russian normalizer serves the Kokoro voices, not a Pocket file).

### A Pocket file serves from its prepared image {#pocket-image}

`stage_pocket` reads the GGUF into `PocketStaging` on the lane it is asked, every load-time form
minted there - the kernels' layouts and repacks, the head's baked
timestep constant, the roster's frames - and `weights_walk` moves each weight into the staging
planes with its `TtsSpan` recorded: floats into `blob`, Q8_0 quants, K-quant plane pairs and an F16
table's halfwords into `qblob`. `load_pocket` maps the sibling `.dlim` under the lane's tag or bakes it from that staging,
through the engine's one carrier loader, as the StyleTTS2 carrier does
(`ARCHITECTURE_TTS.md#tts-image-rail`): `DASLLAMA_IMAGE=0` mints in memory, `mint_pocket` is the
suites' off-rail control, `bake_pocket_image` the converter's arm. A staged read is q8 and native
both on (the unpinned lane), q8 alone, or neither: native without q8 is no lane and is refused. Three lanes, each its own image:
`pocket-native`, the unpinned default - a file's own quants, K-quant planes included, repacked
for the box's backend, so the tag is config-bound; `pocket-q8`, the q8 pin, config-bound the same
way; `pocket-f32`, the reference lane, under a config-free tag. The meta carries the scalars, the
driver rules, the spans, the roster's names and the tokenizer's vocabulary (`serialize_spm`), so an
image serves without its GGUF, and a `.dlim` path maps the lane it was baked on whatever the pin
asks. `pocket_serves_file` reads a file's family from its head - a GGUF's architecture, an image's
lane tag - and the facade routes a path by it. A host that holds an image's bytes hands them over
instead (`load_pocket(base, bytes)`, the facade's `load_tts_model(base, bytes)`;
`ARCHITECTURE_IMAGE.md#image-borrowed-memory`): the family needs no pack, so the bytes are the
whole model, where a phoneme family's image reads its packs from its directory and opens from a
path. A roster voice is a borrowed view over the mapped
plane, in the same table as the voices a run registers, which the model owns: both free through
`release_plane`, and the walk binds a roster voice in the one table its span names, since the
latents' key is what says a voice is stored as frames.

### The published file carries the served quants {#pocket-q8-file}

A file has three lanes and its formats decide which it can take. A K-quant dense layer
(`convert_pocket.py --kq`: the backbone's and the codec transformers' matrices as Q4_K, the flow
head as Q8_0, the rest as the q8 form writes it) serves as its own planes unless a lane is
pinned - `TtsLinear` holds the plane pair the GGUF transcoder wrote, repacked where the backend
the load selected carries kq kernels (the load line says which arm), and the frame loop's GEMV
and the prompt's GEMM take the engine's own K-quant entries (`linear_rows_decode`,
`linear_rows_kq`), the rows requantized to the Q8_K form the way the engine's own decode does.
The text embedding table is a lookup, so a table stored Q4_K, F16 or low-rank serves in that form: a
Q4_K table as its own planes, a token's row read off them at the prompt (`pocket_prompt_embed`), an
F16 table as its halfwords in the int8 plane, a row widened at the prompt, a low-rank one as its
rows and its projection. An F32 table is floats; a table stored any other way is refused by name
on the unpinned lane, and a table of fewer rows than the tokenizer's vocabulary at every load. Pinned q8 or f32, a K-quant tensor - the table among them - dequantizes into that lane
and an F16 table widens to floats, so one file serves every
lane and the rig compares them on the same sentences. A vector layer the file stores Q8_0 (the
head) runs its GEMV on the q8 lane. The formats follow the widths: the 18m model (512 wide)
takes both the q8 and the kq form, the 7m (224 wide, off the 256 lattice) the q8 form alone,
and the 2m (112 wide, off the 32 lattice) is published f16: its backbone serves f32 planes on
every lane, and its codec transformers' matrices, 128 and 512 wide, take the q8 lane's Q8_0 at
the load like any f16 linear on the lattice; the `z_proj` bottleneck, like the EOS logit, stays f16.

The two lanes every Pocket file has, as the StyleTTS2 families have them: f32, the parity
rail's reference, and q8, the served default - the transformer layers' four matrices and the
frame input projection as Q8_0 rows (`linear_prepare`), the decode step on the q8 GEMV entry.
The codec's convs serve f32 on every lane: the q8 rows conv quantizes its activations to int8
per 32 input channels, and in the Mimi decoder those activations are the audio, so one loud
channel's block scale crushes the quiet ones into a hiss floor 30 dB over the reference in
every silence - the quietest twentieth of 100 ms windows of `harness/tts_synth.das` renders of
four fixture sentences at `stuart_bell`, per band above 1 kHz, on an Apple M5 Max with the
`arm64-gen` backend and the `arm-i8mm` tune profile, no overrides; `test_pocket_quiet_floor`
reads the same effect as 11 dB on its differenced whole-signal metric and holds the served lane
to the f32 lane's floor. The
published GGUF (`convert_pocket.py --q8`) stores the served linears as Q8_0 in the layout the
kernels read - [nout][nin] with the 32-blocks along nin - so `read_linear` takes the blocks
into the int8 planes and repacks for the backend; every conv and every other tensor stays f16,
and the f32 lane of such a file serves the same weights dequantized. A file of the older form
that stores a codec conv as a Q8_0 slab dequantizes it at load (`read_conv_q8`). The
converter's `q8_linear` is the engine's eligibility rule written a second time;
`test_pocket_q8_file` holds the two files to each other and the rig holds the published file to
the reference. The file's block scales
are f16, the load-time quantizer's f32, and the flow head amplifies that half a thousandth per
block into about one percent of a frame (`test_pocket_q8_file`'s teacher-forced compare of the
two loads) - the same picture as the lane itself against the f32
oracle, and why the rig, not a per-element bar, is the q8 gate.
