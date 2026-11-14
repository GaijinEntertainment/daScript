# dasCUDA - the emitted kernel

Companion to `ARCHITECTURE.md` in this folder (contract: `ARCHITECTURE_COMMON.md` at the repo
root): what `cuda/cuda_emit.das` writes for a `[cuda_kernel]` method and the kernel vocabulary's
two tiers. Its companion `ARCHITECTURE_EMIT_TILES.md` carries the decoded tile's pipeline and the
flash tile's operands.

## The emitted kernel

A `[cuda_kernel]` class method lowers to one self-contained CUDA C++ text:

- **The signature is `extern "C" __global__ void __launch_bounds__(B) Class_method(const Class_method_bufs __das_b, const Args ka)`.**
  `min_blocks = N` on the annotation adds the bound's second argument, the blocks an SM the
  register allocation must leave room for - a tile at the register cap spills a little and
  runs two blocks an SM instead of one.
- **`precise = true` on the annotation emits every scalar f32 add, subtract, multiply and divide as its
  correctly rounded intrinsic** (`__fadd_rn`, `__fsub_rn`, `__fmul_rn`, `__fdiv_rn`), the compound
  assignments included. The kernels compile under `--use_fast_math`, which contracts `a * b + c` into an
  fma and approximates a division; a kernel whose stored bytes must be the CPU's - the tq4 K/V codec's
  rotation and quantizer - names its sums through the intrinsics, which neither pass touches. The vector
  lanes and the half family are untouched: a precise kernel is written in scalar f32.
  The `@ssbo` members are the fields of `Class_method_bufs`, one slot per `@binding` in binding
  order, each a pointer - `const T*` where no statement writes through the binding, `T*` where
  one does - and the kernel's first lines unpack them into bare names
  (`const float* __restrict__ y = __das_b.y;`), so the body spells a member as its das name. Two
  members on one binding are two views of one buffer (the Vulkan idiom `wq : array<uint>` beside
  `wq4 : array<uint4>`): the first declared owns the slot, the other unpacks through a cast of
  it, and `__restrict__` is given only while no view of that binding is written - a lie there
  would let the compiler reorder a store past a load of the same bytes. A buffer element may be
  a struct: its declaration is emitted once at CUDA's natural layout, which must equal the
  std430 layout the Vulkan home reads the same bytes through, field by field
  (`shader_block_layout`'s `compute_block_layout` is the witness; a 3-lane vector field is
  refused because the two rules part there). The `@kargs` member (also accepted spelled `@uniform` or
  `@push_constant`, the forms the other homes' lenses add) is the second parameter, a struct of
  int/uint/float fields declared `__align__(8)` so its parameter-space size is the size das gives
  the host's `(pointers, args)` struct, which pads to 8. The entry point is `Class_method`
  (`entry=` overrides), unique per class so a family's texts can share one NVRTC program.
- **Builtins are prologue locals.** A referenced `gl_*` ID is declared once at entry from
  `blockIdx` / `threadIdx` / `blockDim` / `gridDim`, in dependency order
  (`gl_SubgroupInvocationID` reads `gl_LocalInvocationIndex`); a subgroup is a warp of 32.
- **Every shared declaration is carved from one dynamic block.** A `@workgroup` member, a cm2
  tile local and a reduce's scratch each take a 16-aligned offset of
  `extern __shared__ char __das_smem[]` at emit time, bound as a pointer (an array) or a
  reference (a value or a tile); the kernel's `_smem` companion is the carve's end, the launch
  passes it as the dynamic shared bytes, and `run_compute_1d` sets the function's
  max-dynamic-shared attribute when it passes the 48 KB default. A kernel whose carve passes the
  device's opt-in limit (near 100 KB on sm_120, 227 KB on sm_90) fails at launch, not in ptxas.
- **A method or a free function is a `__device__ __forceinline__` function** - the SPIR-V call
  model in text, never a splice. A method takes, after its own parameters, the builtins it reads
  and the members it touches (transitively through its callees) as hidden trailing parameters:
  a buffer as its pointer, the args struct by const reference, a shared array as its pointer;
  a free function takes the builtins. `array<T>` parameters lower to `T*`, `T[N]` to a pointer
  into shared memory, `T?` to a pointer, a non-const `&` to a reference. A plain class member is
  rejected: a function cannot see it.
- **The prelude is generated on demand and emitted key-sorted.** `typedef`s for `uint`, `ushort`,
  `uchar` always; per vector type the lane-wise operators CUDA's own types lack (vector-vector,
  vector-scalar, scalar-vector, compound, unary minus, the scalar-bool `==` / `!=` helpers);
  the `half` struct over a `ushort` with the two PTX converts and float-routed arithmetic, and
  `half2/3/4` over it; splat, convert, multi-lane swizzle, lane-wise math, saturating-narrow and
  clamp helpers per type; the warp reductions (`__das_warp_add_T`, a xor-shuffle butterfly over
  32 lanes) and the clustered add; `usdot4`. A text names only what its kernel uses.
- **The lowering table.** `barrier()` and `memoryBarrierShared()` are `__syncthreads()`,
  `memoryBarrierBuffer()` is `__threadfence()`; `subgroupShuffle` / `Broadcast` /
  `BroadcastFirst` are `__shfl_sync`, `subgroupShuffleXor` is `__shfl_xor_sync`,
  `subgroupAll` / `Any` are `__all_sync` / `__any_sync`, each over `__activemask()` - the lanes
  executing the call, which is what the SPIR-V op means in divergent code (a lane-pair fold
  inside a guard half the warp skips reads its partner; the full-warp mask there is undefined
  behaviour and reads garbage) - and `subgroupElect` / `BroadcastFirst` pick the lowest active
  lane; the warp reductions (`subgroupAdd` and its kin) stay full-mask butterflies, so a kernel
  calls them where every lane of the warp is live, as the shared kernel bodies do;
  `atomic*` take the address of their das reference (`atomicExchange` -> `atomicExch`,
  `atomicCompSwap` -> `atomicCAS`); the das math names take CUDA's `f`-suffixed forms
  (`exp` -> `expf`, `rsqrt` -> `rsqrtf`, `saturate` -> `__saturatef`, `mad` -> `fmaf`) with the
  integer twins for `abs` / `min` / `max` and a lane-wise helper on vectors; the bitcasts are
  `__float_as_uint` and kin; `sdot4` is `__dp4a`; `packHalf2x16` / `unpackHalf2x16` are the
  PTX `cvt` pair over a `uint` (lane x in the low half), `unpack8` / `pack32` byte-shift
  helpers per type; a lane of a vector by index (`v[i]`, which CUDA's vector types lack) goes
  through three helpers picked by the expression's role and, for a read, by where the vector
  lives - not by the operand's constness: a read is `__das_at_T`, a nested ternary over the lanes
  (three selects, which keep a register vector in registers); a read off a buffer's element
  (`member[i][lane]`) is `__das_atm_T`, the lane alone loaded at the element's address (the
  selects would load the whole element first, and the address is the element's - a word past
  2^32 words of a buffer stays in reach of a 32-bit element index); an assignment's or compound assignment's target is `__das_atw_T`, pointer arithmetic
  over the contiguous lanes (an indexed store on a shared or global vector; a reference picked by
  a ternary chain compiles to a jump table on the index, a four-way divergent branch around every
  lane store - and a register vector written by a runtime lane goes to local memory, so a hot body
  masks its lane to a constant); float `%` is `fmodf`; a `let` fixed array of literals is a
  `__constant__` table; a loop hint is `#pragma unroll` (`unroll_disable` and `dont_unroll` ->
  `#pragma unroll 1`, `unroll_count=N` -> `#pragma unroll N`). A generic or `[template]`
  instance keeps its base name: the mangled form carries a decimal (generic) or hex
  (template) hash segment, and the emitter names the function by the last segment that is
  neither.
- **A KHR cooperative-matrix tile is an `mma.sync` fragment set.** The seven subgroup-scope
  markers (`coopmatA_f16_16x16`, `coopmatB_f16_16x16`, the f32 / f16 accumulators, the s8 pair
  and its s32 accumulator) lower to structs of fragment registers over the `m16n8k16` (f16) and
  `m16n8k32` (s8) lane layouts, a 16-wide N being two n-halves. `coopmatLoad` and
  `coopmatStore` take the buffer's element index and stride exactly as the SPIR-V load reads
  them (in the buffer's elements, whatever their type) and gather per lane from the byte
  address: a contiguous 32-bit load where the layout keeps a fragment's pair or quad adjacent,
  a lane-wise gather of halves or bytes where it runs the other way. `coopmatMulAdd` is two
  `mma.sync` per tile, one per n-half; `coopmatConvert` widens the f16 accumulator's pairs to
  f32 quads or rounds the f32 quads to f16 pairs.
  Global and shared operands take the same helper through a generic pointer.
- **A cm2 workgroup-scope tile is a shared array or a per-warp fragment grid.** A `coopmatWgA_*`
  tile is a k-contiguous [rows][ld] element array in shared memory and a `coopmatWgB_*` tile a
  k-contiguous [cols][ld] one - the mma's own B layout, and the token panel's as it sits in
  memory - with `ld` the contiguous extent plus a 16-byte pad, so ldmatrix's eight row reads
  land on eight bank groups. A B tile every tensor load reads under a plain view instead keeps
  the source's own [k][n] rows (the scan records each B tile local's views; a tile loaded both
  ways is refused at its load), and the product reads its fragments through the transposing
  `ldmatrix` - the flash tile's V, which an [n][k] store would transpose element by element a
  step. An f16 B tile that tensor loads from 16-bit sources write under the transposed view
  and products alone read, into accumulators the warps divide by column, is never staged: it
  is a view of its source (the window's pointer, strides, bounds and origin in every thread's
  registers; the load opens no barrier), and the product gathers the warp's own B fragments
  from global memory two k steps at a time - lane t of the mma's quad reads its eight
  consecutive k of a row as one 16-byte load, which the kernel's k permutation (below) makes the
  fragment layout. A B tile a row-owning warp reads whole (the
  flash tile's K) stays shared: staging it once for every warp pays. Every thread fills a
  shared tile (a tensor load is a cooperative loop under two barriers
  walking the source's contiguous direction, reading the layout's dimensions, strides and pad
  clamp, the window transposed under a `tensorView2Dt`; where that direction is the tile's own
  over a flat, aligned, 8-multiple-stride window, eight 16-bit elements a thread move as one
  16-byte copy, under a pad layout a run wholly inside the bounds the same way and a run at the
  edge element by element; the walk's trips run at a constant count unrolled two at a time, so
  a thread's loads issue in pairs while its registers stay bounded; the decode form calls the
  `[spirv_decode]` function or `self.<method>` at a block coordinate and coordinate in block
  over runs of eight columns of one row, the block struct read once a run by const reference,
  the per-element form where the block does not span a run; the four-wide twin, where the
  call's tenth argument names a method returning a `half4`, decodes four elements a call). A
  `coopmatWgAcc_*` tile is a register array of KHR fragments: fragment
  f = s x W + w of the (M/16) x (N/16) grid belongs to warp w at slot s, W the block's warps
  from the annotation, so the struct name carries W; with nI = M/16 fragment rows and nJ = N/16
  fragment columns (`frag_rows`, `frag_cols`), it sits at row f / nJ, column f % nJ (a
  warp owns a column of fragments) unless the scan found a row reduce over an accumulator
  whose fragment rows the warps divide - then every accumulator of the kernel sits at row
  f % nI, column f / nI (a warp owns a fragment row; the struct name carries an `r`).
  `coopmatMulAdd` loads each of a warp's A and B fragments once a k step through `ldmatrix`
  (f16; the lane gather for s8) and multiplies its own slots - a row-owning warp its one A
  fragment against every B fragment, else the (M/16) rows at nJ / W columns where W divides nJ,
  one column where nJ divides W, a slot at a time otherwise; the element-wise forms (fill,
  scale, copy, add, sub, Hadamard, clamp, per-element with its callback and extras) walk a
  lane's own fragment elements with their (row, col); a reduce combines within the lane, across
  the lane group by shuffles, then across the warp's own slots where the warp owns the row,
  else across fragments and warps through a shared scratch the site owns; an accumulator
  converts into an A tile by writing its elements to the shared array - a row-owning warp its
  own rows under a warp sync, else every warp under block barriers. The walks' temporaries
  carry double-underscore names so no das local is shadowed.

### The k permutation {#cuda-k-permutation}

A kernel with a global-view B tile stores k permuted: within every 32 k, position
16 kk + 8 h + 2 t + e holds element 8 t + 4 kk + 2 h + e, so lane t of the mma's quad owns eight
consecutive k over two k steps and a view's lane reads them as one 16-byte load. Every f16 A tile
and shared B tile of such a kernel is written at the permuted k: the mma contracts by position, so
both operands agree. A [k][n] tile permutes its rows and keeps its 16-byte copies whole; an A or
[n][k] tile lands a run of eight as four words eight elements apart, and a decoded pair stays
adjacent.

### Tile slot reuse {#cuda-tile-slot-reuse}

A shared tile local takes the smallest carved slot whose holder was last read ahead of its
declaration. The scan records every tile's declaration and last read; a read inside a loop the
tile was declared ahead of holds it to the loop's end, and a name two declarations share takes
no part. The next write into a reused slot is a tensor load or a fill, which opens on a block
barrier every reader of the old tile has passed. A convert target, written under a warp sync,
carves its own slot. The flash tile's K and V each keep a prefetch slot of their own, K's the
register A tile's dead staging slot (`ARCHITECTURE_EMIT_TILES.md#cuda-prefetched-f16-tile`).

### The ones tile's row sum {#cuda-ones-row-sum}

A B tile filled with 1.0 that the products alone read, landing in row-owned accumulators, is
never carved: its fill is nothing, and the product is the A tile's row sums added into the
accumulator. A row-owning warp reads its own rows off the shared tile, a lane a quarter of a row,
and the quad folds the row; the sum lands in the lane's elements of those rows in every slot.

## The kernel vocabulary's two tiers {#cuda-kernel-vocabulary-tiers}

A `[cuda_kernel]` body speaks the shared GLSL/SPIR-V vocabulary (`cuda_builtins`, the lowering
table above): the compute IDs, barriers, the atomics (`atomicAdd` and its kin, min/max/and/or/xor
included), the subgroup set, pack/unpack, the dot products, the half family, the KHR fragments and
the cm2 tiles. Both emitters lower every name of that tier, so a kernel class the Vulkan home
stamps stamps here unchanged, and a name with a GLSL twin is added to that tier and lowered on both
rails, never to this one. The second tier, `cuda_intrinsics`, carries what has no twin; its names
are PTX's and CUDA's, and a body that calls one is a CUDA-only kernel by construction:

| intrinsic | lowering | CPU body |
|---|---|---|
| `cp_async_16(dst, src)` | `cp.async.cg.shared.global [dst], [src], 16` - `dst` a `@workgroup` array pointer (`addr(sh[i])`), `src` a buffer pointer; the spaces are checked at emit time. The copy lands once `cp_async_commit` has closed its group and `cp_async_wait` has drained it, and other threads see it only after a barrier | no-op |
| `cp_async_commit()` / `cp_async_wait(n)` | `cp.async.commit_group` / `cp.async.wait_group n` (`n` a constant) | no-op |
| `ldmatrix(frag, p)` | `ldmatrix.sync.aligned.m8n8.x4.shared.b16` into a KHR A or B fragment: each lane hands the address of one 16-byte row, lanes 8i..8i+7 the rows of the i-th 8x8 matrix, and matrix i lands in fragment register i. The mma reads an A fragment's registers as (rows 0-7, k lo), (rows 8-15, k lo), (rows 0-7, k hi), (rows 8-15, k hi), and a B fragment over a k-contiguous [n][k] tile as (n 0-7, k lo), (n 0-7, k hi), (n 8-15, k lo), (n 8-15, k hi), k lo and k hi the two 8-column (f16) or 16-byte (s8) halves; the transposing form is `ldmatrix_trans` below | no-op |
| `ldmatrix_trans(frag, p)` | `ldmatrix.sync.aligned.m8n8.x4.trans.shared.b16` into an f16 A or B fragment, each 8x8 matrix transposed on the way: an A fragment off a [k][m] tile (lanes 8i..8i+7 hand the k rows (k lo, m 0-7), (k lo, m 8-15), (k hi, m 0-7), (k hi, m 8-15)), a B fragment off a [k][n] tile ((k lo, n 0-7), (k hi, n 0-7), (k lo, n 8-15), (k hi, n 8-15)) - the layout a product's output lands in when the next product reads it as an operand, spared the element transpose | no-op |
| `mma_frag_set(frag, i, w)` | `(frag).r[i] = w`: word i (0..3) of the lane's s8 A or B fragment set from a register, the word `ldmatrix` would have loaded there - with g = lane / 4 and t = lane % 4 an A fragment's words are (row g, k lo), (row g + 8, k lo), (row g, k hi), (row g + 8, k hi) and a B fragment's (column g, k lo), (column g, k hi), (column g + 8, k lo), (column g + 8, k hi), each word the k bytes 4t..4t+3 of its 16-byte half. It feeds the mma from registers, the shared-memory round trip skipped. SPIR-V has no form of it: a KHR cooperative matrix's lane layout is implementation-defined, so no store names a lane's word | no-op |
| `mma_acc_get(f, k)` / `mma_acc_set(f, k, v)` | `f.r[k]`: the lane's element k (0..7) of an f32 accumulator, at row `mma_acc_row(lane, k)` (g and g + 8 for k & 2 clear and set, g = lane / 4) and column `mma_acc_col(lane, k)` (2t + (k & 1) in n-half k / 4, t = lane % 4), elements k and k + 1 (k even) a row's column pair - an epilogue that reads the product where it lands, the tile never stored | `0` / no-op (the row and column helpers are plain functions) |
| `mma_scale_acc(f, s, rs, cs)` | `f.r[k] += float(s.r[k]) * rs[row half of k] * cs[column of k]` over the lane's eight elements: the block-scaled int8 product's fixup. With g = lane / 4 and t = lane % 4, `rs` holds the scales of the lane's rows g and g + 8, `cs` those of its columns 2t, 2t + 1, 2t + 8, 2t + 9 | no-op |
| `bar_sync(id, count)` | `bar.sync id, count` | no-op |
| `byte_perm(a, b, sel)` | `__byte_perm` | emulated (the selector nibble's high bit ignored, as the intrinsic ignores it) |
| `clock32()` | `(unsigned)clock()` | the CPU tick counter's low word |
| `cuda_asm(fmt, a0..)` / `cuda_asm_value(fmt, a0..)` | a `__device__` helper a call site wrapping `asm volatile(fmt :: "r"(a0)..)`, the `_value` form with `%0` its `unsigned` result; `fmt` a constant, up to three operands | no-op / `0` |
| `coopmatA_e2m1_16x64` / `coopmatB_e2m1_64x16` + `ldmatrix` | the FP4 fragments: four words a lane, the s8 fragments' lane map over nibbles (A: k 8t..8t+7 and 32+8t.. of rows g, g + 8; B: the same over the columns of each n-half) - loaded through the same `ldmatrix.x4` off a 32-byte k64 row; `coopmatLoad` / `coopmatMulAdd` refuse them | none (a fixture's CPU replay computes its reference apart) |
| `mma_block_scaled(f, a, b, scale_a, scale_b0, scale_b1)` | two `mma.sync.aligned.kind::mxf4nvf4.block_scale.scale_vec::4X.m16n8k64.row.col.f32.e2m1.e2m1.f32.ue4m3`, one an n-half: f += A x B over k64, every value e2m1 x the UE4M3 scale of its k16 strip with no halving, `scale_a` the four scale bytes of the lane's A row (row g read from the lanes with lane % 4 == 0, row g + 8 from lane % 4 == 1), `scale_b0` / `scale_b1` column g's of each n-half (lane % 4 == 0); the other lanes' scale words are ignored | no-op |
| `mma_block_scaled_mx(f, a, b, scale_a, scale_b0, scale_b1)` | the same two instructions as `kind::mxf4.block_scale.scale_vec::2X ... ue8m0`: every value e2m1 x the UE8M0 scale (2^(code - 127)) of its k32 block, a scale word's low two bytes the lane's row's two blocks of the k64, the same lanes read | no-op |
| `mma_scale_cols(f, cs)` | `f.r[k] *= cs[column of k]` over the lane's eight elements, `cs` in `mma_scale_acc`'s column order: the FP4 product's per-token epilogue, the block-scaled mma having no s32 stage to fold a column scale into | no-op |
| `cvt_e2m1x2(lo, hi)` | `cvt.rn.satfinite.e2m1x2.f32` into a `.b8` register widened by `cvt.u16.u8` (the destination is a byte, which no asm constraint names) - the pair's byte, `lo` in the low nibble (the PTX operand order puts the first source in the upper nibble, so the helper swaps) | round to nearest even over {0, 0.5, 1, 1.5, 2, 3, 4, 6}, a tie to the even code, saturated at 6 |
| `cvt_ue4m3(x)` | `cvt.rn.satfinite.e4m3x2.f32` over max(x, 0) in both sources (a constant source lands as an immediate `cvt` refuses), the low byte: the UE4M3 code | round to nearest even at bias 7 with the m x 2^-9 subnormals, 448 and past it 0x7E |

The FP4 rung assembles under the arch-specific target alone (`sm_120a`; plain `sm_120` rejects every
block-scaled kind), so a text that uses it carries `#define __DAS_ARCH_A__ 1` and `module_from_source`
compiles it with the `a` suffix - the launcher never learns which kernels need it. An `a` cubin has no
forward PTX JIT, so those kernels bake per arch.

The floor is `cuda_asm`: anything PTX says a body can say from day one; a named intrinsic is the
rung above it with a census kind and a fixture (`tests/cuda`, the `Intrin` and `IntrinFp4` classes).
