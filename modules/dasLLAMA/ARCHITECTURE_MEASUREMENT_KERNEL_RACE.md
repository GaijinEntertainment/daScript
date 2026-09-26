# dasLLAMA Architecture - the kernel race and bench instruments

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This
document carries the instruments that time a kernel away from the
served graph and the conditions their verdict holds under: what makes an isolated race predict
the graph it imitates, how the gemv earns a second tune seat, and the fixture the CPU kernel
bench times on. It also carries the vocabulary of the timing-race checklist. The rig that
produces recorded numbers, the tune gate, the Vulkan GEMM probe's axes and the instrumentation
rails stay in `ARCHITECTURE_MEASUREMENT.md`.

### The timing-race vocabulary

`REVIEW_GPU_RACE.md` reviews code that times kernels to rank them. A hazard between serving
dispatches - a data race - is not its subject. Its terms:

- **Race** - two candidates for one computation, timed in one process, either of which the run
  could adopt.
- **Knockout** - a timing that skips a stage to measure that stage's cost. A run of the serving
  code under a stage-drop knob - a knob whose runs do not serve correct output - is a knockout:
  the branch that reads the knob and drops the stage is arm code, whoever sets the knob, and the
  stage it drops stays serving code, reviewed as such.
- **Overhead measurement** - one chain timed with and without an interposed stage (a timestamp,
  a barrier, a flush) to measure that stage. It is not a race.
- **Timing arm** (arm) - one timed run of a race, a knockout or an overhead measurement: code
  that dispatches a kernel to measure it rather than to serve a call. A same-build knob A/B -
  two runs of the serving code under an environment override, both serving correct output - is
  not an arm; its knob is serving code, reviewed under `REVIEW_GPU_VULKAN.md` or
  `REVIEW_TOWER.md`.
- **Chain** - the dispatches an arm times.
- **Ranking input** - a value a ranking's selection reads: a shape dimension (a head width, a
  batch width, a row count, a lane split, a tile's own width, the workgroups one row dispatches
  relative to the device's compute-unit count) or a layer kind (dense, MoE, per-layer-embedding).
- **Decided ranking** - an arm's ranking is decided while a checked-in document, box profile or
  sidecar records the arm's figure or names the arm as the shipped form. Decided is a state of
  the tree; a diff decides a ranking when it adds that record.
- **Retained-reference arm** - an arm ledgered as a retained reference: in
  `ARCHITECTURE_GPU_RACE_SHAPES.md#tensor-gemm-shapes-that-measured-out-m5` for Metal, in the
  Vulkan arm ledger for Vulkan.
- **Vulkan arm ledger** - the GEMM probe's arms in
  `ARCHITECTURE_MEASUREMENT_VK_GEMM_PROBE.md#vk-gemm-probe`, the decode rulers' in
  `ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`.

### An isolated kernel race is only as good as the graph it imitates {#kernel-race-fidelity}

A kernel A/B race times two spellings of one compute on a synthetic fixture. Its winner is
minted as a crown - the family's name in the box sidecar's `runtime.metal_tensor` set, which
makes every later run on that box serve the twin in place of the base kernel
(`metal_tensor_crowned`, `dasllama/dasllama_common.das`). Three conditions decide whether its
winner is the winner the served graph would pick, and a race missing any of them mints a wrong
crown:

- **Overlap.** The served graph's consecutive GEMVs write DIFFERENT output buffers, so the
  scheduler overlaps them. A race chaining every dispatch through one output buffer serializes
  on the write-after-write hazard; on the iq2xxs f4-slab twin that one difference reads +9% for
  an arm the served graph rejects at -8.6%.
- **A warmed clock.** An Apple GPU's clock governor ramps under load, and `race_pair_ms` runs
  base-then-twin per round, so the first side pays the ramp the second rides. About 150 ms of
  GPU work before the first timed round removes the bias; back-to-back dispatches inside each
  timed encoder hold the clock there. A sparse invocation without the burn reads idle-clock
  times and can flip a verdict outright.
- **A real site shape.** The race sizes its operands at a real model shape. A slab small enough
  to sit in cache ranks the kernels by an effect production never sees, and the race then picks
  the slower kernel.

A race arm owns a transient command queue for its timed pairs and releases it before returning -
the one exception to the family's shared device and queue (`ARCHITECTURE_GPU.md#gpu-backends`) - so
the tune-time race never queues behind served work.

Even a race meeting all three can be structurally blind. `kq_gemv_iq2xxs_f4` is the standing
case: every isolated regime crowns the twin and the served decode graph loses 8.6% with it,
because the effect is mixed-stream occupancy - it exists only when the GEMV runs beside the
rest of the token step. That crown is therefore NOT auto-raced. It is minted on the real shapes
by `harness/tune_kernels.das`'s serving confirm (its `benches` list keeps `dot_q8q8_laneq4x4`
last, since that bench pins one matmul backend for the rest of the process - `harness/REVIEW.das`'s
`check_last_bench_row` holds the order, licensing no names): two temporary manifests differing only in
`runtime.metal_tensor` membership, each served to
`benchmarks/lcpp_bench.das --for-debug-purposes --ngl 99 -p 32 -n 128 -r 3` under
`DAS_TUNE_MANIFEST`, the tg128 line the verdict. Both arms carry an EMPTY `kernels` section, so
they stamp identical fallbacks and the tg delta isolates the crown under test. The margin is
`CONFIRM_TG_MARGIN` = 1.005 over the mean of the run's three timed reps - the `tg128:` line the
bench prints: the crown's serving win where it wins is +0.6% (m5) and its serving loss where it
loses is -8.6% (m4), so the margin only has to clear run noise, and a spuriously minted crown
costs a re-mint rather than a board row. No IQ2_XXS vehicle on the box
means no crown - the base kernel is the safe side - and the run says so with a provisioning
hint.

The per-format isolated rig is `benchmarks/matmul/bench_metal_kq_race.das`: synthetic planes,
no model, no tuner, every arm gated against a CPU plane-dequant oracle before it is timed, and
`--burn-ms` (default 150) spent on GPU work before each cell's first timed round. Its cells
chain every dispatch through ONE shared output buffer on purpose - the serialized regime is
the instrument's probe shape, imitating the reference tool it is compared against - and its
numbers reach the engine only through a human porting decision, never a minted crown.

### The gemv takes its own tune seat {#gemv-seat}

A kq family's manifest entry is its tile-best row, and the gemv gets a SECOND entry when a
different row serves the streamed decode better. Only same-mr rows can differ, because the
layout companion pins the plane's interleave; of those the two best by tile time race, the
winner takes the gemv only by the margin over the tile winner's own gemv, and the incumbent
keeps a tie. Every family's perm grid therefore carries a 256-wide `mr = 16` alternate beside
its 512-wide tile crown. The seat is decided at the engine's decode shape - a DRAM-bound plane
streamed by every lane through the engine's own splitter - because the engine's row length moves
the answer (k3 on Granite Rapids: the 256 seat wins at n=2048 and loses at 14336 -
`benchmarks/matmul/kq_kernel_bench.das`, tune mode, seats pinned, d=32768). The seat fixture is
a 512-row build at the ffn width tiled 320 times, past the largest L3 a socket lends a slice of,
and the seat takes the MEDIAN of seven rounds, which discards a round that finds the plane in
L3. In normal mode `llvm_tune` stamps a companion from its own manifest entry when one exists
and is a perm this box can run, else from the tile's.

### The CPU kernel bench's fixture conditions {#cpu-kernel-bench-fixture}

`benchmarks/matmul/kq_kernel_bench.das` times raw kernels on synthetic planes, and three
fixture properties decide whether its numbers mean anything. Every plane of one format lives in
ONE arena at fixed offsets, staggered so no two starts share their low 12 address bits: the heap
places separate arrays at run-dependent relative addresses, and planes that alias in the L1/L2
set logic make a run's time depend on where the heap put them. Scale planes are filled with a
byte that is a normal number in every scale form, never random bytes, because denormal math runs
orders of magnitude slower. Each row is warmed before it is timed - three unmeasured rounds
solo, six dispatches per row on the team arm - because a core ramps over several rounds and one
warm call is not enough. The q8 row exists in two flavors: f32 group scales (the engine's own
quantization) and `q8s16` over binary16 scales - the wscale_f16 rail a GGUF q8_0 tensor runs,
and the like-for-like row against the reference's q8_0.
Provenance for every figure in this section: `benchmarks/matmul/kq_kernel_bench.das` under
`DAS_TUNE_MODE=tune`, one thread, its default `--fmt` / `-n` / `-d` shape.
