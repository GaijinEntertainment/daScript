# dasLLAMA Architecture - runtime mechanisms

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.2 Kernel SHAPE is compile-time; only DATA is runtime

The test is one question: *for a given compiled kernel, can this value change between dispatches?*

- **Yes -> DATA.** Context depth, row counts, buffer offsets, `kv_dim`, scales, head counts. It
  belongs in a uniform or a kargs struct.
- **No -> SHAPE.** A codec's block stride, a scale-plane stride, a lane width, an unroll factor, a
  format selector. It must NOT reach the kernel as a uniform, a kargs field, or a helper parameter.

Shape belongs to the specialization: a separate kernel class and PSO, a per-codec overload, a
monomorphized generic, or a `static_if` on a compile-time witness. Handing a shape constant over as
a value and trusting the shader compiler to fold it back is an assumption, not a guarantee, and it
is worth nothing in the kernels that matter.

**Verify against the EMITTED shader, never the das source.** Read the `*_msl` global or the SPIR-V
dump and confirm the constant is literal there: `blk * 34u`, not `blk * bstr`. A helper that looks
specialized in das can still lower to a runtime multiply.

**kargs structure.** Twins of a family bind the SAME kargs type at the SAME binding, even where one
twin ignores a field. A twin that carries an extra scalar must not shift the others to different
slots, because that asymmetry propagates into the encoder as a per-form branch. Two tells that a
fold is overdue:

1. *A value reaches the encoder twice* - a pooled scalar uniform BUFFER passed alongside the
   identical value as a parameter (`bd` next to `d`). The buffer is uploaded and released per step
   to carry a number the encoder already holds.
2. *A field is a function of the fields beside it.* An expert plane's block stride is
   `kdim * ndim / blocksize`; a reciprocal scale is `1/sqrt(dim)`. Derive it in the builder - each
   one passed separately is a second place to get it wrong. Likewise, when a kernel's grid IS the
   geometry it reads, take the grid off the kargs rather than re-passing the numbers.

**Nothing dispatches a kernel except its `enc_*` builder.** A hand-rolled bind list elsewhere - a
tune-race harness, a benchmark, a probe - duplicates the builder and desyncs the moment the
family's args change, silently: the slots still exist, the types still compile, and the kernel
reads a struct out of a 4-byte buffer. The dispatch census only catches a builder that binds kargs
on some paths and not others; a duplicate that binds NO kargs is invisible to it.

### 2.3 GPU-resident cache identity

An address-keyed entry carries its SPAN, and a hit must cover the request - a shorter first upload
must never serve a wider later one. Different upload FORMS (plain span vs concat) live in separate
tables so they can never alias; the metal `RegionEntry` rail is the model. Buffers grown out of an
entry retire to a list released only at quiesce boundaries, because unretained command buffers may
still bind them.

### 2.3a Making weights live bumps the weights epoch {#weights-epoch-on-load}

Every path that makes weights live bumps the weights epoch, the image rail included. A fresh
mapping or chunk can land on a deleted model's recycled addresses, and an address-keyed region
cache would otherwise serve the previous model's bytes out of an entry that still looks like a
hit.

### 2.4 Complexity and length lint

STYLE037 (cyclomatic) and STYLE038 (line count) are prompts to look, not orders to refactor. This
module has shapes that are irreducible by design and they take `// nolint:STYLE03x` with a one-line
reason: flat one-call-per-item runs (a registration or release list with one line per kernel), and
GPU kernel bodies whose phases are coupled by barriers, cooperative-matrix ops or register
residency and so cannot cross a function boundary without changing the shader.

Split only where a real seam exists - genuine duplication, a distinct phase, a self-contained arm -
and only when the extracted helper stands on its own. Two corollaries this module keeps tripping
over: **a kargs fold that grows an already-over-cap kernel body is not a reason to abandon the
fold** - unpacking N fields adds N lines; take the growth and ledger the real seam.

### 2.6 Capability questions and readiness questions are different questions

A predicate that mixes them cannot be reused. `prefill_decline` answers "can metal serve this
model" (capability) *and* "is this window staged" (readiness - are the rope tables built). A caller
that runs before the window is staged must ask the capability half only, or it gets "not yet"
forever and its feature silently never runs. Split such predicates rather than reordering the
caller; an optimistic capability answer is safe when the late path has a fallback, and here it does.

### 2.7 A quantized activation carries its scale lattice (Vulkan) {#activation-scale-lattice}

Two activation quant forms ride the vulkan rail, and they differ in the SCALE LATTICE, not the
int8 payload: the Q8_0 form scales per 32 values, the superblock form per 256 (with per-32
sub-scales inside). A compiled kernel indexes ONE lattice - the q8 GEMV/GEMM rail reads per-32
scales; the k-quant (k4/k5/k6/q40) kernels index the per-256 lattice. `kq_sb(fmt)` is the
predicate (`ARCHITECTURE_ENGINE.md` sec.1.2), and it answers for the WEIGHT plane the dispatch
consumes.

Three consequences the code is shaped around:

- **A quant/act encoder is picked by the CONSUMING plane's format, never by a rail-wide
  default.** Every site that encodes activations for a GEMV/GEMM keys its encoder (and its
  grid: superblock counts are `n/256`) on the consumer's `kq_sb`. The failure mode is silent
  per-dispatch: the wrong lattice indexes garbage scales, outputs stay finite, and nothing
  panics - only end-to-end token parity (`harness/parity.das`) catches it, so a resident
  change is witnessed only by parity runs over both a q8 and a k-quant model.
- **The fused add-rms+requant twin exists only for the per-32 form.** The rail gates it on
  "every consumer of this buffer is Q8_0-scaled" (`rd_x_quants_b32`), and the profiler stamp
  shape must ride the SAME gate, or profiles desync from what actually dispatched.
- **A GEMV group sharing one activation buffer must be lattice-homogeneous.** q/k/v share one
  quantized x; gate/up share another. Resident arming classifies each member's consumer form
  and DECLINES a mixed group rather than serving one member wrong scales. The prefill f16 feed
  answers the same question one step further: one activation buffer serves every GEMM of a
  group, so the f16 (cm2 decode-in-load) form engages only when EVERY member of the group is
  cm2-servable - one member on the quant route pins its whole group to the quant feed.

### 2.8 Every program root declares its stack budget and its prefill intent

`options stack` is main-module-only: it does not unify up from required modules, so no library in
the forward chain can declare the depth it needs. Every program that drives the engine - each test,
harness, benchmark, and tool - must therefore declare it, and dasLLAMA's frames are deep enough
(by-value `Session`s, the forward/prefill chain, the generated kernel tier) that the default is
never enough.

The budget is **one number in every root**, currently 524288. Per-root numbers do not survive: a
frame that grows past the smallest declared budget breaks only the program that declared least, so
the limit is discovered by crashing - and the program that crashes is whichever one is run
rarest. A measurement rig sized below a test suite is the worst case of this, because the suite
stays green while the rig dies. The cost of the uniform number is reserved address space per
context; the cost of per-root numbers is a runtime crash found by the least-covered program.

The second declaration is prefill intent. A model-loading root declares `allow_cpu_prefill()`
on the arms that hit the CPU-prefill guard - `set_metal_mode` with a runtime value declares
nothing, since `MetalMode.off` leaves the guard armed - and logs, once, which configuration it
ended on. The guard panics, and a panic takes every live stream down, so an undeclared root is
a serving outage waiting on its first long prompt. Both halves of root discipline are enforced
by `tests/test_program_roots.das`.

### 2.9 Environment knobs {#env-knobs}

A knob is an `[EnvConfig]` field in `dasllama_env.das`, read as `g_env_*.<field>`; the field is
also what generates its `ENVIRONMENT.md` row, so a knob declared anywhere else is invisible to
the documentation and to the registry test. The sanctioned forms beyond a plain typed field:

- **Tri-state knobs** - presence matters, or the effective default is computed at runtime - are
  `Option<T>` fields.
- **Dynamic names** - a variable named by data, not by code - go through `env_is_set` /
  `env_value_of`; there is no field to declare because the name is not known at compile time.
- **The config loads once at context init**, so `set_env_variable` mid-process is invisible to
  the running config: arm a child process's environment instead.
- **A write of a foreign library's knob** (`set_env_variable` with a literal name) is allowed
  only before that library first reads it, and the name must be a declared `[EnvConfig]` knob -
  the registry test scans writes too, so a re-spelled name fails it.

`tests/test_env_registry.das` enforces the lot in both directions (declared <-> documented,
read <-> registered, writes included).

**Override announces.** A knob that is a gate escape, a policy override, or a threshold
recalibration - one whose presence makes a run measure, mint, or emit something its defaults
would not (`DASLLAMA_ALLOW_UNTUNED`, the `DAS_TUNE_NOISE_*` pair and `DAS_TUNE_POLICY` read
through llvm_tune's accessors) - announces at the point it changes the outcome: at least one
printed or logged line naming the knob by its env spelling. Set-but-inert stays silent;
per-site repeats are correct (the same knob may legitimately announce at its `[init]` wire, at
a gate it bypasses, and in a tool banner). Ordinary behavior knobs (thread counts, rail
selection, formats) are not overrides under this contract - their state belongs in the tools'
existing config/status lines, not in per-knob announces.

### 2.11 The [hot_path] coverage model

`[hot_path]` sits at the REGION ENTRY - the `*_encode` / `*_decode` / step drivers - and its
`[no_alloc]` / `[no_env]` / `[no_io]` contracts arm transitively down the call graph, so
interior kernels stay bare. A new function needs the annotation itself only when no annotated
entry reaches it: a new entry point carries it, and a new backend entry (kernel-backend
override, batch donor) carries it too, because backends are also reached from un-annotated
harness paths. Reused buffers take `@scratch`; debug and profiling legs take `[cold_path]`.
The tokenizer encode/decode path is sanctioned UNCOVERED by the region contracts - its perf
gate is the `--tok` scaling rows, whose instrument (the size-ladder ratio) catches what the
contracts cannot.

### 2.12 The post-CPU-burn GPU ramp - the residency heartbeat holds it; do not build a warm-up

Figures in this section: M1 Max, 2026-08-23 - probes are quiet `-jit` runs with the rig's
tune manifest; cells are the released `lcpp_bench` exe (`--image --image-think -r 3 -t 8
--ngl 99`); full tables in `history/dasLLAMA/qwen3vl_plan.md` slices M/J.

After a CPU-only phase, the first Metal submission runs degraded - one time per window,
entirely in the kernel-side driver window (kernelStart->kernelEnd; queue hand-off and GPU
execution stay flat - split probe-verified 2026-08-23, `harness/residency_ramp_probe.das` +
`das_metal_boost`'s `metal_submit_trace`). The long-window mechanism is the OS collecting a
committed+requested `MTLResidencySet` during inactivity; the fix is the
residency HEARTBEAT - a dasMetal background thread re-requesting
residency every 5 ms for `DASLLAMA_METAL_HEARTBEAT_S` (default 180 s, 0 = A/B rail) after
the last served step, kicked from `residency_flush`. Measured on the qwen3v tower encode:
3000 ms burn drv 17.7 -> 3.0 ms. The `MTLResidencySet` pin itself
(`DASLLAMA_METAL_RESIDENCY`, on by default) holds the short-window case (-15 ms/encode on
the tower's first submission after its CPU stem); the heartbeat holds the long ones.
REFUTED by measurement (`PERF_LEDGER.md`, the heartbeat entry): every pre-payment - empty
command buffers and driver round-trips (0.107 ms - never asleep), single-dispatch kernels,
per-page touch kernels, light pulse-trains through the burn - a warm-up always pays its own
cost ON TOP of the slack it was meant to hide; do not re-attempt one. Also refuted: a
whole-map no-copy anchor buffer (page pre-wiring moves nothing) and the once-proposed
per-buffer tracking audit (resource count is not the lever) - prefill pools simply stay
tracked (untracked reds parity). The ~1-3 ms window-scaled residual is the driver/GPU
idle-state wake class, no user-space lever found. Related, same ledger: merely arming Metal
makes a CPU q8 tower encode ~1.7x slower - mechanism unnamed, deleted by a GPU-served tower.


### 2.18 The CPU worker pool on a hybrid box {#hybrid-pool-policy}

SMT siblings share the FMA and load ports, so the default pool is physical cores - 1 workers. A box
with two core tiers splits on the SECOND tier's KIND: a compute tier (an M5's Super plus Performance
cores) extends the pool to every core with GEMV capped to the fast tier, while an efficiency tier
(M1, M4) straggles batch barriers and gets no worker at all. A compute-grade second tier only
exists beside a KNOWN fast tier - the `[init]` extension reads both, so a box reporting one
without the other has an inconsistent topology, not a third policy.

Chunk-starved slow-tier workers spinning beside the fast decode lanes cost ~15% of tg on an M5 Max
(the decode-only rep probe of `benchmarks/lcpp_bench.das`), and the rank gate does not recover it -
only parking does. The pool is therefore phase-shaped: a decode step parks the slow tier
(`dispatch_phase_decode`), and batch-shaped work of 32 rows or more wakes every worker - a prefill
through `dispatch_phase_batch`, a CPU tower/ASR encode through `dispatch_phase_encode(rows)` at its
entry - while smaller work keeps the parked pool, since it cannot fill the slow tier past the
dispatch grain.

Precedence, strongest first: `DAS_JOBQUE_THREADS` and an app's own cap, then the box profile's
`jobque_pool` / `phase_decode_workers` / `dispatch_worker_limit` entries, then this tier-kind
policy. A profile declares the shape of the NEXT queue and is inert for one that already exists when
the profile loads. A minted `gemv_lane_cap` of 0 records the old fast-tier-only "uncapped" rather
than a measured choice, so a hybrid box ignores it and keeps the fast-tier cap; a non-zero mint is
an opinion and applies.

### 2.19 The CPU MoE region list caps a region at 32 rows {#moe-region-split}

The grouped MoE prefill hands its expert regions to the batch dispatcher as (weight offset, first
row, count) triples. That dispatcher chunks units by COUNT, not by work, so one heavy expert in a
skewed routing draw becomes one unit that straggles the barrier - measured 620 against 3929 GFLOP/s
on a zipf k4 draw (`harness/moe_kq_probe.das`). The CPU arms therefore split a region into sub-regions of at most 32 rows:
sub-regions of one expert share its weight offset, and the per-expert bias lists repeat once per
sub-region. The GPU arms keep whole regions - their kernels chunk by work already.

### 2.20 The engine dispatches only on a configured job queue {#jobque-policy}

A queue as `create_job_que` makes it clones and destroys a fork context per job, wakes one worker
per push, and parks a worker the moment its job ends. The engine's fork/join dispatch issues ~160
small dispatches per decoded token, so on that queue an E2B q8 forward step takes 3.6 s against
16 ms configured - the whole gap, not a fraction of it, and each of the four knobs in
`setup_dasllama_jobque()` removes one factor (team mode alone leaves 2.0 s; the fork pool,
batched dispatch or the spin window alone each leave ~25 ms). The pool of fork contexts is the
observable: `set_jobque_fork_pool` is per context and outlives the queue, and the setup call
always turns it on. A caller that sets the pool directly, as the dispatch probes and benches do,
has configured its queue by the same token.

The first counted dispatch of a process therefore reads it, once: a queue without the pool panics
naming the setup call, and `DASLLAMA_ALLOW_BARE_JOBQUE=1` downgrades that to a warning for a run
that means to measure the bare regime. A process with no queue at all runs its arms inline and is
not checked. The engine's own scoped queues - the ones `load_gguf` and `load_gguf_streaming` spin
for a caller that has none - configure themselves, so the rule reaches only queues a caller opened.
