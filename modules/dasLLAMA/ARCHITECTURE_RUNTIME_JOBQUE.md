# dasLLAMA Architecture - runtime mechanisms, the CPU worker pool and the job queue

Companion to `ARCHITECTURE_RUNTIME.md`; a section is cited by its anchor.

### The CPU worker pool on a hybrid box {#hybrid-pool-policy}

SMT siblings share the FMA and load ports, so the default pool is (physical cores - 1) workers -
a cap the engine's `[init]` sets on every platform but a browser, where the reported count can be
a fingerprint cap of two and the runtime's own floor and cap size the pool
(`ARCHITECTURE_RUNTIME_JOBQUE.md#browser-workers-spin`). A box
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

Precedence, strongest first: single-thread mode (`ARCHITECTURE_RUNTIME_JOBQUE.md#single-thread`), under which both phase entries do
nothing at all, then `DAS_JOBQUE_THREADS` and an app's own cap, then the box profile's
`jobque_pool` / `phase_decode_workers` / `dispatch_worker_limit` entries, then this tier-kind
policy. A profile declares the shape of the NEXT queue and is inert for one that already exists when
the profile loads. A `gemv_lane_cap` of 0 in a minted profile - one the tuner wrote from a
measured race - records the fast-tier-only "uncapped" default rather than a measured choice, so a
hybrid box ignores it and keeps the fast-tier cap; a non-zero value is a measured choice and
applies.

### A browser's workers spin, like every other target's {#browser-workers-spin}

The spin-before-park window (`g_jobque_spin_us`, 30 ms) keeps a worker hot through the serial gaps
of a token, and a browser takes it on the same terms as a desktop: `setup_dasllama_jobque` pushes
the one value to the queue on every platform, and nothing special-cases emscripten. Spinning is
the faster arm where it was measured - the spin loop no longer crosses into JS twice an iteration
(`src/misc/ARCHITECTURE.md` sec.8), and the K-quant dot makes parallel sections about twice as
long: Pocket TTS in Chrome reads 1.7x real time parked against 2.3x spinning, the same text, three
runs each. A slower box may still prefer parking, and the knobs are unchanged for it - a box
profile's `jobque_spin_us` and the setter override the default, and a program reads the value in
force through `get_jobque_spin_us`. The window is the CPU chain's: while the whole-model GPU driver
serves the loaded model the pool carries only the sampler's argmax between steps, and a worker
still spinning when the step's command is submitted starves the driver's submission thread - on
the pod (48 vCPUs, `-jit`, `benchmarks/lcpp_bench.das --npl 4`; `PERF_LEDGER.md`'s 2026-09-19
section) Llama-3.2-1B at four streams read 707 tok/s summed under the 30 ms window and 1053 under
500 us, the knee at the step's own length. So the Vulkan driver's arm and drop (`rdec_set_active`)
and a Metal driver's served forward (`metal_served`, until the next weights load) report to
`set_dispatch_gpu_served`, which pushes the GPU window (`g_jobque_spin_gpu_us`, 500 us) to the live
queue; `setup_dasllama_jobque` reads the window in force for a queue made later. Under the CPU
window the pool's workers spin through a Metal prefill, and its windows slow within seconds.
The pool is one per process: on a GPU-served slot the CPU audio and speech routes park after
500 us too, and a box profile's `runtime.jobque_spin_gpu_us` or `set_jobque_spin_gpu_us` widens it.
An embedder's own `set_jobque_worker_spin` value is replaced at each report. The pool's size
is the runtime's (`src/misc/job_que.cpp`): the browser's reported cores, capped and floored by
`DAS_MAX_HW_JOBS` and `DAS_MIN_WEB_JOBS` (eight and four unless the build overrides them), minus
one for the computing main thread - seven workers on a real box, the pool
an eight-core desktop runs, and three under fingerprint protection, where a browser reports two
cores whatever the box has; the floor is what keeps such a visitor off a one-worker pool.

### The CPU MoE region list caps a region at 32 rows {#moe-region-split}

The grouped MoE prefill hands its expert regions to the batch dispatcher as (weight offset, first
row, count) triples. That dispatcher chunks units by COUNT, not by work, so one heavy expert in a
skewed routing draw becomes one unit the barrier waits on - measured 620 against 3929 GFLOP/s on a
Zipf k4 draw (`harness/moe_kq_probe.das`). The CPU arms therefore split a region into sub-regions
of at most 32 rows: sub-regions of one expert share its weight offset, and the per-expert bias
lists repeat once per sub-region. The GPU arms keep whole regions - their kernels chunk by work
already. The list runs biggest bucket first (`lpt_order`): the dispatcher hands out units in list
order, so the heavy spans start first; in expert order a late heavy expert left the other lanes idle.

### The engine dispatches only on a configured job queue {#jobque-policy}

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

### Single-thread mode runs every arm inline {#single-thread}

`set_single_thread(true)` - `DASLLAMA_SINGLE_THREAD=1` from the environment, applied by an
`[init]` - makes every kernel run on the calling thread while a queue, configured or not, stays up
and untouched. The flag is a context global in `dasllama_par.das`, the module every dispatch site
requires, so each `maybe_parallel_for` form reads it in the runtime condition its macro emits,
beside the queue check it already makes: the bool form and the count form take their inline arm,
the indexed form's fifo arm sizes its slot range to one. `get_dispatch_lanes()` answers 1 under
it, which is what folds the fused decode chains (their gates want more than one lane), the
shapers' chunk math and the phase parking (`dispatch_phase_decode` and `dispatch_phase_batch`
do nothing). A worker limit cannot stand in for this: with every worker dormant a team op still
pays its publish, its seqlock and its per-stage barriers on the caller, and the count-form sites
that pass a literal count - the TTS head loops - still dispatch. The mode is per context, like
the fork pool: a `new_thread` body that runs inference sets its own. The announce rides the first
inline run a context makes, not that `[init]`. A context that sets the flag and runs no kernel -
the fork pool's clones among them - would announce a mode it never takes, so it stays silent. It
is the rail a build without threads runs on, and the baseline a pool's worth is measured from on
any box.
