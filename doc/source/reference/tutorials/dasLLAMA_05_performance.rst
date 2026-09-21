.. _tutorial_dasLLAMA_performance:

=========================
dasLLAMA-05 — Performance
=========================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Performance
    single: Tutorial; Quantization

The three dials that matter for CPU inference speed: the JIT, the thread pool,
and the weight quantization. The tutorial measures all of them with
``stats()`` on your machine::

   daslang.exe -jit tutorials/dasLLAMA/05_performance.das -- path/to/model.gguf

Always -jit
===========

The kernels are daslang code, and the tuning framework specializes them for
the current box through the LLVM JIT. This is part of dasLLAMA's execution
model, not an optional acceleration: interpreted and AOT execution are
intentionally unsupported. ``jit_enabled()`` verifies that requirement. On
top of that, ``options _jit_fast_math = true`` lets the JIT relax FP ordering
in the kernels (non-bit-exact, roughly +10%) — matching how llama.cpp compiles
its own; the parity test suite stays bit-exact.

Threads: the job queue
======================

The matmul kernels split their rows across the job queue — they *require* one
(model code outside ``with_job_que()`` panics). ``setup_dasllama_jobque()``
configures the queue for this pure fork/join workload, and every dasLLAMA
runner calls it. Its biggest setting is team dispatch
(``set_jobque_team_mode``): the workers run as a standing team instead of
paying fifo fork/join per job — an EPYC 9654 measured Llama-1B decode going
from 7.6 to 43 t/s (5.7×) on this one switch. The rest: pooled fork contexts,
batched dispatch, spin-before-park windows for the workers and the joining
thread, and a raised priority for the dispatch caller (in team mode only the
caller publishes work).

The worker count is fixed when the queue is created; the
``DAS_JOBQUE_THREADS`` environment variable overrides the (deliberately
conservative) default. Re-run the tutorial with ``DAS_JOBQUE_THREADS=1`` to
see what threading buys — the win grows with model size, since tiny models
have tiny matmuls.

Inside a fixed queue the kernel dispatch can still be capped:
``set_dispatch_worker_limit(n)`` bounds how many workers the kernels use
(``0`` = no limit), and ``get_dispatch_worker_limit()`` reads the cap back.
``setup_dasllama_jobque()`` latches the cap into the queue, so set it before that
call — a serving box does this at startup to keep cores free for the rest of the
process:

.. das-doc: given var m = Model()
.. das-doc: given let path = "model.gguf"
.. code-block:: das

   set_dispatch_worker_limit(2)
   with_job_que() {
       setup_dasllama_jobque()   // the cap is latched here
       print("cap: {get_dispatch_worker_limit()}\n")
   }
   set_dispatch_worker_limit(0)   // 0 = back to all of them

A cap still dispatches, to fewer workers. ``set_single_thread(true)`` goes the
whole way: every kernel runs on the calling thread and the queue is never
dispatched to, whatever queue exists. It takes effect at the next kernel, so it
needs no new queue, and ``get_single_thread()`` reads it back
(``DASLLAMA_SINGLE_THREAD=1`` is the environment spelling). This is the figure
the pool is measured against, and the browser build's own rail, where waking a
web worker costs more than a small kernel does:

.. code-block:: das

   set_single_thread(true)
   with_job_que() {
       setup_dasllama_jobque()
       print("single thread: {get_single_thread()}\n")
   }
   set_single_thread(false)

The workers' spin-before-park window is the other queue knob a program sets:
``set_jobque_spin_us(us)`` is how long an idle worker spins before it parks
(``0`` = park at once), and ``get_jobque_spin_us()`` reads it back. The
default is 30 ms on every platform, a browser included, which keeps the workers
hot through a token's serial gaps (Pocket TTS in Chrome measured 1.7x real time
parked against 2.3x spinning). Like the cap, ``setup_dasllama_jobque()`` latches
it:

.. code-block:: das

   let spin_was = get_jobque_spin_us()
   set_jobque_spin_us(0l)
   with_job_que() {
       setup_dasllama_jobque()   // the window is latched here
       print("spin window: {get_jobque_spin_us()} us\n")
   }
   set_jobque_spin_us(spin_was)   // the platform's default back

The window has a GPU-served twin. While the whole-model GPU driver serves the
loaded model, the pool carries only the sampler's small work between steps, so
the workers park after ``set_jobque_spin_gpu_us(us)`` (500 us by default)
instead of the CPU window - a 30 ms spin there would only heat the cores the
device does not need. ``get_jobque_spin_gpu_us()`` reads that knob back, and
``get_jobque_spin_in_force()`` reads whichever of the two windows the pool is
on right now: the GPU one while a resident model is armed, the CPU one
otherwise. The switch is process-wide, so a CPU audio or speech route served
beside a GPU-served model sits under the GPU window too; widen it when that
route's wake latency shows. The box profile carries it as
``runtime.jobque_spin_gpu_us``.

.. code-block:: das

   print("in force: {get_jobque_spin_in_force()} us\n")   // the CPU window: no GPU driver armed here
   set_jobque_spin_gpu_us(2000l)                          // the GPU-served window, for when one is
   set_jobque_spin_gpu_us(500l)                           // the default back

On a big SMT box also set ``DAS_JOBQUE_AFFINITY`` (``1`` = ideal-CPU hint,
``2`` = hard pin): unpinned, the OS placement lottery can land two compute
lanes on one physical core's SMT pair, which roughly halves batched prefill
while barely moving decode. On macOS there is no pin API — mode ``2`` instead
gives the workers QoS classification, and dasLLAMA turns it on by default
there, because unclassified threads lose several percent of decode when the
box is busy with anything else. Set ``0`` to switch it off.

Both are daslang knobs rather than dasLLAMA ones — :ref:`the daslang environment
variables <environment_variables>` page lists every one it reads. dasLLAMA has
its own set — over a hundred knobs covering backend selection, GPU rails and
the probe harnesses: see :ref:`the dasLLAMA knob reference <dasllama_env>`.

Prefill vs generation
=====================

``stats()`` separates the two phases because their physics differ: the prompt
runs as **one batched forward** (compute-bound, fast), while generation runs
**one token per forward** (memory-bandwidth-bound, slower). ``ttft_s`` — time
to first token — spans the prefill plus the first generation step.

Quantization: memory vs fidelity
================================

``QuantMode`` picks the in-memory weight representation, whatever the GGUF
stores. Measured on SmolLM2-135M (Apple M1 Max, 7 jobs):

=========  ====================  ====================================================
Mode       Resident              Throughput
=========  ====================  ====================================================
``fp32``   544 MB                prefill 905 t/s, gen 170 t/s — the token-exact reference
``q8``     7 MB (+175 MB map)    prefill 2260 t/s, gen 441 t/s — the everyday choice
``q4_0``   187 MB                prefill 281 t/s, gen 273 t/s — the legacy requant tier
=========  ====================  ====================================================

The q8 heap number is small because a q8 load rides the mmap'd *prepared
image* minted beside the GGUF on the first load — the OS pages the weight
planes in on demand, and later loads take milliseconds. ``DASLLAMA_IMAGE=0``
disables the cache; fp32 and q4_0 requant from the GGUF on the heap.

Generation is bandwidth-bound, so between fp32 and q8 smaller weights are also
faster. ``q4_0`` is the legacy requant tier — it squeezes *this* f32/q8 load
down to 4-bit blocks on the portable dot, batched prefill and batched decode
included, so it is the smallest footprint, not the fastest path. For small *and* fast, prefer a
K-quant / mxfp4 / Q4_0 GGUF (Q4_K_M / Q5_K_M / Q6_K): under ``QuantMode.q8``
those files keep their native 4-6 bit planes on the same fast rails, so both
phases stay at full speed with a q4-class footprint.

The load's two caches
=====================

Two more calls round out the load story. ``select_matmul_backend_for_load()``
picks the fastest kernel backend for the *next* load (honoring a pin) and
answers one question: must the loader repack the weights into that backend's
interleaved layout? ``load_model`` runs it itself; call it yourself when you
schedule loads and want the answer early.

And the prepared images have a management surface. ``dlim_inventory`` lists
the ``.dlim`` images minted beside a GGUF, each with a verdict: ``CURRENT``
loads; ``STALE vN`` was minted by an older image version; ``OTHER`` was minted
under a different bake configuration (``DlimConfiguration`` holds every knob
that changes image bytes) or on another box; ``FOREIGN`` belongs to a different
flavor. ``dlim_clean`` is the garbage collector: ``apply = false`` only reports,
``apply = true`` removes ``STALE`` and ``OTHER`` (``keep_other`` spares ``OTHER``)
— the server runs this at startup:

.. code-block:: das

   let repack = select_matmul_backend_for_load()
   var inscope images <- dlim_inventory(path)
   for (im in images) {
       print("{im.file}: {im.bytes} bytes, {im.verdict}\n")
   }
   let r = dlim_clean(path, false)   // report only
   print("{r.stale} of {r.total} stale\n")

Where to go deeper: ``modules/dasLLAMA/tune_for_this_box.md`` covers kernel
tuning (token-block size, unrolls, the ``[dasllama_grid]`` tuner) when you
want to squeeze a specific machine.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/05_performance.das <../../../../tutorials/dasLLAMA/05_performance.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_add_an_arch`
