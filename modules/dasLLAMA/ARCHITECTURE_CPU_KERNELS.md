# dasLLAMA architecture - the CPU kernel planes and decodes

Companion of `ARCHITECTURE.md` (contract: `../../ARCHITECTURE_COMMON.md`). Section 2 here continues
the mechanism numbering; each section is cited by the code that embodies it.

## 2. Mechanisms

### 2.22 The k3 and k6 planes are packed per sub-block {#kq-subblock-planes}

A k6 grp<mr> plane's qh columns `2blk` and `2blk + 1` carry one sub-block's four `j` sites, each as
a 2-bit field at bit `2j`; the disk byte `h*32 + half*16 + j*4 + t` feeds sub-blocks `4h..` at bit
`2b`. k3 packs the same way - qs columns `2blk + half` carry the sub-block's four `j` sites at `2j`,
and the hmask column `blk` carries its eight sites at bit `s` (lo at `j`, hi at `4 + j`). One
sub-block's decode then costs two loads for k6 and three for k3, and nothing loaded lives past it;
the row-interleaved disk order cost one load per `j`. The layout is CPU-flavor: the `.dlim` a box
bakes is for the hardware that runs it, so a CPU plane owes nothing to the GPU tiers' shapes.
The layout is part of what `IMAGE_VERSION` stamps.

### 2.23 A grid format's CPU gemv decodes as a panel or as row groups {#grid-decode-forms}

Five formats (iq3s, iq3xxs, iq2s, iq2xs, iq2xxs) have two gemv decode forms. The PANEL form gathers
a superblock into an alloca panel first and reads packed positions through one dword load per
4-byte column, the four positions of a column sharing the load. The ROW-GROUP form composes a
weight-width vector straight from the grid words - width/64 rows x 8 weights, one u64 grid entry
per iq2 row - and reverts to byte loads, because the column dword read only pays inside the panel.
The sdot lattice always takes row groups; on x86 the panel's latency chain does not scale with the
core, so `x86-vnni512` takes row groups for iq2xxs and `x86-amx` for iq2xxs and iq3xxs, everything
else the panel. A VBMI seat (sec.2.24) takes row groups unconditionally.

### 2.24 The VBMI symbol lattice {#vbmi-lattice}

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
(`ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md` sec.2.26) races.

### 2.42 A CPU tier selects on the TARGET, not the host {#cpu-tier-target-select}

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

### 2.54 Classic prefill scores through the decode's own dot {#prefill-decode-same-score-dot}

Classic prefill quantizes each query row and runs its scores through `kv_score` - the dot a cached position takes at decode - so a prefix served by one prefill pass and the same prefix served a token at a time agree bit for bit under the block codecs (q8_0, tq4). Flash prefill tiles with online softmax and reorders the sum, so it holds to a tolerance instead.

### 2.55 A kernel a worker lambda invokes is not `private` {#kernel-visibility-lifted-workers}

The tier kernels run from lambdas the job dispatch lifts out of the function that wrote them, and a lifted body reaches its callee by module-scope name, so every kernel a `parallel_for` body or a registered kernel pointer reaches is declared without `private` - the row-range cores, the groupN kernels and their wscale_f16 twins among them. The same reach is why a backend with no native groupN tier registers the portable one: a `KernelBackend` slot is never null.

### 2.56 A grp<mr> repack is two interleaves {#grp-repack-interleaves}

Every format's repack is made of the same two moves over a row of units - a 256-weight superblock, or a 32-block - of `ubytes`: the quant-plane interleave, which stacks mr rows per group and lands column c of unit u in row r at `[g][u][c][r][colw]`, and the scale-plane interleave, which is field-major, `[g][u][field][i][r][width]`, the fields in destination order with their source offsets. Tail rows (`d % mr`) stay disk-order and the row-major dots serve them.

### 2.57 A hot leaf is instantiated in its caller's JIT partition {#jit-partition-inlining}

The split-module JIT inlines within one partition only, so a leaf a hot loop calls is written to instantiate in the caller's: a generic over its operand (`iq_grid_octet`), or a plain function stamped beside the codecs it drives (`kq_transcode_units`). A call into another module's function inside a `[tune]` loop body blocks the loop's vectorization outright - `dot_bf16` spells its bf16 widen as the shift for that reason. The same rule runs the other way for the leaves themselves - the inliner's decisions over a leaf follow its call-site count, which is why the run-time-format `dot_kq` stamp lives in the test fixture `tests/_kq_dot.das` and not beside the sixteen tag overloads in `dasllama_math_default.das`.

### 2.58 The deltanet per-token core is one code path for every caller {#dn-token-core-shared}

The Gated-DeltaNet recurrence is written once per token, in three pieces: the prelude (the beta and g
transforms, the causal conv against the session's history, SiLU, the q/k L2-norm and the q
pre-scale), the per-(token, v-head) delta rule against that session's state, and the z-gated
out-norm epilogue. One token of a decode step, one position of a prefill and one row of a
B-session batched step all walk those same three pieces, so no caller can round apart from
another; what differs is only the lane split over (token, head) and the projections around them.
