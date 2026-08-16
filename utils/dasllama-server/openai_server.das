options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true   // ggml-parity FP laxity (non-bit-exact); inference tool, not the test suite

module openai_server

require dasllama/dasllama       // the public facade — the main dasLLAMA surface this tool touches
require dasllama/dasllama_scheduler   // continuous-batching scheduler (layer 3 over the facade)
require dashv/dashv_boost public
require daslib/json_boost
require daslib/toml             // write_toml: POST /config persists the authoritative config
require dasllama/dasllama_math  // gpu_tier_status: the GET /config GPU badge
require dasllama/dasllama_image   // dlim_inventory / dlim_clean: GET /v1/images + the bake GC
require dasllama/dasllama_config  // dlim_config_current/json: the bake child's --config payload
require ../../modules/dasLLAMA/performance/exchange_client.das   // the /exchange surface + submit rails
require llvm/daslib/llvm_tune   // tune_manifest_path — the /exchange apply target
require daslib/defer            // defer() { delete js } — free the parsed request JSON on every exit path
require daslib/jobque_boost
require daslib/rtti             // this_context().last_exception inside ASR worker recovery
require daslib/strings_boost    // join()
require daslib/fio              // sleep(), path_join, base_name, remove, get_env_variable
require daslib/module_path      // get_this_module_dir: control.html ships beside the server sources
require strings
require math
require dasllama/dasllama_audio_io   // load_audio_16k_mono: any uploaded audio format -> 16 kHz mono f32
require dasllama/dasllama_vision_io  // load_image_rgb(bytes): the chat route's data-URI image parts
require daslib/base64                // base64_decode: the data: URI payload
require dasllama/dasllama_vad        // POST /vad: silero speech spans for the page's waveform overlay
require dasllama/dasllama_gpu_resident   // multi-model slot switch (marks save/restore) + the resident-prefill pin

// An OpenAI-compatible HTTP server over the dasLLAMA facade. The SERVING paths reach only public
// facade verbs — no engine internals — which is the acceptance test for the API: if a full OpenAI
// surface builds without reaching into the engine, the facade is complete. (The dashboard-only
// surfaces additionally read dasllama_math's tier-status getters — see the require above.)
// Chat and completion requests batch continuously: handlers parse + render + register and return;
// run_server's loop drives dasllama_scheduler (one eval_batch decode step over every live stream + one
// bounded prefill chunk per tick) and routes SchedEvents back to each request's writer. Streaming
// and buffered responses both ride the deferred writer ops, so N streams generate concurrently.
// ASR uploads are queued to long-lived new_thread workers, each with its own model/session/context;
// the small buffered holdouts (/v1/models, /v1/embeddings, /shutdown, /v1/stats) still complete
// in-handler.

// ===== Served state (slots set at startup; handlers run serially on the tick thread) =====

//! Per-slot serving options (main.das resolves them from a [[models]] config entry). The -1/-2/""
//! sentinels mean "inherit the init_server default". `backend` is a REQUEST (auto | cpu | gpu);
//! the arm outcome lands in ModelSlot.backend_effective.
struct SlotOpts {
    backend : string = "auto"
    streams : int64 = -1l
    chunk : int64 = -1l
    page_rows : int64 = -2l         // -2 = inherit (0 is meaningful: flat sessions)
    prefix : int64 = -2l
    kv_dtype : KVDtype = KVDtype.f16
    kv_dtype_set : bool = false
    mtp : int = -1                  // -1 inherit | 0 off | 1 on
}

//! One live served model: the loaded model, its scheduler (sessions + KV pool + prefix cache —
//! the host caches that survive switches), and its serving options. Execution is SERIALIZED:
//! the tick loop steps one slot's scheduler at a time (round-robin over slots with work).
struct ModelSlot {
    name : string
    path : string                   // the source GGUF — the bake button + /v1/images key off it
    model : Model = Model()
    sched : Scheduler = Scheduler()
    opts : SlotOpts = SlotOpts()
    backend_effective : string = "cpu"   // arm outcome: cpu | gpu:rails | gpu:resident
    is_default : bool
    last_used : int64
    sw_count : int64                // gpu-switch telemetry (count / total usec)
    sw_us : int64
    gpu_want : GpuTierWant = GpuTierWant()   // the tier request this slot re-arms with on switch-in
    gpu_capable : bool              // want is non-zero and backend != cpu (set_slot_gpu_want)
    gpu_marks : GpuModelMarks <- GpuModelMarks()   // this model's tier routing marks while NOT installed
    vision : Gemma4uvEmbedder = Gemma4uvEmbedder()   // set_model_vision; proj_dim == 0 = text only (encode runs on the vision worker)
    vision_path : string                             // the worker loads its own embedder from this
    think_default : bool   // family default when the request sends no enable_thinking (gemma-4 off, qwen3 on)
}

var g_slots : array<ModelSlot>
var g_default_slot = 0
var g_active = 0                // slot the tick loop stepped last
var g_cur = 0                   // slot the request being handled resolved to (handlers are serial)
var g_gpu_owner = -1            // slot holding the GPU tier's VRAM state (-1 = none)
var g_marks_slot = -1           // slot whose routing marks are INSTALLED in the engine globals

def private cur_model : Model & {
    unsafe {
        return g_slots[g_cur].model
    }
}

def private cur_sched : Scheduler & {
    unsafe {
        return g_slots[g_cur].sched
    }
}

def private cur_name : string => g_slots[g_cur].name

def private total_active : int64 {
    var n = 0l
    for (s in g_slots) {
        n += n_active(s.sched)
    }
    return n
}

def private total_queued : int64 {
    var n = 0l
    for (s in g_slots) {
        n += n_queued(s.sched)
    }
    return n
}

// Slot index for a request's "model" field: "" -> the default slot; the exact slot name -> that
// slot; unknown -> -1 (the caller owns the 404). The legacy "{name}-asr" alias routes to the
// DEFAULT slot only — that is the only "-asr" id /v1/models ever advertises, so a made-up
// "{other}-asr" stays a 404 instead of silently serving.
def private slot_index(model_field : string) : int {
    if (empty(model_field)) {
        return g_default_slot
    }
    for (i in range(length(g_slots))) {
        if (g_slots[i].name == model_field
                || (i == g_default_slot && "{g_slots[i].name}-asr" == model_field)) {
            return i
        }
    }
    return -1
}
var g_has_asr = false
var g_asr_path = ""
var g_asr_mmproj = ""
var g_asr_worker_count = 0
var g_asr_team_dispatch = false
var g_shutdown_requested = false
var g_gc_requested = false
var g_req_counter = 0l
var g_events : array<SchedEvent>

// One registered request's HTTP side: where this stream id's SchedEvents go. The writer is owned by
// dasHV's active_writers (respond/close_writer/release_writer drop it) — never deleted from here.
struct private StreamWire {
    @do_not_delete writer : HttpResponseWriter?
    streaming : bool
    is_chat : bool                  // chat.completion.chunk vs text_completion framing
    slot : int                      // the ModelSlot this stream runs on (flush sets g_cur from it)
    id : string                     // "chatcmpl-N" / "cmpl-N"
    created : int64
    reply_parts : array<string>     // non-streaming (or has_tools): pieces accumulate until the finished event
    utf8_tail : string              // streaming: held-back bytes of a multi-byte char split across tokens
    has_tools : bool                // chat request declared tools on a tool-capable template
    last_keepalive_ticks : int64    // tool streams buffer native envelopes; empty deltas keep HTTP alive
    call_open : string              // the family's tool-call markers (copied from the chat template)
    call_close : string
    tool_mode : ToolMode = ToolMode.none   // the family's tool wire format — the finish parse dispatches on it
    think : ThinkStream = ThinkStream()   // the reply-side reasoning splitter (pass-through when unarmed)
}

var g_wires : table<int64; StreamWire>
var g_default_max_new = 256l
var private g_started_at = 0l   // ref_time_ticks at init_server; /v1/stats derives uptime_s from it
                                // (created_ts is a per-request monotone stand-in, NOT a clock)

//! Append a loaded model as a serving slot (call once per model, before ``run_server``). ``name``
//! is the id ``/v1/models`` reports and requests route on via their ``"model"`` field. The first
//! slot added is the default until ``set_default_model`` says otherwise.
def add_model(var m : Model; name : string; opts : SlotOpts = SlotOpts(); path : string = "") {
    var slot <- ModelSlot(name = name, path = path, is_default = empty(g_slots))
    slot.model <- m
    slot.opts = opts
    // the load's arm outcome (routing marks + resident-driver state) belongs to THIS slot:
    // capture it, so two models' offset-keyed marks never mix. Whichever slot is stepped gets
    // its set re-installed (install_slot_marks); an armed capture makes this slot the GPU owner.
    // Attribution reads the CAPTURED marks, not the process-global VRAM counter — that still
    // holds the previous load's bytes when a later CPU-only slot loads.
    let was_resident = moe_gpu_resident_active()
    moe_gpu_model_marks_save(slot.gpu_marks)
    let was_gpu = was_resident || moe_gpu_marks_armed(slot.gpu_marks.marks)
    slot.backend_effective = was_resident ? "gpu:resident" : (was_gpu ? "gpu:rails" : "cpu")
    var probe <- create_chat_renderer(slot.model, "", 1l)
    // reporting the raw template default would show the page a checked box for a family whose
    // vocab cannot think
    slot.think_default = probe.enable_thinking && probe.think_vocab
    delete probe
    g_slots |> emplace(slot)
    if (was_gpu) {
        g_gpu_owner = length(g_slots) - 1
    }
}

//! Move a loaded model into the server as the ONLY slot (call once, before run_server). ``name``
//! is the id reported by ``/v1/models`` and echoed back in responses. Single-model compatibility
//! wrapper over ``add_model``; multi-model hosts call ``add_model`` per entry instead.
def set_model(var m : Model; name : string) {
    unsafe {
        delete g_slots
    }
    g_default_slot = 0
    g_active = 0
    g_cur = 0
    g_gpu_owner = -1
    g_marks_slot = -1
    add_model(m, name)
}

//! Record the GPU tier want slot `name` re-arms with when the tier switches to it (main.das
//! passes the same want it set before that slot's load). A zero want or ``backend = "cpu"``
//! keeps the slot off the device permanently.
def set_slot_gpu_want(name : string; want : GpuTierWant) {
    for (s in g_slots) {
        if (s.name == name) {
            s.gpu_want = want
            s.gpu_capable = (s.opts.backend != "cpu"
                && (want.auto_tier || want.moe_layers != 0l || want.moe_stream != 0l || want.dn
                    || want.attn || want.dense || want.dnd || want.shexp || want.qkv || want.heat > 0l))
        }
    }
}

// The stepped slot's marks must be the installed ones — swap on change (pure table moves, no
// device work; this is what keeps gpu<->cpu slot alternation free).
def private install_slot_marks(si : int) {
    if (g_marks_slot == si) {
        return
    }
    if (g_marks_slot >= 0 && g_marks_slot < length(g_slots)) {
        moe_gpu_model_marks_save(g_slots[g_marks_slot].gpu_marks)
    }
    moe_gpu_model_marks_restore(g_slots[si].gpu_marks)
    g_marks_slot = si
}

// Move the GPU tier's VRAM state to slot `si`: hydrate + drop the current owner's whole state,
// re-arm `si` from its mmap'd image (the same walk its load ran). Serialized with stepping —
// only called between scheduler steps.
def private bind_gpu_slot(si : int) {
    if (g_gpu_owner == si || !g_slots[si].gpu_capable) {
        return
    }
    let t0 = ref_time_ticks()
    let prev = g_gpu_owner
    var prev_note = ""
    if (prev >= 0) {
        install_slot_marks(prev)   // the drop acts on the INSTALLED per-model state
        for (st in g_slots[prev].sched.streams) {   // server flow keeps host KV authoritative; belt-and-braces
            moe_gpu_hydrate_session(g_slots[prev].model, st.session)
        }
        moe_gpu_drop_model()
        g_slots[prev].backend_effective = "cpu"
        prev_note = "from {g_slots[prev].name}, "
    }
    install_slot_marks(si)
    set_gpu_tier_want(g_slots[si].gpu_want)
    moe_gpu_tier_arm()
    // a vulkan-flavor image re-binds by SLICING its baked planes (the same rail its load used);
    // on a planes-trimmed image a live re-gather would panic — the slice is the only path
    if (!empty(g_slots[si].model.vkplan)) {
        vulkan_bake_slice_begin()
        moe_gpu_upload_resident(g_slots[si].model)
        vulkan_bake_slice_end(g_slots[si].model)
    } else {
        moe_gpu_upload_resident(g_slots[si].model)
    }
    g_gpu_owner = si
    let us = int64(get_time_usec(t0))
    g_slots[si].sw_count ++
    g_slots[si].sw_us += us
    g_slots[si].backend_effective = (moe_gpu_resident_active() ? "gpu:resident"
        : (gpu_tier_status().vram_bytes > 0l ? "gpu:rails" : "cpu"))
    to_log(LOG_INFO, "dasllama-server: GPU tier -> {g_slots[si].name} ({prev_note}{us / 1000l}ms, {g_slots[si].backend_effective})\n")
}

// Auto-switch policy: move the tier to a gpu-wanting slot only when the current owner is idle
// (drain, never preempt — its live streams keep their device speed). gpu<->cpu alternation
// never lands here: cpu slots are not gpu_capable and never evict the owner.
def private maybe_bind_gpu(si : int) {
    if (g_gpu_owner == si || !g_slots[si].gpu_capable
            || (g_gpu_owner >= 0
                && (n_active(g_slots[g_gpu_owner].sched) > 0l || n_queued(g_slots[g_gpu_owner].sched) > 0l))) {
        return
    }
    bind_gpu_slot(si)
}

//! Record a slot's backend arm outcome for the stats/config surfaces (P1: coarse gpu/cpu; the
//! P2 switch path refines to gpu:resident / gpu:rails). No-op on an unknown name.
def set_slot_backend_effective(name : string; eff : string) {
    for (s in g_slots) {
        if (s.name == name) {
            s.backend_effective = eff
        }
    }
}

//! Mark the default slot (the one serving requests with no ``"model"`` field). No-op on an
//! unknown name.
def set_default_model(name : string) {
    var ni = -1
    for (i in range(length(g_slots))) {
        if (g_slots[i].name == name) {
            ni = i
        }
    }
    if (ni < 0) {
        return
    }
    g_default_slot = ni
    for (i in range(length(g_slots))) {
        g_slots[i].is_default = i == ni
    }
}

//! Default reply token budget for requests that omit ``max_tokens`` (call before run_server;
//! clamped to the context length per request like an explicit value).
def set_default_max_tokens(n : int64) {
    g_default_max_new = n
}

//! Give slot `name` a vision arm: load the gemma4uv mmproj at `path` so its chat route accepts
//! `image_url` content parts. Call ONCE per slot, after ``add_model``, before ``init_server`` /
//! ``run_server`` (the vision worker snapshots the arms at init). Anything wrong with the pair
//! PANICS here, not on the first request.
def set_model_vision(name : string; path : string) {
    if (g_server_started) {
        panic("dasllama-server: set_model_vision after init_server - the vision worker snapshots the arms at init; arm the slot before starting")
    }
    let mi = slot_index(name)
    if (mi < 0) {
        panic("dasllama-server: --image-mmproj names slot '{name}', which is not served")
    }
    if (g_slots[mi].vision.proj_dim != 0l) {
        panic("dasllama-server: slot '{name}' already has a vision arm ({base_name(g_slots[mi].vision_path)}) - set_model_vision is once per slot")
    }
    var inscope emb <- load_gemma4uv_embedder(path)
    if (emb.proj_dim != g_slots[mi].model.config.dim) {
        panic("dasllama-server: {base_name(path)} projects to {emb.proj_dim} but {name} is {g_slots[mi].model.config.dim}-wide - mismatched pair")
    }
    var probe <- create_chat_renderer(g_slots[mi].model, "", 1l)
    let no_markers = empty(probe.tmpl.image_pre.parts) || !image_vocab_ok(g_slots[mi].model, probe.tmpl)
    delete probe
    if (no_markers) {
        panic("dasllama-server: {name}'s chat template or vocab carries no image span markers - it has no vision arm")
    }
    g_slots[mi].vision <- emb
    g_slots[mi].vision_path = path
    to_log(LOG_INFO, "dasllama-server: {name} vision arm: {base_name(path)} ({g_slots[mi].vision.proj_dim}-wide, {g_slots[mi].vision.patch}px superpatch)\n")
}

//! Configure long-lived ASR worker threads. Each worker loads and owns its model in its cloned
//! context; model/session pointers never cross context heaps. Call once before ``run_server``.
def configure_asr_workers(path, mmproj : string; workers : int = 1) {
    g_asr_path = path
    g_asr_mmproj = mmproj
    g_asr_worker_count = clamp(workers, 1, 8)
    g_has_asr = !empty(path)
}

//! Stop admitting new inference/ASR work and exit after all accepted work finishes. Also wired to
//! ``POST /shutdown``.
def request_stop() {
    g_shutdown_requested = true
}

var private g_restart_requested = false
var private g_config_surface_json = ""   // main's pre-serialized {config, sources, ...} object
var private g_config_save_path = ""
var private g_config_model_path = ""

//! Register the ``/config`` surface (call before run_server): the launcher's pre-serialized
//! effective-config object, the path ``POST /config`` saves to, and the served model file
//! (its directory feeds the model picker).
def set_config_surface(surface_json : string; save_path : string; model_path : string) {
    g_config_surface_json = surface_json
    g_config_save_path = save_path
    g_config_model_path = model_path
}

//! True when the drain was started by ``POST /restart`` — the lifecycle host exits with the
//! watchdog's config-restart code (4) instead of 0 so it relaunches with the saved config.
def server_restart_requested : bool {
    return g_restart_requested
}

//! True while POST /bench's worker runs its child benchmarks.
def server_bench_running : bool {
    return g_bench_state == "running"
}

// ===== benchmark button (the llama.cpp A/B) =====
// POST /bench runs, QUIESCED ONLY, one lcpp_bench child (--ref runs llama-bench inside it — same
// model, same thread budget, llama-bench methodology) on a worker thread; progress and results
// flow back over a channel the tick loop drains. GET /bench serves state/log/result. Numbers come
// from the child's --json-path record file (the machine rail — stdout carries engine logs), and
// the raw record rides along in the result as `record` for the copy-record affordance.

var private g_lcpp_bin = ""
var private g_daslang_bin = ""
var private g_bench_state = "idle"        // idle | running | done | failed
var private g_bench_log : array<string>
var private g_bench_result_json = ""      // "" until a run completes
var private g_bench_events : Channel?

struct private BenchEvent {
    kind : int         // 0 = log line, 1 = result (text = result json), 2 = failed (text = reason)
    text : string
}

//! Register the benchmark configuration (call before run_server): ``lcpp_bin`` is the llama-bench
//! binary (config key; empty disables the button), ``daslang_bin`` the running daslang used to
//! spawn our bench child ("" in a standalone-exe deployment — also disables).
def set_bench_config(lcpp_bin : string; daslang_bin : string) {
    g_lcpp_bin = lcpp_bin
    g_daslang_bin = daslang_bin
}

def private bench_emit(events : Channel?; kind : int; text : string) {
    events |> push_clone(BenchEvent(kind = kind, text = text))
}

// argv joined into a copy-pasteable shell line (quotes only where a space demands one)
def private cmdline(args : array<string>) : string {
    var parts <- [for (a in args); find(a, " ") >= 0 ? "\"{a}\"" : a]
    let line = join(parts, " ")
    delete parts
    return line
}

def private bench_worker(daslang_bin, lcpp_bin, model_path : string; threads : int; var events : Channel?&) {
    // release nulls the lambda's captured pointer — the capture teardown guard
    // (release_capture_jobque_channel) panics on a still-held channel, and a panic on a
    // catch-less new_thread worker is std::terminate -> silent fail-fast (the "S8
    // second-spawn crash": it was this defer missing, detonating at worker EXIT)
    defer() {
        events |> release
    }
    let t0 = ref_time_ticks()
    let logs_dir = path_join(get_das_root(), "logs")
    mkdir_rec(logs_dir)   // the child writes its --json-path record here; a fresh checkout has no logs/
    let record_path = path_join(logs_dir, "server_bench_record.json")
    remove(record_path)   // drop any prior run's record — a rc=0 child that wrote nothing must not read stale
    bench_emit(events, 0, "lcpp_bench A/B: das + llama-bench, pp512/tg128 x 3 reps (llama-bench methodology) — minutes-class, hold on")
    var out = ""
    var args <- [daslang_bin, "-jit", path_join(get_das_root(), "modules/dasLLAMA/benchmarks/lcpp_bench.das"), "--",
                 "-m", model_path, "-r", "3", "-t", "{threads}",
                 "--ref", lcpp_bin, "-o", "json", "--json-path", record_path]
    bench_emit(events, 0, "$ {cmdline(args)}")
    var rc = run_and_capture(args, out, 3600.0)
    if (rc == 3 && find(out, "restart to apply") >= 0) {
        // [tune] bootstrap: the child tuned this box's kernels and exited with the restart code
        // (the watchdog convention) — relaunch once to run with the winners
        bench_emit(events, 0, "bench child tuned this box's kernels first — relaunching with the winners")
        out = ""
        rc = run_and_capture(args, out, 3600.0)
    }
    delete args
    if (rc != 0) {   // child died — check rc BEFORE parsing so js is never allocated (and leaked) on the failure path
        bench_emit(events, 2, "bench failed (rc={rc}) — tail: {slice(out, max(0, length(out) - 300))}")
        return
    }
    var record_text = ""
    if (stat(record_path).is_valid) {
        record_text = fread(record_path)
    }
    var jerr = ""
    var js = read_json(record_text, jerr)
    if (js == null) {   // rc==0 but the record didn't materialize or parse (see bench_worker's remove-before-launch)
        bench_emit(events, 2, "bench failed (rc={rc}, no parseable record) — tail: {slice(out, max(0, length(out) - 300))}")
        return
    }
    var ours_pp = 0.0lf
    var ours_tg = 0.0lf
    var theirs_pp = 0.0lf
    var theirs_tg = 0.0lf
    let runs = js?[0]?["runs"]
    if (runs != null && runs is _array) {
        for (rv in runs as _array) {
            let eng : string = rv?["engine"] ?? ""
            let pp : double = rv?["tests"]?["pp512"]?["tok_s"] ?? 0.0lf
            let tg : double = rv?["tests"]?["tg128"]?["tok_s"] ?? 0.0lf
            if (eng == "das") {
                ours_pp = pp
                ours_tg = tg
            } elif (eng == "llama.cpp") {
                theirs_pp = pp
                theirs_tg = tg
            }
        }
    }
    unsafe {
        delete js
    }
    if (ours_pp <= 0.0lf || ours_tg <= 0.0lf) {
        bench_emit(events, 2, "bench produced no das rows — tail: {slice(out, max(0, length(out) - 300))}")
        return
    }
    bench_emit(events, 0, "ours: pp512 {ours_pp} tok/s · tg128 {ours_tg} tok/s")
    if (theirs_pp <= 0.0lf || theirs_tg <= 0.0lf) {
        bench_emit(events, 2, "llama-bench rows missing from the record — tail: {slice(out, max(0, length(out) - 300))}")
        return
    }
    bench_emit(events, 0, "theirs: pp512 {theirs_pp} tok/s · tg128 {theirs_tg} tok/s")
    let elapsed = int64(get_time_usec(t0)) / 1000000l
    bench_emit(events, 1,
        "\{\"ours_pp\":{ours_pp},\"ours_tg\":{ours_tg},\"theirs_pp\":{theirs_pp},\"theirs_tg\":{theirs_tg},\"pp_ratio\":{ours_pp / theirs_pp},\"tg_ratio\":{ours_tg / theirs_tg},\"threads\":{threads},\"elapsed_s\":{elapsed},\"ts\":{created_ts()},\"record\":{record_text}}")
}

def private drain_bench_events() {
    if (g_bench_events == null) {
        return
    }
    var more = true
    while (more) {
        more = g_bench_events |> try_pop_clone() $(ev : BenchEvent#) {
            if (ev.kind == 0) {
                g_bench_log |> push(clone_string(ev.text))
                to_log(LOG_INFO, "dasllama-server: bench: {ev.text}\n")
            } elif (ev.kind == 1) {
                g_bench_result_json = clone_string(ev.text)
                g_bench_state = "done"
                to_log(LOG_INFO, "dasllama-server: bench complete\n")
            } else {
                g_bench_log |> push(clone_string(ev.text))
                g_bench_state = "failed"
                to_log(LOG_WARNING, "dasllama-server: bench failed: {ev.text}\n")
            }
        }
    }
}

def private handle_bench_start(var resp : HttpResponse?) : http_status {
    if (g_bench_state == "running") {
        return resp |> JSON(error_body("a benchmark is already running", "invalid_request_error"), http_status.CONFLICT)
    }
    if (empty(g_lcpp_bin) || empty(g_daslang_bin)) {
        return resp |> JSON(
            error_body(empty(g_lcpp_bin) ? "set lcpp_bin in the config to enable benchmarking" : "benchmarking needs a source-tree daslang (standalone exe cannot spawn the bench child)", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    if (total_active() > 0l || total_queued() > 0l || g_asr_pending > 0 || !empty(g_vision_wires)) {
        return resp |> JSON(
            error_body("streams are active — contended numbers are misleading; drain first, then bench", "invalid_request_error"),
            http_status.CONFLICT)
    }
    g_bench_state = "running"
    delete g_bench_log
    g_bench_result_json = ""
    let daslang_bin = g_daslang_bin
    let lcpp_bin = g_lcpp_bin
    let model_path = g_config_model_path
    let threads = get_dispatch_worker_limit() > 0 ? get_dispatch_worker_limit() : 16
    var events_capture = g_bench_events
    new_thread() <| @capture(= daslang_bin, = lcpp_bin, = model_path, = threads, = events_capture) {
        bench_worker(daslang_bin, lcpp_bin, model_path, threads, events_capture)
    }
    to_log(LOG_INFO, "dasllama-server: benchmark started (quiesced) — ours then {lcpp_bin}\n")
    return resp |> JSON("\{\"ok\":true,\"state\":\"running\"}", http_status.ACCEPTED)
}

def private handle_bench_get(var resp : HttpResponse?) : http_status {
    var log_doc = JV(g_bench_log)
    let log_json = write_json_compact(log_doc)
    unsafe {
        delete log_doc
    }
    let result = empty(g_bench_result_json) ? "null" : g_bench_result_json
    return resp |> JSON(
        "\{\"state\":{json_str(g_bench_state)},\"log\":{log_json},\"result\":{result},\"hardware\":{json_str(g_hardware_line)}}",
        http_status.OK)
}

// ===== bake button (the .dlim converter) =====
// POST /bake spawns dasllama-convert as a child (the bench worker's proven single-spawn shape):
// planar slots stream (peak ~16 GB regardless of model size), the GPU-owner slot bakes the
// vulkan flavor against the server's LIVE config (--config, eager — RAM peak ≈ the planes).
// GET /bake serves state/log. The dlim GC (never-loadable images) runs when a bake completes.

var private g_bake_state = "idle"        // idle | running | done | failed
var private g_bake_log : array<string>
var private g_bake_result_json = ""      // "" until a run completes
var private g_bake_events : Channel?
var private g_bake_model = ""            // slot name of the running/last bake
var private g_bake_gguf = ""             // its source path — the completion GC target

struct private BakeEvent {
    kind : int         // 0 = log line, 1 = done (text = result json), 2 = failed (text = reason)
    text : string
}

def private bake_emit(events : Channel?; kind : int; text : string) {
    events |> push_clone(BakeEvent(kind = kind, text = text))
}

def private bake_worker(daslang_bin, gguf_path, flavor, cfg_path : string; var events : Channel?&) {
    // same defer the bench worker carries: a still-held channel at worker exit is std::terminate
    defer() {
        events |> release
    }
    let t0 = ref_time_ticks()
    var args <- [daslang_bin, "-jit", path_join(get_das_root(), "utils/dasllama-convert/main.das"), "--",
                 "-m", gguf_path, "--force"]
    if (flavor == "vulkan") {
        args |> push("-f")
        args |> push("vulkan")
        if (!empty(cfg_path)) {
            args |> push("--config")
            args |> push(cfg_path)
        }
        bake_emit(events, 0, "vulkan flavor bakes eagerly — RAM peak is the model's planes; minutes-class on big models")
    } else {
        args |> push("--stream")
    }
    bake_emit(events, 0, "$ {cmdline(args)}")
    var out = ""
    var rc = run_and_capture(args, out, 7200.0)
    if (rc == 3 && find(out, "restart to apply") >= 0) {
        // [tune] bootstrap: the child tuned this box's kernels first (the watchdog convention)
        bake_emit(events, 0, "converter tuned this box's kernels first — relaunching with the winners")
        out = ""
        rc = run_and_capture(args, out, 7200.0)
    }
    delete args
    if (!empty(cfg_path)) {
        remove(cfg_path)
    }
    if (rc != 0) {
        bake_emit(events, 2, "bake failed (rc={rc}) — tail: {slice(out, max(0, length(out) - 300))}")
        return
    }
    // surface the converter's own receipt lines (identity, output path, wrote N MB)
    peek_data(out) $(bytes) {
        var at = 0
        var line_start = 0
        while (at < length(bytes)) {
            if (bytes[at] == uint8('\n')) {
                let line = slice(bytes, line_start, at)
                if (find(line, "dasllama-convert:") >= 0) {
                    bake_emit(events, 0, clone_string(line))
                }
                line_start = at + 1
            }
            at++
        }
    }
    let elapsed = int64(get_time_usec(t0)) / 1000000l
    bake_emit(events, 1, "\{\"ok\":true,\"flavor\":{json_str(flavor)},\"elapsed_s\":{elapsed},\"ts\":{created_ts()}}")
}

def private drain_bake_events() {
    if (g_bake_events == null) {
        return
    }
    var more = true
    while (more) {
        more = g_bake_events |> try_pop_clone() $(ev : BakeEvent#) {
            if (ev.kind == 0) {
                g_bake_log |> push(clone_string(ev.text))
                to_log(LOG_INFO, "dasllama-server: bake: {ev.text}\n")
            } else {
                if (ev.kind == 1) {
                    g_bake_result_json = clone_string(ev.text)
                    g_bake_state = "done"
                    to_log(LOG_INFO, "dasllama-server: bake complete ({g_bake_model})\n")
                } else {
                    g_bake_log |> push(clone_string(ev.text))
                    g_bake_state = "failed"
                    to_log(LOG_WARNING, "dasllama-server: bake failed: {ev.text}\n")
                }
                // the GC rides every bake completion (and server start) — never-loadable only
                if (!empty(g_bake_gguf)) {
                    let gc = dlim_clean(g_bake_gguf, true)
                    if (gc.removed > 0) {
                        g_bake_log |> push("GC reclaimed {gc.freed >> 20l} MB from {gc.removed} stale image(s)")
                    }
                }
            }
        }
    }
}

def private handle_bake_start(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (g_bake_state == "running") {
        return resp |> JSON(error_body("a bake is already running", "invalid_request_error"), http_status.CONFLICT)
    }
    if (server_bench_running()) {
        return resp |> JSON(error_body("a benchmark is running — one child at a time", "invalid_request_error"), http_status.CONFLICT)
    }
    if (empty(g_daslang_bin)) {
        return resp |> JSON(
            error_body("baking needs a source-tree daslang (standalone exe cannot spawn the converter child)", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    if (total_active() > 0l || total_queued() > 0l || g_asr_pending > 0 || !empty(g_vision_wires)) {
        return resp |> JSON(
            error_body("streams are active — the converter competes for cores and RAM; drain first, then bake", "invalid_request_error"),
            http_status.CONFLICT)
    }
    var model_field = ""
    var jerr = ""
    let body_text = string(req.body)
    var js = read_json(body_text, jerr)
    defer() {
        unsafe {
            delete js
        }
    }
    if (js != null) {
        model_field = js?["model"] ?? ""
    } elif (!empty(body_text)) {
        // a malformed body must NOT fall through to the default slot — that would bake the
        // wrong model on a typo (an empty body legitimately means "the default")
        return resp |> JSON(error_body("request body is not valid JSON: {jerr}", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    let si = slot_index(model_field)
    if (si < 0) {
        return resp |> JSON(error_body("unknown model '{model_field}'", "invalid_request_error"), http_status.NOT_FOUND)
    }
    if (empty(g_slots[si].path)) {
        return resp |> JSON(error_body("slot '{g_slots[si].name}' has no source path — cannot bake", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    // the CURRENT config bakes: the GPU owner's live tier config yields the vulkan flavor,
    // every other slot bakes the planar (CPU) image this box would map
    var flavor = "planar"
    var cfg_path = ""
    if (si == g_gpu_owner) {
        let dc = dlim_config_current("{g_slots[si].model.quant}")
        if (dc.vulkan_on) {
            flavor = "vulkan"
            var terr : string
            let tmp = temp_directory(terr)
            cfg_path = path_join(empty(tmp) ? "." : tmp, "dasllama_bake_cfg_{ref_time_ticks()}.json")
            var wrote = false
            fopen(cfg_path, "wb") $(f) {
                if (f != null) {
                    f |> fwrite(dlim_config_json(dc))
                    wrote = true
                }
            }
            if (!wrote) {
                return resp |> JSON(error_body("cannot write the bake config to '{cfg_path}'", "server_error"), http_status.INTERNAL_SERVER_ERROR)
            }
        }
    }
    g_bake_state = "running"
    delete g_bake_log
    g_bake_result_json = ""
    g_bake_model = g_slots[si].name
    g_bake_gguf = g_slots[si].path
    let daslang_bin = g_daslang_bin
    let gguf_path = g_slots[si].path
    let flavor_c = flavor
    let cfg_path_c = cfg_path
    var events_capture = g_bake_events
    new_thread() <| @capture(= daslang_bin, = gguf_path, = flavor_c, = cfg_path_c, = events_capture) {
        bake_worker(daslang_bin, gguf_path, flavor_c, cfg_path_c, events_capture)
    }
    to_log(LOG_INFO, "dasllama-server: bake started ({g_bake_model}, {flavor})\n")
    return resp |> JSON("\{\"ok\":true,\"state\":\"running\",\"model\":{json_str(g_bake_model)},\"flavor\":{json_str(flavor)}}", http_status.ACCEPTED)
}

def private handle_bake_get(var resp : HttpResponse?) : http_status {
    var log_doc = JV(g_bake_log)
    let log_json = write_json_compact(log_doc)
    unsafe {
        delete log_doc
    }
    let result = empty(g_bake_result_json) ? "null" : g_bake_result_json
    return resp |> JSON(
        "\{\"state\":{json_str(g_bake_state)},\"model\":{json_str(g_bake_model)},\"log\":{log_json},\"result\":{result}}",
        http_status.OK)
}

// the loaded model's image form: which flavor is actually mapped (locks the matching config knobs)
def private slot_flavor(s : ModelSlot) : string {
    if (s.model.metal_blob) {
        return "metal"
    }
    if (long_length(s.model.vkplan) > 0l) {
        return "vulkan"
    }
    return s.model.image_map != null ? "planar" : ""
}

struct private OaiSlotImages {
    model : string
    path : string
    flavor : string       // what THIS process mapped: planar | vulkan | metal | "" (raw gguf)
    trimmed : bool
    images : array<DlimImageInfo>
}

struct private OaiImagesResponse {
    baking : string       // "" or the slot name a bake is running for
    models : array<OaiSlotImages>
}

def private handle_images(var resp : HttpResponse?) : http_status {
    var out = OaiImagesResponse(baking = g_bake_state == "running" ? g_bake_model : "")
    out.models |> reserve(length(g_slots))
    for (s in g_slots) {
        var e = OaiSlotImages(model = s.name, path = s.path, flavor = slot_flavor(s),
                              trimmed = s.model.planes_trimmed)
        if (!empty(s.path)) {
            e.images <- dlim_inventory(s.path)
        }
        out.models |> emplace(e)
    }
    let body = sprint_json(out, false)
    delete out
    return resp |> JSON(body, http_status.OK)
}

var private g_hardware_line = ""   // "<cpu> · N worker lanes · <gpu>" — composed once at init_server

// The CPU marketing name: registry on Windows, /proc/cpuinfo on Linux, sysctl on macOS.
def private cpu_brand : string {
    var out = ""
    if (get_platform_name() == "windows") {
        var args <- ["reg", "query", "HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                     "/v", "ProcessorNameString"]
        run_and_capture(args, out, 10.0)
        delete args
        for (ln in split(out, "\n")) {
            let i = find(ln, "REG_SZ")
            if (i >= 0 && find(ln, "ProcessorNameString") >= 0) {
                return strip(slice(ln, i + length("REG_SZ")))
            }
        }
        return ""
    }
    let cpuinfo = fread("/proc/cpuinfo")
    if (!empty(cpuinfo)) {
        for (ln in split(cpuinfo, "\n")) {
            if (ln |> starts_with("model name")) {
                let c = find(ln, ":")
                return c >= 0 ? strip(slice(ln, c + 1)) : ""
            }
        }
    }
    var args <- ["sysctl", "-n", "machdep.cpu.brand_string"]
    run_and_capture(args, out, 10.0)
    delete args
    return strip(out)
}

def private compose_hardware_line {
    var cpu = cpu_brand()
    if (empty(cpu)) {
        cpu = "unknown cpu"
    }
    // real lane count (jobque workers + the calling thread) — the dispatch cap only ever
    // narrows it. The old rendering printed the cap sentinel 0 as "all", which next to a
    // 64-core brand string read as "uses all 64 cores" on a 16-lane run.
    let lanes = is_job_que_available() ? get_total_hw_jobs() + 1 : 1
    let cap = get_dispatch_worker_limit()
    let lanes_desc = (cap > 0 && cap < lanes) ? "{cap} of {lanes}" : "{lanes}"
    let aff = get_jobque_affinity()
    let aff_desc = aff == 1 ? " · affinity: ideal-cpu hint" : (aff == 2 ? " · affinity: hard mask" : "")
    let tier = gpu_tier_status()
    var gpu_part = ""
    if (!empty(tier.device)) {
        gpu_part = " · {tier.device}"
    } elif (get_metal_mode() != MetalMode.off) {
        gpu_part = " · metal"
    }
    g_hardware_line = "{cpu} · {lanes_desc} worker lanes{aff_desc}{gpu_part}"
}

//! Consume a coalesced request from POST /gc. The lifecycle loop calls this only after update()
//! returns, when no request-local values remain on its stack.
def take_gc_request() : bool {
    let requested = g_gc_requested
    g_gc_requested = false
    return requested
}

// ===== OpenAI wire shapes (response) =====

// Function-calling shapes: `arguments` is JSON TEXT (the OpenAI wire carries it as a string).
// Public (with parse_tool_calls) for the sibling test.
struct OaiToolCallFn {
    name : string
    arguments : string
}

struct OaiToolCall {
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : OaiToolCallFn   // `function` is a das keyword
}

struct private OaiMessage {
    role : string
    content : string
    @optional reasoning_content : string   // the reply's reasoning span (thinking models; absent when empty)
    @optional tool_calls : array<OaiToolCall>
}

struct private OaiChoice {
    index : int
    message : OaiMessage
    finish_reason : string
}

struct private OaiUsage {
    prompt_tokens : int64
    completion_tokens : int64
    total_tokens : int64
}

struct private OaiChatResponse {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiChoice>
    usage : OaiUsage
}

// Streaming (stream=true) chunk shapes. @optional so an empty delta emits `{}` and an absent
// finish_reason is dropped (clients read absent as null) — matching OpenAI's chunk framing.
struct private OaiStreamToolCall {
    index : int
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : OaiToolCallFn
}

struct private OaiDelta {
    @optional role : string
    @optional content : string
    @optional reasoning_content : string   // streamed reasoning deltas (the DeepSeek/llama.cpp framing)
    @optional tool_calls : array<OaiStreamToolCall>
}

struct private OaiStreamChoice {
    index : int
    delta : OaiDelta
    @optional finish_reason : string
}

struct private OaiStreamChunk {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiStreamChoice>
}

struct private OaiComplStreamChoice {
    index : int
    text : string
    @optional finish_reason : string
}

struct private OaiComplStreamChunk {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiComplStreamChoice>
}

struct private OaiComplChoice {
    index : int
    text : string
    finish_reason : string
}

struct private OaiComplResponse {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiComplChoice>
    usage : OaiUsage
}

struct private OaiModel {
    id : string
    @rename = "object" _object : string
    created : int64
    owned_by : string
}

struct private OaiModelList {
    @rename = "object" _object : string
    data : array<OaiModel>
}

struct private OaiErrDetail {
    message : string
    @rename = "type" _type : string
}

struct private OaiErr {
    error : OaiErrDetail
}

// /v1/audio/transcriptions response shapes.
struct private OaiTranscription {
    text : string
}

struct private OaiVerboseSegment {
    id : int
    start : float
    end : float
    text : string
}

struct private OaiVerboseTranscription {
    task : string
    language : string
    duration : float
    text : string
    segments : array<OaiVerboseSegment>
}

enum private AsrEventKind {
    ready
    started
    completed
    stopped
}

struct private AsrJob {
    id : int64
    temp_path : string
    language : string
    response_format : string
    translate : bool
    stop : bool
}

struct private AsrEvent {
    kind : AsrEventKind
    id : int64
    status : int
    content_type : string
    body : string
    audio_s : float      // completed: decoded clip length in seconds (0 on failures)
}

//! One ASR job on the poll surface: running while the worker holds it, then done/failed with
//! its wall time and RTF (audio seconds per wall second) — lingers ~30 s like finished streams.
struct private AsrJobInfo {
    id : int64
    state : string       // "running" | "done" | "failed"
    audio_s : float
    wall_ms : float
    rtf : float
    started_at : int64   // ref_time_ticks at the started event
    done_at : int64      // 0 while running
}

struct private AsrWire {
    @do_not_delete writer : HttpResponseWriter?
}

var g_asr_recent : array<AsrJobInfo>   // running + lingering done jobs for the poll surface
var g_asr_done_jobs = 0l               // cumulative completed transcriptions
var g_asr_audio_s = 0.0                // cumulative audio seconds transcribed
var g_asr_jobs : Channel?
var g_asr_events : Channel?
var g_asr_wires : table<int64; AsrWire>
var g_asr_ready = 0
var g_asr_active = 0
var g_asr_pending = 0
let ASR_QUEUE_LIMIT = 32
let ASR_WRITER_KEEPALIVE_MS = 3600000
let LLM_WRITER_KEEPALIVE_MS = 3600000
let DEFAULT_WRITER_KEEPALIVE_MS = 75000

// /v1/embeddings response shapes.
struct private OaiEmbedding {
    @rename = "object" _object : string
    embedding : array<float>
    index : int
}

struct private OaiEmbUsage {
    prompt_tokens : int64
    total_tokens : int64
}

struct private OaiEmbeddingList {
    @rename = "object" _object : string
    data : array<OaiEmbedding>
    model : string
    usage : OaiEmbUsage
}

// ===== Helpers =====

// Stream ids are the request counter: the scheduler keys on the int64, the wire keys the JSON id.
def private next_id() : int64 {
    g_req_counter ++
    return g_req_counter
}

// `created` is a unix-seconds field no OpenAI client validates; a monotone stand-in is honest and
// avoids a wall-clock dependency in the hot path.
def private created_ts() : int64 {
    return 1751200000l + g_req_counter
}

def private error_body(msg, kind : string) : string {
    return sprint_json(OaiErr(error = OaiErrDetail(message = msg, _type = kind)), false)
}

// Terminal error on a streaming route's writer (chat/completions run through the writer).
def private bad_request(srv : WebSocketServer; var writer : HttpResponseWriter?; msg : string) {
    to_log(LOG_WARNING, "dasllama-server: 400 rejected: {msg}\n")
    respond(srv, writer, int(http_status.BAD_REQUEST), "application/json",
            error_body(msg, "invalid_request_error"))
}

def private service_unavailable(srv : WebSocketServer; var writer : HttpResponseWriter?; msg : string) {
    respond(srv, writer, int(http_status.SERVICE_UNAVAILABLE), "application/json",
            error_body(msg, "server_error"))
}

def private reject_deferred_while_draining(
                                           srv : WebSocketServer;
                                           var writer : HttpResponseWriter?
                                           ) : bool {
    if (!g_shutdown_requested) {
        return false
    }
    service_unavailable(srv, writer, "the server is draining for shutdown — retry shortly")
    return true
}

// Bytes of an INCOMPLETE trailing UTF-8 sequence (0 = the string ends on a character boundary).
// A token piece can end mid-character (multi-byte chars span BPE tokens); every SSE delta must
// be valid UTF-8 on its own or strict clients (the OpenAI python lib) abort the stream.
def private utf8_hold_bytes(s : string) : int {
    var hold = 0
    peek_data(s) $(arr) {
        let n = length(arr)
        var back = 0
        while (back < 4 && back < n) {
            let b = uint(arr[n - 1 - back])
            if ((b & 0xC0u) != 0x80u) {         // found a non-continuation byte
                if (b >= 0xC0u) {               // multi-byte lead: expected sequence length
                    let need = b >= 0xF0u ? 4 : (b >= 0xE0u ? 3 : 2)
                    if (back + 1 < need) {
                        hold = back + 1         // sequence is cut short — hold it back
                    }
                }
                break                           // ascii / complete / invalid: nothing to hold
            }
            back++
        }
    }
    return hold
}

// One chat.completion.chunk SSE event. role-only for the opening delta, content and/or
// reasoning_content for each token, empty delta + finish_reason for the terminating chunk.
def private send_chat_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                            id : string; created : int64; role, content, finish : string;
                            reasoning : string = "") {
    var chunk = OaiStreamChunk(id = id, _object = "chat.completion.chunk", created = created, model = cur_name())
    chunk.choices |> emplace(OaiStreamChoice(index = 0,
                                             delta <- OaiDelta(role = role, content = content,
                                                               reasoning_content = reasoning),
                                             finish_reason = finish))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

// The whole call set of a streaming reply as ONE tool_calls delta (index enumerates calls per the
// OpenAI incremental framing); the caller follows with the finish_reason="tool_calls" chunk.
def private send_chat_tool_calls_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                                       id : string; created : int64; calls : array<OaiToolCall>) {
    var chunk = OaiStreamChunk(id = id, _object = "chat.completion.chunk", created = created, model = cur_name())
    var delta = OaiDelta()
    delta.tool_calls |> reserve(length(calls))
    var idx = 0
    for (c in calls) {
        delta.tool_calls |> push(OaiStreamToolCall(index = idx, id = c.id, _type = c._type,
                                                   _function = c._function))
        idx ++
    }
    chunk.choices |> emplace(OaiStreamChoice(index = 0, delta <- delta))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

//! Extract Hermes-style tool calls from a finished reply: every well-formed open…close span becomes
//! one call; returns the content BEFORE the first opener (everything when nothing parses, so no
//! reply text is silently dropped). The scan itself is dasllama_tools' ``marker_parse`` — this
//! wrapper maps its calls onto the OpenAI wire shapes and logs when spans parse to nothing.
def parse_tool_calls(full : string; open_mark, close_mark, id_base : string;
                     var calls : array<OaiToolCall>) : string {
    var tr <- marker_parse(full, open_mark, close_mark)
    if (empty(tr.calls) && !empty(open_mark) && find(full, open_mark) >= 0) {
        to_log(LOG_WARNING, "dasllama-server: {open_mark} span(s) present but no calls parsed (unterminated or malformed JSON)\n")
    }
    calls |> reserve(length(calls) + length(tr.calls))
    for (c in tr.calls) {
        calls |> push(OaiToolCall(id = "{id_base}_{length(calls)}", _type = "function",
                                  _function = OaiToolCallFn(name = c.name, arguments = c.args)))
    }
    let content = tr.content
    delete tr
    return content
}

// Normalize one of the JSON call objects used by common model families. OpenAI/Hermes/Mistral
// spell the arguments "arguments"; Llama 3 spells them "parameters". Some templates replay the
// OpenAI wire object itself, with name/arguments nested under "function".
def private tool_args_text(body : JsonValue?) : string {
    let av = body?["arguments"]
    if (av != null && !(av.value is _null)) {
        if (av.value is _string) {
            let s = av ?? ""
            return empty(s) ? "\{}" : s
        }
        return write_json_compact(av)
    }
    let parameters = body?["parameters"]
    if (parameters != null && !(parameters.value is _null)) {
        return write_json_compact(parameters)
    }
    let args = body?["args"]
    if (args != null && !(args.value is _null)) {
        return write_json_compact(args)
    }
    return "\{}"
}

def private append_json_tool_body(body : JsonValue?; id_base : string;
                                  var calls : array<OaiToolCall>) : bool {
    var cname : string = body?["name"] ?? ""
    if (empty(cname)) {
        let fn = body?["function"]
        if (fn != null && fn.value is _string) {
            cname = fn ?? ""
        }
    }
    if (empty(cname)) {
        return false
    }
    calls |> push(OaiToolCall(id = "{id_base}_{length(calls)}", _type = "function",
                              _function = OaiToolCallFn(name = cname, arguments = tool_args_text(body))))
    return true
}

def private append_json_tool_call(cjs : JsonValue?; id_base : string;
                                  var calls : array<OaiToolCall>) : bool {
    if (cjs == null || !(cjs.value is _object)) {
        return false
    }
    let fv = cjs?["function"]
    if (fv != null && fv.value is _object) {
        return append_json_tool_body(fv, id_base, calls)
    }
    return append_json_tool_body(cjs, id_base, calls)
}

def private parse_tool_json(body : string; id_base : string; var calls : array<OaiToolCall>) : bool {
    var perr = ""
    var js = read_json(strip(body), perr)
    if (js == null) {
        return false
    }
    defer() {
        unsafe {
            delete js
        }
    }
    let before = length(calls)
    if (js.value is _array) {
        for (cjs in (js.value as _array)) {
            append_json_tool_call(cjs, id_base, calls)
        }
    } else {
        append_json_tool_call(js, id_base, calls)
    }
    return length(calls) > before
}

//! Normalize the output envelopes used by the model families most often served through an
//! OpenAI-compatible endpoint. The template-declared wrapper wins; the remaining probes cover
//! Hermes/Qwen XML, Mistral's array marker, Llama 3's python tag, and strict bare JSON. Bare JSON
//! is accepted only when it is entirely a valid named call, so ordinary JSON answers stay content.
def parse_tool_calls_auto(full : string; declared_open, declared_close, id_base : string;
                          var calls : array<OaiToolCall>) : string {
    if (!empty(declared_open) && !empty(declared_close) && find(full, declared_open) >= 0) {
        let content = parse_tool_calls(full, declared_open, declared_close, id_base, calls)
        if (!empty(calls)) {
            return content
        }
    }
    if ((declared_open != "<tool_call>" || declared_close != "</tool_call>") &&
        find(full, "<tool_call>") >= 0) {
        let content = parse_tool_calls(full, "<tool_call>", "</tool_call>", id_base, calls)
        if (!empty(calls)) {
            return content
        }
    }
    let mistral = find(full, "[TOOL_CALLS]")
    if (mistral >= 0 &&
        parse_tool_json(slice(full, mistral + length("[TOOL_CALLS]")), id_base, calls)) {
        return strip(slice(full, 0, mistral))
    }
    let llama = find(full, "<|python_tag|>")
    if (llama >= 0 &&
        parse_tool_json(slice(full, llama + length("<|python_tag|>")), id_base, calls)) {
        return strip(slice(full, 0, llama))
    }
    if (parse_tool_json(full, id_base, calls)) {
        return ""
    }
    return full
}

def private send_compl_chunk(srv : WebSocketServer; var writer : HttpResponseWriter?;
                             id : string; created : int64; text, finish : string) {
    var chunk = OaiComplStreamChunk(id = id, _object = "text_completion", created = created, model = cur_name())
    chunk.choices |> push(OaiComplStreamChoice(index = 0, text = text, finish_reason = finish))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

// Clamp a requested reply budget to [1, model context].
def private clamp_max(requested : int64) : int64 {
    return clamp(requested, 1l, cur_model().config.seq_len)
}

// Error on a buffered (resp-based) route — /v1/models and the audio endpoints use the response path.
def private resp_error(var resp : HttpResponse?; status : http_status; msg : string) : http_status {
    return resp |> JSON(error_body(msg, "invalid_request_error"), status)
}

def private tmpdir() : string {
    var err = ""
    let d = temp_directory(err)   // fio: platform temp dir (Windows GetTempPath, POSIX $TMPDIR/tmp)
    return empty(d) ? "/tmp" : d
}

// ===== Unhandled-request visibility =====

// Top-level fields each JSON route reads; anything else in a request logs a warning instead of
// silently vanishing — the diagnostic for "agent X immediately fails against this server".
let CHAT_FIELDS : table<string> <- { "model", "messages", "stream", "temperature", "top_k", "top_p", "min_p",
                                     "presence_penalty", "frequency_penalty", "repeat_penalty", "seed",
                                     "max_tokens", "max_completion_tokens", "tools", "tool_choice",
                                     "enable_thinking", "chat_template_kwargs", "truncation" }
let COMPL_FIELDS : table<string> <- { "model", "prompt", "stream", "temperature", "top_k", "top_p", "min_p",
                                     "presence_penalty", "frequency_penalty", "repeat_penalty", "seed",
                                     "max_tokens" }
let EMB_FIELDS : table<string> <- { "model", "input" }

// Sampling knobs shared by both completion routes — OpenAI names, plus the llama.cpp-style
// extensions (top_k / min_p / repeat_penalty). Absent fields keep the greedy defaults.
def private parse_sampling(js : JsonValue?) : SamplingParams {
    var params = SamplingParams()
    params.temp = js?["temperature"] ?? 0.0
    params.top_k = js?["top_k"] ?? 0l
    params.top_p = js?["top_p"] ?? 1.0
    params.min_p = js?["min_p"] ?? 0.0
    params.penalty = js?["repeat_penalty"] ?? 1.0
    params.presence_penalty = js?["presence_penalty"] ?? 0.0
    params.frequency_penalty = js?["frequency_penalty"] ?? 0.0
    return params
}

def private warn_ignored_fields(js : JsonValue?; known : table<string>; route : string) {
    if (js == null || !(js.value is _object)) {
        return
    }
    for (k in keys(js.value as _object)) {
        if (!key_exists(known, k)) {
            to_log(LOG_WARNING, "{route}: ignoring unsupported request field '{k}'\n")
        }
    }
}

// A message's content: a plain string, or the OpenAI content-parts array (agents send
// [\{type:"text", text:"..."}] even for pure text). Text parts concatenate into the return value;
// `image_url` parts push their url into `images` for the caller to decide on; anything else logs.
def private message_parts(mv : JsonValue?; route : string; var images : array<string>) : string {
    let cv = mv?["content"]
    if (cv == null || cv.value is _null) {
        return ""
    }
    if (cv.value is _string) {
        return cv ?? ""
    }
    if (cv.value is _array) {
        var parts : array<string>
        for (pv in (cv.value as _array)) {
            let ptype = pv?["type"] ?? ""
            if (ptype == "text") {
                parts |> push(pv?["text"] ?? "")
            } elif (ptype == "image_url") {
                // object form \{url:...} per OpenAI; the bare-string shorthand some clients send works too
                let uv = pv?["image_url"]
                images |> push(uv != null && uv.value is _string ? (uv ?? "") : (uv?["url"] ?? ""))
            } elif (ptype == "input_image") {   // the Responses-API spelling: image_url is a string
                images |> push(pv?["image_url"] ?? "")
            } else {
                to_log(LOG_WARNING, "{route}: skipping non-text content part '{ptype}'\n")
            }
        }
        let s = join(parts, "")
        delete parts
        return s
    }
    to_log(LOG_WARNING, "{route}: message content is neither a string nor a parts array — parsed as empty\n")
    return ""
}

// `message_parts` for routes with no image path: image parts warn and drop.
def private message_text(mv : JsonValue?; route : string) : string {
    var images : array<string>
    let s = message_parts(mv, route, images)
    if (!empty(images)) {
        to_log(LOG_WARNING, "{route}: skipping {length(images)} image content part(s) — no vision arm on this path\n")
    }
    delete images
    return s
}

// image file-byte and pixel ceilings: past either the request is a 400, never an allocation.
// 32 MB of file covers any sane photo; 67 MP (64 * 2^20) is ~4x a modern phone sensor.
let private IMAGE_MAX_FILE_BYTES = 32 * 1024 * 1024
let private IMAGE_MAX_PIXELS = 64l * 1024l * 1024l

// `data:image/png;base64,<payload>` -> the raw file bytes (whitespace-tolerant: wrapped base64
// from GNU `base64` decodes too). The ONLY image_url form the server accepts: a remote URL would
// make the server fetch on its request thread. On failure `why` is set — the caller answers 400.
def private decode_data_uri_bytes(url : string; var bytes : array<uint8>; var why : string&) : bool {
    if (!starts_with(url, "data:")) {
        why = "image_url must be a data: URI (remote URLs are not fetched) — got a {length(url)}-char non-data url"
        return false
    }
    let marker = find(url, ";base64,")
    if (marker < 0) {
        why = "image_url must be base64-encoded (data:<mime>;base64,<payload>)"
        return false
    }
    let payload_at = marker + length(";base64,")
    let max_encoded = (IMAGE_MAX_FILE_BYTES / 3) * 4 + 8   // base64 expansion of the cap, plus padding slack (wire bytes, pre-strip)
    if (length(url) - payload_at > max_encoded) {
        why = "image payload exceeds the {IMAGE_MAX_FILE_BYTES / (1024 * 1024)} MB cap (wrapped base64 counts its newlines)"
        return false
    }
    var raw : array<uint8>
    defer() { delete raw }
    peek_data(url) $(d) {
        raw |> reserve(length(d) - payload_at)
        for (i in range(payload_at, length(d))) {
            let b = int(d[i])
            if (b != '\n' && b != '\r' && b != ' ' && b != '\t') {
                raw |> push(d[i])
            }
        }
    }
    if (base64_decode(raw, bytes) <= 0) {
        why = "image_url payload is not valid base64"
        return false
    }
    return true
}

// decoded-bytes -> RGB8, under the pixel cap. Zero-size + `why` on failure — the caller 400s.
def private decode_image_bytes(bytes : array<uint8>; var why : string&) : VisionImage {
    var img <- load_image_rgb(bytes, IMAGE_MAX_PIXELS)
    if (img.width == 0) {
        // name the right mistake: a valid-but-huge image is not a corrupt one
        let dims = probe_image_size(bytes)
        if (int64(dims.x) * int64(dims.y) > IMAGE_MAX_PIXELS) {
            why = "image is {dims.x}x{dims.y} ({int64(dims.x) * int64(dims.y) / 1000000l} MP), over the {IMAGE_MAX_PIXELS / 1000000l} MP cap"
        } else {
            why = "could not decode the image ({length(bytes)} bytes) — png, jpeg, bmp, gif, tga and psd are supported"
        }
    }
    return <- img
}

// Unknown endpoint: log the probe (method, path, body head) and 404 with an OpenAI error body.
def private handle_unknown(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let body = string(req.body)
    let head = length(body) > 400 ? slice(body, 0, 400) : body
    if (empty(head)) {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path}\n")
    } else {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path} body: {head}\n")
    }
    return resp_error(resp, http_status.NOT_FOUND, "unknown endpoint: {req.path}")
}

// ===== Handlers =====

def private handle_models(var resp : HttpResponse?) : http_status {
    var list = OaiModelList(_object = "list")
    list.data |> reserve(length(g_slots) + 1)
    for (s in g_slots) {
        list.data |> push(OaiModel(id = s.name, _object = "model", created = created_ts(), owned_by = "dasllama"))
    }
    if (g_has_asr && !empty(g_slots)) {
        list.data |> push(OaiModel(id = "{g_slots[g_default_slot].name}-asr", _object = "model", created = created_ts(), owned_by = "dasllama"))
    }
    let body = sprint_json(list, false)
    delete list
    return resp |> JSON(body, http_status.OK)
}

// /v1/embeddings: mean-pooled, L2-normalized sentence embeddings over the facade `embed` verb.
// `input` is a string or an array of strings; usage is counted via the facade `encode` (cheap
// relative to the forward). Decoder-only-as-embedder — RAG-grade vectors, not a dedicated model.
def private handle_embeddings(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, EMB_FIELDS, "/v1/embeddings")
    let want_model = js?["model"] ?? ""
    let mi = slot_index(want_model)
    if (mi < 0) {
        return resp_error(resp, http_status.NOT_FOUND, "model '{want_model}' not found — GET /v1/models lists the served ids")
    }
    g_cur = mi
    install_slot_marks(mi)   // this handler evals IN-handler — the embed forward needs ITS marks
    let iv = js?["input"]
    if (iv == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "'input' is required (a string or array of strings)")
    }
    var inputs : array<string>
    if (iv.value is _array) {
        inputs <- from_JV(iv, type<array<string>>)   // nolint:PERF030 — first write to the empty decl above
    } else {
        let single = iv ?? ""
        if (empty(single)) {
            return resp_error(resp, http_status.BAD_REQUEST, "'input' must be a non-empty string or array of strings")
        }
        inputs |> push(single)
    }
    if (empty(inputs)) {
        return resp_error(resp, http_status.BAD_REQUEST, "'input' is empty")
    }

    var out = OaiEmbeddingList(_object = "list", model = cur_name())
    out.data |> reserve(length(inputs))
    var prompt_tokens = 0l
    var idx = 0
    for (text in inputs) {
        let toks <- encode(cur_model(), text, true, false)
        prompt_tokens += long_length(toks)
        var vec = embed(cur_model(), text)
        out.data |> emplace(OaiEmbedding(_object = "embedding", embedding <- vec, index = idx))
        idx ++
    }
    out.usage = OaiEmbUsage(prompt_tokens = prompt_tokens, total_tokens = prompt_tokens)
    let body = sprint_json(out, false)
    delete out
    delete inputs
    return resp |> JSON(body, http_status.OK)
}

// POST /v1/models/activate {"model": name}: admin warm-switch — make `name` the stepped slot
// and move the GPU tier to it NOW instead of waiting for the owner to drain. 409 while any
// work is live: the switch drops the owner's whole VRAM state.
def private handle_activate(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() {
        unsafe {
            delete js
        }
    }
    let want = js?["model"] ?? ""
    if (empty(want)) {
        // an admin switch has one required parameter — silently activating the default slot on
        // a malformed body would hide the client bug behind a real VRAM-state switch
        return resp |> JSON(
            error_body("'model' is required — GET /v1/models lists the served ids", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    let mi = slot_index(want)
    if (mi < 0) {
        return resp |> JSON(
            error_body("model '{want}' not found — GET /v1/models lists the served ids", "model_not_found"),
            http_status.NOT_FOUND)
    }
    if (total_active() > 0l || total_queued() > 0l || g_asr_pending > 0 || !empty(g_vision_wires)) {
        return resp |> JSON(
            error_body("streams are live (active {total_active()}, queued {total_queued()}) — retry when idle", "busy"),
            http_status.CONFLICT)
    }
    let t0 = ref_time_ticks()
    g_active = mi
    g_cur = mi
    set_mtp_spec(g_slots[mi].sched.mtp)
    install_slot_marks(mi)
    bind_gpu_slot(mi)   // no-op for cpu slots and the current owner
    g_slots[mi].last_used = ref_time_ticks()
    let sw_ms = int64(get_time_usec(t0)) / 1000l
    return resp |> JSON(
        "\{\"ok\":true,\"model\":{json_str(g_slots[mi].name)},\"backend_effective\":{json_str(g_slots[mi].backend_effective)},\"switch_ms\":{sw_ms}}",
        http_status.OK)
}

// Resolve a streaming request's "model" field to a slot (sets g_cur). False = unknown model —
// the 404 with OpenAI error framing is already on the wire.
def private resolve_stream_slot(srv : WebSocketServer; var writer : HttpResponseWriter?; js : JsonValue?) : bool {
    let want = js?["model"] ?? ""
    let mi = slot_index(want)
    if (mi < 0) {
        respond(srv, writer, int(http_status.NOT_FOUND), "application/json",
                error_body("model '{want}' not found — GET /v1/models lists the served ids", "model_not_found"))
        return false
    }
    g_cur = mi
    return true
}

// Validate the rendered prompt, submit it to the scheduler, and register the wire that routes its
// SchedEvents back to `writer`. Owns the mandatory 400 (prompt does not fit the context — submit
// would panic) and the 503 (queue full). Moves prompt/stop_ids/close_toks into the request.
def private register_request(srv : WebSocketServer; var writer : HttpResponseWriter?;
                             var prompt, stop_ids, close_toks : array<int64>;
                             var media : array<float>; media_at, n_media : int64;
                             params : SamplingParams; max_new : int64;
                             streaming, is_chat : bool; prefix : string;
                             tools_declared : bool = false; call_open : string = ""; call_close : string = "";
                             seed : int = 0; think : ThinkStream = ThinkStream();
                             tool_mode : ToolMode = ToolMode.none; reuse_id : int64 = 0l) {
    let np = long_length(prompt) + n_media
    if (np + 1l > cur_model().config.seq_len) {
        bad_request(srv, writer, "the rendered prompt is {np} tokens; the model context is {cur_model().config.seq_len} — shorten the conversation or raise --ctx")
        return
    }
    let rid = reuse_id != 0l ? reuse_id : next_id()   // a resumed image request keeps its park id, so one [req N] spans the request
    let cid = "{prefix}-{rid}"
    let created = created_ts()
    g_media_rows += n_media
    var preq <- PendingReq(id = rid, params = params, max_new = max_new, seed = seed,
                           media_at = media_at, n_media = n_media,
                           media_non_causal = n_media > 0l)   // image rows only on this route: the gemma image span is non-causal
    preq.prompt <- prompt
    preq.stop_ids <- stop_ids
    preq.close_toks <- close_toks
    preq.media_embd <- media
    if (!submit(cur_model(), cur_sched(), preq)) {
        delete preq
        to_log(LOG_WARNING, "dasllama-server: 503 queue full ({cur_sched().max_queue}) — rejected {prefix} request ({np} tok)\n")
        service_unavailable(srv, writer, "the server is at capacity ({cur_sched().max_queue} requests queued) — retry shortly")
        return
    }
    g_slots[g_cur].last_used = ref_time_ticks()
    set_writer_keepalive_timeout(srv, writer, LLM_WRITER_KEEPALIVE_MS)
    to_log(LOG_INFO, "[req {rid}] {prefix}: prompt={np} tok, stream={streaming}, max_new={max_new}\n")
    var wire <- StreamWire(writer = writer, streaming = streaming, is_chat = is_chat,
                           slot = g_cur, id = cid, created = created,
                           has_tools = tools_declared && tool_mode != ToolMode.none,
                           last_keepalive_ticks = ref_time_ticks(),
                           call_open = call_open, call_close = call_close,
                           tool_mode = tool_mode, think = think)
    g_wires[rid] <- wire
    if (streaming && is_chat) {
        send_chat_chunk(srv, writer, cid, created, "assistant", "", "")   // opening role delta
    }
}

// Parse + validate + render + register, then return — no generation in the handler. The prompt is
// rendered through the S1 render seam (tokens only, no KV), so a queued request holds no cache
// memory; the session exists only once the scheduler admits the stream.
// Render a user-led suffix of the transcript. System messages are always retained (they were
// folded into `system`); `start` applies only to conversational messages in the original array.
def private render_chat_suffix(msgs_v : JsonValue?; system : string; tools : array<string>;   // nolint:STYLE037,STYLE038 — one arm per message role, each accumulating into the same renderer state
                               max_new : int64; start : int; think_set, think_val, has_image : bool;
                               var prompt, stop_ids, close : array<int64>&;
                               var call_open, call_close : string&; var think_out : ThinkStream&;
                               var tool_mode_out : ToolMode&; var media_at : int64&) : string {
    var chat <- create_chat_renderer(cur_model(), system, max_new)
    defer() { delete chat }
    if (think_set) {   // absent from the request => the family's own default (gemma-4 E: off)
        set_thinking(chat, think_val)
    }
    if (!empty(tools)) {
        if (chat.tmpl.tool_mode == ToolMode.none) {
            return "'tools' is not supported: the {cur_name()} chat template declares no tool-call format"
        }
        var render_tools <- [for (tool in tools); tool]
        set_tools(chat, render_tools)
        delete render_tools
    }

    // tool_call_id -> function name over the WHOLE transcript: results carry their names, so the
    // name-spelling framings (harmony, gemma-4) pair correctly even out of call order
    var id2name : table<string; string>
    defer() { delete id2name }
    for (av in (msgs_v.value as _array)) {
        let tcv = av?["tool_calls"]
        if (((av?["role"] ?? "") == "assistant") && tcv != null && tcv.value is _array) {
            for (tcall in (tcv.value as _array)) {
                let cid = tcall?["id"] ?? ""
                let cnm = tcall?["function"]?["name"] ?? ""
                if (!empty(cid) && !empty(cnm)) {
                    id2name[cid] = cnm
                }
            }
        }
    }
    var rendered : array<int64>
    var pending = false
    var seen_images : array<string>   // handle_chat already validated and queued these
    defer() { delete seen_images }
    var tool_results : array<string>
    var tool_names : array<string>
    defer() {
        delete tool_results
        delete tool_names
    }
    for (i in range(length(msgs_v.value as _array))) {
        let mv = (msgs_v.value as _array)[i]
        let role = mv?["role"] ?? ""
        if (role != "system" && i < start) {
            continue
        }
        let content = message_parts(mv, "/v1/chat/completions", seen_images)
        if (role != "tool" && !empty(tool_results)) {
            add_tool_results(chat, tool_results, tool_names)
            tool_results |> clear()
            tool_names |> clear()
            pending = true
        }
        if (role == "user") {
            add_user(chat, content)
            pending = true
        } elif (role == "assistant") {
            var calls : array<string>
            let tc = mv?["tool_calls"]
            if (tc != null && tc.value is _array) {
                calls |> reserve(length(tc.value as _array))
                for (tcall in (tc.value as _array)) {
                    let fv = tcall?["function"]
                    var nv = JV(fv?["name"] ?? "")
                    let cname = write_json_compact(nv)
                    unsafe {
                        delete nv
                    }
                    let av = fv?["arguments"]
                    var argstr = "\{}"
                    if (av != null && av.value is _string) {
                        let s = av ?? ""
                        if (!empty(s)) {
                            argstr = s
                        }
                    } elif (av != null && !(av.value is _null)) {
                        argstr = write_json_compact(av)
                    }
                    calls |> push("\{\"name\": {cname}, \"arguments\": {argstr}}")
                }
            }
            if (pending) {
                if (!empty(calls)) {
                    render_assistant_calls(cur_model(), chat, content, calls, rendered)
                } else {
                    render_assistant(cur_model(), chat, content, rendered)
                }
                pending = false
            }
            delete calls
        } elif (role == "tool") {
            let tcid = mv?["tool_call_id"] ?? ""
            tool_results |> push(content)
            tool_names |> push(id2name?[tcid] ?? "")   // "" falls back to positional pairing
        } elif (role != "system") {
            to_log(LOG_WARNING, "/v1/chat/completions: dropping message with unsupported role '{role}'\n")
        }
    }
    if (!empty(tool_results)) {
        add_tool_results(chat, tool_results, tool_names)
        pending = true
    }
    if (!pending) {
        delete rendered
        return "the last message must be a user or tool message"
    }
    media_at = 0l
    if (has_image) {
        // the image turn renders as TWO token spans; the soft-token rows prefill between them, so
        // the scheduler gets one flat prompt plus the index the rows splice at
        var head : array<int64>
        var tail : array<int64>
        render_turn_image(cur_model(), chat, head, tail)
        media_at = long_length(rendered) + long_length(head)
        rendered |> push_from(head, tail)
        delete head
        delete tail
    } else {
        var turn <- render_turn(cur_model(), chat)
        rendered |> push_from(turn)
        delete turn
    }
    // out-params: the sole caller hands them in empty and deletes them before every re-render
    prompt <- rendered      // nolint:PERF030 — empty on entry
    stop_ids <- effective_stop_ids(chat)   // nolint:PERF030 — empty on entry; + thinking-off extras
    close <- render_close(cur_model(), chat)   // nolint:PERF030 — empty on entry
    call_open = chat.tmpl.tool_call_open
    call_close = chat.tmpl.tool_call_close
    think_out = make_think_stream(chat)   // armed per the family's reply format + effective thinking
    tool_mode_out = chat.tmpl.tool_mode
    return ""
}

// Move to the next user-led suffix, dropping a complete oldest user/assistant/tool exchange.
def private next_user_start(msgs_v : JsonValue?; start : int) : int {
    var saw_user = false
    for (i in range(start, length(msgs_v.value as _array))) {
        if (((msgs_v.value as _array)[i]?["role"] ?? "") == "user") {
            if (saw_user) {
                return i
            }
            saw_user = true
        }
    }
    return -1
}

def private handle_chat(srv : WebSocketServer; var req : HttpRequest?; var writer : HttpResponseWriter?) {
    if (reject_deferred_while_draining(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, CHAT_FIELDS, "/v1/chat/completions")
    if (!resolve_stream_slot(srv, writer, js)) {
        return
    }
    let msgs_v = js?["messages"]
    if (msgs_v == null || !(msgs_v.value is _array)) {
        bad_request(srv, writer, "'messages' must be an array")
        return
    }
    // v1 vision: ONE image, on the FINAL message (a user message carrying it). Any other
    // placement warns and drops — an earlier turn has no render seam (replays go through
    // render_assistant, which has no splice), and a trailing tool turn must render as tools.
    {
        var images : array<string>
        defer() { delete images }
        let n_msgs = length(msgs_v.value as _array)
        for (i in range(n_msgs)) {
            var here : array<string>
            message_parts((msgs_v.value as _array)[i], "/v1/chat/completions", here)
            let is_final_user = i == n_msgs - 1 && ((msgs_v.value as _array)[i]?["role"] ?? "") == "user"
            if (!empty(here) && !is_final_user) {
                to_log(LOG_WARNING, "/v1/chat/completions: dropping {length(here)} image(s) on message {i} — the image is read from the FINAL user message only\n")
            } elif (!empty(here)) {
                images |> push_from(here)
            }
            delete here
        }
        if (!empty(images)) {
            if (g_slots[g_cur].vision.proj_dim == 0l) {
                bad_request(srv, writer, "model '{cur_name()}' has no vision arm — start the server with an image mmproj for it to accept image_url parts")
                return
            }
            if (length(images) > 1) {
                bad_request(srv, writer, "one image per request — this message carries {length(images)}")
                return
            }
            if (!g_vision_armed || g_vision_ready < 1) {
                service_unavailable(srv, writer, "the vision worker is not ready")
                return
            }
            if (length(g_vision_wires) >= VISION_QUEUE_LIMIT) {
                service_unavailable(srv, writer, "the vision queue is at capacity ({VISION_QUEUE_LIMIT}) — retry shortly")
                return
            }
            var file_bytes : array<uint8>
            var why = ""
            if (!decode_data_uri_bytes(images[0], file_bytes, why)) {
                delete file_bytes
                bad_request(srv, writer, why)
                return
            }
            // park at the encode boundary: the worker decodes + encodes off-thread, and
            // drain_vision_events resumes at handle_chat_tail with the rows
            let rid = next_id()
            set_writer_keepalive_timeout(srv, writer, LLM_WRITER_KEEPALIVE_MS)
            g_vision_wires[rid] <- VisionWire(writer = writer, js = js, slot = g_cur)
            var vjob = VisionJob(id = rid, slot = g_cur, file_bytes <- file_bytes, tag = "req{rid}")
            g_vision_jobs |> push_clone(vjob)
            delete vjob
            to_log(LOG_INFO, "[req {rid}] image parked for the vision worker ({length(g_vision_wires)} in flight)\n")
            js = null   // ownership parked with the wire; the defer above must not free it
            return
        }
    }
    var no_media : array<float>
    defer() { delete no_media }
    handle_chat_tail(srv, writer, js, g_cur, no_media, 0l)
}

// everything after the media boundary: re-derives the request fields from `js` (cheap), renders,
// and registers. Runs synchronously for a text request; an image request re-enters here from
// drain_vision_events with the worker's rows.
def private handle_chat_tail(srv : WebSocketServer; var writer : HttpResponseWriter?; js : JsonValue?;   // nolint:STYLE037,STYLE038 — flat request-field derivation: one guard and one client-facing message per field
                             slot : int; var media : array<float>; n_media : int64;
                             resume_rid : int64 = 0l) {
    g_cur = slot
    let msgs_v = js?["messages"]
    let streaming = js?["stream"] ?? false
    let params = parse_sampling(js)
    let seed = js?["seed"] ?? 0
    var req_max = js?["max_tokens"] ?? 0l
    if (req_max <= 0l) {
        req_max = js?["max_completion_tokens"] ?? g_default_max_new   // the newer OpenAI spelling
    }
    let max_new = clamp_max(req_max)
    let truncation = js?["truncation"] ?? "disabled"
    if (truncation != "disabled" && truncation != "auto") {
        bad_request(srv, writer, "'truncation' must be 'disabled' or 'auto'")
        return
    }

    // tools: each tools[] entry re-serialized verbatim into the system turn's tool block.
    // tool_choice: "none" drops them; the object form (forced function) is not honored.
    var tools : array<string>
    defer() { delete tools }
    let tools_v = js?["tools"]
    if (tools_v != null && !(tools_v.value is _null)) {
        if (!(tools_v.value is _array)) {
            bad_request(srv, writer, "'tools' must be an array of tool definitions")
            return
        }
        tools |> reserve(length(tools_v.value as _array))
        for (tv in (tools_v.value as _array)) {
            tools |> push(write_json_compact(tv))   // the template's one-tool-per-line tojson shape
        }
    }
    let tcv = js?["tool_choice"]
    if (tcv != null && !(tcv.value is _null)) {   // ?[] yields the null SENTINEL for a missing key
        if (tcv.value is _string) {
            let choice = tcv ?? ""
            if (choice == "none") {
                tools |> clear()
            } elif (choice != "auto" && choice != "required") {
                to_log(LOG_WARNING, "/v1/chat/completions: unknown tool_choice '{choice}' — treating as auto\n")
            }
        } else {
            to_log(LOG_WARNING, "/v1/chat/completions: forced tool_choice is not honored — treating as auto\n")
        }
    }

    // OpenAI is stateless — the client resends the whole transcript. Fold system turns, replay
    // prior user/assistant/tool turns through the renderer, then render the trailing turn + the
    // generation prompt.
    var system = ""
    for (mv in (msgs_v.value as _array)) {
        if ((mv?["role"] ?? "") == "system") {
            let sc = message_text(mv, "/v1/chat/completions")
            system = empty(system) ? sc : "{system}\n{sc}"
        }
    }
    // top-level enable_thinking, or the llama.cpp-server spelling chat_template_kwargs.enable_thinking;
    // other kwargs keys warn like any unsupported field instead of vanishing
    let ctk = js?["chat_template_kwargs"]
    if (ctk != null && ctk.value is _object) {
        for (k in keys(ctk.value as _object)) {
            if (k != "enable_thinking") {
                to_log(LOG_WARNING, "/v1/chat/completions: ignoring unsupported chat_template_kwargs key '{k}'\n")
            }
        }
    } elif (ctk != null && !(ctk.value is _null)) {
        to_log(LOG_WARNING, "/v1/chat/completions: 'chat_template_kwargs' must be an object — ignoring\n")
    }
    // tri-state: an ABSENT enable_thinking leaves the family's own default in force (qwen thinks,
    // gemma-4 E doesn't); a present bool — either spelling — overrides it
    let tv = js?["enable_thinking"]
    let cv = ctk?["enable_thinking"]
    let think_set = (tv != null && tv.value is _bool) || (cv != null && cv.value is _bool)
    let think_val = (tv ?? true) && (cv ?? true)
    let has_tools = !empty(tools)
    var prompt : array<int64>
    defer() { delete prompt }
    var stop_ids : array<int64>
    defer() { delete stop_ids }
    var close : array<int64>
    defer() { delete close }
    var call_open = ""
    var call_close = ""
    var think_stream = ThinkStream()
    var wire_tool_mode = ToolMode.none
    var media_at = 0l
    var start = 0
    while (true) {
        let render_error = render_chat_suffix(msgs_v, system, tools, max_new, start, think_set, think_val,
                                              n_media > 0l, prompt, stop_ids, close, call_open, call_close,
                                              think_stream, wire_tool_mode, media_at)
        if (!empty(render_error)) {
            bad_request(srv, writer, render_error)
            return
        }
        let required = long_length(prompt) + n_media + (truncation == "auto" ? max_new : 1l)
        if (required <= cur_model().config.seq_len || truncation != "auto") {
            break
        }
        let next = next_user_start(msgs_v, start)
        if (next < 0) {
            bad_request(srv, writer,
                        "the system prompt, tools, and latest turn need {required} tokens including the requested output; the model context is {cur_model().config.seq_len} - shorten the request, lower max_tokens, or raise --ctx")
            return
        }
        to_log(LOG_INFO, "/v1/chat/completions: auto truncation dropped messages [0, {next}) to reserve {max_new} output tokens\n")
        start = next
        delete prompt
        delete stop_ids
        delete close
    }
    register_request(srv, writer, prompt, stop_ids, close, media, media_at, n_media, params, max_new,
                     streaming, true, "chatcmpl",
                     has_tools, call_open, call_close, seed, think_stream, wire_tool_mode, resume_rid)
}

def private handle_completion(srv : WebSocketServer; var req : HttpRequest?; var writer : HttpResponseWriter?) {
    if (reject_deferred_while_draining(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(string(req.body), perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, COMPL_FIELDS, "/v1/completions")
    if (!resolve_stream_slot(srv, writer, js)) {
        return
    }
    let prompt = js?["prompt"] ?? ""
    if (empty(prompt)) {
        bad_request(srv, writer, "'prompt' must be a non-empty string")
        return
    }
    let streaming = js?["stream"] ?? false
    let params = parse_sampling(js)
    let seed = js?["seed"] ?? 0
    let max_new = clamp_max(js?["max_tokens"] ?? g_default_max_new)
    var toks <- encode(cur_model(), prompt, true, false)
    defer() { delete toks }
    // no stop ids and no close: raw completions run to the budget, matching bare generate()
    var none_stop : array<int64>
    var none_close : array<int64>
    var no_media : array<float>
    register_request(srv, writer, toks, none_stop, none_close, no_media, 0l, 0l,
                     params, max_new, streaming, false, "cmpl",
                     [seed = seed])
}

// Evict streams whose client vanished: is_writer_connected reads false once the connection is
// gone (the async writer ops themselves never report a dead peer). The scheduler appends the
// "evicted" finished event; flush_events below releases the writer and drops the wire.
def private reap_disconnected(srv : WebSocketServer) {
    for (id, w in keys(g_wires), values(g_wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            evict(g_slots[w.slot].sched, id, g_events)
        }
    }
    var disconnected_asr : array<int64>
    for (id, w in keys(g_asr_wires), values(g_asr_wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            release_writer(srv, w.writer)
            disconnected_asr |> push(id)
        }
    }
    for (id in disconnected_asr) {
        delete g_asr_wires[id]
        g_asr_wires |> erase(id)
    }
    delete disconnected_asr
    var disconnected_vision : array<int64>
    for (id, w in keys(g_vision_wires), values(g_vision_wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            release_writer(srv, w.writer)
            disconnected_vision |> push(id)
        }
    }
    for (id in disconnected_vision) {
        unsafe {
            delete g_vision_wires[id].js
        }
        g_vision_wires[id].js = null
        delete g_vision_wires[id]
        g_vision_wires |> erase(id)
    }
    delete disconnected_vision
}

// Tool-capable streaming buffers the model's native envelope until it can be normalized into an
// OpenAI tool_calls delta. Slow CPU inference can otherwise leave the socket silent beyond libhv's
// keep-alive window during prefill or generation. Emit a valid empty delta at least every 10s;
// scheduler steps may be long, so this is intentionally time-based rather than token-count-based.
def private flush_tool_keepalives(srv : WebSocketServer) {
    for (id in keys(g_wires)) {
        var w = unsafe(g_wires?[id])
        if (w != null && w.streaming && w.has_tools &&
            get_time_usec(w.last_keepalive_ticks) >= 10000000) {
            g_cur = w.slot
            send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "")
            w.last_keepalive_ticks = ref_time_ticks()
        }
    }
}

// Split a complete buffered reply through the wire's splitter (fresh on buffered wires — pieces
// never fed it): reasoning lands in the out-param, the content half returns. One engine with the
// chat layer's split_reasoning — the strip rule lives in think_drain.
def private split_wire_reasoning(var w : StreamWire?; full : string; var reasoning : string&) : string {
    let res = think_drain(w.think, full)
    reasoning = res.reasoning
    return res.content
}

// dasllama_tools calls -> the OpenAI wire shape, minting ids where the format carries none.
def private oai_calls_of(tcs : array<ToolCall>; id_base : string; var calls : array<OaiToolCall>) {
    calls |> reserve(length(tcs))
    var i = 0
    for (tc in tcs) {
        let id = empty(tc.id) ? "{id_base}_{i}" : tc.id
        calls |> emplace(OaiToolCall(id = id, _type = "function",
                                     _function = OaiToolCallFn(name = tc.name, arguments = tc.args)))
        i ++
    }
}

// The finish-time parse for a tool-capable wire, dispatched on its ToolMode: harmony walks the
// channel structure once (reasoning + content + calls); the marker modes split reasoning first
// and parse their format from the content half; hermes keeps the marker+sniff path.
def private parse_wire_tools(var w : StreamWire?; full, id_base : string;
                             var reasoning : string&; var calls : array<OaiToolCall>) : string {
    if (w.tool_mode == ToolMode.harmony) {
        var tr <- harmony_parse(full)
        reasoning = tr.reasoning
        oai_calls_of(tr.calls, id_base, calls)
        let content = tr.content
        delete tr
        return content
    }
    let body = split_wire_reasoning(w, full, reasoning)
    if (w.tool_mode == ToolMode.gemma4 || w.tool_mode == ToolMode.mistral || w.tool_mode == ToolMode.llama_json) {
        var tr <- (w.tool_mode == ToolMode.gemma4 ? gemma4_parse(body)
            : (w.tool_mode == ToolMode.mistral ? mistral_parse(body) : llama_json_parse(body)))
        oai_calls_of(tr.calls, id_base, calls)
        let content = tr.content
        delete tr
        return content
    }
    return parse_tool_calls_auto(body, w.call_open, w.call_close, id_base, calls)
}

// Route drained SchedEvents to their writers: streaming wires emit SSE chunks, buffered wires
// accumulate pieces and respond once at finish. "evicted" releases the writer with no bytes —
// the client is gone (reap_disconnected produced it). Finished wires unregister.
def private flush_events(srv : WebSocketServer) {   // nolint:STYLE037,STYLE038 — one arm per (event kind x wire flavor); the arms share the wire's UTF-8 hold and reply buffer
    for (e in g_events) {
        // interior pointer into g_wires — nothing inserts into the table between here and the erase
        var w = unsafe(g_wires?[e.stream])
        if (w == null) {
            continue
        }
        g_cur = w.slot   // response builders read the wire's slot (cur_name / cur_model)
        if (e.kind == SchedEventKind.piece) {
            if (!w.streaming || w.has_tools) {
                w.reply_parts |> push(e.text)   // buffered reply, and/or the finish-time tool-call parse
            }
            if (w.streaming && w.has_tools) {
                // Buffer all tool-capable output until finish so native model envelopes never leak
                // onto the OpenAI wire. flush_tool_keepalives covers slow prefill and generation.
            } elif (w.streaming) {
                // re-join any held-back partial character, then hold the new incomplete tail
                // so every delta on the wire is valid UTF-8 by itself
                let whole = "{w.utf8_tail}{e.text}"
                let hold = utf8_hold_bytes(whole)
                let cut = length(whole) - hold
                let emit_now = hold > 0 ? slice(whole, 0, cut) : whole
                w.utf8_tail = hold > 0 ? slice(whole, cut) : ""
                if (!empty(emit_now)) {
                    if (w.is_chat) {
                        // reasoning/content split rides the wire's splitter; either half may be
                        // empty (partial markers hold back across deltas)
                        var rsn = ""
                        var cnt = ""
                        think_feed(w.think, emit_now, rsn, cnt)
                        if (!empty(rsn)) {
                            send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "", rsn)
                        }
                        if (!empty(cnt)) {
                            send_chat_chunk(srv, w.writer, w.id, w.created, "", cnt, "")
                        }
                    } else {
                        send_compl_chunk(srv, w.writer, w.id, w.created, emit_now, "")
                    }
                }
            }
            continue
        }
        if (e.finish == "evicted") {
            release_writer(srv, w.writer)
        } elif (w.streaming) {
            set_writer_keepalive_timeout(srv, w.writer, DEFAULT_WRITER_KEEPALIVE_MS)
            if (!empty(w.utf8_tail)) {
                // generation ended mid-character (max-tokens cut): the held bytes can never
                // complete, and emitting them would put invalid UTF-8 on the wire — drop loudly
                to_log(LOG_WARNING, "dasllama-server: stream {w.id} ended mid-UTF-8-sequence; dropped {length(w.utf8_tail)} byte(s)\n")
            }
            var finish = e.finish
            if (w.has_tools) {
                // Normalize the complete reply: reasoning first, then calls, in OpenAI framing.
                let full = join(w.reply_parts, "")
                var rsn = ""
                var calls : array<OaiToolCall>
                let content = parse_wire_tools(w, full, "call_{e.stream}", rsn, calls)
                if (!empty(rsn)) {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "", rsn)
                }
                if (!empty(content)) {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", content, "")
                }
                if (!empty(calls)) {
                    send_chat_tool_calls_chunk(srv, w.writer, w.id, w.created, calls)
                    finish = "tool_calls"
                }
                delete calls
            } elif (w.is_chat) {
                // flush the splitter's tail (an unclosed reasoning span classifies as reasoning)
                var rsn = ""
                var cnt = ""
                think_finish(w.think, rsn, cnt)
                if (!empty(rsn)) {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "", rsn)
                }
                if (!empty(cnt)) {
                    send_chat_chunk(srv, w.writer, w.id, w.created, "", cnt, "")
                }
            }
            if (w.is_chat) {
                send_chat_chunk(srv, w.writer, w.id, w.created, "", "", finish)
            } else {
                send_compl_chunk(srv, w.writer, w.id, w.created, "", finish)
            }
            sse_event(srv, w.writer, "[DONE]", "")
            close_writer(srv, w.writer)
        } elif (w.is_chat) {
            let full = join(w.reply_parts, "")
            var rsn = ""
            var content = ""
            var finish = e.finish
            var calls : array<OaiToolCall>
            if (w.has_tools) {
                content = parse_wire_tools(w, full, "call_{e.stream}", rsn, calls)
                if (!empty(calls)) {
                    finish = "tool_calls"
                }
            } else {
                content = split_wire_reasoning(w, full, rsn)
            }
            var out = OaiChatResponse(id = w.id, _object = "chat.completion", created = w.created, model = cur_name())
            out.choices |> emplace(OaiChoice(index = 0,
                                             message <- OaiMessage(role = "assistant", content = content,
                                                                   reasoning_content = rsn,
                                                                   tool_calls <- calls),
                                             finish_reason = finish))
            out.usage = OaiUsage(prompt_tokens = e.n_prompt, completion_tokens = e.n_gen,
                                 total_tokens = e.n_prompt + e.n_gen)
            set_writer_keepalive_timeout(srv, w.writer, DEFAULT_WRITER_KEEPALIVE_MS)
            respond(srv, w.writer, int(http_status.OK), "application/json", sprint_json(out, false))
            delete out
        } else {
            var out = OaiComplResponse(id = w.id, _object = "text_completion", created = w.created, model = cur_name())
            out.choices |> push(OaiComplChoice(index = 0, text = join(w.reply_parts, ""), finish_reason = e.finish))
            out.usage = OaiUsage(prompt_tokens = e.n_prompt, completion_tokens = e.n_gen,
                                 total_tokens = e.n_prompt + e.n_gen)
            set_writer_keepalive_timeout(srv, w.writer, DEFAULT_WRITER_KEEPALIVE_MS)
            respond(srv, w.writer, int(http_status.OK), "application/json", sprint_json(out, false))
            delete out
        }
        let tps = e.gen_us > 0l ? int64(double(e.n_gen) * 1.0e6lf / double(e.gen_us)) : 0l
        to_log(LOG_INFO, "stream {w.id}: finish={e.finish} prompt={e.n_prompt} cached={e.n_cached} gen={e.n_gen} ttft={e.ttft_us / 1000l}ms tps={tps}\n")
        delete g_wires[e.stream]   // finalize the wire in place (erase is a raw remove)
        g_wires |> erase(e.stream)
    }
    g_events |> clear()
}

// /v1/stats: the scheduler's counters as JSON (buffered — cheap enough to run in-handler).
// Top-level counters describe the ACTIVE slot (single-model deployments see the old shape
// unchanged); `models` carries one entry per slot for the page's roster panel.
struct private OaiSlotStats {
    name : string
    is_default : bool
    is_active : bool          // the slot the tick loop is stepping
    holds_gpu : bool          // the slot whose VRAM state is armed (the GPU owner)
    backend : string          // requested: auto | cpu | gpu
    backend_effective : string
    flavor : string           // mapped image form: planar | vulkan | metal | "" (raw gguf) — the page locks matching config knobs
    ctx : int64
    active : int64
    queued : int64
    admitted : int64
    finished : int64
    gen_tokens : int64
    cached_tokens : int64
    prefill_tokens : int64
    prefix_pages : int64
    weights_bytes : int64
    kv_bytes : int64
    vision : bool             // a gemma4uv arm is attached: this slot's chat route takes image_url parts
    thinking_default : bool   // this family's own default when a request sends no enable_thinking
    last_used_s : float       // seconds since this slot last admitted a request (-1 = never)
    sw_count : int64          // P2 gpu-switch telemetry
    sw_avg_ms : float
}

struct private OaiStatsResponse {
    model : string
    active_model : string
    models : array<OaiSlotStats>
    ctx : int64
    uptime_s : int64
    draining : bool
    active : int64
    queued : int64
    peak_active : int64
    admitted : int64
    finished : int64
    evicted : int64
    decode_steps : int64
    gen_tokens : int64
    avg_batch : float
    cached_tokens : int64
    prefix_pages : int64
    max_streams : int64
    chunk_tokens : int64
    asr_workers : int
    asr_ready : int
    asr_active : int
    asr_pending : int
    mtp_drafted : int64
    mtp_accepted : int64
    prefill_tokens : int64
    ttft_avg_ms : float
    ttft_last_ms : float
    weights_bytes : int64
    kv_bytes : int64
    heap_bytes : int64
    string_heap_bytes : int64
    gpu_vram_bytes : int64    // resident weight bytes on the device (0 = no tier / nothing resident)
    gpu_budget_bytes : int64  // the tier's weight cap — the page's VRAM bar denominator
    hardware : string
    asr_done_jobs : int64
    asr_audio_s : float
    vision : bool             // the DEFAULT slot's vision arm — the slot a model-less page request lands on
    thinking_default : bool   // the DEFAULT slot's family thinking default (the page's checkbox seed)
    vision_pending : int      // image requests parked on the vision worker (0 = idle)
    media_rows : int64        // media (image span) rows admitted to prefill since boot
}

def private handle_stats(var resp : HttpResponse?) : http_status {
    // budget only when the device already EXISTS (tier.device is "" until first upload creates
    // it) — the budget hook device-inits on first call, and `supported` can read true while the
    // tier is merely armed, so it must not gate this
    let tier = gpu_tier_status()
    var out = OaiStatsResponse(
        model = g_slots[g_active].name, active_model = g_slots[g_active].name,
        ctx = g_slots[g_active].model.config.seq_len,
        uptime_s = g_started_at > 0l ? get_time_nsec(g_started_at) / 1_000_000_000l : 0l,
        draining = g_shutdown_requested,
        active = n_active(g_slots[g_active].sched), queued = n_queued(g_slots[g_active].sched),
        peak_active = g_slots[g_active].sched.peak_active,
        admitted = g_slots[g_active].sched.n_admitted, finished = g_slots[g_active].sched.n_finished,
        evicted = g_slots[g_active].sched.n_evicted,
        decode_steps = g_slots[g_active].sched.decode_steps, gen_tokens = g_slots[g_active].sched.gen_tokens,
        avg_batch = float(g_slots[g_active].sched.sum_batch) / float(max(g_slots[g_active].sched.decode_steps, 1l)),
        cached_tokens = g_slots[g_active].sched.cached_tokens,
        prefix_pages = prefix_held_groups(g_slots[g_active].sched.prefix),
        max_streams = g_slots[g_active].sched.max_streams, chunk_tokens = g_slots[g_active].sched.chunk_tokens,
        asr_workers = g_asr_worker_count, asr_ready = g_asr_ready,
        asr_active = g_asr_active, asr_pending = g_asr_pending,
        mtp_drafted = g_slots[g_active].sched.mtp_drafted, mtp_accepted = g_slots[g_active].sched.mtp_accepted,
        prefill_tokens = g_slots[g_active].sched.prefill_tokens,
        ttft_avg_ms = g_slots[g_active].sched.ttft_count > 0l
            ? float(g_slots[g_active].sched.ttft_sum_us / g_slots[g_active].sched.ttft_count) / 1000.0 : 0.0,
        ttft_last_ms = float(g_slots[g_active].sched.last_ttft_us) / 1000.0,
        weights_bytes = model_weights_bytes(g_slots[g_active].model),
        kv_bytes = kv_pool_bytes(g_slots[g_active].sched.pool),
        heap_bytes = int64(heap_bytes_allocated()),
        string_heap_bytes = int64(string_heap_bytes_allocated()),
        gpu_vram_bytes = tier.vram_bytes,
        gpu_budget_bytes = !empty(tier.device) ? moe_gpu_weight_budget() : 0l,
        hardware = g_hardware_line,
        asr_done_jobs = g_asr_done_jobs, asr_audio_s = g_asr_audio_s,
        // DEFAULT slot, not g_active: these two gate the page's request UI, and a page request
        // carries no model field, so it lands on the default slot - the UI must show ITS truth
        vision = g_slots[g_default_slot].vision.proj_dim > 0l,
        thinking_default = g_slots[g_default_slot].think_default,
        vision_pending = length(g_vision_wires), media_rows = g_media_rows)
    out.models |> reserve(length(g_slots))
    for (i in range(length(g_slots))) {
        var s & = unsafe(g_slots[i])
        out.models |> push(OaiSlotStats(
            name = s.name, is_default = s.is_default, is_active = i == g_active,
            holds_gpu = i == g_gpu_owner,
            backend = s.opts.backend, backend_effective = s.backend_effective,
            flavor = slot_flavor(s), vision = s.vision.proj_dim > 0l,
            thinking_default = s.think_default,
            ctx = s.model.config.seq_len,
            active = n_active(s.sched), queued = n_queued(s.sched),
            admitted = s.sched.n_admitted, finished = s.sched.n_finished,
            gen_tokens = s.sched.gen_tokens, cached_tokens = s.sched.cached_tokens,
            prefill_tokens = s.sched.prefill_tokens,
            prefix_pages = prefix_held_groups(s.sched.prefix),
            weights_bytes = model_weights_bytes(s.model),
            kv_bytes = kv_pool_bytes(s.sched.pool),
            last_used_s = s.last_used > 0l ? age_seconds(s.last_used) : -1.0,
            sw_count = s.sw_count,
            sw_avg_ms = s.sw_count > 0l ? float(s.sw_us / s.sw_count) / 1000.0 : 0.0))
    }
    let body = sprint_json(out, false)
    delete out
    return resp |> JSON(body, http_status.OK)
}

// /v1/streams: the poll surface behind the page's swimlane + stream cards — per-stream state,
// token counts, TTFT, and capped text tails. Finished streams linger ~10 s flagged "finished".
struct private OaiStreamInfo {
    id : int64
    model : string       // the slot this stream runs on
    state : string       // "queued" | "prefilling" | "decoding" | "finished"
    finish : string      // finished only: "stop" | "length" | "evicted"
    age_s : float
    n_prompt : int64
    n_cached : int64
    n_prefilled : int64
    n_gen : int64
    max_new : int64
    ttft_ms : float
    prompt_head : string
    gen_tail : string
}

// One prefix-cache donation chain: what a re-sent conversation would reuse.
struct private OaiCacheEntry {
    model : string
    tokens : int64
    pages : int64
    hits : int64
    age_s : float
    idle_s : float       // since the last hit (age when never hit)
    preview : string
}

// One ASR job row on the poll surface (drain-side tracking; RTF = audio s per wall s).
struct private OaiAsrRow {
    id : int64
    state : string
    audio_s : float
    wall_ms : float
    rtf : float
    age_s : float
}

struct private OaiStreamsResponse {
    max_streams : int64
    streams : array<OaiStreamInfo>
    cache : array<OaiCacheEntry>
    asr : array<OaiAsrRow>
}

// Age of a ref_time_ticks stamp in seconds, wrap-safe: get_time_usec returns int and wraps at
// ~35.8 min — poison for prefix-cache chains and anything else that lives hours.
def private age_seconds(t : int64) : float {
    return float(double(get_time_nsec(t)) * 1e-9lf)
}

def private handle_streams(var resp : HttpResponse?) : http_status {
    // the stream list below aggregates every slot, so the lane capacity the page sizes its
    // swimlane from must be the cross-slot total, not the active slot's own cap
    var total_cap = 0l
    for (slot in g_slots) {
        total_cap += slot.sched.max_streams
    }
    var out = OaiStreamsResponse(max_streams = total_cap)
    for (slot in g_slots) {
        var sch & = unsafe(slot.sched)
        let mname = slot.name
        out.streams |> reserve(length(out.streams) + length(sch.streams) + length(sch.queue) + length(sch.recent_done))
        for (st in sch.streams) {
            out.streams |> push(OaiStreamInfo(
                id = st.id, model = mname,
                state = st.state == StreamState.prefilling ? "prefilling" : "decoding",
                age_s = age_seconds(st.t_admit),
                n_prompt = long_length(st.prompt) + st.n_media, n_cached = st.n_cached,
                n_prefilled = st.n_prefilled, n_gen = st.n_gen, max_new = st.max_new,
                ttft_ms = float(st.ttft_us) / 1000.0,
                prompt_head = st.prompt_head, gen_tail = utf8_trim_partial_end(string(st.gen_tail_bytes))))
        }
        for (q in sch.queue) {
            out.streams |> push(OaiStreamInfo(
                id = q.id, model = mname, state = "queued",
                age_s = q.t_submit != 0l ? age_seconds(q.t_submit) : 0.0,
                n_prompt = long_length(q.prompt) + q.n_media, max_new = q.max_new))
        }
        for (d in sch.recent_done) {
            out.streams |> push(OaiStreamInfo(
                id = d.id, model = mname, state = "finished", finish = d.finish,
                n_prompt = d.n_prompt, n_cached = d.n_cached, n_prefilled = d.n_prompt, n_gen = d.n_gen,
                ttft_ms = float(d.ttft_us) / 1000.0,
                prompt_head = d.prompt_head, gen_tail = utf8_trim_partial_end(d.gen_tail)))
        }
        var chains <- prefix_chain_list(sch.prefix)
        out.cache |> reserve(length(out.cache) + length(chains))
        for (c in chains) {
            let age = age_seconds(c.born_at)
            out.cache |> push(OaiCacheEntry(
                model = mname,
                tokens = c.n_tokens, pages = c.pages_live, hits = c.hits, age_s = age,
                idle_s = c.last_hit_at != 0l ? age_seconds(c.last_hit_at) : age,
                preview = c.preview))
        }
        delete chains
    }
    out.asr |> reserve(length(g_asr_recent))
    for (j in g_asr_recent) {
        out.asr |> push(OaiAsrRow(
            id = j.id, state = j.state, audio_s = j.audio_s, wall_ms = j.wall_ms, rtf = j.rtf,
            age_s = age_seconds(j.started_at)))
    }
    let body = sprint_json(out, false)
    delete out
    return resp |> JSON(body, http_status.OK)
}

// key -> expected JSON kind for POST /config bodies: "s"tring | "i"nteger | "b"ool. The one
// schema the save endpoint validates against — mirrors ServerArgs (main.das CONFIG_KEYS).
// "models" is the [[models]] array-of-tables; entries validate against MODEL_KEY_KINDS.
let private CONFIG_KEY_KINDS <- {
    "model" => "s", "port" => "i", "quant" => "s", "kv_dtype" => "s", "metal" => "s",
    "gpu" => "s", "gpu_layers" => "i", "gpu_stream" => "i", "gpu_dn" => "b", "gpu_attn" => "b",
    "gpu_dense" => "b", "gpu_vram_mb" => "i", "asr" => "s", "mmproj" => "s", "asr_workers" => "i",
    "ctx" => "i", "max_tokens" => "i", "streams" => "i", "threads" => "i", "team_dispatch" => "s",
    "affinity" => "i", "chunk" => "i", "page_rows" => "i", "prefix" => "i", "flat" => "b",
    "mtp" => "b", "lcpp_bin" => "s", "image_mmproj" => "s", "models" => "a",
    "exchange_url" => "s", "exchange_accept" => "s", "exchange_submit" => "s"
}

let private MODEL_KEY_KINDS <- {
    "name" => "s", "path" => "s", "backend" => "s", "ctx" => "i", "quant" => "s",
    "kv_dtype" => "s", "streams" => "i", "chunk" => "i", "page_rows" => "i", "prefix" => "i",
    "mtp" => "b", "default" => "b", "image_mmproj" => "s"
}

// Validate + sanitize a POST /config "models" array: objects only, per-key kinds, a non-empty
// path per entry, backend in {auto, cpu, gpu}, unique route names, at most one default = true.
// Returns the sanitized array (JV) or null with verr set.
def private validate_models_array(v : JsonValue?; var verr : string&) : JsonValue? {
    if (v == null || !(v.value is _array)) {
        verr = "'models' must be an array of model entries"
        return null
    }
    var marr : array<JsonValue?>
    var names : table<string>
    var ndefault = 0
    for (e in v.value as _array) {
        if (e == null || !(e.value is _object)) {
            verr = "'models' entries must be objects"
            break
        }
        var ent : table<string; JsonValue?>
        for (mk, mv in keys(e.value as _object), values(e.value as _object)) {
            let kind = MODEL_KEY_KINDS?[mk] ?? ""
            if (empty(kind)) {
                verr = "unknown models key '{mk}'"
                break
            }
            if (kind == "s" && mv != null && (mv.value is _string)) {
                ent |> insert(mk, JV(mv.value as _string))
            } elif (kind == "i" && mv != null && (mv.value is _longint)) {
                ent |> insert(mk, JV(mv.value as _longint))
            } elif (kind == "i" && mv != null && (mv.value is _number) && (mv.value as _number) == floor(mv.value as _number)) {
                ent |> insert(mk, JV(int64(mv.value as _number)))
            } elif (kind == "b" && mv != null && (mv.value is _bool)) {
                ent |> insert(mk, JV(mv.value as _bool))
            } else {
                verr = "models key '{mk}' must be a {kind_name(kind)}"
                break
            }
        }
        if (!empty(verr)) {
            var junk = JV(ent)
            unsafe {
                delete junk
            }
            break
        }
        let path = e?["path"] ?? ""
        if (empty(path)) {
            verr = "every models entry needs a non-empty 'path'"
        } else {
            let backend = e?["backend"] ?? "auto"
            if (backend != "auto" && backend != "cpu" && backend != "gpu") {
                verr = "models backend '{backend}' is not auto | cpu | gpu"
            }
        }
        let rname = e?["name"] ?? ""
        let route = empty(rname) ? base_name(e?["path"] ?? "") : rname
        if (empty(verr) && key_exists(names, route)) {
            verr = "duplicate models name '{route}'"
        }
        if (empty(verr) && (e?["default"] ?? false)) {
            ndefault ++
            if (ndefault > 1) {
                verr = "more than one models entry marked default = true"
            }
        }
        if (!empty(verr)) {
            var junk = JV(ent)
            unsafe {
                delete junk
            }
            break
        }
        names |> insert(route)
        marr |> push(JV(ent))
    }
    if (!empty(verr)) {
        var junk = JV(marr)
        unsafe {
            delete junk
        }
        return null
    }
    return JV(marr)
}

// POST /vad: the page's waveform-overlay preview — silero speech spans over an uploaded clip.
// Buffered + in-handler: silero is a tiny RNN, preview clips are capped, no worker needed.
var private g_vad_loaded = false
var private g_vad = VadModel()
let private VAD_PREVIEW_CAP_S = 120

def private handle_vad(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!g_vad_loaded) {
        let path = path_join(get_das_root(), "modules/dasLLAMA/models/silero_vad.bin")
        if (!stat(path).is_valid) {
            return resp |> JSON(
                error_body("silero_vad.bin not found ({path}) — the VAD preview needs a source tree", "server_error"),
                http_status.NOT_IMPLEMENTED)
        }
        g_vad <- load_vad_model(path)   // nolint:PERF030 — one-shot lazy load; g_vad_loaded proves the target is still the empty default
        g_vad_loaded = true
    }
    var fname = ""
    each_form_field(req) $(name, _content, filename) {
        if (name == "file" && !empty(filename)) {
            fname = filename
        }
    }
    if (empty(fname)) {
        return resp |> JSON(
            error_body("multipart form must include a 'file' audio upload", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    let temppath = path_join(tmpdir(), "dasllama_vad_{next_id()}_{base_name(fname)}")
    save_form_file(req, "file", temppath)
    var samples <- load_audio_16k_mono(temppath)
    remove(temppath)
    defer() {
        delete samples
    }
    if (empty(samples)) {
        return resp |> JSON(
            error_body("could not decode the uploaded audio", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    if (length(samples) > 16000 * VAD_PREVIEW_CAP_S) {
        return resp |> JSON(
            error_body("clip too long for the VAD preview (cap {VAD_PREVIEW_CAP_S} s)", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    var segs <- find_speech_timestamps(g_vad, samples, default_vad_opts())
    var parts : array<string>
    parts |> reserve(length(segs))
    for (sg in segs) {
        parts |> push("\{\"start\":{float(sg.start) / 16000.0},\"end\":{float(sg.end) / 16000.0}}")
    }
    let body = "\{\"duration\":{float(length(samples)) / 16000.0},\"segments\":[{join(parts, ",")}]}"
    delete segs
    delete parts
    return resp |> JSON(body, http_status.OK)
}

def private json_str(s : string) : string {
    return sprint_json(s, false)   // a bare string serializes as its quoted+escaped JSON literal
}

def private kind_name(kind : string) : string {
    if (kind == "s") {
        return "string"
    }
    return kind == "i" ? "whole number" : "bool"
}

// GET /config: the launcher-registered surface (effective values + per-key provenance) plus
// live status — model files beside the served one, the active rail, and the GPU tier badge.
def private handle_config_get(var resp : HttpResponse?) : http_status {
    var entries : array<JsonValue?>
    let mdir = dir_name(g_config_model_path)
    if (!empty(mdir)) {
        dir(mdir) $(fname) {
            let lower = to_lower(fname)
            if (ends_with(lower, ".gguf") || ends_with(lower, ".dlim")) {
                let st = stat(path_join(mdir, fname))
                entries |> push(JV((name = fname, size = int64(st.size),
                    kind = ends_with(lower, ".dlim") ? "dlim" : "gguf")))
            }
        }
    }
    let tier = gpu_tier_status()
    var extras <- {
        "model_files" => JV(entries),
        "active_model" => JV(base_name(g_config_model_path)),
        "dlim_rail" => JV(g_slots[g_default_slot].model.image_map != null),
        "metal" => JV("{get_metal_mode()}"),
        "gpu" => JV((
            backend = tier.backend, supported = tier.supported, reason = tier.reason,
            resident_layers = tier.resident_layers, streamed_layers = tier.streamed_layers,
            dn_layers = tier.dn_layers, attn_layers = tier.attn_layers,
            dense_planes = tier.dense_planes, cls_resident = tier.cls_resident,
            vram_bytes = tier.vram_bytes))
    }
    var extras_doc = JV(extras)
    let extras_json = write_json_compact(extras_doc)
    unsafe {
        delete extras_doc
    }
    let surface = empty(g_config_surface_json) ? "null" : g_config_surface_json
    return resp |> JSON("\{\"surface\":{surface},\"status\":{extras_json}}", http_status.OK)
}

// POST /config: validate a {key: value} JSON body against the schema, rebuild it as an
// explicitly-typed tree + `authoritative = true`, and write it as TOML to the save path.
// The running server keeps its config — POST /restart applies the saved one.
def private handle_config_save(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    var jerr = ""
    var body = read_json(string(req.body), jerr)
    defer() {
        unsafe {
            delete body
        }
    }
    if (body == null || !(body.value is _object)) {
        return resp |> JSON(
            error_body("config body must be a JSON object: {jerr}", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    var out_tab : table<string; JsonValue?>
    var verr = ""
    for (k, v in keys(body.value as _object), values(body.value as _object)) {
        let kind = CONFIG_KEY_KINDS?[k] ?? ""
        if (empty(kind)) {
            continue if (k == "authoritative")   // server-owned marker; re-added below
            verr = "unknown config key '{k}'"
            break
        }
        if (v == null) {
            verr = "config key '{k}' is null"
            break
        }
        if (kind == "a") {
            var sanitized = validate_models_array(v, verr)
            break if (!empty(verr))
            out_tab |> insert(k, sanitized)
            continue
        }
        if (kind == "s" && (v.value is _string)) {
            out_tab |> insert(k, JV(v.value as _string))
        } elif (kind == "i" && (v.value is _longint)) {
            out_tab |> insert(k, JV(v.value as _longint))
        } elif (kind == "i" && (v.value is _number) && (v.value as _number) == floor(v.value as _number)) {
            out_tab |> insert(k, JV(int64(v.value as _number)))
        } elif (kind == "b" && (v.value is _bool)) {
            out_tab |> insert(k, JV(v.value as _bool))
        } else {
            verr = "config key '{k}' must be a {kind_name(kind)}"
            break
        }
    }
    if (!empty(verr)) {
        var junk = JV(out_tab)
        unsafe {
            delete junk
        }
        return resp |> JSON(error_body(verr, "invalid_request_error"), http_status.BAD_REQUEST)
    }
    out_tab |> insert("authoritative", JV(true))
    var doc = JV(out_tab)
    var werr = ""
    let toml = write_toml(doc, werr)
    unsafe {
        delete doc
    }
    if (!empty(werr)) {
        return resp |> JSON(error_body("config not TOML-representable: {werr}", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (!fwrite(g_config_save_path, toml)) {
        return resp |> JSON(error_body("could not write '{g_config_save_path}'", "server_error"), http_status.INTERNAL_SERVER_ERROR)
    }
    to_log(LOG_INFO, "dasllama-server: authoritative config saved to {g_config_save_path} ({length(toml)} bytes)\n")
    return resp |> JSON("\{\"saved\":{json_str(g_config_save_path)},\"restart_required\":true}", http_status.OK)
}

// ===== the exchange surface (GET /exchange + the apply/submit/retune levers) =====

def private exchange_policy_now : ExchangePolicy {
    // the save path covers every start shape: --config, auto-discovered, and config-less
    // (where the file may not exist yet and the baked defaults stand)
    return exchange_policy_from_config(g_config_save_path)
}

// The exchange control routes are an operator surface reached through the loopback control
// page. The matching route does a blocking network call on the tick thread; apply/retune are
// destructive; submit publishes. Gate them to the loopback peer so a LAN client (the inference
// API deliberately binds every interface) cannot stall generation or trigger those actions.
def private is_loopback_req(req : HttpRequest?) : bool {
    let ip = client_ip(req)
    // an unknown/empty peer counts as remote (fail closed, like the ladder's is_loopback_peer):
    // this service binds every interface, so a LAN caller must not slip through on an unresolved peer
    let loopback = ip == "127.0.0.1" || ip == "::1" || ip == "::ffff:127.0.0.1"
    return loopback && same_origin_or_headerless(req)
}

// CSRF guard for the operator routes: allow_cors() is global and the peer is 127.0.0.1 for ANY
// local browser, so a page the operator merely visits could POST to the destructive routes. Accept
// only a same-origin request (the control page is served from here) or a header-less CLI client.
def private same_origin_or_headerless(req : HttpRequest?) : bool {
    let origin = get_header(req, "Origin")
    if (empty(origin)) {
        return true   // curl/CLI and same-origin navigations send no Origin; only a cross-site POST does
    }
    let sep = find(origin, "://")
    let authority = sep >= 0 ? slice(origin, sep + 3) : origin
    return authority == get_header(req, "Host")
}

// the forbidden reply shared by every operator/control route (the public inference API stays open)
def private deny_operator(var resp : HttpResponse?) : http_status {
    return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
}

def private handle_exchange_get(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!is_loopback_req(req)) {
        // the status carries the local sidecar path (username in it) — operator-only, like the rest
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    return resp |> JSON(exchange_status_json(exchange_policy_now()))
}

def private handle_exchange_matches(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    // a live exchange lookup — seconds on the tick thread; the page requests it explicitly
    return resp |> JSON(exchange_matches_json(exchange_policy_now()))
}

def private handle_exchange_apply(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    var jerr = ""
    var js = read_json(string(req.body), jerr)
    defer() {
        unsafe {
            delete js
        }
    }
    let sha : string = js?["sha"] ?? ""
    if (empty(sha)) {
        return resp |> JSON(error_body("body must carry the sidecar sha", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    // the listing box the page picked: passed through so the downloaded sidecar is verified to
    // match what the exchange listed — the same lying-exchange gate the boot resolver applies
    let box : string = js?["box"] ?? ""
    let err = exchange_fetch_and_apply(exchange_policy_now().url, sha, tune_manifest_path(), box)
    if (!empty(err)) {
        return resp |> JSON(error_body(err, "invalid_request_error"), http_status.BAD_REQUEST)
    }
    to_log(LOG_INFO, "dasllama-server: exchange sidecar {sha} applied via the control page; draining for restart\n")
    g_shutdown_requested = true
    g_restart_requested = true
    return resp |> JSON("\{\"ok\":true,\"restarting\":true}", http_status.OK)
}

def private handle_exchange_submit(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    let err = exchange_submit_current(exchange_policy_now())
    if (!empty(err)) {
        return resp |> JSON(error_body(err, "invalid_request_error"), http_status.BAD_REQUEST)
    }
    to_log(LOG_INFO, "dasllama-server: this box's tune submitted to the exchange\n")
    return resp |> JSON("\{\"ok\":true}", http_status.OK)
}

def private handle_exchange_retune(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    let err = exchange_request_retune()
    if (!empty(err)) {
        // the marker write failed and the sidecar is untouched — do NOT drain/restart
        return resp |> JSON(error_body(err, "server_error"), http_status.INTERNAL_SERVER_ERROR)
    }
    to_log(LOG_INFO, "dasllama-server: re-tune armed (sidecar removed, exchange skipped once); draining for restart\n")
    g_shutdown_requested = true
    g_restart_requested = true
    return resp |> JSON("\{\"ok\":true,\"restarting\":true,\"tune\":true}", http_status.OK)
}

def private emit_asr_event(events : Channel?; event : AsrEvent) {
    events |> push_clone(event)
}

def private catch_worker_panic(var failure : string&; blk : block<() : void>) : bool {
    var caught = false
    try {
        invoke(blk)
    } recover {
        caught = true
        failure = this_context().last_exception |> rtrim
    }
    return caught
}

def private process_asr_job(
                            model : AsrModel;
                            var session : AsrSession;
                            var session_language : string&;
                            var has_session : bool&;
                            job : AsrJob;
                            events : Channel?
                            ) {
    emit_asr_event(events, AsrEvent(kind = AsrEventKind.started, id = job.id))
    defer() {
        remove(job.temp_path)
    }
    var samples <- load_audio_16k_mono(job.temp_path)
    defer() {
        delete samples
    }
    if (empty(samples)) {
        emit_asr_event(events, AsrEvent(
            kind = AsrEventKind.completed, id = job.id, status = int(http_status.BAD_REQUEST),
            content_type = "application/json",
            body = error_body("could not decode the uploaded audio", "invalid_request_error")))
        return
    }
    let language = empty(job.language) ? "en" : job.language
    if (!has_session || session_language != language) {
        if (has_session) {
            delete session
        }
        session <- create_session(model, language)
        session_language = language
        has_session = true
    }
    session.translate = job.translate
    if (job.response_format == "verbose_json") {
        var vt = OaiVerboseTranscription(
            task = job.translate ? "translate" : "transcribe",
            language = language, duration = float(length(samples)) / 16000.0)
        var full : array<string>
        var sid = 0
        transcribe(model, session, samples) $(seg) {
            vt.segments |> push(OaiVerboseSegment(
                id = sid, start = float(seg.t0) / 100.0,
                end = float(seg.t1) / 100.0, text = seg.text))
            full |> push(seg.text)
            sid++
        }
        vt.text = join(full, "")
        vt.language = session.detected_lang
        emit_asr_event(events, AsrEvent(
            kind = AsrEventKind.completed, id = job.id, status = int(http_status.OK),
            content_type = "application/json", body = sprint_json(vt, false),
            audio_s = float(length(samples)) / 16000.0))
        delete full
        delete vt
        return
    }
    let text = transcribe(model, session, samples)
    let content_type = job.response_format == "text" ? "text/plain; charset=utf-8" : "application/json"
    let body = job.response_format == "text" ? text : sprint_json(OaiTranscription(text = text), false)
    emit_asr_event(events, AsrEvent(
        kind = AsrEventKind.completed, id = job.id, status = int(http_status.OK),
        content_type = content_type, body = body,
        audio_s = float(length(samples)) / 16000.0))
}

def private asr_worker_main(
                            worker_index : int;
                            model_path, mmproj_path : string;
                            team_dispatch : bool;
                            var jobs : Channel?&;
                            var events : Channel?&
                            ) {
    this_context().name := "dasllama_asr_{worker_index}"
    set_jobque_thread_team_mode(team_dispatch)
    var model <- empty(mmproj_path) ? load_asr_model(model_path) : load_asr_model(model_path, mmproj_path)
    emit_asr_event(events, AsrEvent(kind = AsrEventKind.ready))
    var session = AsrSession()
    var session_language = ""
    var has_session = false
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(250) $(job : AsrJob#) {
            if (job.stop) {
                stopping = true
            } else {
                var job_copy : AsrJob
                job_copy := job
                var failure = ""
                let failed = catch_worker_panic(failure) {
                    process_asr_job(model, session, session_language, has_session, job_copy, events)
                }
                if (failed) {
                    if (has_session) {
                        delete session
                        has_session = false
                    }
                    session_language = ""
                    to_log(LOG_ERROR, "[req {job_copy.id}] asr: failed: {failure}\n")
                    emit_asr_event(events, AsrEvent(
                        kind = AsrEventKind.completed, id = job_copy.id,
                        status = int(http_status.INTERNAL_SERVER_ERROR),
                        content_type = "application/json",
                        body = error_body(failure, "server_error")))
                }
                delete job_copy
            }
        }
    }
    if (has_session) {
        delete session
    }
    delete model
    emit_asr_event(events, AsrEvent(kind = AsrEventKind.stopped))
    jobs |> release
    events |> release
}

// /v1/audio/transcriptions (translate=false) and /v1/audio/translations (translate=true, to English).
// Multipart upload: save the 'file' part to disk (binary-safe), decode to 16 kHz mono f32, transcribe.
def private queue_transcription(
                                srv : WebSocketServer;
                                var req : HttpRequest?;
                                var writer : HttpResponseWriter?;
                                translate : bool
                                ) {
    if (reject_deferred_while_draining(srv, writer)) {
        return
    }
    if (!g_has_asr || g_asr_ready < g_asr_worker_count) {
        service_unavailable(srv, writer, "ASR workers are not ready")
        return
    }
    if (g_asr_pending >= ASR_QUEUE_LIMIT) {
        service_unavailable(srv, writer, "the ASR queue is at capacity ({ASR_QUEUE_LIMIT}) — retry shortly")
        return
    }
    var fname = ""
    each_form_field(req) $(name, _content, filename) {
        if (name == "file" && !empty(filename)) {
            fname = filename
        }
    }
    if (empty(fname)) {
        bad_request(srv, writer, "multipart form must include a 'file' audio upload")
        return
    }
    set_writer_keepalive_timeout(srv, writer, ASR_WRITER_KEEPALIVE_MS)
    let rid = next_id()
    let temppath = path_join(tmpdir(), "dasllama_upload_{rid}_{base_name(fname)}")
    save_form_file(req, "file", temppath)
    let fmt = get_form_data(req, "response_format")
    var lang = get_form_data(req, "language")
    if (empty(lang)) {
        lang = "en"
    }
    g_asr_wires[rid] <- AsrWire(writer = writer)
    g_asr_pending++
    g_asr_jobs |> push_clone(AsrJob(
        id = rid, temp_path = temppath, language = lang, response_format = fmt,
        translate = translate))
    to_log(LOG_INFO, "[req {rid}] asr: queued (pending={g_asr_pending}, active={g_asr_active})\n")
}

def private start_asr_worker_threads() {
    g_asr_ready = 0
    if (!g_has_asr) {
        return
    }
    to_log(LOG_INFO, "dasllama-server: starting {g_asr_worker_count} asynchronous ASR worker(s)\n")
    for (index in range(g_asr_worker_count)) {
        let worker_index = index
        let model_path = g_asr_path
        let mmproj_path = g_asr_mmproj
        let team_dispatch = g_asr_team_dispatch
        var jobs_capture = g_asr_jobs
        var events_capture = g_asr_events
        new_thread() <| @capture(= worker_index, = model_path, = mmproj_path, = team_dispatch,
                                = jobs_capture, = events_capture) {
            asr_worker_main(
                worker_index, model_path, mmproj_path, team_dispatch, jobs_capture, events_capture)
        }
    }
    while (g_asr_ready < g_asr_worker_count) {
        g_asr_events |> pop_with_timeout_clone(1000) $(event : AsrEvent#) {
            if (event.kind == AsrEventKind.ready) {
                g_asr_ready++
                to_log(LOG_INFO, "dasllama-server: ASR worker ready ({g_asr_ready}/{g_asr_worker_count})\n")
            }
        }
    }
}

def private drain_asr_events(srv : WebSocketServer) {
    var draining = true
    while (draining) {
        draining = g_asr_events |> try_pop_clone() $(event : AsrEvent#) {
            if (event.kind == AsrEventKind.started) {
                g_asr_active++
                g_asr_recent |> push(AsrJobInfo(id = event.id, state = "running", started_at = ref_time_ticks()))
            } elif (event.kind == AsrEventKind.completed) {
                g_asr_active = max(g_asr_active - 1, 0)
                g_asr_pending = max(g_asr_pending - 1, 0)
                for (i in range(length(g_asr_recent))) {
                    continue if (g_asr_recent[i].id != event.id)
                    let wall_ms = float(double(get_time_nsec(g_asr_recent[i].started_at)) * 1e-6lf)   // wrap-safe: clips can run > 35 min
                    g_asr_recent[i].state = event.status == int(http_status.OK) ? "done" : "failed"
                    g_asr_recent[i].audio_s = event.audio_s
                    g_asr_recent[i].wall_ms = wall_ms
                    g_asr_recent[i].rtf = wall_ms > 0.0 ? event.audio_s * 1000.0 / wall_ms : 0.0
                    g_asr_recent[i].done_at = ref_time_ticks()
                    break
                }
                if (event.status == int(http_status.OK)) {
                    g_asr_done_jobs ++
                    g_asr_audio_s += event.audio_s
                }
                var wire = unsafe(g_asr_wires?[event.id])
                if (wire != null) {
                    if (is_writer_connected(srv, wire.writer)) {
                        set_writer_keepalive_timeout(srv, wire.writer, DEFAULT_WRITER_KEEPALIVE_MS)
                        respond(srv, wire.writer, event.status, event.content_type, event.body)
                    } else {
                        release_writer(srv, wire.writer)
                    }
                    delete g_asr_wires[event.id]
                    g_asr_wires |> erase(event.id)
                }
                to_log(LOG_INFO, "[req {event.id}] asr: completed (pending={g_asr_pending}, active={g_asr_active})\n")
            }
        }
    }
    // finished jobs linger ~30 s on the poll surface, then drop (bounded backstop at 16).
    // Index sweep, not a head-gated loop: a long-RUNNING job at the head must not block the
    // expiry of finished jobs behind it. Wrap-safe nsec compare (clips can run > 35 min).
    var i = 0
    while (i < length(g_asr_recent)) {
        if (g_asr_recent[i].done_at != 0l
                && (get_time_nsec(g_asr_recent[i].done_at) > 30_000_000_000l || length(g_asr_recent) > 16)) {
            delete g_asr_recent[i]
            g_asr_recent |> erase(i)
        } else {
            i ++
        }
    }
}

def private stop_asr_worker_threads() {
    if (!g_has_asr) {
        return
    }
    for (_index in range(g_asr_worker_count)) {
        g_asr_jobs |> push_clone(AsrJob(stop = true))
    }
    var stopped = 0
    while (stopped < g_asr_worker_count) {
        g_asr_events |> pop_with_timeout_clone(1000) $(event : AsrEvent#) {
            if (event.kind == AsrEventKind.stopped) {
                stopped++
            }
        }
    }
    g_asr_ready = 0
}

// ===== async vision encode (the ASR worker pattern: own context, own embedder) =====
// The decode + letterbox + embedder GEMMs run on ONE worker thread, so an image request never
// stalls the tick loop's decode of every live stream.

struct private VisionJob {
    id : int64
    slot : int
    file_bytes : array<uint8>   // the base64-decoded image FILE; the worker decodes + encodes
    tag : string                // DASLLAMA_VISION_DUMP artifact name
    stop : bool
}

enum private VisionEventKind {
    completed   // one job's result (ok + why say how it went); zero value — the channel pops zeroed clones
    ready       // the worker loaded its embedders
    stopped     // the worker exited
}

struct private VisionEvent {
    kind : VisionEventKind
    id : int64
    ok : bool
    why : string
    n_media : int64
    rows : array<float>
    width : int
    height : int
    enc_ms : int64
}

struct private VisionWire {
    @do_not_delete writer : HttpResponseWriter?
    @do_not_delete js : JsonValue?   // the parked parsed request — drain_vision_events deletes it explicitly
    slot : int
}

let VISION_QUEUE_LIMIT = 8   // parked image requests are heavy (decoded bytes ride the channel)

var private g_vision_jobs : Channel?
var private g_vision_events : Channel?
var private g_vision_wires : table<int64; VisionWire>
var private g_media_rows = 0l   // media (image span) rows admitted to prefill since boot
var private g_vision_ready = 0
var private g_vision_armed = false

def private vision_worker_main(slot_path_lines : string; var jobs : Channel?&; var events : Channel?&) {
    this_context().name := "dasllama_vision"
    set_jobque_thread_team_mode(false)
    var embs : table<int; Gemma4uvEmbedder>
    var states : table<int; Gemma4uvState>
    for (line in split(slot_path_lines, "\n")) {
        continue if (empty(line))
        let sep = find(line, "\t")
        let slot = to_int(slice(line, 0, sep))
        var load_failure = ""
        let load_failed = catch_worker_panic(load_failure) {
            embs[slot] <- load_gemma4uv_embedder(slice(line, sep + 1))   // own mapping; pages shared with the slot's
            states[slot] <- Gemma4uvState()
        }
        if (load_failed) {   // the slot's requests take the no-embedder 400; the boot must not hang on this thread
            to_log(LOG_ERROR, "dasllama-server: vision worker could not load slot {slot}'s embedder: {load_failure}\n")
            embs |> erase(slot)
            states |> erase(slot)
        }
    }
    var hello = VisionEvent(kind = VisionEventKind.ready)
    events |> push_clone(hello)
    delete hello
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(250) $(job : VisionJob#) {
            if (job.stop) {
                stopping = true
                return
            }
            var job_copy : VisionJob
            job_copy := job
            var out = VisionEvent(id = job_copy.id)
            if (!key_exists(embs, job_copy.slot)) {
                out.why = "this slot has no vision embedder in the worker (armed after init_server, or its load failed at boot)"
            } else {
                // the one rail that chews untrusted bytes: an escaping panic in a new_thread
                // worker is a PROCESS abort, so every job runs under the catch (the ASR shape)
                var failure = ""
                let failed = catch_worker_panic(failure) {
                    var inscope img <- decode_image_bytes(job_copy.file_bytes, out.why)
                    if (img.width != 0) {
                        let t0 = ref_time_ticks()
                        out.n_media = encode_image(embs[job_copy.slot], states[job_copy.slot], img, job_copy.tag, out.rows)
                        out.ok = true
                        out.width = img.width
                        out.height = img.height
                        out.enc_ms = int64(get_time_usec(t0)) / 1000l
                    }
                }
                if (failed) {
                    to_log(LOG_ERROR, "[req {job_copy.id}] vision encode failed: {failure}\n")
                    out.ok = false
                    out.why = "the vision encode failed - see the server log"
                }
            }
            events |> push_clone(out)
            delete out
            delete job_copy
        }
    }
    unsafe {
        delete embs
        delete states
    }
    var farewell = VisionEvent(kind = VisionEventKind.stopped)
    events |> push_clone(farewell)
    delete farewell
    jobs |> release       // the captured channel refs, or the thread teardown panics
    events |> release
}

def private start_vision_worker() {
    g_vision_ready = 0
    g_vision_armed = false
    var arms : array<string>
    defer() { delete arms }
    for (i in range(length(g_slots))) {
        if (g_slots[i].vision.proj_dim != 0l) {
            arms |> push("{i}\t{g_slots[i].vision_path}")
        }
    }
    if (empty(arms)) {
        return
    }
    g_vision_armed = true
    let arms_spec = join(arms, "\n")   // "<slot index>\t<mmproj path>" per line
    var jobs_capture = g_vision_jobs
    var events_capture = g_vision_events
    to_log(LOG_INFO, "dasllama-server: starting the asynchronous vision encode worker\n")
    new_thread() <| @capture(= arms_spec, = jobs_capture, = events_capture) {
        vision_worker_main(arms_spec, jobs_capture, events_capture)
    }
    while (g_vision_ready < 1) {
        g_vision_events |> pop_with_timeout_clone(1000) $(event : VisionEvent#) {
            if (event.kind == VisionEventKind.ready) {
                g_vision_ready++
                to_log(LOG_INFO, "dasllama-server: vision encode worker ready\n")
            }
        }
    }
}

def private stop_vision_worker() {
    if (!g_vision_armed) {
        return
    }
    var halt = VisionJob(stop = true)
    g_vision_jobs |> push_clone(halt)
    delete halt
    var stopped = false
    while (!stopped) {
        g_vision_events |> pop_with_timeout_clone(1000) $(event : VisionEvent#) {
            if (event.kind == VisionEventKind.stopped) {
                stopped = true
            } elif (event.kind == VisionEventKind.completed) {
                to_log(LOG_WARNING, "[req {event.id}] vision result discarded during teardown\n")
            }
        }
    }
    g_vision_ready = 0
    g_vision_armed = false
}

def private drain_vision_events(srv : WebSocketServer) {
    var draining = true
    while (draining) {
        draining = g_vision_events |> try_pop_clone() $(event : VisionEvent#) {
            return if (event.kind != VisionEventKind.completed)
            var ev : VisionEvent
            ev := event
            defer() { delete ev }
            // interior pointer into g_vision_wires — nothing inserts into the table between here
            // and the erase (the only insert is handle_chat's park, and handlers cannot run inside
            // this drain)
            var wire = unsafe(g_vision_wires?[ev.id])
            if (wire == null) {
                return   // the client vanished while encoding; reap released the writer
            }
            if (!is_writer_connected(srv, wire.writer)) {
                release_writer(srv, wire.writer)
            } elif (!ev.ok) {
                bad_request(srv, wire.writer, ev.why)
            } elif (reject_deferred_while_draining(srv, wire.writer)) {
                pass   // a clean terminal 503: the drain must not admit a new generation
            } else {
                to_log(LOG_INFO, "[req {ev.id}] image {ev.width}x{ev.height} -> {ev.n_media} soft tokens in {ev.enc_ms}ms off-thread\n")
                handle_chat_tail(srv, wire.writer, wire.js, wire.slot, ev.rows, ev.n_media, ev.id)
            }
            unsafe {
                delete wire.js
            }
            wire.js = null
            delete g_vision_wires[ev.id]
            g_vision_wires |> erase(ev.id)
        }
    }
}

// ===== Server =====

class OpenAiServer : HvWebServer {
    def override onInit {   // nolint:STYLE038 — the route table: one registration per endpoint
        allow_cors()
        GET("/v1/models") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_models(resp)
        }
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            handle_chat(server, req, writer)
        }
        STREAM("/v1/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            handle_completion(server, req, writer)
        }
        STREAM("/v1/audio/transcriptions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            queue_transcription(server, req, writer, false)
        }
        STREAM("/v1/audio/translations") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            queue_transcription(server, req, writer, true)
        }
        POST("/v1/embeddings") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (g_shutdown_requested) {
                return resp |> JSON(
                    error_body("the server is draining for shutdown — retry shortly", "server_error"),
                    http_status.SERVICE_UNAVAILABLE)
            }
            return handle_embeddings(req, resp)
        }
        POST("/v1/models/activate") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_activate(req, resp)
        }
        GET("/v1/stats") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_stats(resp)
        }
        GET("/v1/streams") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_streams(resp)
        }
        GET("/") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            // the control page ships beside the server sources; libhv sets text/html from the extension
            return resp |> SERVE_FILE(path_join(get_this_module_dir(), "control.html"))
        }
        GET("/config") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_config_get(resp)
        }
        POST("/vad") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_vad(req, resp)
        }
        POST("/config") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            return handle_config_save(req, resp)
        }
        GET("/exchange") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_exchange_get(req, resp)
        }
        GET("/exchange/matches") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_exchange_matches(req, resp)
        }
        POST("/exchange/apply") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_exchange_apply(req, resp)
        }
        POST("/exchange/submit") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_exchange_submit(req, resp)
        }
        POST("/exchange/retune") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_exchange_retune(req, resp)
        }
        POST("/bench") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            return handle_bench_start(resp)
        }
        GET("/bench") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_bench_get(resp)
        }
        POST("/bake") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            return handle_bake_start(req, resp)
        }
        GET("/bake") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_bake_get(resp)
        }
        GET("/v1/images") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_images(resp)
        }
        POST("/restart") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            if (!g_shutdown_requested) {
                to_log(
                    LOG_INFO,
                    "dasllama-server: restart requested; draining llm_active={total_active()}, llm_queued={total_queued()}, asr_pending={g_asr_pending}, vision_parked={length(g_vision_wires)}\n")
            }
            g_shutdown_requested = true
            g_restart_requested = true
            return resp |> JSON("\{\"ok\":true,\"draining\":true,\"restarting\":true}", http_status.OK)
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            if (!g_shutdown_requested) {
                to_log(
                    LOG_INFO,
                    "dasllama-server: graceful shutdown requested; draining llm_active={total_active()}, llm_queued={total_queued()}, asr_pending={g_asr_pending}, vision_parked={length(g_vision_wires)}\n")
            }
            g_shutdown_requested = true
            return resp |> JSON("\{\"ok\":true,\"draining\":true}", http_status.OK)
        }
        POST("/gc") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!is_loopback_req(req)) return deny_operator(resp)
            let coalesced = g_gc_requested
            g_gc_requested = true
            to_log(LOG_INFO, "dasllama-server: GC requested via POST /gc (coalesced={coalesced})\n")
            return resp |> JSON(
                "\{\"ok\":true,\"scheduled\":true,\"coalesced\":{coalesced}}",
                http_status.ACCEPTED)
        }
        ANY("*") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_unknown(req, resp)
        }
    }
}

// Lifecycle-owned state. Keeping it global is required by both daslang-live (which calls
// init/update/shutdown independently) and standalone JIT heap collection: collectable state that
// survives an update must be visible to the collector as a global root, never a main-loop local.
var private g_server : OpenAiServer?
var private g_server_started = false
var private g_jobque_owned = false
var private g_prev_mtp_spec = false
var private g_mtp_spec_changed = false

//! Initialize the server lifecycle. Creates a persistent JobQue explicitly so the pool spans
//! externally-driven update calls; ``shutdown_server`` destroys it after every worker and owned
//! queue object has stopped. Call after ``set_model``.
//! ``max_streams`` concurrent generation streams, ``max_queue`` waiting requests (503 beyond),
//! ``chunk_tokens`` the prefill quantum (decode stalls at most this many tokens per tick).
//! ``page_rows`` > 0 (the default) serves PAGED: KV memory tracks actual context and a prefix
//! cache reuses completed streams' pages, so repeated prompts (system prompts, multi-turn chats)
//! skip their prefill; ``prefix_groups`` caps the cache (0 = auto: one full context per stream
//! slot, < 0 = unbounded). ``page_rows`` = 0 restores flat preallocated sessions (each the full
//! ``--ctx`` slab). ``kv_dtype`` picks the KV-cache codec for every stream (pool-wide when paged;
//! see ``KVDtype`` — ``tq4`` needs a power-of-two head_size).
def init_server(port : int; max_streams : int64 = 4l; max_queue : int64 = 32l; chunk_tokens : int64 = 64l;
                page_rows : int64 = 64l; prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
                team_dispatch : bool = true; asr_team_dispatch : bool = false; mtp : bool = false) : bool {
    if (g_server != null || g_jobque_owned) {
        to_log(LOG_ERROR, "dasllama-server: init_server called while already initialized\n")
        return false
    }
    g_shutdown_requested = false   // reset so a restart in the same context after /shutdown doesn't drain at once
    g_restart_requested = false    // NOT reset in shutdown_server — the lifecycle host reads it after
    g_gc_requested = false
    create_job_que()
    g_jobque_owned = true
    setup_dasllama_jobque()
    set_jobque_team_mode(team_dispatch)
    g_asr_team_dispatch = asr_team_dispatch
    let dispatch_mode = !team_dispatch ? "inline" : (asr_team_dispatch ? "team" : "hybrid")
    to_log(LOG_INFO, "dasllama-server: jobque compute dispatch = {dispatch_mode}\n")
    g_asr_jobs = unsafe(channel_create())
    g_asr_events = unsafe(channel_create())
    g_vision_jobs = unsafe(channel_create())
    g_vision_events = unsafe(channel_create())
    g_bench_events = unsafe(channel_create())
    g_bench_state = "idle"
    g_bench_result_json = ""
    g_bake_events = unsafe(channel_create())
    g_bake_state = "idle"
    g_bake_result_json = ""
    start_asr_worker_threads()
    start_vision_worker()
    if (empty(g_slots)) {
        to_log(LOG_ERROR, "dasllama-server: init_server with no model slots (call set_model/add_model first)\n")
        return false
    }
    // one scheduler per slot: the init args are the defaults, SlotOpts sentinels override per slot
    for (s in g_slots) {
        let s_streams = s.opts.streams >= 0l ? s.opts.streams : max_streams
        let s_chunk = s.opts.chunk >= 0l ? s.opts.chunk : chunk_tokens
        let s_pages = s.opts.page_rows != -2l ? s.opts.page_rows : page_rows
        var pfx = s.opts.prefix != -2l ? s.opts.prefix : prefix_groups
        if (s_pages > 0l && pfx == 0l) {   // auto: retain up to one full context per stream slot
            pfx = s_streams * ((s.model.config.seq_len + s_pages - 1l) / s_pages)
        }
        if (pfx < 0l) {
            pfx = 0l   // explicit unbounded
        }
        let s_kv = s.opts.kv_dtype_set ? s.opts.kv_dtype : kv_dtype
        var spec = s.opts.mtp >= 0 ? s.opts.mtp != 0 : mtp
        if (spec && s.model.config.n_layer_nextn <= 0l) {
            to_log(LOG_WARNING, "dasllama-server: mtp requested for {s.name} but the model has no NextN head — self-spec stays off\n")
            spec = false
        }
        s.sched <- create_scheduler(s.model, s_streams, max_queue, s_chunk, s_pages, pfx, s_kv, spec)
    }
    g_active = g_default_slot
    g_cur = g_default_slot
    g_prev_mtp_spec = get_mtp_spec()
    g_mtp_spec_changed = true
    // engine gate: prefills maintain the draft head's KV (chunked included); follows the stepped
    // slot on switch (update_server); restored at exit
    set_mtp_spec(g_slots[g_active].sched.mtp)
    // the single shared KV mirror + chunked multi-stream prefill don't mix: a second stream's
    // resident prefill would strand the first's device-only KV. The batch-decode override (which
    // resyncs host<->mirror per step) stays on; per-slot mirrors re-enable device prefill here.
    set_resident_prefill_allowed(false)
    g_server = new OpenAiServer()
    g_server->init(port)
    let rc = g_server->start()   // 0 on success; non-zero = bind/listen failed (e.g. port in use)
    if (rc != 0) {
        to_log(LOG_ERROR, "dasllama-server: could not start on port {port} (start rc {rc}; is the port already in use?)\n")
        return false
    }
    g_server_started = true
    g_started_at = ref_time_ticks()
    compose_hardware_line()
    var names <- [for (s in g_slots); s.is_default && length(g_slots) > 1 ? "{s.name}*" : s.name]
    let kv_mode = page_rows > 0l ? "{kv_dtype} paged P={page_rows}" : "{kv_dtype} flat"
    to_log(LOG_INFO, "dasllama-server listening on http://127.0.0.1:{port}/v1  (models: {join(names, ", ")}; streams: {max_streams}, prefill chunk: {chunk_tokens}, kv: {kv_mode})\n")
    delete names
    return true
}

// Round-robin pick of the next slot to step, starting AFTER the last stepped one so concurrent
// work on several slots interleaves fairly. -1 = everything idle.
def private pick_step_slot : int {
    let n = length(g_slots)
    for (k in range(n)) {
        let i = (g_active + 1 + k) % n
        if (n_active(g_slots[i].sched) > 0l || n_queued(g_slots[i].sched) > 0l) {
            return i
        }
    }
    return -1
}

//! Drive one non-blocking server iteration. False means the graceful drain completed (or the
//! server never started); an external lifecycle host should then exit and call shutdown.
def update_server() : bool {
    if (!g_server_started || g_server == null) {
        return false
    }
    g_server->tick()   // drains queued handlers: requests parse + render + submit here
    drain_asr_events(g_server.server)
    drain_vision_events(g_server.server)
    drain_bench_events()
    drain_bake_events()
    reap_disconnected(g_server.server)
    // serialized multi-model stepping: ONE scheduler step per tick, slots with work interleave
    // round-robin. Every step runs under ITS model's routing marks; the GPU tier follows a
    // gpu-wanting slot once the current owner drains.
    let si = pick_step_slot()
    var busy = false
    if (si >= 0) {
        if (si != g_active) {
            g_active = si
            set_mtp_spec(g_slots[si].sched.mtp)   // the engine prefill-warm gate follows the stepped slot
        }
        install_slot_marks(si)
        maybe_bind_gpu(si)
        let t_step = ref_time_ticks()
        busy = scheduler_step(g_slots[si].model, g_slots[si].sched, g_events)
        let step_ms = int64(get_time_usec(t_step)) / 1000l
        if (step_ms > 3000l) {
            // the "why does it pause" catcher: one decode step / prefill chunk should be
            // well under a second — anything slower means contention or a stall
            to_log(LOG_WARNING, "dasllama-server: slow scheduler step: {step_ms}ms ({g_slots[si].name}: active={n_active(g_slots[si].sched)}, queued={n_queued(g_slots[si].sched)})\n")
        }
    }
    flush_events(g_server.server)
    flush_tool_keepalives(g_server.server)
    if (g_shutdown_requested && total_active() == 0l && total_queued() == 0l &&
        g_asr_pending == 0 && empty(g_vision_wires)) {
        to_log(LOG_INFO, "dasllama-server: graceful shutdown drain complete\n")
        return false
    }
    if (!busy) {
        sleep(2u)
    }
    return true
}

//! Release every lifecycle-owned object in dependency order. Safe after a partially failed init.
def shutdown_server() {   // nolint:STYLE038 — one ordered teardown: workers stop before their channels, VRAM drops before the slots, the jobque dies last
    if (g_server_started && g_server != null) {
        g_server->stop()
    }
    g_server_started = false
    if (g_asr_jobs != null && g_asr_events != null) {
        stop_asr_worker_threads()
    }
    if (g_vision_jobs != null && g_vision_events != null) {
        stop_vision_worker()
    }
    if (g_server != null) {
        g_server->cleanup()
        unsafe {
            delete g_server
        }
        g_server = null
    }
    // clean tier epoch: drop the owner's VRAM state and leave the engine globals bare, so a
    // same-context restart (tests) starts from an unarmed device with no stale marks
    if (g_gpu_owner >= 0 && g_gpu_owner < length(g_slots)) {
        install_slot_marks(g_gpu_owner)
        moe_gpu_drop_model()
    } elif (g_marks_slot >= 0 && g_marks_slot < length(g_slots)) {
        moe_gpu_model_marks_save(g_slots[g_marks_slot].gpu_marks)
    }
    g_gpu_owner = -1
    g_marks_slot = -1
    unsafe {
        delete g_slots   // per slot: live sessions + workspace + the model go together (writers already died with the server)
    }
    g_default_slot = 0
    g_active = 0
    g_cur = 0
    delete g_events
    delete g_wires
    delete g_asr_wires
    for (w in values(g_vision_wires)) {   // parked image requests die with the server
        unsafe {
            delete w.js
        }
        w.js = null
    }
    delete g_vision_wires
    var asr_jobs = g_asr_jobs
    var asr_events = g_asr_events
    var vision_jobs = g_vision_jobs
    var vision_events = g_vision_events
    var bench_events = g_bench_events
    var bake_events = g_bake_events
    g_asr_jobs = null
    g_asr_events = null
    g_vision_jobs = null
    g_vision_events = null
    g_bench_events = null
    g_bake_events = null
    unsafe {
        if (asr_jobs != null) {
            asr_jobs |> channel_remove
        }
        if (asr_events != null) {
            asr_events |> channel_remove
        }
        if (vision_jobs != null) {
            vision_jobs |> channel_remove
        }
        if (vision_events != null) {
            vision_events |> channel_remove
        }
        // a still-running worker holds a channel ref, and channel_remove throws on outstanding
        // refs — leave the channel to the worker (its exit release is safe on the live object;
        // the worker can't be cancelled, its child process runs minutes) instead of aborting stop
        if (bench_events != null) {
            if (g_bench_state == "running") {
                to_log(LOG_WARNING, "dasllama-server: shutdown with a bench still running — channel left to the worker\n")
            } else {
                bench_events |> channel_remove
            }
        }
        if (bake_events != null) {
            if (g_bake_state == "running") {
                to_log(LOG_WARNING, "dasllama-server: shutdown with a bake still running — channel left to the worker\n")
            } else {
                bake_events |> channel_remove
            }
        }
    }
    delete g_bench_log
    if (g_mtp_spec_changed) {
        set_mtp_spec(g_prev_mtp_spec)   // a /shutdown + restart in the same context starts from a clean gate
        g_mtp_spec_changed = false
    }
    g_has_asr = false
    g_asr_path = ""
    g_asr_mmproj = ""
    g_asr_worker_count = 0
    g_asr_ready = 0
    g_asr_active = 0
    g_asr_pending = 0
    if (g_jobque_owned) {
        destroy_job_que()
        g_jobque_owned = false
    }
    g_shutdown_requested = false
}

//! Compatibility wrapper for tests and simple callers. Lifecycle hosts should call
//! ``init_server`` / ``update_server`` / ``shutdown_server`` directly.
def run_server(port : int; max_streams : int64 = 4l; max_queue : int64 = 32l; chunk_tokens : int64 = 64l;
               page_rows : int64 = 64l; prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
               team_dispatch : bool = true; asr_team_dispatch : bool = false; mtp : bool = false) {
    if (init_server(port, max_streams, max_queue, chunk_tokens, page_rows, prefix_groups, kv_dtype,
            team_dispatch, asr_team_dispatch, mtp)) {
        while (update_server()) {
        }
    }
    shutdown_server()
}
