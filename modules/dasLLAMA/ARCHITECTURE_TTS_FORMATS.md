# dasLLAMA architecture - text to speech, the formats a model's weights take

The companion `ARCHITECTURE.md` indexes: the format each weight group of a StyleTTS2-lineage model (Kitten, Kokoro)
serves in (`ARCHITECTURE_TTS_FORMATS.md#tts-recipe`) and the file that stores a lane's weights as minted
(`ARCHITECTURE_TTS_FORMATS.md#tts-stored-file`). `ARCHITECTURE_COMMON.md` (repo root) is the contract. The block
home, the lanes' images and the facade are `ARCHITECTURE_TTS.md`; a Pocket file's own formats are
`ARCHITECTURE_POCKET.md#pocket-q8-file`.

## Mechanisms

### The recipe is what the rig picked, group by group {#tts-recipe}

`st2_recipe_default` names a format for each weight group (`ST2_GROUPS`, a tensor's group read off
its name by `st2_group`), and each entry is a measurement: `set_styletts2_recipe` asks the lane and
pins groups in process (`harness/tts_synth.das --small` / `--recipe`, the rig's flags of the same
names), a load under group pins mints in memory and writes no image, and the stage logs what each
group asked and how many of its weights minted in it. The decoder stack, the generator's res blocks
and the text encoder's convs take Q4_K, the transposed upsamplers Q6_K, the noise convs and
`conv_post` f32 - the first reads the harmonic source and the second writes the spectrum, so a
block scale over their activations is a noise floor under the audio, as it is in the Pocket codec
(`ARCHITECTURE_POCKET.md#pocket-q8-file`) - and every other group Q8_0 wherever its widths quantize
per 32: the voice packs, a style row read back off its own blocks (`styletts2_style`), and the
LSTMs, a step's input row and state requantized ahead of a q8 GEMV. The lane is the small form, not
the default: it is a quarter smaller than the q8 lane on Kokoro and a tenth to a seventh slower on
the CPU chain of an Apple M5 Max. The scores each pick rests on, the sizes and the speeds are `PERF_LEDGER.md`'s
"TTS weight formats" entry.

### A file can store a lane's weights as minted {#tts-stored-file}

`write_styletts2_gguf` (`dasllama-convert --gguf`) writes a file of floats as the file that stores a lane's
weights: the q8 lane's, or the recipe lane's. It is an ordinary GGUF with the source's keys and tensor names. Each
weight the lane mints is stored as the quant matrix the kernel reads, in ordinary blocks: a dense conv's tap-stacked
matrix as Q8_0 or as Q4_K, Q5_K or Q6_K with its row zero-padded to the format's 256
(`ARCHITECTURE_TTS.md#tts-tap-stacking`), a dense layer's and a voice pack's as Q8_0, an LSTM's two matrices as Q8_0
at the source's dims with each direction's rows where the source keeps them, and a style projection as Q8_0 at the
padded width its block reads (`ARCHITECTURE_TTS.md#tts-padded-input-width`). Every other tensor is the source's, byte
for byte. Three keys are added: `styletts2.stored_layout` (the layout's number, `ST2_STORED_LAYOUT`),
`styletts2.stored_lane` (`q8` or `kq`), and for each quantized conv `styletts2.stored_conv.<tensor>` - its output
channels, its input channels as stored and the source's input channels - because a matrix padded to 256 no longer
says how many channels it holds.

The file decides the lane. `styletts2_stored_lane` reads it before the load, `stage_styletts2` takes it whatever
lane it was asked for, and a q8 pin or a recipe ask is announced as not applied. A quantized tensor is taken as its
planes - the disk block split, then the backend's repack (`q8_matrix_read`, `conv1d_read_quant`) - with no float
between the file and the kernel, and a tensor of floats is served as floats: the load mints nothing. The planes of
a stored file are therefore the lane's own: the K-quant planes and the Q8_0 quants byte for byte, each Q8_0 scale
the half its block holds where the in-memory lane keeps the float. That half moves the waveform and not the speech
- the harmonic source's phase (`ARCHITECTURE_TTS.md#tts-phase-law`) turns a rounding in F0 into a shift of every
later sample - so a stored file is scored by the rig on the file itself, never held to the lane's samples. The
writer captures the planes at the sites that mint them, in disk order, so the file cannot store a matrix the
reader's sites do not take. A layout number this build does not read, a quantized tensor in a file that declares no
layout, a type a site does not take, and the writer over a file that already stores its weights are each refused by
name. The image a stored file mints is its lane's (`tts-q8`, `tts-kq`).
