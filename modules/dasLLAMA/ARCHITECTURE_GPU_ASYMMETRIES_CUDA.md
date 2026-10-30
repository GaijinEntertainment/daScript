# dasLLAMA Architecture - the CUDA home's asymmetries

Companion to `ARCHITECTURE_GPU_ASYMMETRIES.md`; a section is cited by its anchor. This document
carries the entries of the closed asymmetry list that are the CUDA home's alone - the int8 MMQ
prefill, the MXFP4 expert pieces on the FP4 tensor rung, the nv4 nibble decode and the MMQ pin
(`#cuda-asymmetries`). The list's rule and the Metal and Vulkan entries stay in
`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`.

### The CUDA home's asymmetries {#cuda-asymmetries}

**The list is closed with its parent's; a new asymmetry of the CUDA home alone lands with its entry here:**

- **The int8 MMQ prefill is the CUDA home's alone** (`DASLLAMA_CUDA_MMQ`, on by default; `crd_site_mmq`): a window
  GEMM site whose plane's format has a leaf - Q8_0; the superblock Q4_K, IQ4_XS, Q4_0 and IQ4_NL; the per-32 Q5_1, Q5_0
  and the off-lattice Q4_0 and IQ4_NL - whose output width is a multiple of 128 and whose reduction of 256 (512 where
  the leaf carries a min term), at a window past one s tile of rows (`CUD_MMQ_S_ROWS`: the leaf pads the window's rows to
  its 128-row tile, and at 32 rows or fewer the padded tile costs more than the s column split along K - the ledger's
  short-window entry; the admission reads the window being run, `CudRdec.pf_wlen`) and never on an embedding's window (the
  hidden ask: a batch's texts and the same texts alone ride one GEMM form, so a batch's vectors are its singles'), runs on that leaf over the activations requantized
  to Q8_0 blocks between the GEMMs, as the token step's feeds are; every other site - the other formats, a shape off the
  tile, `=0` - takes the f16-fed cm2 tiles, and a layer whose sites split produces both feeds (the residual step's
  requant, then its f16 norm pass with no add). `=1` leaves out the leaves whose windows did not beat the tiles on every
  measured card - Q5_1 lost on both (its five-word blocks and its m cost the leaf a third of the Q8 one's rate, under the
  feeds' cost), Q5_0 won on the RTX 5060 Ti and lost on the RTX PRO 4500, the first format the two cards split - and
  `=2` arms every leaf, a new card's own measurement; the split is what makes the pick a per-box race rather than a
  default (the ledger's row). The Q8_0 leaf stages raw blocks by cp.async; the other leaves
  (`MmqT` under `KQ` for the superblock formats, `B32` with the format's tag for the per-32 ones) decode their codes to
  int8 through registers in the weight stage - the codebook formats through the home's permute decode, Q4_0 and Q5_0
  with their offsets folded into the signed codes - a scale a block. The leaves with a min term (Q4_K's `dmin x mn`,
  Q5_1's `m`) carry it apart: the requant kernels write each activation block's scale x quant sum beside its scale (`OUT_SUM`), the stage
  keeps each row's `-dmin x mn` and each token's sum as halves over sixteen blocks, and one f16 tensor product a fragment
  pair every sixteen blocks folds the rank-sixteen correction into the f32 accumulators - a per-block fixup off shared
  memory cost the leaf a fifth of its rate. A format whose scale changes every sixteen elements (Q6_K, Q3_K, Q2_K, the
  iq2 strips) stays on the f16 tiles: the int8 product over sixteen issues at the k32 shape's cost for half its work, so
  a Q6_K leaf on it read 37 to 39 TFLOP/s against the Q6_K cm2 tile's 38 to 51 on the same shapes - MMQ is a
  per-32-scale property. NVFP4 takes the same slot on the FP4 tensor rung instead (`CuNv4Mmq` over `MmqFp4T`, the
  arch-specific `sm_120a` target the emitter marks): raw e2m1 nibbles on both sides under the block-scaled mma, whose
  UE4M3 scale a k16 strip is the format's own, so the weight stage re-pairs the k4-tiled 32-blocks into the feed's
  (k, k + 8) order and stages no int8 at all - the sixteen-wide scale that keeps Q6_K off the int8 leaf is what the
  instruction consumes. Its feed is W4A4: the activations quantized to e2m1 per token (`fp4_rows`, `fp4_actrows`, the
  residual step's `cls_ar_fp4_b`; a row scale of amax / (6 x 448), a UE4M3 code a strip - the nearest to its largest over
  6, stepped up once where that code would saturate the largest value (`fp4_strip_code`: the nearest alone saturates on
  half of all strips and reads 9% on the 1B's pp-path perplexity against 5.7% with the step; a least-squared-error pick
  over five candidate codes trades the outlier for the bulk and measured worse than either - `PERF_LEDGER.md`, the dasCUDA
  arc's W4A4 entry) - the nibbles in the same
  pairing; a feed reads its row twice, once for the row scale and once for the strips - a row kept in registers between the
  two passes measured no faster at 14336 wide and slower at 4096), the row scale and the plane's per-tensor factor applied to the accumulator's columns in
  the epilogue - the factor read once a tile off the tile's first row's fifth scale word, the device row's
  (`ARCHITECTURE_ENGINE_FORMATS_NV4.md#nv4-site-scale`), so a deltanet site and a merged K/V pair carry their planes' own factors
  with no argument; a layer whose sites split across the three feeds produces each from the one residual step, and a recurrent layer's
  out site feeds the leaf through `fp4_rows` over the scan's o rows. A plane the tile cannot take (the deltanet beta and alpha rows, 16
  or 48 wide) runs on the cm2 s tile, whose grid the pick splits k across like the l and m grids - 16 workgroups a 512-row window each
  walked all of k at 180 us a plane on the 27B against llama.cpp's stream-k 11 us (`PERF_LEDGER.md`, the 27B NVFP4 window profile). On the RTX
  5060 Ti the leaf ran 110 to 121 TFLOP/s against the Q4_0 int8 leaf's 65 to 72 and the cm2 tiles' 40 to 56 at the 1B
  window's shapes (`PERF_LEDGER.md`, the dasCUDA arc's FP4-leaf entry: the command, the shapes, the forms not kept). The
  knob changes the prefill's rounding and its logits; the window log line names the planes on each form, and under the
  test pin `set_cuda_mmq_pin` it names the pin's value in place of the knob's. The first window sizes the feeds under the
  knob's answer and keeps it (`CudRdec.pf_knob`), so a pin moved after it changes no later pick onto a feed that was never
  allocated. The FP4 leaf is picked on a card at compute capability 120 or above (`FP4_RUNG_SM`: the block-scaled mma is
  an `sm_120a` text); below it an nv4 plane's window runs on the cm2 tiles under any knob value. Both forms stay compiled whichever one a card favors: the int8 against f16 tensor rate
  is the card's and the driver's, and the default is a static pick until a card flips it. The Vulkan window's form follows the device's coopmat mode
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`), and the knob reaches nothing there or on Metal.
- **The CUDA window's MXFP4 expert pieces ride the FP4 tensor rung fed from the expert schedule; the Vulkan window keeps the
  f16 cm2 tiles.** The routed block's gate, up and down pieces take `MmqFp4T` under its `MX`, `KTAIL` and `MOE` arms
  (`CuMx4Fp4MmqMoe`, `CudLayer.moe_mmq`, on a card at `FP4_RUNG_SM` under `DASLLAMA_CUDA_MMQ`, both reduction widths on the
  64 lattice - `crd_moe_mmq_fits`, since the k tail stages a k64 pair and the feed packs two codes a word): the weights' e2m1 nibbles and
  UE8M0 bytes straight off the mx4 arena planes (the pair of a k64 read as one half-word: a row's first block is even), the
  gathered rows and the hidden rows as the MXFP4 feed (`CuMx4Rows`, `CuMx4ActRows` / `CuMx4ActRowsB`: e2m1 under a
  power-of-two scale a 32-block by the MXFP4 rule, two codes a k64 word, the row scale 1), the product through
  `mma_block_scaled_mx` (`kind::mxf4`, both operands UE8M0-scaled). The schedule pads every bucket's rows to a 128-row
  multiple and puts every row on the m column (the MMQ schedule stamps `CuMoeSchedMmq` / `CuMoeSched2Mmq`, `MoeSchedT`'s `ROW_PAD` and `M_ONLY` constants), so a tile past a bucket's rows lands in its
  pad and the f16 s column retires; the workgroup's record names the expert plane's block base, the bucket's row base and
  the tile in place of the grid's; a plane off the 128 lattice (gpt-oss's 2880) stores its last column tile through a
  staged guard, and a reduction off the 128 lattice runs a k tail (the last step's second k64 under a zero scale on both
  sides: the weight stage zero codes, the feed stage the next row's real codes, which the zero scale annuls - the dense
  FP4 site keeps the 256 lattice). The int8 form of the same pieces (Q8_0 feeds on
  the `MmqT` tile) measured behind the cm2 tiles on the same card - the tile's token rows are a bucket's, so a 512-row
  window's 32 buckets of about 64 rows each waste half of every 128-row tile - and the FP4 rate is what pays for the pad;
  the 4-bit activation widens the gpt-oss cells' logit distance from a tenth of the bar to half of it (the routed-block
  entry of `PERF_LEDGER.md`, 2026-10-26). Below the rung, and
  with the knob off, the pieces run on the schedule's s and e cm2 tiles as the Vulkan window's do.
- **The nv4 decode GEMV's nibble decode is a byte permute on CUDA, the SWAR formula on Vulkan and a staged codebook on
  Metal.** The shared leaf (`Nv4GemvT`) routes the e2m1 nibble decode through one virtual (`e2m1_split`); the CUDA home's
  `Nv4GemvPrmtT` overrides it with `byte_perm` over the doubled-magnitude table and its negation - 18 lane ops a quant word
  against the formula's 29, the same int8 lanes, so the sums agree bit for bit - and the decode reads at the Q4_0 leaf's
  rate at the 8B shapes where the formula read 4 to 5% behind (`PERF_LEDGER.md`, the dasCUDA arc's nv4 decode GEMV entry).
  Vulkan has no byte permute, so it keeps the formula. Metal does not stamp the shared leaf at all: nv4 rides the iq4
  family's templates under `NV4`, the doubled e2m1 table staged in threadgroup memory as iq4nl's codebook is and the
  UE4M3 byte pair read off the compact 16-byte row (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`);
  the per-tensor `.scale` the Vulkan and CUDA rows carry in their pad word has no Metal carrier, so a plane under one
  declines there.
- **`set_cuda_mmq_pin` is a CUDA-only setter.** It pins the MMQ knob's answer ahead of a load, so one process can hold a
  carrier to the leaves and then to the tiles (the resident cells' knob-off control, the W4A4 instrument). The Vulkan
  window's form follows the device's coopmat mode and Metal has one window form, so neither home has a pin to set.

### The CUDA home binds its context to the calling thread at every entry {#cuda-thread-binding}

The CUDA driver's current context is a property of the OS thread, not of the process (the dasCUDA module's
`ARCHITECTURE.md`, its per-thread context section): a context made on one thread is unbound on every other. A Vulkan
device has no such binding - any thread submits on it under the queue lock - so the Vulkan home's entry gate checks the
device once. The CUDA home's entry gate, `cud_init`, binds `g_cud.cc` to the calling thread on every call once the device
is ready (`cuda_bind_context`), because a context's `[init]` runs on the thread that made it and a server's worker thread,
or a job on the team, reaches its first device call on another: without the rebind the query answers an invalid-context
error and the worker falls to the CPU with no decline named. The bind is one driver call, cheaper than any device work
behind it.
