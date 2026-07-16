options gen2

module llm_scheduler

require dasllama/dasllama   // the public facade — the scheduler reaches no engine internals
require math

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

//! A parsed + rendered request awaiting admission: tokens only, NO session — queued requests hold
//! no KV memory. The caller renders (create_chat_renderer / render_assistant / render_turn, or a
//! raw encode) and assigns unique ids.
struct PendingReq {
    id : int64
    prompt : array<int64>                   // the full rendered prompt
    close_toks : array<int64>               // the turn-terminating tokens (render_close; evaled at donation so the next turn extends the cached prefix)
    stop_ids : array<int64>                 // sampling one of these finishes "stop" (the token is not emitted)
    params : SamplingParams = SamplingParams()
    max_new : int64 = 256l                  // reply token budget (>= 1)
    seed : int = 0                          // != 0 => set_seed at admit (reproducible sampling)
    t_submit : int64 = 0l                   // ref_time_ticks at submit (queue-wait logging; 0 = unset)
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
    n_prefilled : int64                     // prompt tokens evaled so far
    max_new : int64
    n_gen : int64                           // tokens emitted so far
    cur_tok : int64 = -1l                   // sampled but not yet evaled — the next decode step's input
    state : StreamState = StreamState.prefilling
    finish : string = ""
    n_cached : int64                        // prefix-cache hit length (set at admit; 0 in flat mode)
    t_admit : int64                         // ref_time_ticks at admit
    t_first : int64                         // ref_time_ticks at the first sampled token (0 until then)
    ttft_us : int64
}

//! The continuous-batching scheduler: active streams, the admission queue, the shared eval_batch
//! workspace, knobs, and counters. One per served model; drive it with scheduler_step.
struct Scheduler {
    ws : BatchWorkspace
    streams : array<Stream>                 // admission order (the FCFS prefill scan is first-to-last)
    queue : array<PendingReq>
    max_streams : int64 = 4l
    max_queue : int64 = 32l
    chunk_tokens : int64 = 64l              // prefill quantum K: decode stalls at most this many tokens
    // paged serving (page_rows > 0 at create): sessions page KV out of the pool, and finished
    // streams donate their pages to the prefix cache so repeat prefixes skip their prefill
    paged : bool
    pool : KVPool
    prefix : PrefixCache
    kv_dtype : KVDtype = KVDtype.f16        // KV codec for every stream (pool-wide when paged)
    mtp : bool                              // MTP/NextN self-spec ticks for greedy streams (the server also sets the engine's prefill-warm gate)
    // per-step gather scratch. @do_not_delete is load-bearing: delete of a plain array<T?>
    // CASCADES to the pointees — these are borrowed rows into streams[i].session
    @do_not_delete batch_rows : array<Session?>
    batch_toks : array<int64>
    batch_idx : array<int64>                // batch row -> streams index
    chunk : array<int64>                    // the current prefill chunk
    donate_toks : array<int64>              // reap-time scratch: tail eval + the insert token list
    // counters (/v1/stats and server_bench read these)
    n_admitted : int64
    n_finished : int64                      // reaped streams (any finish, evictions included)
    n_evicted : int64                       // evictions, active or still queued
    decode_steps : int64
    gen_tokens : int64
    sum_batch : int64                       // Σ decode batch size (avg batch = sum_batch / decode_steps)
    peak_active : int64
    cached_tokens : int64                   // prefix-cache hits (tokens attached instead of prefilled)
    mtp_drafted : int64                     // self-spec telemetry across streams (drafts attempted / accepted)
    mtp_accepted : int64
}

//! Create a scheduler over `model`: `max_streams` concurrent sessions, `max_queue` waiting
//! requests, `chunk_tokens` the prefill quantum. `page_rows` > 0 switches to PAGED serving — KV
//! pages allocate on demand (memory tracks actual context, not seq_len × streams) and a prefix
//! cache reuses completed streams' pages across requests (`prefix_groups` caps its retention;
//! 0 = unbounded); 0 keeps today's flat preallocated sessions. Free with `unsafe(delete sch)` —
//! live sessions, the pool, and the workspace go with it.
def create_scheduler(model : Model; max_streams : int64 = 4l; max_queue : int64 = 32l;
                     chunk_tokens : int64 = 64l; page_rows : int64 = 0l;
                     prefix_groups : int64 = 0l; kv_dtype : KVDtype = KVDtype.f16;
                     mtp : bool = false) : Scheduler {
    var sch <- Scheduler(ws <- create_batch_workspace(model), max_streams = max(1l, max_streams),
                         max_queue = max(1l, max_queue), chunk_tokens = max(1l, chunk_tokens),
                         kv_dtype = kv_dtype, mtp = mtp && model.config.n_layer_nextn > 0l)
    if (page_rows > 0l) {
        sch.paged = true
        sch.pool <- create_kv_pool(model, page_rows, kv_dtype)
        sch.prefix <- create_prefix_cache(prefix_groups)
    }
    return <- sch
}

//! Active stream count (prefilling + decoding).
def n_active(sch : Scheduler) : int64 => int64(length(sch.streams))

//! Queued (not yet admitted) request count.
def n_queued(sch : Scheduler) : int64 => int64(length(sch.queue))

//! Queue a request; false = queue full (the wiring answers 503). Preconditions PANIC — the wiring
//! must reject them first (its 400s): a non-empty prompt that fits the model context with room to
//! generate, and max_new >= 1.
def submit(model : Model; var sch : Scheduler; var req : PendingReq) : bool {
    let np = int64(length(req.prompt))
    if (np <= 0l) {
        panic("llm_scheduler: empty prompt (stream {req.id}) — the wiring must reject it first")
    }
    if (np + 1l > model.config.seq_len) {
        panic("llm_scheduler: {np}-token prompt does not fit seq_len {model.config.seq_len} (stream {req.id}) — the wiring must reject it first")
    }
    if (req.max_new < 1l) {
        panic("llm_scheduler: max_new {req.max_new} < 1 (stream {req.id}) — the wiring must clamp it first")
    }
    if (int64(length(sch.queue)) >= sch.max_queue) {
        return false
    }
    req.t_submit = ref_time_ticks()
    sch.queue |> emplace(req)
    if (int64(length(sch.queue)) + int64(length(sch.streams)) > sch.max_streams) {
        to_log(LOG_INFO, "[req {sch.queue[length(sch.queue) - 1].id}] queued: depth {length(sch.queue)}, active {length(sch.streams)}/{sch.max_streams}\n")
    }
    return true
}

// Terminate a stream: mark done (reaped at step end) and append its finished event.
def private finish_stream(var st : Stream; finish : string; var events : array<SchedEvent>) {
    st.state = StreamState.done
    st.finish = finish
    let gen_us = st.t_first != 0l ? int64(get_time_usec(st.t_first)) : 0l
    let rate = gen_us > 0l ? 1000000.0 * float(st.n_gen) / float(gen_us) : 0.0
    to_log(LOG_INFO, "[stream {st.id}] finish: reason={finish}, gen={st.n_gen} tok in {gen_us / 1000l}ms ({rate} tok/s), total {int64(get_time_usec(st.t_admit)) / 1000l}ms\n")
    events |> push(SchedEvent(stream = st.id, kind = SchedEventKind.finished, finish = finish,
                              n_prompt = int64(length(st.prompt)), n_gen = st.n_gen,
                              n_cached = st.n_cached, ttft_us = st.ttft_us,
                              gen_us = gen_us))
}

// Sample the next token from st.session's fresh logits and run the emit protocol — stop-id check,
// piece event, recent push, budget check — identical for the inline first token and every decode
// step (generate_'s ordering). Returns true when a piece was emitted (the caller counts it).
// Shared emit bookkeeping for one FINAL token (freshly sampled, or a verified spec draft):
// ttft stamp, stop-token finish, event + repetition window + budget accounting.
def private advance_with(m : Model; var st : Stream; tok : int64; var events : array<SchedEvent>) : bool {
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
    events |> push(SchedEvent(stream = st.id, token = tok, text = piece(m, tok)))
    st.session.recent |> push(tok)
    st.n_gen ++
    st.cur_tok = tok
    if (st.n_gen >= st.max_new) {
        finish_stream(st, "length", events)
    }
    return true
}

def private sample_advance(m : Model; var st : Stream; var events : array<SchedEvent>) : bool {
    return advance_with(m, st, sample(st.session, st.params), events)
}

//! Evict a stream (the client disconnected): an active stream finishes "evicted" (its finished
//! event appends NOW; the session is reaped next step), a queued request is dropped with the same
//! event. False = unknown id.
def evict(var sch : Scheduler; id : int64; var events : array<SchedEvent>) : bool {
    for (i in range64(int64(length(sch.streams)))) {
        if (sch.streams[i].id == id && sch.streams[i].state != StreamState.done) {
            to_log(LOG_WARNING, "[stream {id}] evicting: client disconnected mid-generation\n")
            finish_stream(sch.streams[i], "evicted", events)
            sch.n_evicted ++
            return true
        }
    }
    for (i in range64(int64(length(sch.queue)))) {
        if (sch.queue[i].id == id) {
            events |> push(SchedEvent(stream = id, kind = SchedEventKind.finished, finish = "evicted",
                                      n_prompt = int64(length(sch.queue[i].prompt))))
            delete sch.queue[i]   // erase is a raw memmove — finalize the owned arrays first
            sch.queue |> erase(i)
            sch.n_evicted ++
            return true
        }
    }
    return false
}

//! One scheduler tick: admit from the queue while under max_streams, ONE eval_batch decode step
//! over every decoding stream, at most ONE chunk_tokens prefill chunk (FCFS; the final chunk's
//! logits sample the first token inline), then reap finished streams. Events append to `events`
//! (the caller drains it). Returns true while any stream or queued request remains.
def scheduler_step(m : Model; var sch : Scheduler; var events : array<SchedEvent>) : bool {
    // (1) admit FIFO while under the cap — sessions (KV memory) exist only from this point on.
    // Paged mode also attaches the longest prefix-cache hit: those positions never prefill.
    while (!empty(sch.queue) && int64(length(sch.streams)) < sch.max_streams) {
        var st <- Stream(id = sch.queue[0].id,
                         params = sch.queue[0].params, prompt <- sch.queue[0].prompt,
                         close_toks <- sch.queue[0].close_toks, stop_ids <- sch.queue[0].stop_ids,
                         max_new = sch.queue[0].max_new, t_admit = ref_time_ticks())
        if (sch.paged) {
            st.session <- create_session(m, sch.pool)
            let matched = prefix_attach(sch.prefix, sch.pool, st.session, st.prompt)
            st.n_prefilled = matched
            st.n_cached = matched
            sch.cached_tokens += matched
            if (matched > 0l) {
                to_log(LOG_INFO, "[stream {st.id}] prefix cache HIT: {matched}/{length(st.prompt)} tok attached, no prefill for them\n")
            } else {
                to_log(LOG_INFO, "[stream {st.id}] prefix cache MISS: full {length(st.prompt)}-tok prefill\n")
            }
        } else {
            st.session <- create_session(m, sch.kv_dtype)
        }
        if (sch.queue[0].seed != 0) {
            set_seed(st.session, sch.queue[0].seed)
        }
        let wait_ms = sch.queue[0].t_submit != 0l ? int64(get_time_usec(sch.queue[0].t_submit)) / 1000l : 0l
        to_log(LOG_INFO, "[stream {st.id}] start: prompt={length(st.prompt)} tok (cached={st.n_cached}), queue_wait={wait_ms}ms, active={length(sch.streams) + 1}/{sch.max_streams}\n")
        sch.streams |> emplace(st)
        delete sch.queue[0]   // erase is a raw memmove — finalize what the move-out left behind
        sch.queue |> erase(0)
        sch.n_admitted ++
    }
    sch.peak_active = max(sch.peak_active, int64(length(sch.streams)))
    // (2) ONE batched decode step over every decoding stream (decode priority). Rows are gathered
    // fresh each step: admission above may have reallocated `streams`, and nothing below grows it.
    sch.batch_rows |> clear()
    sch.batch_toks |> clear()
    sch.batch_idx |> clear()
    for (i in range64(int64(length(sch.streams)))) {
        if (sch.streams[i].state != StreamState.decoding) {
            continue
        }
        if (sch.streams[i].session.n_past + 1l > m.config.seq_len) {   // pre-check: never reach the eval_batch panic
            finish_stream(sch.streams[i], "length", events)
            continue
        }
        if (sch.mtp && sch.streams[i].params.temp <= 0.0 && sch.streams[i].params.penalty == 1.0 &&
                sch.streams[i].params.presence_penalty == 0.0 && sch.streams[i].params.frequency_penalty == 0.0) {
            // greedy self-spec tick: draft + 2-row batched verify on this stream's own session
            // (the stream trades its cross-stream batch row for up to two positions per tick;
            // sampled/penalized streams stay on the batch — the greedy verify wouldn't match them).
            // Donation-side note: an accepted token is already evaled, so a "length" finish here
            // makes donate_stream's pending-eval redundant — harmless, and unreachable anyway:
            // every MTP-capable arch is a deltanet hybrid, which the prefix cache refuses.
            let d0 = sch.streams[i].session.mtp_drafted
            let a0 = sch.streams[i].session.mtp_accepted
            var acc = 0l
            if (mtp_spec_eval(m, sch.streams[i].session, sch.streams[i].cur_tok, acc)) {
                if (advance_with(m, sch.streams[i], acc, events)) {
                    sch.gen_tokens ++
                }
            }
            sch.mtp_drafted += sch.streams[i].session.mtp_drafted - d0
            sch.mtp_accepted += sch.streams[i].session.mtp_accepted - a0
            if (sch.streams[i].state == StreamState.decoding) {
                if (sample_advance(m, sch.streams[i], events)) {
                    sch.gen_tokens ++
                }
            }
            sch.decode_steps ++
            sch.sum_batch ++
            continue
        }
        sch.batch_rows |> push(unsafe(addr(sch.streams[i].session)))
        sch.batch_toks |> push(sch.streams[i].cur_tok)
        sch.batch_idx |> push(i)
    }
    if (!empty(sch.batch_rows)) {
        eval_batch(m, sch.ws, sch.batch_rows, sch.batch_toks)
        sch.decode_steps ++
        sch.sum_batch += int64(length(sch.batch_rows))
        for (bi in range64(int64(length(sch.batch_idx)))) {
            if (sample_advance(m, sch.streams[sch.batch_idx[bi]], events)) {
                sch.gen_tokens ++
            }
        }
    }
    // (3) at most ONE prefill chunk, FCFS — decode-priority with a bounded prefill quantum
    for (i in range64(int64(length(sch.streams)))) {
        if (sch.streams[i].state != StreamState.prefilling) {
            continue
        }
        let np = int64(length(sch.streams[i].prompt))
        let take = min(sch.chunk_tokens, np - sch.streams[i].n_prefilled)
        sch.chunk |> resize(int(take))
        for (k in range64(take)) {
            sch.chunk[k] = sch.streams[i].prompt[sch.streams[i].n_prefilled + k]
        }
        eval(m, sch.streams[i].session, sch.chunk)
        sch.streams[i].n_prefilled += take
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
            if (sch.paged) {
                donate_stream(m, sch, sch.streams[i])
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

// Donate a finished stream's KV pages, first evaling what the cache still lacks. A "length"
// finish leaves the last emitted token pending (sampled, never fed back); a "stop" finish already
// evaled every emitted token — the stop was sampled from the last one's eval — so only the close
// tokens are pending (re-evaling the last token would shift every row past the reply boundary
// against the page's token list). Either way the close tokens follow, so the NEXT turn of this
// conversation (prompt + reply + close + new user turn) extends the cached prefix. The tail is
// skipped when it cannot fit; evicted streams donate only what ran. prefix_insert trims to n_past.
def private donate_stream(m : Model; var sch : Scheduler; var st : Stream) {
    if (st.finish != "evicted") {
        sch.donate_toks |> clear()
        if (st.finish == "length" && st.cur_tok >= 0l) {
            sch.donate_toks |> push(st.cur_tok)
        }
        sch.donate_toks |> push_from(st.close_toks)
        if (!empty(sch.donate_toks)
                && st.session.n_past + int64(length(sch.donate_toks)) <= m.config.seq_len) {
            eval(m, st.session, sch.donate_toks)
        }
    }
    sch.donate_toks |> clear()
    sch.donate_toks |> reserve(int(st.n_prefilled) + length(st.session.recent) + length(st.close_toks))
    for (k in range64(st.n_prefilled)) {
        sch.donate_toks |> push(st.prompt[k])
    }
    sch.donate_toks |> push_from(st.session.recent, st.close_toks)
    prefix_insert(sch.prefix, sch.pool, st.session, sch.donate_toks)
}
