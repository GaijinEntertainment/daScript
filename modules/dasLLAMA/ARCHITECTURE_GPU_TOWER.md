# dasLLAMA Architecture - the tower attention routes and encode chains

Companion to `ARCHITECTURE_GPU.md`; a section is cited by its anchor. This document
carries what a tower driver is and the registrations its hooks come through, the three routes
that serve tower attention on Metal, the
one-command-buffer encode chain each family the Metal tower driver serves gets, and what the Metal
home answers at the TTS driver's site hooks. The GPU backend role table these sections build on - the tower driver's role row
included - stays in `ARCHITECTURE_GPU.md#gpu-backends`.

- `ARCHITECTURE_GPU_TOWER_FC.md` - the FastConformer chain's rotary, LayerNorm-conv form, the
  one GigaAM serves through.
- `ARCHITECTURE_GPU_TOWER_TTS.md` - the TTS driver's StyleTTS2 and Pocket seats, one text over every GPU
  home's rail.
- `ARCHITECTURE_GPU_TOWER_VULKAN.md` - the Vulkan tower's row classes and attention routes, the tower
  encode chains on the discrete homes, and the whisper ASR decoder on the discrete homes.
- `ARCHITECTURE_GPU_TOWER_RAIL.md` - the rail the Vulkan and CUDA homes' chain bodies dispatch
  through: the plane roles, the rail records, the lens-generated stamp overload, the primitives, the
  audio seats' install by home, and the family chains on the rail (the spectral fronts, gemma4a,
  qwen3a's front and the VAD; the FastConformer trio and its decoders; the whisper class and its decoder).

### A tower driver and the hooks it registers {#tower-reach}

A tower driver is a GPU driver file that serves a family's encoder or synthesis stages through
hook seats - stage slots the CPU chain calls and the driver fills. Every hook a tower serves
through is registered by an `[init]` of that tower's driver files: a seat, a drop hook, or a
reload or weights-epoch listener. The registrations run in `dasllama_metal_tower_register` (the
Metal tower), `dasllama_vulkan_tower_register` (the Vulkan tower), `dasllama_vulkan_tts_register`
and `dasllama_cuda_tts_register` (each discrete home's TTS seats, shells over the shared driver
`dasllama_tower_tts`), `dasllama_cuda_tower_register` (the CUDA
tower) and `dasllama_cuda_tower_audio_register` (the CUDA home's audio installer), so the hooks a
function can reach are the ones those register. The vision and audio seats follow the GPU home: the
Vulkan tower's `vulkan_tower_install_vision`, `vulkan_tower_install_audio` and the ASR decoder's
`vulkan_asr_dec_install`, and the CUDA tower's `cuda_tower_install_vision` and
`cuda_tower_install_audio`, are each registered under their home's name (`register_gpu_home`), run at
`[init]` only where that home is asked for (`gpu_want_cuda`), and run again when `set_gpu_home`
moves the tier; the CUDA home's audio installer seats each audio family from the home its row names
(`ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs`), and the translation and TTS seats stay the
Vulkan tower's under either home (`ARCHITECTURE_GPU_ASYMMETRIES_CUDA.md#cuda-asymmetries`).

A gate covers a hook when it asserts the hook's counter rising on a leg where that hook is the
only reachable hook raising it; `REVIEW_TOWER.md` names the counter each hook kind raises.

### The tower attention routes {#tower-attn-routes}

A tower head width is padded to `hs_pad = max(64, ceil32(hs))` - 72 and 80 both land on 96 -
because the QK stamps walk multiples of 32 while the guarded AV forms carry no multiple-of-64
requirement. A route that uses the padded width pays a restride to and from the towers' compact
rows. Three routes serve tower attention:

- **The flash route** takes a head size of exactly 72 on a canvas whose row count divides 64.
  It runs one pass - no score slab, no rowstat, no head restrides - reading K and V as
  per-head-contiguous f16 panels (the panel and tail-slack contract sits on the kernel class)
  while Q and the output ride the compact f32 rows. `DASLLAMA_METAL_TOWER_FLASH=0` pins the
  slab trio.
- **The per-window route** serves the qwen2.5 ViT's block-diagonal layers: one threadgroup per
  (window, head), one thread per query row, windows capped at 64 rows. It reads q, k and v
  compact and straight from device in f32 - a window's K and V sit in L1, and f16 staging noise
  would compound over the tower's 28 layers. Layers that attend in full restride onto the
  padded slab and take the shared trio.
- **The slab trio** - the prefill driver's QK, rowstat and AV builders - serves everything else
  and sizes its score slab at `heads x mp x nk64` halfs. Off the flash route that slab is the
  encode's only attention allocation; on it the driver takes a stub.

### The tower driver's encode chains {#tower-encode-chains}

Each family the tower driver serves gets one chain that encodes the whole encode into ONE
command buffer and reads back once: the block loop, and - for gemma4a and qwen3a - the mel and
conv front ahead of it, for parakeet the conv front. A chain is dispatch for dispatch the same graph as its family's CPU
encoder loop, in the same order and at the same operand shapes, so the CPU loop is the chain's
specification and the CPU-vs-GPU transcript cells are its parity instrument. Every chain is
best-effort: it answers false (or -1) on any shape, knob, quant-mode or device decline, and the
CPU chain serves that encode. Engage is read from counter deltas (`metal_tower_stats`,
`metal_tower_f16_encodes`), never from "the model ran".

The whisper-class chain serves both weight lanes. An f32 tower's block GEMMs read the f32 blob or
its halfword twin. A q8 tower's read the q8 planes themselves: `tw_q8_attach` uploads the stem's
second conv and each block's six GEMM regions once a tower, out of the CPU backend's repack layout
into the 34-byte q8 blocks the prefill driver's q8 GEMM reads (`q8_region_to_metal_blob`, the
transform the ASR-decoder driver uploads its planes through), and every site runs on the prefill
ladder (`pf_enc_q8_mm`) with the activations converted to one half panel where that ladder reads
one. The device blob is 34/32 of the q8 planes and no image carries it; it drops with the weights
epoch and the tower shutdown. The stem's first conv reads the f32 blob on both lanes - its
3 x n_mel columns are not quantized - and where those columns are off the GEMM's 32 lattice (80
mels: 240) it reads a device copy of its rows zero-padded to the lattice (`tw_conv1_pad_attach`),
the im2col pass padding its own rows with zeros to the same width. A position plane adds through
the shared bias rows stamp (`tw_bias` at `BIAS_ACT_NONE`) as one bias row as wide as the rows it
covers, because the stamp repeats its bias row every `d` elements: the whisper stem's row is its
whole plane, so each position row lands once; qwen3a's is one chunk's position rows, so every chunk
takes them, in a second pass after the `conv_out` bias row - the CPU's order, (x + b) + pos.

The whisper-class chain has three forms over one block loop (`TwForm`): the blocks alone, the
blocks with the tower's post-norm (whisper's encoder output), and the blocks with the projector
tail - the chat towers' soft tokens (`tw_tail_body`). The tail is the CPU tail's steps at its
widths, by projector kind: qwen2a pools row pairs (`MetalTwPool2`), norms, and runs one biased
linear; voxtral pools, norms, and runs the stacked rows through a GELU MLP; ultravox norms every
position, rms-norms the stacked rows at the projector's own eps, and runs the gated MLP
(`MetalTwSwigluRows`: silu of a row's second half times its first). A stack is no kernel: the
stacked row is the same memory read at the wider row. The tail's GEMMs read the f32 blob on the
f32 tile, so the tail seat serves it on an f32 tower whose widths sit on that tile's lattice
(`tw_tail_shape_ok`) and serves the blocks alone otherwise - a q8 tower's projector planes are not
on the device - answering which it served (`TowerTailServed`), so the CPU tail runs exactly when
the device one did not. Its engage counter is `metal_tower_tail_encodes`.

The vision chains run their ends on the device too. Every vision chain takes the image planes, not
the stem's columns: the chain uploads them (`tw_planes_up`) and runs the patch im2col on the device
(`tw_patch_im2col` on `MetalTwPatchIm2col`, the shared `GkPatchIm2col` template), which writes the pad
rows and columns zero and applies gemma4v's [0, 1] -> [-1, 1] map as its scale and shift; the family
skips its CPU im2col while a chain is registered and runs it only after a decline. Each block loop
runs block 0's pre-norm alone, ahead of the loop, and folds every later pre-norm into the residual
seam before it: the seam stamps (`MetalTwAddLn`, `MetalTwAddRms`, `MetalTwAddPostRms`, the Metal stamps
of the shared `GkPostAddLn` template) add the branch and write the next block's pre-norm from the
updated row in one pass, the last seam writing the tower's post-norm where the chain has one (the
whisper-class `ln_post` form, qwen25v's post-rms) and no norm otherwise, and a tail reads the rows the
last seam left. The gemma3v chain ends in its tail - the post-norm, the grid mean pool
(`MetalTwPool2d`), the soft norm and the projection (`g3v_tail_body`). The gemma4v chain runs the
patch conv and the 2-axis position add itself (`g4v_stem_body`), then ends in its tail: the grid pool, the sqrt(d) scale and the
standardize in one row pass (`MetalTwAffineRows`), the weightless rms, and the projection between its
two clamps (`g4v_tail_body`). With the tail on the device the soft tokens alone come back; the block
rows stay there. The vision chains serve both weight lanes: an exact tower's block GEMMs ride the f32
tiles over the blob (the crowned halfword twin where the crown compiles it), a q8 tower's the prefill
ladder's q8 GEMM over the vision q8 slab (`g_tw_q8_vis`: the family's block regions repacked once a
tower beside the audio slab, `tw_q8_attach_vis` over the family's block list - `gemma3v_all_regions`,
`qwen3v_block_regions` with the fused qkv split three ways, `gemma4v_all_regions` -
keyed by the tower's q8 planes and their scales) with the half feed where the ladder reads one (`pf_q8_mm_half`), so a pinned q8 tower
serves on the driver and declines nothing; the families register the f32 lane as served, so the policy
prefers the file's planes under the tower.

The gemma4a chain ends in the projector tail, in the blocks' command buffer: the out projection
and its bias at the encoder's own output width (`gemma4a_mid_dim`, the length of the weightless
norm's ones row), that norm, then the audio embedder to the decoder's width (`proj_dim`). The
embedder is square on E2B (1536 by 1536) and widens on E4B (1536 to 2560), so the two widths are
read apart off the file - the out projection's rows, the embedder's rows - and never assumed equal.
The chain lands the soft tokens in `Gemma4aState.out` and says so (`out_ready`). It serves the two lanes a
Metal image carries the way the FastConformer chain does (below): every weighted GEMM - the input
projection, the eleven a block, the relative-position table's projection, the tail's two - on the q8
ladder over the encoder's own q8 slab, or on the half tensor GEMM over the staged halfwords; a clamped
site clamps into the copy its GEMM reads either way.

Canary's front runs whole on the device ahead of its block seat (`metal_canary_front`, the front
seat): off the CPU-windowed frames, the DFT as a GEMM over the transposed twiddles, the power
spectrum, the mel sums, `MetalCnMelNorm` - the log and the per-feature normalization over the valid
frames, one thread a feature - then the parakeet front's convs and the input projection. The first
conv is the im2col GEMM both homes run (the shared `GkIm2col2d` at one channel, the f16-staged GEMM
over a `[ch][32]` tap panel kept once an encoder - `tw_conv0_panel`, canary's taps channel-major in
its file, parakeet's tap-major - and the bias-and-ReLU rows); the two depthwise convs stamp the template both homes share
(`GkDwConv2d`) over the file's taps, and a stage's valid rows ride the template's length mask
(`ih_valid`), so a row past them adds nothing and no mask pass is dispatched.

The FastConformer chain (canary, parakeet and gigaam share it: one context, one block body over the
canary offsets record, parakeet's offsets mapped onto it with no GEMM biases and its tap-major
depthwise stamp; gigaam's rotary, LayerNorm-conv form is `ARCHITECTURE_GPU_TOWER_FC.md#tower-fc-rotary`)
in its rel form runs the rel-pos (Transformer-XL) attention one head at a time on the f32 GEMM
builder, because a per-row kernel walking every key is the encode's cost past a minute of audio:
a pack writes the head's (Q+u), (Q+v), K_h, V_h^T and pos_h panels contiguous and zero-padded to
the GEMM lattice (rows to 32, keys and rel positions to 64), two GEMMs give the content scores
(Q+u)*K_h^T and the rel scores (Q+v)*pos_h^T, one row kernel adds the rel score at the CPU form's
shift (npos - 1 - q + k), scales, and takes the softmax with zeros in every pad, a third GEMM
gives P*V_h into the head's slot of the AV rows, and one unpack per layer writes the compact
output. The score slabs are per-head scratch reused head after head - each head's writer is
encoded after the previous head's reader. The head size is held to a multiple of 64 (the AV
GEMM's column lattice) and the head count to the per-head uniform seats. Behind gigaam's encode the CTC head
(`metal_gigaam_ctc`, `register_gigaam_gpu_ctc`) is one command buffer on both weight lanes: the f32 GEMM over the held
encoder rows (else `st.x` uploaded), its weight panel padded to the 64-row column tile with zero rows, then the pick
(`enc_ctc_pick`, `MetalCtcPick` on the template both homes stamp, `GkCtcPick`) - the bias, each frame's argmax (the lowest index of a tie, as the CPU's) and the
emitted class's log-sum - two words a frame read back; the collapse stays on the host.

The chain serves the two lanes a Metal image carries, canary's projector riding behind the blocks in
their command buffer on the same lane. On the q8 lane every weighted GEMM - the eleven a block, a rel-form
front's two 1x1 convs and pre-projection (`fc_front_records`), canary's projector - runs on the prefill
ladder's q8 GEMM (`pf_enc_q8_mm`) over the family's q8 slab: the regions repacked once at the bake into
the ladder's 34-byte blocks and named by their element offsets in the family's `qblob` (`TwQ8Rg`), the
activations converted to one shared half panel where the ladder reads one. On the exact lane the GEMMs
are the image's own halfwords (`wblob`: canary's and parakeet's file f32 saturated to f16 at staging, the
bytes the discrete homes read; GigaAM's carrier's own), bound as the GEMM plane and served by the half x
half tensor GEMM where the toolchain compiled it (`pf_hmm_ready`), X converted into one shared half
panel; the f32 blob holds no GEMM. The flavor pick (`metal_tower_lane_ok` under each family's, `metal_gigaam_serves_model`)
maps the CPU image for an exact lane the box cannot serve - `DASLLAMA_METAL_TOWER_F16=0`, or no half
GEMM - so no Metal image carries an f32 GEMM plane, and `metal_tower_f16_encodes()` counts the encodes on
the halfword plane. GigaAM's q8 lane reads the same slab form (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`).
The per-head attention GEMMs multiply packed f32 panels and stay on the tile stamp.

Under the `attn_dev` crown the attention runs over EVERY head in five dispatches a block instead
of the per-head loop's forty-one (`pf_enc_fc_attn_dev`), on the prefill driver's device pair
(`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device`), which reads Q, K and V in the compact
row layout with the head in the grid's third axis - so nothing is packed a head at a time and the
AV writes the compact output itself. `MetalFcTwin` writes the five operands as halves ((Q+u),
(Q+v), K, V, the projected rel table; the scale folded into K and the table, zeros in every pad
row); the pair's QK stamp runs twice, content scores into one slab and rel scores into a second
at the rel table's width; `MetalFcPExp` adds the rel score at the shift, leaves `exp(s - max)` in
the content slab and the row's reciprocal sum in the stat plane (0 for a pad row); the pair's AV
reduces over the live keys. The rows sit no farther from the CPU rail than the per-head form's.
The two score slabs hold all heads at once -
`heads x mp x (nkp + wwpad)` halves - so past `FC_DEV_SLAB_MAX_BYTES` (about three minutes of
audio at eight heads) the per-head loop's reused scratch serves; `set_metal_fc_attn_dev` is the
lever and `metal_tower_fc_dev_encodes()` the engage counter.

Parakeet's chain starts at the mel: its whole-encode seat (`metal_parakeet_encode`,
`register_parakeet_gpu_encode`) runs the subsample front and the pre-projection ahead of the
blocks in the same command buffer - the first conv off the one-channel mel as the im2col GEMM
with its bias-and-ReLU rows and the two depthwise stride-2 convs as one-thread-an-element kernels over
the blob's tap-major taps (the shared `GkDwConv2d` under TAP_MAJOR), the two 1x1 convs and the
pre-projection as weighted GEMMs of the chain with a bias-and-ReLU or a bias row pass behind them,
the feature permute between (the shared `GkFeatShuffle`, which also zeroes the pre-projection's row
pad). The rel table is built on the device at the head of every FastConformer command buffer
(the shared `GkRelSinCos` into the context's zero-padded table, canary's and parakeet's alike), so
no session table is uploaded. The seat declines the knob, a model that is not the Metal-flavored image
and its lane's weights off the device (`device`: the q8 slab unbound, or the half GEMM the halfwords need
not compiled), each counted, and answers -1 with nothing counted on a clip whose first conv output passes
1 GB (about five minutes of audio), a shape off the lattice, a scratch buffer it cannot take or a failed
command buffer: a CPU image's encode then runs whole on the CPU, and a Metal-flavored image's refuses
(`tower_cpu_refuse`).
`metal_tower_stats().convs` counts the encodes whose front ran on the device.

### The Metal home's speech seats {#tower-tts-chain}

The StyleTTS2 seats of the kitten and kokoro families and the Pocket family's seats on this home are the
shared TTS driver's bodies (`dasllama_tower_tts`, `ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats` and
`ARCHITECTURE_GPU_TOWER_TTS.md#tts-pocket-seats`) over the home's rail: the gates, the slabs and scratch, the
dispatch sequences and the readbacks are that one text, and the CPU chain is its specification. The home adds
four files. The rail (`dasllama_metal_rail`: `MtRail`) carries the buffer, upload, readback and zero primitives and
runs a seat's chain as one command buffer of the home's queue (`rail_tts_shot`; under the profile ledger one a mark). The lens
(`dasllama_metal_dispatch`: `[metal_rail_dispatch]`) reads a stamp's members through the scan the Vulkan and CUDA
lenses read and generates its `ensure_<name>()`, its census row under the stamp's name, its rail overload
`enc_<name>(rl, roles, pc, ...)` and `rail_ensure_<name>(rl)`, a pipeline a stamp built on first use; the overload
binds the seats at the stamp's slots and dispatches (`mtl_rail_dispatch`), a member that views another member's
buffer (`@role = "alias"`) on a slot of its own, since two members cannot share a binding in this home's shading
language. The primitives of the schedule-fed and the f16 tiles (`rail_mm_kq`, `rail_mm16`, `rail_site_groups`)
panic on this rail: it answers false at `rail_tts_cm2_on` and `rail_tts_f16_on`, so no body reaches them. The stamps (`dasllama_metal_classes_tts`) are the common templates of `dasllama_gpu_kernels_common` under
this home's emitter, the source kernels compiled without fast math. The seat file (`dasllama_metal_tts`) holds
the rail, a seat function a stage, the home's `TtsHome` - its name, the tower knob and arm, a decline's words - and
the two families' flavors for `TowerDev.metal`.

The driver asks the rail before each dense site, and this home answers where its own tiles outrun the shared
kernels (`dasllama_metal_sites`); a `false` leaves the site to the shared route, which is every answer on the
discrete homes.

- **A conv and a dense layer** (`rail_tts_conv`, `rail_tts_mm`) ride the home's tile ladders - the f32 tile GEMM
  and its conv-gathering stamps (`MetalSt2ConvMm` a forward conv, `MetalSt2ConvTrMm` a transposed one, the X
  loader reading a row's 8-run of k through the conv's geometry, so no im2col is materialized), their f32-exact
  twins (float tiles, twice the threadgroup memory) where the chain asks exact, and the stored-plane tile over a
  quantized or half-float leaf (`MetalSt2StoredMmT`: a format a stamp, the leaf widened in the kernel as the CPU
  reader widens it) - then the bias row. The tile reads whole 32-row tiles and writes the leaf's padded width, so
  a call whose rows carry an offset, a narrower output or no room for the whole tiles lands in the rail's own
  rows and is copied. A conv's f32 rows or half-float leaf rides the tile only where the tile takes its shape -
  the inputs on its 8 lattice (32 over a stored transposed lift), the outputs on the 64 lattice - and y holds the
  whole 32-row tiles the tile writes. A quant leaf in the shared layout leaves the hook at once. A home-layout
  quant leaf off that shape panics: the bake's own-layout rule put it there. The dense sites run the float tile,
  since they feed the durations and the phases; their f32 rows or half-float leaf ride it only where x and y need
  no copy - at offset 0, at the leaf's own widths, with room for the whole tiles.
- **The AdaIN column statistics** (`rail_tts_colstats`) are one pass, a threadgroup a channel, up to
  `MTL_COLSTATS_MAX_T` rows; past it a threadgroup's walk of a whole column loses to the shared block passes.
- **A Q8_0 LSTM recurrence** (`rail_tts_lstm_q8`) runs the home's recurrence stamp over the leaf's blocks
  (`MetalSt2LstmQ8`, one direction a threadgroup, `hidden` the gate-block stride in `St2LstmArgs`).
- **No schedule-fed conv tile** (`rail_tts_cm2_on` false): every conv rides the exact or the f16-staged ladder.

A leaf's device layout follows its site. The flavored model's bake tags every stored leaf with its site
(`TtsStoredSite`), and a leaf at a hooked site is staged in this home's layout (`ts_gather_member_metal`): a
Q8_0 plane as 34-byte blocks row-major (`q8_region_to_metal_blob`), a K-quant plane as its quants in disk order
from an even superblock under its scale plane in the Metal form (`metal_kq_scale_form`). A quantized leaf in the
home's layout is always served by the hook, since no shared stamp reads that layout; a leaf at any other site
keeps the layout the discrete homes read. The Pocket frame loop's row GEMV is the shared template with the
home's row dot for a Q8_0 and a Q4_K leaf in its layout (`MtPkGemvStoredT`, its `OWN` constant a stamp, the
driver reaching them through the `rail_tts_pk_own` hook), sixteen rows a workgroup; a Q5_K or Q6_K leaf of that
loop stays in the shared layout under the shared dot's stamp of its member.

The declines are the shared driver's (`TowerDecline`), counted there a home; engage is
`tts_home_stats(TowerDev.metal).encodes`, one a seat a call. The parity instruments are the per-stage cells on identical
inputs (`tests/_tts_parity.das`), the served synthesis across the knob, and the stamps' cells
(`tests/test_metal_tts_kernels.das`, their case bodies the discrete homes' own).
