# dasLLAMA architecture - text to speech, the memory a synthesis holds

The companion `ARCHITECTURE.md` indexes: the mechanisms that decide what a StyleTTS2-lineage
synthesis (Kitten, Kokoro) allocates, holds and gives back (`ARCHITECTURE_TTS_MEMORY.md#tts-scratch-carrier`, 2.51-2.53).
`ARCHITECTURE_COMMON.md` (repo root) is the contract. The block home, the facade and the phoneme
families are `ARCHITECTURE_TTS.md`; the Pocket family's own memory law is `ARCHITECTURE_POCKET.md#pocket-codec-stream`.

## Mechanisms

### One carrier per synthesis {#tts-scratch-carrier}

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

### The generator runs on six buffers {#tts-generator-buffers}

The iSTFTNet generator is where a synthesis holds its memory. Its activations are token-major
`[t][c]` rows at the stage's width, every one the size of the stage's whole stream (the last
stage sits at a fraction of the sample count times its channel count), and a chunk's stream is
long: the carrier's generator fields are the bulk of what a say leaves behind, and they grow
with the chunk. So the generator's footprint is the count of buffers live at once, and that
count is six, each carrier field reused in a second role once its first holder is dead:

| field | in `generator_stage` | in `gen_res_block` |
|---|---|---|
| `gen_a`, `gen_b` | the stage stream's ping-pong - a stage reads one as `x` and writes the other as `out`; the first stage reads the caller's rows | - |
| `st_y` | first the noise branch's conv output (`src`); once the branch's residual block has consumed it, the upsampling conv's output `y`, the stage's running stream, which the residual sum and every Snake block read | - |
| `st_res` | the noise branch's residual output (`src_res`) until it is added into `y`; then each Snake block's output, copied or added into `out` | the block's `y` |
| `grb_t0` | - | each conv pair's AdaIN + Snake input (`xt`), dead once the pair's first conv has read it; then the pair's second conv output (`y2`), summed into `y` |
| `grb_t1` | - | the pair's first conv output (`y1`), live from that conv through the second |

The pairing rule is liveness, checked kernel by kernel: at every call the rows a kernel reads
and the rows it writes are distinct fields, because a rows conv, an AdaIN-into and a sum write
their whole output while still reading their input. The three-operand sites are the ones that
decide the count - the first pair's `sum_rows_into(y, src, y2)` reads the block input and the
second conv output while writing the block output, so `src`, `grb_t0` and the block's `y` are
three fields; the noise branch's `gen_res_block(src -> src_res)` reads `st_y` while writing
`st_res`, so those are two; and the stage's own Snake blocks read `y` while writing `st_res`,
which is why `st_y` cannot double as the block output. Everything else was a buffer whose
value was dead by the time the next one was written, and those are gone. Reuse moves values
between addresses and nothing else: no operation, operand or order changes, so the six-buffer
form is bit-identical to a form with a buffer per role - the parity rail holds it against the
oracle and the facade's streaming cell against itself. A buffer added to the generator is
justified against this table: a new role names the field whose holder is dead at that point,
or says which three-operand site forces a seventh.

### The peak is the chunk's, and idle memory is given back {#tts-idle-release}

A synthesis runs one chunk at a time (`tts_chunks`), so what it holds is the largest chunk's
working set, linear in the chunk's length: the carrier's rows fields at the sample count the
chunk speaks (the source's tables excepted, `ARCHITECTURE_TTS_MEMORY.md#tts-source-stream`), the block home's scratch globals at the
largest rows GEMM the chunk ran. The chunker's cap is the ceiling -
`TTS_CHUNK_CHARS` codepoints, a sentence of about twenty-five seconds - and a platform where
memory is the scarce resource lowers it per model with `tts_set_chunk_chars`: the same text
speaks in more, shorter pieces, each its own synthesis with its own prosody (a phrase split
across two chunks is read as two phrases), and the peak scales with the cap. Pocket takes no
cap: its peak is its window's (`ARCHITECTURE_POCKET.md#pocket-codec-stream`), not its chunk's. What a say leaves behind stays
sized to the largest chunk it saw, so the next chunk allocates nothing (`ARCHITECTURE_TTS_MEMORY.md#tts-scratch-carrier`);
`tts_release_scratch` gives it all back - the carrier, the Pocket carrier and the block home's
globals - for a caller that says idle memory matters more than the allocation the next run then
pays. The carrier's source noise goes with it: a synthesis draws its noise from the seed and the
chunk index, so nothing is lost, except on the parity rail, whose captured noise the release
keeps by leaving that carrier alone.

### The harmonic source streams in frame windows {#tts-source-stream}

The source's tables sit at samples times harmonics - the cycles per sample, the phase
upsampled back from frame rate, the noise draw, the sine rows - four tables the size of nine
waveforms to produce one, and every one ran the whole chunk before `linear_rows` mixed the
harmonics down. Nothing in that chain reaches across the chunk but the cumulative sum, and the
sum runs at frame rate: one accumulator per harmonic (`SineSourceCarry`, the double one for
PyTorch's arithmetic beside the float one for onnxruntime's). So `styletts2_source` runs the
source `SOURCE_WINDOW_FRAMES` phase frames at a time - 64, which is 19200 samples at Kitten's
and Kokoro's 300x upsample (`set_styletts2_source_window`, 0 for the whole run) - and
`sine_source` takes a frame window: the cycles for the samples the downsample's
taps read (`resize_span`, the same tap arithmetic the resample itself runs), the low-rate
frames the window adds, the sum carried on from the state, then the phase for the window's
samples from the carried frames - the frame before the window and the one after it stay in the
carry, because the upsample's taps reach one frame each way - and the sine rows for
`linear_rows` into the window's slice of the mixed signal. The resamplers are one
implementation each, the window form; the whole-row `resize_linear` runs it over every column,
which is the form the kernel tests hold to the reference arithmetic, and the block test holds the
window form at offsets and strides off the whole row bit-equal to it. The window knob is a
`TtsStreamWindow`, the record Pocket's codec window is too (`ARCHITECTURE_POCKET.md#pocket-codec-stream`):
a set is announced, a negative count refused. The noise stream is the reference's one stream: `styletts2_draw_noise` opens
it for the initial phases, and the source reopens it from the same seed, skips those draws, and
takes the normals in row order a window at a time; a captured stream (the parity rail's) is
read by window offset instead. The window changes where a value is computed and nothing else -
no operand, order or rounding - so every window speaks the same samples as the whole run, which
is what the facade test's window-zero control holds.
