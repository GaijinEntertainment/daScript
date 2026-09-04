# dasLLAMA architecture - text to speech

The companion `ARCHITECTURE.md` indexes: the TTS file charters (sec.1.7c) and the mechanisms the
TTS files implement (sec.2.28-2.35). `ARCHITECTURE_COMMON.md` (repo root) is the contract.

## 1. File charters

### 1.7c Text to speech

- **`dasllama_textnorm.das`** - text normalization: numbers, ordinals, years and decades,
  currency, percentages, clock times, dates, fractions, units, abbreviations, URLs and e-mail
  addresses become the words a reader says; prosodic punctuation and apostrophes survive,
  everything else becomes a space. The defects of the reference normalizer are fixed here and
  pinned by the tests. A digit run wider than eighteen digits reads digit by digit: the reader
  accumulates in int64, so past that width there is no number to say, and a reader says a
  number that long one digit at a time anyway. A unit carries both its forms and the literal
  picks: exactly one is singular ("1 cm" is one centimeter), anything else - a fraction of one
  included - is plural. Pure code, no data file.
- **`dasllama_postag.das`** - the tokenizer and part-of-speech tagger. The tokenizer reproduces
  the reference pipeline's English tokenization over normalized text (the exception table for
  contractions and abbreviations, prefix and suffix punctuation, infix hyphens); the tagger is
  a greedy averaged perceptron over the PTB tagset. Both load from `tts_postag.bin`
  (`harness/train_postag.py`: UD English-EWT gold plus normalizer-shaped prose tagged by the
  reference tagger, `harness/mint_postag_silver.py`). Tags matter downstream only through the
  lexicon's part-of-speech entries and the punctuation phonemes. The tagger's word class is
  the trainer's own `normalize`: a hyphen anywhere in a token that does not start with one, so
  "--" and "-well-known" are ordinary words, not `!HYPHEN`.
- **`dasllama_g2p.das`** - grapheme-to-phoneme into the reference front end's inventory, in
  either English dialect: a gold lexicon with part-of-speech keyed entries, a silver lexicon,
  function-word rules that read what follows (the pass runs right to left), the heteronym
  rules of sec.2.34, inflection stemming, capitalization and acronym stress, then a fallback
  chain - the lexicon's own plain reading for each letter run, then CMUdict pre-rendered into
  the same inventory, then a GRU spelling model - so no word is ever dropped. The lexicon leads
  that chain because a glued group sends its WHOLE surface to the fallback as soon as one piece
  is unresolvable, so a run both dialects carry does arrive there, and CMUdict is pruned of
  exactly those words. The `british` flag picks the dialect: the gold and silver tables carry an
  American and a British value per key (a key present for one dialect only reads as unknown to
  the other, as the reference does), the inflection rules switch their suffix vowels and drop
  the American flap, and the two American sources of the fallback chain rewrite their answer
  into the British inventory through a table derived from aligning the two lexicons (the
  non-rhotic rule keyed on whether a vowel follows, the goat, trap, lot and reduced vowels
  onto their British symbols, the length marks) - the lexicon's own reading is already British
  and takes no rewrite, which matters because the rewrite is not the identity on one: it reads
  the DRESS vowel before a linking rhotic as SQUARE, having nothing in the string to tell merry
  from Mary. A vowel the two lexicons give no evidence for before a dropped rhotic keeps that
  rhotic rather than losing it. The bath-trap split reaches only lexicon
  words. Loads `tts_g2p.bin`, pack
  version 2 (`harness/build_g2p_data.py`: the gold tier extended by
  `harness/g2p_local_additions.json`, the US and GB keys merged into one string table per
  tier, the GRU stored as f16, CMUdict pruned of the words both dialects' lexicons carry -
  safe because the fallback reads the lexicon first),
  searched in place as byte-sorted string tables; a version 1 pack is refused by name. The
  200-sentence fixtures under `tests/_tts_fixtures/` (American, minted by
  `harness/mint_tts_g2p_fixture.py` from the G2P fidelity experiment; British, minted by
  `harness/mint_tts_g2p_gb_fixture.py` from the reference's own British front end) are the
  parity rails for all three stages.
- **`dasllama_tts_types.das`** - the TTS floor: `TtsCaps`, `TtsAudio` (f32 PCM + rate), `TtsNoise`
  (the source noise a synthesis consumed - captured from the oracle, or drawn into a reused
  carrier). Family files require this, never each other.
- **`dasllama_tts_blocks.das`** - the StyleTTS2-lineage block home, the TTS twin of
  `dasllama_tower.das`, in the two layouts of sec.2.28: Conv1d (dense, depthwise, forward and
  transposed), the dense layer, LayerNorm over rows and over channels, InstanceNorm and AdaIN,
  AdaLayerNorm, the bidirectional LSTM (gates i,f,g,o, both bias halves pre-summed), LeakyReLU /
  Snake / sigmoid / tanh, nearest and ONNX-half-pixel linear resampling, the duration-to-frame
  expansion, half-to-even rounding, PCG32 with a polar normal, the harmonic-plus-noise sine
  source, multi-head attention, and the STFT pieces (edge pad, magnitude and phase, polar to
  rectangular, reflection pad). A weight is an ONNX-layout array plus the served layout
  `conv1d_prepare` / `linear_prepare` mint for the consumer the reader names (`served_rows`,
  `rows_only`, `vec_only`), the unread one dropped; beside every weight array sits its `TtsSpan` into the
  model's blob, and `weights_walk` is the one walk that moves weights into a staging blob or
  binds them as borrowed views over a served plane (`release_weight` is the one teardown). One
  home: the block home holds the operators, and it names no family type.
- **`dasllama_styletts2.das`** - the StyleTTS2-lineage model both families share: the weight
  map of the converted GGUF (conv geometry rides as `styletts2.conv.<weight>` metadata, so the
  assembly hardcodes the wiring and reads the shapes; the STFT convention - replicate or reflect
  padding, basis-folded or envelope-normalized inverse, the magnitude epsilon, which
  resampler's arithmetic the sine source mirrors - rides as `styletts2.*` metadata too), the
  voices as `voice.<name>` [rows x style] tensors, and the assembly: PL-BERT (one ALBERT layer
  applied twelve times) -> text encoder -> duration encoder -> durations -> alignment ->
  prosody (F0, energy) -> decoder -> iSTFTNet generator, with a stopwatch per stage
  (`TtsTimings`) and `StyleTts2Trace` collecting the stage tensors the parity rail compares.
  The served carrier rides the image rail (sec.2.32) and every synthesis reuses one activation
  carrier (sec.2.31). The generator carries the sec.2.14 hook slot
  (`register_styletts2_generator_gpu`, `styletts2_generator_gpu_stats`): a driver takes the
  rows-form input, the style and the source spectrum as rows and answers with the waveform or
  declines; the SineGen phase chain and the harmonic STFT stay on the CPU in both routes
  (sec.2.33), the trace rail keeps the CPU chain, and engage is read from the counters. No
  driver fills the slot yet. The assembly names no family; a family's quirk lives in its family
  file.
- **`dasllama_kitten.das`** - the KittenTTS family (nano and mini): the reference driver's symbol
  table, re-spacing rule and style-row rule (the chunk's character count), its speed priors and
  voice aliases (`kitten.*` metadata), its 5000-sample tail trim, and the rewrite of the front
  end's inventory into the espeak-style IPA these models consume. A phoneme the symbol table
  does not carry takes its separator space with it, so a dropped symbol never leaves a doubled
  space token where the model was trained on none.
- **`dasllama_kokoro.das`** - the Kokoro family (Kokoro-82M): the reference pipeline's
  vocabulary (`kokoro.symbol_*` metadata - the front end's own inventory, no rewrite), its token
  wrapping and style-row rule (the phoneme string's character count less one). Fifty-four voice
  packs of 510 rows, each named `<language><f|m>_<name>`; a pack's language is read from that
  shape and only from it, because the language letters are ordinary first letters of ordinary
  names, so a name of any other shape has no language rather than the one its first letter spells.
- **`dasllama_tts.das`** - the TTS facade: `load_tts_model` (the shared model plus the family
  picked by `general.architecture`; `tts_g2p.bin` and `tts_postag.bin` read from the GGUF's
  directory; the packs it leaves out are named once in the log), `caps` (the voices the front
  end can drive - a Kokoro pack's name carries its language, and only the languages the family
  declares are listed or accepted; the rest refuse with the language in the message, or, where
  the name carried none, with the fact that the front end cannot phonemize it), `tts_voice_lang`
  (the language a voice reads in - the resolution `synthesize` runs, aliases included, so a
  caller can phonemize in the dialect the voice will actually be spoken in), the front-end pair
  `tts_normalize` (the
  normalization pass alone - the spoken form a synthesis reads, and it consults no pack) and
  `tts_phonemize` (one already normalized sentence in the front end's own inventory, the
  string each chunk carries before a family rewrites it into its own symbols; the language
  form takes a code from `caps` and refuses one the family does not declare), the lane pin
  (`set_tts_q8`, `reset_tts_q8`, `tts_serves_q8`), `synthesize_stream` (text -> normalize -> the reference sentence chunker, 400 codepoints a
  chunk - `length()` on a string is bytes, and an em dash costs three of them - abbreviations
  and decimals never split, a whitespace-free run longer than the cap hard-split at the cap on
  a codepoint boundary; a chunk the split left without a closing mark gets a comma under
  Kitten's driver rule and nothing under Kokoro's, whose pipeline sends the text as it is and
  whose voices render an added mark as an audible breath -> per chunk: phonemize -> the
  family's symbols
  and style row -> PCM, timed, delivered to the caller's block as it lands) and `synthesize`
  (the same, concatenated, timings summed). Every chunk draws its own source noise: the seed
  is the facade's constant plus the chunk's index, so consecutive sentences of one request
  never share a draw, and `synthesize` and `synthesize_stream` stay sample-identical because
  both walk the same chunk list in the same order. Requires no `audio` module. `REVIEW.das`'s
  `check_tutorial_floor` walks both facade files - `dasllama.das` and this one - and licenses
  exactly three kinds of def: a `def private` one, a `def operator` overload, and `finalize`,
  the language's own teardown hook the compiler calls at `delete` (the check's `FLOOR_HOOKS`
  set); none of the three is a name a tutorial could call, so none carries a teaching duty.

Every local container on the TTS path is `var inscope`: the persistent heap frees nothing at
scope exit, and a bare local holding a per-sentence buffer is a per-sentence leak that ends in
the OS killing a long run.

The product surfaces sit outside the module: `utils/dasllama-server/txt2wav.das` (text or a
file -> a WAV, the timings line on stderr) and the server's `/v1/audio/speech` route (a
dedicated TTS worker thread, one synthesis at a time, the audio served from a temp file the
route reaps a minute after the wire closes - dasHV writes string bodies only). The rig that
scores the whole chain is `harness/tts_rig.py` over `harness/tts_synth.das` (sec.2.35).

## 2. Mechanisms

### 2.28 Two layouts, one oracle {#tts-two-layouts}

Every block kernel exists twice. The channel-major [C][T] form is the reference: the straight
transcription of the model's operator, the form the parity rails hold against the PyTorch and
ONNX oracles. The token-major [T][C] "rows" form is what bert, the prosody branches, the decoder
and the generator run: a row is one token's channels, so a dense stride-1 conv is k tap-GEMMs
on the tiled f32 kernel over the shifted input rows with no im2col, the dense layer the same
tiled GEMM off a transposed weight, attention one head per lane on the tiles, Snake and AdaIN
four channels a lane. Served widths sit on the tile: a conv's output width pads to 16
(`cout_s`), so `conv_post`'s 22 columns are served at 32 and read back strided. The F0 and
energy curves and their single-channel projections stay channel-major - one channel is a
column either way. `tests/test_tts_blocks.das` holds every rows form to its channel-major twin
at the dot-envelope bar (a tolerance times the sum of |w|*|x| feeding each output).

A rows kernel's result never depends on how its rows split across the parallel workers: block
edges snap to the 4-row tile (`tile_edge`), a conv tap whose shifted range starts inside a
block peels the rows up to the next tile edge through the scalar path before its GEMM, so
every GEMM base sits on the tile and a row is tiled or scalar-remainder the same way in every
split, and a reduction accumulates per fixed 256-row block (`STAT_BLOCK`) and merges the blocks
in a fixed order. `tests/test_tts_blocks.das` runs each rows kernel at one lane and at the
box's lanes on both axes that move the split - the batch lane cap, which clamps inside
`lanes_for_work`, and the jobque worker limit, which moves `get_dispatch_lanes()` itself and so
reaches the shapers that pass no cap (the AdaIN column stats, every bare `lanes_for_work(work,
0)` site) - and asserts bit equality, each axis carrying its own witness that it moved the
split, since a leg pair whose shaper answers the same lane count compares nothing; the facade's
streaming cell does the same for a whole synthesis. Per-dispatch-slot partials and an off-tile
GEMM base each broke it once.

### 2.29 Tap stacking: one GEMM per chunk, nothing accumulates across taps {#tts-tap-stacking}

On the q8 lane a conv's weight bakes as one [cout][k*cin] Q8_0 matrix, tap-major within a
row, repacked for the box's backend. The conv quantizes its input rows once, then per chunk of
output rows copies the k shifted quantized rows side by side - zero scales where a shift leaves
the input - and makes one Q8*Q8 batch-GEMM call with K = k*cin, the bias added row-locally
while the chunk is still in cache. The strided f32 convs (the generator's noise branch, k =
2*stride) use the same shape over float rows and the tiled f32 kernel reading the served
weight as [k*cin][cout_s]. The chunk stays inside a 4 MB budget, so the stacked rows live in
cache while the kernel reads them.

The per-tap alternative - one short-K GEMM per tap that stores a fresh output, then a pass that
adds it into the accumulator - paid three sweeps of the output plane per tap and, on a bias128
stamp, a fresh token block-sum pass per call. Stacking removes both without touching the kernel
backends: the TTS convs are consumers of the LLM's Q8*Q8 batch entry, never a variant of it.

### 2.30 The decoder's concat rows pad to the q8 lane's 32 {#tts-padded-input-width}

A decoder block fed by the concat of the previous stream, the F0 and energy columns and the
asr residual reads 514 or 1090 channels - off the 32 the q8 lane quantizes per. The block
serves a padded input width instead: at bake, conv1, the 1x1 shortcut and the depthwise pool
zero-pad their input channels before their served layouts mint (the reader learns the width
from the tensor dims first, since the mint drops the ONNX weight), the AdaIN affine re-lays its
two halves at the padded width with a zero row for each pad channel, and the concat writes the
zero columns. The zero rests on the producer: an `affine=False` instance norm gives a pad
channel the scale 1/sqrt(eps), so the column must arrive exactly zero - then it stays zero
through the norm, the LeakyReLU and the conv, the parity bars do not move, and the decoder's
four largest convs serve q8.
The rows AdaIN kernels also take a width off the four-lane one channel at a time, which is
what a block without the padding runs on.

### 2.31 One carrier per synthesis {#tts-scratch-carrier}

`styletts2_synthesize` carries `[hot_path]`: nothing on the synthesis path allocates. Every
stage activation is a `@scratch @exact_size` field of the `St2Scratch` carrier the facade's
`TtsModel` reuses across syntheses (the waveform is `sc.wave`; ping-pong fields replace
delete-and-move handoffs), the block home's kernel-private transients are `@scratch` module
globals, and every block-home out-parameter is `@scratch`. Sizing is the builtin
`scratch_resize` at the site - a helper wrapping it would hide the contract from the lint - and
a `@scratch` mark on a local is inert: the mark says "this buffer persists between syntheses
and grows to its working size once", which only a field or a global can promise. The source
noise lives in the carrier too, and `TtsNoise.captured` decides whether a synthesis draws it:
a reused carrier is never empty after the first chunk, so emptiness cannot.

### 2.32 The served carrier rides the image rail {#tts-image-rail}

`stage_styletts2` reads the GGUF into `St2Staging` with the served layouts minted - one form per
weight, the consumer named by the reader - and every weight moved into one staging blob with its
`TtsSpan` recorded; `load_styletts2` maps the sibling `.dlim` under the lane's tag or bakes it
from that staging (`DASLLAMA_IMAGE=0` mints in memory; `mint_styletts2` is the suites' off-rail
control). Two lanes, each its own image: `tts-q8`, the served default - the rows GEMMs'
weights as Q8_0 quants in an int8 plane repacked for the box's backend, the tag config-bound -
and `tts-f32`, the file's planes under a config-free tag, the reference lane the parity rail
and the block test hold against, which carries no optimization duty: the rig held q8 at f32
quality, and f32 costs memory for nothing. The meta blob carries the scalars, the spans and the
voice roster through the leaf structs' own `serialize` overloads; the loader binds every weight
array as a borrowed view over the mapped plane after the parse (post-load runs before the
planes bind), so the carrier and every struct holding it take an explicit `finalize`. The
layout fingerprint refuses a struct-shape change by name; a served-layout change that keeps the
shape is invisible to it, which is why such a change bumps `IMAGE_VERSION` and why a rows conv
served from an image older than its layout panics by name rather than indexing an empty array.

### 2.33 The sine source keeps the reference's operation order {#tts-phase-law}

The harmonic source's phase reaches 1e5 radians in float32, where one ulp is a hundredth of a
radian, and the reference's sine is accurate at that argument. Only the reference's own
operation order - the cumulative sum, the resampler's arithmetic, the multiply by the harmonic
index - reproduces its phase, so `sine_source` and `source_resize` keep it exactly, the scalar
sine stays on libm, and the phase chain stays on the CPU in the GPU route.

### 2.34 What a word sees around it {#tts-heteronym-context}

The lexicon's part-of-speech keyed entries settle most heteronyms, and the pass runs right to
left so a word sees the vowel and the "to" that follow it. The tag alone cannot settle the
rest, so the token context also carries the word and tag on either side, and the entry keys
are tried in an order those neighbours set: the infinitive after "to" reads the verb key; after
a determiner a verb tag is a mis-tag (the noun, then the default) and an adjective before a
non-noun is the noun. A small collocation table settles the readings no tag carries ("lead
pipe", "get the lead out", "bass player", "minute detail"), and keyed entries the lexicon holds
as one reading join it ("the does", "my resume"). Every collocation names a noun reading, so a
verb tag out-ranks the table: "the pipes lead out" is the verb whatever follows it. The corpus
cell names the sentences these
rules and the lexicon additions read past the reference front end and keeps a budget of tag
calls for the rest.

### 2.35 The rig scores against what a person says {#tts-rig-scoring}

`harness/tts_rig.py` runs the 200-sentence fixture through one model and voice, transcribes
with parakeet, scores UTMOS in the metrics venv, and prints one table with a per-category line.
WER counts against the fixture's expected spoken form: the upstream normalizer's reading
(`norm`, what the phoneme and tagger cells compare on), or the hand-corrected `expected` where
that reading is not what a person says - units as their symbols, a fraction read digit by digit,
a duration glued to the next word, a decade misspelled, pence called cents. The corpus loader
exposes `expected` so the normalizer cell and the rig read the one correction; the minter
(`harness/mint_tts_g2p_fixture.py`) carries the table so a re-mint reproduces it. The reference
line is the reference arms' own WAVs re-scored from the experiment's transcripts on the same
forms with the same scorer, the clock suffix ("a m", "am") counted as one word on both sides.
