# dasLLAMA Architecture - the Metal prefill window's device forms

Companion to `ARCHITECTURE_GPU_PREFILL.md`; a section is cited by its anchor. This document
carries the third form of the prefill attention - the pair of device-direct tensor GEMMs around a
row softmax written in place - the mirror-fed window that pair reads its keys from and the
adoption of a released session's mirror, the recurrent scan over half rows, and how a kernel's
hot loads are addressed. The score slab the attention pair shares with the other two forms, and their
kernels, stay in `ARCHITECTURE_GPU_PREFILL.md#prefill-attn-slab`.

### A hot load is a pointer and a constant offset {#kernel-load-addressing}

A kernel loop that reads `buf[base + i]` under a `uint` index asks the Metal compiler for a 32-bit
sum a load: unsigned wrap is defined, so the sums do not fold into one address and adjacent loads
do not merge into a wide one. Taking the address once - `let p = unsafe(addr(buf[base]))` - and
reading `p[i]` at a constant `i` gives it a base and constant offsets. The recurrent scan reads its
64 row values a token this way (`harness/dn_scan_race.das` holds both forms), and the K-quant
stamps their quant words a staged run. Every fully unrolled loop of a Metal kernel reads this way:
`REVIEW.das`'s `check_metal_kernel_load_addressing` flags a bound buffer indexed in such a loop by
a run-time base plus a constant of the loop variable, and licenses no names. A load whose offset
steps with a run-time loop gains nothing from the pointer, its offset no constant. A store keeps
the indexed form - the kernel lens lowers no pointer store.

### The recurrent scan {#prefill-dn-scan}

The gated delta rule walks a window's tokens in order inside one dispatch, a simdgroup holding
eight state columns in registers, so the walk is bound by what it reads a token: 32 key and 32
query values a lane. Four lanes share a state column, each holding a quarter of its rows (32 at a
head of 128) in registers; a column's reductions are a pairwise tree inside each lane, then two
lane shuffles. Three things follow.

- **The window's stamp reads half rows** (`MetalDnScanH`). The conv rows convert to halves once a
  layer and the walk reads half the bytes. The state stays f32. The decode step's stamp
  (`MetalDnScan`) keeps f32 rows: one row is no stream.
- **The gates are prepared, not walked.** `MetalDnPrep` writes beta and the decay over the raw
  projections once a (position, value head); the decode stamp computes them in every lane of
  every state column, where one row makes that free.
- **Two simdgroups a threadgroup.** The walk shares nothing across simdgroups; four a threadgroup
  read slower than two or one.

### The device attention pair {#prefill-attn-device}

The staged forms copy K, P and V into threadgroup memory a tile at a time, and that copy - not
the multiply - is what a window on a long context pays for (`harness/attn_depth_race.das` holds
both forms). The device pair hands `matmul2d` device tensors of half on both sides - the kernel DSL's `devab`
steps, which take a row stride and a run-time reduction width - so nothing is staged and no barrier runs.
It serves under the `attn_dev` crown. Over the f32 K/V panels it asks both attention classes on
the 64 lattice (`mmattn`) and a query panel of 256 rows or more; the mirror-fed window asks the 32
lattice and no row floor, since its keys are the mirror's.

Five dispatches a layer replace the trio's three:

- **The K/V twin passes** (`enc_kv_twin`, `MetalHalfRows` twice) convert the layer's K and V
  panels to halves over all `nk64` rows, the softmax scale folded into K. The twins are one
  scratch pair every layer rewrites: the K/V panels are themselves rebuilt each window, so no
  twin outlives its layer.
- **QK** (`MetalAttnQKDev`) multiplies the Q twin by the K twin into the score slab, a tile of
  query rows by 64 keys, skipping the tiles the causal and window rules empty.
- **The row softmax** (`MetalAttnPExp`) reads a row's maximum, then writes `exp(s - max)` over
  the scores and the row's reciprocal sum into the stat plane. The slab holds the unnormalized
  weights: their largest is 1, so a small weight keeps more of the half's mantissa than the
  normalized one the staged forms round.
- **AV** (`MetalAttnAVDev`) multiplies the slab's rows by the V twin in its natural layout and
  scales each output row by its reciprocal sum before the store. A tile is 64 of the head's
  columns, and the grid carries the head's whole tiles (`ntile`, never a ceiling: a tile past the
  head would write the next head's columns); a head off the 64 lattice (phi-3's 96) takes its last
  32 columns through the TAIL stamp (`MetalAttnAVDevTail`), one 32-column tile at the head's
  last whole-64 boundary. QK needs no tail: its reduction width is the head, a run-time extent.

**The reduction span is live rows only.** An AV tile reduces over `[pf_av_lo, pf_av_hi)`: from
the first key the tile's first row's window admits to the tile's causal reach, clamped to `npos`.
A V row past `npos` is stale pool bytes, and a zero weight times a non-finite value is not zero,
so the span - not a zero guard - keeps pad rows out.

**The softmax pass writes the widest span any AV tile of the row can read.** It walks the row's
128-row tile's span and writes zero outside the row's own `[jlo, cnt)`. A 32-row tile's span is a
part of the 128-row tile's, so one softmax stamp serves both AV heights.

**Two tile heights, picked by the query panel.** The 128-row stamps serve a panel that divides
by 128, the 32-row stamps any other; a panel pads to 32 rows, never to 128, and a 128-row tile
over a shorter panel would read and write past it.

### The mirror-fed window {#prefill-kv-mirror}

A host-cached session with f16 K and V keeps no K/V panel past the window's own rows. Each layer's
K and V rows convert to halves straight into the session's KV mirror (`MetalHalfRows`, at the
window's first row of the layer's slab), and the device pair reads every key there, the slab bound
at its byte offset in the arena. It serves at any row count: the pair's 256-row floor prices the
whole-panel twin pass, and this form has none. The mirror's K carries no softmax scale, so the Q
twin does - `QkRopeArgs.hscale` on the fused rope, a `MetalHalfRows` pass after the unfused one.
The host cache takes the window's rows from the mirror byte for byte and the watermark moves to
the window's end, so a continuation gathers no cached row and the first decode step uploads none;
the f32-panel form copies every cached row to the device each window, and the decode driver
uploads them again. It needs the `attn_dev` crown, the forward's twin scratch panels, and key rows
that pad to 64 inside `seq_len`; `DASLLAMA_METAL_PF_MIRROR=0` keeps the panels.

A prepare that moves an arena slice or uploads rows first retires every step encoded ahead
(`mirror_quiesce`); one that finds the mirror standing at the window's first row
(`mirror_stands`) leaves them.

**A released session's mirror goes to the next session on the same pages.** `release_kv_pages_`
announces a paged session before its groups return to the pool, and its mirror stays under a
signature: the allocation stamp (`KVPool.gen`) of each whole page below its watermark. A new
session's first prepare takes the released mirror that shares the most leading stamps with its
own block table, the watermark at their end (`mirror_adopt`). A stamp is process-wide and moves
each time a group is allocated, so a group freed and taken again matches nothing. The pages a
prefix-cache hit attaches are a finished stream's, so the rows a warm turn skips on the host it
skips on the device too; `DASLLAMA_METAL_MIRROR_ADOPT=0` uploads them.
