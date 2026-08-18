options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require dasllama/dasllama_scheduler
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-13: Serving Many Conversations
//
// This tutorial covers:
//   - The raw mechanism: paged KV sessions + one hand-rolled eval_batch step
//   - The continuous-batching scheduler: create_scheduler / submit / scheduler_step
//   - Batched vs sequential wall-clock for the same four conversations
//   - The prefix cache: a repeated system prompt skips its own prefill
//   - Stateless requests: rendered token prompts hold no KV memory
//   - Multi-model GPU serving: the tier want/arm calls and the slot dance
//     (capture, rearm, hydrate, drop, the resident-prefill pin)
//
// Prerequisites: tutorials 02 and 04.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/13_serving.das -- <path/to/model.gguf>

let SYSTEM = ("You are a concise, knowledgeable assistant. Answer in at most two short " +
    "sentences, plain language, no lists. If a question has a factual answer, give the " +
    "fact first and add one supporting detail only when it helps.")
let QUESTIONS = fixed_array("What is the capital of France?",
    "Name the largest planet in the solar system.",
    "What does a compiler do?",
    "Which ocean is the deepest?")
let QUESTIONS2 = fixed_array("What is the capital of Japan?",
    "Name the smallest planet in the solar system.",
    "What does a linker do?",
    "Which desert is the largest?")
let MAX_NEW = 32l

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — The raw mechanism: one batched step by hand
// ──────────────────────────────────────────────────────────────────────────
//
// Serving B conversations means B sessions over ONE model. Two primitives make
// that cheap. A paged KV pool (tutorial 04 sized one session's cache; here
// sessions allocate pages on demand instead of full slabs). And eval_batch:
// one decode step for ALL sessions in one pass of the weights — B matrix-vector
// products fuse into one matrix-matrix product, which is exactly where the
// throughput comes from. This section is what a scheduler's inner step IS.

def section_raw(m : Model) {
    var pool <- create_kv_pool(m, 16l)
    var ws <- create_batch_workspace(m)
    var s0 = create_session(m, pool)
    var s1 = create_session(m, pool)
    var s2 = create_session(m, pool)
    var rows <- [unsafe(addr(s0)), unsafe(addr(s1)), unsafe(addr(s2))]
    for (i in range(3)) {
        let prompt <- encode(m, QUESTIONS[i])
        eval(m, *rows[i], prompt)   // prefill each stream at its own position
    }
    var toks : array<int64>
    toks |> resize(3)
    for (i in range(3)) {
        toks[i] = sample(*rows[i], SamplingParams())
    }
    eval_batch(m, ws, rows, toks)   // ONE weight pass advances all three
    print("one eval_batch step advanced 3 streams; sampled token ids {toks}\n")
    let pages_used = pool.capacity_groups - long_length(pool.free_groups)
    print("\n{pages_used} KV pages checked out; the pool backs them with {kv_pool_bytes(pool) / 1024l} KB\n\n")
    for (i in range(3)) {
        release_kv_pages(*rows[i])
    }
    delete s0
    delete s1
    delete s2
    delete pool
    delete ws
}

// Render one request with NO session: a queued request holds tokens only, so a
// deep queue costs no KV memory. The renderer chat supplies the same template,
// stop ids, and turn close that a live chat would.
def render_request(m : Model; id : int64; question : string) : PendingReq {
    var chat = create_chat_renderer(m, SYSTEM, MAX_NEW)
    add_user(chat, question)
    var r <- PendingReq(id = id, max_new = MAX_NEW)
    r.prompt <- render_turn(m, chat)
    r.close_toks <- render_close(m, chat)
    r.stop_ids <- effective_stop_ids(chat)
    return <- r
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — The scheduler; Section 3 — the prefix cache
// ──────────────────────────────────────────────────────────────────────────
//
// dasllama_scheduler is the production shape of section 1: scheduler_step
// admits queued requests, runs one batched decode step over every live stream,
// then at most one bounded prefill chunk — a long prompt stalls the others by
// a chunk, not by its whole length. Results come back as SchedEvents. The
// digit stream printed below is the stream id of each generated piece — the
// interleaving IS continuous batching, visible.
//
// With page_rows set, finished streams donate their KV pages to a prefix
// cache. The four prompts share one system turn, so every round after the
// first attaches those pages instead of prefilling them: watch n_cached.

def run_round(m : Model; var sch : Scheduler; id_base : int64; tag : string;
              questions : string[4]) : int64 {
    for (i in range(4)) {
        var req <- render_request(m, id_base + int64(i), questions[i])
        if (!submit(m, sch, req)) {
            panic("queue full — max_queue is 32, four requests cannot fill it")
        }
    }
    var events : array<SchedEvent>
    let t0 = ref_time_ticks()
    while (scheduler_step(m, sch, events)) {
        for (ev in events) {
            if (ev.kind == SchedEventKind.piece) {
                print("{ev.stream - id_base + 1l}")   // one digit per generated piece
            } else {
                print("\n[{tag} stream {ev.stream - id_base + 1l}] {ev.finish}: {ev.n_gen} tok, ttft {ev.ttft_us / 1000l}ms, cached {ev.n_cached} of {ev.n_prompt} prompt tokens\n")
            }
        }
        events |> clear()
    }
    let wall = int64(get_time_usec(t0))
    print("[{tag}] wall {wall / 1000l}ms\n\n")
    delete events
    return wall
}

// The same four conversations one at a time — the shape tutorials 01-11 used.
def sequential_round(m : Model) : int64 {
    let t0 = ref_time_ticks()
    for (i in range(4)) {
        var chat = create_chat(m, SYSTEM, MAX_NEW)
        add_user(chat, QUESTIONS[i])
        respond(m, chat, SamplingParams()) $(_piece) => true
    }
    let wall = int64(get_time_usec(t0))
    print("[sequential] wall {wall / 1000l}ms\n\n")
    return wall
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Multi-model GPU serving: the slot dance
// ──────────────────────────────────────────────────────────────────────────
//
// A server holding several models has ONE GPU tier: the engine's tier state
// (routing marks + resident-driver state) is per-process, not per-model. So a
// multi-model host runs a small dance around every load and every switch. On
// a CPU-only box every step below is a cheap no-op that reports "cpu" — the
// calls are the contract; run this with the vulkan backend to watch the tier
// actually move. (In a real host the want/arm pair runs BEFORE load_model;
// this section runs after it so it can share the model the rest of the
// tutorial loaded, which is why every step here is a no-op re-arm.)
//
// At load time we request a tier BEFORE the load: set_gpu_tier_want records
// the request (the programmatic form of the DASLLAMA_GPU_* env vars; a zero
// want keeps the load off the device), moe_gpu_tier_arm arms it so a backend
// can install its hooks, and moe_gpu_weight_budget says how many bytes of
// weights the armed backend will hold resident. gpu_tier_status is the
// running snapshot the server's GPU badge reads.
//
// After the load, the arm outcome belongs to THAT model. gpu_slot_capture
// moves it into a GpuModelMarks holder (gpu_model_marks_init makes an empty
// one) and classifies what the model got: "cpu", "gpu:rails" (some weights
// ride device rails), or "gpu:resident" (the whole stack lives on device).
//
// A switch has two halves, and the ORDER is the contract. Switch-out first:
// moe_gpu_hydrate_session pulls each live session's KV back to host memory
// (the device mirror dies with the drop), moe_gpu_drop_model frees the
// outgoing model's whole device state, moe_gpu_model_marks_save banks what is
// left in its slot. Only then switch-in: restore the incoming model's marks
// and gpu_slot_rearm — want, arm, resident upload (bake-slice path for a mapped
// vulkan flavor). Re-arming onto a device that still holds a model is the bug
// this order exists to prevent.

def section_gpu_slots(m : Model) {
    set_gpu_tier_want(GpuTierWant(auto_tier = true))
    let armed = moe_gpu_tier_arm()
    let tier = gpu_tier_status()
    print("tier want armed: {armed}; resident weight budget {moe_gpu_weight_budget()} bytes; device '{tier.device}'\n")

    var slot_a = gpu_model_marks_init()
    let outcome = gpu_slot_capture(slot_a)   // after A's load: A's tier state moves into the slot
    print("model A captured as '{outcome}'\n")

    // switch A OUT: KV home first, then drop, then bank the slot
    var s = create_session(m)
    moe_gpu_hydrate_session(m, s)
    moe_gpu_drop_model()
    delete s
    moe_gpu_model_marks_save(slot_a)

    // switch A back IN: marks install, then the re-arm onto the now-empty device
    moe_gpu_model_marks_restore(slot_a)
    let rearmed = gpu_slot_rearm(GpuTierWant(auto_tier = true), m)
    print("A switched in: '{rearmed}'\n")

    // the serving pin: a multi-stream scheduler turns the resident prefill arm OFF
    // for good — its one shared mirror plus chunked prefill would leave device-only
    // KV that a second stream's steal strands (dasllama-server does exactly this)
    set_resident_prefill_allowed(false)
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/13_serving.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    with_job_que() {
        setup_dasllama_jobque()
        section_raw(m)

        var sch <- create_scheduler(m, 4l, 32l, 64l, 16l)   // 4 streams, paged KV, prefix cache on
        let batched = run_round(m, sch, 1l, "batched, cold", QUESTIONS)
        let seq = sequential_round(m)
        print("four conversations: batched {batched / 1000l}ms vs sequential {seq / 1000l}ms\n\n")

        run_round(m, sch, 11l, "batched, warm prefix", QUESTIONS2)
        // four NEW questions over the same system prompt: the warm round's n_cached
        // is the shared system turn's pages — donated by round one, attached instead
        // of prefilled, so only each question's tail pays prefill

        delete sch

        section_gpu_slots(m)
    }
    // From here the OpenAI-compatible server is the same module with more streams:
    // utils/dasllama-server drives this exact scheduler under HTTP + SSE.
}
