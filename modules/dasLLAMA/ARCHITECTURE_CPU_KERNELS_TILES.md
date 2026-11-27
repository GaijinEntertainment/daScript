# dasLLAMA architecture - the CPU tile families

Companion of `ARCHITECTURE_CPU_KERNELS.md` (contract: `../../ARCHITECTURE_COMMON.md`); each section is cited by
the code that embodies it. This document carries the tile kernels of the CPU chain: the matrix-unit bf16 tile, the
f32 batch tile's lattice, the generated f32 GEMM family, the half-float weight rows and the bf16 weight family.

## Mechanisms

### The bf16 tile keeps its sum in the matrix unit {#amx-bf16-tile}

An int8 matrix tile (AMX TDPBSSD, and the reference build's own AMX path) accumulates in int32,
and every quant format carries a scale every 32 weights, so the tile has to be stored and
scale-folded at every block: that traffic is what made the int8 tile lose to the vector lattice
on every box it was raced on. The `amx_bf16` perm of every superblock family and of the q8
family has no fold.

The batch wrapper dequantizes a row group's weights once per token block into a bf16 panel in
the pair-interleaved B layout (`[group][k-step of 32][16 pair-rows][16 cols][2]`, every scale
folded in) and widens the quantized activations to bf16 once per call (q times its block scale
- the Q8_K superblock's, the Q8 block's - the same rounding the vector lanes multiply by); the
tile then runs `TDPBF16PS` over all of K with C in the tiles and stores straight into y, so the
K length is free. The tile covers 32 tokens (two A tiles) by one or two row groups (the perm's
`nrsplit`, the tile-form companion's value). A tile form above 0 IS the 32-token tile, so the
superblock families carry no tokstep companion (`amx_bf16_tile_tokens()`); the q8 family keeps
its own, which the int8 tile also answers. A token tail short of the tile walks four tokens at a
time through the family's `tile4` companion - the stamped perm's 4-token vector tile over the
same int8 plane, the busd lattice like every amx companion - and the last one to three tokens,
like a group tail short of the tile, through the gemv rows core. A batch shorter than the tile
step runs no amx tile at all: the q8 kernels and the superblock cell take the vector walk
(`q8q8_batch_cell_gen`, `kq_batch_cell_gen`) on `tile4`, which builds no activation plane and no
bf16 panel and dispatches once - the decode step's four rows are that batch. A `tile4` stamped
from an amx perm prefetches its weight stream a fixed distance ahead (`tile4_perm`, one prefetch a
line, on the packed planes; a byte panel is the walk's fresh scratch): four tokens of dots a
weight line leave the core too few lines in flight off DRAM without it, and the four-row step
runs a fifth slower (`debug-jit`; `PERF_LEDGER.md`, the AMX short-batch arc). The
panel amortizes over the token block, so the walk reads its block off `q8_token_block` floored
at 512 tokens and rounded down to whole tiles (`bf16_token_block`), under no L2 clamp - a block
the clamp cuts pays a second panel and a token tail - and it chunks its groups in whole tile units
over the whole token range: a chunk of one group misses a two-group tile outright, and a token
slice per cell rebuilds every panel per slice. The q8 family carries the leg on both scale
planes - a Q8_0 GGUF keeps its binary16 weight scales, so a real q8 model prefills on the
wscale_f16 twin, whose panel companion widens the f16 scales into the same panel.

The panel is the tile's bill: written as a daslang loop it runs scalar and costs more than the
matrix unit's whole multiply, so every panel is an emitted companion (`k4q8_panel_gen` and its
siblings, one generator over eight 64-byte quads per block: the format's code planes decoded to
a byte per weight - nibble split, the k5/k6/k3 high-bit planes, a `VPSHUFB` codebook for the
LUT formats, the gemv's grid gather with the sign column applied for the five grid formats -
then the per-row scale FMA and `vcvtneps2bf16`). That convert is an AVX512_BF16 instruction, a
feature apart from `amx-bf16`: the perm requires both, the `x86-amx` class lists both, and a
clone built without `avx512bf16` would call a soft-float routine for it. A quad is four consecutive weights of each of
the group's 16 rows: quad `q` of a 32-weight block holds weights `4q..4q+3`, 4 bytes a row, 64
bytes in all. The grp16 int8 plane's four-byte columns are already quads, so the q8 panel
companion loads them as they lie and decodes nothing. A panel companion stamped from a perm
that is not `amx_bf16` is an empty body - the tile form is 0 there and nothing calls it; its
das body, which no stamp runs, is the row dequant scattered into the pair layout
(`kq_panel_rows_bf16`).

The numerics are a bf16 envelope of the vector route, the Metal f16-tile class, never
bit-exact: the tuner gates every variant on that bar, and the q8 family's crown also passes the
end-to-end confirm - a superblock family's crown is the race's alone. The tile ops are the raw
immediate-tmm intrinsics: the cfg companion loads the full palette (eight 16x64 tiles) once per
dispatch chunk after the witness's XTILEDATA grant. The tile operands are line-aligned where
the walk owns them (the panel and the bf16 activation plane): Intel splits a tile row that
straddles a cache line. The activation plane is a module global of the context that calls the
batch kernel - one per inference thread; the kernel builds it before it dispatches, and no lane
builds one, since a fork-pool lane owns no globals. The panels are that context's walk scratch
(`walk_panels`): one panel per dispatch slot (`get_dispatch_slot_bound()`), each on its own
cache lines with a line to spare, sized before the dispatch; the indexed dispatch
(`maybe_parallel_for_indexed`) hands a chunk its slot's panel, so a chunk allocates nothing and
two chunks never share one. The byte panel of a format whose tile reads unpacked quants rides
the same scratch, and the q8 walk's bias sums a scratch of their own (`q8q8_bias_sums`).

### The batch tile walks an absolute lattice {#f32-batch-tile-lattice}

`matmul_batch_core` computes Y = X*W^T in four-row by four-token tiles over an f32 plane
(`f32_batch_tile`) or a bf16 plane (`bf16_batch_tile`, each weight widened by the exact high-half
shift as it is read, so the outputs are the f32 walk's over the widened plane): each step along n
loads four weights and four inputs and issues sixteen FMAs into sixteen SIMD accumulators, where one
`dot` per output loads two values per FMA. A tile's accumulators and a lone `dot` round differently,
so which kernel computes an output has to be a property of the output alone. The lattice is the 4x4
grid counted from row 0 and token 0, the same whatever range a chunk covers: the dispatch unit is a
row tile, a token chunk is whole tiles, every token block (`f32_batch_token_block`: the X rows that
fit the smaller of the box's q8 L2 budget and `g_f32_batch_x_block` bytes, rounded down to whole
tiles, or the whole token range when it fits) starts on the lattice, and only the rows past the last
whole row tile of d and the tokens past the last whole token tile of ntok take a per-output dot
(`batch_dot_rows`: `dot` on an f32 plane, `dot_bf16` on a bf16 one). Any row chunking, worker count
or token split then lands the same bits (`test_matmul_batch.das`'s lanes cell holds the threaded
walk to the one-worker walk bit for bit); a batch and the same tokens one at a time do not, and
`test_matmul_batch.das` holds the fp32 path and the bf16-plane path to the per-token dot over the
widened weights within `F32_TILE_REL`. `g_f32_batch_tile_min_n` routes a dot shorter than it to
`batch_dot_rows` whole; it ships at 1, so the tile serves every length, and
`harness/f32_batch_race.das` is the lever that races the two constants and the nest against the
tile. `matmul_batch_rows` is the nest as its own entry, under no knob: a batch whose outputs decide
what decode decides on the GEMV's bits takes it - the MoE router's prefill logits (`mm_fblob_rows`),
whose top-k over tile-rounded logits parts from decode's at a near tie and routes the token to
another expert than decode would (`test_parity.das` holds the 26B-A4B's Q4_0 file to its pinned
tokens on the CPU backend; `test_matmul_batch.das` holds the entry to the per-token matmul bit for
bit at the router's shape).

### The f32 GEMM is a generated tile family, and every output is one ordered sum {#f32-gemm-tile-family}

`gemm_f32` and `gemm_f32_jo` compute C += A x B over f32 planes - the conformer attention's three
products, the encoder towers, whisper, the prefill attention and the TTS blocks' convolutions. Under
the LLVM JIT they run the `gemm_f32_tile_gen` family (`dasllama_math.das`, beside the walks; the generator is `dasllama_gemm_gen_f32.das`): a tile of `mr` rows by `nv` vectors of the perm's width - 128 bits on
any target, 256 on x86-64 with AVX2, 512 with AVX-512 - with one accumulator a row and vector, each
k step one broadcast of A against the tile's B vectors.

Every output C[i][j] is one sum: it starts from C's own value and adds a[i][k] x b[k][j] for k
ascending, each step fused where the target fuses. No tile shape changes that, so every perm, every
width and the hand-written tile land the same bits, and the family's cell holds a stamped tile to
the scalar sum bit for bit (`test_math_activations.das`).

A tile walks its own columns: whole tiles, then single vectors, then each narrower vector down to
four lanes, then columns one at a time, so a column count off the tile's width keeps its columns on
vectors. The conformer's key count is rarely a multiple of sixteen - 138 keys for an 11-second clip
- and the hand-written kernel hands the columns past its last sixteen-wide tile to a scalar loop
(`gemm_f32_region`). The
walk (`gemm_f32_family`) steps whole row blocks, with the column blocks outermost where the caller
asks (`gemm_f32_jo`: B's strip stays in the cache across the row blocks). The rows past the last block
- every row of a call shorter than a row block, a decode step's among them - take one row with the k
positions outermost (`gemm_f32_row_k`): B is read along its rows, as it is stored, where the row
companion (one row at eight vectors abreast) walks a plane wider than the cache down its columns. The
half-weight walk's rows keep the companion: their strip of B is the packed 8 KB buffer.

The companions `gemm_f32_mr_gen` and `gemm_f32_cols_gen` answer the stamped tile's rows and columns;
the reference bodies answer zero rows, and the walk then declines, so a tier with no generated tile
- the interpreter, AOT, a perm the target declines - runs the hand-written `float4` tile. A tile
takes each plane's row stride apart from its counts, so the walk hands it a k block or a column
strip of a larger plane; with the column blocks outermost the walk steps K in blocks whose strip of
B is 8 KB, and since each block adds onto C's own value in k order the bits do not move.

A plane of half floats multiplies through the same tile (`gemm_f16w_jo`): the walk widens one strip
of B - a tile's columns by a k block - into an 8 KB buffer and every row block multiplies that
buffer. The convert is paid once a strip, never once a tile: a convert inside the tile's k loop runs
once a row block and takes the same vector units the multiply-adds are bound by.
`target_half_converts` answers whether the target converts halves in hardware, off the JIT's own
target record; a loader asks it before it keeps a weight plane as halves.

### A half-float weight row is served as halves only where the target converts them in hardware {#half-rows-by-target}

A weight row the CPU reads as half floats - the transducer decoder's gate and joint rows, a Pocket
codec conv a file stores F16 - costs nothing extra where the CPU converts halves in hardware (every
aarch64, x86-64 with F16C) and half the bytes. Where the convert is software (wasm, the `x86-base`
class) every read pays it, so those targets serve the same rows as floats.

`DlimCpuConfig.rows_f32` is that outcome, and `active_rows_f32` reads it: a cross-box bake's target,
else the process pin (`set_rows_f32`: `dasllama-convert` sets it from `--config`, so a target whose
backend this host registers bakes in the target's form, and a test reaches the float form on a box
that converts), else the JIT target's own answer (`target_half_converts`, a companion of the f32 tile
family; interpreted, the answer is no). The identity spells it ` f32rows` inside its last field only
when set, so an image of a converting target keeps its identity, and the two forms never share one:
a lane is its CPU class, and no class holds both kinds of box.

The decoder's rows keep their planes: `RnntState.wh` and the baked `wh` / `jw` hold halves, or
floats as two halfwords each, and the step reads the form the plane's size names
(`rnnt_baked_rows_f32`). A GPU flavor's staging keeps halves whatever the CPU - its device seat
binds them, and `rnnt_dev_planes_ok` takes no float plane.

The K/V cache follows the same answer: `KVDtype.automatic`, the default of every session, pool, chat and
scheduler constructor and of the GPU mirror's ask, is q8_0 where rows are served as floats and f16
elsewhere (`default_kv_dtype_`), so a default session and the mirror it meets agree;
`kv_codec_served` resolves it a model, and a model whose heads are not whole 32-blocks keeps f16
with no step-down line, because nothing was asked.

### A bf16 plane multiplies through the matrix unit on bf16 activations {#bf16-weight-tile-family}

A bf16 weight plane - an encoder tower's GEMMs, gemma-E's per-layer projection - multiplies as bf16
times bf16, the f32 activations rounded to bf16 first: `bf16w_batch` runs the `bf16w_tile_gen` family
where the family serves the call; a declined call multiplies the f32 activations unrounded. Once per call, `xbf16_build_f32` rounds the activations to
bf16 (nearest even) and also keeps the rounded values widened back to f32.

For each unit, `bf16w_panel_build` packs the plane's rows into the B-tile layout the quant families'
bf16 tile reads: for each pair of K positions (2k, 2k+1), the pair from each of sixteen rows side by
side. That is one 32-bit move per pair, because the plane already stores each pair adjacent. The
same `amx_bf16` emitter then runs the AMX bf16 tile (`TDPBF16PS`) over the panel and the activation
plane.

A unit is `tileform` groups of sixteen rows (the perm's `nrsplit`, one or two) over every whole
`tokstep` block of tokens (32 under `amx_bf16`). Three tails go through `batch_tile_block`, the
batch tile walk over the bf16 plane, fed the widened rounded copy: a unit short of `tileform`
groups, the tokens past the last whole block, and the rows past the last whole group. So every
output of one call multiplies the same activation bits, whichever kernel computed it.

`bf16w_batch` declines the whole call when the selected perm is not `amx_bf16` or the target did not
emit it (`bf16w_tileform_gen` is 0, or `bf16w_witness_gen` is false), when n is not a multiple of
32, when d is under 16, or when ntok is under `tokstep`. A one-token call never reaches it (the GEMV
path serves). A declined call runs `matmul_batch_core` over the f32 activations, unrounded. The
matrix unit adds in a different order from a sequential dot, so the probe
(`harness/gen_tune_probe.das`) holds the family to `bf16_envelope` - the bf16 rounding step at the
largest output - and not to `batch_tile_block`'s bits.
