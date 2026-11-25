# How to add a weight format to a TTS family

This document walks the code in the order a weight format lands on a text-to-speech weight
group - the mint, the kernel that multiplies the stored bytes, the GPU seats, the measurement
that decides whether the format ships - and cites the architecture section that owns each step.
The formats the TTS families serve are Q8_0 and the K-quant members Q4_K, Q5_K and Q6_K; the
worked examples are the StyleTTS2 carriers' recipe lane (Kitten, Kokoro: an f32 GGUF whose served
quants mint at the image bake) and Pocket (a file that stores its own quants).

**A format is added to a weight group when the group mints in it at the image bake, the CPU chain
multiplies the stored bytes with no f32 copy of the weight, every GPU seat reads it, and the rig
scores the lane at the reference lane's quality on every language the family speaks.**

A format the engine's kernels do not take yet - a `KqFmt` member with no row core on the CPU
backends - is `HOW_TO_ADD_A_FORMAT.md`'s work first; this document starts where the kernel entry
takes the format as an argument.

## 0. Decide the group's shape before touching code

Answer these from the model's assembly and the group's tensors; the answers are the first lines
of the PR body. The recipe and what each group's pick rests on are
`ARCHITECTURE_TTS_FORMATS.md#tts-recipe`.

1. **The group and the kernel form that reads it.** A dense forward rows conv, a transposed rows
   conv, a channel-major conv, a rows linear, a vector linear (one GEMV a call), an LSTM
   direction, or a lookup table. Each form has one quant layout (step 2), and a group whose form
   has none yet needs the form before the format.
2. **Whether the group reads or writes the signal.** A conv whose input or output is the audio
   itself - the generator's noise convs, `conv_post`, the Pocket codec's convs - stays unquantized:
   f32, or the halves a file stores it in
   (`ARCHITECTURE_TTS_FORMATS.md#tts-recipe`, `ARCHITECTURE_POCKET.md#pocket-q8-file`). Decide this
   before the measurement, and let the measurement confirm it.
3. **The lattice.** A Q8_0 matrix needs its reduction width on 32 (a vector linear and an LSTM
   need both widths there); every dense conv takes a K-quant matrix
   (`ARCHITECTURE_TTS.md#tts-tap-stacking`). A width off the lattice keeps the group's f32 form,
   and the mint reports how many of the group's weights minted as asked.
4. **Where the bytes come from.** A StyleTTS2 carrier mints from the f32 GGUF, so its published
   file does not change. A Pocket file stores the format: the converter and the reader change
   together (step 7).

## 1. Give the leaf its planes

Files: `dasllama/dasllama_tts_blocks.das` (the leaf structs), and the family carrier where a
group's weights sit outside a leaf (a voice table).

A weight leaf - `TtsConv1d`, `TtsLinear`, `TtsLstm` - holds each served form as an array with a
`TtsSpan` beside it. A new form adds its arrays and spans, the scalars that describe it (the
format id, whether the planes are in the backend's layout, the padded row width), a line for each
in the leaf's `serialize`, the field count beside it, `finalize` and `weights_walk`
(`ARCHITECTURE_TTS.md#tts-image-rail`). Quants and K-quant plane pairs ride the int8 plane, floats
the float plane; `weight_slot` routes by the array's element type. A tensor a file stores F16 and
no quant form covers - a codec conv's rows (`wkh`, `wtrh`), a vector linear's (`wh`) - rides the
int8 plane as its halves, two bytes a value, and the leaf drops the floats: `rows_narrow` at the
leaf's prepare, on the unpinned lane, where `active_rows_f32` is false
(`ARCHITECTURE_CPU_KERNELS_TILES.md#half-rows-by-target`).

- A field with no `serialize` line trips the leaf's field-count `verify` at the first mint.
- A named table of weights walks through `table_slot`, which mints no key the table does not
  carry: a walk that indexes the table directly inserts an empty entry, and a reader that takes
  the key for the weight's form reads nothing.
- A mint that moves bytes while every struct keeps its shape leaves an image at the same path
  valid and stale, so the cells compare a fresh staging against the old image: delete the lane's
  images under the models directory while iterating, and bump `IMAGE_VERSION` where the change
  ships with no shape change beside it.

## 2. Mint the quant matrix and read it

Files: `dasllama/dasllama_tts_blocks.das`, `dasllama/dasllama_convert.das`,
`tests/test_tts_blocks.das`, `tests/test_gguf_quant.das`.

A group's quant matrix has one layout for Q8_0 and for every K-quant member
(`ARCHITECTURE_TTS.md#tts-tap-stacking`): a forward conv's is [cout][k*cin], a row an output
channel's taps stacked along K; a transposed conv's is its lift [k*cout][cin]; a linear's and an
LSTM's are [nout][nin]. The mint builds the rows as floats, encodes them - `quantize_q8_0_into`,
or `quantize_kq_plane` with the format - repacks for the box's backend where the load asks, and
drops the f32 weight. The kernel requantizes its input rows to the format's activation form (Q8_0
per 32, Q8_K for a K-quant matrix) and calls the engine's batched GEMM entry with the format as
its argument; a one-row call takes the GEMV entry.

- Size every loop and every encode call by the matrix's row count, never by `cout`: a transposed
  conv has k times as many rows, and an encode over `cout` rows leaves the rest of the planes
  zero - audio that is silence under a 100% word error rate.
- Hold the planes to the source weights as well as the kernel to the planes: a cell whose
  reference is built from the minted planes passes a mint that leaves rows unwritten, so the
  K-quant cells also run the f32 conv over the dequantized planes against the f32 conv over the
  source at the format's weight bar.
- Hold the row a quant matrix reads back through `conv1d_weight_row` to its dequantized planes
  bit for bit, on both plane layouts: that read is the GPU slab's, and no CPU kernel runs it.
- The cells a new lane owes are `REVIEW_TTS.md`'s: the numeric cell against a reference, the rows
  form against its channel-major twin at the dot-envelope bar, and bit equality at one lane and at
  many on both split axes (`ARCHITECTURE_TTS.md#tts-two-layouts`).

## 3. Name the group in the recipe

Files: `dasllama/dasllama_styletts2.das`, `tests/test_model_image.das`.

`st2_group` reads a tensor's group off its name, `ST2_GROUPS` lists the groups a pin may name, and
`stage_fmt` answers the format a staged tensor mints in: the recipe's on the recipe lane, the
recipe with every K-quant group at Q8_0 on the q8 lane, the pin's where
`set_styletts2_recipe` names the group (`ARCHITECTURE_TTS_FORMATS.md#tts-recipe`). The group's reader asks
`stage_fmt`, mints the form where the leaf is eligible, and calls `stage_note` so the stage's
report counts it.

- Keep the group's entry in `st2_recipe_default` at the format it serves today until step 5 has a
  number: the recipe is what every recipe-lane load mints and the converter's `--recipe` bakes.
- A channel-major conv takes a quant matrix only where its group is admitted by name in
  `read_conv`: the other channel-major convs read a pitch or energy curve or the signal.

## 4. Let the GPU seats read the form

Files: `dasllama/dasllama_tts_slab.das` (the writer), `dasllama/dasllama_tts_blocks.das`
(`conv1d_weight_row`, `q8_matrix_row`), `dasllama/dasllama_tower_tts.das` (the driver every GPU home seats:
`ts_gather_member`, and `ts_gather_member_metal` for the Metal home's own layout),
`dasllama/dasllama_gpu_kernels_common.das` (the stored members), `dasllama/dasllama_metal_sites.das` (the Metal
home's tile ladders).

A GPU seat reads a leaf in the form the carrier holds it. The host writer (`tts_slab_build_stored`)
lays a part's f32 rows into one slab - norms, biases, a leaf held as floats - and records a stored
plane (`TtsStoredPlane`) for each leaf whose form is in the caller's `stored_mask`; the flavor's bake
then gathers that plane into the device layout its site reads and a kernel member widens it as the CPU
reader does (`ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats`; the Metal home's own layout,
`ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`).
A new form adds, on every home that serves the group:

1. its bit in the driver's stored mask (`TS_STORED_MASK`), its member (`TS_ST_*`, a `ts_st_of` overload on its tag) and its gather
   into the shared device layout (`ts_gather_member`);
2. its member of the kernels that read the group - the exact tile (`F32GemmT` under `WSTORED`, a stamp named
   `kq_f32_gemm_<fmt>_cls` on every home, which `ts_mm_stored`'s generated arms reach), the
   decoder convs' cm2 tile on the homes that have one, the LSTM recurrence, the Pocket row GEMV
   (its `WFMT` arm in `TtsPkGemvStoredT`, a stamp of every kind on every home, the format out of
   `tts_pkst_enc`'s skip words and the stamps in `tts_pkst_ensure`) - each with a kernel cell against the rows the CPU reader widens the same
   plane to;
2a. on the Metal home, where a site its own tile serves reads the form: the form's stamps under the names the
   tile ladders generate (`enc_kq_st2_<fmt>_*`; `mtl_stored_mm`, `mtl_kq_conv`: the format out of
   `MTL_ST2_SKIP`), its own-layout gather (`ts_gather_member_metal`) and, for the
   Pocket frame loop, its bit in `TS_OWN_GEMV` with the home's row dot (`MtPkGemvStoredT`, an `OWN` arm, its
   stamps in `mtl_pk_own_ensure` and the format out of `MTL_PKOWN_SKIP`, the skip words `rail_tts_pk_own`'s arms generate from); a change to
   the own layout's bytes moves `TS_BLOCK_METAL`. A form with no arm there stays in the shared layout
   under the shared stamps;
3. its arm in `conv1d_weight_row` - the read-back into f32 slab rows for a leaf off a member's
   lattice - or, for a leaf the writer reads directly, in the writer; and, for a form that is
   neither Q8_0, a K-quant member nor halves, its term in the slot's widened mark
   (`TtsConvSlot.widened`, set in `tts_slab_conv` and `tts_slab_linear_rows`);
4. the form's term in the part's `tts_*_ok` check, and - where the form changes a device-block piece's
   bytes - the block's layout version (`TS_BLOCK_PLANES`).

- A form staged as f32 rows where a member could read it holds a second copy of the weights on the
  device and serves weights the file does not store: stage it stored. The home counts every dense
  dispatch over such rows (`tts_home_stats(dev).widened`), and the stored-format cells hold that
  count exact, so a leaf widened by mistake reds them.
- A form the check refuses sends the whole carrier to the CPU chain with no error, at several
  times the seat's real-time factor (`PERF_LEDGER.md`'s "TTS weight formats" entry): read the
  factor after every change to a leaf, and give the lane a form in `tests/_tts_parity.das` - its name
  in `styletts2_lane_forms`, its in-memory mints in `styletts2_form` (the lane's CPU image and its `@dev` form), its
  bar in `parity_case`, the decode seat's legs (`tts_parity_legs`): the one gate that compares the seat's read of the
  form against the CPU chain.

## 5. Measure, then set the default

Files: `harness/tts_rig.py`, `harness/tts_synth.das`, `dasllama/dasllama_styletts2.das`,
`ARCHITECTURE_TTS.md`.

The rig speaks the 200-sentence fixture, transcribes it and scores WER and UTMOS
(`ARCHITECTURE_TTS.md#tts-rig-scoring`); `--lang ru` runs the Russian set over the kokoro-ru
voices, and `--recipe group=format,...` pins groups for one run, which mints in memory and writes
no image. Run the recipe as it is (`--small`), then each candidate format for the group; a run's
folder carries its lane, so the two never share one:

    DASLLAMA_METAL_TOWER=0 python harness/tts_rig.py --root <rig root> --out <dir> \
        --models kitten-nano:Bella kokoro-82m:af_heart --small
    DASLLAMA_METAL_TOWER=0 python harness/tts_rig.py --root <rig root> --out <dir> \
        --models kitten-nano:Bella kokoro-82m:af_heart --recipe "<group>=<format>"
    DASLLAMA_METAL_TOWER=0 python harness/tts_rig.py --root <rig root> --out <dir> \
        --lang ru --recipe "<group>=<format>"

- Run the CPU chain (`DASLLAMA_METAL_TOWER=0` on a Metal box) for the pick: it is the run that
  exercises the kernel, and its real-time factor beside the q8 lane's is the lane's cost in speed.
- Pin every group under test to the q8 lane's format once first: that run reproduces the q8
  lane's score to the digit, and a run that does not names a broken pin.
- A score at 100% word error rate is a mint or kernel defect, never a format's quality.
- UTMOS moves by a few thousandths between equivalent runs of the small model; a group's cost is
  real from about 0.02, and Russian voices show a cost English hides.

The pick is the smallest format that scores with the reference on every voice. Set it in
`st2_recipe_default`, state it in `ARCHITECTURE_TTS_FORMATS.md#tts-recipe`, and record the scores, the
image's size and the CPU chain's factor in `PERF_LEDGER.md`.

## 6. Hold the image

Files: `tests/test_model_image.das`, `tests/run.das`, `utils/dasllama-convert/`.

The family's image arm holds the lane: the cold load, the warm map and the in-memory mint serve
the staged planes whole, each group reads the format the recipe names, and the pin cell moves the
named groups and no other (`ARCHITECTURE_TTS.md#tts-image-rail`). Add the group's assertion to the
recipe cell, and the pin's to the pin cell.

- `dasllama-convert -q q8` bakes a family's default lane and `--recipe` a phoneme family's recipe
  lane; a new lane tag is one line in the converter's TTS arm and a cell in
  `utils/dasllama-convert/test_convert.das`.

## 7. A file that stores the format

Files: `harness/convert_pocket.py`, `dasllama/dasllama_pocket.das`, `tests/test_tts_pocket.das`,
`dasllama/dasllama_styletts2.das`, `dasllama/dasllama_gguf.das`, `dasllama/dasllama_convert.das`,
`tests/test_tts_kitten.das`, `harness/tts_model_card.md`, `performance/model_specs.das`.

For a StyleTTS2 carrier's file of the lane's weights (`dasllama-convert --gguf`,
`ARCHITECTURE_TTS_FORMATS.md#tts-stored-file`): add the format's plane-to-block writer beside its
transcode (`kq_disk_block_p`, read by `kq_planes_to_disk`), admit the format in `stored_quant`,
take it at the group's site (`conv1d_read_quant`, `q8_matrix_read`), and extend
`test_kitten_stored_file` to hold the staged file's planes to the lane's byte for byte.

- A layout change - what a stored tensor's dims or keys mean - takes a new `ST2_STORED_LAYOUT`
  number: a file that carries the old number keeps reading by that number's layout, or is
  refused by the number.

A Pocket file's formats decide the lanes it takes, and which tensors a format covers is written
once in the converter and again in the reader (`ARCHITECTURE_POCKET.md#pocket-q8-file`): change
the two together, and compare the one file against itself across its lanes
(`ARCHITECTURE_POCKET.md#pocket-image`).

- A converter that writes a key a published file carries re-mints that file and lands its bytes
  and sha256 in the model card, the catalog and every example manifest pinning it; one that adds
  a key leaves the published file at its pinned bytes (`REVIEW_TTS.md`).

## 8. Prove it end to end

The gates, in the order they run; the rig is the one that hears the lane
(`ARCHITECTURE_TTS.md#tts-rig-scoring`):

1. The kernel cells: `tests/test_tts_blocks.das` and `tests/test_gguf_quant.das` through dastest.
2. The image arms: `run.das -- --suite image --arm kitten,pocket`.
3. The families on the box's serving backend and on the CPU chain: `run.das -- --changed`.
4. The preflight fast tier. Its folder gate re-stamps `IMAGE_LAYOUT_STAMP_HASH` whenever the
   layout closure moves, and names the stamp to write.
5. The rig's tables in the PR body, before and after, on every model and lane the change reaches,
   as `REVIEW_TTS.md` asks of a change to what a flag-free run synthesizes.
