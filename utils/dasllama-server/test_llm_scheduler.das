options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // each test frees its model/scheduler before the next loads

require dastest/testing_boost public
require llm_scheduler                                   // the scheduler under test (sibling — bare require, hyphenated dir)
require dasllama/dasllama                               // facade: load/encode/generate references + engine knobs
require daslib/jobque_boost
require daslib/fio
require math
require dasllama/dasllama_env

// Unit gates for the continuous-batching scheduler — all through the SchedEvent seam, zero HTTP:
// scheduler ≡ generate bit-exact (1 stream), chunk-size invariance under classic prefill, staggered
// admit + queue overflow, stop/budget/context-length finishes, evict mid-run. Model-gated + JIT-only
// (SmolLM2-135M GGUF via DASLLAMA_MODELS_DIR), same skip contract as test_openai_server.

def private models_dir() : string {
    let env = g_env_harness.models_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}
let MODEL_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private model_missing(t : T?) : bool {
    if (!jit_enabled()) {
        t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
        return true
    }
    if (!stat(MODEL_PATH).is_valid) {
        t |> success(true, "skipped: {MODEL_PATH} not present")
        return true
    }
    return false
}

def private load_capped(cap : int64) : Model {
    var m <- load_model(MODEL_PATH, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, cap)
    return <- m
}

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

// One stream's reference: generate() with the wiring's stop protocol (stop token ends the stream
// unemitted; otherwise the budget runs out) — what the scheduler must reproduce.
struct private RefStream {
    toks : array<int64>
    pieces : array<string>
    finish : string
}

def private ref_generate(m : Model; prompt : array<int64>; params : SamplingParams;
                         max_new : int64; stop_ids : array<int64>; seed : int = 0) : RefStream {
    var s <- create_session(m)
    if (seed != 0) {
        set_seed(s, seed)
    }
    var r : RefStream
    var stops := stop_ids
    var hit_stop = false
    generate(m, s, prompt, params, max_new) $(id, pc) {
        for (sid in stops) {
            if (id == sid) {
                hit_stop = true
                return false
            }
        }
        r.toks |> push(id)
        r.pieces |> push(pc)
        return true
    }
    r.finish = hit_stop ? "stop" : "length"
    delete s
    return <- r
}

def private make_req(id : int64; prompt : array<int64>; params : SamplingParams; max_new : int64;
                     stop_ids : array<int64>; seed : int = 0) : PendingReq {
    var r <- PendingReq(id = id, params = params, max_new = max_new, seed = seed)
    r.prompt := prompt
    r.stop_ids := stop_ids
    return <- r
}

// step until idle (bounded — a runaway scheduler must not hang the suite)
def private drain(m : Model; var sch : Scheduler; var events : array<SchedEvent>) {
    var steps = 0
    while (scheduler_step(m, sch, events)) {
        steps ++
        if (steps > 4096) {
            panic("test_llm_scheduler: scheduler did not go idle in 4096 steps")
        }
    }
}

def private toks_of(events : array<SchedEvent>; id : int64) : array<int64> {
    return <- [for (e in events); e.token; where e.stream == id && e.kind == SchedEventKind.piece]
}

def private pieces_of(events : array<SchedEvent>; id : int64) : array<string> {
    return <- [for (e in events); e.text; where e.stream == id && e.kind == SchedEventKind.piece]
}

def private finished_of(events : array<SchedEvent>; id : int64) : SchedEvent {
    for (e in events) {
        if (e.stream == id && e.kind == SchedEventKind.finished) {
            return e
        }
    }
    return SchedEvent(stream = -1l)
}

// ===== scheduler ≡ generate (1 stream): greedy bit-exact, seeded sampling, stop finish =====

[test]
def test_scheduler_matches_generate(t : T?) {   // nolint:STYLE038 — one model load per test: every assertion runs against that model inside the same jobque scope
    t |> run("scheduler ≡ generate: bit-exact stream, seeded sampling, stop/budget (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(128l)
            var prompt <- encode(m, "Once upon a time", true, false)
            var none : array<int64>
            // greedy, single-chunk prefill: the eval sequence is identical to generate()'s, so the
            // emitted stream must be BIT-exact (B==1 eval_batch delegates to the same forward())
            let greedy = SamplingParams()
            var ref <- ref_generate(m, prompt, greedy, 10l, none)
            var sch <- create_scheduler(m, 1l, 4l, 512l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, greedy, 10l, none)), "greedy stream queued")
            drain(m, sch, events)
            var got <- toks_of(events, 1l)
            var gotp <- pieces_of(events, 1l)
            t |> success(same(got, ref.toks), "greedy tokens ≡ generate() bit-exact")
            t |> equal(length(gotp), length(ref.pieces))
            for (i in range(length(gotp))) {
                t |> equal(gotp[i], ref.pieces[i])
            }
            let fin = finished_of(events, 1l)
            t |> equal(fin.finish, "length")
            t |> equal(fin.n_gen, 10l)
            t |> equal(fin.n_prompt, int64(length(prompt)))
            t |> success(fin.ttft_us > 0l, "TTFT recorded")
            t |> equal(sch.gen_tokens, 10l)
            t |> success(sch.decode_steps > 0l, "decode steps counted")
            // seeded sampling (temp + top-k): the scheduler makes the same sample() calls in the
            // same order, so a seeded run reproduces generate() exactly
            let sp = SamplingParams(temp = 0.8, top_k = 40l)
            var ref2 <- ref_generate(m, prompt, sp, 10l, none, 4242)
            events |> clear()
            t |> success(submit(m, sch, make_req(2l, prompt, sp, 10l, none, 4242)), "seeded stream queued")
            drain(m, sch, events)
            var got2 <- toks_of(events, 2l)
            t |> success(same(got2, ref2.toks), "seeded sampled tokens ≡ generate()")
            // stop finish: pick the first greedy token that did not occur earlier in the stream,
            // declare it a stop id — the scheduler must end there, stop token unemitted
            var kstop = -1
            for (k in range(1, length(ref.toks))) {
                var seen = false
                for (j in range(k)) {
                    seen ||= ref.toks[j] == ref.toks[k]
                }
                if (!seen) {
                    kstop = k
                    break
                }
            }
            if (kstop > 0) {
                var stops <- [ref.toks[kstop]]
                var ref3 <- ref_generate(m, prompt, greedy, 10l, stops)
                events |> clear()
                t |> success(submit(m, sch, make_req(3l, prompt, greedy, 10l, stops)), "stop stream queued")
                drain(m, sch, events)
                var got3 <- toks_of(events, 3l)
                t |> equal(length(got3), kstop)
                t |> success(same(got3, ref3.toks), "stop-bounded tokens ≡ generate()")
                let fin3 = finished_of(events, 3l)
                t |> equal(fin3.finish, "stop")
                t |> equal(fin3.n_gen, int64(kstop))
                delete got3
                delete ref3
                delete stops
            } else {
                t |> success(true, "no unique stop candidate in 10 greedy tokens — stop arm skipped")
            }
            delete got
            delete gotp
            delete got2
            delete ref
            delete ref2
            delete events
            unsafe {
                delete sch
            }
            delete prompt
            delete m
        }
    }
}

// ===== chunked prefill: {1,3,17,64} produce the identical stream under classic prefill =====

[test]
def test_scheduler_chunk_sizes(t : T?) {
    t |> run("prefill chunk sizes 1/3/17/64 give the same stream — classic mode (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(128l)
            // classic prefill is bit-exact under ANY chunking (test_prefill's contract) — the same
            // pin as test_chat's render-seam case, so every chunk size must yield ONE stream
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery and counts every bird that crosses the evening sky", true, false)
            t |> success(int64(length(prompt)) > 17l, "prompt spans multiple chunks ({length(prompt)} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var first <- [-1l, -1l, -1l, -1l]
            var streams : array<array<int64>>
            for (ci, K in range(4), [1l, 3l, 17l, 64l]) {
                var sch <- create_scheduler(m, 1l, 4l, K)
                var events : array<SchedEvent>
                t |> success(submit(m, sch, make_req(int64(ci), prompt, greedy, 6l, none)), "stream queued (K={K})")
                drain(m, sch, events)
                var got <- toks_of(events, int64(ci))
                first[ci] = length(got) > 0 ? got[0] : -1l
                streams |> emplace(got)
                delete events
                unsafe {
                    delete sch
                }
            }
            t |> success(first[0] >= 0l, "chunked runs produced tokens")
            for (ci in range(1, 4)) {
                t |> equal(first[ci], first[0])
                t |> success(same(streams[ci], streams[0]), "K arm {ci}: full stream identical to K=1")
            }
            set_prefill_attn_mode(saved_mode)
            delete first
            delete streams
            delete prompt
            delete m
        }
    }
}

// ===== staggered admit, queue overflow, queued eviction, multi-stream token match =====

[test]
def test_scheduler_batching(t : T?) {
    t |> run("staggered admit + queue overflow: every stream matches its solo reference (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(128l)
            // the batch worker mirrors the std decode path — pin the fused chain off so the solo
            // generate() references share its numerics (test_batch_decode's contract)
            let prev_fused = get_fused_decode()
            set_fused_decode(false)
            let greedy = SamplingParams()
            var none : array<int64>
            var pa <- encode(m, "Once upon a time", true, false)
            var pb <- encode(m, "The little robot", true, false)
            var pc <- encode(m, "In a small village", true, false)
            var refs <- [ <- ref_generate(m, pa, greedy, 6l, none), <- ref_generate(m, pb, greedy, 6l, none),
                         <- ref_generate(m, pc, greedy, 6l, none)]
            var sch <- create_scheduler(m, 2l, 2l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(10l, pa, greedy, 6l, none)), "stream A queued")
            t |> success(submit(m, sch, make_req(11l, pb, greedy, 6l, none)), "stream B queued")
            // 3 ticks: A and B admit (cap 2) and start decoding; C arrives mid-run — staggered
            for (_i in range(3)) {
                scheduler_step(m, sch, events)
            }
            t |> equal(n_active(sch), 2l)
            t |> success(submit(m, sch, make_req(12l, pc, greedy, 6l, none)), "stream C queued mid-run")
            t |> success(submit(m, sch, make_req(13l, pa, greedy, 6l, none)), "stream D queued (fills the queue)")
            t |> success(!submit(m, sch, make_req(14l, pb, greedy, 6l, none)), "queue full: submit rejected")
            t |> success(evict(sch, 13l, events), "queued stream D evicted")
            t |> equal(finished_of(events, 13l).finish, "evicted")
            t |> equal(n_queued(sch), 1l)
            drain(m, sch, events)
            var ga <- toks_of(events, 10l)
            var gb <- toks_of(events, 11l)
            var gc <- toks_of(events, 12l)
            t |> success(same(ga, refs[0].toks), "stream A (batched) matches its solo reference")
            t |> success(same(gb, refs[1].toks), "stream B (batched) matches its solo reference")
            t |> success(same(gc, refs[2].toks), "stream C (admitted after a slot freed) matches its solo reference")
            t |> equal(finished_of(events, 12l).finish, "length")
            t |> equal(length(toks_of(events, 13l)), 0)
            t |> equal(sch.peak_active, 2l)
            t |> equal(sch.n_evicted, 1l)
            t |> equal(sch.n_finished, 3l)
            t |> success(sch.sum_batch >= 2l * 2l, "decode steps actually batched (Σ batch {sch.sum_batch})")
            set_fused_decode(prev_fused)
            delete ga
            delete gb
            delete gc
            delete events
            unsafe {
                delete sch
            }
            delete refs
            delete pa
            delete pb
            delete pc
            delete m
        }
    }
}

// ===== paged serving + prefix cache: a repeat request attaches pages and skips their prefill =====

[test]
def test_scheduler_prefix_cache(t : T?) {   // nolint:STYLE038 — one model load per test: every assertion runs against that model inside the same jobque scope
    t |> run("paged scheduler: repeat request attaches cached pages, identical stream (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(128l)
            // classic prefill: the warm run re-chunks the prompt (tail-only), and only classic is
            // bit-exact under re-chunking — the same pin as the chunk-size arm above
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var prompt <- encode(m, "Once upon a time in a small village near the mountains there lived a very curious little robot who loved counting stars", true, false)
            let np = int64(length(prompt))
            t |> success(np > 17l, "prompt spans multiple pages at P=16 ({np} tokens)")
            var none : array<int64>
            let greedy = SamplingParams()
            var sch <- create_scheduler(m, 1l, 4l, 64l, 16l)   // paged, P=16, unbounded prefix cache
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(1l, prompt, greedy, 8l, none)), "cold stream queued")
            drain(m, sch, events)
            var cold <- toks_of(events, 1l)
            t |> equal(finished_of(events, 1l).n_cached, 0l)
            t |> success(prefix_held_groups(sch.prefix) > 0l, "the finished stream donated its pages")
            t |> equal(sch.cached_tokens, 0l)
            events |> clear()
            t |> success(submit(m, sch, make_req(2l, prompt, greedy, 8l, none)), "warm stream queued")
            drain(m, sch, events)
            var warm <- toks_of(events, 2l)
            let fin2 = finished_of(events, 2l)
            t |> equal(fin2.n_cached, (np - 1l) / 16l * 16l)   // whole pages, capped below np
            t |> success(same(warm, cold), "warm stream identical to cold")
            t |> equal(sch.cached_tokens, fin2.n_cached)
            // idle scheduler: the only live page holds are the cache's
            t |> equal(int64(length(sch.pool.free_groups)),
                       sch.pool.capacity_groups - prefix_held_groups(sch.prefix))

            // ===== stop-finished streams donate an ALIGNED history: continuation attach == cold.
            // A "stop" finish must donate rows matching prompt + emitted + close exactly — the stop
            // was sampled from the last emitted token's eval, so only the CLOSE tokens are pending.
            // Re-evaling the last token would shift every row past the reply boundary against the
            // page's token list, poisoning multi-turn continuation attaches.
            var prompt3 <- encode(m, "The little robot climbed the old bakery roof to count the evening birds", true, false)
            var ref3 <- ref_generate(m, prompt3, greedy, 8l, none)
            var kstop = -1
            for (k in range(1, length(ref3.toks))) {   // first token unique so far — a safe stop id
                var seen = false
                for (j in range(k)) {
                    seen ||= ref3.toks[j] == ref3.toks[k]
                }
                if (!seen) {
                    kstop = k
                    break
                }
            }
            if (kstop >= 1) {
                var stops <- [ref3.toks[kstop]]
                var close <- [100l, 200l]   // any valid ids: the donated history must include them
                var sch2 <- create_scheduler(m, 1l, 4l, 64l, 16l)
                events |> clear()
                var req <- make_req(50l, prompt3, greedy, 8l, stops)
                req.close_toks := close
                t |> success(submit(m, sch2, req), "stop stream queued")
                drain(m, sch2, events)
                t |> equal(finished_of(events, 50l).finish, "stop")
                var emitted <- toks_of(events, 50l)
                t |> equal(length(emitted), kstop)
                // continuation: the next turn's prompt replays prompt + reply + close, then diverges
                var prompt4 : array<int64>
                prompt4 |> push_from(prompt3, emitted, close)
                for (i in range64(16l)) {
                    prompt4 |> push(500l + i)
                }
                events |> clear()
                t |> success(submit(m, sch2, make_req(51l, prompt4, greedy, 6l, none)), "warm continuation queued")
                drain(m, sch2, events)
                var warm4 <- toks_of(events, 51l)
                let fin4 = finished_of(events, 51l)
                t |> success(fin4.n_cached > int64(length(prompt3)),
                    "continuation attached past the reply boundary (cached {fin4.n_cached})")
                // cold reference: the same continuation with no cache
                var sch3 <- create_scheduler(m, 1l, 4l, 64l, 16l)
                events |> clear()
                t |> success(submit(m, sch3, make_req(52l, prompt4, greedy, 6l, none)), "cold continuation queued")
                drain(m, sch3, events)
                var cold4 <- toks_of(events, 52l)
                t |> success(same(warm4, cold4), "continuation attach bit-exact vs cold — donated rows aligned")
                delete cold4
                unsafe {
                    delete sch3
                }
                delete warm4
                delete prompt4
                delete emitted
                unsafe {
                    delete sch2
                }
                delete close
                delete stops
            } else {
                t |> success(true, "no unique stop candidate in 8 greedy tokens — continuation arm skipped")
            }
            delete ref3
            delete prompt3
            set_prefill_attn_mode(saved_mode)
            delete cold
            delete warm
            delete events
            unsafe {
                delete sch
            }
            delete prompt
            delete m
        }
    }
}

// ===== active eviction mid-run + the context-exhaustion length pre-check =====

[test]
def test_scheduler_evict_and_context_length(t : T?) {
    t |> run("evict mid-run leaves the survivor exact; context exhaustion finishes 'length' (needs SmolLM2-135M)") @(t : T?) {
        if (model_missing(t)) {
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(64l)
            let prev_fused = get_fused_decode()
            set_fused_decode(false)
            let greedy = SamplingParams()
            var none : array<int64>
            var pa <- encode(m, "Once upon a time", true, false)
            var pb <- encode(m, "The little robot", true, false)
            var ref_a <- ref_generate(m, pa, greedy, 8l, none)
            var sch <- create_scheduler(m, 2l, 4l, 64l)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(20l, pa, greedy, 8l, none)), "survivor queued")
            t |> success(submit(m, sch, make_req(21l, pb, greedy, 8l, none)), "victim queued")
            for (_i in range(4)) {   // both prefilled + a few decode steps at B=2
                scheduler_step(m, sch, events)
            }
            let b_before = length(toks_of(events, 21l))
            t |> success(b_before > 0, "victim generated before eviction")
            t |> success(evict(sch, 21l, events), "victim evicted mid-run")
            t |> equal(finished_of(events, 21l).finish, "evicted")
            drain(m, sch, events)
            t |> equal(length(toks_of(events, 21l)), b_before)
            var ga <- toks_of(events, 20l)
            t |> success(same(ga, ref_a.toks), "survivor stream unaffected by the eviction")
            t |> equal(finished_of(events, 20l).finish, "length")
            // context exhaustion: budget far beyond the capped seq_len — the decode pre-check must
            // finish "length" (n_gen = seq_len - n_prompt + 1) instead of reaching the engine panic
            events |> clear()
            t |> success(submit(m, sch, make_req(22l, pa, greedy, 512l, none)), "exhaustion stream queued")
            drain(m, sch, events)
            let fin = finished_of(events, 22l)
            t |> equal(fin.finish, "length")
            t |> equal(fin.n_gen, m.config.seq_len - int64(length(pa)) + 1l)
            set_fused_decode(prev_fused)
            delete ga
            delete events
            unsafe {
                delete sch
            }
            delete ref_a
            delete pa
            delete pb
            delete m
        }
    }
}

// ===== scheduler mtp: greedy self-spec ticks are output-invariant vs the plain scheduler =====

let MTP_MODEL_PATH = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")

[test]
def test_scheduler_mtp(t : T?) {
    t |> run("scheduler mtp: self-spec greedy stream ≡ plain scheduler, chunked prefill + paged (needs Qwen3.5-0.8B-MTP)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        if (!stat(MTP_MODEL_PATH).is_valid) {
            t |> success(true, "skipped: {MTP_MODEL_PATH} not present")
            return
        }
        // pin the bit-exact kernels (same reasoning as test_mtp): the spec arm's verify rides the
        // BATCH kernels while the plain arm decodes per-token — near-tie flips are not bugs, so the
        // token-exact gate must not ride them. Both arms run paged/f16 so only spec mechanics differ.
        let prev_attn = get_prefill_attn_mode()
        set_prefill_attn_mode(AttnPrefillMode.classic)
        let prev_act = get_act_vec()
        set_act_vec(false)
        let prev_dn = get_deltanet_prefill_mode()
        set_deltanet_prefill_mode(DnPrefillMode.recurrent)
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_model(MTP_MODEL_PATH, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, 512l)
            var prompt <- encode(m, "Count from one to twenty: one, two, three,", true, false)
            var none : array<int64>
            let greedy = SamplingParams()
            // reference: the PLAIN scheduler (mtp off), paged, 8-token prefill quanta
            var sch0 <- create_scheduler(m, 1l, 4l, 8l, 16l)
            var events0 : array<SchedEvent>
            t |> success(submit(m, sch0, make_req(1l, prompt, greedy, 24l, none)), "plain stream queued")
            drain(m, sch0, events0)
            var ref_toks <- toks_of(events0, 1l)
            unsafe {
                delete sch0
            }
            // spec: mtp on — the chunked prefill crosses the warm's seam rows, and the paged
            // sessions ride the pool's draft-block slab (the server default rail)
            let prev_spec = get_mtp_spec()
            set_mtp_spec(true)
            var sch <- create_scheduler(m, 1l, 4l, 8l, 16l, 0l, KVDtype.f16, true)
            var events : array<SchedEvent>
            t |> success(submit(m, sch, make_req(2l, prompt, greedy, 24l, none)), "spec stream queued")
            drain(m, sch, events)
            set_mtp_spec(prev_spec)
            var got <- toks_of(events, 2l)
            t |> success(same(got, ref_toks), "spec stream tokens ≡ plain scheduler")
            t |> success(sch.mtp_drafted > 0l, "the spec tick actually drafted ({sch.mtp_drafted} drafts)")
            t |> success(sch.mtp_accepted > 0l, "some drafts accepted ({sch.mtp_accepted} of {sch.mtp_drafted})")
            t |> equal(finished_of(events, 2l).n_gen, 24l)
            delete got
            delete events
            unsafe {
                delete sch
            }
            delete ref_toks
            delete events0
            delete prompt
            delete m
        }
        set_prefill_attn_mode(prev_attn)
        set_act_vec(prev_act)
        set_deltanet_prefill_mode(prev_dn)
    }
}
