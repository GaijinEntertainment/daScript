# dasLLAMA Architecture - runtime mechanisms

Companion to `ARCHITECTURE.md`; section numbers are that document's.

### 2.2 Kernel SHAPE is compile-time; only DATA is runtime

The test is one question: *for a given compiled kernel, can this value change between dispatches?*

- **Yes -> DATA.** Context depth, row counts, buffer offsets, `kv_dim`, scales, head counts. It
  belongs in a uniform or a kargs struct - the per-dispatch argument struct a kernel binds.
- **No -> SHAPE.** A codec's block stride, a scale-plane stride, a lane width, an unroll factor, a
  format selector. It does not reach the kernel as a uniform, a kargs field, or a helper parameter.

Shape belongs to the specialization: a separate kernel class and PSO, a per-codec overload, a
generic stamped out once per shape, or a `static_if` on a compile-time witness. Handing a shape
constant over as a value and trusting the shader compiler to fold it back is an assumption, not a
guarantee.

**The EMITTED shader is where a shape constant is visible, not the das source.** In the `*_msl`
global or the SPIR-V dump the constant is literal - `blk * 34u`, not `blk * bstr`. A helper that
looks specialized in das can still lower to a runtime multiply.

**kargs structure.** Twins of a family bind the SAME kargs type at the SAME binding, even where one
twin ignores a field. A twin that carries an extra scalar keeps the others in their slots; shifting
them would reach the encoder as a per-form branch. Two signs a kargs field carries a number
the dispatch already has:

1. *A value reaches the encoder twice* - a pooled scalar uniform BUFFER passed alongside the
   identical value as a parameter (`bd` next to `d`). The buffer is uploaded and released per step
   to carry a number the encoder already holds.
2. *A field is a function of the fields beside it.* An expert plane's block stride is
   `kdim * ndim / blocksize`; a reciprocal scale is `1/sqrt(dim)`. The builder derives it - each
   one passed separately is a second place to get it wrong. Likewise, when a kernel's grid IS the
   geometry it reads, the grid carries the numbers and the kargs do not repeat them.

**Nothing dispatches a kernel except its `enc_*` builder.** A hand-rolled bind list elsewhere - a
tune-race harness, a benchmark, a probe - duplicates the builder and desyncs the moment the
family's args change, silently: the slots still exist, the types still compile, and the kernel
reads a struct out of a 4-byte buffer. The dispatch census only catches a builder that binds kargs
on some paths and not others; a duplicate that binds NO kargs is invisible to it.

### 2.3 GPU-resident cache identity

An address-keyed entry carries its SPAN, and a hit covers the request: a shorter first upload never
serves a wider later one. Different upload FORMS (plain span vs concat) live in separate tables so
they can never alias; the Metal `RegionEntry` rail is the model. Buffers grown out of an entry
retire to a list released only at a quiesce boundary - a point where no submitted command buffer is
still in flight - because unretained command buffers may still bind them.

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

A split lands only where a real seam exists - genuine duplication, a distinct phase, a
self-contained arm - and only when the extracted helper stands on its own. One corollary:
**merging kargs fields into a kernel body that is already over the cap is not a reason to abandon
the merge** - unpacking N fields adds N lines, so the growth is taken and the real seam goes to
the module's follow-up ledger.

### 2.6 Capability questions and readiness questions are different questions

A predicate that mixes them cannot be reused. `prefill_decline_caps` answers "can metal serve this
model" (capability); `prefill_decline` adds "is this window staged" (readiness - are the rope
tables built) on top of it. A caller that runs before the window is staged asks the capability
half: `metal_ple_pre_gpu_gate` calls `prefill_decline_caps` directly, because asking both would
get it "not yet" forever and its feature would silently never run. An optimistic capability answer
is safe here, because the late path has a fallback.

### 2.7 A quantized activation carries its scale lattice (Vulkan) {#activation-scale-lattice}

Two activation quant forms ride the Vulkan rail, and they differ in the SCALE LATTICE, not the
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
  shape rides the SAME gate; on a different gate the profiles desync from what actually
  dispatched.
- **The activation group - the dispatches that read one activation buffer, never a layer - is the
  decision unit: its members share one lattice, and its feed is picked once for all of them.**
  q/k/v share one quantized x; gate/up share another. Resident arming classifies each member's
  consumer form and DECLINES a mixed group rather than serving one member wrong scales. The
  prefill f16 feed answers the same question one step further: one buffer serves every GEMM of a
  group, so the f16 form engages only when EVERY member is tile-servable (the cm2 decode-in-load
  tile, or the KHR kq tile) - one member on the quant route pins its whole group to the quant
  feed. The recurrent head (a deltanet block) holds two groups, not one: its qkv and z GEMMs read
  the block's input rows, and its out GEMM reads the scan's output rows - the raw rows the
  delta-rule scan writes. So the two feeds are decided apart, and a Q8_0 out plane decides the out
  feed alone: it leaves the qkv and z planes on the tiles.

### 2.8 Every program root declares its stack budget and its prefill intent

`options stack` is main-module-only: it does not unify up from required modules, so no library in
the forward chain can declare the depth it needs. Every program that drives the engine - each test,
harness, benchmark, and tool - declares it instead, and dasLLAMA's frames are deep enough
(by-value `Session`s, the forward/prefill chain, the generated kernel tier) that the default is
never enough.

The budget is **one number in every root**, 524288. Per-root numbers do not survive: a frame that
grows past the smallest declared budget breaks only the program that declared least, so the limit
is discovered by crashing - and the program that crashes is whichever one is run rarest. A
measurement rig sized below a test suite is the worst case of this, because the suite stays green
while the rig dies. The cost of the uniform number is reserved address space per context; the cost
of per-root numbers is a runtime crash found by the least-covered program.

The second declaration is prefill intent. A model-loading root declares `allow_cpu_prefill()`
on the arms that hit the CPU-prefill guard - `set_metal_mode` with a runtime value declares
nothing, since `MetalMode.off` leaves the guard armed - and logs, once, which configuration it
ended on. The guard panics, and a panic takes every live stream down: an undeclared root serves
normally until its first long prompt. Both halves of root discipline are enforced by
`tests/test_program_roots.das`.

### 2.9 Environment knobs {#env-knobs}

A knob is an `[EnvConfig]` field in `dasllama_env.das`, read as `g_env_*.<field>`; the field is
also what generates its `ENVIRONMENT.md` row, so a knob declared anywhere else is invisible to
the documentation and to the registry test. The sanctioned forms beyond a plain typed field:

- **Tri-state knobs** - presence matters, or the effective default is computed at runtime - are
  `Option<T>` fields.
- **Dynamic names** - a variable named by data, not by code - go through `env_is_set` /
  `env_value_of`; there is no field to declare because the name is not known at compile time.
- **The config loads once at context init**, so `set_env_variable` mid-process is invisible to
  the running config; the environment a child process inherits is the way to change it.
- **A write of a foreign library's knob** (`set_env_variable` with a literal name) is allowed
  only before that library first reads it, and its name is a declared `[EnvConfig]` knob - the
  registry test scans writes too, so a re-spelled name fails it.

`tests/test_env_registry.das` enforces the lot in both directions (declared <-> documented,
read <-> registered, writes included).

**Override announces.** `DASLLAMA_ALLOW_UNTUNED`, and the `DAS_TUNE_NOISE_*` pair and
`DAS_TUNE_POLICY` that llvm_tune's accessors read, each announce at every site where they
change the outcome - the `[init]` wire, the gate they bypass, a tool banner - and stay silent
when set but inert. Thread counts, rail selection and format knobs report their state in the
tools' existing config and status lines instead.

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

### 2.12 The post-CPU-burn GPU ramp and the residency heartbeat

After a CPU-only phase, the first Metal submission runs degraded. The cost is paid once per idle
phase, and all of it sits in the kernel-side driver span (kernelStart->kernelEnd); the queue
hand-off and the GPU execution stay flat. A longer idle phase costs more, because the OS collects
a committed+requested `MTLResidencySet` during inactivity.

Two mechanisms cut the ramp. The `MTLResidencySet` pin (`DASLLAMA_METAL_RESIDENCY`, on by
default) covers a short idle phase: it saves 15 ms on the tower's first submission after a
CPU-only stretch. The residency HEARTBEAT covers the long ones - a dasMetal background thread
re-requests residency every 5 ms for `DASLLAMA_METAL_HEARTBEAT_S` (default 180 s; 0 turns it off,
the A/B comparison setting) after the last served step, kicked from `residency_flush`. On the
qwen3v tower encode, a 3000 ms burn reads 17.7 ms of driver time without the heartbeat and 3.0 ms
with it.

A warm-up does not help: work that pre-pays the cost pays it in full, on top of the delay it was
meant to remove. Prefill pools therefore stay tracked. The residual - 1 to 3 ms, and larger the
longer the idle phase - is the driver and GPU waking from an idle power state, and no user-space
call reaches it. Arming Metal at all makes a CPU q8 tower encode ~1.7x slower; the cause is
unknown, and a GPU-served tower removes the cost. `PERF_LEDGER.md`'s heartbeat entry carries the
measurements behind this section and each refuted attempt.

### 2.18 The CPU worker pool on a hybrid box {#hybrid-pool-policy}

SMT siblings share the FMA and load ports, so the default pool is (physical cores - 1) workers -
a cap the engine's `[init]` sets on every platform but a browser, where the reported count can be
a fingerprint cap of two and the runtime's own floor and cap size the pool (2.18a). A box
with two core tiers splits on the SECOND tier's KIND: a compute tier (an M5's Super plus Performance
cores) extends the pool to every core with GEMV capped to the fast tier, while an efficiency tier
(M1, M4) makes batch barriers wait and gets no worker at all. A compute-grade second tier only
exists beside a KNOWN fast tier - the `[init]` extension reads both, so a box reporting one
without the other has an inconsistent topology, not a third policy.

Chunk-starved slow-tier workers spinning beside the fast decode lanes cost ~15% of tg (tokens
generated per second) on an M5 Max (the decode-only rep probe of `benchmarks/lcpp_bench.das`),
and the team rank gate - which admits only as many workers to a team op as its widest stage
published chunks - does not recover it; only parking does. The pool is therefore phase-shaped: a
decode step parks the slow tier (`dispatch_phase_decode`), and batch-shaped work of 32 rows or
more wakes every worker - a prefill through `dispatch_phase_batch`, a CPU tower/ASR encode
through `dispatch_phase_encode(rows)` at its entry - while smaller work keeps the parked pool,
since it cannot fill the slow tier past the dispatch grain.

Precedence, strongest first: `DAS_JOBQUE_THREADS` and an app's own cap, then the box profile's
`jobque_pool` / `phase_decode_workers` / `dispatch_worker_limit` entries, then this tier-kind
policy. A profile declares the shape of the NEXT queue and is inert for one that already exists when
the profile loads. A `gemv_lane_cap` of 0 in a minted profile - one the tuner wrote from a
measured race - records the fast-tier-only "uncapped" default rather than a measured choice, so a
hybrid box ignores it and keeps the fast-tier cap; a non-zero value is a measured choice and
applies.

### 2.18a A browser's workers park {#browser-workers-park}

The spin-before-park window (`g_jobque_spin_us`, 30 ms on a desktop) keeps a worker hot through
the serial gaps of a token; in a browser the workers are web workers, and one that spins holds a
core the caller's own thread is competing for, so the window costs more than the wake it saves.
Pocket TTS in Chrome (wasm64, eight workers in the pool) generates at 1.1x real time with the
window and the renderer at 800% CPU, and at 1.4x with the workers parked at ~112% - the same
text, three runs each; team dispatch keeps its small edge there, and the pool still pays (one
worker reads 0.7x). The engine's `[init]` therefore sets the window to 0 when the platform is
emscripten (`dasllama_jobque_spin_default`); a box profile's `jobque_spin_us` and the setter still
override it, and a program reads the value in force through `get_jobque_spin_us`. The pool's size
is the runtime's (`src/misc/job_que.cpp`): the browser's reported cores, capped and floored by
`DAS_MAX_HW_JOBS` and `DAS_MIN_WEB_JOBS` (eight and four unless the build overrides them), minus
one for the computing main thread - seven workers on a real box, the pool
an eight-core desktop runs, and three under fingerprint protection, where a browser reports two
cores whatever the box has; the floor is what keeps such a visitor off a one-worker pool.

### 2.19 The CPU MoE region list caps a region at 32 rows {#moe-region-split}

The grouped MoE prefill hands its expert regions to the batch dispatcher as (weight offset, first
row, count) triples. That dispatcher chunks units by COUNT, not by work, so one heavy expert in a
skewed routing draw becomes one unit the barrier waits on - measured 620 against 3929 GFLOP/s on a
Zipf k4 draw (`harness/moe_kq_probe.das`). The CPU arms therefore split a region into sub-regions
of at most 32 rows: sub-regions of one expert share its weight offset, and the per-expert bias
lists repeat once per sub-region. The GPU arms keep whole regions - their kernels chunk by work
already.

### 2.44 The engine dispatches only on a configured job queue {#jobque-policy}

A queue from a bare `create_job_que` clones and destroys a fork context per job, wakes one worker
per push, and parks a worker the moment its job ends. The engine's fork/join dispatch issues ~160
small dispatches per decoded token, so on that queue a q8 forward step of gemma-4 E2B takes 3.6 s
against 16 ms configured - the whole gap, not a fraction of it. Each of the four knobs in
`setup_dasllama_jobque()` removes one factor: with team mode turned back off the step still takes
2.0 s, and with the fork pool, batched dispatch or the spin window turned back off it takes ~25 ms
each. The pool of fork contexts is the observable: `set_jobque_fork_pool` is per context and
outlives the queue, and the setup call always turns it on. A caller that sets the pool directly,
as the dispatch probes and benches do, has configured its queue too.

The first counted dispatch of a process reads that pool, once. A queue without it panics, naming
the setup call, and `DASLLAMA_ALLOW_BARE_JOBQUE=1` downgrades the panic to a warning for a run
that means to measure the bare regime. A process with no queue at all runs its arms inline and is
not checked. The engine's own scoped queues - the ones `load_gguf` and `load_gguf_streaming` spin
for a caller that has none - configure themselves, so the check reaches only queues a caller
opened.
