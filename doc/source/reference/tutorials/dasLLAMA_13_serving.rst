.. _tutorial_dasLLAMA_serving:

========================================
dasLLAMA-13 — Serving Many Conversations
========================================

.. index::
    single: Tutorial; dasLLAMA
    single: Tutorial; Serving
    single: Tutorial; Continuous Batching

Every tutorial so far drove one conversation at a time. A server holds many —
and dasLLAMA's serving layer is the same facade plus one module,
``dasllama/dasllama_scheduler``: the continuous-batching scheduler behind
``dasllama-server``. This capstone shows the raw mechanism once, then drives
the real thing.

Run it like tutorial 01 (SmolLM2-135M-Instruct works well)::

   daslang.exe -jit tutorials/dasLLAMA/13_serving.das -- path/to/model.gguf

The raw mechanism
=================

Serving B conversations means B sessions over one model, and two primitives
make that cheap. A **paged KV pool**: sessions created over
``create_kv_pool`` allocate fixed-size page groups on demand instead of a
full ``seq_len`` slab each (tutorial 04 sized one such slab). And
``eval_batch``: one decode step for *all* sessions in one pass of the
weights — B matrix-vector products fuse into one matrix-matrix product,
which is where the throughput comes from:

.. das-doc: given var m = Model()
.. code-block:: das

   var pool <- create_kv_pool(m, 16l)
   var ws <- create_batch_workspace(m)
   gpu_dn_room(2l)                 // a GPU-served deltanet hybrid: this many streams' recurrent state stays resident
   var s0 = create_session(m, pool)
   var s1 = create_session(m, pool)
   var rows <- [unsafe(addr(s0)), unsafe(addr(s1))]
   var toks <- [sample(*rows[0], SamplingParams()), sample(*rows[1], SamplingParams())]
   eval_batch(m, ws, rows, toks)   // ONE weight pass advances every stream
   release_kv_pages(s0)            // pages go back to the pool before the session dies
   release_kv_pages(s1)

A hand-rolled batch over a GPU-served deltanet hybrid names its stream count
first: ``gpu_dn_room`` tells the driver how many sessions' recurrent state it
keeps device-resident at once, so no stream's state evicts under the batched
step it rides. Call it before the first prefill; ``create_scheduler`` does it
for its own streams, and a driver with no such cache ignores the call.

The scheduler
=============

``dasllama_scheduler`` is that step made production-shaped. Requests are
rendered to tokens with **no session** — ``create_chat_renderer`` supplies
the template, stop ids, and turn close, so a deep queue holds no KV memory:

.. code-block:: das

   var chat = create_chat_renderer(m, "You are a concise assistant.", 32l)
   add_user(chat, "What is the capital of France?")
   var req <- PendingReq(id = 1l, max_new = 32l)
   req.prompt <- render_turn(m, chat)
   req.close_toks <- render_close(m, chat)
   req.stop_ids <- effective_stop_ids(chat)

``create_scheduler`` sizes the machine; ``submit`` queues requests;
``scheduler_step`` admits, runs one batched decode step over every live
stream, then at most one bounded prefill chunk — a long prompt stalls the
others by a chunk, not by its whole length. Results come back as
``SchedEvent`` values:

.. code-block:: das

   var sch <- create_scheduler(m, 4l, 32l, 64l, 16l)   // 4 streams, paged KV, prefix cache
   submit(m, sch, req)
   var events : array<SchedEvent>
   while (scheduler_step(m, sch, events)) {
       for (ev in events) {
           if (ev.kind == SchedEventKind.piece) {
               print("{ev.text}")
           }
       }
       events |> clear()
   }

The tutorial prints one digit — the stream id — per generated piece, so the
run shows continuous batching literally::

   123412341234 234234234 3434343434...

Streams finish at their own pace; new ones admit while others still decode.
On four SmolLM2 conversations the batched round beats the same four run one
at a time by roughly a third of the wall clock — and the gap grows with
stream count and model size.

The prefix cache
================

With ``page_rows`` set, a finished stream donates its KV pages to a prefix
cache. The four prompts share one system turn, so a second round of *new*
questions attaches the shared pages instead of prefilling them — the
``finished`` event reports it::

   [warm stream 2] stop: 10 tok, ttft 22ms, cached 48 of 67 prompt tokens

Forty-eight of sixty-seven prompt tokens — the system turn's three pages —
arrived from the cache; only the question's tail paid prefill, and time to
first token dropped to match.

Multi-model GPU serving: the slot dance
=======================================

A server holding several models has *one* GPU tier: the engine's tier state
(routing marks + resident-driver state) is per-process, not per-model. A
multi-model host runs a small dance around every load and switch — on a
CPU-only box each step is a cheap no-op that reports ``"cpu"``, so the code
runs anywhere.

Step zero, before any want: ``kernel_backend_available`` answers "can this
box serve on that backend at all?" by running the backend's own witness (the
vulkan one enumerates devices once and caches its verdict) — a defaults-first
host picks its backend from these facts instead of making the user configure
one. Then before a load we request a tier: ``set_gpu_tier_want`` records the
request (the programmatic form of the core ``DASLLAMA_GPU_*`` knobs; a zero
want keeps the load off the device), ``moe_gpu_tier_arm`` arms it so a
backend can install its hooks, ``moe_gpu_weight_budget`` says how many bytes
of weights the armed backend will hold resident, and ``gpu_tier_status`` is
the running snapshot a status page reads. After the load,
``gpu_slot_capture`` moves the arm outcome into a ``GpuModelMarks`` holder
(``gpu_model_marks_init`` makes an empty one) and classifies it: ``"cpu"``,
``"gpu:rails"``, or ``"gpu:resident"``:

.. das-doc: given var s = Session()
.. code-block:: das

   print("vulkan here: {kernel_backend_available("vulkan")}\n")
   set_gpu_tier_want(GpuTierWant(auto_tier = true))
   let armed = moe_gpu_tier_arm()
   print("budget {moe_gpu_weight_budget()} bytes on '{gpu_tier_status().device}'\n")
   // ... load_model runs here ...
   var slot = gpu_model_marks_init()
   let outcome = gpu_slot_capture(slot)   // "cpu" | "gpu:rails" | "gpu:resident"

A switch has two halves, and the order is the contract. Switch-out first:
``moe_gpu_hydrate_session`` pulls each live session's KV back to host memory
(the device mirror dies with the drop), ``moe_gpu_drop_model`` frees the
outgoing model's device state, ``moe_gpu_model_marks_save`` banks what is left
in its slot. Only then switch-in: restore the incoming model's marks and
``gpu_slot_rearm`` — want, arm, resident upload, through the bake-slice path
when the image is a mapped vulkan flavor. Re-arming onto a device that still
holds a model is the bug this order prevents:

.. code-block:: das

   moe_gpu_hydrate_session(m, s)                         // per live session
   moe_gpu_drop_model()
   moe_gpu_model_marks_save(slot)                        // A is out
   moe_gpu_model_marks_restore(slot)                     // B (here: A again) is in...
   let backend = gpu_slot_rearm(GpuTierWant(auto_tier = true), m)   // ...and re-armed

One more serving pin: a multi-stream scheduler calls
``set_resident_prefill_allowed(false)`` once and leaves it — a host-cached
stream's chunked prefill would leave device-only KV that a second stream's
steal strands. ``dasllama-server`` does exactly this.

Streams served from the device
==============================

A session normally keeps its K/V cache in host memory, and a GPU that serves
it copies rows over the bus. The vulkan whole-model driver can hold the cache
itself. It splits its K/V memory into *regions*, one per stream. A session
that lives in one region, with no host cache at all, is a **device-home**
session.

We size this before the load. ``set_gpu_resident_regions`` says how many
streams we serve, and ``set_gpu_ctx_max`` caps the positions one region
holds. After the load we ask what we got: ``gpu_device_sessions`` answers the
region count (0 on a CPU box, under the per-op rails, and on Metal),
``gpu_resident_decline`` says why the driver does not serve,
``gpu_device_session_ctx`` answers the positions per region,
``gpu_device_session_dtype`` the K/V codec a device-home session carries, and
``gpu_device_prefill_window`` how many positions the device prefills in one
window - a host that feeds a prompt in chunks makes them at least that long:

.. code-block:: das

   set_gpu_resident_regions(4l)
   set_gpu_ctx_max(8192l)
   // ... load_model runs here ...
   let regions = gpu_device_sessions()
   if (regions == 0l) {
       print("no device-home sessions here: {gpu_resident_decline()}\n")
   } else {
       print("{regions} streams, {gpu_device_session_ctx()} positions each, K/V in {gpu_device_session_dtype()}\n")
   }

``create_device_session`` makes the session. A finished one calls
``gpu_device_kv_park`` before its ``delete``: the region is free for the next
stream, and park answers a *claim* — a number that names the rows until
another session takes the region. ``gpu_device_kv_adopt`` hands those rows to
a fresh session, so the next turn of a conversation prefills only its new
tokens. ``gpu_device_prefill_continues`` says whether this model may do that;
a recurrent model prefills every prompt whole, from position zero:

.. das-doc: given let turn1 <- [1l, 2l, 3l]
.. das-doc: given let rest <- [4l, 5l]
.. code-block:: das

   var first <- create_device_session(m, gpu_device_session_dtype())
   eval(m, first, turn1)
   let held = first.n_past
   let claim = gpu_device_kv_park(first)   // before every device-home delete
   delete first

   var second <- create_device_session(m, gpu_device_session_dtype())
   if (gpu_device_prefill_continues(m) && gpu_device_kv_adopt(second, claim, held)) {
       eval(m, second, rest)               // turn one's rows are already there
   }
   gpu_device_kv_park(second)
   delete second

The scheduler does all of this per stream once we switch it with
``set_device_kv(sch, true)``. Only an idle scheduler switches, and we switch
it on only when every stream gets a region — ``gpu_device_sessions()`` is at
least the scheduler's stream count.

``gpu_cpu_passes`` is the honesty counter: every call the GPU path handed
back to the CPU, by reason, with the reason in plain words. Streams served
from the device keep it empty. Run this tutorial with the vulkan tier armed
and the counter lists the earlier sections' paged sessions — those keep their
cache on the host, so their calls ran on the CPU:

.. code-block:: das

   for (p in gpu_cpu_passes()) {
       print("{p.count} calls ran on the CPU: {p.words}\n")
   }

The same module, more streams
=============================

That is the whole serving story: rendered stateless requests, one batched
step, pages donated and reattached. The OpenAI-compatible server
(:ref:`utils/dasllama-server <utils_dasllama_server>`) drives this exact
scheduler under HTTP and SSE — same ``submit``, same ``scheduler_step``,
same events.

.. seealso::

   Full source: :download:`tutorials/dasLLAMA/13_serving.das <../../../../tutorials/dasLLAMA/13_serving.das>`

   Next tutorial: :ref:`tutorial_dasLLAMA_vision_chat`

   Sessions and the KV cache: :ref:`tutorial_dasLLAMA_sessions_and_memory`

   The server it scales into: :ref:`utils_dasllama_server`
