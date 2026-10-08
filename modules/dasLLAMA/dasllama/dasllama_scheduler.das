options gen2
options _dasllama_internal = true

module dasllama_scheduler shared public

require dasllama/dasllama_lint public
require dasllama/dasllama   // the public facade — the scheduler reaches no engine internals
require math
require strings             // tail bookkeeping for the poll surface (length / slice / peek_data)
require daslib/utf8_utils   // is_first_byte_of_utf8_char: codepoint-safe tail/head trims

// Continuous-batching scheduler over the dasLLAMA facade. One thread, synchronous: each
// scheduler_step admits queued requests, runs ONE eval_batch decode step over every decoding
// stream (decode priority), then at most ONE bounded prefill chunk FCFS — a long prompt stalls
// decode by T_chunk, not T_prefill. Results flow out as SchedEvents; no HTTP here — the wiring
// (openai_server) owns the writers, so the whole scheduler unit-tests with zero network.

//! Stream lifecycle: prefilling (prompt chunks pending) -> decoding (one token per step) -> done.
enum StreamState {
    prefilling
    decoding
    done
}

//! What a SchedEvent reports: a generated piece, or the stream's completion.
enum SchedEventKind {
    piece
    finished
}

//! One scheduler output: a text piece for a stream, or its completion (with usage + timing).
//! The wiring dispatches these to HTTP writers; tests assert on them directly.
struct SchedEvent {
    stream : int64                          // Stream/PendingReq id
    kind : SchedEventKind = SchedEventKind.piece
    token : int64 = -1l                     // piece: the sampled token id
    text : string = ""                      // piece: its decoded text
    finish : string = ""                    // finished: "stop" | "length" | "evicted"
    n_prompt : int64 = 0l                   // finished: prompt tokens prefilled
    n_gen : int64 = 0l                      // finished: tokens generated
    n_cached : int64 = 0l                   // finished: prefix-cache hit length (tokens attached at admit)
    ttft_us : int64 = 0l                    // finished: admit -> first sampled token (microseconds)
    gen_us : int64 = 0l                     // finished: first sampled token -> finish (microseconds)
}

//! One media span of a prompt: `n` positions from prompt index `at`, each entry a `media_position_id` of the media's
//! content key, so the prefix cache matches across the span as across text. `rows` are the span's soft tokens (dim-wide
//! audio, dim*(1+n_deepstack)-wide images); a request leaves them empty for a span it counts on the cache to hold.
struct MediaSpan {
    at : int64
    n : int64
    non_causal : bool                       // the rows attend each other in full (an image span); audio is causal
    grid : int2                             // qwen mrope: the merged image grid roping the rows; (0,0) = sequential positions
    rows : array<float>
    done : bool                             // the span's K/V stands: attached off the prefix cache, or evaled
}

//! A parsed + rendered request awaiting admission: tokens only, NO session — queued requests hold
//! no KV memory. The caller renders (create_chat_renderer / render_assistant / render_turn, or a
//! raw encode) and assigns unique ids.
struct PendingReq {
    id : int64
    prompt : array<int64>                   // the full rendered prompt
    close_toks : array<int64>               // the turn-terminating tokens (render_close; evaled at donation so the next turn extends the cached prefix)
    stop_ids : array<int64>                 // sampling one of these finishes "stop" (the token is not emitted)
    stop_strs : array<string>               //! a reply that reaches one of these finishes "stop" where it begins; the text that could still begin one is held back (`stop_scan`)
    nothink : NothinkStopGuard = NothinkStopGuard()   // the instruct-mode channel-marker guard (`make_nothink_guard`); unarmed = every token passes
    params : SamplingParams = SamplingParams()
    max_new : int64 = 256l                  // reply token budget (>= 1)
    seed : int = 0                          // != 0 => set_seed at admit (reproducible sampling)
    t_submit : int64 = 0l                   // ref_time_ticks at submit (queue-wait logging; 0 = unset)
    stable_at : int64 = 0l                  //! prompt tokens the request's own last turn does not reach (`render_turn_marked`): where the requests sharing this history part; a recurrent model's prefill leaves a checkpoint there (0 = unknown)
    system_at : int64 = 0l                  //! prompt tokens of the conversation's opening every request on this system prompt and tool set shares (`ChatSession.first_opening`); a recurrent model's prefill leaves a second checkpoint there when it sits a page or more under `stable_at` (0 = unknown)
    system_head_at : int64 = 0l             //! the opening's tokens shared beyond this system prompt and tool set - its system text or its tool block, whichever the template writes first (`ChatSession.system_head_at`): a third checkpoint, a page or more under `system_at` (0 = unknown)
    // MEDIA splice (images today; audio rides the same fields): `n_media` embedding rows that
    // prefill BETWEEN prompt[media_at-1] and prompt[media_at] — the render seam hands the two
    // token spans and the encoder hands the rows. Zero rows = an ordinary text request.
    media_embd : array<float>               // n_media rows: dim-wide (audio), dim*(1+n_deepstack)-wide (images — what encode_image emits)
    n_media : int64
    media_at : int64                        // prompt index the rows splice at (0 <= media_at <= |prompt|)
    media_non_causal : bool                 // the rows attend each other in full (gemma vision); audio is causal
    media_grid : int2                       // qwen mrope: the merged image grid (nx, ny) roping the rows; (0,0) = sequential positions (gemma, audio)
    spans : array<MediaSpan>                //! media already INLINE in `prompt` as position ids (a transcript renderer's form); `submit` folds the splice fields above into one
    constraint : TokenConstraintBox         //! the token admission the stream samples under (null = unconstrained); the stream takes it over at admission, an evicted request frees it
}

//! One active generation stream: the owned KV session plus its request state. Managed wholly by
//! the scheduler; readable for stats and tests.
struct Stream {
    id : int64
    session : Session
    params : SamplingParams = SamplingParams()
    prompt : array<int64>
    close_toks : array<int64>
    stop_ids : array<int64>
    stop_strs : array<string>
    stop_hold : string = ""                 //! generated text not yet emitted: it could still begin a stop string
    nothink : NothinkStopGuard = NothinkStopGuard()
    n_prefilled : int64                     // prompt positions whose K/V stands: attached or evaled
    spans : array<MediaSpan>                // the prompt's media spans, in prompt order (see PendingReq)
    n_media : int64                         // media positions in the prompt, every span's
    max_new : int64
    n_gen : int64                           // tokens emitted so far
    cur_tok : int64 = -1l                   // sampled but not yet evaled — the next decode step's input
    state : StreamState = StreamState.prefilling
    finish : string = ""
    constraint : TokenConstraintBox         //! owned here (freed with the stream); the session carries the pointer and never frees it
    n_cached : int64                        // prefix-cache hit length (set at admit; 0 in flat mode)
    ckpt_at : int64                         //! the prompt position the prefill stops at to leave a recurrent checkpoint (0 = none)
    ckpt_sys_at : int64                     //! an earlier stop for the checkpoint at the shared system opening (0 = none; always under `ckpt_at`)
    ckpt_head_at : int64                    //! an earlier stop still, at the opening's shared head - the system text ahead of the tool block, or the tool block ahead of the text (0 = none; always under `ckpt_sys_at`)
    ckpt_at_tail : bool                     //! `ckpt_at` is the request's own stable opening and no shared stop: its checkpoint is a tail, which only this conversation's next turn extends
    ckpt_end_at : int64                     //! the last stop, one token short of the prompt, so the same prompt asked again attaches whole (0 = none; a page or more past `ckpt_at`)
    t_admit : int64                         // ref_time_ticks at admit
    t_first : int64                         // ref_time_ticks at the first sampled token (0 until then)
    ttft_us : int64
    prompt_head : string = ""               // poll surface: first ~160 chars of the detokenized prompt
    gen_tail_bytes : array<uint8>           // poll surface: last ~240..480 generated BYTES — a reused
                                            // buffer (string(bytes) only at poll/reap), so the hot
                                            // decode path allocates no strings per token
}

//! A finished stream's summary, kept ~10 s on the scheduler for the poll surface (the page
//! moves it to its history), then dropped.
struct DoneStream {
    id : int64
    finish : string
    n_prompt : int64
    n_cached : int64
    n_gen : int64
    ttft_us : int64
    prompt_head : string
    gen_tail : string
    done_at : int64                         // ref_time_ticks at reap
}

//! true while a stream is decoding: the tick's prefill chunk is a stall every one of them waits out
def private any_decoding(sch : Scheduler) : bool {
    for (st in sch.streams) {
        return true if (st.state == StreamState.decoding)
    }
    return false
}

//! on a host-cached stream `chunk_tokens` while a stream decodes and at least `idle_chunk_tokens` while none does; on a device-home stream at least the device's window, and the whole head at once for a model whose device prefill cannot continue past zero
def private prefill_chunk(m : Model; sch : Scheduler; head_end : int64) : int64 {
    if (!sch.device_kv) {
        return any_decoding(sch) ? sch.chunk_tokens : max(sch.chunk_tokens, sch.idle_chunk_tokens)
    }
    return gpu_device_prefill_continues(m) ? max(sch.chunk_tokens, gpu_device_prefill_window()) : head_end   // a chunk under the device's window is one submit per chunk for no bounded-stall gain
}

let DEFAULT_CHUNK_TOKENS = 512l   //! one prefill window: a window's cost is mostly fixed, so a smaller chunk runs a prompt at a fraction of the window's rate
let CPU_CHUNK_TOKENS = 64l        //! the CPU's chunk while a stream decodes: a CPU chunk's time is its token count, so the stall follows it down - under 32 tokens a batch leaves the batched kernels
let METAL_IDLE_CHUNK_TOKENS = 2048l   //! four windows in one call: within a few percent of the whole prompt's rate at a tick a quarter the length

//! The chunk while a stream decodes on a backend: one window where a GPU prefills, `CPU_CHUNK_TOKENS` on the CPU.
def chunk_default(on_gpu : bool) : int64
    => on_gpu ? DEFAULT_CHUNK_TOKENS : CPU_CHUNK_TOKENS

//! The prefill quantum's two sizes on the backend `m` is served by: `chunk` while a stream decodes, `idle` while none does.
def chunk_defaults(m : Model) : tuple<chunk : int64; idle : int64>
    => (chunk = chunk_default(m.metal_blob || gpu_tier_status().supported), idle = m.metal_blob ? METAL_IDLE_CHUNK_TOKENS : DEFAULT_CHUNK_TOKENS)

//! A finished device-home stream's K/V rows, kept on the device for the next request that opens with the same tokens.
struct ParkedKv {
    claim : int64           //! the region claim the rows sit under (`gpu_device_kv_park`)
    tokens : array<int64>   //! the tokens those rows hold, in order
}

//! The continuous-batching scheduler: active streams, the admission queue, the shared eval_batch
//! workspace, knobs, and counters. One per served model; drive it with scheduler_step.
struct Scheduler {
    ws : BatchWorkspace
    streams : array<Stream>                 // admission order (the FCFS prefill scan is first-to-last)
    queue : array<PendingReq>
    max_streams : int64 = 4l
    max_queue : int64 = 32l
    chunk_tokens : int64 = DEFAULT_CHUNK_TOKENS   // prefill quantum K for TEXT chunks of a host-cached stream (`prefill_chunk` widens a device-home one); a media span evals as ONE body (per-call mask), so it may stall decode by its row count
    idle_chunk_tokens : int64 = DEFAULT_CHUNK_TOKENS   //! the quantum's floor on a tick with no stream decoding (0 = `chunk_tokens` always)
    // paged serving (page_rows > 0 at create): sessions page KV out of the pool, and finished
    // streams donate their pages to the prefix cache so repeat prefixes skip their prefill
    paged : bool
    pool : KVPool
    prefix : PrefixCache
    kv_dtype : KVDtype = KVDtype.f16        // KV codec for every stream (pool-wide when paged)
    device_kv : bool                        //! admitted sessions are DEVICE-HOME: their K/V lives only in the GPU driver's regions (`set_device_kv`)
    parked : array<ParkedKv>
    mtp : bool                              // MTP/NextN self-spec ticks for every decoding stream - sampled and penalized ones draw the walk through their own sampler (the server also sets the engine's prefill-warm gate)
    // per-step gather scratch. @do_not_delete is load-bearing: delete of a plain array<T?>
    // CASCADES to the pointees — these are borrowed rows into streams[i].session
    @do_not_delete batch_rows : array<Session?>
    batch_toks : array<int64>
    batch_idx : array<int64>                // batch row -> streams index
    mtp_acc : array<int64>                  // the joint speculative tick's answer per batch row: the accepted draft, or -1
    chunk : array<int64>                    // the current prefill chunk
    chunk_tail : array<int64>               // a media body's text past its span
    @exact_size body : array<float>         // a media body's rows
    donate_toks : array<int64>              // reap-time scratch: tail eval + the insert token list
    // counters (/v1/stats and server_bench read these)
    n_admitted : int64
    mrope_streams : int64                   // admitted streams whose media rode the mrope grid walk
    n_finished : int64                      // reaped streams (any finish, evictions included)
    n_evicted : int64                       // evictions, active or still queued
    decode_steps : int64
    gen_tokens : int64
    sum_batch : int64                       // Σ decode batch size (avg batch = sum_batch / decode_steps)
    peak_active : int64
    cached_tokens : int64                   // prefix-cache hits (tokens attached instead of prefilled)
    recent_done : array<DoneStream>         // finished-stream linger for the poll surface (~10 s)
    prefill_tokens : int64                  // prompt tokens actually computed (cache hits excluded)
    prefill_evals : int64                   // prefill calls those tokens took
    ttft_sum_us : int64                     // Σ ttft over reaped streams that produced a token
    ttft_count : int64
    last_ttft_us : int64                    // most recently reaped stream's ttft
    mtp_drafted : int64                     // self-spec telemetry across streams (drafts attempted / accepted)
    mtp_accepted : int64
    mtp_joined_max : int64                  // the most streams one joint verify carried in a tick (0 = every tick stepped its streams alone)
}

//! Create a scheduler over `model`: `max_streams` concurrent sessions, `max_queue` waiting
//! requests, `chunk_tokens` the prefill quantum while a stream decodes and `idle_chunk_tokens` its
//! floor while none does (0 = `chunk_tokens` always); a `chunk_tokens` under 1 or a negative
//! `idle_chunk_tokens` takes the backend's default (`chunk_defaults`). `page_rows` > 0 switches to PAGED serving — KV
//! pages allocate on demand (memory tracks actual context, not seq_len × streams) and a prefix
//! cache reuses completed streams' pages across requests (`prefix_groups` caps its retention;
//! 0 = unbounded; a recurrent model keeps `prefix_states` checkpoints instead, 0 = none, within
//! `prefix_state_bytes`, 0 = the count alone); 0 keeps today's flat preallocated sessions. Free with
//! `unsafe(delete sch)` — live sessions, the pool, and the workspace go with it.
def create_scheduler(model : Model; max_streams : int64 = 4l; max_queue : int64 = 32l;
                     chunk_tokens : int64 = 0l; page_rows : int64 = 0l;
                     prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
                     mtp : bool = false; idle_chunk_tokens : int64 = -1l;
                     prefix_states : int64 = DEFAULT_PREFIX_STATES; prefix_state_bytes : int64 = 0l) : Scheduler {
    let dflt = chunk_defaults(model)
    var sch <- Scheduler(ws <- create_batch_workspace(model), max_streams = max(1l, max_streams),
                         max_queue = max(1l, max_queue), chunk_tokens = chunk_tokens > 0l ? chunk_tokens : dflt.chunk,
                         idle_chunk_tokens = idle_chunk_tokens >= 0l ? idle_chunk_tokens : dflt.idle,
                         kv_dtype = kv_dtype, mtp = mtp && mtp_capable_(model))
    gpu_dn_room(sch.max_streams)   //! every stream's recurrent state device-resident at once, before the first prefill
    if (page_rows > 0l) {
        sch.paged = true
        sch.pool <- create_kv_pool(model, page_rows, kv_dtype)
        kv_pool_reserve(sch.pool, sch.max_streams * (model.config.seq_len + page_rows) + prefix_groups * page_rows)
        sch.prefix <- create_prefix_cache(prefix_groups, prefix_states, prefix_state_bytes)
    }
    return <- sch
}

//! Serve the streams admitted from here on as DEVICE-HOME sessions (`on`), or as the scheduler was created. Only an idle
//! scheduler switches - one batched step takes one kind of session - so the answer is what it now does.
def set_device_kv(var sch : Scheduler; on : bool) : bool {
    if (sch.device_kv != on && empty(sch.streams)) {
        sch.device_kv = on
        to_log(LOG_INFO, "dasLLAMA scheduler: streams admitted from here on are {on ? "device-home (their K/V lives only on the GPU)" : "host-cached"} (set_device_kv)\n")
        if (!on) {
            delete sch.parked   // the claims die with the device state
        }
    }
    return sch.device_kv
}

//! Active stream count (prefilling + decoding).
def n_active(sch : Scheduler) : int64 => long_length(sch.streams)

//! the streams' own speculative telemetry summed, read around a joint tick for the scheduler's totals
def private mtp_totals(sch : Scheduler) : tuple<drafted : int64; accepted : int64> {
    var d = 0l
    var a = 0l
    for (st in sch.streams) {
        d += st.session.mtp_drafted
        a += st.session.mtp_accepted
    }
    return (drafted = d, accepted = a)
}

//! Queued (not yet admitted) request count.
def n_queued(sch : Scheduler) : int64 => long_length(sch.queue)

//! A splice request as an inline span: the rows' position ids enter the prompt at the splice, keyed by the rows' own
//! bytes, so a caller that hands rows with no key still shares a prefix with a request carrying the same rows.
def splice_to_span(var req : PendingReq) {
    var key = 0x9e3779b97f4a7c15ul
    for (v in req.media_embd) {
        key = hash_combine64(key, uint64(unsafe(reinterpret<uint>(v))))
    }
    var inscope tail <- [for (i in range64(req.media_at, long_length(req.prompt))); req.prompt[i]]
    req.prompt |> resize(req.media_at)
    req.prompt |> reserve(req.media_at + req.n_media + long_length(tail))
    for (r in range64(req.n_media)) {
        req.prompt |> push(media_position_id(key, r))
    }
    req.prompt |> push_from(tail)
    req.spans |> emplace(MediaSpan(at = req.media_at, n = req.n_media, non_causal = req.media_non_causal,
                                   grid = req.media_grid, rows <- req.media_embd))
    req.n_media = 0l
    req.media_at = 0l
    req.media_non_causal = false
    req.media_grid = int2(0)
}

//! a request's inline spans: in prompt order, over position ids, each with rows of the model's width or none
def private check_spans(model : Model; sch : Scheduler; req : PendingReq) {
    return if (empty(req.spans))
    if (sch.device_kv) {
        panic("dasllama_scheduler: a media request in device mode (stream {req.id}) - a device-home session serves no media span; the wiring keeps `set_device_kv` off for a model that takes media")
    }
    var floor = 0l
    for (sp in req.spans) {
        if (sp.n <= 0l || sp.at < floor || sp.at + sp.n > long_length(req.prompt)) {
            panic("dasllama_scheduler: media span [{sp.at}, {sp.at + sp.n}) outside the {length(req.prompt)}-position prompt or out of order (stream {req.id})")
        }
        if (!is_media_position(req.prompt[sp.at]) || !is_media_position(req.prompt[sp.at + sp.n - 1l])) {
            panic("dasllama_scheduler: media span [{sp.at}, {sp.at + sp.n}) does not stand on position ids (stream {req.id})")
        }
        let need = sp.n * model.config.dim * (1l + (sp.non_causal ? model.config.n_deepstack : 0l))
        if (!empty(sp.rows) && long_length(sp.rows) != need) {
            panic("dasllama_scheduler: a {sp.n}-row media span needs {need} floats, got {length(sp.rows)} (stream {req.id})")
        }
        floor = sp.at + sp.n
    }
}

//! Queue a request; false = queue full (the wiring answers 503). Preconditions PANIC — the wiring
//! must reject them first (its 400s): a non-empty prompt that fits the model context with room to
//! generate, and max_new >= 1.
def submit(model : Model; var sch : Scheduler; var req : PendingReq) : bool {
    let np = long_length(req.prompt)
    if (np <= 0l) {
        panic("dasllama_scheduler: empty prompt (stream {req.id}) - the wiring must reject it first")
    }
    if (req.n_media < 0l) {
        panic("dasllama_scheduler: negative n_media {req.n_media} (stream {req.id})")
    }
    if (req.n_media > 0l && sch.device_kv) {
        panic("dasllama_scheduler: a media request in device mode (stream {req.id}) - a device-home session serves no media span; the wiring keeps `set_device_kv` off for a model that takes media")
    }
    if (req.n_media > 0l) {
        if (req.media_at < 0l || req.media_at > np) {
            panic("dasllama_scheduler: media splice at {req.media_at} outside the {np}-token prompt (stream {req.id})")
        }
        // image rows on a deepstack decoder must arrive (1+n)·dim-wide (what the tower's
        // encode_image emits) — a narrow feed would silently skip the per-layer slice adds
        let media_ds = req.media_non_causal ? model.config.n_deepstack : 0l
        let media_need = req.n_media * model.config.dim * (1l + media_ds)
        if (long_length(req.media_embd) != media_need) {
            panic("dasllama_scheduler: {req.n_media} media rows need {media_need} floats ({media_ds} deepstack slices at dim {model.config.dim}), got {length(req.media_embd)} (stream {req.id})")
        }
        if (min(req.media_grid.x, req.media_grid.y) < 0 || ((req.media_grid.x > 0) != (req.media_grid.y > 0))) {
            panic("dasllama_scheduler: media_grid {req.media_grid} is neither (0,0) nor a positive grid (stream {req.id})")
        }
        if (req.media_grid.x > 0) {
            if (int64(req.media_grid.x) * int64(req.media_grid.y) != req.n_media) {
                panic("dasllama_scheduler: media_grid {req.media_grid} shapes {req.media_grid.x * req.media_grid.y} rows but n_media is {req.n_media} (stream {req.id})")
            }
            if (!req.media_non_causal) {
                panic("dasllama_scheduler: media_grid on causal media rows (stream {req.id}) - the mrope walk is an image-span contract")
            }
        }
    } elif (long_length(req.media_embd) != 0l || req.media_at != 0l || req.media_grid != int2(0)) {
        panic("dasllama_scheduler: media rows, media_at, or media_grid set with n_media == 0 (stream {req.id}) - the rows would be silently dropped")
    }
    if (req.n_media > 0l) {
        if (!empty(req.spans)) {
            panic("dasllama_scheduler: a request with both a media splice and inline spans (stream {req.id})")
        }
        splice_to_span(req)
    }
    check_spans(model, sch, req)
    if (long_length(req.prompt) + 1l > model.config.seq_len) {
        panic("dasllama_scheduler: {length(req.prompt)}-position prompt does not fit seq_len {model.config.seq_len} (stream {req.id}) - the wiring must reject it first")
    }
    if (req.max_new < 1l) {
        panic("dasllama_scheduler: max_new {req.max_new} < 1 (stream {req.id}) - the wiring must clamp it first")
    }
    if (long_length(sch.queue) >= sch.max_queue) {
        return false
    }
    req.t_submit = ref_time_ticks()
    sch.queue |> emplace(req)
    if (long_length(sch.queue) + long_length(sch.streams) > sch.max_streams) {
        to_log(LOG_INFO, "[req {sch.queue[length(sch.queue) - 1].id}] queued: depth {length(sch.queue)}, active {length(sch.streams)}/{sch.max_streams}\n")
    }
    return true
}

// Terminate a stream: mark done (reaped at step end) and append its finished event.
//! the nearest checkpoint stop past the rows prefilled so far, 0 when none is left: the system text, the system opening, the request's own
def private next_checkpoint(st : Stream) : int64 {
    for (at in fixed_array(st.ckpt_head_at, st.ckpt_sys_at, st.ckpt_at, st.ckpt_end_at)) {
        if (at > st.n_prefilled) {
            return at
        }
    }
    return 0l
}

def private at_checkpoint(st : Stream) : bool =>
    st.n_prefilled == st.ckpt_head_at || st.n_prefilled == st.ckpt_sys_at || st.n_prefilled == st.ckpt_at || st.n_prefilled == st.ckpt_end_at

//! a checkpoint stop inside the rows [lo, hi]: the span's one-call prefill cannot take them, since the stop needs the eval to end there
def private checkpoint_within(st : Stream; lo, hi : int64) : bool {
    for (at in fixed_array(st.ckpt_head_at, st.ckpt_sys_at, st.ckpt_at, st.ckpt_end_at)) {
        if (at > lo && at <= hi) {
            return true
        }
    }
    return false
}

//! The text a stop-string scan releases once `piece` joins `hold`: everything before the first stop string when one
//! is complete (true), else everything that cannot still begin one, the rest staying in `hold` (false).
def stop_scan(var hold : string&; piece : string; stops : array<string>; var emit : string&) : bool {
    let whole = empty(hold) ? piece : "{hold}{piece}"
    var at = -1
    for (s in stops) {
        let i = find(whole, s)
        if (i >= 0 && (at < 0 || i < at)) {
            at = i
        }
    }
    if (at >= 0) {
        emit = at > 0 ? slice(whole, 0, at) : ""
        hold = ""
        return true
    }
    var keep = 0
    peek_data(whole) $(d) {
        let n = length(d)
        for (s in stops) {
            var k = min(n, length(s) - 1)
            while (k > keep) {
                if (starts_with(d, n - k, s, uint(k))) {
                    keep = k
                    break
                }
                k --
            }
        }
    }
    if (keep == 0) {
        emit = whole
        hold = ""
    } else {
        emit = slice(whole, 0, length(whole) - keep)
        hold = slice(whole, length(whole) - keep)
    }
    return false
}

def private finish_stream(var st : Stream; finish : string; var events : array<SchedEvent>) {
    if (!empty(st.stop_hold)) {   //! held back for a stop string that never completed
        events |> push(SchedEvent(stream = st.id, text = st.stop_hold))
        st.stop_hold = ""
    }
    st.state = StreamState.done
    st.finish = finish
    let gen_us = st.t_first != 0l ? int64(get_time_usec(st.t_first)) : 0l
    let rate = gen_us > 0l ? 1000000.0 * float(st.n_gen) / float(gen_us) : 0.0
    to_log(LOG_INFO, "[stream {st.id}] finish: reason={finish}, gen={st.n_gen} tok in {gen_us / 1000l}ms ({rate} tok/s), total {int64(get_time_usec(st.t_admit)) / 1000l}ms\n")
    events |> push(SchedEvent(stream = st.id, kind = SchedEventKind.finished, finish = finish,
                              n_prompt = long_length(st.prompt), n_gen = st.n_gen,
                              n_cached = st.n_cached, ttft_us = st.ttft_us,
                              gen_us = gen_us))
}

// Sample the next token from st.session's fresh logits and run the emit protocol — stop-id check,
// piece event, recent push, budget check — identical for the inline first token and every decode
// step (generate_'s ordering). Returns true when a piece was emitted (the caller counts it).
// Shared emit bookkeeping for one FINAL token (freshly sampled, or a verified spec draft):
// ttft stamp, stop-token finish, event + repetition window + budget accounting.
//! Drop a trailing INCOMPLETE UTF-8 sequence (a lead byte whose continuations are missing) —
//! byte-fallback BPE pieces can end mid-codepoint; display surfaces call this before shipping.
def utf8_trim_partial_end(s : string) : string {
    let n = length(s)
    var cut = n
    peek_data(s) $(bytes) {
        var i = n - 1
        while (i >= 0 && !is_first_byte_of_utf8_char(bytes[i])) {
            i --
        }
        if (i >= 0) {
            let lead = int(bytes[i])
            var need = 1
            if ((lead & 0xE0) == 0xC0) {
                need = 2
            } elif ((lead & 0xF0) == 0xE0) {
                need = 3
            } elif ((lead & 0xF8) == 0xF0) {
                need = 4
            }
            if (n - i < need) {
                cut = i
            }
        }
    }
    return cut == n ? s : slice(s, 0, cut)
}

def private advance_with(m : Model; var st : Stream; tok : int64; var events : array<SchedEvent>) : bool {
    if (tok < 0l) {   //! the constraint admits no token: the reply ends where the grammar left it
        finish_stream(st, "constraint", events)
        return false
    }
    if (st.t_first == 0l) {
        st.t_first = ref_time_ticks()
        st.ttft_us = int64(get_time_usec(st.t_admit))
        to_log(LOG_INFO, "[stream {st.id}] first token after {st.ttft_us / 1000l}ms\n")
    }
    for (sid in st.stop_ids) {
        if (tok == sid) {
            finish_stream(st, "stop", events)
            return false
        }
    }
    let ptxt = piece(m, tok)
    if (nothink_stop_here(st.nothink, tok, ptxt)) {
        finish_stream(st, "stop", events)
        return false
    }
    var emit = ptxt
    if (!empty(st.stop_strs) && stop_scan(st.stop_hold, ptxt, st.stop_strs, emit)) {
        if (!empty(emit)) {
            events |> push(SchedEvent(stream = st.id, token = tok, text = emit))
        }
        finish_stream(st, "stop", events)
        return false
    }
    peek_data(ptxt) $(bytes) {
        for (b in bytes) {
            st.gen_tail_bytes |> push(b)
        }
    }
    if (length(st.gen_tail_bytes) > 480) {
        // compact in place to the last ~240 bytes, landing on a codepoint boundary — capacity
        // stays, so steady-state decode allocates nothing here
        var cut = length(st.gen_tail_bytes) - 240
        while (cut < length(st.gen_tail_bytes) && !is_first_byte_of_utf8_char(st.gen_tail_bytes[cut])) {
            cut ++
        }
        st.gen_tail_bytes |> erase(0, cut)
    }
    events |> push(SchedEvent(stream = st.id, token = tok, text = emit))
    st.session.recent |> push(tok)
    st.n_gen ++
    st.cur_tok = tok
    if (st.n_gen >= st.max_new) {
        finish_stream(st, "length", events)
    }
    return true
}

def private sample_advance(m : Model; var st : Stream; var events : array<SchedEvent>) : bool =>
    advance_with(m, st, sample(st.session, st.params), events)

// one plain decode step over the gathered batch arrays (nothing when they are empty): the device pick
// asked for an argmax stream with no constraint, then every row sampled and advanced
def private plain_batch_step(m : Model; var sch : Scheduler; var events : array<SchedEvent>) {
    return if (empty(sch.batch_rows))
    let ts_step = prof_ticks()
    for (si in sch.batch_idx) {
        sch.streams[si].session.pick_asked = sampler_is_argmax(sch.streams[si].params) && sch.streams[si].constraint.constraint == null
    }
    eval_batch(m, sch.ws, sch.batch_rows, sch.batch_toks)
    prof_add("sched_eval_batch", ts_step)
    sch.decode_steps ++
    sch.sum_batch += long_length(sch.batch_rows)
    let ts_sample = prof_ticks()
    for (bi in range64(long_length(sch.batch_idx))) {
        if (sample_advance(m, sch.streams[sch.batch_idx[bi]], events)) {
            sch.gen_tokens ++
        }
        sch.streams[sch.batch_idx[bi]].session.pick_asked = false   //! the ask is a step's
    }
    prof_add("sched_sample", ts_sample)
}

//! Evict a stream (the client disconnected): an active stream finishes "evicted" (its finished
//! event appends NOW; the session is reaped next step), a queued request is dropped with the same
//! event. False = unknown id.
def evict(var sch : Scheduler; id : int64; var events : array<SchedEvent>) : bool {
    for (i in range64(long_length(sch.streams))) {
        if (sch.streams[i].id == id && sch.streams[i].state != StreamState.done) {
            to_log(LOG_WARNING, "[stream {id}] evicting: client disconnected mid-generation\n")
            finish_stream(sch.streams[i], "evicted", events)
            sch.n_evicted ++
            return true
        }
    }
    for (i in range64(long_length(sch.queue))) {
        if (sch.queue[i].id == id) {
            events |> push(SchedEvent(stream = id, kind = SchedEventKind.finished, finish = "evicted",
                                      n_prompt = long_length(sch.queue[i].prompt)))
            delete sch.queue[i]   // erase is a raw memmove — finalize the owned arrays first
            sch.queue |> erase(i)
            sch.n_evicted ++
            return true
        }
    }
    return false
}

//! One scheduler tick: admit from the queue while under max_streams, ONE eval_batch decode step
//! over every decoding stream, at most ONE prefill chunk (`chunk_tokens`, or `prefill_chunk`'s wider one on an idle tick and in device mode; FCFS; the final chunk's
//! logits sample the first token inline), then reap finished streams. Events append to `events`
//! (the caller drains it). Returns true while any stream or queued request remains.
[arch(at="../ARCHITECTURE_ENGINE_SERVING.md#scheduler-step")]
def scheduler_step(m : Model; var sch : Scheduler; var events : array<SchedEvent>) : bool {  // nolint:STYLE037,STYLE038 - admit/decode/prefill/reap, one linear pass
    // (0) drop finished-stream linger entries past their poll window (bounded backstop at 32).
    // get_time_nsec, NOT get_time_usec: the usec helper returns int and wraps at ~35.8 min,
    // which would freeze this comparison for any entry that survived a wrap
    while (!empty(sch.recent_done)
           && (get_time_nsec(sch.recent_done[0].done_at) > 10_000_000_000l || length(sch.recent_done) > 32)) {
        delete sch.recent_done[0]
        sch.recent_done |> erase(0)
    }
    // (1) admit FIFO while under the cap — sessions (KV memory) exist only from this point on.
    // Paged mode also attaches the longest prefix-cache hit: those positions never prefill.
    while (!empty(sch.queue) && long_length(sch.streams) < sch.max_streams) {
        var st <- Stream(id = sch.queue[0].id,
                         params = sch.queue[0].params, prompt <- sch.queue[0].prompt,
                         close_toks <- sch.queue[0].close_toks, stop_ids <- sch.queue[0].stop_ids,
                         stop_strs <- sch.queue[0].stop_strs, nothink <- sch.queue[0].nothink,
                         spans <- sch.queue[0].spans, constraint <- sch.queue[0].constraint,
                         max_new = sch.queue[0].max_new, t_admit = ref_time_ticks())
        if (sch.device_kv) {
            st.session <- create_device_session(m, gpu_device_session_dtype())
            let matched = parked_attach(sch, st.session, st.prompt)   // a model whose prefill cannot continue parks nothing, so nothing matches
            st.n_prefilled = matched
            st.n_cached = matched
            sch.cached_tokens += matched
            to_log(LOG_INFO, "[stream {st.id}] device prefix: {matched}/{length(st.prompt)} tok already on the device\n")
        } elif (sch.paged) {
            st.session <- create_session(m, sch.pool)
            let t_attach = ref_time_ticks()   // clock: control - the attach's cost in the log line
            var matched = prefix_attach(sch.prefix, sch.pool, st.session, st.prompt)
            let attach_ms = int64(get_time_usec(t_attach)) / 1000l
            if (cuts_a_span(st.spans, matched)) {   // a span evals whole: a hit that ends inside one is no hit
                release_kv_pages(st.session)
                delete st.session
                st.session <- create_session(m, sch.pool)
                matched = 0l
            }
            adopt_spans(st, matched)
            st.n_prefilled = matched
            st.n_cached = matched
            sch.cached_tokens += matched
            st.ckpt_at = prefix_checkpoint_at(sch.prefix, sch.pool, st.session, st.prompt, matched, sch.queue[0].stable_at)
            //! the shared system opening gets its own stop when it sits a page or more past the hit and under the request's
            //! own stop - a request's own stop under a page past the opening yields to it instead, since the opening serves
            //! every request on it - and the opening's shared head one more, a page or more under the opening's
            let pr = sch.pool.page_rows
            let sys_at = sch.queue[0].system_at
            if (st.ckpt_at > 0l && sys_at >= matched + pr) {
                if (sys_at + pr <= st.ckpt_at) {
                    st.ckpt_sys_at = sys_at
                } elif (sys_at <= st.ckpt_at) {
                    st.ckpt_at = sys_at
                }
            }
            let head_at = sch.queue[0].system_head_at
            let above = st.ckpt_sys_at > 0l ? st.ckpt_sys_at : st.ckpt_at
            if (above > 0l && head_at >= matched + pr && head_at + pr <= above) {
                st.ckpt_head_at = head_at
            }
            st.ckpt_at_tail = st.ckpt_at > 0l && st.ckpt_at == sch.queue[0].stable_at && st.ckpt_at != sys_at && st.ckpt_at != head_at
            //! the prompt's own end, one token short so a token is left to eval: a page or more past the last stop, else that stop stands for it
            let end_at = long_length(st.prompt) - 1l
            if (sch.prefix.max_states > 0l && !empty(st.session.dn_state_prefix) && end_at >= max(st.ckpt_at, matched) + pr) {
                st.ckpt_end_at = end_at
            }
            if (matched > 0l) {
                to_log(LOG_INFO, "[stream {st.id}] prefix cache HIT: {matched}/{length(st.prompt)} tok attached in {attach_ms} ms, no prefill for them (checkpoints planned at {st.ckpt_head_at}, {st.ckpt_sys_at}, {st.ckpt_at}, {st.ckpt_end_at}; stable {sch.queue[0].stable_at}, system {sys_at}, head {head_at})\n")
            } else {
                to_log(LOG_INFO, "[stream {st.id}] prefix cache MISS: full {length(st.prompt)}-tok prefill (checkpoints planned at {st.ckpt_head_at}, {st.ckpt_sys_at}, {st.ckpt_at}, {st.ckpt_end_at}; stable {sch.queue[0].stable_at}, system {sys_at}, head {head_at})\n")
            }
        } else {
            st.session <- create_session(m, sch.kv_dtype)
        }
        if (sch.queue[0].seed != 0) {
            set_seed(st.session, sch.queue[0].seed)
        }
        st.session.constrain = st.constraint.constraint   // the sampler's view; a constrained stream never joins a speculative round or asks the device for its pick
        var lost = false
        for (sp in st.spans) {
            st.n_media += sp.n
            lost ||= !sp.done && empty(sp.rows)
            if (sp.grid.x > 0) {
                sch.mrope_streams ++
            }
        }
        let wait_ms = sch.queue[0].t_submit != 0l ? int64(get_time_usec(sch.queue[0].t_submit)) / 1000l : 0l
        let media_note = st.n_media > 0l ? " ({st.n_media} media rows in {length(st.spans)} span(s))" : ""
        to_log(LOG_INFO, "[stream {st.id}] start: prompt={length(st.prompt)} pos{media_note} (cached={st.n_cached}), queue_wait={wait_ms}ms, active={length(sch.streams) + 1}/{sch.max_streams}\n")
        if (lost) {   // the request counted on the cache for a span the cache no longer holds: the caller encodes it and asks again
            finish_stream(st, "media_lost", events)
        }
        for (tk in st.prompt) {   // poll-surface preview: whole pieces up to the head cap
            continue if (is_media_position(tk))
            let p = piece(m, tk)
            break if (length(st.prompt_head) + length(p) > 160)
            st.prompt_head += p
        }
        st.prompt_head = utf8_trim_partial_end(st.prompt_head)   // byte-fallback pieces can end mid-codepoint
        st.gen_tail_bytes |> reserve(512)   // one allocation per stream; the compaction keeps it
        sch.streams |> emplace(st)
        delete sch.queue[0]   // erase is a raw memmove — finalize what the move-out left behind
        sch.queue |> erase(0)
        sch.n_admitted ++
    }
    sch.peak_active = max(sch.peak_active, long_length(sch.streams))
    // (2) ONE batched decode step over every decoding stream (decode priority). Rows are gathered
    // fresh each step: admission above may have reallocated `streams`, and nothing below grows it.
    sch.batch_rows |> clear()
    sch.batch_toks |> clear()
    sch.batch_idx |> clear()
    for (i in range64(long_length(sch.streams))) {
        if (sch.streams[i].state != StreamState.decoding) {
            continue
        }
        if (sch.streams[i].session.n_past + 1l > m.config.seq_len) {   // pre-check: never reach the eval_batch panic
            finish_stream(sch.streams[i], "length", events)
            continue
        }
        if (sch.mtp && sch.streams[i].constraint.constraint != null) {
            continue   // a constrained stream decodes plain, after the speculative round (below)
        }
        if (sch.mtp) {
            // self-spec tick: the stream trades its cross-stream batch row for up to two positions per
            // tick, every stream's round verified together below.
            sch.streams[i].session.spec_params = unsafe(addr(sch.streams[i].params))
            sch.streams[i].session.pick_asked = sampler_is_argmax(sch.streams[i].params)
        }
        sch.batch_rows |> push(unsafe(addr(sch.streams[i].session)))
        sch.batch_toks |> push(sch.streams[i].cur_tok)
        sch.batch_idx |> push(i)
    }
    if (sch.mtp && !empty(sch.batch_rows)) {
        let ts_step = prof_ticks()
        let before = mtp_totals(sch)
        let joined = mtp_spec_eval_batch(m, sch.batch_rows, sch.batch_toks, sch.mtp_acc)
        note_batch_step(joined == long_length(sch.batch_rows))   //! the census: one device step when every stream's rows rode the joint verify
        sch.mtp_joined_max = max(sch.mtp_joined_max, joined)
        prof_add("sched_eval_batch", ts_step)
        for (i, acc in sch.batch_idx, sch.mtp_acc) {
            if (acc >= 0l) {
                if (advance_with(m, sch.streams[i], acc, events)) {
                    sch.gen_tokens ++
                }
                if (sch.streams[i].state == StreamState.done) {
                    sch.streams[i].cur_tok = -1l   //! the accepted draft is evaled already: the close has nothing pending
                }
            }
            if (sch.streams[i].state == StreamState.decoding) {
                if (sample_advance(m, sch.streams[i], events)) {
                    sch.gen_tokens ++
                }
            }
            sch.streams[i].session.spec_params = null   // the pointer aims into `streams`, which admission and reaping move
            sch.streams[i].session.pick_asked = false
        }
        let after = mtp_totals(sch)
        sch.mtp_drafted += after.drafted - before.drafted
        sch.mtp_accepted += after.accepted - before.accepted
        sch.decode_steps ++
        sch.sum_batch += long_length(sch.batch_rows)
        sch.batch_rows |> clear()
        sch.batch_toks |> clear()
        sch.batch_idx |> clear()
    }
    if (sch.mtp) {
        for (i in range64(long_length(sch.streams))) {   // the constrained streams the round skipped: one plain step, round or no round
            if (sch.streams[i].state == StreamState.decoding && sch.streams[i].constraint.constraint != null) {
                sch.batch_rows |> push(unsafe(addr(sch.streams[i].session)))
                sch.batch_toks |> push(sch.streams[i].cur_tok)
                sch.batch_idx |> push(i)
            }
        }
    }
    plain_batch_step(m, sch, events)
    sch.batch_rows |> clear()
    sch.batch_toks |> clear()
    sch.batch_idx |> clear()
    // (3) at most ONE prefill chunk, FCFS — decode-priority with a bounded prefill quantum
    for (i in range64(long_length(sch.streams))) {
        if (sch.streams[i].state != StreamState.prefilling) {
            continue
        }
        let np = long_length(sch.streams[i].prompt)
        // a media span takes the whole quantum: its rows eval as ONE body, because the mask (gemma vision's
        // non-causal span) is per-call and a chunk boundary would split it. Text chunks normally up to the next span.
        let si = next_span(sch.streams[i].spans)
        if (si >= 0 && prefill_media_body(m, sch, sch.streams[i], si)) {
            pass
        } elif (si >= 0 && sch.streams[i].n_prefilled >= sch.streams[i].spans[si].at) {
            let sn = sch.streams[i].spans[si].n
            if (sch.streams[i].spans[si].grid.x > 0) {   // qwen mrope: grid angles + the session-tracked position delta
                eval_embd_span_mrope(m, sch.streams[i].session, sch.streams[i].spans[si].rows, sn, 0l, sn,
                                     sch.streams[i].spans[si].grid)
            } else {
                eval_embd(m, sch.streams[i].session, sch.streams[i].spans[si].rows, sn, sch.streams[i].spans[si].non_causal)
            }
            sch.streams[i].spans[si].done = true
            sch.streams[i].n_prefilled += sn
            sch.prefill_tokens += sn
            sch.prefill_evals ++
        } else {
            let head_end = si >= 0 ? sch.streams[i].spans[si].at : np
            let chunk = prefill_chunk(m, sch, head_end)
            let next_ckpt = next_checkpoint(sch.streams[i])
            let to_ckpt = next_ckpt - sch.streams[i].n_prefilled
            //! a checkpoint within a chunk and a half ends this chunk: stopping a chunk short of it would add a window, and a window's cost is mostly fixed
            let take = to_ckpt > 0l && to_ckpt <= chunk + chunk / 2l ? to_ckpt : min(chunk, head_end - sch.streams[i].n_prefilled)
            sch.chunk |> resize(take)
            for (k in range64(take)) {
                sch.chunk[k] = sch.streams[i].prompt[sch.streams[i].n_prefilled + k]
            }
            let t_window = ref_time_ticks()   // clock: control - the window's cost in the log line
            eval(m, sch.streams[i].session, sch.chunk)
            to_log(LOG_INFO, "[stream {sch.streams[i].id}] prefill window: {take} tok at {sch.streams[i].n_prefilled} in {int64(get_time_usec(t_window)) / 1000l} ms\n")
            sch.streams[i].n_prefilled += take
            sch.prefill_tokens += take
            sch.prefill_evals ++
            if (at_checkpoint(sch.streams[i])) {
                let tail = (sch.streams[i].ckpt_at_tail && sch.streams[i].n_prefilled == sch.streams[i].ckpt_at) || sch.streams[i].n_prefilled == sch.streams[i].ckpt_end_at
                prefix_insert(sch.prefix, sch.pool, sch.streams[i].session, sch.streams[i].prompt, "", !tail)
                to_log(LOG_INFO, "[stream {sch.streams[i].id}] prefix checkpoint at {sch.streams[i].n_prefilled} tok: {tail ? "this conversation's next turn" : "the next prompt on this opening"} attaches here ({length(sch.prefix.states)} checkpoints, {sch.prefix.state_bytes / 1048576l} MB)\n")
            }
        }
        if (sch.streams[i].n_prefilled >= np) {
            let pf_ms = int64(get_time_usec(sch.streams[i].t_admit)) / 1000l
            let pf_tok = np - sch.streams[i].n_cached
            let pf_rate = pf_ms > 0l ? 1000.0 * float(pf_tok) / float(pf_ms) : 0.0
            to_log(LOG_INFO, "[stream {sch.streams[i].id}] prefill done: {pf_tok} tok in {pf_ms}ms ({pf_rate} tok/s)\n")
            sch.streams[i].state = StreamState.decoding   // before sampling: sample_advance may finish it
            if (sample_advance(m, sch.streams[i], events)) {   // the final chunk's logits sample the first token inline
                sch.gen_tokens ++
            }
        }
        break
    }
    // (4) reap done streams (their finished events were emitted at the state flip); paged streams
    // donate their pages to the prefix cache on the way out
    var i = 0
    while (i < length(sch.streams)) {
        if (sch.streams[i].state == StreamState.done) {
            if (sch.streams[i].ttft_us > 0l) {
                sch.ttft_sum_us += sch.streams[i].ttft_us
                sch.ttft_count ++
                sch.last_ttft_us = sch.streams[i].ttft_us
            }
            sch.recent_done |> push(DoneStream(
                id = sch.streams[i].id, finish = sch.streams[i].finish,
                n_prompt = long_length(sch.streams[i].prompt),
                n_cached = sch.streams[i].n_cached,
                n_gen = sch.streams[i].n_gen, ttft_us = sch.streams[i].ttft_us,
                prompt_head = sch.streams[i].prompt_head, gen_tail = string(sch.streams[i].gen_tail_bytes),
                done_at = ref_time_ticks()))
            if (sch.streams[i].session.device_kv) {
                park_stream(m, sch, sch.streams[i])
            } elif (sch.paged) {
                if (sch.streams[i].finish != "media_lost") {
                    donate_stream(m, sch, sch.streams[i])
                }
                release_kv_pages(sch.streams[i].session)
            }
            delete sch.streams[i]   // erase is a raw memmove — finalize the owned session first
            sch.streams |> erase(i)
            sch.n_finished ++
        } else {
            i ++
        }
    }
    return !empty(sch.streams) || !empty(sch.queue)
}

//! One eval for span `si` and the text around it, up to a chunk of text: a prefill call's cost is mostly fixed, so
//! the text either side of a span rides the span's call. False where the body is not one eval - a recurrent
//! checkpoint ends a chunk, the span stands bare.
def private prefill_media_body(m : Model; var sch : Scheduler; var st : Stream; si : int) : bool {
    let lo = st.n_prefilled
    let at = st.spans[si].at
    let sn = st.spans[si].n
    let head = at - lo
    let room = prefill_chunk(m, sch, at)
    return false if (head > room)
    let stop = si + 1 < length(st.spans) ? st.spans[si + 1].at : long_length(st.prompt)
    let hi = min(stop, at + sn + room - head)
    return false if (hi - lo == sn || checkpoint_within(st, lo, hi))
    sch.chunk |> resize(head)
    for (k in range64(head)) {
        sch.chunk[k] = st.prompt[lo + k]
    }
    let ntail = hi - at - sn
    sch.chunk_tail |> resize(ntail)
    for (k in range64(ntail)) {
        sch.chunk_tail[k] = st.prompt[at + sn + k]
    }
    media_body_rows(m, st.session, sch.chunk, st.spans[si].rows, sn, long_length(st.spans[si].rows) / sn, sch.chunk_tail, sch.body)
    let masked = st.spans[si].non_causal || st.spans[si].grid.x > 0
    eval_embd_body(m, st.session, sch.body, hi - lo, masked ? head : 0l, masked ? head + sn : 0l, st.spans[si].grid)
    st.spans[si].done = true
    st.n_prefilled = hi
    sch.prefill_tokens += hi - lo
    sch.prefill_evals ++
    return true
}

//! the first span whose K/V does not stand yet; -1 when every span's does
def private next_span(spans : array<MediaSpan>) : int {
    for (sp, i in spans, count()) {
        return i if (!sp.done)
    }
    return -1
}

//! whether a prefix hit of `matched` positions ends inside a span
def private cuts_a_span(spans : array<MediaSpan>; matched : int64) : bool {
    for (sp in spans) {
        return true if (matched > sp.at && matched < sp.at + sp.n)
    }
    return false
}

//! The spans a prefix hit of `matched` positions covers stand as attached: their rows are never evaled, and a grid
//! span's rope advance is restored on the session, which ropes the positions after it by that delta.
def private adopt_spans(var st : Stream; matched : int64) {
    for (sp in st.spans) {
        continue if (sp.at + sp.n > matched)
        sp.done = true
        if (sp.grid.x > 0) {
            st.session.rope_pos_delta += int64(max(sp.grid.x, sp.grid.y)) - sp.n
        }
    }
}

// Donate a finished stream's KV pages, first evaling what the cache still lacks. A "length"
// finish leaves the last emitted token pending (sampled, never fed back); a "stop" finish already
// evaled every emitted token — the stop was sampled from the last one's eval — so only the close
// tokens are pending (re-evaling the last token would shift every row past the reply boundary
// against the page's token list). Either way the close tokens follow, so the NEXT turn of this
// conversation (prompt + reply + close + new user turn) extends the cached prefix. The tail is
// skipped when it cannot fit; evicted streams donate only what ran. prefix_insert trims to n_past.
def private donate_stream(m : Model; var sch : Scheduler; var st : Stream) {
    close_turn(m, sch, st)
    prefix_insert(sch.prefix, sch.pool, st.session, sch.donate_toks, st.prompt_head)
}

def private park_stream(m : Model; var sch : Scheduler; var st : Stream) {
    if (!gpu_device_prefill_continues(m)) {
        gpu_device_kv_park(st.session)
        return
    }
    close_turn(m, sch, st)
    let claim = gpu_device_kv_park(st.session)
    if (claim == 0l) {
        return
    }
    sch.donate_toks |> resize(min(long_length(sch.donate_toks), st.session.n_past))
    while (long_length(sch.parked) >= sch.max_streams) {
        drop_parked(sch, 0)
    }
    sch.parked |> emplace(ParkedKv(claim = claim, tokens := sch.donate_toks))
}

//! Adopt the parked rows sharing the longest opening with `prompt` (always short of its last token, whose logits the
//! prefill must produce); 0 when none stands. A claim another session took since is dropped where it is found.
def private parked_attach(var sch : Scheduler; var session : Session; prompt : array<int64>) : int64 {
    var best = -1
    var best_len = 0l
    for (p, pi in sch.parked, count()) {
        var n = 0l
        let cap = min(long_length(p.tokens), long_length(prompt) - 1l)
        while (n < cap && p.tokens[n] == prompt[n]) {
            n ++
        }
        if (n > best_len) {
            best = pi
            best_len = n
        }
    }
    if (best < 0) {
        return 0l
    }
    let adopted = gpu_device_kv_adopt(session, sch.parked[best].claim, best_len)
    drop_parked(sch, best)
    return adopted ? best_len : 0l
}

//! `erase` moves the entries after it down and finalizes nothing, so the entry's token list goes first
def private drop_parked(var sch : Scheduler; at : int) {
    delete sch.parked[at].tokens
    sch.parked |> erase(at)
}

//! Eval the turn's closing tokens and leave in `donate_toks` the token list the session's rows hold.
def private close_turn(m : Model; var sch : Scheduler; var st : Stream) {
    if (st.finish != "evicted") {
        sch.donate_toks |> clear()
        if (st.finish == "length" && st.cur_tok >= 0l) {
            sch.donate_toks |> push(st.cur_tok)
        }
        sch.donate_toks |> push_from(st.close_toks)
        if (!empty(sch.donate_toks)
                && st.session.n_past + long_length(sch.donate_toks) <= m.config.seq_len) {
            eval(m, st.session, sch.donate_toks)
        }
    }
    sch.donate_toks |> clear()
    sch.donate_toks |> reserve(int(st.n_prefilled) + length(st.session.recent) + length(st.close_toks))
    for (k in range64(st.n_prefilled)) {
        sch.donate_toks |> push(st.prompt[k])
    }
    sch.donate_toks |> push_from(st.session.recent, st.close_toks)
}
