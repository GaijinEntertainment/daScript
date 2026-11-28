# dasLLAMA architecture - the CPU kernel planes and decodes

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`). Section 2 here continues
the mechanism numbering; each section is cited by the code that embodies it.

## Mechanisms

### The k3 and k6 planes are packed per sub-block {#kq-subblock-planes}

A k6 grp<mr> plane's qh columns `2blk` and `2blk + 1` carry one sub-block's four `j` sites, each as
a 2-bit field at bit `2j`; the disk byte `h*32 + half*16 + j*4 + t` feeds sub-blocks `4h..` at bit
`2b`. k3 packs the same way - qs columns `2blk + half` carry the sub-block's four `j` sites at `2j`,
and the hmask column `blk` carries its eight sites at bit `s` (lo at `j`, hi at `4 + j`). One
sub-block's decode then costs two loads for k6 and three for k3, and nothing loaded lives past it;
the row-interleaved disk order cost one load per `j`. The layout is CPU-flavor: the `.dlim` a box
bakes is for the hardware that runs it, so a CPU plane owes nothing to the GPU tiers' shapes.
The layout is part of what `IMAGE_VERSION` stamps.

### A grid format's CPU gemv decodes as a panel or as row groups {#grid-decode-forms}

Five formats (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs) have two gemv decode forms. The PANEL form gathers
a superblock into an alloca panel first and reads packed positions through one dword load per
4-byte column, the four positions of a column sharing the load. The ROW-GROUP form composes a
weight-width vector straight from the grid words - width/64 rows x 8 weights, one u64 grid entry
per iq2 row - and reverts to byte loads, because the column dword read only pays inside the panel.
The sdot lattice always takes row groups; on x86 the panel's latency chain does not scale with the
core, so `x86-vnni512` takes row groups for iq2xxs and `x86-amx` for iq2xxs and iq3xxs, everything
else the panel. A VBMI seat (`ARCHITECTURE_CPU_KERNELS.md#vbmi-lattice`) takes row groups unconditionally.

### The VBMI symbol lattice {#vbmi-lattice}

On a zmm VBMI target a grid block decodes as row groups through a symbol lattice. Every grid byte
comes from a tiny alphabet (three symbols for the iq2 family, eight for iq3), so the grid is baked
as two compact code planes - entry `e`'s low and high half, four 2-bit symbols each for iq2, two
3-bit for iq3 - plus the alphabet as a per-lane `vpshufb` table and `ksigns_iq2xs` whole (128 bytes:
exactly the two registers one `VPERMI2B` indexes). Per format the block's index bytes gather into one
64-lane vector per column (lane `r*4 + position`) and look up in the code planes - `VPERMI2B` per 128
entries, index bit 7 blends the pairs, the 9th and 10th index bits arrive as lane masks (iq3s and iq2s
from the row's qh byte, iq2xs from bit 0 of its u16 word's high byte). The row's sign bytes land in
the same lane layout: the plane's own column for iq3s and iq2s, one ksigns `VPERMI2B` over the 7-bit
codes for the rest. Per row group and weight octet a constant two-source shuffle places each row's
code bytes in its qword, `VPMULTISHIFTQB` spreads the symbols into bytes, one `vpshufb` maps them to
magnitudes, and the signs ride the activation copy as a mask `(x ^ m) - m`. The lattice row shares
its tile body and planes with the 512/mr16 row, so only the gemv differs - what the gemv's own seat
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#gemv-seat`) races.

### q20's 2-bit codes reach the dot three ways {#q20-code-spreads}

A q20 block's eight code bytes hold element j at bits (j % 4) x 2 of byte j / 4, four consecutive
elements a byte. `rm_b32_gen`'s q20 arm turns them into byte lanes by one of three spreads, the
`spread` knob of the `q20q8_rm_gemv_gen` grid (`q20_spread`):

- `mshift` - each 16-byte group of lanes takes its block's code word in both qwords, one
  `VPMULTISHIFTQB` pulls element c from bit 2c and a mask keeps two bits: k order, AVX-512 VBMI at
  width 512 only.
- `shuf` - a byte shuffle replicates each code byte over its four lanes, one 16-bit shift (0 or 4,
  by word) and a fixed shift of two place a word's two fields at bits 0 and 8, two masks keep them:
  k order. The IR is generic, so NEON reads it as `tbl` and `ushl`.
- `shift` - a dword shuffle copies a code dword over the four dwords of a 16-byte group and a dword
  shift by 0, 2, 4, 6 under one mask leaves element 4t + s at byte 4s + t: the transposed order.
  The activation bytes take the same transpose (`rm_x_order`), one byte shuffle a vector, which the
  rows of a row-lane group share.

The levels are the second knob, `lev`. `u` dots the codes 0..3 as the unsigned side of
`vpdpbusd` / `maddubs` and subtracts the block's activation sum, a ones dot the rows of a row-lane
group share; `s` subtracts one in the lanes and takes the signed path - the sign trick on x64,
`sdot` direct on NEON. A block pair of the one-row form starts on an even block, so its two blocks
read the group's one f16 d (`emit_soff`).

### q10's sign bits reach the dot three ways {#q10-sign-tests}

A q10 block's sign word holds element j at bit j % 8 of byte j / 8, bit j of the little-endian
word. `rm_b32_gen`'s q10 arm turns the words into a lane mask by one of three tests, the `spread` knob
of the `q10q8_rm_gemv_gen` grid (`q10_signs`):

- `mask` - the words' halfwords picked into lane order and read as an `i1` vector: a mask-register
  move a vector, AVX-512 BW only, and VL at width 256.
- `shuf` - a byte shuffle replicates each sign byte over its eight lanes and a per-lane bit test
  sets the lane: the reference's AVX2 and NEON form, in generic IR.
- `plane` - NEON's GEMV: one bit of the span's sixteen sign bytes a plane, no shuffle
  (`rm_q10_plane_span`, `#plane-gemv`). The four-token tile keeps `shuf`.

The levels are the second knob, `lev`. `u` dots the bits as 0/1 on the unsigned side of
`vpdpbusd` / `maddubs`, doubles the dot and subtracts the block's activation sum; `s` is the sign
flip on x64 - the activation negated where a bit is clear, dotted against ones - and the bits as signed
weights +-1 into `sdot` on NEON. Each is the exact integer sum `dot_q10q8_scalar` folds once a
block under the group's d. A block pair of the one-row form shares its group's one f16 d, as q20's.

### NEON's low-bit GEMV reorders the activation, not the codes {#plane-gemv}

A q20 or q10 byte packs the codes of 4 or 8 consecutive weights, so the block walk spreads sixteen
bytes over their weights' lanes before every dot. The `plane` spread of the two GEMV grids moves
that cost to the activation, once a call: `rm_emit_xplanes` rewrites the row into the kernel's
frame a span of sixteen code bytes at a time, plane k the activation bytes of each byte's code k,
with the span's four quarter sums. A row then dots plane k against code k of the bytes in place -
a shift and mask for q20, a bit test for q10 - and folds the sums once a span.

### The low-bit CPU rows decode a row group into the q8 tile's plane {#q20-device-layout-walk}

The CPU planes of q20 and q10 are their device planes and their disk order - interleave 1, no
repack - so neither has a grp<mr> tile family. Their `B32Slots` rows in the generated tier
(`gen_b32_slots`) run the per-32 walks (`b32g_batch`, `b32g_groupn`, `b32g_batch_groupn`) with a
walk group of the stamped q8 tile's rows (`q8q8_layout_gen`) where a grp family's interleave would
be, one body over the member's tag. A group's GEMV is the member's stamped row-major GEMV
(`rm_gemv_gen`) over its rows. Its token blocks ride the q8 tile: `b32g_panel` expands the group's
rows to levels in a per-dispatch-slot panel (`walk_panels`) laid out as the stamp's q8 plane - its
k-group and weight bias (`q8q8_kgroup_gen`, `q8q8_wbias_gen`), a block's binary16 scale a row - and
the stamped s16 tile (`q8q8_tile_s16_gen`, `q8q8_tile4_s16_gen` at a wider token step) reads it, the
decode paid once a group for every token of the block. Rows past the last whole group take the
scalar dot (`dot_lowbit_q8_scalar`). The rows set no repack and no layout, so the portable identity
and interleave 1 stand: the dense and expert CPU sites, the one-copy rail's device arm
(`rm_dev_serves`, q20's) and the `.dlim` read the same bytes.

### A CPU tier selects on the TARGET, not the host {#cpu-tier-target-select}

The arm64 SDOT tier registers its backends only under the JIT and only for an arm64 TARGET - the
artifact's architecture, never the running host's. Off the JIT the `sdot4` family runs its scalar
fallback bodies, which are slower than the portable `dot_q8q8` the vectorizer handles, so the
portable tier stays selected wherever hardware SDOT is not emitted.

The portable tier picks its dot form on the target at compile time. On wasm SIMD128 the
auto-vectorized template dot is the slow form - the ISA carries no int8 dot for LLVM to find,
while the `idot4` builtin lowers there to the ISA's own widening multiply-adds - so a wasm target
takes `dot_q8q8_idot4_ps`. Every other target keeps the template.

The K-quant dot needs no such pick. The k4 arm of `dot_kq` splits a packed byte into its two nibbles on a
SIGNED `byte16` - `q & 15` and `(q >> 4) & 15`, both non-negative, so both are valid int8 - and
feeds `idot`, which every target lowers for itself. One body, no target branch, and the lattice op
carries the per-target knowledge instead of this file. Nothing here competes with the x64 kernel
the tune grid crowns: that one walks the repacked grp planes (`k4q8_gemv_gen`), a different
layout, and reaches the k4 arm of `dot_kq` only on the disk-order arm.

### Where integer SIMD stops at 128 bits the disk-order K-quant and q8 batch GEMMs read a 16-bit activation image {#x16-rows}

Wasm SIMD128 has one cheap widening multiply-add, `i32x4.dot_i16x8_s`: sixteen-bit lanes in, pair
sums out. The int8 `idot` costs more there - an unsigned nibble against a signed activation has no
`extmul`, so each half is two extends and an `i16x8.mul`, then a pairwise add. x86 below AVX2 is
the same machine for this purpose: `pmaddwd` is that multiply-add, lane for lane, and no generated
family has a 128-bit integer row. The disk-order
GEMM of Q4_K, Q5_K and Q6_K on those targets therefore leaves `dot_kq` for a generated rows
kernel over operands already sixteen bits wide: `k4x16_rows_gen`, `k5x16_rows_gen` and
`k6x16_rows_gen` (`dasllama_gemm_gen::<fmt>_x16rows`, one generator keyed by format).

`kqx16_batch` builds the activation side once a call: the Q8_K quants widened to int16, and the
sums of each 32-element block (the per-16 sums added in pairs, which fit int16). The kernel
builds the weight side once a row, into a scratch of its dispatch slot: each quant widened and
multiplied by its sub-block's scale, and the superblock's scales as floats. A Q4_K or Q5_K row
stores quant times scale (at most 15 x 63, or 31 x 63), the eight mins as int16, `d` and `dmin`.
A Q6_K row has no mins: its quant carries an offset of 32, and the weight stored is the quant
less 32, times the signed scale of its 16-element sub-block (at most 32 x 128) - the offset term
`dot_kq` takes from the per-16 sums sits inside the dot. Every token then costs one dot for each
eight weights of that row, `nt` tokens abreast, and for Q4_K and Q5_K one dot of the eight mins
against the block sums. A lane's pair sums stay far inside int32.

The integer sums are `dot_kq`'s and the float fold is its fold in its order, so each kernel
agrees with the portable rows kernel bit for bit. The Q6_K sum is `dot_kq`'s scaled dots less 32
times its scaled sums, as one integer: the per-16 sums are the sums of the quants the image
widens. The reference body of each family is the same arithmetic over the widened image, and on
every other target the generator declines: the family's witness (`k4x16_witness_gen` and its
twins) answers false there, `kqx16_batch` returns false, and the portable walk serves the call.
The generated tier reaches the portable GEMM through one function slot (`set_kq_gemm_gen`),
because the portable module sits below it.

The portable q8 batch GEMM has the same twin, `q8x16_rows_gen`, over either scale plane (f32 or
f16; the stub's scale type picks the load). Its weights need no scratch: eight bytes sign-extend
into one operand, shared by the `nt` tokens of the walk. A Q8_0 block folds to float every 32
weights, so the kernel keeps the block's four pair-sum lanes, scales them into four float lanes
and sums the lanes once, at the row's end. That grouping is its own - lane l holds weight pairs
l, l + 4, l + 8 and l + 12 of each block - so it agrees with the portable kernel to float
reassociation, not bit for bit, and its reference body states the grouping. It reaches the
portable backend through `set_q8_batch_gen`: the backend's batch slots try the generated kernel
first, and the backend keeps its name, so an image baked for `portable` still loads.

The four kernels are `[tune]` families whose seats differ in `nt` and are gated on
`simd128|f16c,simd128|!avx2`: a wasm target, or an x86 class with hardware half converts and no
AVX2 (`x86-avx`). The emitter reads its scales as halves, so it declines an x86 target without
F16C; an AVX2 class is served by the 256-bit families, so the gate names the absence and the
emitter declines there too - a whole-grid tune build on an AVX2 box races no 16-bit seat. Every
other family's rows are gated on their own instruction set too - the NEON-shaped rows on `neon`,
the x64 rows on theirs - so on a wasm target those families have no seat: they stamp their
reference bodies, owe a wasm profile no entry, and gain one when a wasm seat is added to them.
The one ungated row is the bf16-weight tile's `mr16`: no target emits it, and a profile of a
class with no bf16 tile names it to say so, which is why it stays a seat on every class.

### Classic prefill scores through the decode's own dot {#prefill-decode-same-score-dot}

Classic prefill quantizes each query row and runs its scores through `kv_score` - the dot a cached position takes at decode - so a prefix served by one prefill pass and the same prefix served a token at a time agree bit for bit under the block codecs (q8_0, tq4). Flash prefill tiles with online softmax and reorders the sum, so it holds to a tolerance instead.

### The q8 chain's activation forms are the reference's, site by site {#q8-activation-forms}

A superblock plane (the k and iq families) reads its activation as Q8_K - one f32 scale per 256
elements and the per-16 quant sums - the form the reference build's CPU dot reads for those weight
types; a q8 or per-32 plane reads Q8_0, one scale per 32. Everything else runs in f32: the norms,
RoPE, the scores and the V sum against the f16 cache (the reference rounds the query and the
probabilities to f16), an F32 plane, and a BF16 plane against its bf16 rows (the reference rounds
the activation to bf16). No site quantizes coarser than the reference.

On a residual stream with outlier channels the Q8_K step is large for the rest of its block - on
gemma-4 E2B a down-projection input peaks at 84 times its rms, its elements carry 2.2% relative
error at 256 against 0.98% at 32, and 9.5% to 16% of them round to 0. A forced feed through such
a chain is chaotic: an ulp moves an element across a rounding tie, one level in a large block
moves the next layer's input, and the error grows from there. The q8 chain matches its f32
emulation (the f32 chain with every site's activation Q8_K-rounded) to 3e-7 a layer up to the
first tie, and an ulp-sized change anywhere - the f32 GEMV's summation order on another ISA,
`x / d` for `x * (1 / d)` in the rounding - moves a step's top logit by more than one. The q8
chain then lands anywhere in the band the reference build's quantized CPU path lands in by its
attention form and batching, and the file's f32 chain is the reference a device chain is held to.
A sampled speculative walk parts from the plain seeded walk where a draw lands near a boundary the
verify row and the one-token decode place an ulp apart, on the CPU chains and on the Metal chain
alike.

The figures are a scratch probe's on the M5 Max (an engine build instrumented at every GEMV input,
kept outside the tree), over `gemma-4-E2B-it-Q4_K_M.gguf` and the forced-feed cells' 48 prose ids
(`ARCHITECTURE_MEASUREMENT_FORCED_FEED.md#reference-forced-feed`). The input statistics are the
f32 chain's (`QuantMode.fp32`) one-row decode GEMVs over those ids, averaged a site and width: the
12288-wide down-projection input's largest |x| over its rms is 83.7 (the 6144-wide one's 70.6);
the relative error, the root of the summed squared rounding error over the summed squares, of
absmax/127 blocks is 2.21% at 256 and 0.98% at 32 (the 6144-wide 2.18% and 0.99%); 9.5% of the
12288-wide input's elements and 15.9% of the 6144-wide one's round to 0 at 256. The per-layer
match is the BOS token's decode step: each layer's attention and FFN residual reads 4e-8 to 4e-7
relative to the emulation's through layer 21's attention, and layer 21's FFN, the first tie, 8e-5.

### A kernel a worker lambda invokes is not `private` {#kernel-visibility-lifted-workers}

The tier kernels run from lambdas the job dispatch lifts out of the function that wrote them, and a lifted body reaches its callee by module-scope name, so every kernel a `parallel_for` body or a registered kernel pointer reaches is declared without `private` - the row-range cores, the groupN kernels and their wscale_f16 twins among them. The same reach is why a backend with no native groupN tier registers the portable one: a `KernelBackend` slot is never null.

### A grp<mr> repack is two interleaves {#grp-repack-interleaves}

Every format's repack is made of the same two moves over a row of units - a 256-weight superblock, or a 32-block - of `ubytes`: the quant-plane interleave, which stacks mr rows per group and lands column c of unit u in row r at `[g][u][c][r][colw]`, and the scale-plane interleave, which is field-major, `[g][u][field][i][r][width]`, the fields in destination order with their source offsets. Tail rows (`d % mr`) stay disk-order and the row-major dots serve them.

### A hot leaf is instantiated in its caller's JIT partition {#jit-partition-inlining}

The split-module JIT inlines within one partition only, so a leaf a hot loop calls is written to instantiate in the caller's: a generic over its operand (`iq_grid_octet`), or a plain function stamped beside the codecs it drives (`kq_transcode_units`). A call into another module's function inside a `[tune]` loop body blocks the loop's vectorization outright - `dot_bf16` spells its bf16 widen as the shift for that reason. The same rule runs the other way for the leaves themselves - the inliner's decisions over a leaf follow its call-site count, which is why the run-time-format `dot_kq` stamp lives in the test fixture `tests/_kq_dot.das` and not beside the sixteen tag overloads in `dasllama_math_default.das`.

### The deltanet per-token core is one code path for every caller {#dn-token-core-shared}

The Gated-DeltaNet recurrence is written once per token, in three pieces: the prelude (the beta and g
transforms, the causal conv against the session's history, SiLU, the q/k L2-norm and the q
pre-scale), the per-(token, v-head) delta rule against that session's state, and the z-gated
out-norm epilogue. One token of a decode step, one position of a prefill and one row of a
B-session batched step all walk those same three pieces, so no caller can round apart from
another; what differs is only the lane split over (token, head) and the projections around them.

### A row-major kernel walks a row's blocks one after another {#row-major-walk}

The rare rail's CPU kernels read the device planes, which are row-major - a row's blocks one after another -
where every other generated CPU kernel walks the grp<mr> interleave (`dasllama_gemm_gen.das`, `rm_rows_walk` and
its bodies): a vector holds 32 weights of one row, the
lanes are k chunks, each block's lane dot folds under its scale, a row's lanes sum in order. A fold is one
fused multiply-add a block on a loop-carried accumulator, so a row's chain is latency-bound alone; the
block loop runs the body on several accumulator sets abreast (`rm_block_loop`: four in the GEMV; two in
the four-token tile on x64, whose four tokens already run four chains; one in the NEON tile, where a
second set spills its 32 vector registers; the sets summed once after the loop, the blocks past the last
whole group one set at a time). On x64 the row loop carries `llvm.loop.unroll_and_jam.disable` (`attach_loop_flags`, `llvm_jit_common.das`):
the parameters' `noalias` hints make jamming rows legal, and a jammed row pair or triple shares only the activation
load while adding each row's sign and unpack work on the ports the dot fills. On zen4 (AMD Ryzen 7 PRO 8700GE,
class `x86-vnni512`, `debug-jit`) the jam loses on six of 81 rows - mlx8 `vpdpbusd_width256_mr8_accs1` reads 1.099
ms jammed against 0.959 unjammed at 640 x 32768 - wins on one and is level on the rest (`harness/rm_row_jam_race.das`,
the row jam's `PERF_LEDGER.md` entry; `row_jam = true` on the `[llvm_code]` stub jams again); nv4 folds a strip - sixteen a superblock against q40's eight - off its sixteen scales
decoded once as vectors and picked by shuffle a block. One body
a family: q8 (int8 rows, one f16 scale a block), the per-32 and MXFP4 planes (16 nibble bytes a block
as 32 codes in k order, the codebook formats' weights off the LUT under the signed lattice, the 5-bit
codes under the unsigned one with q51's m term on the activation sum), and the superblock formats
(a superblock an iteration, its eight blocks' codes off the device's k / k+16 nibble pairing with the
fifth and high bits deposited from the block's qh bytes and the 2-bit lanes off their byte fields,
each vector's dot widened through the sub-scales a quarter, the min and offset terms as scalars off
the Q8_K sums, one fold a superblock under d times the activation scale). On the 256-bit x64 arm k4 and k5 take
the pair form (`rm_pair_codes`, `rm_sb_min_vec`): one 32-byte load holds two blocks, its low nibbles both blocks'
low strips and its high nibbles their high strips (the k / k+16 pairing), the activation's two blocks re-split the
same way, the two strips' pmaddubsw pair sums added in i16 and widened under the pair's sub-scales by one pmaddwd,
k5's fifth bits off the two blocks' eight qh bytes in one spread, and the min term one vector a superblock - the
eight mins times the eight 32-blocks' activation sums off the Q8_K strip sums - folded under -dmin x xs; the
other widths and NEON keep the block-a-vector walk. The pair form halves a core's time over the 35B's K-quant
planes (k4 80 -> 48 us, k5 100 -> 64 at n=2048 d=768, one thread); the token's host step at sixteen lanes streams a
layer's picks at the box's DRAM ceiling either way, so what the rail still trails the planar path by on a token is its
per-layer structure (the dasCUDA host-experts rows of `PERF_LEDGER.md`). Each format is its own
`[tune]` family (`<fmt>q8_rm_gemv_gen`, `dasllama_math_gen.das`, the shared perms over the maddubs,
vpdpbusd and sdot lattices, a scalar decode of the device layout the reference) with its four-token
tile as the companion (`<fmt>q8_rm_tile4_gen`: every row against four activation rows at a row's
stride, the weights decoded once a block) and a witness companion (`<fmt>q8_rm_witness_gen`), raced
by the generator half on the streamed shape alone
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race`). At the rail's many-lane,
DRAM-bound regime the row-major shape costs what the interleaved one does (llama.cpp's row-major
kernels read even with ours there, `harness/kernel_ladder.sh` at sixteen lanes), and the tile reads
an expert's planes once a tile where a GEMV a row read them once a row. The five codebook-grid
formats (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs) take the superblock body with the block's weights gathered
off the module's grid table (`rm_grid_block`: the iq3 formats eight words of four magnitudes, the iq2
formats four word pairs of eight) and its signs - the plane's own sign bytes, or the ksigns rows of
its 7-bit codes - applied as one masked negate, the sub-scales 1 + 2 x the two-word row's fields or
the per-16 strips after d. The 1-bit grid members (iq1s, iq1m) take the same body over their own planes, their
weights unsigned eighths under an offset term (`ARCHITECTURE_ENGINE_FORMATS_IQ1.md#iq1-delta-fold`).

The rail admits a format by name (`rm_dev_serves`, `dasllama_blocks.das`): the members a discrete home stamps,
which are the members that have a device layout, each with a row-major family that reads it. Vulkan and CUDA
read one layout of a member, so the CPU's kernel serves a stack whichever home pinned it.
`check_rail_names_stamped_members` (`REVIEW.das`) holds the names in that function equal to the `KqFmt` members
off `KQ_DISCRETE_UNSTAMPED` (`dasllama_kqformat.das`), in both directions; it licenses none.
