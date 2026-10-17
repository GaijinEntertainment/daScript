# dasLLAMA Architecture - runtime mechanisms

Companion to `ARCHITECTURE.md`; a section is cited by its anchor. The CPU worker pool, the MoE
region split, the job queue the engine dispatches on, and single-thread mode are
`ARCHITECTURE_RUNTIME_JOBQUE.md`.

### Kernel SHAPE is compile-time; only DATA is runtime

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

### GPU-resident cache identity

An address-keyed entry carries its SPAN, and a hit covers the request: a shorter first upload never
serves a wider later one. Different upload FORMS (plain span vs concat) live in separate tables so
they can never alias; the Metal `RegionEntry` rail is the model. Buffers grown out of an entry
retire to a list released only at a quiesce boundary - a point where no submitted command buffer is
still in flight - because unretained command buffers may still bind them.

### Making weights live bumps the weights epoch {#weights-epoch-on-load}

Every path that makes weights live bumps the weights epoch, the image rail included. A fresh
mapping or chunk can land on a deleted model's recycled addresses, and an address-keyed region
cache would otherwise serve the previous model's bytes out of an entry that still looks like a
hit.

### Complexity and length lint

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

### Capability questions and readiness questions are different questions

A predicate that mixes them cannot be reused. `prefill_decline_caps` answers "can metal serve this
model" (capability); `prefill_decline` adds "is this window staged" (readiness - are the rope
tables built) on top of it. A caller that runs before the window is staged asks the capability
half: `metal_ple_pre_gpu_gate` calls `prefill_decline_caps` directly, because asking both would
get it "not yet" forever and its feature would silently never run. An optimistic capability answer
is safe here, because the late path has a fallback.

### A quantized activation carries its scale lattice (Vulkan) {#activation-scale-lattice}

Two activation quant forms ride the Vulkan rail, and they differ in the SCALE LATTICE, not the
int8 payload: the Q8_0 form scales per 32 values, the superblock form per 256 (with per-32
sub-scales inside). A compiled kernel indexes ONE lattice - the q8 GEMV/GEMM rail reads per-32
scales; the k-quant (k4/k5/k6/q40) kernels index the per-256 lattice. `kq_sb(fmt)` is the
predicate (`ARCHITECTURE_ENGINE_FORMATS.md#formats-and-data-movement`), and it answers for the WEIGHT plane the dispatch
consumes.

Three consequences the code is shaped around:

- **A quant/act encoder is picked by the CONSUMING plane's format, never by a rail-wide
  default.** Every site that encodes activations for a GEMV/GEMM keys its encoder (and its
  grid: superblock counts are `n/256`) on the consumer's `kq_sb`. The failure mode is silent
  per-dispatch: the wrong lattice indexes garbage scales, outputs stay finite, and nothing
  panics - only end-to-end token parity (`harness/parity.das`) catches it, so a resident
  change is witnessed only by parity runs over both a q8 and a k-quant model.
- **The fused add-rms+requant twin quantizes the consumer's block form per site** (`RqStamp`:
  Q8_0 or Q8_K, the normed row also stored where a consumer reads it as floats), picked once
  every layer is registered (`rd_ensure_fused_sets`); the profiler keys on the stamps' recorded
  names, so no count rides the gate.
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

### Every program root declares its stack budget and its prefill intent

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

### Environment knobs {#env-knobs}

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

### The [hot_path] coverage model {#the-hot-path-coverage-model}

`[hot_path]` sits at the REGION ENTRY - the `*_encode` / `*_decode` / step drivers - and its `[no_alloc]` / `[no_env]` /
`[no_io]` contracts arm transitively down the call graph, so interior kernels stay bare. A new function needs the annotation
itself only when no annotated entry reaches it: a new entry point carries it, and a new backend entry (kernel-backend override,
batch donor) carries it too, because backends are also reached from un-annotated harness paths. Reused buffers take
`@scratch`; debug and profiling legs take `[cold_path]`. The tokenizer encode/decode path is sanctioned UNCOVERED by the
region contracts - its perf gate is the `--tok` scaling rows, whose instrument (the size-ladder ratio) catches what the
contracts cannot. So is the scheduler's tick (`scheduler_step` and its per-token `advance_with`): it builds its events array
and stop-string tails a tick by design, so an entry contract would name its own mechanism; its perf gate is the served-turn
instrument (`harness/served_bench.das`), and its one logging per-token branch - the budget's forced close - is a `[cold_path]` helper.
The batched step's `eval_batch_` (reached through `scheduler_step` and the public `eval_batch`) and the media prefill's
`eval_embd_` are uncovered the same way: each grows its workspace and its row set a call by design, and their perf gates are
the batched-row cells and the served-turn instrument.

### The post-CPU-burn GPU ramp and the residency heartbeat {#the-post-cpu-burn-gpu-ramp-and}

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
