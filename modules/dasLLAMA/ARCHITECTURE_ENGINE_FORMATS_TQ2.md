# dasLLAMA Architecture - the ternary members

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document carries
the two ternary superblock members: how tq2's planes hold TQ2_0's disk bytes and the k2 bodies serve them,
and how tq1's planes hold TQ1_0's and ride tq2's walk, fold and device row.

### The tq2 planes are TQ2_0's disk bytes, read by k2's bodies under constants {#tq2-k2-lattice}

A TQ2_0 disk block holds 256 weights in 66 bytes: 64 qs bytes, then an f16 d. Within each 128-weight
half, byte m of its 32 holds elements m, m + 32, m + 64 and m + 96 at bit shifts 0, 2, 4 and 6, and a
weight is d x (q - 1) for a code q of 0, 1 or 2. That is Q2_K's quant layout exactly: TQ2_0 is Q2_K's
block with every sub-scale 1, every min 1 and dmin = d.

The tq2 member (`KqFmt.tq2`, kernel id and stream code 22) keeps the disk bytes and no more. Its quant
plane is the 64 qs bytes verbatim (`TQ2_QSB`); its scale plane is the f16 d alone (`TQ2_SSB`, 2 bytes a
superblock where k2's decoded row takes 20). The grp<mr> repack lays the qs bytes as k2's sixteen
four-byte columns and the scale row as `[mr x f16 d]`.

Each CPU body of the 2-bit lattice is one walk over the member's tag: the disk-order dot `k2l_dot`, the
grp row dot `k2l_grp_row_dot`, the grp row dequant and the plane dequant. The tag's overloads supply what
the two members differ in - the scale-row reads (`k2l_row_pair` / `k2l_row_head` on the disk-order row,
`k2l_grp_pair` / `k2l_grp_head` on the grp row, `k2l_pair` / `k2l_head` in the plane dequant) and the
superblock fold (`k2l_fold`). tq2's pair is `TQ2_K2_PAIR` (sub-scale 1, min `TQ2_ZERO`) and its head
reads its d as both d and dmin. k2 folds its integer sum on d and its min term on dmin; tq2 folds once a
superblock, d x d8 x (sum(q x) - sum(x)), the activation sum taken off the Q8_K form's per-16 sums, as
`ggml_vec_dot_tq2_0_q8_K` folds. The plane dequant's d x q - d equals `dequantize_row_tq2_0`'s
(q - 1) x d bit for bit: q is at most 3, so every product is exact.

The generated tier's block body is k2's in `emit_block_kqv2` under the same constants: the sub-scale and
min splats fold the multiplies away, the d row is the one f16 row, and the superblock fold is k6's at
offset `TQ2_ZERO`. The `tq2q8_tile_gen` family carries k2's grid and class-profile rows.

Metal's `kq_fmt_gpu_supported` refuses tq2: its loads leave the planes to the CPU.

### The discrete homes read the plane bytes, through k2's templates {#tq2-device-row}

The Vulkan and CUDA homes serve tq2 off the same bytes the CPU reads. The device quant row is the 64 qs
bytes (`kq_dev_qsb`) and the device scale row is the plane's f16 d (`kq_dev_ssb` answers `TQ2_SSB`), so a
device plane holds the disk block's 66 bytes a superblock. The gather (`kq_gather_rows_dev`) copies both
rows verbatim from disk order and takes a grp row's `[mr x f16 d]` back to one d a superblock. A scale
plane is half a word a superblock: a kernel reads superblock b's d from word b / 2, the half that b's
parity picks (`q40_d_of`), and a slab, an arena or a fixture plane rounds to a whole word at its end alone.

Every tq2 stamp is a k2 template under the `TQ2` template constant, whose arm swaps the scale read and
folds sub-scale 1, min 1 and dmin = d: `K2GemvT` gives the decode GEMV, its N and four-column N leaves and
the token-table gather on both homes and the rotated N leaves on Vulkan; `K2Cm2T` gives the cm2 l, m and s
tiles on both homes (the e column is the m stamp, `KQ_CM2E_ALIASES_M`) and the KHR tile with its rotated
alternate on Vulkan; the Vulkan batch tile `KqBatchTq2` derives `KqBatchK2` and stages the d into all four
of its scale planes. tq2 has no int8 window tiles (`MMQ_SKIP`), no CUDA rows tensor leaf
(`CUD_ROWS_MMA_SKIP`) and no fused gate-up or summed down GEMV (`GEMV_SHELL_SKIP`), as k2 has none, and
takes the k-lattice side of the lanes rule (`gemv_lanes_per_row`).

The one-copy rare rail reads a streamed layer's experts off that device layout on the CPU: the
row-major family `tq2q8_rm_gemv_gen` (k2's arm of `rm_kq_gen` under sub-scale 1, min 1 and dmin = d, its
reference `rm_kq_dev_row`) is admitted by `rm_dev_serves`, and the host twin stamps `K2GemvT` under `TQ2`
as `HostKqGemvTq2`.

### The tq1 planes are TQ1_0's disk bytes, read as trits of four-byte columns {#tq1-trit-columns}

A TQ1_0 disk block holds 256 weights in 54 bytes: 48 qs bytes, 4 qh bytes, then an f16 d. Each qs byte
packs five ternary digits and each qh byte four, in a fixed-point base-3 form: digit n of byte b is
((b x 3^n wrapped to 8 bits) x 3) >> 8, a code q of 0, 1 or 2, and a weight is d x (q - 1). The first 32
qs bytes carry elements n x 32 + m (digit n of byte m), the last 16 carry 160 + n x 16 + m, and the qh
bytes carry 240 + n x 4 + j.

The tq1 member (`KqFmt.tq1`, kernel id and stream code 21) keeps the disk bytes and no more: its quant plane
is the 48 qs bytes then the 4 qh bytes verbatim (`TQ1_QSB`, 52 bytes a superblock) and its scale plane the
f16 d alone (`TQ1_SSB`), tq2's row. No plane expands a digit to a wider code. The grp<mr> repack lays the
quant row as thirteen four-byte columns and the scale row as `[mr x f16 d]`, one overload with tq2's.

The element order makes every weight dword one column's trit: elements 4w .. 4w + 3 are trit n of the four
bytes of one column, the 32-weight blocks 0 to 4 reading trit blk of columns 0 to 7, blocks 5 to 7 trits 0 to
4 of columns 8 to 11, and the last half the qh column 12 (`tq1_dword_trit`). Every CPU body reads the plane
through that map and `tq1_trit`, the one digit extraction: the row dot over an mr-row group's columns, which
is the disk-order dot at mr 1 and the grp row dot at the group's mr, and the superblock dequant, which is the
plane dequant at mr 1 and the grp row dequant at the group's mr - `dequantize_row_tq1_0`'s float order, so the
plane dequant is the file dequant bit for bit. tq1 folds as tq2 does, once a superblock: d x d8 x
(sum(q x) - sum(x)), the sum(x) off the Q8_K form's per-16 sums.

The generated tier's block body is tq2's in `emit_block_kqv2` with each quant vector a column's trit: the
multiply by 3^n is the NEON byte multiply or the x64 adds and masked word shifts of `ggml_vec_dot_tq1_0_q8_K`,
and the times-three high part the NEON halving add of q and q >> 1 or the x64 rounding halving adds over q - 1
saturated, then >> 6. The x64 halving adds and the saturated subtract are the target's own intrinsics
(VPAVGB, VPSUBUSB): spelled as widened adds, the combiner folds the pair into a 16-bit multiply by three and the
kernel runs at a third of the speed. Two more x64 spellings race in the grid (`trit`): the word multiply-high,
where (b << 8) x 3^n keeps b x 3^n mod 256 in a word's high byte and VPMULHUW by 3 leaves the digit, and two
nibble tables through VPSHUFB for the multiply by 3^n ahead of the halving adds. Blocks 5 and 6 read one column
twice and share its load. Every sub-scale is 1, so the x64 maddubs body runs each i16 chain across the whole
superblock (32 pair sums of at most 512 a chain) and reads the activation sums once a superblock, the sixteen
per-16 sums added, where tq2 folds per block. The `tq1q8_tile_gen` family carries tq2's grid, the trit rows and
class-profile rows.

Metal's `kq_fmt_gpu_supported` refuses tq1: its loads leave the planes to the CPU.

### The discrete homes read tq1's plane bytes, the trits taken in the kernel {#tq1-device-row}

The Vulkan and CUDA homes serve tq1 off the bytes the CPU reads. The device quant row is the 52 quant bytes, thirteen words
(`TQ1_QWORDS`), and the device scale row tq2's two-byte d, so a device plane holds the disk block's 54 bytes a superblock and
no wider form: the gather copies both rows verbatim from disk order and takes a grp row's thirteen four-byte columns and
`[mr x f16 d]` back to one superblock. A quant row is on no 16-byte boundary, so every tq1 kernel reads it as words.

A kernel takes a weight dword's trits the CPU's way (`tq1_dword_trit`): the dword's column word, then digit n of each of
its four bytes in one 32-bit word (`tq1_word`) - the even and the odd bytes as 16-bit lanes, each lane's byte times 3^n
kept to 8 bits, then times three over 256. A staged table of each byte's five digits loses to that multiply on the RTX
5060 Ti: the decode GEMV reads 1.43x slower over the 2-bit lattice's shapes (`PERF_LEDGER.md`, the tq1 races; the twin
`ProbeTq1GemvLut` in `harness/vk_gemv_probe.das`), so no tq1 kernel stages one. The GEMV takes a
32-block's two halves' column words and digits once a block rather than a dword at a time.

Every tq1 stamp derives tq2's templates with the trits as the block term: `K2GemvT` under `TQ2` and `TQ1` gives the decode
GEMV, its N and four-column N leaves and the token-table gather on both homes; `Tq1Cm2T` (`K2Cm2BaseT` over the 52-byte
block, under `TQ1`, whose arm reads tq2's scale row itself) gives the cm2 l, m and s tiles on both homes (the e column is the m stamp,
`KQ_CM2E_ALIASES_M`) and the KHR tile with its rotated alternate on Vulkan; the Vulkan batch tile `KqBatchTq1` derives
`KqBatchTq2`. tq1 has no int8 window tiles (`MMQ_SKIP`), no CUDA rows tensor leaf (`CUD_ROWS_MMA_SKIP`), no fused gate-up
or summed down GEMV (`GEMV_SHELL_SKIP`) and no rotated N leaves (`GEMV_N_ROT`: its 52-byte rows never sit a power of two
apart), and takes the k-lattice side of the lanes rule (`gemv_lanes_per_row`).

The one-copy rare rail reads a streamed layer's experts off the same device layout on the CPU: the row-major family
`tq1q8_rm_gemv_gen` (tq2's arm of `rm_kq_gen` with each 32-block's codes the trits of its dwords' bytes, `rm_tq1_block`, its
reference `rm_kq_dev_row`) is admitted by `rm_dev_serves`, and the host twin stamps `K2GemvT` under `TQ2` and `TQ1` as
`HostKqGemvTq1`.
