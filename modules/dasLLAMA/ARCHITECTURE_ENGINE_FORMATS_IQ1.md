# dasLLAMA Architecture - the 1-bit grid members

Companion to `ARCHITECTURE_ENGINE_FORMATS.md`; a section is cited by its anchor. This document carries the
`iq1s` and `iq1m` superblock members - llama.cpp's IQ1_S and IQ1_M: how their planes hold the disk bytes
(`#iq1-planes`), the one grid table both read (`#iq1-grid`), how the grid's constant offset folds into
the dot (`#iq1-delta-fold`), how the CPU serves their rows (`#iq1-grp-cpu`), and how the discrete GPU homes read
them (`#iq1-device-row`).

### The iq1s and iq1m planes are the disk bytes in two pieces {#iq1-planes}

An IQ1_S superblock is 50 bytes: an f16 `d`, 32 `qs` bytes - the low eight bits of 32 grid indices, one
an octet of weights - and eight little-endian `qh` words, one a 32-weight block. Bits 0-11 of block
`ib`'s word carry the high three bits of its four octets' 11-bit indices (octet `l` at bit `3l`), bits
12-14 its scale field `s`, bit 15 the sign of its delta. A weight is `d x (2s + 1) x (g + delta)`, `g`
the grid value in {-1, 0, +1} and `delta` +-1/8.

An IQ1_M superblock is 56 bytes: the same 32 `qs` bytes, 16 `qh` bytes - a nibble an octet, its low
three bits the index high bits and bit 3 its delta sign (octet `l` of block `ib` in byte `2ib + l / 2`,
the high nibble for odd `l`) - and four little-endian scale words. Word `k` holds the scale fields of
half-blocks `4k .. 4k + 3`, three bits each from bit 0, and its top nibble is nibble `k` of the f16 `d`
(`iq1m_d_bits`). A weight is `d x (2s + 1) x (g + delta)` with `s` its 16-weight half-block's field and
`delta` its octet's.

Both members keep those bytes and nothing else: the quant plane is the 32 `qs` bytes then the `qh`
bytes (48 a superblock, `IQ1S_QSB` / `IQ1M_QSB`), the scale plane IQ1_S's `d` (2 bytes) or IQ1_M's four
scale words (8 bytes) - 50 and 56 bytes a superblock, the disk block's. The transcode is two copies
and decodes nothing. The planes are the device planes too (`kq_dev_is_plane`): they stay in disk order
at every interleave - `repack_grp` moves no byte - the device gather copies them verbatim and Metal's
scale form keeps them as they are. Their kernel and stream ids are 13 and 16 - an
i-quant's bit width times ten plus a variant digit, S taking 3 as IQ2_S and IQ3_S do, M a digit of its
own. The Vulkan and CUDA homes read the planes in place (`#iq1-device-row`); Metal's format gate (`kq_fmt_gpu_supported`)
names neither, so its loads leave the planes to the CPU.

### Both members read one packed grid table {#iq1-grid}

The two formats share llama.cpp's 2048-entry `iq1s_grid`: eight ternary weights an entry. The engine
holds it once, as `iq1s_grid()` - two bits a weight (the code `g + 1`, 0 to 2), 16 bits an entry, two
entries a word, 4 KB in all - returned as a literal so a worker lane reads it from its own frame
(`IQ1S_GRID` is the main-context copy). Every reader decodes the entry where it reads it:
`iq1_grid_entry` takes the 16 bits out, the portable dots and the dequant take a weight's two bits
from them, and the emitter bakes the table into each module it emits as `[2048 x i64]` of the bytes
`8 x code + 1 + delta` - the eight-byte gather the reference's x86 kernels run, its codes already in
eighths and the delta folded in: `[4096 x i64]`, index bit 11 the delta's sign, so a lookup carries the
delta with no vector work.
The device table is the 4 KB form: the discrete homes' grid buffer holds it at `KQ_GRID_IQ1`
(`kq_grid_dev`, `ARCHITECTURE_GPU_VULKAN.md#kq-gemv-grid-buffer`), and the tiles stage it from its per-word accessor
(`iq1s_grid_word`); no wider copy of it exists on a device.

### The delta folds into the weight as eighths {#iq1-delta-fold}

`delta` is an eighth, so eight times a weight over its scale, `8g + delta`, is an integer in
{-9, -7, -1, 1, 7, 9}. The portable dots sum `(8g + delta) x x` exactly in int - one helper,
`grid1_octet_dot`, for both members, each passing its delta sign: IQ1_S its block's, IQ1_M its
octet's - and fold once a superblock under `d x x_scale / 8`, which equals the reference's
`sumi + 1/8 x sumi1` exactly in float. The tile's panel carries the weight as that signed byte,
read by the grid members' sign-trick dots with no offset term, its `d` multiplied by 1/8 at the fold.
The row-major body - the device rows and the grp GEMV - moves the weight to an unsigned byte for the
u8 x i8 dots instead: `u = 8 x code + 1 + delta`, 0 to 18, the table byte itself - and takes the 9 it
added back as the offset term, `9 x` the strip's Q8_K activation sum under the strip's scale. The strip is the scale's unit on
both members: IQ1_S's block scale covers its two strips, IQ1_M's half-block scale one, so neither
needs a sum the activation does not already carry.

### The CPU tile runs iq2xs's panel body over a decoded panel {#iq1-grp-cpu}

The grp tile reads a 1-bit group of `mr` disk-order rows through iq2xs's panel body. Their scales sit
in the quant plane (IQ1_S) or in fields of packed words (IQ1_M), so the body cannot read them where
they lie: the tile reads a panel the walk decodes once a group and token block (`unpack_kq_tile_panel`)
- the weights as signed eighths in the grid members' panel form, then the scale rows in iq2xs's grp
scale form: the f16 `d` undivided at `2r`, two pad bytes, the sixteen strip scales `2s + 1` from
`4mr`. The decode is emitted (`grid1_unpack_gen`, a tile companion; `unpack_grid1_panel` is its
reference body): a row's blocks through the row-major body's own block decode less 9, the strips and
`d` off its own scale helpers. The walk repeats it every token block, so its cost counts once a block
in the prefill pass. The decoded form is working memory; the planes keep the disk bytes. The planes
stay in disk order because the GEMV reads them there: the GEMV slots and the grp GEMV companion are the
row-major body (`iq1sq8_rm_gemv_gen`, `iq1mq8_rm_gemv_gen`, `rm_kq_emit`), which decodes a row's blocks
straight into its dot. Read off a grp interleave the same body loses up to 40% at one lane on rizen (the dated
`PERF_LEDGER.md` entry on the iq1s and iq1m grp tile) - a row's
48 bytes a superblock spread over twelve cache lines - and a panel gathered a superblock costs more
than the decode the dot runs on. The reference rows, GEMV and tile alike, are the panel's
dot under the `d` eighth (`grid1_grp_row_dot`), so the gates hold the reference tile to the reference
GEMV bit for bit. The members have no AMX leg: their tile-form and AMX companions decline
(`iq1sq8_tileform_gen` answers 0), and the bf16 panel companion is the row dequant (`kq_panel_rows_bf16`), the
reference.

### The discrete homes read the planes in place off one grid {#iq1-device-row}

The Vulkan and CUDA homes serve both members off the bytes the CPU reads. A device quant row is the 48-byte quant plane,
twelve words a superblock (`IQ1_QWORDS`), and a device scale row the plane's own - IQ1_S's two-byte d, two superblocks a
word and the half by parity (`q40_d_of`), IQ1_M's four scale words as two (`IQ1M_SWORDS`) - so a device plane holds the disk
block's 50 or 56 bytes a superblock and no wider form: only an IQ1_S scale slab's end rounds to a whole word, and the gather
copies both rows verbatim. Both members lay a 32-block out alike: its four `qs` bytes are quant word `b`, its 16 `qh` bits
the half `b % 2` of word `IQ1_QH_WORD + b / 2`. They differ in where three things come from - IQ1_S takes an octet's index
high bits three a field from those bits, the block's delta sign from bit 15 and its scale from bits 12-14; IQ1_M takes the
high bits and the delta sign from the octet's nibble and the half-block scale and the d from the two scale words
(`iq1m_d` and `iq1m_sc`, over the CPU's `iq1m_d_bits` and `iq1m_half_scale`) - and one template constant, `IQ1M`, picks
the source on every template the members stamp.

The grid reaches a kernel the way the grid members' codebooks do: the decode GEMV family stages the 4 KB grid from the grid
buffer at `KQ_GRID_IQ1` into workgroup memory (`GRID`), the batch tile and the cm2 and KHR tiles stage it from its accessor
(`iq1s_grid_word`), and every kernel takes an entry's codes off its 16-bit half. On the RTX 5060 Ti that stage is level
with or ahead of the grid read off its buffer, of the reference's nibble form (an entry a word, `grid1_nibbles`) expanded
into 8 KB of workgroup memory - which leads at 14336 x 4096 and trails at the 1B's rows, its stage paid a workgroup - and
of 8, 16 or 32 lanes a row on both homes (`PERF_LEDGER.md`, the iq1s and iq1m entry); those forms stay template constants
of `Iq1GemvT` (`G1BUF`, `GRIDX` with `G1NIB`) raced by `harness/vk_gemv_probe.das -- iq1`. The GEMV,
its N-column leaves, the token-table gather and the batch tile carry the weights in eighths, as the CPU does: signed int8
lanes `8 x (code - 1) +- 1` (`grid1_eighths`: 8 x code plus 0x79 or 0x77 a byte, which carries into no next byte, its top
bit flipped), so the fold is the scale over 8 against `sdot4` and reads no activation sum - IQ1_S's delta as a term of
its own against the half's activation sum reads level (`G1SEP`) and IQ1_M's, an octet's, has no such sum; the cm2 decodes
and the KHR stage compute `dl x (code - 1 + delta)` in f32 and round it to f16.

`Iq1GemvT` gives both homes the decode GEMV, its N and four-column N leaves and the token-table gather; `Iq1sCm2T` and
`Iq1mCm2T`, two leaves of one body (`Iq1Cm2BaseT`), give both homes the cm2 l, m and s tiles and a 32-deep e column - the
grid members' k step - and Vulkan the KHR tile with its rotated alternate; the Vulkan batch tiles `KqBatchIq1s` and
`KqBatchIq1m` derive iq2's two-plane split fold. The members have no int8 window tiles (`MMQ_SKIP`), no CUDA rows tensor
leaf (`CUD_ROWS_MMA_SKIP`), no fused gate-up or summed-down GEMV (`GEMV_SHELL_SKIP`) and no rotated N leaves
(`GEMV_N_ROT`: their 48-byte rows never sit a power of two apart), and take the grid side of the lanes rule
(`gemv_grid_fmt`).

The one-copy rare rail reads a streamed layer's experts off the same device rows on the CPU through the row-major family
(`#iq1-grp-cpu`, admitted by `rm_dev_serves`), and the host twin stamps `Iq1GemvT` as `HostKqGemvIq1s` and
`HostKqGemvIq1m`, its workgroup grid filled from `IQ1S_GRID`.
