options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true   // ggml-parity FP laxity (non-bit-exact); inference tool, not the test suite

module openai_server

require dasllama/dasllama       // the public facade — the main dasLLAMA surface this tool touches
require dasllama/dasllama_scheduler   // continuous-batching scheduler (layer 3 over the facade)
require dasllama/dasllama_bench       // the llama-bench rows the in-process /bench runs on the served model
require user_state                    // write_config - the /config save
require dashv/dashv_boost public
require daslib/json_boost
require daslib/toml             // write_toml: POST /config persists the authoritative config
require ?llvm dasllama/dasllama_exchange   // nolint:STYLE030,LINT019 — OPTIONAL: the /exchange surface + submit rails, which ride the tune framework
require [tune_framework]   // nolint:STYLE030,LINT019 — OPTIONAL: tune_manifest_path, the /exchange apply target, reached only inside the exchange arms
require daslib/defer            // defer() { delete js } — free the parsed request JSON on every exit path
require model_catalog           // the curated list + the /catalog download rail
require serving_knobs public    // the serving spellings shared with dasllama-cli (quant / kv / metal / tts lane + file set / the --gpu pick)
require daslib/jobque_boost
require daslib/rtti             // this_context().last_exception inside ASR worker recovery
require daslib/strings_boost    // join()
require daslib/linq   // sum: the ASR worker counts on the stats surface
require daslib/utf8_utils       // utf8_length: the TTS input cap counts characters, not bytes
require daslib/fio              // sleep(), path_join, base_name, remove, get_env_variable
require daslib/module_path      // get_this_module_dir: control.html ships beside the server sources
require strings
require math
require daslib/base64                // base64_decode: the data: URI payload

// An OpenAI-compatible HTTP server over the dasLLAMA facade. The SERVING paths reach only public
// facade verbs — no engine internals — which is the acceptance test for the API: if a full OpenAI
// surface builds without reaching into the engine, the facade is complete.
// Chat and completion requests batch continuously: handlers parse + render + register and return;
// run_server's loop drives dasllama_scheduler (one eval_batch decode step over every live stream + one
// bounded prefill chunk per tick) and routes SchedEvents back to each request's writer. Streaming
// and buffered responses both ride the deferred writer ops, so N streams generate concurrently.
// ASR uploads are queued to long-lived new_thread workers, each with its own model/session/context;
// the small buffered holdouts (/v1/models, /v1/embeddings, /shutdown, /v1/stats) still complete
// in-handler.

// ===== The HTTP front and the engine =====
// Two contexts. The FRONT is a thread of its own: it owns the HTTP server and every response writer, parses nothing
// and runs no inference - a request crosses to the engine as a FrontReq, and the engine's replies come back as
// FrontOut ops addressed by the front's wire id. The ENGINE is the context that called init_server: it owns the
// slots, the schedulers and the models, and runs every handler serially inside update_server. A handler that takes
// seconds (a model load, a long prefill chunk) holds the engine, never the socket side.

enum private Route {
    unknown
    models
    chat
    completion
    transcribe
    translate
    speech
    phonemes
    embeddings
    activate
    load
    unload
    stats
    streams
    config_get
    vad
    config_save
    exchange_get
    exchange_matches
    exchange_apply
    exchange_submit
    exchange_retune
    exchange_consent
    bench_start
    bench_get
    bake_start
    catalog
    catalog_download
    bake_get
    images
    restart
    shutdown
    gc
}

enum private FrontKind {
    request     //! a request for `route`
    gone        //! the client of wire `wid` disconnected
    ready       //! the front is listening: `status` its start rc, `port` the bound port
    stopped     //! the front's server is down, its channels released
    drain       //! engine -> speech thread: the server is draining, admit no new work
    halt        //! engine -> speech thread: stop the workers and exit
    query       //! engine -> speech thread: publish the state now, answering `wid`
}

//! what crosses from the front to the engine; the zero value is a request for the unknown route
struct FrontReq {
    kind : FrontKind
    wid : int64             //! the front's wire id, which the reply names
    route : Route
    method : string
    path : string
    body : string
    loopback : bool         //! the peer, its Host and its Origin all say loopback (the operator surface's gate)
    file_path : string      //! a multipart upload the front saved to a temp file
    form_format : string    //! the upload's `response_format` field
    form_language : string  //! the upload's `language` field
    form_model : string     //! the upload's `model` field
    status : int
    port : int
}

enum private FrontOp {
    respond         //! a whole reply: `status`, `ctype`, `text` - ends the wire
    sse             //! one SSE event: `text` the data, `aux` the event name
    close           //! end a stream - ends the wire
    release         //! drop the wire without a reply
    keepalive       //! the wire's keepalive timeout, `ms`
    header          //! a response header, `aux` its name and `text` its value
    serve_file      //! a file reply, `text` its path - ends the wire
    stop            //! shut the front down
}

//! what crosses from the engine to the front
struct FrontOut {
    op : FrontOp
    wid : int64
    status : int
    ctype : string
    text : string
    aux : string
    ms : int
}

//! the engine's name for a response writer the front holds
struct WriterRef {
    id : int64
}

//! the engine's handle on the front: every writer op below rides the out channel
struct FrontLink {
    @do_not_delete out : Channel?
}

//! a buffered handler's answer, as `resp |> JSON(...)` leaves it
struct Reply {
    status : int = 200
    ctype : string = "application/json"
    body : string
}

var private g_link = FrontLink()
var private g_front_in : Channel?           //! front -> engine: FrontReq
var private g_gone : table<int64>           //! wires whose client disconnected, until the engine drops them

//! A channel a thread blocks on. A fresh channel counts nothing pending, and a timed pop on it returns at once -
//! the waiter spins a core; one pending count makes the pop wait for a message or its timeout.
def private worker_channel() : Channel? {
    var ch = unsafe(channel_create())
    ch |> append(1)
    return ch
}

def private front_op(srv : FrontLink; o : FrontOut) {
    if (srv.out != null) {
        srv.out |> push_clone(o)
    }
}

def private sse_event(srv : FrontLink; w : WriterRef; data, event : string implicit) {
    front_op(srv, FrontOut(op = FrontOp.sse, wid = w.id, text = data, aux = event))
}

def private respond(srv : FrontLink; w : WriterRef; status : int; ctype, body : string implicit) {
    g_gone |> erase(w.id)
    front_op(srv, FrontOut(op = FrontOp.respond, wid = w.id, status = status, ctype = ctype, text = body))
}

def private close_writer(srv : FrontLink; w : WriterRef) {
    g_gone |> erase(w.id)
    front_op(srv, FrontOut(op = FrontOp.close, wid = w.id))
}

def private release_writer(srv : FrontLink; w : WriterRef) {
    g_gone |> erase(w.id)
    front_op(srv, FrontOut(op = FrontOp.release, wid = w.id))
}

def private set_writer_keepalive_timeout(srv : FrontLink; w : WriterRef; ms : int) {
    front_op(srv, FrontOut(op = FrontOp.keepalive, wid = w.id, ms = ms))
}

def private set_header(srv : FrontLink; w : WriterRef; key, value : string implicit) {
    front_op(srv, FrontOut(op = FrontOp.header, wid = w.id, aux = key, text = value))
}

def private SERVE_FILE(srv : FrontLink; w : WriterRef; path : string implicit) {
    g_gone |> erase(w.id)
    front_op(srv, FrontOut(op = FrontOp.serve_file, wid = w.id, text = path))
}

[unused_argument(srv)]
def private is_writer_connected(srv : FrontLink; w : WriterRef) : bool => !key_exists(g_gone, w.id)

def private JSON(var resp : Reply; body : string; status : http_status = http_status.OK) : http_status {
    resp.status = int(status)
    resp.ctype = "application/json"
    resp.body = body
    return status
}

// ===== Served state (slots set at startup; handlers run serially on the engine's context) =====

//! Per-slot serving options (main.das resolves them from a [[models]] config entry). The -1/-2/""
//! sentinels mean "inherit the init_server default". `backend` is a REQUEST (auto | cpu | gpu);
//! the arm outcome lands in ModelSlot.backend_effective.
struct SlotOpts {
    backend : string = "auto"
    streams : int64 = -1l
    chunk : int64 = -1l
    chunk_idle : int64 = -2l        //! -2 = inherit (-1 is meaningful: the backend's default; 0: never wider than chunk)
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
    gpu_regions : int64             //! K/V regions the whole-model driver armed for this slot, one per device-home stream; 0 = not that driver
    gpu_ask_streams : int64         //! the K/V regions and the context this slot's load asked the whole-model driver for:
    gpu_ask_ctx : int64             //! the engine keeps one process-wide ask, so a rearm restores the slot's own first
    gpu_ask_kv : KVDtype = KVDtype.f16   //! the K/V codec this slot's load asked the device mirror in
    resident_decline : string               //! why the whole-model GPU driver does not serve this slot (its decline, with the remedy where one exists); "" when it does, or no GPU was asked for
    is_default : bool
    last_used : int64
    sw_count : int64                // gpu-switch telemetry (count / total usec)
    sw_us : int64
    gpu_want : GpuTierWant = GpuTierWant()   // the tier request this slot re-arms with on switch-in
    gpu_capable : bool              // want is non-zero and backend != cpu (set_slot_gpu_want)
    gpu_marks : GpuModelMarks <- gpu_model_marks_init()   // this model's tier routing marks while NOT installed
    vision : VisionEmbedder = VisionEmbedder()   // set_model_vision; kind == none = text only (encode runs on the media worker)
    vision_path : string                             // the worker loads its own embedder from this
    audio_armed : bool                           // set_model_audio; the media worker owns the encoder itself
    audio_bare : bool                            // the audio encoder's rows splice with no span marker (ultravox)
    audio_path : string                              // the mmproj the worker loads the audio encoder from
    think_default : bool   // family default when the request sends no enable_thinking (THINKING.md's table)
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
        return empty(g_slots) ? -1 : g_default_slot
    }
    for (i in range(length(g_slots))) {
        if (g_slots[i].name == model_field
                || (i == g_default_slot && "{g_slots[i].name}-asr" == model_field)) {
            return i
        }
    }
    return -1
}

//! Path identity for roster matching and the one-serving-slot-per-GGUF dedup: canonical
//! absolute form (relative and `..` spellings resolve), separator- and (on windows)
//! case-insensitive — never exact strings.
def same_model_path(a, b : string) : bool {
    let fa = get_full_file_name(a)
    let fb = get_full_file_name(b)
    let ga = to_generic_path(empty(fa) ? a : fa)
    let gb = to_generic_path(empty(fb) ? b : fb)
    return get_platform_name() == "windows" ? to_lower(ga) == to_lower(gb) : ga == gb
}

//! One served ASR model: its workers load and own it, each in a context of its own, and take its jobs off `jobs`.
struct AsrSlot {
    id : string                 //! the request's `model` names it; the file's stem unless the config says otherwise
    path : string
    mmproj : string
    workers : int
    ready : int                 //! workers that loaded and answered
    decoder : string            //! where the workers decode, off a ready event: `metal` for a decoder loaded as the Metal flavor, else `cpu`
}

//! One served TTS model: one worker loads and owns it. The answer-side fields are the worker's own, off its ready event.
struct TtsSlot {
    id : string                 //! the request's `model` names it; the file's stem unless the config says otherwise
    path : string
    lane : string = TTS_LANE_DEFAULT   //! the weight lane the worker is ASKED for; `lane_served` is the pin it took
    voices_dir : string         //! clips a cloning model adds to its voices at boot, each under its file's stem
    armed : bool                //! the route serves this model: its file set was there and its worker loaded it
    ready : int
    error : string              //! why the worker could not load: the slot stays down and the tts stats block carries this
    voices : array<string>
    sample_rate : int
    lane_served : string
    cloning : bool
    speed : bool
    lang : string
}

var g_asr : array<AsrSlot>          //! the ASR models, the first the default a request with no `model` gets
var g_tts : array<TtsSlot>          //! the TTS models, the first ARMED one the default
var private g_asr_alias = ""                //! the default ASR model's other name, `<default slot>-asr`; empty with no LLM slot
var private g_asr_jobs : array<Channel?>   //! a job channel a running ASR model, by its row; the thread that started the workers holds them
var private g_tts_jobs : array<Channel?>   //! the same a TTS model, null for one that is not armed
var g_asr_team_dispatch = false
var g_speech_metal = MetalMode.off   //! the engine's Metal mode, handed to the ASR workers whose decoder is a Model session
var g_tts_team_dispatch = false
let TTS_VOICE_CLIP_EXTENSIONS : array<string> <- [".wav", ".flac", ".mp3", ".ogg"]   // what the decode rail reads
var g_shutdown_requested = false
var g_gc_requested = false
var g_req_counter = 0l
var g_events : array<SchedEvent>

// One registered request's HTTP side: where this stream id's SchedEvents go. The writer is owned by
// dasHV's active_writers (respond/close_writer/release_writer drop it) — never deleted from here.
struct private StreamWire {
    @do_not_delete writer : WriterRef
    streaming : bool
    include_usage : bool
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
    slot.gpu_ask_streams = g_gpu_ask_streams
    slot.gpu_ask_ctx = g_gpu_ask_ctx
    slot.gpu_ask_kv = g_gpu_ask_kv
    slot_clamp_ctx_to_region(slot)
    // the load's arm outcome (routing marks + resident-driver state) belongs to THIS slot:
    // capture it, so two models' offset-keyed marks never mix. Whichever slot is stepped gets
    // its set re-installed (install_slot_marks); an armed capture makes this slot the GPU owner.
    slot.gpu_regions = gpu_device_sessions()
    slot.backend_effective = gpu_slot_capture(slot.gpu_marks)
    slot.resident_decline = slot.backend_effective == "gpu:resident" ? "" : gpu_resident_decline()
    let was_gpu = slot.backend_effective != "cpu"
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
    if (ibench_running()) {
        ibench_stop("idle", "the model set was replaced under it")
    }
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
                && (want.auto_tier || want.resident || want.moe_layers != 0l || want.moe_stream != 0l || want.dn
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

// add_model's capture takes whatever is INSTALLED — a runtime load must recreate the boot
// precondition (nothing installed) or the new slot steals the stepped slot's routing marks.
def private uninstall_slot_marks() {
    if (g_marks_slot >= 0 && g_marks_slot < length(g_slots)) {
        moe_gpu_model_marks_save(g_slots[g_marks_slot].gpu_marks)
    }
    g_marks_slot = -1
}

// Move the GPU tier's VRAM state to slot `si`: hydrate + drop the current owner's whole state,
// re-arm `si` from its mmap'd image (the same walk its load ran). Serialized with stepping —
// only called between scheduler steps.
var private g_gpu_ask_streams = 0l
var private g_gpu_ask_ctx = 0l
var private g_gpu_ask_kv = KVDtype.f16

//! before the load or rearm: a K/V region per stream of `ctx` positions, asked whole when `ctx` > 0 (0 = the model's own, shortened
//! to the card's room), held in the streams' codec `kv`
def gpu_residency_ask(streams, ctx : int64; kv : KVDtype = KVDtype.f16) {
    g_gpu_ask_streams = streams
    g_gpu_ask_ctx = ctx
    g_gpu_ask_kv = kv
    set_gpu_resident_regions(streams)
    set_gpu_ctx_max(ctx)
    set_gpu_ctx_strict(gpu_ctx_asked_whole(ctx))
    set_gpu_kv_dtype(kv)
}

//! a stream's context is its region's where the driver armed any: a request never runs past the rows the card holds for it
def region_clamped_ctx(seq_len, regions, region_ctx : int64) : int64 => regions > 0l ? min(seq_len, region_ctx) : seq_len

def private slot_clamp_ctx_to_region(var slot : ModelSlot) {
    slot.model.config.seq_len = region_clamped_ctx(slot.model.config.seq_len, gpu_device_sessions(), gpu_device_session_ctx())
}

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
        slot_default_chunk(g_slots[prev])
        prev_note = "from {g_slots[prev].name}, "
    }
    install_slot_marks(si)
    gpu_residency_ask(g_slots[si].gpu_ask_streams, g_slots[si].gpu_ask_ctx, g_slots[si].gpu_ask_kv)
    // want, arm, resident upload (bake-slice path for a mapped vulkan-flavor image) — the
    // order-sensitive half of the switch lives in the engine
    g_slots[si].backend_effective = gpu_slot_rearm(g_slots[si].gpu_want, g_slots[si].model)
    slot_default_chunk(g_slots[si])
    slot_clamp_ctx_to_region(g_slots[si])
    g_slots[si].resident_decline = g_slots[si].backend_effective == "gpu:resident" ? "" : gpu_resident_decline()
    g_slots[si].gpu_regions = gpu_device_sessions()
    g_gpu_owner = si
    let us = int64(get_time_usec(t0))
    g_slots[si].sw_count ++
    g_slots[si].sw_us += us
    to_log(LOG_INFO, "dasllama-server: GPU tier -> {g_slots[si].name} ({prev_note}{us / 1000l}ms, {g_slots[si].backend_effective})\n")
}

//! Read with the slot's marks installed, so the facade answers for THIS model.
def slot_wants_device_kv(s : ModelSlot) : bool {
    return (s.backend_effective == "gpu:resident" && s.vision.kind == VisionKind.none && !s.audio_armed
        && !s.sched.mtp && s.gpu_regions >= s.sched.max_streams)
}

def private sync_slot_device_kv(var s : ModelSlot; holds_gpu : bool) {
    let want = holds_gpu && slot_wants_device_kv(s)   // a slot the tier is not armed for keeps host-cached sessions until it holds the card again
    let was = s.sched.device_kv
    if (set_device_kv(s.sched, want) != was) {
        let kind = want ? "device-home sessions: every stream's K/V stays on the GPU, {gpu_device_session_ctx()} positions each" : "host-cached sessions"
        to_log(LOG_INFO, "dasllama-server: {s.name} admits {kind}\n")
    }
}

def slot_served(s : ModelSlot; device_kv, holds_gpu : bool) : string {
    if (slot_flavor(s) == "metal") {
        return "GPU (Metal): the whole model and every stream's cache, in the memory the CPU and the GPU share"
    }
    if (s.backend_effective != "cpu" && !holds_gpu) {
        return "GPU, off the card now: another model holds it, and this one takes it back once that one is idle"
    }
    if (s.backend_effective == "gpu:resident") {
        return (device_kv ? "GPU: the whole model and every stream's cache on the card"
            : "GPU: the whole model on the card, the streams' caches on the host")
    }
    return (s.backend_effective == "gpu:rails" ? "GPU in part: some layers' work on the card, the rest on the CPU, the streams' caches on the host"
        : "CPU")
}

def slot_served_note(s : ModelSlot; device_kv, holds_gpu : bool) : string {
    if (s.backend_effective != "cpu" && !holds_gpu) {
        return ""
    }
    if (s.backend_effective != "gpu:resident") {
        if (s.opts.backend == "cpu" || empty(s.resident_decline)) {
            return ""
        }
        let lead = s.backend_effective == "cpu" ? "the GPU serves nothing of this model" : "the whole model does not run on the GPU, so only part of the work does"
        return "{lead}. The engine's reason, in its own words: {s.resident_decline}"
    }
    if (device_kv) {
        return ""
    }
    if (s.vision.kind != VisionKind.none || s.audio_armed) {
        return "this model also reads images or audio, and those parts of a prompt run on the host: every stream's cache stays there, and each step runs on the CPU"
    }
    if (s.sched.mtp) {
        return "self-speculation (the model drafting its own next tokens) has no GPU path yet: every stream's cache stays on the host, and each step runs on the CPU"
    }
    return "the card has room for {s.gpu_regions} stream caches and this model serves {s.sched.max_streams} streams: every stream's cache stays on the host, and each step runs on the CPU"
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

var private g_boot_gpu_want = GpuTierWant()
var private g_boot_ctx = 0l

//! Register the box policy a runtime load re-applies: the boot GPU tier want (zero on
//! non-vulkan boxes) and the ``--ctx`` clamp — the same policy every boot load got, NOT
//! whatever slot happens to be default at load time. Call once before ``run_server`` —
//! without it POST /v1/models/load loads CPU-only with the model's trained context.
def set_runtime_load_policy(want : GpuTierWant; ctx : int64) {
    g_boot_gpu_want = want
    g_boot_ctx = ctx
}

// The arm proper, shared by the boot API (panics on misuse) and the runtime load route (which
// turns the same failures into 400s). Empty return = armed.
def private arm_slot_vision(mi : int; path : string) : string {
    if (g_slots[mi].vision.kind != VisionKind.none) {
        return "slot '{g_slots[mi].name}' already has a vision arm ({base_name(g_slots[mi].vision_path)}) - the arm is once per slot"
    }
    var inscope emb <- load_vision_embedder(path)
    if (vision_proj_dim(emb) != g_slots[mi].model.config.dim) {
        return "{base_name(path)} projects to {vision_proj_dim(emb)} but {g_slots[mi].name} is {g_slots[mi].model.config.dim}-wide - mismatched pair"
    }
    if (vision_deepstack(emb) != g_slots[mi].model.config.n_deepstack) {
        return "{base_name(path)} carries {vision_deepstack(emb)} deepstack slices but {g_slots[mi].name} expects {g_slots[mi].model.config.n_deepstack} - mismatched pair"
    }
    var probe <- create_chat_renderer(g_slots[mi].model, "", 1l)
    let no_markers = empty(probe.tmpl.image_pre.parts) || !image_vocab_ok(g_slots[mi].model, probe.tmpl)
    delete probe
    if (no_markers) {
        return "{g_slots[mi].name}'s chat template or vocab carries no image span markers - it has no vision arm"
    }
    g_slots[mi].vision <- emb
    g_slots[mi].vision_path = path
    to_log(LOG_INFO, "dasllama-server: {g_slots[mi].name} vision arm: {base_name(path)} ({vision_family(g_slots[mi].vision)}, {vision_proj_dim(g_slots[mi].vision)}-wide, {vision_align(g_slots[mi].vision)}px per token)\n")
    return ""
}

//! Give slot `name` a vision arm: load the vision mmproj at `path` (any family) so its chat route accepts
//! `image_url` content parts. Call ONCE per slot, after ``add_model``, before ``init_server`` /
//! ``run_server`` (the media worker snapshots the arms at init). Anything wrong with the pair
//! PANICS here, not on the first request.
def set_model_vision(name : string; path : string) {
    if (g_server_started) {
        panic("dasllama-server: set_model_vision after init_server - the media worker snapshots the arms at init; arm the slot before starting (a runtime load arms towers through POST /v1/models/load)")
    }
    let mi = slot_index(name)
    if (mi < 0) {
        panic("dasllama-server: --image-mmproj names slot '{name}', which is not served")
    }
    let err = arm_slot_vision(mi, path)
    if (!empty(err)) {
        panic("dasllama-server: {err}")
    }
}

// ``arm_slot_vision``'s audio twin — META-only validation, the media worker owns the encoder.
def private arm_slot_audio(mi : int; path : string) : string {
    if (g_slots[mi].audio_armed) {
        return "slot '{g_slots[mi].name}' already has an audio arm ({base_name(g_slots[mi].audio_path)}) - the arm is once per slot"
    }
    let proj_dim = audio_probe_proj_dim(path)
    if (proj_dim == 0l) {
        return "{base_name(path)} carries no audio encoder dasLLAMA serves (gemma4a, or a whisper-class tower: qwen2a, qwen2.5o, ultravox, voxtral) - it cannot arm audio for {g_slots[mi].name}"
    }
    if (proj_dim != g_slots[mi].model.config.dim) {
        return "{base_name(path)}'s audio encoder projects to {proj_dim} but {g_slots[mi].name} is {g_slots[mi].model.config.dim}-wide - mismatched pair"
    }
    var probe <- create_chat_renderer(g_slots[mi].model, "", 1l)
    // an ultravox span takes no marker: the stock decoder it pairs with declares none
    let no_markers = !audio_probe_span_bare(path) && (empty(probe.tmpl.audio_pre.parts) || !audio_vocab_ok(g_slots[mi].model, probe.tmpl))
    delete probe
    if (no_markers) {
        return "{g_slots[mi].name}'s chat template or vocab carries no audio span markers - it has no audio arm"
    }
    g_slots[mi].audio_armed = true
    g_slots[mi].audio_bare = audio_probe_span_bare(path)
    g_slots[mi].audio_path = path
    to_log(LOG_INFO, "dasllama-server: {g_slots[mi].name} audio arm: {base_name(path)} ({proj_dim}-wide)\n")
    return ""
}

//! Give slot `name` an audio arm: the mmproj at `path` must carry an audio encoder the engine serves - gemma4a,
//! or a whisper-class tower (an E-series or Omni mmproj carries BOTH towers, so the same file arms vision
//! and audio). The chat route
//! then accepts `input_audio` content parts — the clip serves through THIS slot, no second
//! decoder copy. Same call contract as ``set_model_vision``: once per slot, before
//! ``init_server`` (the media worker snapshots the arms at init); a bad pair PANICS here.
def set_model_audio(name : string; path : string) {
    if (g_server_started) {
        panic("dasllama-server: set_model_audio after init_server - the media worker snapshots the arms at init; arm the slot before starting (a runtime load arms towers through POST /v1/models/load)")
    }
    let mi = slot_index(name)
    if (mi < 0) {
        panic("dasllama-server: audio mmproj names slot '{name}', which is not served")
    }
    let err = arm_slot_audio(mi, path)
    if (!empty(err)) {
        panic("dasllama-server: {err}")
    }
}

//! a speech model's id: the one the config gave, else its file's stem
def private speech_model_id(id, path : string) : string => empty(id) ? stem(base_name(path)) : id

//! whether a speech model already answers to `id`: the request's `model` field has to name one model
def private speech_id_taken(id : string) : bool {
    for (a in g_asr) {
        return true if (a.id == id)
    }
    for (m in g_tts) {
        return true if (m.id == id)
    }
    return false
}

//! Serve one more ASR model: `workers` long-lived threads each load and own it in a cloned context (model/session
//! pointers never cross context heaps). A request's `model` field names it by `id` (the file's stem when empty); the
//! first model added is the default. An empty path adds nothing. Call before ``run_server``.
def add_asr_model(path, mmproj : string; workers : int = 1; id : string = "") {
    return if (empty(path))
    let mid = speech_model_id(id, path)
    if (speech_id_taken(mid)) {
        to_log(LOG_ERROR, "dasllama-server: ASR model {base_name(path)} left out - the id '{mid}' is already served (give the entry an id of its own)\n")
        return
    }
    g_asr |> emplace(AsrSlot(id = mid, path = path, mmproj = mmproj, workers = clamp(workers, 1, 8)))
}

//! Serve ONE ASR model: every model added before is dropped, then `path` is added (an empty path leaves none).
def configure_asr_workers(path, mmproj : string; workers : int = 1) {
    delete g_asr
    add_asr_model(path, mmproj, workers)
}

//! The ASR model a request's `model` field names. One served model takes every request, whatever the field says - a
//! client written for another server names a model of its own. Several: a model's id, or the default for an empty
//! field and for the `<default slot>-asr` id /v1/models lists; -1 for any other name.
def private asr_slot_of(model : string) : int {
    return -1 if (empty(g_asr))
    for (i in range(length(g_asr))) {
        return i if (g_asr[i].id == model)
    }
    return (length(g_asr) == 1 || empty(model) || model == g_asr_alias) ? 0 : -1
}

//! the default TTS model: the first one armed, -1 when none is
def private tts_default : int {
    for (i in range(length(g_tts))) {
        return i if (g_tts[i].armed)
    }
    return -1
}

//! the TTS model a request's `model` field names: the default for an empty field, -1 for an id no armed model carries
def private tts_slot_of(model : string) : int {
    return tts_default() if (empty(model))
    for (i in range(length(g_tts))) {
        return i if (g_tts[i].armed && g_tts[i].id == model)
    }
    return -1
}

//! The weight lane ``/v1/audio/speech`` requested for its default model — what ``configure_tts`` kept
//! after normalizing. The lane the worker actually pinned rides the ``tts`` block of
//! ``GET /v1/stats``.
def server_tts_lane : string {
    return empty(g_tts) ? TTS_LANE_DEFAULT : g_tts[0].lane
}

//! Configure the TTS model ``/v1/audio/speech`` serves: one worker thread loads and owns it in
//! its own context, the route id is the file's stem. Call once before ``run_server``. A missing
//! GGUF or front-end pack DEGRADES instead of panicking — the missing file is logged at
//! LOG_ERROR and the speech route is dropped (``server_has_tts`` then reads false), because a
//! crash-looping boot under the watchdog never gets a page to fix the config from. The rest of
//! the boot serves on; ``tts_files_present`` answers the same file check before the call. A load
//! that fails once the worker has the files - a front-end pack from before this build, a
//! truncated GGUF - degrades the same way at ``init_server``, and the reason then rides the
//! ``tts`` block of ``GET /v1/stats``.
//! `lane` is the weight lane the worker pins around its load — ``q8`` (the default) or ``f32``;
//! an unknown spelling warns and serves q8, the way every other enum-ish config key degrades.
//! `voices_dir` names a directory of voice clips a cloning model adds to its voices once loaded,
//! each under its file's stem; a clip that does not decode is logged and skipped, and the
//! directory on a model that cannot clone is logged and ignored - neither drops the route.
def configure_tts(path : string; lane : string = TTS_LANE_DEFAULT; voices_dir : string = "") {
    delete g_tts   // a re-configure is a fresh answer: the previous boot's models and load failures are not this one's
    add_tts_model(path, lane, voices_dir)
}

//! Serve one more TTS model beside the ones configured, under `id` (the file's stem when empty): the same file check
//! and the same degrade as ``configure_tts``, a model at a time - a model whose file set is incomplete is logged and
//! left out, the others serve. The first model armed is the default. An empty path adds nothing.
def add_tts_model(path : string; lane : string = TTS_LANE_DEFAULT; voices_dir : string = ""; id : string = "") {
    return if (empty(path))
    let missing = tts_missing_file(path)
    if (!empty(missing)) {
        to_log(LOG_ERROR, "dasllama-server: dropping the speech route for {base_name(path)} - a TTS file was not found: {missing} (a phoneme family's GGUF and the front-end packs tts_g2p.bin + tts_postag.bin sit in one directory, a Pocket TTS GGUF stands alone; fix the config, then POST /restart)\n")
    }
    let mid = speech_model_id(id, path)
    if (speech_id_taken(mid)) {
        to_log(LOG_ERROR, "dasllama-server: TTS model {base_name(path)} left out - the id '{mid}' is already served (give the entry an id of its own)\n")
        return
    }
    g_tts |> emplace(TtsSlot(id = mid, path = path, lane = pick_tts_lane(lane), voices_dir = voices_dir, armed = empty(missing)))
}

//! True when a TTS model is armed: false when none was configured, and false when every configured
//! one's file set was incomplete or its worker could not load it.
def server_has_tts : bool {
    return tts_default() >= 0
}

//! True when a speech model serves with no LLM slot beside it: an armed TTS model or a configured ASR model.
def server_has_speech : bool
    => server_has_tts() || !empty(g_asr)

//! Stop admitting new inference/ASR work and exit after all accepted work finishes. Also wired to
//! ``POST /shutdown``.
def request_stop() {
    g_shutdown_requested = true
}

var private g_restart_requested = false
var private g_config_surface_json = ""   // main's pre-serialized {config, sources, ...} object
var private g_config_save_path = ""
var private g_config_model_path = ""

var private g_catalog_dir = ""
var private g_setup_mode = false

//! Setup mode: no model is loaded; the page serves the catalog/config/recovery surface and
//! every inference route 404s. Entered when no model is configured, or every configured
//! path fails its file check.
def set_setup_mode(on : bool) {
    g_setup_mode = on
}

//! Register the resolved models dir (call before run_server) — the /catalog routes serve from it.
def set_catalog_dir(dir : string) {
    g_catalog_dir = dir
}

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

//! True while a benchmark runs - the A/B child measuring this box, or the in-process rows.
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

//! `ab` when POST /bench spawns the lcpp_bench + llama-bench child (a source-tree daslang and
//! `lcpp_bin` both configured), else `inprocess`: the rows run on the served model right here.
def private bench_mode() : string {
    return (!empty(g_lcpp_bin) && !empty(g_daslang_bin)) ? "ab" : "inprocess"
}

def private handle_bench_start(var resp : Reply) : http_status {
    if (g_bench_state == "running") {
        return resp |> JSON(error_body("a benchmark is already running", "invalid_request_error"), http_status.CONFLICT)
    }
    if (bench_mode() == "inprocess") {
        return ibench_start(resp)
    }
    if (streams_busy()) {
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

def private handle_bench_get(var resp : Reply) : http_status {
    var log_doc = JV(g_bench_log)
    let log_json = write_json_compact(log_doc)
    unsafe {
        delete log_doc
    }
    let result = empty(g_bench_result_json) ? "null" : g_bench_result_json
    return resp |> JSON(
        "\{\"state\":{json_str(g_bench_state)},\"mode\":{json_str(bench_mode())},\"log\":{log_json},\"result\":{result},\"hardware\":{json_str(g_hardware_line)}}",
        http_status.OK)
}

// ===== the in-process benchmark =====
// A bundle carries no daslang to spawn and no llama-bench to compare against, so /bench runs the
// llama-bench rows itself on the served model: pp512 and tg128, an untimed warmup then three
// timed reps each - one pp prefill, or one tg token, per tick, so the HTTP surface stays live
// between them - and, on a slot serving two streams or more, tg128@N: N streams through the
// scheduler (device-home where the device homes them), the summed rate, one rep a tick. Every route that reaches the model or the cores holds for the run's duration
// (the text, audio and speech routes with a 503, the model-switching, bake and download routes
// with a 409): work admitted mid-row would share the cores with it and the numbers would
// measure the contention. The rows run llama-bench's flat session, sized to the rows, under the
// slot's KV codec; the result names the device that served them, the exec tier and the tune
// state, so a number never travels without what produced it.

let private IBENCH_REPS = 3
let private IBENCH_NPL = 4l   //! the batched row's streams, where the device homes that many (fewer homes, fewer streams; under two, no row)

let private IBENCH_PHASE_IDLE = 0
let private IBENCH_PHASE_BIND = 1   //! the bind and the pp warmup
let private IBENCH_PHASE_PP = 2
let private IBENCH_PHASE_TG_WARM = 3
let private IBENCH_PHASE_TG = 4
let private IBENCH_PHASE_BATCHED = 5

var private g_ibench_phase = IBENCH_PHASE_IDLE
var private g_ibench_rep = 0
var private g_ibench_tok = 0l      // the tg rep's next token position
var private g_ibench_tok_ns = 0l   // the tg rep's forwards so far, nanoseconds
var private g_ibench_slot = 0
var private g_ibench_threads = 0
var private g_ibench_t0 : int64
var private g_ibench_full_seq = 0l   // the benched model's served seq_len, restored when the rows' session goes
var private g_ibench_pp : array<double>
var private g_ibench_tg : array<double>
var private g_ibench_tgb : array<double>   //! the batched row's reps, the summed rate each
var private g_ibench_npl = 0l              //! ... its streams; 0 = the slot serves too few for the row
var private g_ibench_kv = ""               //! the codec the rows ran, and the device that served them, read before the flat session goes
var private g_ibench_backend = ""
var private g_ibench_session : Session
var private g_ibench_passes0 = 0l   // the CPU-pass count as the rows began: a GPU slot's rows that move it say so

//! True while the in-process rows run.
def ibench_running() : bool {
    return g_ibench_phase != IBENCH_PHASE_IDLE
}

//! True while any benchmark measures this box - the in-process rows, or the A/B child: every
//! route that could contend with it holds on this, whichever mode runs.
def bench_hold_active() : bool {
    return server_bench_running()
}

def private ibench_hold_text() : string {
    return "a benchmark is running on this server - retry when GET /bench reports it done"
}

//! the deferred routes' hold: the audio and speech workers share the cores the timed reps run
//! on, so their queues answer 503 for the run's duration like the text routes do
def private reject_while_benching(srv : FrontLink; writer : WriterRef) : bool {
    if (!bench_hold_active()) {
        return false
    }
    service_unavailable(srv, writer, ibench_hold_text())
    return true
}

//! the llama-bench line that measures the same GGUF with the same shapes on the same device,
//! for the comparison a bundle cannot run itself: `-ngl 99` where a GPU served the rows (Metal,
//! or the Vulkan tier's `gpu:*` words), `-ngl 0` where the CPU did (lcpp_bench's own --ref spelling)
def private ibench_ref_cmd(gguf : string; threads : int; backend : string; npl : int64) : string {
    let ngl = (backend == "metal" || (backend |> starts_with("gpu"))) ? 99 : 0
    let flat = "llama-bench -m \"{gguf}\" -ngl {ngl} -p {BENCH_PP_TOKENS} -n {BENCH_TG_TOKENS} -r {IBENCH_REPS} -t {threads}"
    if (npl <= 0l) {
        return flat
    }
    let batched <- llama_batched_bench_args("llama-batched-bench", "\"{gguf}\"", ngl, threads, BENCH_PP_TOKENS, BENCH_TG_TOKENS, npl)
    return "{flat} ; {join(batched, " ")}"
}

//! what served the rows, read from the session after they ran: `metal` when the slot maps the
//! Metal flavor (the tier's `backend_effective` speaks only of the Vulkan tier and reads `cpu`
//! under Metal); `gpu:resident` when the resident driver stamped the session - it takes the
//! rows' flat session where it declines the paged serving one - else the slot's own word
def private ibench_backend(si : int) : string {
    if (slot_flavor(g_slots[si]) == "metal") {
        return "metal"
    }
    if (g_ibench_session.rdec_gen != 0l) {
        return "gpu:resident"
    }
    return g_slots[si].backend_effective
}

//! the tune state the rows measured on - `fat` (the shipped class clones), `tuned` (every kernel
//! from a manifest or a profile), `untuned (N of M on fallback)`, or `none` without the framework
def private ibench_tune_state() : string {
    static_if (typeinfo module_exists(llvm_tune)) {
        if (tune_fat_built()) {
            return "fat"
        }
        var inscope rows <- tune_status()
        if (empty(rows)) {
            return "none"
        }
        var fallback = 0
        for (r in rows) {
            fallback += (r.source == "manifest" || r.source == "profile") ? 0 : 1
        }
        return fallback == 0 ? "tuned" : "untuned ({fallback} of {length(rows)} on fallback)"
    } else {
        return "none"
    }
}

def private ibench_log(text : string) {
    g_bench_log |> push(text)
    to_log(LOG_INFO, "dasllama-server: bench: {text}\n")
}

def private ibench_start(var resp : Reply) : http_status {
    if (empty(g_slots)) {
        return resp |> JSON(error_body("no model is loaded - setup mode: pick one from the catalog first", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (g_shutdown_requested) {
        return resp |> JSON(error_body("the server is draining - nothing starts now", "invalid_request_error"), http_status.SERVICE_UNAVAILABLE)
    }
    let ctx = g_slots[g_default_slot].model.config.seq_len
    if (ctx < BENCH_PP_TOKENS) {   // the pp row prefills 512 tokens into the rows' session, and a flat session's prefill has no capacity check - a shorter cache would be written past
        return resp |> JSON(error_body("the served context is {ctx} tokens and the pp{BENCH_PP_TOKENS} row needs {BENCH_PP_TOKENS} - raise --ctx (or the slot's ctx) to bench", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (streams_busy() || g_tts_pending > 0) {
        return resp |> JSON(
            error_body("streams are active — contended numbers are misleading; drain first, then bench", "invalid_request_error"),
            http_status.CONFLICT)
    }
    if (g_bake_state == "running" || catalog_download_running()) {   // a converter child or a download shares the cores and the disk with the rows
        return resp |> JSON(
            error_body(g_bake_state == "running" ? "a bake is running — its converter competes for the cores; bench when it finishes" : "a catalog download is running — bench when it finishes", "invalid_request_error"),
            http_status.CONFLICT)
    }
    g_bench_state = "running"
    delete g_bench_log
    g_bench_result_json = ""
    if (!empty(g_lcpp_bin)) {   // the A/B the config asks for needs a source-tree daslang to spawn; a bundle has none
        ibench_log("lcpp_bin is set but no daslang is here to spawn the A/B child - measuring in process")
    }
    g_ibench_slot = g_default_slot
    g_ibench_threads = served_lanes()
    g_ibench_t0 = ref_time_ticks()
    delete g_ibench_pp
    delete g_ibench_tg
    g_ibench_rep = 0
    g_ibench_phase = IBENCH_PHASE_BIND
    return resp |> JSON("\{\"ok\":true,\"state\":\"running\"}", http_status.ACCEPTED)
}

//! a device-home session lets go of its K/V region before it is deleted, or the region stays pinned to a dead session
def private ibench_park_session() {
    if (g_ibench_session.device_kv && g_ibench_slot < length(g_slots)) {
        install_slot_marks(g_ibench_slot)
        gpu_device_kv_park(g_ibench_session)
    }
}

//! ends the run in `state`; `why` names an abandoned run in the log the page shows
def private ibench_stop(state, why : string) {
    ibench_park_session()
    delete g_ibench_session
    if (g_ibench_full_seq != 0l && g_ibench_slot < length(g_slots)) {   // the rows shrank the model's seq_len to their session; the serving session was built at the full one
        g_slots[g_ibench_slot].model.config.seq_len = g_ibench_full_seq
    }
    g_ibench_full_seq = 0l
    delete g_ibench_pp
    delete g_ibench_tg
    delete g_ibench_tgb
    g_ibench_phase = IBENCH_PHASE_IDLE
    g_bench_state = state
    if (!empty(why)) {
        ibench_log("run abandoned - {why}")
    }
}

def private ibench_finish() {
    let pp = bench_row("pp{BENCH_PP_TOKENS}", g_ibench_pp)
    let tg = bench_row("tg{BENCH_TG_TOKENS}", g_ibench_tg)
    let name = g_slots[g_ibench_slot].name
    let path = g_slots[g_ibench_slot].path
    let backend = g_ibench_backend
    let kv = g_ibench_kv
    let cpu_calls = cpu_pass_total() - g_ibench_passes0
    if (cpu_calls > 0l) {
        ibench_log("WARNING: {cpu_calls} of the rows' calls ran on the CPU instead of the GPU - the model card lists the reasons; these numbers are not the GPU's")
    }
    let exec = is_standalone_exe() ? "exe-native" : (jit_enabled() ? "jit" : "interpreted")
    let tune = ibench_tune_state()
    let elapsed = double(get_time_nsec(g_ibench_t0)) / 1.0e9lf   // the 64-bit clock: three reps of a big model on a slow box can pass the 32-bit microsecond one
    var tgb_text = ""
    var tgb_json = ""
    if (g_ibench_npl > 0l) {   //! the batched row: its rate is the streams' summed one
        let tgb = bench_row("tg{BENCH_TG_TOKENS}@{g_ibench_npl}", g_ibench_tgb)
        tgb_text = " · tg{BENCH_TG_TOKENS}@{g_ibench_npl} {tgb.mean:.1f} ± {tgb.sd:.1f} tok/s summed over {g_ibench_npl} streams"
        tgb_json = ",\"ours_tgb\":{tgb.mean},\"tgb_sd\":{tgb.sd},\"npl\":{g_ibench_npl}"
    }
    ibench_log("ours: pp{BENCH_PP_TOKENS} {pp.mean:.1f} ± {pp.sd:.1f} tok/s · tg{BENCH_TG_TOKENS} {tg.mean:.1f} ± {tg.sd:.1f} tok/s{tgb_text} ({IBENCH_REPS} reps, {elapsed:.1f}s, {backend}, kv {kv}, {exec}, tune {tune})")
    g_bench_result_json = "\{\"mode\":\"inprocess\",\"ours_pp\":{pp.mean},\"ours_tg\":{tg.mean},\"pp_sd\":{pp.sd},\"tg_sd\":{tg.sd}{tgb_json},\"reps\":{IBENCH_REPS},\"model\":{json_str(name)},\"gguf\":{json_str(base_name(path))},\"backend\":{json_str(backend)},\"kv\":{json_str(kv)},\"exec\":{json_str(exec)},\"tune\":{json_str(tune)},\"threads\":{g_ibench_threads},\"elapsed_s\":{elapsed:.1f},\"ts\":{created_ts()},\"ref_cmd\":{json_str(ibench_ref_cmd(path, g_ibench_threads, backend, g_ibench_npl))}}"
    ibench_stop("done", "")
    to_log(LOG_INFO, "dasllama-server: bench complete\n")
}

//! one step of the rows per tick - the bind and the pp warmup, one pp rep, the tg warmup, or one
//! tg token - on the default slot's model; a panic inside ends the run as failed, never the server
def private ibench_tick() {
    return if (g_ibench_phase == IBENCH_PHASE_IDLE)
    var failure = ""
    let panicked = catch_worker_panic(failure) {
        ibench_step()
    }
    if (panicked) {
        ibench_log("the rows panicked: {failure}")
        ibench_stop("failed", "")
    }
}

def private ibench_step() {
    let si = g_ibench_slot
    install_slot_marks(si)
    maybe_bind_gpu(si)
    set_mtp_spec(g_slots[si].sched.mtp)   // the engine's prefill-warm gate follows the benched slot, as it follows the stepped one
    g_cur = si
    let m & = cur_model()
    if (g_ibench_phase == IBENCH_PHASE_BIND) {
        let device = slot_flavor(g_slots[si]) == "metal" ? "metal" : g_slots[si].backend_effective   // the tier's word speaks only of Vulkan and reads `cpu` under Metal
        ibench_log("in-process: pp{BENCH_PP_TOKENS}/tg{BENCH_TG_TOKENS} x {IBENCH_REPS} reps on {g_slots[si].name} ({device}, kv {g_slots[si].opts.kv_dtype}, {g_ibench_threads} lanes), llama-bench shape - the text, audio and speech routes hold until it finishes")
        let device_home = slot_wants_device_kv(g_slots[si])
        let served_ctx = g_slots[si].model.config.seq_len   //! read before the flat rows' session narrows it
        if (!device_home) {   // a device-home run takes its region's context, which the slot's seq_len already is: nothing to put back
            g_ibench_full_seq = served_ctx
        }
        ibench_park_session()
        delete g_ibench_session   // the previous run's cache, when a run ended without its stop
        g_ibench_session <- (device_home ? create_device_session(g_slots[si].model, gpu_device_session_dtype())
            : bench_session(g_slots[si].model, BENCH_PP_TOKENS, BENCH_TG_TOKENS, g_slots[si].opts.kv_dtype, g_slots[si].opts.kv_dtype))
        g_ibench_kv = device_home ? "{gpu_device_session_dtype()}" : "{g_slots[si].opts.kv_dtype}"
        g_ibench_npl = min(IBENCH_NPL, device_home ? gpu_device_sessions() : g_slots[si].sched.max_streams)   //! the batched row's streams: a region each where the device homes them, else the slot's served count
        if (g_ibench_npl < 2l) {
            g_ibench_npl = 0l
        }
        let row_needs_ctx = BENCH_PP_TOKENS + BENCH_TG_TOKENS + g_ibench_npl
        if (g_ibench_npl > 0l && served_ctx < row_needs_ctx) {
            ibench_log("no tg{BENCH_TG_TOKENS}@{g_ibench_npl} row: the served context is {served_ctx} tokens and the row's streams need {row_needs_ctx}")
            g_ibench_npl = 0l
        }
        g_ibench_passes0 = cpu_pass_total()
        bench_pp_warm(m, g_ibench_session, BENCH_PP_TOKENS)
        let sane = bench_warm_sanity(m, g_ibench_session)
        if (!sane.ok) {
            ibench_log("REFUSED (pp{BENCH_PP_TOKENS}): {sane.detail}; the serving pipeline is broken and its timings would be meaningless")
            ibench_stop("failed", "")
            return
        }
        ibench_log("sanity (pp{BENCH_PP_TOKENS}): {sane.detail}")
        g_ibench_phase = IBENCH_PHASE_PP
        g_ibench_rep = 0
    } elif (g_ibench_phase == IBENCH_PHASE_PP) {
        let tps = bench_pp_rep(m, g_ibench_session, BENCH_PP_TOKENS, int64(g_ibench_rep))
        g_ibench_pp |> push(tps)
        ibench_log("pp{BENCH_PP_TOKENS} rep {g_ibench_rep + 1}/{IBENCH_REPS}: {tps:.1f} tok/s")
        g_ibench_rep ++
        if (g_ibench_rep >= IBENCH_REPS) {
            g_ibench_phase = IBENCH_PHASE_TG_WARM
        }
    } elif (g_ibench_phase == IBENCH_PHASE_TG_WARM) {
        bench_tg_warm(m, g_ibench_session)
        let sane = bench_warm_sanity(m, g_ibench_session)
        if (!sane.ok) {
            ibench_log("REFUSED (tg{BENCH_TG_TOKENS}): {sane.detail}; the serving pipeline is broken and its timings would be meaningless")
            ibench_stop("failed", "")
            return
        }
        ibench_log("sanity (tg{BENCH_TG_TOKENS}): {sane.detail}")
        g_ibench_phase = IBENCH_PHASE_TG
        g_ibench_rep = 0
        g_ibench_tok = 0l
        g_ibench_tok_ns = 0l
    } elif (g_ibench_phase == IBENCH_PHASE_TG) {
        ibench_tg_token(m, si)
    } else {
        ibench_batched_rep(m)
    }
}

//! one tg token per tick: a tg rep is 128 forwards, seconds on a real model, and the tick between them is what answers
//! the supervisor's health probe; the last rep hands over to the batched row, or finishes
def private ibench_tg_token(m : Model; si : int) {
    g_ibench_tok_ns += bench_tg_step(m, g_ibench_session, g_ibench_tok, int64(g_ibench_rep))
    g_ibench_tok ++
    if (g_ibench_tok < BENCH_TG_TOKENS) return
    let tps = double(BENCH_TG_TOKENS) * 1.0e9lf / double(g_ibench_tok_ns)
    g_ibench_tg |> push(tps)
    ibench_log("tg{BENCH_TG_TOKENS} rep {g_ibench_rep + 1}/{IBENCH_REPS}: {tps:.1f} tok/s")
    g_ibench_rep ++
    g_ibench_tok = 0l
    g_ibench_tok_ns = 0l
    if (g_ibench_rep < IBENCH_REPS) return
    g_ibench_backend = ibench_backend(si)   // before the flat session goes
    if (g_ibench_npl > 0l) {
        ibench_park_session()   //! the flat row's region goes back: the batched row's streams take every region
        delete g_ibench_session
        if (g_ibench_full_seq != 0l) {   //! the flat rows' session narrowed the context; the batched row's streams need the served one
            g_slots[si].model.config.seq_len = g_ibench_full_seq
        }
        g_ibench_phase = IBENCH_PHASE_BATCHED
        g_ibench_rep = 0
    } else {
        ibench_finish()
    }
}

//! one batched rep per tick: the streams' prefills and 128 batched steps, a second or so on a small model;
//! a refused rep omits the batched row and the flat rows already measured stand as the result
def private ibench_batched_rep(m : Model) {
    let br = bench_tg_batched_rep(m, g_ibench_npl, BENCH_PP_TOKENS, BENCH_TG_TOKENS, int64(g_ibench_rep), g_slots[g_ibench_slot].sched.mtp)   //! a slot serving self-speculative streams benches them as it serves them
    if (!empty(br.why)) {
        ibench_log("REFUSED (tg{BENCH_TG_TOKENS}@{g_ibench_npl}): {br.why} - the batched row is omitted")
        g_ibench_npl = 0l
        ibench_finish()
        return
    }
    g_ibench_tgb |> push(br.tps)
    ibench_log("tg{BENCH_TG_TOKENS}@{g_ibench_npl} rep {g_ibench_rep + 1}/{IBENCH_REPS}: {br.tps:.1f} tok/s summed")
    g_ibench_rep ++
    if (g_ibench_rep >= IBENCH_REPS) {
        ibench_finish()
    }
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

def private handle_bake_start(req : FrontReq; var resp : Reply) : http_status {
    if (g_bake_state == "running") {
        return resp |> JSON(error_body("a bake is already running", "invalid_request_error"), http_status.CONFLICT)
    }
    if (server_bench_running()) {
        return resp |> JSON(error_body("a benchmark is running — a bake and a benchmark are one at a time; retry when GET /bench reports it done", "invalid_request_error"), http_status.CONFLICT)
    }
    if (empty(g_daslang_bin)) {
        return resp |> JSON(
            error_body("baking needs a source-tree daslang (standalone exe cannot spawn the converter child)", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    if (streams_busy() || bench_hold_active()) {
        return resp |> JSON(
            error_body(bench_hold_active() ? ibench_hold_text() : "streams are active — the converter competes for cores and RAM; drain first, then bake", "invalid_request_error"),
            http_status.CONFLICT)
    }
    var model_field = ""
    var jerr = ""
    let body_text = req.body
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

def private handle_catalog_download(req : FrontReq; var resp : Reply) : http_status {
    if (bench_hold_active()) {   // the twin of the start refusal: a download shares the cores and the disk with the rows
        return resp |> JSON(error_body(ibench_hold_text(), "invalid_request_error"), http_status.CONFLICT)
    }
    var jerr = ""
    var js = read_json(req.body, jerr)
    defer() {
        unsafe {
            delete js
        }
    }
    let want = js?["name"] ?? ""
    let tower = js?["tower"] ?? ""
    if (tower == "asr") {
        let refusal = catalog_download_start_asr(g_catalog_dir)
        if (!empty(refusal)) {
            return resp |> JSON(error_body(refusal, "invalid_request_error"), http_status.CONFLICT)
        }
        to_log(LOG_INFO, "dasllama-server: catalog download started: asr tower (into {g_catalog_dir})\n")
        return resp |> JSON("\{\"ok\":true,\"name\":\"asr\"}", http_status.ACCEPTED)
    }
    if (tower == "tts") {
        let file = js?["file"] ?? ""
        if (empty(file)) {
            let why = empty(jerr) ? "" : " ({jerr})"
            return resp |> JSON(error_body("the tts arm names one file of the speech set: \{\"tower\": \"tts\", \"file\": ...}{why}", "invalid_request_error"), http_status.BAD_REQUEST)
        }
        let refusal = catalog_download_start_tts(g_catalog_dir, file)
        if (!empty(refusal)) {
            return resp |> JSON(error_body(refusal, "invalid_request_error"), http_status.CONFLICT)
        }
        let dl_name = "tts:{file}"
        to_log(LOG_INFO, "dasllama-server: catalog download started: {dl_name} (into {g_catalog_dir})\n")
        return resp |> JSON("\{\"ok\":true,\"name\":{json_str(dl_name)}}", http_status.ACCEPTED)
    }
    if (empty(want)) {
        let why = empty(jerr) ? "" : " ({jerr})"
        return resp |> JSON(error_body("body must name a catalog entry: \{\"name\": ...}{why}", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (!empty(tower) && tower != "vision") {
        return resp |> JSON(error_body("unknown tower '{tower}' (vision | asr | tts)", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    for (e in model_catalog()) {
        if (e.name == want) {
            let refusal = (tower == "vision" ? catalog_download_start_vision(g_catalog_dir, e)
                                             : catalog_download_start(g_catalog_dir, e))
            if (!empty(refusal)) {
                return resp |> JSON(error_body(refusal, "invalid_request_error"), http_status.CONFLICT)
            }
            // echo the id the download state machine registered, so a caller can match the
            // ACCEPTED body to the download.name it will poll
            let dl_name = tower == "vision" ? "vision:{e.name}" : e.name
            let bytes = tower == "vision" ? e.vision_bytes : e.bytes
            to_log(LOG_INFO, "dasllama-server: catalog download started: {dl_name} ({bytes >> 20l} MB into {g_catalog_dir})\n")
            return resp |> JSON("\{\"ok\":true,\"name\":{json_str(dl_name)},\"bytes\":{bytes}}", http_status.ACCEPTED)
        }
    }
    return resp |> JSON(error_body("unknown catalog entry '{want}'", "invalid_request_error"), http_status.NOT_FOUND)
}

def private handle_bake_get(var resp : Reply) : http_status {
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

def private handle_images(var resp : Reply) : http_status {
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

var private g_box_ram_gb = 0   // total physical RAM in whole GiB, probed once at init_server (0 = unknown)

// linux /proc/meminfo: "MemTotal:       131842348 kB" -> whole GiB; 0 = parse failure
def private ram_gb_from_meminfo(text : string) : int {
    for (ln in split(text, "\n")) {
        if (ln |> starts_with("MemTotal")) {
            let c = find(ln, ":")
            if (c >= 0) {
                var toks <- split(strip(slice(ln, c + 1)), " ")
                let kb = empty(toks) ? 0.0 : to_float(toks[0])
                delete toks
                return int(kb / (1024.0 * 1024.0) + 0.5)
            }
        }
    }
    return 0
}

// Total physical RAM — a twin of profile_common's private platform_ram_gb (kept separate so
// the server never pays that file's tool closure); wmic falls through to CIM on Windows.
def private box_ram_gb() : int {
    let gib = 1024.0 * 1024.0 * 1024.0
    var out = ""
    if (get_platform_name() == "windows") {
        var args <- ["wmic", "ComputerSystem", "get", "TotalPhysicalMemory", "/value"]
        run_and_capture(args, out, 10.0)
        delete args
        for (ln in split(out, "\n")) {
            let i = find(ln, "=")
            if (i >= 0 && find(ln, "TotalPhysicalMemory") >= 0) {
                return int(to_float(strip(slice(ln, i + 1))) / gib + 0.5)
            }
        }
        var ps <- ["powershell", "-NoProfile", "-Command", "(Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory"]
        out = ""
        run_and_capture(ps, out, 15.0)
        delete ps
        let t = strip(out)
        return empty(t) ? 0 : int(to_float(t) / gib + 0.5)
    }
    if (get_platform_name() == "darwin") {
        var args <- ["sysctl", "-n", "hw.memsize"]
        run_and_capture(args, out, 10.0)
        delete args
        return int(to_float(strip(out)) / gib + 0.5)
    }
    return ram_gb_from_meminfo(fread("/proc/meminfo"))
}

//! the lanes the kernels actually dispatch on: the jobque workers plus the calling thread,
//! narrowed by the dispatch cap when one is set (the cap sentinel 0 means all of them)
def private served_lanes() : int {
    let lanes = is_job_que_available() ? get_total_hw_jobs() + 1 : 1
    let cap = get_dispatch_worker_limit()
    return (cap > 0 && cap < lanes) ? cap : lanes
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
    let served = served_lanes()
    let lanes_desc = served < lanes ? "{served} of {lanes}" : "{lanes}"
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

struct private OaiPromptDetails {
    cached_tokens : int64
}

struct private OaiUsage {
    prompt_tokens : int64
    completion_tokens : int64
    total_tokens : int64
    prompt_tokens_details : OaiPromptDetails
}

//! a finished turn's token counts, the prefix-cache hit among them
def private usage_of(e : SchedEvent) : OaiUsage
    => OaiUsage(prompt_tokens = e.n_prompt, completion_tokens = e.n_gen, total_tokens = e.n_prompt + e.n_gen,
        prompt_tokens_details = OaiPromptDetails(cached_tokens = e.n_cached))

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

struct private OaiStreamTimings {
    ttft_ms : float
    gen_ms : float
}

struct private OaiUsageChunk {
    id : string
    @rename = "object" _object : string
    created : int64
    model : string
    choices : array<OaiStreamChoice>
    usage : OaiUsage
    timings : OaiStreamTimings
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
    failed       // the worker could not load its model: `body` carries the panic text
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
    model : string       // the ASR model that took it, by id
    state : string       // "running" | "done" | "failed"
    audio_s : float
    wall_ms : float
    rtf : float
    started_at : int64   // ref_time_ticks at the started event
    done_at : int64      // 0 while running
}

struct private AsrWire {
    @do_not_delete writer : WriterRef
    model : string       // the id of the model the request was queued on
}

var g_asr_recent : array<AsrJobInfo>   // running + lingering done jobs for the poll surface
var g_asr_done_jobs = 0l               // cumulative completed transcriptions
var g_asr_audio_s = 0.0                // cumulative audio seconds transcribed
var g_asr_events : Channel?             //! every ASR worker's events, whichever model it serves
var g_asr_wires : table<int64; AsrWire>
var g_asr_active = 0
var g_asr_pending = 0
let ASR_QUEUE_LIMIT = 32
let WORKER_STOP_WAIT_NS = 30_000_000_000l   // a worker that died on its own never answers the stop; the teardown must still finish

//! Wait on `events` until `asked` of them were stops or `deadline_ns` passed, and answer the stops seen. `is_stop` is
//! shown every event and says which are stops, so a caller settles the others - a late completion - right there.
def private await_stopped(events : Channel?; asked : int; deadline_ns : int64; is_stop : block<(ev : auto(TT)#) : bool>) : int {
    var stopped = 0
    let since = ref_time_ticks()
    while (stopped < asked && get_time_nsec(since) < deadline_ns) {
        events |> pop_with_timeout_clone(100) $(ev : TT#) {
            if (invoke(is_stop, ev)) {
                stopped++
            }
        }
    }
    return stopped
}
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
def private bad_request(srv : FrontLink; writer : WriterRef; msg : string) {
    to_log(LOG_WARNING, "dasllama-server: 400 rejected: {msg}\n")
    respond(srv, writer, int(http_status.BAD_REQUEST), "application/json",
            error_body(msg, "invalid_request_error"))
}

def private service_unavailable(srv : FrontLink; writer : WriterRef; msg : string) {
    respond(srv, writer, int(http_status.SERVICE_UNAVAILABLE), "application/json",
            error_body(msg, "server_error"))
}

def private reject_deferred_while_draining(
                                           srv : FrontLink;
                                           writer : WriterRef
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
def private send_chat_chunk(srv : FrontLink; writer : WriterRef;
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

//! a chat piece's two halves, each a chunk of its own when it carries text: the reasoning, then the content
def private send_chat_halves(srv : FrontLink; w : StreamWire; rsn, cnt : string) {
    if (!empty(rsn)) {
        send_chat_chunk(srv, w.writer, w.id, w.created, "", "", "", rsn)
    }
    if (!empty(cnt)) {
        send_chat_chunk(srv, w.writer, w.id, w.created, "", cnt, "")
    }
}

//! The chunk a stream that asked for `stream_options.include_usage` ends on, before `[DONE]`: no choices, the turn's
//! token counts with the prefix-cache hit among them, and the scheduler's own clock for the turn.
def private send_usage_chunk(srv : FrontLink; writer : WriterRef;
                             id : string; created : int64; is_chat : bool; e : SchedEvent) {
    var chunk = OaiUsageChunk(id = id, _object = is_chat ? "chat.completion.chunk" : "text_completion",
                              created = created, model = cur_name(),
                              usage = usage_of(e),
                              timings = OaiStreamTimings(ttft_ms = float(e.ttft_us) / 1000.0, gen_ms = float(e.gen_us) / 1000.0))
    sse_event(srv, writer, sprint_json(chunk, false), "")
    delete chunk
}

// The whole call set of a streaming reply as ONE tool_calls delta (index enumerates calls per the
// OpenAI incremental framing); the caller follows with the finish_reason="tool_calls" chunk.
def private send_chat_tool_calls_chunk(srv : FrontLink; writer : WriterRef;
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

def private send_compl_chunk(srv : FrontLink; writer : WriterRef;
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
def private resp_error(var resp : Reply; status : http_status; msg : string) : http_status {
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
                                     "enable_thinking", "chat_template_kwargs", "truncation", "stream_options" }
let COMPL_FIELDS : table<string> <- { "model", "prompt", "stream", "temperature", "top_k", "top_p", "min_p",
                                     "presence_penalty", "frequency_penalty", "repeat_penalty", "seed",
                                     "max_tokens", "stream_options" }
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
def private message_parts(mv : JsonValue?; route : string; var images : array<string>; var audios : array<string>) : string {
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
            } elif (ptype == "input_audio") {
                // OpenAI shape: input_audio: \{data: <base64 of the audio FILE>, format: "wav"|"mp3"}
                // — the decoder sniffs the container, so `format` is not needed here
                audios |> push(pv?["input_audio"]?["data"] ?? "")
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

// `message_parts` for routes with no media path: image/audio parts warn and drop.
def private message_text(mv : JsonValue?; route : string) : string {
    var images : array<string>
    var audios : array<string>
    let s = message_parts(mv, route, images, audios)
    if (!empty(images)) {
        to_log(LOG_WARNING, "{route}: skipping {length(images)} image content part(s) — no vision arm on this path\n")
    }
    if (!empty(audios)) {
        to_log(LOG_WARNING, "{route}: skipping {length(audios)} audio content part(s) — no audio arm on this path\n")
    }
    delete images
    delete audios
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
    strip_b64_whitespace(url, payload_at, raw)
    if (base64_decode(raw, bytes) <= 0) {
        why = "image_url payload is not valid base64"
        return false
    }
    return true
}

// base64 wire bytes minus the whitespace GNU `base64` wraps with, from `from` to the end
def private strip_b64_whitespace(s : string; from : int; var raw : array<uint8>) {
    peek_data(s) $(d) {
        raw |> reserve(length(d) - from)
        for (i in range(from, length(d))) {
            let b = int(d[i])
            if (b != '\n' && b != '\r' && b != ' ' && b != '\t') {
                raw |> push(d[i])
            }
        }
    }
}

// `decode_data_uri_bytes`' RAW twin for input_audio.data: bare base64 of the audio file (the
// OpenAI shape carries no data: prefix), whitespace-tolerant, under the same size cap.
def private decode_base64_bytes(data : string; var bytes : array<uint8>; var why : string&) : bool {
    let max_encoded = (IMAGE_MAX_FILE_BYTES / 3) * 4 + 8
    if (length(data) > max_encoded) {
        why = "audio payload exceeds the {IMAGE_MAX_FILE_BYTES / (1024 * 1024)} MB cap (wrapped base64 counts its newlines)"
        return false
    }
    var raw : array<uint8>
    defer() { delete raw }
    strip_b64_whitespace(data, 0, raw)
    if (base64_decode(raw, bytes) <= 0) {
        why = "input_audio.data is not valid base64"
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
def private handle_unknown(req : FrontReq; var resp : Reply) : http_status {
    let body = req.body
    let head = length(body) > 400 ? slice(body, 0, 400) : body
    if (empty(head)) {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path}\n")
    } else {
        to_log(LOG_WARNING, "unhandled request: {req.method} {req.path} body: {head}\n")
    }
    return resp_error(resp, http_status.NOT_FOUND, "unknown endpoint: {req.path}")
}

// ===== Handlers =====

def private handle_models(var resp : Reply) : http_status {
    var list = OaiModelList(_object = "list")
    list.data |> reserve(length(g_slots) + length(g_asr) + length(g_tts) + 1)
    for (s in g_slots) {
        list.data |> push(OaiModel(id = s.name, _object = "model", created = created_ts(), owned_by = "dasllama"))
    }
    if (!empty(g_asr) && !empty(g_slots)) {   // the default ASR model's first name: the default LLM's, suffixed
        list.data |> push(OaiModel(id = "{g_slots[g_default_slot].name}-asr", _object = "model", created = created_ts(), owned_by = "dasllama"))
    }
    if (length(g_asr) > 1 || empty(g_slots)) {   // several ASR models, or none of the suffixed name: each under its own id
        for (a in g_asr) {
            list.data |> push(OaiModel(id = a.id, _object = "model", created = created_ts(), owned_by = "dasllama"))
        }
    }
    for (m in g_tts) {
        if (m.armed) {
            list.data |> push(OaiModel(id = m.id, _object = "model", created = created_ts(), owned_by = "dasllama"))
        }
    }
    let body = sprint_json(list, false)
    delete list
    return resp |> JSON(body, http_status.OK)
}

// /v1/embeddings: mean-pooled, L2-normalized sentence embeddings over the facade `embed` verb.
// `input` is a string or an array of strings; usage is counted via the facade `encode` (cheap
// relative to the forward). Decoder-only-as-embedder — RAG-grade vectors, not a dedicated model.
def private handle_embeddings(req : FrontReq; var resp : Reply) : http_status {
    var perr = ""
    var js = read_json(req.body, perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, EMB_FIELDS, "/v1/embeddings")
    let want_model = js?["model"] ?? ""
    let mi = slot_index(want_model)
    if (mi < 0) {
        return resp_error(resp, http_status.NOT_FOUND, "model '{want_model}' not found — GET /v1/models lists the served ids{empty(g_slots) ? " (setup mode: no model loaded yet — download one from /catalog)" : ""}")
    }
    if (bench_hold_active()) {
        return resp_error(resp, http_status.SERVICE_UNAVAILABLE, ibench_hold_text())
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

def private streams_busy() : bool {
    return total_active() > 0l || total_queued() > 0l || g_asr_pending > 0 || g_media_pending > 0
}

// Free the tier for a runtime load and recreate the boot precondition (nothing installed, the
// want set for THIS load). Boot-order semantics: the load arms under the engine's want, so the
// owner's VRAM state drops first — streams are idle, but prefix-cache KV can still own mirror
// rows, hence the hydrate. Returns the evicted owner (-1 = none) for the failure re-arm.
def private free_gpu_tier_for_load(cpu_only : bool) : int {
    var prev_owner = -1
    if (!cpu_only && g_gpu_owner >= 0) {
        prev_owner = g_gpu_owner
        install_slot_marks(g_gpu_owner)
        for (st in g_slots[g_gpu_owner].sched.streams) {
            moe_gpu_hydrate_session(g_slots[g_gpu_owner].model, st.session)
        }
        moe_gpu_drop_model()
        g_slots[g_gpu_owner].backend_effective = "cpu"
        g_gpu_owner = -1
    }
    uninstall_slot_marks()
    set_gpu_tier_want(cpu_only ? GpuTierWant() : g_boot_gpu_want)
    return prev_owner
}

// Undo a runtime load that panicked after load_model: drop + remove the half-added slot (its
// scheduler may have refused the KV pool) and re-arm the GPU owner the load evicted.
def private unwind_failed_runtime_load(id : string; prev_owner : int) {
    let ni = length(g_slots) - 1
    if (ni >= 0 && g_slots[ni].name == id) {
        if (g_gpu_owner == ni) {
            install_slot_marks(ni)
            moe_gpu_drop_model()
            g_gpu_owner = -1
        }
        uninstall_slot_marks()
        delete g_slots[ni]
        g_slots |> erase(ni)
    }
    if (prev_owner >= 0) {
        bind_gpu_slot(prev_owner)
    }
}

// Arm towers on a runtime-loaded slot from its mmproj; the returned note is "" or the
// client-facing reason the slot serves text-only. Panics (a non-GGUF file, a bad projector)
// are caught here — the model itself already serves.
def private arm_runtime_towers(mi : int; mmproj, audio_mmproj : string) : string {
    var note = ""
    var failure = ""
    let failed = catch_worker_panic(failure) {
        if (!empty(mmproj)) {
            note = arm_slot_vision(mi, mmproj)
        }
        if (empty(note) && !empty(audio_mmproj)) {
            note = arm_slot_audio(mi, audio_mmproj)
        } elif (empty(note) && audio_probe_proj_dim(mmproj) > 0l) {
            note = arm_slot_audio(mi, mmproj)   // a file with both towers arms audio too
        }
    }
    if (failed) {
        note = "the tower arm failed: {failure}"
    }
    if (g_slots[mi].vision.kind != VisionKind.none || g_slots[mi].audio_armed) {
        stop_media_worker()   // the worker snapshots arms — bounce it to load the new encoders
        start_media_worker()
    }
    return note
}

// A runtime load must not double-serve: neither an id another slot answers to, nor a GGUF
// another slot already maps (two slots on one file = two full KV pools for zero capacity).
def private runtime_load_conflict(id, rpath : string) : string {
    if (slot_index(id) >= 0) {
        return "model '{id}' is already served — unload it first, or pass a different 'id'"
    }
    for (s in g_slots) {
        if (same_model_path(s.path, rpath)) {
            return "this GGUF is already serving as '{s.name}' — one slot per file (activate it, or unload it first)"
        }
    }
    return ""
}

// POST /v1/models/load. Contract: the README's route row. In-code precondition: streams must
// be idle (the busy gate) because the load blocks the tick thread for its duration.
def private handle_model_load(req : FrontReq; var resp : Reply) : http_status {   // nolint:STYLE037,STYLE038 — one guard + one client-facing message per validation
    if (!is_loopback_req(req)) {
        return deny_operator(resp)
    }
    if (!g_server_started || g_setup_mode) {
        return resp |> JSON(error_body("runtime loads need a serving server (setup mode restarts into the first model)", "invalid_request_error"), http_status.CONFLICT)
    }
    var perr = ""
    var js = read_json(req.body, perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() {
        unsafe {
            delete js
        }
    }
    let path = js?["path"] ?? ""
    if (empty(path)) {
        return resp |> JSON(error_body("'path' is required — an absolute GGUF path, or a filename under the models dir", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    var rpath = path
    if (!stat(rpath).is_valid) {
        rpath = path_join(g_catalog_dir, path)
    }
    if (!stat(rpath).is_valid) {
        return resp |> JSON(error_body("'{path}' not found (also tried {rpath})", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    let id = js?["id"] ?? base_name(rpath)
    let conflict = runtime_load_conflict(id, rpath)
    if (!empty(conflict)) {
        return resp |> JSON(error_body(conflict, "conflict"), http_status.CONFLICT)
    }
    if (streams_busy() || bench_hold_active()) {
        return resp |> JSON(error_body(bench_hold_active() ? ibench_hold_text() : "streams are live (active {total_active()}, queued {total_queued()}) — retry when idle", "busy"), http_status.CONFLICT)
    }
    let backend = js?["backend"] ?? "auto"
    if (backend != "auto" && backend != "cpu") {
        return resp |> JSON(error_body("'backend' must be \"auto\" or \"cpu\" (got '{backend}')", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    let quant_spelling = js?["quant"] ?? "q8"
    if (!empty(quant_spelling) && !quant_spelling_known(quant_spelling)) {
        return resp |> JSON(error_body("'quant' value '{quant_spelling}' is not known — valid: fp32 | q8 | q8_0 | q4_0 | q4_k | q5_k | q6_k | mxfp4 | f16 | bf16 | q4", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    let cpu_only = backend == "cpu"
    let prev_owner = free_gpu_tier_for_load(cpu_only)
    let t0 = ref_time_ticks()
    let quant = pick_quant(quant_spelling)
    var m = Model()
    var failure = ""
    if (!cpu_only) {
        gpu_residency_ask(g_sched_defaults.max_streams, js?["ctx"] ?? g_boot_ctx, g_sched_defaults.kv_dtype)
    }
    var failed = catch_worker_panic(failure) {
        delete m   // fresh above; the delete licenses the move on this heap (PERF030)
        m <- load_model(rpath, quant)
    }
    if (failed) {
        if (prev_owner >= 0) {
            bind_gpu_slot(prev_owner)   // a failed load must not leave the box demoted to CPU
        }
        to_log(LOG_ERROR, "dasllama-server: runtime load of {rpath} failed: {failure}\n")
        return resp |> JSON(error_body("the load failed: {failure}", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    let want_ctx = js?["ctx"] ?? g_boot_ctx   // body ctx > boot --ctx > model default
    if (want_ctx > 0l) {
        m.config.seq_len = min(m.config.seq_len, want_ctx)
    }
    var opts = SlotOpts()
    opts.backend = cpu_only ? "cpu" : "auto"
    clamp_ctx_to_device_room(m, id, slot_kv_dtype(opts))   // the codec the scheduler will inherit, not SlotOpts' f16 seed
    failed = catch_worker_panic(failure) {
        // the KV-geometry validate inside create_scheduler can refuse the pool — caught, so an
        // operator POST cannot abort the tick thread
        add_model(m, id, opts, rpath)
        init_one_scheduler(g_slots[length(g_slots) - 1])
    }
    if (failed) {
        unwind_failed_runtime_load(id, prev_owner)
        to_log(LOG_ERROR, "dasllama-server: runtime load of {rpath} failed after the model loaded: {failure}\n")
        return resp |> JSON(error_body("the load failed: {failure}", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (!cpu_only) {
        set_slot_gpu_want(id, g_boot_gpu_want)   // the want the switch-in re-arms with
    }
    if (prev_owner >= 0 && g_gpu_owner < 0) {
        // the new load did not take the tier (fp32 arm, no device claim) — re-arm the owner we
        // dropped instead of leaving an idle box demoted until a later step happens to re-bind
        bind_gpu_slot(prev_owner)
    }
    let mi_new = length(g_slots) - 1
    let mmproj = js?["image_mmproj"] ?? ""
    let audio_mmproj = js?["audio_mmproj"] ?? ""
    let tower_note = empty(mmproj) && empty(audio_mmproj) ? "" : arm_runtime_towers(mi_new, mmproj, audio_mmproj)
    if (js?["activate"] ?? true) {
        set_default_model(id)   // the default is the slot model-less (page) requests land on
        g_active = mi_new
        g_cur = mi_new
        set_mtp_spec(g_slots[mi_new].sched.mtp)
        install_slot_marks(mi_new)
    }
    g_slots[mi_new].last_used = ref_time_ticks()
    let load_ms = int64(get_time_usec(t0)) / 1000l
    to_log(LOG_INFO, "dasllama-server: runtime load: {id} in {load_ms}ms ({g_slots[mi_new].backend_effective})\n")
    return resp |> JSON(
        "\{\"ok\":true,\"model\":{json_str(id)},\"backend_effective\":{json_str(g_slots[mi_new].backend_effective)},\"load_ms\":{load_ms},\"ctx\":{g_slots[mi_new].model.config.seq_len},\"tower_note\":{json_str(tower_note)}}",
        http_status.OK)
}

// POST /v1/models/unload. Contract: the README's route row.
def private handle_model_unload(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return deny_operator(resp)
    }
    var perr = ""
    var js = read_json(req.body, perr)
    if (js == null) {
        return resp_error(resp, http_status.BAD_REQUEST, "invalid JSON: {perr}")
    }
    defer() {
        unsafe {
            delete js
        }
    }
    let want = js?["model"] ?? ""
    let mi = slot_index(want)
    if (mi < 0) {
        return resp |> JSON(error_body("model '{want}' not found — GET /v1/models lists the served ids", "model_not_found"), http_status.NOT_FOUND)
    }
    if (mi == g_default_slot) {
        return resp |> JSON(error_body("'{want}' is the default slot — activate another model first (POST /v1/models/activate)", "invalid_request_error"), http_status.BAD_REQUEST)
    }
    if (streams_busy() || bench_hold_active()) {
        return resp |> JSON(error_body(bench_hold_active() ? ibench_hold_text() : "streams are live (active {total_active()}, queued {total_queued()}) — retry when idle", "busy"), http_status.CONFLICT)
    }
    if (g_gpu_owner == mi) {
        install_slot_marks(mi)
        moe_gpu_drop_model()
        g_gpu_owner = -1
    }
    if (g_marks_slot == mi) {
        uninstall_slot_marks()   // the engine must not keep a dying slot's marks installed
    }
    let had_towers = g_slots[mi].vision.kind != VisionKind.none || g_slots[mi].audio_armed
    let dead_name = g_slots[mi].name
    delete g_slots[mi]
    g_slots |> erase(mi)
    // every slot index above mi shifted down one (the worker is immune: its tables key on names)
    if (g_default_slot > mi) {
        g_default_slot--
    }
    let was_active = g_active == mi
    g_active = g_active > mi ? g_active - 1 : (g_active == mi ? g_default_slot : g_active)
    g_cur = g_cur > mi ? g_cur - 1 : (g_cur == mi ? g_default_slot : g_cur)
    if (g_gpu_owner > mi) {
        g_gpu_owner--
    }
    if (g_marks_slot > mi) {
        g_marks_slot--
    }
    if (was_active) {
        // the MTP gate is process-global and update_server refreshes it only on an ACTIVE-slot
        // change — the dying slot's setting must not survive onto the inherited default
        set_mtp_spec(g_slots[g_active].sched.mtp)
    }
    if (had_towers && g_media_armed) {
        var drop = MediaJob(kind = MediaJobKind.drop, name = dead_name)
        g_media_jobs |> push_clone(drop)
        delete drop
    }
    to_log(LOG_INFO, "dasllama-server: runtime unload: {want}\n")
    return resp |> JSON("\{\"ok\":true,\"unloaded\":{json_str(want)}}", http_status.OK)
}

// POST /v1/models/activate {"model": name}: admin warm-switch — make `name` the DEFAULT +
// stepped slot (model-less page requests follow) and move the GPU tier to it NOW instead of
// waiting for the owner to drain. 409 while any work is live: the switch drops the owner's
// whole VRAM state. Loopback-only, like its load/unload siblings.
def private handle_activate(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return deny_operator(resp)
    }
    var perr = ""
    var js = read_json(req.body, perr)
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
            error_body("model '{want}' not found — GET /v1/models lists the served ids{empty(g_slots) ? " (setup mode: no model loaded yet — download one from /catalog)" : ""}", "model_not_found"),
            http_status.NOT_FOUND)
    }
    if (streams_busy() || bench_hold_active()) {
        return resp |> JSON(
            error_body(bench_hold_active() ? ibench_hold_text() : "streams are live (active {total_active()}, queued {total_queued()}) — retry when idle", "busy"),
            http_status.CONFLICT)
    }
    let t0 = ref_time_ticks()
    g_active = mi
    g_cur = mi
    set_default_model(g_slots[mi].name)   // model-less (page) requests follow the activation
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
def private resolve_stream_slot(srv : FrontLink; writer : WriterRef; js : JsonValue?) : bool {
    let want = js?["model"] ?? ""
    let mi = slot_index(want)
    if (mi < 0) {
        respond(srv, writer, int(http_status.NOT_FOUND), "application/json",
                error_body("model '{want}' not found — GET /v1/models lists the served ids{empty(g_slots) ? " (setup mode: no model loaded yet — download one from /catalog)" : ""}", "model_not_found"))
        return false
    }
    g_cur = mi
    return true
}

// Validate the rendered prompt, submit it to the scheduler, and register the wire that routes its
// SchedEvents back to `writer`. Owns the mandatory 400 (prompt does not fit the context — submit
// would panic) and the 503 (queue full). Moves prompt/stop_ids/close_toks into the request.
def private register_request(srv : FrontLink; writer : WriterRef;
                             var prompt, stop_ids, close_toks : array<int64>;
                             var spans : array<MediaSpan>;
                             params : SamplingParams; max_new : int64;
                             streaming, is_chat : bool; prefix : string;
                             tools_declared : bool = false; call_open : string = ""; call_close : string = "";
                             seed : int = 0; think : ThinkStream = ThinkStream();
                             tool_mode : ToolMode = ToolMode.none; reuse_id : int64 = 0l;
                             var nothink : NothinkStopGuard = NothinkStopGuard(); stable_at : int64 = 0l;
                             include_usage : bool = false; resumed : bool = false) {
    if (bench_hold_active()) {
        service_unavailable(srv, writer, ibench_hold_text())
        return
    }
    let np = long_length(prompt)   // media positions included: a span's ids stand in the prompt
    if (np + 1l > cur_model().config.seq_len) {
        bad_request(srv, writer, "the rendered prompt is {np} tokens; the model context is {cur_model().config.seq_len} — shorten the conversation or raise --ctx")
        return
    }
    let rid = reuse_id != 0l ? reuse_id : next_id()   // a resumed image request keeps its park id, so one [req N] spans the request
    let cid = "{prefix}-{rid}"
    let created = created_ts()
    var preq <- PendingReq(id = rid, params = params, max_new = max_new, seed = seed, stable_at = stable_at)
    var encoded_rows = 0l
    for (sp in spans) {
        encoded_rows += empty(sp.rows) ? 0l : sp.n
    }
    preq.prompt <- prompt
    preq.stop_ids <- stop_ids
    preq.nothink <- nothink
    preq.close_toks <- close_toks
    preq.spans <- spans
    if (!submit(cur_model(), cur_sched(), preq)) {
        delete preq
        to_log(LOG_WARNING, "dasllama-server: 503 queue full ({cur_sched().max_queue}) — rejected {prefix} request ({np} tok)\n")
        service_unavailable(srv, writer, "the server is at capacity ({cur_sched().max_queue} requests queued) — retry shortly")
        return
    }
    g_media_rows += encoded_rows   // after submit: a 503-rejected request counts nothing; a span the prefix cache serves brought no rows
    g_slots[g_cur].last_used = ref_time_ticks()
    set_writer_keepalive_timeout(srv, writer, LLM_WRITER_KEEPALIVE_MS)
    to_log(LOG_INFO, "[req {rid}] {prefix}: prompt={np} tok, stream={streaming}, max_new={max_new}\n")
    var wire <- StreamWire(writer = writer, streaming = streaming, include_usage = include_usage, is_chat = is_chat,
                           slot = g_cur, id = cid, created = created,
                           has_tools = tools_declared && tool_mode != ToolMode.none,
                           last_keepalive_ticks = ref_time_ticks(),
                           call_open = call_open, call_close = call_close,
                           tool_mode = tool_mode, think = think)
    g_wires[rid] <- wire
    if (streaming && is_chat && !resumed) {
        send_chat_chunk(srv, writer, cid, created, "assistant", "", "")   // opening role delta
    }
}

// Parse + validate + render + register, then return — no generation in the handler. The prompt is
// rendered through the S1 render seam (tokens only, no KV), so a queued request holds no cache
// memory; the session exists only once the scheduler admits the stream.
// Render a user-led suffix of the transcript. System messages are always retained (they were
// folded into `system`); `start` applies only to conversational messages in the original array.
def private render_chat_suffix(msgs_v : JsonValue?; system : string; tools : array<string>;   // nolint:STYLE037,STYLE038 — one arm per message role, each accumulating into the same renderer state
                               max_new : int64; start : int; think_set, think_val : bool; refs : array<MediaRef>;
                               var prompt, stop_ids, close : array<int64>&;
                               var nothink_out : NothinkStopGuard&;
                               var call_open, call_close : string&; var think_out : ThinkStream&;
                               var tool_mode_out : ToolMode&; var stable_at : int64&) : string {
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
    var seen_audios : array<string>   // ditto for input_audio parts
    defer() { delete seen_audios }
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
        let content = message_parts(mv, "/v1/chat/completions", seen_images, seen_audios)
        if (role != "tool" && !empty(tool_results)) {
            add_tool_results(chat, tool_results, tool_names)
            tool_results |> clear()
            tool_names |> clear()
            pending = true
        }
        if (role == "user") {
            add_user(chat, content)
            for (r in refs) {   // the message's media, inline where its parts sit among the text
                continue if (r.msg != i)
                add_user_span(cur_model(), chat, r.key, r.n_rows, r.text_before, !r.is_audio, r.is_audio && g_slots[g_cur].audio_bare)
            }
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
    var turn <- render_turn_marked(cur_model(), chat)
    stable_at = long_length(rendered) + turn.opening   //! every earlier turn, and the last turn's own opening
    rendered |> push_from(turn.toks)
    delete turn
    // out-params: the sole caller hands them in empty and deletes them before every re-render
    prompt <- rendered      // nolint:PERF030 — empty on entry
    stop_ids <- turn_stop_ids(chat)   // nolint:PERF030 — empty on entry; the thinking-off extras ride the guard
    nothink_out <- make_nothink_guard(chat)   // nolint:PERF030 — empty on entry, like stop_ids
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

def private handle_chat(srv : FrontLink; req : FrontReq; writer : WriterRef) {
    if (reject_deferred_while_draining(srv, writer) || reject_while_benching(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(req.body, perr)
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
    let parked = park_media_request(srv, writer, js, msgs_v)
    if (parked == MediaPark.parked) {
        js = null   // ownership parked with the wire; the defer above must not free it
        return
    }
    if (parked == MediaPark.answered) {
        return   // an error already went out; the defer frees js
    }
    var no_refs : array<MediaRef>
    defer() { delete no_refs }
    handle_chat_tail(srv, writer, js, g_cur, no_refs)
}

enum private MediaPark {
    none       // no media parts — the caller continues as a text request
    answered   // a validation error already went out; the caller returns (and frees js)
    parked     // the request is parked with the media worker, which now owns js
}

//! One media part of a request: the message and text offset it sits at, the content key its positions carry, and its
//! soft-token rows once encoded. `n_rows` is known ahead of the rows for media this slot has encoded before.
struct private MediaRef {
    key : uint64
    is_audio : bool
    msg : int
    text_before : int
    payload : string            // the part's base64 / data URI, inside the parked request's JSON
    n_rows : int64
    grid : int2
    rows : array<float>
    want_rows : bool            // known, but the prefix cache does not hold its span under this prompt: encode it
}

//! what a slot remembers of media it encoded: the span's shape, which a transcript needs to lay the span out
struct private MediaSeen {
    n_rows : int64
    grid : int2
}

let MEDIA_SPANS_LIMIT = 16     // media parts one request may carry, every message's
let MEDIA_SEEN_LIMIT = 4096    // remembered span shapes; past it the table starts over

var private g_media_seen : table<uint64; MediaSeen>

//! the media parts of every user message, in transcript order, each at its text offset in its message
def private collect_media_refs(msgs_v : JsonValue?; var refs : array<MediaRef>) {
    let salt = hash(g_slots[g_cur].name)
    for (i in range(length(msgs_v.value as _array))) {
        let mv = (msgs_v.value as _array)[i]
        let cv = mv?["content"]
        continue if (cv == null || !(cv.value is _array))
        let is_user = (mv?["role"] ?? "") == "user"
        var at = 0
        for (pv in (cv.value as _array)) {
            let ptype = pv?["type"] ?? ""
            var payload = ""
            if (ptype == "text") {
                at += length(pv?["text"] ?? "")
            } elif (ptype == "image_url") {
                let uv = pv?["image_url"]
                payload = uv != null && uv.value is _string ? (uv ?? "") : (uv?["url"] ?? "")
            } elif (ptype == "input_image") {
                payload = pv?["image_url"] ?? ""
            } elif (ptype == "input_audio") {
                payload = pv?["input_audio"]?["data"] ?? ""
            }
            continue if (ptype == "text" || empty(payload) && ptype != "image_url" && ptype != "input_image" && ptype != "input_audio")
            if (!is_user) {
                to_log(LOG_WARNING, "/v1/chat/completions: dropping a media part on message {i} - media rides user messages\n")
                continue
            }
            let is_audio = ptype == "input_audio"
            refs |> emplace(MediaRef(key = hash_combine64(hash_combine64(hash(payload), salt), is_audio ? 1ul : 2ul),
                                     is_audio = is_audio, msg = i, text_before = at, payload = payload))
        }
    }
}

//! Send the worker a job for every ref that needs rows - all of them under `force`, else the ones this slot never
//! encoded; a job's id maps back to its ref. False with `why` when a part does not decode.
def private encode_media_refs(rid : int64; var wire : MediaWire; force : bool; var why : string&) : bool {
    for (r, ri in wire.refs, count()) {
        continue if (!empty(r.rows) || (!force && !r.want_rows && r.n_rows > 0l))
        r.want_rows = false
        var file_bytes : array<uint8>
        let decoded = (r.is_audio ? decode_base64_bytes(r.payload, file_bytes, why)
            : decode_data_uri_bytes(r.payload, file_bytes, why))
        if (!decoded) {
            delete file_bytes
            return false
        }
        let jid = next_id()
        var vjob = MediaJob(id = jid, name = g_slots[wire.slot].name, file_bytes <- file_bytes, tag = "req{rid}",
                            is_audio = r.is_audio,
                            max_audio_frames = r.is_audio ? audio_decode_frame_cap(g_slots[wire.slot].model.config.seq_len) : 0)
        g_media_jobs |> push_clone(vjob)
        delete vjob
        g_media_job_ref[jid] = MediaJobRef(wire = rid, at = ri)
        g_media_pending++
        wire.jobs_left++
    }
    return true
}

// The chat route's media boundary: every image and audio part of every user message stays in the transcript, at its
// place among the message's text. A part this slot has encoded before goes to the scheduler with no rows - the
// prefix cache holds its span on a continued chat - and only a new part parks with the media worker.
def private park_media_request(srv : FrontLink; writer : WriterRef;
                               var js : JsonValue?; msgs_v : JsonValue?) : MediaPark {
    var refs : array<MediaRef>
    collect_media_refs(msgs_v, refs)
    if (empty(refs)) {
        delete refs
        return MediaPark.none
    }
    var refusal = ""
    var n_new = 0
    for (r in refs) {
        if (r.is_audio && !g_slots[g_cur].audio_armed) {
            refusal = "model '{cur_name()}' has no audio arm — start the server with --audio-mmproj (or an --image-mmproj that carries an audio tower) for it to accept input_audio parts"
        } elif (!r.is_audio && g_slots[g_cur].vision.kind == VisionKind.none) {
            refusal = "model '{cur_name()}' has no vision arm — start the server with an image mmproj for it to accept image_url parts"
        }
        let seen = g_media_seen?[r.key] ?? MediaSeen()
        r.n_rows = seen.n_rows
        r.grid = seen.grid
        n_new += seen.n_rows == 0l ? 1 : 0
    }
    let has_new = n_new > 0
    if (empty(refusal) && length(refs) > MEDIA_SPANS_LIMIT) {
        refusal = "this request carries {length(refs)} media parts; the limit is {MEDIA_SPANS_LIMIT}"
    }
    if (!empty(refusal)) {
        delete refs
        bad_request(srv, writer, refusal)
        return MediaPark.answered
    }
    if (has_new && (!g_media_armed || g_media_ready < 1)) {
        delete refs
        service_unavailable(srv, writer, "the media worker is not ready")
        return MediaPark.answered
    }
    if (g_media_pending + n_new > MEDIA_QUEUE_LIMIT) {
        delete refs
        service_unavailable(srv, writer, "the media queue is at capacity ({MEDIA_QUEUE_LIMIT}) — retry shortly")
        return MediaPark.answered
    }
    // park at the encode boundary: the worker decodes + encodes off-thread, and drain_media_events resumes at
    // handle_chat_tail with the rows; a request of known media resumes at once, its wire kept for a lost span
    let rid = next_id()
    set_writer_keepalive_timeout(srv, writer, LLM_WRITER_KEEPALIVE_MS)
    g_media_wires[rid] <- MediaWire(writer = writer, js = js, slot = g_cur, refs <- refs)
    var wire = unsafe(g_media_wires?[rid])
    var why = ""
    if (!encode_media_refs(rid, *wire, false, why)) {
        drop_media_wire(rid, false)
        bad_request(srv, writer, why)
        return MediaPark.answered
    }
    if (wire.jobs_left > 0) {
        to_log(LOG_INFO, "[req {rid}] {wire.jobs_left} media part(s) parked for the media worker, {length(wire.refs) - wire.jobs_left} known ({g_media_pending} queued or in flight)\n")
    } else {
        to_log(LOG_INFO, "[req {rid}] {length(wire.refs)} media part(s), every one known\n")
        resume_media_request(srv, rid)
    }
    return MediaPark.parked
}

//! free a parked request's wire; `keep_js` where the caller's own defer frees the request JSON
def private drop_media_wire(rid : int64; free_js : bool) {
    return if (!key_exists(g_media_wires, rid))
    if (free_js) {
        unsafe {
            delete g_media_wires[rid].js
        }
    }
    g_media_wires[rid].js = null
    delete g_media_wires[rid]
    g_media_wires |> erase(rid)
}

//! every job of a parked request is in: render and submit it. The wire stays until the stream's first event, since a
//! span submitted with no rows may turn out lost.
def private resume_media_request(srv : FrontLink; rid : int64) {
    var wire = unsafe(g_media_wires?[rid])
    return if (wire == null)
    wire.submitted = true
    handle_chat_tail(srv, wire.writer, wire.js, wire.slot, wire.refs, rid, wire.retried)
    wire = unsafe(g_media_wires?[rid])
    if (wire != null && wire.submitted && !key_exists(g_wires, rid)) {   // answered with an error: no stream to wait on
        drop_media_wire(rid, true)
    }
}

//! A stream finished `media_lost`: the prefix cache no longer held a span the request sent no rows for. Encode every
//! part and submit again, once. False where there is nothing to retry.
def private retry_lost_media(rid : int64) : bool {
    var wire = unsafe(g_media_wires?[rid])
    return false if (wire == null || wire.retried)
    wire.retried = true
    wire.submitted = false
    var why = ""
    to_log(LOG_INFO, "[req {rid}] a span the prefix cache no longer holds: encoding the request's media\n")
    return encode_media_refs(rid, *wire, true, why) && wire.jobs_left > 0
}

// everything after the media boundary: re-derives the request fields from `js` (cheap), renders,
// and registers. Runs synchronously for a text request; an image request re-enters here from
// drain_media_events with the worker's rows.
def private handle_chat_tail(srv : FrontLink; writer : WriterRef; js : JsonValue?;   // nolint:STYLE037,STYLE038 — flat request-field derivation: one guard and one client-facing message per field
                             slot : int; var refs : array<MediaRef>; resume_rid : int64 = 0l; resumed : bool = false) {
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
    var nothink = NothinkStopGuard()
    defer() { delete nothink }
    var close : array<int64>
    defer() { delete close }
    var call_open = ""
    var call_close = ""
    var think_stream = ThinkStream()
    var wire_tool_mode = ToolMode.none
    var stable_at = 0l
    var start = 0
    while (true) {
        let render_error = render_chat_suffix(msgs_v, system, tools, max_new, start, think_set, think_val,
                                              refs, prompt, stop_ids, close, nothink, call_open, call_close,
                                              think_stream, wire_tool_mode, stable_at)
        if (!empty(render_error)) {
            bad_request(srv, writer, render_error)
            return
        }
        let required = long_length(prompt) + (truncation == "auto" ? max_new : 1l)
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
        delete nothink
        delete close
    }
    if (resume_rid != 0l && g_media_cover_probe && uncovered_media(prompt, refs)) {
        // a part this slot knows, under a prompt whose cached prefix stops short of it: the worker encodes it, and
        // the request resumes here with its rows
        var wire = unsafe(g_media_wires?[resume_rid])
        var why = ""
        if (wire != null && encode_media_refs(resume_rid, *wire, false, why) && wire.jobs_left > 0) {
            wire.submitted = false
            return
        }
    }
    var spans <- media_spans(prompt, refs)
    register_request(srv, writer, prompt, stop_ids, close, spans, params, max_new,
                     streaming, true, "chatcmpl",
                     has_tools, call_open, call_close, seed, think_stream, wire_tool_mode, resume_rid,
                     nothink, stable_at, wants_usage(js), resumed)
}

//! Marks the refs that bring no rows and whose span the slot's prefix cache does not hold under `prompt`; true when
//! there is one. A flat (unpaged) scheduler holds nothing, so every rowless ref is marked.
def private uncovered_media(prompt : array<int64>; var refs : array<MediaRef>) : bool {
    let covered = cur_sched().paged ? prefix_match_len(cur_sched().prefix, cur_sched().pool, prompt) : 0l
    var any = false
    var from = 0l
    for (r in refs) {
        let first = media_position_id(r.key, 0l)
        var at = from
        while (at < long_length(prompt) && prompt[at] != first) {
            at++
        }
        continue if (at >= long_length(prompt))
        from = at + r.n_rows
        if (empty(r.rows) && from > covered) {
            r.want_rows = true
            any = true
        }
    }
    return any
}

//! The spans of a rendered prompt: each ref's run of position ids, in prompt order, with the ref's rows moved in. A ref
//! whose message the renderer dropped (auto truncation) has no run and makes no span.
def private media_spans(prompt : array<int64>; var refs : array<MediaRef>) : array<MediaSpan> {
    var spans : array<MediaSpan>
    var from = 0l
    for (r in refs) {
        let first = media_position_id(r.key, 0l)
        var at = from
        while (at < long_length(prompt) && prompt[at] != first) {
            at++
        }
        continue if (at >= long_length(prompt))
        spans |> emplace(MediaSpan(at = at, n = r.n_rows, non_causal = !r.is_audio, grid = r.grid, rows <- r.rows))
        from = at + r.n_rows
    }
    return <- spans
}

//! whether a request asks its stream to end on the usage chunk (`stream_options.include_usage`)
def private wants_usage(js : JsonValue?) : bool => js?["stream_options"]?["include_usage"] ?? false

def private handle_completion(srv : FrontLink; req : FrontReq; writer : WriterRef) {
    if (reject_deferred_while_draining(srv, writer) || reject_while_benching(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(req.body, perr)
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
    var no_spans : array<MediaSpan>
    register_request(srv, writer, toks, none_stop, none_close, no_spans,
                     params, max_new, streaming, false, "cmpl",
                     [seed = seed, include_usage = wants_usage(js)])
}

//! the ids of the `wires` whose client vanished, each one's writer released
def private gone_wires(srv : FrontLink; wires : table<int64; auto(TT)>) : array<int64> {
    var gone : array<int64>
    for (id, w in keys(wires), values(wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            release_writer(srv, w.writer)
            gone |> push(id)
        }
    }
    return <- gone
}

//! drop the `wires` whose client vanished; the work behind one runs on and its answer finds no wire
def private reap_wires(srv : FrontLink; var wires : table<int64; auto(TT)>) {
    var gone <- gone_wires(srv, wires)
    for (id in gone) {
        delete wires[id]
        wires |> erase(id)
    }
    delete gone
}

//! drop the transcription and synthesis wires whose client vanished (a synthesis's file is still swept when it lands)
def private reap_speech_wires(srv : FrontLink) {
    reap_wires(srv, g_asr_wires)
    reap_wires(srv, g_tts_wires)
}

// Evict streams whose client vanished: is_writer_connected reads false once the connection is
// gone (the async writer ops themselves never report a dead peer). The scheduler appends the
// "evicted" finished event; flush_events below releases the writer and drops the wire.
def private reap_disconnected(srv : FrontLink) {
    for (id, w in keys(g_wires), values(g_wires)) {
        if (!is_writer_connected(srv, w.writer)) {
            evict(g_slots[w.slot].sched, id, g_events)
        }
    }
    reap_speech_wires(srv)
    var disconnected_vision <- gone_wires(srv, g_media_wires)
    for (id in disconnected_vision) {
        drop_media_wire(id, true)
    }
    delete disconnected_vision
}

// Tool-capable streaming buffers the model's native envelope until it can be normalized into an
// OpenAI tool_calls delta. Slow CPU inference can otherwise leave the socket silent beyond libhv's
// keep-alive window during prefill or generation. Emit a valid empty delta at least every 10s;
// scheduler steps may be long, so this is intentionally time-based rather than token-count-based.
def private flush_tool_keepalives(srv : FrontLink) {
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
def private flush_events(srv : FrontLink) {   // nolint:STYLE037,STYLE038 — one arm per (event kind x wire flavor); the arms share the wire's UTF-8 hold and reply buffer
    for (e in g_events) {
        // interior pointer into g_wires — nothing inserts into the table between here and the erase
        var w = unsafe(g_wires?[e.stream])
        if (w == null) {
            continue
        }
        g_cur = w.slot   // response builders read the wire's slot (cur_name / cur_model)
        if (e.kind != SchedEventKind.piece && e.finish == "media_lost" && retry_lost_media(e.stream)) {
            delete g_wires[e.stream]   // the retry registers the stream's wire again, under the same id
            g_wires |> erase(e.stream)
            continue
        }
        drop_media_wire(e.stream, true)   // the stream is under way: its parked request has nothing left to retry
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
                        send_chat_halves(srv, *w, rsn, cnt)
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
                send_chat_halves(srv, *w, rsn, cnt)
            }
            if (w.is_chat) {
                send_chat_chunk(srv, w.writer, w.id, w.created, "", "", finish)
            } else {
                send_compl_chunk(srv, w.writer, w.id, w.created, "", finish)
            }
            if (w.include_usage) {
                send_usage_chunk(srv, w.writer, w.id, w.created, w.is_chat, e)
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
            out.usage = usage_of(e)
            set_writer_keepalive_timeout(srv, w.writer, DEFAULT_WRITER_KEEPALIVE_MS)
            respond(srv, w.writer, int(http_status.OK), "application/json", sprint_json(out, false))
            delete out
        } else {
            var out = OaiComplResponse(id = w.id, _object = "text_completion", created = w.created, model = cur_name())
            out.choices |> push(OaiComplChoice(index = 0, text = join(w.reply_parts, ""), finish_reason = e.finish))
            out.usage = usage_of(e)
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
    file : string             // base name of the source GGUF
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
    vision : bool             // a vision arm is attached: this slot's chat route takes image_url parts
    audio : bool              // an audio arm is attached: this slot's chat route takes input_audio parts
    thinking_default : bool   // this family's own default when a request sends no enable_thinking
    last_used_s : float       // seconds since this slot last admitted a request (-1 = never)
    served : string           //!< how this slot is served, in plain words: where the weights sit and where the streams' caches do
    served_note : string      //!< why it is not served better; "" when nothing holds the slot back
    device_kv : bool          //!< the streams' K/V caches live on the GPU, a region each
    sw_count : int64          // P2 gpu-switch telemetry
    sw_avg_ms : float
}

struct private OaiCpuPass {
    reason : string
    words : string   //!< the reason as the control page prints it
    count : int64
}

struct private OaiStatsResponse {
    model : string
    active_model : string
    models : array<OaiSlotStats>
    gpu_cpu_passes : array<OaiCpuPass>   //!< calls the armed GPU path handed back to the CPU rails since the model armed, by reason; empty = the device served every call
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
    tts_done_jobs : int64     // cumulative served syntheses
    tts_audio_s : float       // cumulative speech seconds synthesized
    vision : bool             // the DEFAULT slot's vision arm — the slot a model-less page request lands on
    audio : bool              // the DEFAULT slot's audio arm (native audio through the serving slot)
    thinking_default : bool   // the DEFAULT slot's family thinking default (the page's checkbox seed)
    media_pending : int       // media encode jobs queued or in flight (0 = idle)
    media_rows : int64        // media (image span) rows admitted to prefill since boot
    mrope_streams : int64     // admitted streams whose media rode the qwen mrope grid walk
}

// budget only when the device already EXISTS (tier.device is "" until first upload creates
// it) — the budget hook device-inits on first call, and `supported` can read true while the
// tier is merely armed, so it must not gate this
def private gpu_weight_budget_bytes() : int64 {
    return !empty(gpu_tier_status().device) ? moe_gpu_weight_budget() : 0l
}

// The `tts` member of /v1/stats, comma-prefixed, or "" when the boot configured no speech model.
// A boot whose worker could not load its model still carries the block: `ready` reads false and
// `error` names the failure, which is what the page's speech card draws instead of the studio.
// `voices`, `sample_rate` and `lane` are the worker's own answers about the model it loaded.
def private tts_stats_member() : string {
    var k = tts_default()
    if (k < 0) {   // none armed: a model whose worker could not load still shows why
        for (i in range(length(g_tts))) {
            if (!empty(g_tts[i].error)) {
                k = i
                break
            }
        }
    }
    return "" if (k < 0)
    assume m = g_tts[k]
    let voices = build_string() $(w) {
        for (i in range(length(m.voices))) {
            if (i > 0) {
                w |> write(",")
            }
            w |> write(json_str(m.voices[i]))
        }
    }
    // the other armed models, when there are several: the block above is the default one's
    var armed <- [for (t in g_tts); json_str(t.id); where t.armed]
    let models = length(armed) > 1 ? ",\"models\":[{join(armed, ",")}]" : ""
    delete armed
    // no worker answer yet (loading, or the load failed): the lane is the one the boot asked for
    let lane = empty(m.lane_served) ? m.lane : m.lane_served
    let failed = empty(m.error) ? "" : ",\"error\":{json_str(m.error)}"
    return ",\"tts\":\{\"id\":{json_str(m.id)},\"ready\":{m.ready > 0},\"pending\":{g_tts_pending},\"done_jobs\":{g_tts_done_jobs},\"audio_s\":{g_tts_audio_s},\"voices\":[{voices}],\"sample_rate\":{m.sample_rate},\"lane\":{json_str(lane)},\"cloning\":{m.cloning},\"speed\":{m.speed},\"lang\":{json_str(m.lang)}{models}{failed}}"
}

//! the stats answer's `asr` member: each served ASR model with its workers and where they decode; "" with none
def private asr_stats_member() : string {
    return "" if (empty(g_asr))
    var models <- [for (a in g_asr); "\{\"id\":{json_str(a.id)},\"workers\":{a.workers},\"ready\":{a.ready},\"decoder\":{json_str(a.decoder)}}"]
    let member = ",\"asr\":\{\"models\":[{join(models, ",")}]}"
    delete models
    return member
}

// splice a member into a compact JSON object body: sprint_json/write_json_compact both close on
// the final `}`, and the stats shapes are objects with at least one member
def private with_json_member(body : string; member : string) : string {
    if (empty(member)) {
        return body
    }
    return "{slice(body, 0, length(body) - 1)}{member}}"
}

def private handle_stats(var resp : Reply) : http_status {
    if (empty(g_slots)) {
        // no LLM slot: the minimum shape control.html's apply() reads (it gates on `setup`).
        // `setup` is the MODE, not the slot count — a speech-only server serves the speech route
        // and must not draw the page's pick-a-model surface.
        return resp |> JSON(
            "\{\"setup\":{g_setup_mode},\"model\":\"(no model)\",\"active_model\":\"\",\"ctx\":0,\"uptime_s\":{g_started_at > 0l ? get_time_nsec(g_started_at) / 1_000_000_000l : 0l},\"draining\":{g_shutdown_requested},\"active\":0,\"queued\":0,\"peak_active\":0,\"admitted\":0,\"finished\":0,\"evicted\":0,\"decode_steps\":0,\"gen_tokens\":0,\"avg_batch\":0,\"cached_tokens\":0,\"prefix_pages\":0,\"max_streams\":0,\"chunk_tokens\":0,\"asr_workers\":{[for (a in g_asr); a.workers] |> sum()},\"asr_ready\":{[for (a in g_asr); a.ready] |> sum()},\"asr_active\":{g_asr_active},\"asr_pending\":{g_asr_pending},\"mtp_drafted\":0,\"mtp_accepted\":0,\"prefill_tokens\":0,\"ttft_avg_ms\":0,\"ttft_last_ms\":0,\"weights_bytes\":0,\"kv_bytes\":0,\"gpu_vram_bytes\":0,\"gpu_budget_bytes\":0,\"hardware\":{json_str(g_hardware_line)},\"asr_done_jobs\":{g_asr_done_jobs},\"asr_audio_s\":{g_asr_audio_s},\"tts_done_jobs\":{g_tts_done_jobs},\"tts_audio_s\":{g_tts_audio_s},\"vision\":false,\"audio\":false,\"models\":[]{asr_stats_member()}{tts_stats_member()}}",
            http_status.OK)
    }
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
        asr_workers = [for (a in g_asr); a.workers] |> sum(), asr_ready = [for (a in g_asr); a.ready] |> sum(),
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
        gpu_budget_bytes = gpu_weight_budget_bytes(),
        hardware = g_hardware_line,
        asr_done_jobs = g_asr_done_jobs, asr_audio_s = g_asr_audio_s,
        tts_done_jobs = g_tts_done_jobs, tts_audio_s = g_tts_audio_s,
        // DEFAULT slot, not g_active: these two gate the page's request UI, and a page request
        // carries no model field, so it lands on the default slot - the UI must show ITS truth
        vision = g_slots[g_default_slot].vision.kind != VisionKind.none,
        audio = g_slots[g_default_slot].audio_armed,
        thinking_default = g_slots[g_default_slot].think_default,
        media_pending = g_media_pending, media_rows = g_media_rows,
        mrope_streams = g_slots[g_active].sched.mrope_streams)
    out.gpu_cpu_passes <- [for (p in gpu_cpu_passes()); OaiCpuPass(reason = p.reason, words = p.words, count = p.count)]
    out.models |> reserve(length(g_slots))
    for (i in range(length(g_slots))) {
        var s & = unsafe(g_slots[i])
        let holds_gpu = i == g_gpu_owner
        let device_kv = holds_gpu ? slot_wants_device_kv(s) : s.sched.device_kv
        out.models |> push(OaiSlotStats(
            name = s.name, file = base_name(s.path), is_default = s.is_default, is_active = i == g_active,
            holds_gpu = holds_gpu,
            backend = s.opts.backend, backend_effective = s.backend_effective,
            flavor = slot_flavor(s), vision = s.vision.kind != VisionKind.none,
            audio = s.audio_armed,
            thinking_default = s.think_default,
            ctx = s.model.config.seq_len,
            active = n_active(s.sched), queued = n_queued(s.sched),
            admitted = s.sched.n_admitted, finished = s.sched.n_finished,
            gen_tokens = s.sched.gen_tokens, cached_tokens = s.sched.cached_tokens,
            prefill_tokens = s.sched.prefill_tokens,
            prefix_pages = prefix_held_groups(s.sched.prefix),
            weights_bytes = model_weights_bytes(s.model),
            kv_bytes = kv_pool_bytes(s.sched.pool),
            served = slot_served(s, device_kv, holds_gpu), served_note = slot_served_note(s, device_kv, holds_gpu), device_kv = device_kv,
            last_used_s = s.last_used > 0l ? age_seconds(s.last_used) : -1.0,
            sw_count = s.sw_count,
            sw_avg_ms = s.sw_count > 0l ? float(s.sw_us / s.sw_count) / 1000.0 : 0.0))
    }
    let body = sprint_json(out, false)
    delete out
    return resp |> JSON(with_json_member(body, "{asr_stats_member()}{tts_stats_member()}"), http_status.OK)
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
    model : string
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

def private handle_streams(var resp : Reply) : http_status {
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
                n_prompt = long_length(st.prompt), n_cached = st.n_cached,
                n_prefilled = st.n_prefilled, n_gen = st.n_gen, max_new = st.max_new,
                ttft_ms = float(st.ttft_us) / 1000.0,
                prompt_head = st.prompt_head, gen_tail = utf8_trim_partial_end(string(st.gen_tail_bytes))))
        }
        for (q in sch.queue) {
            out.streams |> push(OaiStreamInfo(
                id = q.id, model = mname, state = "queued",
                age_s = q.t_submit != 0l ? age_seconds(q.t_submit) : 0.0,
                n_prompt = long_length(q.prompt), max_new = q.max_new))
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
            id = j.id, model = j.model, state = j.state, audio_s = j.audio_s, wall_ms = j.wall_ms, rtf = j.rtf,
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
    "tts" => "s", "tts_lane" => "s", "tts_voices_dir" => "s",
    "ctx" => "i", "max_tokens" => "i", "streams" => "i", "threads" => "i", "team_dispatch" => "s",
    "affinity" => "i", "chunk" => "i", "chunk_idle" => "i", "page_rows" => "i", "prefix" => "i", "flat" => "b",
    "mtp" => "b", "rope_scaling" => "s", "rope_scale" => "f", "yarn_orig_ctx" => "i",
    "lcpp_bin" => "s", "models_dir" => "s", "image_mmproj" => "s", "audio_mmproj" => "s", "models" => "a",
    "exchange_url" => "s", "exchange_accept" => "s", "exchange_submit" => "s"
}

let private MODEL_KEY_KINDS <- {
    "name" => "s", "path" => "s", "backend" => "s", "ctx" => "i", "quant" => "s",
    "kv_dtype" => "s", "streams" => "i", "chunk" => "i", "chunk_idle" => "i", "page_rows" => "i", "prefix" => "i",
    "mtp" => "b", "default" => "b", "image_mmproj" => "s", "audio_mmproj" => "s",
    "rope_scaling" => "s", "rope_scale" => "f", "yarn_orig_ctx" => "i"
}

//! A posted value coerced to its schema kind - a fresh node, or null when the kind does not fit:
//! "s"tring, "i"nteger (a whole float too), "f"loat (an integer too), "b"ool
def private json_of_kind(kind : string; v : JsonValue?) : JsonValue? {
    if (v == null) {
        return null
    }
    if (kind == "s" && (v.value is _string)) {
        return JV(v.value as _string)
    }
    if (kind == "i" && (v.value is _longint)) {
        return JV(v.value as _longint)
    }
    if (kind == "i" && (v.value is _number) && (v.value as _number) == floor(v.value as _number)) {
        return JV(int64(v.value as _number))
    }
    if (kind == "b" && (v.value is _bool)) {
        return JV(v.value as _bool)
    }
    if (kind == "f" && (v.value is _number)) {
        return JV(v.value as _number)
    }
    if (kind == "f" && (v.value is _longint)) {
        return JV(double(v.value as _longint))
    }
    return null
}

let private ASR_KEY_KINDS <- { "id" => "s", "path" => "s", "mmproj" => "s", "workers" => "i" }

let private TTS_KEY_KINDS <- { "id" => "s", "path" => "s", "lane" => "s", "voices_dir" => "s" }

// Validate + sanitize one array-of-tables key of a POST /config body: objects only, per-key kinds, a non-empty
// path per entry, unique names - an entry's name is its `name_key` value, else its file (the stem for a speech
// model, the base name for an LLM). Returns the sanitized array (JV) or null with verr set.
def private validate_table_array(v : JsonValue?; what : string; kinds : table<string; string>; name_key : string; var verr : string&) : JsonValue? {
    if (v == null || !(v.value is _array)) {
        verr = "'{what}' must be an array of entries"
        return null
    }
    var arr : array<JsonValue?>
    var names : table<string>
    for (e in v.value as _array) {
        if (e == null || !(e.value is _object)) {
            verr = "'{what}' entries must be objects"
            break
        }
        var ent : table<string; JsonValue?>
        for (mk, mv in keys(e.value as _object), values(e.value as _object)) {
            let kind = kinds?[mk] ?? ""
            if (empty(kind)) {
                verr = "unknown {what} key '{mk}'"
                break
            }
            var jv = json_of_kind(kind, mv)
            if (jv == null) {
                verr = "{what} key '{mk}' must be a {kind_name(kind)}"
                break
            }
            ent |> insert(mk, jv)
        }
        let path = e?["path"] ?? ""
        let given = e?[name_key] ?? ""
        let name = !empty(given) ? given : (name_key == "id" ? stem(base_name(path)) : base_name(path))
        if (empty(verr) && empty(path)) {
            verr = "every {what} entry needs a non-empty 'path'"
        }
        if (empty(verr) && key_exists(names, name)) {
            verr = "duplicate {what} {name_key} '{name}'"
        }
        if (!empty(verr)) {
            var junk = JV(ent)
            unsafe {
                delete junk
            }
            break
        }
        names |> insert(name)
        arr |> push(JV(ent))
    }
    if (!empty(verr)) {
        var junk = JV(arr)
        unsafe {
            delete junk
        }
        return null
    }
    return JV(arr)
}

// the "models" array's own rules, past the shared ones: backend in {auto, cpu, gpu}, at most one default = true
def private models_array_error(v : JsonValue?) : string {
    var ndefault = 0
    for (e in v.value as _array) {
        let backend = e?["backend"] ?? "auto"
        return "models backend '{backend}' is not auto | cpu | gpu" if (backend != "auto" && backend != "cpu" && backend != "gpu")
        if (e?["default"] ?? false) {
            ndefault ++
        }
    }
    return ndefault > 1 ? "more than one models entry marked default = true" : ""
}

// the sanitized value of one array-kind config key (`models`, or a speech roster under `asr` / `tts`), null with verr set
def private validate_config_array(key : string; v : JsonValue?; var verr : string&) : JsonValue? {
    if (key == "asr") {
        return validate_table_array(v, key, ASR_KEY_KINDS, "id", verr)
    }
    if (key == "tts") {
        return validate_table_array(v, key, TTS_KEY_KINDS, "id", verr)
    }
    var sanitized = validate_table_array(v, key, MODEL_KEY_KINDS, "name", verr)
    return sanitized if (sanitized == null)
    verr = models_array_error(v)
    if (!empty(verr)) {
        unsafe {
            delete sanitized
        }
        return null
    }
    return sanitized
}

// POST /vad: the page's waveform-overlay preview — silero speech spans over an uploaded clip.
// Buffered + in-handler: silero is a tiny RNN, preview clips are capped, no worker needed.
var private g_vad_loaded = false
var private g_vad = VadModel()
let private VAD_PREVIEW_CAP_S = 120

def private handle_vad(req : FrontReq; var resp : Reply) : http_status {
    if (bench_hold_active()) {   // Silero runs in-handler, on the cores the timed reps own
        drop_upload(req)
        return resp_error(resp, http_status.SERVICE_UNAVAILABLE, ibench_hold_text())
    }
    if (!g_vad_loaded) {
        let path = path_join(get_das_root(), "modules/dasLLAMA/models/silero_vad.bin")
        if (!stat(path).is_valid) {
            drop_upload(req)
            return resp |> JSON(
                error_body("silero_vad.bin not found ({path}) — the VAD preview needs a source tree", "server_error"),
                http_status.NOT_IMPLEMENTED)
        }
        g_vad <- load_vad_model(path)   // nolint:PERF030 — one-shot lazy load; g_vad_loaded proves the target is still the empty default
        g_vad_loaded = true
    }
    if (empty(req.file_path)) {   // the front saves the form's 'file' upload; none came
        return resp |> JSON(
            error_body("multipart form must include a 'file' audio upload", "invalid_request_error"),
            http_status.BAD_REQUEST)
    }
    let temppath = req.file_path
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
    if (kind == "f") {
        return "number"
    }
    return kind == "i" ? "whole number" : "bool"
}

// GET /config: the launcher-registered surface (effective values + per-key provenance) plus
// live status — model files beside the served one, the active rail, and the GPU tier badge.
// ggufs only: a .dlim is a baked ARTIFACT of a gguf (the bake panel owns them) - one in
// the model picker sets a model path that cannot be served
def private push_picker_ggufs(scan_dir : string; var entries : array<JsonValue?>&) {
    if (empty(scan_dir)) {
        return
    }
    dir(scan_dir) $(fname) {
        if (ends_with(to_lower(fname), ".gguf")) {
            let full = path_join(scan_dir, fname)
            let st = stat(full)
            entries |> push(JV((name = fname, size = int64(st.size), kind = "gguf", path = full)))
        }
    }
}

def private handle_config_get(var resp : Reply) : http_status {
    var entries : array<JsonValue?>
    let mdir = dir_name(g_config_model_path)
    push_picker_ggufs(mdir, entries)
    if (g_catalog_dir != mdir) {
        // catalog downloads stay reachable from the picker when the served model lives elsewhere
        push_picker_ggufs(g_catalog_dir, entries)
    }
    let tier = gpu_tier_status()
    var extras <- {
        "model_files" => JV(entries),
        "active_model" => JV(base_name(g_config_model_path)),
        "dlim_rail" => JV(!empty(g_slots) && g_slots[g_default_slot].model.image_map != null),
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
def private handle_config_save(req : FrontReq; var resp : Reply) : http_status {
    var jerr = ""
    var body = read_json(req.body, jerr)
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
        // a speech key is one path, or a roster of tables ([[asr]] / [[tts]])
        if (kind == "a" || ((k == "asr" || k == "tts") && (v.value is _array))) {
            var sanitized = validate_config_array(k, v, verr)
            break if (!empty(verr))
            out_tab |> insert(k, sanitized)
            continue
        }
        var jv = json_of_kind(kind, v)
        if (jv == null) {
            verr = "config key '{k}' must be a {kind_name(kind)}"
            break
        }
        out_tab |> insert(k, jv)
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
    if (!write_config(g_config_save_path, toml)) {
        return resp |> JSON(error_body("could not write '{g_config_save_path}'", "server_error"), http_status.INTERNAL_SERVER_ERROR)
    }
    to_log(LOG_INFO, "dasllama-server: authoritative config saved to {g_config_save_path} ({length(toml)} bytes)\n")
    return resp |> JSON("\{\"saved\":{json_str(g_config_save_path)},\"restart_required\":true}", http_status.OK)
}

// ===== the exchange surface (GET /exchange + the apply/submit/retune levers) =====

def private exchange_absent(var resp : Reply) : http_status {   // nolint:LINT022,LINT019 — reached only from the no-framework static_if arm, which a build with dasLLVM drops; on a host without dasLLVM the arm compiles and the rule is silent
    return resp |> JSON(error_body("the sidecar exchange is not in this build (no tune framework)", "not_found"), http_status.NOT_FOUND)
}

// The exchange control routes are an operator surface reached through the loopback control
// page. The matching route does a blocking network call on the tick thread; apply/retune are
// destructive; submit publishes. Gate them to the loopback peer so a LAN client (the inference
// API deliberately binds every interface) cannot stall generation or trigger those actions.
def private is_loopback_req(req : FrontReq) : bool => req.loopback

//! the front's own read of a request: loopback peer, loopback Host, and no cross-site Origin
def private front_is_loopback(req : HttpRequest?) : bool {
    let ip = client_ip(req)
    // an unknown/empty peer counts as remote (fail closed, like the ladder's is_loopback_peer):
    // this service binds every interface, so a LAN caller must not slip through on an unresolved peer
    let loopback = ip == "127.0.0.1" || ip == "::1" || ip == "::ffff:127.0.0.1"
    return loopback && is_loopback_host(get_header(req, "Host")) && same_origin_or_headerless(req)
}

def private authority_host(authority : string) : string {
    // the host of an authority (host / host:port / [v6]:port), lowercased
    if (authority |> starts_with("[")) {
        let close = find(authority, "]")
        return close > 0 ? to_lower(slice(authority, 1, close)) : to_lower(authority)
    }
    let colon = find(authority, ":")
    return to_lower(colon >= 0 ? slice(authority, 0, colon) : authority)
}

def private is_loopback_host(authority : string) : bool {
    let host = authority_host(authority)
    // same set as the peer check above, incl. the IPv4-mapped IPv6 loopback
    return host == "127.0.0.1" || host == "localhost" || host == "::1" || host == "::ffff:127.0.0.1"
}

// CSRF guard for the operator routes: allow_cors() is global and the peer is 127.0.0.1 for ANY
// local browser, so a page the operator merely visits could POST to the destructive routes. A
// same-origin request (the control page is served from here) or a header-less CLI client passes;
// the loopback-Host requirement above defeats DNS rebinding, which Origin==Host alone would not
// (a rebound name owns both).
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
def private deny_operator(var resp : Reply) : http_status {
    return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
}

[arch(at = "README.md#exchange-policy")]
def private handle_exchange_get(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        // the status carries the local sidecar path (username in it) — operator-only, like the rest
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    static_if (typeinfo module_exists(dasllama_exchange)) {
        return resp |> JSON(exchange_status_json(exchange_policy_from_config(g_config_save_path)))
    } else {
        return exchange_absent(resp)
    }
}

def private handle_exchange_matches(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    // a live exchange lookup — seconds on the tick thread; the page requests it explicitly
    static_if (typeinfo module_exists(dasllama_exchange)) {
        return resp |> JSON(exchange_matches_json(exchange_policy_from_config(g_config_save_path)))
    } else {
        return exchange_absent(resp)
    }
}

// A fat release bakes one kernel clone per CPU class and never tunes, so the exchange has nothing
// to apply, share or redo on it; the read-only status and matches stay, the levers answer this.
def private fat_exe_exchange_refusal(var resp : Reply) : http_status {   // nolint:LINT012,LINT022,LINT019 — resp and every caller sit in llvm_tune static_if halves; a host without dasLLVM compiles the bare-OK arm alone
    static_if (typeinfo module_exists(llvm_tune)) {
        if (!tune_fat_built()) return http_status.OK
        return resp |> JSON(error_body("this is a fat release: the kernels are baked per CPU class, so there is no tune of this box to share, apply or redo - the SDK's daslang -jit run tunes the box", "invalid_request_error"), http_status.BAD_REQUEST)
    } else {
        return http_status.OK
    }
}

def private handle_exchange_apply(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    static_if (typeinfo module_exists(dasllama_exchange)) {
        let refused = fat_exe_exchange_refusal(resp)
        if (refused != http_status.OK) return refused
        var jerr = ""
        var js = read_json(req.body, jerr)
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
        let err = exchange_fetch_and_apply(exchange_policy_from_config(g_config_save_path).url, sha, tune_manifest_path(), box)
        if (!empty(err)) {
            return resp |> JSON(error_body(err, "invalid_request_error"), http_status.BAD_REQUEST)
        }
        // explicitly fetching a sidecar IS engaging the exchange - record the consent it expresses
        exchange_record_consent(true)
        to_log(LOG_INFO, "dasllama-server: exchange sidecar {sha} applied via the control page; draining for restart\n")
        g_shutdown_requested = true
        g_restart_requested = true
        return resp |> JSON("\{\"ok\":true,\"restarting\":true}", http_status.OK)
    } else {
        return exchange_absent(resp)
    }
}

def private handle_exchange_submit(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    static_if (typeinfo module_exists(dasllama_exchange)) {
        let refused = fat_exe_exchange_refusal(resp)
        if (refused != http_status.OK) return refused
        let err = exchange_submit_current(exchange_policy_from_config(g_config_save_path))
        if (!empty(err)) {
            return resp |> JSON(error_body(err, "invalid_request_error"), http_status.BAD_REQUEST)
        }
        to_log(LOG_INFO, "dasllama-server: this box's tune submitted to the exchange\n")
        return resp |> JSON("\{\"ok\":true}", http_status.OK)
    } else {
        return exchange_absent(resp)
    }
}

def private handle_exchange_consent(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    static_if (typeinfo module_exists(dasllama_exchange)) {
        let refused = fat_exe_exchange_refusal(resp)
        if (refused != http_status.OK) return refused
        var jerr = ""
        var js = read_json(req.body, jerr)
        defer() {
            unsafe {
                delete js
            }
        }
        if (js == null) {
            return resp |> JSON(error_body("invalid JSON: {jerr}", "invalid_request_error"), http_status.BAD_REQUEST)
        }
        let av = js?["accept"]
        if (av == null || !(av.value is _bool)) {
            return resp |> JSON(error_body("body must carry accept:true|false", "invalid_request_error"), http_status.BAD_REQUEST)
        }
        let accepted = av.value as _bool
        if (!exchange_record_consent(accepted)) {
            return resp |> JSON(error_body("could not write the consent file", "server_error"), http_status.INTERNAL_SERVER_ERROR)
        }
        let manifest = tune_manifest_path()
        if (accepted && (!stat(manifest).is_valid || tune_sidecar_stale(manifest))) {
            // untuned (or stale) box just enabled the exchange — restart so the boot resolver
            // runs the lookup; a tuned box records the choice and keeps serving
            to_log(LOG_INFO, "dasllama-server: exchange enabled via the control page; draining for restart to run the lookup\n")
            g_shutdown_requested = true
            g_restart_requested = true
            return resp |> JSON("\{\"ok\":true,\"accepted\":true,\"restarting\":true}", http_status.OK)
        }
        to_log(LOG_INFO, "dasllama-server: exchange consent recorded ({accepted ? "accepted" : "declined"}) via the control page\n")
        return resp |> JSON("\{\"ok\":true,\"accepted\":{accepted}}", http_status.OK)
    } else {
        return exchange_absent(resp)
    }
}

def private handle_exchange_retune(req : FrontReq; var resp : Reply) : http_status {
    if (!is_loopback_req(req)) {
        return resp |> JSON(error_body("loopback-only (operator surface)", "forbidden"), http_status.FORBIDDEN)
    }
    static_if (typeinfo module_exists(dasllama_exchange)) {
        let refused = fat_exe_exchange_refusal(resp)
        if (refused != http_status.OK) return refused
        let err = exchange_request_retune()
        if (!empty(err)) {
            // the marker write failed and the sidecar is untouched — do NOT drain/restart
            return resp |> JSON(error_body(err, "server_error"), http_status.INTERNAL_SERVER_ERROR)
        }
        to_log(LOG_INFO, "dasllama-server: re-tune armed (sidecar removed, exchange skipped once); draining for restart\n")
        g_shutdown_requested = true
        g_restart_requested = true
        return resp |> JSON("\{\"ok\":true,\"restarting\":true,\"tune\":true}", http_status.OK)
    } else {
        return exchange_absent(resp)
    }
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
    let language = job.language   // empty = the family's own default, which the engine picks
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

//! Configure the worker context without changing the shared queue's dispatch policy.
def configure_media_worker_dispatch(team_dispatch : bool) {
    set_jobque_fork_pool(true, true)
    set_jobque_thread_team_mode(team_dispatch)
}

def private asr_worker_main(
                            worker_index : int;
                            model_path, mmproj_path : string;
                            team_dispatch : bool;
                            metal : MetalMode;
                            var jobs : Channel?&;
                            var events : Channel?&
                            ) {
    this_context().name := "dasllama_asr_{worker_index}"
    configure_media_worker_dispatch(team_dispatch)
    // a two-file model's decoder is a Model session and takes the engine's Metal mode; a one-file model (whisper,
    // parakeet) decodes on hand-written CPU loops over planar weights, which the mode's portable pin would slow
    if (!empty(mmproj_path)) {
        set_metal_mode(metal)
    }
    allow_cpu_prefill()   // a decline lands the decoder's prefill on the CPU arm: served, not a profiling slip
    var model = AsrModel()
    var load_failure = ""
    let load_failed = catch_worker_panic(load_failure) {
        model <- empty(mmproj_path) ? load_asr_model(model_path) : load_asr_model(model_path, mmproj_path)   // nolint:PERF030 - target is the fresh empty decl above; the load runs inside the catch block, so it cannot fold into the declaration
    }
    if (load_failed) {   // an escaping panic here would kill only this thread and hang the ready-wait
        to_log(LOG_ERROR, "dasllama-server: ASR worker {worker_index} could not load {model_path}: {load_failure}\n")
        emit_asr_event(events, AsrEvent(kind = AsrEventKind.failed, body = load_failure))
        jobs |> release
        events |> release
        return
    }
    emit_asr_event(events, AsrEvent(kind = AsrEventKind.ready, body = model.dec.metal_blob ? "metal" : "cpu"))
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
                                srv : FrontLink;
                                req : FrontReq;
                                writer : WriterRef;
                                translate : bool
                                ) {
    if (reject_deferred_while_draining(srv, writer) || reject_while_benching(srv, writer)) {
        drop_upload(req)
        return
    }
    let k = asr_slot_of(req.form_model)
    if (k < 0 && !empty(g_asr)) {
        respond(srv, writer, int(http_status.NOT_FOUND), "application/json",
            error_body("model '{req.form_model}' is not a served ASR model — GET /v1/models lists the ids", "invalid_request_error"))
        drop_upload(req)
        return
    }
    if (k < 0 || g_asr[k].ready < g_asr[k].workers) {
        service_unavailable(srv, writer, "ASR workers are not ready")
        drop_upload(req)
        return
    }
    if (g_asr_pending >= ASR_QUEUE_LIMIT) {
        service_unavailable(srv, writer, "the ASR queue is at capacity ({ASR_QUEUE_LIMIT}) — retry shortly")
        drop_upload(req)
        return
    }
    if (empty(req.file_path)) {   // the front saves the form's 'file' upload; none came
        bad_request(srv, writer, "multipart form must include a 'file' audio upload")
        return
    }
    set_writer_keepalive_timeout(srv, writer, ASR_WRITER_KEEPALIVE_MS)
    let rid = next_id()
    let temppath = req.file_path
    let fmt = req.form_format
    let lang = req.form_language   // empty = the worker's default for its model
    g_asr_wires[rid] <- AsrWire(writer = writer, model = g_asr[k].id)
    g_asr_pending++
    g_asr_jobs[k] |> push_clone(AsrJob(
        id = rid, temp_path = temppath, language = lang, response_format = fmt,
        translate = translate))
    to_log(LOG_INFO, "[req {rid}] asr: queued on {g_asr[k].id} (pending={g_asr_pending}, active={g_asr_active})\n")
}

//! Start every ASR model's workers, a model at a time, and wait for each to load: a worker's ready event names no
//! model, so the models take turns. A worker that cannot load panics the boot, as a missing model would.
def private start_asr_worker_threads() {
    var index = 0
    var failure = ""   // raised past the loop: a panic inside it would leave both arrays locked for whoever catches it
    g_asr_jobs |> resize(length(g_asr))
    for (a, jobs in g_asr, g_asr_jobs) {
        break if (!empty(failure))
        a.ready = 0
        jobs = worker_channel()
        to_log(LOG_INFO, "dasllama-server: starting {a.workers} asynchronous ASR worker(s) for {a.id}\n")
        for (_w in range(a.workers)) {
            let worker_index = index
            let model_path = a.path
            let mmproj_path = a.mmproj
            let team_dispatch = g_asr_team_dispatch
            let metal = g_speech_metal
            var jobs_capture = jobs
            var events_capture = g_asr_events
            new_thread() <| @capture(= worker_index, = model_path, = mmproj_path, = team_dispatch, = metal,
                                    = jobs_capture, = events_capture) {
                asr_worker_main(
                    worker_index, model_path, mmproj_path, team_dispatch, metal, jobs_capture, events_capture)
            }
            index++
        }
        var load_failure = ""
        while (a.ready < a.workers && empty(load_failure)) {
            g_asr_events |> pop_with_timeout_clone(1000) $(event : AsrEvent#) {
                if (event.kind == AsrEventKind.ready) {
                    a.ready++
                    a.decoder = clone_string(event.body)
                    to_log(LOG_INFO, "dasllama-server: ASR worker ready ({a.id}: {a.ready}/{a.workers}, decoder on {a.decoder})\n")
                } elif (event.kind == AsrEventKind.failed) {
                    load_failure = clone_string(event.body)
                }
            }
        }
        if (!empty(load_failure)) {
            failure = "dasllama-server: an ASR worker failed to load {a.path}: {load_failure}"
        }
    }
    if (!empty(failure)) {
        panic(failure)
    }
}

def private drain_asr_events(srv : FrontLink) {
    var draining = true
    while (draining) {
        draining = g_asr_events |> try_pop_clone() $(event : AsrEvent#) {
            if (event.kind == AsrEventKind.started) {
                g_asr_active++
                g_asr_recent |> push(AsrJobInfo(id = event.id, model = g_asr_wires?[event.id]?.model ?? "", state = "running", started_at = ref_time_ticks()))
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
    var asked = 0
    for (a, jobs in g_asr, g_asr_jobs) {
        continue if (jobs == null)
        for (_index in range(a.ready)) {   // the workers that loaded: one that failed to has left already
            jobs |> push_clone(AsrJob(stop = true))
            asked++
        }
    }
    let stopped = await_stopped(g_asr_events, asked, WORKER_STOP_WAIT_NS) $(event : AsrEvent#) => event.kind == AsrEventKind.stopped
    if (stopped < asked) {
        to_log(LOG_WARNING, "dasllama-server: {asked - stopped} ASR worker(s) did not answer the stop within {WORKER_STOP_WAIT_NS / 1_000_000_000l} s - a dead worker must not hang the teardown\n")
    }
    for (a in g_asr) {
        a.ready = 0
    }
}

// ===== /v1/audio/speech =====
// One worker thread owns the TTS model in its own context (the ASR shape); a request is a job,
// the answer is a temp file the writer rail serves binary-safe, then reaps.

enum private TtsEventKind {
    ready
    completed
    stopped
    failed       // the worker could not load its model: `body` carries the panic text
}

enum private TtsJobKind {
    speak        // zero value — the channel pops zeroed clones
    phonemes     // the front end alone: normalize, chunk, phonemize. No synthesis, no temp file
    stop         // worker exits
}

struct private TtsJob {
    kind : TtsJobKind
    id : int64
    text : string
    voice : string
    speed : float
    format : string      // wav | pcm
}

struct private TtsEvent {
    kind : TtsEventKind
    slot : int           //! the TTS model's slot: the ready and failed events name which model answered
    id : int64
    status : int
    content_type : string
    body : string        // the error body on a failure, the phonemes document on that route's success
    file_path : string   // the served audio on a synthesis — empty on every body-carrying answer
    audio_s : float
    // the ready event's payload: what the loaded model can say, and the lane it was loaded on.
    // The worker owns the model, so this is the only crossing where the main context learns them.
    voices : array<string>
    sample_rate : int
    lane : string
    cloning : bool       // the model clones a voice from a clip (the Pocket family)
    speed : bool         // a speed scales its durations; a model without one refuses any speed but 1.0
    lang : string        // the first language caps declares - the one a one-language model reads everything in
}

struct private TtsWire {
    @do_not_delete writer : WriterRef
}

struct private TtsServedFile {
    path : string
    at : int64
}

var g_tts_events : Channel?             //! every TTS worker's events; each names its model's slot
var g_tts_wires : table<int64; TtsWire>
var g_tts_served : array<TtsServedFile>
var g_tts_pending = 0
var g_tts_done_jobs = 0l
var g_tts_audio_s = 0.0
let TTS_QUEUE_LIMIT = 16
let TTS_WRITER_KEEPALIVE_MS = 600000
let TTS_MAX_INPUT_CHARS = 4096
let SPEECH_FIELDS : table<string> <- { "model", "input", "voice", "speed", "response_format", "instructions", "stream_format" }
let PHONEMES_FIELDS : table<string> <- { "model", "input", "voice" }

def private emit_tts_event(events : Channel?; event : TtsEvent) {
    events |> push_clone(event)
}

def private process_tts_job(var model : TtsModel; job : TtsJob; events : Channel?) {
    var inscope c <- caps(model)   // freed at scope exit: the worker context never collects, so a per-request caps struct would accumulate
    let voice = empty(job.voice) ? c.voices[0] : job.voice   // the ready event refused an empty roster, so index 0 is there
    var inscope audio <- synthesize(model, job.text, voice, job.speed)
    var inscope bytes <- job.format == "pcm" ? pcm16_bytes(audio.pcm) : wav_pcm16_bytes(audio.pcm, audio.sample_rate)
    let temp = create_temp_file_result("dasllama_speech", ".{job.format}")   // unguessable name: a shared tmpdir is not ours alone
    if (!(temp is value)) {
        panic("cannot create a temp file for the synthesis: {temp as error}")
    }
    let path = temp as value
    let f = fopen(path, "wb")
    if (f == null) {
        panic("cannot write {path}")
    }
    fwrite(f, bytes)
    fclose(f)
    emit_tts_event(events, TtsEvent(
        kind = TtsEventKind.completed, id = job.id, status = int(http_status.OK),
        content_type = job.format == "pcm" ? "audio/pcm" : "audio/wav", file_path = path,
        audio_s = float(length(audio.pcm)) / float(audio.sample_rate)))
}

// The front end alone, on the same worker because the packs live with the model: the spoken form
// the normalizer produces, then one row per chunk the synthesis would take, each row carrying that
// chunk's text and the 45-symbol phoneme string the g2p pass reads out of it.
// the dialect a phonemes request reads in. A model whose front end phonemizes ONE language has
// only that answer to give, so every voice name reads in it - an alias, a name the model does not
// carry, a name typed into the page's box. A model that phonemizes several must be told which
// voice, and refuses an unservable one exactly as the speech route does.
def private tts_phonemes_lang(model : TtsModel; voice : string) : string {
    var inscope c <- caps(model)
    if (empty(voice) || length(c.langs) < 2) {
        return c.langs[0]
    }
    return tts_voice_lang(model, voice)
}

def private process_tts_phonemes(model : TtsModel; job : TtsJob; events : Channel?) {
    let lang = tts_phonemes_lang(model, job.voice)
    // the normalizer reads English: a Pocket model in another language takes the text as it is
    let norm = tts_has_phonemes(model) || lang == "en" ? tts_normalize(job.text) : job.text
    var inscope chunks <- tts_chunks(model, norm)
    let doc = build_string() $(w) {
        w |> write("\{\"normalized\":{json_str(norm)},\"lang\":{json_str(lang)},\"chunks\":[")
        for (i in range(length(chunks))) {
            if (i > 0) {
                w |> write(",")
            }
            // a model that reads text has no phoneme string to show; its chunks still say what it will read
            let phonemes = tts_has_phonemes(model) ? tts_phonemize(model, chunks[i], lang) : ""
            w |> write("\{\"text\":{json_str(chunks[i])},\"phonemes\":{json_str(phonemes)}}")
        }
        w |> write("]}")
    }
    emit_tts_event(events, TtsEvent(
        kind = TtsEventKind.completed, id = job.id, status = int(http_status.OK),
        content_type = "application/json", body = doc))
}

// a model whose every voice pack speaks a language the front end cannot phonemize has nothing to
// serve - the route would 500 on its own default voice - so the worker refuses to arm and takes
// the degrade path a failed load takes
def private refuse_voiceless_tts(model : TtsModel; model_path : string) {
    var inscope c <- caps(model)
    if (empty(c.voices)) {
        panic("'{model_path}' carries no voice the front end can drive")
    }
}

// what the main context is told about the loaded model: the voices it will accept, the rate its
// wav headers carry, and the lane the load actually took (the pin, or the policy when none)
def private tts_ready_event(model : TtsModel) : TtsEvent {
    var inscope c <- caps(model)
    var inscope ev = TtsEvent(kind = TtsEventKind.ready, sample_rate = c.sample_rate,
        lane = tts_serves_q8() ? "q8" : "f32", cloning = c.cloning, speed = c.speed, lang = empty(c.langs) ? "" : c.langs[0])
    ev.voices := c.voices
    return <- ev
}

def private is_voice_clip_name(fname : string) : bool {
    let lower = to_lower(fname)
    for (ext in TTS_VOICE_CLIP_EXTENSIONS) {
        if (ends_with(lower, ext)) {
            return true
        }
    }
    return false
}

// the clips of `voices_dir` join the loaded model's voices, each under its file's stem, in the
// directory's own order; a clip that does not decode, or one the model refuses, is logged and
// skipped - a bad clip is not a reason to drop the route
def private register_voice_clips(var model : TtsModel; voices_dir : string) {
    return if (empty(voices_dir))
    var inscope c <- caps(model)
    if (!c.cloning) {
        to_log(LOG_ERROR, "dasllama-server: tts_voices_dir '{voices_dir}' is set, but the speech model cannot clone a voice - ignored\n")
        return
    }
    if (!stat(voices_dir).is_dir) {
        to_log(LOG_ERROR, "dasllama-server: tts_voices_dir '{voices_dir}' is not a directory - no voice clip added\n")
        return
    }
    var added = 0
    var clips = 0
    var seen : table<string>
    dir(voices_dir) $(fname) {
        return if (!is_voice_clip_name(fname))
        clips++
        let clip_path = path_join(voices_dir, fname)
        let name = stem(fname)
        if (key_exists(seen, name)) {
            to_log(LOG_WARNING, "dasllama-server: voice clip {clip_path} shares its stem with an earlier clip - it replaces that voice\n")
        } elif (find_index(c.voices, name) >= 0) {
            to_log(LOG_WARNING, "dasllama-server: voice clip {clip_path} replaces the model's own voice '{name}'\n")
        }
        let pcm <- load_audio_mono(clip_path, c.sample_rate)
        if (empty(pcm)) {
            to_log(LOG_ERROR, "dasllama-server: voice clip {clip_path} did not decode - skipped\n")
            return
        }
        var why = ""
        let refused = catch_worker_panic(why) {
            tts_register_voice(model, name, pcm, c.sample_rate)
        }
        if (refused) {
            to_log(LOG_ERROR, "dasllama-server: voice clip {clip_path} refused: {why} - skipped\n")
            return
        }
        seen |> insert(name)
        added++
        to_log(LOG_INFO, "dasllama-server: voice '{name}' cloned from {clip_path} ({float(length(pcm)) / float(c.sample_rate)} s)\n")
    }
    if (clips == 0) {
        to_log(LOG_WARNING, "dasllama-server: tts_voices_dir '{voices_dir}' holds no clip - a voice is a {join(TTS_VOICE_CLIP_EXTENSIONS, " / ")} file\n")
    } elif (added == 0) {
        to_log(LOG_WARNING, "dasllama-server: tts_voices_dir '{voices_dir}' added no voice - every clip there was skipped\n")
    }
}

def private tts_worker_main(slot : int; model_path : string; lane : string; voices_dir : string; team_dispatch : bool; var jobs : Channel?&; var events : Channel?&) {
    this_context().name := "dasllama_tts_{slot}"
    configure_media_worker_dispatch(team_dispatch)
    var inscope model = TtsModel()
    var load_failure = ""
    // the lane pins HERE, in the context that loads: this thread started every dasLLAMA module
    // global at its declared default, so a pin set by the spawning context never arrived
    set_tts_q8(lane == "q8")
    defer() {
        reset_tts_q8()
    }
    let load_failed = catch_worker_panic(load_failure) {
        model <- load_tts_model(model_path)   // nolint:PERF030 - target is the fresh empty decl above; the load runs inside the catch block, so it cannot fold into the declaration
        refuse_voiceless_tts(model, model_path)
        register_voice_clips(model, voices_dir)
    }
    if (load_failed) {   // an escaping panic here would kill only this thread and hang the ready-wait
        to_log(LOG_ERROR, "dasllama-server: the TTS worker could not load {model_path}: {load_failure}\n")
        emit_tts_event(events, TtsEvent(kind = TtsEventKind.failed, slot = slot, body = load_failure))
        // no delete: whatever the load built, whole or half, dies with this thread's context
        jobs |> release
        events |> release
        return
    }
    var ready <- tts_ready_event(model)
    ready.slot = slot
    emit_tts_event(events, ready)
    delete ready
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(250) $(job : TtsJob#) {
            if (job.kind == TtsJobKind.stop) {
                stopping = true
            } else {
                var job_copy : TtsJob
                job_copy := job
                var failure = ""
                let failed = catch_worker_panic(failure) {
                    if (job_copy.kind == TtsJobKind.phonemes) {
                        process_tts_phonemes(model, job_copy, events)
                    } else {
                        process_tts_job(model, job_copy, events)
                    }
                }
                if (failed) {
                    // a voice the model lacks, or a speed on a model with no speed control, is the
                    // caller's mistake; anything else is ours
                    let client = find(failure, "unknown voice") >= 0 || find(failure, "speed control") >= 0
                    to_log(LOG_ERROR, "[req {job_copy.id}] tts: failed: {failure}\n")
                    emit_tts_event(events, TtsEvent(
                        kind = TtsEventKind.completed, id = job_copy.id,
                        status = int(client ? http_status.BAD_REQUEST : http_status.INTERNAL_SERVER_ERROR),
                        content_type = "application/json",
                        body = error_body(failure, client ? "invalid_request_error" : "server_error")))
                }
                delete job_copy
            }
        }
    }
    delete model
    emit_tts_event(events, TtsEvent(kind = TtsEventKind.stopped, slot = slot))
    jobs |> release
    events |> release
}

// The readiness gate both TTS routes answer with, since both ride the one worker queue. True when
// the request has already been refused.
def private tts_route_unavailable(srv : FrontLink; writer : WriterRef) : bool {
    if (reject_deferred_while_draining(srv, writer) || reject_while_benching(srv, writer)) {
        return true
    }
    if (tts_default() < 0) {
        service_unavailable(srv, writer, "no TTS model is served — start with --tts <model.gguf>")
        return true
    }
    if (g_tts_pending >= TTS_QUEUE_LIMIT) {
        service_unavailable(srv, writer, "the TTS queue is at capacity ({TTS_QUEUE_LIMIT}) — retry shortly")
        return true
    }
    return false
}

// the `input` field both TTS routes take, under the one cap; "" back means already refused. The
// cap counts CHARACTERS, the way the text box that fills it does — `length()` on a string is
// bytes, and a dash or a curly quote pasted from a document costs three of them
def private tts_input_field(srv : FrontLink; writer : WriterRef; js : JsonValue?; verb : string) : string {
    let input = js?["input"] ?? ""
    if (empty(strip(input))) {   // blank text has nothing to say: every family would answer no samples
        bad_request(srv, writer, "'input' is required (the text to {verb})")
        return ""
    }
    let chars = utf8_length(input)
    if (chars > TTS_MAX_INPUT_CHARS) {
        bad_request(srv, writer, "'input' is {chars} characters, past the {TTS_MAX_INPUT_CHARS} this route takes — split the text")
        return ""
    }
    return input
}

def private served_tts_ids : string {
    return join([for (m in g_tts); "'{m.id}'"; where m.armed], ", ")
}

// the `model` field both TTS routes take: a served id, or nothing (the default model). The model's slot, or -1
// when the request is already refused.
def private tts_request_slot(srv : FrontLink; writer : WriterRef; js : JsonValue?) : int {
    let want_model = js?["model"] ?? ""
    let k = tts_slot_of(want_model)
    if (k < 0) {
        respond(srv, writer, int(http_status.NOT_FOUND), "application/json",
            error_body("model '{want_model}' is not a served TTS model ({served_tts_ids()}) — GET /v1/models lists the ids", "invalid_request_error"))
    }
    return k
}

// the wire and the queue slot a deferred TTS answer needs, plus the keepalive the synthesis outlives
def private enqueue_tts_job(srv : FrontLink; writer : WriterRef; slot : int; job : TtsJob; note : string) {
    set_writer_keepalive_timeout(srv, writer, TTS_WRITER_KEEPALIVE_MS)
    g_tts_wires[job.id] <- TtsWire(writer = writer)
    g_tts_pending++
    g_tts_jobs[slot] |> push_clone(job)
    to_log(LOG_INFO, "[req {job.id}] tts: {note} on {g_tts[slot].id} (pending={g_tts_pending})\n")
}

// POST /v1/audio/phonemes: {"input", "voice"?} -> {"normalized", "lang", "chunks":[{"text","phonemes"}]}.
// The front end's own view of what a synthesis of the same text in that voice would say, in the
// voice's dialect, answered by the worker that owns the packs; the control page draws it under
// the studio's waveform.
def private queue_phonemes(srv : FrontLink; req : FrontReq; writer : WriterRef) {
    if (tts_route_unavailable(srv, writer) || reject_while_benching(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(req.body, perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, PHONEMES_FIELDS, "/v1/audio/phonemes")
    let slot = tts_request_slot(srv, writer, js)
    return if (slot < 0)
    let input = tts_input_field(srv, writer, js, "phonemize")
    if (empty(input)) {
        return
    }
    enqueue_tts_job(srv, writer, slot, TtsJob(kind = TtsJobKind.phonemes, id = next_id(), text = input, voice = js?["voice"] ?? ""),
        "phonemes queued ({utf8_length(input)} chars)")
}

// POST /v1/audio/speech: {"model"?, "input", "voice"?, "speed"?, "response_format"?: "wav" | "pcm"}.
def private queue_speech(srv : FrontLink; req : FrontReq; writer : WriterRef) {
    if (tts_route_unavailable(srv, writer) || reject_while_benching(srv, writer)) {
        return
    }
    var perr = ""
    var js = read_json(req.body, perr)
    if (js == null) {
        bad_request(srv, writer, "invalid JSON: {perr}")
        return
    }
    defer() { unsafe { delete js } }
    warn_ignored_fields(js, SPEECH_FIELDS, "/v1/audio/speech")
    let slot = tts_request_slot(srv, writer, js)
    return if (slot < 0)
    let input = tts_input_field(srv, writer, js, "speak")
    if (empty(input)) {
        return
    }
    let fmt = js?["response_format"] ?? "wav"
    if (fmt != "wav" && fmt != "pcm") {
        bad_request(srv, writer, "response_format '{fmt}' is not served — wav (the default) and pcm (16-bit little-endian at the model's rate) are; mp3, opus, aac and flac need an encoder this server does not carry")
        return
    }
    let speed = float(js?["speed"] ?? 1.0lf)
    if (speed < 0.25 || speed > 4.0) {
        bad_request(srv, writer, "'speed' must be between 0.25 and 4.0")
        return
    }
    enqueue_tts_job(srv, writer, slot,
        TtsJob(id = next_id(), text = input, voice = js?["voice"] ?? "", speed = speed, format = fmt),
        "queued ({utf8_length(input)} chars, {fmt})")
}

// the ready event's payload into its model's slot: every string is cloned, because the popped
// clone lives in the channel's heap and dies with this callback
def private adopt_tts_ready(var m : TtsSlot; event : TtsEvent#) {
    m.voices |> clear()
    m.voices |> reserve(length(event.voices))
    for (v in event.voices) {
        m.voices |> push(clone_string(v))
    }
    m.sample_rate = event.sample_rate
    m.lane_served = clone_string(event.lane)
    m.cloning = event.cloning
    m.speed = event.speed
    m.lang = clone_string(event.lang)
}

//! Start every armed TTS model's worker and wait for each to load. A worker that cannot load DEGRADES its model
//! as ``add_tts_model`` degrades a missing file, for the failures only the load can see - a front-end pack from
//! before this build, a truncated GGUF, a model with no voice this front end can drive: the model's route drops and
//! the boot serves on (a crash-looping boot under the watchdog never gets a page to fix the config from), and the
//! `tts` block of /v1/stats carries the reason so that page can say it.
def private start_tts_worker() {
    g_tts_jobs |> resize(length(g_tts))
    for (k in range(length(g_tts))) {
        assume m = g_tts[k]
        m.ready = 0
        continue if (!m.armed)
        g_tts_jobs[k] = worker_channel()
        to_log(LOG_INFO, "dasllama-server: starting the TTS worker for {m.id} on the {m.lane} lane\n")
        let slot = k
        let model_path = m.path
        let lane = m.lane
        let voices_dir = m.voices_dir
        let team_dispatch = g_tts_team_dispatch
        var jobs_capture = g_tts_jobs[k]
        var events_capture = g_tts_events
        new_thread() <| @capture(= slot, = model_path, = lane, = voices_dir, = team_dispatch, = jobs_capture, = events_capture) {
            tts_worker_main(slot, model_path, lane, voices_dir, team_dispatch, jobs_capture, events_capture)
        }
        var load_failure = ""
        while (m.ready < 1 && empty(load_failure)) {
            g_tts_events |> pop_with_timeout_clone(1000) $(event : TtsEvent#) {
                if (event.kind == TtsEventKind.ready) {
                    adopt_tts_ready(m, event)
                    m.ready++
                    to_log(LOG_INFO, "dasllama-server: TTS worker ready ({m.id}: {length(m.voices)} voice(s), {m.sample_rate} Hz, {m.lane_served} lane)\n")
                } elif (event.kind == TtsEventKind.failed) {
                    load_failure = clone_string(event.body)
                }
            }
        }
        if (!empty(load_failure)) {
            m.error = load_failure
            m.armed = false
            to_log(LOG_ERROR, "dasllama-server: dropping the speech route for {m.id} - the TTS worker could not load {m.path}: {load_failure} (fix the file set, then POST /restart)\n")
        }
    }
}

def private drain_tts_events(srv : FrontLink) {
    var draining = true
    while (draining) {
        draining = g_tts_events |> try_pop_clone() $(event : TtsEvent#) {
            if (event.kind == TtsEventKind.completed) {
                g_tts_pending = max(g_tts_pending - 1, 0)
                // a temp file means a synthesis landed; a refusal and a phonemes document both
                // answer as a body, and neither is speech the counters may claim
                let synthesized = !empty(event.file_path)
                if (synthesized) {
                    g_tts_done_jobs++
                    g_tts_audio_s += event.audio_s
                }
                var wire = unsafe(g_tts_wires?[event.id])
                if (wire != null) {
                    if (is_writer_connected(srv, wire.writer)) {
                        set_writer_keepalive_timeout(srv, wire.writer, DEFAULT_WRITER_KEEPALIVE_MS)
                        if (synthesized) {
                            set_header(srv, wire.writer, "Content-Type", event.content_type)
                            SERVE_FILE(srv, wire.writer, event.file_path)
                        } else {
                            respond(srv, wire.writer, event.status, event.content_type, event.body)
                        }
                    } else {
                        release_writer(srv, wire.writer)
                    }
                    delete g_tts_wires[event.id]
                    g_tts_wires |> erase(event.id)
                }
                if (!empty(event.file_path)) {
                    g_tts_served |> push(TtsServedFile(path = clone_string(event.file_path), at = ref_time_ticks()))
                }
                // only a synthesis carries seconds; a phonemes document and a refusal have none
                to_log(LOG_INFO, synthesized
                    ? "[req {event.id}] tts: completed {event.status} ({event.audio_s} s of audio, pending={g_tts_pending})\n"
                    : "[req {event.id}] tts: completed {event.status} (pending={g_tts_pending})\n")
            }
        }
    }
    // the async file write is done long before this: served files go after a minute
    var i = 0
    while (i < length(g_tts_served)) {
        if (get_time_nsec(g_tts_served[i].at) > 60_000_000_000l) {
            remove_result(g_tts_served[i].path)
            g_tts_served |> erase(i)
        } else {
            i++
        }
    }
}

// A synthesis in flight when the stop arrives still lands a completed event: its wire and its
// temp file are ours to close, so the teardown finishes the job instead of dropping it.
def private discard_tts_completion(id : int64; file_path : string) {
    g_tts_pending = max(g_tts_pending - 1, 0)
    var wire = unsafe(g_tts_wires?[id])
    if (wire != null) {
        if (g_link.out != null) {
            release_writer(g_link, wire.writer)
        }
        delete g_tts_wires[id]
        g_tts_wires |> erase(id)
    }
    if (!empty(file_path)) {
        remove_result(file_path)
    }
    to_log(LOG_WARNING, "[req {id}] tts result discarded during teardown\n")
}

def private stop_tts_worker() {
    var asked = 0
    for (m, jobs in g_tts, g_tts_jobs) {
        if (m.armed && m.ready > 0 && jobs != null) {
            jobs |> push_clone(TtsJob(kind = TtsJobKind.stop))
            asked++
        }
    }
    let stopped = await_stopped(g_tts_events, asked, WORKER_STOP_WAIT_NS) $(event : TtsEvent#) {
        if (event.kind == TtsEventKind.completed) {
            discard_tts_completion(event.id, clone_string(event.file_path))
        }
        return event.kind == TtsEventKind.stopped
    }
    if (stopped < asked) {
        to_log(LOG_WARNING, "dasllama-server: {asked - stopped} TTS worker(s) did not answer the stop within {WORKER_STOP_WAIT_NS / 1_000_000_000l} s - a dead worker must not hang the teardown\n")
    }
    for (m in g_tts) {
        m.ready = 0
    }
}

// ===== async vision encode (the ASR worker pattern: own context, own embedder) =====
// The decode + letterbox + embedder GEMMs run on ONE worker thread, so an image request never
// stalls the tick loop's decode of every live stream.

enum private MediaJobKind {
    encode   // zero value — the channel pops zeroed clones
    stop     // worker exits
    drop     // free slot `name`'s encoders (unload without a worker bounce)
}

struct private MediaJob {
    kind : MediaJobKind
    id : int64
    name : string               // the serving slot's NAME — the worker's encoder tables key on it,
                                // so an unload's index shift never reaches the worker
    file_bytes : array<uint8>   // the base64-decoded media FILE (image or audio); the worker decodes + encodes
    tag : string                // DASLLAMA_VISION_DUMP artifact name (image jobs)
    is_audio : bool             // audio job: decode to 16 kHz PCM, encode via the slot's audio embedder
    max_audio_frames : int
}

enum private MediaEventKind {
    completed   // one job's result (ok + why say how it went); zero value — the channel pops zeroed clones
    ready       // the worker loaded its embedders
    stopped     // the worker exited
}

struct private MediaEvent {
    kind : MediaEventKind
    id : int64
    ok : bool
    why : string
    n_media : int64
    rows : array<float>
    width : int
    height : int
    grid : int2                 // qwen mrope: the merged image grid; (0,0) = sequential positions (gemma, audio)
    is_audio : bool
    audio_cs : int    // decoded audio length in centiseconds (audio jobs; ints ride the channel clone)
    enc_ms : int64
}

struct private MediaWire {
    @do_not_delete writer : WriterRef
    @do_not_delete js : JsonValue?   // the parked parsed request — `drop_media_wire` deletes it explicitly
    slot : int
    refs : array<MediaRef>           // the request's media parts, in transcript order
    jobs_left : int                  // encode jobs the worker still owes this request
    failed : string                  // the first job failure's reason
    submitted : bool                 // rendered and handed to the scheduler: kept only for a lost span's retry
    retried : bool                   // a lost span was retried once already
}

//! which ref of which parked request an encode job serves
struct private MediaJobRef {
    wire : int64
    at : int
}

var private g_media_job_ref : table<int64; MediaJobRef>

let MEDIA_QUEUE_LIMIT = MEDIA_SPANS_LIMIT   //!< encode jobs queued or in flight, a job a new part: one request's parts fill it alone (decoded bytes ride the channel)

let GEMMA4A_AUDIO_SOFT_TOKENS_PER_SEC = 25l
let AUDIO_DECODE_SAMPLE_RATE = 16000l

//! The 16 kHz decode-frame ceiling for a clip a model with `seq_len` context can hold: the clip's
//! soft tokens must fit the context, so the longest usable clip is `seq_len / tokens-per-sec`
//! seconds. Caps the decode of an untrusted upload before a compressed bomb reaches the encoder.
def private audio_decode_frame_cap(seq_len : int64) : int {
    return max(int(AUDIO_DECODE_SAMPLE_RATE), int(seq_len / GEMMA4A_AUDIO_SOFT_TOKENS_PER_SEC * AUDIO_DECODE_SAMPLE_RATE))
}

var private g_media_jobs : Channel?
var private g_media_events : Channel?
var private g_media_wires : table<int64; MediaWire>
var private g_media_rows = 0l   // media (image span) rows admitted to prefill since boot
var private g_media_cover_probe = true

//! Whether a rowless part's span is looked up in the prefix cache before the request is submitted (the default). Off,
//! every known part goes to the scheduler rowless and a span the cache does not hold comes back `media_lost` - the
//! route a span evicted between the lookup and the admission takes. Non-private: test_openai_server_audio.das gates
//! the retry through it.
def set_media_cover_probe(on : bool) {
    g_media_cover_probe = on
}
var private g_media_ready = 0
var private g_media_armed = false
var private g_media_pending = 0   // encode jobs queued or in flight; the wire table undercounts
                                  // (reap frees a dead client's wire while its job runs on)

// One media job on the worker thread: decode the file bytes (image OR audio), run the slot's
// encoder, and return the completed event — ok + rows, or why. The one rail that chews untrusted
// bytes: an escaping panic in a new_thread worker is a PROCESS abort, so the decode+encode runs
// under the catch (the ASR shape).
def private media_encode_one(job : MediaJob; var embs : table<string; VisionEmbedder>;
                             var states : table<string; VisionState>;
                             var audio_embs : table<string; AudioEmbedder>;
                             var audio_states : table<string; AudioState>) : MediaEvent {
    var out = MediaEvent(id = job.id, is_audio = job.is_audio)
    if (job.is_audio && !key_exists(audio_embs, job.name)) {
        out.why = "this slot has no audio encoder in the worker (armed after init_server, or its load failed at boot)"
        return <- out
    }
    if (!job.is_audio && !key_exists(embs, job.name)) {
        out.why = "this slot has no vision embedder in the worker (armed after init_server, or its load failed at boot)"
        return <- out
    }
    var failure = ""
    let failed = catch_worker_panic(failure) {
        if (job.is_audio) {
            var inscope samples <- decode_audio_16k_mono(job.file_bytes, job.max_audio_frames)
            if (empty(samples)) {
                out.why = "the audio did not decode, or it is longer than this model's context can hold (wav/mp3/flac/ogg, up to ~{job.max_audio_frames / int(AUDIO_DECODE_SAMPLE_RATE)}s)"
            } else {
                let t0 = ref_time_ticks()
                out.n_media = encode_audio(audio_embs[job.name], audio_states[job.name], samples, out.rows)
                out.ok = true
                out.audio_cs = int(long_length(samples) * 100l / 16000l)
                out.enc_ms = int64(get_time_usec(t0)) / 1000l
            }
        } else {
            var inscope img <- decode_image_bytes(job.file_bytes, out.why)
            if (img.width != 0) {
                let t0 = ref_time_ticks()
                out.n_media = encode_image(embs[job.name], states[job.name], img, job.tag, out.rows)
                out.ok = true
                out.width = img.width
                out.height = img.height
                out.grid = vision_mrope_grid(embs[job.name], states[job.name])
                out.enc_ms = int64(get_time_usec(t0)) / 1000l
            }
        }
    }
    if (failed) {
        to_log(LOG_ERROR, "[req {job.id}] {job.is_audio ? "audio" : "vision"} encode failed: {failure}\n")
        out.ok = false
        out.why = "the {job.is_audio ? "audio" : "vision"} encode failed - see the server log"
    }
    return <- out
}

def private media_worker_main(slot_path_lines : string; var jobs : Channel?&; var events : Channel?&) {
    this_context().name := "dasllama_media"
    configure_media_worker_dispatch(false)
    var embs : table<string; VisionEmbedder>
    var states : table<string; VisionState>
    var audio_embs : table<string; AudioEmbedder>
    var audio_states : table<string; AudioState>
    for (line in split(slot_path_lines, "\n")) {   // "<slot name>\t<V|A>\t<path>" per line
        continue if (empty(line))
        let sep = find(line, "\t")
        let name = slice(line, 0, sep)
        let kind = slice(line, sep + 1, sep + 2)
        let path = slice(line, sep + 3)
        var load_failure = ""
        let load_failed = catch_worker_panic(load_failure) {
            if (kind == "A") {
                audio_embs[name] <- load_audio_embedder(path)   // own mapping; pages shared with a vision arm on the same file
                audio_states[name] <- AudioState()
            } else {
                embs[name] <- load_vision_embedder(path)   // own mapping; pages shared with the slot's
                states[name] <- VisionState()
            }
        }
        if (load_failed) {   // the slot's requests take the no-encoder 400; the boot must not hang on this thread
            to_log(LOG_ERROR, "dasllama-server: media worker could not load slot {name}'s {kind == "A" ? "audio encoder" : "vision embedder"}: {load_failure}\n")
            if (kind == "A") {
                audio_embs |> erase(name)
                audio_states |> erase(name)
            } else {
                embs |> erase(name)
                states |> erase(name)
            }
        }
    }
    var hello = MediaEvent(kind = MediaEventKind.ready)
    events |> push_clone(hello)
    delete hello
    var stopping = false
    while (!stopping) {
        jobs |> pop_with_timeout_clone(250) $(job : MediaJob#) {
            if (job.kind == MediaJobKind.stop) {
                stopping = true
                return
            }
            if (job.kind == MediaJobKind.drop) {
                embs |> erase(job.name)
                states |> erase(job.name)
                audio_embs |> erase(job.name)
                audio_states |> erase(job.name)
                return
            }
            var job_copy : MediaJob
            job_copy := job
            var out <- media_encode_one(job_copy, embs, states, audio_embs, audio_states)
            events |> push_clone(out)
            delete out
            delete job_copy
        }
    }
    unsafe {
        delete embs
        delete states
        delete audio_embs
        delete audio_states
    }
    var farewell = MediaEvent(kind = MediaEventKind.stopped)
    events |> push_clone(farewell)
    delete farewell
    jobs |> release       // the captured channel refs, or the thread teardown panics
    events |> release
}

def private start_media_worker() {
    g_media_ready = 0
    g_media_armed = false
    var arms : array<string>
    defer() { delete arms }
    for (s in g_slots) {
        if (s.vision.kind != VisionKind.none) {
            arms |> push("{s.name}\tV\t{s.vision_path}")
        }
        if (s.audio_armed) {
            arms |> push("{s.name}\tA\t{s.audio_path}")
        }
    }
    if (empty(arms)) {
        return
    }
    g_media_armed = true
    let arms_spec = join(arms, "\n")   // "<slot name>\t<V|A>\t<mmproj path>" per line
    var jobs_capture = g_media_jobs
    var events_capture = g_media_events
    to_log(LOG_INFO, "dasllama-server: starting the asynchronous media encode worker\n")
    new_thread() <| @capture(= arms_spec, = jobs_capture, = events_capture) {
        media_worker_main(arms_spec, jobs_capture, events_capture)
    }
    while (g_media_ready < 1) {
        g_media_events |> pop_with_timeout_clone(1000) $(event : MediaEvent#) {
            if (event.kind == MediaEventKind.ready) {
                g_media_ready++
                to_log(LOG_INFO, "dasllama-server: media encode worker ready\n")
            }
        }
    }
}

def private stop_media_worker() {
    if (!g_media_armed) {
        return
    }
    var halt = MediaJob(kind = MediaJobKind.stop)
    g_media_jobs |> push_clone(halt)
    delete halt
    let stopped = await_stopped(g_media_events, 1, WORKER_STOP_WAIT_NS) $(event : MediaEvent#) {
        if (event.kind == MediaEventKind.completed) {
            g_media_pending = max(g_media_pending - 1, 0)
            to_log(LOG_WARNING, "[req {event.id}] media result discarded during teardown\n")
        }
        return event.kind == MediaEventKind.stopped
    }
    if (stopped < 1) {
        to_log(LOG_WARNING, "dasllama-server: the media worker did not answer the stop within {WORKER_STOP_WAIT_NS / 1_000_000_000l} s - a dead worker must not hang the teardown\n")
    }
    g_media_ready = 0
    g_media_armed = false
}

def private drain_media_events(srv : FrontLink) {
    var draining = true
    while (draining) {
        draining = g_media_events |> try_pop_clone() $(event : MediaEvent#) {
            return if (event.kind != MediaEventKind.completed)
            g_media_pending = max(g_media_pending - 1, 0)   // even when the reap already freed the wire
            var ev : MediaEvent
            ev := event
            defer() { delete ev }
            let jr = g_media_job_ref?[ev.id] ?? MediaJobRef()
            g_media_job_ref |> erase(ev.id)
            // interior pointer into g_media_wires — nothing inserts into the table inside this drain
            var wire = unsafe(g_media_wires?[jr.wire])
            if (wire == null) {
                return   // the client vanished while encoding; reap released the writer
            }
            wire.jobs_left--
            if (!ev.ok) {
                wire.failed = empty(wire.failed) ? ev.why : wire.failed
            } else {
                if (ev.is_audio) {
                    to_log(LOG_INFO, "[req {jr.wire}] audio {float(ev.audio_cs) / 100f}s -> {ev.n_media} soft tokens in {ev.enc_ms}ms off-thread\n")
                } else {
                    to_log(LOG_INFO, "[req {jr.wire}] image {ev.width}x{ev.height} -> {ev.n_media} soft tokens in {ev.enc_ms}ms off-thread\n")
                }
                wire.refs[jr.at].n_rows = ev.n_media
                wire.refs[jr.at].grid = ev.grid
                delete wire.refs[jr.at].rows
                wire.refs[jr.at].rows <- ev.rows
                if (length(g_media_seen) >= MEDIA_SEEN_LIMIT) {
                    g_media_seen |> clear()
                }
                g_media_seen[wire.refs[jr.at].key] = MediaSeen(n_rows = ev.n_media, grid = ev.grid)
            }
            return if (wire.jobs_left > 0)
            if (!is_writer_connected(srv, wire.writer)) {
                release_writer(srv, wire.writer)
                drop_media_wire(jr.wire, true)
            } elif (!empty(wire.failed)) {
                bad_request(srv, wire.writer, wire.failed)
                drop_media_wire(jr.wire, true)
            } elif (reject_deferred_while_draining(srv, wire.writer)) {
                drop_media_wire(jr.wire, true)   // a clean terminal 503: the drain must not admit a new generation
            } else {
                resume_media_request(srv, jr.wire)
            }
        }
    }
}

// ===== Server =====

// The front's route table: path and method to the engine's route. STREAM takes every method, so the method is
// checked here and a mismatch reads as the unknown route, as a GET / POST registration answered it.
struct private RouteRow {
    path : string
    method : string
    route : Route
    upload : bool       //! a multipart form whose 'file' the front saves for the engine
}

let private ROUTES = fixed_array(
    RouteRow(path = "/v1/models", method = "GET", route = Route.models),
    RouteRow(path = "/v1/chat/completions", method = "POST", route = Route.chat),
    RouteRow(path = "/v1/completions", method = "POST", route = Route.completion),
    RouteRow(path = "/v1/audio/transcriptions", method = "POST", route = Route.transcribe, upload = true),
    RouteRow(path = "/v1/audio/translations", method = "POST", route = Route.translate, upload = true),
    RouteRow(path = "/v1/audio/speech", method = "POST", route = Route.speech),
    RouteRow(path = "/v1/audio/phonemes", method = "POST", route = Route.phonemes),
    RouteRow(path = "/v1/embeddings", method = "POST", route = Route.embeddings),
    RouteRow(path = "/v1/models/activate", method = "POST", route = Route.activate),
    RouteRow(path = "/v1/models/load", method = "POST", route = Route.load),
    RouteRow(path = "/v1/models/unload", method = "POST", route = Route.unload),
    RouteRow(path = "/v1/stats", method = "GET", route = Route.stats),
    RouteRow(path = "/v1/streams", method = "GET", route = Route.streams),
    RouteRow(path = "/config", method = "GET", route = Route.config_get),
    RouteRow(path = "/config", method = "POST", route = Route.config_save),
    RouteRow(path = "/vad", method = "POST", route = Route.vad, upload = true),
    RouteRow(path = "/exchange", method = "GET", route = Route.exchange_get),
    RouteRow(path = "/exchange/matches", method = "GET", route = Route.exchange_matches),
    RouteRow(path = "/exchange/apply", method = "POST", route = Route.exchange_apply),
    RouteRow(path = "/exchange/submit", method = "POST", route = Route.exchange_submit),
    RouteRow(path = "/exchange/retune", method = "POST", route = Route.exchange_retune),
    RouteRow(path = "/exchange/consent", method = "POST", route = Route.exchange_consent),
    RouteRow(path = "/bench", method = "POST", route = Route.bench_start),
    RouteRow(path = "/bench", method = "GET", route = Route.bench_get),
    RouteRow(path = "/bake", method = "POST", route = Route.bake_start),
    RouteRow(path = "/bake", method = "GET", route = Route.bake_get),
    RouteRow(path = "/catalog", method = "GET", route = Route.catalog),
    RouteRow(path = "/catalog/download", method = "POST", route = Route.catalog_download),
    RouteRow(path = "/v1/images", method = "GET", route = Route.images),
    RouteRow(path = "/restart", method = "POST", route = Route.restart),
    RouteRow(path = "/shutdown", method = "POST", route = Route.shutdown),
    RouteRow(path = "/gc", method = "POST", route = Route.gc))

// ===== The front's own state: it lives in the front thread's context, never the engine's =====
var private g_fwires : table<int64; HttpResponseWriter?>   //! wire id -> the writer the engine's reply goes to
var private g_fnext = 0l
var private g_fgone : table<int64>                          //! wires already reported gone
var private g_fto_engine : Channel?
var private g_fto_speech : Channel?                         //! the speech thread's inbox; null on a server with no speech model
var private g_fspeech : table<int64>                        //! the wires whose request went to the speech thread

//! a status request the engine has not answered yet
struct private StatusWait {
    path : string
    since : int64       //! ref_time_ticks when it arrived
}

var private g_fstatus : table<int64; StatusWait>            //! status wires, until the engine's answer arrives
var private g_fcache : table<string; string>                //! path -> the engine's last good answer

let private FRONT_STATUS_WAIT_US = 50000l   //! how long a status request waits on the engine before the last answer serves it

//! the routes a held engine must not hold up: each answers from the engine's last answer past the wait
def private status_route(route : Route) : bool
    => route == Route.models || route == Route.stats || route == Route.streams

//! the table's row for a path under a method; the unknown route when there is none
def private route_row(path, method : string) : RouteRow {
    for (row in ROUTES) {
        if (row.path == path && row.method == method) {
            return row
        }
    }
    return RouteRow(route = Route.unknown)
}

//! the routes the speech thread answers when the server has one
def private speech_route(route : Route) : bool
    => route == Route.transcribe || route == Route.translate || route == Route.speech || route == Route.phonemes

//! one request across to the engine: the wire parked under a fresh id, the upload saved, the body cloned
def private front_forward(var req : HttpRequest?; var writer : HttpResponseWriter?) {
    g_fnext++
    let wid = g_fnext
    g_fwires[wid] = writer
    let method = "{req.method}"
    let path = "{req.path}"
    let row = route_row(path, method)
    var fr = FrontReq(kind = FrontKind.request, wid = wid, route = row.route,
        method = method, path = path, loopback = front_is_loopback(req))
    if (status_route(row.route)) {
        g_fstatus[wid] <- StatusWait(path = path, since = ref_time_ticks())
    }
    if (row.upload && fr.route != Route.unknown) {
        var fname = ""
        each_form_field(req) $(name, _content, filename) {
            if (name == "file" && !empty(filename)) {
                fname = filename
            }
        }
        if (!empty(fname)) {
            fr.file_path = path_join(tmpdir(), "dasllama_upload_{ref_time_ticks()}_{wid}_{base_name(fname)}")
            save_form_file(req, "file", fr.file_path)
        }
        fr.form_format = get_form_data(req, "response_format")
        fr.form_language = get_form_data(req, "language")
        fr.form_model = get_form_data(req, "model")
    } else {
        fr.body = "{req.body}"
    }
    if (g_fto_speech != null && speech_route(fr.route)) {
        g_fspeech |> insert(wid)
        g_fto_speech |> push_clone(fr)
    } else {
        g_fto_engine |> push_clone(fr)
    }
}

class OpenAiFront : HvWebServer {
    def override onInit {
        allow_cors()
        GET("/") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            // the control page ships beside the server sources; libhv sets text/html from the extension
            return resp |> SERVE_FILE(path_join(get_this_module_dir(), "control.html"))
        }
        GET("/favicon.ico") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> SERVE_FILE(path_join(get_this_module_dir(), "tray.ico"))
        }
        STREAM("*") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            front_forward(req, writer)   // every other path: the route table decides, by path and method
        }
    }
}

//! one engine op applied to its wire; a terminal op drops the wire
def private front_apply(srv : WebSocketServer; o : FrontOut) {
    if (o.op == FrontOp.respond && key_exists(g_fstatus, o.wid)) {   // a status answer refreshes the cache, late or not
        if (o.status == 200) {
            g_fcache[g_fstatus[o.wid].path] = clone_string(o.text)
        }
        g_fstatus |> erase(o.wid)
    }
    var writer = g_fwires?[o.wid] ?? null
    return if (writer == null)
    var ends = false
    if (o.op == FrontOp.sse) {
        sse_event(srv, writer, o.text, o.aux)
    } elif (o.op == FrontOp.respond) {
        respond(srv, writer, o.status, o.ctype, o.text)
        ends = true
    } elif (o.op == FrontOp.close) {
        close_writer(srv, writer)
        ends = true
    } elif (o.op == FrontOp.release) {
        release_writer(srv, writer)
        ends = true
    } elif (o.op == FrontOp.keepalive) {
        set_writer_keepalive_timeout(srv, writer, o.ms)
    } elif (o.op == FrontOp.header) {
        set_header(srv, writer, o.aux, o.text)
    } elif (o.op == FrontOp.serve_file) {
        SERVE_FILE(srv, writer, o.text)
        ends = true
    }
    if (ends) {
        g_fwires |> erase(o.wid)
        g_fgone |> erase(o.wid)
        g_fspeech |> erase(o.wid)
    }
}

//! A status request the engine has held past the wait is answered with the engine's last answer for its path, the
//! header saying how long it waited; its wire goes, and the engine's own answer, when it comes, refreshes the cache.
def private front_serve_stale(srv : WebSocketServer) {
    var served : array<int64>
    for (wid, wait in keys(g_fstatus), values(g_fstatus)) {
        let waited = int64(get_time_usec(wait.since))
        continue if (waited < FRONT_STATUS_WAIT_US || !key_exists(g_fwires, wid) || !key_exists(g_fcache, wait.path))
        var writer = g_fwires?[wid] ?? null
        set_header(srv, writer, "X-Dasllama-Stale-Ms", "{waited / 1000l}")
        respond(srv, writer, 200, "application/json", g_fcache?[wait.path] ?? "")
        served |> push(wid)
    }
    for (wid in served) {
        g_fwires |> erase(wid)
        g_fgone |> erase(wid)
    }
    delete served
}

//! tell the engine, once a wire, that its client went away
def private front_reap(srv : WebSocketServer) {
    for (wid, writer in keys(g_fwires), values(g_fwires)) {
        if (!key_exists(g_fgone, wid) && !is_writer_connected(srv, writer)) {
            g_fgone |> insert(wid)
            if (key_exists(g_fspeech, wid)) {   // the thread that holds the wire is the one that drops it
                g_fto_speech |> push_clone(FrontReq(kind = FrontKind.gone, wid = wid))
            } else {
                g_fto_engine |> push_clone(FrontReq(kind = FrontKind.gone, wid = wid))
            }
        }
    }
}

let private FRONT_REAP_EVERY_US = 20000l    //! how often the front looks for dropped clients
let private FRONT_OUT_BURST = 512           //! engine ops applied between two server ticks

//! The front thread: the HTTP server, its writers, and nothing else. It blocks on the engine's op channel for a
//! millisecond at a time, so an op reaches its socket as it arrives and a new request waits a millisecond at most.
def private front_main(port : int; var to_engine : Channel?&; var from_engine : Channel?&; var to_speech : Channel?&) {
    this_context().name := "dasllama-front"
    g_fto_engine = to_engine
    g_fto_speech = to_speech
    var server = new OpenAiFront()
    server->init(port)
    let rc = server->start()   // 0 on success; non-zero = bind/listen failed (e.g. port in use)
    to_engine |> push_clone(FrontReq(kind = FrontKind.ready, status = rc, port = rc == 0 ? server->bound_port() : 0))
    var stop = rc != 0
    var last_reap = ref_time_ticks()
    while (!stop) {
        server->tick()
        var burst = 0
        var more = true
        while (more && !stop && burst < FRONT_OUT_BURST) {
            more = from_engine |> pop_with_timeout_clone(burst == 0 ? 1 : 0) $(o : FrontOut#) {
                if (o.op == FrontOp.stop) {
                    stop = true
                } else {
                    var op : FrontOut
                    op := o
                    front_apply(server.server, op)
                }
            }
            burst++
        }
        if (!empty(g_fstatus)) {
            front_serve_stale(server.server)
        }
        if (int64(get_time_usec(last_reap)) >= FRONT_REAP_EVERY_US) {
            front_reap(server.server)
            last_reap = ref_time_ticks()
        }
    }
    if (rc == 0) {
        server->stop()
    }
    server->cleanup()
    unsafe {
        delete server
    }
    unsafe {
        delete g_fwires   // the writers died with the server
    }
    delete g_fgone
    delete g_fstatus
    delete g_fcache
    g_fto_engine = null
    delete g_fspeech
    if (to_speech != null) {   // let go before the answer below: the engine removes this channel once it has it
        g_fto_speech = null
        to_speech |> release
    }
    to_engine |> push_clone(FrontReq(kind = FrontKind.stopped))
    // the pending count each channel was created with is the front's life: dropping it with the reference, in one
    // step, is what the engine's join waits for before it removes the channel
    from_engine |> notify_and_release
    to_engine |> notify_and_release
}

// ===== The engine's side of a request =====

//! the upload the front saved for a request its handler refused: nobody else removes it
def private drop_upload(req : FrontReq) {
    if (!empty(req.file_path)) {
        remove(req.file_path)
    }
}

//! a buffered handler's reply on its wire
def private reply_on(wid : int64; resp : Reply) {
    respond(g_link, WriterRef(id = wid), resp.status, resp.ctype, resp.body)
}

def private operator_route(route : Route) : bool
    => (route == Route.config_save || route == Route.bench_start || route == Route.bake_start
        || route == Route.catalog_download || route == Route.restart || route == Route.shutdown || route == Route.gc)

def private note_drain_request(what : string) {
    if (!g_shutdown_requested) {
        to_log(
            LOG_INFO,
            "dasllama-server: {what} requested; draining llm_active={total_active()}, llm_queued={total_queued()}, asr_pending={g_asr_pending}, tts_pending={g_tts_pending}, media_pending={g_media_pending}\n")
    }
}

//! a buffered route's handler, run on the engine
def private buffered_route(req : FrontReq; var resp : Reply) : http_status {   // nolint:STYLE037 — the route table's engine half: one arm per endpoint
    if (operator_route(req.route) && !is_loopback_req(req)) return deny_operator(resp)
    if (req.route == Route.models || req.route == Route.stats || req.route == Route.streams) {
        refresh_speech_snapshot()
    }
    if (req.route == Route.models) return handle_models(resp)
    if (req.route == Route.embeddings) {
        if (g_shutdown_requested) {
            return resp |> JSON(
                error_body("the server is draining for shutdown — retry shortly", "server_error"),
                http_status.SERVICE_UNAVAILABLE)
        }
        return handle_embeddings(req, resp)
    }
    if (req.route == Route.activate) return handle_activate(req, resp)
    if (req.route == Route.load) return handle_model_load(req, resp)
    if (req.route == Route.unload) return handle_model_unload(req, resp)
    if (req.route == Route.stats) return handle_stats(resp)
    if (req.route == Route.streams) return handle_streams(resp)
    if (req.route == Route.config_get) return handle_config_get(resp)
    if (req.route == Route.vad) return handle_vad(req, resp)
    if (req.route == Route.config_save) return handle_config_save(req, resp)
    if (req.route == Route.exchange_get) return handle_exchange_get(req, resp)
    if (req.route == Route.exchange_matches) return handle_exchange_matches(req, resp)
    if (req.route == Route.exchange_apply) return handle_exchange_apply(req, resp)
    if (req.route == Route.exchange_submit) return handle_exchange_submit(req, resp)
    if (req.route == Route.exchange_retune) return handle_exchange_retune(req, resp)
    if (req.route == Route.exchange_consent) return handle_exchange_consent(req, resp)
    if (req.route == Route.bench_start) return handle_bench_start(resp)
    if (req.route == Route.bench_get) return handle_bench_get(resp)
    if (req.route == Route.bake_start) return handle_bake_start(req, resp)
    if (req.route == Route.bake_get) return handle_bake_get(resp)
    if (req.route == Route.catalog) {
        return resp |> JSON(catalog_state_json(g_catalog_dir, g_box_ram_gb, gpu_weight_budget_bytes() / (1024l * 1024l), @@tts_needs_packs))
    }
    if (req.route == Route.catalog_download) return handle_catalog_download(req, resp)
    if (req.route == Route.images) return handle_images(resp)
    if (req.route == Route.restart) {
        note_drain_request("restart")
        g_shutdown_requested = true
        g_restart_requested = true
        return resp |> JSON("\{\"ok\":true,\"draining\":true,\"restarting\":true}", http_status.OK)
    }
    if (req.route == Route.shutdown) {
        note_drain_request("graceful shutdown")
        g_shutdown_requested = true
        return resp |> JSON("\{\"ok\":true,\"draining\":true}", http_status.OK)
    }
    if (req.route == Route.gc) {
        let coalesced = g_gc_requested
        g_gc_requested = true
        to_log(LOG_INFO, "dasllama-server: GC requested via POST /gc (coalesced={coalesced})\n")
        return resp |> JSON(
            "\{\"ok\":true,\"scheduled\":true,\"coalesced\":{coalesced}}",
            http_status.ACCEPTED)
    }
    return handle_unknown(req, resp)
}

//! one message from the front, on the engine: a dropped client is noted, a request runs its handler
def private engine_dispatch(req : FrontReq) {
    if (req.kind == FrontKind.gone) {
        g_gone |> insert(req.wid)
        return
    }
    return if (req.kind != FrontKind.request)
    let writer = WriterRef(id = req.wid)
    if (req.route == Route.chat) {
        handle_chat(g_link, req, writer)
    } elif (req.route == Route.completion) {
        handle_completion(g_link, req, writer)
    } elif (speech_route(req.route)) {
        speech_dispatch(req, writer)
    } else {
        var resp = Reply()
        buffered_route(req, resp)
        reply_on(req.wid, resp)
    }
}

//! every message the front has queued, in order; `wait_ms` > 0 blocks that long for the first one
//! one speech request: on the speech thread when the server runs one, else on the engine (no speech model: the refusals)
def private speech_dispatch(req : FrontReq; writer : WriterRef) {
    if (req.route == Route.speech) {
        queue_speech(g_link, req, writer)
    } elif (req.route == Route.phonemes) {
        queue_phonemes(g_link, req, writer)
    } else {
        queue_transcription(g_link, req, writer, req.route == Route.translate)
    }
}

def private drain_front(wait_ms : int) {
    return if (g_front_in == null)
    var first = wait_ms
    var more = true
    while (more) {
        more = g_front_in |> pop_with_timeout_clone(first) $(req : FrontReq#) {
            var copy : FrontReq
            copy := req
            engine_dispatch(copy)
        }
        first = 0
    }
}

// Lifecycle-owned state. Keeping it global is required by both daslang-live (which calls
// init/update/shutdown independently) and standalone JIT heap collection: collectable state that
// survives an update must be visible to the collector as a global root, never a main-loop local.
var private g_front_up = false   //! the front thread is running: its channels are live
var private g_front_stopped = false   //! the front answered its stop: its channels are the engine's to remove
var private g_front_launched = false  //! a front thread was started on the channels; a boot that failed before it holds them alone
var private g_server_started = false
var private g_jobque_owned = false
var private g_prev_mtp_spec = false
var private g_mtp_spec_changed = false

let private FRONT_START_WAIT_MS = 30000
let private FRONT_STOP_WAIT_MS = 10000

//! Start the front thread on `port` and wait for it to listen: its start rc, or -1 when it never answered.
def private start_front(port : int) : int {
    g_front_launched = true
    var to_engine = g_front_in
    var from_engine = g_link.out
    if (g_speech_up) {
        var to_speech = g_speech_in
        new_thread() <| @capture(= port, = to_engine, = from_engine, = to_speech) {
            front_main(port, to_engine, from_engine, to_speech)
        }
    } else {   // a capture takes a reference, and there is no channel to take one of
        new_thread() <| @capture(= port, = to_engine, = from_engine) {
            var no_speech : Channel?
            front_main(port, to_engine, from_engine, no_speech)
        }
    }
    g_front_up = true
    var rc = -1
    g_front_in |> pop_with_timeout_clone(FRONT_START_WAIT_MS) $(ev : FrontReq#) {
        if (ev.kind == FrontKind.ready) {
            rc = ev.status
        }
    }
    if (rc != 0) {
        stop_front()
    }
    return rc
}

//! Stop the front: its server goes down and it answers `stopped`. Its channels stay up until shutdown_server
//! removes them with the workers' - the front releases its own references after that answer.
def private stop_front() {
    return if (!g_front_up)
    g_link.out |> push_clone(FrontOut(op = FrontOp.stop))
    let stops = await_stopped(g_front_in, 1, int64(FRONT_STOP_WAIT_MS) * 1_000_000l) $(ev : FrontReq#) => ev.kind == FrontKind.stopped
    let stopped = stops == 1
    if (stopped) {   // the front's references go with its last two statements: wait them out
        g_link.out |> join
        g_front_in |> join
    }
    g_front_up = false
    g_front_stopped = stopped
    delete g_gone
    if (!stopped) {
        to_log(LOG_WARNING, "dasllama-server: the HTTP front did not stop in {FRONT_STOP_WAIT_MS} ms - its channels are left to it\n")
    }
}

// ===== The speech thread: the ASR and TTS models' intake, workers and answers, off the engine =====
// A handler that holds the engine - a model load, an embeddings forward - holds no speech request: the front hands the
// speech routes to this thread, whose workers answer through the front's channel. It is a clone of the engine's context,
// so it runs the engine's own speech code over its own copy of the speech state, and the engine reads that state off
// the snapshots it publishes.

//! what the speech thread starts from: the configured models and the dispatch its workers take
struct private SpeechBoot {
    asr : array<AsrSlot>
    tts : array<TtsSlot>
    asr_team : bool
    tts_team : bool
    asr_alias : string
    metal : MetalMode
}

//! the speech state the engine's status routes and its drain read
struct private SpeechSnapshot {
    stopped : bool               //! the last one: the workers are down, the thread is leaving
    failure : string             //! why the thread could not start its workers; it is leaving, and the boot fails with this
    answers : int64              //! the engine's query this one answers, 0 for one the thread sent on its own
    asr : array<AsrSlot>
    tts : array<TtsSlot>
    asr_recent : array<AsrJobInfo>
    asr_done_jobs : int64
    asr_audio_s : float
    asr_active : int
    asr_pending : int
    tts_pending : int
    tts_done_jobs : int64
    tts_audio_s : float
}

var private g_speech_in : Channel?     //! front and engine -> speech thread: FrontReq
var private g_speech_out : Channel?    //! speech thread -> engine: SpeechSnapshot
var private g_speech_up = false
var private g_speech_drain_sent = false
var private g_speech_query = 0l
let private SPEECH_QUERY_WAIT_MS = 50   //! a status answer waits this long for the speech thread's current state, then serves its last
let private SPEECH_START_WAIT_MS = 1000
let private SPEECH_SNAPSHOT_EVERY_US = 250000l   //! a running job's progress reaches the poll surface this often

//! the speech state as a snapshot
def private speech_snapshot(stopped : bool) : SpeechSnapshot {
    var snap = SpeechSnapshot(stopped = stopped, asr_done_jobs = g_asr_done_jobs, asr_audio_s = g_asr_audio_s,
        asr_active = g_asr_active, asr_pending = g_asr_pending, tts_pending = g_tts_pending,
        tts_done_jobs = g_tts_done_jobs, tts_audio_s = g_tts_audio_s)
    snap.asr := g_asr
    snap.tts := g_tts
    snap.asr_recent := g_asr_recent
    return <- snap
}

//! what a snapshot would change for a reader, as one number: the counters and the models' load state
def private speech_signature : int64 {
    var sig = g_asr_done_jobs * 31l + g_tts_done_jobs * 17l + int64(g_asr_active) * 7l + int64(g_asr_pending) * 5l + int64(g_tts_pending) * 3l + long_length(g_asr_recent)
    for (a in g_asr) {
        sig = sig * 3l + int64(a.ready)
    }
    for (m in g_tts) {
        sig = sig * 3l + int64(m.ready + (m.armed ? 2 : 0))
    }
    return sig
}

//! Take everything the speech thread's inbox holds, waiting 2 ms for the first message: the front's requests and
//! disconnects, the engine's drain word and its queries. True once the engine said halt.
def private speech_inbox(var inbox : Channel?&; var to_engine : Channel?&) : bool {
    var halt = false
    var more = true
    var wait = 2
    while (more && !halt) {
        more = inbox |> pop_with_timeout_clone(wait) $(ev : FrontReq#) {
            if (ev.kind == FrontKind.halt) {
                halt = true
            } elif (ev.kind == FrontKind.drain) {
                g_shutdown_requested = true
            } elif (ev.kind == FrontKind.gone) {
                g_gone |> insert(ev.wid)
            } elif (ev.kind == FrontKind.query) {   // in the order it arrived: everything answered before it is counted
                var now <- speech_snapshot(false)
                now.answers = ev.wid
                to_engine |> push_clone(now)
                delete now
            } elif (ev.kind == FrontKind.request) {
                var req : FrontReq
                req := ev
                speech_dispatch(req, WriterRef(id = req.wid))
                delete req
            }
        }
        wait = 0
    }
    return halt
}

def private speech_main(var boot : Channel?&; var inbox : Channel?&; var to_engine : Channel?&; var to_front : Channel?&) {
    this_context().name := "dasllama-speech"
    boot |> pop_with_timeout_clone(SPEECH_START_WAIT_MS) $(b : SpeechBoot#) {
        g_asr := b.asr
        g_tts := b.tts
        g_asr_team_dispatch = b.asr_team
        g_tts_team_dispatch = b.tts_team
        g_speech_metal = b.metal
        g_asr_alias = clone_string(b.asr_alias)
    }
    boot |> release
    g_link.out = to_front
    g_speech_up = false   // this context IS the speech thread: its own dispatch is the direct one
    g_shutdown_requested = false
    g_asr_events = worker_channel()
    g_tts_events = worker_channel()
    var failure = ""
    let failed = catch_worker_panic(failure) {   // an ASR model that cannot load panics its boot: here that is this thread, and the engine waits on its first snapshot
        start_asr_worker_threads()
        start_tts_worker()
    }
    var first <- speech_snapshot(failed)
    first.failure = failure
    to_engine |> push_clone(first)
    delete first
    var sent_sig = speech_signature()
    var sent_at = ref_time_ticks()
    var halt = failed
    while (!halt) {
        halt = speech_inbox(inbox, to_engine)
        drain_asr_events(g_link)
        drain_tts_events(g_link)
        reap_speech_wires(g_link)
        let sig = speech_signature()
        let busy = g_asr_active + g_asr_pending + g_tts_pending > 0
        if (sig != sent_sig || (busy && int64(get_time_usec(sent_at)) >= SPEECH_SNAPSHOT_EVERY_US)) {
            var snap <- speech_snapshot(false)
            to_engine |> push_clone(snap)
            delete snap
            sent_sig = sig
            sent_at = ref_time_ticks()
        }
    }
    stop_asr_worker_threads()
    stop_tts_worker()
    release_speech_state()
    if (!failed) {   // a failed start said so in its one snapshot
        var last <- speech_snapshot(true)
        to_engine |> push_clone(last)
        delete last
    }
    delete g_asr
    delete g_tts
    delete g_gone
    g_link.out = null
    to_front |> release
    inbox |> notify_and_release       // each with the channel's pending count, in one step: what the engine's join waits for
    to_engine |> notify_and_release
}

//! this context's speech wires, the synthesis files nobody collected, and its worker channels
def private release_speech_state {
    delete g_asr_wires
    delete g_tts_wires
    for (served in g_tts_served) {   // the synthesis temp files outlive the process otherwise: nothing else sweeps tmpdir
        remove_result(served.path)
    }
    delete g_tts_served
    var asr_events = g_asr_events
    var tts_events = g_tts_events
    g_asr_events = null
    g_tts_events = null
    for (jobs in g_asr_jobs) {
        remove_worker_channels(jobs, null)
    }
    for (jobs in g_tts_jobs) {
        remove_worker_channels(jobs, null)
    }
    remove_worker_channels(asr_events, tts_events)
    g_asr_jobs |> clear()   // the channels are gone: only the buffers are left to free
    g_tts_jobs |> clear()
    unsafe {
        delete g_asr_jobs
        delete g_tts_jobs
    }
    delete g_asr_recent
    g_asr_active = 0
    g_asr_pending = 0
    g_tts_pending = 0
}

//! the engine's copy of the speech state, off a snapshot
def private adopt_speech_snapshot(snap : SpeechSnapshot#) {
    g_asr := snap.asr
    g_tts := snap.tts
    g_asr_recent := snap.asr_recent
    g_asr_done_jobs = snap.asr_done_jobs
    g_asr_audio_s = snap.asr_audio_s
    g_asr_active = snap.asr_active
    g_asr_pending = snap.asr_pending
    g_tts_pending = snap.tts_pending
    g_tts_done_jobs = snap.tts_done_jobs
    g_tts_audio_s = snap.tts_audio_s
}

//! Start the speech thread when a speech model is configured, and wait for its models to load: its first snapshot is
//! their load state, which the boot's own checks read. A server with no speech model runs none - the engine answers
//! the speech routes' refusals itself.
def private start_speech_thread {
    return if (empty(g_asr) && empty(g_tts))
    g_speech_in = worker_channel()
    g_speech_out = worker_channel()
    var boot = worker_channel()
    g_asr_alias = empty(g_slots) ? "" : "{g_slots[g_default_slot].name}-asr"
    var first = SpeechBoot(asr_team = g_asr_team_dispatch, tts_team = g_tts_team_dispatch, asr_alias = g_asr_alias, metal = get_metal_mode())
    first.asr := g_asr
    first.tts := g_tts
    boot |> push_clone(first)
    delete first
    var inbox = g_speech_in
    var to_engine = g_speech_out
    var to_front = g_link.out
    new_thread() <| @capture(= boot, = inbox, = to_engine, = to_front) {
        speech_main(boot, inbox, to_engine, to_front)
    }
    var loaded = false
    var failure = ""
    while (!loaded) {   // the loads take as long as they take; a worker that cannot load answers too
        g_speech_out |> pop_with_timeout_clone(1000) $(snap : SpeechSnapshot#) {
            adopt_speech_snapshot(snap)
            failure = clone_string(snap.failure)
            loaded = true
        }
    }
    unsafe(channel_remove(boot))   // the thread released it after its pop, ahead of its first snapshot
    if (!empty(failure)) {   // the thread is leaving: wait it out, give its channels back, and fail the boot as it would have
        g_speech_in |> join
        g_speech_out |> join
        remove_worker_channels(g_speech_in, g_speech_out)
        g_speech_in = null
        g_speech_out = null
        panic(failure)
    }
    g_speech_up = true
    g_speech_drain_sent = false
}

//! the speech thread's news since the last tick, and the drain word once the server starts draining
def private drain_speech_snapshots {
    return if (!g_speech_up)
    var more = true
    while (more) {
        more = g_speech_out |> pop_with_timeout_clone(0) $(snap : SpeechSnapshot#) {
            adopt_speech_snapshot(snap)
        }
    }
    if (g_shutdown_requested && !g_speech_drain_sent) {
        g_speech_in |> push_clone(FrontReq(kind = FrontKind.drain))
        g_speech_drain_sent = true
    }
}

//! The speech state as of now, for a status answer: a request the speech thread answered before this call is counted
//! in what the engine reads next. The thread takes its inbox in order, so its answer to the query follows every
//! request ahead of it; a thread too busy to answer inside the wait leaves the engine its last snapshot.
def private refresh_speech_snapshot {
    return if (!g_speech_up)
    g_speech_query++
    let asked = g_speech_query
    g_speech_in |> push_clone(FrontReq(kind = FrontKind.query, wid = asked))
    var answered = false
    let since = ref_time_ticks()
    while (!answered && get_time_usec(since) / 1000 < SPEECH_QUERY_WAIT_MS) {
        g_speech_out |> pop_with_timeout_clone(5) $(snap : SpeechSnapshot#) {
            adopt_speech_snapshot(snap)
            answered = snap.answers == asked
        }
    }
}

//! Stop the speech thread: its workers go down and its last snapshot says so. The front is down already, so nothing
//! else holds its two channels once it has let go of them.
def private stop_speech_thread {
    return if (!g_speech_up)
    g_speech_in |> push_clone(FrontReq(kind = FrontKind.halt))
    let stops = await_stopped(g_speech_out, 1, WORKER_STOP_WAIT_NS * 2l) $(snap : SpeechSnapshot#) => snap.stopped
    let stopped = stops == 1
    g_speech_up = false
    if (stopped) {
        g_speech_in |> join
        g_speech_out |> join
        remove_worker_channels(g_speech_in, g_speech_out)
    } else {
        to_log(LOG_WARNING, "dasllama-server: the speech thread did not stop - its channels are left to it\n")
    }
    g_speech_in = null
    g_speech_out = null
}

//! Initialize the server lifecycle. Creates a persistent JobQue explicitly so the pool spans
//! externally-driven update calls; ``shutdown_server`` destroys it after every worker and owned
//! queue object has stopped. Call after ``set_model``.
//! ``max_streams`` concurrent generation streams, ``max_queue`` waiting requests (503 beyond),
//! ``chunk_tokens`` the prefill quantum while a stream decodes (decode stalls at most this many
//! tokens per tick) and ``idle_chunk_tokens`` its floor while none does; a ``chunk_tokens`` of 0 and
//! an ``idle_chunk_tokens`` of -1 take the serving backend's defaults (``chunk_defaults``).
//! ``page_rows`` > 0 (the default) serves PAGED: KV memory tracks actual context and a prefix
//! cache reuses completed streams' pages, so repeated prompts (system prompts, multi-turn chats)
//! skip their prefill; ``prefix_groups`` caps the cache (0 = auto: one full context per stream
//! slot, < 0 = unbounded). ``page_rows`` = 0 restores flat preallocated sessions (each the full
//! ``--ctx`` slab). ``kv_dtype`` picks the KV-cache codec for every stream (pool-wide when paged;
//! see ``KVDtype`` — ``tq4`` needs a power-of-two head_size).
// created here, torn down together by shutdown_server's channel_remove block
def private init_worker_rails {
    g_bench_events = worker_channel()
    g_bench_state = "idle"
    g_bench_result_json = ""
    g_bake_events = worker_channel()
    g_bake_state = "idle"
    g_bake_result_json = ""
    catalog_events_init()
    start_speech_thread()
    start_media_worker()
}

// The boot-time scheduler knobs, kept for slots loaded at RUNTIME (POST /v1/models/load builds
// its scheduler with the same defaults every boot slot got).
struct private SchedDefaults {
    max_streams : int64
    max_queue : int64
    chunk_tokens : int64
    idle_chunk_tokens : int64
    page_rows : int64
    prefix_groups : int64
    kv_dtype : KVDtype
    mtp : int   //! -1 auto (`mtp_auto_arms`) | 0 off | 1 on
}

var private g_sched_defaults = SchedDefaults()

//! Whether an unset `mtp` arms a slot: one stream whose decode runs on a GPU - the Metal flavor, or the whole model resident on a Vulkan device (`backend_effective` "gpu:resident"). On the CPU, on a slot the device serves in part, and where the resident driver sums the routed experts on the host (`experts_host`: a hyper-connection MoE the card does not hold whole - `PERF_LEDGER.md`'s NextN entry), the verify round costs more than the accepted drafts return. Non-private: test_openai_server_mtp.das gates it.
def mtp_auto_arms(streams : int64; on_metal : bool; backend_effective : string; experts_host : bool = false) : bool
    => streams == 1l && decodes_on_gpu(on_metal, backend_effective, experts_host)

//! Whether a slot's decode runs on a GPU, the worker team left idle: the Metal flavor, or the whole model resident on a Vulkan device with its routed experts on the card. Non-private: test_server_flags.das gates it.
def decodes_on_gpu(on_metal : bool; backend_effective : string; experts_host : bool = false) : bool
    => on_metal || (backend_effective == "gpu:resident" && !experts_host)

//! Whether an unset ASR dispatch publishes to the worker team: every LLM slot decodes on a GPU, so nothing else publishes there and a transcription runs at twice its inline rate. Beside a slot that decodes on the CPU the team is that slot's, and the worker runs inline. Non-private: test_server_flags.das gates it.
def asr_auto_team(llm_slots, gpu_slots : int) : bool
    => gpu_slots == llm_slots

//! Which speech workers publish to the worker team, by `init_server`'s `asr_team`: 1 puts both on it, 0 neither, -1 the ASR workers where `asr_auto_team` says so. Non-private: test_server_flags.das gates it.
def speech_team_dispatch(team_dispatch : bool; asr_team, llm_slots, gpu_slots : int) : tuple<asr : bool; tts : bool>
    => (asr = team_dispatch && (asr_team > 0 || (asr_team < 0 && asr_auto_team(llm_slots, gpu_slots))), tts = team_dispatch && asr_team > 0)

def private gpu_decoding_slots : int {
    let experts_host = gpu_resident_experts_host()
    var n = 0
    for (s in g_slots) {
        if (decodes_on_gpu(slot_flavor(s) == "metal", s.backend_effective, experts_host)) {
            n++
        }
    }
    return n
}

//! A model served from its image alone has no CPU stack to fall to, so its context ends where the device
//! holds a session's K/V: `seq_len` is cut to `device_kv_max_rows` under the `kdt` codec, with a warning.
def clamp_ctx_to_device_room(var m : Model; name : string; kdt : KVDtype) {
    return if (!m.metal_blob)
    let room = device_kv_max_rows(m, kdt, kdt)
    if (room < m.config.seq_len) {
        to_log(LOG_WARNING, "dasllama-server: {name} ctx {m.config.seq_len} -> {room}: the device KV mirror holds that many rows of this model under DASLLAMA_METAL_KV_MIRROR_MB (hard max 4096 MB)\n")
        m.config.seq_len = room
    }
}

//! the K/V codec a slot's scheduler runs on: the slot's own where one was asked, else the server default
def slot_kv_dtype(opts : SlotOpts) : KVDtype => opts.kv_dtype_set ? opts.kv_dtype : g_sched_defaults.kv_dtype

//! the server-wide K/V codec a slot that asked for none inherits
def sched_default_kv_dtype() : KVDtype => g_sched_defaults.kv_dtype

def private init_one_scheduler(var s : ModelSlot) {
    let s_streams = s.opts.streams >= 0l ? s.opts.streams : g_sched_defaults.max_streams
    let s_chunk = s.opts.chunk >= 0l ? s.opts.chunk : g_sched_defaults.chunk_tokens
    let s_idle = s.opts.chunk_idle != -2l ? s.opts.chunk_idle : g_sched_defaults.idle_chunk_tokens
    let s_pages = s.opts.page_rows != -2l ? s.opts.page_rows : g_sched_defaults.page_rows
    var pfx = s.opts.prefix != -2l ? s.opts.prefix : g_sched_defaults.prefix_groups
    if (s_pages > 0l && pfx == 0l) {   // auto: retain up to one full context per stream slot
        pfx = s_streams * ((s.model.config.seq_len + s_pages - 1l) / s_pages)
    }
    if (pfx < 0l) {
        pfx = 0l   // explicit unbounded
    }
    let s_kv = slot_kv_dtype(s.opts)
    let want = s.opts.mtp >= 0 ? s.opts.mtp : g_sched_defaults.mtp
    var spec = want > 0 || (want < 0 && mtp_auto_arms(s_streams, slot_flavor(s) == "metal", s.backend_effective, gpu_resident_experts_host()))
    if (spec && s.model.config.n_layer_nextn <= 0l) {
        let side = mtp_drafter_sidecar(s.path)
        if (!empty(side) && attach_mtp_drafter(s.model, side)) {
            to_log(LOG_INFO, "dasllama-server: {s.name} speculates through the assistant drafter {base_name(side)}\n")
        } elif (want > 0) {
            to_log(LOG_WARNING, "dasllama-server: mtp requested for {s.name} but the model has no NextN head and no assistant drafter beside it — self-spec stays off\n")
            spec = false
        } else {
            to_log(LOG_INFO, "dasllama-server: {s.name} has no NextN head and no assistant drafter beside it — the single-stream self-spec default stays off\n")
            spec = false
        }
    }
    s.sched <- create_scheduler(s.model, s_streams, g_sched_defaults.max_queue, s_chunk, s_pages, pfx, s_kv, spec, s_idle)
    slot_default_chunk(s)
}

//! Whether a GPU prefills a slot: the Metal flavor, or a Vulkan tier arm of either kind - what picks the slot's default chunk. Non-private: test_server_flags.das gates it.
def prefills_on_gpu(on_metal : bool; backend_effective : string) : bool
    => on_metal || backend_effective != "cpu"

//! A slot's unset chunk follows the backend that prefills THIS slot now. The scheduler's own default reads the
//! tier's one process-wide status, which beside another slot's armed model is that model's answer.
def private slot_default_chunk(var s : ModelSlot) {
    let set_chunk = s.opts.chunk >= 0l ? s.opts.chunk : g_sched_defaults.chunk_tokens
    if (set_chunk <= 0l) {
        s.sched.chunk_tokens = chunk_default(prefills_on_gpu(slot_flavor(s) == "metal", s.backend_effective))
    }
}

// one scheduler per slot: the init args are the defaults, SlotOpts sentinels override per slot
def private init_slot_schedulers(max_streams, max_queue, chunk_tokens, idle_chunk_tokens, page_rows, prefix_groups : int64; kv_dtype : KVDtype; mtp : int) {
    g_sched_defaults = SchedDefaults(max_streams = max_streams, max_queue = max_queue,
                                     chunk_tokens = chunk_tokens, idle_chunk_tokens = idle_chunk_tokens, page_rows = page_rows,
                                     prefix_groups = prefix_groups, kv_dtype = kv_dtype, mtp = mtp)
    for (s in g_slots) {
        init_one_scheduler(s)
    }
}

//! `mtp`: -1 lets each slot decide (self-speculation on when it runs one stream on a GPU, `mtp_auto_arms`), 0 / 1 set it
//! `asr_team`: -1 lets the slots decide whether the ASR workers publish to the worker team (`asr_auto_team`), 0 runs them inline, 1 puts them and the TTS worker on the team
def init_server(port : int; max_streams : int64 = 4l; max_queue : int64 = 32l; chunk_tokens : int64 = 0l;
                page_rows : int64 = 64l; prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
                team_dispatch : bool = true; asr_team : int = -1; mtp : int = -1;
                idle_chunk_tokens : int64 = -1l) : bool {
    if (g_front_up || g_jobque_owned) {
        to_log(LOG_ERROR, "dasllama-server: init_server called while already initialized\n")
        return false
    }
    if (empty(g_slots) && !g_setup_mode && !server_has_speech()) {
        to_log(LOG_ERROR, "dasllama-server: init_server with no model slots (call set_model/add_model first, or add_tts_model / add_asr_model for a speech-only server)\n")
        return false
    }
    g_shutdown_requested = false   // reset so a restart in the same context after /shutdown doesn't drain at once
    g_restart_requested = false    // NOT reset in shutdown_server — the lifecycle host reads it after
    g_gc_requested = false
    create_job_que()
    g_jobque_owned = true
    setup_dasllama_jobque()
    set_jobque_team_mode(team_dispatch)
    let speech = speech_team_dispatch(team_dispatch, asr_team, length(g_slots), gpu_decoding_slots())
    g_tts_team_dispatch = speech.tts
    g_asr_team_dispatch = speech.asr
    let dispatch_mode = !team_dispatch ? "inline" : (asr_team > 0 ? "team" : "hybrid")
    to_log(LOG_INFO, "dasllama-server: jobque compute dispatch = {dispatch_mode}, ASR workers {g_asr_team_dispatch ? "on the team" : "inline"}\n")
    g_front_in = worker_channel()   // the front starts last, but a speech thread answers through its channel from its first request
    g_link.out = worker_channel()
    g_asr_events = worker_channel()
    g_tts_events = worker_channel()
    g_media_jobs = worker_channel()
    g_media_events = worker_channel()
    init_worker_rails()
    if (empty(g_slots)) {
        if (!g_setup_mode && !server_has_speech()) {
            // the speech-only boot's worker dropped the route as it loaded: the catalog/config
            // page is all that is left to serve, and it is where the config gets fixed
            to_log(LOG_WARNING, "dasllama-server: the speech route did not arm and no model is loaded - falling back to setup mode\n")
            g_setup_mode = true
        }
        to_log(LOG_INFO, g_setup_mode ? "dasllama-server: setup mode - serving the catalog/config page only\n" : "dasllama-server: no LLM slot - serving the speech route and the catalog only\n")
    }
    init_slot_schedulers(max_streams, max_queue, chunk_tokens, idle_chunk_tokens, page_rows, prefix_groups, kv_dtype, mtp)
    g_active = g_default_slot
    g_cur = g_default_slot
    g_prev_mtp_spec = get_mtp_spec()
    g_mtp_spec_changed = true
    // engine gate: prefills maintain the draft head's KV (chunked included); follows the stepped
    // slot on switch (update_server); restored at exit
    if (!empty(g_slots)) {
        set_mtp_spec(g_slots[g_active].sched.mtp)
    }
    // a resident prefill would strand device-only rows when host-cached sessions outnumber the driver's K/V regions; device-home streams prefill on the device regardless
    set_resident_prefill_allowed(false)
    let rc = start_front(port)   // 0 on success; non-zero = bind/listen failed (e.g. port in use)
    if (rc != 0) {
        to_log(LOG_ERROR, "dasllama-server: could not start on port {port} (start rc {rc}; is the port already in use?)\n")
        return false
    }
    g_server_started = true
    g_started_at = ref_time_ticks()
    g_tickprof = env_noisy_diag()
    compose_hardware_line()
    g_box_ram_gb = box_ram_gb()
    var names <- [for (s in g_slots); s.is_default && length(g_slots) > 1 ? "{s.name}*" : s.name]
    let kv_mode = page_rows > 0l ? "{kv_dtype} paged P={page_rows}" : "{kv_dtype} flat"
    let chunk_note = empty(g_slots) ? "none" : "{g_slots[g_active].sched.chunk_tokens}, {g_slots[g_active].sched.idle_chunk_tokens} idle"
    to_log(LOG_INFO, "dasllama-server listening on http://127.0.0.1:{port}/v1  (models: {join(names, ", ")}; streams: {max_streams}, prefill chunk: {chunk_note}, kv: {kv_mode})\n")
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
var private g_tickprof = false
var private g_prof_win = ref_time_ticks()
var private g_prof_ticks = 0l
var private g_prof_busy = 0l
var private g_prof_pump_us = 0l
var private g_prof_drain_us = 0l
var private g_prof_reap_us = 0l
var private g_prof_step_us = 0l
var private g_prof_flush_us = 0l

def private prof_window_reset() {
    g_prof_win = ref_time_ticks()
    g_prof_ticks = 0l
    g_prof_busy = 0l
    g_prof_pump_us = 0l
    g_prof_drain_us = 0l
    g_prof_reap_us = 0l
    g_prof_step_us = 0l
    g_prof_flush_us = 0l
}

def private prof_window_report() {
    if (!g_tickprof) {
        return
    }
    let win_us = int64(get_time_usec(g_prof_win))
    if (win_us < 5_000_000l) {
        return
    }
    let n = g_prof_ticks
    if (g_prof_busy > 0l && n > 0l) {
        to_log(LOG_INFO, "dasllama-server: [tickprof] {n} ticks ({g_prof_busy} busy) in {win_us / 1000l}ms | per-tick us: pump={g_prof_pump_us / n} drain={g_prof_drain_us / n} reap={g_prof_reap_us / n} step={g_prof_step_us / n} flush={g_prof_flush_us / n} | sandwich total {(g_prof_pump_us + g_prof_drain_us + g_prof_reap_us + g_prof_flush_us) / 1000l}ms vs step {g_prof_step_us / 1000l}ms\n")
    }
    prof_window_reset()
}

def update_server() : bool {
    if (!g_server_started || !g_front_up) {
        return false
    }
    let t_pump = ref_time_ticks()
    drain_front(0)   // the requests the front queued: parse + render + submit here
    if (g_tickprof) {
        g_prof_pump_us += int64(get_time_usec(t_pump))
    }
    let t_drain = ref_time_ticks()
    drain_speech_snapshots()
    drain_asr_events(g_link)
    drain_tts_events(g_link)
    drain_media_events(g_link)
    drain_bench_events()
    if (g_shutdown_requested && ibench_running()) {
        ibench_stop("idle", "the server is shutting down")   // a benchmark holds nothing worth draining for, and no rep runs on the tick that learns it
    }
    ibench_tick()
    drain_bake_events()
    catalog_download_tick()
    let t_reap = ref_time_ticks()
    if (g_tickprof) {
        g_prof_drain_us += int64(get_time_usec(t_drain))
    }
    reap_disconnected(g_link)
    if (g_tickprof) {
        g_prof_reap_us += int64(get_time_usec(t_reap))
    }
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
        sync_slot_device_kv(g_slots[si], si == g_gpu_owner)
        let t_step = ref_time_ticks()
        busy = scheduler_step(g_slots[si].model, g_slots[si].sched, g_events)
        let step_us = int64(get_time_usec(t_step))
        let step_ms = step_us / 1000l
        if (g_tickprof) {
            g_prof_step_us += step_us
        }
        if (step_ms > 3000l) {
            // the "why does it pause" catcher: one decode step / prefill chunk should be
            // well under a second — anything slower means contention or a stall
            to_log(LOG_WARNING, "dasllama-server: slow scheduler step: {step_ms}ms ({g_slots[si].name}: active={n_active(g_slots[si].sched)}, queued={n_queued(g_slots[si].sched)})\n")
        }
    }
    let t_flush = ref_time_ticks()
    flush_events(g_link)
    flush_tool_keepalives(g_link)
    if (g_tickprof) {
        g_prof_flush_us += int64(get_time_usec(t_flush))
        g_prof_ticks++
        if (busy) {
            g_prof_busy++
        }
        prof_window_report()
    }
    if (g_shutdown_requested && total_active() == 0l && total_queued() == 0l &&
        g_asr_pending == 0 && g_tts_pending == 0 && g_media_pending == 0) {
        to_log(LOG_INFO, "dasllama-server: graceful shutdown drain complete\n")
        return false
    }
    if (!busy) {
        drain_front(2)   // idle: block on the front's channel, so a request starts as it arrives
    }
    return true
}

// a stopped worker's channel pair: both released, either may already be null
def private remove_worker_channels(var jobs : Channel?; var events : Channel?) {
    unsafe {
        if (jobs != null) {
            jobs |> channel_remove
        }
        if (events != null) {
            events |> channel_remove
        }
    }
}

//! Release every lifecycle-owned object in dependency order. Safe after a partially failed init.
def shutdown_server() {   // nolint:STYLE038 — one ordered teardown: workers stop before their channels, VRAM drops before the slots, the jobque dies last
    if (ibench_running()) {
        ibench_stop("idle", "the server stopped")   // the rows' cache goes before the model it was sized from
    }
    stop_front()   // the sockets go first: no request arrives while the workers stop
    stop_speech_thread()
    g_server_started = false
    if (g_asr_events != null) {
        stop_asr_worker_threads()
    }
    if (g_tts_events != null) {
        stop_tts_worker()
    }
    if (g_media_jobs != null && g_media_events != null) {
        stop_media_worker()
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
    release_speech_state()   // the engine's own: empty rails on a server whose speech thread ran them
    for (w in values(g_media_wires)) {   // parked image requests die with the server
        unsafe {
            delete w.js
        }
        w.js = null
    }
    delete g_media_wires
    var vision_jobs = g_media_jobs
    var vision_events = g_media_events
    var bench_events = g_bench_events
    var bake_events = g_bake_events
    let front_let_go = g_front_stopped || !g_front_launched
    var front_in = front_let_go ? g_front_in : null
    var front_out = front_let_go ? g_link.out : null
    g_front_in = null
    g_link.out = null
    g_front_stopped = false
    g_front_launched = false
    g_media_jobs = null
    g_media_events = null
    g_bench_events = null
    g_bake_events = null
    catalog_events_shutdown()
    unsafe {
        remove_worker_channels(vision_jobs, vision_events)
        remove_worker_channels(front_in, front_out)
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
    delete g_asr
    delete g_tts
    g_media_pending = 0
    if (g_jobque_owned) {
        destroy_job_que()
        g_jobque_owned = false
    }
    g_shutdown_requested = false
}

//! Compatibility wrapper for tests and simple callers. Lifecycle hosts should call
//! ``init_server`` / ``update_server`` / ``shutdown_server`` directly.
def run_server(port : int; max_streams : int64 = 4l; max_queue : int64 = 32l; chunk_tokens : int64 = 0l;
               page_rows : int64 = 64l; prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
               team_dispatch : bool = true; asr_team : int = -1; mtp : int = -1;
               idle_chunk_tokens : int64 = -1l) {
    if (init_server(port, max_streams, max_queue, chunk_tokens, page_rows, prefix_groups, kv_dtype,
            team_dispatch, asr_team, mtp, idle_chunk_tokens)) {
        while (update_server()) {
        }
    }
    shutdown_server()
}
