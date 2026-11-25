# dasLLAMA Architecture - the TTS driver's seats

Companion to `ARCHITECTURE_GPU_TOWER_RAIL.md`; a section is cited by its anchor. This document carries the seats
the TTS driver serves on the Vulkan, the CUDA and the Metal home: the StyleTTS2 synthesis seats of the kitten and
kokoro families, and the Pocket TTS encode, voice, codec, frames and prompt seats. The CPU chain is the
specification, dispatch for dispatch. What the Metal home answers at the driver's site hooks is
`ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`; the GPU backend role table these sections build on stays in
`ARCHITECTURE_GPU.md#gpu-backends`.

### The StyleTTS2 seats {#tts-st2-seats}

The driver is one text (`dasllama_tower_tts`), every function generic over the tower rail
(`ARCHITECTURE_GPU_TOWER_RAIL.md#tower-rail`): a seat's gate, its slab and scratch, its dispatch
sequence and its readback are written once, a dispatch seats its planes with `rail_seats` and goes
through the stamp's rail overload, and a class is built through `rail_ensure_<name>(rl)`. Every call a
home overloads is written `_::name(rl, ...)` - a stamp ladder's placeholder too (`_::tile_enc_...`) - and
resolves where the home's seat file instantiates the body, so the driver requires no CUDA module. What a
home adds is its seat file (`dasllama_vulkan_tts`, `dasllama_cuda_tts`, `dasllama_metal_tts`): a rail, a seat
function a stage that calls the driver's body with it, and the home's `TtsHome` - its log name, its knob (`on`), its arm (`arms`), a decline's words
(`words`), its forget, its activation release and held bytes (`ARCHITECTURE_TTS_MEMORY.md#tts-device-release`)
and the conv site's schedule-fed tile pick (`cm2`); the driver counts each home's served seats, declines and dense dispatches over widened rows (`tts_home_stats`, `tts_home_declines`). The
seat file installs the seats, the two families' flavors for its `TowerDev` (the serve gate and the bake) and the
flavor's block layout: the Vulkan home on a build without das_metal where the tier does not want CUDA, the CUDA
home where it does, the Metal home on every Apple build - so the home that holds the tier fills the seats. A
device buffer's allocation, upload and readback, a zero fill and the chain's one-shot are the home's rail
primitives (`rail_tts_alloc`, `rail_tts_put`, `rail_tts_take`, `rail_zero`, `rail_tts_shot`). The CUDA home's
shot launches in plain stream order, programmatic dependent launch off for its launches alone
(`set_cuda_tts_shot_pdl`, a race seam, lets the home's mode through): raced in one process, every family's
synthesis runs slower under it (`PERF_LEDGER.md`, the CUDA speech seats' carveout and launch order). Three CUDA TTS stamp families compile without NVRTC's fast math (`[cuda_kernel(fastmath = false)]`, `modules/dasCUDA/ARCHITECTURE_EMIT.md`) - the tanh GELU, the harmonic source's noise draw and sines, and the Pocket row GEMV with its stored members -
so their transcendentals and quotients round as the other homes' and the speech cells hold the other homes' bars; every other stamp, the f32 tile GEMM and tower row kernels a seat launches included, keeps it (`PERF_LEDGER.md`, the CUDA speech stamps' exact math). The driver's
slabs, scratch and voice slots are one home's at a time: a seat on another home's rail first runs the last
home's forget (`ts_claim`).

Every home keeps the profile ledger the driver's marks fill, and answers the driver's question whether it is on
(`rail_tts_prof`, the ledger's switch `rail_prof_on`: `DASLLAMA_GPU_PROF`, or a test's `set_rail_prof_pin`). The
question has no generic default, so a rail that does not answer it does not compile the driver. A mark after a
dispatch (`rail_pt`, a `RailProfRole.tts_*` role) ends a stamp: the span since the stamp before it, charged to the
mark's role. The Vulkan rail reads the span off its timestamp query pool; the CUDA rail off timed events on the rail's
stream, so a span is stream time and would count a gap the host leaves between launches - the host runs ahead of
the device on all but a few of a shot's stamps (`PERF_LEDGER.md`, the CUDA speech seats' carveout and launch order),
so the spans read as the device's; the Metal rail runs a command buffer a mark, committed and waited, and reads its
GPU start-to-end time. A home says once a process, at its first profiled shot, how the ledger changes that shot
(`rail_prof_say`), and its forget drops the last report (`rail_prof_forget`), so a swapped model reads none. After
the shot every home reports through the one report (`rail_prof_report`, `dasllama_gpu_rail`) - a
`<home> tower <chain> gpu:` line, then a line a role in microseconds, stamps and share - so two homes' logs compare
role by role; the dispatches past a shot's last mark run uncharged.

The kitten and kokoro families' synthesis rides the home's tower knob (`DASLLAMA_VK_TOWER`, the CUDA tower's,
`DASLLAMA_METAL_TOWER`) through the family's hook record (`ARCHITECTURE_MEDIA.md#tower-gpu-hook`), one seat per stage. A
seat the driver does not fill stays on the CPU chain; every home registers every seat, so the parity rail reds a stage no seat serves.

Every seat computes in f32 but the decoder's and generator's wide convs over a stored leaf on a home with an f16 feed. A part's weights live in two
places the shared host writer lays out (`dasllama_tts_slab.das`, `tts_slab_build_stored`). The slab holds f32 rows: every norm as a scale and a shift
row, every bias row, and a conv or linear its leaf holds as floats - dense as [cout padded to 64 x k*cin padded to 32] with column j = tap*cin + ci -
with an LSTM direction's f32 recurrence transposed to [H][4H], so the recurrence kernel's lanes read a step's column as consecutive floats. A leaf the
carrier holds quantized or as half floats stays out of the slab and is staged as stored (`ts_gather_member`), a member a format: Q8_0 blocks, Q4_K,
Q5_K and Q6_K superblocks and half floats, each leaf at its element offset in the member's plane pair, in the layout every shared stamp reads - a Q8_0
row row-major under an f16 scale a block (`q8_gather_rows`), a K-quant row's quants under the five-word scale row a superblock (`kq_gather_rows_dev`),
a half plane as the rows a tile reads (`tts_stored_half_rows`). Four more members hold the quant formats again in a home's own layout (`TS_ST_OWN`):
the writer tags each stored leaf with its site (`TtsStoredSite`: a conv, a dense layer, a recurrence, other), and a leaf at a site the home's own tile
serves goes to that layout (`ts_leaf_member`, `ts_own_sites` - the Metal home's alone, `ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`), which no shared
stamp reads (`ts_mm_stored` asserts it). The served cells hold each format on a file that stores it - Q8_0, Q4_K, Q6_K and half floats: the loader's
tag on a leaf, then that format's stamps launched on the serving home (`gpu_served_lane`, `test_pocket_stored_members`); no published file stores
Q5_K, whose stamps the kernel cells alone hold. The exact tile's stored members read the shared planes (`F32GemmT` under `WSTORED`): a thread widens
eight weights of one row at the stage as the CPU reader widens them - w = A q + B off the block's scale and minimum, a half as its float - so the
products are the f32 rows' own; a transposed conv's leaf is its lift [tap * cout + co][cin], which the tile addresses as row co, column tap * cin + ci
(`F32GemmArgs.tcin`). So the served lane's weights are the CPU lane's exactly - a load-time Q8_0 mint keeps a block's scale as the half a file stores
(`q8_matrix_mint`), the value the device plane holds - and what still parts the two is that lane's quantization of the activations, which the exact
seat does not do. A leaf off the members' lattices - a conv whose inputs sit off 8 (32 for a transposed one), a K-quant plane the CPU backend repacked
(a GPU flavor's staging repacks none, `kq_planes_seat`), a Q8_0 recurrence whose hidden width sits off 32 - is read back into the slab as f32 rows
through the active repack (`conv1d_weight_row`, `q8_plane_row_f32`, `kq_plane_row_f32`). The writer marks such a slot (`TtsConvSlot.widened`), and the
driver counts every dense dispatch over one for the rail's home - a dense layer, a conv, a row GEMV, the f32 recurrence of a Q8_0 direction
(`ts_count_widened`, read as `tts_home_stats(dev).widened`). The cells hold the count exact, so every other stored leaf of their files is read as stored: a
StyleTTS2 chunk dispatches the generator's two noise convs, whose 22 inputs - the harmonic source's spectrum - a home's image holds as halves
(`st2_stored_formats_held`), and a Pocket clip's encode its first conv, one input channel wide, a window (`test_pocket_voice_seats`); the English
file's synthesis and its Q8_0 and K-quant forms' frames, prompt and voice seats dispatch none (`pocket_dev_formats_held`,
`test_pocket_stored_members`).

A part's slab is baked into the flavored image, not built at attach. The flavor's bake (`tts_st2_bake`,
`tts_pk_bake` through `tts_part_bake`) runs the writer's two passes with no device, gathers the stored members and
puts the part into the image's device block as pieces a name: the f32 rows, each member's plane pair, the leaf
records (`TsLeaf`) and the archived slot record. The parts are `st2.albert`, `st2.text`, `st2.pred` and `st2.dec`,
and Pocket's `pk.codec`, `pk.frames` and - a cloning model's - `pk.enc`. The bake spends the planes the slab took
(`TowerDevBuild.spent`), so a flavored image holds them once; the flavor's serve gate maps the CPU image where the
home's seats would decline the load. Each home's StyleTTS2 and Pocket serve gate admits every lane (the lane flag
goes unread) on three answers: the home's tower knob, its device, and every class the seats dispatch
(`tts_st2_ready`, `tts_pk_ready`). The block's layout hash (`tts_st2_block_layout`, `tts_pk_block_layout`) folds
the slot records, the leaf record and the home's layout, so an image baked for another layout is not this flavor's.
A seat's attach (`ts_slab_attach`) uploads each piece byte for byte and reads the leaves and slots back, the slab
resident under the address of the block's rows piece; a failed upload releases the slab and the seat declines
`memory`. That address is the slab's key, so a home's forget also runs when the weights epoch moves (the Metal
home's on its reload prep): a reload that maps the new image at the old address would otherwise find the old slab.
`tts_forget` releases every slab, scratch and voice slot the caller's rail owns - on a home's drop hook, on the
weights epoch and when the serving home changes - and releases nothing where the device state is another home's or
no home's, since every home hears every drop. The seats' activation rows are a scratch per seat, one device buffer
a slot sized in floats a quarter past the call's need in 256 KB steps, kept across calls and rebuilt only when a
call needs a slot wider than it holds (a decoder's scratch runs to gigabytes and a rebuild costs tens of ms, so the
headroom is what lets the longer sentence after a short one reuse it), with one host readback buffer sized to the
rows the seat copies out (the wave, the post rows, the encoder rows), never to the widest slot - a decoder's
sample-rate column slot runs to hundreds of MB, and a host-visible buffer that wide costs a tenth of a second to
map; the buffer is the home's, one for every seat, grown to the widest read a seat names (the CUDA home copies off
the stream and keeps none). The pipelines' build and the device bring-up the process's first seat call paid are
reported under the info log (`ts_arm_ensure`).

Before a dense site's shared dispatch the driver asks the rail, and a `false` - every answer on the Vulkan and
the CUDA home - leaves the site to the shared route:

| Hook | Asked in | The shared route |
|---|---|---|
| `rail_tts_conv` | `ts_conv_padded` | the im2col and the exact or the biased f32 tile |
| `rail_tts_mm` | `ts_lin` | the exact tile's stored member, or the f32 tile over the slab's rows |
| `rail_tts_colstats` | `ts_colstats` | the four block passes |
| `rail_tts_lstm_q8` | `ts_lstm_dir`, for a recurrence in a home's own layout | none: the leaf is that home's alone |

A decoder or generator conv on the bias pass's four lattice over a stored leaf rides an f16 feed where the home
has one (`ts_conv_fast`): its columns are gathered as halves at the leaf's row width (`TtsIm2col16`), and a
quant leaf's cm2 tile reads the leaf's planes as stored (`KqCm2BatchT` through the rail's site tile,
`rail_mm_kq`, under `rail_tts_cm2_on`: the home picks the tile for the shape - `TtsHome.cm2` - and the
dispatch's schedule record and map are written in place on a host-mapped plane, `RailMapped`, as the chain is
recorded), a half leaf through the f16 GEMM over its half plane (`rail_mm16`, under `rail_tts_f16_on`: the
Vulkan home where it serves an f16 tile, the CUDA home always); a transposed conv runs as one dense GEMM of
its input rows against the lift, into [tin][taps * cout] rows, and a fold (`TtsFoldT`: the overlap-add at
to = ti * stride + tap * dilation - pad); the bias row is added in place (`TowerBiasAct`). Where the home has
no such tile, the schedule plane has no room, or the conv is narrower than 16, the exact tile's stored member
serves; a conv whose rows are the slab's runs the im2col and the f32 tile. The half column slot holds 256 rows
of slack past the widest conv's rows at its row width: a cm2 tile loads whole row tiles and the KHR tile whole
16-row blocks, reading up to 15 rows past the last output row of the slot (its stores are masked past `rows`;
the cm2 tile clamps its loads), and the slack is what those reads land in. The narrow F0, energy and post
convs and every conv of the front end and the predictor stay on the exact or the f32 tile.

The front end - the PL-BERT encoder, the text encoder, the duration encoder, the duration head
and prosody - runs on the f32-exact tile GEMM (`F32GemmT`: 64 positions by 64 weight rows a
workgroup on plain f32 workgroup tiles, every product one mad in k order, the edges masked, the
bias added at the store) so the durations round as the CPU's and the F0 drives the same phase.
The albert seat is the embeddings gathered from three slab planes (`TtsRowGather`), the
embedding norm, the map-in, then the one ALBERT layer n_layers times: q, k and v projections,
the bidirectional f32 attention row (`TtsAttn`, one workgroup per query row and head, softmax
the CPU way), the dense projection, the residual add with the attention norm, the ffn, the
exact tanh GELU (`TtsGeluTanh`), the ffn output, the residual add with the full norm - the
residual seams on the tower's post-add layernorm stamp (`TowerPostAddLn`: the branch linear
runs the unbiased tile and the seam adds its bias row, then the norm of the sum, as the CPU's
post-LN block orders them), the norms on the tower's layernorm stamp reading the slab's rows.
The rows come back to the host through the home's readback; one seat call is one
submit.

The text seat runs on rows [t][c] where the CPU chain runs channel-major: the embedding rows
gathered (`TtsRowGather` with the position and type rows switched off), then per conv the
im2col over the rows form (`TtsIm2col`, column j = tap*cin + ci, the slab's weight-row order)
and the biased tile over the slot's rows, the layernorm over the row (the CPU's channel
layernorm per position) on the tower's layernorm stamp, and the leaky ReLU (`TtsLeaky`), the
conv stack ping-ponging two row sets as the CPU chain does; then the BiLSTM as two directions,
each its input gates on the biased tile ([t][4H], gate order i, f, g, o) and one workgroup
walking the recurrence (`TtsLstmDir`: four lanes a hidden unit, one a gate, each dotting the
transposed recurrence's column with the previous step's h over four accumulators - i mod 4,
folded at the end, so a step's loads pipeline; the CPU's `matmul` dot carries its own order,
so no k order is the reference's - the unit's first lane folding the gates into the cell; the
backward direction writing the second half of the shared [t][2H] rows; a Q8_0 recurrence on the 32 lattice read from its own
stored plane [4H][H], the lane's row widened a block at a time under the same accumulators - `TtsLstmDirQ8`). The workgroup is 1024 invocations, so a home whose compute limit is
under that declines the LSTM seats as `device` at their ensure (`rail_tts_lstm_ok`). The rows read back and transpose on the host into the channel-major
[c][t] the CPU stage answers. Every seat's scratch is one float-count list, a slot a row set,
the seat's own slot table naming them, plus two more row sets - the aux rows a call uploads
(the style vector, then fc(style) per norm: gamma then beta, gamma + 1 where a layernorm reads
it as its weight) and the AdaIN column stats the device writes.

The predictor's three seats share one slab (`tts_pred_write`). The duration encoder: the BERT
projection on the biased tile, the style appended as a broadcast residual of stride 0
(`TtsConcat`), then per LSTM the BiLSTM, the AdaLayerNorm as the layernorm stamp reading the
aux rows, and the style appended again; the rows come back [t][c + style]. The durations: the
BiLSTM, the projection, then the sigmoid sum a token over speed (`TtsSigSum`), one float a
token back. Prosody: the encoder rows transposed on the host into the rows form, the shared
BiLSTM, then each branch's residual blocks pinging between two row sets - the AdaIN as the
column stats (four coalesced passes over 128-row blocks: `TtsColPartSum` and
`TtsColReduceMean`, then `TtsColPartSq` and `TtsColReduceSq`, the block partials past the two
stats rows of the scratch) and the fold with the leaky ReLU
(`TtsAdainLeaky`, the CPU `adain_affine`'s scale and shift), the upsampling block's depthwise
transposed pool (`TtsPoolDw`) and its nearest shortcut as the row gather over a r / 2 map, the
convs on the im2col and the biased tile, the learned shortcut the same, the join as the scaled
add (`TtsAddScale` at 1 / sqrt 2) - and the branch's one-channel projection; F0 and energy
read back a float a frame.

The decode seat has its own slab (`tts_dec_write`) and scratch: the
F0, energy and text front convs on the im2col and the biased tile, the encode block over the
concat rows (`TtsConcat` laying the text rows, the residual rows and the two curves as columns),
each decode block re-concatenated where its width asks, the stream pinging between two row sets
and landing in the first; then the source - the noise rows drawn on the device (`TtsSrcNoise`,
a counter-keyed normal, not the CPU's PCG32 stream) unless the call carries a captured stream,
the phase increments and the phase per frame on the resample law's stamps (`TtsSrcLowTorch` /
`TtsSrcLowOnnx`, `TtsSrcCumsumTorch` / `TtsSrcCumsumOnnx` - the torch law's double
accumulator as a compensated two-float sum, the kernel marked `precise` so the device compiler
keeps the correction terms it would otherwise fold to zero), the harmonics' sines at the interpolated phases
through the source linear and its tanh (`TtsSrcSinesTorch` / `TtsSrcSinesOnnx`, the sine of a
phase past a hundred thousand radians reduced by 2 pi in exact pieces), and its spectrum rows
on the pad law's stamp (`TtsStftReflect` / `TtsStftEdge`: the magnitude and the phase); then
the generator - per stage the leaky ReLU, the source rows through the stage's noise conv and
Snake residual block (`TtsAdainSnake` on the norm slot's alpha row), the transposed upsample
conv on the im2col's transposed read, the last stage's reflected row (two row copies on `TtsPkRows`), the
kernels' blocks averaged into the stage's out (`TtsAxpy`), then the final leaky and conv_post -
and the inverse STFT (`TtsIstft`, the log magnitudes and phases through the transposed conv
weights, the window envelope divided out where the model asks). The waveform reads back.

### The Pocket seats {#tts-pocket-seats}

The Pocket family's five seats - the voice encoder, the voice state, the codec, the frame loop and the text
prompt - ride the same driver and knob, registered through `register_pocket_gpu`. A Pocket leaf is staged as
the StyleTTS2 leaves are: the codec's and the encoder's convs (halves on the unpinned lane) and their
transformers' matrices ride the exact tile's stored members, the frame loop's matrices the row GEMV's stored
stamps, so a served lane reads the file's weights as the CPU chain reads them, without that chain's quantized
activations.

The row GEMV (`GkPkGemv`: `TtsPkGemvF32T` over the slab's f32 rows, `TtsPkGemvStoredT` over a stored leaf) is
`ROWS` rows a workgroup, 32 lanes a row, x staged - four rows on the Vulkan and the CUDA home, sixteen on the
Metal home, a template constant its stamps set. The f32 lanes each read a float4 of the row a step - the slab
writer lays every run on the 16-byte lattice - and a float where a row base sits off it, the lanes folded by
shuffle. A stored lane decodes a word a step (`stored_dot_into`), the member a template constant (`WFMT`), so a
stamp reads one member and the driver picks the stamp by the leaf's format (`tts_pkst_enc`, its format arms
`kq_tile_stamp`'s): a half lane a word of two halves; a Q8_0 block eight lanes, a word of four quants each,
under the block's f16 scale; a K-quant superblock the row's 32 lanes, a word of a sub-block's quants each, under
the five-word scale row. The bias, norm and vector rows are still the slab's. The epilogues are stamps, an f32
one and a stored one a member each: the bare dot, the layernorm-modulate-SiLU prologue, the gated residual, the add-SiLU over
the slab vector, the tail that adds the noise row by the flow's weight and denormalizes the latent, the
layernorm with the qkv rope-and-store - the rope's pairs sit in a workgroup's even and odd rows, so the spans
sit on the four lattice; the q span and the k span at their own widths - the residual joins with and without
the scale row, and the layernorm with the GELU.

The codec seat decodes a call's latents as a stream of windows (`TS_PK_CODEC_WINDOW` latent frames, 32), each
window one upload, one submit and one readback of its samples, up to `TS_PK_CODEC_MAX_FRAMES` (4096) frames a
call - the positions the slab's rope tables hold. What crosses a window stays on the device: every conv whose
kernel passes its stride is a stream conv, its carried input rows (`conv1d_carry_rows`: k - stride of a forward
conv, ceil(k / stride) - 1 of a transposed one) kept in a carry slot and zeroed before the first window, and
the codec transformer's keys and values a layer, kept across calls while they hold the call's rows; the host
keeps the plan alone (`ts_pk_codec_stream_plan`: each conv's carried rows and their place). A window is the
CPU's `pocket_decode_latents` window: the latent projection, the depthwise transposed upsample (`TtsPoolDw`)
reading its carry as a left pad, the transformer as the CPU's layer loop at the window's first position - the
layernorm into a copy, the qkv projection, the NORM rope on the q and k spans against the slab's tables
(`TtsPkRope`), the k and v columns copied into the layer's rows (`TtsPkRows`), the causal attention over the
context window (`TtsPkAttn`, a workgroup a head and query row, the head size an argument from
`TTS_PK_ATTN_HS_MIN` (32) to 64, the value sum walking eight keys a step so a lane's loads sit in flight
together), the output projection, the layer scale (`TtsPkRowScale`), the residual seam on the post-add
layernorm stamp against the slab's zero row, the ffn with the tanh GELU - then dec_in, per ratio the ELU
(`TtsPkRowsElu`), the transposed upsample and the ELU-conv-ELU-conv residual block, the last ELU, dec_out and
the sample column copied out.

The encode seat turns a cloning model's reference clip into latent frames, the codec's stream run the other
way (`tts_pk_encode`, the CPU's `pocket_encode_latents`): windows of `TS_PK_ENC_WINDOW` (8) latent frames, up
to `TS_PK_ENC_MAX_FRAMES` (1024) a clip, enc_in, each residual block, ELU and strided conv, enc_out, the
encoder transformer at the window's positions and the downsampler, whose carry repeats the first row on the
first window. It shares the codec seat's scratch and K/V rows, and its slab (`pk.enc`) is baked only for a
cloning model the admission takes (`tts_pk_enc_ok`). The voice seat is the second half of the CPU's
`voice_state_from_latents` (`tts_pk_voice`): the speaker projection over the latents into the rows after the
BOS row, then the backbone's rows form from position 0, the last layer ending at its keys and values, the
caches read back to the host.

The frames seat: the voice's K/V rows live on the device per backbone layer as [cap][kv_heads dh] rows with
the rope tables for every position they can hold, current (`tts_pk_voice_current`) under a key over the host
caches' addresses, their fill and capacity and a sample of the rows they hold - an address alone outlives the
voice that held it, since a later voice's caches can land at the freed address with the same fill and
capacity. The attention row reads a query head's keys at its key-value head (`GkPkAttnArgs.kvs`: `d / kvs`
query heads a key-value head). The chunk's text rows come up before the loop, but for the rows a served prompt
seat left there (`TtsPkPromptDev`, naming the voice, the fill, the row count and a sample of the embedding
rows, spent on entry by the frames call after it, served or declined). Each frame is the CPU's `frame_step`
and `head_step` at t = 1 - the input linear, per layer five dispatches (the qkv row off the residual's norm
with its q span roped in place and its k and v spans roped and stored into the caches' row at the frame's
position, the attention over the cache, the out projection added into the residual under its layer scale, the
ffn up off the residual's norm with its GELU, the down projection added into the residual), then the output
norm's row copied to the frame's conditioning row, the EOS logit, and the flow head on the row GEMV's
epilogues. A grouped-query layer's fused projection is `attn_dim + 2 kv_heads dh` wide; the `z_proj`
bottleneck is one more GEMV off the normed residual, whose row - `cond_dim` wide - the EOS logit and the
head's conditioning read; a time-free head carries a zero row where the lsd head's time constant sits, and its
tail weighs the noise row by the flow (`GkPkGemvArgs.x0mul`: 1 where the step adds it, 0 for a drifting head
whose output is the frame). The frames run in batches of eight a submit (`set_pocket_frame_batch`), the EOS
rule walked on the host between batches from the logits read back, the noise drawn a batch ahead in the frame
order the CPU draws it and the generator rewound past the frames made (`pocket_frames_batched`,
`dasllama_pocket.das`, handed the seat's noise sink, its submit and its EOS source).

The prompt seat is the codec transformer's layer loop (`ts_pk_rows_tf`, the one rows form both chains
dispatch) over the frames slab's layers and the chunk's embedding rows (the host's gather,
`pocket_prompt_embed`) at the positions after the voice's rows: the rope from the voice slot's tables at the
first row's position (`TtsPkRope`, stamped from `GkRopeTab`), the keys and values into the voice slot's rows
at those positions, the attention over the slot's rows below them, the last layer ending at its keys and
values (only the caches are read after a prompt - the CPU chain's `transformer_rows` makes the same cut under
`kv_only_last`); the rows come back to the host caches after the submit, so the CPU frame loop reads them as
its own. The prompt seat serves an f32 linear at any input width on the 4 lattice: the f32 GEMM stages each x
row's columns out to the slab's 32-padded K, reading zero past the row's width (`stage_x`), where the weight
columns past the width are zero too - no row reads past its own bytes.

The declines are the reasons of `TowerDecline`, counted by the home: `knob`; `flavor` (the model carries no
device block for the home); `shape` (a StyleTTS2 width off the 64 lattice, a head width other than 64 or 128,
more than 512 tokens for the attention stage, an LSTM direction over 256 hidden; a Pocket backbone head other
than 64 wide or a codec or encoder head outside 32 to 64, a width off the 4 lattice or past the GEMV's
4096-wide stage, a cache past 2048 rows, a codec call past 4096 frames, a clip past 1024 - `tts_pk_tf_ok`,
`tts_pk_frames_ok`, `tts_pk_codec_ok` and `tts_pk_enc_ok` name them); `device` (the tier has no device, or a
class failed to build); `memory` (a slab piece, a scratch or a binding past the home's range). Engage is the
serving home's `tts_home_stats` `encodes` (the driver counts a home's serves and declines) beside the families' own
seat counters (`styletts2_gpu_stats`, `pocket_gpu_stats`, `pocket_encode_gpu_stats`); the parity instruments
are the per-stage cells on identical inputs (`tests/_tts_parity.das`, driver-neutral over the home that
serves) and the kernel cells of `tests/test_vulkan_tts_kernels.das`, `tests/test_vulkan_tts_conv_kernels.das`,
`tests/test_vulkan_tts_pocket_kernels.das`, `tests/test_vulkan_tts_source_kernels.das`,
`tests/test_cuda_tts_kernels.das` and
`tests/test_metal_tts_kernels.das`, their case bodies shared (`tests/_discrete_tts_cells.das` and its conv,
Pocket and source companions).
