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

### Classic prefill scores through the decode's own dot {#prefill-decode-same-score-dot}

Classic prefill quantizes each query row and runs its scores through `kv_score` - the dot a cached position takes at decode - so a prefix served by one prefill pass and the same prefix served a token at a time agree bit for bit under the block codecs (q8_0, tq4). Flash prefill tiles with online softmax and reorders the sum, so it holds to a tolerance instead.

### A kernel a worker lambda invokes is not `private` {#kernel-visibility-lifted-workers}

The tier kernels run from lambdas the job dispatch lifts out of the function that wrote them, and a lifted body reaches its callee by module-scope name, so every kernel a `parallel_for` body or a registered kernel pointer reaches is declared without `private` - the row-range cores, the groupN kernels and their wscale_f16 twins among them. The same reach is why a backend with no native groupN tier registers the portable one: a `KernelBackend` slot is never null.

### A grp<mr> repack is two interleaves {#grp-repack-interleaves}

Every format's repack is made of the same two moves over a row of units - a 256-weight superblock, or a 32-block - of `ubytes`: the quant-plane interleave, which stacks mr rows per group and lands column c of unit u in row r at `[g][u][c][r][colw]`, and the scale-plane interleave, which is field-major, `[g][u][field][i][r][width]`, the fields in destination order with their source offsets. Tail rows (`d % mr`) stay disk-order and the row-major dots serve them.

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
its own, which the int8 tile also answers. The walk hands sub-32 token tails and a group tail
short of the tile to the gemv rows core, which rides busd512 like every amx companion. The
panel amortizes over the token block, so the walk floors its block at 512 tokens under the
per-box L2 clamp whatever `q8_token_block` says, and it chunks its groups in whole tile units
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

### A hot leaf is instantiated in its caller's JIT partition {#jit-partition-inlining}

The split-module JIT inlines within one partition only, so a leaf a hot loop calls is written to instantiate in the caller's: a generic over its operand (`iq_grid_octet`), or a plain function stamped beside the codecs it drives (`kq_transcode_units`). A call into another module's function inside a `[tune]` loop body blocks the loop's vectorization outright - `dot_bf16` spells its bf16 widen as the shift for that reason. The same rule runs the other way for the leaves themselves - the inliner's decisions over a leaf follow its call-site count, which is why the run-time-format `dot_kq` stamp lives in the test fixture `tests/_kq_dot.das` and not beside the sixteen tag overloads in `dasllama_math_default.das`.

### The deltanet per-token core is one code path for every caller {#dn-token-core-shared}

The Gated-DeltaNet recurrence is written once per token, in three pieces: the prelude (the beta and g
transforms, the causal conv against the session's history, SiLU, the q/k L2-norm and the q
pre-scale), the per-(token, v-head) delta rule against that session's state, and the z-gated
out-norm epilogue. One token of a decode step, one position of a prefill and one row of a
B-session batched step all walk those same three pieces, so no caller can round apart from
another; what differs is only the lane split over (token, head) and the projections around them.
