# dasCUDA - the emitted kernel's tile pipelines

Companion to `ARCHITECTURE_EMIT.md` in this folder; a section is cited by its anchor. This document
carries the decoded tile's pipeline and the flash tile's operands. The k permutation
(`ARCHITECTURE_EMIT.md#cuda-k-permutation`) and tile slot reuse
(`ARCHITECTURE_EMIT.md#cuda-tile-slot-reuse`) they build on stay in that file.

## The decoded tile's pipeline {#cuda-decoded-tile-pipeline}

A decode-in-load whose A tile is f16, whose layout's block size is a compile-time (1, b1) with
b1 a multiple of sixteen dividing the tile's k, and whose block holds one byte an element (the
scan's `can_pipeline_decode`; the Q8_0 tiles) is lowered as a software pipeline over the kernel's
k loop, because the plain form - the block read element by element through its const reference
into global memory, a byte and a scale half a load, nothing issued ahead - left the mma pipe
idle on every step while eight warps waited on the same latencies. The tile's local is a
pointer at two f16 slots: the decode of a step lands in the slot the product of the step
before does not read, and rebinds the pointer. A step's raw blocks land in one shared slot by
cp.async, each thread copying the 16-byte chunk its own decode run of sixteen reads, so a
thread waits only on its own copies (`cp.async.wait_group`), never on a block barrier. The
decode statement of step k opens on the step's one barrier (the f16 slot of k published, the
slot of k - 1 free), checks the prediction registers (base, row, column, slot of the step
decoded and ready - uniform values every thread holds), and rebinds the pointer; the decode
of step k + 1 is emitted after the next product over the tile, in the product's own basic block,
so the scheduler interleaves it with the mma stream: wait for the own chunks, decode them into
the other f16 slot as eight packed pairs at the permuted k, record the step, and copy the chunks
of k + 2 into the raw slot the decode just freed. A step the registers did not predict (the
first, a change of layout or base) copies, decodes and publishes synchronously, then starts the
next step's copies. The pad rows and the blocks past the extent are never copied, and the
decode never reads them. Measured on the l tile at the window's three shapes under locked
clocks against the plain form: 109 / 341 / 420 us to 115 / 324 / 429 at two blocks an SM, and the
m tile 102 / 413 / 377 to 93 / 351 / 345 - past the Vulkan driver's m tile (102 / 403 / 373).
The register budget decides the rest: at the 128-register cap of two blocks an SM the l tile
spills, and at one block its small grids lose the second block's latency hiding; prefetching
the view's B fragments a block ahead and double-buffering the A fragments both measured neutral
and are not emitted.

Both forms, the pipeline and the plain one, decode through the call's four-wide twin where the
tenth argument names a method returning a `half4`: a run of sixteen (the pipeline) or eight (the
plain form) is four calls, each one block read and one scale read for four elements, and the
scalar decode serves only a run the layout's bound cuts. The superblock formats (a 256-element
block of 144 to 210 bytes, wider than the 64-k step) run the plain form with the four-wide call
at two blocks an SM, and that is their measured best: a staged form - every row's block
footprint in one shared slot, a step's byte ranges copied by cp.async, a block barrier either
side of the decode because a thread decodes bytes other threads copied - halved the blocks an
SM through the slot's footprint and measured slower on every K-quant tile (the k4 m tile 91 /
344 / 346 us plain against 129 / 499 / 498 staged, the l tile 110 / 330 / 417 against 113 / 442 /
427), so it is not emitted. What the four-wide call bought: the k4 tiles 1.3x and the k6 tiles
1.2x (the k4 l tile 125 / 487 / 487 us to 109 / 312 / 411), ahead of the Vulkan driver's tiles
on the gate/up shape and within a run's drift of them on the other two.

An s8 tile a tensor load fills off an int8 plane along the tile's k - an A tile under a plain
view, a B tile under the transposed one (the scan's `raw_s8`) - rides a cp.async ring of
`RAW_S8_RING` slots behind a pointer (`raw_s8_text`): a window's 16-byte chunks go by cp.async
into a slot, each thread copying the chunks of its own run, with the window being read in one
slot, the next in the slot after, and the windows ahead in flight into the rest, so a step's
worth of DRAM latency stays in flight. The prediction registers hold the base, row, column and
slot of the window copied and ready, and a predicted window opens no barrier: the ring's
barrier - every second window, flushed past the product over the tile - made every thread's
waited chunks visible and every read of the slot the next copy lands in done. A window the
registers did not predict copies itself and the next synchronously, waits, and starts the rest.
The product over the tile defers the ring's advance (`deferred`) and the two copies past the
barrier (`deferred_post`) behind itself, and the kernel's last line waits for the ring's last
speculative copies before the block leaves. The census kinds are
`call.coopmatLoadTensor.{A|B}.s8.<elem>[.t].raw` and `stmt.var.wg_shared.ring`.

## The flash tile's operands {#cuda-prefetched-f16-tile}

An f16 B tile one tensor load fills from a half source inside a loop and one product reads,
the load ahead of the product in the same loop's body and neither under an if of that body, in a
kernel with no global view (the scan's `prefetch`; the flash tile's K and V), keeps one slot - a
dead slot it takes outright, else a fresh carve, and nobody reuses it - and its load site waits
instead of copying: the window the prediction registers name (buffer, base, row, column, the
layout's extents and row stride; uniform values at kernel scope) is already landing by cp.async,
so the site waits on the group with later groups still in flight and opens the block barrier
that publishes it. The scan reads that order off two numbers it records at each 16-bit tensor
load and each product: the id of the innermost loop open there - every loop body the scan
enters takes a fresh id - and the ifs opened since that body's entry, so an if around the whole
loop does not count; a body holding a break or continue at any depth runs no fixed count of
products a trip, so its tiles keep the plain load, and a return at any depth does the same to
every loop open around it; a loop admits eight prefetched tiles at most, the outstanding groups
cp.async queues. The groups it may leave in flight are the products of the loop's other
prefetched tiles that commit between its own product and its next load - the ones after its
product in the body and the ones ahead of its load - which the one-load-one-product order makes
a fixed count a trip: K and V loaded each ahead of its product wait at one, both loaded ahead of
both products wait at one and none. The product over the tile defers the next window's copy
behind itself (`deferred`): a barrier, every warp past the slot, then the window the stride the
base's first two windows taught - the same step along the rows for K and V, which is why the
stride is learned and not read off the tile - goes out as 16-byte chunks, a thread the chunks of
its own run, the chunks past the layout's extent as zeros under every layout since a predicted
window may lie past the loop's end, and the group commits whether it copied or not so the wait
count stays uniform. The chunks go out only where every chunk is whole and aligned - the column
origin, the column step and the layout's column extent at multiples of eight, the row stride
too, the base at sixteen bytes; otherwise the site keeps the plain load. A window the registers
did not predict (the first two of a base, a ring's wrap, a changed base or buffer) waits for
every copy in flight, copies synchronously and waits, and the scalar helper serves a source with
no 16-byte run. The kernel's last line waits for the last speculative copy. An f16 A tile one convert from an f32 accumulator writes
outside every loop and row-owning products into f32 accumulators alone read, at eight k fragments or fewer (the scan's `areg`; the flash
tile's scaled Q), is the register A tile: the convert stages the warp's own rows and loads its
fragments once through ldmatrix, the product (`__das_wg_mma_areg_*`) multiplies the warp's own
fragment row against every B fragment of each k step, and
the staging is dead from the convert on, so the prefetched tiles take its slot - the flash
tile at head 128 stays at three blocks an SM. The f16 accumulator's add, sub and Hadamard are
one packed `f16x2` instruction a fragment word, each sum or product of two halves rounded once
as the element walk rounds it; the scale keeps the element walk, since a scalar rounded to half
ahead of the multiply would not. Against the synchronous copy the three forms take the vision
tower's bidirectional h128 tile at 4096 rows to a little over half its time, bit-identical, the
prefetch worth most of it, the register A tile the next part (it keeps the tile at three blocks
an SM where the prefetch alone sits at two), the packed forms the rest; the reading is the vision
entry of `dasLLAMA/PERF_LEDGER.md`. The census kinds are
`call.coopmatLoadTensor.{A|B}.f16.<elem>[.t|.kn].prefetch`, `stmt.var.wg_shared.prefetch`,
`stmt.var.wg_areg`, `call.coopmatConvert.wg.<comp>.A.reg`, `call.coopmatMulAdd.wg.f16.<comp>.areg`
and `call.coopmat{Add|Sub|MulElem}.wg.f16x2`.
