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
`IMAGE_VERSION` 28 is this layout.

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
(`ARCHITECTURE_MEASUREMENT.md` sec.2.26) races.

### 2.42 A CPU tier selects on the TARGET, not the host {#cpu-tier-target-select}

The arm64 SDOT tier registers its backends only under the JIT and only for an arm64 TARGET - the
artifact's architecture, never the running host's. Off the JIT the `sdot4` family runs its scalar
fallback bodies, which are slower than the portable `dot_q8q8` the vectorizer handles, so the
portable tier stays selected wherever hardware SDOT is not emitted.

The portable tier picks its dot form on the target at compile time. On wasm SIMD128 the
auto-vectorized template dot is the slow form - the ISA carries no int8 dot for LLVM to find,
while the `idot4` builtin lowers there to the ISA's own widening multiply-adds - so a wasm target
takes `dot_q8q8_idot4_ps`. Every other target keeps the template.
