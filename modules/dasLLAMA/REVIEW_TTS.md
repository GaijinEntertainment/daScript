# dasLLAMA text-to-speech Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_TTS.md`, `ARCHITECTURE_TTS_FRONT_END.md`, `ARCHITECTURE_TTS_TEXTNORM_RU.md`,
`ARCHITECTURE_TTS_MEMORY.md`, `ARCHITECTURE_POCKET.md`, `ARCHITECTURE_GPU.md`. Planned work:
`followup_general.md`.

A TTS source file is a file the glob `dasllama/dasllama_tts*.das` matches,
`dasllama/dasllama_styletts2.das`, a TTS family file - a `dasllama/dasllama_<family>.das` holding
exactly one voice model's code, never the two-family carrier `dasllama/dasllama_styletts2.das` -
or a text front-end file - a `dasllama/` file holding one stage of the pass that turns text into
phonemes in any language, or the pack shape or codepoint scanning those stages share.

**A synthesis entry point of a family (`styletts2_synthesize`, `pocket_synthesize`) without
`[hot_path]` is a defect.**

**A function of a text front-end file that a family's synthesis entry point reaches, directly or
through its callees, is a defect - phonemize before the entry point.**

**A diff that adds or changes an entry of `RESPELL_WORD` in `dasllama/dasllama_g2p_ru.das` whose
value has a different count of the vowels а е ё и о у ы э ю я from its key is a defect - respell
within the key's vowel count.** The stress mark lands on the same-numbered vowel of the respelled
word, so a changed count moves the stress.

**In a TTS source file, an array or table that a synthesis sizes or fills, or that model load fills
for later syntheses to reuse, is `@scratch` - on its declaration or on the callee parameter it grows
through - or the diff is a defect. This covers module globals and the fields of the activation
scratch struct of a synthesis (`St2Scratch`, `PocketScratch`), never the model struct.** The
annotation is what lets `[hot_path]` hold through every stage the entry point drives.

**A function that exists only for tests, debugging or profiling - a stats, trace or dump reader
no synthesis entry point calls - in a TTS source file, that is not `[cold_path]`, is a defect.**

**A GEMM in `dasllama/dasllama_styletts2.das` or a TTS family file that does not go through
a kernel `dasllama/dasllama_tts_blocks.das` exports is a defect, hand-written dot-product
loops included.**

**A block in `dasllama/dasllama_tts_blocks.das` that gains a rows form (token-major [T][C])
ships its channel-major ([C][T]) form in the same change.** The channel-major form is what the
parity tests and any GPU driver are checked against.

**A rows kernel whose result depends on how its row blocks split across the parallel workers
is a defect.** How a rows kernel stays split-invariant is `ARCHITECTURE_TTS.md#tts-two-layouts`.

**A diff that adds to `dasllama/dasllama_tts_blocks.das` a function that writes floats it computed
rather than copied - an export, a private function an export reaches, or a new exported wrapper
over an existing function - or adds a weight lane (the number format a model stores and multiplies
its weights in: f32, q8) to one, ships in the same change a numeric cell in `tests/test_tts_blocks.das`
(`tests/test_tower_asr_kernels.das` for the resamplers shared with ASR). The cell holds the function,
bit for bit or within a bar it names, against one of: the single-row reference kernel it calls for
each row, a double-precision form of its arithmetic, or expected values the test computes from the
definition of the function without calling into `dasllama/dasllama_tts_blocks.das`.**

**A diff that adds a rows form to `dasllama/dasllama_tts_blocks.das` also ships a cell holding it
against its channel-major form within the dot-envelope bar - each element within a tolerance
times the sum of `|w|*|x|` feeding it - in the same change.**

**A diff that adds a window form to `dasllama/dasllama_tts_blocks.das` - a form that computes one
window of the whole-row result at a time - also ships a cell holding it bit for bit equal to the
whole-row form, in the same change.**

**A function, weight lane or window form a diff adds to `dasllama/dasllama_tts_blocks.das` whose
rows split across workers - in its own `maybe_parallel_for` / `lanes_for_work`, or in a backend
kernel it hands a row block to - also ships a bit-equality cell on both axes that move the split:
the batch lane cap (`set_batch_lane_cap`, the cap `matmul_chunks` hands `lanes_for_work`) and the
jobque worker limit (`set_jobque_worker_limit`).**

**A `read_*` call in `dasllama/dasllama_styletts2.das` that leaves a conv or linear on the
channel-major default while the forward assembly runs it through a rows kernel is a defect -
pass the consumer (`rows`, `rows_only`) so `conv1d_prepare` / `linear_prepare` drop the
layout nobody reads.**

**A caller that pins a TTS weight lane (`set_tts_q8` / `set_styletts2_q8` / `set_pocket_q8`)
for a load pins in the context that runs the load, never in the context that spawned that loading
context.** A `new_thread` context starts every module global at its declared default, so a pin set
in another context never arrives.

**A diff that reorders the float operations, or changes the rounding of any step, of the phase
the CPU harmonic source builds in `dasllama/dasllama_tts_blocks.das` - the cycles, the resamples,
the cumulative sum - is a defect.** One float32 ulp of the accumulated phase is a hundredth of a
radian, so only the operation order of the upstream PyTorch model (the reference) reproduces it.

**A diff that moves or rewrites any step of the phase the CPU harmonic source builds in
`dasllama/dasllama_tts_blocks.das`, without changing its arithmetic, ships in the PR body the PCM
hash of one synthesis per StyleTTS2 family on the CPU chain, before and after, and the two
match.**

**A tensor operator - a conv, a norm, an activation, a resampler, an LSTM, an RNG, or an STFT
step - implemented in a TTS family file is a defect; it goes in
`dasllama/dasllama_tts_blocks.das`.**

**Code in `dasllama/dasllama_tts_blocks.das` or `dasllama/dasllama_styletts2.das` - outside the
family records' reader and writer, `stage_family_data` and `serialize_image_meta` - that branches
on which family is running, or hard-codes a constant or a symbol or token rule only one family
uses, is a defect - the family file passes the constant in as an argument, or the family's data
record on the shared carrier holds it, filled by its reader from the gguf or the image
(`ARCHITECTURE_TTS.md#tts-image-rail`).**

**A diff that adds a field to a family's data record on the shared carrier (`KittenFamily`,
`KokoroFamily`) also serializes that field in `serialize_image_meta` and grows the field-count
`verify` beside it (`dasllama/dasllama_styletts2.das`), in the same change** - an unwritten
field reads back zero from a mapped image.

**A diff that changes what a flag-free run synthesizes from English text on a model it
synthesized before - a run with no flags and no environment overrides, on the CPU chain or on a GPU
TTS seat (a stage's device hook a tower driver fills) that serves by default
(`ARCHITECTURE_GPU.md#gpu-backends`), the English front end included (a moved phoneme of
`test_corpus_phonemes` in `tests/test_tts_g2p.das`) - ships in the PR body the WER and UTMOS of
`harness/tts_rig.py`, before and after, at the rig's voice, on every model the change reaches and
every weight lane that model can take - the unpinned default and each pin - or the PCM hash of one
flag-free synthesis per model the change reaches, of a text whose synthesis runs the changed code,
before and after, the two matching.**

**A diff that changes what a flag-free run synthesizes from text in another language, on a model
that synthesized that language before, lists in the PR body what it moved, each with its reason:
every word of that language's g2p corpus fixture (`tests/_tts_fixtures/g2p_corpus_<lang>.json`)
for which the front end's phonemes change, and every pinned sentence of that language's normalizer
test file (`tests/test_tts_textnorm_<lang>.das`) the diff adds or whose expected string it changes;
and names the `followup_general.md` row under which that language's rig arm is owed.** The rig
measures English alone; for a stage whose file that language lacks, the PR body lists by hand the
inputs the diff moved there.

**A diff that newly serves a model or a language no flag-free run could synthesize before ships,
in the same change, a facade cell on that model - a synthesis whose sample count is the front
end's own phoneme string's and not another reading's - and names in the PR body the
`followup_general.md` row under which its oracle dump and its rig arm are owed.** The rig
measures English alone, and a model that could not speak before has no hash to match.

**A diff that changes what a GPU TTS seat's chain computes, on a seat that is not armed by
default, ships in the PR body the WER and UTMOS of `harness/tts_rig.py` with that seat armed
(`DASLLAMA_GPU=1` on Vulkan), before and after, at the rig's voice, on one model of each family
the seat serves, on its served lane - or the PCM hash of one synthesis per family with that seat
armed, before and after, the two matching.** The rig measures what the seat's own cells cannot -
the speech.

**A text normalization or grapheme-to-phoneme error that a transcript of `harness/tts_rig.py` or a
listening pass exposes lands as a failing-first case in the test file of the front-end file the
fix edits (`tests/test_tts_textnorm.das`, `tests/test_tts_g2p.das`, `tests/test_tts_textnorm_ru.das`,
`tests/test_tts_g2p_ru.das`), in the same change as its fix.**

**Never accept a voice or language a model's `caps()` does not declare, a cloning request when
`caps().cloning` is false, or a speed other than 1.0 when `caps().speed` is false - panic at
the call site instead. A function taking a language code answers in that language, hands the text
back untouched, or panics - never in another language.**

**A diff that adds a windowed stage `dasllama/dasllama_styletts2.das` or a TTS family file
assembles from kernels - one that runs its input a window at a time over a carry, the state one
window hands to the next - ships, in the same change, the cell that runs that stage windowed and
over the whole input in one pass on the f32 lane and holds the two together within float rounding
(a few ulp per element), in the `tests/` file that holds that stage's cells**
(`ARCHITECTURE_TTS_MEMORY.md#tts-source-stream`, `ARCHITECTURE_POCKET.md#pocket-codec-stream`).

**A diff that pads a Pocket codec conv (`dasllama/dasllama_pocket.das`) symmetrically, or trims
the final output by hand, is a defect - carry the causal context as the stream's carry: the rows
its taps reach before a window, zero or edge-replicated ahead of the first
(`ARCHITECTURE_POCKET.md#pocket-codec-stream`).**

**A change to which quant format a published file stores a Pocket tensor in, or to its layout
(`q8_linear` / `kq_tensor` / `head_q8_linear` / `dense_codec_conv` in
`harness/convert_pocket.py`, `read_linear` / `read_conv_q8` and the K-quant branch in
`dasllama/dasllama_pocket.das`), changes the converter side and the reader side together, in the
same diff** - which tensors a format covers is written once in the converter and again in the
reader.

**A diff that changes what a TTS converter under `harness/` writes for a key a published file
already carries re-mints that file and, in the same change, lands its new bytes and sha256 in
`harness/tts_model_card.md` and `performance/model_specs.das` and its new sha256 in every
`examples/dasLLAMA/*/models.json` (repo root) pinning it.** The fetches refuse a file whose
sha256 moved, and the card is what a reader downloads against.

**A diff that makes a TTS converter under `harness/` write a key a published file does not carry
leaves that file at its pinned bytes and names, in its `performance/model_specs.das` recipe, the
converter commit that minted it.** The published voice stays the one its listeners rated.
