# dasLLAMA Architecture - the CUDA home's asymmetries

Companion to `ARCHITECTURE_GPU_ASYMMETRIES.md`; a section is cited by its anchor. This document
carries the entries of the closed asymmetry list that are the CUDA home's alone - the int8 MMQ
prefill, the MXFP4 expert pieces on the FP4 tensor rung, the nv4 nibble decode, the MMQ pin and the N-row form knees
(`#cuda-asymmetries`) - and the home's own cm2 column pick (`#cuda-cm2-column`), its tower chains' graphs
(`#cuda-chain-graphs`), its shared-memory carveout (`#cuda-carveout`), the rows tensor leaf (`#cuda-rows-leaf`) and its block drafter's round (`#cuda-dflash-pass`). The list's rule and the Metal and Vulkan entries stay in
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
- **The CUDA window's MXFP4 and NVFP4 expert pieces ride the FP4 tensor rung fed from the expert schedule; the Vulkan window
  keeps the f16 cm2 tiles.** The routed block's gate, up and down pieces take `MmqFp4T` under its `MOE` arm (`CudLayer.moe_mmq`, on
  a card at `FP4_RUNG_SM` under `DASLLAMA_CUDA_MMQ`, the three stacks in one format with a stamp on the rung - `crd_moe_mmq_fits`):
  nv4 stacks on the dense nv4 stamp's weight stage (`CuNv4Fp4MmqMoe` / `CuNv4Fp4MmqMoeS`: the superblock rows, the sixteen UE4M3
  strip codes, each expert plane's per-tensor factor off its own scale rows' pad word - the schedule's block base is the expert's
  superblock base - both reduction widths on the 256 lattice, the gathered rows and the hidden rows through the dense site's feeds
  `CuFp4Rows` / `CuFp4ActRows`, whose act form adds no bias rows, so a biased nv4 stack keeps the tiles); mx4 stacks under the `MX`
  arm (`CuMx4Fp4MmqMoe` / `CuMx4Fp4MmqMoeS`, both reduction widths on the 64 lattice: the last step's tail runs whole k64s and the
  MXFP4 feed packs two block codes a k64 word, so a width at 32 past a multiple of 64 - a legal mx4 plane - has no half pair and
  keeps the tiles, as every other format does; `crd_moe_mmq_fits`): the weights' e2m1 nibbles and
  UE8M0 bytes straight off the mx4 arena planes (the pair of a k64 read as one half-word: a row's first block is even), the
  gathered rows and the hidden rows as the MXFP4 feed (`CuMx4Rows`, `CuMx4ActRows` / `CuMx4ActRowsB`: e2m1 under a
  power-of-two scale a 32-block by the MXFP4 rule, two codes a k64 word, the row scale 1), the product through
  `mma_block_scaled_mx` (`kind::mxf4`, both operands UE8M0-scaled). The chain reads the cm2 tiles' own packed ladder schedule
  (`CuMoeSched` / `CuMoeSched2`: a bucket's rows within the s column on one 32-row piece, past it whole 128-row m columns and a
  remainder past the s column joined to the last as a partial): the m pieces run on the 128-token stamps and the s pieces on the
  32-token ones (`MmqFp4T`'s `TN`: four warps down the weight rows and one across, a 128-thread block whose lanes stage four
  weight quads a step), and the store guards its token rows by the record's count as well as its columns by `d`, since a
  packed bucket's last tile reaches into the next bucket's rows (or the plane's read slack past the last). The workgroup's
  record names the expert plane's block base, the bucket's row base and count and the tile in place of the grid's; a plane
  off the 128 lattice (gpt-oss's 2880) stores its last column tile through the staged guard, and a reduction off the 128
  lattice runs a k tail (the last step's second k64 under a zero scale on both sides: the weight stage zero codes, the feed
  stage the next row's real codes, which the zero scale annuls - the dense FP4 site keeps the 256 lattice). The s column is
  what keeps a 256-expert window off eight times its work: at pp512 a bucket holds about 16 rows, and a 128-row tile a bucket
  would be mostly padding (`PERF_LEDGER.md`, the 35B NVFP4 prefill entry); the int8 form of the same pieces (Q8_0 feeds on the `MmqT` tile) measured behind the cm2 tiles on the same card; the 4-bit
  activation widens the gpt-oss cells' logit distance from a tenth of the bar to half of it (the routed-block entry of
  `PERF_LEDGER.md`, 2026-10-26). Below the rung, and with the knob off, the pieces run on the schedule's s and e cm2 tiles as
  the Vulkan window's do.
- **The nv4 decode GEMV's nibble decode is a byte permute on CUDA, the SWAR formula on Vulkan and a staged codebook on
  Metal.** The shared leaf (`Nv4GemvT`) routes the e2m1 nibble decode through one virtual (`e2m1_split`); the CUDA home's
  `Nv4GemvPrmtT` overrides it with `byte_perm` over the doubled-magnitude table and its negation - 18 lane ops a quant word
  against the formula's 29, the same int8 lanes, so the sums agree bit for bit - and the decode reads at the Q4_0 leaf's
  rate at the 8B shapes where the formula read 4 to 5% behind (`PERF_LEDGER.md`, the dasCUDA arc's nv4 decode GEMV entry).
  Vulkan has no byte permute, so it keeps the formula. Metal does not stamp the shared leaf at all: nv4 rides the iq4
  family's templates under `NV4`, the doubled e2m1 table staged in threadgroup memory as iq4nl's codebook is and the UE4M3
  byte pair read off the compact 16-byte row (`ARCHITECTURE_GPU_QUANT_PLANES.md#metal-kq-split-scale-plane`); the per-tensor
  `.scale` the Vulkan and CUDA rows carry in their pad word has no Metal carrier, so a plane under one declines there.
- **The CUDA window's chunked deltanet scan is its own class on the intrinsics tier** (`DnScanChunkCuT` /
  `dn_scan_chunk_cu_body` in `dasllama_cuda_classes`; the Vulkan home stamps the shared `DnScanChunkT`,
  `ARCHITECTURE_GPU_SHARED_KERNELS.md#dn-discrete-kernels`). The algorithm is the shared body's; what the shared surface
  cannot express is where a chunk's time went: a 16 x 16 cooperative tile has no element access and loads A and B from one
  layout each, so the shared body lands every product as f32 in shared memory, re-splits it into the next product's pairs
  and re-lays the operands a product needs transposed - ten barriers a chunk and a chunk-long chain of staged tiles. The
  CUDA class runs eight subgroups over 32 of a head's value columns (`dn_scan_chunk_wgs` at `DNC_V`: half the shared body's
  grid, so the 35B's 128 workgroups fit one wave at two a multiprocessor - `min_blocks = 2` holds the register allocation to
  128 a lane, where the unbounded kernel took 226 and ran one workgroup a multiprocessor - and a head's q and k rows are
  staged half as often), holds the state slice in accumulator fragments across chunks - a subgroup one 16-column half and
  two d-blocks - and writes each product's epilogue straight off the lane elements (`mma_acc_get` / `mma_acc_set`: delta, P,
  the lower matrix and the o rows read where they land, the decayed Q S held in the registers of the warp that lands the o
  rows), reads the operands a product lands transposed through the transposing `ldmatrix` (delta and solved as [t][v] serve
  the solve and the o product as B, solved x decay-to-end serves the update as A, the k rows serve the update as B - no
  transpose pass), folds the chunk's decay into the state by the column scale, fetches the next chunk's q, k, v rows and
  gates into registers a chunk ahead (the gates' prefix a shuffle scan over sixteen lanes, where the shared body's one lane
  walks sixteen dependent loads), and runs a chunk on five barriers - ahead of the per-token scan at every length measured,
  where the shared body on this home is not (`PERF_LEDGER.md`, the 35B NVFP4 prefill entry's scan cuts). The cells hold it
  to the per-token scan and the CPU oracle on the CUDA kernel file's dn family, and the intrinsics' lane map and transposing
  loads to a CPU reference in `tests/cuda/test_cuda_oracle.das`.
- **`set_cuda_mmq_pin` and the N-row step's form knees are CUDA-only setters.** The pin holds the MMQ knob's answer ahead
  of a load, so one process can hold a carrier to the leaves and then to the tiles (the resident cells' knob-off control,
  the W4A4 instrument); `set_cuda_rows_mma_min` sets the rows from which a site takes the rows leaf (`#cuda-rows-leaf`),
  `set_cuda_moe_fp4_rows` / `set_cuda_moe_tile_rows` those from which an N-row routed site takes the FP4 expert chain or the f16
  tiles, each dropping the captured graphs. The Vulkan window's form follows its coopmat mode and its N-row command has no
  rows leaf or routed tile form; Metal has one window form and its row levers (`ARCHITECTURE_GPU_ASYMMETRIES.md#gpu-asymmetries`).
- **The CUDA tower's in-process A/B seams are CUDA-only setters.** Each answers the value it replaced, has a getter beside
  it, and logs once when it moves its policy off the default, so a race flips a pick between rounds with no reload:
  `set_cuda_tower_carveout_full` / `cuda_tower_carveout_full` (`#cuda-carveout`, default off), `set_cuda_tower_fork` /
  `cuda_tower_fork_on` (off: the pre-LN q / k / v GEMMs launch on the one stream, `ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives`;
  default on), `set_cuda_fc_front_handoff` / `cuda_fc_front_handoff_on` (off: the parakeet / GigaAM front reads its rows back
  and the blocks upload them; default on), `set_cuda_tower_splitk_fill` / `cuda_tower_splitk_fill` (the fc2 site's split
  gate, `#cuda-cm2-column`; default 4, 0 the pick's own rule), `set_cuda_tower_splitk_min_rows` /
  `cuda_tower_splitk_min_rows` (an encode row floor the split must also reach; default 0, no floor), and
  `set_cuda_tower_graphs` / `cuda_tower_graphs_on` (`#cuda-chain-graphs`; default on). The Vulkan tower has none of the first three to flip:
  its driver sizes the shared-memory split, its hazard rail overlaps independent dispatches with no marks, and its front
  hands its rows to the blocks through the host.
- **The CUDA tower serves the vision and the audio families; the translation and TTS seats stay the Vulkan tower's**
  (`dasllama_cuda_tower`: gemma3v, gemma4v, gemma4uv, qwen3v, qwen25v - `ARCHITECTURE_GPU_TOWER_VULKAN_VISION.md#vk-vision-chains`;
  the audio and ASR-decoder seats from the home each family's row of `CUDA_AUDIO_HOMES` names, every row the CUDA home's -
  `ARCHITECTURE_GPU_TOWER_RAIL.md#rail-audio-installs`). A process serving a translation or TTS tower beside the CUDA home
  holds both APIs - the Vulkan device armed for that tower, the CUDA context for the rest - and the Vulkan tower's vision
  seats stand down there (`vulkan_tower_install_vision` is the Vulkan home's installer,
  `ARCHITECTURE_GPU_TOWER.md#tower-reach`). The CUDA tower keeps no hazard rail and no descriptor sets (the `cud_`
  rail's launches are stream-ordered), carries no per-role GPU profile (`DASLLAMA_GPU_PROF` is the Vulkan tower's -
  its encode is timed by the lane probe's wall), declines no tile family (the home has one feed, cm2), and takes its
  f16 x f16 GEMM through `CuF16GemmCm2W` and its s column `CuF16GemmCm2WS` - the `F16GemmCm2T` stamps with the weight
  offset (`#cuda-cm2-column`), beside the E-series seat's `CuF16GemmCm2`, which pins the offset off - where the Vulkan
  tower's `F16GemmCm2` stamps serve both sites.
- **The residual step runs a row on 1024 lanes** (`LANES` on the `GkAddRmsRows` stamps, `TG` on gemma's combine): a
  256-thread block walks a 5120-wide row twenty columns a lane, and a token step launches the step some 130 times, each a
  block's whole latency; 1024 lanes walk the row five columns a lane.

### The CUDA home picks its cm2 column by its own wave model {#cuda-cm2-column}

Every tower Q8_0 cm2 GEMM on this home (`cut_pick`, `cut_pick_whole`) takes its column from `cut_cm2_tc`, not from the
Vulkan home's `cm2_wave_col` (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`); the LLM window's site
(`cud_cm2_pick`) takes its own column from `cm2_wave_col` at two blocks an SM (`cud_cm2_tc`) and a tower's only where a
tower hands it one. The two rules stay separate: at 512 rows they pick the same columns at a Qwen3-4B window's widths,
and at 1000 rows the towers' rule takes m where the window's takes l on the 4096- and 9728-wide GEMMs, whose last l
column is partial, and measured slower on the window there (`PERF_LEDGER.md`, the in-process tower race entry). Both
rules open alike (64 rows or fewer take the s column, 128 or fewer m); on the towers' rule alone `DASLLAMA_CM2_TILE`
forces one, and in process the test seam `set_cuda_tower_cm2_col` forces one over the knob, so a race flips the towers'
column between rounds. Past them the towers' three columns race on whole waves of two blocks an SM - the occupancy the loader reports for the s, m and l tiles alike (the m and l tiles at 128 registers a
thread and the s tile at 117, each 45088 bytes of dynamic shared memory - the class-load log line) - at the step weights
s 6, m 10 and l 16, and the l column's weight is 20 where its last column is partial, a tie to m. Every figure in this section was read on the reference box (Threadripper 3990X,
RTX 5060 Ti 16 GB, 36 SMs, the clock locked at 2392 MHz). Two properties of the CUDA tiles set the shape. The l tile takes
the edge path on a partial last column, and on this home that column costs more than a whole one: at 784 rows a 1280 x 1280
GEMM read 143 us on the l column against 110 at 1024 rows (four whole columns, the same 40 workgroups). And the rule weighs
a wave of the s column at six tenths of an m wave (the step weights 6 and 10), where the Vulkan weights give it half (5 and
10), yet where few weight tiles leave the m grid short of a wave the s column's wider grid still wins (a 3072-deep,
768-wide GEMM at 138 rows: 61 us on s against 95 on m). The Vulkan model at two blocks an SM picked the l column at 1500
rows, where it ran 40% behind m (whisper turbo's 1280-wide sites: 170 against 123 us).

The fc2 site - the one tower site whose body sums split planes (`split_ok`) - takes `cud_cm2_pick`'s k split only where
the split pays (`cut_split_pays`): where the whole pick's workgroups, at the model's column, times four fit the card's
block slots, so the whole grid would leave half the SMs idle; a forced split count (`DASLLAMA_CM2_SPLITK`) passes the gate
whatever the grid. The window's pick asks whether the widened l grid fills half
the slots, which is the wrong grid at a tower site. At whisper tiny's 1500 rows the fc2 GEMM (384 wide, 1536 deep) runs 36
m tiles whole, a block on every SM, and the window's rule splits it three ways into 54 l tiles with a partial last column
and a reduce over three 2.3 MB planes, which reads no faster; at qwen3a's 104- and 52-row windows (896 or 1024 wide, 3584
or 4096 deep) the whole grid is 7 to 16 tiles, under a quarter of the slots, and four to eight k planes put the idle SMs
to work. The row count does not separate the two shapes; the whole grid does. The Vulkan tower keeps its row floor
(`ARCHITECTURE_GPU_TOWER_VULKAN.md`, `VT_SK_MIN_ROWS`). Raced in process (`harness/tower_race.das`, `debug-jit`, 10 rounds;
`PERF_LEDGER.md`, the fc2 split gate entry), encode ms medians: qwen3a 0.6B 16.130 under the gate against 17.834 under
a 512-row floor and 18.430 never split, qwen3a 1.7B 19.680 against 22.219 and 21.941; whisper tiny 2.303 whole
under the gate against 2.297 and 2.413 in the two arms that split it (the floor, no gate) - one split configuration's
spread - the whole arms taking 8 of the 10 rounds; the bound at 8 drops qwen3a's 52-row splits and reads 17.391 and 20.861, the bound
at 2 splits tiny. With the fc2 site split, qwen3a's encode reads the model's pick (m at 104 rows, s at 52) level with s
everywhere on the 0.6B (15.915 against 15.923 ms) and ahead of s and m on the 1.7B (18.822 against 19.630 and 19.002).
A column `DASLLAMA_CM2_TILE` or `set_cuda_tower_cm2_col` forces holds at a split site too (`cut_split_held`: the k
planes at the forced column, up to the slots its grid leaves idle - `cud_split_want`, the window pick's own count), where
the window's pick would widen m to l. Each caller bounds the split planes by its own scratch: the window by its split
buffer (`pf_splitk_bytes`), a tower site by none at the pick, since `cut_pick` grows the tower's own planes to the split
and picks whole where they do not fit - so a tower's split never reads whether an LLM decoder is armed.

Over 162 shapes (rows 138 to 4096, the towers' widths 384 to 8192 and their depths) the CUDA rule's pick sits 0.7% behind
the shape's fastest column on average and the Vulkan model at two blocks an SM 8.1%; on the whisper turbo encode the two
read 66.6 and 81.1 ms (`debug-jit`; `PERF_LEDGER.md`, the audio towers on the CUDA home). The same race on the Vulkan home put its own model 10.1% behind the
fastest column and the three-column race at one block an SM (s 5, m 10, l 16, the partial l at 20) 0.2%; the Vulkan
window keeps its model until that form is measured on its windows.

The f16 tile (`F16GemmCm2T`, the f16 lane's GEMM over the halfword plane, `rail_mm16`) takes its column from the same
wave form at its own occupancy (`f16_cm2_col`, through `cud_f16_tc`; `DASLLAMA_CM2_TILE` forces s or m): the s column at
64 rows or fewer, else whichever of s and m costs fewer whole waves at the step weights s 6 and m 10, the s tile at four
blocks an SM (62 registers) and the m tile at two (126 registers, 18 KB of shared memory - the A tile alone, the B tile
read straight from the feed). The l tile runs at one block an SM (174 registers) and trails m on every shape, so the f16
tile has no l stamp. The weights hold in the encodes: at 138 rows the 1024-wide sites take s (canary's 81 us a launch
against 121 on m) and the 4096-wide ones m (78 against 90 on s), at 275 rows every site takes s (gemma4a's 1024 x 4096:
106 against 126), at 1500 rows every site m (`PERF_LEDGER.md`, the f16 lane on the discrete homes). Every column sums an output's k in the same order, so the columns' outputs are
bit-identical. The f16 tile accumulates in f32, the reference lane's sum, where the Q8_0 tiles accumulate in f16, and this
card runs an f32-accumulate tensor product well under the f16-accumulate rate (the same m tile at 1500 x 1280 -> 5120: 34
TFLOP/s against 55 with an f16 accumulator), so at whisper's 1500 rows the f16 lane's GEMMs trail the q8 lane's by that rate
rather than by a tile.

### Five to sixteen token rows take the rows tensor leaf {#cuda-rows-leaf}

**A dense site at five to sixteen token rows runs one weight pass on the tensor cores, its time flat in the row count.** The leaf (`RowsMmaT`,
`dasllama_cuda_classes_rows`) gives a block of eight warps sixteen weight rows, the warps splitting the rows' K; a 32-block is the int8
product m16n8k32 twice - sixteen rows by sixteen token rows - its weight fragment filled from the lane's own plane words and its activation
fragment one 16-byte load off the feed. The quant plane binds as 16-byte words, a lane loading its one word at the element's address: an arena
past 2^32 words stays in reach. The feed holds the sixteen rows' quants in the product's lane order with their scales and block sums: the
requant that made the rows writes it in the same launch (`GkQ8BlockStore`'s `OUT_FEED` stamps), and a site whose rows another launch followed
re-lays them (`RowsMmaRelayT`; `crd_feed_stands` tracks which rows the feed holds). `crd_rows_mma_takes` admits a plane of a leaf format -
Q8_0 and the superblock formats of sixteen nibble bytes a 32-block: Q4_K, Q5_K, Q6_K, NVFP4, Q4_0, IQ4_NL, IQ4_XS, IQ3_S4
(`cud_rows_mma_stamped`, `kq_tile_stamp`'s `rows` family) - at the knee's rows and up, whole units wide past the floor (1024 for Q8_0, 2048
for a superblock format), sixteen regions at the most, inside the feed's width; every other site takes the N-column GEMV
(`ARCHITECTURE_GPU_SHARED_GEMM.md#ncol-load-waves`), which is the faster form to four rows. A format is its hooks alone: the lane's two words
of a block through the word decode its GEMV reads (`nib_word`, `k5_word`, `k6_word`, a codebook's word map), and the block's scales. Q3_K,
Q2_K, the grid formats (a codebook plane the leaf does not bind) and the per-32 formats have no leaf. The walk loads a plane word once: a
turn's four blocks are two pairs of neighbours, and the words a pair shares - a K format's d | dmin, sc and mn, Q6_K's sub-scale word, d and
qh words, NVFP4's scale word and factor, an f16-d format's d word, IQ4_XS's two scale words, the pair's feed scale - load once a pair
(`w_pair_raw`), a second load of one being a second wait on its plane's stream. An offset format keeps its nibbles unsigned and takes the
offset as a min term against the feed's block sums (Q4_0's 8 d). The
Q8_0 member has a second stamp for a plane of wide rows that waits on memory (`enc_q8_rows_mma`: 4096 wide and 16 MB at the least): a turn's
four block scales in one load, a row's lane t taking block t's and shuffles handing them around the row. That form loses on the superblock
formats and on a 2B model's Q8_0 planes, narrow or small, whose turn is not waiting on memory (`debug-jit`, the RTX PRO 4500;
`PERF_LEDGER.md`, the 2026-11-13 entry on where the CUDA home's eight-stream tick goes).

### The block drafter's round on the CUDA home keeps its accept on the host {#cuda-dflash-pass}

**The round is the drafter's pass and the verify command on one stream, the recurrent commit beside the pass on a second.**
The pass (`dasllama_cuda_dflash`) stamps the templates Metal stamps (`GkDflashConv`, `GkDflashQkn`, `GkDflashAttn`,
`GkDflashTopk`, `GkDflashWalk`), runs its weight sites through the token chain's GEMV leaves in each plane's own format
(`cud_rows_gemv`), and ends in the device selector - the top-k, then the walk over the two codebooks uploaded in the CPU
form, the picks alone landing (`dflash_device_stats().walked`; `set_dflash_device_selector` lands the lists for the
host's walk, the reference the device walk is held to). The verify lands every row's pick and no logits, and the host
walks the picks for the count (`mtp_seat_accept_walk`): that walk cuts at a stop id, and the commit and the next pass
take the count as launch arguments, so a device accept (Metal's `MetalDflashAccept`, that home's own kernel) would add a
launch and save no wait here. The recurrent rows a round keeps are scanned into the slot on a side stream as the next
pass starts (`cud_rdec_commit_kick`), joined before a command reads the state.

### The tower chains replay from captured graphs {#cuda-chain-graphs}

A tower chain on this home is hundreds of launches - a FastConformer block chain 608 a span (gigaam v3 e2e-rnnt on the
q8 lane, ru_long), a transducer round four or five - and on the reference box (RTX 5060 Ti) the driver's launch call
costs about 13 us of host time (`cuLaunchKernelEx` under `nsys`), near the device time of the small kernels between the
GEMMs, so a chain launched kernel by kernel leaves the GPU waiting on the host.
Vulkan records the same chain into one command buffer and pays no call a dispatch. A seat runs its chain body through
`cut_chain`, naming it by a tag and its shape: the first time the rail's state - every seat's plane, offset and bytes, every
site's pick and bind, the lane, the schedule maps - is seen under that tag and shape, the body launches kernel by kernel;
when it recurs the body is walked dry (`cud_walk`: every launch's function, grid and parameter blob, every rail copy and
zero and every fork, branch and join - `ARCHITECTURE_GPU_TOWER_RAIL.md#rail-primitives` - folded into one hash in order,
nothing launched, the census counted as a launch would), and it replays as one graph launch where the walk equals the one
the graph was captured under, or is captured now and replayed. Where the GPU is the bound (the whisper blocks, gemma4a)
the replay changes nothing; where the launches are (the FastConformer spans, the rounds, gemma4v) it is the encode's
largest lever (`PERF_LEDGER.md`, the audio towers on the CUDA home). A replay is a launch
sequence proven equal to its capture by the parameters themselves, never a cache key assumed complete: two encodes whose
rail states collide but whose launches differ (two spans of different rows over the same tile columns) recapture rather
than replay - so the shape a seat names is what keeps its graphs apart, and a shape left out costs recaptures, never a wrong
replay. A decoder's round chain (`cut_chain_rounds`) replays its rounds from two graphs, eight rounds and one. The
captured chains are a 32-entry cache that starts over past it, dropped with any resident and with any reallocated resident
plane (`cut_realloc`: a captured launch names the freed address); a capture the driver refuses turns the graphs off for the
process with a warning, and `DASLLAMA_CUDA_GRAPHS=0` launches every chain kernel by kernel, said once a process.

### The shared-memory carveout follows the class family {#cuda-carveout}

The policy is per class family, by the class module that stamps the kernel (`cud_class_fn`, the module `[cuda_dispatch]`
passes it). Every kernel asks the whole carveout (`cuda_function_set_shared_carveout(fn, 100)`): the tiles' blocks an SM are
the latency hiding the home has. The one exception is a kernel of the tower families' own class modules
(`dasllama_cuda_classes_front`, `_blocks`, `_asr`) that takes no shared memory: it leaves the split to the driver, which
gives it the L1. Those are the shared templates' row kernels (the log filterbank, the subsample convs' LayerNorm-ReLU rows,
gemma4a's windowed attention), which read their rows thread by thread through L1, and under the full carveout their working
set spilled past it. The LLM window and token classes (`dasllama_cuda_classes`, a tower kernel stamped there included,
since the window chain launches some) keep the full carveout: left to the driver they read 0.4% slower on tg128 on the reference box (RTX 5060 Ti), a cost the
tower's gain does not buy on the served LLM shape (`PERF_LEDGER.md`, the audio towers on the CUDA home). The class-load log line names each kernel's module and
carveout. The test seam `set_cuda_tower_carveout_full` puts the tower modules' no-shared-memory kernels on the full
carveout too and re-applies the policy to every kernel already loaded, so a race compares the two policies with no reload. The Vulkan driver sizes the split per pipeline from its shared-memory use, so the Vulkan home has nothing to set.

### The CUDA home binds its context to the calling thread at every entry {#cuda-thread-binding}

The CUDA driver's current context is a property of the OS thread, not of the process (the dasCUDA module's
`ARCHITECTURE.md`, its per-thread context section): a context made on one thread is unbound on every other. A Vulkan
device has no such binding - any thread submits on it under the queue lock - so the Vulkan home's entry gate checks the
device once. The CUDA home's entry gate, `cud_init`, binds `g_cud.cc` to the calling thread on every call once the
device is ready (`cuda_bind_context`), because a context's `[init]` runs on the thread that made it and a server's
worker thread, or a job on the team, reaches its first device call on another: without the rebind the query answers an
invalid-context error and the worker falls to the CPU with no decline named. The bind is one driver call, cheaper than
any device work behind it.
