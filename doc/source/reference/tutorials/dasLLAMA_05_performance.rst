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

The kernels are daslang code; the LLVM JIT compiles them to native SIMD.
Interpreted inference is orders of magnitude slower — never benchmark it.
``jit_enabled()`` tells you which world you're in. On top of that,
``options _jit_fast_math = true`` lets the JIT relax FP ordering in the
kernels (non-bit-exact, roughly +10%) — matching how llama.cpp compiles its
own; the parity test suite stays bit-exact.

Threads: the job queue
======================

The matmul kernels split their rows across the job queue — they *require* one
(model code outside ``with_job_que()`` panics). ``set_jobque_fork_pool(true,
true)`` pools the per-job fork contexts: the kernels are pure data-parallel
jobs, so their contexts can be reused and skip re-init.

The worker count is fixed when the queue is created; the
``DAS_JOBQUE_THREADS`` environment variable overrides the (deliberately
conservative) default. Re-run the tutorial with ``DAS_JOBQUE_THREADS=1`` to
see what threading buys — the win grows with model size, since tiny models
have tiny matmuls.

Prefill vs generation
=====================

``stats()`` separates the two phases because their physics differ: the prompt
runs as **one batched forward** (compute-bound, fast), while generation runs
**one token per forward** (memory-bandwidth-bound, slower). ``ttft_s`` — time
to first token — spans the prefill plus the first generation step.

Quantization: memory vs fidelity
================================

``QuantMode`` picks the in-memory weight representation, whatever the GGUF
stores. Measured on SmolLM2-135M (Apple M1 Max, 8 jobs):

=========  ==================  ====================================================
Mode       Resident            Throughput
=========  ==================  ====================================================
``fp32``   546 MB              prefill 471 t/s, gen 86 t/s — the token-exact reference
``q8``     241 MB              prefill 1391 t/s, gen 219 t/s — the everyday choice
``q4``     189 MB              prefill 70 t/s, gen 69 t/s — smallest, scalar kernel only
=========  ==================  ====================================================

Generation is bandwidth-bound, so between fp32 and q8 smaller weights are also
faster. ``q4`` currently has no batched prefill kernel — it's the smallest
footprint, not the fastest path.

Where to go deeper: ``modules/dasLLAMA/tune_for_this_box.md`` covers kernel
tuning (token-block size, unrolls, the ``[dasllama_grid]`` tuner) when you
want to squeeze a specific machine.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/05_performance.das <../../../../tutorials/dasLLAMA/05_performance.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_add_an_arch`
